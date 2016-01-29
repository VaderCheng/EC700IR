#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <time.h>
#include <ctype.h>

#include <drv_device.h>

#include "swpa.h"

#ifdef SWPA_DEVICE_SETTING
#define SWPA_DEVICE_SETTING_PRINT(fmt, ...) SWPA_PRINT("[%s:%d]"fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define SWPA_DEVICE_SETTING_CHECK(arg)      {if (!(arg)){SWPA_PRINT("[%s:%d]Check failed : %s [%d]\n", __FILE__, __LINE__, #arg, SWPAR_INVALIDARG);return SWPAR_INVALIDARG;}}
#else
#define SWPA_DEVICE_SETTING_PRINT(fmt, ...)
#define SWPA_DEVICE_SETTING_CHECK(arg)
#endif

#define    VALID_TIME_RECORD    99    // ��Ч��¼��
#define    MAX_TIME_RECORD      (VALID_TIME_RECORD+1)

#define    MAX_LOG_LENGTH        64
#define    VALID_LOG_RECORD      24    // ��Ч��¼��
#define    MAX_LOG_RECORD        (VALID_LOG_RECORD+1)

#define    MASTER    1
#define    SLAVER    0

#pragma pack(push,1)

///* ���ݽṹ˵��:��������ʵʱʱ��. */
typedef struct {
    uint16_t wYear;    //����.
    uint16_t wMonth;   //����.
    uint16_t wDay;     //����.
    uint16_t wWeekNum; //��ǰ���ڵ�������.
    uint16_t wHour;    //С����,24Сʱ��.
    uint16_t wMinute;  //Сʱ��.
    uint16_t wSecond;  //����.
    uint16_t wMSecond; //������.
} REAL_TIME_STRUCT;

typedef struct boot_time_table_head_s {
    uint32_t head_crc;                // ��ͷcrc
    uint32_t current_times;            // ��ǰ��������
    uint32_t head_index;                // ʱ���¼��ͷ����
    uint32_t current_index;            // ��ǰ��¼����
} boot_time_table_head_t;

typedef struct boot_time_s {
    uint32_t times;                         // ��ǰ��������
    REAL_TIME_STRUCT real_time;            // ����ʱ��
} boot_time_t;

typedef struct boot_time_table_s {
    boot_time_table_head_t head;    // ����ʱ���ͷ
    boot_time_t    boot_time[MAX_TIME_RECORD];        // ����ʱ��
} boot_time_table_t;

typedef struct boot_log_table_head_s {
    uint32_t head_crc;                // ��ͷcrc
    uint32_t version;                // �汾��
    uint32_t head_index;                // ��־��¼��ͷ����
    uint32_t current_index;            // ��ǰ��¼����
} boot_log_table_head_t;

typedef struct boot_log_s {
    uint32_t boot_times;                // ��ǰ��������
    uint8_t  boot_message[MAX_LOG_LENGTH];        // ��־��Ϣ
} boot_log_t;

typedef struct boot_log_table_s {
    boot_log_table_head_t head;        // ��־��ͷ
    boot_log_t    boot_log[MAX_LOG_RECORD];        // ��־
} boot_log_table_t;

typedef struct boot_arg_s {
    uint32_t     table_crc;          // ��crcֵ
    uint32_t     factory_setting;    // �������
    uint32_t     version;            // ��汾��
    uint32_t     rtm_flag;           // rtmֵ
    uint32_t     boot_count;         // ��������
    uint32_t     bootdelay;          // ����������
    uint8_t      bootarg[256];       // bootarg����
} boot_arg_t;

typedef struct boot_arg_s2 {
    uint32_t    table_crc;          // ��crcֵ
    uint32_t    factory_setting;    // �������
    uint32_t    version;            // ��汾��
    uint32_t    rtm_flag;           // rtmֵ
    uint32_t    boot_count;         // ��������
    uint32_t    bootdelay;          // ����������
    uint8_t     bootarg[256];       // bootarg����
    uint32_t    boot_count_en;      // ʹ����������(�¼�)
} boot_arg_t2;

typedef union boot_configure {
    boot_arg_t boot_table;
    boot_arg_t2 boot_table2;
    uint8_t fill_empty[1024];
} boot_cfg_u;

// flash��������
//#define CFG_DEF_EEPROM_ADDR F_DM6467_ADDR_UBOOT
#define CFG_SPI_FLASH_SECTOR_LEN (1024*64)
#define CFG_ENV_OFFSET (CFG_SPI_FLASH_SECTOR_LEN * 7) // uboot = 512k, 8��64k
#define CFG_ENV_SIZE   (CFG_SPI_FLASH_SECTOR_LEN)     // env = 64k

typedef    struct environment_s {
    unsigned long    crc;        /* CRC32 over data bytes    */
#ifdef CFG_REDUNDAND_ENVIRONMENT
    unsigned char    flags;        /* active/obsolete flags    */
#endif
    unsigned char    data[CFG_ENV_SIZE]; /* Environment data        */
} env_t;


#pragma pack(pop)

static boot_cfg_u g_boot_cfg;
static boot_time_table_t g_boot_time_table;
static boot_log_table_t g_boot_log_table;

static void convert_ms2time(uint32_t time_low, uint32_t time_high, REAL_TIME_STRUCT* ts)
{
    uint64_t millsecond = 0;
    time_t second = 0;
    struct tm* timeinfo = NULL;

    if (ts == NULL)
        return;

    millsecond = time_high;
    millsecond <<= 32;
    millsecond |= time_low;

    ts->wMSecond = (uint32_t)(millsecond % 1000);
    second = (time_t)((millsecond - ts->wMSecond) / 1000);

    timeinfo = localtime(&second);
    ts->wYear = timeinfo->tm_year + 1900;
    ts->wMonth  = timeinfo->tm_mon + 1;
    ts->wWeekNum = timeinfo->tm_wday;
    ts->wDay  = timeinfo->tm_mday;
    ts->wHour = timeinfo->tm_hour;
    ts->wMinute  = timeinfo->tm_min;
    ts->wSecond  = timeinfo->tm_sec;
}

static void convert_time2ms(REAL_TIME_STRUCT* ts, uint32_t* time_low, uint32_t* time_high)
{
    struct tm tm_time;
    time_t time = 0;
    uint64_t millsecond = 0;

    if ((NULL == ts) || (NULL ==time_low) || (NULL == time_high))
        return;

    tm_time.tm_year = ts->wYear - 1900;
    tm_time.tm_mon = ts->wMonth - 1;
    tm_time.tm_mday = ts->wDay;
    tm_time.tm_hour = ts->wHour;
    tm_time.tm_min = ts->wMinute;
    tm_time.tm_sec = ts->wSecond;

    time = mktime(&tm_time);    /* �õ����� */
    /*
    ���ܺϲ�Ϊ��ʽ
        millsecond = time*1000 + ts->wMSecond;
    */
    millsecond = time;
    millsecond *= 1000;
    millsecond += ts->wMSecond;

    *time_low = (uint32_t)(millsecond);
    *time_high = (uint32_t)(millsecond >> 32);
}

static int get_timezone(int* timezone)
{
    char* date_cmd = "date +%z";
    FILE *fp = NULL;
    char timezone_info[32] = {0};
    int tmp_time_zone = 0;

    fp = popen(date_cmd, "r");
    if (fp == NULL)
    {
        return -1;
    }
    fread(timezone_info, sizeof(timezone_info), 1, fp);
    pclose(fp);

    timezone_info[strlen(timezone_info) - 1] = '\0';

    if (isdigit(timezone_info[1]) && isdigit(timezone_info[2]))
    {
        tmp_time_zone = (timezone_info[1] - '0') * 10 + (timezone_info[2] - '0');
        if (timezone_info[0] == '-')
        {
            tmp_time_zone = - tmp_time_zone;
        }
        *timezone = tmp_time_zone;

        return 0;
    }
    else
    {
        *timezone = 0;
        return -1;
    }

    return 0;
}

// ����ʱ����old_time��new_time������ͬһ�ṹ��ָ��
static void adjust_time(REAL_TIME_STRUCT* old_time, REAL_TIME_STRUCT* new_time, int timezone)
{
    uint32_t time_high = 0;
    uint32_t time_low = 0;

    convert_time2ms(old_time, &time_low, &time_high);

    time_low += timezone * 3600 * 1000;

    convert_ms2time(time_low, time_high, new_time);
}

//////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
int swpa_device_set_normal_bootcount(int count)
{
	SWPA_DEVICE_SETTING_CHECK(0 < count);

	if (0 != drv_device_set_normal_bootcount(count))
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to set normal boot count to %d\n", count);
        return SWPAR_FAIL;
	}

	return SWPAR_OK;
}


int swpa_device_set_backup_bootcount(int count)
{
	SWPA_DEVICE_SETTING_CHECK(0 < count);

	if (0 != drv_device_set_backup_bootcount(count))
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to set backup boot count to %d\n", count);
		return SWPAR_FAIL;
	}

	return SWPAR_OK;
}




int swpa_device_set_bootdelay(int delaytime)
{
    SWPA_DEVICE_SETTING_CHECK(0 < delaytime && 32 > delaytime);

	if (0 != drv_device_set_bootdelay(delaytime))
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to set boot delay time to %d\n", delaytime);
		return SWPAR_FAIL;
	}

	return SWPAR_OK;
}

int swpa_device_lock_uboot(void)
{
    int ret = 0;

	if (0 != drv_device_lock_uboot())
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to lock uboot!!\n");
		return SWPAR_FAIL;
	}

    return SWPAR_OK;
}

int swpa_device_unlock_uboot(void)
{
    int ret = 0;

	if (0 != drv_device_unlock_uboot())
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to unlock uboot!!\n");
		return SWPAR_FAIL;
	}

    return SWPAR_OK;
}

int swpa_device_lock_linuxinfo(void)
{
	if (0 != drv_device_lock_linuxinfo())
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to lock linux info\n");
        return SWPAR_FAIL;
	}

    return SWPAR_OK;
}

int swpa_device_unlock_linuxinfo(void)
{
	if (0 != drv_device_unlock_linuxinfo())
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to unlock linux info\n");
        return SWPAR_FAIL;
	}

    return SWPAR_OK;
}

int swpa_device_restore_defaultinfo()
{
    int ret = 0;
    unsigned int len = E_LEN_PARAM_BACKUP;

	if (0 != drv_device_restore_defaultinfo())
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to restore device default info\n");
        return SWPAR_FAIL;
	}

    return SWPAR_OK;
}


int swpa_device_read_boottime(char* boottime, unsigned int len)
{
	if (0 != drv_device_read_boottime(boottime, len))
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to read boot time\n");
        return SWPAR_FAIL;
	}

    return SWPAR_OK;
}

int swpa_device_read_bootlog(char* bootlog, unsigned int len)
{
    unsigned int tmp_len = 0;
    tmp_len = sizeof(boot_log_table_t);
    int index = 0;
    int index_end = 0;
    int ret = 0;
    int timezone = 0;
    char boot_buffer[4096] = {0};
    char tmp_buffer[128] = {0};
    char tmp_info[64] = {0};
    char* ptr = NULL;
    REAL_TIME_STRUCT real_time;
    int year, month, day, hour, minute, second;

    SWPA_DEVICE_SETTING_CHECK(NULL != bootlog);
    if (len > 4096)
    {
        return -2;
    }

    get_timezone(&timezone);

    memset(&g_boot_log_table, 0, sizeof(boot_log_table_t));
    ret = drv_eeprom_read(DEVID_IIC_EEPROM, E_ADDR_UBOOT_LOG, (unsigned char*)&g_boot_log_table, &tmp_len);
    if (SWPAR_OK != ret)
    {
        SWPA_DEVICE_SETTING_PRINT("ERROR >> Read log table error return %08x\n", ret);
        return SWPAR_FAIL;
    }

    // todo: ��У��crc ����ʾ
//    printf("boot_log_table head\n");
//    printf("crc        version        head_index     next_index\n");
//    printf("%08x    %08x    %08x    %08x\n",
//            g_boot_log_table.head.head_crc,
//            g_boot_log_table.head.version,
//            g_boot_log_table.head.head_index,
//            g_boot_log_table.head.current_index
//    );

    if (g_boot_log_table.head.head_index == g_boot_log_table.head.current_index)
    {
        SWPA_DEVICE_SETTING_PRINT("No more boot log\n");
        return SWPAR_OK;
    }

    index = g_boot_log_table.head.head_index;;
    index_end = g_boot_log_table.head.current_index;

    ptr = boot_buffer;

    while (index != index_end)
    {
        //1. read time info
        sscanf((char*)g_boot_log_table.boot_log[index].boot_message,
                "%04d-%02d-%02d %02d:%02d:%02d%[ -~]",
                &year, &month, &day, &hour, &minute, &second,
                tmp_info);

        real_time.wYear = year;
        real_time.wMonth = month;
        real_time.wDay = day;
        real_time.wHour = hour;
        real_time.wMinute = minute;
        real_time.wSecond = second;

        // 2. adjust time zone
        adjust_time(&real_time, &real_time, timezone);

        // 3. make new info
        sprintf(tmp_buffer, "[%d]: %04d-%02d-%02d %02d:%02d:%02d%s\n",
                g_boot_log_table.boot_log[index].boot_times,
                real_time.wYear, real_time.wMonth, real_time.wDay,
                real_time.wHour, real_time.wMinute, real_time.wSecond,
                tmp_info);

        strcpy(ptr, tmp_buffer);
        ptr += strlen(tmp_buffer);

        index++;

        if (index == MAX_LOG_RECORD)
        {
            index = 0;
        }
    }

    memcpy(bootlog, boot_buffer, len);

    return SWPAR_OK;
}

////////////////////////////////////////////////////////////////////

int swpa_device_write_sn(char* sn)
{
    // ��ֹд���кų���ÿ�����д255�ֽ�
    unsigned int len = 255 < strlen(sn)+1 ? 255 : strlen(sn)+1;
    unsigned char buffer[256] = {0};
    int ret = 0;

    SWPA_DEVICE_SETTING_CHECK(NULL != sn);

    strncpy((char*)buffer, sn, len);
    ret = drv_eeprom_write(DEVID_IIC_EEPROM, E_ADDR_MACHINE_SN, buffer, &len);

    return ret;
}

int swpa_device_read_sn(char* sn,  unsigned int* len)
{
    SWPA_DEVICE_SETTING_CHECK(NULL != sn);
    SWPA_DEVICE_SETTING_CHECK(NULL != len);
	int ret = 0;

    ret = drv_eeprom_read(DEVID_IIC_EEPROM, E_ADDR_MACHINE_SN, (unsigned char*)sn, len);
	if (0 == ret)
	{
		*len = 255 < strlen(sn) ? 255 : strlen(sn);
	}

	return ret;
}

// ע�������ֽ���
static int MyGetIpString(unsigned int ip, char* ip_str, int len)
{
    int ret = 0;
    if( ip_str == NULL ) return -1;

    unsigned char* byte=(unsigned char*)&ip;
    ret = snprintf((char *)ip_str, len, "%d.%d.%d.%d", byte[3],byte[2],byte[1],byte[0]);
    if( ret < 0)
    {
        return -1;
    }
    return 0;
}

// ע�������ֽ���
static int MyGetIpDWord(char* ip_str, unsigned int* ip)
{
    int ret = 0;
    if( ip_str == NULL ) return -1;

    unsigned int tmp[4]={0};

    ret = sscanf((char*)ip_str,"%d.%d.%d.%d",tmp+3,tmp+2,tmp+1,tmp);
    if(ret != 4)
    {
        return -1;
    }

    *ip = (tmp[3]<<24)|(tmp[2]<<16)|(tmp[1]<<8)|tmp[0];
    return 0;
}

int swpa_device_write_ipinfo(char* ip_str, char* netmark_str, char* gateway_str)
{
    unsigned char buffer[12] = {0};
    unsigned int ip = 0;
    unsigned int netmask = 0;
    unsigned int gateway = 0;
    unsigned int len = 4 * 3;
    int ret = 0;

    SWPA_DEVICE_SETTING_CHECK(NULL != ip_str);
    SWPA_DEVICE_SETTING_CHECK(NULL != netmark_str);
    SWPA_DEVICE_SETTING_CHECK(NULL != gateway_str);

    MyGetIpDWord(ip_str, &ip);
    MyGetIpDWord(netmark_str, &netmask);
    MyGetIpDWord(gateway_str, &gateway);

    ip = htonl(ip);
    netmask = htonl(netmask);
    gateway = htonl(gateway);

    memcpy(buffer, (void*)&ip, 4);
    memcpy(buffer + 4, (void*)&gateway, 4);
    memcpy(buffer + 4 + 4, (void*)&netmask, 4);

    ret = drv_eeprom_write(DEVID_IIC_EEPROM, E_ADDR_IP_ADDR, buffer, &len);

    if (ret < 0)
    {
        SWPA_DEVICE_SETTING_PRINT("write ip failed!\n");
        return SWPAR_FAIL;
    }
    return SWPAR_OK;
}

int swpa_device_read_ipinfo(char* ip_str, int ip_len, char* netmark_str, int netmark_len, char* gateway_str, int gateway_len)
{
    unsigned char buffer[12] = {0};
    unsigned int ip = 0;
    unsigned int gateway = 0;
    unsigned int netmask = 0;
    unsigned int len = 4 * 3;
    int ret = 0;

    SWPA_DEVICE_SETTING_CHECK(NULL != ip_str);
    SWPA_DEVICE_SETTING_CHECK(NULL != netmark_str);
    SWPA_DEVICE_SETTING_CHECK(NULL != gateway_str);

    ret = drv_eeprom_read(DEVID_IIC_EEPROM, E_ADDR_IP_ADDR, buffer, &len);
    if (ret < 0)
    {
        SWPA_DEVICE_SETTING_PRINT("read ip failed!\n");
        return SWPAR_FAIL;
    }

    memcpy((void *)&ip, buffer, 4);
    memcpy((void *)&gateway, buffer + 4, 4);
    memcpy((void *)&netmask, buffer + 4 + 4, 4);

    ip = htonl(ip);
    netmask = htonl(netmask);
    gateway = htonl(gateway);

    ret = MyGetIpString(ip, ip_str, ip_len);
    if (ret < 0)
    {
        return SWPAR_FAIL;
    }
    ret = MyGetIpString(netmask, netmark_str, netmark_len);
    if (ret < 0)
    {
        return SWPAR_FAIL;
    }
    ret = MyGetIpString(gateway, gateway_str, gateway_len);
    if (ret < 0)
    {
        return SWPAR_FAIL;
    }
    return SWPAR_OK;
}

int swpa_device_set_bootcount_flag(int is_enable)
{
    SWPA_DEVICE_SETTING_CHECK((0 == is_enable || 1 == is_enable));

    if (0 != drv_device_set_bootcount_flag(is_enable))
    {
        SWPA_DEVICE_SETTING_PRINT("Err: failed to set boot count flag\n");
        return SWPAR_FAIL;
    }

    return SWPAR_OK;
}

int swpa_device_get_bootcount_flag(int* flag)
{
    SWPA_DEVICE_SETTING_CHECK(NULL != flag);

    if (0 != drv_device_get_bootcount_flag(flag))
    {
        SWPA_DEVICE_SETTING_PRINT("Err: failed to get boot count flag\n");
        return SWPAR_FAIL;
    }

    return SWPAR_OK;
}


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
int swpa_device_set_hdd_flag(int has_hdd)
{
	return drv_set_hdd_flag(has_hdd);
}

 /**
 * ����Ӳ�̱�־
 *
 * @param has_hdd [out]   : 1: �豸�ϴ���Ӳ�� 0: �豸��Ӳ�� -1: ���ݶ�ȡ��Ч
 *
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note
 */
int swpa_device_get_hdd_flag(int* has_hdd)
{
	return drv_get_hdd_flag(has_hdd);
}




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
int swpa_device_set_resolution(int resolution_sel)
{
	unsigned int addr;
	unsigned char data;
	int ret = 0;

    SWPA_DEVICE_SETTING_CHECK((0 == resolution_sel || 1 == resolution_sel));

	addr = 0x0C;

	ret = drv_cpld_read(DEVID_SPI_CPLD, addr, &data);

    // 720P
    if (resolution_sel == 0)
    {
        data &= 0x0f;   // ��4λ����
        data |= (1 << 3);   // ��3λ��1
    }
    // 1080P
    else if (resolution_sel == 1)
    {
        data &= 0x17;   // ��3λ����
        data |= (1 << 4); // ��4λ��1
    }
    else
    {
        return -2;
    }

	ret = drv_cpld_write(DEVID_SPI_CPLD, addr, data);
	if (ret< 0)
	{
	   SWPA_DEVICE_SETTING_PRINT("Set resolution failed(cpld io)!!\n");
	   return -1;
	}
	return 0;
}



/**
* ���ָ�Ӧ�ò�Ĭ�ϲ�����־
*
* @param flag [out] : �ָ�Ĭ�ϲ�����־  0 : ���ָ�Ĭ��  1 : �ָ�Ĭ��
*
* @return �ɹ���0  ʧ�ܣ�-1 
*
*/
int swpa_device_get_default_param_flag(int *flag)
{
	SWPA_DEVICE_SETTING_CHECK(NULL != flag);

	int value = -1;
	if (0 == drv_device_get_default_param_flag(&value))
	{
		if (-1 == value)
		{
			SWPA_DEVICE_SETTING_PRINT("Err: got invalid default param flag value\n");
			return SWPAR_FAIL;
		}
		else if (0 == value)
		{
			*flag = 0;
		}
		else
		{	
			*flag = 1;
		}

		return SWPAR_OK;
	}
	else
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to get default param flag\n");
		return SWPAR_FAIL;
	}
}



/**
* ����ָ�Ӧ�ò�Ĭ�ϲ�����־
*
* @return �ɹ� : 0  ʧ��:-1 
*/
int swpa_device_clear_default_param_flag(void)
{
	if (0 == drv_device_clear_default_param_flag())
	{
		return SWPAR_OK;
	}
	else
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to clear default param flag\n");
		return SWPAR_FAIL;
	}
}


/**
* @brief ��ȡ�����豸����尴���¼�
*
*@param [out] event : �����¼���ȡֵ��:0-���£�1-̧��
* @retval 0 : ִ�гɹ�
* @retval 1 : ִ��ʧ��
* @note .
* @see .
*/
int swpa_device_get_reset_key_event(int * event)
{
	SWPA_DEVICE_SETTING_CHECK(NULL != event);

	int value = -1;
	if (0 == drv_gpio_get_input(DEVID_GPIO2, 21, &value))
	{
		if (-1 == value)
		{
			SWPA_DEVICE_SETTING_PRINT("Err: got invalid gpio(DEVID_GPIO2, 21) input: %d\n", value);
			return SWPAR_FAIL;
		}
		else if (0 == value)
		{
			*event = 0;
		}
		else
		{
			*event = 1;
		}

		return SWPAR_OK;
	}
	else
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to get gpio(DEVID_GPIO2, 21) input\n");
		return SWPAR_FAIL;
	}
}



int swpa_device_set_bootsystem(const int sys_type)
{
	SWPA_DEVICE_SETTING_CHECK((0 == sys_type) || (1 == sys_type));

	if (0 != drv_device_set_bootsystem(sys_type))
	{
		SWPA_DEVICE_SETTING_PRINT("Err: failed to set boot system to %d\n",  sys_type);
		return SWPAR_FAIL;
	}

	return SWPAR_OK;
}

