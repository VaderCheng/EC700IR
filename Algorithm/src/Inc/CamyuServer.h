/*
 *	(C)��Ȩ���� 2010 ������·���Ƽ���չ���޹�˾
 */

/**
* @file		CamyuServer.h
* @version	1.0
* @brief	CCamyuServer��Ķ��壬ʵ������CamyuЭ�鴫��ͼ��֡��
* @author	Shaorg
* @date		2010-11-29
*/

#ifndef _CAMYUSERVER_H_
#define _CAMYUSERVER_H_

#include "ImgGatherer.h"

class CCamyuServer : public CHvThreadBase
{
public:
	CCamyuServer();
	virtual ~CCamyuServer();

public:
	// CHvThreadBase Interface
	virtual const char* GetName()
	{
	    static char szName[] = "CCamyuServer";
	    return szName;
    }
	virtual HRESULT Run(void* pvParam);

public:
    HRESULT PutOneFrame(IMG_FRAME imgFrame);

private:
    HRESULT GetOneFrame(IMG_FRAME* pImgFrame);

private:
    HiVideo::CHvList<IMG_FRAME> m_queImg;
	HV_SEM_HANDLE m_hSemQueCount;
	HV_SEM_HANDLE m_hSemQueCtrl;
};

#endif
