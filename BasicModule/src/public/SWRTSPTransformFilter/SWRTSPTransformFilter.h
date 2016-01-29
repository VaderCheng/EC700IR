/**
* @file SWRTSP .h 
* @brief RTSPģ��
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-04-01
* @version 1.0
*/


#ifndef __SW_RTSP_TRANSFORM_FILTER_H__
#define __SW_RTSP_TRANSFORM_FILTER_H__

#include "SWBaseFilter.h"
#include "SWClassFactory.h"

#include "SWOnvifRtspParameter.h"
/**
* @brief RTSP ģ��
*
*
*
* 
*/
typedef struct _STAT_INFO { 	//statistics infomation
	DWORD				dwOriginBitrate;			//ԭʼ�û����õ�����
	DWORD				dwLastBitrate;			//�ϴγ����޸ĵ�����
	DWORD				dwOriginIFrameInterval; 	//ԭʼ�û����õ�I֡���
	DWORD				dwLastIFrameInterval;		//�ϴ��޸ĵ�I֡���
	DWORD				dwLastReduceTick;			//���һ�μ�С����ʱ��TICK��
	DWORD				dwLastLostTick;
	DWORD				dwLostCounter;			//��ǰ���ʶ���������
	DWORD				dwLastAscendInterval;		//��һ�����Ĵ������
	DWORD				dwNextAscendTick;
	DWORD				dwLastAscendBitrate;
	DWORD				dwLastLostBitrate;			//��һ�ζ���ʱ������
}STAT_INFO;



class CSWRTSPTransformFilter : public CSWBaseFilter
{
	CLASSINFO(CSWRTSPTransformFilter, CSWBaseFilter);

public:
	/*
	* @brief ���캯��
	*
	*/
	CSWRTSPTransformFilter();
	
	/*
	* @brief ��������
	*
	*/
	virtual ~CSWRTSPTransformFilter();

	/*
	* @brief ��ʼ������
	* @param [in] iChannelId ����H264����ͨ����
	* @retval S_OK : �ɹ�
	* @retval E_OUTOFMEMORY : �ڴ治��
	*/
	HRESULT Initialize(INT iChannelId);

	
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


private:
	/**
	   @brief �������ʣ�����I ֡���
	   @param [IN] pStatusHeader ״̬��Ϣͷ��
	   @param [IN] pStatusSock ��װsocket
	 */
	HRESULT ReduceStreamBitrate(H264_STATUS_HEADER *pStatusHeader, CSWTCPSocket *pStatusSock);

	/**
	   @brief ������ʣ�����I ֡���
	   @param [IN] pStatusHeader ״̬��Ϣͷ��
	   @param [IN] pStatusSock ��װsocket
	 */
	HRESULT AscendStreamBitrate(H264_STATUS_HEADER *pStatusHeader, CSWTCPSocket *pStatusSock);

	/**
	 @brief ����������Ϣ
	 @param [IN] pStatusHeader ״̬��Ϣͷ��
	 @param [IN] pStatusSock ��װsocket
	 */
	HRESULT UpdateConnectionInfo(H264_STATUS_HEADER *pStatusHeader, CSWTCPSocket *pStatusSock);

	/**
	   @brief ��֤RTSP�û�
	   @param [IN] pStatusHeader ״̬��Ϣͷ��
	   @param [IN] pStatusSock ��װsocket
	 */
	HRESULT Authenticate(H264_STATUS_HEADER *pStatusHeader, CSWTCPSocket *pStatusSock);

protected:

	SW_BEGIN_DISP_MAP(CSWRTSPTransformFilter,CSWBaseFilter)
		SW_DISP_METHOD(Initialize, 1)
	SW_END_DISP_MAP();

	static PVOID OnSendDataProxy(PVOID pvArg);
	static PVOID OnGetConnStatusProxy(PVOID pvArg);
	HRESULT OnSendData();
	HRESULT OnGetConnStatus();

private:

	BOOL				m_fInited;

	DWORD				m_dwQueueSize;
	CSWSemaphore		m_cSemaSync;
	CSWThread			m_cThreadSend;
	CSWThread           m_cThreadGetConnStatus;
	CSWMutex			m_cMutexLock;

	CSWList<CSWImage*>	m_lstFrames;

	INT m_iChannelId;	//H264ͨ����

	//��������ͳ����Ϣ
	STAT_INFO m_cStatInfo[2];
};

REGISTER_CLASS(CSWRTSPTransformFilter);
	
#endif //__SW_RTSP_TRANSFORM_FILTER_H__





