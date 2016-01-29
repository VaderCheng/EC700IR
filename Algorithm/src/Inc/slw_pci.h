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

#ifndef __SLW_PCI__
#define __SLW_PCI__

#ifdef __cplusplus
extern "C" {
#endif

// PCI�����ṩ�����ж��ź�
enum{
	SLW_SIG_NONE = 0,
	SLW_SIG_PCI_GETR, // �ȴ����ж�
	SLW_SIG_PCI_WAIT, // �ȴ������ж�
	SLW_SIG_MAX,
};

// ע�����µ�����Ϊ��������ˣ��Ӷ�Ϊ���������
enum {
  SWPCI_IOCTL_LIST_BUFS = 2000, // ȡ�������ڴ�����Ϣ
  SWPCI_IOCTL_GETW_BUF, // ���˻�ȡȡ�ÿ����ڴ��
  SWPCI_IOCTL_PUTW_BUF, // ����д�꣬�Ż�
  SWPCI_IOCTL_GETR_BUF, // �Ӷ�ȡ��Ȩ��
  SWPCI_IOCTL_PUTR_BUF, // �Ӷ�д�꣬�Ż�
  SWPCI_IOCTL_WAIT_BUF, // ���˵ȴ�����
  SWPCI_IOCTL_FREE_BUF, // �����ͷ�
  SWPCI_IOCTL_LAST,
};

// PCI�ڴ�����Ϣ�ṹ
typedef struct swpci_buffer {
  unsigned int index; // ���
  unsigned int size; // ����
  unsigned int stat; // ״̬
  unsigned int req; // �����
  unsigned int sent; // �Ƿ���֪ͨ
  unsigned long phys; // �����ַ
  unsigned long virt; // �����ַ
  void *user; // �û����ַ
} swpci_buffer;

#define NETLINK_SOCKET_PCI_EVENT 20

#define NETLINK_TYPE_PCI_PID   0x10
#define NETLINK_TYPE_PCI_CLOSE 0x20
#define NETLINK_TYPE_PCI_GETR  0x30
#define NETLINK_TYPE_PCI_RTN_VALUE  0x40

#ifdef __KERNEL__

#include <linux/config.h>

#define DM6467_PCI_BASEADDR      0x01C1A000
#define DM6467_PCIBE_PCIBAR4TRL (0x01D0 + DM6467_PCI_BASEADDR)
#define DM6467_PCIBE_PCIBAR5TRL (0x01D4 + DM6467_PCI_BASEADDR)

#define RBUFFER_PHYS_START "0x8A800000" // ��PCI��DDR2�ϵ�д��ַ 120~128M
#define RBUFFER_PHYS_END   "0x8B000000"
#define WBUFFER_PHYS_START "0x8B000000" // ��PCI��DDR2�ϵĶ���ַ 128~136M
#define WBUFFER_PHYS_END   "0x8B800000"

#define PCI_FLAG_WAIT  0x66bb66bb // һ���ȴ�
#define PCI_FLAG_READY 0x77cc77cc // ����׼��
#define PCI_FLAG_DONE  0x88dd88dd // ��������

#define PCI_STATUS_WAIT  0x11111111 // �ȴ�
#define PCI_STATUS_READY 0x22222222 // ׼��
#define PCI_STATUS_DONE  0x33333333 // ���

#define PCI_TIMER 1 // PCI��ʱ����ʱ��(��)

// master PCI
#ifdef CONFIG_PCI
#define GPIO_READ_INT  2
#define GPIO_WRITE_INT 3

#define MAX_RBUFFER_LEN   (8*1024*1024) // ��PCI���Ļ�������С
#define MAX_WBUFFER_LEN   (8*1024*1024) // ��PCIд�Ļ�������С

#define PCI_DMA_PHYS_START "0x8B800000" // ��PCI��DDR2�ϵ�д��ַ 184~192M
#define PCI_DMA_PHYS_END   "0x8C000000"

#define PRD_START_PHYS_ADDR (pci_dma_phys + 0x000000)
#define PRD_START_VIRT_ADDR (pci_dma_virt + 0x000000)

#define DMA_START_PHYS_ADDR (pci_dma_phys + 0x400000)
#define DMA_START_VIRT_ADDR (pci_dma_virt + 0x400000)
#else
// slave PCI
#define GPIO_READ_INT  2
#define GPIO_WRITE_INT 3
#endif // CONFIG_PCI

#define BUFFER_TIMEOUT 5

#define GPIO_ACK       1 // �ظ�ȷ��
#define GPIO_RST       0 // ��λ

#define ENABLE_BUFFER  1
#define DISABLE_BUFFER 0

#endif // __KERNEL__

#ifdef __cplusplus
}
#endif

#endif // __SLW_PCI__
