/**  
* @file  swpa_dma.c
* @brief  dma��ؽӿ�
* @author  
* @date  2013-2-20
* @version  V0.1
* @note     
*/ 


#include "drv_dma.h"

#ifdef SWPA_DMA
#define SWPA_DMA_PRINT(fmt, ...) SWPA_PRINT("[%s:%d]"fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define SWPA_DMA_CHECK(arg)      {if (!(arg)){SWPA_PRINT("[%s:%d]Check failed : %s [%d]\n", __FILE__, __LINE__, #arg, SWPAR_INVALIDARG);return SWPAR_INVALIDARG;}}
#else
#define SWPA_DMA_PRINT(fmt, ...)
#define SWPA_DMA_CHECK(arg)
#endif
/**
* @brief DMAһά����\n
*
* 
* @param [in] src_phys_addr : Դ���ݿ����������ַ
* @param [in] dst_phys_addr : Ŀ�����ݿ����������ַ
* @param [in] size : ���������ݳ���
* @retval 0 : ��ʾ�����ɹ�
* @retval ���� : ��ʾ����ʧ��
* @note .
* @see .
*/
int swpa_dma_copy_1d(
	unsigned char * src_phys_addr, 
	unsigned char * dst_phys_addr, 	
	int size
)
{
	return drv_dma_copy_1d(src_phys_addr, dst_phys_addr, size);
}



/**
* @brief DMA��ά����\n
*
* 
* @param [in] src_phys_addr : Դ���ݿ����������ַ
* @param [in] src_stride : Դ���ݿ���
* @param [in] dst_phys_addr : Ŀ�����ݿ����������ַ
* @param [in] dst_stride : Ŀ�����ݿ���
* @param [in] width : ���������ݿ��
* @param [in] height : ���������ݸ߶�
* @retval 0 : ��ʾ�����ɹ�
* @retval ���� : ��ʾ����ʧ��
* @note .
* @see .
*/
int swpa_dma_copy_2d(
	unsigned char * src_phys_addr, 
	int src_stride,
	unsigned char * dst_phys_addr, 
	int dst_stride,
	int width, 
	int height
)
{
	return drv_dma_copy_2d(src_phys_addr, src_stride, dst_phys_addr, dst_stride, width, height);
}




