#include "stdafx.h"
#include "VideoMemory.h"


CVideoMemory::CVideoMemory(void)
{
}


CVideoMemory::~CVideoMemory(void)
{
	ClearVideoFrame();
}

//����һ֡
int CVideoMemory::AddOneFrame(PBYTE pbH264BitStream, int iSize, DWORD dwVideoType, int iWidth, int iHeight)
{
	VideoFrameData *pVideoData = new VideoFrameData();
	pVideoData->dwVideoType = dwVideoType;
	pVideoData->dwVideoDataLength = iSize;
	pVideoData->pbVideoData =  new BYTE[pVideoData->dwVideoDataLength];
	memcpy(pVideoData->pbVideoData, pbH264BitStream,  pVideoData->dwVideoDataLength);

	m_VideoFrameDataList.push_back(pVideoData);

	m_iWidth = iWidth;
	m_iHeight = iHeight;

	return 0;
}

//���浽���̣�����ջ���
int CVideoMemory::SaveToDisk(const char* szOutputFile, int iFrameNum)
{
	if(m_VideoFile.m_fOpenFile)
	{
		m_VideoFile.H264VideoSaverClose();
	}
	m_VideoFile.H264VideoSaverOpen(szOutputFile, iFrameNum, m_iWidth, m_iHeight);
	if(!m_VideoFile.m_fOpenFile)
	{
		return -1;
	}

	VideoFrameData *pVideoData = NULL;
	//��˳��д���֡
	while(!m_VideoFrameDataList.empty())
	{
		pVideoData = m_VideoFrameDataList.front();
		m_VideoFrameDataList.pop_front();
		m_VideoFile.H264VideoSaverWirteOneFrame(pVideoData->pbVideoData, pVideoData->dwVideoDataLength, 
			(VIDEO_TYPE_H264_HISTORY_I == pVideoData->dwVideoType || VIDEO_TYPE_H264_NORMAL_I == pVideoData->dwVideoType));

		if(pVideoData->pbVideoData)
		{
			delete[] pVideoData->pbVideoData;
			pVideoData->pbVideoData = NULL;
		}
		delete pVideoData;
		pVideoData = NULL;
	}
	m_VideoFile.H264VideoSaverClose();
	return 0;
}

//��ն��л���
int CVideoMemory::ClearVideoFrame()
{
	VideoFrameData *pVideoData = NULL;
	while(!m_VideoFrameDataList.empty())
	{
		pVideoData = m_VideoFrameDataList.front();
		m_VideoFrameDataList.pop_front();

		delete[] pVideoData->pbVideoData;
		pVideoData->pbVideoData = NULL;
		delete pVideoData;
		pVideoData = NULL;
	}
	return 0;
}

//�Ƿ�Ϊ�ջ���
bool CVideoMemory::IsVideoNull()
{
	return m_VideoFrameDataList.empty();
}
