/**
* @file swpa_mem.h
* @brief �ڴ����
* @author Shaorg
* @date 2013-02-26
* @version v0.1
* @note �����Ը����ڴ�������롢�ͷ��Լ���ѯ��һ�麯����
*/

#ifndef _SWPA_MEM_H_
#define _SWPA_MEM_H_

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct tag_meminfo
{
	unsigned int size;
	unsigned int used;
	unsigned int free;
}
SWPA_MEM_INFO;

/**
* @brief ��ʼ��ϵͳ���ڴ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
*/
int swpa_mem_init(void);

/**
* @brief ����ʼ��ϵͳ���ڴ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
*/
int swpa_mem_uninit(void);

/**
* @brief ����ϵͳ���ڴ�
* @param [in] len �ڴ泤��
* @retval �ڴ��ַ
* @see swpa_mem.h
*/
void* swpa_mem_alloc(unsigned int len);

/**
* @brief �ͷ�ϵͳ���ڴ�
* @param [in] addr �ڴ��ַ
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
*/
int swpa_mem_free(void* addr);

/**
* @brief ��ȡϵͳ���ڴ���Ϣ
* @param [out] info �ڴ���Ϣ�����а����ڴ�������Լ��������������Ϣ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
*/
int swpa_mem_getinfo(SWPA_MEM_INFO* info);

/**
* @brief ��ʼ�������ڴ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
*/
int swpa_cachemem_init(void);

/**
* @brief ����ʼ�������ڴ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
*/
int swpa_cachemem_uninit(void);

/**
* @brief ��������ڴ�
* @param [in] segid �ڴ�λ�ñ�ʶ
* @param [in] len �ڴ泤��
* @param [in] align_size �ڴ��׵�ַ�����С
* @retval �ڴ��ַ
* @see swpa_mem.h
* @note �����ڴ漴Ƭ�ϣ�on-chip���ڴ棬��DSP�п��䡣
*/
void* swpa_cachemem_alloc(int segid, unsigned int len, unsigned int align_size);

/**
* @brief �ͷſ����ڴ�
* @param [in] segid �ڴ�λ�ñ�ʶ
* @param [in] addr �ڴ��ַ
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
*/
int swpa_cachemem_free(int segid, void* addr);

/**
* @brief ��ȡ�����ڴ���Ϣ
* @param [out] info �ڴ���Ϣ�����а����ڴ�������Լ��������������Ϣ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
*/
int swpa_cachemem_getinfo(SWPA_MEM_INFO* info);

/**
* @brief ��ʼ�������ڴ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
* @note ��ν�Ĺ����ڴ漴TI�����ܹ�������ARM��DSP���õ��ڴ档
*/
int swpa_sharedmem_init(void);

/**
* @brief ����ʼ�������ڴ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
*/
int swpa_sharedmem_uninit(void);

/**
* @brief ���乲���ڴ�
* @param [out] addr �����ڴ��ַ
* @param [out] phys �����ڴ��ַ
* @param [in] len �ڴ泤��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
*/
int swpa_sharedmem_alloc(void** addr, void** phys, unsigned int len);

/**
* @brief �ͷŹ����ڴ�
* @param [in] addr �ڴ��ַ
* @param [in] is_phys_addr ָ���ڴ��ַ�Ƿ�Ϊ�����ַ
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
*/
int swpa_sharedmem_free(void* addr, unsigned int is_phys_addr);

/**
* @brief ��ȡ�����ڴ���Ϣ
* @param [out] info �ڴ���Ϣ�����а����ڴ�������Լ��������������Ϣ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_mem.h
*/
int swpa_sharedmem_getinfo(SWPA_MEM_INFO* info);

/**
* @brief ��⹲���ڴ�״̬
* @retval 0 ����
* @retval 1 �쳣�����������쳣��
* @see swpa_mem.h
*/
int swpa_sharedmem_check();

/* BEGIN: Modified by huangdch, 2015/8/21 
*ϵͳ��ͨ�����ڴ�ͳһ�����ӿ�*/

/*****************************************************************************
 �� �� ��  : swpa_normal_sharedmem_init
 ��������  : ��ͨ�����ڴ��ʼ���ӿ�
 �������  : void  
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ע������  : Ŀǰ��ʵ�ʲ���
 
 �޸���ʷ      :
  1.��    ��   : 2015��8��28��
	��    ��   : huangdch
	�޸�����   : �����ɺ���
*****************************************************************************/
int swpa_normal_sharedmem_init(void);

/*****************************************************************************
 �� �� ��  : swpa_normal_sharedmem_uninit
 ��������  : ����ʼ�������ڴ�
 �������  : void  
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ע������  : Ŀǰ��ʵ�ʲ���
 
 �޸���ʷ      :
  1.��    ��   : 2015��8��28��
	��    ��   : huangdch
	�޸�����   : �����ɺ���
*****************************************************************************/
int swpa_normal_sharedmem_uninit(void);

/*****************************************************************************
 �� �� ��  : swpa_normal_sharedmem_alloc
 ��������  : ��ͨ�����ڴ����ӿ�
 �������  : const int id  �����ڴ�����ʶ  
			 const int size  �����ڴ�Ĵ�С
			 int* state -1��ʾ���������ڴ�id������,ֱ�ӷ���ʧ��
			 			0 ��ʾ���������ڴ�id������,�����·��䲢����
 �������  : int* state 
 						С�� 0��ʾ�ڴ����ʧ��
 						���� 0��ʾ�ڴ�id�Ѿ����������̷���,��Ӧ��ֵΪ�Ѿ�������ڴ��С
 						���� 0��ʾ�ڴ�id�ǵ�һ�η����,�����ڴ�Ĵ�СΪ�����size
 �� �� ֵ  : void*  	��NULL��ʾ����ɹ�,NULL��ʾ����ʧ��
 ���ú���  : 
 ע������  : *sate������Ϊ��������ֵ
 
 �޸���ʷ      :
  1.��    ��   : 2015��8��28��
	��    ��   : huangdch
	�޸�����   : �����ɺ���
*****************************************************************************/
void* swpa_normal_sharedmem_alloc(const int id, const int size, int* state);

/*****************************************************************************
 �� �� ��  : swpa_normal_sharedmem_free
 ��������  : ��ͨ�����ڴ���ͷ�
 �������  : const int id �ڴ�����id
			 void* pAdd swpa_normal_sharedmem_alloc()���صĵ�ַ
 �������  : ��
 �� �� ֵ  : int 0�ͷųɹ� С��0�ͷ�ʧ�� ����0��ʾ�����ڴ汻��������ռ��
 ���ú���  : 
 ע������  : ������Ҫ��֤id��pAddΪ�����ԭʼֵ������ִ�н�����ܻ��쳣�쳣���ڴ��ͷ�ʧ��
 			 ��������ڴ汻��������ռ��,�ͷ�ʧ��,ͬʱ����ռ�õĽ�����
 
 �޸���ʷ      :
  1.��    ��   : 2015��8��28��
	��    ��   : huangdch
	�޸�����   : �����ɺ���
*****************************************************************************/
int swpa_normal_sharedmem_free(const int id, void* pAdd);


/**
* @brief �ڴ濽������
*/
#define swpa_memcpy		memcpy
/**
* @brief �ڴ����ݸ�ʽ��
*/
#define swpa_memset		memset

/* 
* @brief �ڴ����ݶԱ�
*/
#define swpa_memcmp		memcmp

#define swpa_memmove  memmove

#ifdef __cplusplus
}
#endif

#endif

