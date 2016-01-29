/**
* @file swpa_normal_file.c
* @brief ʵ�ִ����ļ���صĲ�������
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-02-27
* @version 1.0
*/

#include "swpa_file.h"
#include "swpa_file_private.h"
#include "swpa_normal_file.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


typedef struct tagNORMAL_FILE_INFO
{
	int offset;	//�ļ�ƫ����
	int fd;     //�ļ����
}NORMAL_FILE_INFO;

/**
* @brief �򿪴����ļ�
* 
* @param [in] filename �ļ���
* @param [in] mode �ļ���ģʽ,�����¼���ģʽ:
* - "r"��"br"  : ����ʽ��
* - "r+"��"br+"  : ��д��ʽ��
* - "w"��"bw"  :   д��ʽ��
* - "w+"��"bw+"  : ��д��ʽ�򿪣�����ļ����������ȴ����ļ�
* - "a"��"ba"  :   ׷�ӷ�ʽ��
* - "a+"��"ba+"  : ׷�ӷ�ʽ�򿪣��ɶ���д
* @retval �ļ�������: �ɹ���(ʵ���Ͼ���SWPA_FILEHEADER�ṹ��ָ��)
* @retval SWPAR_FAIL : ��ʧ��
* @retval SWPAR_INVALIDARG : �����Ƿ�
*/
int swpa_normal_file_open(
	const char*  filename, 
	const char * mode
)
{
	int ret = SWPAR_OK;
	SWPA_FILEHEADER * pheader = NULL;
	NORMAL_FILE_INFO* fInfo = NULL;
		
	//������Ч�Լ��
	SWPA_FILE_CHECK(NULL != filename); 	//filenameָ��ǿ�
	SWPA_FILE_CHECK(0 != swpa_strcmp(filename, ""));//filename�ǿ��ַ���
	SWPA_FILE_CHECK(NULL != mode); 		//modeָ��ǿ�
	SWPA_FILE_CHECK(0 != swpa_strcmp(mode, ""));	//mode�ǿ��ַ���

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
	
	fInfo = (NORMAL_FILE_INFO *)swpa_mem_alloc(sizeof(NORMAL_FILE_INFO));
	if (NULL == fInfo)
	{
		ret = SWPAR_OUTOFMEMORY;
		SWPA_FILE_PRINT("Err: No enough memory for fInfo! [%d]\n", ret);

		goto _ERR_HANDLING;
	}
	fInfo->offset = 0;
	
	//�����ļ���дģʽ��־
	ret = swpa_file_parse_mode(pheader, mode);
	if (SWPAR_OK != ret)
	{
		SWPA_FILE_PRINT("Err: swpa_file_parse_mode() failed! [%d]\n", ret);
		ret = SWPAR_FAIL;
		goto _ERR_HANDLING; 
	}	

	//���ļ�
	int flag = 0;
	if(SWPA_FILE_IS_READ_MODE(pheader) && SWPA_FILE_IS_WRITE_MODE(pheader))
	{
		flag = O_RDWR;
	}
	else if(SWPA_FILE_IS_READ_MODE(pheader) && SWPA_FILE_IS_APPEND_MODE(pheader))
	{
		flag = O_RDWR | O_APPEND;
	}
	else if(SWPA_FILE_IS_READ_MODE(pheader))
	{
		flag = O_RDONLY;
	}
	else if(SWPA_FILE_IS_WRITE_MODE(pheader))
	{
		flag = O_WRONLY|O_CREAT;
	}
	else if(SWPA_FILE_IS_APPEND_MODE(pheader))
	{
		flag = O_APPEND;
	}

	if (SWPA_FILE_IS_CREATE_MODE(pheader))
	{
		flag |= O_CREAT;
	}
	
	fInfo->fd = open(filename, flag, 0777);
	if(fInfo->fd < 0)
	{
		perror("Normal file open failed");
		ret = SWPAR_FAIL;
		SWPA_FILE_PRINT("Err: failed to open %s ! [%d]\n", filename, ret);
		
		goto _ERR_HANDLING;
	}	
	

	pheader->device_param = (int)fInfo;	
	
	pheader->file_open	  = swpa_normal_file_open;
	pheader->file_close   = swpa_normal_file_close;
	pheader->file_seek	  = swpa_normal_file_seek;
	pheader->file_eof	  = swpa_normal_file_eof;
	pheader->file_tell	  = swpa_normal_file_tell;
	pheader->file_ioctl   = swpa_normal_file_ioctl;
	pheader->file_read	  = swpa_normal_file_read;
	pheader->file_write   = swpa_normal_file_write;

	if(SWPA_FILE_IS_APPEND_MODE(pheader))
	{
		swpa_normal_file_seek((int)pheader, 0, SWPA_SEEK_END);
		SWPA_FILE_PRINT("Info: L%d: offset = %d\n", __LINE__, fInfo->offset);
	}
		
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
		swpa_mem_free(pheader);
		pheader = NULL;
	}
	
	if (NULL != fInfo)
	{
		swpa_mem_free(fInfo);
		fInfo = NULL;
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
int swpa_normal_file_close(
	int fd
)
{
	int ret = SWPAR_OK;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		//fd ���벻����0

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);

	//�ر��ļ�
	NORMAL_FILE_INFO* fInfo = (NORMAL_FILE_INFO *)pheader->device_param;
	SWPA_FILE_CHECK(0 != fInfo);
	fsync(fInfo->fd);
	close(fInfo->fd);


	//�ͷ���Դ
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
	SWPA_FILE_CHECK_RET(swpa_mem_free(fInfo), SWPAR_FAIL);

	return ret;
}






/**
* @brief �ж��Ƿ����ļ�β��
* @param [in] fd �ļ�������
* @retval 0  :  û�е����ļ�β��
* @retval -1 :���ļ�β����
* @attention ��������ķ���ֵ�Ƚ����⣬������Դ�
*/
int swpa_normal_file_eof(
	int fd
)
{
	int ret = SWPAR_OK;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;

	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		//fd ���벻����0

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);
	
	NORMAL_FILE_INFO* fInfo = (NORMAL_FILE_INFO *)pheader->device_param;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fInfo);
	
	struct stat buf;
	if(fstat(fInfo->fd, &buf))
	{
		return SWPAR_FAIL;
	}
	
	return buf.st_size > fInfo->offset + 1 ? 0 /*means FALSE*/: -1 /*means TRUE*/;
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
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_normal_file_seek(
	int fd, 
	int offset, 
	int pos
)
{
	int ret = SWPAR_OK;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		//fd ���벻����0
	SWPA_FILE_CHECK(SWPA_SEEK_SET == pos || SWPA_SEEK_CUR == pos || SWPA_SEEK_END == pos);		//pos���еĿ���ֵ

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);
	SWPA_FILE_PRINT("offset=%d\n", offset);
	SWPA_FILE_PRINT("pos=%d\n", pos);
	
	NORMAL_FILE_INFO* fInfo = (NORMAL_FILE_INFO *)pheader->device_param;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fInfo);
	
	struct stat buf;
	if(fstat(fInfo->fd, &buf))
	{
		return SWPAR_FAIL;
	}
	
	switch(pos)
	{
	case SWPA_SEEK_SET : 
		{
			if(buf.st_size < offset)
			{
				ret = SWPAR_FAIL;
			}
			else
			{
				fInfo->offset  = offset; 
			}
			break;
		}
	case SWPA_SEEK_CUR : 
		{
			if(buf.st_size < fInfo->offset + offset)
			{
				ret = SWPAR_FAIL;
			}
			else
			{
				fInfo->offset += offset; 
			}
			break;
		}
	case SWPA_SEEK_END :
		{
			if(buf.st_size < offset)
			{
				ret = SWPAR_FAIL;
			}
			else
			{
				fInfo->offset = buf.st_size - offset; 
			}
		}break;
	}
	
	return ret;
}



/**
* @brief ��õ�ǰ�ļ�ָ��λ��
*
* 
* @param [in] fd �ļ�������
* @retval �ļ�ָ��λ��: ִ�гɹ�
* @retval SWPAR_FAIL : ʧ��
* @attention ��������ķ���ֵ�Ƚ����⣬������Դ�
*/
int swpa_normal_file_tell(
	int fd
)
{
	int ret = SWPAR_OK;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		//fd ���벻����0

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);
	
	NORMAL_FILE_INFO* fInfo = (NORMAL_FILE_INFO *)pheader->device_param;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fInfo);
	
	return fInfo->offset;
}






/**
* @brief ���ļ��������Ŀ���
*
* 
* @param [in] fd �ļ�������
* @param [in] cmd �����ʶ
* @param [in] args �����־����
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
* @retval SWPAR_NOTIMPL : δʵ��
*/
int swpa_normal_file_ioctl(
	int fd, 
	int cmd, 
	void* args
)
{
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		//fd ���벻����0
	//SWPA_FILE_CHECK(cmd);		//
	//SWPA_FILE_CHECK(NULL != args);		//args�ǿ�
	
	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);
	SWPA_FILE_PRINT("cmd=%d\n", cmd);
	SWPA_FILE_PRINT("args=0x%08x\n", (unsigned int)args);

	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
		
	NORMAL_FILE_INFO* fInfo = (NORMAL_FILE_INFO *)pheader->device_param;

	switch (cmd)
	{
		case SWPA_FILE_IOCTL_TRANCATE:
		{
			SWPA_FILE_CHECK(NULL != args);		//args�ǿ�
			int* size = (int*)args;

			SWPA_FILE_CHECK(0 <= *size);		//size�Ǹ�

			if (0 != ftruncate(fInfo->fd, *size))
			{
				perror("ftrancate failed");
				return SWPAR_FAIL;
			}
		}
		break;

		default:
			//û��ʵ��
			return SWPAR_NOTIMPL;
	}

	
	return SWPAR_OK;
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
int swpa_normal_file_read(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
)
{
	int ret = SWPAR_OK;
	int bytes = 0;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	
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
	
	NORMAL_FILE_INFO* fInfo = (NORMAL_FILE_INFO *)pheader->device_param;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fInfo);

	if(!SWPA_FILE_IS_READ_MODE(pheader))
	{
		SWPA_FILE_PRINT("Err: file was not opened as Read mode!\n");
		return SWPAR_FAIL;
	}
	
	bytes = pread(fInfo->fd, buf, size, fInfo->offset);
	fInfo->offset += bytes;
	if (NULL != ret_size)
	{		
		*ret_size = bytes;
	}
	
	if (bytes != size)
	{
		if (!swpa_normal_file_eof(fd))
		{
			SWPA_FILE_PRINT("Err: only read %d (!= %d)![%d]\n", bytes, size, SWPAR_FAIL);
			ret = SWPAR_FAIL;
		}
		else
		{
			//�����ļ�ĩβ����ȡ����������С�ڸ����Ĵ�С�ǿ��ܵ�
		}
	}

	return ret;
}



/**
* @brief д�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf �����ݻ�����������ǿ�
* @param [in] size ��������С���������0����λΪ�ֽ�
* @param [out] ret_size ����д������ݴ�С���������ĸ���ֵ���ɴ�NULL
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_normal_file_write(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
)
{
	int ret = SWPAR_OK;
	int bytes = 0;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	
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


	NORMAL_FILE_INFO* fInfo = (NORMAL_FILE_INFO *)pheader->device_param;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fInfo);

	if(!SWPA_FILE_IS_WRITE_MODE(pheader)
		&& !SWPA_FILE_IS_APPEND_MODE(pheader))
	{
		SWPA_FILE_PRINT("Err: file was not opened as Write mode!\n");
		return SWPAR_FAIL;
	}
	
	//do write
	SWPA_FILE_PRINT("Info: L%d: offset = %d\n", __LINE__, fInfo->offset);
	bytes = pwrite(fInfo->fd, buf, size, fInfo->offset);

	if (bytes == 0)
	{
		SWPA_FILE_PRINT("Err: wrote %d (!= %d)! [%d]\n", bytes, size, SWPAR_FAIL);
		return SWPAR_FAIL;
	}

	fInfo->offset += bytes;
	if (NULL != ret_size)
	{		
		*ret_size = bytes;
	}
	
	return ret;

}



