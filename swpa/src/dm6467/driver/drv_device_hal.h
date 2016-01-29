/**
 * @file   drv_device_hal.h
 * @author lijj
 * @date   2013-02-18
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
	DEVID_SPI_EEPROM          = 0xEF000001,     /**< SPI EEPROM(��6467��UBL��Ӧ�ò㲻Ҫ����д��) */
	DEVID_IIC_EEPROM          = 0xEF000002,     /**< I2C EEPROM(�ò�����־����Ϣ) */
	DEVID_SPI_FLASH_DM6467    = 0xEF000003,     /**< SPI Flash(��6467����������) */
	DEVID_SPI_FLASH_DM368     = 0xEF000004,     /**< SPI Flash(��368����������) */
	DEVID_SPI_CPLD            = 0xEF000005,     /**< CPLD */
	DEVID_SPI_FPGA            = 0xEF000006,     /**< FPGA */
	DEVID_IIC_AT88SC          = 0xEF000007,     /**< ����оƬ */
	DEVID_IIC_LM75            = 0xEF000008,     /**< �¶ȴ����� */
	DEVID_IIC_RTC             = 0xEF000009,     /**< ʵʱʱ�� */
	DEVID_LED_RED             = 0xEF00000A,     /**< LED����� */
	DEVID_LED_GREEN           = 0xEF00000B,     /**< LED���̵� */
	DEVID_BUZZER              = 0xEF00000C,     /**< ������ */
	DEVID_PHY                 = 0xEF00000D,     /**< �����豸 */
	DEVID_RS485_1             = 0xEF00000E,     /**< ��1·RS485 */
	DEVID_RS485_2             = 0xEF00000F,     /**< ��2·RS485 */
    DEVID_GPIO                = 0xEF000010,     /**< GPIO */
    DEVID_USB                 = 0xEF000011,     /**< USB�豸 */
    DEVID_DM368               = 0xEF000012,     /**< dm368�豸 */
    DEVID_DM6467              = 0xEF000013,     /**< dm6467�豸 */
    DEVID_PWM                 = 0xEF000014,     /**< PWM�豸 */
}DRV_DEVICE_TYPE;

/// �汾��ID
typedef enum DRV_VERID_TYPE_T {
    VER_UBL_DM6467      = 0xFF000001,   /**< DM6467 UBL�汾�� */
    VER_UBOOT_DM6467    = 0xFF000002,   /**< DM6467 UBOOT�汾�� */
    VER_KERNEL_DM6467   = 0xFF000003,   /**< DM6467 �ں˰汾�� */
    VER_SLWDEV_DM6467   = 0xFF000004,   /**< DEV�豸�����汾�� */
    VER_SERIAL_VER      = 0xFF000005,   /**< �豸���к� */
    //VER_HARDWARE_VER    = 0xFF000006,   /**< Ӳ���汾�� */ // ע��Ӳ���汾�������ἰ����ȥ��
    VER_MAC_ADDR        = 0xFF000007,   /**< MAC��ַ */
}DRV_VERID_TYPE;

#ifdef __cplusplus
}
#endif

#endif /* DRV_DEVICE_HAL_H */
