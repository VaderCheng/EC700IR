#ifndef __SWPA_LIST_H__
#define __SWPA_LIST_H__

#ifdef __cplusplus
#define DEFAULT(val) = val
extern "C"
{
#else
#define DEFAULT(val)
#endif

/**
 *@brief ��������
 *@param [in] max_size ���������С��0���������ƴ�С
 *@return �ɹ�������������ʧ�ܷ���-1
 */
int swpa_list_create(int max_size DEFAULT(0));

/**
 *@brief �ͷ�������Դ
 *@param [in] handle ������
 *@return �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_list_destroy(int handle);

/**
 *@brief �������β���һ���ڵ�
 *@param [in] handle ������
 *@param [in] value �ڵ��ֵ
 *@return �ɹ������²���ڵ��λ�ã�ʧ�ܷ���-1
 */
int swpa_list_add(int handle, void *value);

/**
 *@brief ���������һ���ڵ�
 *@param [in] handle ������
 *@param [in] pos �ڵ�λ��
 *@param [in] value �ڵ��ֵ
 *@return �ɹ����ؽڵ�λ�ã�ʧ�ܷ���-1
 */
int swpa_list_insert(int handle, int pos, void *value);

/**
 *@brief ɾ�������е�һ���ڵ�
 *@param [in] handle ������     
 *@param [in] pos �ڵ�λ��
 *@param [in] �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_list_delete(int handle, int pos);

/**
 *@brief �������ͷ���ɾ��һ���ڵ�
 *@param [in] handle ������
 *@param [in] from_head Ϊ0����ɾ��ͷ�ڵ㣬Ϊ1����ɾ��β�ڵ�
 *@param [in] timeout -1��ʾ����������0��ʾ��������������0��ʾ������ʱʱ�ޡ�
 *@retval �ɹ����ؽڵ��ֵ��ʧ�ܷ���0
 */
void* swpa_list_remove(int handle, int from_head DEFAULT(0), int timeout DEFAULT(0));

/**
 *@brief ���ݽڵ�λ�û�ýڵ��ֵ
 *@param [in] handle ������
 *@param [in] pos �ڵ�λ��
 *@return �ɹ����ؽڵ�ֵ��ʧ�ܷ���0
 */
void* swpa_list_value(int handle, int pos);

/**
 *@brief ����Ĵ�С
 *@param [in] handle ������
 *@return ��������ڵ�ĸ���
 */
int swpa_list_size(int handle);

/**
 *@brief ������������֮ǰ������open
 *@param [in] handle ������
 *@param [in] from_head �ͷŴ�ͷ�ڵ㿪ʼ������0������ͷ�ڵ㿪ʼ��1������β�ڵ㿪ʼ����
 *@return �ɹ�����ͷ�ڵ�λ�ã�ʧ�ܷ���-1
 */
int swpa_list_open(int handle, int from_head DEFAULT(0));

/**
 *@brief ����ڵ�����һ���ڵ�
 *@brief handle ����ľ��
 *@return �ɹ����ص�ǰ�ڵ�λ��,ʧ�ܷ���-1
 */
int swpa_list_next(int handle);

/**
 *@brief ����ڵ�����һ���ڵ�
 *@brief handle ������
 *@return ���ص�ǰ�ڵ�λ��
 */
int swpa_list_prev(int handle);

/**
 *@brief ֹͣ����
 *@brief handle ������
 *@return �ɹ�����0��ʧ�ܷ���-1
 */
int swpa_list_close(int handle);

#ifdef __cplusplus
}
#endif
#endif
