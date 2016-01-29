///////////////////////////////////////////////////////////
//  CSWBaseLinkCtrl.h
//  Implementation of the Class CSWBaseLinkCtrl
//  Created on:      
//  Original author: 
///////////////////////////////////////////////////////////

#if !defined(_SWSYSLINKCTRL_H_)
#define _SWSYSLINKCTRL_H_
#include "SWBaseLinkCtrl.h"
//#define PR_IN_ARM
#ifdef PR_IN_ARM
#include "EPAppDetCtrl.h"
#endif


/**
 * @brief 
 */
class CSWSysLinkCtrl : public CSWBaseLinkCtrl
{
CLASSINFO(CSWSysLinkCtrl,CSWBaseLinkCtrl)

public:
	/**
	 * @brief ���캯��
	 */
	CSWSysLinkCtrl();
	/**
	 * @brief ��������
	 */
	virtual ~CSWSysLinkCtrl();
	
	virtual HRESULT Initialize(VOID);
	VOID Clear();

	/**
	 * @brief ��Ƶ��ʶ���ʼ���ӿ�
	 * 
	 * @param [in] cVideoRecogerParam : ��Ƶ��ʶ�������
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	virtual HRESULT InitVideoRecoger(INIT_VIDEO_RECOGER_PARAM& cVideoRecogerParam);

	/**
	 * @brief ��Ƶ��ʶ�������̬�޸ġ�
	 *
	 * @param [in] cVideoRecogerParam : ��Ƶ��ʶ�������
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	virtual HRESULT ModifyVideoRecogerParam(TRACKER_CFG_PARAM& cVideoRecogerParam);

	/**
	 * @brief �����㷨˽�в�����
	 *
	 * @param [in] pbData : �㷨����
	 * @param [in] iDataSize : ������С
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	virtual HRESULT UpdateAlgParam(unsigned char* pbData, int iDataSize);

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
	virtual HRESULT UpdateModel(unsigned int uiModelId, unsigned char* pbData, int iDataSize);

	/**
	 * @brief ͼƬ��ʶ���ʼ���ӿ�
	 *
	 * @param [in] cPhotoRecogerParam : ͼƬ��ʶ�������
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	virtual HRESULT InitPhotoRecoger(INIT_PHOTO_RECOGER_PARAM& cPhotoRecogerParam);

	virtual HRESULT RegisterCallBack(VOID (*OnDSPAlarm)(PVOID pvParam, INT iInfo), PVOID pvParam);
	virtual HRESULT EncodeJpeg(CSWImage* pImageSrc
		, CSWImage** ppImageJpeg
		, const DWORD& dwCompressRate = 50
		, const DWORD& dwJpegDataType = 0
		, const PBYTE bPhys = NULL
		, const CHAR* szTime = NULL
		, const INT iX = 0
		, const INT iY = 0
		, const BYTE yColor = 0
		, const BYTE uColor = 128
		, const BYTE vColor = 128
		, const SW_RECT* pRect = NULL
        , const BOOL fCrop = FALSE
	);
protected:
	virtual HRESULT DoProcess(CSWImage* pImage, const INT& iTimeOut);
	static int OnResult(void *pContext, int type, void *struct_ptr);
private:
	INT m_hlstJpeg;
	INT m_hlstDspRes;
#ifdef PR_IN_ARM
	CEPAppDetCtrl m_cEPApp;
	INIT_VIDEO_RECOGER_PARAM m_cVideoRecogerParam;
	BOOL m_fInitEPApp;
#endif	
};

#endif 

