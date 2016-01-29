/**
 * @file   SWDomeRockerControlMSG.h
 * @author 
 * @date   Fri Aug 01 08:43:45 2014
 * 
 * @brief  ľ��ƽ̨ҡ�˽���ģ��
 *         ���̣�ҡ�� ->(rs485)               |-> �佹���Խ�����Ȧ -> ��о
 *               -> �������ݲ����� -> ת������| 
 *                                            |-> �������� -> ���
 * @note ֻ֧��pelco-dЭ��
 */

#ifndef SWDOMEROCKERCONTROLMSG_H_
#define SWDOMEROCKERCONTROLMSG_H_

#include "SWFC.h"
#include "SWBaseFilter.h"
#include "SWMessage.h"


class CSWDomeRockerControlMSG: public CSWObject, CSWMessage
{
CLASSINFO(CSWDomeRockerControlMSG, CSWObject)
public:
	CSWDomeRockerControlMSG();
    virtual ~CSWDomeRockerControlMSG();

protected:
    HRESULT Run();
    HRESULT Stop();

    static VOID OnProcessProxy(PVOID pvParam);
    HRESULT OnProcess();

    // ˵����iProtocol��Ԥ��������Ŀǰֻ֧��һ�֣���ʵ����ûʹ�õ�
    HRESULT Initialize(BOOL fEnable, INT iBaudrate, INT iProtocol);
    HRESULT OpenSerial(const char *pDeviceName, int iBaudrate, int iDatabits, int iParity, int iStopbits);
	virtual HRESULT Close(VOID);
	virtual HRESULT Read(PVOID pBuffer, INT* iSize);

    int DispatchCommand(unsigned char* szCommand, int iCommandLen);
    void ProcessPelcoCommand(unsigned char* szCommand, int iCommandLen);
    //////////////
    //�Զ���ӳ���
    SW_BEGIN_DISP_MAP(CSWDomeRockerControlMSG, CSWObject)
        SW_DISP_METHOD(Initialize, 3)
        SW_DISP_METHOD(Run, 0)
        SW_DISP_METHOD(Stop, 0)
    SW_END_DISP_MAP()

private:
    CSWThread m_thPro;
    int m_iNeedSleep;
    int m_iFd;
};
REGISTER_CLASS(CSWDomeRockerControlMSG)

#endif /*  */
