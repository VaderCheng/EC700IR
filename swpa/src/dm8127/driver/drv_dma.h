/**
 * @file   drv_dma.h
 * @author lijj
 * @date   2013-02-06
 *
 * @brief
 *         DMA API������ƽ̨�������ļ��ӿ�
 *
 * @log
 *        2013-04-07 lijj������2��������ע��
 *
 */
 
#ifndef DRV_DMA_H
#define DRV_DMA_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * DMA 1ά����
 *
 * @param dst_phys [in] : Ŀ�������ַ
 * @param src_phys [in] : Դ�����ַ
 * @param size [in]     : ���ݴ�С
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 * @note
 *        !!! ���ñ�����(1ά����)���������ȺϷ���Χ��1~65535�ֽڣ�������Ӳ�����Ծ����ġ�
 *        �绺�����ϴ���ʹ��2ά���� !!!
 */
int drv_dma_copy_1d(unsigned char* dst_phys, unsigned char* src_phys, unsigned int size);

/**
 * DMA 2ά����
 *
 * @param dst_phys [in]   : Ŀ�������ַ
 * @param dst_stride [in] : ���
 * @param src_phys [in]   : Դ�����ַ
 * @param src_stride [in] : ���
 * @param width [in]      : ��
 * @param height [in]     : ��
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *        ���߷�Χ��1~65535
 */
int drv_dma_copy_2d(unsigned char* dst_phys, unsigned int dst_stride,
                    unsigned char* src_phys, unsigned int src_stride,
                    unsigned int width, unsigned int height);

#ifdef __cplusplus
}
#endif

#endif /* DRV_DMA_H */
