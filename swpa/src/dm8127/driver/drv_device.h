/**
 * @file   drv_device.h
 * @author lujy
 * @date   2013-11-12
 *
 * @brief
 *         �豸���API��
 * @note
 *         1��Ŀǰ������ģ���У�eeprom��������֤�����Ź���
 *         LED�ơ��¶ȡ���ȡ�汾��\n
 *         2���纯��ע�����ر�˵������������ʽ���ʡ�\n
 *		   3��Ϊ�˼�������ƽ̨��flash��cpldģ��Ľӿ��Ա���������Ϊ��ʵ�֡�
 *
 */
#ifndef DRV_DEVICE_H
#define DRV_DEVICE_H

#include "drv_device_hal.h"
#include "drv_device_eeprom_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

//---------------------- EEPROM  START-----------------------------

/**
 * ��ȡEEPROM����
 *
 * @param dev_id [in]  : EEPROM�豸���
 * @param addr [in]    : EEPROM��ַ
 * @param buffer [out] : �������ݻ�����
 * @param len [in/out] : ��ȡ���ݳ���(������lenΪ������ʵ�ʳ���)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_EEPROM��DEVID_IIC_EEPROM\n
 *         2������ͬһʱ�����ͬһIIC�����ϵ��豸��IIC EEPROM��LM75�¶ȴ�������RTCʵʱ��оƬΪͬһIIC���ߡ��ײ������Ѿ�ʵ�ֻ��⡣
 *         3����ַ��Χ�μ�drv_device_eeprom_hal.h�ļ�
 *         4������ȡ����Ϊ0ʱ������������ִ�ж�����
 *         5�����������ش���ʱ�����������ݺͳ�������Ч��
 */
int drv_eeprom_read(DRV_DEVICE_TYPE dev_id, unsigned int addr, unsigned char* buffer, unsigned int* len);

/**
 * дEEPROM����
 *
 * @param dev_id [in]  : EEPROM�豸���
 * @param addr [in]    : EEPROM��ַ
 * @param buffer [in]  : д��EEPROM�����ݻ�����
 * @param len [in/out] : ���ݳ���(������lenΪд���ʵ�ʳ���)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_EEPROM��DEVID_IIC_EEPROM\n
 *         2����ַ��Χ�μ�drv_device_eeprom_hal.h�ļ�
 */
int drv_eeprom_write(DRV_DEVICE_TYPE dev_id, unsigned int addr, unsigned char* buffer, unsigned int* len);

//---------------------- EEPROM  END-----------------------------

//---------------------- ������֤ START-----------------------------
// ע����ΪAT88�������Ժͱ����ԣ�����ַ�����ʽ��;��޷�֪���������밴һ���ԭ��������������AT88������
/*
�������̣�
1����׼��NCֵ��drv_sec_std_read����ַ��0x19�����ȣ�0x07
2����ʼ�����ܣ�drv_sec_crypt_init
3�����ܶ�KEYֵ��drv_sec_crypt_read����ַ��0x40�����ȣ�0x10
4������дNCֵ��drv_sec_crypt_write ��ַ��0x0�����ȣ�0x07
5�����ܶ�NCֵ��drv_sec_crypt_read ��ַ��0x0�����ȣ�0x07


�Զ������ԣ�
1����׼дdrv_sec_std_write ��ַ��0x0a��ֵ������һ���ֽ�����
2����׼��drv_sec_std_read  ��ַ��0x0a���õ���ֵ��д���ֵ�Ա�

*/

/**
 * ��дAT88(��NC��KEY)

 * @param dev_id [in]   : �豸���
 * @param nc [in]       : NCֵ
 * @param key [in]      : ֻ������KEY
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *       1����˿λ���˺󣬲����ٵ��ô˺�����д����
 *       2��nc����Ϊ7��key����Ϊ16
 */
int drv_sec_init_config(DRV_DEVICE_TYPE dev_id, unsigned char* nc, unsigned char* key);

/**
 * ��ʼ�����ܹ���

 * @param dev_id [in]   : �豸���
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *        <b>��׼��/дֻ���ڵ��ñ�����ǰ����</b>��\n
 *        <b>���ñ�������ֻ�ܽ��м��ܶ�/д������˿״̬���۶���˿�Ȳ���
 *
 */
int drv_sec_crypt_init(DRV_DEVICE_TYPE dev_id);

/**
 * ��׼��
 *
 * @param dev_id [in]  : �豸���
 * @param addr [in]    : ��ַ
 * @param data [out]   : ����
 * @param len [in/out] : ����(������lenΪ������ʵ�ʳ���)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_IIC_AT88SC\n
 *            ֻ���ڳ�ʼ�����ܹ���(drv_sec_crypt_init)֮ǰ����!!!
 */
int drv_sec_std_read(DRV_DEVICE_TYPE dev_id, unsigned int addr, unsigned char* data, unsigned int* len);

/**
 * ��׼д
 *
 * @param dev_id [in]  : �豸���
 * @param addr [in]    : ��ַ
 * @param data [in]   : ����
 * @param len [in/out] : ����(������lenΪ������ʵ�ʳ���)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_IIC_AT88SC\n
 *         2��ֻ���ڳ�ʼ�����ܹ���(drv_sec_crypt_init)֮ǰ����!!!\n
 *         3��Ŀǰ�ú���ֻӦ�����Զ��������У���
 */
int drv_sec_std_write(DRV_DEVICE_TYPE dev_id, unsigned int addr, unsigned char* data, unsigned int* len);

/**
 * ���ܶ�
 *
 * @param dev_id [in] : �豸���
 * @param addr [in]    : ��ַ
 * @param data [out]   : ����
 * @param len [in/out] : ����(������lenΪ������ʵ�ʳ���)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         ֻ���ڳ�ʼ�����ܹ���(drv_sec_crypt_init)֮�����!!!
 */
int drv_sec_crypt_read(DRV_DEVICE_TYPE dev_id, unsigned int addr, unsigned char* data, unsigned int* len);

/**
 * ����д
 *
 * @param dev_id [in]  : �豸���
 * @param addr [in]    : ��ַ
 * @param data [out]   : ����
 * @param len [in/out] : ����(������lenΪд���ʵ�ʳ���)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 * ֻ���ڳ�ʼ�����ܹ���(drv_sec_crypt_init)֮�����!!!
 */
int drv_sec_crypt_write(DRV_DEVICE_TYPE dev_id, unsigned int addr, unsigned char* data, unsigned int* len);

/**
 * ��ȡ��˿״̬
 *
 * @param dev_id [in] : �豸���
 * @param data [out]  : ��˿״̬��0��ʾ�Ѿ��۶ϣ���0��ʾδ�۶�
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note   �κ�����¶����Զ���˿״̬
 */
int drv_sec_fuse_read(DRV_DEVICE_TYPE dev_id, unsigned char* data);

/**
 * �۶���˿
 *
 * @param dev_id [in]   : �豸���
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note   !!!ֻ��ȷ���Ѿ���д��AT88�����۶���˿λ!!��\n
 *         һ�����ñ������������ٵ���drv_sec_init_config������\n
 *         Ҳ����˵�������ٴ���дAT88�ˡ�
 */
int drv_sec_fuse_write(DRV_DEVICE_TYPE dev_id);
//---------------------- ������֤ END-----------------------------

//---------------------- ���Ź� START -----------------------------
/**
 * ʹ�ܿ��Ź�
 *
 * @param timeout_ms [in] : ��ʱʱ��(��λΪ����)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *      1��timeout_ms��λΪ���룬�ӿ��ڲ�ʹ��ceil����ת�����룬\
 *          ������õĲ��������������������ȡ����������500msʵ��ת��Ϊ1s��
 *       2��timeout_ms���Ϊ0����ʱʱ��ΪĬ��ֵ��10s����0��Ϊ�����õ�ʱ��
 *       3����ʱʱ�䷶Χ��0 < time <= 60s
 */
int drv_watchdog_enable(int timeout_ms);

/**
 * ��ֹ���Ź�
 *
 *
 * @return �ɹ���0  ʧ�ܣ�-1
 */
int drv_watchdog_disable(void);

/**
 * ���Ź�����(ι��)
 *
 *
 * @return �ɹ���0  ʧ�ܣ�-1
 */
int drv_watchdog_handshake(void);
//---------------------- ���Ź� END -----------------------------

//---------------------- LED START -----------------------------
/**
 * LED����
 *
 * @param dev_id [in]   : �豸ID��
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_id���ҽ���: DEVID_LED_RED��DEVID_LED_GREEN
 */
int drv_led_on(DRV_DEVICE_TYPE dev_id);

/**
 * LED����
 *
 * @param dev_id [in]   : �豸ID��
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_id���ҽ���: DEVID_LED_RED��DEVID_LED_GREEN
 */
int drv_led_off(DRV_DEVICE_TYPE dev_id);
//---------------------- LED END -----------------------------

//---------------------- GPIO START -----------------------------
/**
 * ����GPIO���ֵ
 *
 * @param dev_id [in]  : �豸���
 * @param gpio_id [in] : GPIO���
 * @param value [in]   : ��GPIO��Ӧ��ֵ
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *        1��dev_id: DEVID_GPIO0��DEVID_GPIO1��DEVID_GPIO2��DEVID_GPIO3\n
 *        2��valueֻ����0��1
 */
int drv_gpio_set_output(DRV_DEVICE_TYPE dev_id, int gpio_id, int value);

/**
 * ��ȡGPIO����ֵ
 *
 * @param dev_id [in]  : �豸���
 * @param gpio_id [in] : GPIO���
 * @param value [out]  : ��GPIO��Ӧ��ֵ
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *        1��dev_id: DEVID_GPIO0��DEVID_GPIO1��DEVID_GPIO2��DEVID_GPIO3\n
 *        2��valueΪ��ȡ��ֵ��0��1\n
 *        3��ע�⣡��GPIO��Ϊ����ڣ������ڳ�����������ֵ��ֻ�ܶ�ȡ��ֵ
 */
int drv_gpio_get_input(DRV_DEVICE_TYPE dev_id, int gpio_id, int* value);

/**
 * ��ȡGPIO״̬(�����ӿڣ�����ʵ��)
 *
 * @param dev_id [in]  : �豸���
 * @param gpio_id [in] : GPIO���
 * @param status [out]  : ��GPIO ��״̬   ����״̬:0     ���״̬:1
 *
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *        1��dev_id: DEVID_GPIO0��DEVID_GPIO1��DEVID_GPIO2��DEVID_GPIO3\n
 *
 */
int drv_gpio_get_status(DRV_DEVICE_TYPE dev_id, int gpio_id,int *status);

/**
 * ��ȡGPIO״̬(�����ӿڣ�����ʵ��)
 *
 * @param dev_id [in]  : �豸���
 * @param gpio_id [in] : GPIO���
 * @param status [out]  : ��GPIO ��ֵ��0��1
 *
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *        1��dev_id: DEVID_GPIO0��DEVID_GPIO1��DEVID_GPIO2��DEVID_GPIO3\n
 *
 */
int drv_gpio_get_value(DRV_DEVICE_TYPE dev_id, int gpio_id,int *value);


//---------------------- GPIO END-----------------------------

//---------------------- FPGA START -----------------------------

/// һ������FPGA�Ĵ���������
#define MAX_FPGA_REG 64

/// FPGA�Ĵ����ṹ��
/**
@note
<pre>
addr��4λ���12λ��ַ��ɣ���ο�FPGA˵���ĵ�������֯��ַ��ʽ
��ʽ��  |   16     |   4  |          12       |
        |    0     | cmd  |       addr        |
��ʹ�����еĺ꣺
</pre>
\code
// ��ַת��
// addr����ַ
#define FPGA_W(addr) ((0x1 << 12) | (addr))
#define FPGA_R(addr) ((0x9 << 12) | (addr))

// num��ADоƬ���(1~4) addr����ַ
#define AD_W(num, addr) ((0 << 15) | (((num)-1) << 13) | (addr))
#define AD_R(num, addr) ((1 << 15) | (((num)-1) << 13) | (addr))
\endcode
ʾ����AD_W(1, 0x4)��ʾд��1ƬAD��0x4��ַ

*/
typedef struct {
    unsigned int addr;  /**< FPGA�Ĵ�����ַ */
    unsigned int data;  /**< FPGA�Ĵ�����Ӧ����ֵ */
} FPGA_REG;

/// FPGA��Ϣ�ṹ�壬ռ��516�ֽڿռ�
typedef struct _FPGA_STRUCT{
    unsigned int count;           /**< �Ĵ������� */
    FPGA_REG regs[MAX_FPGA_REG];  /**< �Ĵ����ṹ�� */
#ifdef _cpluscplus
    _FPGA_STRUCT
    {
        count = 0;
    };
#endif
} FPGA_STRUCT;

/**
 * ����FPGA����
 *
 * @param dev_id [in]   : FPGA�豸���
 * @param fpga_img [in] : FPGA���񻺳���ָ��(����FPGA�ļ�)
 * @param fpga_len [in] : ��������С
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_FPGA
 */
int drv_fpga_load(DRV_DEVICE_TYPE dev_id, unsigned char* fpga_img, int fpga_len);

/**
 * ��FPGA�Ĵ���
 *
 * @param dev_id [in]      : FPGA�豸���
 * @param fpga_info [out]  : FPGA��Ϣ�ṹ�壬�ο�FPGA_STRUCT
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_FPGA\n
 *         2��һ�����ɶ�64���Ĵ���\n
 *         3����������ע���ǼĴ�����ַ�����ڴ�������ݣ�����������ֵ��\
 *            ���ظýṹ��ʱ�������ǼĴ���ʵ��ֵ
 *
 */
int drv_fpga_read(DRV_DEVICE_TYPE dev_id, FPGA_STRUCT* fpga_info);

/**
 * һ�ζ�ȡһ��FPGA�Ĵ���
 *
 * @param dev_id [in] : FPGA�豸���
 * @param addr [in]   : �Ĵ�����ַ
 * @param data [out]   : ����
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_FPGA
 *
 */
int drv_fpga_read_single(DRV_DEVICE_TYPE dev_id, unsigned int fpga_reg, unsigned int* fpga_data);

/**
 * дFPGA
 *
 * @param dev_id [in]    : FPGA�豸���
 * @param fpga_info [in] : FPGA��Ϣ�ṹ�壬�ο�FPGA_STRUCT
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_FPGA\n
 *         2����������FPGA�Ĵ�����ͨ����������������
 *         3��һ����������64���Ĵ���
 *
 * @sa �ṹ��� FPGA_REG
 */
int drv_fpga_write(DRV_DEVICE_TYPE dev_id, FPGA_STRUCT* fpga_info);

/**
 * ��ȡFPGA�汾��
 *
 * @param dev_id [in]   : FPGA�豸���
 * @param version [out] : FPGA�汾��
 * @param len [in]      : ����
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_FPGA
 *         2������ĳ�������16�ֽڣ�ʵ�ʳ�����ʹ��strlen��ȡ
 *
 */
int drv_fpga_get_version(DRV_DEVICE_TYPE dev_id, unsigned char* version, int len);


// ����ʹ��
int drv_fpga_get_reset(DRV_DEVICE_TYPE dev_id);

//---------------------- FPGA END -----------------------------

//---------------------- ����ģ�� START -----------------------------
/**
 * ��ȡ����״̬
 *
 * @param dev_id [in]  : �豸���
 * @param status [out] : ����״̬(0���޽��� 1������)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *        dev_id: DEVID_PHY
 *
 */
int drv_phy_get_status(DRV_DEVICE_TYPE dev_id, unsigned int* status);

/**
 * ����485����
 *
 * @param dev_id [in] : �豸���
 * @param dir [in]    : ����(0: ����  1: ���)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *        dev_id���ҽ���: DEVID_RS485_1��DEVID_RS485_2
 *
 */
int drv_device_set_rs485_direction(DRV_DEVICE_TYPE dev_id, int dir);

/**
 * ��λָ���豸
 *
 * @param dev_id [in] : �豸���
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *        1�����ݲ�ͬID�Ÿ�λ��ͬ���豸
 */
int drv_device_reset(DRV_DEVICE_TYPE dev_id);

/**
 * ��ȡ�豸�¶�
 *
 * @param dev_id [in]       : �豸���
 * @param temperature [out] : �¶�
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰֻ���ǣ�DEVID_IIC_LM75
 *         2������ͬһʱ�����ͬһIIC�����ϵ��豸��IIC EEPROM��LM75�¶ȴ�������RTCʵʱ��оƬΪͬһIIC���ߡ��ײ������Ѿ�ʵ�ֻ��⡣
 */
int drv_device_get_temperature(DRV_DEVICE_TYPE dev_id, int* temperature);

/**
 * ʹ��PWM�첽֪ͨ�ź�
 *
 * @param dev_id [in]  : �豸���
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰֻ���ǣ�DEVID_PWM
 */
int drv_device_get_enable_pwm_signal(DRV_DEVICE_TYPE dev_id);

/**
 * ��ȡPWM�Ĵ���ֵ
 *
 * @param dev_id [in]  : �豸���
 * @param info [out]   : �Ĵ���ֵ
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰֻ���ǣ�DEVID_PWM
 *         2��info��Ŀǰ��Ӧ����Ҫ�ǹ����ڴ�ĵ�ֵַ��
 */
int drv_device_get_pwminfo(DRV_DEVICE_TYPE dev_id, unsigned int* info);

 /**
 * ��ȡ�̼��汾��
 *
 * @param ver_id [in]   : ���ְ汾�Ŷ�Ӧ��ID
 * @param version [out] : �汾��
 * @param len [in/out]  : ����(���ص�lenΪʵ�ʵİ汾����)
 *
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note
 *        1��ͨ����ͬ��ID�Ż�ȡ��ͬ�İ汾�ţ�����UBL��UBOOT���ںˡ�����������ο�DRV_VERID_TYPE�г�������ID\n
 *        2�������len��������Ϊ256�ֽ�
 *
 * @sa DRV_VERID_TYPE
 */
int drv_device_get_firmaware_version(DRV_VERID_TYPE ver_id, unsigned char* version, int* len);

/**
 * ���sd��״̬(���ϻ�γ�)
 *
 * @param dev_id [in]  : �豸���
 * @param status [out] : ����״̬(1���γ� 0������)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *        dev_id: DEVID_SD_CARD
 *
 */
int drv_sdcard_check_status(DRV_DEVICE_TYPE dev_id,int *status);


//---------------------- ����ģ�� END -----------------------------

//---------------------- �Զ���Ȧ���� START -----------------------------
/**
 * ����pwm�����Զ���Ȧ�򿪻�ر�
 *
 * @param dev_id [in]  : �豸���
 * @param duty_val [in]    : ռ�ձ�ֵ(��λΪǧ��֮һ)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *        dev_id: DEVID_PWM\n
 *        duty_valΪռ�ձȣ���λΪǧ��֮һ����duty_valΪ900��ʾǧ��֮�Ű�\n
 *        �򿪣�����Լ750����������Ϊ0\n
 *���������رգ�����Լ750����������Ϊ1000\n
 *        ע�⣡��������ֻ���ڿ����Զ���Ȧ�򿪻�رգ����߱��Զ����ڹ��ܡ�\n
 *        M3��ʹ���Զ����ƣ����ñ��������ܻ��M3�Ŀ������Ӱ�졣��ע�⡣
 */
int drv_autoiris_pwm(DRV_DEVICE_TYPE dev_id, int duty_val);

//---------------------- �Զ���Ȧ���� END -----------------------------

//---------------------- CPLD ��flash���ȿ�ʵ�ֽӿ� START-----------------------------

//---------------------- CPLD START-----------------------------

/**
 * ��ȡCPLD����
 *
 * @param dev_id [in] : CPLD�豸���
 * @param addr [in]   : CPLD��ַ
 * @param data [out]  : ��ȡ������(ע������Ϊ8λ)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_CPLD\n
 */
int drv_cpld_read(DRV_DEVICE_TYPE dev_id, unsigned int addr, unsigned char* data);

/**
 * дCPLD����
 *
 * @param dev_id [in] : CPLD�豸���
 * @param addr [in]   : CPLD��ַ
 * @param data [in]   : д�������(ע������Ϊ8λ)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_CPLD\n
 */
int drv_cpld_write(DRV_DEVICE_TYPE dev_id, unsigned int addr, unsigned char data);

/**
 * ��ȡCPLD�汾��
 *
 * @param dev_id [in]   : CPLD�豸���
 * @param version [out] : CPLD�汾��
 * @param len [in]      : ����
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_CPLD\n
 *         2�����ȱ���Ϊ1�ֽ�
 */
int drv_cpld_get_version(DRV_DEVICE_TYPE dev_id, unsigned char* version, int len);

//---------------------- CPLD END-----------------------------

//---------------------- FLASH START-----------------------------

/**
 * ��ȡFlash����
 *
 * @param dev_id [in]  : Flash�豸���
 * @param addr [in]    : Flash��ַ
 * @param buffer [out] : �������ݻ�����
 * @param len [in/out] : ��ȡ���ݳ���(������lenΪ������ʵ�ʳ���)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_FLASH_DM6467��DEVID_SPI_FLASH_DM368\n
 */
int drv_flash_read(DRV_DEVICE_TYPE dev_id, unsigned int addr, unsigned char* buffer, unsigned int* len);

/**
 * дFlash����
 *
 * @param dev_id [in]  : Flash�豸���
 * @param addr [in]    : Flash��ַ
 * @param buffer [in]  : д��Flash�����ݻ�����
 * @param len [in/out] : ���ݳ���(������lenΪд���ʵ�ʳ���)
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_FLASH_DM6467��DEVID_SPI_FLASH_DM368\n
 */
int drv_flash_write(DRV_DEVICE_TYPE dev_id, unsigned int addr, unsigned char* buffer, unsigned int* len);

/**
 * ����Flash
 *
 * @param dev_id [in]  : Flash�豸���
 * @param addr [in]    : Ҫ������Flash��ַ
 * @param len [in/out] : ������Flash����
 *
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 *
 * @note
 *         1��dev_idĿǰ���ҽ���DEVID_SPI_FLASH_DM6467��DEVID_SPI_FLASH_DM368\n
 */
int drv_flash_erase(DRV_DEVICE_TYPE dev_id, unsigned int addr, unsigned int* len);


//---------------------- FLASH END -----------------------------

// ---------------------- Ӳ�̱�־ START-----------------------------
 /**
 * ����Ӳ�̱�־
 *
 * @param has_hdd [in]   : 1: �豸�ϴ���Ӳ�� 0: �豸��Ӳ��
 *
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note
 */
int drv_set_hdd_flag(int has_hdd);

 /**
 * ��ȡӲ�̱�־
 *
 * @param has_hdd [out]   : 1: �豸�ϴ���Ӳ�� 0: �豸��Ӳ�� -1: ��־�Ƿ����޷��ж��Ƿ���Ӳ��
 *
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note
 */
int drv_get_hdd_flag(int* has_hdd);

// ---------------------- Ӳ�̱�־��END-----------------------------

//---------------------- CPLD ��flash���ȿ�ʵ�ֽӿ� END-----------------------------

#ifdef __cplusplus
}
#endif

#endif
