/**
 * @file   drv_cmem.h
 * @author lijj
 * @date   2013-02-18
 *
 * @brief
 *         �����ڴ�cmem���API
 * @note   �ڴ��ַ�滮��

 ARM(0~96MB����96MB):0x80000000 ~ 0x86000000
 CMEM(96MB~448MB����352MB): 0x86000000 ~ 0x9c000000
 DSP(448MB~512MB����64MB):0x9c000000 ~ 0x20000000

 */

#ifndef DRV_CMEM_H
#define DRV_CMEM_H

//#include <cmem.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * ��ʼ��CMEMģ�鲢���빲���ڴ�
 *
 * @param virt_addr [out] : ���뵽���ڴ������ַ
 * @param phys_addr [out] : ���뵽���ڴ������ַ
 * @param len [out]       : �����ڴ��ܳ���
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         ������ֻ���������õĹ����ڴ棬lenΪ���ڴ��С��\n
 *         �������ʹ�ô��ڴ棬�ɵ����߾�����
 * @sa drv_cmem_exit
 */
int drv_cmem_init(void** virt_addr, void** phys_addr, int* len);

/**
 * �ͷŹ����ڴ沢�˳�CMEMģ��
 *
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note
 *         ���ñ������󣬹����ڴ潫�����á�
 * @sa drv_cmem_init
 */
int drv_cmem_exit(void);

#ifdef __cplusplus
}
#endif

#endif /* DRV_CMEM_H */
