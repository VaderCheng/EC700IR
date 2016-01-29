/**
 * @file   drv_dsplink.h
 * @author lijj
 * @date   2013-02-19
 *
 * @brief
 *         DspLink API������ƽ̨�������ļ��ӿ�
 *
 * @note  DSPLINK��ڵ�ַ�Ѿ��޸�Ϊ0x9FFFF000
 * @todo
           һ�����CheckHDVICP01()�����Ƿ���Ҫ��
 */

#ifndef DRV_DSPLINK_H
#define DRV_DSPLINK_H

#ifdef __cplusplus
extern "C"
{
#endif
 
/**
 * ��ʼ��DSP����������ִ�г�����ص�DSP������
 *
 * @param dsp_img [in] : DSP���񻺳���ָ��(����DSP�ļ�)
 * @param len [in]     : ��������С
 *
 * @return �ɹ���0 ʧ�ܣ�-1
 *
 * @note
 *         �����dsp_img�ǽ��ܺ��dsp������dsp�����Ǽ��ܹ��ģ��������н���
 */
int drv_dsplink_setup(unsigned char* dsp_img, int len);

/**
 * ����DSP����
 *
 * @param phy_addr [in/out] : ����/������ݵ������ַ
 * @param timeout_ms [in]   : ��ʱʱ��(��λΪ����)
 *
 * @note
 *         1��������ֻ������ARM��DSP֮�䴫��phy_addr�����漰��Э����ơ�\n
 *         2�����߳�����£��˺�����Ҫ��<b>������</b>��֤ԭ�Ӳ�����\n
 *            �Ա�ÿ��DSP�����ӦARM�˵Ĵ���
 * @return �ɹ���0  ʧ�ܣ�-1 �����Ƿ���-2
 */
int drv_dsplink_doprocess(unsigned int phy_addr, int timeout_ms);

/**
 * �˳�DSPLink
 *
 * @return �ɹ���0 ʧ�ܣ�-1
 */
int drv_dsplink_exit(void);

#ifdef __cplusplus
}
#endif

#endif /* DRV_DSPLINK_H */
