/**
* @file swpa_dsplink_file.c 
* @brief ʵ�ֲ���DSPLINK����غ���
*
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-03-11
* @version 1.0
*/


#include "swpa_file.h"

#include "swpa_file_private.h"

#include "swpa_dsplink_file.h"

#ifdef __SWPA_FILE_VERIFY
#include "swpa_dsplink_verify.h"
#else
//#include "drv_device.h"
#include "drv_dsplink.h"
#endif


#define __DSPLINK_TIMEOUT_MS_DEFAULT 200 //in ms



typedef struct _DSPLINK_FILE_INFO
{
	unsigned int dev_id; // �ļ����ڵ��豸ID
	int timeout_ms;		//DSP����ĳ�ʱ����
} DSPLINK_FILE_INFO;




typedef struct _DSPLINK_DEV_INFO
{
	char * image_file;
	int 	len;
	int		fd;
} DSPLINK_DEV_INFO;



static DSPLINK_DEV_INFO _dsplink_dev_info[] =
{
	{"FLASH/0/DSPLINK_IMAGE",		1024,		0}, //note: �����в����ã���ʽ�����ļ�����Ҫ����FLASH�Ĺ滮ʹ��
	{"BLOCK/./MercuryDsp.out",		5*1024*1024,		0}, //note: �����в����ã���ʽ�����ļ�����Ҫ����FLASH�Ĺ滮ʹ��
	{"", 0x0}
};


#define DSPLINK_DEV_NUM (sizeof(_dsplink_dev_info) / sizeof(_dsplink_dev_info[0]))



/**
* @brief ��dsplink�ļ�,��Ҫ�ǳ�ʼ��SWPA_FILEHEADER�ṹ��
*
* 
* @param [in] filename �ļ���
* @param [in] mode �ļ���ģʽ
* - "w"  :д��ʽ��
* @retval �ļ�������(int��) :  �ɹ���ʵ���Ͼ���SWPA_FILEHEADER�ṹ��ָ��
* @retval SWPAR_FAIL : ��ʧ��
* @retval SWPAR_INVALIDARG : �����Ƿ�
* @retval SWPAR_OUTOFMEMORY : �ڴ治��
*/
int swpa_dsplink_file_open(
	const char * filename, 
	const char * mode
)
{
	int ret = 0;
	SWPA_FILEHEADER * pheader = NULL;
	DSPLINK_FILE_INFO * pinfo = NULL;
	int dev_id = 0;
	int i = 0;
	int img_file = 0;
	int img_file_len = 0;
	unsigned char * img_buf = NULL;
	int ret_size = 0;
		
	//������Ч�Լ��
	SWPA_FILE_CHECK(NULL != filename);    	//filenameָ��ǿ�
	SWPA_FILE_CHECK(0 != swpa_strcmp(filename, ""));//filename�ǿ��ַ���
	SWPA_FILE_CHECK(NULL != mode);			//modeָ��ǿ�
	SWPA_FILE_CHECK(0 != swpa_strcmp(mode, ""));	//mode�ǿ��ַ���

	//��ӡ����
	SWPA_FILE_PRINT("filename=%s\n", filename);
	SWPA_FILE_PRINT("mode=%s\n", mode);
	

	//��ȡ�豸ID
	swpa_sscanf(filename, "%d", &dev_id);
	
	for (i=0; i<DSPLINK_DEV_NUM; i++)
	{
		if (dev_id == i)
		{
			//���豸�Ѿ���ʼ������ֱ�ӷ���fd
			if (0 != _dsplink_dev_info[dev_id].fd)
			{
				return _dsplink_dev_info[dev_id].fd;
			}
			else
			{
				break;
			}
		}
	}
	
	//û���ҵ���Ӧ���豸����������
	if (DSPLINK_DEV_NUM <= i)
	{
		ret = SWPAR_INVALIDARG;
		SWPA_FILE_PRINT("Err: %s not found! [%d]\n", filename, ret);		
		
		goto _ERR_HANDLING;
	}
	

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

	pinfo = (DSPLINK_FILE_INFO *)swpa_mem_alloc(sizeof(*pinfo));
	if (NULL == pinfo)
	{
		ret = SWPAR_OUTOFMEMORY;
	    SWPA_FILE_PRINT("Err: No enough memory for pinfo! [%d]\n", ret);
		
		goto _ERR_HANDLING;
	}

	//
	ret = swpa_file_parse_mode(pheader, mode);
	if (SWPAR_OK != ret)
	{
		SWPA_FILE_PRINT("Err: swpa_file_parse_mode() failed! [%d]\n", ret);		
		goto _ERR_HANDLING; 
		
	}
	else if (SWPA_FILE_IS_APPEND_MODE(pheader) || 
		SWPA_FILE_IS_READ_MODE(pheader))
	{
		ret = SWPAR_INVALIDARG;
		SWPA_FILE_PRINT("Err: mode (%s)  is invalid for dsplink files! [%d]\n", mode, ret);		
		goto _ERR_HANDLING; 
	}

	SWPA_FILE_PRINT("dsp:%s\n", _dsplink_dev_info[dev_id].image_file);

	//��DSPLINK�����ļ�
	img_file = swpa_file_open(_dsplink_dev_info[dev_id].image_file, "r");
	if (img_file <= 0 && img_file > SWPAR_MIN)
	{
		ret = SWPAR_FAIL;
		SWPA_FILE_PRINT("Err: open %s failed (%d)! [%d]\n", _dsplink_dev_info[dev_id].image_file, img_file, SWPAR_FAIL);		
		
		goto _ERR_HANDLING; 
	}

	//��ȡDSPLINK�����ļ��Ĵ�С
	swpa_file_seek(img_file, 0, SWPA_SEEK_END);
	img_file_len = swpa_file_tell(img_file);
	swpa_file_seek(img_file, 0, SWPA_SEEK_SET);
	if (0 >= img_file_len)
	{
		ret = SWPAR_FAIL;
		SWPA_FILE_PRINT("Err: length (%d) of %s is invalid! [%d]\n", img_file_len, _dsplink_dev_info[dev_id].image_file, SWPAR_FAIL);		
		
		goto _ERR_HANDLING; 
	}

	//�����ž����ļ����ڴ��
	img_buf = (unsigned char *)swpa_mem_alloc(img_file_len);
	if (NULL == img_buf)
	{
		ret = SWPAR_OUTOFMEMORY;
	    SWPA_FILE_PRINT("Err: No enough memory for img_buf! [%d]\n", ret);
		
		goto _ERR_HANDLING;
	}

	//��ȡDSPLINK�����ļ����ڴ棬�Ա����
	ret = swpa_file_read(img_file, img_buf, img_file_len, &ret_size);
	if (SWPAR_OK != ret || ret_size != img_file_len)
	{
		ret = SWPAR_FAIL;
	    SWPA_FILE_PRINT("Err: read %s failed! [%d]\n", _dsplink_dev_info[dev_id].image_file, ret);
		
		goto _ERR_HANDLING;
	}

	swpa_file_close(img_file);
	
	//����DSPLINK�����ļ�����ʼ��DSP�豸
	ret = drv_dsplink_setup(img_buf, img_file_len);
	if (0 != ret)
	{
		ret = SWPAR_FAIL;
	    SWPA_FILE_PRINT("Err: drv_dsplink_setup() failed! [%d]\n", ret);
		
		goto _ERR_HANDLING;
	}

	//�ͷž����ļ�ռ�õ��ڴ�
	swpa_mem_free(img_buf);
	img_buf = NULL;

	pinfo->timeout_ms = __DSPLINK_TIMEOUT_MS_DEFAULT;
	pinfo->dev_id = dev_id;
	
	_dsplink_dev_info[dev_id].fd = (int)pheader;
	
	pheader->device_param = (int)pinfo;	
			
	pheader->file_open 	= swpa_dsplink_file_open;
	pheader->file_close = swpa_dsplink_file_close;
	pheader->file_seek 	= NULL;//swpa_dsplink_file_seek;
	pheader->file_eof 	= NULL;//swpa_dsplink_file_eof;
	pheader->file_tell 	= NULL;//swpa_dsplink_file_tell;
	pheader->file_ioctl = swpa_dsplink_file_ioctl;
	pheader->file_read 	= NULL;//swpa_dsplink_file_read;
	pheader->file_write	= swpa_dsplink_file_write;
	
	return (int)pheader;


//����������
_ERR_HANDLING:

	if (0 < img_file)
	{
		swpa_file_close(img_file);
	}

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

	if (NULL != pinfo)
	{
		swpa_mem_free(pinfo);
		pinfo = NULL;
	}
	if (NULL != img_buf)
	{
		swpa_mem_free(img_buf);
		img_buf = NULL;
	}
	
	return ret;
	
}



/**
* @brief �ر�dsplink�豸�ļ�
*
* 
* @param [in] fd �ļ�������
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
int swpa_dsplink_file_close(
	int fd
)
{
	int ret = SWPAR_OK;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);    	//fd ���벻����0
	SWPA_FILE_CHECK(0 != pheader->device_param);

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);	


	//�ر�DSP�豸
	ret = drv_dsplink_exit();
	if (0 != ret)
	{
		SWPA_FILE_PRINT("Err: drv_dsplink_exit failed! [%d]\n", ret);
		return ret;
	}

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

	SWPA_FILE_CHECK_RET(swpa_mem_free((void*)pheader->device_param), SWPAR_FAIL);
	pheader->device_param = (int)NULL;

	SWPA_FILE_CHECK_RET(swpa_mem_free(pheader), SWPAR_FAIL);
	pheader = NULL;

	return ret;
}





/**
* @brief дdsplink�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf д���ݻ�����
* @param [in] size ��������С
* @param [out] ret_size ����д������ݴ�С
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
int swpa_dsplink_file_write(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
)
{
	int ret = SWPAR_OK;
	unsigned int bytes = 0;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	DSPLINK_FILE_INFO * pinfo = NULL;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		//fd ���벻����0
	SWPA_FILE_CHECK(NULL != buf);		//buf�ǿ�
	SWPA_FILE_CHECK(0 < size);		//size �������0
	//SWPA_FILE_CHECK(NULL != ret_size);		//Note: ret_size������NULL����ע�͵�
	
	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);
	SWPA_FILE_PRINT("buf=0x%08x\n", buf);
	SWPA_FILE_PRINT("size=%d\n", size);
	SWPA_FILE_PRINT("ret_size=0x%08x\n", ret_size);
	

	pinfo = (DSPLINK_FILE_INFO *)pheader->device_param;
	SWPA_FILE_CHECK(NULL != pinfo);		//pinfo�ǿ�


	if (!SWPA_FILE_IS_WRITE_MODE(pheader))
	{
		SWPA_FILE_PRINT("Err: file is not in write mode![%d]\n", SWPAR_FAIL );
		return SWPAR_FAIL;
	}

	//do write
	ret =  drv_dsplink_doprocess((unsigned int)buf, pinfo->timeout_ms);
	if (0 != ret)
	{
		SWPA_FILE_PRINT("Err: drv_dsplink_doprocess() failed (%d)![%d]\n", ret, SWPAR_FAIL);
		return SWPAR_FAIL;
	}

	
	if (NULL != ret_size)
	{
		*ret_size = bytes;
	}

	return SWPAR_OK;
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
int swpa_dsplink_file_ioctl(
	int fd, 
	int cmd, 
	void* args
)
{
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	DSPLINK_FILE_INFO * pinfo = (DSPLINK_FILE_INFO *)pheader->device_param;	

	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);    	//fd ���벻����0
	//SWPA_FILE_CHECK(0 < cmd);    	//cmdȡֵû���޶�����mark��
	//SWPA_FILE_CHECK(NULL != args);    	//args �ڲ�ͬcmd�¿���ΪNULL����mark��

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);	
	SWPA_FILE_PRINT("cmd=%d\n", cmd);	
	SWPA_FILE_PRINT("args=0x%08x\n", args);


	switch (cmd)
	{
		case SWPA_FILE_SET_READ_TIMEOUT:
		case SWPA_FILE_SET_WRITE_TIMEOUT:
		{
			int timeout = *(int*)args;

			SWPA_FILE_CHECK(0 < timeout);    	//��ʱʱ���������
			
			pinfo->timeout_ms = timeout;
		}
		break;
		
		default:
			SWPA_FILE_PRINT("Err: invalid cmd (%d) for dsplink ioctl! [%d]\n", cmd, SWPAR_INVALIDARG);
			return SWPAR_INVALIDARG;
	}

	return SWPAR_OK;
}



