#ifndef SWPA_DEVICE_SETTING_H
#define SWPA_DEVICE_SETTING_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * д���к�
 *
 * @param sn [in] : ���к��ַ���
 *
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL �����Ƿ���-2
 *
 * @note

 */
int swpa_device_write_sn(char* sn);

/**
 * �����к�
 *
 * @param sn [out] : ���к��ַ���
  * @param len [in/out] : ���뻺�����Ĵ�С������ʵ��д��ĳ��ȡ�
 *
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL �����Ƿ���-2
 *
 * @note
 *        sn�ɵ����������ڴ棬�����С����Ϊ256Bytes��
 */
int swpa_device_read_sn(char* sn,  unsigned int* len);

/**
 * дIP��ַ�����ء���������
 *
 * @param ip [in] : IP��ַ�ַ���
 * @param netmark [in] : �����ַ���
 * @param gateway [in] : ���������ַ���
 *
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL �����Ƿ���-2
 *
 * @note

 */
int swpa_device_write_ipinfo(char* ip_str, char* netmark_str, char* gateway_str);

/**
 * ��IP��ַ�����ء���������
 *
 * @param ip [out] : IP��ַ�ַ���
 * @param ip_len [in] : IP��ַ�ַ�������
 * @param netmark [out] : �����ַ���
 * @param netmark_len [in] : �����ַ�������
 * @param gateway [out] : ���������ַ���
 * @param gateway_len [in] : ���������ַ�������
 *
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL �����Ƿ���-2
 *
 * @note
 *         1��!!! IP��ַ�Ȼ������ɵ��������룬���뱣֤�㹻��С !!!
 *         2������������Сʱ����ȡ���ַ�������ֻ��ָ����С���磺\n
 *         ָ��IP��ַ������Ϊ5���ֽڣ���ip_strֵΪ��172.����ʹ��ָ��������Ϊ�˱����ڴ����
 */
int swpa_device_read_ipinfo(char* ip_str, int ip_len, char* netmark_str, int netmark_len, char* gateway_str, int gateway_len);

/**
 * ����uboot
 *
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL
 *
 * @note
 *         ����ʱ���˲���\n

 */
int swpa_device_lock_uboot(void);

/**
 * ����uboot
 *
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL
 *
 * @note

 */
int swpa_device_unlock_uboot(void);

/**
 * �ر�(����)linux��Ϣ��ӡ����
 *
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL
 *
 * @note
 *         ����ʱ���˲���
 */
int swpa_device_lock_linuxinfo(void);

/**
 * ����linux��Ϣ��ӡ����
 *
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL
 *
 * @note
 */
int swpa_device_unlock_linuxinfo(void);

/**
 * �ָ�ubootĬ�ϲ���(�����ʹ�ã�����)
 *
 *
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL �����Ƿ���-2
 *
 * @note
 *         ������\n
 *         �������ʱ������uboot��־\n
 *         ���ô˺�������Ҫ�����豸��uboot��Ҫ����2�Ρ���Ϊ�п��Ź���Ӧ�ò������ע�˹��̡�
 */
int swpa_device_restore_defaultinfo();

/**
 * дuboot��������
 *
 * @param bootargs [in] : ���������ַ���
 *
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL �����Ƿ���-2
 *
 * @note

 */
int swpa_device_write_bootargs(char* bootargs);

/**
 * ��uboot��������
 *
 * @param bootargs [out] : ���������ַ���
 * @param len [in] : ���뻺������С��
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL �����Ƿ���-2
 *
 * @note
 *        bootargs�ɵ����������ڴ棬�����С����Ϊ256Bytes��\n
 *        ���len̫С����bootargsֻ��len��С��
 */
int swpa_device_read_bootargs(char* bootargs, unsigned int len);

/**
 * ������ʱ���
 *
 * @param boottime [out] :
 * @param len [in] : ���뻺������С
 *
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL �����Ƿ���-2
 *
 * @note
 *         1��!!!starttable�������ɵ��������룬len���Ϊ2048Byte\n
 *         2������ʱ�����ubootд��Ӧ�ò�ֻ�ܶ�
 */
int swpa_device_read_boottime(char* boottime, unsigned int len);


/**
 * ��������־
 *
 * @param boottime [out] : ������־
 * @param len [in] : ���뻺������С
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL �����Ƿ���-2
 *
 * @note
 *         1��len���Ϊ2048Byte\n
 *         2��������־��ubootд��Ӧ�ò�ֻ�ܶ�
 */
int swpa_device_read_bootlog(char* bootlog, unsigned int len);


/**
 * ������������
 *
 * @param count [out] : ��������
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL
 *
 * @note
 */
int swpa_device_set_bootcount(int count);



/**
 * ��������ϵͳ��������
 *
 * @param count [out] : ��������
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL
 *
 * @note
 */
int swpa_device_set_normal_bootcount(int count);

/**
 * ���ñ���ϵͳ��������
 *
 * @param count [out] : ��������
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL
 *
 * @note
 */
int swpa_device_set_backup_bootcount(int count);



/**
 * ������������ʹ�ܱ�־
 *
 * @param is_enable [in] : ��������ʹ�ܱ�־(0����ֹ 1��ʹ��)
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL
 *
 * @note ֻ�����°汾uboot(v13)�ϴ˺�������Ч��
 */
int swpa_device_set_bootcount_flag(int is_enable);

/**
 * ��ȡ��������ʹ�ܱ�־
 *
 * @param flag [flag] : ��������ʹ�ܱ�־(0����ֹ 1��ʹ��)
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL
 *
 * @note �ھɰ汾uboot�ϣ���ȡ����flagΪ��Чֵ-1��
 */
int swpa_device_get_bootcount_flag(int* flag);

// ���º����Ǹ߼�ʹ�ã�ǰ���ṩ�Ľӿ��Ѿ�����Ӧ�����󣬵����Ǳ�������Ľӿ�
/**
 * ����uboot������ʱʱ��
 *
 * @param delaytime [out] : ��ʱʱ�䣬��λΪ�룬��Χ��[0~30]
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL �����Ƿ���-2
 *
 * @note
 */
int swpa_device_set_bootdelay(int delaytime);


// ---------------------- ����Ӳ�̱�־ START-----------------------------
 /**
 * ����Ӳ�̱�־
 *
 * @param has_hdd [in]   : 1: �豸�ϴ���Ӳ�� 0: �豸��Ӳ��
 *
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note
 */
int swpa_device_set_hdd_flag(int has_hdd);

 /**
 * ����Ӳ�̱�־
 *
 * @param has_hdd [out]   : 1: �豸�ϴ���Ӳ�� 0: �豸��Ӳ�� -1: ���ݶ�ȡ��Ч
 *
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note
 */
int swpa_device_get_hdd_flag(int* has_hdd);


/**
 * ͨ��CPLD����H264�ķֱ���
 *
 * @param resolution_sel [in] : �ֱ���ѡ��0: 720P 1��1080P
 * @return �ɹ���SWPAR_OK  ʧ�ܣ�SWPAR_FAIL �����Ƿ���-2
 *
 * @note
 �Ĵ���0x0c
 01000       720P
 10000       1080P

 */
int swpa_device_set_resolution(int resolution_sel);



/**
* ���ָ�Ӧ�ò�Ĭ�ϲ�����־
*
* @param flag [out] : �ָ�Ĭ�ϲ�����־  0 : ���ָ�Ĭ��  1 : �ָ�Ĭ��
*
* @return �ɹ���0  ʧ�ܣ�-1 
*
*/
int swpa_device_get_default_param_flag(int *flag);


/**
* ����ָ�Ӧ�ò�Ĭ�ϲ�����־
*
* @return �ɹ� : 0  ʧ��:-1 
*/
int swpa_device_clear_default_param_flag(void);


/**
* @brief ��ȡ�����豸����尴���¼�
*
*@param [out] event : �����¼���ȡֵ��:0-���£�1-̧��
* @retval 0 : ִ�гɹ�
* @retval 1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_device_get_reset_key_event(int * event);



/**
 * ������������ϵͳ
 *
 * @param sys_type [in] : ����ϵͳ���� 0����ʽϵͳ  1������ϵͳ
 * @retval SWPAR_OK : ִ�гɹ�
 * @retval SWPAR_FAIL : ִ��ʧ��
 * @note
 */
int swpa_device_set_bootsystem(const int sys_type);



#ifdef __cplusplus
}
#endif

#endif // SWPA_DEVICE_SETTING_H
