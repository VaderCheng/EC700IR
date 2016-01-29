// ���ļ���������WINDOWS-936
/**
 * @file   drv_device_cpld_hal.h
 * @author lijj
 * @date   2013-02-18
 *
 * @brief
 *         CPLD�Ĵ���
 *
 * @note   �ο���MERCURY����CPLD�Ĵ���˵��.docx��
 *
 */

#ifndef DRV_DEVICE_CPLD_HAL_H
#define DRV_DEVICE_CPLD_HAL_H

#ifdef __cplusplus
extern "C"
{
#endif

// ������CPLD_ADDR_XXX
// ע����Ӳ��������CPLD�Ĵ���Ϊ׼

/// CPLD�Ĵ��������ر�˵�����Ĵ���Ϊ�ɶ�����д�� ���мĴ���Ϊ8λ

/// ��λ���ƼĴ���
#define CPLD_ADDR_RESET     0x00

/// ״̬�Ĵ���(ֻ��)
#define CPLD_ADDR_STAT      0x01

/// FPGA��DM368 Flash���üĴ���
// Ϊ3ʱ������FPGA��
#define CPLD_ADDR_CONF      0x02

/// FPGA���üĴ���(δʹ��)
#define CPLD_ADDR_FPGA      0x03

/// ����(��ʹ��)
#define CPLD_ADDR_REV       0x04

/// ���Ź�ʹ�ܿ��ƼĴ���
#define CPLD_ADDR_WDT_EN    0x05

/// ���Ź���ʱ�����Ĵ���1(��8λ)
#define CPLD_ADDR_WDT_CNT1  0x06

/// ���Ź���ʱ�����Ĵ���2(��8λ)
#define CPLD_ADDR_WDT_CNT2  0x07

/// ����Ĭ�ϼĴ���(ֻ��)
#define CPLD_ADDR_DEFAULT   0x08

/// LED���ƼĴ���
#define CPLD_ADDR_LED       0x09

/// CPLD�汾�Ĵ���(ֻ��)
#define CPLD_ADDR_VERSION   0x0A

/// ͨ�üĴ���
#define CPLD_ADDR_GPREG     0x0B

/// GPIO�Ĵ���1
#define CPLD_ADDR_GPIO1     0x0C

/// GPIO�Ĵ���2
#define CPLD_ADDR_GPIO2     0x0D

// 0x0E 0x0FΪ�����Ĵ���
#ifdef __cplusplus
}
#endif

#endif /* DRV_DEVICE_CPLD_HAL_H */
