/**
* @file SWGBH264 .h
* @brief GBH264ģ��
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-04-01
* @version 1.0
*/


#ifndef __SW_GBH264_TRANSFORM_FILTER_H__
#define __SW_GBH264_TRANSFORM_FILTER_H__

#include "SWBaseFilter.h"
#include "SWClassFactory.h"
#include "SWFC.h"
#include "SWGB28181Parameter.h"
#include "SWMessage.h"

class CSWH264TransformClient;

class CSWGBH264TransformFilter : public CSWBaseFilter,CSWMessage
{
    CLASSINFO(CSWGBH264TransformFilter, CSWBaseFilter);

public:
	/*
	* @brief ���캯��
    *
	*/
    CSWGBH264TransformFilter();
	
	/*
	* @brief ��������
	*
	*/
    virtual ~CSWGBH264TransformFilter();

	/*
	* @brief ��ʼ������
	* @retval S_OK : �ɹ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	*/
	HRESULT Initialize();

	
	/**
	 * @brief ������������ݰ�
	 * @param [in] pdu �������ݰ�
	 * @�ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Receive(CSWObject* obj);

	/*
	* @brief ��ʼ���к���
	* @retval S_OK : �ɹ�
	* @retval E_FAIL: ʧ��
	*/
	virtual HRESULT Run();

	/*
	* @brief ֹͣ���к���
	* @retval S_OK : �ɹ�
	* @retval E_FAIL: ʧ��
	*/
	virtual HRESULT Stop();

    CSWImage* GetFirstImage();
    BOOL  GetCanBeginPlay();

    float GetSpeed(){return m_fCurrentSpeed;}
    BOOL  GetIsHistoryVideo(){return m_fIsHistoryVideo;}
    BOOL  GetIsPause(){return m_fIsPause;}

    DWORD GetH264FramRate(){return m_nH264FramRate;}
    HRESULT ReadNextSecondVideoData(INT seconds = 1);

protected:

    SW_BEGIN_DISP_MAP(CSWGBH264TransformFilter,CSWBaseFilter)
		SW_DISP_METHOD(Initialize, 0)
	SW_END_DISP_MAP();

    static PVOID AcceptLinkProxy(PVOID pvArg);
    HRESULT AcceptLink();


    HRESULT OnGB28181Command(PVOID pvBuffer, INT iSize);

    //��Ϣӳ���
    SW_BEGIN_REMOTE_MESSAGE_MAP(CSWGBH264TransformFilter, CSWMessage)
        SW_REMOTE_MESSAGE_HANDLER(MSG_GB2818_TRANSFORM_COMMAND,OnGB28181Command)
    SW_END_REMOTE_MESSAGE_MAP()

private:	
    BOOL				m_fInited;                  //��ʼ�����
    CSWThread			m_cThreadAccept;            //Filter��չ�߳�,������������
    DWORD				m_dwQueueSize;              //���л���֡����
    DWORD               m_nH264FramRate;            //�������֡��
    CSWList<CSWImage*>	m_lstFrames;                //�������

    CSWSemaphore		m_cSemaSync;                //������ж�д��
    CSWMutex			m_cMutexLock;               //������з�����
    CSWMutex			m_cMutexThread;             //�����߳���
    CSWH264TransformClient* m_pTransformClient;     //�����߳�
    CSWTCPSocket        m_cLocalTcpSock;            //������Socket
    BOOL                m_fNeedReadNextSecond;      //��ȡ��һ��ı��
    float               m_fCurrentSpeed;            //��ǰ�����ٶ�
    BOOL                m_fIsHistoryVideo;          //��ʷ��Ƶ���
    BOOL                m_fIsPause;                 //��ͣ���ű��
    BOOL                m_fBackward;                //���ŷ���
    CSWDateTime         m_dtHistoryFrom;            //�ȴ���ȡ��ʷ��Ƶ�Ŀ�ʼʱ��
    CSWDateTime         m_dtHistoryEnd;             //�ȴ���ȡ��ʷ��Ƶ�Ľ���ʱ��
    BOOL                m_FirstPlay;                //�״β��ű��
};

REGISTER_CLASS(CSWGBH264TransformFilter);
	
class CSWH264TransformClient : public CSWThread
{
    CLASSINFO(CSWH264TransformClient, CSWThread)
public:
    CSWH264TransformClient(const SWPA_SOCKET_T & sock,const void* pFilter)
    {
        m_tcpClient.Attach(sock);
        m_tcpClient.SetSendTimeout(300000);
        m_tcpClient.SetSendBufferSize(32768);

        m_pFilter = (CSWGBH264TransformFilter*)pFilter;
        m_dtLastSendTime = 0;
        
        SW_TRACE_DEBUG("Construct New CSWH264TransformClient Object\n");
    }

    virtual ~CSWH264TransformClient()
    {
        if(m_tcpClient.IsValid())
        {
            m_tcpClient.Close();
        }

        SW_TRACE_DEBUG("Destroy CSWH264TransformClient Object\n");
    }

    CSWTCPSocket & GetTCP(VOID)
    {
        return m_tcpClient;
    }

    virtual HRESULT Run();
private:
    CSWGBH264TransformFilter*   m_pFilter;
    CSWTCPSocket                m_tcpClient;
    SWPA_DATETIME_TM            m_dtTempVarTime;            //��ʱ������ʱ�����
    DWORD                       m_dtLastSendTime;
};

#endif //__SW_GBH264_TRANSFORM_FILTER_H__





