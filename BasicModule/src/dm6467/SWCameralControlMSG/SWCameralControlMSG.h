#ifndef CSWCAMERALCONTROLMSG_H
#define CSWCAMERALCONTROLMSG_H
#include "SWObject.h"
#include "SWMessage.h"
#include "SWAutoLock.h"
#include "SWAgcAwbTask.h"
#include "SWCaptureTask.h"
#include "SWChannelDifferenceCorrectTask.h"
#include "SWImageQualitySet.h"
#include "SWCameraControlParameter.h"
#include "SWExtDevControl.h"


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
    * @brief MSG_GC �ֶ�����ָ��
    * @param [in] wParam ����ֵ(��200)
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetAGCGain(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_AGC �Զ�����ָ��
    * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
    * @param [in] ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetAGCEnable(WPARAM wParam,LPARAM lParam);
    
    HRESULT OnGetAGCEnable(WPARAM wParam,LPARAM lParam);

	/**
    * @brief MSG_SET_AGCZONE �豸AGC�������
    * @param [in] wParam AGC�������ʹ�ܱ�־INT���ݣ�16�����ݡ�
    * @param [in] ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
	HRESULT OnSetAGCZone(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_GET_GC ��ȡ��ǰ����
    * @param [in] wParam ����
    * @param [out] lParam��INT*���ͣ� ���淵��ֵ(200)
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnGetAGCGain(WPARAM wParam,LPARAM lParam);
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
    * @brief MSG_WB ����RGB����
    * @param [in] wParam RGB����(0��7λΪBͨ��ֵ��8��15λGͨ��ֵ��16��23ΪRͨ��ֵ,����:0x00AFAAFF
    * ,�ֱ�ΪFFΪBͨ�����棬AAΪGͨ�����棬AFΪRͨ������ֵ)
    * ����ʹ����Ϣ����Ϊʹ�ܱ��0=��ʹ�ܣ�1 = ʹ��
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetRGBGain(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_AWB �Զ���ƽ��ָ��
    * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
    * @param [in] ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetAWBEnable(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_GET_AWBENABLE ��ȡ�Զ���ƽ��ʹ�ܿ���
    * @param [in] wParam ����
    * @param [out] lParam(INT*����) ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnGetAWBEnable(WPARAM wParam,LPARAM lParam);
    /**
    * @brief MSG_GET_WB ��ȡ��ǰRGB����
    * @param [in] wParam ����
    * @param [out] lParam(DWORD*����) RGB����(0��7λΪBͨ��ֵ��8��15λGͨ��ֵ��16��23ΪRͨ��ֵ,����:0x00AFAAFF
    * ,�ֱ�ΪFFΪBͨ�����棬AAΪGͨ�����棬AFΪRͨ������ֵ)
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnGetRGBGain(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_GAMMA ٤������ָ��
	* @param [in] wParam ����Gammaֵ�б�ָ��Gamma[8][2]
	* @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetGamma(WPARAM wParam,LPARAM lParam);


    /**
    * @brief MSG_DIFFCORR ������У��ʹ��ָ��
    * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetDiffCorrEnable(WPARAM wParam,LPARAM lParam);


    /**
    * @brief MSG_CAPTURE_ENABLE ץ�Ĳ�����У��ʹ��
    * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */

    HRESULT OnSetCaptureEnable(WPARAM wParam,LPARAM lParam);


    /**
    * @brief MSG_CAPTURERGB_ENABLE ץ��RGBʹ��
    * @param [in] wParam ʹ����Ϣ��ʹ�ܱ�ǣ�0=��ʹ�ܣ�1 = ʹ��
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetCaptureRGBEnable(WPARAM wParam,LPARAM lParam);


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
    * @brief MSG_SET_SOFT_TOUCH,����ץ������
	* @param [in] wParam ��1�ֽ�Ϊ�������������ϣ�Ŀǰ֧��3·����ƣ��ӵ�λ��ʼ
	* ����һλ��Ӧ��һ·����ƣ��ڶ�λ��Ӧ�ڶ�·����ơ�����λΪ1���ʾ���ø�·����ƣ�
	* @param [in] wParam ��3�ֽ�Ϊץ����Ϣ����Ӧ��ץ��ͼ����д���Ϣ��������ץ��ͼ�ı�ʶ��
	* ��Ӳ������֡��Ϊ�ۼӷ�ʽ
    *  ����: 0x000000013   1Ϊ�������Ϣ��3Ϊץ����Ϣ
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetCaptureSofttouch(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_SET_MAXSHU_GAIN,�����豸��Ϣ
    * @param [in] wParam ��16λ������
    * @param [in] wParam ��16λΪ�������
    *  ����: 0x100300C8,1003Ϊ�����ţ�00C8Ϊ�������
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetAGCMaxShuAndGain(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_GET_MAXSHU_GAIN,�����豸��Ϣ
    * @param [in] wParam ����
    * @param [in] lParam lParam(DWORD*����) ��16λ������
    * @param [in] lParam lParam(DWORD*����) ��16λΪ�������
    *  ����: 0x100300C8,1003Ϊ�����ţ�00C8Ϊ�������
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnGetAGCMaxShuAndGain(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_SET_MINSHU_GAIN,�����豸��Ϣ
    * @param [in] wParam ��16λ��С����
    * @param [in] wParam ��16λΪ��С����
    *  ����: 0x100300C8,1003Ϊ�����ţ�00C8Ϊ�������
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetAGCMinShuAndGain(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_GET_MINSHU_GAIN,�����豸��Ϣ
    * @param [in] wParam ����
    * @param [in] lParam lParam(DWORD*����) ��16λ��С����
    * @param [in] lParam lParam(DWORD*����) ��16λΪ��С����
    *  ����: 0x100300C8,1003Ϊ�����ţ�00C8Ϊ�������
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnGetAGCMinShuAndGain(WPARAM wParam,LPARAM lParam);



	HRESULT OnSetAGCParam(WPARAM wParam,LPARAM lParam);

     /**
    * @brief MSG_SET_DCAPERTUREENABLE,ʹ��DC��Ȧ
    * @param [in] wParam 1Ϊʹ�ܣ�0Ϊ��ʹ��
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetDCApertureEnable(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_SET_SHARPENMODE,WDRǿ��ֵ
    * @param [in] wParam 1Ϊ�ر��񻯣�2Ϊ��ץ�ģ�3������֡
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetSharpeMode(WPARAM wParam,LPARAM lParam);


    /**
    * @brief MSG_SET_SHARPENMODE,ʹ����
    * @param [in] wParam 1Ϊ�ر��񻯣�2Ϊ��ץ�ģ�3������֡
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetSharpeMode(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_SET_SHARPENTHRESHOLD,�����񻯷�ֵ
    * @param [in] wParam �񻯷�ֵ[0~100]���Ƽ�ʹ��7
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetSharpeThreshold(WPARAM wParam,LPARAM lParam);


    /**
    * @brief MSG_SET_COLORMATRIXMODE,ʹ��ͼ����ǿ
    * @param [in] wParam 1Ϊ�ر�ͼ����ǿ��2Ϊͼ����ǿץ�ģ�3ͼ����ǿ����֡
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetColoMaxtrixMode(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_SET_CONTRASTTHRESHOLD,���öԱȶ�
    * @param [in] wParam �Աȶ�[-100~100]
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetContrastThreshold(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_SET_SATURATIONTHRESHOLD,���ñ��Ͷ�
    * @param [in] wParam �Աȶ�[-100~100]
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetSaturationThreshold(WPARAM wParam,LPARAM lParam);


    /**
    * @brief MSG_SET_SATURATIONTHRESHOLD,����ɫ��
    * @param [in] wParam ɫ��5������[32,40,52,55,60,70]
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetTemperatureThreshold(WPARAM wParam,LPARAM lParam);
    /**
    * @brief MSG_SET_HUETHRESHOLD,����ɫ��
    * @param [in] wParam �Աȶ�[-180~180]
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetHueThreshold(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_SET_HAZEREMOVALMODE,ʹ�ܳ���ģʽ
    * @param [in] wParam 1Ϊ�رճ���2Ϊ����ץ�ģ�3Ϊ��������֡
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetHazeremovalMode(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_SET_HUETHRESHOLD,���ó���ֵ
    * @param [in] wParam ����ֵ[0~10]
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetHazeremovalThreshold(WPARAM wParam,LPARAM lParam);

	/**
    * @brief ����Ƶ���ź�ʹ�ܡ�
    * @param [in] wParam 0��1
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetFlashRateEnable(WPARAM wParam,LPARAM lParam);

	 /**
    * @brief ����ץ���ź�ʹ��
    * @param [in] wParam 0��1
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetCaptrueSyncEnable(WPARAM wParam,LPARAM lParam);

	/**
    * @brief ����Ƶ������
    * @param [in] wParam ���� 100~20000����λ΢��
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetFlashRatePulse(WPARAM wParam,LPARAM lParam);


	/**
    * @brief MSG_SET_FILTERSWITCH �˹�Ƭ�л�
    * @param [in] wParam 1�����˹�Ƭ��2�����˹�Ƭ
    * @param [in] lParam ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetFilterSwitch(WPARAM wParam,LPARAM lParam);

	/**
    * @brief MSG_SET_REDLIGHT_RECT ���ú�ƼӺ�����
    * @param [in] wParam SW_RECT*
    * @param [in] lParam INT ��ƼӺ���������
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetRedLightRect(WPARAM wParam,LPARAM lParam);

	/**
    * @brief MSG_SET_GAMMA_MODE ����GAMMAʹ��ģʽ
    * @param [in] wParam INT ͨ��ѡ��ֵ��0��6467��368����GAMMA��1��6467��368�ޣ�2��6467��368�У�3��6467��368��
    * @param [in] ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetGammaMode(WPARAM wParam,LPARAM lParam);

	/**
    * @brief MSG_SET_GAMMA_MODE ����GAMMAʹ��ģʽ
    * @param [in] ����
    * @param [out] INT* ����GAMMAʹ��ģʽ
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
	HRESULT OnGetGammaMode(WPARAM wParam,LPARAM lParam);

	/**
    * @brief MSG_SET_CAPTUREEDGE ���ô���ץ����
    * @param [in] wParam 0~3
    * @param [out] ����
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnSetCaptureEdge(WPARAM wParam,LPARAM lParam);

    /**
    * @brief MSG_GET_CAPTUREEDGE ��ȡ����ץ����
    * @param [in] ����
    * @param [out] lParam 0~3
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnGetCaptureEdge(WPARAM wParam,LPARAM lParam);

     /**
    * @brief MSG_GET_BARRIER_STATE ��ȡ���˻�״̬
    * @param [in] ����
    * @param [out] lParam : ���˻�״̬
    * @return �ɹ�����S_OK������ֵ����ʧ��
    */
    HRESULT OnGetBarrierState(WPARAM wParam,LPARAM lParam);

	 /**
     *@brief ��ȡǰ�������̨����
     *@param [PVOID] pvBuffer PTZ���Ʋ���
     *@param [PVOID] iSize ������С
     *@return �ɹ�����S_OK,����ֵΪ�������
     */
	HRESULT OnGetPTZInfo(PVOID pvBuffer, INT iSize);

     //��Ϣӳ��궨��
    SW_BEGIN_MESSAGE_MAP(CSWCameralControlMSG, CSWMessage)
        //��ʼ��ģ��
        SW_MESSAGE_HANDLER(MSG_INITIAL_PARAM, OnInitialCameralParam)

        // ���û��ȡAGC������ֵ
        SW_MESSAGE_HANDLER( MSG_SET_AGCTH , OnSetAGCTh)
        SW_MESSAGE_HANDLER( MSG_GET_AGCTH , OnGetAGCTh)

		SW_MESSAGE_HANDLER( MSG_SET_AGCZONE , OnSetAGCZone)

        // ����AGC���漰AGCʹ�ܵ�
        SW_MESSAGE_HANDLER(MSG_SET_AGCGAIN , OnSetAGCGain )
        SW_MESSAGE_HANDLER(MSG_GET_AGCGAIN , OnGetAGCGain );
        SW_MESSAGE_HANDLER(MSG_SET_AGCENABLE , OnSetAGCEnable )
        SW_MESSAGE_HANDLER(MSG_GET_AGCENABLE , OnGetAGCEnable )
		
        // ���û�ȡ����
        SW_MESSAGE_HANDLER(MSG_SET_SHUTTER , OnSetShutter );
        SW_MESSAGE_HANDLER(MSG_GET_SHUTTER , OnGetShutter);
        // ��ƽ�⹦��
        SW_MESSAGE_HANDLER(MSG_SET_RGBGAIN , OnSetRGBGain );
        SW_MESSAGE_HANDLER(MSG_GET_RGBGAIN , OnGetRGBGain );
        SW_MESSAGE_HANDLER(MSG_SET_AWBENABLE , OnSetAWBEnable );
        SW_MESSAGE_HANDLER(MSG_GET_AWBENABLE , OnGetAWBEnable );
        // GAMMA����
        SW_MESSAGE_HANDLER(MSG_SET_GAMMA , OnSetGamma );
		SW_MESSAGE_HANDLER(MSG_SET_GAMMA_MODE , OnSetGammaMode );
		SW_MESSAGE_HANDLER(MSG_GET_GAMMA_MODE , OnGetGammaMode );

        // �����Խ���ʹ��
        SW_MESSAGE_HANDLER(MSG_SET_DIFFCORRENABLE , OnSetDiffCorrEnable )


        // ץ��RGBʹ��
        SW_MESSAGE_HANDLER(MSG_CAPTURERGB_ENABLE , OnSetCaptureRGBEnable );
        // ץ�Ŀ���ʹ��
        SW_MESSAGE_HANDLER(MSG_CAPTURESHUTTER_ENABLE , OnSetCaptureShutterEnable );
        // ץ������ʹ��
        SW_MESSAGE_HANDLER(MSG_CAPTUREGAIN_ENABLE , OnSetCaptureGainEnable );

        // ץ�Ĳ�����ʹ��
        SW_MESSAGE_HANDLER(MSG_SET_CAPTURE_ENABLE , OnSetCaptureEnable );

		 // ץ�Ŀ���
        SW_MESSAGE_HANDLER(MSG_SET_CAPTURESHUTTER , OnSetCaptureShutter );
        SW_MESSAGE_HANDLER(MSG_GET_CAPTURESHUTTER , OnGetCaptureShutter );
        // ץ������
        SW_MESSAGE_HANDLER(MSG_SET_CAPTRUEGAIN , OnSetCaptureGain )
        SW_MESSAGE_HANDLER(MSG_GET_CAPTUREGAIN , OnGetCaptureGain );

        // ץ�İ�ƽ��
        SW_MESSAGE_HANDLER(MSG_SET_CAPTRUERGBGAIN , OnSetCaptureRGBGain )
        SW_MESSAGE_HANDLER(MSG_GET_CAPTRUERGBGAIN , OnGetCaptureRGBGain );
        // ץ������
        SW_MESSAGE_HANDLER(MSG_SET_SOFT_TOUCH , OnSetCaptureSofttouch );

        // ���úͻ�ȡAGC�����ż��������
        SW_MESSAGE_HANDLER( MSG_SET_MAXSHU_GAIN , OnSetAGCMaxShuAndGain);
        SW_MESSAGE_HANDLER( MSG_GET_MAXSHU_GAIN , OnGetAGCMaxShuAndGain);

        // ���úͻ�ȡAGC��С���ż��������
        SW_MESSAGE_HANDLER( MSG_SET_MINSHU_GAIN , OnSetAGCMinShuAndGain);
        SW_MESSAGE_HANDLER( MSG_GET_MINSHU_GAIN , OnGetAGCMinShuAndGain);

		SW_MESSAGE_HANDLER( MSG_SET_AGCPARAM , OnSetAGCParam);

		// ��
		SW_MESSAGE_HANDLER(MSG_SET_SHARPENMODE,OnSetSharpeMode);
		SW_MESSAGE_HANDLER(MSG_SET_SHARPENTHRESHOLD,OnSetSharpeThreshold);

		// ͼ����ǿ
		SW_MESSAGE_HANDLER(MSG_SET_COLORMATRIXMODE, OnSetColoMaxtrixMode);
		// ��ƼӺ�
		SW_MESSAGE_HANDLER(MSG_SET_REDLIGHT_RECT, OnSetRedLightRect);

		SW_MESSAGE_HANDLER(MSG_SET_CONTRASTTHRESHOLD, OnSetContrastThreshold);
		SW_MESSAGE_HANDLER(MSG_SET_SATURATIONTHRESHOLD, OnSetSaturationThreshold);
		SW_MESSAGE_HANDLER(MSG_SET_TEMPERATURETHRESHOLD, OnSetTemperatureThreshold);
		SW_MESSAGE_HANDLER(MSG_SET_HUETHRESHOLD, OnSetHueThreshold);

		// ������
		SW_MESSAGE_HANDLER(MSG_SET_HAZEREMOVALMODE, OnSetHazeremovalMode);
		SW_MESSAGE_HANDLER(MSG_SET_HAZEREMOVALTHRESHOLD, OnSetHazeremovalThreshold);

		// IO����
		SW_MESSAGE_HANDLER(MSG_SET_FLASHRATE_ENABLE, OnSetFlashRateEnable);
		SW_MESSAGE_HANDLER(MSG_SET_CAPTURESYNC_ENABLE, OnSetCaptrueSyncEnable);
		SW_MESSAGE_HANDLER(MSG_SET_FLASHRATE_PULSE, OnSetFlashRatePulse);


		// �ⲿ�豸����
		SW_MESSAGE_HANDLER(MSG_SET_FILTERSWITCH , OnSetFilterSwitch);

		SW_MESSAGE_HANDLER(MSG_SET_DCAPERTUREENABLE , OnSetDCApertureEnable);
		
		// ����ץ����
		SW_MESSAGE_HANDLER(MSG_SET_CAPTUREEDGE, OnSetCaptureEdge);
		SW_MESSAGE_HANDLER(MSG_GET_CAPTUREEDGE, OnGetCaptureEdge);

		SW_MESSAGE_HANDLER(MSG_GET_BARRIER_STATE, OnGetBarrierState);


    SW_END_MESSAGE_MAP()

	//Զ����Ϣӳ�亯��
    SW_BEGIN_REMOTE_MESSAGE_MAP(CSWCameralControlMSG, CSWMessage)
		SW_REMOTE_MESSAGE_HANDLER(MSG_APP_REMOTE_GET_PTZ_INFO, OnGetPTZInfo)
    SW_END_REMOTE_MESSAGE_MAP();
	
private:
    // �����������ֵ������
    CSWMutex m_objSWMutex;
    // ���漰��ƽ������
    CSWAgcAwbTask *m_pSWAgcAwbTask;
    // ץ����Ϣ
    CSWCaptureTask *m_pSWCapture;
    // �����Խ�������
    CSWChannelDifferenceCorrectTask *m_pSWChanneCorrTask;
    // ͼ����ǿ�ȹ�������
    CSWImageQualitySet *m_pSWImageQualitySet;
    // �ⲿ�豸����
    CSWExtDevControl *m_pSWExtDevControl;
};
REGISTER_CLASS(CSWCameralControlMSG)
#endif // CSWCAMERALCONTROLMSG_H

