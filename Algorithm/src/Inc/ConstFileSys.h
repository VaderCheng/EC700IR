// ���ļ����������WINDOWS-936��ʽ
#ifndef _CONST_FILE_SYS_H_
#define _CONST_FILE_SYS_H_

#include <vector>
#ifdef WIN32
#include <vcl.h>
#else
#include "hvutils.h"
#endif

#ifdef WIN32
#ifndef WORD16
typedef unsigned short WORD16;
#endif
#ifndef DWORD32
typedef unsigned int DWORD32;
#endif
#endif

typedef HRESULT (*CF_ReadFile)(char* szFileName, char* szBuffer, DWORD32 dwBufLen, DWORD32& dwDataLen);
typedef HRESULT (*CF_WriteFile)(char* szFileName, char* szData, DWORD32 dwDataLen);
typedef HRESULT (*CF_DeleteFile)(char* szFileName);
typedef HRESULT (*CF_DeleteFolder)(char* szPath);


//ϵͳ��
struct SSysTable
{
    DWORD32         dwMaxFileCount;         //����ļ�����
    DWORD32         dwMaxFileSize;          //һ���ļ�����С
    DWORD32         dwFileBegin;            //���������͵ĵ�һ���ļ���ʼ���
    DWORD32         dwExtend1;              //��չ�ֶ�1

    SSysTable()
    {
        memset(this, 0, sizeof(*this));
    }
};

//�����ļ�ϵͳ��Сʱ������
struct SHourIndex
{
    DWORD32         dwHour;                 //Сʱ������11102213��ʾ2011��10��23��13ʱ
    DWORD32         dwFileIndex;            //�ڸ��������͵��ļ�ϵͳ�е�index
    DWORD32         dwHourIndex;            //�ڸ�Сʱ�е�index,����������¶�Ϊ0
    DWORD32         dwCount;                //��Сʱ���ļ�����

    SHourIndex()
    {
        memset(this, 0, sizeof(*this));
    }
};

//һ��������
struct SCfItem
{
    DWORD32         dwTimeLow;
    DWORD32         dwTimeHigh;
    DWORD32         dwIndex;

    SCfItem()
    {
        memset(this, 0, sizeof(*this));
    }
};

//Сʱ��Ϣ
struct SHourInfo
{
    DWORD32         dwHour;                 //Сʱ������11102213��ʾ2011��10��23��13ʱ
    unsigned char   szInfo[4096];

    SHourInfo()
    {
        memset(this, 0, sizeof(*this));
    }
};


//��ʼ���ṹ
struct SConstFileSysParam
{
    bool            fFormat;                //�Ƿ��ʽ��
    DWORD32         dwMaxFileCount;         //��ʽ��ʱ,��������ļ�����
    DWORD32         dwMaxFileSize;          //��ʽ��ʱ,����һ���ļ�����С
    char            szRootDir[MAX_PATH];    //�洢ϵͳ�ĸ�Ŀ¼
    DWORD32         dwFileBegin;            //���������͵ĵ�һ���ļ���ʼ���
    char            szDataType[8];          //�������� record, video
    CF_ReadFile     fnReadFile;             //������
    CF_WriteFile    fnWriteFile;            //д����
    CF_DeleteFile   fnDeleteFile;           //ɾ������
    CF_DeleteFolder fnDeleteFolder;         //ɾ��Ŀ¼�ĺ���
    bool            fHourFix; 
    bool            fHourInfo;				  //�Ƿ�Ҫ��¼Сʱ�ļ�¼��Ϣ

    SConstFileSysParam()
    {
        memset(this, 0, sizeof(*this));
    }
};

//�����ļ�ϵͳ
class CConstFileSys
{
public:
    CConstFileSys();
    ~CConstFileSys();

    //��ʼ��
    HRESULT Init(const SConstFileSysParam* pParam);

    //����ʱ��洢�ļ�
    HRESULT WriteFile(const DWORD32 dwTimeLow
                     ,const DWORD32 dwTimeHigh
                     ,const DWORD32 dwDataInfo          //������Ϣ(1:Υ������)
                     ,DWORD32& dwIndex
                     ,char* szBuf
                     ,const DWORD32 dwDataLen
                     ,const bool fForceNewHour = false
                     );

    //����ʱ���ȡ�ļ�
    HRESULT ReadFile(const DWORD32 dwTimeLow
                    ,const DWORD32 dwTimeHigh
                    ,const DWORD32 dwDataInfo          //������Ϣ(1:Υ������)
                    ,DWORD32& dwIndex
                    ,char* szBuf
                    ,const DWORD32 dwBufLen
                    ,DWORD32& dwDataLen
                    );

    //��ȡĳ��Сʱ���ļ�����
    DWORD32 GetHourCount(const DWORD32 dwTimeLow, const DWORD32 dwTimeHigh);

	//���ݵ�ǰ���ȡ��һ����
    HRESULT GetNextItem(const SCfItem& sCfItem, SCfItem& sCfItemNext);

    //��ȡ�洢ϵͳ״̬
    HRESULT GetStatus();

    //��ȡϵͳ��
    const SSysTable& GetSysTable() const;

    //��ȡСʱ����
    const std::vector<SHourIndex>& GetHourIndex() const;

    //δ�ύ�ĸ�������dwMinCountʱ���ύ������صı�
    HRESULT Submit(const DWORD32 dwMinCount);

protected:

	//����Сʱ�������
	HRESULT Update();
	
    //����Сʱ������
    HRESULT UpdateHourIndex();

    //����Сʱ�������ݱ�
    HRESULT UpdateHourIndexBak();

    //����ϵͳ�ļ������ȵ��ļ�·��
    HRESULT GetFileByIndex(const DWORD32 dwFileIndex
                          ,char* szBuf
                          ,const DWORD32 dwBufLen
                          );

    //ת��ʱ��Ϊ�����Сʱ��
    DWORD32 TimeToHour(const DWORD32 dwTimeLow, const DWORD32 dwTimeHigh);

    //���洢����ɾ����һ��Сʱ�Ľ��
    HRESULT DeleteFirstHour();

    //���ϵͳ���Ƿ�����
    HRESULT CheckSysTable();

	//���
    HRESULT Clear(); 

    //����Сʱ��ȡСʱ��Ϣ�ļ�
    HRESULT GetHourInfoFile(const DWORD32 dwHour
    					   ,const bool fBak
                           ,char* szBuf
                           ,const DWORD32 dwBufLen
                           );
                           
    //����ĳ��Сʱ����Ϣ
    HRESULT GetHourInfo(const DWORD32 dwHour, SHourInfo& sHourInfo);

	//���µ�ǰ����Сʱ����Ϣ
    HRESULT UpdateHourInfo();

private:
    bool                                    m_fSysOk;                       //�ļ�ϵͳ״̬����
    std::vector<SHourIndex>                 m_listHourIndex;                //Сʱ����
    SSysTable                               m_sSysTable;                    //ϵͳ��
    bool                                    m_fHourFix;                     //Сʱ�����ݹ̶�  
    bool                                    m_fHourInfo;				     //�Ƿ�Ҫ��¼Сʱ�ļ�¼��Ϣ
    SHourInfo                               m_sWriteHourInfo;              //��ǰдСʱ����Ϣ
    SHourInfo                               m_sReadHourInfo;               //��ǰ��Сʱ����Ϣ
	DWORD32                                 m_dwLastCount;                 //��¼���ϴ��ύ���������ڵļ�¼����
	
    char                                    m_szDataDir[MAX_PATH];          //�������ݵĸ�Ŀ¼
    char                                    m_szLogDir[MAX_PATH];           //������������־�ĸ�Ŀ¼
    char                                    m_szHourIndexFile[MAX_PATH];    //Сʱ�������ļ�
    char                                    m_szHourIndexFileBak[MAX_PATH]; //Сʱ������ı����ļ�
    char                                    m_szSysTableFile[MAX_PATH];     //ϵͳ���ļ�

	DWORD32                                 m_dwBufLen;                      //�����С
    char*                                   m_pBuffer;                       //��д����
    CF_ReadFile                             m_fnReadFile;
    CF_WriteFile                            m_fnWriteFile;
    CF_DeleteFile                           m_fnDeleteFile;

};

#endif
