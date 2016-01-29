#ifndef __SW_LPR_APPLICATION_H__
#define __SW_LPR_APPLICATION_H__
#include "SWLPRParameter.h"
#include "SWMessage.h"
#include "SWBaseFilter.h"
#include "SWBaseGraph.h"

//Filterģ���ʼ������
#define BEGIN_INITIALIZE_OBJECT()       virtual HRESULT OnObjectInitialize(CSWObject *pObject){if(0){}
#define ON_INITIALIZE_OBJECT(cls, func) else if(!swpa_strcmp(#cls, pObject->Name())){return func(pObject);}
#define END_INITIALIZE_OBJECT()         return S_OK;}

class CSWLPRApplication : public CSWApplication, CSWBaseGraph, CSWMessage
{
	CLASSINFO(CSWLPRApplication, CSWApplication)
public:
	CSWLPRApplication();
	virtual ~CSWLPRApplication();
	virtual HRESULT InitInstance(const WORD wArgc, const CHAR** szArgv);
	virtual HRESULT ReleaseInstance();
	virtual HRESULT Run();
  virtual HRESULT OnCommand(CSWBaseFilter* pFilter, DWORD dwID, DWORD wParam, DWORD lParam);
  CSWLPRParameter& GetParam(VOID);
  CSWObject * CreateObjectByVariable(LPCSTR szVar, LPCSTR szDefaultClassName="");
protected:
	static  INT OnMSGCompare(PVOID key1, PVOID key2);
	HRESULT InitVariant(VOID);
	HRESULT InitParameter(VOID);
	HRESULT InitObject(VOID);
	HRESULT InitFilter(VOID);



	static  VOID OnDSPAlarm(PVOID pvParam, INT iInfo);
	static  PVOID OnReleaseInstance(PVOID pvParam);

	// reset info
	HRESULT WriteResetInfo();
	INT GetResetCount();
	CSWString GetResetInfo();

	HRESULT GetRedRectInfo(SW_RECT* pRect, INT& iRectCount);
	
	HRESULT GetGreenRectInfo(SW_RECT* pRect, INT& iRectCount);

	// AT88 check
	HRESULT InitAT88();
	HRESULT VerifyCrypt();

	// ��������
	VOID SetBootCountInfo();

	//��ȡ����ϵͳ�İ汾��
	HRESULT ReadBackupSystemVersion(CHAR* pszVersion);

	//��ȡǰ���ͺ�
	HRESULT ReadSensorType(INT *piSensorType);

	//����ǰ���ͺ�
	HRESULT SaveSensorType(INT iSensorType);

	//��鸴λ����
	HRESULT CheckRebootEvent(void);

	//��ȡ�豸����
	HRESULT ReadCustomizedDevName(LPSTR pszName);

	//�����豸����
	HRESULT SaveCustomizedDevName(LPCSTR pszName);

	//�ж��豸�����Ƿ�Ϊ����
	BOOL IsVenus(VOID);

	//�ж��豸�����Ƿ�Ϊľ��
	BOOL IsJupiter(VOID);

protected:
	//Object��ʼ������
	HRESULT OnCameralControlMSG(CSWObject *pObject);
	HRESULT OnCameraControl2AMSG(CSWObject *pObject);
	HRESULT OnDomeCameraControlMSG(CSWObject *pObject);
	HRESULT OnNetCommandProcessInitialize(CSWObject *pObject);
	HRESULT OnTriggerOutInitialize(CSWObject *pObject);
	//����Filter
	CSWBaseFilter *CreateFilter(TiXmlElement *element);
	//Filter�ĳ�ʼ������
	HRESULT OnH264HDDTransformFilterInitialize(CSWObject *pObject);
	HRESULT OnH264NetRenderFilterInitialize(CSWObject *pObject);
	HRESULT OnResultHDDTransformFilterInitialize(CSWObject *pObject);
	HRESULT OnResultNetRenderFilterInitialize(CSWObject *pObject);	
	HRESULT OnJPEGNetRenderFilterInitialize(CSWObject *pObject);	
	HRESULT OnNetSourceFilterInitialize(CSWObject *pObject);
	HRESULT OnH264SourceFilterInitialize(CSWObject *pObject);
	HRESULT OnCentaurusH264EncodeFilterInitialize(CSWObject *pObject);
	HRESULT OnCentaurusJpegEncodeFilterInitialize(CSWObject *pObject);
	HRESULT OnRecognizeTransformFilterInitialize(CSWObject *pObject);
    HRESULT OnRecognizeTransformTollGateFilterInitialize(CSWObject *pObject);
	HRESULT OnRecognizeTransformPTFilterInitialize(CSWObject *pObject);
	HRESULT OnRecognizeTransformEPFilterInitialize(CSWObject *pObject);
	HRESULT OnRecognizeTGTransformFilterInitialize(CSWObject *pObject);
	HRESULT OnJPEGEncodeTransformFilterInitialize(CSWObject *pObject);
	HRESULT OnVPIFSourceFilterInitialize(CSWObject *pObject);
	HRESULT OnMatchTransformFilterInitialize(CSWObject *pObject);	
	HRESULT OnAutoControlRenderFilterInitialize(CSWObject *pObject);
	HRESULT OnResultFilterInitialize(CSWObject *pObject);
	HRESULT OnEPoliceEventCheckerTransformFilter(CSWObject *pObject);
	HRESULT OnONVIFControlInitialize(CSWObject *pObject);
	HRESULT OnRTSPTransformFilterInitialize(CSWObject *pObject);
	HRESULT OnJPEGOverlayFilterInitialize(CSWObject *pObject);
	HRESULT OnH264OverlayFilterInitialize(CSWObject *pObject);	
	HRESULT OnH264SecondOverlayFilterInitialize(CSWObject *pObject);
    HRESULT OnGBH264TransformFilterInitialize(CSWObject *pObject);
    HRESULT OnH264RTPFilterInitialize(CSWObject *pObject);
    HRESULT OnJPEGCachTransformFilterInitialize(CSWObject *pObject);
    HRESULT OnCentaurusSourceFilterInitialize(CSWObject *pObject);
	HRESULT OnCSWResultFtpRenderFilterInitialize(CSWObject *pObject);
    HRESULT OnCSWDomeRockerControlMSGInitialize(CSWObject *pObject);
	HRESULT OnRecognizeTransformPPFilterInitialize(CSWObject *pObject);
    HRESULT OnCSWH264QueueRenderFilterInitialize(CSWObject *pObject);
    HRESULT OnCSWResultVideoTransformFilterInitialize(CSWObject *pObject);
	HRESULT OnMD5EncryptFilterInitialize(CSWObject *pObject);
	HRESULT OnCSWExtendDataTransformFilterInitialize(CSWObject *pObject);

	//��ʼ������ӳ��
	BEGIN_INITIALIZE_OBJECT()
		ON_INITIALIZE_OBJECT(CSWCameralControlMSG,        OnCameralControlMSG)
		ON_INITIALIZE_OBJECT(CSWCameraControl2AMSG,		  OnCameraControl2AMSG)
		ON_INITIALIZE_OBJECT(CSWDomeCameraControlMSG,     OnDomeCameraControlMSG)
		ON_INITIALIZE_OBJECT(CSWNetCommandProcess,        OnNetCommandProcessInitialize)
		ON_INITIALIZE_OBJECT(CSWTriggerOut,               OnTriggerOutInitialize)
		ON_INITIALIZE_OBJECT(CSWH264HDDTransformFilter,   OnH264HDDTransformFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWH264NetRenderFilter,   	  OnH264NetRenderFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWResultHDDTransformFilter, OnResultHDDTransformFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWResultNetRenderFilter,    OnResultNetRenderFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWJPEGNetRenderFilter,      OnJPEGNetRenderFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWNetSourceFilter,          OnNetSourceFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWH264SourceFilter,         OnH264SourceFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWCentaurusH264EncodeFilter,OnCentaurusH264EncodeFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWCentaurusJpegEncodeFilter,OnCentaurusJpegEncodeFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWRecognizeTransformFilter, OnRecognizeTransformFilterInitialize)
        ON_INITIALIZE_OBJECT(CSWRecognizeTransformTollGateFilter, OnRecognizeTransformTollGateFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWRecognizeTransformPTFilter, OnRecognizeTransformPTFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWRecognizeTransformEPFilter, OnRecognizeTransformEPFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWRecognizeTGTransformFilter, OnRecognizeTGTransformFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWJPEGEncodeTransformFilter,OnJPEGEncodeTransformFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWVPIFSourceFilter,         OnVPIFSourceFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWMatchTransformFilter,     OnMatchTransformFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWAutoControlRenderFilter,  OnAutoControlRenderFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWResultFilter,             OnResultFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWEPoliceEventCheckerTransformFilter, OnEPoliceEventCheckerTransformFilter)
		ON_INITIALIZE_OBJECT(CSWONVIFControl,             OnONVIFControlInitialize)
		ON_INITIALIZE_OBJECT(CSWRTSPTransformFilter,      OnRTSPTransformFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWJPEGOverlayFilter,         OnJPEGOverlayFilterInitialize)
        ON_INITIALIZE_OBJECT(CSWH264OverlayFilter,         OnH264OverlayFilterInitialize)
        ON_INITIALIZE_OBJECT(CSWH264SecondOverlayFilter,	OnH264SecondOverlayFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWGBH264TransformFilter,    OnGBH264TransformFilterInitialize)
        ON_INITIALIZE_OBJECT(CSWH264Filter,    OnH264RTPFilterInitialize)
        ON_INITIALIZE_OBJECT(CSWJPEGCachTransformFilter,    OnJPEGCachTransformFilterInitialize)
        ON_INITIALIZE_OBJECT(CSWCentaurusSourceFilter,    OnCentaurusSourceFilterInitialize)
        ON_INITIALIZE_OBJECT(CSWResultFtpRenderFilter,    OnCSWResultFtpRenderFilterInitialize)
        ON_INITIALIZE_OBJECT(CSWDomeRockerControlMSG,    OnCSWDomeRockerControlMSGInitialize)
		ON_INITIALIZE_OBJECT(CSWRecognizeTransformPPFilter, OnRecognizeTransformPPFilterInitialize)
		ON_INITIALIZE_OBJECT(CSWH264QueueRenderFilter,    OnCSWH264QueueRenderFilterInitialize)
        ON_INITIALIZE_OBJECT(CSWResultVideoTransformFilter,    OnCSWResultVideoTransformFilterInitialize)
        ON_INITIALIZE_OBJECT(CSWMD5EncryptFilter,    		OnMD5EncryptFilterInitialize)
        ON_INITIALIZE_OBJECT(CSWExtendDataTransformFilter,OnCSWExtendDataTransformFilterInitialize)
	END_INITIALIZE_OBJECT()
	
	//��Ϣ������
	/**
	 *@brief ����ʱ��
	 *@param [in] wParam SWPA_DATETIME_TM*ָ��
	 *@param [in] lParam ��
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnSetTime(WPARAM wParam, LPARAM lParam);
	/**
	 *@brief ����ʱ��
	 *@param [in]  wParam ��
	 *@param [out] lParam SWPA_DATETIME_TM*ָ��
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnGetTime(WPARAM wParam, LPARAM lParam);
	/**
	 *@brief ����IP��ַ
	 *@param [in] wParam ����Ϊ3��Ԫ�ص�ָ��,array[0]=MAC��ַ;array[1]=IP��ַ;array[2]=��������;array[3]=����
	 *@param [in] lParam ��
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnSetIP(WPARAM wParam, LPARAM lParam);	
	/**
	 *@brief ����IP��ַ
	 *@param [in] wParam ��
	 *@param [out] lParam ����Ϊ3��Ԫ�ص�ָ��,array[0]=MAC��ַ;array[1]=IP��ַ;array[2]=��������;array[3]=����
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnGetIP(WPARAM wParam, LPARAM lParam);
	/**
	 *@brief ����XML
	 *@param [in] wParam XML����ָ��
	 *@param [in] lParam ��
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */	
	HRESULT OnSetXML(WPARAM wParam, LPARAM lParam);
	/**
	 *@brief ��ȡXML
	 *@param [in]   wParam ��
	 *@param [out]  lParam CSWString*ָ�룬����xml����
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */	
	HRESULT OnGetXML(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ȡ�ù���ģʽ����
	 *@param [in] wParam ��
	 *@param [out] lParam��int* ����,���湤��ģʽ����
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */
	HRESULT OnGetWorkModeCount(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ��ȡ�豸����ģʽ
	 *@param [out] wParam ����ģʽ����ָ��,int* ����
	 *@param [out] lParam ����ģʽ�ַ�����CSWString* ����
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */
	HRESULT OnGetWorkMode(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ��λ�豸
	 *@param [in] wParam ����
	 *@param [in] lParam ����
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */
	HRESULT OnResetDevice(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief �ָ�����Ĭ��
	 *@param [in] wParam ����
	 *@param [in] lParam ����
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */
	HRESULT OnRestoreDefault(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief �ָ�����Ĭ��
	 *@param [in] wParam ����
	 *@param [in] lParam ����
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */
	HRESULT OnRestoreFactory(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief �����ַ�����
	 */
	//HRESULT OnSetCharacterEnable(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ��ȡ�Ƿ��ַ�����
	 */
	//HRESULT OnGetCharacterEnable(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ��ȡ�豸����
	 *@param [in] wParam ��
	 *@param [out] lParam,CSWString*ָ�룬�����豸����
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnGetHvName(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ��ȡ�豸id
	 *@param [in] wParam ��
	 *@param [out] lParam, INT*ָ�룬�����豸id
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnGetHvID(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ��ȡ��Ʒ����
	 *@param [in] wParam ��
	 *@param [out] lParam,CSWString*ָ�룬�����Ʒ����
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnGetProductName(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief �����Ƶ����
	 *@param [in] wParam ��
	 *@param [out] lParam, INT*ָ�룬������Ƶ����
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnGetVideoCount(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ȡ�ð汾
	 *@param [in] wParam ��
	 *@param [out] lParam, INT*ָ�룬����汾��
	 */
	HRESULT OnGetVersion(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ȡ�ð汾�ַ���
	 *@param [in] wParam ��
	 *@param [out] lParam, CSWString*ָ�룬����汾���ַ���
	 */
	HRESULT OnGetVersionString(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ȡ�����ӵ�IP��ַ
	 */
	HRESULT OnGetConnectedIP(WPARAM wParam, LPARAM lParam);
	 
	/**
	 *@brief Ӳ�̷���
	 *@param wParam [in] Ҫ�������豸�ַ���ָ�룬��"/dev/sda"
	 *@param lParam [in] ��Ҫ���ַ�����������ÿ��������Сһ��
	 */
	HRESULT OnHDDFDisk(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief Ӳ�̼��
	 *@param [in] wParam ���ģʽ,0:�������,1�򵥼��
	 *@param [in] lParam ��
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnHDDCheck(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ��ȡӲ��״̬
	 *@param [in] wParam ��
	 *@param [out] lParam,CSWString*ָ�룬����Ӳ��״̬
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	
	HRESULT OnHDDStatus(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ��ø�λ����
	 *@param [in] wParam ��
	 *@param [out] lParam,INT*ָ�룬���渴λ����
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */	
	HRESULT OnResetCount(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ��ø�λ��¼
	 *@param [in] wParam ���渴λ��¼������,char*ָ��
	 *@param [in] lParam ���渴λ��¼��������С
	 */
	HRESULT OnResetReport(WPARAM wParam, LPARAM lParam);
	/**
	 *@brief ��ø�λģʽ
	 *@param [in] wParam ��
	 *@param [out] lParam,INT*ָ��
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */	
	HRESULT OnResetMode(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ȡ���豸����
	 *@param [in] wParam ��
	 *@param [out] lParam,CSWString*ָ�룬�����豸����
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnGetDevType(WPARAM wParam, LPARAM lParam);
	
	/**
	 *@brief ȡ��CPU�¶�
	 *@param [in] wParam ��
	 *@param [out] lParam,INT*ָ�룬����CPU�¶�
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnGetCpuTemperature(WPARAM wParam, LPARAM lParam);

	/**
	 *@brief ����״̬��ÿ��FILTER��ʱΪ״̬����APP
	 *@param [in] wParam ״̬�ַ�����
	 *@param [out] lParam 
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnUpdateStatus(WPARAM wParam, LPARAM lParam);

	/**
	 *@brief ����״̬��ÿ��FILTER��ʱΪ״̬����APP
	 *@param [in] wParam ��
	 *@param [out] lParam,CHAR*ָ�룬״̬�ַ���.
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnGetRunStatus(WPARAM wParam, LPARAM lParam);

	/**
	 *@brief �״ﴥ��ץ���¼�
	 *@param [in] wParam ��Ȧ��
	 *@param [in] lParam ����
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */
	HRESULT OnRadarTrigger(WPARAM wParam, LPARAM lParam);

	/**
	 *@brief ��Ȧ��״̬
	 *@param [in] wParam ��Ȧ��״̬��ÿһλ����ÿ����Ȧ��0����������1�����쳣
	 *@param [in] lParam ��Ȧ�Ĵ�����־��ÿһλ����ÿ����Ȧ��0����������,1��������
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnCoilStatus(WPARAM wParam, LPARAM lParam);
	 
	/**
	 *@brief ��Ȧ����ץ���¼�
	 *@param [in] wParam ��Ȧ��
	 *@param [in] lParam ����
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */
	HRESULT OnCoilTrigger(WPARAM wParam, LPARAM lParam);

	/**
	 *@brief ��ȡ��ϻ�Ӽ�¼����Ϣ
	 *@param [in] wParam ��
	 *@param [out] lParam,CHAR*ָ�룬״̬�ַ���.
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	HRESULT OnGetBlackBoxMessage(WPARAM wParam, LPARAM lParam);

	/**
	 *@brief ��ȡ�豸����ģʽ
	 *@param [in] wParam ����ģʽ����ָ��,int ����
	 *@param [out] lParam ����ģʽ�ַ�����CHAR* ����
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */
	HRESULT OnGetWorkModeFromIndex(WPARAM wParam, LPARAM lParam);

	/**
	 *@brief NTPͬ��ʱ�亯��
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */
	HRESULT NtpTimeSync();

	/**
	 *@brief ��ʼ��Ӳ�̺���
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */
	HRESULT InitHdd(VOID);	
	/**
	 *@brief ж��Ӳ�̺���
	 *@return �ɹ�����S_OK,����ֵΪ�������
	 */
	HRESULT DeinitHdd(VOID);	
	HRESULT OnGetNetPackageVersion(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetFirmwareVersion(WPARAM wParam, LPARAM lParam);
    HRESULT OnGetBackUpVersion(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetFPGAVersion(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetKernelVersion(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetUBootVersion(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetUBLVersion(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetSN(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetCPU(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetRAM(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetLogLevel(WPARAM wParam, LPARAM lParam);
	HRESULT OnSetLogLevel(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetNTPServerIP(WPARAM wParam, LPARAM lParam);
	HRESULT OnSetNTPServerIP(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetNTPEnable(WPARAM wParam, LPARAM lParam);
	HRESULT OnSetNTPEnable(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetNTPTimeZone(WPARAM wParam, LPARAM lParam);
	HRESULT OnSetNTPTimeZone(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetNTPInterval(WPARAM wParam, LPARAM lParam);
	HRESULT OnSetNTPInterval(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetHDDCheckReport(WPARAM wParam, LPARAM lParam);
	HRESULT OnSetCustomizedDevName(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetCustomizedDevName(WPARAM wParam, LPARAM lParam);
	HRESULT OnReInitHDD(WPARAM wParam, LPARAM lParam);
	HRESULT OnSetAWBWorkMode(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetAWBWorkMode(WPARAM wParam, LPARAM lParam);
	HRESULT OnSetAutoControlCammeraAll(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetAutoControlCammeraAll(WPARAM wParam, LPARAM lParam);
	HRESULT OnSetH264Resolution(WPARAM wParam, LPARAM lParam);
	HRESULT OnGetH264Resolution(WPARAM wParam, LPARAM lParam);
    /**
     *@brief ���ý����������2A���̿���
     *@param [PVOID] pvBuffer �������
     *@param [PVOID] iSize ����
     *@return �ɹ�����S_OK,����ֵΪ�������
     */
	HRESULT OnSetDenoiseStatus(WPARAM wParam, LPARAM lParam);

	//��Ϣӳ�亯��
	SW_BEGIN_MESSAGE_MAP(CSWLPRApplication, CSWMessage)
		SW_MESSAGE_HANDLER(MSG_APP_SETTIME, OnSetTime)
		SW_MESSAGE_HANDLER(MSG_APP_GETTIME, OnGetTime)
		SW_MESSAGE_HANDLER(MSG_APP_SETIP,   OnSetIP)
		SW_MESSAGE_HANDLER(MSG_APP_GETIP,   OnGetIP)
		SW_MESSAGE_HANDLER(MSG_APP_SETXML,  OnSetXML)
		SW_MESSAGE_HANDLER(MSG_APP_GETXML,  OnGetXML)
		SW_MESSAGE_HANDLER(MSG_APP_GETWORKMODE_COUNT, OnGetWorkModeCount)
		SW_MESSAGE_HANDLER(MSG_APP_GETWORKMODE, OnGetWorkMode)
		SW_MESSAGE_HANDLER(MSG_APP_GETWORKMODEINDEX, OnGetWorkModeFromIndex)
		SW_MESSAGE_HANDLER(MSG_APP_RESETDEVICE, OnResetDevice)
		SW_MESSAGE_HANDLER(MSG_APP_RESTORE_DEFAULT, OnRestoreDefault)
		SW_MESSAGE_HANDLER(MSG_APP_RESTORE_FACTORY, OnRestoreFactory)
//		SW_MESSAGE_HANDLER(MSG_APP_SETCHARACTER_ENABLE, OnSetCharacterEnable)
//		SW_MESSAGE_HANDLER(MSG_APP_GETCHARACTER_ENABLE, OnGetCharacterEnable)
		SW_MESSAGE_HANDLER(MSG_APP_GETHVNAME, OnGetHvName)
		SW_MESSAGE_HANDLER(MSG_APP_GETHVID, OnGetHvID)
		SW_MESSAGE_HANDLER(MSG_APP_GETPRODUCT_NAME, OnGetProductName)
		SW_MESSAGE_HANDLER(MSG_APP_GETVIDEO_COUNT, OnGetVideoCount)
		SW_MESSAGE_HANDLER(MSG_APP_GETVERSION, OnGetVersion)
		SW_MESSAGE_HANDLER(MSG_APP_GETVRESION_STRING, OnGetVersionString)
		SW_MESSAGE_HANDLER(MSG_APP_GETCONNECTED_IP, OnGetConnectedIP)
		SW_MESSAGE_HANDLER(MSG_APP_HDD_FDISK, OnHDDFDisk)
		SW_MESSAGE_HANDLER(MSG_APP_HDD_CHECK, OnHDDCheck)
		SW_MESSAGE_HANDLER(MSG_APP_HDD_STATUS, OnHDDStatus)
		SW_MESSAGE_HANDLER(MSG_APP_RESET_COUNT, OnResetCount)
		SW_MESSAGE_HANDLER(MSG_APP_RESET_REPORT, OnResetReport)
		SW_MESSAGE_HANDLER(MSG_APP_RESET_MODE, OnResetMode)
		SW_MESSAGE_HANDLER(MSG_APP_GETDEVTYPE, OnGetDevType)
		SW_MESSAGE_HANDLER(MSG_APP_GETCPU_TEMPERATURE, OnGetCpuTemperature)
		SW_MESSAGE_HANDLER(MSG_APP_UPDATE_STATUS, OnUpdateStatus)
		SW_MESSAGE_HANDLER(MSG_APP_GET_RUN_STATUS, OnGetRunStatus)
		
		SW_MESSAGE_HANDLER(MSG_APP_RADAR_TRIGGER, OnRadarTrigger)	//�״ﴥ��ץ��
		
		SW_MESSAGE_HANDLER(MSG_APP_COIL_STATUS, OnCoilStatus)
		SW_MESSAGE_HANDLER(MSG_APP_COIL_TRIGGER, OnCoilTrigger)
		SW_MESSAGE_HANDLER(MSG_APP_GET_BLACKBOX_MESSAGE, OnGetBlackBoxMessage)
		SW_MESSAGE_HANDLER(MSG_APP_GETFIRMWAREVERSION, OnGetFirmwareVersion)
		SW_MESSAGE_HANDLER(MSG_APP_GETNETPACKAGEVERSION, OnGetNetPackageVersion)
        SW_MESSAGE_HANDLER(MSG_APP_GETBACKUPTVERSION, OnGetBackUpVersion)
		SW_MESSAGE_HANDLER(MSG_APP_GETFPGAVERSION, OnGetFPGAVersion)
		SW_MESSAGE_HANDLER(MSG_APP_GETKERNALVERSION, OnGetKernelVersion)
		SW_MESSAGE_HANDLER(MSG_APP_GETUBOOTVERSION, OnGetUBootVersion)
		SW_MESSAGE_HANDLER(MSG_APP_GETUBLVERSION, OnGetUBLVersion)
		SW_MESSAGE_HANDLER(MSG_APP_GETSN, OnGetSN)
		SW_MESSAGE_HANDLER(MSG_APP_GETCPU, OnGetCPU)
		SW_MESSAGE_HANDLER(MSG_APP_GETRAM, OnGetRAM)
		SW_MESSAGE_HANDLER(MSG_APP_GET_LOG_LEVEL, OnGetLogLevel)
		SW_MESSAGE_HANDLER(MSG_APP_SET_LOG_LEVEL, OnSetLogLevel)
		SW_MESSAGE_HANDLER(MAG_APP_GET_NTP_SERVER_IP, OnGetNTPServerIP)
		SW_MESSAGE_HANDLER(MAG_APP_SET_NTP_SERVER_IP, OnSetNTPServerIP)
		SW_MESSAGE_HANDLER(MAG_APP_GET_NTP_ENABLE, OnGetNTPEnable)
		SW_MESSAGE_HANDLER(MAG_APP_SET_NTP_ENABLE, OnSetNTPEnable)
		SW_MESSAGE_HANDLER(MAG_APP_GET_NTP_TIMEZONE, OnGetNTPTimeZone)
		SW_MESSAGE_HANDLER(MAG_APP_SET_NTP_TIMEZONE, OnSetNTPTimeZone)
		SW_MESSAGE_HANDLER(MAG_APP_GET_NTP_INTERVAL, OnGetNTPInterval)
		SW_MESSAGE_HANDLER(MAG_APP_SET_NTP_INTERVAL, OnSetNTPInterval)
		SW_MESSAGE_HANDLER(MSG_APP_HDD_CHECKREPORT, OnGetHDDCheckReport)
		SW_MESSAGE_HANDLER(MSG_APP_SET_CUSTOMIZED_DEVNAME, OnSetCustomizedDevName)
		SW_MESSAGE_HANDLER(MSG_APP_GET_CUSTOMIZED_DEVNAME, OnGetCustomizedDevName)
		SW_MESSAGE_HANDLER(MSG_APP_REINIT_HDD, OnReInitHDD)
		SW_MESSAGE_HANDLER(MSG_APP_SET_AWB_WORK_MODE, OnSetAWBWorkMode)
		SW_MESSAGE_HANDLER(MSG_APP_GET_AWB_WORK_MODE, OnGetAWBWorkMode)
		SW_MESSAGE_HANDLER(MSG_APP_SET_AUTO_CONTROL_CAMMERA_ALL, OnSetAutoControlCammeraAll)
		SW_MESSAGE_HANDLER(MSG_APP_GET_AUTO_CONTROL_CAMMERA_ALL, OnGetAutoControlCammeraAll)
		SW_MESSAGE_HANDLER(MSG_APP_SET_H264_RESOLUTION, OnSetH264Resolution)
		SW_MESSAGE_HANDLER(MSG_APP_GET_H264_RESOLUTION, OnGetH264Resolution)
		SW_MESSAGE_HANDLER(MSG_APP_SET_DENOISE_STATUS, OnSetDenoiseStatus)
	SW_END_MESSAGE_MAP()

    HRESULT OnGetGB28181Param(PVOID pvBuffer, INT iSize);
    /**
     *@brief ��λ�豸
     *@param [PVOID] pvBuffer ����
     *@param [PVOID] iSize ����
     *@return �ɹ�����S_OK,����ֵΪ�������
     */
    HRESULT OnReBoot(PVOID pvBuffer, INT iSize);

	HRESULT OnSendRtspParam(PVOID pvBuffer, INT iSize);

	HRESULT OnGetOnvifParam(PVOID pvBuffer, INT iSize);


    //Զ����Ϣӳ�亯��
    SW_BEGIN_REMOTE_MESSAGE_MAP(CSWLPRApplication, CSWMessage)
	    SW_REMOTE_MESSAGE_HANDLER(MSG_APP_GET_28181_PARAM,OnGetGB28181Param)
	    SW_REMOTE_MESSAGE_HANDLER(MSG_APP_REBOOT,OnReBoot)
		SW_REMOTE_MESSAGE_HANDLER(MSG_APP_GET_RTSP_PARAM, OnSendRtspParam)
		SW_REMOTE_MESSAGE_HANDLER(MSG_APP_GET_ONVIF_PARAM, OnGetOnvifParam)
		//SW_REMOTE_MESSAGE_HANDLER(MSG_APP_SET_DENOISE_STATUS, OnSetDenoiseStatus)
    SW_END_REMOTE_MESSAGE_MAP()
private:
	TiXmlDocument m_xmlDoc;
	CSWLPRParameter *m_pParam;
	INT    m_hObject;

	struct tag_resetInfoHeader
	{
		DWORD dwFlag;
		DWORD dwCount;
		DWORD dwHead;
		DWORD dwTail;
		DWORD dwCRC;
	} m_cResetInfoHeader;
	static const DWORD RESET_INFO_FLAG = 0xFFEE0004;

	enum
	{
		DEVTYPE_MERCURY = 0,
		DEVTYPE_VENUS,
		DEVTYPE_JUPITER
	};

	CSWString m_strH264Status;
	CSWString m_strVPIFStatus;
	CSWString m_strRecogStatus;
	CSWString m_strDM6467Status;
	CSWString m_strRunStatus;

	CSWString m_strResultNetStatus;
	CSWString m_strJpegNetStatus;
	CSWString m_strH264NetStatus;

	CSWString m_strResultHDDStatus;
	CSWString m_strFTPStatus;
	CSWString m_strH264HDDStatus;
	CSWString m_strRTSPConnStatus;
	CSWString m_strRTSPSecondConnStatus;

	BOOL	m_fNetDiskMounted;
	BOOL	m_fHardDiskReady;
	CHAR	m_szHDDPath[64];
	DWORD	m_dwDevType;

	
	// AT88
	DWORD m_rgdwNC[4];
	
	//DSP��������,������־����Ƶ���������ⲿ����
	BOOL  m_fDSPTrigger;	//����ץ�ģ�0��ʾ��ץ�ģ�1��ʾ����ץ��
    INT m_iFlashDiffFlag;           // �����������ǣ�0��������һ���ƣ�1��������һ����
	CSWString m_strCoilStatus;

	//SD�������¼
	struct SdErrRecord
	{
		UINT id;
		DWORD time;
	};
	CSWList<SdErrRecord*>	m_lstSdErrRecord;
	INT m_MaxSdErrNum;
	INT m_SdErrNum;

	BOOL m_fIsAppReady;		//APP��������
};
extern CSWLPRApplication *theApp;
#endif

