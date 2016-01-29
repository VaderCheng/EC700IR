#include "SWAgcAwbTask.h"
#include "SWImageStatisticsInfo.h"
#include "swpa_camera.h"
#include "SWLog.h"

// DC��Ȧ��س���ֵ
#define YMAX_DIFF  5
// �����׼ֵ
#define GAIN_BASELINE 200




#ifndef MIN
#define MIN(x, y) ((x) > (y) ? (y) : (x))
#endif

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

static FLOAT fdbThresholdValue = 0.10;
// ��ʼ��ͳ�ƶ���
CSWImageStatisticsInfo CSWAgcAwbTask::m_objSWImageStatisticsInfo;
// �����µ�ͼ��ƽ������ֵ
int  CSWAgcAwbTask::fiLasttimeYValue = 0;

CSWAgcAwbTask::CSWAgcAwbTask()
: m_iAGCShutterLOri( 0 )
, m_iAGCShutterHOri( 0 )
, m_iAGCGainLOri( 0 )
, m_iAGCGainHOri( 0 )
, m_iCCDChannels( 0 )
, m_isEnableAGC( 0 )
, m_isEnableAWB( 0 )
, m_iAGCTh( 100 )
, m_iAwbStep( 100 )
, m_iAgcAccurate( 15 )
, m_iAGCEffectCounter( 0 )
, m_iNowShutter( 0 )
, m_iNowGain( 0 )
, m_iAwbMaxCount( 0 )
, m_PIDInitFlag( false )
, m_IsAutoAperture( false )
, m_nAWBFlatRate( 0 )
, m_nBeginTime( 0 )
{
    m_iAgcAccurate = 15;
    m_iAgcAdjustAccuracy = 5;
    m_iAgcMaxCount = 30;
    m_fltShutterProportion = 0.005;
    m_fltShutterIntegral = 0.005;
    m_fltShutterDerivative = 0.005;
    m_fltGainProportion = 0.01;
    m_fltGainIntegral = 0.01;
    m_fltGainDerivative = 0.01;
    m_fltAwbAccurate = 0.05;


    m_fltGainRSum = m_fltGainGSum = m_fltGainBSum = 0;
    m_iNowGainR =  m_iNowGainG = m_iNowGainB = 0;

    memset(&m_cPIDShutter, 0, sizeof(m_cPIDShutter));
    memset(&m_cPIDGain, 0, sizeof(m_cPIDGain));

    m_iAGCEffectCounter = m_iAGCAdjustDirection = 0;

    CSWImageStatisticsInfo::Initialization( );
}

CSWAgcAwbTask::~CSWAgcAwbTask()
{

}
VOID CSWAgcAwbTask::Initial( )
{
    swpa_camera_basicparam_get_shutter( &m_iNowShutter );
    swpa_camera_basicparam_get_gain( &m_iNowGain );
}
SInt64 CSWAgcAwbTask::Run( )
{
static INT fOldYarg = 0 ;
static INT fnDiffValue = 30;
static INT fnTime = 80;

	EventFlags events = this->GetEvents( );

	if( events & CSWTask::kIdleEvent ) return 0;
	if( !GetAWBEnable( ) && !GetAGCEnable() ) return 0;

	while( !(events & CSWTask::kKillEvent) )
	{
	    // �����Ҫʹ��AGC
	    if( GetAGCEnable( ))
	    {
            INT iYArg = CSWImageStatisticsInfo::GetPartYArgValue();
            if( iYArg != -2)
            {
                fOldYarg = iYArg >= 0 ? iYArg : fOldYarg;
                if( swpa_abs( m_iAGCTh - iYArg ) > fnDiffValue ){
                     iYArg = fOldYarg; fnTime = 20;
                }
                else  fnTime = 80;

                if( iYArg >= 0 )
                {

                    if ( !m_IsAutoAperture && 1 == CalcShutterGain(
                                    iYArg,
                                    m_iAGCTh,  // YԤ��ֵ��������������
                                    m_iAGCShutterLOri, m_iAGCShutterHOri,
                                    m_iAGCGainLOri, m_iAGCGainHOri,
                                    m_iNowShutter,
                                    m_iNowGain) ){

                            this->SetAGCShutter(m_iNowShutter);
                            this->SetAGCGain(m_iNowGain);

                            SW_TRACE_DEBUG( "<AGC> Y-th-ss-gg-s-g[%d, %d, %d-%d, %d-%d, %d, %d]\n",
                                     iYArg,
                                     m_iAGCTh,
                                     m_iAGCShutterLOri, m_iAGCShutterHOri,
                                     m_iAGCGainLOri, m_iAGCGainHOri,
                                     m_iNowShutter,
                                     m_iNowGain);
                        }
                        else if ( m_IsAutoAperture && 1 == AdjustAperture(
                                    iYArg,
                                    m_iAGCTh,  // YԤ��ֵ��������������
                                    m_iAGCShutterLOri, m_iAGCShutterHOri,
                                    m_iAGCGainLOri, m_iAGCGainHOri,
                                    m_iNowShutter,
                                    m_iNowGain) )
                        {

                            this->SetAGCShutter(m_iNowShutter);
                            this->SetAGCGain(m_iNowGain);

                            SW_TRACE_DEBUG( "<DCaperture_AND_AGC> Y-th-ss-gg-s-g[%d, %d, %d-%d, %d-%d, %d, %d]\n",
                                     iYArg,
                                     m_iAGCTh,
                                     m_iAGCShutterLOri, m_iAGCShutterHOri,
                                     m_iAGCGainLOri, m_iAGCGainHOri,
                                     m_iNowShutter,
                                     m_iNowGain);
                        }
                    }
                }
	    }
        // �����Ҫʹ�ܰ�ƽ��
        if( GetAWBEnable( )){

            INT iRSum , iGSum ,iBSum;
            CSWImageStatisticsInfo::GetRGBSum(iRSum , iGSum , iBSum);

            //AWB����
            if ( iRSum > 0 && iBSum > 0 )
            {
                FLOAT fltR = 1.0*iGSum / iRSum;
                FLOAT fltB = 1.0*iGSum / iBSum;

                if ( 1 == CalcRGBGain(
                            fltR, m_iNowGainR,
                            1.0, m_iNowGainG,
                            fltB, m_iNowGainB,
                            -100, 511) )
                {

                    this->AdjustRGBGain(m_iNowGainR, m_iNowGainG, m_iNowGainB);
                    this->SetAWBGain( m_iNowGainR,m_iNowGainG, m_iNowGainB);

                    SW_TRACE_DEBUG( "<AWB> GainR[%d:%d]; GainG[%d:%d]; GainB[%d:%d]\n",
                             iRSum , m_iNowGainR, iGSum , m_iNowGainG, iBSum , m_iNowGainB);

                }
            }
        }

        return fnTime;
    }

    return -1;
}

void CSWAgcAwbTask::SetAGCTh(INT iAGCTh)
{
    m_iAGCTh = iAGCTh;
}
// ����ͼ������Ԥ��ֵ
VOID CSWAgcAwbTask::GetAGCTh(INT &iAGCTh)
{
    iAGCTh = m_iAGCTh;
}

// ������СAGC����ֵ������ֵ
void CSWAgcAwbTask::SetAGCMaxParam( DWORD dwiAGCShutterHOri , DWORD iGainHMax)
{
    this->SetAGCParam( m_iAGCShutterLOri , dwiAGCShutterHOri , m_iAGCGainLOri , iGainHMax);
}
// ������СAGC����ֵ������ֵ
void CSWAgcAwbTask::SetAGCMinParam( DWORD &dwiAGCShutterLOri , DWORD &iGainLMax)
{
    this->SetAGCParam( dwiAGCShutterLOri , m_iAGCShutterHOri , iGainLMax , m_iAGCGainHOri);
}
// ��ȡ���AGC����ֵ������ֵ
void CSWAgcAwbTask::GetAGCMinParam( DWORD &dwiAGCShutterLOri , DWORD &iGainLMax)
{
    dwiAGCShutterLOri = m_iAGCShutterLOri; iGainLMax = m_iAGCGainLOri;
}
// ����AGCʹ�ܱ�־
VOID CSWAgcAwbTask::SetAGCEnable( BOOL isEnable )
{
	m_isEnableAGC = isEnable;
	if( m_isEnableAGC )
	{
		m_iNowShutter = MIN(m_iAGCShutterHOri, m_iNowShutter);
		m_iNowShutter = MAX(m_iAGCShutterLOri, m_iNowShutter);
		m_iNowGain = MIN(m_iAGCGainHOri, m_iNowGain);
		m_iNowGain = MAX(m_iAGCGainLOri, m_iNowGain);
		this->SetAGCShutter( m_iNowShutter );
		this->SetAGCGain( m_iNowGain );
	}
}
void CSWAgcAwbTask::SetAGCParam(INT iShutterMin, INT iShutterMax, INT iGainMin, INT iGainMax)
{
    m_iAGCShutterLOri = iShutterMin;
    m_iAGCShutterHOri = iShutterMax;

    m_iAGCGainLOri = iGainMin;
    m_iAGCGainHOri = iGainMax;

    if( m_iAGCShutterHOri == 0 || m_iAGCGainHOri == 0 ) return ;

	if( GetAGCEnable() )
	{
		m_iNowShutter = MIN(m_iAGCShutterHOri, m_iNowShutter);
		m_iNowShutter = MAX(m_iAGCShutterLOri, m_iNowShutter);
		m_iNowGain = MIN(m_iAGCGainHOri, m_iNowGain);
		m_iNowGain = MAX(m_iAGCGainLOri, m_iNowGain);
	}

    this->SetAGCShutter( m_iNowShutter );
    this->SetAGCGain( m_iNowGain );
}
void CSWAgcAwbTask::GetAGCParam( DWORD &dwiAGCShutterHOri , DWORD &iGainMax)
{
    dwiAGCShutterHOri = m_iAGCShutterHOri ; iGainMax = m_iAGCGainHOri;
}

void CSWAgcAwbTask::SetAGCShutter(INT iShutter)
{
    swpa_camera_basicparam_set_shutter((m_iNowShutter = iShutter));
}

void CSWAgcAwbTask::GetAGCShutter(INT& iShutter)
{
    iShutter = m_iNowShutter;
}

void CSWAgcAwbTask::SetAGCGain(INT iGain)
{
    swpa_camera_basicparam_set_gain((m_iNowGain = iGain));
}

void CSWAgcAwbTask::GetAGCGain( INT& iGain)
{
    iGain = m_iNowGain;
}

// ���õ�ǰ����ֵ����ʽΪ0x00RGB,RΪRͨ�����棬GΪGͨ������ֵ �� BΪBͨ������ֵ
VOID CSWAgcAwbTask::SetAWBGain( DWORD* pdwRGBGain )
{
    this->SetAWBGain( pdwRGBGain[0] , pdwRGBGain[1] , pdwRGBGain[2]);
}
 // ��ȡ��ǰ����ֵ����ʽΪ0x00RGB,RΪRͨ�����棬GΪGͨ������ֵ �� BΪBͨ������ֵ
VOID CSWAgcAwbTask::GetAWBGain( DWORD &dwRGBGain )
{
    dwRGBGain = m_iNowGainB;
    dwRGBGain |= (m_iNowGainG << 8);
    dwRGBGain |= (m_iNowGainR << 16);
}
void CSWAgcAwbTask::SetAWBGain(BYTE iGainR, BYTE iGainG, BYTE iGainB)
{
    m_iNowGainR = iGainR;
    m_iNowGainG = iGainG;
    m_iNowGainB = iGainB;

    swpa_camera_basicparam_set_rgb_gain( m_iNowGainR , m_iNowGainG , m_iNowGainB );
}

void CSWAgcAwbTask::GetAWBGain(BYTE& iGainR, BYTE& iGainG, BYTE& iGainB)
{
    iGainR = m_iNowGainR;
    iGainG = m_iNowGainG;
    iGainB = m_iNowGainB;
}

INT CSWAgcAwbTask::CalcRGBGain(
    FLOAT fltGainR, INT &iGainR,
    FLOAT fltGainG, INT &iGainG,
    FLOAT fltGainB, INT &iGainB,
    INT iMin, INT iMax
)
{
    if ( m_iAwbMaxCount++ < AWB_FREQ )
    {
        m_fltGainRSum += fltGainR;
        m_fltGainGSum += fltGainG;
        m_fltGainBSum += fltGainB;

        return 0;
    }
    else
    {
        m_fltGainRSum += fltGainR;
        m_fltGainGSum += fltGainG;
        m_fltGainBSum += fltGainB;

        fltGainR = m_fltGainRSum / AWB_FREQ;
        fltGainG = m_fltGainGSum / AWB_FREQ;
        fltGainB = m_fltGainBSum / AWB_FREQ;

        m_iAwbMaxCount = 0;
        m_fltGainRSum = m_fltGainGSum = m_fltGainBSum = 0;

        // ����ƽ��󣬽��ж��Դ���
        if( m_nBeginTime++ > AWB_ADJUSTMENT_TIME && (swpa_fabs(1.0 - fltGainR) >= fdbThresholdValue
           || swpa_fabs(1.0 - fltGainB) >= fdbThresholdValue) )
           {    // ���ö��Է�ֵ
                if( m_nBeginTime >= 0xFFFFFEEB ) m_nBeginTime = AWB_ADJUSTMENT_TIME;
                if( m_nAWBFlatRate++ < AWB_ADJUSTMENT_THRESHOLD) return 0;
           }
        else if( m_nBeginTime > AWB_ADJUSTMENT_TIME ){m_nAWBFlatRate = 0; return 0;}
        // ��ձ�־
        if( m_nBeginTime > AWB_ADJUSTMENT_TIME ) m_nBeginTime = 0;

        INT iRet = 0;
        if ( swpa_fabs(1.0f - fltGainR) >= m_fltAwbAccurate )
        {
            AWB_ADJUST(iGainR, fltGainR, m_iAwbStep, iMin, iMax);
            iRet = 1;
        }
        if ( swpa_fabs(1.0f - fltGainG) >= m_fltAwbAccurate )
        {
            AWB_ADJUST(iGainG, fltGainG, m_iAwbStep, iMin, iMax);
            iRet = 1;
        }
        if ( swpa_fabs(1.0f - fltGainB) >= m_fltAwbAccurate )
        {
            AWB_ADJUST(iGainB, fltGainB, m_iAwbStep, iMin, iMax);
            iRet = 1;
        }

        m_nAWBFlatRate = 0; return iRet;
    }
}

INT CSWAgcAwbTask::AdjustRGBGain(INT &iGainR, INT &iGainG, INT &iGainB)
{
    INT iMin = MIN(iGainR, MIN(iGainG, iGainB));
    iGainR -= iMin;
    iGainG -= iMin;
    iGainB -= iMin;
    iGainR = MIN(iGainR, 511);
    iGainG = MIN(iGainG, 511);
    iGainB = MIN(iGainB, 511);
    return 0;
}

DOUBLE CSWAgcAwbTask::PIDCale(PID *p, DOUBLE feedback)
{
    DOUBLE Err=0,dErr=0;

    Err = p->Command - feedback;	//��ǰ���
    p->sumErr += Err;				//����ۼ�
    dErr = Err - p->preErr;			//���΢��
    p->preErr = Err;

    return (p->Proportion * Err		//������
            + p->Derivative * dErr	    //΢����
            + p->Integral * p->sumErr);	//������
}

void CSWAgcAwbTask::ResetAGC()
{
    m_iAGCEffectCounter = 0;
    m_iAGCAdjustDirection = 0;
}

/**
* @brief �Զ�AGC
* @param [in] iYArg ͼ��ƽ������
* @param [in] iAGCTh ͼ������ƽ������
* @param [in] iAGCShutterL ��Ϳ���
* @param [in] iAGCShutterH ������
* @param [in] iAGCGainL �������
* @param [in] iAGCGainH �������
* @param [out] iShutter ��ǰ����
* @param [out] iGain ��ǰ����
* @retval true �ɹ�
* @retval 0 ʧ��
*/
INT CSWAgcAwbTask::CalcShutterGain(
    INT iYArg,      //��ǰƽ�����ȣ�0~255��
    INT iAGCTh,     //����ƽ�����ȣ�0~255��
    INT iAGCShutterL,
    INT iAGCShutterH,
    INT iAGCGainL,
    INT iAGCGainH,
    INT &iShutter,
    INT &iGain
)
{
    static INT fCurrentCounts = 0 ;
    const INT iDiff = abs(iYArg - iAGCTh);


    // �������������ʱ��AGC��Ч��������ʼ������������㡣
    if (iDiff > m_iAgcAccurate)
    {
        m_iAGCEffectCounter++;
    }
    else if (m_iAGCEffectCounter > m_iAgcMaxCount && iDiff > m_iAgcAdjustAccuracy)
    {
        m_iAGCEffectCounter++;
    }
    else
    {
        m_iAGCEffectCounter = 0;
    }

    if (m_iAGCEffectCounter < m_iAgcMaxCount)
    {
        m_iAGCAdjustDirection = 0;

        m_cPIDShutter.preErr = 0;
        m_cPIDShutter.sumErr = 0;
        m_cPIDGain.preErr = 0;
        m_cPIDGain.sumErr = 0;
        return 0;  // ������AGC���ڣ���Ϊ��δ�����������ڵ�����
    }

    // iAGCShutterMid��Ϊ�˽��ĳЩ�����
    // ��ϣ����Ϊ���ŵ�������������β��������һ����ʱ������
    // ��Ϸ�ֵ����iAGCShutterL��iAGCShutterH֮�䡣
    const INT iAGCShutterMid = iAGCShutterL + INT((iAGCShutterH - iAGCShutterL) * 0.8);

    // ���ֳ��ֵ��ڱ���ʱ��������λPID������PID���񵴡�
    if (1 == m_iAGCAdjustDirection && iYArg < iAGCTh)
    {
        m_iAGCAdjustDirection = -1;

        m_cPIDShutter.preErr = 0;
        m_cPIDShutter.sumErr = 0;
        m_cPIDGain.preErr = 0;
        m_cPIDGain.sumErr = 0;
    }
    else if (-1 == m_iAGCAdjustDirection && iYArg > iAGCTh)
    {
        m_iAGCAdjustDirection = 1;

        m_cPIDShutter.preErr = 0;
        m_cPIDShutter.sumErr = 0;
        m_cPIDGain.preErr = 0;
        m_cPIDGain.sumErr = 0;
    }
    else // 0 == m_iAGCAdjustDirection
    {
        m_iAGCAdjustDirection = (iYArg > iAGCTh) ? (1) : (-1);
    }

    // �Ƿ��ʼ��PID����ز���
    if (false == m_PIDInitFlag)
    {
        m_PIDInitFlag = true;

        m_cPIDShutter.Proportion = m_fltShutterProportion;
        m_cPIDShutter.Integral = m_fltShutterIntegral;
        m_cPIDShutter.Derivative = m_fltShutterDerivative;

        m_cPIDGain.Proportion = m_fltGainProportion;
        m_cPIDGain.Integral = m_fltGainIntegral;
        m_cPIDGain.Derivative = m_fltGainDerivative;
    }

    // ������ֵ����PID
    m_cPIDShutter.Command = iAGCTh;
    m_cPIDGain.Command = iAGCTh;

    if (iYArg < iAGCTh)  //�����ǰͼ��
    {
        if (iShutter < iAGCShutterMid)
        {
            INT nmmm = (INT)PIDCale(&m_cPIDShutter, iYArg);

            iShutter += nmmm;//(INT)PIDCale(&m_cPIDShutter, iYArg);

            iShutter = MIN(iAGCShutterMid, iShutter);

            iShutter = MAX(iAGCShutterL, iShutter);

            if (iGain > iAGCGainL)
            {
                iGain -= ((iGain - iAGCGainL)>>4);
            }
        }
        else if (iGain < iAGCGainH)
        {
            iGain += (INT)PIDCale(&m_cPIDGain, iYArg);
            iGain = MIN(iAGCGainH, iGain);
            iGain = MAX(iAGCGainL, iGain);
        }
        else if (iShutter < iAGCShutterH)
        {
            iShutter += (INT)PIDCale(&m_cPIDShutter, iYArg);
            iShutter = MIN(iAGCShutterH, iShutter);
            iShutter = MAX(iAGCShutterL, iShutter);
        }
        else
        {
            return 0;  // ������AGC���ڣ���Ϊ�Ѿ����ڵ�������
        }
    }
    else if (iYArg > iAGCTh)  //�����ǰͼ����
    {
        if (iShutter > iAGCShutterMid)
        {
            iShutter += (INT)PIDCale(&m_cPIDShutter, iYArg);
            iShutter = MIN(iAGCShutterH, iShutter);
            iShutter = MAX(iAGCShutterMid, iShutter);
        }
        else if (iGain > iAGCGainL)
        {
            iGain += (INT)PIDCale(&m_cPIDGain, iYArg);
            iGain = MIN(iAGCGainH, iGain);
            iGain = MAX(iAGCGainL, iGain);

            if (iShutter < iAGCShutterH)
            {
                iShutter += ((iAGCShutterH - iShutter)>>4);
            }
        }
        else if (iShutter > iAGCShutterL)
        {
            iShutter += (INT)PIDCale(&m_cPIDShutter, iYArg);
            iShutter = MIN(iAGCShutterMid, iShutter);
            iShutter = MAX(iAGCShutterL, iShutter);
        }
        else
        {
            return 0;  // ������AGC���ڣ���Ϊ�Ѿ����ڵ����
        }
    }

    return 1;  // ����AGC����
}

/**
* @brief ����DC��Ȧ���Զ�AGC
* @param [in] iYArg ͼ��ƽ������
* @param [in] iAGCTh ͼ������ƽ������
* @param [in] iAGCShutterL ��Ϳ���
* @param [in] iAGCShutterH ������
* @param [in] iAGCGainL �������
* @param [in] iAGCGainH �������
* @param [out] iShutter ��ǰ����
* @param [out] iGain ��ǰ����
* @retval true �ɹ�
* @retval 0 ʧ��
*/
bool CSWAgcAwbTask::AdjustAperture(
    int iYArg,    //��ǰƽ�����ȣ�0~255��
    int iAGCTh,   //����ƽ�����ȣ�0~255��
    int iAGCShutterL,
    int iAGCShutterH,
    int iAGCGainL,
    int iAGCGainH,
    int &iShutter,
    int &iGain )
{
    int iDiff = iYArg - CSWAgcAwbTask::fiLasttimeYValue;
    if( iYArg < 0 ){ return false;}

    if( iYArg < iAGCTh ){
        if( iDiff > YMAX_DIFF){
             // ���ô�״̬
             swpa_camera_io_control_dc_iris( 1 );
             CSWAgcAwbTask::fiLasttimeYValue = iYArg; return false;
        }
        else if( iYArg < iAGCTh )
        {
            if( iShutter != m_iAGCShutterHOri )
                return CalcShutterGain(
                                iYArg,
                                iAGCTh,  // YԤ��ֵ��������������
                                m_iAGCShutterLOri, m_iAGCShutterHOri,
                                m_iAGCGainLOri, m_iAGCGainHOri,
                                iShutter,
                                iGain);
            else{
                // ����ֹͣ״̬
                swpa_camera_io_control_dc_iris( 0 );
            }
        }
    }
    else{
        if( iGain > GAIN_BASELINE ){

            iShutter = iShutter < m_iAGCShutterHOri ? m_iAGCShutterHOri : iShutter;
             // ����ֹͣ״̬
            swpa_camera_io_control_dc_iris( 0 );
            // �ı�����
            UINT uStep = ((iGain - m_iAGCGainLOri)>>4);
            uStep = uStep == 0 ? 1 : uStep;
            iGain -= uStep; return true;
        }
        else if( (iYArg - iAGCTh) > YMAX_DIFF )
        {
            // ���ùر�״̬
            swpa_camera_io_control_dc_iris( 2 );

            // �򿪹�Ȧ��ʱ��
            UINT uSleepTime = 0 ; int nDiffYCTH = (iYArg - iAGCTh);
            if( nDiffYCTH > 100 ) uSleepTime = 1000*120;
            else if( nDiffYCTH > 50 ) uSleepTime = 1000*100;
            else if( nDiffYCTH > 35 ) uSleepTime = 1000*80;
            else if( nDiffYCTH > 25 ) uSleepTime = 1000*60;
            else if( nDiffYCTH > 15 ) uSleepTime = 1000*50;
            else if( nDiffYCTH > 10 ) uSleepTime = 1000*40;
            else if( nDiffYCTH > 5 ) uSleepTime = 1000*30;
            else uSleepTime = 1000*10;

            // ���ù�Ȧ��ͣ����ʱ��
            swpa_thread_sleep_ms( uSleepTime / 1000 );

            SW_TRACE_DEBUG( "[AdjustAperture====== %d , %d ; %d ; %d, %d ; %d]\n"
            , iYArg , iDiff , iShutter , iGain , m_iAGCShutterHOri , uSleepTime / 1000);

            // ����ֹͣ״̬
            swpa_camera_io_control_dc_iris( 0 );
        }
    }

    return false;
}

