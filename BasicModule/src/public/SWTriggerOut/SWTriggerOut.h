/*
 * CSWTriggerOut.h
 *
 *  Created on: 2013-7-22
 *      Author: Administrator
 */

#ifndef CSWTriggerOut_H_
#define CSWTriggerOut_H_

#include "SWFC.h"
#include "SWBaseFilter.h"
#include "SWMessage.h"

typedef struct _tag_TriggerOutInitParam
{
    BOOL fEnableTriggerOut;
    int nCaptureSynOutputType;
    int nTriggerOutNormalStatus;
    int nTriggerOutPlusWidth;
} TRIGGER_OUT_INITPARAM;

class CSWTriggerOut: public CSWObject, CSWMessage
{
CLASSINFO(CSWTriggerOut, CSWObject)
public:
    CSWTriggerOut();
    virtual ~CSWTriggerOut();

protected:
    HRESULT Run();
    HRESULT Stop();

    static void* OnTriggerOutRun(void* pvParam);

    /**
     *@breif ��ʼ��
     *@param [in] fEnableTriggerOut �������ʹ�ܿ���
     *@param [in] nTriggerOutNormalStatus ���������̬��ƽ
     *@param [in] nTriggerOutPlusWidth �������ͨ��ʱ��
     *@return �ɹ�����S_OKʧ�ܷ���E_FAIL
     */
    HRESULT InitTrigger(
        BOOL fEnableTriggerOut,
        int nTriggerOutNormalStatus,
        int nTriggerOutPlusWidth,
        int nCaptureSynOutputType
        );

    /**
     *@breif �������
     *@param [in] wParam, ��
     *@param [in] lParam, ��
     *@return �ɹ�����S_OKʧ�ܷ���E_FAIL
     */
    HRESULT OnTriggerProcess(WPARAM wParam, LPARAM lParam);

    //�Զ���ӳ���
    SW_BEGIN_DISP_MAP(CSWTriggerOut, CSWObject)
        SW_DISP_METHOD(Run, 0)
        SW_DISP_METHOD(Stop, 0)
        SW_DISP_METHOD(InitTrigger, 4)
    SW_END_DISP_MAP()

    //��Ϣӳ���
    SW_BEGIN_MESSAGE_MAP(CSWTriggerOut, CSWMessage)
        SW_MESSAGE_HANDLER(MSG_TRIGGER_OUT_PROCESS, OnTriggerProcess)
    SW_END_MESSAGE_MAP()

public:
    CSWSemaphore m_semCount;

private:
    CSWThread m_thTriggerPro;
    TRIGGER_OUT_INITPARAM m_cParam;
    BOOL m_fIsInited;
};
REGISTER_CLASS(CSWTriggerOut)
#endif /* CSWTriggerOut_H_ */
