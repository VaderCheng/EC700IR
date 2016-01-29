#include "SWCaptureTask.h"
#include "swpa_camera.h"
#include "SWLog.h"
#include "SWImageStatisticsInfo.h"

CSWCaptureTask::CSWCaptureTask()
: m_iCaptureShutter( 0 )
, m_iCaptureGain( 0 )
, m_iNowGainR( 0 )
, m_iNowGainG( 0 )
, m_iNowGainB( 0 )

{
    //ctor
}

CSWCaptureTask::~CSWCaptureTask()
{
    //dtor
}

// ��������ʹ��
INT CSWCaptureTask::SetRGBEnable( BOOL isEnable )
{
    if( !isEnable )
        return swpa_camera_basicparam_set_capture_rgb_gain( -1 , -1 , -1);
    else
        return swpa_camera_basicparam_set_capture_rgb_gain( m_iNowGainR , m_iNowGainG , m_iNowGainB);
}
// ���ÿ���ʹ��
INT CSWCaptureTask::SetShutterEnable( BOOL isEnable )
{
    return swpa_camera_basicparam_set_capture_shutter( (isEnable ? m_iCaptureShutter : -1));
}
// �����Ƿ�ʹ��
INT CSWCaptureTask::SetGainEnable( BOOL isEnable )
{
    return swpa_camera_basicparam_set_capture_gain( (isEnable ? m_iCaptureGain: -1));
}
// ���õ�ǰ����
VOID CSWCaptureTask::SetShutter(INT iShutter)
{
    swpa_camera_basicparam_set_capture_shutter( m_iCaptureShutter = iShutter );
}
// ��ȡ��ǰ����
VOID CSWCaptureTask::GetShutter(INT& iShutter)
{
    iShutter = m_iCaptureShutter;
}
// ���õ�ǰ����
VOID CSWCaptureTask::SetGain(INT iGain)
{
    swpa_camera_basicparam_set_capture_gain( (m_iCaptureGain = iGain) );
}
// ��ȡ��ǰ����
VOID CSWCaptureTask::GetGain( INT& iGain)
{
    iGain = m_iCaptureGain;
}
// ���õ�ǰ����ֵ����ʽΪ0x00RGB,RΪRͨ�����棬GΪGͨ������ֵ �� BΪBͨ������ֵ
VOID CSWCaptureTask::SetRGBGain( DWORD dwRGBGain )
{
	if( dwRGBGain == 0xFF000000 )
	{
		m_iNowGainR = -1;
		m_iNowGainG = -1;
		m_iNowGainB = -1;
	}
	else
	{
		m_iNowGainR = ((dwRGBGain & 0x00FF0000) >> 16);
		m_iNowGainG = ((dwRGBGain & 0x0000FF00) >> 8);
		m_iNowGainB = (dwRGBGain & 0x000000FF);
	}

    swpa_camera_basicparam_set_capture_rgb_gain( m_iNowGainR , m_iNowGainG , m_iNowGainB);
}
 // ��ȡ��ǰ����ֵ����ʽΪ0x00RGB,RΪRͨ�����棬GΪGͨ������ֵ �� BΪBͨ������ֵ
VOID CSWCaptureTask::GetRGBGain( DWORD &dwRGBGain )
{
    dwRGBGain = m_iNowGainB;
    dwRGBGain |= (m_iNowGainG << 8);
    dwRGBGain |= (m_iNowGainR << 16);
}
SInt64 CSWCaptureTask::Run()
{
 	EventFlags events = this->GetEvents( );
	if( events & CSWTask::kIdleEvent ) return 0;

	while( !(events & CSWTask::kKillEvent) )
	{
        FLOAT fDifferenceFactor ; BOOL IsUpdate = false;
        CSWImageStatisticsInfo::GetCaptureDiffStatistValue( fDifferenceFactor , IsUpdate);
        // ����ץ�Ĳ�����
        if( IsUpdate ) this->SetCaptureYinyangSideFactor( fDifferenceFactor );

        return 200;
	}

    return -1;
}
void CSWCaptureTask::SetCaptureYinyangSideFactor( float fCaptureFactor )
{
#define MINBOUNDER_VALUE 0.005
static int fnAChannelCurrCaptureGain = 0 ;
static int fnBChannelCurrCaptureGain = 0 ;
static int fCurrCaptureGainEnable = 0;
static int fnOldGain = 0;

    int nCurrentCaptureGain = 0; this->GetGain( nCurrentCaptureGain );

    // �������ı������µ���
    if( fnOldGain != nCurrentCaptureGain ){
        fnOldGain = nCurrentCaptureGain;
        fnAChannelCurrCaptureGain = fnBChannelCurrCaptureGain = 0;

        swpa_camera_init_set_capture_gain_correction( fnAChannelCurrCaptureGain
                                                  , fnBChannelCurrCaptureGain , 0 , 0);
    }

    /* dengtx(2013-01-08)
     ���ӻ�ȡͳ����Ϣ������ץ������������
    */
    float fDiffValue = 0.0f;

    fDiffValue = fabs(1.0 - fCaptureFactor);

    if( fDiffValue < MINBOUNDER_VALUE){
      //  SW_TRACE(1,"Current Capture yinyang side factor %f; the differe = %f\n",fCaptureFactor,fDiffValue);
        return ;
    }

        if( fCaptureFactor > 1.0 ){
        if( fnAChannelCurrCaptureGain > 0 ){
            fnAChannelCurrCaptureGain -= 1;
        }
        else{
            fnBChannelCurrCaptureGain += 1;
        }
    }
    else{

        if( fnBChannelCurrCaptureGain > 0 ){
            fnBChannelCurrCaptureGain -= 1;
        }
        else{
            fnAChannelCurrCaptureGain += 1;
        }
    }

    // ��������
    swpa_camera_init_set_capture_gain_correction( fnAChannelCurrCaptureGain
                                                  , fnBChannelCurrCaptureGain , 0 , 0);

    // ������������
    this->SetGain(m_iCaptureGain );

    SW_TRACE_DEBUG( "��Capture Gain�� A=%d;B=%d;fDiffValue=%f;fCaptureFactor=%f;nCurrentCaptureGain=%d;fnOldGain=%d\r\n"
           ,fnAChannelCurrCaptureGain,fnBChannelCurrCaptureGain,fDiffValue
           ,fCaptureFactor,nCurrentCaptureGain,fnOldGain);

}

