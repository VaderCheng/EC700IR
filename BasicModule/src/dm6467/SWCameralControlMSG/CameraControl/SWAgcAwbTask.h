#ifndef _SWAGCAWBTASK_H_
#define _SWAGCAWBTASK_H_

#include "SWTask.h"
#include "SWImageStatisticsInfo.h"


//#define GAIN_BASELINE 200 // �����׼ֵ��Ϊ�˽��ͼƬƫɫ���⣬������̧������6DB��

#define AWB_FREQ 10 // AWBÿ6֡����һ��
// AWB������ֵ
#define AWB_ADJUSTMENT_THRESHOLD 15

#define AWB_ADJUSTMENT_TIME      6

#define AWB_ADJUST(a,b,c,d,e)       \
    a = a + INT((b - 1.0f) * c);    \
    a = MAX(d, a);                  \
    a = MIN(e, a);                  \

// --- PID�����㷨 ---

/*
 * (1) ����ϵ����P����ϵͳ���ܵ�Ӱ�죺����ϵ���Ӵ�ʹϵͳ�Ķ����������ٶȼӿ죬��̬����С��Pƫ���񵴴����Ӷ࣬����ʱ��ӳ���P̫��ʱ��ϵͳ�����ڲ��ȶ���P̫С���ֻ�ʹϵͳ�Ķ���������
 * (2) ����ϵ����I����ϵͳ���ܵ�Ӱ�죺��������ʹϵͳ���ȶ����½���I�󣨻�������ǿ����ʹϵͳ���ȶ�������������̬�����ϵͳ�Ŀ��ƾ��ȡ�
 * (3) ΢��ϵ����D����ϵͳ���ܵ�Ӱ�죺΢�����ÿ��Ը��ƶ�̬���ԣ�Dƫ��ʱ���������ϴ󣬵���ʱ��϶̡�DƫСʱ����������С������ʱ��ϳ���ֻ��D���ʣ�����ʹ��������С�����̵���ʱ�䡣
 */

typedef struct _PID
{
    DOUBLE Command;	//������ֵ
    DOUBLE Proportion;	//����ϵ��
    DOUBLE Integral;	//����ϵ��
    DOUBLE Derivative;	//΢��ϵ��
    DOUBLE preErr;		//ǰ�����
    DOUBLE sumErr;		//����ۻ�
} PID;

typedef struct tagRGB_Sum
{
    INT iRSum;
    INT iGSum;
    INT iBSum;
} RGB_Sum;

class CSWAgcAwbTask : public CSWTask
{
CLASSINFO(CSWAgcAwbTask, CSWTask)

public:
    CSWAgcAwbTask();
    virtual ~CSWAgcAwbTask();

    virtual SInt64 Run( );

public:
    VOID ResetAGC();

    // ��ʼ����������ʼ����Ŀ���ǻ�ȡϵͳĬ������
    VOID Initial( );

    // ����ͼ������Ԥ��ֵ
    VOID SetAGCTh(INT iAGCTh);
    // ����ͼ������Ԥ��ֵ
    VOID GetAGCTh(INT &iAGCTh);

    // ����AGC�����СԼ��ֵ
    VOID SetAGCParam(INT iShutterMin, INT iShutterMax, INT iGainMin, INT iGainMax);
    // ���ô�AGC����ֵ������ֵ
    void SetAGCMaxParam( DWORD dwiAGCShutterHOri , DWORD iGainHMax);
    // ��ȡ���AGC����ֵ������ֵ
    void GetAGCParam( DWORD &dwiAGCShutterHOri , DWORD &iGainMax);
    // ������СAGC����ֵ������ֵ
    void SetAGCMinParam( DWORD &dwiAGCShutterLOri , DWORD &iGainLMax);
    // ��ȡ���AGC����ֵ������ֵ
    void GetAGCMinParam( DWORD &dwiAGCShutterLOri , DWORD &iGainLMax);

    // ���õ�ǰ����
    VOID SetAGCShutter(INT iShutter);
    // ��ȡ��ǰ����
    VOID GetAGCShutter(INT& iShutter);
    // ���õ�ǰ����
    VOID SetAGCGain(INT iGain);
    // ��ȡ��ǰ����
    VOID GetAGCGain( INT& iGain);

    // ���õ�ǰ����ֵ����ʽΪ0x00RGB,RΪRͨ�����棬GΪGͨ������ֵ �� BΪBͨ������ֵ
    VOID SetAWBGain( DWORD* pdwRGBGain );
     // ��ȡ��ǰ����ֵ����ʽΪ0x00RGB,RΪRͨ�����棬GΪGͨ������ֵ �� BΪBͨ������ֵ
    VOID GetAWBGain( DWORD &dwRGBGain );

    // ���ð�ƽ������
    VOID SetAWBGain(BYTE iGainR, BYTE iGainG, BYTE iGainB);
    // ��ȡ��ƽ������
    VOID GetAWBGain(BYTE& iGainR, BYTE& iGainG, BYTE& iGainB);

	// ����AGCʹ�ܱ�־
	VOID SetAGCEnable( BOOL isEnable );
    BOOL GetAGCEnable(){ return m_isEnableAGC;}
    // ����AWBʹ�ܱ�־
    VOID SetAWBEnable( BOOL isEnable ){
        m_isEnableAWB = isEnable;
        if( m_isEnableAWB ){
            m_nBeginTime = m_nAWBFlatRate = 0; this->SetAWBGain( 0 , 0 , 0);
        }
    }
    BOOL GetAWBEnable(){ return m_isEnableAWB;}

    // �����Զ���Ȧ
    void SetAutoAperture( bool IsAutoAperture ){ m_IsAutoAperture = IsAutoAperture;}

protected:
    //for awb
    INT CalcRGBGain(
        FLOAT fltGainR, INT &iGainR,
        FLOAT fltGainG, INT &iGainG,
        FLOAT fltGainB, INT &iGainB,
        INT iMin, INT iMax
    );
    INT AdjustRGBGain(INT &iGainR, INT &iGainG, INT &iGainB);

    //for agc
    DOUBLE PIDCale(PID *p, DOUBLE feedback);
    INT CalcShutterGain(
        INT iYArg,      //��ǰƽ�����ȣ�0~255��
        INT iAGCTh,     //����ƽ�����ȣ�0~255��
        INT iAGCShutterL,
        INT iAGCShutterH,
        INT iAGCGainL,
        INT iAGCGainH,
        INT &iShutter,
        INT &iGain
    );
private:
    // ����DC��Ȧ
    bool AdjustAperture(
        int iYArg,      //��ǰƽ�����ȣ�0~255��
        int iAGCTh,     //����ƽ�����ȣ�0~255��
        int iAGCShutterL,
        int iAGCShutterH,
        int iAGCGainL,
        int iAGCGainH,
        int &iShutter,
        int &iGain );
private:
    // AGC��AWB�㷨���Ĳ���
    INT m_iAgcAccurate;
    INT m_iAgcAdjustAccuracy;
    INT m_iAgcMaxCount;
    FLOAT m_fltShutterProportion;
    FLOAT m_fltShutterIntegral;
    FLOAT m_fltShutterDerivative;
    FLOAT m_fltGainProportion;
    FLOAT m_fltGainIntegral;
    FLOAT m_fltGainDerivative;
    FLOAT m_fltAwbAccurate;
    INT m_iAwbStep;

    FLOAT m_fltGainRSum;
    FLOAT m_fltGainGSum;
    FLOAT m_fltGainBSum;

    FLOAT m_fltOldGainRSum;
    FLOAT m_fltOldGainGSum;
    FLOAT m_fltOldGainBSum;

    INT m_iAwbMaxCount;

    PID m_cPIDShutter;
    PID m_cPIDGain;
    bool m_PIDInitFlag;

    // AGC��Ч������
    INT m_iAGCEffectCounter;

    // AGC��ǰ���ڷ���0��û�е��ڣ�1������������-1������������
    INT m_iAGCAdjustDirection;

private:
    // �Ƿ��Զ�����DC��Ȧ
    BOOL m_IsAutoAperture;
    // �����µ�ͼ��ƽ������ֵ
    static int  fiLasttimeYValue;

    INT m_iAGCTh;
    INT m_iAGCShutterLOri;
    INT m_iAGCShutterHOri;
    INT m_iAGCGainLOri;
    INT m_iAGCGainHOri;
    INT m_iNowShutter;
    INT m_iNowGain;

    // AWB����ֵ�����Ϊ255����СΪ0
    INT m_iNowGainR;
    INT m_iNowGainG;
    INT m_iNowGainB;

    // �豸CCDͨ����
    INT m_iCCDChannels;

    // AGCʹ�ܱ�־
    BOOL m_isEnableAGC;
    // AWBʹ�ܲ���
    BOOL m_isEnableAWB;
    // AWB����ƽ��
    WCHAR m_nAWBFlatRate;
    DWORD m_nBeginTime;

    // ��ʼ��ͳ�ƶ���
    static CSWImageStatisticsInfo m_objSWImageStatisticsInfo;
};

#endif

