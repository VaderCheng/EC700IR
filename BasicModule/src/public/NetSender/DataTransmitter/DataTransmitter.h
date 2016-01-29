/**
* @file LinkManager.h 
* @brief ���ݷ���ģ��
*
* ������������������H264��JPEG��RECORD������\n
*
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-04-11
* @version 1.0
*/

#ifndef _DATA_TRANSMITTER_H_
#define _DATA_TRANSMITTER_H_

#include "SWStream.h"
#include "SWList.h"
#include "SWThread.h"
#include "SWMutex.h"
#include "SWDateTime.h"
#include "SWImage.h"
#include "SWCarLeft.h"
#include "SWPosImage.h"
#include "SWRecord.h"
#include "SWNetOpt.h"
#include "SWTransmittingOpt.h"


class CDataTransmitter : public CSWObject
{
	CLASSINFO(CDataTransmitter, CSWObject)
public:
	
	/**
	* @brief ���캯��
	*/
	CDataTransmitter(CSWStream* pStream);

	/**
	* @brief ��������
	*/
	virtual ~CDataTransmitter();

	/**
	* @brief ��ʼ������
	* @param [in] dwDataType : �������������
	* @param [in] sBegin : ����Ŀ�ʼʱ��
	* @param [in] sEnd : ����Ľ���ʱ��
	* @retval S_OK : �ɹ�
	* @retval E_FAIL : ʧ��
	* @retval E_INVALIDARG : �����Ƿ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	*/
	HRESULT Initialize(const DWORD dwDataType, const DWORD dwTransmittingType, const SWPA_TIME sBegin, const SWPA_TIME sEnd);

	/**
	* @brief ���ù�����In Pin ��ID
	* @param [in] dwInPin : Pin ��ID
	* @retval S_OK : �ɹ�
	* @retval E_NOTIMPL : ����δ��ʼ��
	*/
	HRESULT SetPinID(const DWORD dwInPin)
	{
		return (m_fInited && (m_dwPinID = dwInPin)) ? S_OK : E_NOTIMPL;
	};

	/**
	* @brief ��ȡ������In Pin ��ID
	* @retval 0 : ����δ��ʼ��
	* @retval ������: ������Pin ID 
	*/
	DWORD GetPinID()
	{
		return m_fInited ? m_dwPinID : 0;
	};

	/**
	* @brief �������ݰ�
	* @param [in] pObj : �����͵����ݰ�
	* @retval S_OK : �ɹ�
	* @retval E_FAIL : ʧ��
	* @retval E_INVALIDARG : �����Ƿ�
	*/
	HRESULT Send(CSWObject* pObj);

	/**
	* @brief ��ȡ��ǰ״̬
	* @retval ö��ֵ�����ܵ�ȡֵ��:
	* - 0 : STATE_READY, 
	* - 1 : STATE_RUNNING,
	* - 2 : STATE_STOPPED,
	*/
	DWORD GetState() 
	{
		return m_dwState;
	};

	
	/**
	* @brief �ж��Ƿ��Ѿ��������
	* @retval 0 : δ���
	* @retval 1 : �����
	*/
	BOOL Done() 
	{
		return (STATE_STOPPED == GetState());
	};

	/**
	* @brief ��ȡ��������ָ��
	* @retval NULL : ����δ��ʼ��
	* @retval ��Чָ�� : ������Streamָ��
	*/
	CSWStream* GetStream()
	{
		return (m_fInited ? m_pStream : NULL);
	};

	/**
	* @brief ��ȡ��������������
	*/
	DWORD GetTransmittingType()
	{
		return m_fInited ? m_dwTransmittingType : 0;
	};

	/**
	* @brief ���ù�������������
	*/
	HRESULT SetTransmittingType(const DWORD dwTransmittingType)
	{
		if (m_fInited)
		{
			m_dwTransmittingType = dwTransmittingType;
			return S_OK;
		}

		return E_NOTIMPL;
	};

	/**
	* @brief ��ȡ������ʷ���ݵ���ʼʱ���ַ���
	*/
	CHAR* GetBeginTimeString()
	{
		return m_fInited ? m_szTimeBegin : NULL;
	};

	/**
	* @brief ���÷�����ʷ���ݵ���ʼʱ���ַ���
	*/
	HRESULT SetBeginTimeString(const CHAR* szTimeString)
	{
		if (NULL == szTimeString)
		{
			return E_INVALIDARG;
		}
		
		swpa_memset(m_szTimeBegin, 0, sizeof(m_szTimeBegin));
		swpa_strncpy(m_szTimeBegin, szTimeString, sizeof(m_szTimeBegin)-1);
		
		return S_OK;
	};

	/**
	* @brief ��ȡ������ʷ���ݵĽ���ʱ���ַ���
	*/
	CHAR* GetEndTimeString()
	{
		return m_fInited ? m_szTimeEnd: NULL;
	};

	/**
	* @brief ��ȡ������ʷ���ݵĽ���ʱ���ַ���
	*/
	HRESULT SetEndTimeString(const CHAR* szTimeString)
	{
		if (NULL == szTimeString)
		{
			return E_INVALIDARG;
		}

		swpa_memset(m_szTimeEnd, 0, sizeof(m_szTimeEnd));
		swpa_strncpy(m_szTimeEnd, szTimeString, sizeof(m_szTimeEnd)-1);
		
		return S_OK;
	}

	/**
	* @brief ��ȡ������ʷ���ݵĽ���ʱ���ַ���
	*/
	DWORD GetCarID()
	{
		return m_fInited ? m_dwCarID: 0xFFFFFFFF;
	};

	/**
	* @brief ���÷�����ʷ���ݵĽ���ʱ���ַ���
	*/
	VOID SetCarID(const DWORD dwCarID)
	{
		m_dwCarID = dwCarID;
	}

	/**
	* @brief ����Info�Ĵ���ƫ��
	*/
	VOID SetInfoOffset(const DWORD dwOffset)
	{
		m_dwInfoOffset = dwOffset;
	}

	/**
	* @brief ��ȡInfo�Ĵ���ƫ��
	*/
	DWORD GetInfoOffset(VOID)
	{
		return m_dwInfoOffset;
	}

	
	/**
	* @brief ����Data�Ĵ���ƫ��
	*/
	VOID SetDataOffset(const DWORD dwOffset)
	{
		m_dwDataOffset = dwOffset;
	}

	/**
	* @brief ��ȡData�Ĵ���ƫ��
	*/
	DWORD GetDataOffset(VOID)
	{
		return m_dwDataOffset;
	}


	/**
	* @brief ��ȡData�Ĵ���ƫ��
	*/
	DWORD GetDataType(VOID)
	{
		return m_dwDataType;
	}
	
	/**
	* @brief �����Ƿ���Υ��¼��
	*/
	VOID SetIllegalVideoFlag(BOOL fSendIllegalVideo)
	{
		m_fSendIllegalVideo = fSendIllegalVideo;
	}
	
	HRESULT Stop();

	/**
	* @brief ���ý������ģ��������״̬
	*
	* ��һ�����ӶϿ�֮�󣬷���ģ����Զ������״̬��
	* ��״ֻ̬�����һ���ӣ����ڵȴ���λ�������½������ӣ���һ���ӳ�ʱ��ģ����Ϊ�����������Ͽ�������ģ��ֹͣ
	* �ڸ�״̬�У�����ģ����Ȼ���ս���������仺�棻���������½����󣬻�ӻ�����ȡ�����ݷ��ͣ�������г���Ϊ32
	* 
	* @note ��״ֻ̬�Խ��������Ч����ͼƬ����Ƶ�ȷ����޹أ�
	*/
	HRESULT Pending(const BOOL fPend);
	
	/**
	* @brief �жϽ������ģ���Ƿ��ڹ���״̬
	*
	*/
	HRESULT IsPending(BOOL * fPending);

	/**
	* @brief �������ݷ��͵���
	* 
	* һ�����ڻ��ѹ���״̬�еķ���ģ�飬��ʹ���µ�����������
	*
	*/
	HRESULT UpdateStream(CSWStream* pStream);
	
	/**
	* @brief ����������ݵ�SR2�ڴ���������
	* 
	* ��SR2�ڴ汻�����������ڴ棬���ɾ���ú���
	*
	*/
	HRESULT CopyRecordToSRMemory(CSWRecord * pSrcRecord, CSWRecord** ppDestRecord);

protected:

	
	HRESULT BuildImageInfoXml(CSWPosImage* pPosImage, CHAR** pszXml, DWORD* pdwSize);

	HRESULT BuildVideoInfoXml(CSWImage* pVideo, CHAR** pszXml, DWORD* pdwSize);

	HRESULT BuildRecordInfoXml(CSWRecord* pRecord, CHAR** pszXml, DWORD* pdwSize);

	HRESULT BuildTrafficInfoXml(CSWRecord* pRecord, CHAR** pszXmlData, DWORD* pdwSize);

private:

	
	typedef struct __PACKAGE_HEADER
	{
		DWORD dwType;
		DWORD dwInfoSize;
		DWORD dwDataSize;
		
	}_PACKAGE_HEADER;
	
	
	typedef struct __PACKAGE
	{
		_PACKAGE_HEADER 	Header;
		PBYTE				pbInfo;
		PBYTE				pbData;
		
	}_PACKAGE;

	
	
	

	/**
	* @brief �������ݵ�������
	*/
	HRESULT OnSendData();

	/**
	* @brief �������ݵ��������ľ�̬������
	*/
	static VOID* OnSendDataProxy(VOID* pvArg);

	/**
	* @brief ����������
	*/
	HRESULT SendThrobPackage();

	/**
	* @brief ���ͱ�־��ʷ���ݷ�����ɵİ�
	*/
	HRESULT SendHistoryEndPackage();

	/**
	* @brief ����JPEGͼ������
	*
	*/
	HRESULT SendImage(CSWPosImage* pImage);

	/**
	* @brief ����ʶ��������
	* @note �ȴ�������棬��һ���Է��͡�֧�ֶϵ�����������ռ�ý϶��ڴ�
	*/
	HRESULT SendRecord(CSWRecord* pRecord);

	/**
	* @brief ����ʶ����������ǿ��
	* @note ����������棬��η������ݡ���֧�ֶϵ���������ռ�ý϶��ڴ�
	*/
	HRESULT SendRecordEx(CSWRecord* pRecord);

	/**
	* @brief ���ͽ�ͨ�ɼ���Ϣ
	*/
	HRESULT SendTrafficInfo(CSWRecord* pRecord);

	/**
	* @brief ����H264��Ƶ����
	*/
	HRESULT SendVideo(CSWImage* pImage);

	/**
	* @brief �������ݰ���"Info"�ε�һ���ڵ�
	*/
	HRESULT MakeNode(const DWORD dwBlockID, const DWORD dwNodeLen, const PBYTE pbData, PBYTE pbAddr);

	/**
	* @brief �������ݰ�
	*/
	HRESULT SendPackage(_PACKAGE * pPackage);

private:
	#define _TIME_STRING_LEN 32
	
	enum {
		STATE_READY = 0,
		STATE_RUNNING,
		STATE_STOPPED,
	
		STATE_COUNT
	};		
	
	BOOL		m_fInited;
	DWORD		m_dwPinID;
	DWORD		m_dwTransmittingType;
	DWORD		m_dwDataType;
	CSWStream*	m_pStream;
	CSWMutex*				m_pStreamLock;
	DWORD		m_dwState;
	CSWList<CSWObject*>		m_lstData;
	CSWMutex*				m_pListLock;
	CSWThread*				m_pThread;
	CSWSemaphore*			m_pSemaSync;
	DWORD		m_dwMaxHaltTimeMs;

	DWORD		m_dwQueueSize;
	CHAR		m_szTimeBegin[_TIME_STRING_LEN];	
	CHAR		m_szTimeEnd[_TIME_STRING_LEN];
	//SWPA_TIME	m_sTimeBegin;
	//SWPA_TIME	m_sTimeEnd;	
	SWPA_TIME	m_sTimeCur;

	DWORD		m_dwCarID;

	DWORD		m_dwInfoOffset;
	DWORD		m_dwDataOffset;
	
	BOOL 		m_fSendIllegalVideo;

	BOOL		m_fPending;
};

#endif //_DATA_TRANSMITTER_H_


