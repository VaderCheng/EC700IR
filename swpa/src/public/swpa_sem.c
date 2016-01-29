#include "swpa.h"
#include "swpa_private.h"
#include "swpa_sem.h"
#include "swpa_datetime.h"

#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#ifdef SWPA_SEM
#define SWPA_SEM_PRINT(fmt, ...) SWPA_PRINT("[%s:%d]"fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define SWPA_SEM_CHECK(arg)      {if (!(arg)){SWPA_PRINT("[%s:%d]Check failed : %s [%d]\n", __FILE__, __LINE__, #arg, SWPAR_INVALIDARG);return SWPAR_INVALIDARG;}}
#else
#define SWPA_SEM_PRINT(fmt, ...)
#define SWPA_SEM_CHECK(arg)
#endif

typedef struct tag_SEM
{
	sem_t sem;
	int max; 
	int id; /* BEGIN: add by huangdch, 2015/9/4:���̼��ź���Ψһ��ʶ*/
}
SEM_HANDLE;

/* BEGIN: Modified by huangdch, 2015/9/4 
*���̼���ź�������100Ҫ���ݾ����Ӧ�����������޸����ֵ*/
#define SHM_PROCESS_SEM_MAX_NUM 	100   /* ���̼��ź����������� */
#define SHM_PROCESS_SEM_KEY_ID      10000 /* �ź�����Ӧ�Ĺ����ڴ�����key */
static void* g_shm_sem_msg_addr = NULL;   /* ͬһ�������ڹ����׵�ַ */
static int g_shm_sem_number = 0;          /* ͬһ�������ڵ������� */

/* BEGIN: Modified by huangdch, 2015/9/4 
*�ź����������׵�ַ*/
typedef struct _shm_msg_sem
{
	pthread_mutex_t mutex; /* �� */
	short total_attach; /* �ܵ�������,Ŀǰ�����ֶ� */
	short attach[SHM_PROCESS_SEM_MAX_NUM]; /* ÿ���ڵ�������� */
}sw_shm_msg_sem;

/*****************************************************************************
 �� �� ��  : sem_mem_alloc
 ��������  : ����id�ӹ����ڴ�����һ���ڴ�ڵ�,����ɹ��������ڴ���׵�ַ
 �������  : const int id �ź�����ʶid   
             const int size δ��
             int* state ����     
 �������  : ��
 �� �� ֵ  : NULLʧ�� �ǿճɹ�
 ���ú���  : 
 ע������  : ֻ�ڵ�ǰ�ļ���ʹ��
 
 �޸���ʷ      :
  1.��    ��   : 2015��9��4��
    ��    ��   : huangdch
    �޸�����   : �����ɺ���

*****************************************************************************/
void* sem_mem_alloc(const int id, const int size, int* state)
{
	if (id < 1 || size < 1)
	{
		return -1;
	}

	SWPA_SEM_PRINT("alloc sem id=%d\n", id);
	SWPA_SEM_PRINT("alloc sem size=%d\n", size);
	SWPA_SEM_PRINT("alloc sem state=%d\n", *state);
	int ret = 0;
	int shm_id = 0;
	int temp_size = sizeof(sw_shm_msg_sem) + SHM_PROCESS_SEM_MAX_NUM * sizeof(SEM_HANDLE);
	int key_id = SHM_PROCESS_SEM_KEY_ID;
	int temp_state = 0;
	int process_flag = 0;
	void* radd = NULL;

	if (NULL == g_shm_sem_msg_addr)
	{
		/* ���ӹ����� */
		int temp_state = 0;
		g_shm_sem_msg_addr = swpa_normal_sharedmem_alloc(key_id, temp_size, &temp_state);
		if (NULL == g_shm_sem_msg_addr)
		{
			SWPA_SEM_PRINT("create key_id=[%d] size=[%d] failed,ret=[%d].\n", key_id, temp_size, ret);
			return NULL;
		}

		/* ��ʼ���� */
		if (0 == temp_state)
		{
			sw_shm_msg_sem* msg_node = (sw_shm_msg_sem*)g_shm_sem_msg_addr;

			pthread_mutexattr_t attr;
			if (0 != pthread_mutexattr_init(&attr))
			{
				return NULL;
			}
			if (0 != pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED))
			{
				return NULL;
			}	
			if (0 != pthread_mutex_init(&msg_node->mutex, &attr))
			{
				return NULL;
			}
			if (0 != pthread_mutexattr_destroy(&attr))
			{
				return NULL;
			}

			msg_node->total_attach = 0;
			memset(&msg_node->attach, 0, sizeof(msg_node->attach));
			memset(msg_node + 1, 0, temp_size - sizeof(sw_shm_msg_sem));
		}

		process_flag = 1;
	}

	sw_shm_msg_sem* msg_node = (sw_shm_msg_sem*)g_shm_sem_msg_addr;

	pthread_mutex_lock(&msg_node->mutex);

	/* ����id���Ѿ�����Ĺ����ڴ�����һ���ѷ����δռ�õĽڵ� */
	SEM_HANDLE* shm_node = (SEM_HANDLE*)(msg_node + 1);
	int k = 0;
	int flag = -1;
	int node = -1;
	for (k = 0 ; k < SHM_PROCESS_SEM_MAX_NUM ; k++)
	{
		if (id == shm_node[k].id) /* ���Ѿ�����Ľڵ� */
		{
			flag = 2;
			msg_node->attach[k]++;/* ���ӵĽ����� */
			msg_node->total_attach++;
			radd = (void*)&shm_node[k]; /* ����һ����ַ */
			*state = 1;
			node = k;
			break;
		}
	}
	if (-1 == flag)
	{
		for (k = 0 ; k < SHM_PROCESS_SEM_MAX_NUM ; k++)
		{
			if (0 == shm_node[k].id) /* ��δ����Ľڵ� */
			{
				msg_node->attach[k]++;/* ���ӵĽ����� */
				msg_node->total_attach++;
				shm_node[k].id = id;
				flag = 1;
				radd = (void*)&shm_node[k]; /* ����һ����ַ */
				*state = 0;
				node = k;
				break;
			}
		}
	}

	SWPA_SEM_PRINT("sem_id=[%d] flag=[%d] node=[%d]\n", id, flag, node);

	/* û���ҵ����������Ľڵ���з��� */
	if (-1 == flag && 1 == process_flag)
	{
		pthread_mutex_unlock(&msg_node->mutex);
		
		ret = swpa_normal_sharedmem_free(SHM_PROCESS_SEM_KEY_ID, g_shm_sem_msg_addr);
		if (ret < 0)
		{
			SWPA_SEM_PRINT("free:sem_id=[%d] failed,rang out of [%d] ret=[%d].\n", SHM_PROCESS_SEM_KEY_ID, SHM_PROCESS_SEM_MAX_NUM, ret);
		}
		else
		{
			SWPA_SEM_PRINT("free:sem_id=[%d] success,ret=[%d].\n", SHM_PROCESS_SEM_KEY_ID, ret);
		}
		
		SWPA_SEM_PRINT("free:sem_id=[%d] failed,rang out of [%d] ret=[%d].\n", SHM_PROCESS_SEM_KEY_ID, SHM_PROCESS_SEM_MAX_NUM, ret);

		return NULL;
	}

	if (NULL != radd)
	{
		g_shm_sem_number++; /* ͬһ�������ڵ�������+1 */
	}

	pthread_mutex_unlock(&msg_node->mutex);

	return radd;
}

/*****************************************************************************
 �� �� ��  : sem_mem_free
 ��������  : �����ź���id���Ҷ�Ӧ���ڴ�鲢�ͷ���Ӧ�Ľڵ�
 �������  : const int id  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ע������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��9��4��
    ��    ��   : huangdch
    �޸�����   : �����ɺ���

*****************************************************************************/
int sem_mem_free(const int id)
{
	if (id < 1 || NULL == g_shm_sem_msg_addr)
	{
		return -1;
	}

	int ret = 0;
	int shm_id = 0;

	sw_shm_msg_sem* msg_node = (sw_shm_msg_sem*)g_shm_sem_msg_addr;

	pthread_mutex_lock(&msg_node->mutex);

	SEM_HANDLE* shm_node = (SEM_HANDLE*)(msg_node + 1);

	int flag = -1;
	int k = 0;
	for (k = 0 ; k < SHM_PROCESS_SEM_MAX_NUM ; k++)
	{
		if (id == shm_node[k].id)
		{
			flag = 1;
			g_shm_sem_number--;
			msg_node->attach[k]--;
			msg_node->total_attach--;
			if (0 == msg_node->attach[k])/* ����ź������ڴ�ڵ�������Ϊ0���ÿ��ٷ��� */
			{
				SWPA_SEM_PRINT("node=[%d] id=[%d] is set 0.\n", k, id);
				shm_node[k].id = 0;
			}
			break;
		}
	}

	pthread_mutex_unlock(&msg_node->mutex);

	if (g_shm_sem_number == 0)
	{
		SWPA_SEM_PRINT("call swpa_normal_sharedmem_free, id=[%d] num=[%d] total_attach=[%d]\n", id, g_shm_sem_number, msg_node->total_attach);

		ret = swpa_normal_sharedmem_free(SHM_PROCESS_SEM_KEY_ID, g_shm_sem_msg_addr);
		if (0 == ret)
		{
			g_shm_sem_msg_addr = NULL;
		}
	}
	
	return ret;
}

/**
* @brief �����ź���
* @param [out] sem �ź����ṹ��ָ��
* @param [in] init �ź�����ʼֵ
* @param [in] init �ź������ֵ
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_sem.h
* @note ���ź���Ϊ�̼߳���
*/
int swpa_sem_create(int* sem, unsigned int init, unsigned int max)
{
	SWPA_SEM_CHECK(sem != NULL);
	SWPA_SEM_CHECK(init <= max);
	SWPA_SEM_PRINT("sem=0x%08x\n", (unsigned int)sem);
	SWPA_SEM_PRINT("init=%d\n", init);
	SWPA_SEM_PRINT("max=%d\n", max);

	int iret = -1;
	SEM_HANDLE* handle = swpa_mem_alloc(sizeof(SEM_HANDLE));
	if( handle == NULL )
	{
		return SWPAR_OUTOFMEMORY;
	}
	iret = sem_init((sem_t*)(&handle->sem), 0, init);
	if(iret < 0)
	{
		swpa_mem_free(handle);
		handle = NULL;
		return SWPAR_FAIL;
	}
	handle->max = max;
	handle->id = 0;

	(*sem) = (int)handle;
	return SWPAR_OK;
}

/* BEGIN: Modified by huangdch, 2015/9/4 
*���̼��ź����Ĵ���*/
int swpa_processsem_create(int* sem, unsigned int init, unsigned int max, const int sem_id)
{
	if (sem < 1)
	{
		return -1;
	}
	SWPA_SEM_PRINT("process sem=0x%08x\n", (unsigned int)sem);
	SWPA_SEM_PRINT("init=%d\n", init);
	SWPA_SEM_PRINT("max=%d\n", max);
	SWPA_SEM_PRINT("process sem_id=%d\n", sem_id);

	int k = 0;
	int ret = -1;
	int size = 0;
	int id = sem;

	SEM_HANDLE* psem = NULL;
	size = sizeof(*psem);

	/* ���乲���ڴ� */
	int state = 0;
	psem = (SEM_HANDLE *)sem_mem_alloc(sem_id, size, &state);
	if (NULL == psem)
	{
		return -1;
	}

	if (0 == state)
	{
		/* ��ʼ���ź��� */
		ret = sem_init(&psem->sem, 3, init);/* 3��ʾ���̼�ʹ�� */
		if (ret < 0)
		{
			return -2;
		}
		psem->max = max;
		psem->id = sem_id;
	}

	(*sem) = (int)psem;

	return 0;
}


/**
* @brief �����ź���
* @param [in] sem �ź����ṹ��ָ��
* @param [in] timeout ������ʱ����λ������
* @retval 0 �ɹ������
* @retval -1 ʧ�ܻ򲻿���
* @see swpa_sem.h
* @note ��timeoutΪ-1ʱ����ʾ������������timeout����0ʱ����ʾ������������timeout����0ʱ������ʾ������ʱʱ�ޡ�
*/
int swpa_sem_pend(int* sem, int timeout)
{
	SWPA_SEM_CHECK(sem != NULL);
	SWPA_SEM_PRINT("sem=0x%08x\n", (unsigned int)sem);
	SWPA_SEM_PRINT("timeout=%d\n", timeout);
	int iret = -1;
	SEM_HANDLE* handle = (SEM_HANDLE*)(*sem);
	// ����ʱ��
	struct timespec ts;
	if(timeout == -1)
	{
		iret = sem_wait(&(handle->sem));
		if(iret < 0)
		{
			return SWPAR_FAIL;
		}
		return SWPAR_OK;
	}

	clock_gettime(CLOCK_REALTIME, &ts);    //��ȡ��ǰʱ��
	ts.tv_sec += (timeout / 1000);			//���ϵȴ�ʱ�������
	ts.tv_nsec += (timeout % 1000) * 1000000; //���ϵȴ�ʱ��������
	// ����������1��ʱ��ŵ�����
	ts.tv_sec += ts.tv_nsec / 1000000000;
	ts.tv_nsec = ts.tv_nsec % 1000000000;
	while(((iret = sem_timedwait(&(handle->sem), &ts)) != 0) && (errno == EINTR));
	if(iret < 0)
	{
		return SWPAR_FAIL;
	}

	return SWPAR_OK;
}

/**
* @brief �����ź���
* @param [in] sem �ź����ṹ��ָ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_sem.h
*/
int swpa_sem_post(int* sem)
{
	SWPA_SEM_CHECK(sem != NULL);
	SWPA_SEM_PRINT("sem=0x%08x\n", (unsigned int)sem);
	int nval = 0;
	int iret = -1;
	SEM_HANDLE* handle = (SEM_HANDLE*)(*sem);
	if(handle->max > 0)
	{
		iret = sem_getvalue(&(handle->sem), &nval);
		if(nval >= (int)handle->max)
		{
			return SWPAR_FAIL;
		}
	}
	iret = sem_post(&(handle->sem));
	if(iret < 0)
	{
		return SWPAR_FAIL;
	}
	return SWPAR_OK;
}

int swpa_sem_value(int* sem)
{
	SWPA_SEM_CHECK(sem != NULL);
	SWPA_SEM_PRINT("sem=0x%08x\n", (unsigned int)sem);
	SEM_HANDLE* handle = (SEM_HANDLE*)(*sem);
	int nval = 0;
	sem_getvalue(&(handle->sem), &nval);
	return nval;
}

/**
* @brief ɾ���ź���
* @param [in] sem �ź����ṹ��ָ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_sem.h
*/
int swpa_sem_delete(int* sem)
{
	SWPA_SEM_CHECK(sem != NULL);
	SWPA_SEM_PRINT("sem=0x%08x\n", (unsigned int)sem);
	int iret = -1;
	SEM_HANDLE* handle = (SEM_HANDLE*)(*sem);
	iret = sem_destroy(&(handle->sem));
	if (handle->id > 0)
	{
		sem_mem_free(handle->id);
	}
	else
	{
		swpa_mem_free(handle);
		handle = NULL;
	}

	if (iret < 0)
	{
		return SWPAR_FAIL;
	}
	return SWPAR_OK;
}


typedef struct tag_Mutex
{
	pthread_mutex_t mutex;
	int id;  /* BEGIN: add by huangdch, 2015/9/4:���̼���Ψһ��ʶ*/
}
MUTEX_HANDLE;

/* BEGIN: Modified by huangdch, 2015/9/4 
*���̼����������10Ҫ���ݾ����Ӧ��������е���*/
#define SHM_PROCESS_MUTEX_MAX_NUM   10   /* �������������� */
#define SHM_PROCESS_MUTEX_KEY_ID   10001 /* ��������Ӧ�Ĺ����ڴ�����key */
static void* g_shm_mutex_msg_addr = NULL; /* ͬһ�������ڹ����׵�ַ */
static int g_shm_mutex_number = 0;        /* ͬһ�������ڵ������� */

typedef struct _shm_msg_mutex
{
	pthread_mutex_t mutex; /* �� */
	short total_attach; /* �ܵ�������,Ŀǰ�����ֶ� */
	short attach[SHM_PROCESS_MUTEX_MAX_NUM]; /* ������,���̼����õ� */
}sw_shm_msg_mutex;

/*****************************************************************************
 �� �� ��  : mutex_mem_alloc
 ��������  : ����id�ӹ����ڴ�����һ���ڴ�ڵ�,����ɹ��������ڴ���׵�ַ
 �������  : const int id �ź�����ʶid   
             const int size δ��
             int* state ����     
 �������  : ��
 �� �� ֵ  : NULLʧ�� �ǿճɹ�
 ���ú���  : 
 ע������  : ֻ�ڵ�ǰ�ļ���ʹ��
 
 �޸���ʷ      :
  1.��    ��   : 2015��9��4��
    ��    ��   : huangdch
    �޸�����   : �����ɺ���

*****************************************************************************/
void* mutex_mem_alloc(const int id, const int size, int* state)
{
	if (id < 1 || size < 1)
	{
		return -1;
	}

	int ret = 0;
	int shm_id = 0;
	int temp_size = sizeof(sw_shm_msg_mutex) + SHM_PROCESS_MUTEX_MAX_NUM * sizeof(MUTEX_HANDLE);
	int key_id = SHM_PROCESS_MUTEX_KEY_ID;
	int temp_state = 0;
	int process_flag = 0;
	void* radd = NULL;

	if (NULL == g_shm_mutex_msg_addr)
	{
		/* ���ӹ����� */
		int temp_state = 0;
		g_shm_mutex_msg_addr = swpa_normal_sharedmem_alloc(key_id, temp_size, &temp_state);
		if (NULL == g_shm_mutex_msg_addr)
		{
			SWPA_SEM_PRINT("create key_id=[%d] size=[%d] failed,ret=[%d].\n", key_id, temp_size, ret);
			return NULL;
		}

		/* ��ʼ���� */
		if (0 == temp_state)
		{
			sw_shm_msg_mutex* msg_node = (sw_shm_msg_mutex*)g_shm_mutex_msg_addr;

			pthread_mutexattr_t attr;
			if (0 != pthread_mutexattr_init(&attr))
			{
				return NULL;
			}
			if (0 != pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED))
			{
				return NULL;
			}	
			if (0 != pthread_mutex_init(&msg_node->mutex, &attr))
			{
				return NULL;
			}
			if (0 != pthread_mutexattr_destroy(&attr))
			{
				return NULL;
			}

			/* �״γ�ʼ�� */
			memset(&msg_node->attach, 0, sizeof(msg_node->attach));
			msg_node->total_attach = 0;
			memset(msg_node + 1, 0, temp_size - sizeof(sw_shm_msg_mutex));
		}

		process_flag = 1;
	}

	sw_shm_msg_mutex* msg_node = (sw_shm_msg_mutex*)g_shm_mutex_msg_addr;

	pthread_mutex_lock(&msg_node->mutex);

	/* ����id���Ѿ�����Ĺ����ڴ�����һ���ѷ����δռ�õĽڵ� */
	MUTEX_HANDLE* shm_node = (MUTEX_HANDLE*)(msg_node + 1);
	int k = 0;
	int flag = -1;
	int node = -1;
	for (k = 0 ; k < SHM_PROCESS_MUTEX_MAX_NUM ; k++)
	{
		if (id == shm_node[k].id) /* ���Ѿ�����Ľڵ� */
		{
			flag = 2;
			msg_node->attach[k]++;/* �ڵ�������� */
			msg_node->total_attach++;
			radd = (void*)&shm_node[k]; /* ����һ����ַ */
			*state = 1;
			node = k;
			break;
		}
	}
	if (-1 == flag)
	{
		for (k = 0 ; k < SHM_PROCESS_MUTEX_MAX_NUM ; k++)
		{
			if (0 == shm_node[k].id) /* ��δ����Ľڵ� */
			{
				msg_node->attach[k]++;/* ���ӵĽ����� */
				msg_node->total_attach++;
				shm_node[k].id = id;
				flag = 1;
				radd = (void*)&shm_node[k]; /* ����һ����ַ */
				*state = 0;
				node = k;
				break;
			}
		}
	}


	/* û���ҵ����������Ľڵ���з��� */
	if (-1 == flag && 1 == process_flag)
	{
		pthread_mutex_unlock(&msg_node->mutex);

		ret = swpa_normal_sharedmem_free(SHM_PROCESS_MUTEX_KEY_ID, g_shm_mutex_msg_addr);
		if (ret < 0)
		{
			SWPA_SEM_PRINT("free:mutex_id=[%d] failed,rang out of [%d] ret=[%d].\n", SHM_PROCESS_MUTEX_KEY_ID, SHM_PROCESS_MUTEX_MAX_NUM, ret);
		}

		SWPA_SEM_PRINT("free:mutex_id=[%d] failed,rang out of [%d] ret=[%d].\n", SHM_PROCESS_MUTEX_KEY_ID, SHM_PROCESS_MUTEX_MAX_NUM, ret);

		return NULL;
	}

	if (NULL != radd)
	{
		g_shm_mutex_number++; /* ͬһ�������ڵ�������+1 */
	}

	SWPA_SEM_PRINT("alloc:mutex_id=[%d] flag=[%d] node=[%d] num=[%d] attch=[%d]\n", id, flag, node, g_shm_mutex_number, msg_node->total_attach);

	pthread_mutex_unlock(&msg_node->mutex);

	return radd;
}

/*****************************************************************************
 �� �� ��  : sem_mem_free
 ��������  : ������id���Ҷ�Ӧ���ڴ�鲢�ͷ���Ӧ�Ľڵ�
 �������  : const int id  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ע������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��9��4��
    ��    ��   : huangdch
    �޸�����   : �����ɺ���

*****************************************************************************/
int mutex_mem_free(const int id)
{
	if (id < 1 || NULL == g_shm_mutex_msg_addr)
	{
		return -1;
	}

	int ret = 0;
	int shm_id = 0;

	sw_shm_msg_mutex* msg_node = (sw_shm_msg_mutex*)g_shm_mutex_msg_addr;

	pthread_mutex_lock(&msg_node->mutex);

	MUTEX_HANDLE* shm_node = (MUTEX_HANDLE*)(msg_node + 1);

	int flag = -1;
	int k = 0;
	for (k = 0 ; k < SHM_PROCESS_MUTEX_MAX_NUM ; k++)
	{
		if (id == shm_node[k].id)
		{
			flag = 1;
			g_shm_mutex_number--;
			msg_node->attach[k]--;
			msg_node->total_attach--;
			if (0 == msg_node->attach[k])/* ��������ڴ�ڵ�������Ϊ0���ÿ��ٷ��� */
			{
				SWPA_SEM_PRINT("node=[%d] id=[%d] is set 0.\n", k, id);
				shm_node[k].id = 0;
			}
			break;
		}
	}

	pthread_mutex_unlock(&msg_node->mutex);

	if (g_shm_mutex_number == 0)
	{
		SWPA_SEM_PRINT("call swpa_normal_sharedmem_free, id=[%d] num=[%d] attach=[%d]\n", id, g_shm_mutex_number, msg_node->attach);
		
		ret = swpa_normal_sharedmem_free(SHM_PROCESS_MUTEX_KEY_ID, g_shm_mutex_msg_addr);
		if (0 == ret)
		{
			g_shm_mutex_msg_addr = NULL;
		}
	}
	
	return ret;
}


/**
* @brief ����������
* @param [out] mutex ������ṹ��ָ��
* @param [in] name ����������ƣ�����Ҫ�����ʱ����NULL��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_sem.h
* @note ���е�name��Ҫ���ڿ���̻���ʱ���Ա�����һ��������ͨ�����ƻ�ȡ���û����壬���������б�֤�����Ƶ�Ψһ�ԡ�
*/
int swpa_mutex_create(int* mutex, const char* name)
{
	SWPA_SEM_CHECK(mutex != NULL);
	SWPA_SEM_PRINT("mutex=0x%08x\n", (unsigned int)mutex);
	SWPA_SEM_PRINT("name=%s\n", name);
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
	MUTEX_HANDLE* handle = (MUTEX_HANDLE*)swpa_mem_alloc(sizeof(MUTEX_HANDLE));
	if( handle == NULL)
	{
		return SWPAR_OUTOFMEMORY;
	}

	if(name == NULL || strlen(name) <= 0)
	{
		pthread_mutex_init(&handle->mutex, &attr);
		handle->id = 0;
		(*mutex) = (int)handle;
		return SWPAR_OK;
	}
	else
	{
		swpa_mem_free(handle);
		handle = NULL;
		return SWPAR_NOTIMPL;
	}

	return SWPAR_OK;
}

/* BEGIN: Modified by huangdch, 2015/9/4 
*���̼�������*/
int swpa_processmutex_create(int* mutex, const int mutex_id)
{
	if (mutex_id < 1)
	{
		return -1;
	}

	int size = 0;

	SWPA_SEM_PRINT("mutex=0x%08x\n", (unsigned int)mutex);
	SWPA_SEM_PRINT("mutex_id=%d\n", mutex_id);
	MUTEX_HANDLE* pmutex = NULL;
	size = sizeof(*pmutex);

	/* ����id���빲���ڴ� */
	int state = 0;
	pmutex = (MUTEX_HANDLE *)mutex_mem_alloc(mutex_id, size, &state);
	if (NULL == pmutex)
	{
		return -1;
	}

	if (0 == state)
	{
		pthread_mutexattr_t attr;
		/*���û���������*/
		if (0 != pthread_mutexattr_init(&attr))
		{
			return -3;
		}

		/*���̼�ʹ��*/
		if (0 != pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED))
		{
			return -4;
		}	

		/*���û�����*/
		if (0 != pthread_mutex_init(&pmutex->mutex, &attr))
		{
			return -5;
		}

		/*�ͷŻ���������*/
		if (0 != pthread_mutexattr_destroy(&attr))
		{
			return -6;
		}

		pmutex->id = mutex_id;
	}

	(*mutex) = (int)pmutex;

	return 0;
}

/**
* @brief ����������
* @param [in] mutex ������ṹ��ָ��
* @param [in] timeout ������ʱ����λ������
* @retval 0 �ɹ������
* @retval -1 ʧ�ܻ򲻿���
* @see swpa_sem.h
* @note ��timeoutΪ-1ʱ����ʾ������������timeout����0ʱ����ʾ������������timeout����0ʱ������ʾ������ʱʱ�ޡ�
*/
int swpa_mutex_lock(int* mutex, int timeout)
{

	SWPA_SEM_CHECK(mutex != NULL);
	SWPA_SEM_PRINT("mutex=0x%08x\n", (unsigned int)mutex);
	SWPA_SEM_PRINT("timeout=%d\n", timeout);
	int iret = -1;
    // ����ʱ��
	struct timespec ts;
	MUTEX_HANDLE* handle = (MUTEX_HANDLE*)(*mutex);
	if(timeout == -1)
	{
		iret = pthread_mutex_lock(&handle->mutex);
	}
	else if(timeout == 0)
	{
		iret = pthread_mutex_trylock(&handle->mutex);
	}
	else
	{
		clock_gettime(CLOCK_REALTIME, &ts);    //��ȡ��ǰʱ��
		ts.tv_sec += (timeout / 1000);			//���ϵȴ�ʱ�������
		ts.tv_nsec += (timeout % 1000) * 1000000; //���ϵȴ�ʱ��������
		// ����������1��ʱ��ŵ�����
		ts.tv_sec += ts.tv_nsec / 1000000000;
		ts.tv_nsec = ts.tv_nsec % 1000000000;
		iret = pthread_mutex_timedlock(&(handle->mutex), &ts);
	}
	if(iret < 0)
	{
		return SWPAR_FAIL;
	}
	return SWPAR_OK;
}

/**
* @brief ����������
* @param [in] mutex ������ṹ��ָ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_sem.h
*/
int swpa_mutex_unlock(int* mutex)
{
	SWPA_SEM_CHECK(mutex != NULL);
	SWPA_SEM_PRINT("mutex=0x%08x\n", (unsigned int)mutex);

	MUTEX_HANDLE* handle = (MUTEX_HANDLE*)(*mutex);
	int iret = pthread_mutex_unlock(&handle->mutex);
	if(iret < 0)
	{
		return SWPAR_FAIL;
	}
	return SWPAR_OK;
}

/**
* @brief ɾ��������
* @param [in] mutex ������ṹ��ָ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_sem.h
*/
int swpa_mutex_delete(int* mutex)
{
	SWPA_SEM_CHECK(mutex != NULL);
	SWPA_SEM_PRINT("mutex=0x%08x\n", (unsigned int)mutex);

	MUTEX_HANDLE* handle = (MUTEX_HANDLE*)(*mutex);

	int iret = pthread_mutex_destroy(&handle->mutex);
	if (handle->id > 0)
	{
		mutex_mem_free(handle->id);
	}
	else
	{
		swpa_mem_free(handle);
		handle = NULL;
	}
	if (iret < 0)
	{
		return SWPAR_FAIL;
	}
	return SWPAR_OK;
}

typedef struct tag_rwlock
{
	pthread_rwlock_t rwlock;
}
RWLOCK_HANDLE;

/**
* @brief ������д��
* @param [out] rwlock ��д���ṹ��ָ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_sem.h
*/
int swpa_rwlock_create(int* rwlock, const char* name)
{
	SWPA_SEM_CHECK(rwlock != NULL);
	SWPA_SEM_PRINT("rwlock=%0x08x", (unsigned int)rwlock);

	RWLOCK_HANDLE* handle = swpa_mem_alloc(sizeof(RWLOCK_HANDLE));
	int iret = pthread_rwlock_init((pthread_rwlock_t*)&handle->rwlock, NULL);
	if(iret < 0)
	{
		swpa_mem_free(handle);
		handle = NULL;
		return SWPAR_FAIL;
	}
	*rwlock = (int)handle;
	return SWPAR_OK;
}

/**
* @brief ������д��
* @param [in] rwlock ��д���ṹ��ָ��
* @param [in] timeout ������ʱ����λ������
* @param [in] is_read_mode ָ���Ƿ����Զ�ģʽ����������Ϊдģʽ��
* @retval 0 �ɹ������
* @retval -1 ʧ�ܻ򲻿���
* @see swpa_sem.h
* @note ��timeoutΪ-1ʱ����ʾ������������timeout����0ʱ����ʾ������������timeout����0ʱ������ʾ������ʱʱ�ޡ�
*/
int swpa_rwlock_lock(int* rwlock, int timeout, unsigned int is_read_mode)
{
	SWPA_SEM_CHECK(rwlock != NULL);
	SWPA_SEM_PRINT("rwlock=0x%08x\n", (unsigned int)rwlock);
	SWPA_SEM_PRINT("timeout=%d\n", timeout);
	SWPA_SEM_PRINT("is_read_mode=%d\n", is_read_mode);

	RWLOCK_HANDLE* handle = (RWLOCK_HANDLE*)(*rwlock);
	if(timeout == -1)
	{
		int iret = -1;
		if(is_read_mode)
		{
			iret = pthread_rwlock_rdlock((pthread_rwlock_t*)&handle->rwlock);
		}
		else
		{
			iret = pthread_rwlock_wrlock((pthread_rwlock_t*)&handle->rwlock);
		}
		if(iret < 0)
		{
			return SWPAR_FAIL;
		}
		return SWPAR_OK;
	}
	else if(timeout == 0)
	{
		int iret = -1;
		if(is_read_mode)
		{
			iret = pthread_rwlock_tryrdlock((pthread_rwlock_t*)&handle->rwlock);
		}
		else
		{
			iret = pthread_rwlock_trywrlock((pthread_rwlock_t*)&handle->rwlock);
		}
		if(iret < 0)
		{
			return SWPAR_FAIL;
		}
		return SWPAR_OK;
	}
	else
	{
		int iret = -1;
		struct timespec curtime;
		clock_gettime(CLOCK_MONOTONIC, &curtime);
		curtime.tv_sec += (timeout / 1000);
		curtime.tv_nsec = (timeout % 1000) * 1000000;
		if(is_read_mode)
		{
			iret = pthread_rwlock_timedrdlock((pthread_rwlock_t*)&handle->rwlock, &curtime);
		}
		else
		{
			iret = pthread_rwlock_timedwrlock((pthread_rwlock_t*)&handle->rwlock, &curtime);
		}
		if(iret < 0)
		{
			return SWPAR_FAIL;
		}
		return SWPAR_OK;
	}
}

/**
* @brief ������д��
* @param [in] rwlock ��д���ṹ��ָ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_sem.h
*/
int swpa_rwlock_unlock(int* rwlock)
{
	SWPA_SEM_CHECK(rwlock != NULL);
	SWPA_SEM_PRINT("rwlock=0x%08x\n", (unsigned int)rwlock);
	RWLOCK_HANDLE* handle = (RWLOCK_HANDLE*)(*rwlock);

	int iret = pthread_rwlock_unlock((pthread_rwlock_t*)&handle->rwlock);
	if(iret < 0)
	{
		return SWPAR_FAIL;
	}
	return SWPAR_OK;
}

/**
* @brief ɾ����д��
* @param [in] rwlock ��д���ṹ��ָ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_sem.h
*/
int swpa_rwlock_delete(int* rwlock)
{
	SWPA_SEM_CHECK(rwlock != NULL);
	SWPA_SEM_PRINT("rwlock=0x%08x\n", (unsigned int)rwlock);

	RWLOCK_HANDLE* handle = (RWLOCK_HANDLE*)(*rwlock);
	int iret = pthread_rwlock_destroy((pthread_rwlock_t*)&handle->rwlock);
	swpa_mem_free(handle);
	handle = NULL;
	if(iret < 0)
	{
		return SWPAR_FAIL;
	}
	return SWPAR_OK;
}

