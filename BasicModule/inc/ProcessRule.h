#ifndef __PROCESS_RULE_H__
#define __PROCESS_RULE_H__
#define MAX_OUTPUT_BUF_LEN 32*1024
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
	BOOL fOutPutFilterInfo;		// ���������Ϣ

    _ProcessRule()
    {
        swpa_strcpy(szCompatyRule, "NULL");
        swpa_strcpy(szReplaceRule, "NULL");
        swpa_strcpy(szLeachRule, "NULL");
        fCompaty = TRUE;
        fReplace = TRUE;
        fLoopReplace = TRUE;
        fLeach = TRUE;
		fOutPutFilterInfo = FALSE;
    };

} PROCESS_RULE;  //�������
#endif
