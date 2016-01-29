#include "swpa.h"
#include "SWFC.h"
#include "SWClassFactory.h"

CSWClassFactory theClassFactory;

///////////////////////////////////////////////////////////////////////////////////////
CSWCreateClass::CSWCreateClass()
{
}

CSWCreateClass::~CSWCreateClass()
{
}

LPCSTR CSWCreateClass::GetObjectName(void)
{
	return NULL;
}

CSWObject* CSWCreateClass::CreateObject(const char *szclsName)
{
	return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////
CSWClassFactory::CSWClassFactory()
{
}

CSWClassFactory::~CSWClassFactory()
{
}

CSWObject *CSWClassFactory::CreateObject(LPCSTR lpzclsName)
{
	SW_POSITION pos = m_lstClass.GetHeadPosition();
	while(m_lstClass.IsValid(pos))
	{
		CSWCreateClass* pClass = m_lstClass.GetNext(pos);
		if(!swpa_strcmp(pClass->GetObjectName(), lpzclsName))
		{
			return pClass->CreateObject(lpzclsName);
		}
	}
	return NULL;
}

HRESULT CSWClassFactory::Add(CSWCreateClass *pClass)
{
	if(NULL == m_lstClass.Find(pClass))
	{
		if(m_lstClass.IsFull())
		{
			m_lstClass.SetMaxCount(m_lstClass.GetCount() + 1);
		}
		m_lstClass.AddTail(pClass);
	}
	return S_OK;
}
	
void ClassInitialize()
{
	//�������������ģ��
	LOAD_CLASS(CSWCameralControlMSG)
	LOAD_CLASS(CSWCameraControl2AMSG)

	//�����������ģ��
	LOAD_CLASS(CSWDomeCameraControlMSG)

	
	//�ź�ƥ��ģ��
	LOAD_CLASS(CSWMatchTransformFilter)
	//����ģ��
	LOAD_CLASS(CSWResultFilter)
	//JPEGѹ��ģ��
	LOAD_CLASS(CSWJPEGEncodeTransformFilter)
	//�Զ�������ģ��
	LOAD_CLASS(CSWAutoControlRenderFilter)
	//H264����ģ��
	LOAD_CLASS(CSWH264HDDTransformFilter)
	//H264���緢��ģ��
	LOAD_CLASS(CSWH264NetRenderFilter)
	//ʶ��������ģ��
	LOAD_CLASS(CSWResultHDDTransformFilter)
	//ʶ�������緢��ģ��
	LOAD_CLASS(CSWResultNetRenderFilter)
	//JPEG�����������緢��ģ��
	LOAD_CLASS(CSWJPEGNetRenderFilter)
	//��������ģ��
	LOAD_CLASS(CSWNetCommandProcess)
	//ʶ��ģ��
	LOAD_CLASS(CSWRecognizeTransformFilter)
	//ʶ��ģ��
	LOAD_CLASS(CSWRecognizeTransformPTFilter)
	//ʶ��ģ��
	LOAD_CLASS(CSWRecognizeTransformEPFilter)
    //ʶ��ģ��
    LOAD_CLASS(CSWRecognizeTransformTollGateFilter)

	LOAD_CLASS(CSWRecognizeTGTransformFilter)
	//����ɼ�ģ��
	LOAD_CLASS(CSWNetSourceFilter)

	LOAD_CLASS(CSWYUVNetRenderFilter)
	// �������ģ��
	LOAD_CLASS(CSWTriggerOut)
	//��ͨ�¼����ģ��
	LOAD_CLASS(CSWEPoliceEventCheckerTransformFilter)

	//Recordת��ģ��
	LOAD_CLASS(CSWRecordTransformFilter)

    //JPEGCach����ģ��
    LOAD_CLASS(CSWJPEGCachTransformFilter)
	


	//�ַ�����ģ��
	LOAD_CLASS(CSWJPEGOverlayFilter)
	LOAD_CLASS(CSWH264OverlayFilter)
	LOAD_CLASS(CSWH264SecondOverlayFilter)
	LOAD_CLASS(CSWImage2SWPosImageTransformFilter)
	//JPEG��ѹģ��
	LOAD_CLASS(CSWJPEGFilter)
#ifdef DM6467	
	//VPIFģ��
	LOAD_CLASS(CSWVPIFSourceFilter)
	//H264�ɼ�ģ��
	LOAD_CLASS(CSWH264SourceFilter)
#endif

#ifdef DM8127
	LOAD_CLASS(CSWJPEGDecodeTransformFilter)
	LOAD_CLASS(CSWCentaurusSourceFilter)
	LOAD_CLASS(CSWIPNCTransformFilter)
	LOAD_CLASS(CSWCentaurusH264EncodeFilter)
	LOAD_CLASS(CSWCentaurusJpegEncodeFilter)
	LOAD_CLASS(CSWExtendDataTransformFilter)
#endif	
    //RTSPģ��
    LOAD_CLASS(CSWRTSPTransformFilter)

    //GB28181ģ��
    LOAD_CLASS(CSWGBH264TransformFilter)

    //H264����ģ��
    LOAD_CLASS(CSWH264Filter)

	//�û�����ģ��
    LOAD_CLASS(CSWUserManager)

	//�������ģ��
    LOAD_CLASS(CSWDomeRockerControlMSG)

	//FTP�ϴ�ģ��
	LOAD_CLASS(CSWResultFtpRenderFilter)

	//Υ��ͣ�����ģ��
	LOAD_CLASS(CSWRecognizeTransformPPFilter)

	//H264����ģ��
	LOAD_CLASS(CSWH264QueueRenderFilter)

	//���ȡ��¼��ģ��
	LOAD_CLASS(CSWResultVideoTransformFilter)

	// MD5����ģ��
	LOAD_CLASS(CSWMD5EncryptFilter)
}

