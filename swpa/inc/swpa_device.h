/**
* @file  swpa_device.h   
* @brief  �豸ƽ̨���
* @author  
* @date  2013-2-20
* @version  V0.1
* @note     
*/ 


#ifndef _SWPA_DEVICE_H_
#define _SWPA_DEVICE_H_

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum
{
	SWPA_DEVICE_CAMERA_FPGA = 0,
	SWPA_DEVICE_DM368 ,
	SWPA_DEVICE_USB,
	
	SWPA_DEVICE_COUNT	
} SWPA_DEVICE_TYPE;



/**
* @brief ʹ�ܿ��Ź���ͬʱ���ù���ʱʱ��
*
* 
* @param [in] timeout_ms ���õĳ�ʱʱ�䣬��λΪ���루ms������ϸ˵���뿴ע������
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @note 
* - timeout_ms���Ϊ0����ʱʱ��Ϊ�ײ�Ĭ��ֵ��2000 ms��
* - timeout_ms�������0����Ϊ�����õ�ʱ��
* - timeout_ms���С��0����Ϊ���޵ȴ�
* @see .
*/
int swpa_watchdog_enable(	
	int 	timeout_ms
);


/**
* @brief �رտ��Ź�
*
* 
* @param .
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @note .
* @see .
*/
int swpa_watchdog_disable(	
	void
);


/**
* @brief ���Ź�����
*
* 
* @param .
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @note .
* @see .
*/
int swpa_watchdog_handshake(	
	void
);


/**
* @brief �ж�DM368�Ƿ��ѿ����ӡ�
*
* 
* @retval 0 : ������
* @retval -1 : ��������
* @note .
* @see SWPA_DEVICE_TYPE.
*/
int swpa_device_dm368_ready();


/**
* @brief ��λ�����豸
*
* ���������ָ����豸�����������������Ҫ���豸��λ����ʱ�����Ե��øú�����ɸ�λ�豸��
* �첽�������̰߳�ȫ��
*
* 
* @param [in] dev_type ָ��slave�豸���࣬ȡֵ��Χ��Camera FPGA��DM368�� USB�豸
* @retval 0 : ִ�гɹ�
* @retval -1 : ִ��ʧ��
* @retval -2 : �����Ƿ�
* @note .
* @see SWPA_DEVICE_TYPE.
*/
int swpa_device_reset(	
  SWPA_DEVICE_TYPE dev_type
);


/**
* @brief ���ظ����豸�����ļ�
*
* 
* @param [in] dev_type ָ���豸���࣬Ŀǰȡֵ��Χ��Camera FPGA��DM368�� USB�豸��
* @param [in] filename ָ���豸Ҫ���صľ���·�����ļ���
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note 
* - USB�豸û�о����ļ�����˲��ܴ�����豸����.
* @see SWPA_DEVICE_TYPE.
*/
int swpa_device_load(	
  SWPA_DEVICE_TYPE dev_type,
  char * filename
);



/**
* @brief ��ʼ������
*
* 
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @note 
* - Ŀǰˮ��ƽ̨û�з��������ʸ�API���գ�ֱ��return -1.
* @see .
*/
	
int swpa_buzzer_on(	
	void
);


/**
* @brief ֹͣ������
*
* 
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @note .
* - Ŀǰˮ��ƽ̨û�з��������ʸ�API���գ�ֱ��return -1.
* @see .
*/
	
int swpa_buzzer_off(	
	void	
);



/**
* @brief ����LED�Ƶ���
*
* 
* @param .
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @note .
* @see .
*/	
int swpa_heart_led_on(	
	void
);


/**
* @brief ����LED��Ϩ��
*
* 
* @param .
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @note .
* @see .
*/
	
int swpa_heart_led_off(
	void
);



/**
* @brief ����LED�Ƶ���
*
* 
* @param .
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @note .
* @see .
*/	
int swpa_alarm_led_on(	
	void
);


/**
* @brief ����LED��Ϩ��
*
* 
* @param .
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @note .
* @see .
*/
	
int swpa_alarm_led_off(
	void
);


/**
* @brief Ӳ��LED�Ƶ���
*
* 
* @param .
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @note .
* @see .
*/	
int swpa_hdd_led_on(	
	void
);


/**
* @brief Ӳ��LED��Ϩ��
*
* 
* @param .
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @note .
* @see .
*/
	
int swpa_hdd_led_off(
	void
);



/**
* @brief ��������LED�Ƶ���
*
* 
* @param .
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @note .
* @see .
*/	
int swpa_network_led_on(	
	void
);


/**
* @brief ��������LED��Ϩ��
*
* 
* @param .
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @note .
* @see .
*/
	
int swpa_network_led_off(
	void
);




/**
* @brief ��ȡCPU�¶�
*
* 
* @param [out] temperature  ��ȡ�����¶�ֵ
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note 
* - ��ˮ��ƽ̨����CPU���¸��豸���ʸ�APIʵ��Ϊ�գ�ֱ�ӷ���-1��
* @see .
*/
	
int swpa_info_get_cpu_temperature(	
	int * temperature
);



/**
* @brief ��ȡ�����¶�
*
* 
* @param [out] temperature  ��ȡ�����¶�ֵ
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note .
* @see .
*/	
int swpa_info_get_env_temperature(	
	int * temperature
);




/**
* @brief ��ȡ��ǰ��λģʽ
*
* 
* @param [out] pmode  ��ȡ���ĸ�λģʽ���Ϸ�ֵ��:
* - 0 : ��������ģʽ
* - 1 : ��������ģʽ
* - 2 : ��������ģʽ
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ��ȡ��λģʽʧ��
* @note .
* @see .
*/	
int swpa_device_get_resetmode(int* pmode);



/**
* @brief ���õ�ǰ��λģʽ
*
* 
* @param [out] pmode  ��Ҫ���õĸ�λģʽ���Ϸ�ֵ��:
* - 0 : ��������ģʽ
* - 1 : ��������ģʽ
* - 2 : ��������ģʽ
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ���ø�λģʽʧ��
* @note .
* @see .
*/	
int swpa_device_set_resetmode(const int mode);





/**
* @brief ��ʼ������оƬ
* 
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  �ײ�ִ��ʧ��
* @note .
* @see .
*/	
int swpa_device_crypt_init();



/**
 *
 * @brief ����д
 *
 * @param addr [in]    : ��ַ
 * @param data [out]   : ����
 * @param len [in/out] : ����(������lenΪд���ʵ�ʳ���)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 * ֻ���ڳ�ʼ�����ܹ���(swpa_device_crypt_init)֮�����!!!
 */
int swpa_device_crypt_write(unsigned int addr, unsigned char* data, unsigned int* len);


/**
 *
 * @brief ���ܶ�
 *
 * @param addr [in]    : ��ַ
 * @param data [out]   : ����
 * @param len [in/out] : ����(������lenΪд���ʵ�ʳ���)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 * ֻ���ڳ�ʼ�����ܹ���(swpa_device_crypt_init)֮�����!!!
 */
int swpa_device_crypt_read(unsigned int addr, unsigned char* data, unsigned int* len);




/**
 * ��д����оƬ(��NC��KEY)
 *
 * @param nc [in]       : NCֵ
 * @param key [in]      : ֻ������KEY
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *       1����˿λ���˺󣬲����ٵ��ô˺�����д����
 *       2��nc����Ϊ7��key����Ϊ16
 */
int swpa_device_init_crypt_config(unsigned char* nc, unsigned char* key);




/**
* @brief ��ȡ��ǰ����оƬ��˿״̬
* 
* @param [out] pvalue  ��ȡ������˿״̬���Ϸ�ֵ��:
* - 0 : �Ѿ��۶�
* - ��0 : ��δ�۶�
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  �ײ�ִ��ʧ��
* @note .
* @see .
*/	
int swpa_device_get_fuse_status(unsigned char* pvalue);




/**
* @brief �նϼ���оƬ��˿
* 
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  �ײ�ִ��ʧ��
* @note .
* @see .
*/	
int swpa_device_fuse_write(void);





/**
* @brief ��ȡ�̼��汾��Ϣ
*
* 
* @param [in] version ��Ű汾��Ϣ���ַ���ָ��
* @param [inout] len ����Ԥ�ȷ����version�Ŀռ��С������ʵ�ʻ�ȡ�����ַ����Ĵ�С
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note 
* - �ú�����uboot��kernel��ubl��camera fpga�� cpld�İ汾��Ϣȫ����Ϸ���version�ִ���
* - �ִ���ÿ��ģ��汾��Ϣ�İڷŽṹ�ǡ�ģ������ģ��汾�ִ�[���з�]�����硰uboot:vxxxxx\n��
* - ���versionΪNULL����lenС����ʵ�汾��Ϣ�ĳ��ȣ��ú���ͨ��len����version�ִ�����ʵ���ȣ�versionָ����ڴ����ݲ��䡣
* @see .
*/
int swpa_info_get_firmware_version(
	char * version,
	int * len
);



/**
* @brief ��ȡӲ������汾
*
* 
* @param [out] version ��Ű汾�ַ���
* @param [inout] len ����version�������Ĵ�С��������ʵ��version�ִ��ĳ���
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note .
* @see .
*/	
int swpa_info_get_hardware_version(	
	char * version,
	int * len
);


/**
* @brief ��ȡ�豸���б��
*
* 
* @param [out] serial_no ����豸���б���ַ���
* @param [inout] len ����serial_no�������Ĵ�С��������ʵ��serial_no�ִ��ĳ���
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note .
* @see .
*/	
int swpa_info_get_device_serial_no(	
	char * serial_no,
	int * len
);

/**
 *@brief ע��ص�����,����dsp֪ͨarm
 *@param [in] dsp_alarm �ص�����
                        puser �û��Զ������
                        info  dsp֪ͨarm����Ϣ
 */
int swpa_device_register_callback( void (*dsp_alarm)(void *puser, int info), void *puser);

/**
 *@brief ע��ص�����,����dsp֪ͨarm
 *@param [in] dsp_alarm �ص�����
                        puser �û��Զ������
                        info  dsp֪ͨarm����Ϣ
 */
int swpa_device_register_callback_venus( void (*dsp_alarm)(void *puser, int info), void *puser);

/**
* @brief ��ȡ��λ���Ƿ񳤰�5S�¼���
*
* @retval 0 : ���¼�
* @retval 1 :  ���¼�
* @note .
* @see .
*/	
int swpa_device_get_reset_flag();



/**
* @brief ��ȡSD���Ƿ�����豸��
*
* @param [out] status SD������״̬��0���ѽ��룻1�����Ƴ�
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note .
* @see .
*/	
int swpa_device_get_sdcard_status(int* status);

/**
* @brief ����485����
*
* @param [in] dir ����״̬��0�����룻1�����
* @retval 0 : ִ�гɹ�
* @retval -1 :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note ���ǡ�ľ��ƽֻ̨��һ��485���ʲ���ӡ��豸���͡�
* @see .
*/	
int swpa_device_set_rs485(int dir);

/**
* @brief ���³�ʼ��SD��
*
* @retval 0 : ��ʼ���ɹ�
* @retval -1 : ��ʼ��ʧ��
* @note .
* @see .
*/	
int swpa_device_sd_reinit();

/**
* @brief ��ȡFPGA�Ĵ���
*
* @retval 0 : ��ȡ�ɹ�
* @retval -1 : ��ȡʧ��
* @note .
* @see .
*/	
int swpa_fpga_reg_read(unsigned int addr,unsigned int *value);

/**
* @brief д����FPGA�Ĵ���
*
* @retval 0 : д��ɹ�
* @retval -1 : д��ʧ��
* @note .
* @see .
*/																	
int swpa_fpga_reg_write(unsigned int addr,unsigned int value);

/**
* @brief д���FPGA�Ĵ���
*	count д��Ĵ������������ܴ���MAX_FPGA_REG
*	addr  д��Ĵ�����ַ����
*	value д��Ĵ���ֵ���飬�±����ַ�����Ӧ
* @retval 0 : д��ɹ�
* @retval -1 : д��ʧ��
* @note .
* @see .
*/																	
int swpa_fpga_multi_reg_write(int count, unsigned int *addr,unsigned int *value);

/**
* @brief DC��Ȧ����
*	duty Ϊռ�ձȣ���λΪǧ��֮һ����duty_valΪ900��ʾǧ��֮�Ű�\n
* @retval 0 : д��ɹ�
* @retval -1 : д��ʧ��
* @note .
* @see .
*/	
int spwa_autoiris_pwm(int duty);


#ifdef __cplusplus
}
#endif

#endif // _SWPA_DEVICE_H_


