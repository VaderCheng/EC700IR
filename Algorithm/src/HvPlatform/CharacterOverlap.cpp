// ���ļ����������WINDOWS-936��ʽ
/*******************************************************************************

log:
1. 2012-04-19 lijj
   ��ӱ������EEPROM���ܡ�
2. 2012-05-10 lijj
   �ַ������·�����ʱ�����洢��FPGA�У�һ���ֻ��ʱ����ֵ������ʱ�����
   ʱ�侫ȷ�����룬ʱ����Ϣ��ͼ�����һ��Yֵ��(��ϸ�ο�Ӳ���ĵ�)
   �������Ϊ(width-25*12)/24����200Wͼ���Ϊ1600�����������54����EEPROM
   ����ĺ������Ϊ56����
3. 2012-10-22 lijj
   �ַ������»��ƣ���DSP�˽��е���(ԭ��������FPGAʵ��)������ӿڲ��䣬���ļ�
   ���߳����Ϊ��ͨ�ࡣ
*******************************************************************************/

#include "config.h"
// һ���������򻯰�
#if defined(_HVCAM_LITE_PLATFORM_) || defined(_CAM_APP_)

#include "CharacterOverlap.h"
#include "hvtarget_ARM.h"

// �򻯰�һ��������������������ȫ��ģ������ṹ��
#if defined(_HVCAM_LITE_PLATFORM_) && !defined(_CAM_APP_)
#include "HvParamInc.h" //  g_cModuleParams
#endif

// �����ȫ��ģ������ṹ��
#if defined(_CAM_APP_)
#include "../CamApp/LoadParam.h"
#endif

extern HV_SEM_HANDLE g_hSemEDMA;

extern "C" void DotMatrixConvert(unsigned char* old_lattice, unsigned char* new_lattice, int width, int height);

/* LoadParam.cpp */
extern CParamStore g_cParamStore;

// EEPROM�����������
// ע����EEPROM��Ԥ��4KB�洢�������ݣ�����ܴ洢������ȡ���ڵ���Ĵ�С��
// ���㹫ʽ��4096/(W*W/8)������WΪ�����ַ��Ŀ�(�����ͬ)
// ����24*24��������ܴ洢56�����֣���112������ַ�
//     32*32��������ܴ洢32�����֣���64������ַ�
#define EEPROM_CHARACTER_LEN 4096

CCharacterOverlap::CCharacterOverlap()
{
}

CCharacterOverlap::~CCharacterOverlap()
{
}

/**
 * EnableCharacterOverlap - �����ַ�����ʹ��
 *
 * @param iValue  1: ʹ��(�����͵���DSP�˽��е���)  0����ʹ��
 *
 * @return E_FAIL��ʧ��   S_OK���ɹ�
 * TODO����һ�ָ��õķ���ʵ��ʹ������ʵʱ��ʾ
 */
HRESULT CCharacterOverlap::EnableCharacterOverlap(int iValue)
{
    HRESULT hr = E_FAIL;

    g_cModuleParams.cCamAppParam.iEnableCharacterOverlap = iValue;
    //�ַ�����ʹ���ж�
    if (g_cModuleParams.cCamAppParam.iEnableCharacterOverlap)
    {
        if (g_cModuleParams.cCamAppParam.iLatticeLen > EEPROM_CHARACTER_LEN)
        {
            g_cModuleParams.cCamAppParam.iLatticeLen = EEPROM_CHARACTER_LEN;
        }
        if (!g_cModuleParams.cCamAppParam.pbLattice && g_cModuleParams.cCamAppParam.iLatticeLen > 0)
        {
            g_cModuleParams.cCamAppParam.pbLattice = new char[g_cModuleParams.cCamAppParam.iLatticeLen];
            memset(g_cModuleParams.cCamAppParam.pbLattice, 0, g_cModuleParams.cCamAppParam.iLatticeLen);
            if (0 > EEPROM_Read(CHARACTER_START_ADDR, (PBYTE8)g_cModuleParams.cCamAppParam.pbLattice, g_cModuleParams.cCamAppParam.iLatticeLen))
            {
                delete []g_cModuleParams.cCamAppParam.pbLattice;
                g_cModuleParams.cCamAppParam.pbLattice = NULL;
            }
        }

        SemPend(&g_hSemEDMA);

        int iFontHeightByte = GetFontHeightByte(g_cModuleParams.cCamAppParam.iFontSize);
        int iFontHeiht = GetFontHeight(g_cModuleParams.cCamAppParam.iFontSize);
        m_cParam.x = g_cModuleParams.cCamAppParam.iX;
        m_cParam.y = g_cModuleParams.cCamAppParam.iY;
        m_cParam.w = g_cModuleParams.cCamAppParam.iLatticeLen/iFontHeightByte; //���������ܴ�С���Ը�ռ�õ��ֽ������ó���ȡ�
        m_cParam.h = iFontHeiht;
        m_cParam.iIsFixedLight = g_cModuleParams.cCamAppParam.iEnableFixedLight;
        m_cParam.iFontColor = g_cModuleParams.cCamAppParam.iFontColor;
        m_cParam.fIsSideInstall = g_cModuleParams.cCamAppParam.fIsSideInstall;

        HV_Trace(5, "<EnableCharacterOverlap> len:%d,size:%d,x:%d,y:%d, w:%d,h:%d, color: %d",
                 g_cModuleParams.cCamAppParam.iLatticeLen,
                 g_cModuleParams.cCamAppParam.iFontSize,
                 g_cModuleParams.cCamAppParam.iX,
                 g_cModuleParams.cCamAppParam.iY,
                 m_cParam.w,
                 m_cParam.h,
                 m_cParam.iFontColor);

        // ���빲���ڴ�
        int nBufferLen = m_cParam.w * m_cParam.h * sizeof(char);
        CreateShareMemOnPool(&m_cData.rgDotMatrix, nBufferLen);

        // ����ת��
        DotMatrixConvert((PBYTE8)g_cModuleParams.cCamAppParam.pbLattice,
                        (PBYTE8)m_cData.rgDotMatrix.addr,
                        m_cParam.w,
                        m_cParam.h);

        hr = g_cHvDspLinkApi.SendStringOverlayInitCmd(&m_cParam, &m_cData, &m_cRespond);

        // �ͷŹ����ڴ�
        FreeShareMemOnPool(&m_cData.rgDotMatrix);
        SemPost(&g_hSemEDMA);
    }
    else
    {
        SemPend(&g_hSemEDMA);

        m_cParam.x = 0;
        m_cParam.y = 0;
        m_cParam.w = 0;
        m_cParam.h = 0;
        m_cParam.iIsFixedLight = 0;
        m_cParam.iFontColor = 0;

        m_cData.rgDotMatrix.addr = NULL;
        m_cData.rgDotMatrix.phys = 0;
        hr = g_cHvDspLinkApi.SendStringOverlayInitCmd(&m_cParam, &m_cData, &m_cRespond);

        SemPost(&g_hSemEDMA);
    }

    return hr;
}

/**
 * SetCharacterOverlap - ������λ�����͹����ĵ������ݵ��ڴ��У�
                         �����͵�DSP�˽�����ʾ
 *
 * @param pLattice �������ݻ�����ָ��
 *
 * @return S_OK
 */
HRESULT CCharacterOverlap::SetCharacterOverlap(char* pLattice)
{
    if (g_cModuleParams.cCamAppParam.pbLattice != NULL)
    {
        delete []g_cModuleParams.cCamAppParam.pbLattice;
        g_cModuleParams.cCamAppParam.pbLattice = NULL;
    }
    if (g_cModuleParams.cCamAppParam.iLatticeLen > 0)
    {
        g_cModuleParams.cCamAppParam.pbLattice = new char[g_cModuleParams.cCamAppParam.iLatticeLen];
        if (g_cModuleParams.cCamAppParam.pbLattice)
        {
            memcpy(g_cModuleParams.cCamAppParam.pbLattice, pLattice, g_cModuleParams.cCamAppParam.iLatticeLen);
        }
    }
    EnableCharacterOverlap(g_cModuleParams.cCamAppParam.iEnableCharacterOverlap);
    return S_OK;
}

/**
 * SetCharacterLightness - �������������Ƿ�̶�
 *
 * @param iIsFixedLight 1�����ȹ̶�(ʹ�������RGB��ɫ)
 *                      0�����Ȳ��̶�(�뱳��ɫ�γɷ����DSPʵ��)
 *
 * @return S_OK
 */
HRESULT CCharacterOverlap::SetCharacterLightness(int iIsFixedLight)
{
    g_cModuleParams.cCamAppParam.iEnableFixedLight = iIsFixedLight;
    EnableCharacterOverlap(g_cModuleParams.cCamAppParam.iEnableCharacterOverlap);
    return S_OK;
}
/**
 * SaveCharacterInfo - ���������Ϣ���������ݱ��浽EEPROM�����ꡢ��ɫ�ȱ��浽XML
 *
 *
 * @return S_OK��ִ�гɹ�  E_FAIL:����ʧ��
 */
HRESULT CCharacterOverlap::SaveCharacterInfo(void)
{
    if (g_cModuleParams.cCamAppParam.pbLattice)
    {
        if (g_cModuleParams.cCamAppParam.iLatticeLen > EEPROM_CHARACTER_LEN)
        {
            g_cModuleParams.cCamAppParam.iLatticeLen = EEPROM_CHARACTER_LEN;
        }
        //����������д��EEPROM����
        if (EEPROM_Write(CHARACTER_START_ADDR, (PBYTE8)g_cModuleParams.cCamAppParam.pbLattice, g_cModuleParams.cCamAppParam.iLatticeLen) < 0)
        {
            HV_Trace(5, "<SaveCharacterInfo>: WriteCharacterToEEPROM error!\n");
            return E_FAIL;
        }
        HV_Trace(5, "<CCharacterOverlap> Write to EEPROM x:%d, y:%d, size:%d, len: %d\n", g_cModuleParams.cCamAppParam.iX, g_cModuleParams.cCamAppParam.iY, g_cModuleParams.cCamAppParam.iFontSize, g_cModuleParams.cCamAppParam.iLatticeLen);
        g_cParamStore.SetInt("\\CamApp", "Character_X", g_cModuleParams.cCamAppParam.iX);
        g_cParamStore.SetInt("\\CamApp", "Character_Y", g_cModuleParams.cCamAppParam.iY);
        g_cParamStore.SetInt("\\CamApp", "Character_Size", g_cModuleParams.cCamAppParam.iFontSize);
        g_cParamStore.SetInt("\\CamApp", "Character_DateFormat", g_cModuleParams.cCamAppParam.iDateFormat);
        g_cParamStore.SetInt("\\CamApp", "Character_FontColor", g_cModuleParams.cCamAppParam.iFontColor);
        g_cParamStore.SetInt("\\CamApp", "Character_LatticeLen", g_cModuleParams.cCamAppParam.iLatticeLen);
        g_cParamStore.SetInt("\\CamApp", "EnableCharacter", g_cModuleParams.cCamAppParam.iEnableCharacterOverlap);
        g_cParamStore.Save();
        return S_OK;
    }
    return S_OK;
}

/**
 * MakeDateTime - ��װ����ʱ����Ϣ�ַ���
 *
 * @param pDateTimeString: ����ʱ���ַ�����������߿��ٿռ�
 * @param dwTimeLow      : 64λʱ���32λ
 * @param dwTimeHigh     : 64λʱ���32λ
 *
 * @return S_OK : �ɹ�   E_FAIL : ʧ��
 */
HRESULT CCharacterOverlap::MakeDateTimeString(BYTE8* pDateTimeString, DWORD32 dwTimeLow, DWORD32 dwTimeHigh)
{
    if (pDateTimeString == NULL)
        return E_FAIL;
    if (g_cModuleParams.cCamAppParam.iEnableCharacterOverlap)
    {
        REAL_TIME_STRUCT rt;
        ConvertMsToTime(dwTimeLow, dwTimeHigh, &rt);
        if (g_cModuleParams.cCamAppParam.iDateFormat == 0)
        {
            sprintf((char *)pDateTimeString, "%04d/%02d/%02d %02d:%02d:%02d:%03d ", rt.wYear, rt.wMonth, rt.wDay, rt.wHour, rt.wMinute, rt.wSecond, rt.wMSecond);
        }
        else if (g_cModuleParams.cCamAppParam.iDateFormat == 1)
        {
            sprintf((char *)pDateTimeString, "%04d-%02d-%02d %02d:%02d:%02d:%03d ", rt.wYear, rt.wMonth, rt.wDay, rt.wHour, rt.wMinute, rt.wSecond, rt.wMSecond);
        }
    }
    else
    {
        strcpy((char *)pDateTimeString, "");
    }

    return S_OK;
}

#endif
