#pragma once

#include "FFmpeg.h"
#include "HvDeviceCommDef.h"
#include <list>
using namespace std;

//h264 ÿ֡����
typedef struct _VideoFrameData
{
	PBYTE pbVideoData;
	DWORD dwVideoDataLength;
	DWORD dwVideoType;

	_VideoFrameData()
	{
		pbVideoData = NULL;
		dwVideoDataLength = 0;
		dwVideoType = 0;
	}

	~_VideoFrameData()
	{
		//��дʱ�ͷ��ڴ�
		if(pbVideoData)
		{
			delete[] pbVideoData;
			pbVideoData = NULL;
		}
	}
}VideoFrameData;

typedef list<VideoFrameData*> VideoFrameDataList;

//���ã���H264���������ڴ��С�
//���ã�Save��������д����̡�
//add by zengmx at 2014/9/28
class CVideoMemory
{
public:
	CVideoMemory(void);
	~CVideoMemory(void);

	//��ʼ�����롢������
	static int H264VideoCodeInit()
	{
		av_register_all();
		return 0;
	}

	//����һ֡
	int AddOneFrame(PBYTE pbH264BitStream, int iSize, DWORD dwVideoType, int iWidth, int iHeight);

	//���浽���̣�����ջ���
	int SaveToDisk(const char* szOutputFile, int iFrameNum);

	//��ն��л���
	int ClearVideoFrame();

	//�Ƿ�Ϊ�ջ���
	bool IsVideoNull();
private:
	char m_strFileName[512];
	int m_iWidth;
	int m_iHeight;
	int m_iFrameNum;

	CFFmpeg m_VideoFile;
	VideoFrameDataList m_VideoFrameDataList;
};

