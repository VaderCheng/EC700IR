///////////////////////////////////////////////////////////
//  CSWBaseLinkCtrl.h
//  Implementation of the Class CSWBaseLinkCtrl
//  Created on:      
//  Original author: 
///////////////////////////////////////////////////////////

#if !defined(_SWBASELINKCTRL_H_)
#define _SWBASELINKCTRL_H_

#include "SWObject.h"
#include "SWFile.h"
#include "SWMutex.h"
#include "DspLinkCmd.h"
#include "trackerdef.h"

#define MAX_BUFFER_SIZE  (1024 * 1024)

typedef struct tag_ProcessOneFrameInfo
{
	INT iVideoID;
	INT iDiffTick;
	BOOL fIsCheckLightType;
	DWORD dwIOLevel;
	INT iCarArriveTriggerType;
}
SW_FRAME_OTHER_INFO;

/**
 * @brief 
 */
class CSWBaseLinkCtrl : public CSWObject
{
CLASSINFO(CSWBaseLinkCtrl,CSWObject)
public:
	static CSWBaseLinkCtrl* GetInstance();

public:
	virtual BOOL IsInitialize(VOID){return m_fInitialized;}
	virtual HRESULT Initialize(VOID);
	virtual VOID Clear();
	/**
	 * @brief JPEGѹ���ӿ�
	 * 
	 * @param [in] pImageSrc : ��ѹ����ͼƬ
	 * @param [out] ppImageJpeg : ѹ�����JPEGͼƬ,�ɽӿ����룬ʹ�����ͷš�
	 * @param [in] dwCompressRate : ѹ����
	 * @param [in] dwJpegDataType : JPEGͼƬ��ʽ��0����ʾJpeg���ݶη����Σ�1����ʾJpeg���ݶ�Ϊһ�Σ�
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
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
	 * @brief ��Ƶ��ʶ��֡�ӿ�
	 * 
	 * @param [in] pImage : ��ǰ����֡ͼƬ��
	 * @param [out] pFrameRespond : һ֡���������ķ�����Ϣ��
	 * @param [in] pFrameRecognizeParam: ��֡ʶ��Ĳ�����
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	virtual HRESULT ProcessOneFrame(CSWImage* pImage, PROCESS_EVENT_STRUCT* pProcessEvent, FRAME_RECOGNIZE_PARAM* pFrameRecognizeParam = NULL);

	 /**
	  * @brief ��Ƶ���������ӿ�
	  *
	  * @param [in] pImage : ��ǰ����֡ͼƬ��
	  * @param [in] pDetParam : ��������
	  * @param [out] pFrameRespond : һ֡���������ķ�����Ϣ
	  * @return
	  * - S_OK : �ɹ�
	  * - E_FAIL : ʧ��
	  */
	 virtual HRESULT ProessDetOneFrame(CSWImage* pImage, CAR_DETECT_PARAM* pDetParam, PROCESS_DETECT_INFO* pFrameRespond);


	/**
	 * @brief ͼƬ��ʶ���ʼ���ӿ�
	 *
	 * @param [in] cPhotoRecogerParam : ͼƬ��ʶ�������
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	virtual HRESULT InitPhotoRecoger(INIT_PHOTO_RECOGER_PARAM& cPhotoRecogerParam);
	/**
     * @brief ͼƬ��ʶ��֡�ӿ�
     *
     * @param [in] pImage : ��ǰ����֡ͼƬ��
     * @param [in] pFrameParam : ʶ�������
     * @param [out] pProcessEvent : һ֡���������ķ�����Ϣ��
     * @return
     * - S_OK : �ɹ�
     * - E_FAIL : ʧ��
     */
  virtual HRESULT ProcessPhoto(CSWImage* pImage, PR_PARAM *pFrameParam, PROCESS_EVENT_STRUCT* pProcessEvent);

	virtual DWORD AddRef()
	{
		return 0;
	}
	virtual HRESULT Release()
	{
		return S_OK;
	}
	
	virtual HRESULT RegisterCallBack(VOID (*OnDSPAlarm)(PVOID pvParam, INT iInfo), PVOID pvParam);

protected:
	/**
	 * @brief ���캯��
	 */
	CSWBaseLinkCtrl();
	/**
	 * @brief ��������
	 */
	virtual ~CSWBaseLinkCtrl();
	
	/**
	 * @brief ������
	 * 
	 * @param [in] cCmdId : ����ID
	 * @param [in] pvParam : �������
	 * @param [in] dwParamSize : �����������
	 * @param [in] pvData : ��������
	 * @param [in] dwParamSize : �������ݳ���
	 * @param [out] pvRespond : ���������
	 * @param [int] dwRespondSize : ��������ݳ���
	 * @param [in] dwTimeOut : ��ʱʱ�䣬��λms
	 * @return
	 * - S_OK : �ɹ�
	 * - E_FAIL : ʧ��
	 */
	virtual HRESULT ProcessCmd(
		CSWImage* pImage,
		const INT& iCmdId, 
		PVOID pvParam, const DWORD& dwParamSize, 
		PVOID pvData, const DWORD& dwDataSize,
		PVOID pvRespond, const DWORD& dwRespondSize, 
		const INT& iTimeOut
		);
	
	virtual HRESULT DoProcess(CSWImage* pImage, const INT& iTimeOut);
	virtual HRESULT ConvertImage(CSWImage *pImage, HV_COMPONENT_IMAGE& hvImage);
protected:
	CSWMemory* m_pMemoryData;
	CSWMemory* m_pMemoryRespond;
	CSWMemory* m_pMemoryPoint;
	CSWMemoryFactory* m_pMemoryFactory;
	CSWSemaphore m_cSem;
	CSWSemaphore m_cSemRecog;
	BOOL m_fInitialized;

	BOOL m_fJpegIsWait;	

	CSWImage* m_rgpSmallImage[MAX_EVENT_COUNT];
	CSWImage* m_rgpBinImage[MAX_EVENT_COUNT];
};

#endif 

