/**
 * @file   drv_device_hal.h
 * @author lujy
 * @date   2013-11-12
 *
 * @brief
 *         �豸��ϢID�ŵ�
 *
 * @note   ��ͷ�ļ��б���ܻ����ʵ����������޸�
 *
 */

#ifndef DRV_DEVICE_HAL_H
#define DRV_DEVICE_HAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/// �豸ID��
typedef enum DRV_DEVICE_TYPE_T {
	DEVID_SPI_EEPROM          = 0xEF000001,     /**< SPI EEPROM(��6467��UBL��Ӧ�ò㲻Ҫ����д��)(���Ǳ���) */
	DEVID_IIC_EEPROM          = 0xEF000002,     /**< I2C EEPROM(�ò�����־����Ϣ) */
	DEVID_SPI_FLASH_DM6467    = 0xEF000003,     /**< SPI Flash(��6467����������)(���Ǳ���) */
	DEVID_SPI_FLASH_DM368     = 0xEF000004,     /**< SPI Flash(��368����������)(���Ǳ���) */
	DEVID_SPI_CPLD            = 0xEF000005,     /**< CPLD(���Ǳ���) */
	DEVID_SPI_FPGA            = 0xEF000006,     /**< FPGA */
	DEVID_IIC_AT88SC          = 0xEF000007,     /**< ����оƬ */
	DEVID_IIC_LM75            = 0xEF000008,     /**< �¶ȴ����� */
	DEVID_IIC_RTC             = 0xEF000009,     /**< ʵʱʱ�� */
	DEVID_LED_RED             = 0xEF00000A,     /**< LED����� */
	DEVID_LED_GREEN           = 0xEF00000B,     /**< LED���̵� */
	DEVID_BUZZER              = 0xEF00000C,     /**< ������(���Ǳ���) */
	DEVID_PHY                 = 0xEF00000D,     /**< �����豸 */
	DEVID_RS485_1             = 0xEF00000E,     /**< ��1·RS485 */
	DEVID_RS485_2             = 0xEF00000F,     /**< ��2·RS485(���Ǳ���) */
    DEVID_GPIO                = 0xEF000010,     /**< GPIO(���Ǳ���) */
    DEVID_USB                 = 0xEF000011,     /**< USB�豸(���Ǳ���) */
    DEVID_DM368               = 0xEF000012,     /**< dm368�豸(���Ǳ���) */
    DEVID_DM6467              = 0xEF000013,     /**< dm6467�豸(���Ǳ���) */
    DEVID_PWM                 = 0xEF000014,     /**< PWM�豸 */
    DEVID_GPIO0               = 0xEF000015,     /**< GPIO bank 0 */
    DEVID_GPIO1               = 0xEF000016,     /**< GPIO bank 1 */
    DEVID_GPIO2               = 0xEF000017,     /**< GPIO bank 2 */
    DEVID_GPIO3               = 0xEF000018,     /**< GPIO bank 3 */
    DEVID_DM8127              = 0xEF000019,     /**< dm8127�豸 */
}DRV_DEVICE_TYPE;

/// �汾��ID
typedef enum DRV_VERID_TYPE_T {
    VER_UBOOT           = 0xFF000001,   /**< DM385 UBOOT�汾�� */
    VER_KERNEL          = 0xFF000002,   /**< DM385 �ں˰汾�� */
    VER_SERIAL_VER      = 0xFF000003,   /**< �豸���к� */
    VER_MAC_ADDR        = 0xFF000004,   /**< MAC��ַ */
}DRV_VERID_TYPE;


#ifdef __cplusplus
}
#endif

#endif /* DRV_DEVICE_HAL_H */
