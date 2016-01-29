/**
* @file SWH264SourceFilter.h 
* @brief H264�ɼ�Filter
* @copyright Signalway All Rights Reserved
* @author zhaopy
* @date 2013-04-02
* @version 1.0
*/

#ifndef _SWH264_SOURCEFILTER_H_
#define _SWH264_SOURCEFILTER_H_

#include "SWBaseFilter.h"
#include "SWFilterStruct.h"
#include "SWMessage.h"
#include "tinyxml.h"

class CSWH264SourceFilter : public CSWBaseFilter, CSWMessage
{
	CLASSINFO(CSWH264SourceFilter, CSWBaseFilter)
public:
	CSWH264SourceFilter();
	virtual ~CSWH264SourceFilter();


	virtual HRESULT Run();
	virtual HRESULT Stop();

public:
	/**
   *@brief ��ʼ��
   */
	HRESULT Initialize(BOOL fEnableOverlay, PVOID pvParam);
	/**
   *@brief ����֡��
   */
	HRESULT SetFps(INT iFps);
	/**
   *@brief ��������
   */
	HRESULT SetBitrate(INT iBitrate);

	/**
   *@brief ��ʾ�˵�
   */
	HRESULT OnInitMenu(WPARAM wParam, LPARAM lParam);
	/**
   *@brief ����H264�ַ�����
   */
	HRESULT OnSetCharOverlay(WPARAM wParam, LPARAM lParam);
	/**
   *@brief ��ȡDM368��������Ϣ��
   */
	HRESULT OnGetHeartbeat(WPARAM wParam, LPARAM lParam);
	/**
   *@brief ͬ��DM368ʱ�䡣
   */
	HRESULT OnSyncTime(WPARAM wParam, LPARAM lParam);
	/**
   *@brief CVBS�˵��󰴼���
   */
	HRESULT OnMenuLeft(WPARAM wParam, LPARAM lParam);
	/**
   *@brief CVBS�˵��Ұ�����
   */
	HRESULT OnMenuRight(WPARAM wParam, LPARAM lParam);
	/**
   *@brief CVBS�˵��ϰ�����
   */
	HRESULT OnMenuUp(WPARAM wParam, LPARAM lParam);
	/**
   *@brief CVBS�˵��°�����
   */
	HRESULT OnMenuDwon(WPARAM wParam, LPARAM lParam);
	/**
   *@brief CVBS�˵�ȷ��������
   */
	HRESULT OnMenuOK(WPARAM wParam, LPARAM lParam);
	/**
   *@brief CVBS�˵�ȡ��������
   */
	HRESULT OnMenuCancel(WPARAM wParam, LPARAM lParam);
	/**
   *@brief ȡDM368�汾��Ϣ��
   */
	HRESULT OnGetVersion(WPARAM wParam, LPARAM lParam);
	/**
   *@brief ����H264������
   */
	HRESULT OnSetH264Param(WPARAM wParam, LPARAM lParam);

	static VOID OnH264Proxy(PVOID pvParam);

protected:	
	// H264�����߳�
	HRESULT OnH264();

private:
	VOID Clear();
	
	HRESULT SetResolution(INT iResolution);

	// ���ݽ���
	HRESULT GetFrameInfo(PBYTE pbInfo, const DWORD& dwInfoSize, PDWORD pdwTick, PDWORD pdwFrameType, PDWORD pdwWidth, PDWORD pdwHeight);
	// ��������
	HRESULT ConnectCtrl();
	// �Ͽ���������
	HRESULT DisconnectCtrl();
	// ��������
	HRESULT SendCtrlCmd(const DWORD& dwId, const DWORD& dwInfoSize, const PBYTE pbInfo, PDWORD pdwRespondSize);
	// ���ɲ˵�XML
	HRESULT CreateMenuXML(TiXmlDocument& cXMLDoc);

	// ����368
	HRESULT ResetDM368();
	// ��ʼ��DM368
	HRESULT InitDM368();

protected:
	//�Զ���ӳ���
	SW_BEGIN_DISP_MAP(CSWH264SourceFilter, CSWBaseFilter)
		SW_DISP_METHOD(SetFps, 1)
		SW_DISP_METHOD(Initialize, 2)
		SW_DISP_METHOD(SetBitrate, 1)
	SW_END_DISP_MAP()
	//��Ϣӳ���
	SW_BEGIN_MESSAGE_MAP(CSWH264SourceFilter, CSWMessage)
	SW_MESSAGE_HANDLER(MSG_DM368_INIT_MENU, OnInitMenu)
	SW_MESSAGE_HANDLER(MSG_DM368_CHAR_OVERLAY, OnSetCharOverlay)
	SW_MESSAGE_HANDLER(MSG_DM368_HEARTBEAT, OnGetHeartbeat)
	SW_MESSAGE_HANDLER(MSG_DM368_SYNC_TIME, OnSyncTime)
	SW_MESSAGE_HANDLER(MSG_DM368_MENU_LEFT, OnMenuLeft)
	SW_MESSAGE_HANDLER(MSG_DM368_MENU_RIGHT, OnMenuRight)
	SW_MESSAGE_HANDLER(MSG_DM368_MENU_UP, OnMenuUp)
	SW_MESSAGE_HANDLER(MSG_DM368_MENU_DWON, OnMenuDwon)
	SW_MESSAGE_HANDLER(MSG_DM368_MENU_OK, OnMenuOK)
	SW_MESSAGE_HANDLER(MSG_DM368_MENU_CANCEL, OnMenuCancel)
	SW_MESSAGE_HANDLER(MSG_DM368_GET_VERSION, OnGetVersion)
	SW_END_MESSAGE_MAP()
private:
	BOOL m_fEnableOverlay;
	int m_iFps;	// ֡�ʣ� -1 �����ı�֡�ʡ�
	int m_iBitrate; // ����
	int m_iWidth; // ͼƬ��
	int m_iHeight;// ͼƬ��

	CSWMemoryFactory* m_pMemoryFactory; 
	BOOL m_fInitialized;
	CSWThread m_cThreadH264;
	CSWTCPSocket m_cSocketCtrl;
	CSWMutex m_cCtrlMutex;
	CSWTCPSocket m_cSocketH264;
	CHAR m_szCtrlIp[32];
	WORD m_wCtrlPort;
	CHAR m_szH264Ip[32];
	WORD m_wH264Port;


	static const INT s_dwSyncTime = 60 * 60 * 1000;	// ͬ��368ʱ������
	DWORD m_dwLastSyncTime;

	H264_OVERLAY_INFO m_cOverlayInfo;
	H264_PARAM m_cH264Param;

	DWORD m_dwResolution;
	

	// cmd
	enum
	{
		CMD_INIT_MENU = 0xFFCC0020,
		CMD_CHAR_OVERLAY = 0xFFCC0010,
		CMD_HEARTBEAT = 0xFFCC0001,
		CMD_SYNC_TIME = 0xFFCC0004,
		CMD_MENU_CTRL = 0xFFCC0021,
		CMD_GET_VERSION = 0xFFCC0002,
		CMD_SET_H264_PARAM = 0xFFCC0030
	};

	// key
	enum
	{
		KEY_LEFT = 75,
		KEY_UP = 72,
		KEY_RIGHT = 77,
		KEY_DWON = 80,
		KEY_OK = 13,
		KEY_CANCEL = 27
	};
};
REGISTER_CLASS(CSWH264SourceFilter)

#endif

