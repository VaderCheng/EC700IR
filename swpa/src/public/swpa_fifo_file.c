/**
* @file swpa_fifo_file.c 
* @brief ʵ��FIFO��صĲ�������
*
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-03-11
* @version 1.0
*/

#include "swpa_file.h"
#include "swpa_file_private.h"
#include "swpa_fifo_file.h"

#include <stdio.h>
#include <errno.h>
#include <sys/io.h>
#include <unistd.h>
#include <sys/stat.h>  

#include  <sys/types.h> 
#include  <sys/time.h> 
#include  <sys/select.h> 
#include <fcntl.h>


typedef struct _FIFO_FILE_INFO
{
	char * 	filename;
	int  	pfile;//FILE * pfile;
	int	   	rd_timeout_ms;
	int	   	wr_timeout_ms;
	
}FIFO_FILE_INFO;




/**
* @brief ��ʵ���ܵ��ļ�
*
* 
* @param [in] filename �ļ���
* @param [in] mode �ļ���ģʽ,�����¼���ģʽ:
* - "r"  ����ʽ��
* - "w"  д��ʽ��
* - "r+"  ��д��ʽ��
* - "w+"  ��д��ʽ��
* @retval �ļ�������: �ɹ���(ʵ���Ͼ���SWPA_FILEHEADER�ṹ��ָ��)
* @retval SWPAR_FAIL : ��ʧ��
* @retval SWPAR_INVALIDARG : �����Ƿ�
*/
int swpa_fifo_file_open(
	const char * filename, 
	const char * mode
)
{
	int ret = SWPAR_OK;
	SWPA_FILEHEADER * pheader = NULL;
	FIFO_FILE_INFO * pinfo = NULL;
	//FILE * pfile = NULL;
	int modeval = 0;
	struct stat stat_info;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(NULL != filename);	//filenameָ��ǿ�
	SWPA_FILE_CHECK(0 != swpa_strcmp(filename, ""));//filename�ǿ��ַ���
	SWPA_FILE_CHECK(NULL != mode);		//modeָ��ǿ�
	SWPA_FILE_CHECK(0 != swpa_strcmp(mode, "")); //mode�ǿ��ַ���

	//��ӡ����
	SWPA_FILE_PRINT("filename=%s\n", filename);
	SWPA_FILE_PRINT("mode=%s\n", mode);


	pheader = (SWPA_FILEHEADER *)swpa_mem_alloc(sizeof(*pheader));
	if (NULL == pheader)
	{
		ret = SWPAR_OUTOFMEMORY;
		SWPA_FILE_PRINT("Err: No enough memory for pheader! [%d]\n", ret);

		return ret;
	}
	swpa_memset(pheader, 0, sizeof(*pheader));
	
	ret = swpa_file_create_rwlock(pheader, filename);
	if (SWPAR_OK != ret)
	{
		SWPA_FILE_PRINT("Err: swpa_file_create_rwlock() failed! [%d]\n", ret);
		ret = SWPAR_FAIL;
		goto _ERR_HANDLING; 
	}
	

	pinfo = (FIFO_FILE_INFO *)swpa_mem_alloc(sizeof(*pinfo));
	if (NULL == pinfo)
	{
		ret = SWPAR_OUTOFMEMORY;
		SWPA_FILE_PRINT("Err: No enough memory for pinfo! [%d]\n", ret);

		goto _ERR_HANDLING;
	}
	swpa_memset(pinfo, 0, sizeof(*pinfo));
	

	pinfo->filename = (char *)swpa_mem_alloc(swpa_strlen(filename)+1);
	if (NULL == pinfo->filename)
	{
		ret = SWPAR_OUTOFMEMORY;
		SWPA_FILE_PRINT("Err: No enough memory for pinfo->filename! [%d]\n", ret);

		goto _ERR_HANDLING;
	}

	swpa_strcpy(pinfo->filename, filename);

	pinfo->rd_timeout_ms = -1; //Ĭ��������ʽ��
	pinfo->wr_timeout_ms = -1; //Ĭ��������ʽд
	

	//�����ļ���дģʽ��־
	ret = swpa_file_parse_mode(pheader, mode);
	if (SWPAR_OK != ret)
	{
		SWPA_FILE_PRINT("Err: swpa_file_parse_mode() failed! [%d]\n", ret);		
		goto _ERR_HANDLING; 
		
	}

	if (SWPA_FILE_IS_APPEND_MODE(pheader))
	{
		ret = SWPAR_INVALIDARG;
		SWPA_FILE_PRINT("Err: mode (%s) is invalid for fifo file! [%d]\n", mode, ret);		
		goto _ERR_HANDLING; 
	}


	//�ܵ��Ѵ���
	if ( 0 == access(filename, F_OK))
	{
		if ( 0 != stat(filename,  &stat_info))
	 	{
			ret = SWPAR_FAIL;
			SWPA_FILE_PRINT("Err: stat(%s) failed! (%d: %s) [%d]\n", filename, errno, strerror(errno), ret);
			
			goto _ERR_HANDLING;
		}

		if (!(stat_info.st_mode & S_IFIFO))//(S_IFIFO != (stat_info.st_mode & S_IFIFO))
		{
			ret = SWPAR_FAIL;
			SWPA_FILE_PRINT("Err: %s exists and it is not a fifo file! [%d]\n", filename, ret);
			
			goto _ERR_HANDLING;
		}
	}
	//�ܵ������ڣ�����mkfifo
	else
	{
		if (0 != mkfifo(filename, S_IFIFO|0666))
		{
			ret = SWPAR_FAIL;
			SWPA_FILE_PRINT("Err: mkfifo(%s) failed! (%d: %s) [%d]\n", filename, errno, strerror(errno), ret);
			
			goto _ERR_HANDLING;
		}
	}

	
	//��ʵ���ܵ�
	//modeval = O_NONBLOCK; //������ʽ
	if (SWPA_FILE_IS_READ_MODE(pheader))
	{
		modeval |= (SWPA_FILE_IS_WRITE_MODE(pheader) ? O_RDWR : O_RDONLY);
	}
	else if (SWPA_FILE_IS_WRITE_MODE(pheader))
	{
		modeval |= O_WRONLY;
	}

	pinfo->pfile = open(filename, modeval); 
	if (-1 == pinfo->pfile)
	{
		ret = SWPAR_FAIL;
		SWPA_FILE_PRINT("Err: failed to open %s (%d: %s) [%d]\n", filename, errno, strerror(errno), ret);
		
		goto _ERR_HANDLING;
	}	
		
	pheader->device_param = (int)pinfo; 
	pheader->file_open	  = swpa_fifo_file_open;
	pheader->file_close   = swpa_fifo_file_close;
	pheader->file_seek	  = swpa_fifo_file_seek;
	pheader->file_eof	  = swpa_fifo_file_eof;
	pheader->file_tell	  = swpa_fifo_file_tell;
	pheader->file_ioctl   = swpa_fifo_file_ioctl;
	pheader->file_read	  = swpa_fifo_file_read;
	pheader->file_write   = swpa_fifo_file_write;

	return (int)pheader;

//����������
_ERR_HANDLING:

	if (0 != pheader->rwlock)
	{
		swpa_mutex_delete(&pheader->rwlock);
		pheader->rwlock = 0;
	}
	
	if (NULL != pheader)
	{
		SWPA_FILE_CHECK_RET(swpa_mem_free(pheader), SWPAR_FAIL);
		pheader = NULL;
	}

	if (NULL != pinfo->filename)
	{
		SWPA_FILE_CHECK_RET(swpa_mem_free(pinfo->filename), SWPAR_FAIL);
		pinfo = NULL;
	}

	if (NULL != pinfo)
	{
		SWPA_FILE_CHECK_RET(swpa_mem_free(pinfo), SWPAR_FAIL);
		pinfo = NULL;
	}
	
	return ret;
	
}





/**
* @brief �ر��ļ�
*
* 
* @param [in] fd �ļ�������
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_fifo_file_close(
	int fd
)
{
	int ret = SWPAR_OK;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	FIFO_FILE_INFO * pinfo = NULL;

	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		//fd ���벻����0

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);

	pinfo = (FIFO_FILE_INFO *)pheader->device_param;


	//�ر��ļ�
	SWPA_FILE_CHECK_RET(close(pinfo->pfile), SWPAR_FAIL);

	//�����д�뷽�رչܵ������ڹرպ�ɾ���ùܵ���������?
	if (SWPA_FILE_IS_WRITE_MODE(pheader))
	{
	//	unlink(pinfo->filename);
	}	


	//�ͷ���Դ
	pheader->device_param = (int)NULL;
	pheader->mode	 	  = 0;
	pheader->file_open	  = NULL;
	pheader->file_close   = NULL;
	pheader->file_seek	  = NULL;
	pheader->file_eof	  = NULL;
	pheader->file_tell	  = NULL;
	pheader->file_ioctl   = NULL;
	pheader->file_read	  = NULL;
	pheader->file_write   = NULL;

	if (0 != pheader->rwlock)
	{
		swpa_mutex_delete(&pheader->rwlock);
		pheader->rwlock = 0;
	}
	
	SWPA_FILE_CHECK_RET(swpa_mem_free(pheader), SWPAR_FAIL);
	pheader = NULL;

	SWPA_FILE_CHECK_RET(swpa_mem_free(pinfo->filename), SWPAR_FAIL);
	pinfo->filename = NULL;
	
	SWPA_FILE_CHECK_RET(swpa_mem_free(pinfo), SWPAR_FAIL);
	pinfo = NULL;

	return ret;
}





/**
* @brief �ж��Ƿ����ļ�β��
* @param [in] fd �ļ�������
* @retval 0 : û�е��ļ�β����
* @retval -1 : �����ļ�β��
* @attention ��������ķ���ֵ�Ƚ����⣬������Դ�
*/
int swpa_fifo_file_eof(
	int fd
)
{
#if 0
	int ret = SWPAR_OK;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	FIFO_FILE_INFO * pinfo = NULL;

	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		//fd ���벻����0

	//������ӡ
	SWPA_PRINT_INT(fd);
	
	pinfo = (FIFO_FILE_INFO *)pheader->device_param;

	//�ж�
	// 0 : ����δ���ļ�ĩβ
	//-1: �������ļ�ĩβ�������ǳ���!
	return (feof(pinfo->pfile)) ? (-1) : (0);
#else
	return SWPAR_NOTIMPL;
#endif
}


/**
* @brief �ı��ļ�ָ��
*
* 
* @param [in] fd �ļ�������
* @param [in] offset ָ���ƫ����
* @param [in] pos ָ��ƫ��������ʼλ�ã�ȡֵ��Χ��: 
* - �ļ�ͷ0(SWPA_SEEK_SET)
* - ��ǰλ��1(SWPA_SEEK_CUR)
* - �ļ�β2(SWPA_SEEK_END)
* @retval SWPAR_NOTIMPL : δʵ��
*/
int swpa_fifo_file_seek(
	int fd, 
	int offset, 
	int pos
)
{
	return SWPAR_NOTIMPL;
}





/**
* @brief ��õ�ǰ�ļ�ָ��λ��
*
* 
* @param [in] fd �ļ�������
* @retval SWPAR_NOTIMPL : δʵ��
*/
int swpa_fifo_file_tell(
	int fd
)
{
	return SWPAR_NOTIMPL;
}






/**
* @brief ���ļ��������Ŀ���
*
* 
* @param [in] fd �ļ�������
* @param [in] cmd �����ʶ
* @param [in] args �����־����
* @retval SWPAR_NOTIMPL : δʵ��
*/
int swpa_fifo_file_ioctl(
	int fd, 
	int cmd, 
	void* args
)
{
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	FIFO_FILE_INFO * pinfo = (FIFO_FILE_INFO *)pheader->device_param;	

	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);    	//fd ���벻����0
	//SWPA_FILE_CHECK(0 < cmd);    	//cmdȡֵû���޶�����mark��
	//SWPA_FILE_CHECK(NULL != args);    	//args �ڲ�ͬcmd�¿���ΪNULL����mark��

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);	
	SWPA_FILE_PRINT("cmd=%d\n", cmd);	
	SWPA_FILE_PRINT("args=0x%08x\n", (unsigned int)args);
	
	switch (cmd)
	{		
		case SWPA_FILE_SET_READ_TIMEOUT:
		{	
			int timeout = *(int*)args;

			//SWPA_FILE_CHECK(0 < timeout);    	//��ʱʱ���������
			
			pinfo->rd_timeout_ms = timeout;
		}
			
		break;

		case SWPA_FILE_SET_WRITE_TIMEOUT:
		{	
			int timeout = *(int*)args;

			//SWPA_FILE_CHECK(0 < timeout);    	//��ʱʱ���������
			
			pinfo->wr_timeout_ms = timeout;
		}
			
		break;

		default:
			SWPA_FILE_PRINT("Err: invalid cmd for fifo file! [%d]\n", SWPAR_INVALIDARG);
			return SWPAR_INVALIDARG;
	}

	
}






/**
* @brief ���ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf �����ݻ�����������ǿ�
* @param [in] size ��������С���������0����λΪ�ֽ�
* @param [out] ret_size ���ض��������ݴ�С���������ĸ���ֵ���ɴ�NULL
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_fifo_file_read(
	int fd, 
	void * buf, 
	int size, 
	int * ret_size
)
{
	int ret = SWPAR_OK;
	int bytes = 0;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	FIFO_FILE_INFO * pinfo = NULL;
	fd_set  rdset;
	struct timeval timeout, *ptimeout = NULL;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		//fd ���벻����0
	SWPA_FILE_CHECK(NULL != buf);		//buf�ǿ�
	SWPA_FILE_CHECK(0 < size);		//size �������0
	//SWPA_FILE_CHECK(NULL != ret_size);		//Note: ret_size������NULL����ע�͵�
	
	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);
	SWPA_FILE_PRINT("buf=0x%08x\n", (unsigned int)buf);
	SWPA_FILE_PRINT("size=%d", size);
	SWPA_FILE_PRINT("ret_size=0x%08x\n", (unsigned int)ret_size);

	pinfo = (FIFO_FILE_INFO *)pheader->device_param;

	FD_ZERO(&rdset);
	FD_SET(pinfo->pfile, &rdset);
	if (0 <= pinfo->rd_timeout_ms)
	{
		timeout.tv_sec = pinfo->rd_timeout_ms / 1000; // ms to sec
		timeout.tv_usec = (pinfo->rd_timeout_ms % 1000) * 1000; // ms to usec

		ptimeout = &timeout;
	}
	switch (select(pinfo->pfile + 1, &rdset, NULL, NULL, ptimeout))
	{
		case 0: //timeout
		{
			ret = SWPAR_OUTOFTIME;
			SWPA_FILE_PRINT("Info: select timeout, nothing to read!\n");
		}
		break;
		
		case -1: //error occured
		{
			ret = SWPAR_FAIL;
			SWPA_FILE_PRINT("Err: select failed (%d:%s) [%d]!\n", errno, strerror(errno), ret);
		}
		break;
		
		default: //succeeded
		{			
			if (FD_ISSET(pinfo->pfile, &rdset))
			{
				//do read
				bytes = read(pinfo->pfile, buf, size);
				if (-1 == bytes)
				{
					ret = SWPAR_FAIL;
					SWPA_FILE_PRINT("Err: failed to read fifo %s (%d:%s) [%d]!\n", pinfo->filename, errno, strerror(errno), ret);
				}
				else if (0 == bytes)
				{
					ret = SWPAR_FAIL;
					SWPA_FILE_PRINT("Err: nothing to read %s [%d]!\n", pinfo->filename,  ret);
				}
				else if (bytes != size)
				{
					//if (0 == feof(pinfo->pfile))
					{
						ret = SWPAR_OK;//SWPAR_FAIL;
						SWPA_FILE_PRINT("Info: only read %d (!= %d)![%d]\n", bytes, size, ret);
					}
					//else
					{
						//�����ļ�ĩβ����ȡ����������С�ڸ����Ĵ�С�ǿ��ܵ�
					}
				}

				if (NULL != ret_size)
				{
					*ret_size = bytes;
				}
			}
			else
			{		
				ret = SWPAR_FAIL;
				SWPA_FILE_PRINT("Err: select confused! [%d]\n",  ret);
			}
		}
		break;
	}

	
	return ret;
}




/**
* @brief д�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf  д���ݻ�����
* @param [in] size ��������С
* @param [out] ret_size ����д������ݴ�С
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_fifo_file_write(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
)
{
	int ret = SWPAR_OK;
	int bytes = 0;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	FIFO_FILE_INFO * pinfo = NULL;
	fd_set  wrset;
	struct timeval timeout, *ptimeout = NULL;
	
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		//fd ���벻����0
	SWPA_FILE_CHECK(NULL != buf);		//buf�ǿ�
	SWPA_FILE_CHECK(0 < size);		//size �������0
	//SWPA_FILE_CHECK(NULL != ret_size);		//Note: ret_size������NULL����ע�͵�
	
	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);
	SWPA_FILE_PRINT("buf=0x%08x\n", (unsigned int)buf);
	SWPA_FILE_PRINT("size=%d\n", size);
	SWPA_FILE_PRINT("ret_size=0x%08x\n", (unsigned int)ret_size);

	pinfo = (FIFO_FILE_INFO *)pheader->device_param;
	

	FD_ZERO(&wrset);
	FD_SET(pinfo->pfile, &wrset);
	if (0 <= pinfo->rd_timeout_ms)
	{
		timeout.tv_sec = pinfo->wr_timeout_ms / 1000; // ms to sec
		timeout.tv_usec = (pinfo->wr_timeout_ms % 1000) * 1000; // ms to usec

		ptimeout = &timeout;
	}
	switch (select(pinfo->pfile + 1, NULL, &wrset, NULL, ptimeout))
	{
		case 0: //timeout
		{
			ret = SWPAR_OUTOFTIME;
			SWPA_FILE_PRINT("Info: select timeout, nothing to write!\n");
		}
		break;
		
		case -1: //error occured
		{
			ret = SWPAR_FAIL;
			SWPA_FILE_PRINT("Err: select failed (%d:%s) [%d]!\n", errno, strerror(errno), ret);
		}
		break;
		
		default: //succeeded
		{			
			if (FD_ISSET(pinfo->pfile, &wrset))
			{
				//do read
				bytes = write(pinfo->pfile, buf, size);
				if (-1 == bytes)
				{
					ret = SWPAR_FAIL;
					SWPA_FILE_PRINT("Err: failed to write fifo %s (%d:%s) [%d]!\n", pinfo->filename, errno, strerror(errno), ret);
				}
				else if (bytes != size)
				{
					ret = SWPAR_OK;;
					SWPA_FILE_PRINT("Info: only wrote %d (!= %d)![%d]\n", bytes, size, ret);
				}

				if (NULL != ret_size)
				{
					*ret_size = bytes;
				}
			}
			else
			{		
				ret = SWPAR_FAIL;
				SWPA_FILE_PRINT("Err: select confused! [%d]\n",  ret);
			}
		}
		break;
	}


	return ret;
}


