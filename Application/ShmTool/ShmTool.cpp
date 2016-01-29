#include "SWFC.h"
#include "swpa.h"
#include "SWMessage.h"

int OperShm(int agc, char* agv[]);

int main(int agc, char* agv[])
{
	OperShm(agc, agv);
    return 0;
}

/*****************************************************************************
 �� �� ��  : OperShm
 ��������  : �ͷŽ��̼�ͨ�ŵĹ����ڴ�
 �������  : int agc      
             char* agv[]  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ע������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��9��6��
    ��    ��   : huangdch
    �޸�����   : �����ɺ���

*****************************************************************************/
int OperShm(int agc, char* agv[])
{
	int iRet = 0;
	int keyId = -1;

	//���ӹ�����
	int state = -1;

	for ( int k = 0 ; k < MSG_SHM_PROCESS_NUMBER; k++ )
	{
		keyId = MSG_SHM_REGISTER_START_KEY + (k * 20);
	    iRet = swpa_normal_sharedmem_free(keyId, NULL);
		if  ( iRet != 0 )
		{
		    //SW_TRACE_DEBUG("destroy shm key=0x%08x(%d) failed, ret=[%d].\n", keyId, keyId, iRet);
		}
		else
		{
		    SW_TRACE_NORMAL("destroy shm key=0x%08x(%d) success.\n", keyId, keyId);
		}
	}

	for ( int k = 0 ; k < 4; k++ )
	{
		keyId = 10000 + k;
	    iRet = swpa_normal_sharedmem_free(keyId, NULL);
		if  ( iRet != 0 )
		{
		    //SW_TRACE_DEBUG("destroy shm key=0x%08x(%d) failed, ret=[%d].\n", keyId, keyId, iRet);
		}
		else
		{
		    SW_TRACE_NORMAL("destroy shm key=0x%08x(%d) success.\n", keyId, keyId);
		}
	}

	//ɾ���������������id
	int* init = (int*)swpa_mem_alloc((agc) * sizeof(int));
	if (NULL == init)
	{
	    return -1;
	}

	for ( int k = 1 ; k < agc ; k++ )
	{
		keyId = atoi(agv[k]);
		iRet = swpa_normal_sharedmem_free(keyId, NULL);
		if  ( iRet != 0 )
		{
		    SW_TRACE_DEBUG("destroy shm key=0x%08x(%d) failed, ret=[%d].\n", keyId, keyId, iRet);
		}
		else
		{
		    SW_TRACE_NORMAL("destroy shm key=0x%08x(%d) success.\n", keyId, keyId);
		}
	}
	swpa_mem_free(init);
	
	return 0;
}
