/**
* @file swpa_file_private.c
* @brief ʵ���ļ�ģ���ڲ�ʹ�� ����غ���
* 
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-02-27
* @version 1.0
*/


#include "swpa_file_private.h"
#include <stdlib.h>


/**
* @brief �����ļ���д��
*
* 
* @param [inout] pheader �ļ�ͷ�ṹ��ָ��
* @param [in] filename  �ļ���
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_INVALIDARG : �����Ƿ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_file_create_rwlock(
	SWPA_FILEHEADER * pheader, 
	const char * filename
)
{
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(NULL != pheader);    	//pheaderָ��ǿ�
	SWPA_FILE_CHECK(NULL != filename);			//filenameָ��ǿ�
	SWPA_FILE_CHECK(swpa_strcmp(filename, ""));	//filename�ǿ��ַ���

	//��ӡ����
	SWPA_FILE_PRINT("pheader=0x%08x\n", (unsigned int)pheader);
	SWPA_FILE_PRINT("filename=%s\n", filename);

	//if (SWPAR_OK != swpa_mutex_create(&pheader->rwlock, filename))
	if (SWPAR_OK != swpa_mutex_create(&pheader->rwlock, NULL))
	{
		SWPA_FILE_PRINT("Err: failed to create _rwlock %s ! [%d]\n", filename, SWPAR_FAIL);		
		return SWPAR_FAIL;
	}

	return SWPAR_OK;	
}



/**
* @brief �����������ļ�����ģʽ
*
* 
* @param [inout] pheader �ļ�ͷ�ṹ��ָ��
* @param [in] mode ����ģʽ�ַ���
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_INVALIDARG : �����Ƿ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_file_parse_mode(
	SWPA_FILEHEADER * pheader, 
	const char * mode
)
{
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(NULL != pheader);    	//pheaderָ��ǿ�
	SWPA_FILE_CHECK(NULL != mode);			//modeָ��ǿ�
	SWPA_FILE_CHECK(swpa_strcmp(mode, ""));	//mode�ǿ��ַ���

	//��ӡ����
	SWPA_FILE_PRINT("pheader=0x%08x\n", (unsigned int)pheader);
	SWPA_FILE_PRINT("mode=%s\n", mode);

	
	pheader->mode = 0;																
	if (0 == swpa_strcmp(mode, "r"))												
	{																				
		SWPA_FILE_SET_MODE((pheader), SWPA_FILE_READ_MODE); /*//����Ϊ�ɶ�*/	
		return SWPAR_OK;
	}																				
	else if (0 == swpa_strcmp(mode, "r+"))											
	{																				
		SWPA_FILE_SET_MODE((pheader), SWPA_FILE_READ_MODE); /*//����Ϊ�ɶ�*/		
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_WRITE_MODE); /*//����Ϊ��д*/ 	
		
		return SWPAR_OK;
	}																				
	else if (0 == swpa_strcmp(mode, "w"))											
	{																				
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_WRITE_MODE); /*//����Ϊ��д*/ 
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_CREATE_MODE); /*//����Ϊ�ļ��������򴴽�*/ 
		return SWPAR_OK;
	}																				
	else if (0 == swpa_strcmp(mode, "w+"))											
	{																				
		SWPA_FILE_SET_MODE((pheader), SWPA_FILE_READ_MODE); /*//����Ϊ�ɶ�*/		
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_WRITE_MODE); /*//����Ϊ��д*/ 	
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_CREATE_MODE); /*//����Ϊ�ļ��������򴴽�*/ 
		return SWPAR_OK;
	}
	else if (0 == swpa_strcmp(mode, "wt"))				//�ı��ļ�								
	{																				
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_WRITE_MODE); /*//����Ϊ��д*/ 
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_CREATE_MODE); /*//����Ϊ�ļ��������򴴽�*/ 
		return SWPAR_OK;
	}																				
	else if (0 == swpa_strcmp(mode, "wt+"))				//�ı��ļ�			
	{																				
		SWPA_FILE_SET_MODE((pheader), SWPA_FILE_READ_MODE); /*//����Ϊ�ɶ�*/		
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_WRITE_MODE); /*//����Ϊ��д*/ 	
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_CREATE_MODE); /*//����Ϊ�ļ��������򴴽�*/ 
		return SWPAR_OK;
	}
	else if (0 == swpa_strcmp(mode, "wb"))				//�������ļ�							
	{																				
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_WRITE_MODE); /*//����Ϊ��д*/ 
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_CREATE_MODE); /*//����Ϊ�ļ��������򴴽�*/ 
		return SWPAR_OK;
	}																				
	else if (0 == swpa_strcmp(mode, "wb+"))				//�������ļ�					
	{																				
		SWPA_FILE_SET_MODE((pheader), SWPA_FILE_READ_MODE); /*//����Ϊ�ɶ�*/		
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_WRITE_MODE); /*//����Ϊ��д*/ 	
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_CREATE_MODE); /*//����Ϊ�ļ��������򴴽�*/ 
		return SWPAR_OK;
	}			
	else if (0 == swpa_strcmp(mode, "a"))											
	{																				
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_APPEND_MODE); /*//����Ϊ׷��*/	
		return SWPAR_OK;
	}																				
	else if (0 == swpa_strcmp(mode, "a+"))											
	{
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_APPEND_MODE); /*//����Ϊ׷��*/	
		SWPA_FILE_SET_MODE(pheader, SWPA_FILE_READ_MODE); /*//����Ϊ�ɶ�*/		
		/*SWPA_FILE_SET_MODE(pheader, SWPA_FILE_WRITE_MODE); *//*//����Ϊ��д*/
		return SWPAR_OK;
	}
	else
	{
		SWPA_FILE_PRINT("Err: mode (%s) is invalid! [%d]\n", mode, SWPAR_INVALIDARG);
		return SWPAR_INVALIDARG;
	}
	
}



