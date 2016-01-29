#ifndef SWPROCESSXMLCMD_H
#define SWPROCESSXMLCMD_H

#include "SWFC.h"
#include "swpa.h"
#include "SWList.h"
#include "tinyxml.h"
#include "SWCOMTestDevice.h"

typedef DWORD WPARAM;
typedef DWORD LPARAM;

//������Ӧӳ��궨��
#define SW_BEGIN_COMMAND_MAP() virtual HRESULT OnCommand(DWORD dwID,WPARAM wParam,LPARAM lParam,INT *iExitCode, BOOL *fGoingToExit, BOOL *fSwitchingSystem){HRESULT hr = E_NOTIMPL;if(hr == E_NOTIMPL){switch(dwID){
#define SW_COMMAND_HANDLER(dwID, func) case dwID : hr = func(wParam,lParam,iExitCode,fGoingToExit,fSwitchingSystem); break;
#define SW_END_COMMAND_MAP()default: break;}}return hr;}

#define MAX_COMMAND_LENGTH 50

enum _DEVTYPE
{
    DEVTYPE_VENUS = 0,
    DEVTYPE_JUPITER,
    DEVTYPE_SATURN
};

enum _FPGATYPE
{
    TYPE_FAKEIMG = 0,   //��ͼ
    TYPE_185,
    TYPE_178,
    TYPE_249
};

//XML����ID
enum _COMMAND_ID{
    CMD_MIN = 0,

    CMD_ResetDevice,            //��λ�豸,�л�ϵͳ
    CMD_OptResetMode,           //��ȡ��λģʽ
    CMD_RestoreFactorySetting,  //�ָ������趨
    CMD_UnlockFactorySetting,   //���������趨
    CMD_WriteSN,                //д�豸���к�

    CMD_SetTime,                //����ʱ��
    CMD_GetTime,                //��ȡʱ��
    CMD_HDDStatus,              //SD��״̬
    CMD_GetDevType,             //��ȡ�豸����

    CMD_SetDCAperture,          //�����Զ�DC��Ȧ
    CMD_GetDCAperture,          //��ȡ�Զ�DC��Ȧ
    CMD_ZoomDCIRIS,             //�Ŵ�DC��Ȧ
    CMD_ShrinkDCIRIS,           //��СDC��Ȧ

    CMD_SetF1IO,                //����F1�����
    CMD_GetF1IO,                //��ȡF1�����
    CMD_SetALMIO,               //����ALM�����
    CMD_GetALMIO,               //��ȡALM�����
    CMD_SetEXPIO,               //���ò�������
    CMD_GetEXPIO,               //��ȡ��������
    CMD_SetTGIO,                //����TG��
    CMD_GetTGIO,                //��ȡTG������
    CMD_SetCtrlCpl,             //�˹�Ƭ����

    CMD_GetCoilTiggerCount,     //��ȡץ�ļ���
    CMD_ResetCoilTiggerCount,   //ץ�ļ�������

    CMD_SetComTestEnable,       //�������ڲ���
    CMD_SetComTestdisable,      //ֹͣ���ڲ���

    CMD_SetAGCEnable,           //����AGC
    CMD_GetAGCEnable,           //��ȡAGC
    CMD_SetGain,                //��������
    CMD_GetGain,                //��ȡ����
    CMD_SetShutter,             //���ÿ���
    CMD_GetShutter,             //��ȡ����
    CMD_SetAWBEnable,           //����AWB
    CMD_GetAWBEnable,           //��ȡAWB
    CMD_SetRGBGain,             //����RGB
    CMD_GetRGBGain,             //��ȡRGB
    CMD_AutoTestCamera,         //�Զ����������
    CMD_SetAF,                  //����AFģʽ
    CMD_GetAF,                  //��ȡAFģʽ

    CMD_SwitchFPGA,             //�л�FPGA
    CMD_GetFPGAMode,            //��ȡFPGAģʽ

    CMD_TuneFocus,              //�ֶ��۽�
    CMD_GetFocus,               //��ǰ�ľ۽�ֵ
    CMD_TuneZoom,               //���ý���
    CMD_GetZoom,                //��ȡ����

    CMD_StartPan,               //��ʼˮƽ�˶�
    CMD_StopPan,                //ֹͣˮƽ�˶�
    CMD_StartTilt,              //��ʼ��ֱ�˶�
    CMD_StopTilt,               //ֹͣ��ֱ�˶�
    CMD_StartWiper,             //�������
    CMD_SetLEDModeEx,           //����LED��
    CMD_SetDefog,               //���ȳ���

    CMD_MAX
};


class CSWProcessXMLCmd
{
public:
    /**
    * @brief ���캯��
    */
    CSWProcessXMLCmd();

    /**
    * @brief ��ʼ������
    */
    HRESULT Initialize(BOOL fBackupMode,INT nDevType);

    /**
    * @brief ��������
    */
    ~CSWProcessXMLCmd();

    /**
    * @brief ��������ϵͳ
    */
    VOID SetBackupMode(BOOL fMode){ m_IsBackupMode = fMode;}

    /**
    * @brief ��ȡ����ϵͳ
    */
    BOOL GetBackupMode(){return m_IsBackupMode;}

    /**
    * @brief �����ϴδ��������ִ�
    */
    char* GetLastCommand(){return m_szLastXMLCmd;}

    /**
    * @brief �����Ƿ�֧����������
    */
    VOID SetSupportNoBreakUpgrade(BOOL fSupport){m_fSupportNoBreakUpgrade = fSupport;}

    /**
    * @brief ���õ�ǰƽ̨
    */
    VOID SetDevType(INT nType){ m_dwDevType = nType;}

    /**
    * @brief ����XML����
    */
    HRESULT ProcessXMLCmd(const PBYTE pbBuf, const DWORD dwSize, PVOID* ppvOutBuf, DWORD* pdwOutSize,INT* iExitCode,BOOL* fGoingToExit,BOOL* fSwitchingSystem);
private:
    /*
    * @brief ������ӦXML���ļ�ͷ
    * @param [out] ppXmlOutputDoc : ��ӦXML���ļ�ָ��
    * @retval S_OK : �ɹ�
    * @retval E_FAIL: ʧ��
    * @retval E_OUTOFMEMORY: �ڴ治��
    */
    HRESULT GenerateReplyXMLHeader(TiXmlDocument ** ppXmlOutputDoc);

    /*
    * @brief ���ɴ�������ʧ�ܵ�XML
    * @param [in] pXmlOutputDoc : ��ӦXML���ļ�ָ��
    * @param [in] szCmdName : �������������
    * @retval S_OK : �ɹ�
    * @retval E_FAIL: ʧ��
    * @retval E_OUTOFMEMORY: �ڴ治��
    */
    HRESULT GenerateCMDProcessFailedXml(TiXmlDocument * pXmlOutputDoc, const CHAR * szCmdName);

    /*
    * @brief ��������û�д���������(Type)���Ե�XML
    * @param [in] pXmlOutputDoc : ��ӦXML���ļ�ָ��
    * @param [in] szCmdName : �������������
    * @retval S_OK : �ɹ�
    * @retval E_FAIL: ʧ��
    * @retval E_OUTOFMEMORY: �ڴ治��
    */
    HRESULT GenerateNoTypeErrorXml(TiXmlDocument * pXmlOutputDoc, const CHAR * szCmdName);

    /*
    * @brief ��������û�д���������(Class)���Ե�XML
    * @param [in] pXmlOutputDoc : ��ӦXML���ļ�ָ��
    * @param [in] szCmdName : �������������
    * @retval S_OK : �ɹ�
    * @retval E_FAIL: ʧ��
    * @retval E_OUTOFMEMORY: �ڴ治��
    */
    HRESULT GenerateNoClassErrorXml(TiXmlDocument * pXmlOutputDoc, const CHAR * szCmdName);

    /*
    * @brief ��������û�д�����ֵ(Value)���Ե�XML
    * @param [in] pXmlOutputDoc : ��ӦXML���ļ�ָ��
    * @param [in] szCmdName : �������������
    * @retval S_OK : �ɹ�
    * @retval E_FAIL: ʧ��
    * @retval E_OUTOFMEMORY: �ڴ治��
    */
    HRESULT GenerateNoValueErrorXml(TiXmlDocument * pXmlOutputDoc, const CHAR * szCmdName);

    /*
    * @brief �����޷���ȷ�������յ���XML�ļ��Ĵ��ļ�
    * @param [in] pXmlOutputDoc : ��ӦXML���ļ�ָ��
    * @retval S_OK : �ɹ�
    * @retval E_FAIL: ʧ��
    * @retval E_OUTOFMEMORY: �ڴ治��
    */
    HRESULT GenerateCMDParseFailedXml(TiXmlDocument * pXmlOutputDoc);

    /*
    * @brief �����������ӦXML
    * @param [in] pXmlOutputDoc : ��ӦXML���ļ�ָ��
    * @param [in] szCmdName : �������������
    * @param [in] pszType : �����������������
    * @param [in] pvData : ������������ݻ�������ַ
    * @retval S_OK : �ɹ�
    * @retval E_FAIL: ʧ��
    * @retval E_OUTOFMEMORY: �ڴ治��
    */
    HRESULT GenerateCMDReplyXml(TiXmlDocument * pXmlOutputDoc, const CHAR * szCmdName, const CHAR * pszType, const CHAR * pszClass, const PVOID pvData);

    /*
    * @brief ����û���ҵ������XML���ļ�
    * @param [in] pXmlOutputDoc : ��ӦXML���ļ�ָ��
    * @param [in] szCmdName : �������������
    * @retval S_OK : �ɹ�
    * @retval E_FAIL: ʧ��
    * @retval E_OUTOFMEMORY: �ڴ治��
    */
    HRESULT GenerateCMDNotFoundXml(TiXmlDocument * pXmlOutputDoc, const CHAR * szCmdName);

    /*
    * @brief ��XML���ת���ַ���
    * @param [in] pXmlOutputDoc : XML���ļ�ָ��
    * @param [out] ppvXMLOutBuf : �ַ�����ַ
    * @param [out] pdwXMLOutSize : �ַ����ĳ���
    * @retval S_OK : �ɹ�
    * @retval E_FAIL: ʧ��
    * @retval E_OUTOFMEMORY: �ڴ治��
    * @retval E_INVALIDARG: �����Ƿ�
    */
    HRESULT PackXml(TiXmlDocument* pXmlOutputDoc,  PVOID* ppvXMLOutBuf,  DWORD *pdwXMLOutSize);

    /*
    * @brief ����������ת����ϢID���͸���������ģ��
    * @param [in] dwCMDID : ����ID
    * @param [in] szValue : ��������ֵ
    * @param [in] szType : ������������
    * @param [out] pvRetBuf : �����ֵ�Ļ����ַ
    * @retval S_OK : �ɹ�
    * @retval E_FAIL: ʧ��
    * @retval E_OUTOFMEMORY: �ڴ治��
    * @retval E_INVALIDARG: �����Ƿ�
    */
    HRESULT SendCMD(const DWORD dwCMDID, const CHAR * szValue, const CHAR * szType, const CHAR* szClass, PVOID* ppvRetBuf,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);

    /*
    * @brief �����Զ�������
    * @param [in] dwCMDID : ����ID
    * @param [in] szValue : ��������ֵ
    * @param [out] pvRetBuf : �����ֵ�Ļ����ַ
    * @retval S_OK : �ɹ�
    * @retval E_FAIL: ʧ��
    * @retval E_OUTOFMEMORY: �ڴ治��
    * @retval E_INVALIDARG: �����Ƿ�
    */
    HRESULT SendCustomCMD(const DWORD dwCMDID, const CHAR * szValue, PVOID* ppvRetBuf,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);

    BOOL IsVenus(){return m_dwDevType==DEVTYPE_VENUS;}

    BOOL IsJupiter(){return m_dwDevType==DEVTYPE_JUPITER;}

    BOOL IsSaturn(){return m_dwDevType==DEVTYPE_SATURN;}

    HRESULT GetVersion(const CHAR* szTag,CHAR* pszVersion);

    HRESULT ipnc_cient_control(int linkId, int cmd, void *pPrm, int prmSize, int timeout);

    HRESULT InitDomeCamera();
private:
    /*
     * @brief д�豸���к�
     * @param [in] szSN:���豸���к�
     * @retval S_OK:�ɹ�
     * @retval E_FAIL:ʧ��
     */
    HRESULT OnWriteSN(const CHAR* szSN);

    /*
     * @brief ��λ�豸���л�ϵͳ
     */
    HRESULT OnResetDevice(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡ�豸��ǰģʽ
     */
    HRESULT OnOptResetMode(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ������,�ָ���������
     */
    HRESULT OnRestoreFactorySetting(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief �������ڣ��ָ�����
     */
    HRESULT OnUnlockFactorySetting(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ����ϵͳʱ��
     */
    HRESULT OnSetTime(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡϵͳʱ��
     */
    HRESULT OnGetTime(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡSD��״̬
     */
    HRESULT OnHDDStatus(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡSD��״̬
     */
    HRESULT OnGetDevType(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ʹ���Զ�DC
     */
    HRESULT OnSetDCAperture(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡ�Զ�DC״̬
     */
    HRESULT OnGetDCAperture(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief �Ŵ�DC��Ȧ
     */
    HRESULT OnZoomDCIRIS(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��СDC��Ȧ
     */
    HRESULT OnShrinkDCIRIS(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ����F1�����
     */
    HRESULT OnSetF1IO(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡF1�����
     */
    HRESULT OnGetF1IO(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ����ALM�����
     */
    HRESULT OnSetALMIO(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡALM�����
     */
    HRESULT OnGetALMIO(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ����EXP�����
     */
    HRESULT OnSetEXPIO(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡEXP�����
     */
    HRESULT OnGetEXPIO(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ����TG������
     */
    HRESULT OnSetTGIO(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡTG������
     */
    HRESULT OnGetTGIO(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief �˹�Ƭ����
     */
    HRESULT OnSetCtrlCpl(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief �������ڲ���
     */
    HRESULT OnSetComTestEnable(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ֹͣ���ڲ���
     */
    HRESULT OnSetComTestdisable(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡץ�ļ���
     */
    HRESULT OnGetCoilTiggerCount(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ץ�ļ�������
     */
    HRESULT OnResetCoilTiggerCount(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ����AGC
     */
    HRESULT OnSetAGCEnable(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡAGC
     */
    HRESULT OnGetAGCEnable(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief �ֶ���������
     */
    HRESULT OnSetGain(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡ�ֶ�����
     */
    HRESULT OnGetGain(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief �ֶ����ÿ���
     */
    HRESULT OnSetShutter(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡ�ֶ�����
     */
    HRESULT OnGetShutter(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ����AWB
     */
    HRESULT OnSetAWBEnable(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡAWB����
     */
    HRESULT OnGetAWBEnable(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ����RGB
     */
    HRESULT OnSetRGBGain(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡRGB
     */
    HRESULT OnGetRGBGain(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief �Զ��������
     */
    HRESULT OnAutoTestCamera(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);

    /*
     * @brief �л�FPGA
     */
    HRESULT OnSwitchFPGA(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡFPGAģʽ
     */
    HRESULT OnGetFPGAMode(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief �۽�ģʽ
     */
    HRESULT OnSetAF(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡ�۽�ģʽ
     */
    HRESULT OnGetAF(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief �ֶ��۽�
     */
    HRESULT OnTuneFocus(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡ�۽�ֵ
     */
    HRESULT OnGetFocus(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ���ý���
     */
    HRESULT OnTuneZoom(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ��ȡ����
     */
    HRESULT OnGetZoom(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief �������ˮƽת��
     */
    HRESULT OnStartPan(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ֹͣ���ˮƽת��
     */
    HRESULT OnStopPan(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ���������ֱת��
     */
    HRESULT OnStartTilt(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ֹͣ�����ֱת��
     */
    HRESULT OnStopTilt(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ����������
     */
    HRESULT OnStartWiper(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief �������LED��
     */
    HRESULT OnSetLEDModeEx(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);
    /*
     * @brief ����������
     */
    HRESULT OnSetDefog(WPARAM wParam,LPARAM lParam,INT* iExitCode, BOOL* fGoingToExit, BOOL* fSwitchingSystem);

    //����ӳ���
    SW_BEGIN_COMMAND_MAP()
        SW_COMMAND_HANDLER(CMD_ResetDevice,OnResetDevice)
        SW_COMMAND_HANDLER(CMD_OptResetMode,OnOptResetMode)
        SW_COMMAND_HANDLER(CMD_RestoreFactorySetting,OnRestoreFactorySetting)
        SW_COMMAND_HANDLER(CMD_UnlockFactorySetting,OnUnlockFactorySetting)
        SW_COMMAND_HANDLER(CMD_SetTime,OnSetTime)
        SW_COMMAND_HANDLER(CMD_GetTime,OnGetTime)
        SW_COMMAND_HANDLER(CMD_HDDStatus,OnHDDStatus)
        SW_COMMAND_HANDLER(CMD_GetDevType,OnGetDevType)


        SW_COMMAND_HANDLER(CMD_SetDCAperture,OnSetDCAperture)
        SW_COMMAND_HANDLER(CMD_GetDCAperture,OnGetDCAperture)
        SW_COMMAND_HANDLER(CMD_ZoomDCIRIS,OnZoomDCIRIS)
        SW_COMMAND_HANDLER(CMD_ShrinkDCIRIS,OnShrinkDCIRIS)

        SW_COMMAND_HANDLER(CMD_SetF1IO,OnSetF1IO)
        SW_COMMAND_HANDLER(CMD_GetF1IO,OnGetF1IO)
        SW_COMMAND_HANDLER(CMD_SetALMIO,OnSetALMIO)
        SW_COMMAND_HANDLER(CMD_GetALMIO,OnGetALMIO)
        SW_COMMAND_HANDLER(CMD_SetEXPIO,OnSetEXPIO)
        SW_COMMAND_HANDLER(CMD_GetEXPIO,OnGetEXPIO)
        SW_COMMAND_HANDLER(CMD_SetTGIO,OnSetTGIO)
        SW_COMMAND_HANDLER(CMD_GetTGIO,OnGetTGIO)
        SW_COMMAND_HANDLER(CMD_SetCtrlCpl,OnSetCtrlCpl)

        SW_COMMAND_HANDLER(CMD_GetCoilTiggerCount,OnGetCoilTiggerCount)
        SW_COMMAND_HANDLER(CMD_ResetCoilTiggerCount,OnResetCoilTiggerCount)

        SW_COMMAND_HANDLER(CMD_SetComTestEnable,OnSetComTestEnable)
        SW_COMMAND_HANDLER(CMD_SetComTestdisable,OnSetComTestdisable)

        SW_COMMAND_HANDLER(CMD_SetAGCEnable,OnSetAGCEnable)
        SW_COMMAND_HANDLER(CMD_GetAGCEnable,OnGetAGCEnable)
        SW_COMMAND_HANDLER(CMD_SetGain,OnSetGain)
        SW_COMMAND_HANDLER(CMD_GetGain,OnGetGain)
        SW_COMMAND_HANDLER(CMD_SetShutter,OnSetShutter)
        SW_COMMAND_HANDLER(CMD_GetShutter,OnGetShutter)
        SW_COMMAND_HANDLER(CMD_SetAWBEnable,OnSetAWBEnable)
        SW_COMMAND_HANDLER(CMD_GetAWBEnable,OnGetAWBEnable)
        SW_COMMAND_HANDLER(CMD_SetRGBGain,OnSetRGBGain)
        SW_COMMAND_HANDLER(CMD_GetRGBGain,OnGetRGBGain)
        SW_COMMAND_HANDLER(CMD_AutoTestCamera,OnAutoTestCamera)

        SW_COMMAND_HANDLER(CMD_SwitchFPGA,OnSwitchFPGA)
        SW_COMMAND_HANDLER(CMD_GetFPGAMode,OnGetFPGAMode)

        SW_COMMAND_HANDLER(CMD_SetAF,OnSetAF)
        SW_COMMAND_HANDLER(CMD_GetAF,OnGetAF)

        SW_COMMAND_HANDLER(CMD_TuneFocus,OnTuneFocus)
        SW_COMMAND_HANDLER(CMD_GetFocus,OnGetFocus)
        SW_COMMAND_HANDLER(CMD_TuneZoom,OnTuneZoom)
        SW_COMMAND_HANDLER(CMD_GetZoom,OnGetZoom)

        SW_COMMAND_HANDLER(CMD_StartPan,OnStartPan)
        SW_COMMAND_HANDLER(CMD_StopPan,OnStopPan)
        SW_COMMAND_HANDLER(CMD_StartTilt,OnStartTilt)
        SW_COMMAND_HANDLER(CMD_StopTilt,OnStopPan)
        SW_COMMAND_HANDLER(CMD_StartWiper,OnStartWiper)
        SW_COMMAND_HANDLER(CMD_SetLEDModeEx,OnSetLEDModeEx)
        SW_COMMAND_HANDLER(CMD_SetDefog,OnSetDefog)
    SW_END_COMMAND_MAP()

private:
    //�Ƿ񱸷�ϵͳ
    BOOL m_IsBackupMode;
    //֧�ֲ���λ�豸��������
    BOOL m_fSupportNoBreakUpgrade;
    //���ִ������
    CHAR m_szLastXMLCmd[MAX_COMMAND_LENGTH];
    //�Ƿ�֧������
    BOOL m_fSupportCommand;
    //�豸����
    INT m_dwDevType;
    //���ڲ����豸
    CSWCOMTestDevice* m_pComTestDevic;
    //IPC_Clientͨ��SOCKET
    CSWTCPSocket m_cLocalTcpSock;
    //�����ַ���
    CHAR m_szXMLCommandSockFile[32];
};

#endif // SWPROCESSXMLCMD_H
