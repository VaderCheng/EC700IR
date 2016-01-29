#include <map>
#include "swpa.h"
#include "swpa_map.h"
using namespace std;

class NODE
{
public:	
	NODE()
	{
		value = 0;
		compare = 0;
	}
	
	NODE(void* v, int (*cmp)(void*,void*) = 0)
	{
		value = v;
		compare = cmp;
	}
	
	NODE(const NODE &node)
	{
		value = node.value;
		compare = node.compare;
	}
	
	virtual ~NODE()
	{
	}
	
	bool operator<(const NODE &node) const
	{
		return 0 != compare ? 0 > compare(value, node.value) : (int)value < (int)node.value;
	}
	
	void* value;
	int (*compare)(void*,void*);
};

typedef struct tagMAP
{
	int       mutex;              //������
	int       lock;               //���ı�־
	map<NODE,NODE>mymap;	        //ӳ���
	int       fdel;               //�Ƿ�ɾ����־
	map<NODE,NODE>::iterator itr; //ָ��λ��
	int (*compare)(void*,void*);  //key�Ƚϵĺ���ָ��,-1����С��,0������ȣ�1��������
}MAP;

/**
 *@brief ����ӳ���
 *@return �ɹ�����ӳ�������ʧ�ܷ���-1
 */
int swpa_map_create(int (*compare)(void*, void*))
{
	MAP *mymap = new MAP;
	if(0 != mymap)
	{
		mymap->lock = 0;
		mymap->fdel = 0;
		swpa_mutex_create(&mymap->mutex, 0);
		mymap->compare = compare;
		return (int)mymap;
	}
	return -1;
}

/**
 *@brief ɾ��ӳ���
 *@param [in] handle ӳ�����
 *return �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_map_destroy(int handle)
{
	MAP *mymap = (MAP *)handle;
	if(0 != mymap)
	{
		swpa_mutex_delete(&mymap->mutex);
		delete mymap;
		return 0;
	}
	return -1;
}

/**
 *@brief ���һ���ڵ�
 *@param [in] handle ӳ�����
 *@param [in] key ӳ����ֵ
 *@param [in] valueӳ����ֵ
 *@return �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_map_add(int handle, void* key, void* value)
{
	int ret = -1;
	MAP *mymap = (MAP *)handle;
	if(mymap)
	{
		swpa_mutex_lock(&mymap->mutex, -1);
		mymap->lock++;
		NODE k(key, mymap->compare), v(value, mymap->compare);
		map<NODE,NODE>::iterator itr = mymap->mymap.find(k);
		if(itr == mymap->mymap.end())
		{
			mymap->mymap.insert(pair<NODE,NODE>(k,v));
			ret = 0;
		}
		mymap->lock--;
		swpa_mutex_unlock(&mymap->mutex);
	}
	return ret;
}

/**
 *@brief ɾ��һ���ڵ�
 *@param [in] handle ӳ�����
 *@param [in] key ӳ����ֵ
 *@return �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_map_delete(int handle, void* key)
{
	int ret = -1;
	MAP *mymap = (MAP *)handle;
	if(mymap)
	{
		swpa_mutex_lock(&mymap->mutex, -1);
		mymap->lock++;
		NODE k(key, mymap->compare);
		map<NODE,NODE>::iterator itr = mymap->mymap.find(k);
		if(itr != mymap->mymap.end())
		{
			mymap->fdel = (mymap->itr == itr);
			mymap->mymap.erase(itr++);
			if(mymap->fdel)
			{
				mymap->itr= itr;
			}
			ret = 0;
		}
		mymap->lock--;
		swpa_mutex_unlock(&mymap->mutex);
	}
	return ret;
}

/**
 *@brief ӳ���Ĵ�С
 *@param [in] handle ӳ�����
 *@return ����ӳ����С
 */
int swpa_map_size(int handle)
{
	int size = 0;
	MAP *mymap = (MAP *)handle;
	if(mymap)
	{
		swpa_mutex_lock(&mymap->mutex, -1);
		mymap->lock++;
		size = mymap->mymap.size();
		mymap->lock--;
		swpa_mutex_unlock(&mymap->mutex);
	}
	return size;
}


/**
 *@brief ��������key�Ľڵ�
 *@param [in] handle ӳ�����
 *@param [in] key ӳ����ֵ
 *@return �ɹ����ؽڵ��ֵ��ʧ�ܷ���0
 */
void* swpa_map_find(int handle, void* key)
{
	void* value = 0;
	MAP *mymap = (MAP *)handle;
	if(mymap)
	{
		swpa_mutex_lock(&mymap->mutex, -1);
		mymap->lock++;
		NODE k(key, mymap->compare);
		map<NODE,NODE>::iterator itr = mymap->mymap.find(k);
		if(itr != mymap->mymap.end())
		{
			value = itr->second.value;
		}
		mymap->lock--;
		swpa_mutex_unlock(&mymap->mutex);
	}
	return value;
}

/**
 *@brief ��ӳ���׼��������
 *@param [in] handle ӳ�����
 *@return �ɹ�����ӳ���ͷ���λ�ã�ʧ�ܷ���-1
 */
int swpa_map_open(int handle)
{
	int ret = -1;
	MAP *mymap = (MAP *)handle;
	if(mymap && 0 < mymap->mymap.size())
	{
		swpa_mutex_lock(&mymap->mutex, -1);
		mymap->lock++;
		mymap->fdel = 0;
		mymap->itr = mymap->mymap.begin();
		ret = (int)&mymap->itr;
	}
	return ret;
}

/**
 *@brief ȡ����һ���ڵ�
 *@param [in] handle ӳ�����
 *@return ������һ���ڵ�λ��
 */
int swpa_map_next(int handle)
{
	int ret = -1;
	MAP *mymap = (MAP *)handle;
	if(mymap && 0 < mymap->mymap.size())
	{
		if(!mymap->fdel)
		{
			mymap->itr++;
		}
		else
		{
			mymap->fdel = 0;
		}
		if(mymap->itr != mymap->mymap.end())
		{
			ret = (int)&mymap->itr;
		}
	}
	return ret;
}

/**
 *@brief �ر�ӳ���ֹͣ����
 *@param [in] handle ӳ�����
 *@return �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_map_close(int handle)
{
	int ret = -1;
	MAP *mymap = (MAP *)handle;
	if(mymap && mymap->lock > 0)
	{
		mymap->lock--;
		swpa_mutex_unlock(&mymap->mutex);
		ret = 0;
	}
	return ret;
}

/**
 *@brief �õ���ǰ�ڵ��key
 *@param [in] handle ӳ�����
 *@param [in] pos λ��
 *@return �ɹ����ؽڵ��key,ʧ�ܷ���0
 */
void* swpa_map_key(int handle, int pos)
{
	void* key = 0;
	MAP *mymap = (MAP *)handle;
	if(mymap && 0 < pos)
	{
		swpa_mutex_lock(&mymap->mutex, -1);
		mymap->lock++;
		map<NODE,NODE>::iterator itr = *(map<NODE,NODE>::iterator *)pos;
		key = itr->first.value;
		mymap->lock--;
		swpa_mutex_unlock(&mymap->mutex);
	}
	return key;
}

/**
 *@brief �õ���ǰ�ڵ��value
 *@param [in] handle ӳ�����
 *@param [in] pos λ��
 *@return �ɹ����ؽڵ��value,ʧ�ܷ���0
 */
void* swpa_map_value(int handle, int pos)
{
	void* value = 0;
	MAP *mymap = (MAP *)handle;
	if(mymap && 0 < pos)
	{
		swpa_mutex_lock(&mymap->mutex, -1);
		mymap->lock++;
		map<NODE,NODE>::iterator itr = *(map<NODE,NODE>::iterator *)pos;
		value = itr->second.value;
		mymap->lock--;
		swpa_mutex_unlock(&mymap->mutex);
	}
	return value;
}

