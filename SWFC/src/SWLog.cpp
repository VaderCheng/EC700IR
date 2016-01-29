///////////////////////////////////////////////////////////
//  CSWLog.cpp
//  Implementation of the Class CSWLog
//  Created on:      28-����-2013 14:09:51
//  Original author: zhouy
///////////////////////////////////////////////////////////
#include "SWFC.h"
#include "SWLog.h"

DWORD CSWLog::m_dwLevel = SW_LOG_LV_DEBUG;
CSWSemaphore CSWLog::m_cSemaLock;


CSWLog::CSWLog(){

}

CSWLog::~CSWLog(){

}

/**
 * @brief �������ü���
 *
 * @note
 * - ʵ��Ϊ��
 */
DWORD CSWLog::AddRef(){

	return 0;
}

VOID CSWLog::SetLevel(DWORD dwLevel)
{
	m_dwLevel = dwLevel;
}

/**
* @brief �����־
* 
* @param [in] szSrcFile : ��־��Դ�ļ�
* @param [in] szSrcLine : ��־��Դ�ļ��еľ�������
* @param [in] dwLevel   : ��־�ȼ�
* @param [in] szFormat  : ��־�ִ���ʽ
* @param [in] ... : �ɱ�����б�
* @return
* - S_OK : �ɹ�
* - E_FAIL : ʧ��
*/
HRESULT CSWLog::Print(
    const char* szSrcFile,
    int iSrcLine,
    DWORD dwLevel,
    const char* szFormat,
    ...
)
{
	static BOOL fInit = FALSE;

	if (!fInit)
	{
		if (SUCCEEDED(m_cSemaLock.Create(1, 1)))
		{
			fInit = TRUE;
		}
	}
	
	if(dwLevel <= m_dwLevel || SW_LOG_LV_DSP == dwLevel )
	{
		
#if 0
		va_list marker;
		va_start(marker, szFormat);
		
		CSWString strFormat;
		CSWDateTime dt;
		
		if(dwLevel == SW_LOG_LV_NORMAL || dwLevel == SW_LOG_LV_OPERATE)
		{
			strFormat.Format("| %d | [%04d-%02d-%02d %02d:%02d:%02d %03d]%s\n", 
				dwLevel, dt.GetYear(), dt.GetMonth(), dt.GetDay(), 
				dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMSSecond(), 
				szFormat);
		}
		else
		{
			strFormat.Format("| %d | [%04d-%02d-%02d %02d:%02d:%02d %03d][%s:%d]%s\n", 
				dwLevel,  dt.GetYear(), dt.GetMonth(), dt.GetDay(), 
				dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMSSecond(), 
				szSrcFile, iSrcLine, szFormat);
		}

		
		if (SW_LOG_LV_DSP != dwLevel)
		{
			//DSP��ӡ�����������
			vprintf((LPCSTR)strFormat, marker);
		}
		
		swpa_vprint(szSrcFile, dwLevel, (char *)(LPCSTR)strFormat, marker);

		
		va_end(marker);
#else

		CSWString strFormat;
		CSWDateTime dt;
		
		if(dwLevel == SW_LOG_LV_NORMAL || dwLevel == SW_LOG_LV_OPERATE || dwLevel == SW_LOG_LV_DSP)
		{
			strFormat.Format("|%d| [%04d-%02d-%02d %02d:%02d:%02d %03d]%s\n", 
				dwLevel, dt.GetYear(), dt.GetMonth(), dt.GetDay(), 
				dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMSSecond(), 
				szFormat);
		}
		else
		{
			strFormat.Format("|%d| [%04d-%02d-%02d %02d:%02d:%02d %03d][%s:%d]%s\n", 
				dwLevel, dt.GetYear(), dt.GetMonth(), dt.GetDay(), 
				dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMSSecond(), 
				szSrcFile, iSrcLine, szFormat);
		}
		
		if (SUCCEEDED(m_cSemaLock.Pend(1000)))
		{
			va_list marker;
			va_start(marker, szFormat);
			
			static CHAR szOutPut[SW_LOG_MAX_LEN] = {0};
			swpa_memset(szOutPut, 0, sizeof(szOutPut));
			vsnprintf(szOutPut, sizeof(szOutPut)-1, (LPCSTR)strFormat, marker);

			va_end(marker);

			SendLogViaSocket(szOutPut, swpa_strlen(szOutPut) + 1);
			m_cSemaLock.Post();
		}
		else
		{
			//��־���æ����ֱ�Ӵ�ӡ�����ڣ����ⶪʧ
			va_list marker;
			va_start(marker, szFormat);
			if (SW_LOG_LV_DSP != dwLevel)
			{
				//DSP��ӡ�����������
				vprintf((LPCSTR)strFormat, marker);
			}
			va_end(marker);
		}
#endif
	
	}
	
	return S_OK;
}


HRESULT CSWLog::SendLogViaSocket(CHAR * szLog, DWORD dwLogLen)
{
	static CSWTCPSocket cSockSend;
	static BOOL fIsConnected = FALSE;
	static DWORD dwReConCount = 0;
	//static DWORD dwDebugCount = 0;

	BOOL fSendOK = FALSE;

	DWORD dwTrial = 3;

	for (DWORD dwID=0; dwID < dwTrial; dwID++)
	{
		if (!cSockSend.IsValid())
		{
			while (FAILED(cSockSend.Create(TRUE)))
			{
				printf("Err: Create log client socket failed. trying again.\n");
			}
		}
		else
		{
			if (!fIsConnected )
			{
				if (FAILED(cSockSend.Connect("/tmp/log.sock")))
				{
					if (++dwReConCount > 100)
					{
						printf("Err: Connect to log server socket failed.\n");
						dwReConCount = 0;
					}
				}
				else
				{
					fIsConnected = TRUE;
					cSockSend.SetSendTimeout(1000);					
					cSockSend.SetRecvTimeout(1000);

					DWORD dwBufSize = 128*1024;
					if (FAILED(cSockSend.SetRecvBufferSize(dwBufSize)))
					{
						printf("err: set log client send buffer size to %dKB failed\n", dwBufSize>>10);
					}

					//dwDebugCount = 0;
				}
			}

			if (fIsConnected)
			{
				DWORD dwSentLen = 0;
				
				if (SUCCEEDED(cSockSend.Send(&dwLogLen, sizeof(dwLogLen)))
					//&& SUCCEEDED(cSockSend.Send(&dwDebugCount, sizeof(dwDebugCount)))
					&& SUCCEEDED(cSockSend.Send(szLog, dwLogLen, &dwSentLen)))
				{
					if (dwSentLen < dwLogLen)
					{
						printf("err: log client only sent %d bytes\n", dwSentLen);
					}
					//dwDebugCount++;
					fSendOK = TRUE;
					break;
				}
				else
				{
					fIsConnected = FALSE;
					cSockSend.Close();
				}
			}		
		}
	}

	if (!fSendOK)
	{
		//��log����socket��δ��ʼ�����ʱ��ֱ����������ڣ����ⶪʧ��־
		printf("%s", szLog);
	}

	return S_OK;	
}



/**
 * @brief �������ü����������ü���Ϊ0ʱ�ͷŶ���
 *
 * @note
 * - ʵ��Ϊ��
 */
HRESULT CSWLog::Release(){

	return S_OK;
}

