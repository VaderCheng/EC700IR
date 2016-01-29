/**
* @file LinkManager.h 
* @brief ���ӹ���ģ��
*
* �����豸�����ӣ������������Ӻͱ�������\n
*
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-04-11
* @version 1.0
*/

#ifndef _LINK_MANAGER_H_
#define _LINK_MANAGER_H_

#include "SWStream.h"
#include "SWTCPSocket.h"
#include "SWThread.h"


typedef HRESULT (*PF_LINK_CALLBACK)(CSWStream * pStream, PVOID pvArg, PVOID pvHandshakeBuf);


class CLinkManager : public CSWObject
{
	CLASSINFO(CLinkManager, CSWObject)
public:

	/*
	* @brief ���캯��
	*/
	CLinkManager();

	/*
	* @brief ��������
	*/
	virtual ~CLinkManager();

	/*
	* @brief ��ʼ������
	* @param [in] wPassiveLinkPort : �������ӵĶ˿�
	* @param [in] wActiveLinkPort : �������ӵĶ˿�
	* @param [in] pArrayDstIp : �������ӵ�IP
	* @param [in] fSafeSaverEnabled : �Ƿ��пɿ��Դ洢
	* @retval S_OK : �ɹ�
	* @retval E_FAIL : ʧ��
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	*/
	HRESULT Initialize(const WORD wLocalPort, const WORD wDstPort,  const CHAR * szDstIp, const BOOL fSafeSaverEnabled);
	
	/*
	* @brief ��ȡ�������ӹ���ģ��ʵ��
	* @param [in] wPassiveLinkPort : �������ӵĶ˿�
	* @param [in] wActiveLinkPort : �������ӵĶ˿�
	* @param [in] pArrayDstIp : �������ӵ�IP�б�
	* @param [in] dwDstIpCount : �������ӵ�IP��Ŀ
	* @retval NULL : ��ȡʧ��
	* @retval CLinkManager��ʵ��ָ��
	*/
	//static CLinkManager* GetInstance(const WORD wPassiveLinkPort, const WORD wActiveLinkPort,  const CHAR ** pArrayDstIp, const DWORD dwDstIpCount);

	/*
	* @brief �ͷ�һ������
	* @param [in] pStream : ������ӹ�������ָ��
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	*/
	HRESULT ReleaseLink(CSWStream * pStream);

	/*
	* @brief �������Ӵ����ɹ��Ļص�����
	* @param [in] pfCallback : �ص�����ָ��
	* @param [in] pvArg : �ص���������ָ��
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	*/
	HRESULT SetLinkCallback(PF_LINK_CALLBACK pfCallback, PVOID pvArg);

	/*
	* @brief ������������
	* @retval S_OK : �ɹ�
	* @retval E_FAIL : ʧ��
	* @retval E_INVALIDARG : �����Ƿ�
	*/
	HRESULT StartActiveLink(VOID);

	/*
	* @brief ֹͣ��������
	* @retval S_OK : �ɹ�
	* @retval E_FAIL : ʧ��
	* @retval E_INVALIDARG : �����Ƿ�
	*/
	HRESULT StopActiveLink(VOID);

	/*
	* @brief ������������
	* @retval S_OK : �ɹ�
	* @retval E_FAIL : ʧ��
	* @retval E_INVALIDARG : �����Ƿ�
	*/
	HRESULT StartPassiveLink(VOID);

	/*
	* @brief ֹͣ��������
	* @retval S_OK : �ɹ�
	* @retval E_FAIL : ʧ��
	* @retval E_INVALIDARG : �����Ƿ�
	*/
	HRESULT StopPassiveLink(VOID);

	/*
	* @brief �������������
	* @param [in] dwMaxCount : ������
	* @retval S_OK : �ɹ�
	* @retval E_INVALIDARG : �����Ƿ�
	*/
	HRESULT SetMaxLinkCount(const DWORD dwMaxCount);

	/*
	* @brief ��ȡ���������
	* @retval �Ǹ����� : ���������
	*/
	DWORD GetMaxLinkCount(VOID);

	/*
	* @brief ��ȡ��ǰ������
	* @retval �Ǹ����� : ���������
	*/
	DWORD GetLinkCount(VOID);

	/*
	* @brief �����û���¼ģʽ
	* @retval 
	*/
	HRESULT EnableUserLogin(VOID);

	/*
	* @brief �ر��û���¼ģʽ
	* @retval 
	*/
	HRESULT DisableUserLogin(VOID);

	/*
	* @brief ���������֤
	* @retval 
	*/
	HRESULT EnableIdentityVerification(VOID);

	/*
	* @brief �ر������֤
	* @retval 
	*/
	HRESULT DisableIdentityVerification(VOID);
	

	/*
	* @brief ȡ����״̬�ַ���
	* @retval 
	*/
	CSWString GetConnectInfo(VOID);

	/*
	* @brief ���·������ݵ���������������ʱ�Ͽ�����������������
	* @retval 
	*/
	HRESULT UpdateStream(CSWStream* pOldStream, CSWStream** ppNewStream);

	


private:	

	
	HRESULT SendHandshakeMsg();

	/*
	* @brief �������ӵ������˻ص�������ģ��
	* @param [in] pTCPSocket : LinkManager������һ��TCP����
	* @retval S_OK : �ɹ�
	* @retval E_FAIL : ʧ��	
	* @retval E_OUTOFMEMORY : �ڴ治��
	*/
	HRESULT PushLink(CSWTCPSocket* pTCPSocket);

	/*
	* @brief ��������������
	* @retval S_OK : �ɹ�
	* @retval E_FAIL : ʧ��	
	* @retval E_OUTOFMEMORY : �ڴ治��
	*/
	HRESULT OnActiveLink();

	/*
	* @brief ���������������ľ�̬������
	*/
	static VOID* OnActiveLinkProxy(VOID * pvArg);

	/*
	* @brief ��������������
	* @retval S_OK : �ɹ�
	* @retval E_FAIL : ʧ��	
	* @retval E_OUTOFMEMORY : �ڴ治��
	*/
	HRESULT OnPassiveLink();

	/*
	* @brief ���������������ľ�̬������
	*/
	static VOID* OnPassiveLinkProxy(VOID * pvArg);

	/*
	* @brief �ж��豸�Ƿ��Ѿ����ض�IP���ض��˿�����
	* @param [in] szIp : IP
	* @param [in] wPort : �˿�
	* @retval TRUE : ������
	* @retval FALSE : δ����
	*/
	BOOL IsConnected(const CHAR* szIp, const WORD wPort);

	
	HRESULT GetDefaultHandshakeXml(PBYTE* pbBuf, DWORD* pdwLen);

	
	HRESULT ActiveLinkHandshake(CSWTCPSocket * pSockClient);

private:
	
#define _IP_LEN 32

	enum {
		STATE_READY = 0,
		STATE_RUNNING,
		STATE_STOPPED,
	
		STATE_COUNT
	};
	
	typedef struct __LINK_INFO
	{
		CHAR 		szIp[_IP_LEN];
		WORD		wPort;
		CSWStream* 	pStream;
		BOOL		fIsActiveLink;
		CSWStream* 	pPrevStream;
		BOOL		fIsRealTimeLink;
	}_LINK_INFO;

	typedef struct __CALLBACK_INFO
	{
		PF_LINK_CALLBACK 	pfCallback;
		PVOID				pvCallbackArg;
	}_CALLBACK_INFO;
	
	BOOL		m_fInited;
	DWORD		m_dwActiveLinkState;
	DWORD		m_dwPassiveLinkState;
	
	BOOL 		m_fNeedVerification;
	BOOL		m_fUserLogin;

	WORD		m_wLocalPort;
	WORD		m_wDstPort;
	BOOL		m_fSafeSaver;
	CHAR*		m_pszDstIp;
	DWORD		m_dwMaxLinkCount;
	DWORD		m_dwLinkCount;
	
	CSWList<_LINK_INFO*>		m_lstLinks;	
	CSWList<_CALLBACK_INFO*>	m_lstCallbacks;
	CSWMutex*	m_pCallbackLock;
	
	CSWThread*	m_pActiveLinkThread;
	CSWThread*	m_pPassiveLinkThread;	

	// zhaopy
	CSWMutex m_cMutex;
	CSWString m_strConnectInfo;
};


#endif //_LINK_MANAGER_H_

