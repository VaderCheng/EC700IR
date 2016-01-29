#ifndef __SWMESSAGESTRUCT_H__
#define __SWMESSAGESTRUCT_H__

/******************************************************************************

				  ��Ȩ���� (C), 2015-2023, sw

 ******************************************************************************
  �� �� ��   : SWMessageStruct.h
  �� �� ��   : ����
  ��	��   : ����
  ��������   : 2015��8��14��
  ����޸�   :
  ��������   : ������Ϣ��صĽṹ�嶨��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2015��8��14��
	��    ��   : ����
	�޸�����   : ����

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�   								*
 *----------------------------------------------*/
#include "SWFC.h"

/*----------------------------------------------*
 * �궨��   							  *
 *----------------------------------------------*/
#define MSG_SHM_PROCESS_LEN   200	  //Ӧ�ó���������󳤶�
#define MSG_SHM_KEY_FILE_LEN  (MSG_SHM_PROCESS_LEN+5)  //key�ļ�����󳤶�

/*----------------------------------------------*
 * �������� 							*
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����   						  *
 *----------------------------------------------*/

/* BEGIN: Modified by huangdch, 2015/9/5 
*���ֵ��Ҫ����ƽ̨��Ҫ�����޸�,���֧��6��*/
#define MSG_SHM_PROCESS_NUMBER	    6                     //��Ҫ���̼佻������������
#define MSG_SHM_NODE_NUMBER 	6   					  //ÿ��Ӧ�ó��������Ĺ����ڴ�ڵ���
#define MSG_SHM_SEM_TOTAIL  	(MSG_SHM_NODE_NUMBER*2+2) //�ź�����

/*ֻ����Ϣ������Сֵ��Χ��Ӧ�Ĺ����ڴ�keyֵ
ͨ��Id�ı�ſ��Բ��ҵ���Ӧ�Ĺ����ڴ���keyֵ
*/
typedef struct _STMessageRangNode
{
	DWORD dwKey; //�����ڴ�keyֵ
	DWORD dwStartID; //��ϢId��minֵ
	DWORD dwEndID; //��ϢId��maxֵ
}STMessageRangNode;

/*ƽ̨��صĽ�����Ϣ�ڵ�:��¼����ƽ̨�������̵������Ϣ
*/
typedef struct _STProcessManageNode
{
	DWORD iProcessId; //����Id,�����ڽ��յ��㲥��Ϣ���������ֶ�,Ŀǰδ�õ�
	CHAR szProcessName[MSG_SHM_PROCESS_LEN];//Ӧ�ó�������
	DWORD dwKey; //�������keyֵ���Բ��ҵ���Ӧ�Ĺ����ڴ�����Ϣ
	void* pShm; //�����ڴ������ָ�� dwKey
	CSWMutex* pMutex; //��������ָ�� 
	CSWSemaphore* pSem[MSG_SHM_SEM_TOTAIL];  //�ź���������ָ������
}STProcessManageNode;

typedef struct _STProcessManageCur
{
	DWORD iProcessId; //����Id,�����ڽ��յ��㲥��Ϣ���������ֶ�,Ŀǰδ�õ�
	CHAR szProcessName[MSG_SHM_PROCESS_LEN];//Ӧ�ó�������

	DWORD dwKey; //�������keyֵ���Բ��ҵ���Ӧ�Ĺ����ڴ�����Ϣ

	void* pShm; //�����ڴ������ָ�� dwKey
}STProcessManageCur;

/* BEGIN: Modified by huangdch, 2015/8/31 
*���̼�ͨ����Ϣid����,һ���80000��ʼ���*/
//enum
//{
//	SHM_PROCESS_MEM_PHONE	= 80010, //�����
//	SHM_PROCESS_MEM_CAPTURE	= 80030, //�ɼ�
//	SHM_PROCESS_MEM_ENCODE	= 80050, //ѹ��
//	SHM_PROCESS_MEM_SEND	= 80070, //����
//	
//	SHM_PROCESS_NUMBER	 = 4 //��Ӧ��Ӧ�ó���Ľ�����,Ҫ��֤���ֵ����ȷ��
//};


/*************************************************************************
*�������̼�ͨ�ŵ���Ϣ�ṹ����Դ����￪ʼ����
**************************************************************************/

typedef struct _STSendMig
{
	int a;
	int b;
}STSendMig;


typedef struct _STCaptureMig
{
	int a;
	int b;
}STCaptureMig;


typedef struct _STEncodeMig
{
	int a;
	int b;
}STEncodeMig;


typedef struct _STPhoneMig
{
	int a;
	int b;
}STPhoneMig;


#endif

