/**
 * @file   drv_device_eeprom_hal.h
 * @author lijj
 * @date   2013-02-20
 *
 * @brief
 *         EEPROM�滮��
 * @note   ע�⣬���ļ����ٵ���uboot��Ӧ�ó���ʹ�õ��ıȽ���Ҫ��
 *         ��ַ��������Ӧ�ó���<�Զ��������>д���ⲿ�ֿռ䡣
 *         ����汾����Щ�Ѿ��滮�õĿռ���Զ�/д��
 *
 *          <pre>
 *          EEPROM�ռ乲128KB����ַ��Χ��0x0 ~ 0x1ffff
 *          ���ļ��оٵĵ�ַ��Χ��0x0 ~ 0x2000�����£�
 *          �豸��Ϣ����0x0~0x700
 *          �豸״̬����0x700~0x800
 *          �豸��������0x800~0xc00
 *          �豸������������ 0xc00~0x1000
 *          �豸��־����0x1000~0x2000
 *          </pre>
 */

#ifndef DRV_DEVICE_EEPROM_HAL_H
#define DRV_DEVICE_EEPROM_HAL_H

#ifdef __cplusplus
extern "C"
{
#endif

// �������򣺵�ַ��E_ADDR_XX ���ȣ�E_LEN_XX

// �豸��Ϣ�� 0x0~0x700
#define E_ADDR_MACHINE_SN           0x00000 /**< �豸���к� */
#define E_LEN_MACHINE_SN            0x100   /**< �豸���кų���(256 byte) */

#define E_ADDR_UBL_VER              0x00100 /**< Ubl�汾�� */
#define E_LEN_UBL_VER               0x100   /**< Ubl�汾�ų���(256 byte) */

#define E_ADDR_UBOOT_VER            0x00200 /**< Uboot�汾�� */
#define E_LEN_UBOOT_VER             0x100   /**< Uboot�汾�ų���(256 byte) */

#define E_ADDR_MAC_ADDR             0x00300 /**< Mac��ַ */
#define E_LEN_MAC_ADDR              0x006   /**< Mac��ַ����(6 byte) */

#define E_ADDR_IP_ADDR              0x00310 /**< IP��ַ */
#define E_LEN_IP_ADDR               0x004   /**< IP��ַ����(4 byte) */

#define E_ADDR_GATEWAY_ADDR         0x00314 /**< ���� */
#define E_LEN_GATEWAY_ADDR          0x004   /**< ���س���(4 byte) */

#define E_ADDR_SNMASK_ADDR          0x00318 /**< �������� */
#define E_LEN_SNMASK_ADDR           0x004   /**< �������볤��(4 byte) */

#define E_ADD_REV1                  0x0031c /**< ���� */
#define E_LEN_REV1                  (0x00700 - 0x0031c) /**< ���������� */

// �豸״̬�� 0x700~0x800
#define E_ADDR_LAST_START_STATUS    0x00700 /**< ���һ������״̬ */
#define E_LEN_LAST_START_STATUS     0x004   /**< ���һ������״̬����(4 byte) */

#define E_ADDR_HDD_FLAG             0x00704 /**< Ӳ�̱�־��ַ */
#define E_LEN_HDD_FLAG              0x004   /**<  ���ȡ�*/

#define E_ADD_REV2                  0x00708 /**< ���� */
#define E_LEN_REV2                  (0x00800 - 0x00708) /**< ���������� */


// �豸������ 0x800~0xc00
#define E_ADDR_CRC                  0x00800 /**< ������CRCУ�� */
#define E_LEN_CRC                   0x004   /**< ������CRCУ�鳤��(4 byte) */

#define E_ADDR_FACTORY_FLAG         0x00804 /**< ������� */
#define E_LEN_FACTORY_FLAG          0x004   /**< ������ǳ���(4 byte) */

#define E_ADDR_PARAM_VER            0x00808 /**< �������汾�� */
#define E_LEN_PARAM_VER             0x004   /**< �������汾�ų���(4 byte) */

#define E_ADDR_RTM_FLAG             0x0080c /**< RTM��־ */
#define E_LEN_RTM_FLAG              0x004   /**< RRM��־(4 byte) */

#define E_ADDR_START_CNT            0x00810 /**< �������� */
#define E_LEN_START_CNT             0x004   /**< ������������(4 byte) */

#define E_ADDR_BOOTDELAY            0x00814 /**< Bootdelayֵ */
#define E_LEN_BOOTDELAY             0x004   /**< Bootdelayֵ����(4 byte) */

#define E_ADDR_LAST_BOOTARG         0x00818 /**< Bootargֵ */
#define E_LEN_LAST_BOOTARG          0x100   /**< Bootargֵ����(256 byte) */

#define E_ADD_REV3                  0x00918 /**< ���� */
#define E_LEN_REV3                  (0x00c00 - 0x00918) /**< ���������� */

// �豸���������� 0xc00~0x01000
#define E_ADDR_PARAM_BACKUP         0x00c00 /**< �豸�������ı��ݾ��� */
#define E_LEN_PARAM_BACKUP          0x400   /**< �豸�������ı��ݾ��񳤶�(1024 byte) */

// �豸��־�� 0x01000~0x02000
#define E_ADDR_START_TABLE          0x01000 /**< ����ʱ��� */
#define E_LEN_START_TABLE           0x800   /**< ����ʱ�����(2048 byte) */

#define E_ADDR_UBOOT_LOG            0x01800 /**< Uboot��־��(����ϻ��) */
#define E_LEN_UBOOT_LOG             0x800   /**< Uboot��־����(2048 byte) */

// ����

#ifdef __cplusplus
}
#endif

#endif /* DRV_DEVICE_EEPROM_HAL_H */
