//���ļ������ʽ����ΪWINDOWS-936��ʽ

#ifndef  _INCLUDE_HVRESULTFILTER_H_
#define  _INCLUDE_HVRESULTFILTER_H_
#include "SWBaseFilter.h"
#include "ProcessRule.h"
#include "SWMessage.h"

//ȫ��ת���
BOOL SBC2DBC( LPCSTR lpszSrc, LPSTR lpszDest );

//����ʱ���ʽ
class CRuleTime
{
public:
    CRuleTime();
    ~CRuleTime();

public:
    BOOL SetTime( const INT iHour, const INT iMinute, const INT iSecond );
    BOOL SetTime( LPCSTR lpszTime );

    INT GetHour() const
    {
        return m_iHour;
    }
    INT GetMinute() const
    {
        return m_iMinute;
    }
    INT GetSecond() const
    {
        return m_iSecond;
    }

    BOOL operator == (const CRuleTime& crt) const;
    BOOL operator > (const CRuleTime& crt) const;
    BOOL operator < (const CRuleTime& crt) const;
    BOOL operator >= (const CRuleTime& crt) const;
    BOOL operator <= (const CRuleTime& crt) const;

public:
    //ʱ�� crtNow �Ƿ����� crtLeft �� crtRight ������
    static BOOL IsCourseOfTime( const CRuleTime& crtLeft, const CRuleTime& crtRight, const CRuleTime& crtNow );

private:
    INT m_iHour;
    INT m_iMinute;
    INT m_iSecond;
};


///////////////////////////////////////////////////////////////////////////
//                           ������
/////////////////////////////////////////////////////////////////////////////////////////

//��lpszFileName����һ��С��lpszSection�����ݵ�lpszOutStr, �ṹΪ key=value\0
//INT GetFileSection( LPCSTR lpszSection, LPSTR lpszOutStr, INT iLen, CParamStore* pParam, LPCSTR lpszFileName = NULL );
//�õ���ǰʱ��
void GetTime( CRuleTime& crtime );

//���ƴ���������Ϣ
typedef struct ResultInfo
{
    CSWString strPlate;
    BOOL    fAlarm;
    BOOL    fLeach;
	INT		iLeachType;
	INT		iPT;
    CSWString strFolder;
    CSWString strOther;
    CSWString strApplied;
}RESULT_INFO;

//���˹���ṹ
typedef struct LeachInfo
{
    CSWString strPattern;
    BOOL fLeach;
	INT iLeachType;
    CSWString strComment;
}LEACH_INFO;

//��������ṹ
typedef struct AlarmInfo
{
    CSWString strPattern;
    BOOL fAlarm;
    CSWString strComment;
}ALARM_INFO;

//����Ŀ¼�Ĺ���ṹ
typedef struct FolderInfo
{
    CSWString strPattern;
    CSWString strFolder;
    CSWString strComment;
}FOLDER_INFO;

//�����滻�Ĺ���ṹ
typedef struct ReplaceInfo
{
    CSWString strPattern;
    CSWString strResult;
    CSWString strComment;
}REPLACE_INFO;

//ͨ�������ṹ
typedef struct  CompatyWord
{
    CSWString strPattern;
    CSWString strResult;
}COMPATY_WORD;

class CSWResultFilter : public CSWBaseFilter, public CSWMessage
{
		CLASSINFO(CSWResultFilter, CSWBaseFilter)
public:
	  CSWResultFilter();
    CSWResultFilter(PROCESS_RULE *pRule);
    virtual ~CSWResultFilter(void);
		virtual HRESULT Receive(CSWObject* obj);
public:
    //��ʼ������
    HRESULT InitRule(PVOID pProcessRule);

    //����
    BOOL FilterProcess(RESULT_INFO* pResultInfo);

private:
    //�����滻
    BOOL ReplacePlate( CSWString& strPlate, const BOOL& fLoopReplace, INT& iPT );
    void DoReplace(CSWString& strPlate, REPLACE_INFO& replaceinfo);
    //�Ƿ�Ҫ����
    INT IsNeedLeach(CSWString& strPlate, INT& iPT);

    //�����ĳ�ʼ��
    BOOL InitLeach();
    BOOL InitReplace();
    BOOL InitCompaty();

    //�������ʱЧ��
    BOOL CheckRuleTimer( const CSWString& strComment, const CRuleTime& crtime );
    //������Ŀ��Ŷ������Ƿ񸽺�
    BOOL CheckRuleRely( const CSWString& strComment, const CSWString& strPlateInfo );
    //��鳵�������Ƿ����
    //��������
    // 0:��ͨ������, 1:˫����, 2:Ħ�г���
    BOOL CheckPlateType( const CSWString& strComment, const CSWString& strPlateInfo );
    //��鳵������
    BOOL CheckPlateSpeed( const CSWString& strComment, const CSWString& strPlateInfo );
    //�����������
    BOOL CheckReverseRun( const CSWString& strComment, const CSWString& strPlateInfo );
    //��鳵��������
    BOOL CheckRoadNumber( const CSWString& strComment, const CSWString& strPlateInfo );
    //�ж�lpszPattern�Ƿ����Ƴ�lpszTarget
    BOOL Match( LPCSTR lpszPattern, LPCSTR lpszTarget );
    //�ж����������Ƿ񽻲�
    BOOL IsCross( LPCSTR lpszLeft, LPCSTR lpszRight );
    //���ɼ����򱨸�
    void MakeReport( LPSTR lpszReport, INT iLen );
    //������
    CSWString CheckRule( CSWString& strLeft, CSWString& strRight,
        CSWString& strStyle);
	//���Υ����������
	BOOL CheckPT( const CSWString& strComment, INT& iPT );

	HRESULT OnOverlayPosChanged(WPARAM wParam, LPARAM lParam);

public:
    //�Ƚ���������İ�����ϵ, -1:�����򻥲�����,0:���,1:����,2:������,-2:����
    INT  RuleCompare( LPCSTR lpszLeft, LPCSTR lpszRight );
	SW_BEGIN_DISP_MAP(CSWResultFilter,CSWBaseFilter)
        SW_DISP_METHOD(InitRule, 1)
	SW_END_DISP_MAP()

	SW_BEGIN_MESSAGE_MAP(CSWResultFilter, CSWMessage)
	    SW_MESSAGE_HANDLER(MSG_RESULT_FILTER_OVERLAY_POS_CHANGED, OnOverlayPosChanged)
	SW_END_MESSAGE_MAP()
private:
    BOOL m_fInitialize;
    PROCESS_RULE *m_pProcessRule;
    //Ӧ�õ��Ĺ���
    CSWString m_strApplied;
    //������Ϣ
    CSWString m_strOther;
    //��������
    static const INT MAX_RULE_COUNT = 150;
    //�����
    CSWList<LEACH_INFO> m_lstLeach;
    CSWList<REPLACE_INFO> m_lstReplace;
    CSWList<COMPATY_WORD> m_lstCompaty;
};
REGISTER_CLASS(CSWResultFilter)
#endif

