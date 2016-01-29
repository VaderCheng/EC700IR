// ���ļ������ʽ����ΪWINDOWS-936��ʽ

/*
 * gpio.c
 * author: pxw
 * date: 2010-06-13
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "config.h"

#ifndef __SLW_DEV__
#define __SLW_DEV__

#ifdef __cplusplus
extern "C" {
#endif

#include "slw_gpio.h"
#include "slw_spi.h"
#include "slw_gpio2spi.h"
#include "slw_gpio2iic.h"
#include "slw_vsc7385.h"
#include "slw_i2c_dev.h"

#define GPIO_WATCHDOG   5

enum {
  SWDEV_IOCTL_WATCHDOG = 0, // ι��

  SWDEV_IOCTL_CPLD_READ, // ��CPLD
  SWDEV_IOCTL_CPLD_WRITE, // дCPLD

  SWDEV_IOCTL_LM75_GET_TEMP, // ȡ���¶�

  SWDEV_IOCTL_EEPROM_READ, // ��EEPROM
  SWDEV_IOCTL_EEPROM_WRITE, // дEEPROM

  SWDEV_IOCTL_AT88SC_INIT_CONFIG, // ��ʼ��AT88
  SWDEV_IOCTL_AT88SC_EEPROM_INIT, // ��ʼ��AT88��EEPROM
  SWDEV_IOCTL_AT88SC_STD_WRITE, // ��׼дAT88
  SWDEV_IOCTL_AT88SC_STD_READ, // ��׼��AT88
  SWDEV_IOCTL_AT88SC_EEPROM_CRYTP_WRITE, // ����дAT88
  SWDEV_IOCTL_AT88SC_EEPROM_CRYTP_READ, // ���ܶ�AT88

  SWDEV_IOCTL_DS2460_INIT, // ��ʼ��DS2460
  SWDEV_IOCTL_DS2460_READ_SERIAL_NUMBER, // ��DS2460�����к�
  SWDEV_IOCTL_DS2460_COMPUTE_SHA, // Ӳ����SHA
  SWDEV_IOCTL_DS2460_COMPUTE_SHAEE, // �����SHA
  SWDEV_IOCTL_DS2460_WRITE_INPUT_SHA, // д���������
  SWDEV_IOCTL_DS2460_READ_INPUT_SHA, // ������������
  SWDEV_IOCTL_DS2460_MATCH_MAC, // ƥ��MAC
  SWDEV_IOCTL_DS2460_READ_MAC, // ��MAC
  SWDEV_IOCTL_DS2460_WRITE_MAC, // дMAC
  SWDEV_IOCTL_DS2460_WRITE_SSECRET,  // дSSECRET
  SWDEV_IOCTL_DS2460_WRITE_ESECRET1, // дESECRET1
  SWDEV_IOCTL_DS2460_WRITE_ESECRET2, // дESECRET2
  SWDEV_IOCTL_DS2460_WRITE_ESECRET3, // дESECRET3
  SWDEV_IOCTL_DS2460_TRANSFER_SECRET, // �ı�SECRET�Ĵ洢

  SWDEV_IOCTL_AD9949_WRITE, // дAD9949

  SWDEV_IOCTL_DMA_CONVERT_IMAGE, // ͨ��DMA��תYUVͼ��(ʹ��CONVERT_IMAGE_STRUCT�ṹ)
                                 // ��x��=copy�ĳ���, ���߲��ܴ���64*1024

  SWDEV_IOCTL_DMA_COPY_JPEG, // ͨ��DMA��֯���ݸ�JPEG CODER(ʹ��CONVERT_IMAGE_STRUCT�ṹ)
                                 // ��x��=copy�ĳ���, ���߲��ܴ���64*1024

  SWDEV_IOCTL_DMA_COPY_H264, // ͨ��DMA��֯���ݸ�H264 CODER(ʹ��CONVERT_IMAGE_STRUCT�ṹ)
                                 // ��x��=copy�ĳ���, ���߲��ܴ���64*1024

  SWDEV_IOCTL_DMA_COPY_YUV, // YUV����copy(ʹ��CONVERT_IMAGE_STRUCT�ṹ)
                            // ��x��=copy�ĳ���, ���߲��ܴ���64*1024

  SWDEV_IOCTL_DMA_ROTATION_YUV, // Y copy һ�벢������ת90��, UV copy���е��Ҳ�������ת
                                // (!!!ʹ��ROTATION_YUV_STRUCT�ṹ !!!)
                                // ��x��=copy�ĳ���, ���߲��ܴ���64*1024

  SWDEV_IOCTL_DMA_PCI_READ, // ͨ��DMA��PCI���ݵ�ָ����ַ(ʹ��CONVERT_IMAGE_STRUCT�ṹ)
                            // ��x��=copy�ĳ���, ���߲��ܴ���64*1024

  SWDEV_IOCTL_DMA_PCI_WRITE, // ͨ��DMA��ָ����ַ������д��PCI(ʹ��CONVERT_IMAGE_STRUCT�ṹ)
                             // ��x��=copy�ĳ���, ���߲��ܴ���64*1024

  SWDEV_IOCTL_DMA_COPY, // DMA copy(!!! ʹ��DMACOPY_STRUCT�ṹ !!!)

  SWDEV_IOCTL_LOAD_FPGA_FROM_JTAG, // ����FPGA

  SWDEV_IOCTL_DMA_COPY_UV, // copy UV���ֱ��U��V(ʹ��COPY_UV_STRUCT�ṹ)
                            // ��x��=copy�ĳ���, ���߲��ܴ���64*1024

  SWDEV_IOCTL_GET_PHOTO_FROM_SLAVE, // ��CPU����ץ��

  SWDEV_IOCTL_QDMA_COPY, // QDMA copy(!!! ʹ��DMACOPY_STRUCT�ṹ !!!)
  SWDEV_IOCTL_QDMA_COPY_JPEG, // ͨ��DMA��֯���ݸ�JPEG CODER(ʹ��CONVERT_IMAGE_STRUCT�ṹ)
                                 // ��x��=copy�ĳ���, ���߲��ܴ���64*1024

#ifdef _CAMERA_PIXEL_500W_
  SWDEV_IOCTL_AD9974_WRITE, // дAD9974
#elif defined(_CAMERA_PIXEL_200W_25FPS_)
  SWDEV_IOCTL_GET_PHY_STATUS,
  SWDEV_IOCTL_AD9974_WRITE,
#else
  SWDEV_IOCTL_GET_PHY_STATUS, // ȡ��PHY��״̬
#endif

  SWDEV_IOCTL_LAST,
};

typedef struct {
  u32 addr;
  u32 data;
} REG_STRUCT;

typedef struct {
  u8 data[EEPROM_AT24C1024_PAGE_SIZE];
  u32 addr;
  u32 len;
} EEPROM_STRUCT;

typedef struct {
  u8 data;
  u8 addr;
} CPLD_STRUCT;

typedef struct {
  union {
    u8 buf[64];
    u8 sn[8];
    u8 key[8];
    u8 mac[20];
  } data;
} DS2460_STRUCT;

typedef struct {
  u8 data[32*4];
  u32 addr;
  u32 len;
  u32 ok_len;
} AT88SC_STRUCT;

typedef struct {
  u32 src_addr; // Դ�����ַ
  u32 dst_addr; // Ŀ�������ַ
  u32 width; // Դͼ���
  u32 height; // Դͼ���
} CONVERT_IMAGE_STRUCT;

typedef struct {
  u32 y_src; // Y��Դ�����ַ
  u32 y_dst; // Y��Ŀ�������ַ
  u32 uv_src; // UV��Դ�����ַ
  u32 uv_dst; // UV��Ŀ�������ַ
  u32 width; // Դͼ���
  u32 height; // Դͼ���
} ROTATION_YUV_STRUCT;

typedef struct {
  u32 uv_src; // UV��Դ�����ַ
  u32 u_dst; // U��Ŀ�������ַ
  u32 v_dst; // V��Ŀ�������ַ
  u32 width; // Դͼ���
  u32 height; // Դͼ���
} COPY_UV_STRUCT;

/****************************
 |        cidx=24       |
 |bidx=8|
 |a=5|
 ooooo...ooooo...ooooo... -
 ooooo...ooooo...ooooo... c=3
 ooooo...ooooo...ooooo... -
 |          b=3         |
****************************/
typedef struct {
  u32 src; // Դ�����ַ
  u32 dst; // Ŀ�������ַ
  short a; // һά(Ԫ�ش�С)
  short b; // ��ά(��)
  short c; // ��ά(��)
  short sbidx; // Դ��һά(Ԫ��)�ļ��
  short scidx; // Դ�Ķ�ά(��)�ļ��
  short dbidx; // Ŀ�ĵ�һά(Ԫ��)�ļ��
  short dcidx; // Ŀ�ĵĶ�ά(��)�ļ��
} DMACOPY_STRUCT;

// PHY��״̬�ṹ
typedef struct {
	u8 linked[2]; // 0Ϊ�����ӣ�1Ϊ����
	u8 speed[2];  // 0x00 = 10Mpbs, 0x01 = 100Mpbs, 0x02 = 1000Mpbs, 0x03 = Reserved
} PHY_STATUS_STRUCT;

#ifdef __cplusplus
}
#endif

#endif // __SLW_DEV__
