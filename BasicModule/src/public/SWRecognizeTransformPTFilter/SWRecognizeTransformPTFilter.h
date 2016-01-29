/**
 * @file SWRecognizeTransformPTFilter.h
 * @brief ʶ��Filter
 * @copyright Signalway All Rights Reserved
 * @author zhaopy
 * @date 2013-03-20
 * @version 1.0
 */

#ifndef __SW_RECOGNIZE__TRANSFORM_FILTER_H__
#define __SW_RECOGNIZE__TRANSFORM_FILTER_H__

#include "SWBaseFilter.h"
#include "SWMessage.h"
#include "SWCarLeft.h"
#include "SWPosImage.h"
#include "SWCameraDataPDU.h"
#include "SWDspLinkCtrl.h"

class CSWRecognizeTransformPTFilter: public CSWBaseFilter, CSWMessage
{
CLASSINFO(CSWRecognizeTransformPTFilter, CSWBaseFilter)
    public:
    CSWRecognizeTransformPTFilter();
    virtual ~CSWRecognizeTransformPTFilter();

    /**
     *@brief ʶ��ģ���ʼ����
     */
    virtual HRESULT Initialize(INT iGlobalParamIndex, PVOID pvFrameParam, PVOID pvCfgParam);
    virtual HRESULT RegisterCallBackFunction(PVOID OnDSPAlarm, PVOID pvParam);
    virtual HRESULT SetOutputImage(INT iBest, INT iLast);
    virtual HRESULT Run();
    virtual HRESULT Stop();

    static VOID OnProcessProxy(PVOID pvParam);

public:
    /**
     *@brief ISWPlateRecognitionEvent �ص��ӿ�
     */
    virtual HRESULT CarArriveEvent(CARARRIVE_INFO_STRUCT *pCarArriveInfo);
    virtual HRESULT CarLeftEvent(CARLEFT_INFO_STRUCT *pCarLeftInfo, const BOOL fEvasion, const DWORD dwEvasionTick);

protected:
    /**
     *@brief ����Receive�ӿڣ�����CSWImage����
     */
    virtual HRESULT Receive(CSWObject* obj);

    /**
     *@brief ʶ�����߳�
     */
    HRESULT OnProcess();

    /**
     *@brief ����ʶ�𿪹�
     */
    HRESULT OnRecognizeGetJPEG(WPARAM wParam, LPARAM lParam);

    /**
     *@brief �����������
     */
    HRESULT OnRecognizeOutPutDebug(WPARAM wParam, LPARAM lParam);

    /**
     *@brief ���ü��忨�ӷѱ�־
    */
    HRESULT SetTollEvasionDetectingFlag(const BOOL fDetectingFlag)
    {
        m_fDetectingTollEvasion = fDetectingFlag;
    return S_OK;
    }

    /**
     *@brief ��ȡ���忨�ӷѱ�־
    */
    BOOL GetTollEvasionDetectingFlag(VOID)
    {
        return m_fDetectingTollEvasion;
    }
    
    /**
    *@brief ���ø����忨ʱ������ֵ
    */
    HRESULT SetTailgatingTimeThreshold(const INT iTime)
    {
        if (0 < iTime )
        {
            m_iTailgatingTimeThreshold = iTime;
            return S_OK;
        }
        else
        {
            return E_INVALIDARG;
        }
    }

    /**
    *@brief ��ȡ�����忨ʱ������ֵ
    */
    INT GetTailgatingTimeThreshold()
    {
        return m_iTailgatingTimeThreshold;
    }

    /**
    *@brief �������˻���̬������״̬�������
    */
    HRESULT SetBarrierNormalState(const INT iMode)
    {
        m_iBarrierNormalMode = iMode;
        return S_OK;
    }

    /**
    *@brief ��ȡ���˻���̬������״̬�������
    */
    INT GetBarrierNormalState()
    {
        return m_iBarrierNormalMode;
    }

    /**
    *@brief ��ȡ����״̬��־
    */
    HRESULT GetBarrierStatus(INT& iStatus);

    /**
    *@brief �ж��Ƿ�忨�ӷ�
    */
    HRESULT CheckTollEvasion(BOOL& fEvasion);

protected:
    //�Զ���ӳ���
    SW_BEGIN_DISP_MAP(CSWRecognizeTransformFilter, CSWBaseFilter)
        SW_DISP_METHOD(Initialize, 3)
        SW_DISP_METHOD(RegisterCallBackFunction, 2)
        SW_DISP_METHOD(SetOutputImage, 2)
        SW_DISP_METHOD(SetTollEvasionDetectingFlag, 1)
        SW_DISP_METHOD(SetTailgatingTimeThreshold, 1)
        SW_DISP_METHOD(SetBarrierNormalState, 1)
    SW_END_DISP_MAP()
    //��Ϣӳ���
    SW_BEGIN_MESSAGE_MAP(CSWRecognizeTransformPTFilter, CSWMessage)
        SW_MESSAGE_HANDLER(MSG_RECOGNIZE_GETJPEG, OnRecognizeGetJPEG)
        SW_MESSAGE_HANDLER(MSG_RECOGNIZE_OUTPUTDEBUG, OnRecognizeOutPutDebug)
    SW_END_MESSAGE_MAP()

private:
    VOID Clear();

public:
    static const INT MAX_IMAGE_COUNT = 3;

private:
    TRACKER_CFG_PARAM *m_pTrackerCfg;
    PR_PARAM m_cPlateRecognitionParam;
    BOOL m_fSendJPEG;
    BOOL m_fSendDebug;
    BOOL m_fInitialized;
    INT m_iCarArriveTriggerType;
    INT m_iIOLevel;

    INT m_iOutPutBestImage;
    INT m_iOutPutLastImage;

    CSWList<CSWImage*> m_lstImage;		// ͼƬ����
    CSWMutex m_cMutexImage;             // �����ٽ���
    CSWThread* m_pThread;				// ʶ���߳�
    CSWSemaphore m_cSemImage;

    BOOL m_fDetectingTollEvasion;  //�Ƿ���忨�ӷ�
    INT m_iTailgatingTimeThreshold; //�жϸ����忨ʱ����ֵ
    INT m_iBarrierNormalMode; //���˻�����ģʽ���

    CSWList<BOOL> m_lstTollEvasionFlag;        // �忨��־����
    CSWMutex m_cMutexTollEvasion;             // �����ٽ���
};
REGISTER_CLASS(CSWRecognizeTransformPTFilter)
#endif

