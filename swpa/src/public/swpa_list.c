#include "swpa.h"
#include "swpa_list.h"

typedef struct tagLISTNODE
{
	void* value;                //�ڵ��ֵ
	struct tagLISTNODE *prev;   //��һ���ڵ�
	struct tagLISTNODE *next;   //��һ���ڵ�
}LISTNODE;

typedef struct tagLIST
{
	int       mutex;  //������
	int       sem;    //�ź���
	int       lock;   //�Ƿ����ı�־
	LISTNODE *head;	  //ͷ���
	LISTNODE *tail;   //β�ڵ�
	LISTNODE *now;    //��ǰ�ڵ�
	int       fdel;   //�Ƿ�ɾ��
}LIST;

/**
 *@brief ��������
 *@return �ɹ�������������ʧ�ܷ���-1
 */
int swpa_list_create(int max_size)
{
	LIST *lst = (LIST *)swpa_mem_alloc(sizeof(LIST));
	if(0 != lst)
	{
		swpa_memset(lst, 0, sizeof(LIST));
		swpa_mutex_create(&lst->mutex, 0);
		swpa_sem_create(&lst->sem, 0, max_size);
		return (int)lst;
	}
	return -1;
}

/**
 *@brief �ͷ�������Դ
 *@param [in] handle ������
 *@return �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_list_destroy(int handle)
{
	LIST *lst = (LIST *)handle;
	if(lst)
	{
		swpa_mutex_lock(&lst->mutex, -1);
		lst->lock++;
		LISTNODE *node = lst->head;
		while(node)
		{
			LISTNODE *tmp = node->next;
			node = node->next;
			swpa_mem_free(tmp);
		}
		lst->lock--;
		swpa_mutex_unlock(&lst->mutex);
		swpa_mutex_delete(&lst->mutex);
		swpa_mem_free(lst);
	}
	return 0;
}

/**
 *@brief ���������һ���ڵ�
 *@param [in] handle ������
 *@param [in] value �ڵ��ֵ
 *@return �ɹ������²���ڵ��λ�ã�ʧ�ܷ���-1
 */
int swpa_list_add(int handle, void *value)
{
	LIST *lst = (LIST *)handle;
	LISTNODE *now = 0;
	if(lst)
	{
		swpa_mutex_lock(&lst->mutex, -1);
		lst->lock++;
		if(0 == swpa_sem_post(&lst->sem))
		{
			now = (LISTNODE *)swpa_mem_alloc(sizeof(LISTNODE));
			if(now)
			{
				now->value = value;
				now->prev = 0;
				now->next = 0;
				
				if(1 == swpa_sem_value(&lst->sem))
				{
					lst->head = lst->tail = now;
				}
				else
				{
					now->prev = lst->tail;
					lst->tail->next = now;
					lst->tail = now;
				}	
			}
		}
		lst->lock--;
		swpa_mutex_unlock(&lst->mutex);		
	}
	return 0 == now ? -1 : (int)now;
}

/**
 *@brief ���������һ���ڵ�
 *@param [in] handle ������
 *@param [in] pos �ڵ�λ��
 *@param [in] value �ڵ��ֵ
 *@return �ɹ����ؽڵ�λ�ã�ʧ�ܷ���-1
 */
int swpa_list_insert(int handle, int pos, void *value)
{
	LIST *lst = (LIST *)handle;
	LISTNODE *now = 0;
	if(lst)
	{
		swpa_mutex_lock(&lst->mutex, -1);
		lst->lock++;
		if(0 == swpa_sem_post(&lst->sem))
		{
			LISTNODE *ln = (LISTNODE *)pos;
			if(ln)
			{
				now = (LISTNODE *)swpa_mem_alloc(sizeof(LISTNODE));
				now->value = value;
				now->prev = ln;
				now->next = ln->next;
				ln->next = now;
			}
		}
		lst->lock--;
		swpa_mutex_unlock(&lst->mutex);
	}
	return 0 == now ? -1 : (int)now;
}

/**
 *@brief ɾ�������е�һ���ڵ�
 *@param [in] handle ������     
 *@param [in] pos �ڵ�λ��
 *@param [in] �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_list_delete(int handle, int pos)
{
	LIST *lst = (LIST *)handle;
	LISTNODE *now = 0;
	if(lst && 0 < swpa_sem_value(&lst->sem) && 0 == swpa_sem_pend(&lst->sem, -1))
	{
		swpa_mutex_lock(&lst->mutex, -1);
		lst->lock++;
		now = (LISTNODE *)pos;
		if(now)
		{
			if(lst->now == now)
			{
				lst->fdel = 1;
				lst->now = lst->now->next;
			}
			if(now->prev)
			{
				now->prev->next = now->next;
			}
			if(now->next)
			{
				now->next->prev = now->prev;
			}
			if(now == lst->tail)
			{
				lst->tail = lst->tail->prev;
			}
			if(now == lst->head)
			{
				lst->head = lst->head->next;
			}
			swpa_mem_free(now);			
		}
		lst->lock--;
		swpa_mutex_unlock(&lst->mutex);
	}
	return 0 == now ? -1 : 0;
}

/**
 *@brief �������ͷ���ɾ��һ���ڵ�
 *@param [in] handle ������
 *@param [in] from_head Ϊ0����ɾ��ͷ�ڵ㣬Ϊ1����ɾ��β�ڵ�
 *@param [in] timeout -1��ʾ����������0��ʾ��������������0��ʾ������ʱʱ�ޡ�
 *@retval ���ؽڵ��ֵ
 */
void* swpa_list_remove(int handle, int from_head, int timeout)
{
	LIST *lst = (LIST *)handle;
	void* value = 0;
	if(lst)
	{
		if(0 == swpa_sem_pend(&lst->sem, timeout))
		{
			swpa_mutex_lock(&lst->mutex, -1);
			LISTNODE *now = 0;
			if(0 == from_head)
			{
				now = lst->head;
				lst->head = lst->head->next;
				if(now == lst->tail)
				{
					lst->tail = lst->tail->next;
				}
			}
			else
			{
				now = lst->tail;
				lst->tail = lst->tail->prev;
				if(now == lst->head)
				{
					lst->head = lst->head->prev;
				}
			}
			if(lst->now == now)
			{
				lst->fdel = 1;
				lst->now = lst->now->next;
			}
			value = now->value;
			swpa_mem_free(now);			
			swpa_mutex_unlock(&lst->mutex);
		}
	}
	return value;
}

/**
 *@brief ���ݽڵ�λ�û�ýڵ��ֵ
 *@param [in] handle ������
 *@param [in] pos �ڵ�λ��
 *@return �ɹ����ؽڵ�ֵ��ʧ�ܷ���0
 */
void* swpa_list_value(int handle, int pos)
{
	LIST *lst = (LIST *)handle;
	if(lst && 0 < pos)
	{
		swpa_mutex_lock(&lst->mutex, -1);
		lst->lock++;
		LISTNODE *now = (LISTNODE *)pos;
		void *value = now->value;
		lst->lock--;
		swpa_mutex_unlock(&lst->mutex);
		return value;
	}
	return 0;
}

/**
 *@brief ����Ĵ�С
 *@param [in] handle ������
 *@return ��������ڵ�ĸ���
 */
int swpa_list_size(int handle)
{
	int size = 0;
	LIST *lst = (LIST *)handle;
	if(lst)
	{
		size = swpa_sem_value(&lst->sem);
	}
	return size;
}

/**
 *@brief ������������֮ǰ������open
 *@param [in] handle ������
 *@return ��������ͷ���
 */
int swpa_list_open(int handle, int from_head)
{
	LIST *lst = (LIST *)handle;
	if(lst && swpa_list_size(handle) > 0)
	{
		swpa_mutex_lock(&lst->mutex, -1);
		lst->lock++;
		lst->now = (0 == from_head) ? lst->head : lst->tail;
		lst->fdel = 0;
		return lst->now ? (int)lst->now : -1;
	}
	return -1;
}

/**
 *@brief ����ڵ�����һ���ڵ�
 *@brief handle ������
 *@return ���ص�ǰ�ڵ�λ��
 */
int swpa_list_next(int handle)
{
	LIST *lst = (LIST *)handle;
	if(lst && lst->now)
	{
		if(lst->fdel)
		{
			lst->fdel = 0;
		}
		else
		{
			lst->now = lst->now->next;
		}
	}
	return lst->now ? (int)lst->now : -1;
}

/**
 *@brief ����ڵ�����һ���ڵ�
 *@brief handle ������
 *@return ���ص�ǰ�ڵ�λ��
 */
int swpa_list_prev(int handle)
{
	LIST *lst = (LIST *)handle;
	if(lst && lst->now)
	{
		lst->fdel = 0;
		lst->now = lst->now->prev;
	}
	return lst->now ? (int)lst->now : -1;
}

/**
 *@brief ֹͣ����
 *@brief handle ������
 *@return �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_list_close(int handle)
{
	LIST *lst = (LIST *)handle;
	if(lst && lst->lock > 0)
	{
		lst->lock--;
		swpa_mutex_unlock(&lst->mutex);
		return 0;
	}
	return -1;
}

