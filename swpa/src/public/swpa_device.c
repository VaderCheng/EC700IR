/**
* @file  swpa_device.c  
* @brief  ʵ���豸ƽ̨��غ���
* @author  
* @date  2013-2-20
* @version  V1.0
* @note     
*/ 

#include "swpa_private.h"
#include "drv_device.h"
#include <signal.h>
#include <fcntl.h>
#include<sys/file.h>



#ifdef SWPA_DEVICE
#define SWPA_DEVICE_PRINT(fmt, ...) SWPA_PRINT("[%s:%d]"fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define SWPA_DEVICE_CHECK(arg)      {if (!(arg)){SWPA_PRINT("[%s:%d]Check failed : %s [%d]\n", __FILE__, __LINE__, #arg, SWPAR_INVALIDARG);return SWPAR_INVALIDARG;}}
#else
#define SWPA_DEVICE_PRINT(fmt, ...)
#define SWPA_DEVICE_CHECK(arg)
#endif

//LED�豸ID����
#define HEART_LED_ID      DEVID_LED_GREEN //��������ָʾ��
#define ALARM_LED_ID      DEVID_LED_RED //�����쳣������
#define HDD_LED_ID        0xff //Ӳ�̹���ָʾ��,Ŀǰδ����
#define NETWORK_LED_ID    0xff //��������ָʾ��,Ŀǰδ����


//FPGA�豸ID����
#define CAMERA_FPGA_ID    DEVID_SPI_FPGA


//CPLD�豸ID����
#define CPLD_ID    DEVID_SPI_CPLD



//�¶ȴ������豸ID����
#define ENV_TEMPERATURE_ID	0



/**
* @brief ʹ�ܿ��Ź���ͬʱ���ù���ʱʱ��
*
* 
* @param [in] timeout_ms ���õĳ�ʱʱ�䣬��λΪ���루ms������ϸ˵���뿴ע������
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL : ִ��ʧ��
* @note 
* - timeout_ms���Ϊ0����ʱʱ��Ϊ�ײ�Ĭ��ֵ��2000 ms��
* - timeout_ms�������0����Ϊ�����õ�ʱ��
* - timeout_ms���С��0����Ϊ���޵ȴ�
* @see .
*/
int swpa_watchdog_enable(	
	int 	timeout_ms
)
{
	if (0 == drv_watchdog_disable()) //disable the watchdog first
	{
		return drv_watchdog_enable(timeout_ms);
	}
	else
	{
		return SWPAR_FAIL;
	}
}



/**
* @brief �رտ��Ź�
*
* 
* @param .
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @note .
* @see .
*/
int swpa_watchdog_disable(	
	void
)
{
	return drv_watchdog_disable();
}



/**
* @brief ���Ź�����
*
* 
* @param .
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @note .
* @see .
*/
int swpa_watchdog_handshake(	
	void
)
{
	return drv_watchdog_handshake();
}


#ifdef DM6467
/**
* @brief �ж�DM368�Ƿ��ѿ����ӡ�
*
* 
* @retval 0 : ������
* @retval -1 : ��������
* @note .
* @see SWPA_DEVICE_TYPE.
*/


// 6467��368 IOͨ�Ų���
// д
static int set_gpio_to_dm368(int data)
{
	return drv_gpio_set_output(DEVID_GPIO, 6, data & 0x1);
}
// ��
int get_gpio_from_dm368(int* value)
{
	return drv_gpio_get_input(DEVID_GPIO, 5, value);
}

int swpa_device_dm368_ready()
{
	int value = 0;
	int data = 1;
	int trytimes = 0;
	int ok_count = 0;
	int old_value = 0;

	while (1)
	{
		// �����16��
		if (trytimes++ == 16)
		{
			printf("swpa_device_dm368_ready(): reach max try times. ok time: %d\n", ok_count);
			return -1;
		}
		// ����10�γɹ�ʱ����˵������ĳɹ�����ֹIO��ƽ������ɵ�����
		if (ok_count == 10)
		{
			return 0;
		}

		// ����6��IO
		set_gpio_to_dm368(data);
		swpa_thread_sleep_ms(5);
		// ��5��IO
		get_gpio_from_dm368(&value);
		value &= 0x01;

		//printf("set value: %d get value: %d old value: %d\n", data & 0x1, value, old_value);

		// ֵ�����仯��˵�����ֳɹ�
		if (old_value != value)
		{
			old_value = value;
			ok_count++;
		}
		else
		{
			ok_count = 0;
		}

		data = (~data) & 0x01;
		swpa_thread_sleep_ms(10);
	}

	return 0;
}
#endif



/**
* @brief ��λ�����豸
*
* ���������ָ����豸�����������������Ҫ���豸��λ����ʱ�����Ե��øú�����ɸ�λ�豸��
* �첽�������̰߳�ȫ��
*
* 
* @param [in] dev_type ָ��slave�豸���࣬ȡֵ��Χ��Camera FPGA��DM368�� USB�豸
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL : ִ��ʧ��
* @retval -2 : �����Ƿ�
* @note .
* @see SWPA_DEVICE_TYPE.
*/
int swpa_device_reset(	
	SWPA_DEVICE_TYPE dev_type
)
{
	SWPA_DEVICE_PRINT("dev_type=%d\n", dev_type);

	switch (dev_type)
 	{
		case SWPA_DEVICE_CAMERA_FPGA:
			return drv_device_reset(CAMERA_FPGA_ID);
		//break;
    
		case SWPA_DEVICE_DM368:
			return drv_device_reset(DEVID_DM368);
		//break;
    
		case SWPA_DEVICE_USB:
			return drv_device_reset(DEVID_USB);
		//break;
    
		default:
			SWPA_DEVICE_PRINT("Err: dev_type (%d) is invalid! [%d]\n", dev_type, SWPAR_INVALIDARG);
			return SWPAR_INVALIDARG;
		//break;
	}
}




/**
* @brief ���������FPGA�����ļ�����ʼ��
*
* 
* @param [in] filename ָ�������FPGA����·�����ļ���
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @retval SWPAR_OUTOFMEMORY :  �ڴ治��
*/
static int _load_camera_fpga(char * filename)
{
	int ret = 0;
 	int fd = 0;
	unsigned char * buf = NULL;
	int len = 0, readlen = 0;
	
	SWPA_DEVICE_CHECK(NULL != filename);

	fd = swpa_file_open(filename, "r"); //swpa_file������һ�㲻�ã��޷������ļ���֪���ļ��洢�ں��ֽ����ϣ����µ���swpa_file_openʱ��֪����δ�mode��������ʱ�ٶ�fpga���ļ�����flash��
	if (0 >= fd && fd > SWPAR_MIN)
	{
		SWPA_DEVICE_PRINT("Err: failed to open %s! [%d]\n", filename, SWPAR_FAIL);
		return SWPAR_FAIL;
	}

	ret = swpa_file_seek(fd, 0, SWPA_SEEK_END);
	if (SWPAR_OK != ret)
	{
		SWPA_DEVICE_PRINT("Err: failed to seek %s! [%d]\n", filename, SWPAR_FAIL);
		ret = SWPAR_FAIL;
		goto _ERR_HANDLING;
	}

	len = swpa_file_tell(fd);
	if (0 >= len)
	{
		SWPA_DEVICE_PRINT("Err: failed to tell %s! [%d]\n", filename, SWPAR_FAIL);
		ret = SWPAR_FAIL;
		goto _ERR_HANDLING;
	}

	buf = (unsigned char*)swpa_mem_alloc(len*sizeof(unsigned char));
	if (NULL == buf)
	{
		SWPA_DEVICE_PRINT("Err: no enough memory for buf! [%d]\n",  SWPAR_OUTOFMEMORY);
		
		ret = SWPAR_OUTOFMEMORY;
		goto _ERR_HANDLING;
	}

	ret = swpa_file_seek(fd, 0, SWPA_SEEK_SET);
	if (SWPAR_OK != ret)
	{
		SWPA_DEVICE_PRINT("Err: failed to seek %s! [%d]\n", filename, SWPAR_FAIL);
		ret = SWPAR_FAIL;
		goto _ERR_HANDLING;
	}
	
	ret = swpa_file_read(fd, buf, len, &readlen);
	if (SWPAR_OK != ret || len != readlen)
	{
		SWPA_DEVICE_PRINT("Err: failed to read %s! [%d]\n", filename, SWPAR_FAIL);
		ret = SWPAR_FAIL;
		goto _ERR_HANDLING;
	}      

	ret = drv_fpga_load(CAMERA_FPGA_ID, buf, len);
	if (0 != ret)
	{
		SWPA_DEVICE_PRINT("Err: drv_fpga_load() failed (%d)! [%d]\n", ret, SWPAR_FAIL);
		ret = SWPAR_FAIL;
		goto _ERR_HANDLING;
	}

	swpa_file_close(fd);
	
	ret = SWPAR_OK;
	
_ERR_HANDLING:
	if (NULL != buf)
	{
		swpa_mem_free(buf);
		buf = NULL;
	}

	swpa_file_close(fd);

	return ret;
}




/**
* @brief ���ظ����豸�����ļ�
*
* 
* @param [in] dev_type ָ���豸���࣬Ŀǰȡֵ��Χ��Camera FPGA��
* @param [in] filename ָ���豸Ҫ���صľ���·�����ļ���
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note 
* - USB�豸û�о����ļ�����˲��ܴ�����豸����.
* @see SWPA_DEVICE_TYPE.
*/
int swpa_device_load(	
  SWPA_DEVICE_TYPE dev_type,
  char * filename
)
{
	SWPA_DEVICE_CHECK(NULL != filename);
	SWPA_DEVICE_CHECK(0 != swpa_strcmp(filename, ""));

	SWPA_DEVICE_PRINT("filename=%s\n", filename);
	SWPA_DEVICE_PRINT("dev_type=%d\n", dev_type);
	
	switch (dev_type)
	{
		case SWPA_DEVICE_CAMERA_FPGA:
			return _load_camera_fpga(filename);
	    //break;
	    
		case SWPA_DEVICE_DM368:
	    //break;	    
		case SWPA_DEVICE_USB:
	    //break;    
		default:
			SWPA_DEVICE_PRINT("Err: dev_type (%d) is invalid! [%d]\n", dev_type, SWPAR_INVALIDARG);
			return SWPAR_INVALIDARG;
		//break;  
	}
}





/**
* @brief ��ʼ������
*
* 
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @note 
* - Ŀǰˮ��ƽ̨û�з��������ʸ�API���գ�ֱ��return SWPAR_NOTIMPL.
* @see .
*/	
int swpa_buzzer_on(	
	void
)
{
    return SWPAR_NOTIMPL;
}




/**
* @brief ֹͣ������
*
* 
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @note .
* - Ŀǰˮ��ƽ̨û�з��������ʸ�API���գ�ֱ��return SWPAR_NOTIMPL.
* @see .
*/
	
int swpa_buzzer_off(	
	void	
)
{
    return SWPAR_NOTIMPL;
}




/**
* @brief ����LED�Ƶ���
*
* 
* @param .
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @note .
* @see .
*/	
int swpa_heart_led_on(	
	void
)
{
    return drv_led_on(HEART_LED_ID);
}




/**
* @brief ����LED��Ϩ��
*
* 
* @param .
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @note .
* @see .
*/
	
int swpa_heart_led_off(
	void
)
{
    return drv_led_off(HEART_LED_ID);
}




/**
* @brief ����LED�Ƶ���
*
* 
* @param .
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @note .
* @see .
*/	
int swpa_alarm_led_on(	
	void
)
{
    return drv_led_on(ALARM_LED_ID);
}




/**
* @brief ����LED��Ϩ��
*
* 
* @param .
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @note .
* @see .
*/
	
int swpa_alarm_led_off(
	void
)
{
    return drv_led_off(ALARM_LED_ID);
}



/**
* @brief Ӳ��LED�Ƶ���
*
* 
* @param .
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @note .
* @see .
*/	
int swpa_hdd_led_on(	
	void
)
{
    return drv_led_on(HDD_LED_ID);
}




/**
* @brief Ӳ��LED��Ϩ��
*

* 
* @param .
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @note .
* @see .
*/
	
int swpa_hdd_led_off(
	void
)
{
    return drv_led_off(HDD_LED_ID);
}




/**
* @brief ��������LED�Ƶ���
*
* 
* @param .
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @note .
* @see .
*/	
int swpa_network_led_on(	
	void
)
{
    return drv_led_on(NETWORK_LED_ID);
}




/**
* @brief ��������LED��Ϩ��
*
* 
* @param .
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @note .
* @see .
*/




int swpa_network_led_off(
	void
)
{
    return drv_led_off(NETWORK_LED_ID);
}





/**
* @brief ��ȡCPU�¶�
*
* 
* @param [out] temperature  ��ȡ�����¶�ֵ
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note 
* - ��ˮ��ƽ̨����CPU���¸��豸���ʸ�APIʵ��Ϊ�գ�ֱ�ӷ���-1��
* @see .
*/
	
int swpa_info_get_cpu_temperature(	
	int * temperature
)
{
    return drv_device_get_temperature(DEVID_IIC_LM75, temperature);
}





/**
* @brief ��ȡ�����¶�
*
* 
* @param [out] temperature  ��ȡ�����¶�ֵ
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note .
* @see .
*/	
int swpa_info_get_env_temperature(	
	int * temperature
)
{
    return drv_device_get_temperature(ENV_TEMPERATURE_ID, temperature);
}



#define WORK_INFO_FLAG 0xFFEE0001
#define WORK_INFO_LEN 16

void swpa_device_init_resetmode()
{
	int fd = 0;
	fd = swpa_file_open("EEPROM/0/WORKINGMODE_INFO", "w+");
	if (fd <= 0 && fd > SWPAR_MIN)
	{
		SWPA_DEVICE_PRINT("Err: failed to open %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
	}
	else
	{
		unsigned char rgbBuffer[WORK_INFO_LEN];
		swpa_memset(rgbBuffer, 0, WORK_INFO_LEN);
		unsigned int iflag = WORK_INFO_FLAG;
		swpa_memcpy(rgbBuffer, &iflag, sizeof(iflag));
		unsigned int crc = 0;
		crc = swpa_utils_calc_crc32(0, rgbBuffer, WORK_INFO_LEN - sizeof(crc));
		swpa_memcpy(rgbBuffer + (sizeof(int) * 3), &crc, sizeof(crc));
		int ret_size = 0;
		swpa_file_seek(fd, SWPA_SEEK_SET, 0);
		if (SWPAR_OK != swpa_file_write(fd, rgbBuffer, WORK_INFO_LEN, &ret_size)
			|| WORK_INFO_LEN != ret_size)
		{
			SWPA_DEVICE_PRINT("Err: failed to write %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
		}
		swpa_file_close(fd);
	}
}
/**
* @brief ��ȡ��ǰ����ģʽ
*
* 
* @param [out] pmode  ��ȡ���Ĺ���ģʽ���Ϸ�ֵ��:
* - 0 : ��������ģʽ
* - 1 : ��������ģʽ
* - 2 : ��������ģʽ
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ��ȡ����ģʽʧ��
* @note .
* @see .
*/	
int swpa_device_get_resetmode(int* pmode)
{
	SWPA_DEVICE_CHECK(NULL != pmode);

	*pmode = 0; //Normal Mode by default
	
	int fd = 0;
	fd = swpa_file_open("EEPROM/0/WORKINGMODE_INFO", "r");
	if (fd <= 0 && fd > SWPAR_MIN)
	{
		SWPA_DEVICE_PRINT("Err: failed to open %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
		//printf("Err: failed to open %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
		return SWPAR_FAIL;
	}

	unsigned char rgbBuffer[WORK_INFO_LEN];
	swpa_memset(rgbBuffer, 0, WORK_INFO_LEN);
	int ret_size = 0;
	int ret = swpa_file_read(fd, rgbBuffer, WORK_INFO_LEN, &ret_size);
	if( SWPAR_OK != ret )
	{
		SWPA_DEVICE_PRINT("Err: failed to read %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
		//printf("Err: failed to read %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
		swpa_file_close(fd);
		return SWPAR_FAIL;
	}
	// ������Ȳ��ԣ������³�ʼ����
	if( WORK_INFO_LEN != ret_size )
	{
		swpa_file_close(fd);
		swpa_device_init_resetmode();
		SWPA_DEVICE_PRINT("Warning: work info init.");
		//printf("Warning: work info init.");
		return SWPAR_OK;
	}
	// �����־λ���ԣ������³�ʼ����
	unsigned int nFlag = 0;
	swpa_memcpy(&nFlag, rgbBuffer, sizeof(nFlag));
	if( nFlag != WORK_INFO_FLAG )
	{
		swpa_file_close(fd);
		swpa_device_init_resetmode();
		SWPA_DEVICE_PRINT("Warning: work info init.");
		//printf("Warning: work info init.");
		return SWPAR_OK;
	}

	int mode = -1;
	swpa_memcpy(&mode, rgbBuffer + sizeof(nFlag), sizeof(mode));
	unsigned int crc_stored = 0;
	swpa_memcpy(&crc_stored, rgbBuffer + (sizeof(nFlag) * 3), sizeof(crc_stored));
	unsigned int crc = swpa_utils_calc_crc32(0, rgbBuffer, WORK_INFO_LEN - sizeof(crc_stored));

	if (0 != mode && 1 != mode && 2 != mode)
	{
		swpa_file_close(fd);
		SWPA_DEVICE_PRINT("Err: got invalid working mode %d! [%d]\n", mode, SWPAR_FAIL);
		//printf("Err: got invalid working mode %d! [%d]\n", mode, SWPAR_FAIL);
		// ����Ĭ�ϵĹ���ģʽ
		return swpa_device_set_resetmode(0);
	}

	if (crc_stored != crc)
	{
		swpa_file_close(fd);
		SWPA_DEVICE_PRINT("Err: check working mode crc failed! [%d]\n", SWPAR_FAIL);
		//printf("Err: check working mode crc failed! [%d]\n", SWPAR_FAIL);
		// ����Ĭ�ϵĹ���ģʽ
		return swpa_device_set_resetmode(0);
	}

	*pmode = mode;

	return swpa_file_close(fd);
}




/**
* @brief ���õ�ǰ����ģʽ
*
* 
* @param [out] pmode  ��Ҫ���õĹ���ģʽ���Ϸ�ֵ��:
* - 0 : ��������ģʽ
* - 1 : ��������ģʽ
* - 2 : ��������ģʽ
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ���ù���ģʽʧ��
* @note .
* @see .
*/	
int swpa_device_set_resetmode(const int mode)
{
	SWPA_DEVICE_CHECK(0 == mode || 1 == mode|| 2 == mode);
	
	int fd = 0;
	fd = swpa_file_open("EEPROM/0/WORKINGMODE_INFO", "w+");
	if (fd <= 0 && fd > SWPAR_MIN)
	{
		SWPA_DEVICE_PRINT("Err: failed to open %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
		return SWPAR_FAIL;
	}

	unsigned char rgbBuffer[WORK_INFO_LEN];
	swpa_memset(rgbBuffer, 0, WORK_INFO_LEN);

	int ret_size = 0;
	int ret = swpa_file_read(fd, rgbBuffer, WORK_INFO_LEN, &ret_size);
	if( SWPAR_OK != ret )
	{
		swpa_file_close(fd);
		SWPA_DEVICE_PRINT("Err: failed to read %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
		return SWPAR_FAIL;
	}
	// ������Ȳ��ԣ������³�ʼ����
	if( WORK_INFO_LEN != ret_size )
	{
		swpa_file_close(fd);
		swpa_device_init_resetmode();
		SWPA_DEVICE_PRINT("Warning: work info init.");
		return SWPAR_OK;
	}
	// �����־λ���ԣ������³�ʼ����
	unsigned int nFlag = 0;
	swpa_memcpy(&nFlag, rgbBuffer, sizeof(nFlag));
	if( nFlag != WORK_INFO_FLAG )
	{
		swpa_file_close(fd);
		swpa_device_init_resetmode();
		SWPA_DEVICE_PRINT("Warning: work info init.");
		return SWPAR_OK;
	}

	int mode_copy = mode;
	swpa_memcpy(rgbBuffer + sizeof(unsigned int), &mode_copy, sizeof(mode_copy));
	unsigned int newcrc = swpa_utils_calc_crc32(0, rgbBuffer, WORK_INFO_LEN - sizeof(unsigned int));
	swpa_memcpy(rgbBuffer + (sizeof(int) * 3), &newcrc, sizeof(newcrc));

	ret_size = 0;
	swpa_file_seek(fd, SWPA_SEEK_SET, 0);
	if (SWPAR_OK != swpa_file_write(fd, rgbBuffer, WORK_INFO_LEN, &ret_size)
		|| WORK_INFO_LEN != ret_size)
	{
		swpa_file_close(fd);
		SWPA_DEVICE_PRINT("Err: failed to write %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
		return SWPAR_FAIL;
	}
	
	return swpa_file_close(fd);
}



#ifdef DM6467
static int _get_dm368_firmware_version(
	char* ublver,
	int * ubllen,
	char* ubootver,
	int * ubootlen,
	char* kernelver,
	int * kernellen,
	char* appver,
	int * applen)
{	
	//SWPA_DEVICE_CHECK(NULL == ublver || NULL == ubootver || NULL == kernelver || NULL == appver);

	int trycount = 8;
	while (SWPAR_OK != swpa_device_dm368_ready() && trycount--)
	{
		SWPA_DEVICE_PRINT("trycount = %d\n", trycount);
		SWPA_DEVICE_PRINT("Info: waiting dm368 ready\n");
	}

	if (0 >= trycount)
	{
		SWPA_DEVICE_PRINT("Err: dm368 is not ready! [%d]\n", SWPAR_FAIL);
		return SWPAR_FAIL;
	}
	
	SWPA_SOCKET_T sock;
	SWPA_SOCKET_ATTR_T sockattr;
    swpa_memset(&sockattr, 0, sizeof(sockattr));
    sockattr.af = SWPA_AF_INET;
    sockattr.type = SWPA_SOCK_STREAM;

    if( SWPAR_FAIL == swpa_socket_create( &sock , &sockattr ))
	{
		SWPA_DEVICE_PRINT("Err: failed to create socket! [%d]\n", SWPAR_FAIL);
        return SWPAR_FAIL;
	}

	struct SWPA_TIMEVAL timeout;
    timeout.tv_sec = 4;
    timeout.tv_usec = 0;

	if( SWPAR_FAIL == swpa_socket_opt( sock , SWPA_SOL_SOCKET, SWPA_SO_RCVTIMEO , (void*)&timeout , sizeof(struct SWPA_TIMEVAL)))
	{
		SWPA_DEVICE_PRINT("Err: failed to set recv timeout! [%d]\n", SWPAR_FAIL);
        return SWPAR_FAIL;
	}

	if( SWPAR_FAIL == swpa_socket_opt( sock , SWPA_SOL_SOCKET, SWPA_SO_SNDTIMEO , (void*)&timeout , sizeof(struct SWPA_TIMEVAL)))
	{
		SWPA_DEVICE_PRINT("Err: failed to set send timeout! [%d]\n", SWPAR_FAIL);
        return SWPAR_FAIL;
	}

	SWPA_SOCKADDR addr;
	swpa_memset(&addr, 0, sizeof(addr));

	swpa_strcpy(addr.ip, "123.123.219.219");
	addr.port = 2133;

	trycount = 8;
	//SWPA_DEVICE_PRINT("Info: connecting to %s:%d!\n", addr.ip, addr.port);
	while( (SWPAR_FAIL == swpa_socket_connect(sock, &addr, sizeof(SWPA_SOCKADDR))) && trycount--) 
	{
		SWPA_DEVICE_PRINT("Info: connecting to %s:%d!\n", addr.ip, addr.port);
	}

	if (0 >= trycount)
	{
		SWPA_DEVICE_PRINT("Err: failed to connect to %s:%d! [%d]\n", addr.ip, addr.port, SWPAR_FAIL);
		return SWPAR_FAIL;
	}

	// �����ͷ
	typedef struct tag_CameraCmdHeader
	{
		unsigned long id;
		unsigned long infosize;
	}
	CAMERA_CMD_HEADER;

	
	// ��Ӧ��ͷ
	typedef struct tag_CameraResponseHeader
	{
		unsigned long id;		
		int   ret;
		unsigned long infosize;
	}
	RESPONSE_CMD_HEADER;

	

	CAMERA_CMD_HEADER cmd;
	cmd.id = 0XFFCC0002;
	cmd.infosize = 0;
	int sentlen = 0;
	if( SWPAR_FAIL == swpa_socket_send(sock, (void*)&cmd, sizeof(cmd), &sentlen ) || 0 >= sentlen)
	{
		SWPA_DEVICE_PRINT("Err: failed to send cmd (0XFFCC0002) to %s:%d! [%d]\n", addr.ip, addr.port, SWPAR_FAIL);
		return SWPAR_FAIL;
	}

	RESPONSE_CMD_HEADER response;
	int recvlen = sizeof(response);
	if( SWPAR_FAIL == swpa_socket_recv(sock, (void*)&response, &recvlen ) || 0 >= recvlen)
	{
		SWPA_DEVICE_PRINT("Err: failed to recv cmd (0XFFCC0002) response header from %s:%d! [%d]\n", addr.ip, addr.port, SWPAR_FAIL);
		return SWPAR_FAIL;
	}

	if (0XFFCC0002 != response.id || 0 != response.ret)
	{
		SWPA_DEVICE_PRINT("Err: failed to get version data from %s:%d! (ret %d)[%d]\n", addr.ip, addr.port, response.ret, SWPAR_FAIL);
		return SWPAR_FAIL;
	}
	
	char* versionxml = (char*)swpa_mem_alloc(response.infosize+1);
	if (NULL == versionxml)
	{
		SWPA_DEVICE_PRINT("Err: no memory for versionxml! [%d]\n",  SWPAR_OUTOFMEMORY);
		return SWPAR_OUTOFMEMORY;
	}
	swpa_memset(versionxml, 0, response.infosize+1);
	
	recvlen = response.infosize;
	if( SWPAR_FAIL == swpa_socket_recv(sock, (void*)versionxml, &recvlen ) || response.infosize != recvlen || 0 == swpa_strcmp("", versionxml))
	{
		SWPA_DEVICE_PRINT("Err: failed to recv cmd (0XFFCC0002) response data from %s:%d! [%d]\n", addr.ip, addr.port, SWPAR_FAIL);
		swpa_mem_free(versionxml);
		return SWPAR_FAIL;
	}

	//SWPA_DEVICE_PRINT("Info: recvlen = %d\n", recvlen);
	//SWPA_DEVICE_PRINT("Info: versionxml = %s\n", versionxml);

	
	char* begin = NULL;
	char* end = NULL;
	int len = 0;

	if (NULL != ublver && NULL != ubllen && 0 < *ubllen)
	{
		begin = swpa_strstr(versionxml, "<UBL>") + swpa_strlen("<UBL>");
		end = swpa_strstr(versionxml, "</UBL>");
		len = (int)((unsigned long)end - (unsigned long)begin);

		if (len > *ubllen)
		{
			len = *ubllen;
		}

		swpa_memset(ublver, 0, *ubllen);
		swpa_strncpy(ublver, begin, len);
		*ubllen = len;
	}

	SWPA_DEVICE_PRINT("Info: ublver = %s\n", ublver);

	if (NULL != ubootver && NULL != ubootlen && 0 < *ubootlen)
	{
		begin = swpa_strstr(versionxml, "<UBoot>") + swpa_strlen("<UBoot>");
		end = swpa_strstr(versionxml, "</UBoot>");
		len = (int)((unsigned long)end - (unsigned long)begin);

		if (len > *ubootlen)
		{
			len = *ubootlen;
		}

		swpa_memset(ubootver, 0, *ubootlen);
		swpa_strncpy(ubootver, begin, len);
		*ubootlen = len;
	}

	//SWPA_DEVICE_PRINT("Info: ubootver = %s\n", ubootver);
	
	if (NULL != kernelver && NULL != kernellen && 0 < *kernellen)
	{
		begin = swpa_strstr(versionxml, "<Kernel>") + swpa_strlen("<Kernel>");
		end = swpa_strstr(versionxml, "</Kernel>");
		len = (int)((unsigned long)end - (unsigned long)begin);

		if (len > *kernellen)
		{
			len = *kernellen;
		}

		swpa_memset(kernelver, 0, *kernellen);
		swpa_strncpy(kernelver, begin, len);
		*kernellen = len;
	}

	//SWPA_DEVICE_PRINT("Info: kernelver = %s\n", kernelver);

	if (NULL != appver && NULL != applen && 0 < *applen)
	{
		begin = swpa_strstr(versionxml, "<App>") + swpa_strlen("<App>");
		end = swpa_strstr(versionxml, "</App>");
		len = (int)((unsigned long)end - (unsigned long)begin);

		if (len > *applen)
		{
			len = *applen;
		}
		swpa_memset(appver, 0, *applen);
		swpa_strncpy(appver, begin, len);
		*applen = len;
	}

	
	//SWPA_DEVICE_PRINT("Info: appver = %s\n", appver);

	swpa_mem_free(versionxml);
	
	return swpa_socket_delete(sock);
	
}
#endif

/**
* @brief ��ȡ�̼��汾��Ϣ
*
* 
* @param [in] version ��Ű汾��Ϣ���ַ���ָ��
* @param [inout] len ����Ԥ�ȷ����version�Ŀռ��С������ʵ�ʻ�ȡ�����ַ����Ĵ�С
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note 
* - �ú�����uboot��kernel��ubl��camera fpga�� cpld�İ汾��Ϣȫ����Ϸ���version�ִ���
* - �ִ���ÿ��ģ��汾��Ϣ�İڷŽṹ�ǡ�ģ������ģ��汾�ִ�;�����硰uboot:vxxxxx;��
* - ���versionΪNULL����lenС����ʵ�汾��Ϣ�ĳ��ȣ��ú���ͨ��len����version�ִ�����ʵ���ȣ�versionָ����ڴ����ݲ��䡣
* @see .
*/
int swpa_info_get_firmware_version(
	char * version,
	int * len
)
{
	char * ver_str = NULL;//[*len] = {0};
	char * mod_ver = NULL;////*len] = {0};
	char * temp_ver = NULL;//[*len] = {0};
	int ver_len = 0;
	int mod_len = *len;

	SWPA_DEVICE_CHECK(NULL != version);	//ָ��ǿ�
	SWPA_DEVICE_CHECK(NULL != len);		//ָ��ǿ�
	SWPA_DEVICE_CHECK(512 <= *len);			//*len��ֵ�費С��512

	SWPA_DEVICE_PRINT("version=0x%08x\n", version);
	SWPA_DEVICE_PRINT("len=0x%08x", len);
	SWPA_DEVICE_PRINT("*len=%d\n", *len);

	ver_str = (char*)swpa_mem_alloc(3*(*len));
	if (NULL == ver_str)
	{
		SWPA_DEVICE_PRINT("Err: no enough memory for ver_str! [%d]", SWPAR_OUTOFMEMORY);
		return SWPAR_OUTOFMEMORY;
	}

	swpa_memset(ver_str, 0, 3*(*len));
	
	mod_ver = ver_str + *len;
    temp_ver = ver_str + 2*(*len);

#ifdef DM6467
	//CPLD
	mod_len = (*len);
	swpa_memset(mod_ver, 0, mod_len);
	if (0 != drv_cpld_get_version(CPLD_ID, mod_ver, 1))
	{
		SWPA_DEVICE_PRINT("Err: drv_cpld_get_version() failed! [%d]\n", SWPAR_FAIL);
		swpa_sprintf(mod_ver, "Failed to get");
	}
	
	swpa_snprintf(temp_ver, *len, "cpld:%s;", mod_ver);	
	ver_len += swpa_strlen(temp_ver);
	if (ver_len < *len)
	{
		swpa_strcat(ver_str, temp_ver);
	}
	else
	{
		SWPA_DEVICE_PRINT("Err: *len (%d) is not enough for storing firmware info (total len >= %d)! [%d]\n", *len, ver_len, SWPAR_INVALIDARG);
		*len = 0;
		swpa_mem_free(ver_str);
		return SWPAR_INVALIDARG;
	}
#endif

	//FPGA
	mod_len = (*len);
	swpa_memset(mod_ver, 0, mod_len);
	if (0 != drv_fpga_get_version(CAMERA_FPGA_ID, mod_ver, mod_len))
	{
		SWPA_DEVICE_PRINT("Err: drv_fpga_get_version() failed! [%d]\n", SWPAR_FAIL);
		swpa_sprintf(mod_ver, "Failed to get");
	}
	swpa_snprintf(temp_ver, *len, "camera_fpga:%s;", mod_ver);
	ver_len += swpa_strlen(temp_ver);
	if (ver_len < *len)
	{
		swpa_strcat(ver_str, temp_ver);
	}
	else
	{
		SWPA_DEVICE_PRINT("Err: *len (%d) is not enough for storing firmware info (total len >= %d)! [%d]\n", *len, ver_len, SWPAR_INVALIDARG);
		*len = 0;
		swpa_mem_free(ver_str);
		return SWPAR_INVALIDARG;
	}

#ifdef DM6467
	//UBL(DM6467)
	mod_len = (*len);
	swpa_memset(mod_ver, 0, mod_len);
	
	if (0 != drv_device_get_firmaware_version(VER_UBL_DM6467, mod_ver, &mod_len))
	{
		SWPA_DEVICE_PRINT("Err: drv_device_get_firmaware_version() failed! [%d]\n", SWPAR_FAIL);
		swpa_sprintf(mod_ver, "Failed to get");
	}
	swpa_snprintf(temp_ver, *len, "ubl_dm6467:%s;", mod_ver);
	ver_len += swpa_strlen(temp_ver);
	if (ver_len < *len)
	{
		swpa_strcat(ver_str, temp_ver);
	}
	else
	{
		SWPA_DEVICE_PRINT("Err: *len (%d) is not enough for storing firmware info (total len >= %d)! [%d]\n", *len, ver_len, SWPAR_INVALIDARG);
		*len = 0;
		swpa_mem_free(ver_str);
		return SWPAR_INVALIDARG;
	}
	

	//UBOOT(DM6467)
	mod_len = (*len);
	swpa_memset(mod_ver, 0, mod_len);
	
	if (0 != drv_device_get_firmaware_version(VER_UBOOT_DM6467, mod_ver, &mod_len))
	{
		SWPA_DEVICE_PRINT("Err: drv_device_get_firmaware_version() failed! [%d]\n", SWPAR_FAIL);
		swpa_sprintf(mod_ver, "Failed to get");
	}
	swpa_snprintf(temp_ver, *len, "uboot_dm6467:%s;", mod_ver);
	ver_len += swpa_strlen(temp_ver);
	if (ver_len < *len)
	{
		swpa_strcat(ver_str, temp_ver);
	}
	else
	{
		SWPA_DEVICE_PRINT("Err: *len (%d) is not enough for storing firmware info (total len >= %d)! [%d]\n", *len, ver_len, SWPAR_INVALIDARG);
		*len = 0;
		swpa_mem_free(ver_str);
		return SWPAR_INVALIDARG;
	}

	
	//KERNEL(DM6467)
	mod_len = (*len);
	swpa_memset(mod_ver, 0, mod_len);
	
	if (0 != drv_device_get_firmaware_version(VER_KERNEL_DM6467, mod_ver, &mod_len))
	{
		SWPA_DEVICE_PRINT("Err: drv_device_get_firmaware_version() failed! [%d]\n", SWPAR_FAIL);
		swpa_sprintf(mod_ver, "Failed to get");
	}
	swpa_snprintf(temp_ver, *len, "kernel_dm6467:%s;", mod_ver);
	ver_len += swpa_strlen(temp_ver);
	if (ver_len < *len)
	{
		swpa_strcat(ver_str, temp_ver);
	}
	else
	{
		SWPA_DEVICE_PRINT("Err: *len (%d) is not enough for storing firmware info (total len >= %d)! [%d]\n", *len, ver_len, SWPAR_INVALIDARG);
		*len = 0;
		swpa_mem_free(ver_str);
		return SWPAR_INVALIDARG;
	}
#endif
#ifdef DM8127
	//UBOOT(DM8127)
	mod_len = (*len);
	swpa_memset(mod_ver, 0, mod_len);
	
	if (0 != drv_device_get_firmaware_version(VER_UBOOT, mod_ver, &mod_len))
	{
		SWPA_DEVICE_PRINT("Err: drv_device_get_firmaware_version() failed! [%d]\n", SWPAR_FAIL);
		swpa_sprintf(mod_ver, "Failed to get");
	}
	swpa_snprintf(temp_ver, *len, "uboot:%s;", mod_ver);
	ver_len += swpa_strlen(temp_ver);
	if (ver_len < *len)
	{
		swpa_strcat(ver_str, temp_ver);
	}
	else
	{
		SWPA_DEVICE_PRINT("Err: *len (%d) is not enough for storing firmware info (total len >= %d)! [%d]\n", *len, ver_len, SWPAR_INVALIDARG);
		*len = 0;
		swpa_mem_free(ver_str);
		return SWPAR_INVALIDARG;
	}
	
	//KERNEL(DM8127)
	mod_len = (*len);
	swpa_memset(mod_ver, 0, mod_len);
	
	if (0 != drv_device_get_firmaware_version(VER_KERNEL, mod_ver, &mod_len))
	{
		SWPA_DEVICE_PRINT("Err: drv_device_get_firmaware_version() failed! [%d]\n", SWPAR_FAIL);
		swpa_sprintf(mod_ver, "Failed to get");
	}
	swpa_snprintf(temp_ver, *len, "kernel:%s;", mod_ver);
	ver_len += swpa_strlen(temp_ver);
	if (ver_len < *len)
	{
		swpa_strcat(ver_str, temp_ver);
	}
	else
	{
		SWPA_DEVICE_PRINT("Err: *len (%d) is not enough for storing firmware info (total len >= %d)! [%d]\n", *len, ver_len, SWPAR_INVALIDARG);
		*len = 0;
		swpa_mem_free(ver_str);
		return SWPAR_INVALIDARG;
	}
#endif

	/*
	//DM368
	char dm368_ubl_ver[256] = {0};
	int  dm368_ubl_len = sizeof(dm368_ubl_ver)-1;
	char dm368_uboot_ver[256] = {0};
	int  dm368_uboot_len = sizeof(dm368_uboot_ver)-1;
	char dm368_kernel_ver[256] = {0};
	int  dm368_kernel_len = sizeof(dm368_kernel_ver)-1;
	char dm368_app_ver[256] = {0};
	int  dm368_app_len = sizeof(dm368_app_ver)-1;
	if (SWPAR_OK != _get_dm368_firmware_version(
						dm368_ubl_ver,
						&dm368_ubl_len,
						dm368_uboot_ver,
						&dm368_uboot_len,
						dm368_kernel_ver,
						&dm368_kernel_len,
						dm368_app_ver,
						&dm368_app_len))
	{
		SWPA_DEVICE_PRINT("Err: failed to get dm368 version info! [%d]\n", SWPAR_FAIL);
		return SWPAR_FAIL;
	}
	
	//UBL(DM368)
	swpa_snprintf(temp_ver, *len, "ubl_dm368:%s;", dm368_ubl_ver);
	ver_len += swpa_strlen(temp_ver);
	if (ver_len < *len)
	{
		swpa_strcat(ver_str, temp_ver);
	}
	else
	{
		SWPA_DEVICE_PRINT("Err: *len (%d) is not enough for storing firmware info (total len >= %d)! [%d]\n", *len, ver_len, SWPAR_INVALIDARG);
		*len = 0;
		return SWPAR_INVALIDARG;
	}

	//UBOOT(DM368)
	swpa_snprintf(temp_ver, *len, "uboot_dm368:%s;", dm368_uboot_ver);
	ver_len += swpa_strlen(temp_ver);
	if (ver_len < *len)
	{
		swpa_strcat(ver_str, temp_ver);
	}
	else
	{
		SWPA_DEVICE_PRINT("Err: *len (%d) is not enough for storing firmware info (total len >= %d)! [%d]\n", *len, ver_len, SWPAR_INVALIDARG);
		*len = 0;
		return SWPAR_INVALIDARG;
	}
	
	//KERNEL(DM368)
	swpa_snprintf(temp_ver, *len, "kernel_dm368:%s;", dm368_kernel_ver);
	ver_len += swpa_strlen(temp_ver);
	if (ver_len < *len)
	{
		swpa_strcat(ver_str, temp_ver);
	}
	else
	{
		SWPA_DEVICE_PRINT("Err: *len (%d) is not enough for storing firmware info (total len >= %d)! [%d]\n", *len, ver_len, SWPAR_INVALIDARG);
		*len = 0;
		return SWPAR_INVALIDARG;
	}

	//APP(DM368)
	swpa_snprintf(temp_ver, *len, "app_dm368:%s;", dm368_app_ver);
	ver_len += swpa_strlen(temp_ver);
	if (ver_len < *len)
	{
		swpa_strcat(ver_str, temp_ver);
	}
	else
	{
		SWPA_DEVICE_PRINT("Err: *len (%d) is not enough for storing firmware info (total len >= %d)! [%d]\n", *len, ver_len, SWPAR_INVALIDARG);
		*len = 0;
		return SWPAR_INVALIDARG;
	}
	*/
	

	*len = ver_len;
	swpa_strncpy(version, ver_str, ver_len);

	swpa_mem_free(ver_str);

	return SWPAR_OK;
}





/**
* @brief ��ȡӲ������汾
*
* 
* @param [out] version ��Ű汾�ַ���
* @param [inout] len ����version�������Ĵ�С��������ʵ��version�ִ��ĳ���
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note .
* @see .
*/	
int swpa_info_get_hardware_version(	
	char * version,
	int * len
)
{
//Marked since no hardware version currently
/*
	int ret = 0;

	SWPA_DEVICE_CHECK(NULL != version);
	SWPA_DEVICE_CHECK(NULL != len);
	SWPA_DEVICE_CHECK(0 != *len);

	SWPA_DEVICE_PRINT("*len=%d\n", *len);

	ret = drv_device_get_firmaware_version(VER_HARDWARE_VER, version, len);
	if (0 != ret)
	{
		SWPA_DEVICE_PRINT("Err: drv_device_get_hardware_version() failed(%d)! [%d]\n", ret, SWPAR_FAIL);
		*len = 0;
		return SWPAR_FAIL;
	}
  
	*len = swpa_strlen(version);
	return SWPAR_OK;
*/	
	return SWPAR_FAIL;
}




/**
* @brief ��ȡ�豸���б��
*
* 
* @param [out] serial_no ����豸���б���ַ���
* @param [inout] len ����serial_no�������Ĵ�С��������ʵ��serial_no�ִ��ĳ���
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  ִ��ʧ��
* @retval -2 :  �����Ƿ�
* @note .
* @see .
*/	
int swpa_info_get_device_serial_no(	
	char * serial_no,
	int * len
)
{
	int ret = 0;

	SWPA_DEVICE_CHECK(NULL != serial_no);
	SWPA_DEVICE_CHECK(NULL != len);
	SWPA_DEVICE_CHECK(0 != *len);

	SWPA_DEVICE_PRINT("*len=%d\n", *len);

	ret = drv_device_get_firmaware_version(VER_SERIAL_VER, serial_no, len);
	if (0 != ret)
	{
		SWPA_DEVICE_PRINT("Err: drv_device_get_serial_number() failed(%d)! [%d]\n", ret, SWPAR_FAIL);
		*len = 0;
		return SWPAR_FAIL;
	}
  
	*len = swpa_strlen(serial_no);
	
	return SWPAR_OK;
}

void (*g_dsp_alarm)(void *puser, int info) = 0;
void* g_puser = 0;
int swpa_device_dsp_alarm(int signal_no)
{
	int info = 0;
	if(g_dsp_alarm && !drv_device_get_pwminfo(DEVID_PWM, (unsigned int *)&info))
	{
		g_dsp_alarm(g_puser, info);
	}
	else
	{
		printf("swpa_device_dsp_alarm error\n");
	}
	return 0;
}

int swpa_device_register_callback( void (*dsp_alarm)(void *puser, int info), void *puser)
{
	g_dsp_alarm = dsp_alarm;
	g_puser = puser;
	
	struct sigaction act;
	drv_device_get_enable_pwm_signal(DEVID_PWM);
	swpa_memset(&act, 0, sizeof(act));
	act.sa_handler = swpa_device_dsp_alarm;
	act.sa_flags = 0;
	sigaction(SIGIO, &act, NULL);
	return SWPAR_OK;
}

// ����DSP�����ص�
// �жϴ���
void sig_handler(int sig)
{
    int ret = 0;
    unsigned int reg = 0;
    //static unsigned long long cnt = 0;

    //log_trace(5, "hello world sig_handler!!!\n");
    // ��ȡ�Ĵ���ֵ(ͨ��driverlib�ṩ�Ľӿ�)
    ret = drv_device_get_pwminfo(DEVID_PWM, &reg);

    if (ret < 0) {
        printf("ioctl error\n");
        return;
    }

    // ����������Ӧ����
    //log_trace(5, "Read reg = %x\n", reg);
    g_dsp_alarm(g_puser, reg);

    //cnt++;
    //log_trace(5, "APP Get async signal, cnt: %d\n", cnt);
    //flag = 1;

    return;
}

// ��ʼ����ע���ж�
static int trigger_init(void)
{
    int ret = 0;
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_handler = sig_handler;
    //act.sa_sigaction = new_op;
    //act.sa_flags = SA_SIGINFO;
    act.sa_flags = 0;

    ret = sigaction(SIGIO, &act, NULL);   // ע����Ӧ����
    if (ret < 0)
    {
        printf("sigaction failed!\n");
    }
    else
    {
        printf("sigaction OK!\n");
    }

    printf("enable pwm signale.\n");
    // driverlib�ṩ�Ľӿ�
    drv_device_get_enable_pwm_signal(DEVID_PWM);

    return ret;
}

int swpa_device_register_callback_venus( void (*dsp_alarm)(void *puser, int info), void *puser)
{
    g_dsp_alarm = dsp_alarm;
    g_puser = puser;
    return trigger_init() >= 0 ? SWPAR_OK : SWPAR_FAIL;
}



/**
* @brief ��ʼ������оƬ
* 
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  �ײ�ִ��ʧ��
* @note .
* @see .
*/	
int swpa_device_crypt_init()
{
	return drv_sec_crypt_init(DEVID_IIC_AT88SC);
}



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
int swpa_device_crypt_write(unsigned int addr, unsigned char* data, unsigned int* len)
{
	return drv_sec_crypt_write(DEVID_IIC_AT88SC, addr, data, len);
}


/**
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
int swpa_device_crypt_read(unsigned int addr, unsigned char* data, unsigned int* len)
{
	return drv_sec_crypt_read(DEVID_IIC_AT88SC, addr, data, len);
}




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
int swpa_device_init_crypt_config(unsigned char* nc, unsigned char* key)
{
	return drv_sec_init_config(DEVID_IIC_AT88SC, nc, key);
}




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
int swpa_device_get_fuse_status(unsigned char* pvalue)
{
	return drv_sec_fuse_read(DEVID_IIC_AT88SC, pvalue);
}



/**
* @brief �նϼ���оƬ��˿
* 
* @retval SWPAR_OK : ִ�гɹ�
* @retval SWPAR_FAIL :  �ײ�ִ��ʧ��
* @note .
* @see .
*/	
int swpa_device_fuse_write(void)
{
	return drv_sec_fuse_write(DEVID_IIC_AT88SC);
}

/**
* @brief ��ȡ��λ���Ƿ񳤰�5S�¼���
*
* @retval 0 : ���¼�
* @retval 1 :  ���¼�
* @note .
* @see .
*/	
int swpa_device_get_reset_flag()
{
	unsigned char bFlag = 0;
	int iret = drv_cpld_read(DEVID_SPI_CPLD, 0x08, &bFlag);

	// ��ȡEEPROM��־
	int fd = 0;
	fd = swpa_file_open("EEPROM/0/WORKINGMODE_INFO", "w+");
	if (fd <= 0 && fd > SWPAR_MIN)
	{
		SWPA_DEVICE_PRINT("Err: failed to open %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
		//printf("Err: failed to open %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
		return 1;
	}

	unsigned char rgbBuffer[WORK_INFO_LEN];
	swpa_memset(rgbBuffer, 0, WORK_INFO_LEN);
	int ret_size = 0;
	if (SWPAR_OK != swpa_file_read(fd, rgbBuffer, WORK_INFO_LEN, &ret_size)
		|| WORK_INFO_LEN != ret_size)
	{
		SWPA_DEVICE_PRINT("Err: failed to read %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
		//printf("Err: failed to read %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
		swpa_file_close(fd);
		return 1;
	}
	int eprflag = 0;
	swpa_memcpy(&eprflag, rgbBuffer + (sizeof(int) * 2), sizeof(eprflag));

	if( 0 == iret  && ((bFlag & 0x01) == 0x01) )
	{
		//printf("info: cpld read [0x%08x]\n", (unsigned int)bFlag);
		if( eprflag != 0xFFEE0001 )
		{
			//printf("info: eprflag read [0x%08x]\n", (unsigned int)eprflag);
			eprflag = 0xFFEE0001;
			swpa_memcpy(rgbBuffer + (sizeof(int) * 2), &eprflag, sizeof(eprflag));
			unsigned int crc = swpa_utils_calc_crc32(0, rgbBuffer, WORK_INFO_LEN - sizeof(unsigned int));
			swpa_memcpy(rgbBuffer + (sizeof(int) * 3), &crc, sizeof(crc));
			ret_size = 0;
			swpa_file_seek(fd, SWPA_SEEK_SET, 0);
			if (SWPAR_OK != swpa_file_write(fd, rgbBuffer, WORK_INFO_LEN, &ret_size)
				|| WORK_INFO_LEN != ret_size)
			{
				SWPA_DEVICE_PRINT("Err: failed to write %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
				//printf("Err: failed to write %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
				swpa_file_close(fd);
				return 1;
			}
			swpa_file_close(fd);
			return 0;
		}
		else
		{
			//printf("<>info: eprflag read [0x%08x]\n", (unsigned int)eprflag);
			swpa_file_close(fd);
			return 1;
		}
	}
	else if( iret == 0 )
	{
		// ���ñ�־��
		if( eprflag == 0xFFEE0001 )
		{
			//printf("<set>info: eprflag read [0x%08x] ==> 0\n", (unsigned int)eprflag);
			eprflag = 0;
			swpa_memcpy(rgbBuffer + (sizeof(int) * 2), &eprflag, sizeof(eprflag));
			unsigned int crc = swpa_utils_calc_crc32(0, rgbBuffer, WORK_INFO_LEN - sizeof(unsigned int));
			swpa_memcpy(rgbBuffer + (sizeof(int) * 3), &crc, sizeof(crc));
			ret_size = 0;
			swpa_file_seek(fd, SWPA_SEEK_SET, 0);
			if (SWPAR_OK != swpa_file_write(fd, rgbBuffer, WORK_INFO_LEN, &ret_size)
				|| WORK_INFO_LEN != ret_size)
			{
				SWPA_DEVICE_PRINT("Err: failed to write %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
				//printf("Err: failed to write %s! [%d]\n", "EEPROM/0/WORKINGMODE_INFO", SWPAR_FAIL);
				swpa_file_close(fd);
				return 1;
			}
		}
		swpa_file_close(fd);
		return 1;
	}

	swpa_file_close(fd);
	
	return 1;
}


#ifdef DM8127
int swpa_device_get_sdcard_status(int* status)
{
	SWPA_DEVICE_CHECK(NULL != status);

	if (0 != drv_sdcard_check_status(DEVID_DM8127, status))
	{
		SWPA_DEVICE_PRINT("Err: failed to get sdcard status.\n");
		return SWPAR_FAIL;
	}

	return SWPAR_OK;
}
#endif

/**
* @brief ���³�ʼ��SD��
*
* @retval 0 : ��ʼ���ɹ�
* @retval 1 : ��ʼ��ʧ��
* @note .
* @see .
*/	
int swpa_device_sd_reinit()
{
	int fd = -1;
	int ret = -1;
	int try = 150;
	char buf[1] = "0";
/*
	fd = open("/sys/class/mmc_host/mmc1/manual_in_re",O_RDWR);
	flock(fd,LOCK_EX | LOCK_NB );
	if(fd < 0)
	{
		printf("open manual_in_re error!!!\n");
		goto err;
	}
	write(fd,"0",1);
	while(swpa_utils_file_exist("/dev/mmcblk0") || swpa_utils_file_exist("/dev/mmcblk1"))
	{
		try--;
		swpa_thread_sleep_ms(100);	
		if(try == 0)
			goto err;
	}
	swpa_thread_sleep_ms(1000);
	write(fd,"1",1);
	while(!swpa_utils_file_exist("/dev/mmcblk0") && !swpa_utils_file_exist("/dev/mmcblk1"))
	{
		try--;
		swpa_thread_sleep_ms(100);	
		if(try == 0)
			goto err;
	}
	swpa_thread_sleep_ms(1000);	
	flock(fd,LOCK_UN);
	return 0;
		
	err:
		printf("reinit sd error!!!!\n");
		return -1;
		*/

	fd = open("/sys/class/mmc_host/mmc1/manual_in_re",O_RDWR);
	flock(fd,LOCK_EX | LOCK_NB );
	if(fd < 0)
	{
		printf("open manual_in_re error!!!\n");
		goto err;
	}
	write(fd,"2",1);
	while(!swpa_utils_file_exist("/dev/mmcblk0") && !swpa_utils_file_exist("/dev/mmcblk1") && !swpa_utils_file_exist("/dev/mmcblk2") 
		&& !swpa_utils_file_exist("/dev/mmcblk3") && !swpa_utils_file_exist("/dev/mmcblk4") && !swpa_utils_file_exist("/dev/mmcblk5") 
		&& !swpa_utils_file_exist("/dev/mmcblk6") && !swpa_utils_file_exist("/dev/mmcblk7"))
	{
		try--;
		swpa_thread_sleep_ms(100);	
		if(try == 0)
			goto err;
	}
	printf("find sd card!!!\n");
	flock(fd,LOCK_UN);
	return 0;
		
	err:
		printf("reinit sd error!!!!\n");
		return -1;
	
}


int swpa_device_set_rs485(int dir)
{
    return drv_device_set_rs485_direction(DEVID_RS485_1, dir);
}

#ifdef DM8127
int swpa_fpga_reg_read(unsigned int addr,unsigned int *value)
{
	SWPA_DEVICE_CHECK(value != NULL);
	drv_fpga_read_single(DEVID_SPI_FPGA, addr, value);
	return 0;
}
                                                                
int swpa_fpga_reg_write(unsigned int addr,unsigned int value)
{
	FPGA_STRUCT fpga_info;
    fpga_info.count = 1;
    fpga_info.regs[0].addr = addr;
    fpga_info.regs[0].data = value;	
	drv_fpga_write(DEVID_SPI_FPGA, &fpga_info);
	return 0;
}

int swpa_fpga_multi_reg_write(int count, unsigned int *addr,unsigned int *value)
{
	FPGA_STRUCT fpga_info;
	SWPA_DEVICE_CHECK(count <= MAX_FPGA_REG);
	SWPA_DEVICE_CHECK(addr != NULL);
	SWPA_DEVICE_CHECK(value != NULL);

	int reg_count = swpa_min(count, MAX_FPGA_REG);
	fpga_info.count = reg_count;
	int i = 0;
	for (i=0; i<reg_count; i++)
	{
		fpga_info.regs[i].addr = addr[i];
    	fpga_info.regs[i].data = value[i];	
	}
 
	drv_fpga_write(DEVID_SPI_FPGA, &fpga_info);
	return 0;
}

int spwa_autoiris_pwm(int duty)
{
	return drv_autoiris_pwm(DEVID_PWM, duty);
}

#endif
