#ifndef __SWPA_MAP_H__
#define __SWPA_MAP_H__
#ifdef __cplusplus
#define DEFAULT(val) = val
extern "C"
{
#else
#define DEFAULT(val)
#endif

/**
 *@brief ����ӳ���
 *@param [in]int (*compare)(void* key1, void* key2)key�Ƚϵĺ���ָ��,-1����С��,0������ȣ�1��������
 *@return �ɹ�����ӳ�������ʧ�ܷ���-1
 */
int swpa_map_create(int (*compare)(void*, void*) DEFAULT(0));

/**
 *@brief ɾ��ӳ���
 *@param [in] handle ӳ�����
 *return �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_map_destroy(int handle);

/**
 *@brief ���һ���ڵ�
 *@param [in] handle ӳ�����
 *@param [in] key ӳ����ֵ
 *@param [in] valueӳ����ֵ
 *@return �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_map_add(int handle, void* key, void* value);

/**
 *@brief ɾ��һ���ڵ�
 *@param [in] handle ӳ�����
 *@param [in] key ӳ����ֵ
 *@return �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_map_delete(int handle, void* key);

/**
 *@brief ӳ���Ĵ�С
 *@param [in] handle ӳ�����
 *@return ����ӳ����С
 */
int swpa_map_size(int handle);

/**
 *@brief ��������key�Ľڵ�
 *@param [in] handle ӳ�����
 *@param [in] key ӳ����ֵ
 *@return �ɹ����ؽڵ��ֵ��ʧ�ܷ���0
 */
void* swpa_map_find(int handle, void* key);

/**
 *@brief ��ӳ���׼��������
 *@param [in] handle ӳ�����
 *@return �ɹ�����ӳ���ͷ���λ�ã�ʧ�ܷ���-1
 */
int swpa_map_open(int handle);

/**
 *@brief ȡ����һ���ڵ�
 *@param [in] handle ӳ�����
 *@return ������һ���ڵ�λ��
 */
int swpa_map_next(int handle);

/**
 *@brief �ر�ӳ���ֹͣ����
 *@param [in] handle ӳ�����
 *@return �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_map_close(int handle);

/**
 *@brief �õ���ǰ�ڵ��key
 *@param [in] handle ӳ�����
 *@param [in] pos λ��
 *@return �ɹ����ؽڵ��key,ʧ�ܷ���0
 */
void* swpa_map_key(int handle, int pos);

/**
 *@brief �õ���ǰ�ڵ��value
 *@param [in] handle ӳ�����
 *@param [in] pos λ��
 *@return �ɹ����ؽڵ��value,ʧ�ܷ���0
 */
void* swpa_map_value(int handle, int pos);

#ifdef __cplusplus
}
#endif
#endif
