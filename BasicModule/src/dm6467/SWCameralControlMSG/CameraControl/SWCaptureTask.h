#ifndef CSWCAPTURE_H
#define CSWCAPTURE_H
#include "SWTask.h"

class CSWCaptureTask : public CSWTask
{
CLASSINFO(CSWCaptureTask, CSWTask)

public:
    CSWCaptureTask();
    virtual ~CSWCaptureTask();


    // ��������ʹ��
    INT SetRGBEnable( BOOL isEnable );
    // ���ÿ���ʹ��
    INT SetShutterEnable( BOOL isEnable );
    // ��������ʹ��
    INT SetGainEnable( BOOL isEnable );


    // ���õ�ǰ����
    VOID SetShutter(INT iShutter);
    // ��ȡ��ǰ����
    VOID GetShutter(INT& iShutter);
    // ���õ�ǰ����
    VOID SetGain(INT iGain);
    // ��ȡ��ǰ����
    VOID GetGain( INT& iGain);

    // ���õ�ǰ����ֵ����ʽΪ0x00RGB,RΪRͨ�����棬GΪGͨ������ֵ �� BΪBͨ������ֵ
    VOID SetRGBGain( DWORD dwRGBGain );
     // ��ȡ��ǰ����ֵ����ʽΪ0x00RGB,RΪRͨ�����棬GΪGͨ������ֵ �� BΪBͨ������ֵ
    VOID GetRGBGain( DWORD &dwRGBGain );

	virtual SInt64 Run();

private:


    // ץ�Ĳ����Խ���
    void SetCaptureYinyangSideFactor( float fCaptureFactor );
private:
    INT m_iCaptureShutter;
    INT m_iCaptureGain;

    INT m_iNowGainR;
    INT m_iNowGainG;
    INT m_iNowGainB;

};

#endif // CSWCAPTURE_H

