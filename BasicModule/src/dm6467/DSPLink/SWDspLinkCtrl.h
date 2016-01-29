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
	 * @brief ��Ƶ��ʶ�������̬�޸ġ�
	 *
	 * @param [in] cVideoRecogerParam : ��Ƶ��ʶ�������
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	HRESULT ModifyVideoRecogerParam(TRACKER_CFG_PARAM& cVideoRecogerParam){return E_NOTIMPL;}

	/**
	 * @brief �����㷨˽�в�����
	 *
	 * @param [in] pbData : �㷨����
	 * @param [in] iDataSize : ������С
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	virtual HRESULT UpdateAlgParam(unsigned char* pbData, int iDataSize){return E_NOTIMPL;}

	/**
	 * @brief ���³������ģ�͡�
	 *
	 * @param [in] uiModelId : ģ��ID
	 * @param [in] pbData : ģ������
	 * @param [in] iDataSize : ģ�����ݴ�С
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	virtual HRESULT UpdateModel(unsigned int uiModelId, unsigned char* pbData, int iDataSize){return E_NOTIMPL;}

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

