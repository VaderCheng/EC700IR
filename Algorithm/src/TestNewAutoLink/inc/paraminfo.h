#ifndef  _PARAM_INFO_H_
#define _PARAM_INFO_H_

typedef struct {
char* chIniParamName;	//INI������
char* chIniSectionName;	//INI������
char* chXmlParamName;	//XML������
char* chXmlSectionName;	//XML������
char* chParamDefault;	//Ĭ��ֵ
char* chParamMin;		//��Сֵ
char* chParamMax;		//���ֵ
char* chParamChineseName;//������
char* chParamContext;	//ע��
int iParamRank;			//����
char* chType;			//��������
} INI_PARAM_INFO;

typedef struct{
char* pszParentName;	//�����
char* pszEnglishName;	//Ӣ����
char* pszChineseName;	//��Ӧ��������
}EN_TO_CH;


extern INI_PARAM_INFO rgIniParamInfo[];
extern EN_TO_CH rgstrEnToCh[],rgstrEnToChEPolice[];

#endif