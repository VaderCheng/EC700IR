/**
 * @file   drv_uboot.h
 * @author lujy
 * @date   2013-11-14
 *
 * @brief
 *         uboot��Ӧ�ò㹲ͬ�ӿ�
 * @note   ���ļ���װ�������д���кţ�����дIP��ַ������uboot�����ȵȵĽӿڡ�
 *
 * @todo   ���ļ�ʵ���ǵ�����eeprom�����������Ƿ���Ҫ���ļ��ӿڣ���Ҫ����ȷ����
 *
 *
 */
#ifndef DRV_UBOOT_H
#define DRV_UBOOT_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * д���к�
 *
 * @param sn [in] : ���к��ַ���
 *
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note

 */
int drv_device_write_sn(char* sn);

/**
 * �����к�
 *
 * @param sn [out] : ���к��ַ���
  * @param len [in/out] : ���뻺�����Ĵ�С������ʵ��д��ĳ��ȡ�
 *
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note
 *        sn�ɵ����������ڴ棬�����С����Ϊ256Bytes��
 */
int drv_device_read_sn(char* sn,  unsigned int* len);


/**
 * ��MAC��
 *
 * @param mac [out] : MAC���ַ���
  * @param len [in/out] : ���뻺�����Ĵ�С������ʵ��д��ĳ��ȡ�
 *
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note
 *        sn�ɵ����������ڴ棬�����С����Ϊ256Bytes��
 */
int drv_device_read_mac(char* mac,  int* len);

/**
 * дIP��ַ�����ء���������
 *
 * @param ip_str [in] : IP��ַ�ַ���
 * @param netmark_str [in] : �����ַ���
 * @param gateway_str [in] : ���������ַ���
 *
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note

 */
int drv_device_write_ipinfo(char* ip_str, char* netmark_str, char* gateway_str);

/**
 * дIP��ַ
 *
 * @param ip_str [in] : IP��ַ�ַ���
 *
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note

 */
int drv_device_write_ipaddr(char* ip_str);

/**
 * д��������
 *
 * @param netmark_str [in] : ���������ַ�ַ���
 *
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note

 */
int drv_device_write_netmask(char* netmark_str);

/**
 * д����
 *
 * @param gateway_str [in] : ���ص�ַ�ַ���
 *
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note

 */
int drv_device_write_gateway(char* gateway_str);

/**
 * ��IP��ַ�����ء���������
 *
 * @param ip_str [out] : IP��ַ�ַ���
 * @param ip_len [in] : IP��ַ�ַ�������
 * @param netmark_str [out] : �����ַ���
 * @param netmark_len [in] : �����ַ�������
 * @param gateway_str [out] : ���������ַ���
 * @param gateway_len [in] : ���������ַ�������
 *
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note
 *         1��!!! IP��ַ�Ȼ������ɵ��������룬���뱣֤�㹻��С !!!
 *         2������������Сʱ����ȡ���ַ�������ֻ��ָ����С���磺\n
 *         ָ��IP��ַ������Ϊ5���ֽڣ���ip_strֵΪ��172.����ʹ��ָ��������Ϊ�˱����ڴ����
 */
int drv_device_read_ipinfo(char* ip_str, int ip_len, char* netmark_str, int netmark_len, char* gateway_str, int gateway_len);

/**
 * ����uboot
 *
 * @return �ɹ���0  ʧ�ܣ�-1 
 *
 * @note
 *         ����ʱ���˲���\n

 */
int drv_device_lock_uboot(void);

/**
 * ����uboot
 *
 * @return �ɹ���0  ʧ�ܣ�-1 
 *
 * @note
 *         �з�����ʱҪ����uboot
 */
int drv_device_unlock_uboot(void);

/**
 * �ر�(����)linux��Ϣ��ӡ����
 *
 * @return �ɹ���0  ʧ�ܣ�-1 
 *
 * @note
 *         ����ʱ���˲���
 */
int drv_device_lock_linuxinfo(void);

/**
 * ����linux��Ϣ��ӡ����
 *
 * @return �ɹ���0  ʧ�ܣ�-1 
 *
 * @note
 */
int drv_device_unlock_linuxinfo(void);

/**
 * �ָ�ubootĬ�ϲ���(�����ʹ�ã�����)
 *
 *
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note
 *         ������\n
 *         <b>�������ʱ������uboot��־</b>\n
 *         <b>���ô˺�������Ҫ�����豸��uboot��Ҫ����2�Ρ���Ϊ�п��Ź���Ӧ�ò������ע�˹��̡�</b>
 */
int drv_device_restore_defaultinfo(void);

/**
 * ������ʱ���
 *
 * @param boottime [out] :
 * @param len [in] : ���뻺������С
 *
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note
 *         1��!!!starttable�������ɵ��������룬len���Ϊ4096Byte\n
 *         2������ʱ�����ubootд��Ӧ�ò�ֻ�ܶ�
 */
int drv_device_read_boottime(char* boottime, unsigned int len);

/**
 * ���豸��������
 *
 * @param count [out] :������
 *
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 */
int drv_device_read_boottime_count(int* count);

/**
 * ��������־
 *
 * @param bootlog [out] : ������־
 * @param len [in] : ���뻺������С
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note
 *         1��len���Ϊ4096Byte\n
 *         2��������־��ubootд��Ӧ�ò�ֻ�ܶ�
 */
int drv_device_read_bootlog(char* bootlog, unsigned int len);

/**
 * ������ʽϵͳ����������
 *
 * @param count [out] : ��������
 * @return �ɹ���0  ʧ�ܣ�-1 
 *
 * @note
 */
int drv_device_set_normal_bootcount(int count);

/**
 * ���ñ���ϵͳ����������
 *
 * @param count [out] : ��������
 * @return �ɹ���0  ʧ�ܣ�-1 
 *
 * @note
 */
int drv_device_set_backup_bootcount(int count);

/**
 * ������������ʹ�ܱ�־
 *
 * @param is_enable [in] : ��������ʹ�ܱ�־(0����ֹ 1��ʹ��)
 * @return �ɹ���0  ʧ�ܣ�-1 
 *
 * @note
 */
int drv_device_set_bootcount_flag(int is_enable);

/**
 * ��ȡ��������ʹ�ܱ�־
 *
 * @param flag [out] : ��������ʹ�ܱ�־(0����ֹ 1��ʹ��)
 * @return �ɹ���0  ʧ�ܣ�-1 
 *
 * @note
 */
int drv_device_get_bootcount_flag(int* flag);

/**
 * ��ȡ��������ʹ�ܱ�־
 *
 * @param type [in] : ����ϵͳ���� 0����ʽϵͳ  1������ϵͳ
 * @return �ɹ���0  ʧ�ܣ�-1 
 *
 * @note
 */
int drv_device_set_bootsystem(int type);

////////////////////////////////////////////

// ���º����Ǹ߼�ʹ�ã���ʹ�������ã�����

/**
 * дuboot��������
 *
 * @param bootargs [in] : ���������ַ���
 *
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note
 */
int drv_device_write_bootargs(char* bootargs);

/**
 * ��uboot��������
 *
 * @param bootargs [out] : ���������ַ���
 * @param len [in] : ���뻺������С��
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note
 *        bootargs�ɵ����������ڴ棬�����С����Ϊ512Bytes��\n
 *        ���len̫С����bootargsֻ��len��С��
 */
int drv_device_read_bootargs(char* bootargs, unsigned int len);

/**
 * ����uboot������ʱʱ��
 *
 * @param delaytime [out] : ��ʱʱ�䣬��λΪ�룬��Χ��[0~30]
 * @return �ɹ���0  ʧ�ܣ�-1  �����Ƿ���-2
 *
 * @note
 */
int drv_device_set_bootdelay(int delaytime);

/**
* ���ָ�Ӧ�ò�Ĭ�ϲ�����־
*
* @param flag [out] : �ָ�Ĭ�ϲ�����־  0 : ���ָ�Ĭ��  1 : �ָ�Ĭ��
*
* @return �ɹ���0  ʧ�ܣ�-1 
*
*/
int drv_device_get_default_param_flag(int *flag);

/**
* ����ָ�Ӧ�ò�Ĭ�ϲ�����־
*
* @return �ɹ� : 0  ʧ��:-1 
*/
int drv_device_clear_default_param_flag(void);


#ifdef __cplusplus
}
#endif

#endif // DRV_UBOOT_H
