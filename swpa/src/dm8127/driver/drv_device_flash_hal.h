/**
 * @file   drv_device_flash_hal.h
 * @author lijj
 * @date   2013-03-19
 *
 * @brief
 *         Flash�滮��
 * @note   ע�⣬��ͷ�ļ����ݡ�Mercury�豸��������.docx����д����ο���\n
 *         1��DM6467��Flash��СΪ32MB\n
 *         2��DM368��Flash��СΪ16MB\n
 */

#ifndef DRV_DEVICE_FLASH_HAL_H
#define DRV_DEVICE_FLASH_HAL_H

#ifdef __cplusplus
extern "C"
{
#endif

// ��������
// ��ַ��F_DM6467_ADDR_XX ���ȣ�F_DM6467_LEN_XX
// ��ַ��F_DM368_ADDR_XX ���ȣ�F_DM368_LEN_XX

#define F_DM6467_ADDR_UBOOT         0x00000000 /**< Uboot */
#define F_DM6467_LEN_UBOOT          0x00080000 /**< Uboot���ȣ�512KB */

#define F_DM6467_ADDR_KERNEL_BAK    0x00080000 /**< �ں˱����� */
#define F_DM6467_LEN_KERNEL_BAK     0x00280000 /**< �ں˱��������ȣ�2.5MB */

#define F_DM6467_ADDR_ROOTFS_BAK    0x00300000 /**< ���ļ�ϵͳ������ */
#define F_DM6467_LEN_ROOTFS_BAK     0x00600000 /**< ���ļ�ϵͳ���������ȣ�6MB */

#define F_DM6467_ADDR_FPGA_ROM      0x00900000 /**< FPGA�����ļ� */
#define F_DM6467_LEN_FPGA_ROM       0x00280000 /**< FPGA�����ļ����ȣ�2.5MB */

#define F_DM6467_ADDR_KERNEL        0x00B80000 /**< �ں˾��� */
#define F_DM6467_LEN_KERNEL         0x00300000 /**< �ں˾��񳤶ȣ�3MB */

#define F_DM6467_ADDR_ROOTFS        0x00E80000 /**< ���ļ�ϵͳ */
#define F_DM6467_LEN_ROOTFS         0x00800000 /**< ���ļ�ϵͳ���ȣ�8MB */

#define F_DM6467_ADDR_ARM_APP       0x01680000 /**< ARM�˿�ִ�г��� */
#define F_DM6467_LEN_ARM_APP        0x00180000 /**< ARM�˿�ִ�г��򳤶ȣ�1.5MB */

#define F_DM6467_ADDR_DSP_APP       0x01800000 /**< DSP�˿�ִ�г��� */
#define F_DM6467_LEN_DSP_APP        0x00500000 /**< DSP�˿�ִ�г��򳤶ȣ�5MB */

#define F_DM6467_ADDR_RESERVED      0x01D00000 /**< ������ */
#define F_DM6467_LEN_RESERVED       0x00200000 /**< ���������ȣ�2MB */

#define F_DM6467_ADDR_APP_LOG       0x01F00000 /**< Ӧ�ò���־ */
#define F_DM6467_LEN_APP_LOG        0x00080000 /**< Ӧ�ò���־��512KB */

#define F_DM6467_ADDR_SYS_LOG       0x01F80000 /**< ϵͳ��־ */
#define F_DM6467_LEN_SYS_LOG        0x00080000 /**< ϵͳ��־���ȣ�512KB */

// DM6467 Flash����

// DM368 Flash

#define F_DM368_ADDR_UBL            0x00000000 /**< UBL */
#define F_DM368_LEN_UBL             0x00040000 /**< UBL���ȣ�256KB */

#define F_DM368_ADDR_PARAMS         0x00040000 /**< ������(�������滮�ο�dm368 driverlib��ͷ�ļ�) */
#define F_DM368_LEN_PARAMS          0x00040000 /**< ���������ȣ�256KB */

#define F_DM368_ADDR_UBOOT          0x00080000 /**< Uboot */
#define F_DM368_LEN_UBOOT           0x00080000 /**< Uboot���ȣ�512KB */

#define F_DM368_ADDR_KERNEL         0x00100000 /**< �ں˾��� */
#define F_DM368_LEN_KERNEL          0x00300000 /**< �ں˾��񳤶ȣ�3MB */

#define F_DM368_ADDR_ROOTFS         0x00400000 /**< ���ļ�ϵͳ */
#define F_DM368_LEN_ROOTFS          0x00A00000 /**< ���ļ�ϵͳ���ȣ�10MB */

#define F_DM368_ADDR_RESERVED       0x00E00000 /**< ������ */
#define F_DM368_LEN_RESERVED        0x00100000 /**< ���������ȣ�1MB */

#define F_DM368_ADDR_APP_LOG        0x00F00000 /**< Ӧ�ò���־ */
#define F_DM368_LEN_APP_LOG         0x00080000 /**< Ӧ�ò���־��512KB */

#define F_DM368_ADDR_SYS_LOG        0x00F80000 /**< ϵͳ��־ */
#define F_DM368_LEN_SYS_LOG         0x00080000 /**< ϵͳ��־���ȣ�512KB */

// DM368 Flash����

// ����

#ifdef __cplusplus
}
#endif

#endif /* DRV_DEVICE_FLASH_HAL_H */
