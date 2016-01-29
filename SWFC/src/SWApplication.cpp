///////////////////////////////////////////////////////////
//  CSWApplication.cpp
//  Implementation of the Class CSWApplication
//  Created on:      28-����-2013 14:09:46
//  Original author: zhouy
///////////////////////////////////////////////////////////
#include "swpa.h"
#include "SWFC.h"
#include "SWApplication.h"

CSWString CSWApplication::m_strExeName;
INT  CSWApplication::m_iPos = -1;
BOOL CSWApplication::m_fExit = FALSE;
INT  CSWApplication::m_iExitCode = 0;
INT  CSWApplication::m_iHandle = -1;
/**
 * @brief ���캯��
 */
CSWApplication::CSWApplication(){

}

/**
 * @brief ��������
 */
CSWApplication::~CSWApplication(){

}

/**
 *@brief ȡ�ô���İ汾�� 
 *@return ���ذ汾��,ʧ�ܷ���-1
 */
INT CSWApplication::GetVersion(VOID)
{
#ifdef VERSION
	return VERSION;
#else
	return -1;
#endif	
}

/**
 *@brief ȡ��DSP����İ汾�� 
 *@return ����DSP�汾��,ʧ�ܷ���-1
 */
INT CSWApplication::GetDSPVersion(VOID)
{
#ifdef DSPVERSION
	return DSPVERSION;
#else
	return -1;
#endif
}

/**
 *@brief ������
 *@return ���س���ȫ·����
 */
LPCSTR CSWApplication::GetExeName(VOID)
{
	return (LPCSTR)m_strExeName;
}

/**
 * @brief �����ӽ���
 * 
 * @param [in] szName : �ӽ�����
 * @param [out] pdwProcessID : �ӽ��̵�ID
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWApplication::CreateChildProcess(const CHAR* szName, DWORD* pdwProcessID){

    int attr = 0;


    if ( 0 == swpa_process_create(
        (int*)pdwProcessID,
        &attr,
        (char*)szName,
        NULL) )
    {
        return S_OK;
    }

	return E_FAIL;
}

/**
 *@brief �жϽ����Ƿ��Ѿ�ִ����Exit����
 *@return �˳��򷵻�S_OK������ֵΪ�������
 */
BOOL CSWApplication::IsExited(VOID)
{
	return m_fExit;
}

/**
 * @brief �����˳�����
 * 
 * @param  [in] sdwExitCode : �˳���
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWApplication::Exit(INT sdwExitCode){

		if(!m_fExit)
		{
			m_fExit = TRUE;
    	m_iExitCode = sdwExitCode;
    }
    return S_OK;
}

INT CSWApplication::GetExitCode(VOID)
{
	return m_iExitCode;
}

/**
 *@brief ����������
 *@return �ɹ�����S_OK����ֵ����ʧ��	 
 */
HRESULT CSWApplication::HeartBeat(DWORD dwTime)
{
	if(-1 != m_iPos)
	{
		CSWFile file;
		if(S_OK == file.Open((LPCSTR)GetCommandString("-f", "BLOCK//tmp/.running"), "w+"))
		{
			file.Seek(m_iPos*sizeof(DWORD), 0);
			file.Write(&dwTime, sizeof(DWORD));
			file.Close();
			return S_OK;
		}
	}
	return E_FAIL;
}

/**
 * @brief ��ʼ������ʵ��
 * 
 * @param  [in] wArgc : ���̲�������
 * @param  [in] szArgv : ���̲�����
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWApplication::InitInstance(const WORD wArgc, const CHAR** szArgv){

	m_strExeName = szArgv[0];
	if(-1 == m_iHandle)
	{
		m_iHandle = swpa_map_create(OnCompare);
	}
	INT i = 1;
	while(i < wArgc)
	{
		if(szArgv[i][0] == '-' && i < wArgc -1)
		{			
			swpa_map_add(m_iHandle, new CSWString(szArgv[i]), new CSWString(szArgv[i + 1]));
			if(!swpa_strcmp(szArgv[i], "-p"))
			{
				m_iPos = swpa_atoi(szArgv[i+1]);
			}
			i+= 2;
		}
		else
		{
			i++;
		}
	}
	return S_OK;
}

/**
 *@brief �Ƚ�2��key�Ĵ�С
 *@param pKey1 ��ֵ1
 *@param pKey1 ��ֵ2
 *@return pKey1 < pKey2,����-1;pKey1 = pKey2,����0;pKey1 > pKey2,����1
 */
INT CSWApplication::OnCompare(PVOID pKey1, PVOID pKey2)
{
	CSWString* strKey1 = (CSWString *)pKey1;
	CSWString* strKey2 = (CSWString *)pKey2;
	return strKey1->Compare(*strKey2);
}

/**
 * @brief ������ִ������
 * 
 * @param  [in] wArgc : ���̲�������
 * @param  [in] szArgv : ���̲�����
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
 
LPCSTR CSWApplication::GetCommandString(LPCSTR szOption, LPCSTR szDefault)
{
	CSWString *strValue = NULL;
	if(-1 != m_iHandle)
	{
		CSWString *strOpt = new CSWString(szOption);
		strValue = (CSWString *)swpa_map_find(m_iHandle, strOpt);
		strOpt->Release();
	}
	return NULL == strValue ? szDefault : (LPCSTR)*strValue;
}

/**
 * @brief �ͷ�ʵ��
 * 
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWApplication::ReleaseInstance(){

	if(-1 != m_iHandle)
	{
		int pos = swpa_map_open(m_iHandle);
		if(-1 != pos)
		{
			while(-1 != pos) 
			{
				CSWString *strKey = (CSWString *)swpa_map_key(m_iHandle, pos);
				CSWString *strValue = (CSWString *)swpa_map_value(m_iHandle, pos);
				
				strKey->Release();
				strValue->Release();
				
				pos = swpa_map_next(m_iHandle);
			}
			swpa_map_close(m_iHandle);
		}
	}
	SW_TRACE_NORMAL("pragram exit.\n");
	//exit(m_iExitCode);
	return S_OK;
}

/**
 * @brief �������庯�������������ظú���ʵ������������
 * 
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWApplication::Run(){

	return S_OK;
}

/**
 * @brief �쳣������
 * @param [in] iSignalNo�쳣�ź�
 */
HRESULT CSWApplication::OnException(INT iSignalNo)
{
	return Exit(iSignalNo);
}

/**
 * @brief ����˯��
 * 
 * @param  [in] dwMS : ˯��ʱ�䣬��λΪ����
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWApplication::Sleep(const DWORD dwMS){

#ifdef WIN32
    ::Sleep(dwMS);
#else
    swpa_thread_sleep_ms(dwMS);
#endif

	return  S_OK;
}

/**
 * @brief ϵͳ���ú���
 * 
 * @param  [in] szCmd : ����ϵͳִ�е�����
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWApplication::System(const CHAR* szCmd){

    int iProcID = 0;
    int iProcAttr = 0;
    int iExitCode = 0;

    if ( 0 == swpa_process_create(
        &iProcID,
        &iProcAttr,
        (char*)szCmd,
        NULL) )
    {
    		WaitFor(iProcID, -1, &iExitCode);
        return !iExitCode ? S_OK : E_FAIL;
    }

	return E_FAIL;
}

/**
 * @brief ��ֹ�ӽ���
 * 
 * @param  [in] dwPID : �ӽ���ID
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWApplication::TerminateChildProcess(const DWORD dwPID){

	if ( 0 == swpa_process_kill(dwPID) )
    {
        return S_OK;
    }

	return E_FAIL;
}

/**
* @brief �ȴ��ӽ����˳�
* 
* @param  [in] dwPID : �ӽ���ID
* @param  [in] iTimeOut : �ȴ���ʱ���ƣ���λΪ����
* @param  [out] iExitCode : �˳���
* @return
* - S_OK : �ӽ��̳ɹ��˳�
* - E_FAIL : �ӽ���δ�ɹ��˳���ʧ�ܡ�
*/
HRESULT CSWApplication::WaitFor(const DWORD& dwPID, const INT& iTimeOut, INT* iExitCode){

		INT kill_code = 0, exit_code = 0;
    if ( 0 == swpa_process_wait(dwPID, iTimeOut, &kill_code, &exit_code) )
    {
    		if(iExitCode)
        {
        	*iExitCode = (kill_code << 8) | (exit_code);
        }
        return S_OK;
    }

	return E_FAIL;
}


TiXmlElement *CSWApplication::GetElement(TiXmlElement *pElement, LPCSTR szPath)
{
	CSWString strTmp;
	for(LPCSTR ch = szPath; pElement && *ch != '\0'; ch++)
	{
		if(*ch != '\\')
		{
			strTmp += *ch;
		}
		else
		{
			pElement = pElement->FirstChildElement((LPCSTR)strTmp);
			strTmp = "";
		}			
	}

	if(!strTmp.IsEmpty() && swpa_strcmp((LPCSTR)strTmp, szPath))
	{
		pElement = pElement->FirstChildElement((LPCSTR)strTmp);
	}
	return pElement;
}

