// ���ļ����������WINDOWS-936��ʽ
#include "CameraController.h"

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <stropts.h>
#include <errno.h>

#include "swdrv.h"
#include "GetImgByVPIF.h"

#ifdef _CAM_APP_
#include "../CamApp/DataCtrl.h"
#include "../CamApp/LoadParam.h"
#endif

CCameraController g_cCameraController;  // ȫ��Ψһ


CCameraController::CCameraController()
{
    memset(m_rgiNowGammaDataXY, 0, 64);
    m_pCameraUtils = NULL;
    m_pCharacterUtils = NULL;

}

CCameraController::~CCameraController()
{

}

#if !defined(SINGLE_BOARD_PLATFORM) && defined(_CAM_APP_)

// ��ADоƬ��FPGAоƬ�ļĴ���д��ֵ
int CCameraController::WriteAD(FPGA_STRUCT datas)
{
    return SwADWrite(datas);
}

void CCameraController::SetFpgaStructContext(
    FPGA_STRUCT& cFpgaStruct,
    DWORD32 dwAddr,
    DWORD32 dwData
)
{
    cFpgaStruct.regs[cFpgaStruct.count].addr  = dwAddr;
    cFpgaStruct.regs[cFpgaStruct.count].data  = dwData;
    cFpgaStruct.count++;
}

/*
 * ����˵����Ӧ200w��AD9949оƬ��
 * 0x01  VGA�������ã�10λ����λ����Ӧ 6 �� 42DB ��
 * 0xd4  ��ɫ������ڼĴ������;�λΪgain0���߾�λΪgain1
 * 0xd5  ��ɫ������ڼĴ������;�λΪgain2���߾�λΪgain3
 * 0x80  0x80Ϊ��չ���żĴ�����ַ�� 0��С��0xffffff���ÿ����1��Ӧʱ��ֵԼΪ53us��
 */
int CCameraController::SetRgbGain(int iGainR, int iGainG, int iGainB)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#ifdef _CAMERA_PIXEL_500W_
    iGainR &= 0x01ff;
    iGainG &= 0x01ff;
    iGainB &= 0x01ff;

    SetFpgaStructContext(datas, 0x90c, iGainR);
    SetFpgaStructContext(datas, 0x90d, iGainG);
    SetFpgaStructContext(datas, 0x90e, iGainG);
    SetFpgaStructContext(datas, 0x90f, iGainB);
    SetFpgaStructContext(datas, 0x91f, iGainR);
    SetFpgaStructContext(datas, 0x920, iGainG);
    SetFpgaStructContext(datas, 0x921, iGainG);
    SetFpgaStructContext(datas, 0x922, iGainB);
    SetFpgaStructContext(datas, 0x988, 0);
#elif defined(_CAMERA_PIXEL_200W_25FPS_)
    iGainR &= 0x01ff;
    iGainG &= 0x01ff;
    iGainB &= 0x01ff;

    SetFpgaStructContext(datas, 0x909, iGainR);
    SetFpgaStructContext(datas, 0x90a, iGainG);
    SetFpgaStructContext(datas, 0x90b, iGainG);
    SetFpgaStructContext(datas, 0x90c, iGainB);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    int data04 = 0;
    int data05 = 0;

    // 0x gain1 gain0
    // 0x gain3 gain2
    //-----------------
    // 0x iGainG iGainB
    // 0x iGainR iGainG

    iGainR &= 0x01ff;
    iGainG &= 0x01ff;
    iGainB &= 0x01ff;

    // for bt1120
    data04 |= iGainG;
    data04 <<= 9;
    data04 |= iGainB;

    data05 |= iGainR;
    data05 <<= 9;
    data05 |= iGainG;

    /*
    // for raw (bayer12)
    data04 |= iGainR;
    data04 <<= 9;
    data04 |= iGainG;

    data05 |= iGainG;
    data05 <<= 9;
    data05 |= iGainB;
    */

    SetFpgaStructContext(datas, 0xd4, data04);
    SetFpgaStructContext(datas, 0xd5, data05);
    SetFpgaStructContext(datas, 0x88, 0);
#endif // _CAMERA_PIXEL_500W_

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

int CCameraController::SetGain(int iGain)
{
    iGain = MAX((iGain&0x03ff), GAIN_BASELINE);

    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#ifdef _CAMERA_PIXEL_500W_
    SetFpgaStructContext(datas, 0x90a, 1);
    SetFpgaStructContext(datas, 0x988, 0);
    SetFpgaStructContext(datas, 0x005, iGain);
    SetFpgaStructContext(datas, 0x90a, 0);
    SetFpgaStructContext(datas, 0x988, 0);
    SetFpgaStructContext(datas, 0x005, iGain);
#elif defined(_CAMERA_PIXEL_200W_25FPS_)
    SetFpgaStructContext(datas, 0x005, iGain);
#else
    SetFpgaStructContext(datas, 0x01, iGain);
    SetFpgaStructContext(datas, 0x88, 0);
#endif

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

int CCameraController::SetShutter(int iShutter)
{
    iShutter &= 0x00ffffff;

    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#if defined(_CAMERA_PIXEL_500W_) || defined(_CAMERA_PIXEL_200W_25FPS_)
    SetFpgaStructContext(datas, 0x900, iShutter);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    SetFpgaStructContext(datas, 0x80, iShutter);
    SetFpgaStructContext(datas, 0x88, 0);
#endif

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

int CCameraController::SetCaptureRgbGain(int iGainR, int iGainG, int iGainB, int fEnable)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#ifdef _CAMERA_PIXEL_500W_
    int data0 = (iGainR&0x01ff);
    int data1 = (iGainG&0x01ff);
    int data2 = (iGainG&0x01ff);
    int data3 = (iGainB&0x01ff);

    if ( fEnable != 0 )
    {
        // ����28λ��Ϊ1
        data0 |= 0x08000000;
        data1 |= 0x08000000;
        data2 |= 0x08000000;
        data3 |= 0x08000000;
    }

    SetFpgaStructContext(datas, 0x911, data0);
    SetFpgaStructContext(datas, 0x912, data1);
    SetFpgaStructContext(datas, 0x913, data2);
    SetFpgaStructContext(datas, 0x914, data3);
    SetFpgaStructContext(datas, 0x924, data0);
    SetFpgaStructContext(datas, 0x925, data1);
    SetFpgaStructContext(datas, 0x926, data2);
    SetFpgaStructContext(datas, 0x927, data3);
    SetFpgaStructContext(datas, 0x988, 0);
#elif defined(_CAMERA_PIXEL_200W_25FPS_)
    int data0 = (iGainR&0x01ff);
    int data1 = (iGainG&0x01ff);
    int data2 = (iGainG&0x01ff);
    int data3 = (iGainB&0x01ff);

    if ( fEnable != 0 )
    {
        // ����28λ��Ϊ1
        data0 |= 0x08000000;
        data1 |= 0x08000000;
        data2 |= 0x08000000;
        data3 |= 0x08000000;
    }

    SetFpgaStructContext(datas, 0x90e, data0);
    SetFpgaStructContext(datas, 0x90f, data1);
    SetFpgaStructContext(datas, 0x910, data2);
    SetFpgaStructContext(datas, 0x911, data3);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    int data73 = 0;
    int data74 = 0;

    data73 |= (iGainG&0x01ff);
    data73 <<= 9;
    data73 |= (iGainB&0x01ff);

    data74 |= (iGainR&0x01ff);
    data74 <<= 9;
    data74 |= (iGainG&0x01ff);

    if ( fEnable != 0 )
    {
        // ����24λ��Ϊ1
        data73 |= 0x00800000;
        data74 |= 0x00800000;
    }

    SetFpgaStructContext(datas, 0x73, data73);
    SetFpgaStructContext(datas, 0x74, data74);
    SetFpgaStructContext(datas, 0x88, 0);
#endif // _CAMERA_PIXEL_500W_

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

int CCameraController::SetCaptureGain(int iGain, int fEnable)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    iGain = MAX((iGain&0x03ff), GAIN_BASELINE); // �������棬��СΪ70

#ifdef _CAMERA_PIXEL_500W_
    iGain &= 0x03ff;
    if ( fEnable != 0 )
    {
        // ����28λ��Ϊ1
        iGain |= 0x08000000;
    }

    SetFpgaStructContext(datas, 0x910, iGain);
    SetFpgaStructContext(datas, 0x923, iGain);
    SetFpgaStructContext(datas, 0x988, 0);
#elif defined(_CAMERA_PIXEL_200W_25FPS_)
    iGain &= 0x03ff;
    if ( fEnable != 0 )
    {
        // ����28λ��Ϊ1
        iGain |= 0x08000000;
    }

    SetFpgaStructContext(datas, 0x90d, iGain);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    iGain &= 0x03ff;
    if ( fEnable != 0 )
    {
        // ����24λ��Ϊ1
        iGain |= 0x00800000;
    }

    SetFpgaStructContext(datas, 0x72, iGain);
    SetFpgaStructContext(datas, 0x88, 0);
#endif // _CAMERA_PIXEL_500W_

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

int CCameraController::SetCaptureShutter(int iShutter, int fEnable)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#if defined(_CAMERA_PIXEL_500W_) || defined(_CAMERA_PIXEL_200W_25FPS_)
    iShutter &= 0x00ffffff;
    if ( fEnable != 0 )
    {
        // ����28λ��Ϊ1
        iShutter |= 0x08000000;
    }

    SetFpgaStructContext(datas, 0x901, iShutter);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    iShutter &= 0x00ffffff;
    if ( fEnable != 0 )
    {
        // ����24λ��Ϊ1
        iShutter |= 0x00800000;
    }

    SetFpgaStructContext(datas, 0x81, iShutter);
    SetFpgaStructContext(datas, 0x88, 0);
#endif // _CAMERA_PIXEL_500W_

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}
int CCameraController::SetControlBoardIOOpt(int fEnable)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    SetFpgaStructContext(datas, 0xd1, fEnable);
#ifdef _CAMERA_PIXEL_500W_
    SetFpgaStructContext(datas, 0x988, 0);
#else
    SetFpgaStructContext(datas, 0x88, 0);
#endif
    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}
// 0: �رյ���ͬ�� 15fps
// 1: ��������ͬ�� 12.5fps
int CCameraController::SetFpsMode(int iMode, BOOL fResetH264/* = FALSE */)
{
#ifdef _CAMERA_PIXEL_200W_25FPS_
    return -1;
#endif

    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#ifdef _CAMERA_PIXEL_500W_
    SetFpgaStructContext(datas, 0x906, iMode&0x01);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    SetFpgaStructContext(datas, 0x86, iMode&0x01);
    SetFpgaStructContext(datas, 0x88, 0);
#endif // _CAMERA_PIXEL_500W_

    if ( 0 == WriteAD(datas) )
    {
        g_cModuleParams.cCamAppParam.iENetSyn = iMode&0x01;
#ifdef _CAM_APP_
        if ( TRUE == fResetH264 && g_pCamApp != NULL)
        {
            g_pCamApp->ResetH264Enc(iMode&0x01);
        }
#endif
        return 0;
    }
    return -1;
}

int CCameraController::GetFpsMode(int& iMode)
{
    iMode = g_cModuleParams.cCamAppParam.iENetSyn;
    return 0;
}

int CCameraController::SynSignalEnable(int iFlashRateEnable, int iCaptureEnable)
{
    int iValue = 0;
    int iFlashValue = 0;
    int iCaptureValue = 0;

    if ( 1 == iFlashRateEnable )
    {
        iFlashValue |= 0x01;
    }
    else
    {
        iFlashValue &= (~0x01);
    }

    if ( 1 == iCaptureEnable )
    {
        iCaptureValue |= 0x02;
    }
    else
    {
        iCaptureValue &= (~0x02);
    }

    iValue = (iFlashValue | iCaptureValue);

    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#if defined(_CAMERA_PIXEL_500W_) || defined(_CAMERA_PIXEL_200W_25FPS_)
    SetFpgaStructContext(datas, 0x902, iValue);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    SetFpgaStructContext(datas, 0x82, iValue);
    SetFpgaStructContext(datas, 0x88, 0);
#endif

    if ( 0 == WriteAD(datas) )
    {
        g_cModuleParams.cCamAppParam.iFlashRateSynSignalEnable = iFlashRateEnable;
        g_cModuleParams.cCamAppParam.iCaptureSynSignalEnable = iCaptureEnable;
        return 0;
    }
    return -1;
}

// 0: �ⲿ�½��ش���
// 1: �ⲿ�����ش���
// 2: �����ء��½��ض�����
// 3��������
int CCameraController::CaptureEdgeSelect(int iMode)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#ifdef _CAMERA_PIXEL_500W_
    SetFpgaStructContext(datas, 0x909, iMode&0x03);
    SetFpgaStructContext(datas, 0x988, 0);
#elif defined(_CAMERA_PIXEL_200W_25FPS_)
    SetFpgaStructContext(datas, 0x907, iMode&0x03);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    SetFpgaStructContext(datas, 0x71, iMode&0x03);
    SetFpgaStructContext(datas, 0x88, 0);
#endif

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

int CCameraController::SoftTriggerCapture()
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#ifdef _CAMERA_PIXEL_500W_
    SetFpgaStructContext(datas, 0x905, 1);
    SetFpgaStructContext(datas, 0x988, 0);
    SetFpgaStructContext(datas, 0x905, 0);
    SetFpgaStructContext(datas, 0x988, 0);
#elif defined(_CAMERA_PIXEL_200W_25FPS_)
    SetFpgaStructContext(datas, 0x904, 1);
    SetFpgaStructContext(datas, 0x988, 0);
    SetFpgaStructContext(datas, 0x904, 0);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    SetFpgaStructContext(datas, 0x85, 1);
    SetFpgaStructContext(datas, 0x88, 0);
    SetFpgaStructContext(datas, 0x85, 0);
    SetFpgaStructContext(datas, 0x88, 0);
#endif

    if ( 0 == WriteAD(datas) )
    {
        HV_Trace(5, "SoftTriggerCapture is succeed.\n");
        return 0;
    }
    else
    {
        HV_Trace(5, "SoftTriggerCapture is failed!\n");
        return -1;
    }
}

/**
 * gamma�����·�ʽ����CPU����gamma��Yֵ���ٴ��ݵ�FPGA��
 * ����ӿڱ�����ԭ��һ�¡�
 *
 */
int CCameraController::SetGammaData(int rgiDataXY[8][2])
{
//    if (!memcmp(m_rgiNowGammaDataXY, rgiDataXY, 64))
//    {
//        return 0;
//    }
//
//    FPGA_STRUCT datas;
//    bzero(&datas, sizeof(datas));
//
//#ifdef _CAMERA_PIXEL_500W_
//    DWORD32 dwDataTmp = 0;
//    datas.count = 8 + 8 + 1;
//    for ( int i = 0; i < 8; ++i )
//    {
//        datas.regs[i].addr  = 0x915 + i;
//        datas.regs[i+8].addr  = 0x928 + i;
//        dwDataTmp = ((rgiDataXY[i][0]&0xff)<<8) | (rgiDataXY[i][1]&0xff);
//        datas.regs[i].data  = dwDataTmp;
//        datas.regs[i+8].data = dwDataTmp;
//    }
//    datas.regs[16].addr = 0x988;
//    datas.regs[16].data = 0;
//#else
//    datas.count = 9;
//    for ( int i = 0; i < 8; ++i )
//    {
//#ifdef _CAMERA_PIXEL_200W_25FPS_
//        datas.regs[i].addr  = 0x912 + i;
//#else
//    datas.regs[i].addr  = 0x75 + i;
//#endif
//        datas.regs[i].data  = ((rgiDataXY[i][0]&0xff)<<8) | (rgiDataXY[i][1]&0xff);
//    }
//    datas.regs[8].addr = 0x88;
//    datas.regs[8].data = 0x00;
//#endif
//
//    if ( 0 == WriteAD(datas) )
//    {
//        SaveGamma(rgiDataXY);
//        memcpy(m_rgiNowGammaDataXY, rgiDataXY, 64);
//        HV_Trace(5, "SetGammaData is succeed.\n");
//        return 0;
//    }
//    else
//    {
//        HV_Trace(5, "SetGammaData is failed!\n");
//        return -1;
//    }

    if (!memcmp(m_rgiNowGammaDataXY, rgiDataXY, 64))
    {
        return 0;
    }

    static int gamma_y[4096] = {0};
    int x = 0, y = 0;
    int x1 = 0, y1 = 0;
    int x2 = 0, y2 = 0;
    int ret = -1;
    int tmp = 0;
    FPGA_STRUCT datas;

    bzero(&datas, sizeof(datas));

    /// 8����
    for (int i = 0; i < 8; i++)
    {
        /// ��һ����
        if (i == 0)     /// ����ԭ��(0, 0)
        {
            x1 = 0;
            y1 = 0;
        }
        else
        {
            x1 = rgiDataXY[i - 1][0] * 4095 / 255;
            y1 = rgiDataXY[i - 1][1] * 4095 / 255;
        }
        /// �ڶ�����
        x2 = rgiDataXY[i][0] * 4095 / 255;
        y2 = rgiDataXY[i][1] * 4095 / 255;

        /// ���������(����������)���е�Yֵ
        for (x = x1; x <= x2; x++)
        {
            y = (y2 - y1) * (x - x1) / (x2 - x1) + y1;
            gamma_y[x] = y;
        }
    }

    /// ע����1��ֵ�����1��ֵ����д��FPGA��
    for (int j = 1; j < 4095; j += 2)
    {
        tmp = (gamma_y[j+1]<<12) | gamma_y[j];

        SetFpgaStructContext(datas, 0x75, tmp);
        ret = WriteAD(datas);
        if (ret != 0)
        {
            break;
        }
        datas.count = 0;
    }

    // д��gammaֵ������0x76д0��ʾ����
    if (0 == ret)
    {
        SetFpgaStructContext(datas, 0x76, 0);
        ret = WriteAD(datas);
    }
    if ( 0 ==  ret )
    {
        SaveGamma(rgiDataXY);
        memcpy(m_rgiNowGammaDataXY, rgiDataXY, 64);
        HV_Trace(5, "SetGammaData is succeed.\n");
        return 0;
    }
    else
    {
        HV_Trace(5, "SetGammaData is failed!\n");
        return -1;
    }

    return 0;
}

int CCameraController::FpgaTimeRegClr()
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#ifdef _CAMERA_PIXEL_500W_
    SetFpgaStructContext(datas, 0x90b, 0);
    SetFpgaStructContext(datas, 0x988, 0);
    SetFpgaStructContext(datas, 0x90b, 1);
    SetFpgaStructContext(datas, 0x988, 0);
#elif defined(_CAMERA_PIXEL_200W_25FPS_)
    SetFpgaStructContext(datas, 0x908, 0);
    SetFpgaStructContext(datas, 0x988, 0);
    SetFpgaStructContext(datas, 0x908, 1);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    SetFpgaStructContext(datas, 0x7d, 0);
    SetFpgaStructContext(datas, 0x88, 0);
    SetFpgaStructContext(datas, 0x7d, 1);
    SetFpgaStructContext(datas, 0x88, 0);
#endif

    if ( 0 == WriteAD(datas) )
    {
        HV_Trace(5, "FpgaTimeRegClr is succeed.\n");
        return 0;
    }
    else
    {
        HV_Trace(5, "FpgaTimeRegClr is failed!\n");
        return -1;
    }
}

int CCameraController::SetRegDirect(DWORD32 addr, DWORD32 data)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    SetFpgaStructContext(datas, addr, data);

#if defined(_CAMERA_PIXEL_500W_) || defined(_CAMERA_PIXEL_200W_25FPS_)
    if ( 0x88 == addr )
    {
        SetFpgaStructContext(datas, 0x988, data);
    }
#endif

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

int CCameraController::SetTrafficLightEnhanceParam(
    int iHTh, int iLTh, int iSTh,
    int iColorFactor
)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#ifdef _CAMERA_PIXEL_500W_
    SetFpgaStructContext(datas, 0x934, iHTh&0x3ffff);
    SetFpgaStructContext(datas, 0x935, iLTh&0x3fff);
    SetFpgaStructContext(datas, 0x936, iSTh&0x3fff);
    SetFpgaStructContext(datas, 0x937, iColorFactor&0x3ffff);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    SetFpgaStructContext(datas, 0x90, iHTh&0x3ffff);
    SetFpgaStructContext(datas, 0x91, iLTh&0x3fff);
    SetFpgaStructContext(datas, 0x92, iSTh&0x3fff);
    SetFpgaStructContext(datas, 0x93, iColorFactor&0x3ffff);
    SetFpgaStructContext(datas, 0x88, 0);
#endif
    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

int CCameraController::SetTrafficLightEnhanceLumParam(int iLTh, int iLFactor, int iHFactor)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#ifdef _CAMERA_PIXEL_500W_
    SetFpgaStructContext(datas, 0x947, iLTh&0x3ffff);
    SetFpgaStructContext(datas, 0x948, iLFactor&0x3fff);
    SetFpgaStructContext(datas, 0x949, iHFactor&0x3fff);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    SetFpgaStructContext(datas, 0xa8, iLTh&0x3ffff);
    SetFpgaStructContext(datas, 0xa9, iLFactor&0x3fff);
    SetFpgaStructContext(datas, 0xaa, iHFactor&0x3fff);
    SetFpgaStructContext(datas, 0x88, 0);
#endif
    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

int CCameraController::SetTrafficLightEnhanceZone(
    int iId,
    int iX1, int iY1,
    int iX2, int iY2
)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

#ifdef _CAMERA_PIXEL_500W_
    SetFpgaStructContext(datas, 0x938, iId&0x1f);
    SetFpgaStructContext(datas, 0x939, iX1&0xfff);
    SetFpgaStructContext(datas, 0x93b, iY1&0xfff);
    SetFpgaStructContext(datas, 0x93a, iX2&0xfff);
    SetFpgaStructContext(datas, 0x93c, iY2&0xfff);
    SetFpgaStructContext(datas, 0x988, 0);
#else
    SetFpgaStructContext(datas, 0x94, iId&0x1f);
    SetFpgaStructContext(datas, 0x96, iX1&0xfff);
    SetFpgaStructContext(datas, 0x98, iY1&0xfff);
    SetFpgaStructContext(datas, 0x97, iX2&0xfff);
    SetFpgaStructContext(datas, 0x99, iY2&0xfff);
    SetFpgaStructContext(datas, 0x88, 0);
#endif
    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}
int CCameraController::SetADSamplingValue(
    int iValueA,
    int iValueB
)
{
#ifdef _CAMERA_PIXEL_500W_
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    // ��ЧλΪ28λ��ͬʱBͨ�������λҪ��1
    iValueA &= 0x0fffffff;
    iValueB &= 0x0fffffff;
    iValueB |= 0x08000000;

    SetFpgaStructContext(datas, 0x90a, 0);  // select channel A
    SetFpgaStructContext(datas, 0x988, 0);  // flush
    SetFpgaStructContext(datas, 0x36, iValueA);   // write valueA
    SetFpgaStructContext(datas, 0x90a, 1);  // select channel B
    SetFpgaStructContext(datas, 0x988, 0);  // flush
    SetFpgaStructContext(datas, 0x36, iValueB);   // write valueB

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
#else
    return -1;
#endif
}

// �ֳ���ץ�Ĺ���
// iPsdVersion���¾ɿ��ư壬0����  1����
int CCameraController::SublaneCapture(int iPsdVersion)
{
#ifdef _CAMERA_PIXEL_200W_25FPS_
    return -1;
#endif

    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    // �ɿ��ư壺PSD04C
    if (iPsdVersion == 0)
    {
        HV_Trace(5, "SublaneCapture PSD04C\n");
#ifdef _CAMERA_PIXEL_500W_
        SetFpgaStructContext(datas, 0x93d, 0);
        SetFpgaStructContext(datas, 0x988, 0);
#else
        SetFpgaStructContext(datas, 0x7f, 0);
        SetFpgaStructContext(datas, 0x88, 0);
#endif
    }
    // �¿��ư壺PSD05C
    else if (iPsdVersion == 1)
    {
        HV_Trace(5, "SublaneCapture PSD05C\n");
#ifdef _CAMERA_PIXEL_500W_
        SetFpgaStructContext(datas, 0x93d, 1);
        SetFpgaStructContext(datas, 0x988, 0);
#else
        SetFpgaStructContext(datas, 0x7f, 1);
        SetFpgaStructContext(datas, 0x88, 0);
#endif
    }
    // ����ֵ������
    else
    {
        return -1;
    }

    if ( 0 == WriteAD(datas) )
    {
        HV_Trace(5, "SublaneCapture is succeed.\n");
        return 0;
    }
    return -1;
}

// ����Ʒֳ�����
// 1���ֳ�����
// 0�����ֳ�����
int CCameraController::FlashDifferentLane(int fEnalbe)
{
#ifdef _CAMERA_PIXEL_500W_
    return -1;
#else
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    SetFpgaStructContext(datas, 0xd0, fEnalbe&0x01);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
#endif
}

// ����Ʒֳ�����(Ӳ����)
// 1���ֳ�����
// 0�����ֳ�����
int CCameraController::FlashDifferentLaneExt(int fEnalbe)
{
#ifdef _CAMERA_PIXEL_500W_
    return -1;
#else
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    SetFpgaStructContext(datas, 0xd1, fEnalbe&0x01);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
#endif
}

// ��λFPGA
int CCameraController::ResetFpga()
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    SetFpgaStructContext(datas, 0xdd, 0);
    SetFpgaStructContext(datas, 0xdd, 0xa5a5a5);
    if ( 0 == WriteAD(datas) )
    {
        HV_Trace(5, "Reset Fpga OK!!\n");
        return 0;
    }
    return -1;
}

// ����С����
// iShutter��Χ��0~0x5a0
// ��Ӧ�ò���˵����Χ��0~40us����ת��
int CCameraController::SetSmallShutterRaw(int iShutter)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));
    SetFpgaStructContext(datas, 0x80, 0);   // ��ԭ������Ϊ0
    SetFpgaStructContext(datas, 0xd2, iShutter & 0xfff);
    SetFpgaStructContext(datas, 0x88, 0);
    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

int CCameraController::SetOrgRgbGainToZero()
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    SetFpgaStructContext(datas, 0x04, 0);
    SetFpgaStructContext(datas, 0x05, 0);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

//-------------------------------------------A67 ��ʼ
///////////////////////////////2012-10-31
/**
 * ʹ����
 *
 * @param  fEnable 0����ʹ��   1��ʹ��
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 */
int CCameraController::EnableSharpen(int fEnable)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    SetFpgaStructContext(datas, 0xab, fEnable&0x01);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

/**
 * �����񻯷�ֵ
 *
 * @param  iThreshold ��ֵ ��Χ��[0~100]����7λ��Чλ
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note Ĭ��Ϊ7����ֵԽС��Ч��Խ��ͼƬԽ������֮��ֵԽ����Ч��ԽС��ͼ��Խ��
 */
int CCameraController::SetSharpenThreshold(int iThreshold)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));
    if (iThreshold < 0)
        iThreshold = 0;
    if (iThreshold > 100)
        iThreshold = 100;

    SetFpgaStructContext(datas, 0xac, iThreshold&0x7f);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

/**
 * ����������ͼ����ץ��ͼ
 *
 * @param  fEnable 1����ץ��ͼ 0��������ͼ��
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note ��Ҫ�˹�����Ч������ʹ����
 */
int CCameraController::SetSharpenCapture(int fEnable)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    SetFpgaStructContext(datas, 0xad, fEnable&0x01);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

/**
 * ���ñ��Ͷ�
 *
 * @param nValue ���Ͷ�ֵ����Χ��[-100~100]
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note Ĭ��ֵ��0(��ֵ���Ӧ�ò㣬����õ���r/g/b������FPGA�Ĵ�����Ĭ��ֵ)
 */
int CCameraController::SetSaturation(int nValue)
{
    if(nValue < -100 || nValue > 100)
        return -1;

    float x = 1+((nValue > 0) ? (3*nValue/100.0) : (nValue/100.0));
    const float lumR = 0.3086;
    const float lumG = 0.6094;
    const float lumB = 0.0820;

    // ���Ͷ�r/g/b�������ֵ����Сֵ
    int Rmax = (int)((lumR*(1-x)+x) * 4096);
    int Rmin = (int)(lumR*(1-x) * 4096);
    int Gmax = (int)((lumG*(1-x)+x) * 4096);
    int Gmin = (int)((lumG*(1-x) * 4096));
    int Bmax = (int)((lumB*(1-x)+x) * 4096);
    int Bmin = (int)(lumB*(1-x) * 4096);

    /// ע���������з�����������ʹ��"��λ��"�ķ�ʽ�жϷ�Χ
    // 15λ��Чλ
    Rmax = MAX(-16184, Rmax);
    Rmax = MIN(Rmax, 16183);
    Rmin = MAX(-16184, Rmin);
    Rmin = MIN(Rmin, 16183);

    Gmax = MAX(-16184, Gmax);
    Gmax = MIN(Gmax, 16183);
    Gmin = MAX(-16184, Gmin);
    Gmin = MIN(Gmin, 16183);

    Bmax = MAX(-16184, Bmax);
    Bmax = MIN(Bmax, 16183);
    Bmin = MAX(-16184, Bmin);
    Bmin = MIN(Bmin, 16183);

    HV_Trace(5, "debug--: SetSaturation R:[%d %d] G: [%d %d] B[%d %d]\n",
                Rmax, Rmin,
                Gmax, Gmin,
                Bmax, Bmin);

    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));
    SetFpgaStructContext(datas, 0xb0, Rmax);
    SetFpgaStructContext(datas, 0xb1, Rmin);
    SetFpgaStructContext(datas, 0xb2, Gmax);
    SetFpgaStructContext(datas, 0xb3, Gmin);
    SetFpgaStructContext(datas, 0xb4, Bmax);
    SetFpgaStructContext(datas, 0xb5, Bmin);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    HV_Trace(5, "SetSaturation is failed!\n");
    return -1;
}

// �Աȶȳ�������
// note: ����10.2�Ǽ���ȥ�ģ�ԭ��û��
static const double DELTA_INDEX[] =
{
    0, 0.01, 0.02, 0.04, 0.05, 0.06, 0.07, 0.08, 0.1,  0.11,
    0.12, 0.14, 0.15, 0.16, 0.17, 0.18, 0.20, 0.21, 0.22, 0.24,
    0.25, 0.27, 0.28, 0.30, 0.32, 0.34, 0.36, 0.38, 0.40, 0.42,
    0.44, 0.46, 0.48, 0.5,  0.53, 0.56, 0.59, 0.62, 0.65, 0.68,
    0.71, 0.74, 0.77, 0.80, 0.83, 0.86, 0.89, 0.92, 0.95, 0.98,
    1.0,  1.06, 1.12, 1.18, 1.24, 1.30, 1.36, 1.42, 1.48, 1.54,
    1.60, 1.66, 1.72, 1.78, 1.84, 1.90, 1.96, 2.0,  2.12, 2.25,
    2.37, 2.50, 2.62, 2.75, 2.87, 3.0,  3.2,  3.4,  3.6,  3.8,
    4.0,  4.3,  4.7,  4.9,  5.0,  5.5,  6.0,  6.5,  6.8,  7.0,
    7.3,  7.5,  7.8,  8.0,  8.4,  8.7,  9.0,  9.4,  9.6,  9.8,
    10.0, 10.2
};

/**
 * ���öԱȶ�
 *
 * @param nValue �Աȶ�ֵ����Χ��[-100~100]
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note Ĭ��ֵ��0(��ֵ���Ӧ�ò㣬����õ���������FPGA�Ĵ�����Ĭ��ֵ)
 */
int CCameraController::SetContrast(int nValue)
{
    if(nValue < -100 || nValue > 100)
        return -1;

    float x = 0;
    if (nValue < 0)
    {
        x = 127 + nValue/100.0*127;
    }
    else
    {
        x = nValue / 100.0;
        if (x == 0)
        {
            x = DELTA_INDEX[ nValue ];
        }
        else
        {
            x = DELTA_INDEX[ nValue ] * (1-x) + DELTA_INDEX[ nValue +1 ] * x; // use linear interpolation for more granularity.
        }
        x = x*127+127;
    }

    int cont_mf = (int)((x / 127) * 4096);      // �˷�����
    int cont_af = (int)(0.5*(127-x));          // �ӷ�����

    /// ע���������з�����������ʹ��"��λ��"�ķ�ʽ�жϷ�Χ
    // 17λ��Чλ
    cont_mf = MAX(-65536, cont_mf);
    cont_mf = MIN(cont_mf, 65535);
    // 11λ��Чλ
    cont_af = MAX(-1024, cont_af);
    cont_af = MIN(cont_af, 1023);

    HV_Trace(5, "debug--: SetContrast cont_mf: %d[0x%x] cont_af: %d[0x%x]\n", cont_mf, cont_mf, cont_af, cont_af);

    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));
    SetFpgaStructContext(datas, 0xb6, cont_mf);
    SetFpgaStructContext(datas, 0xb7, cont_af);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    HV_Trace(5, "SetContrast is failed!\n");
    return -1;
}

/**
 * ����ǰ����������
 *
 * @param iRawR  R����
 * @param iRawGr Gr����
 * @param iRawGb Gb����
 * @param iRawB  B����
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 */
int CCameraController::SetRawRGBGain(int iRawR, int iRawGr, int iRawGb, int iRawB)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    SetFpgaStructContext(datas, 0x8c, iRawR);
    SetFpgaStructContext(datas, 0x8d, iRawGr);
    SetFpgaStructContext(datas, 0x8e, iRawGb);
    SetFpgaStructContext(datas, 0x8f, iRawB);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }

    return -1;
}

//-------------------------------------------A67 ����

//-------------------------------------------A68 ��ʼ

/**
 * ����Gamma���ģʽ
 *
 * @param iMode  ģʽ����Χ��[0~3]
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note
    Ϊ0ʱ���������gamma�����ͼ�����ݵ���DSP/��DSP;
    Ϊ1ʱ�����û��gamma�����ͼ�����DSP/��DSP;
    Ϊ2ʱ���������gamma�����ͼ�����ݵ���DSP�����û��gamma�����ͼ�����DSP��
    Ϊ3ʱ���������gamma�����ͼ�����ݵ���DSP�����û��gamma�����ͼ�����DSP��

    FPGAĬ��ֵΪ3
 */
int CCameraController::SetGammaOutputMode(int iMode)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    SetFpgaStructContext(datas, 0xa2, iMode&0x3);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }

    return -1;
}

//-------------------------------------------A68 ����

//-------------------------------------------A70 ��ʼ
// 2013-03-22
/**
 * ���ñ��Ͷȿ���
 *
 * @param iMode  ģʽֵ
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note iMode���ҽ�������3��ֵ��
         0���رձ��Ͷȹ���
         1�����Ͷ�ֻ������ץ��֡
         2�����Ͷ�����������֡
         Ĭ��ֵ��0
 */
int CCameraController::SetSaturationMode(int iMode)
{
    int tmp = 0;
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    if (iMode == 0)
        tmp = 1;
    else if (iMode == 1)
        tmp = 2;
    else if (iMode == 2)
        tmp = 3;
    else
        tmp = 0;

    SetFpgaStructContext(datas, 0xb8, tmp);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }

    return -1;
}

/**
 * ���öԱȶȿ���
 *
 * @param iMode  ģʽֵ
 * @return �ɹ���0  ʧ�ܣ�-1
 *
 * @note iMode���ҽ�������3��ֵ��
         0���رնԱȶȹ���
         1���Աȶ�ֻ������ץ��֡
         2���Աȶ�����������֡
         Ĭ��ֵ��0
 */
int CCameraController::SetContrastMode(int iMode)
{
    int tmp = 0;
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    if (iMode == 0)
        tmp = 1;
    else if (iMode == 1)
        tmp = 2;
    else if (iMode == 2)
        tmp = 3;
    else
        tmp = 0;

    SetFpgaStructContext(datas, 0xb9, tmp);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }

    return -1;
}

/**
 * ����ȥ����ֵ
 *
 * @param iValue ��ֵ����Χ��0-16383
 *
 * @return �ɹ���0  ʧ��: -1
 * @note Ĭ��ֵ��8000
 *
 */
int CCameraController::SetDpcThreshold(int iValue)
{
    FPGA_STRUCT datas;
    bzero(&datas, sizeof(datas));

    SetFpgaStructContext(datas, 0xd8, iValue);
    SetFpgaStructContext(datas, 0x88, 0);

    if ( 0 == WriteAD(datas) )
    {
        return 0;
    }
    return -1;
}

//-------------------------------------------A70 ����

#endif // #if !defined(SINGLE_BOARD_PLATFORM) && defined(_CAM_APP_)

#if !defined(SINGLE_BOARD_PLATFORM) && !defined(_CAM_APP_)
int CCameraController::SlaveSoftTriggerCapture()
{
    int fd;
    int ret;

    fd = SwDevOpen(O_RDWR); // ��

    ret = ioctl(fd, SWDEV_IOCTL_GET_PHOTO_FROM_SLAVE, 0);
    if (ret < 0)
    {
        printf("===SWDEV_IOCTL_GET_PHOTO_FROM_SLAVE error!===\n");
    }
    else
    {
        HV_Trace(5, "SlaveSoftTriggerCapture is succeed.\n");
    }

    SwDevClose(fd);
    return ret;
}
#endif // #if !defined(SINGLE_BOARD_PLATFORM) && !defined(_CAM_APP_)

//---------------------------------------------------------------

// 500w: 0 ~ 2047 ����ӳ���� 0 ~ 65000
// 200w: 0 ~ 1019 ����ӳ���� 0 ~ 54000
int CCameraController::Shutter_Raw2Camyu(int iRawValue)
{
#ifdef _CAMERA_PIXEL_500W_
    return int(float(iRawValue * 65000) / 2047);
#else
    return int(float(iRawValue * 54000) / 1019);
#endif
}
int CCameraController::Shutter_Camyu2Raw(int iCamyuValue)
{
#ifdef _CAMERA_PIXEL_500W_
    return int(float(iCamyuValue * 2047) / 65000);
#else
    return int(float(iCamyuValue * 1019) / 54000);
#endif
}

// 500w: 0 ~ 1023 ����ӳ���� 0 ~ 370
// 200w: 0 ~ 1023 ����ӳ���� 0 ~ 360
int CCameraController::Gain_Raw2Camyu(int iRawValue)
{
#ifdef _CAMERA_PIXEL_500W_
    return int(float(iRawValue * 370) / 1023);
#else
    return int(float(iRawValue * 360) / 1023);
#endif
}
int CCameraController::Gain_Camyu2Raw(int iCamyuValue)
{
#ifdef _CAMERA_PIXEL_500W_
    return int(float(iCamyuValue * 1023) / 370);
#else
    return int(float(iCamyuValue * 1023) / 360);
#endif
}

// 0 ~ 511 ����ӳ���� 0 ~ 255
int CCameraController::ColorGain_Raw2Camyu(int iRawValue)
{
    return int(float(iRawValue) * 255 / 511);
}
int CCameraController::ColorGain_Camyu2Raw(int iCamyuValue)
{
    return int(float(iCamyuValue) * 511 / 255);
}

// С����ת��
// 0~1440(��FPGA) ����ӳ����0~40(��Ӧ�ò�)
int CCameraController::SmallShutter_Raw2Camyu(int iRawValue)
{
    return int(float(iRawValue) * 40 / 1440);
}

int CCameraController::SmallShutter_Camyu2Raw(int iCamyuValue)
{
    return int(float(iCamyuValue) * 1440 / 40);
}

#ifdef _CAM_APP_
int CCameraController::SetShutter_Camyu(int iShutter)
{
    int iRet = -1;
    if (iShutter < 41)  // 0~40us��ʹ��"С����"
    {
        iRet = SetSmallShutterRaw(SmallShutter_Camyu2Raw(iShutter));
    }
    else
    {
        iRet = SetShutter(Shutter_Camyu2Raw(iShutter));
    }

    if ( 0 == iRet )
    {
        g_cModuleParams.cCamAppParam.iShutter = iShutter;
    }
    return iRet;
}

int CCameraController::GetShutter_Camyu(int& iShutter)
{
    iShutter = g_cModuleParams.cCamAppParam.iShutter;
    return 0;
}

int CCameraController::SetGain_Camyu(int iGain)
{
    int iRet = SetGain(Gain_Camyu2Raw(iGain));
    if ( 0 == iRet )
    {
        g_cModuleParams.cCamAppParam.iGain = iGain;
    }
    return iRet;
}

int CCameraController::GetGain_Camyu(int& iGain)
{
    iGain = g_cModuleParams.cCamAppParam.iGain;
    return 0;
}

int CCameraController::SetRgbGain_Camyu(int iGainR, int iGainG, int iGainB)
{
    int iRet = SetRgbGain(
                   MIN(ColorGain_Camyu2Raw(iGainR), 511),
                   MIN(ColorGain_Camyu2Raw(iGainG), 511),
                   MIN(ColorGain_Camyu2Raw(iGainB), 511)
               );
    if ( 0 == iRet )
    {
        g_cModuleParams.cCamAppParam.iGainR = iGainR;
        g_cModuleParams.cCamAppParam.iGainG = iGainG;
        g_cModuleParams.cCamAppParam.iGainB = iGainB;
    }
    return iRet;
}

int CCameraController::GetRgbGain_Camyu(int& iGainR, int& iGainG, int& iGainB)
{
    iGainR = g_cModuleParams.cCamAppParam.iGainR;
    iGainG = g_cModuleParams.cCamAppParam.iGainG;
    iGainB = g_cModuleParams.cCamAppParam.iGainB;
    return 0;
}

int CCameraController::SetCaptureShutter_Camyu(int iShutter, int fEnable)
{
    int iRet = SetCaptureShutter(Shutter_Camyu2Raw(iShutter), fEnable);
    if ( 0 == iRet )
    {
        g_cModuleParams.cCamAppParam.iCaptureShutter = iShutter;
        g_cModuleParams.cCamAppParam.iEnableCaptureShutter = fEnable;
    }
    return iRet;
}

int CCameraController::GetCaptureShutter_Camyu(int& iShutter, int& fEnable)
{
    iShutter = g_cModuleParams.cCamAppParam.iCaptureShutter;
    fEnable = g_cModuleParams.cCamAppParam.iEnableCaptureShutter;
    return 0;
}

int CCameraController::SetCaptureGain_Camyu(int iGain, int fEnable)
{
    int iRet = SetCaptureGain(Gain_Camyu2Raw(iGain), fEnable);
    if ( 0 == iRet )
    {
        g_cModuleParams.cCamAppParam.iCaptureGain = iGain;
        g_cModuleParams.cCamAppParam.iEnableCaptureGain = fEnable;
    }
    return iRet;
}

int CCameraController::GetCaptureGain_Camyu(int& iGain, int& fEnable)
{
    iGain = g_cModuleParams.cCamAppParam.iCaptureGain;
    fEnable = g_cModuleParams.cCamAppParam.iEnableCaptureGain;
    return 0;
}

int CCameraController::SetCaptureRgbGain_Camyu(int iGainR, int iGainG, int iGainB, int fEnable)
{
    int iRet = SetCaptureRgbGain(
                   MIN(ColorGain_Camyu2Raw(iGainR), 511),
                   MIN(ColorGain_Camyu2Raw(iGainG), 511),
                   MIN(ColorGain_Camyu2Raw(iGainB), 511),
                   fEnable
               );
    if ( 0 == iRet )
    {
        g_cModuleParams.cCamAppParam.iCaptureGainR = iGainR;
        g_cModuleParams.cCamAppParam.iCaptureGainG = iGainG;
        g_cModuleParams.cCamAppParam.iCaptureGainB = iGainB;
        g_cModuleParams.cCamAppParam.iEnableCaptureGainRGB = fEnable;
    }
    return iRet;
}

int CCameraController::GetCaptureRgbGain_Camyu(int& iGainR, int& iGainG, int& iGainB, int& fEnable)
{
    iGainR = g_cModuleParams.cCamAppParam.iCaptureGainR;
    iGainG = g_cModuleParams.cCamAppParam.iCaptureGainG;
    iGainB = g_cModuleParams.cCamAppParam.iCaptureGainB;
    fEnable = g_cModuleParams.cCamAppParam.iEnableCaptureGainRGB;
    return 0;
}
#endif // _CAM_APP_

static inline void GetUnsignedChar(PBYTE8& pbData, unsigned char& ucData)
{
    ucData = 0;

    unsigned char ucTmp = 0;
    unsigned char ucData1 = 0;
    unsigned char ucData2 = 0;

    ucData1 = *pbData;
    ++pbData;

    ucData2 = *pbData;
    ++pbData;

    ucTmp = (ucData1&0x0f) | ((ucData2<<4)&0xf0);

    ucData = ucTmp;
}

static inline void GetUnsignedLong(PBYTE8& pbData, unsigned long& ulData)
{
    ulData = 0;

    unsigned long ulTmp = 0;
    unsigned char ucData = 0;

    GetUnsignedChar(pbData, ucData);
    ulTmp = ucData;
    ulData |= (ulTmp&0x000000ff);

    GetUnsignedChar(pbData, ucData);
    ulTmp = ucData;
    ulData |= ((ulTmp<<8)&0x0000ff00);

    GetUnsignedChar(pbData, ucData);
    ulTmp = ucData;
    ulData |= ((ulTmp<<16)&0x00ff0000);

    GetUnsignedChar(pbData, ucData);
    ulTmp = ucData;
    ulData |= ((ulTmp<<24)&0xff000000);
}

// ��ȡBT1120�������е�������ֵ��
#ifdef _CAMERA_PIXEL_500W_
HRESULT CCameraController::GetExtInfoByImage(
    const PBYTE8 pbImageDataY,
    FpgaRegInfo& cFpgaRegInfo,
    AgcAwbInfo& cAgcAwbInfo
)
{
    //������ֵ�����ݸ�ʽ����4λ��Ч��
    PBYTE8 pbLastLineStartY = pbImageDataY + IMAGE_WIDTH*(IMAGE_HEIGHT-1);
    PBYTE8 pbFpgaRegInfoStartA = pbLastLineStartY + 399 + 0;
    PBYTE8 pbAgcAwbInfoStartA = pbLastLineStartY + 799 + 0;
    PBYTE8 pbFpgaRegInfoStartB = pbLastLineStartY + 1623 + 0;
    PBYTE8 pbAgcAwbInfoStartB = pbLastLineStartY + 2023 + 0;

    FpgaRegInfo cFpgaRegInfoTmp;
    AgcAwbInfo cAgcAwbInfoTmp;

    int i = 0;
    unsigned long ulData = 0;

    //======================================================
#define GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(name) \
    GetUnsignedLong(pbFpgaRegInfoStartA, ulData);       \
    cFpgaRegInfoTmp.cFpgaRegInfoA.name = ulData;        \

    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(reg_video_SHT1);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(reg_capture_SHT2);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(reg_LED_FLASH_OUT_EN);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(reg_capture_mode);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(reg_ACSP);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(reg_soft_capture_trig);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(anti_flicker);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(Th_Custom_ab);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(Th_Count_ab);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(reg_fpga_version);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(reg_capture_edge_select);
    for ( i=0; i<8; ++i )
    {
        GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(Gamma_Data_ab[i]);
    }
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(time_cnt_out);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(pxga_addr_r_ab);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(pxga_addr_gr_ab);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(pxga_addr_gb_ab);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(pxga_addr_b_ab);

    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(data_b_Gr);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(data_k_Gr);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(data_b_Gb);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(data_k_Gb);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(data_b_R);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(data_k_R);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(data_b_B);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(data_k_B);

    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(time_clock_out);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_A(sampling_point_ab);

    //======================================================
#define GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(name)  \
    GetUnsignedLong(pbAgcAwbInfoStartA, ulData);        \
    cAgcAwbInfoTmp.cAgcAwbInfoA.name = ulData;          \

    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SumR_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SumG_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SumB_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SumY_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SumU_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SumV_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(PointCount_ab);
    for ( i=15; i>=0; --i )
    {
        GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(AvgY_ab[i]);
    }

    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SUM_Gr_raw_Low_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SUM_Gr_raw_High_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SUM_Gb_raw_Low_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SUM_Gb_raw_High_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SUM_R_raw_Low_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SUM_R_raw_High_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SUM_B_raw_Low_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(SUM_B_raw_High_ab);

    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(reg_gr_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(reg_gb_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(reg_r_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(reg_b_ab);

    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(th_h_reg);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(th_l_reg);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(th_s_reg);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(colr_reg);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(addr_prt);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(cod_x_min_in);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(cod_x_max_in);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(cod_y_min_in);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_A(cod_y_max_in);

    //======================================================
#define GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(name) \
    GetUnsignedLong(pbFpgaRegInfoStartB, ulData);       \
    cFpgaRegInfoTmp.cFpgaRegInfoB.name = ulData;        \

    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(reg_video_SHT1);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(reg_capture_SHT2);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(reg_LED_FLASH_OUT_EN);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(reg_capture_mode);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(reg_ACSP);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(reg_soft_capture_trig);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(anti_flicker);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(Th_Custom_ab);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(Th_Count_ab);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(reg_fpga_version);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(reg_capture_edge_select);
    for ( i=0; i<8; ++i )
    {
        GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(Gamma_Data_ab[i]);
    }
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(time_cnt_out);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(pxga_addr_r_ab);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(pxga_addr_gr_ab);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(pxga_addr_gb_ab);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(pxga_addr_b_ab);

    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(data_b_Gr);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(data_k_Gr);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(data_b_Gb);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(data_k_Gb);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(data_b_R);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(data_k_R);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(data_b_B);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(data_k_B);

    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(time_clock_out);
    GETEXTINFOBYIMAGE_FPGAREGINFO_GET_VALUE_B(sampling_point_ab);

    //======================================================
#define GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(name)  \
    GetUnsignedLong(pbAgcAwbInfoStartB, ulData);        \
    cAgcAwbInfoTmp.cAgcAwbInfoB.name = ulData;          \

    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SumR_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SumG_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SumB_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SumY_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SumU_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SumV_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(PointCount_ab);
    for ( i=15; i>=0; --i )
    {
        GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(AvgY_ab[i]);
    }

    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SUM_Gr_raw_Low_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SUM_Gr_raw_High_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SUM_Gb_raw_Low_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SUM_Gb_raw_High_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SUM_R_raw_Low_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SUM_R_raw_High_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SUM_B_raw_Low_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(SUM_B_raw_High_ab);

    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(reg_gr_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(reg_gb_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(reg_r_ab);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(reg_b_ab);

    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(th_h_reg);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(th_l_reg);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(th_s_reg);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(colr_reg);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(addr_prt);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(cod_x_min_in);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(cod_x_max_in);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(cod_y_min_in);
    GETEXTINFOBYIMAGE_AGCAWBINFO_GET_VALUE_B(cod_y_max_in);

    //======================================================
    cFpgaRegInfo = cFpgaRegInfoTmp;
    cAgcAwbInfo = cAgcAwbInfoTmp;
    return S_OK;
}
#else
HRESULT CCameraController::GetExtInfoByImage(
    const PBYTE8 pbImageDataY,
    FpgaRegInfo& cFpgaRegInfo,
    AgcAwbInfo& cAgcAwbInfo
)
{
    //������ֵ�����ݸ�ʽ����4λ��Ч��
    PBYTE8 pbLastLineStartY = pbImageDataY + IMAGE_WIDTH*(IMAGE_HEIGHT-1);
    PBYTE8 pbFpgaRegInfoStart = pbLastLineStartY + 800 + 2;
    PBYTE8 pbAgcAwbInfoStart = pbLastLineStartY + 1200 + 2;

    FpgaRegInfo cFpgaRegInfoTmp;
    AgcAwbInfo cAgcAwbInfoTmp;

    int i = 0;
    unsigned long ulData = 0;

    //======================================================

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.reg_video_SHT1 = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.reg_capture_SHT2 = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.reg_LED_FLASH_OUT_EN = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.reg_capture_mode = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.reg_ACSP = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.reg_soft_capture_trig = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.anti_flicker = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.Th_Custom = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.Th_Count = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.reg_fpga_version = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.reg_capture_edge_select = ulData;

    for ( i=0; i<8; ++i )
    {
        GetUnsignedLong(pbFpgaRegInfoStart, ulData);
        cFpgaRegInfoTmp.Gamma_Data[i] = ulData;
    }

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.time_cnt_out = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.time_clock_out = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.sampling_point = ulData;

    GetUnsignedLong(pbFpgaRegInfoStart, ulData);
    cFpgaRegInfoTmp.capture_ID = ulData;

    //======================================================

    GetUnsignedLong(pbAgcAwbInfoStart, ulData);
    cAgcAwbInfoTmp.SumR = ulData;

    GetUnsignedLong(pbAgcAwbInfoStart, ulData);
    cAgcAwbInfoTmp.SumG = ulData;

    GetUnsignedLong(pbAgcAwbInfoStart, ulData);
    cAgcAwbInfoTmp.SumB = ulData;

    GetUnsignedLong(pbAgcAwbInfoStart, ulData);
    cAgcAwbInfoTmp.SumY = ulData;

    GetUnsignedLong(pbAgcAwbInfoStart, ulData);
    cAgcAwbInfoTmp.SumU = ulData;

    GetUnsignedLong(pbAgcAwbInfoStart, ulData);
    cAgcAwbInfoTmp.SumV = ulData;

    GetUnsignedLong(pbAgcAwbInfoStart, ulData);
    cAgcAwbInfoTmp.PointCount = ulData;

    for ( i=15; i>=0; --i )
    {
        GetUnsignedLong(pbAgcAwbInfoStart, ulData);
        cAgcAwbInfoTmp.AvgY[i] = ulData;
    }

    //======================================================
    // A67
    // ��ƼӺ������Ϣ
    PBYTE8 pbpbFpgaRegInfoStartTmp = pbLastLineStartY + 968 + 2;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.th_h_prt = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.th_l_prt = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.th_s_prt = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.colr_prt = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.th_l_reg_new = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.colr_reg_new_l = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.colr_reg_new_h = ulData;

    // �񻯡����Ͷȡ��Աȶ�
    pbpbFpgaRegInfoStartTmp = pbLastLineStartY + 1075 + 2;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.sha_th_o = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.sha_ce_o = (ulData&0x01)>>0;
    cFpgaRegInfoTmp.sha_s_or_a_o = (ulData&0x02)>>1;

    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.satu_r_f_max_o = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.satu_r_f_min_o = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.satu_g_f_max_o = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.satu_g_f_min_o = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.satu_b_f_max_o = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.satu_b_f_min_o = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.cont_rgb_f_o = ulData;
    GetUnsignedLong(pbpbFpgaRegInfoStartTmp, ulData);
    cFpgaRegInfoTmp.cont_add_f_o = ulData;

    cFpgaRegInfo = cFpgaRegInfoTmp;
    cAgcAwbInfo = cAgcAwbInfoTmp;

    return S_OK;
}
#endif // _CAMERA_PIXEL_500W_
