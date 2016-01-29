// ���ļ����������WINDOWS-936��ʽ
#ifndef _SAFESAVERIMPL_CF_H_
#define _SAFESAVERIMPL_CF_H_

#include "safesaver.h"
#if FIX_FILE_VER == 1
#include "ConstFileSys.h"
#elif FIX_FILE_VER == 2
#include "FixFileStream.h"
#elif FIX_FILE_VER == 3
#include "BigFile.h"
#endif
#include <list>
#include "DspLinkMemBlocks.h"

class CSafeSaverDm6467Impl : public ISafeSaver,public CHvThreadBase
{
public:
    CSafeSaverDm6467Impl(void);
    ~CSafeSaverDm6467Impl(void);

	virtual HRESULT Run(void *pvParamter);

public:
    //ISafeSaver�ӿڷ���
    HRESULT Init(const SSafeSaveParam* pParam);

    HRESULT SavePlateRecord(
        DWORD32 dwTimeLow, DWORD32 dwTimeHigh, int* piIndex,
        const CAMERA_INFO_HEADER* pInfoHeader,
        const unsigned char* pbInfo,
        const unsigned char* pbData,
        const char* szDevIno  //�豸��Ϣ(��Ż���IP)
    );

    virtual HRESULT SavePlateRecord(
        DWORD32 dwTimeLow, DWORD32 dwTimeHigh, int* piIndex,
        CCameraDataReference* pRefData //��������
    );

    HRESULT GetPlateRecord(
        DWORD32 dwTimeLow, DWORD32 dwTimeHigh, int& index,
        CAMERA_INFO_HEADER* pInfoHeader,
        unsigned char* pbInfo, const int iInfoLen,
        unsigned char* pbData, const int iDataLen,
        const char* szDevIno,  //�豸��Ϣ(��Ż���IP)
        int & nLastDiskID,
        const DWORD32 dwDataInfo
    );

    HRESULT SaveVideoRecord(
        DWORD32 dwTimeLow, DWORD32 dwTimeHigh,
        const CAMERA_INFO_HEADER* pInfoHeader,
        const unsigned char* pbInfo,
        const unsigned char* pbData
    );

    HRESULT GetVideoRecord(
        DWORD32 dwTimeLow, DWORD32 dwTimeHigh,
        CAMERA_INFO_HEADER* pInfoHeader,
        unsigned char* pbInfo, const int iInfoLen,
        unsigned char* pbData, const int iDataLen
    );

    int GetCurRecordIndex()
    {
    	return -1;
    }

    void SetFreeSpacePerDisk(int iResultFreeSpacePerDisk, int iVideoFreeSpacePerDisk)
    {
    }

    //��ȡĳ��СʱĿ¼�µĽ������
    HRESULT GetHourCount(const DWORD32 dwTimeLow, const DWORD32 dwTimeHigh, DWORD32* pdwCount);

    HRESULT SaveDetectData(CDetectData *pData);

    HRESULT GetDetectData(DWORD32& dwTimeLow, DWORD32& dwTimeHigh, CDetectData *pData);

    HRESULT GetDiskStatus();

    HRESULT GetLastCamStatus(DWORD32* pdwStatus);

    HRESULT GetCurStatus(char* pszStatus, int nStatusSizes);

    //������һ����Ƶ�ļ�
    HRESULT FindNextVideo(DWORD32& dwTimeLow, DWORD32& dwTimeHigh);

private:

	HRESULT PutData(const int iDataType //�������� 0:ʶ���� 1:¼��
					,const DWORD32 dwTimeLow
				    ,const DWORD32 dwTimeHigh
				    ,const DWORD32 dwDataLen
				    ,int* piIndex
				    ,CCameraDataReference* pRefData
					);

	HRESULT GetData(const int iDataType //�������� 0:ʶ���� 1:¼��
				   ,DWORD32 dwTimeLow
				   ,DWORD32 dwTimeHigh
				   ,DWORD32 dwDataInfo
				   ,int& index
    			   ,CAMERA_INFO_HEADER* pInfoHeader
    			   ,unsigned char* pbInfo
    			   ,const int iInfoLen
				   ,unsigned char* pbData
				   ,const int iDataLen
				   ,const char* szDevIno  //�豸��Ϣ(��Ż���IP)
				   );

	//��������������
	HRESULT CopyCfData(SCfData& sCfData);

private:

    HV_SEM_HANDLE               m_hSem;                   //�洢ϵͳ����
    HV_SEM_HANDLE               m_hSemQue;				   //���е���
    HV_SEM_HANDLE               m_hSemQueCount;            //�����ź���
    std::list<SCfData>          m_listCfData;

	SSafeSaveParam			    m_sParam;			 		//��ʼ������
#if FIX_FILE_VER == 1
    CConstFileSys               m_cCfRecord;           	  //�����洢ϵͳ
    CConstFileSys               m_cCfVideo;           		//�����洢ϵͳ
#elif FIX_FILE_VER == 2
    CFixFileStream              m_fixRecord;
    CFixFileStream              m_fixVideo;
#elif FIX_FILE_VER == 3
    CBigFile                    m_bigFileRecord;
    CBigFile                    m_bigFileVideo;
#endif
    DSPLinkBuffer               m_sBuf;
    HRESULT                     m_hrDiskStatus;     		//Ӳ��״̬
    DWORD32                     m_dwLastDataTime;
    REAL_TIME_STRUCT            m_rtsLastVideoTime;       //���һ�α���¼���ʱ��
};

#endif
