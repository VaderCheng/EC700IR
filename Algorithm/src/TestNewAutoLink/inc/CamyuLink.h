#ifndef _CAMYU_LINK_H_
#define _CAMYU_LINK_H_

#include "CamyuLinkOpt.h"
#include "hvsocket.h"
#include "hvinterface.h"
#include "hvthreadbase.h"
#include "hvvartype.h"


const int MAX_RECV_BUF_SIZE = 1024 * 1024;

class CCamyuLink
{
public:
	CCamyuLink();
	~CCamyuLink();
	// ��ȡͼƬ��ʼ
	HRESULT GetImageStart(const char* pszIP);	
	// ��ȡͼƬ��ʼEx�������˸ú����Ͳ������GetImageStart
	HRESULT GetImageStartEx(const char* pszIP, int iPort);	
	// ��ȡͼƬ����
	HRESULT GetImageStop();
	// ��ȡһ֡ͼƬ
	HRESULT GetImage(char* pcBuf, DWORD32* pdwSize, DWORD32* pdwRefTime);
	// �ڲ�����������ݵ������,��ʱ���øú�������������
	HRESULT ProcessData();
	// ��ȡ��ǰ֡���ͣ�ָ�������������I֡��P֡ 
	int GetCurFrameType();
	//��ȡ��ǰ����
	int GetCurRoadNum();
	HRESULT GetImageWidthAndHeight(DWORD32& dwWidth, DWORD32& dwHeight);
	// ���������ʼ
	HRESULT CtrtCamStart(const char* pszIP);
	// �����������
	HRESULT CtrtCamStop();
	// �������������
	HRESULT SendCommand(DWORD32 dwCmdNo, DWORD32 dwParam1 = 0, DWORD32 dwParam2 = 0);
private:
	HRESULT GetOneFrame(char* pcBuf, DWORD32* pdwSize, bool fMustData);
	HiVideo::ISocket* m_psktData;
	HiVideo::ISocket* m_psktCmd;
	HvCore::IHvStream* m_pstmData;
	HvCore::IHvStream* m_pstmCmd;
	char* m_pcTmpData;
	int m_iCurFrameType; // ���������������
	int m_iRoadNum;		//��ǰ������
	DWORD32 m_dwHeight;
	DWORD32 m_dwWidth;

	
};

#endif
