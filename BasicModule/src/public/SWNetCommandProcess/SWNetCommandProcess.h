/**
* @file SWNetCommandProcess .h 
* @brief ���������ģ��
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-04-01
* @version 1.0
*/


#ifndef __SW_NET_COMMAND_PROCESS_H__
#define __SW_NET_COMMAND_PROCESS_H__

#include "SWObject.h"
#include "SWClassFactory.h"
#include "SWMessage.h"
#include "SWThread.h"
#include "SWList.h"
#include "tinyxml.h"
#include "SWSocket.h"
#include "SWUDPSocket.h"
#include "SWNetOpt.h"



/**
* @brief ���������ģ��
*
*
*
* 
*/
class CSWNetCommandProcess : public CSWObject, CSWMessage
{
	CLASSINFO(CSWNetCommandProcess, CSWObject);
		

		
public:
	/*
	* @brief ���캯��
	*
	*/
	CSWNetCommandProcess();
	
	/*
	* @brief ��������
	*
	*/
	virtual ~CSWNetCommandProcess();

	/*
	* @brief ��ʼ������
	* @param [in] wProbePort : �豸�������˿�
	* @param [in] wCMDProcessPort : ������������˿�
	* @retval S_OK : �ɹ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	*/
	HRESULT Initialize(const WORD wProbePort, const WORD wCMDProcessPort);

	/*
	* @brief ��ʼ���к���
	* @retval S_OK : �ɹ�
	* @retval E_FAIL: ʧ��
	*/
	HRESULT Run();

	/*
	* @brief ֹͣ���к���
	* @retval S_OK : �ɹ�
	* @retval E_FAIL: ʧ��
	*/
	HRESULT Stop();


protected:

	SW_BEGIN_DISP_MAP(CSWNetCommandProcess,CSWObject)
		//SW_DISP_METHOD(Initialize, 2)
		SW_DISP_METHOD(Run, 0)
		SW_DISP_METHOD(Stop, 0)
	SW_END_DISP_MAP();
	

	/*
	* @brief �豸����߳�������
	* @retval S_OK : �ɹ�
	* @retval E_FAIL: ʧ��
	* @retval E_OUTOFMEMORY: �ڴ治��
	*/
	HRESULT OnProbe(VOID);	

	/*
	* @brief �豸����߳��������ľ�̬������
	*/
	static VOID* OnProbeProxy(VOID* pParam);

	/*
	* @brief ���������߳�����������
	* @retval S_OK : �ɹ�
	* @retval E_FAIL: ʧ��
	* @retval E_OUTOFMEMORY: �ڴ治��
	*/
	HRESULT OnNetCMDConnect(VOID);	

	/*
	* @brief ���������߳������������ľ�̬������
	*/
	static VOID* OnNetCMDConnectProxy(VOID* pParam);

	/*
	* @brief ���������̴߳���������
	* @retval S_OK : �ɹ�
	* @retval E_FAIL: ʧ��
	* @retval E_OUTOFMEMORY: �ڴ治��
	*/
	HRESULT OnNetCMDProcess(SWPA_SOCKET_T sInSock);	

	/*
	* @brief ���������̴߳����������ľ�̬������
	*/
	static VOID* OnNetCMDProcessProxy(VOID* pParam);
	

private:

	/*
	* @brief ���� ��������XML
	* @param [in] pszXMLInBuf : ���յ���XML�����ַ
	* @param [in] dwXMLInSize : ���յ���XML��������С
	* @param [out] ppvXMLOutBuf :��Ӧ��XML��������ַ
	* @param [out] pdwXMLOutSize : ��Ӧ��XML��������С
	* @param [out] pfReboot : �Ƿ���Ҫ�����豸�ı�־
	* @retval S_OK : �ɹ�
	* @retval E_FAIL: ʧ��
	* @retval E_OUTOFMEMORY: �ڴ治��
	*/
	HRESULT ProcessXmlCmd(const CHAR* pszXMLInBuf,  const DWORD dwXMLInSize, PVOID* ppvXMLOutBuf,  DWORD* pdwXMLOutSize, BOOL* pfReboot);

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
	* @brief ��������û�д����ݷ��س������Ե�XML
	* @param [in] pXmlOutputDoc : ��ӦXML���ļ�ָ��
	* @param [in] szCmdName : �������������
	* @retval S_OK : �ɹ�
	* @retval E_FAIL: ʧ��
	* @retval E_OUTOFMEMORY: �ڴ治��
	*/
	HRESULT GenerateNoRetLenErrorXml(TiXmlDocument * pXmlOutputDoc, const CHAR * szCmdName);

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
	HRESULT SendCMD(const DWORD dwCMDID, const CHAR * szValue, const CHAR * szType, const CHAR* szClass, PVOID* ppvRetBuf);

	
	HRESULT SendDomeCameraCustomCMD(const DWORD dwCMDID, const CHAR * szValue, PVOID* ppvRetBuf);

	
	HRESULT SendCustomCMD(const DWORD dwCMDID, const CHAR * szValue, PVOID* ppvRetBuf);

//#if USE_XML_IN_PROBE

	/*
	* @brief �����豸����������ӦXML
	* @param [out] ppszXmlBuf : XML�Ļ����ַ
	* @param [out] pdwXmlLen : XML�ĳ���
	* @retval S_OK : �ɹ�
	* @retval E_FAIL: ʧ��
	* @retval E_OUTOFMEMORY: �ڴ治��
	* @retval E_INVALIDARG: �����Ƿ�
	*/
	HRESULT GenerateProbeReplyXml(CHAR ** ppszXmlBuf, DWORD *pdwXmlLen);

	/*
	* @brief �����豸����XML
	* @param [in] pszMsg : XML�Ļ����ַ
	* @retval S_OK : ����ƥ��ɹ�
	* @retval E_FAIL: ʧ��
	*/
	HRESULT ParseProbeXmlMsg(CHAR * pszMsg);

//#else	
	HRESULT GenerateGetIPCMDReplay(BYTE** pbReplyPack, DWORD* pdwPackLen);

	HRESULT GenerateSetIPCMDReplay(const BYTE* pbMsg, const DWORD dwMsgLen, BYTE** pbReplyPack, DWORD* pdwPackLen);

	
	HRESULT ProcessProtocolCMD(PVOID* ppvOutBuf,	DWORD *pdwOutSize);


	HRESULT ProcessSetParamCMD(const VOID* pvInputBuf,  PVOID* ppvOutBuf,  DWORD *pdwOutSize);

	HRESULT ProcessGetDomeParamCMD(PVOID* ppvOutBuf,  DWORD *pdwOutSize);

	HRESULT ProcessSetDomeParamCMD(const VOID* pvInputBuf,  PVOID* ppvOutBuf,  DWORD *pdwOutSize);

	HRESULT ProcessGetParamCMD(PVOID* ppvOutBuf,  DWORD *pdwOutSize);

    HRESULT ProcessGetHDD_ReportCMD(PVOID* ppvOutBuf,  DWORD *pdwOutSize);
    HRESULT GetConnectedIPInfo(CHAR * RecordLinkIP,CHAR * ImageLinkIP,CHAR *VideoLinkIP);

//#endif
	
	const CHAR* SearchCmdNameByID(const DWORD dwCMDID);

	HRESULT OnGetProtocol(WPARAM wParam, LPARAM lParam);
   // HRESULT OnNULL(WPARAM wParam, LPARAM lParam);//ǰ�ڿ������Խӿ�


	SW_BEGIN_MESSAGE_MAP(CSWNetCommandProcess, CSWMessage)
		SW_MESSAGE_HANDLER(MSG_NETCOMMAND_GET_PROTOCOL, OnGetProtocol)
   //     SW_MESSAGE_HANDLER(MSG_NULL,OnNULL);//ǰ�ڿ������Խӿ�
	SW_END_MESSAGE_MAP();


private:

	enum {
		STATE_READY = 0,
		STATE_RUNNING,
		STATE_STOPPED,
	
		STATE_COUNT
	};
	
	const DWORD 		m_dwGETIP_COMMAND;
	const DWORD 		m_dwSETIP_COMMAND;

	BOOL				m_fInited;
	INT					m_dwState;
	
	WORD				m_wProbePort;
	CSWThread *			m_pProbeThread;

	WORD				m_wCMDProcessPort;
	CSWThread *			m_pCMDServerThread;
	CSWList<CSWThread*>	m_lstCMDProcessThread;
	
};

REGISTER_CLASS(CSWNetCommandProcess)
	
#endif //__SW_NET_COMMAND_PROCESS_H__



