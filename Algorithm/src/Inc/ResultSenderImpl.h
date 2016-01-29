/**
* @file		ResultSenderImpl.h
* @version	1.0
* @brief	�������ģ��ʵ��
*/

#ifndef _ResultSenderImpl_h_
#define _ResultSenderImpl_h_

#include "IResultSender.h"
#include "DataLinkImpl.h"
#include "sendnetdata.h"
#include "HvResultFilter.h"
#include "ImgGatherer.h"
#include "AutoLinkParam.h"

#ifndef MAX_STREETINFO_LENGTH
#define MAX_STREETINFO_LENGTH 255
#endif

/**
* @brief �������ģ������ṹ��
*/
typedef struct _ResultSenderParam
{
    //�������ģ�����ز���
    PROCESS_RULE cProcRule;     /**< ������� */

    BOOL fInitHdd;              /**< �Ƿ��ʼ��Ӳ�� */

    //�ɿ��Ա���ģ��
    BOOL fIsSafeSaver;            /**< �Ƿ�������ȫ�洢 */
    char szSafeSaverInitStr[64];  /**< ��ȫ��������ʼ������ */  //��ʱ���ã�Ԥ��
    int  iRecordFreeSpacePerDisk;
    int  iVideoFreeSpacePerDisk;

    //������Ƶ��ز���
    int iSaveVideo;         /**< ������Ƶ���� */
    int iVideoDisplayTime;  /**< ������Ƶʱ���� */
    int iDrawRect;          /**< ���ͺ���ʶ */
    int iEddyType;          /**< ͼƬ��ת */

    //������Ϣģ�����
    BOOL fOutputAppendInfo;     //������Ƹ�����Ϣ����
    BOOL fOutputObservedFrames; //�����Ч֡������
    BOOL fOutputCarArriveTime;  //������ʱ�俪��
    BOOL fOutputFilterInfo; //���������Ϣ
    char szStreetName[MAX_STREETINFO_LENGTH];       //·������
    char szStreetDirection[MAX_STREETINFO_LENGTH];  //·�ڷ���

    int iOutputOnlyPeccancy;//1:ֻ���Υ�³��� 2:ֻ�����Υ�½�� 3:������ʷ����ʱ,��Υ������ı�,Υ������ı���ͼƬ
    int iSendRecordSpace; //������ͼ��(*100ms)
    int iSendHisVideoSpace;//��ʷ¼���ͼ��(*100ms)

    int iSpeedLimit;    //�ٶ�����ֵ(km/h)

    int iCheckEventTime;    //�¼��������(��λ:����)

    int iSaveType;    //������淽ʽ
    int iWidth;         //��ͼ���
    int iHeight;        //��ͼ�߶�
    int iBestSnapshotOutput;    //����Ƶ��������ͼ���
    int iLastSnapshotOutput;    //����Ƶ����ͼ���
    int iOutputCaptureImage;    //��Υ�³����������ץ��ͼ

    int iFilterUnSurePeccancy;  //���˲��ϸ��Υ�¼�¼
    int nOutputPeccancyType;    //���Υ�����ͷ�ʽ
    char szPeccancyPriority[256]; //Υ���������ȼ�

    float fltCarAGCLinkage;  //��������AGC�������أ�0Ϊ�أ���0��ʾ��λʱ�䳵�������ڸ�ֵʱ����AGC��������ر�
    int iAWBLinkage;        //����������AWB��������
    int nRedPosBrightness;

    int iEnablePanoramicCamera;     //ȫ�����ʹ�ܿ���
    char szPanoramicCameraIP[20];   //ȫ�����IP

    CAP_CAM_PARAM cCapCamParam; //ץ���������
    AUTO_LINK_PARAM cAutoLinkParam; //�������Ӳ���

    BOOL fMTUSetEnable;

    //��ini�ļ����͡�����ʹ��
    //int nExportParamRank;   /**< ��������ȼ� */
    //BOOL nExportRankInfo;   /**< ����ȼ���Ϣ */

	int iCapMode;  //ץ��ģʽ (0:ץ��һ��; 1:����Υ��ץ��2��; 2:ȫ��ץ��2��)
	int iFlashDifferentLane;  //����Ʒֳ�����

    _ResultSenderParam()
    {
        iBestSnapshotOutput = 1;
        iLastSnapshotOutput = 1;
        iOutputCaptureImage = 0;
#ifndef SINGLE_BOARD_PLATFORM
        fIsSafeSaver = 0;
#else
		fIsSafeSaver = 1;
#endif
        iRecordFreeSpacePerDisk = 10;
        iVideoFreeSpacePerDisk = 10;
        iVideoDisplayTime = 500;
        iDrawRect = 1;
        iEddyType = 0;
        iSpeedLimit = 1000;
        iSaveType = 0;
        iWidth = 720;
        iHeight = 288;
        iCheckEventTime = 1;
        iFilterUnSurePeccancy = 1;
        fOutputAppendInfo = TRUE;
        fOutputObservedFrames = TRUE;
        fOutputCarArriveTime = FALSE;
        strcpy(szSafeSaverInitStr, "NULL");
        strcpy(szStreetName, "NULL");
        strcpy(szStreetDirection, "NULL");
        iOutputOnlyPeccancy = 0;
        iSendRecordSpace = 5;
        iSendHisVideoSpace = 1;
        nOutputPeccancyType = 0;
        fltCarAGCLinkage = 0.0f;
        iAWBLinkage = 0;
        fOutputFilterInfo = 0;
        fInitHdd = FALSE;
        fMTUSetEnable = FALSE;
        strcpy(szPeccancyPriority, "����-�ǻ�������-�����-����������ʻ-ѹ��-����");
		iCapMode = 0;
		iFlashDifferentLane = 0;
    };

} ResultSenderParam;

class CResultSender : public IResultSender
{
public:
    CResultSender();
    ~CResultSender();

public:
    HRESULT Init(
        const ResultSenderParam& cParam,
        CDataLinkCtrlThread* pcDataLinkCtrlThread
    );

public:
    HRESULT PutResult(
        LPCSTR szResultInfo,
        LPVOID lpcData
    );

    HRESULT PutVideo(
        DWORD32* pdwSendCount,
        LPVOID lpFrameData,
        int nRectCount = 0,
        HV_RECT *pRect = NULL
    );

    HRESULT PutString(
        WORD16 wVideoID,
        WORD16 wStreamID,
        DWORD32 dwTimeLow,
        DWORD32 dwTimeHigh,
        const char *pString
    );

    HRESULT PutCarEnterLeaveFlag(
        DWORD32 dwVideoID,
        WORD16 wInLeftFlag,
        DWORD32 dwTimeMsLow,
        DWORD32 dwTimeMsHigh
    );

    virtual HRESULT GetCurStatus(char* pszStatus, int nStatusSizes);

protected:
    bool fInited;
    CDataLinkCtrlThread* m_pcDataLinkCtrlThread;
    CHvResultFilter m_resultFilter;

public:
    //��ȫ�洢��
    CSafeSaverDm6467Impl* m_pcSafeSaver;
    ISafeSaver* m_pSafeSaver;

public:
    static ResultSenderParam m_cParam;  /**< �������ģ���ܲ��� */
};

#endif
