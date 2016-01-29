/**
 * @file   drv_vpif.h
 * @author lijj
 * @date   2013-02-18
 *
 * @brief
 *         VPIFͼ��ɼ�ģ�顡����ƽ̨�������ļ��ӿ�
 * @note
 *         1��ʹ��VPIFģ��֮ǰ<b>�����ȳ�ʼ��FPGA</b>����
 *         2��get�ĵ�ַ��put�ĵ�ַ���Բ�һ�£���ַ����ȷ�����û���֤��\n
 *         3���ɼ����������Ϊ32��������3����\n
 *         4���ɼ�ͼ��ĸ�ʽ��YUV422SP��ͼ���С��w*h*2��\n
 *         5���ɼ�ģ�ͣ�\n
 *         ��ʼ��drv_capture_init -> drv_capture_put_buffer->drv_capture_start \n
 *         ѭ���ɼ���while (1) { get_buffer -> process frame -> put_buffer } \n
 *         ������drv_capture_stop -> drv_capture_release \n
 * @log
 *      2013-04-15 ���Ƶ�������Ϊ������ʽ���ʡ�
 */

#ifndef DRV_VPIF_H
#define DRV_VPIF_H

#ifdef __cplusplus
extern "C"
{
#endif

/// �ɼ�ͼ��ṹ��
/**
@note
�˽ṹ����Ϊ�˷���DM6467ƽ̨�����ڴ��ʹ�ã�
����ʹ��ARM�ڴ���вɼ�ʱ(�����ں˷����ڴ�)���������phys_addr��
**/
typedef struct _V4L2_BUFFER
{
    void* virt_addr;     /**< �����ַ */
    void* phys_addr;     /**< �����ַ */
    int len;            /**< ͼ���С */

#ifdef __cplusplus
    _V4L2_BUFFER()
    {
        virt_addr = NULL;
        phys_addr = NULL;
        len = 0;
    };
#endif

} V4L2_BUFFER;

/**
 * ��ʼ��VPIF���������豸��������Ƶ��ʽ
 *
 * @param is_user_mem [in] : ��ʹ���û�������ڴ棬�����ں��Զ�����(1���û��ڴ棬�������ڴ� 0���ں��Զ�����)
 * @param width [out]      : ͼ���
 * @param height [out]     : ͼ���
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *	       1�����Ϊ�û������ڴ棬��drv_capture_start֮ǰ��Ҫ��ͨ��drv_capture_put_buffer�ṩ����õ��ڴ�\n
 *         ����Ϊ�������������ں˻�ȡ��\n
 *         2�����ʷ�ʽΪ��������
 *         3���ڵ���VPIFģ�麯��ǰ(����������)�������ȼ���FPGA������
 *
 * @sa drv_capture_start drv_capture_put_buffer
 */
int drv_capture_init(int is_user_mem, int* width, int* height);

/**
 * ��ʼ�ɼ�
 *
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note
 *
 */
int drv_capture_start(void);

/**
 * ֹͣ�ɼ�
 *
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 */
int drv_capture_stop(void);

/**
 * ��ȡͼ������
 *
 * @param img_buffer [out] : ͼ�����ݽṹ��
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 *
 * @note
 *        1������������ʧ��ʱ��img_buffer��δ�����\n
 *        2������ȡ����ͼ��ʱ����������ೢ��15��(��15*10ms = 150ms)���󷵻�-6
 */
int drv_capture_get_buffer(V4L2_BUFFER* img_buffer);

/**
 * �Ż�ͼ������
 *
 * @param img_buffer [in] : ͼ�����ݽṹ��
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2 ��ʱ��-6
 *
 * @note
 *         1����������2�����ã������ڴ桢�Ż�ͼ������\n
 *         �����ڴ棺��Ҫѭ�����ñ���������Ҫ����5������������Ҫ����5�α�������\n
 *         ���ʹ���ں��ڴ棬img_buffer��ΪNULL
 *         �Ż�ͼ��img_buffer������drv_capture_get_buffer��ȡ�ĵ�ַ��һ�¡�
 *         2�����Ż�ͼ�����ݳ�ʱʱ����������ೢ��15��(��15*10ms = 150ms)���󷵻�-6
 *
 */
int drv_capture_put_buffer(V4L2_BUFFER* img_buffer);

/**
 * �ͷ���Դ���ر��豸
 *
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 */
int drv_capture_release(void);

#ifdef __cplusplus
}
#endif

#endif /* DRV_VPIF_H */
