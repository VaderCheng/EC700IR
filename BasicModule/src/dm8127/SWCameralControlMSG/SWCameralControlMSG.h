#ifndef CSWCAMERALCONTROLMSG_H
#define CSWCAMERALCONTROLMSG_H
#include "SWObject.h"
#include "SWMessage.h"
#include "SWCameraControlParameter.h"


class CSWCameralControlMSG : public CSWMessage, public CSWObject
{
		CLASSINFO(CSWCameralControlMSG, CSWObject)
public:
    CSWCameralControlMSG();
    virtual ~CSWCameralControlMSG();

protected:
    /**
     * @brief MSG_INITIAL_PARAM �������������ʼ�������ṹ��
     * @param [in] wParam (CAMERAPARAM_INFO* �ṹָ�����ͣ�
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnInitialCameralParam( WPARAM wParam,LPARAM lParam );

    /**
     * @brief MSG_SET_SHU_RANGE ����AGC�ع�ʱ�䷶Χ
     * @param [in] wParam �عⷶΧDWORD���ͣ�4�ֽ�,0~15����С�ع�ʱ�� 16~32������ع�ʱ��
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetAGCShutterRange( WPARAM wParam,LPARAM lParam );
    /**
     * @brief MSG_GET_SHU_RANGE ��ȡAGC�ع�ʱ�䷶Χ
     * @param [in] wParam ����
     * @param [out] lParam �عⷶΧDWORD���ͣ�4�ֽ�,0~15����С�ع�ʱ�� 16~32������ع�ʱ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetAGCShutterRange( WPARAM wParam,LPARAM lParam );

    /**
     * @brief MSG_SET_GAIN_RANGE ����AGC���淶Χ
     * @param [in] wParam DWORD���ͣ�4�ֽڣ�0~15����С����ʱ�� 16~32���������ʱ��
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetAGCGainRange( WPARAM wParam,LPARAM lParam );

    /**
     * @brief MSG_SET_AGC_SCENE ����AGC�龰ģʽ
     * @param [in] wParam DWORD���ͣ�4�ֽڣ�0���Զ�, 1: �龰1, 2: �龰2
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnSetAGCScene( WPARAM wParam,LPARAM lParam );

	/**
     * @brief MSG_GET_AGC_SCENE ��ȡAGC�龰ģʽ
     * @param [in] wParam ����
     * @param [in] lParam DWORD���ͣ�4�ֽڣ�0���Զ�, 1: �龰1, 2: �龰2
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
	HRESULT OnGetAGCScene( WPARAM wParam,LPARAM lParam );

    /**
     * @brief MSG_GET_GAIN_RANGE ��ȡAGC���淶Χ
     * @param [in] wParam ����
     * @param [out] lParam DWORD���ͣ�4�ֽ�0~15����С����ʱ�� 16~32���������ʱ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetAGCGainRange( WPARAM wParam,LPARAM lParam );

    /**
     * @brief MSG_SET_AGCTH ����AGCͼ����������ֵ
     * @param [in] wParam ����ֵ
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetAGCTh( WPARAM wParam,LPARAM lParam );
    /**
     * @brief MSG_GET_AGCTH ��ȡAGCͼ����������ֵ
     * @param [in] wParam ����
     * @param [out] lParam ��INT*���ͣ� ����ֵ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetAGCTh( WPARAM wParam,LPARAM lParam );

    /**
     * @brief MSG_SET_AGCGAIN �ֶ�����ָ��
     * @param [in] wParam ����ֵ(��200)
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetAGCGain(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_AGCGAIN ��ȡ��ǰ����
     * @param [in] wParam ����
     * @param [out] lParam��INT*���ͣ� ���淵��ֵ(200)
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetAGCGain(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_AGCENABLE �Զ�����ָ��
     * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
     * @param [in] ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetAGCEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_AGCENABLE �Զ�����ָ��
     * @param [in] wParam ����
     * @param [in] lParam ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetAGCEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_AGCZONE �豸AGC�������
     * @param [in] wParam AGC�������ʹ�ܱ�־INT���ݣ�16�����ݡ�
     * @param [in] ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetAGCZone(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_AGCZONE ��ȡAGC�������
     * @param [in] wParam ����
     * @param [in] lParam AGC�������ʹ�ܱ�־INT���ݣ�16�����ݡ�
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetAGCZone(WPARAM wParam,LPARAM lParam);


    /**
     * @brief MSG_SET_AGCPARAM ��ȡAGC����
     * @param [in] wParam  DWORD[4]
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetAGCParam(WPARAM wParam,LPARAM lParam);
    /**
     * @brief MSG_SET_AGCPARAM ��ȡAGC����
     * @param [in] wParam  ����
     * @param [out] lParam DWORD[4]
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetAGCParam(WPARAM wParam,LPARAM lParam);
    /**
     * @brief MSG_SHUTTER ���ÿ���ָ��
     * @param [in] wParam ����ֵ
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetShutter(WPARAM wParam,LPARAM lParam);
    /**
     * @brief MSG_SHUTTER ��ȡ����ָ��
     * @param [in] wParam ����
     * @param [out] lParam��INT*���ͣ� ����ֵ
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetShutter(WPARAM wParam,LPARAM lParam);


    /**
     * @brief MSG_SET_RGBGAIN ����RGB����
     * @param [in] wParam RGB����(0��7λΪrͨ��ֵ��8��15λGͨ��ֵ��16��23ΪBͨ��ֵ
     * ����ʹ����Ϣ����Ϊʹ�ܱ��0=��ʹ�ܣ�1 = ʹ��
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetRGBGain(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_RGBGAIN ��Ϣ���պ���
     * @param [in] wParam ����
     * @param [out] lParam(DWORD*����) RGB����(0��7λΪRͨ��ֵ��8��15λGͨ��ֵ��16��23ΪGͨ��ֵ
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetRGBGain(WPARAM wParam,LPARAM lParam);


    /**
     * @brief MSG_SET_AWBENABLE �Զ���ƽ��ָ��
     * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
     * @param [in] ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetAWBEnable(WPARAM wParam,LPARAM lParam);
    /**
     * @brief MSG_GET_AWBENABLE ��ȡ�Զ���ƽ�⿪��
     * @param [in] wParam ����
     * @param [in] lParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetAWBEnable(WPARAM wParam,LPARAM lParam);


    /**
     * @brief MSG_SET_DCAPERTUREENABLE,ʹ���Զ�DC��Ȧ
     * @param [in] wParam 1Ϊʹ�ܣ�0Ϊ��ʹ��
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetDCApertureEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_DCAPERTUREENABLE,��ȡ�Զ�DC��Ȧʹ��
     * @param [in] wParam  ����
     * @param [in] lParam 1Ϊʹ�ܣ�0Ϊ��ʹ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetDCApertureEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_ZOOM_DCAPERTURE,�Ŵ�DC��Ȧ
     * @param [in] wParam ����
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnZoomDCAperture(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SHRINK_DCAPERTURE,��СDC��Ȧ
     * @param [in] wParam ����
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnShrinkDCAperture(WPARAM wParam,LPARAM lParam);



    /**
     * @brief MSG_SET_SHARPENMODE,ʹ����
     * @param [in] wParam 1Ϊ�ر��񻯣�3������֡
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetSharpeMode(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_SHARPENMODE,��ȡ��ʹ��
     * @param [in] wParam ����
     * @param [in] lParam  1Ϊ�ر��񻯣�3������֡
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetSharpeMode(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_SHARPEN_ENABLE,������ʹ��
     * @param [in] wParam 0Ϊ�ر��񻯣�1������֡
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetSharpeEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_SHARPEN_ENABLE,��ȡ��ʹ��
     * @param [in] wParam ����
     * @param [in] lParam ���ͣ� DWORD*�� 0Ϊ�ر��񻯣�1������֡
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetSharpeEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_SHARPENTHRESHOLD,�����񻯷�ֵ
     * @param [in] wParam �񻯷�ֵ[0~100]���Ƽ�ʹ��7
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetSharpeThreshold(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_SHARPENTHRESHOLD,��ȡ�񻯷�ֵ
     * @param [in] wParam  ����
     * @param [in] lParam �񻯷�ֵ[0~100]���Ƽ�ʹ��7
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetSharpeThreshold(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_COLORMATRIX_ENABLE,����ͼ����ǿʹ��
     * @param [in] wParam 0Ϊ�رգ�1����
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */

    HRESULT OnSetColoMaxtrixEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_COLORMATRIX_ENABLE,��ȡͼ����ǿʹ��
     * @param [in] wParam  ����
     * @param [in] lParam 0Ϊ�رգ�1����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */

    HRESULT OnGetColoMaxtrixEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_CONTRASTTHRESHOLD,���öԱȶ�
     * @param [in] wParam �Աȶ�[-100~100]
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetContrastThreshold(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_CONTRASTTHRESHOLD,��ȡ�Աȶ�
     * @param [in] wParam ����
     * @param [in] lParam [-100~100]
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetContrastThreshold(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_SATURATIONTHRESHOLD,���ñ��Ͷ�
     * @param [in] wParam �Աȶ�[-100~100]
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetSaturationThreshold(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_SATURATIONTHRESHOLD,��ȡ���Ͷ�
     * @param [in] wParam ����
     * @param [in] lParam [-100~100]
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetSaturationThreshold(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_FILTERSWITCH �˹�Ƭ�л�
     * @param [in] wParam 1�����˹�Ƭ��2�����˹�Ƭ
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetFilterSwitch(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_FILTERSWITCH �˹�Ƭ�л�
     * @param [in] wParam ����
     * @param [in] lParam 1�����˹�Ƭ��2�����˹�Ƭ
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetFilterSwitch(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_REDLIGHT_ENABLE ���ú�ƼӺ�ʹ��
     * @param [in] wParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
     * @param [in] lParam ������
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetRedLightEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_REDLIGHT_ENABLE ��ȡ��ƼӺ�ʹ��
     * @param [in] wParam ����
     * @param [in] lParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetRedLightEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_REDLIGHT_RECT ���ú�ƼӺ�����
     * @param [in] wParam SW_RECT*
     * @param [in] lParam INT ��ƼӺ���������
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetRedLightRect(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_TRAFFIC_LIGTH_LUM_TH ���ú�ƼӺ�
     * @param [in] wParam
     * @param [in] lParam ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetRedLightThreshold(WPARAM wParam,LPARAM lParam);

	
    /**
     * @brief MSG_SET_REDLIGHT_RECT �����̵Ƽ�������
     * @param [in] wParam SW_RECT*
     * @param [in] lParam INT �̵ƼӺ���������
     * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetGreenLightRect(WPARAM wParam,LPARAM lParam);
    
    /**
     * @brief MSG_SET_TRAFFIC_GREEN_LIGTH_LUM_TH �����̵Ƽ���
     * @param [in] wParam
     * @param [in] lParam ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetGreenLightThreshold(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_BARRIER_STATE ��ȡ���˻�״̬
     * @param [in] ����
     * @param [out] lParam : ���˻�״̬
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetBarrierState(WPARAM wParam,LPARAM lParam){return S_FALSE;};

    /**
     * @brief MSG_SET_WDR_STRENGTH ����WDR��ֵ
     * @param [in] wParam 0Ϊ��ʹ�ܣ�ֵԽ��ǿ��Խ��ȡֵ��Χ��0~4095
     * @param [out] ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetWDRStrength(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_WDR_STRENGTH ����WDR��ֵ
     * @param [in] wParam ����
     * @param [out] lParam 0Ϊ��ʹ�ܣ�ֵԽ��ǿ��Խ��ȡֵ��Χ��0~4095
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetWDRStrength(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_TNF_ENABLE ����TNFʹ��
     * @param [in] wParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetTNFEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_TNF_ENABLE ��ȡTNFʹ��
     * @param [in] wParam  ����
     * @param [out] lParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetTNFEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_SNF_ENABLE ����SNFʹ��
     * @param [in] wParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetSNFEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_SNF_ENABLE ��ȡSNFʹ��
     * @param [in] wParam  ����
     * @param [out] lParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetSNFEnable(WPARAM wParam,LPARAM lParam);


    /**
     * @brief MSG_SET_DENOISE_MODE ���ý���ģʽ
     * @param [in] wParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetDeNoiseMode(WPARAM wParam,LPARAM lParam);
    /**
     * @brief MSG_GET_DENOISE_MODE ��ȡ����ģʽ
     * @param [in] wParam  ����
     * @param [out] lParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetDeNoiseMode(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_DENOISE_STATE ���ý���״̬
     * @param [in] wParam 0~3:�Զ����͡��С���
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetDeNoiseState(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_DENOISE_STATE ��ȡ����״̬
     * @param [in] wParam  ����
     * @param [out] lParam 0~3:�Զ����͡��С���
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetDeNoiseState(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_AC_SYNC_ENABLE ���õ���ͬ��ʹ��
     * @param [in] wParam 0����ʹ�ܣ�1��ʹ��
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetACSyncEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_AC_SYNC_ENABLE ��ȡ����ͬ��ʹ��
     * @param [in] wParam  ����
     * @param [out] lParam 0����ʹ�ܣ�1��ʹ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetACSyncEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_AC_SYNC_DELAY ���õ���ͬ����ʱ
     * @param [in] wParam ��Χ��0~25500
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetACSyncDelay(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_AC_SYNC_DELAY ��ȡ����ͬ����ʱ
     * @param [in] wParam  ����
     * @param [out] lParam ��Χ��0~25500
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetACSyncDelay(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_GRAY_IMAGE_ENABLE ���úڰ�ͼʹ��
     * @param [in] wParam 0����ʹ�ܣ�1��ʹ��
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetGrayImageEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_GRAY_IMAGE_ENABLE ��ȡ�ڰ�ͼʹ��
     * @param [in] wParam  ����
     * @param [out] lParam 0����ʹ�ܣ�1��ʹ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetGrayImageEnable(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_CVBS_STD ����CVBS��ʽ
     * @param [in] wParam 0��PAL��1��NTSC
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetCVBSStd(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_CVBS_STD ��ȡCVBS��ʽ
     * @param [in] wParam  ����
     * @param [out] lParam 0��PAL��1��NTSC
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetCVBSStd(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_CAM_TEST ��������Զ�������
     * @param [in] wParam ����
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetCamTest(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_GAMMA_STRENGTH �������٤��
     * @param [in] wParam ��Χ��10��22
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetGammaStrength(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_GAMMA_STRENGTH ��ȡ���٤��
     * @param [in] wParam ����
     * @param [out] lParam ��Χ��10��22
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetGammaStrength(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_GAMMA_ENABLE ��ȡ�ڰ�ͼʹ��
     * @param [in] wParam  0����ʹ�ܣ�1��ʹ��
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetGammaEnable(WPARAM wParam,LPARAM lParam);


    /**
     * @brief MSG_GET_GRAY_IMAGE_ENABLE ��ȡ�ڰ�ͼʹ��
     * @param [in] wParam  ����
     * @param [out] lParam 0����ʹ�ܣ�1��ʹ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetGammaEnable(WPARAM wParam,LPARAM lParam);
    /**
     * @brief MSG_SET_CAM_FPGA_REG �������FGPAд
     * @param [in] wParam ��Χ����
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetCamFpgaReg(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_GAM_FPGA_REG ������FGPA��
     * @param [in] wParam ����
     * @param [out] lParam ��Χ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetCamFpgaReg(WPARAM wParam,LPARAM lParam);


    /**
     * @brief MSG_SET_EDGE_ENHANCE ͼ���Ե��ǿ
     * @param [in] wParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetEdgeEnhance(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_GET_EDGE_ENHANCE ͼ���Ե��ǿ
     * @param [in] wParam ����
     * @param [out] lParam 0Ϊ��ʹ�ܣ�1Ϊʹ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetEdgeEnhance(WPARAM wParam,LPARAM lParam);


    /**
     * @brief MSG_GET_MANUAL_VALUE ��ȡ�ֶ�RGB ��������
     * @param [in] wParam ����
     * @param [out] lParam
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetManualParamValue(WPARAM wParam,LPARAM lParam);


    /**
     * @brief ��ȡ�������״̬
     * @param [in] wParam ����
     * @param [out] lParam
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetCameraWorkState(WPARAM wParam,LPARAM lParam);


	 /**
	 * @brief MSG_SET_LED_SWITCH ���ò���ƿ���
	 * @param [in] wParam ����
	 * @param [out] lParam
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnSetLEDSwitch(WPARAM wParam,LPARAM lParam);


	 /**
	 * @brief MSG_GET_LED_SWITCH ��ȡ����ƿ���
	 * @param [in] wParam ����
	 * @param [out] lParam
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnGetLEDSwitch(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_SET_TRIGGEROUT_START ���������ʼ
	 * @param [in] wParam ����
	 * @param [out] lParam
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnSetTriggerOutState(WPARAM wParam,LPARAM lParam);


	 /**
	 * @brief MSG_SET_TRIGGEROUT ���ô������
	 * @param [in] polarity output_type 
	 * @param [out] lParam ����
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnSetTriggerOut(WPARAM wParam,LPARAM lParam);

	 
	 /**
	 * @brief MSG_GET_EXP_IO ��ȡ���������״̬
	 * @param [in] ����
	 * @param [out] polarity output_type
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnGetTriggerOut(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_SET_F1_TRIGGEROUT �µ����ô������
	 * @param [in] 	DWORD enable;  1 ʹ�� 0��ʹ��
	 				DWORD coupling;1 ��� 0�����
					DWORD output_type; �������  1OC�����أ�  0��������ƽ��
	 				DWORD polarity;1 ������ 0������
	 				DWORD pulse_width;���� 0~100
	 * @param [out] lParam ����
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnSetF1TriggerOut(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_GET_F1_TRIGGEROUT ��ȡ���������״̬
	 * @param [in] ����
	   @param [out] DWORD enable;  1 ʹ�� 0��ʹ��
	 				DWORD coupling;1 ��� 0�����
					DWORD output_type; �������  1OC�����أ�  0��������ƽ��
	 				DWORD polarity;1 ������ 0������
	 				DWORD pulse_width;���� 0~100
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnGetF1TriggerOut(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_SET_ALM_TRIGGEROUT �µ����ô������
	 * @param [in] 	DWORD enable;  1 ʹ�� 0��ʹ��
	 				DWORD coupling;1 ��� 0�����
					DWORD output_type; �������  1OC�����أ�  0��������ƽ��
	 				DWORD polarity;1 ������ 0������
	 				DWORD pulse_width;���� 0~100
	 * @param [out] lParam ����
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnSetAlarmTriggerOut(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_GET_ALM_TRIGGEROUT ��ȡ���������״̬
	 * @param [in] ����
	   @param [out] DWORD enable;  1 ʹ�� 0��ʹ��
	 				DWORD coupling;1 ��� 0�����
					DWORD output_type; �������  1OC�����أ�  0��������ƽ��
	 				DWORD polarity;1 ������ 0������
	 				DWORD pulse_width;���� 0~100
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnGetAlarmTriggerOut(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_SET_EXP_IO ���ò���ƿ��ƿ����
	 * @param [in] polarity output_type 
	 * @param [out] lParam ����
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnSetEXPIO(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_GET_EXP_IO ��ȡ����ƿ��ƿ�״̬
	 * @param [in] ���� 
	 * @param [out] polarity output_type
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnGetEXPIO(WPARAM wParam,LPARAM lParam);

    /**
      * @brief MSG_SET_EXP_IO ���ò���ƿ��ƿ����
      * @param [in] polarity output_type
      * @param [out] lParam ����
      * @return �ɹ�����S_OK������ֵ����ʧ��
      */
      HRESULT OnSetEXPPluseWidth(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_SET_ALARM_IO ���ø澯���ƿ����
	 * @param [in] polarity output_type 
	 * @param [out] lParam ����
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnSetALMIO(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_GET_ALARM_IO ��ȡ�澯���ƿ�״̬
	 * @param [in] ����
	 * @param [out] polarity output_type 
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnGetALMIO(WPARAM wParam,LPARAM lParam);

	 
	 /**
	 * @brief MSG_SET_TG_IO ���ô���ץ�Ŀڴ�������
	 * @param [in] ������
	 * @param [out] lParam ����
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnSetTGIO(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_GET_TG_IO ��ȡ����ץ�Ŀڴ�������
	 * @param [in] ����
	 * @param [out] ������
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnGetTGIO(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_SET_COLOR_GRADATION ����ɫ��
	 * @param [in] ɫ��
	 * @param [int] ����
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnSetColorGradation(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_SET_CAPTURE_ENABLE ����ץ��
	 * @param [in] ץ��ʹ�ܲ���
	 * @param [int] ����
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnSetCaptureEnable(WPARAM wParam,LPARAM lParam);
	 
	 /**
	 * @brief MSG_SET_WORK_MODE ���ù���ģʽ
	 * @param [in] ����ģʽ
	 * @param [int] ����
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnSetWorkMode(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_SET_AWB_MODE ���ð�ƽ��ģʽ
	 * @param [in] ����ģʽ
	 * @param [int] ����
	 * @return �ɹ�����S_OK������ֵ����ʧ��
	 */
	 HRESULT OnSetAWBMode(WPARAM wParam,LPARAM lParam);

	 /**
	 * @brief MSG_SET_SOFT_TOUCH ��������ץ��
	 */
	 HRESULT OnSetCaptureSofttouch(WPARAM wParam,LPARAM lParam);

	 /**
	* @brief MSG_SET_F1_OUTPUT_TYPE ����F1���ģʽ
	* @param [in] F1���ģʽ
	* @param [int] ����
	* @return �ɹ�����S_OK������ֵ����ʧ��
	*/
	 HRESULT OnSetF1OutputType(WPARAM wParam,LPARAM lParam);

	 /**
     *@brief ��ȡǰ�������̨����
     *@param [PVOID] pvBuffer PTZ���Ʋ���
     *@param [PVOID] iSize ������С
     *@return �ɹ�����S_OK,����ֵΪ�������
     */
	HRESULT OnGetPTZInfo(PVOID pvBuffer, INT iSize);

	/**
     * @brief MSG_CAPTURERGB_ENABLE ץ��RGBʹ��
     * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
     HRESULT OnSetCaptureRGBEnable(WPARAM wParam,LPARAM lParam);

	 /**
     * @brief MSG_SET_CAPTURE_AUTO_PARAM_ENABLE ץ���Զ����ò���
     * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
     HRESULT OnSetCaptureAutoParamEnable(WPARAM wParam,LPARAM lParam);

	 /**
     * @brief MSG_GET_CAPTURE_AUTO_PARAM_ENABLE ��ȡץ���Զ����ò���
     * @param [in] wParam ����
     * @param [out] lParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
     HRESULT OnGetCaptureAutoParamEnable(WPARAM wParam,LPARAM lParam);

     /**
     * @brief MSG_CAPTURESHUTTER_ENABLE ץ�Ŀ���ʹ��
     * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
     HRESULT OnSetCaptureShutterEnable(WPARAM wParam,LPARAM lParam);

     /**
     * @brief MSG_CAPTUREGAIN_ENABLE ץ������ʹ��
     * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
     HRESULT OnSetCaptureGainEnable(WPARAM wParam,LPARAM lParam);

     /**
      * @brief MSG_CAPTURESHARPEN_ENABLE ץ����ʹ��
      * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
      * @param [in] lParam ����
      * @return �ɹ�����S_OK������ֵ����ʧ��
      */
      HRESULT OnSetCaptureSharpenEnable(WPARAM wParam,LPARAM lParam);

     /**
     * @brief MSG_CAPTURESHUTTER,����ץ�Ŀ���
     * @param [in] wParam ����ֵ
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
     HRESULT OnSetCaptureShutter(WPARAM wParam,LPARAM lParam);
     /**
     * @brief MSG_GET_CAPTURESHUTTER, ��ȡ��ǰץ������
     * @param [in] wParam ����
     * @param [out] lParam(INT*����) ����ֵ
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
     HRESULT OnGetCaptureShutter(WPARAM wParam,LPARAM lParam);


     /**
     * @brief MSG_CAPTRUEGAIN, ץ����������
     * @param [in] wParam ����ֵ(200)
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
     HRESULT OnSetCaptureGain(WPARAM wParam,LPARAM lParam);
     /**
     * @brief MSG_GET_CAPTUREGAIN,��ȡץ������
     * @param [in] wParam ����
     * @param [out] lParam(INT* ����) ����ֵ
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
     HRESULT OnGetCaptureGain(WPARAM wParam,LPARAM lParam);
     /**
     * @brief MSG_CAPTRUERGBGAIN,����ץ��RGB����
     * @param [in] wParam RGB����(0��7λΪBͨ��ֵ��8��15λGͨ��ֵ��16��23ΪRͨ��ֵ,����:0x00AFAAFF
     * ,�ֱ�ΪFFΪBͨ�����棬AAΪGͨ�����棬AFΪRͨ������ֵ)
     * @param [in] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
     HRESULT OnSetCaptureRGBGain(WPARAM wParam,LPARAM lParam);
     /**
     * @brief MSG_GET_CAPTRUERGBGAIN,��ȡץ��RGB����
     * @param [in] wParam ����
     * @param [out] lParam(DWORD*����) RGB����(0��7λΪBͨ��ֵ��8��15λGͨ��ֵ��16��23ΪRͨ��ֵ,����:0x00AFAAFF
     * ,�ֱ�ΪFFΪBͨ�����棬AAΪGͨ�����棬AFΪRͨ������ֵ)
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
     HRESULT OnGetCaptureRGBGain(WPARAM wParam,LPARAM lParam);

     /**
      * @brief MSG_SET_CAPTURE_SHARPEN_THR ����ץ�����ָ��
      * @param [in] wParam ���ֵ
      * @param [in] lParam ����
      * @return �ɹ�����S_OK������ֵ����ʧ��
      */
     HRESULT OnSetCaptureSharpen(WPARAM wParam,LPARAM lParam);

     /**
        * @brief MSG_GET_CAPTURE_SHARPEN_THR ��ȡץ�����ָ��
        * @param [in] wParam ����
        * @param [out] lParam ���ֵ
        * @return �ɹ�����S_OK������ֵ����ʧ��
        */
     HRESULT OnGetCaptureSharpen(WPARAM wParam,LPARAM lParam);


    /**
     * @brief MSG_GET_M3_DAYNIGHT_STATUS ��ȡM3�������ϱ�ʶ
     * @param [in] wParam ����
     * @param [out] lParam ��ʶֵ
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnGetM3DayNightStatus(WPARAM wParam,LPARAM lParam);

    /**
     * @brief MSG_SET_WDR_LEVEL ����WDR�ȼ�
     * @param [in] wParam �ȼ�ֵ��0��ʾ�ر�
     * @param [out] lParam ����
     * @return �ɹ�����S_OK������ֵ����ʧ��
     */
    HRESULT OnSetWDRLevel(WPARAM wParam,LPARAM lParam);


	/**
	   @brief ץ������
	 */
	BOOL OnIncreaseBrightness(VOID);

     //��Ϣӳ��궨��
    SW_BEGIN_MESSAGE_MAP(CSWCameralControlMSG, CSWMessage)
        //��ʼ��ģ��
        SW_MESSAGE_HANDLER(MSG_INITIAL_PARAM, OnInitialCameralParam)

        SW_MESSAGE_HANDLER( MSG_SET_AGCPARAM , OnSetAGCParam)
        SW_MESSAGE_HANDLER( MSG_GET_AGCPARAM , OnGetAGCParam)

        // ���û��ȡAGC������ֵ
        SW_MESSAGE_HANDLER( MSG_SET_AGCTH , OnSetAGCTh)
        SW_MESSAGE_HANDLER( MSG_GET_AGCTH , OnGetAGCTh)

		SW_MESSAGE_HANDLER( MSG_SET_AGCZONE , OnSetAGCZone)
        SW_MESSAGE_HANDLER( MSG_GET_AGCZONE , OnGetAGCZone)
        // ����AGC���漰AGCʹ�ܵ�
        SW_MESSAGE_HANDLER(MSG_SET_AGCGAIN , OnSetAGCGain )
        SW_MESSAGE_HANDLER(MSG_GET_AGCGAIN , OnGetAGCGain )
        SW_MESSAGE_HANDLER(MSG_SET_AGCENABLE , OnSetAGCEnable )
        SW_MESSAGE_HANDLER(MSG_GET_AGCENABLE , OnGetAGCEnable )
        // ���û�ȡ�ع�ʱ�䷶Χ
        SW_MESSAGE_HANDLER(MSG_SET_SHU_RANGE , OnSetAGCShutterRange )
        SW_MESSAGE_HANDLER(MSG_GET_SHU_RANGE , OnGetAGCShutterRange )
        // ���û�ȡAGC���淶Χ
        SW_MESSAGE_HANDLER(MSG_SET_GAIN_RANGE , OnSetAGCGainRange )
        SW_MESSAGE_HANDLER(MSG_GET_GAIN_RANGE , OnGetAGCGainRange )

		// ����AGC�龰ģʽ
	 	SW_MESSAGE_HANDLER(MSG_SET_AGC_SCENE , OnSetAGCScene )
	 	SW_MESSAGE_HANDLER(MSG_GET_AGC_SCENE , OnGetAGCScene )
		
        // ���û�ȡ����
        SW_MESSAGE_HANDLER(MSG_SET_SHUTTER , OnSetShutter )
        SW_MESSAGE_HANDLER(MSG_GET_SHUTTER , OnGetShutter)
        // ��ƽ�⹦��
        SW_MESSAGE_HANDLER(MSG_SET_RGBGAIN , OnSetRGBGain )
        SW_MESSAGE_HANDLER(MSG_GET_RGBGAIN , OnGetRGBGain )
        SW_MESSAGE_HANDLER(MSG_SET_AWBENABLE , OnSetAWBEnable )
        SW_MESSAGE_HANDLER(MSG_GET_AWBENABLE , OnGetAWBEnable )


        // ��ʹ��
        SW_MESSAGE_HANDLER(MSG_SET_SHARPEN_ENABLE,OnSetSharpeEnable)
        SW_MESSAGE_HANDLER(MSG_GET_SHARPEN_ENABLE,OnGetSharpeEnable)
        // ����ֵ
		SW_MESSAGE_HANDLER(MSG_SET_SHARPENTHRESHOLD,OnSetSharpeThreshold)
        SW_MESSAGE_HANDLER(MSG_GET_SHARPENTHRESHOLD,OnGetSharpeThreshold)

		// ͼ����ǿ
        SW_MESSAGE_HANDLER(MSG_SET_COLORMATRIX_ENABLE, OnSetColoMaxtrixEnable)
        SW_MESSAGE_HANDLER(MSG_GET_COLORMATRIX_ENABLE, OnGetColoMaxtrixEnable)

        // ��ƼӺ�
        SW_MESSAGE_HANDLER(MSG_SET_REDLIGHT_ENABLE, OnSetRedLightEnable)
        SW_MESSAGE_HANDLER(MSG_GET_REDLIGHT_ENABLE, OnGetRedLightEnable)

		SW_MESSAGE_HANDLER(MSG_SET_REDLIGHT_RECT, OnSetRedLightRect)

        SW_MESSAGE_HANDLER(MSG_SET_TRAFFIC_LIGTH_LUM_TH, OnSetRedLightThreshold)

		SW_MESSAGE_HANDLER(MSG_SET_CONTRASTTHRESHOLD, OnSetContrastThreshold)
        SW_MESSAGE_HANDLER(MSG_GET_CONTRASTTHRESHOLD, OnGetContrastThreshold)

		SW_MESSAGE_HANDLER(MSG_SET_SATURATIONTHRESHOLD, OnSetSaturationThreshold)
        SW_MESSAGE_HANDLER(MSG_GET_SATURATIONTHRESHOLD, OnGetSaturationThreshold)

        SW_MESSAGE_HANDLER(MSG_SET_GREENLIGHT_RECT, OnSetGreenLightRect)

        SW_MESSAGE_HANDLER(MSG_SET_TRAFFIC_GREEN_LIGTH_LUM_TH, OnSetGreenLightThreshold)

		// �ⲿ�豸����
		SW_MESSAGE_HANDLER(MSG_SET_FILTERSWITCH , OnSetFilterSwitch)
        SW_MESSAGE_HANDLER(MSG_GET_FILTERSWITCH , OnGetFilterSwitch)


        // DC��Ȧ
		SW_MESSAGE_HANDLER(MSG_SET_DCAPERTUREENABLE , OnSetDCApertureEnable)
        SW_MESSAGE_HANDLER(MSG_GET_DCAPERTUREENABLE , OnGetDCApertureEnable)
        //�Ŵ��Ȧ
        SW_MESSAGE_HANDLER(MSG_ZOOM_DCAPERTURE , OnZoomDCAperture)
        //��С��Ȧ
        SW_MESSAGE_HANDLER(MSG_SHRINK_DCAPERTURE , OnShrinkDCAperture)


		SW_MESSAGE_HANDLER(MSG_GET_BARRIER_STATE, OnGetBarrierState)

        //WDR
        SW_MESSAGE_HANDLER(MSG_SET_WDR_STRENGTH, OnSetWDRStrength)
        SW_MESSAGE_HANDLER(MSG_GET_WDR_STRENGTH, OnGetWDRStrength)
        //����
        SW_MESSAGE_HANDLER(MSG_SET_TNF_ENABLE, OnSetTNFEnable)
        SW_MESSAGE_HANDLER(MSG_GET_TNF_ENABLE, OnGetTNFEnable)

        SW_MESSAGE_HANDLER(MSG_SET_SNF_ENABLE, OnSetSNFEnable)
        SW_MESSAGE_HANDLER(MSG_GET_SNF_ENABLE, OnGetSNFEnable)

        SW_MESSAGE_HANDLER(MSG_SET_DENOISE_MODE, OnSetDeNoiseMode )
        SW_MESSAGE_HANDLER(MSG_GET_DENOISE_MODE, OnGetDeNoiseMode )

        SW_MESSAGE_HANDLER(MSG_SET_DENOISE_STATE, OnSetDeNoiseState)
        SW_MESSAGE_HANDLER(MSG_GET_DENOISE_STATE, OnGetDeNoiseState)
        //����ͬ��
        SW_MESSAGE_HANDLER(MSG_SET_AC_SYNC_ENABLE, OnSetACSyncEnable )
        SW_MESSAGE_HANDLER(MSG_GET_AC_SYNC_ENABLE, OnGetACSyncEnable)

        SW_MESSAGE_HANDLER(MSG_SET_AC_SYNC_DELAY, OnSetACSyncDelay )
        SW_MESSAGE_HANDLER(MSG_GET_AC_SYNC_DELAY, OnGetACSyncDelay)

        //�ڰ�ͼ
        SW_MESSAGE_HANDLER(MSG_SET_GRAY_IMAGE_ENABLE, OnSetGrayImageEnable)
        SW_MESSAGE_HANDLER(MSG_GET_GRAY_IMAGE_ENABLE, OnGetGrayImageEnable)

        //CVBS��ʽ
        SW_MESSAGE_HANDLER(MSG_SET_CVBS_STD, OnSetCVBSStd)
        SW_MESSAGE_HANDLER(MSG_GET_CVBS_STD, OnGetCVBSStd)
        //����Զ�������
        SW_MESSAGE_HANDLER(MSG_SET_CAM_TEST, OnSetCamTest)

        //٤��
        SW_MESSAGE_HANDLER(MSG_SET_GAMMA_STRENGTH, OnSetGammaStrength)
        SW_MESSAGE_HANDLER(MSG_GET_GAMMA_STRENGTH, OnGetGammaStrength)
        SW_MESSAGE_HANDLER(MSG_SET_GAMMA_ENABLE, OnSetGammaEnable)
        SW_MESSAGE_HANDLER(MSG_GET_GAMMA_ENABLE, OnGetGammaEnable)

        //FPGA��ʽ
        SW_MESSAGE_HANDLER(MSG_SET_CAM_FPGA_REG, OnSetCamFpgaReg)
        SW_MESSAGE_HANDLER(MSG_GET_CAM_FPGA_REG, OnGetCamFpgaReg)

        //ͼ���Ե��ǿ
        SW_MESSAGE_HANDLER(MSG_SET_EDGE_ENHANCE, OnSetEdgeEnhance)
        SW_MESSAGE_HANDLER(MSG_GET_EDGE_ENHANCE, OnGetEdgeEnhance)

        //��ȡ�ֶ����� ���� ���� R G B
        SW_MESSAGE_HANDLER(MSG_GET_MANUAL_VALUE, OnGetManualParamValue)
        //��ȡ�������״̬
        SW_MESSAGE_HANDLER(MSG_GET_CAMERA_WORKSTATE, OnGetCameraWorkState)

		//����ƿ���
		SW_MESSAGE_HANDLER(MSG_SET_LED_SWITCH, OnSetLEDSwitch)
		SW_MESSAGE_HANDLER(MSG_GET_LED_SWITCH, OnGetLEDSwitch)

	 	//�������(�ڲ�����)
	 	SW_MESSAGE_HANDLER(MSG_SET_TRIGGEROUT_STATE, OnSetTriggerOutState)
		
		//���������
		SW_MESSAGE_HANDLER(MSG_SET_TRIGGEROUT, OnSetTriggerOut)
		SW_MESSAGE_HANDLER(MSG_GET_TRIGGEROUT, OnGetTriggerOut)

		//�µĴ����ӿڣ�������FPGA�Ĵ�������λ��
		SW_MESSAGE_HANDLER(MSG_SET_F1_TRIGGEROUT, OnSetF1TriggerOut)
		SW_MESSAGE_HANDLER(MSG_GET_F1_TRIGGEROUT, OnGetF1TriggerOut)
		SW_MESSAGE_HANDLER(MSG_SET_ALM_TRIGGEROUT, OnSetAlarmTriggerOut)
		SW_MESSAGE_HANDLER(MSG_GET_ALM_TRIGGEROUT, OnGetAlarmTriggerOut)

		//����������
		SW_MESSAGE_HANDLER(MSG_SET_EXP_IO, OnSetEXPIO)
		SW_MESSAGE_HANDLER(MSG_GET_EXP_IO, OnGetEXPIO)

		//�澯�����
		SW_MESSAGE_HANDLER(MSG_SET_ALARM_IO, OnSetALMIO)
		SW_MESSAGE_HANDLER(MSG_GET_ALARM_IO, OnGetALMIO)

	 	//����ץ�Ŀ�
	 	SW_MESSAGE_HANDLER(MSG_SET_TG_IO, OnSetTGIO)
	 	SW_MESSAGE_HANDLER(MSG_GET_TG_IO, OnGetTGIO)

	 	// ����ɫ�� 0 �� 0~255��1��16~235.
	 	SW_MESSAGE_HANDLER(MSG_SET_COLOR_GRADATION, OnSetColorGradation)

		//����FPGAץ��ʹ��
		SW_MESSAGE_HANDLER(MSG_SET_CAPTURE_ENABLE, OnSetCaptureEnable)

	 	//���ù���ģʽ
	 	SW_MESSAGE_HANDLER(MSG_SET_WORK_MODE, OnSetWorkMode)
	 	//���ð�ƽ��ģʽ
	 	SW_MESSAGE_HANDLER(MSG_SET_AWB_MODE, OnSetAWBMode)

        // ץ������
        SW_MESSAGE_HANDLER(MSG_SET_SOFT_TOUCH, OnSetCaptureSofttouch )

	 	//F1�������
        SW_MESSAGE_HANDLER(MSG_SET_F1_OUTPUT_TYPE, OnSetF1OutputType )

        // LED���������
        SW_MESSAGE_HANDLER(MSG_SET_EXP_PLUSEWIDTH, OnSetEXPPluseWidth)


		//ץ�Ĳ����Զ�����
		SW_MESSAGE_HANDLER(MSG_SET_CAPTURE_AUTO_PARAM_ENABLE , OnSetCaptureAutoParamEnable)
		//��ȡץ�Ĳ����Զ�����
		SW_MESSAGE_HANDLER(MSG_GET_CAPTURE_AUTO_PARAM_ENABLE , OnGetCaptureAutoParamEnable)
        // ץ��RGBʹ��
        SW_MESSAGE_HANDLER(MSG_CAPTURERGB_ENABLE , OnSetCaptureRGBEnable )
        // ץ�Ŀ���ʹ��
        SW_MESSAGE_HANDLER(MSG_CAPTURESHUTTER_ENABLE , OnSetCaptureShutterEnable )
        // ץ������ʹ��
        SW_MESSAGE_HANDLER(MSG_CAPTUREGAIN_ENABLE , OnSetCaptureGainEnable )
        // ץ����ʹ��
        SW_MESSAGE_HANDLER(MSG_CAPTURESHARPEN_ENABLE , OnSetCaptureSharpenEnable )

        // ץ�Ŀ���
        SW_MESSAGE_HANDLER(MSG_SET_CAPTURESHUTTER , OnSetCaptureShutter )
        SW_MESSAGE_HANDLER(MSG_GET_CAPTURESHUTTER , OnGetCaptureShutter )
        // ץ������
        SW_MESSAGE_HANDLER(MSG_SET_CAPTRUEGAIN , OnSetCaptureGain )
        SW_MESSAGE_HANDLER(MSG_GET_CAPTUREGAIN , OnGetCaptureGain )
        // ץ�İ�ƽ��
        SW_MESSAGE_HANDLER(MSG_SET_CAPTRUERGBGAIN , OnSetCaptureRGBGain )
        SW_MESSAGE_HANDLER(MSG_GET_CAPTRUERGBGAIN , OnGetCaptureRGBGain )

        // ץ�����
        SW_MESSAGE_HANDLER(MSG_SET_CAPTURE_SHARPEN_THR, OnSetCaptureSharpen)
        SW_MESSAGE_HANDLER(MSG_GET_CAPTURE_SHARPEN_THR, OnGetCaptureSharpen)

        SW_MESSAGE_HANDLER(MSG_GET_M3_DAYNIGHT_STATUS, OnGetM3DayNightStatus)

        SW_MESSAGE_HANDLER(MSG_SET_WDR_LEVEL, OnSetWDRLevel);

    SW_END_MESSAGE_MAP()

	 //Զ����Ϣӳ�亯��
    SW_BEGIN_REMOTE_MESSAGE_MAP(CSWCameralControlMSG, CSWMessage)
		SW_REMOTE_MESSAGE_HANDLER(MSG_APP_REMOTE_GET_PTZ_INFO, OnGetPTZInfo)
    SW_END_REMOTE_MESSAGE_MAP();
	 
protected:
        INT m_iManualShutter;
        INT m_iManualGain;
        INT m_iManualGainR;
        INT m_iManualGainG;
        INT m_iManualGainB;
        INT m_FilterStatus;

		enum FPGA_IO_TYPE
		{
			E_NORMAL_LED,	//���ͣ�0-����ƣ��صƣ�1-�澯��� 2-ץ�����
			E_ALARM_OUT,
			E_TRIGGEROUT,
		};
		
		typedef struct _tFPGA_IO_ARG{
			DWORD type;
			DWORD enable;
			DWORD freq_num;
			DWORD polarity;
			DWORD pulse_width;
			DWORD output_type;
			DWORD coupling;
			_tFPGA_IO_ARG()
				: type(0)
				, enable(0)
				, freq_num(0)
				, polarity(0)
				, pulse_width(0)
				, output_type(0)
				, coupling(0)
			{
			}
		}tFPGA_IO_ARG;

		DWORD m_dwLEDPolarity;		//LED����
		DWORD m_dwLEDOutputType;	//�������
		DWORD m_dwLEDPluseWidth;
		DWORD m_dwTriggerOutPolarity;	//�����������
		DWORD m_dwTriggerOutOutputType;	//�����������
		
		DWORD m_dwAGCScene;			//AGC�龰ģʽ

		BOOL m_fIsDayNow;
		BOOL m_fIsM3DayNow;
		BOOL m_fCaptureAutoParamEnable;
		BOOL m_fEnableCaptureRGBGain;
		BOOL m_fEnableCaptureShutter;
		BOOL m_fEnableCaptureGain;
		BOOL m_fEnableCaptureSharpen;

		int m_nCaptureShutter;
		int m_nCaptureGain;
		int m_nCaptureR;
		int m_nCaptureG;
		int m_nCaptureB;
		int m_nCapSharpen;
};
REGISTER_CLASS(CSWCameralControlMSG)
#endif // CSWCAMERALCONTROLMSG_H

