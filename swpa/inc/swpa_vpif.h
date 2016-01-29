/**  
* @file  swpa_vpif.h   
* @brief  ץ���豸���
 *         VPIFͼ��ɼ�ģ��
 *         �ɼ�ģ�ͣ�
 *         ��ʼ��swpa_capture_init -> swpa_capture_put_buffer->swpa_vpif_set_imginfo_callback->swpa_capture_start \n
 *         ѭ���ɼ���while (1) { get_buffer -> process frame -> put_buffer } \n
 *         ������swpa_capture_stop -> swpa_capture_release
* @author  
* @date  2013-2-20
* @version  V0.1
* @note     
*/ 

#ifndef _SWPA_VPIF_H_
#define _SWPA_VPIF_H_

#ifdef __cplusplus
extern "C"
{
#endif

/// ͼ����Ϣ�ṹ��
#define IMAGE_INFO_ENTRY(name) int name; int dummy_##name;

typedef struct _CAPTURE_IMAGE_INFO_Y // Yͨ���������Ϣ
{
	IMAGE_INFO_ENTRY(vgagain)
	IMAGE_INFO_ENTRY(work_mode)
	IMAGE_INFO_ENTRY(syn_ac_ext)
	IMAGE_INFO_ENTRY(shutter_v)
	IMAGE_INFO_ENTRY(r_gain_v)
	IMAGE_INFO_ENTRY(g_gain_v)
	IMAGE_INFO_ENTRY(b_gain_v)
	IMAGE_INFO_ENTRY(capture_en)
	IMAGE_INFO_ENTRY(capture_inf)
	IMAGE_INFO_ENTRY(capture_en_flag)
	IMAGE_INFO_ENTRY(shutter_c)
	IMAGE_INFO_ENTRY(r_gain_c)
	IMAGE_INFO_ENTRY(g_gain_c)
	IMAGE_INFO_ENTRY(b_gain_c)
	IMAGE_INFO_ENTRY(vga_gain_c_1)
	IMAGE_INFO_ENTRY(vga_gain_c_2)
	IMAGE_INFO_ENTRY(vga_gain_c_3)
	IMAGE_INFO_ENTRY(vga_gain_c_4)
	IMAGE_INFO_ENTRY(clamplevel_c_1)
	IMAGE_INFO_ENTRY(clamplevel_c_2)
	IMAGE_INFO_ENTRY(clamplevel_c_3)
	IMAGE_INFO_ENTRY(clamplevel_c_4)
	IMAGE_INFO_ENTRY(led_set)
	IMAGE_INFO_ENTRY(flash_set_1)
	IMAGE_INFO_ENTRY(flash_set_2)
	IMAGE_INFO_ENTRY(flash_set_3)
	IMAGE_INFO_ENTRY(aoto_iris_ctrl)
	IMAGE_INFO_ENTRY(switcher_ctrl)
	IMAGE_INFO_ENTRY(floor_lamps)
	IMAGE_INFO_ENTRY(time_cnt_out)
	IMAGE_INFO_ENTRY(time_cnt_out_vd)
	IMAGE_INFO_ENTRY(time_cap1)
	IMAGE_INFO_ENTRY(time_cap2)
	IMAGE_INFO_ENTRY(capture_flag)
	IMAGE_INFO_ENTRY(factor_r_0)
	IMAGE_INFO_ENTRY(factor_r_1)
	IMAGE_INFO_ENTRY(factor_r_2)
	IMAGE_INFO_ENTRY(factor_r_3)
	IMAGE_INFO_ENTRY(factor_g_0)
	IMAGE_INFO_ENTRY(factor_g_1)
	IMAGE_INFO_ENTRY(factor_g_2)
	IMAGE_INFO_ENTRY(factor_g_3)
	IMAGE_INFO_ENTRY(factor_b_0)
	IMAGE_INFO_ENTRY(factor_b_1)
	IMAGE_INFO_ENTRY(factor_b_2)
	IMAGE_INFO_ENTRY(factor_b_3)
	IMAGE_INFO_ENTRY(vgagain_a)
	IMAGE_INFO_ENTRY(vgagain_b)
	IMAGE_INFO_ENTRY(vgagain_c)
	IMAGE_INFO_ENTRY(vgagain_d)
	IMAGE_INFO_ENTRY(clamplevel_a)
	IMAGE_INFO_ENTRY(clamplevel_b)
	IMAGE_INFO_ENTRY(clamplevel_c)
	IMAGE_INFO_ENTRY(clamplevel_d)
} CAPTURE_IMAGE_INFO_Y;

typedef struct _CAPTURE_IMAGE_INFO_C // Cͨ���������Ϣ
{
	IMAGE_INFO_ENTRY(th_custom)
	IMAGE_INFO_ENTRY(th_count)
	IMAGE_INFO_ENTRY(gamma_data)
	IMAGE_INFO_ENTRY(gamma_addr_prt)
	IMAGE_INFO_ENTRY(addr_prt)
	IMAGE_INFO_ENTRY(cod_x_min_prt)
	IMAGE_INFO_ENTRY(cod_x_max_prt)
	IMAGE_INFO_ENTRY(cod_y_min_prt)
	IMAGE_INFO_ENTRY(cod_y_max_prt)
	IMAGE_INFO_ENTRY(sum_r)
	IMAGE_INFO_ENTRY(sum_g)
	IMAGE_INFO_ENTRY(sum_b)
	IMAGE_INFO_ENTRY(sum_y)
	IMAGE_INFO_ENTRY(sum_u)
	IMAGE_INFO_ENTRY(sum_v)
	IMAGE_INFO_ENTRY(p_count)
	IMAGE_INFO_ENTRY(avg_y_15)
	IMAGE_INFO_ENTRY(avg_y_14)
	IMAGE_INFO_ENTRY(avg_y_13)
	IMAGE_INFO_ENTRY(avg_y_12)
	IMAGE_INFO_ENTRY(avg_y_11)
	IMAGE_INFO_ENTRY(avg_y_10)
	IMAGE_INFO_ENTRY(avg_y_9)
	IMAGE_INFO_ENTRY(avg_y_8)
	IMAGE_INFO_ENTRY(avg_y_7)
	IMAGE_INFO_ENTRY(avg_y_6)
	IMAGE_INFO_ENTRY(avg_y_5)
	IMAGE_INFO_ENTRY(avg_y_4)
	IMAGE_INFO_ENTRY(avg_y_3)
	IMAGE_INFO_ENTRY(avg_y_2)
	IMAGE_INFO_ENTRY(avg_y_1)
	IMAGE_INFO_ENTRY(avg_y_0)
} CAPTURE_IMAGE_INFO_C;

/// �ɼ�ͼ�����ݽṹ��
typedef struct _CAPTURE_BUFFER
{
    void* virt_addr;     /**< �����ַ */
    void* phys_addr;     /**< �����ַ */
    int len;            /**< ͼ���С */

} CAPTURE_BUFFER;

/**
 * @brief ��ȡbt1120����ͼ����Ϣ�Ļص�����
 *
 * @param  [out] img_info_y : ͨ��BT1120��Yͨ�������ͼ����Ϣ
 * @param  [out] img_info_c : ͨ��BT1120��Cͨ�������ͼ����Ϣ
 * @param  [out] sum_y1-sum_y8 : ͨ���߽���������Yֵ�ͣ���ͨ��swpa_vpif_set_y_sum_rows������͵�������
 *                               ���������ͨ����sum_y1��Ӧ��ͨ�������ұ����أ�sum_y2��Ӧ��ͨ������������أ����������Ч
 *                               ������ĸ�ͨ����sum_y1��Ӧ����ͨ�������ұ����أ�sum_y2��Ӧ����ͨ������������أ�
 *                                              sum_y3��Ӧ����ͨ�������±����أ�sum_y4��Ӧ����ͨ�������±����أ�
 *                                              sum_y5��Ӧ����ͨ�������ұ����أ�sum_y6��Ӧ����ͨ������������أ�
 *                                              sum_y7��Ӧ����ͨ�������ϱ����أ�sum_y8��Ӧ����ͨ�������ϱ�����
 *
 * @note .
 * @see ��ˮ��ƽ̨fpga�Ĵ������˵���ĵ���
 */
typedef void (*SWPA_VPIF_IMGINFO_CALLBACK)(
	CAPTURE_IMAGE_INFO_Y * img_info_y, CAPTURE_IMAGE_INFO_C * img_info_c,
	int sum_y1, int sum_y2, int sum_y3, int sum_y4, 
	int sum_y5, int sum_y6, int sum_y7, int sum_y8
);


/**
 * @brief ��ʼ��VPIF
 *
 * @param [in] is_user_mem : ��ʹ���û�������ڴ棬�����ں��Զ�����(1���û��ڴ棬�������ڴ� 0���ں��Զ�����)
 * @param [out] width      : ͼ���
 * @param [out] height     : ͼ���
 * @retval 0 : ִ�гɹ�
 * @retval -1 : ִ��ʧ��
 * @retval -2 : �����Ƿ�
 *
 * @note
 * - ���Ϊ�û������ڴ棬��swpa_capture_start֮ǰ��Ҫ��ͨ��swpa_capture_put_buffer�ṩ����õ��ڴ�
 *
 */
int swpa_capture_init(
	int is_user_mem, 
	int* width, 
	int* height
);


/**
 * @brief ��ʼ�ɼ�
 *
 *
 * @retval 0 : ִ�гɹ�
 * @retval -1 : ִ��ʧ��
 */
int swpa_capture_start(void);



/**
 * @brief  ֹͣ�ɼ�
 *
 *
 * @retval 0 : ִ�гɹ�
 * @retval -1 : ִ��ʧ��
 *
 */
int swpa_capture_stop(void);


/**
 * @brief ��ȡͼ�����ݺ�ͼ�����
 *
 * @param [out] img_buffer : ָ��ͼ�����ݴ洢�ṹ��ָ���ָ��
 * @param [out] img_info_y : ָ��ͼ��Yͨ����Ϣ�洢�ṹ��ָ���ָ�룬ָ�����img_buffer�ڲ�ƫ����
 * @param [out] img_info_c : ָ��ͼ��Cͨ����Ϣ�洢�ṹ��ָ���ָ�룬ָ�����img_buffer�ڲ�ƫ����
 * @retval 0 : ִ�гɹ�
 * @retval -1 : ִ��ʧ��
 * @retval -2 : �����Ƿ�
 * @retval -6 : ������ʱ
 * @note �ڲ������swpa_vpif_set_imginfo_callbackע��Ļص���������Ҫ�ȴ��ûص�������ɺ���ܷ���
 *       ������ʽ���ʣ�����ȡ����ͼ��ʱ����������ೢ��15��(��15*10ms = 150ms)���󷵻�-6
 */
int swpa_capture_get_buffer(
	CAPTURE_BUFFER* img_buffer,
	CAPTURE_IMAGE_INFO_Y ** img_info_y,
	CAPTURE_IMAGE_INFO_C ** img_info_c
);



/**
 * @brief �Ż�ͼ������
 *
 * @param  [in] img_buffer : ͼ�����ݽṹ��ָ��
 *
 * @retval 0 : ִ�гɹ�
 * @retval -1 : ִ��ʧ��
 * @retval -2 : �����Ƿ�
 *
 * @note
 *         1����������2�����ã������ڴ桢�Ż�ͼ������\n
 *         �����ڴ���Ҫѭ�����ñ���������Ҫ����5������������Ҫ����5�α�������\n
 *         ���ʹ���ں��ڴ棬img_buffer��ΪNULL
 */
int swpa_capture_put_buffer(
	CAPTURE_BUFFER* img_buffer
);



/**
 * @brief �ͷ���Դ���ر��豸
 *
 *
 * @retval 0 : ִ�гɹ�
 * @retval -1 : ִ��ʧ��
 *
 */
int swpa_capture_release(
	void
);

/**
 * @brief ���û�ȡbt1120����ͼ����Ϣ�Ļص�����
 *
 * @param  [in] func : ��ȡͼ����Ϣ�Ļص��������˻ص���������swpa_capture_get_bufferȡ����ͼ������ʱ������
 *
 * @retval 0 : ִ�гɹ�
 * @retval -1 : ִ��ʧ��
 * @retval -4 : �ڴ治��
 * @note ����ע�����ص�����
 *       �ص�������Ӧ��ռ��̫��ʱ�䣬����ᵼ��get_buffer����ס
 *       img_info_y��img_info_cָ��ֻ�ڻص������ڲ���Ч���������������ʣ�������ݿ������ṹ�建��
 * @see .
 */
int swpa_vpif_set_imginfo_callback(SWPA_VPIF_IMGINFO_CALLBACK func);

/**
 * @brief ɾ����ȡbt1120����ͼ����Ϣ�Ļص�����
 *
 * @param  [in] func : Ҫɾ����ͼ����Ϣ�ص�����
 *
 * @retval 0 : ִ�гɹ�
 * @retval -1 : ִ��ʧ��
 * @note ���ԭ��û�����ù���func����ͬ������0
 * @see .
 */
int swpa_vpif_remove_imginfo_callback(SWPA_VPIF_IMGINFO_CALLBACK func);

/**
 * @brief ����ͨ���߽���������Yֵ��͵��У��У���
 *
 * @param  [in] rows : ����������Χ1-2
 *
 * @retval 0 : ִ�гɹ�
 * @retval -1 : ִ��ʧ��
 * @note Ĭ��ֵΪ2
 * @see .
 */
int swpa_vpif_set_y_sum_rows(
	int rows
);

#ifdef __cplusplus
}
#endif

#endif // _SWPA_VPIF_H_


