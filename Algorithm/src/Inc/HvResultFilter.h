//���ļ������ʽ����ΪWINDOWS-936��ʽ

#ifndef  _INCLUDE_HVRESULTFILTER_H_
#define  _INCLUDE_HVRESULTFILTER_H_

#include "swbasetype.h"
#include "HvVarType.h"
#include "hvutils.h"
#include "HvParamStore.h"

//ȫ��ת���
bool SBC2DBC( LPCSTR lpszSrc, LPSTR lpszDest );
const int MAX_OUTPUT_BUF_LEN = 32 * 1024;

namespace HiVideo
{
    //����ʱ���ʽ
    class CRuleTime
    {
    public:
        CRuleTime();
        ~CRuleTime();

    public:
        bool SetTime( const int iHour, const int iMinute, const int iSecond );
        bool SetTime( LPCSTR lpszTime );

        int GetHour() const
        {
            return m_iHour;
        }
        int GetMinute() const
        {
            return m_iMinute;
        }
        int GetSecond() const
        {
            return m_iSecond;
        }

        bool operator == (const CRuleTime& crt) const;
        bool operator > (const CRuleTime& crt) const;
        bool operator < (const CRuleTime& crt) const;
        bool operator >= (const CRuleTime& crt) const;
        bool operator <= (const CRuleTime& crt) const;

    public:
        //ʱ�� crtNow �Ƿ����� crtLeft �� crtRight ������
        static bool IsCourseOfTime( const CRuleTime& crtLeft, const CRuleTime& crtRight, const CRuleTime& crtNow );

    private:
        int m_iHour;
        int m_iMinute;
        int m_iSecond;
    };


    ///////////////////////////////////////////////////////////////////////////
    //                           ������
    /////////////////////////////////////////////////////////////////////////////////////////

    //��lpszFileName����һ��С��lpszSection�����ݵ�lpszOutStr, �ṹΪ key=value\0
    int GetFileSection( LPCSTR lpszSection, LPSTR lpszOutStr, int iLen, CParamStore* pParam, LPCSTR lpszFileName = NULL );
    //�õ���ǰʱ��
    void GetTime( CRuleTime& crtime );

    //���ƴ���������Ϣ
    typedef struct ResultInfo
    {
        CHvString strPlate;
        BOOL    fAlarm;
        BOOL    fLeach;
        CHvString strFolder;
        CHvString strOther;
        CHvString strApplied;
    }RESULT_INFO;

    //���˹���ṹ
    typedef struct LeachInfo
    {
        CHvString strPattern;
        BOOL fLeach;
        CHvString strComment;
    }LEACH_INFO;

    //��������ṹ
    typedef struct AlarmInfo
    {
        CHvString strPattern;
        BOOL fAlarm;
        CHvString strComment;
    }ALARM_INFO;

    //����Ŀ¼�Ĺ���ṹ
    typedef struct FolderInfo
    {
        CHvString strPattern;
        CHvString strFolder;
        CHvString strComment;
    }FOLDER_INFO;

    //�����滻�Ĺ���ṹ
    typedef struct ReplaceInfo
    {
        CHvString strPattern;
        CHvString strResult;
        CHvString strComment;
    }REPLACE_INFO;

    //ͨ�������ṹ
    typedef struct  CompatyWord
    {
        CHvString strPattern;
        CHvString strResult;
    }COMPATY_WORD;

    /**
    * @brief �������ģ������ṹ��
    */
    typedef struct _ProcessRule
    {
        //ʶ�����������ļ��ϣ�\\HvDsp\\FilterRule����Ӧ�Ӽ��ļ�ֵ
        CHAR szCompatyRule[MAX_OUTPUT_BUF_LEN];
        CHAR szReplaceRule[MAX_OUTPUT_BUF_LEN];
        CHAR szLeachRule[MAX_OUTPUT_BUF_LEN];
        BOOL fCompaty;
        BOOL fReplace;
        BOOL fLoopReplace;
        BOOL fLeach;

        _ProcessRule()
        {
            strcpy(szCompatyRule, "NULL");
            strcpy(szReplaceRule, "NULL");
            strcpy(szLeachRule, "NULL");
            fCompaty = TRUE;
            fReplace = TRUE;
            fLoopReplace = TRUE;
            fLeach = TRUE;
        };

    } PROCESS_RULE;  //�������

    class CHvResultFilter
    {
    public:
        CHvResultFilter(void);
        ~CHvResultFilter(void);

    public:
        //��ʼ������
        BOOL InitRule(PROCESS_RULE *pProcessRule);

        //����
        BOOL FilterProcess(RESULT_INFO* pResultInfo);

    private:
        //�����滻
        BOOL ReplacePlate( CHvString& strPlate, const BOOL& fLoopReplace = FALSE );
        void DoReplace(CHvString& strPlate, REPLACE_INFO& replaceinfo);
        //�Ƿ�Ҫ����
        BOOL IsNeedLeach(CHvString& strPlate);

        //�����ĳ�ʼ��
        BOOL InitLeach();
        BOOL InitReplace();
        BOOL InitCompaty();

        //�������ʱЧ��
        BOOL CheckRuleTimer( const CHvString& strComment, const CRuleTime& crtime );
        //������Ŀ��Ŷ������Ƿ񸽺�
        BOOL CheckRuleRely( const CHvString& strComment, const CHvString& strPlateInfo );
        //��鳵�������Ƿ����
        //��������
        // 0:��ͨ������, 1:˫����, 2:Ħ�г���
        BOOL CheckPlateType( const CHvString& strComment, const CHvString& strPlateInfo );
        //��鳵������
        BOOL CheckPlateSpeed( const CHvString& strComment, const CHvString& strPlateInfo );
        //�����������
        BOOL CheckReverseRun( const CHvString& strComment, const CHvString& strPlateInfo );
	      //��鳵��������
	      BOOL CheckRoadNumber( const CHvString& strComment, const CHvString& strPlateInfo );
        //�ж�lpszPattern�Ƿ����Ƴ�lpszTarget
        BOOL Match( LPCSTR lpszPattern, LPCSTR lpszTarget );
        //�ж����������Ƿ񽻲�
        BOOL IsCross( LPCSTR lpszLeft, LPCSTR lpszRight );
        //���ɼ����򱨸�
        void MakeReport( LPSTR lpszReport, int iLen );
        //������
        CHvString CheckRule( CHvString& strLeft, CHvString& strRight,
            CHvString& strStyle);

    public:
        //�Ƚ���������İ�����ϵ, -1:�����򻥲�����,0:���,1:����,2:������,-2:����
        int  RuleCompare( LPCSTR lpszLeft, LPCSTR lpszRight );

    private:
        BOOL m_fInitialize;
        PROCESS_RULE *m_pProcessRule;
        //Ӧ�õ��Ĺ���
        CHvString m_strApplied;
        //������Ϣ
        CHvString m_strOther;
        //��������
        static const int MAX_RULE_COUNT = 150;
        //�����
        CHvList<LEACH_INFO> m_lstLeach;
        CHvList<REPLACE_INFO> m_lstReplace;
        CHvList<COMPATY_WORD> m_lstCompaty;
    };
}

extern HRESULT GetLeachData( CParamStore* pParam, BYTE8* pBuffer, DWORD32* pcbData );
extern HRESULT GetReplaceData( CParamStore* pParam, BYTE8* pBuffer, DWORD32* pcbData );
extern HRESULT GetCompatyData( CParamStore* pParam, BYTE8* pBuffer, DWORD32* pcbData );


#endif
