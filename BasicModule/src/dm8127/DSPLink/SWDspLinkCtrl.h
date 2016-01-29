///////////////////////////////////////////////////////////
//  CSWDspLinkCtrl.h
//  Implementation of the Class CSWDspLinkCtrl
//  Created on:      
//  Original author: 
///////////////////////////////////////////////////////////

#if !defined(_SWDSPLINKCTRL_H_)
#define _SWDSPLINKCTRL_H_
#include "SWBaseLinkCtrl.h"

/**
 * @brief 
 */
class CSWDspLinkCtrl : public CSWBaseLinkCtrl
{
CLASSINFO(CSWDspLinkCtrl,CSWBaseLinkCtrl)
public:
	/**
	 * @brief ���캯��
	 */
	CSWDspLinkCtrl();
	/**
	 * @brief ��������
	 */
	virtual ~CSWDspLinkCtrl();
	
	HRESULT Initialize(VOID);
	VOID Clear();
	/**
	 * @brief ��Ƶ��ʶ���ʼ���ӿ�
	 * 
	 * @param [in] cVideoRecogerParam : ��Ƶ��ʶ�������
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	HRESULT InitVideoRecoger(INIT_VIDEO_RECOGER_PARAM& cVideoRecogerParam);

	/**
	 * @brief ͼƬ��ʶ���ʼ���ӿ�
	 *
	 * @param [in] cPhotoRecogerParam : ͼƬ��ʶ�������
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	HRESULT InitPhotoRecoger(INIT_PHOTO_RECOGER_PARAM& cPhotoRecogerParam);
	
	HRESULT RegisterCallBack(VOID (*OnDSPAlarm)(PVOID pvParam, INT iInfo), PVOID pvParam);
protected:
	HRESULT DoProcess(CSWImage* pImage, const INT& iTimeOut);
private:
	CSWFile* m_pDspLink;		
};

#endif 

