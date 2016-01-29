#ifndef _AGCAWBTHREAD_H_
#define _AGCAWBTHREAD_H_

#include "hvthread.h"
#include "hvthreadbase.h"

#define GAIN_BASELINE 200 // �����׼ֵ��Ϊ�˽��ͼƬƫɫ���⣬������̧������6DB��

#define AWB_FREQ 6 // AWBÿ6֡����һ��

#define AWB_ADJUST(a,b,c,d,e)       \
    a = a + int((b - 1.0f) * c);    \
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
    double Command;	//������ֵ
    double Proportion;	//����ϵ��
    double Integral;	//����ϵ��
    double Derivative;	//΢��ϵ��
    double preErr;		//ǰ�����
    double sumErr;		//����ۻ�
} PID;

typedef struct tagRGB_Sum
{
    int iRSum;
    int iGSum;
    int iBSum;
} RGB_Sum;
// zhaopy auto
typedef struct _tagEnvInfo
{
    int iYSum;
    int iYPlate;
    int iShutter;
    int iGain;
}
ENV_INFO;

typedef enum
{
    ENV_UNKNOW = 0,
    ENV_CLOUDY,
    ENV_DAY,
    ENV_BLIGHT,
    ENV_FLIGHT,
    ENV_NEIGHT,
    ENV_COUNT
}
ENV_TYPE;
class CAgcAwbThread : public CHvThreadBase
{
public:
    CAgcAwbThread();
    ~CAgcAwbThread();

public:
    virtual const char* GetName()
    {
        static char szName[] = "CAgcAwbThread";
        return szName;
    }

    virtual HRESULT Run(void *pvParamter);

public:
    void ResetAGC();
    void SetAGCTh(int iAGCTh);
    void SetAGCParam(int iShutterMin, int iShutterMax, int iGainMin, int iGainMax);
    void SetAGCShutterGain(int iShutter, int iGain);
    void GetAGCShutterGain(int& iShutter, int& iGain);
    void SetAWBGain(int iGainR, int iGainG, int iGainB);
    void GetAWBGain(int& iGainR, int& iGainG, int& iGainB);

    HRESULT PutYArg(int iYArg);
    HRESULT PutRgbSum(RGB_Sum cRgbSum);

    // zhaopy auto capture param...
    ENV_TYPE GetEnvType()
    {
        return m_envType;
    }
    HRESULT PutEnvInfo(ENV_INFO& envInfo);
protected:
    HRESULT GetYArg(int& iYArg);
    HRESULT GetRgbSum(RGB_Sum& cRgbSum);
    // zhaopy auto...
    HRESULT GetEnvInfo(ENV_INFO& envInfo);
    ENV_TYPE CheckEnvType(ENV_INFO& envInfo);

    void InitAgcAwbCoreParam();

    //for awb
    int CalcRGBGain(
        float fltGainR, int &iGainR,
        float fltGainG, int &iGainG,
        float fltGainB, int &iGainB,
        int iMin, int iMax
    );
    int AdjustRGBGain(int &iGainR, int &iGainG, int &iGainB);

    //for agc
    double PIDCale(PID *p, double feedback);
    int CalcShutterGain(
        int iYArg,      //��ǰƽ�����ȣ�0~255��
        int iAGCTh,     //����ƽ�����ȣ�0~255��
        int iAGCShutterL,
        int iAGCShutterH,
        int iAGCGainL,
        int iAGCGainH,
        int &iShutter,
        int &iGain
    );

private:
    // zhaopy auto...
    HiVideo::CHvList<ENV_INFO> m_listEnvInfo;
    ENV_TYPE m_envType;
    ENV_TYPE m_etLast;
    DWORD32 m_dwLastETTick;
    HV_SEM_HANDLE m_hSemEnvInfo;	
    HiVideo::CHvList<int> m_listYArg;
    HiVideo::CHvList<RGB_Sum> m_listRgbSum;
    HV_SEM_HANDLE m_hSemListYArg;
    HV_SEM_HANDLE m_hSemListRgbSum;
    // Ϊ��ʵʱ�ԣ����г��ȱ�����Ϊ1��
    static const int LIST_YARG_MAX_COUNT = 1;
    static const int LIST_RGBSUM_MAX_COUNT = 1;
    static const int LIST_ENVINFO_MAX_COUNT = 10;
private:
    // AGC��AWB�㷨���Ĳ���
    int m_iAgcAccurate;
    int m_iAgcAdjustAccuracy;
    int m_iAgcMaxCount;
    float m_fltShutterProportion;
    float m_fltShutterIntegral;
    float m_fltShutterDerivative;
    float m_fltGainProportion;
    float m_fltGainIntegral;
    float m_fltGainDerivative;
    float m_fltAwbAccurate;
    int m_iAwbStep;

    float m_fltGainRSum;
    float m_fltGainGSum;
    float m_fltGainBSum;

    int m_iAwbMaxCount;

    PID m_cPIDShutter;
    PID m_cPIDGain;
    bool m_PIDInitFlag;

    // AGC��Ч������
    int m_iAGCEffectCounter;

    // AGC��ǰ���ڷ���0��û�е��ڣ�1������������-1������������
    int m_iAGCAdjustDirection;

private:
    int m_iAGCTh;
    int m_iAGCShutterLOri;
    int m_iAGCShutterHOri;
    int m_iAGCGainLOri;
    int m_iAGCGainHOri;
    int m_iNowShutter;
    int m_iNowGain;
    int m_iNowGainR;
    int m_iNowGainG;
    int m_iNowGainB;
};

#endif
