/**
* @file swpa_comm_file.h 
* @brief ʵ�ֲ���COMM����غ���
*
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-03-11
* @version 1.0
*/
#include  <sys/types.h> 
#include  <sys/time.h> 
#include  <sys/select.h> 
#include <fcntl.h>

#include "swpa_file.h"

#include "swpa_file_private.h"

#include "swpa_comm_file.h"

#ifdef __SWPA_FILE_VERIFY
//#include "swpa_comm_verify.h"
#else
#include "drv_device.h"
#endif

#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>



#define __COMM_TIMEOUT_MS_DEFAULT 200 //in ms



typedef struct _COMM_FILE_INFO
{
	int fd;				 // �ļ�������
	int dev_id;			//�豸���
	int is_rs485;		//��ʶ�Ƿ�rs485
	int read_timeout;  //����ʱʱ������
	int write_timeout; //д��ʱʱ������
} COMM_FILE_INFO;




typedef struct _COMM_DEV_INFO
{
	char * image_file;
	int len;
} COMM_DEV_INFO;


static int _rs485_lock_1 = 0; //note: no place to delete this mutex, maybe it will cause oom issue!
static int _rs485_lock_2 = 0; //note: no place to delete this mutex, maybe it will cause oom issue!

//#ifdef SWPA_FILE_PRINT
//#undef SWPA_FILE_PRINT
//#endif
//#define SWPA_FILE_PRINT printf("%s L%d ", __FUNCTION__, __LINE__) && printf

/**
* @brief ��comm�ļ�,��Ҫ�ǳ�ʼ��SWPA_FILEHEADER�ṹ��
*
* 
* @param [in] filename �ļ���
* @param [in] mode �ļ���ģʽ
* - "r"  ����ʽ��
* - "w"  д��ʽ��
* - "r+"  ��д��ʽ��
* - "w+"  ��д��ʽ��
* @retval �ļ�������(int��) :  �ɹ���ʵ���Ͼ���SWPA_FILEHEADER�ṹ��ָ��
* @retval SWPAR_FAIL : ��ʧ��
* @retval SWPAR_INVALIDARG : �����Ƿ�
* @retval SWPAR_OUTOFMEMORY : �����ڴ治��
*/
int swpa_comm_file_open(
	const char * filename, 
	const char * mode
)
{
	int ret = 0;
	SWPA_FILEHEADER * pheader = NULL;
	COMM_FILE_INFO * pinfo = NULL;
	int dev_id = 0;
	int i = 0;
	int img_file = 0;
	int img_file_len = 0;
	unsigned char * img_buf = NULL;
	int ret_size = 0;
	int modeval = 0;
	int fd = 0;
	char commfile[64] = {0};
	
		
	//������Ч�Լ��
	SWPA_FILE_CHECK(NULL != filename);		//filenameָ��ǿ�
	SWPA_FILE_CHECK(0 != swpa_strcmp(filename, ""));//filename�ǿ��ַ���
	SWPA_FILE_CHECK(NULL != mode);			//modeָ��ǿ�
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

	pinfo = (COMM_FILE_INFO *)swpa_mem_alloc(sizeof(*pinfo));
	if (NULL == pinfo)
	{
		ret = SWPAR_OUTOFMEMORY;
		SWPA_FILE_PRINT("Err: No enough memory for pinfo! [%d]\n", ret);
		
		goto _ERR_HANDLING;
	}
	swpa_memset(pinfo, 0, sizeof(*pinfo));


	ret = swpa_file_parse_mode(pheader, mode);
	if (SWPAR_OK != ret)
	{
		SWPA_FILE_PRINT("Err: swpa_file_parse_mode() failed! [%d]\n", ret);		
		goto _ERR_HANDLING; 
		
	}
	else if (SWPA_FILE_IS_APPEND_MODE(pheader))
	{
		ret = SWPAR_INVALIDARG;
		SWPA_FILE_PRINT("Err: mode (%s)  is invalid for comm files! [%d]\n", mode, ret);		
		goto _ERR_HANDLING; 
	}

	if (SWPA_FILE_IS_READ_MODE(pheader))
	{
		modeval |= (SWPA_FILE_IS_WRITE_MODE(pheader) ? O_RDWR : O_RDONLY);
	}
	else if (SWPA_FILE_IS_WRITE_MODE(pheader))
	{
		modeval |= O_WRONLY;
	}

	modeval |= O_NOCTTY;
	//modeval |= O_RDWR;

	//��ȡ�豸���
	swpa_sscanf(filename, "%d", &pinfo->dev_id);

	swpa_snprintf(commfile, sizeof(commfile), "/dev/ttyS%d", pinfo->dev_id);	

	//�򿪴����豸
	fd = open(commfile, modeval);
	if (-1 == fd)
	{
		ret = SWPAR_FAIL;
		SWPA_FILE_PRINT("Err: failed to open %s ! [%d]\n", commfile, ret);		
		goto _ERR_HANDLING; 
	}

	ret = fcntl(fd, F_SETFL, 0);
    if (ret < 0)
	{
        SWPA_FILE_PRINT("Err: fcntl ret %d\n", ret);
	}
    ret = isatty(STDIN_FILENO);
    if (ret == 0)
	{
        SWPA_FILE_PRINT("Err: Standard input is not a terminal device.\n");
	}        
	
	pinfo->fd = fd;


	//Ĭ�ϲ���RS485�豸
	pinfo->is_rs485 = 0;
	pinfo->read_timeout = 0;
	pinfo->write_timeout = 0;

	pheader->device_param = (int)pinfo;			
	pheader->file_open	= swpa_comm_file_open;
	pheader->file_close = swpa_comm_file_close;
	pheader->file_seek	= NULL;//swpa_comm_file_seek;
	pheader->file_eof	= NULL;//swpa_comm_file_eof;
	pheader->file_tell	= NULL;//swpa_comm_file_tell;
	pheader->file_ioctl = swpa_comm_file_ioctl;
	pheader->file_read	= swpa_comm_file_read;
	pheader->file_write = swpa_comm_file_write;
	
	
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

	if (NULL != pinfo)
	{
		swpa_mem_free(pinfo);
		pinfo = NULL;
	}

	
	return ret;
	
}




/**
* @brief �ر�comm�豸�ļ�
*
* 
* @param [in] fd �ļ�������
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
int swpa_comm_file_close(
	int fd
)
{
	int ret = SWPAR_OK;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	COMM_FILE_INFO * pinfo = NULL;

	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);    	//fd ���벻����0
	

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);	
	

	SWPA_FILE_CHECK(0 != pheader->device_param);

	pinfo = (COMM_FILE_INFO *)pheader->device_param;

	
	//�ر��豸
	ret = close(pinfo->fd);
	if (0 != ret)
	{
		SWPA_FILE_PRINT("Err: failed to close (%d)! [%d]\n", ret, SWPAR_FAIL);
		return SWPAR_FAIL;
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
* @brief ��COMM�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf �����ݻ�����������ǿ�
* @param [in] size ��������С���������0����λΪ�ֽ�
* @param [out] ret_size ���ض��������ݴ�С���������ĸ���ֵ���ɴ�NULL
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_comm_file_read(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
)
{
	int ret = SWPAR_OK;
	int bytes = 0;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	COMM_FILE_INFO * pinfo = NULL;

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

	pinfo = (COMM_FILE_INFO *)pheader->device_param;


	fd_set  rdset;
	struct timeval tv, *ptimeout = NULL;
	FD_ZERO(&rdset);
	FD_SET(pinfo->fd, &rdset);
	if (0 <= pinfo->read_timeout)
	{		
		tv.tv_sec = pinfo->read_timeout / 1000;           // ms to sec
		tv.tv_usec = (pinfo->read_timeout % 1000) * 1000; // ms to usec

		ptimeout = &tv;
	}

	if (pinfo->is_rs485)
	{
		if (0 == (0 == pinfo->dev_id ? _rs485_lock_1 : _rs485_lock_2))
		{
			SWPA_FILE_PRINT("Err: invalid rs485 lock ![%d]\n",  SWPAR_FAIL );
			return SWPAR_INVALIDARG;
		}
		
		ret = swpa_mutex_lock((0 == pinfo->dev_id ? &_rs485_lock_1 : &_rs485_lock_2), pinfo->read_timeout);
		if (SWPAR_OK != ret)
		{
			SWPA_FILE_PRINT("Err: failed to lock rs485 (%d)![%d]\n", ret, SWPAR_FAIL );
			return SWPAR_FAIL;
		}
		
		ret = drv_device_set_rs485_direction(0 == pinfo->dev_id ? DEVID_RS485_1 : DEVID_RS485_2, 0);
		if (0 != ret)
		{
		    if (SWPAR_OK != swpa_mutex_unlock((0 == pinfo->dev_id ? &_rs485_lock_1 : &_rs485_lock_2)))
            {
                SWPA_FILE_PRINT("Err: failed to unlock rs485 [%d]\n", SWPAR_FAIL );
                return SWPAR_FAIL;
            }
			SWPA_FILE_PRINT("Err: failed to set rs485 direction (%d)![%d]\n", ret, SWPAR_FAIL );
			return SWPAR_FAIL;
		}
		swpa_thread_sleep_ms(10);//mdelay(10);
	}

	switch (select(pinfo->fd + 1, &rdset, NULL, NULL, ptimeout))
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
			if (FD_ISSET(pinfo->fd, &rdset))
			{					
				//do read
				bytes = read(pinfo->fd, buf, size);
				if (-1 == bytes)
				{
					ret = SWPAR_FAIL;
					SWPA_FILE_PRINT("Err: failed to read comm%d (%d:%s) [%d]!\n", pinfo->dev_id, errno, strerror(errno), ret);
				}
				else if (0 == bytes)
				{
					ret = SWPAR_FAIL;
					SWPA_FILE_PRINT("Err: read nothing [%d]!\n",  ret);
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

	if (pinfo->is_rs485)
	{
		if (0 == (0 == pinfo->dev_id ? _rs485_lock_1 : _rs485_lock_2))
		{
			SWPA_FILE_PRINT("Err: invalid rs485 lock ![%d]\n",	SWPAR_FAIL );
			return SWPAR_INVALIDARG;
		}
		
		if (SWPAR_OK != swpa_mutex_unlock((0 == pinfo->dev_id ? &_rs485_lock_1 : &_rs485_lock_2)))
		{
			SWPA_FILE_PRINT("Err: failed to unlock rs485 [%d]\n", SWPAR_FAIL );
			return SWPAR_FAIL;
		}
		swpa_thread_sleep_ms(10);//mdelay(10);
	}

	return ret;
}



/**
* @brief дcomm�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf д���ݻ�����
* @param [in] size ��������С
* @param [out] ret_size ����д������ݴ�С
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
int swpa_comm_file_write(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
)
{
	int ret = SWPAR_OK;
	int bytes = 0;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	COMM_FILE_INFO * pinfo = NULL;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		//fd ���벻����0
	SWPA_FILE_CHECK(NULL != buf);		//buf�ǿ�
	SWPA_FILE_CHECK(0 < size);		//size �������0
	
	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);
	SWPA_FILE_PRINT("buf=0x%08x\n", buf);
	SWPA_FILE_PRINT("size=%d\n", size);
	SWPA_FILE_PRINT("ret_size=0x%08x\n", ret_size);


	pinfo = (COMM_FILE_INFO *)pheader->device_param;

	fd_set  wrset;
	struct timeval tv, *ptimeout;

	FD_ZERO(&wrset);
	FD_SET(pinfo->fd, &wrset);
	if (0 <= pinfo->write_timeout)
	{		
		tv.tv_sec = pinfo->write_timeout / 1000;           // ms to sec
		tv.tv_usec = (pinfo->write_timeout % 1000) * 1000; // ms to usec

		ptimeout = &tv;
	}	
	
	//do write
	if (pinfo->is_rs485)
	{
		if (0 == (0 == pinfo->dev_id ? _rs485_lock_1 : _rs485_lock_2))
		{
			SWPA_FILE_PRINT("Err: invalid rs485 lock ![%d]\n",  SWPAR_FAIL );
			return SWPAR_INVALIDARG;
		}
		
		ret = swpa_mutex_lock((0 == pinfo->dev_id ? &_rs485_lock_1 : &_rs485_lock_2), pinfo->write_timeout);
		if (SWPAR_OK != ret)
		{
			SWPA_FILE_PRINT("Err: failed to lock rs485 (%d)![%d]\n", ret, SWPAR_FAIL );
			return SWPAR_FAIL;
		}
		
		ret = drv_device_set_rs485_direction(0 == pinfo->dev_id ? DEVID_RS485_1 : DEVID_RS485_2, 1);
		if (0 != ret)
		{
		    if (SWPAR_OK != swpa_mutex_unlock((0 == pinfo->dev_id ? &_rs485_lock_1 : &_rs485_lock_2)))
            {
                SWPA_FILE_PRINT("Err: failed to unlock rs485 (%d)![%d]\n", ret, SWPAR_FAIL );
                return SWPAR_FAIL;
            }
			SWPA_FILE_PRINT("Err: failed to set rs485 direction (%d)![%d]\n", ret, SWPAR_FAIL);
			return SWPAR_FAIL;
		}
		swpa_thread_sleep_ms(10);//mdelay(10);
	}
	
	switch (select(pinfo->fd + 1, NULL, &wrset, NULL, ptimeout))
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
			if (FD_ISSET(pinfo->fd, &wrset))
			{
				//do write
				bytes = write(pinfo->fd, buf, size);
				//д��ǿ�����Ӳ������Ȼ�����л�������ģʽ���������ܱ�֤���ݷ���������ͬʱ���ܼ�ʱ���յ�Ӧ������
				tcdrain(pinfo->fd);
				drv_device_set_rs485_direction(0 == pinfo->dev_id ? DEVID_RS485_1 : DEVID_RS485_2, 0);
				if (-1 == bytes)
				{
					ret = SWPAR_FAIL;
					SWPA_FILE_PRINT("Err: failed to write comm%d (%d:%s) [%d]!\n", pinfo->dev_id, errno, strerror(errno), ret);
				}
				else 
				{
					if (bytes != size)
					{
						ret = SWPAR_OK;;
						SWPA_FILE_PRINT("Info: only wrote %d (!= %d)![%d]\n", bytes, size, ret);
					}

					if (NULL != ret_size)
					{
						*ret_size = bytes;
					}
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


	if (pinfo->is_rs485)
	{
		if (0 == (0 == pinfo->dev_id ? _rs485_lock_1 : _rs485_lock_2))
		{
			SWPA_FILE_PRINT("Err: invalid rs485 lock ![%d]\n",  SWPAR_FAIL );
			return SWPAR_INVALIDARG;
		}

		if (SWPAR_OK != swpa_mutex_unlock((0 == pinfo->dev_id ? &_rs485_lock_1 : &_rs485_lock_2)))
		{
			SWPA_FILE_PRINT("Err: failed to unlock rs485 (%d)![%d]\n", ret, SWPAR_FAIL );
			return SWPAR_FAIL;
		}
		swpa_thread_sleep_ms(10);//mdelay(10);
	}

	if (NULL != ret_size)
	{
		*ret_size = bytes;
	}
	
	return ret;
}



static int _setup_port(int fd, int speed, int data_bits, int parity, int stop_bits)
{
    int speed_arr[] = {B115200, B9600, B38400, B19200, B4800};
    int name_arr[] = {115200, 9600, 38400, 19200, 4800};
    struct termios opt;
    int ret=-1;
    int i=0;
    int len=0;

    ret = tcgetattr(fd, &opt);		/* get the port attr */
    if (ret < 0)
	{
        SWPA_FILE_PRINT("Err: Unable to get the attribute\n");
	}

    opt.c_cflag |= (CLOCAL | CREAD); /* enable the receiver, set local mode */
    opt.c_cflag &= ~CSIZE;			/* mask the character size bits*/

    /* baud rate */
    len = sizeof(speed_arr) / sizeof(int);
    for (i = 0; i < len; i++)
    {
        if (speed == name_arr[i])
        {
            cfsetispeed(&opt, speed_arr[i]);
            cfsetospeed(&opt, speed_arr[i]);
        }
        if (i == len)
    	{
            SWPA_FILE_PRINT("Err: Unsupported baud rate.\n");
			return SWPAR_INVALIDARG;
    	}
    }

    /* data bits */
    switch (data_bits)
    {
    case 8:
        opt.c_cflag |= CS8;
        break;
    case 7:
        opt.c_cflag |= CS7;
        break;
    default:
        SWPA_FILE_PRINT("Err: Unsupported data bits.\n");
		return SWPAR_INVALIDARG;
    }

    /* parity bits */
    switch (parity)
    {
    case 'N':
    case 'n':
        opt.c_iflag &= ~INPCK;
        opt.c_cflag &= ~PARENB;
        break;
    case 'O':
    case 'o':
        opt.c_iflag |= (INPCK|ISTRIP); /*enable parity check, strip parity bits*/
        opt.c_cflag |= (PARODD | PARENB);
        break;
    case 'E':
    case 'e':
        opt.c_iflag |= (INPCK|ISTRIP); /*enable parity check, strip parity bits*/
        opt.c_cflag |= PARENB;
        opt.c_cflag &= ~PARODD;
        break;
    default:
        SWPA_FILE_PRINT("Err: Unsupported parity bits.\n");
		return SWPAR_INVALIDARG;
    }

    /* stop bits */
    switch (stop_bits)
    {
    case 1:
        opt.c_cflag &= ~CSTOPB;
        break;
    case 2:
        opt.c_cflag |= CSTOPB;
        break;
    default:
        SWPA_FILE_PRINT("Err: Unsupported stop bits.\n");
		return SWPAR_INVALIDARG;
    }

    /* raw input */
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    /* raw ouput */
    opt.c_oflag &= ~OPOST;

    //��������
    //RTS/CTS (Ӳ��) ������
    opt.c_cflag &= ~CRTSCTS; //������
    //����� XON/XOFF ������
    opt.c_iflag &= ~IXOFF;//������
    //����� XON/XOFF ������
    opt.c_iflag &= ~IXON ;//������

    // ��ֹ0x0D��0x0A
    opt.c_iflag &= ~(ICRNL|IGNCR);

    tcflush(fd, TCIFLUSH);
    opt.c_cc[VTIME] = 1; /* time out */
    opt.c_cc[VMIN] = 0; /* minimum number of characters to read */

    ret = tcsetattr(fd, TCSANOW, &opt); /* update it now */
    if (ret < 0)
	{
        SWPA_FILE_PRINT("Err: Unable to setup the port.\n");
		return SWPAR_FAIL;
	}
	
    SWPA_FILE_PRINT("Info: Setup the port OK!\n");

    return SWPAR_OK; /* everything is OK! */
}



/**
* @brief ���ļ��������Ŀ���
*
*
* Ŀǰ��֧��д�ĳ�ʱʱ������
* 
* @param [in] fd �ļ�������
* @param [in] cmd �����ʶ��Ŀǰ�Ѷ����������
* - SWPA_FILE_IOCTL_COMM_SET_TIMEOUT : д�ĳ�ʱʱ������
* @param [in] args �����־����
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_comm_file_ioctl(
	int fd, 
	int cmd, 
	void* args
)
{
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	COMM_FILE_INFO * pinfo = (COMM_FILE_INFO *)pheader->device_param;	

	
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
		case SWPA_FILE_IOCTL_COMM_IS_RS485:
		{
			if (NULL == args)
			{
				SWPA_FILE_PRINT("Err: NULL == args! [%d]\n", pinfo->dev_id, SWPAR_INVALIDARG);
				return SWPAR_INVALIDARG;
			}
			
			int is_rs485 = *(int*)args;
			switch (is_rs485)
			{
				case 0:
					pinfo->is_rs485 = 0;
				break;

				case 1:
					/*no 'break' here */
				default:
					pinfo->is_rs485 = 1;
					if (0 == (0 == pinfo->dev_id ? _rs485_lock_1 : _rs485_lock_2)
						&& SWPAR_OK != swpa_mutex_create((0 == pinfo->dev_id ? &_rs485_lock_1 : &_rs485_lock_2), NULL))
					{
						SWPA_FILE_PRINT("Err: failed to create rs485 %d lock! [-1]\n", pinfo->dev_id);
						return SWPAR_FAIL;
					}
				break;
			}
		}
		break;
		
		case SWPA_FILE_IOCTL_COMM_SET_ATTR:
		{
			char * argstr = (char *)args;
			int baudrate  , databits, stop;
			char parity;
			char * str = NULL;

			SWPA_FILE_PRINT("Info: argstr=%s\n", argstr);
			
			str = swpa_strstr(argstr, "baudrate");
			if (NULL != str)
			{
				swpa_sscanf(str, "baudrate:%d", &baudrate);
				switch (baudrate)
				{
					//case 2400:			
			        case 4800:
			        case 9600:
			        case 19200:
					case 38400:
			        case 115200:
			        //case 460800:
			            baudrate = baudrate;
		            break;
					
			        default:
			            SWPA_FILE_PRINT("Err: invalid baudrate (%d)!\n", baudrate );
			            return SWPAR_INVALIDARG;
				}
			}
			else
			{
				SWPA_FILE_PRINT("Err: no baudrate value [%d]!\n", SWPAR_INVALIDARG);
				return SWPAR_INVALIDARG;
			}

			//�������ݳ���
			str = swpa_strstr(argstr, "databits");
			if (NULL != str)
			{
				swpa_sscanf(str, "databits:%d", &databits);
				switch (databits)
			    {
			        //case 5:	
			        //case 6:
			        case 7:
			        case 8:
			            databits = databits;
		            break ;
					
			        default:
			            SWPA_FILE_PRINT("Err: invalid data bits (%d)!\n", databits);
			            return SWPAR_INVALIDARG ;
			    }
			}
			else
			{
				SWPA_FILE_PRINT("Err: no data bits value [%d]!\n", SWPAR_INVALIDARG);
				return SWPAR_INVALIDARG;
			}
			

			//������żУ��λ
			str = swpa_strstr(argstr, "parity");
			if (NULL != str)
			{
				swpa_sscanf(str, "parity:%c", &parity);
				switch (parity)
				{
					case 'n': case 'N':
			        case 'o': case 'O':
			        case 'e': case 'E':
			        //case 'S': case 's':  //as no parity
				        parity = parity;
					break;

			        default:
			            SWPA_FILE_PRINT("Err: invalid parity value (%d)!\n", parity);
			            return SWPAR_INVALIDARG;
				}
			}
			else
			{
				SWPA_FILE_PRINT("Err: no parity value [%d]!\n", SWPAR_INVALIDARG);
				return SWPAR_INVALIDARG;
			}

			//����ֹͣλ
			str = swpa_strstr(argstr, "stop");
			if (NULL != str)
			{
				swpa_sscanf(str, "stop:%d", &stop);
				switch (stop)
				{
					case 1:
			        case 2:
			            stop = stop;
		            break;
					
			        default:
			            SWPA_FILE_PRINT("Err: invalid stop bits lenght (%d)!\n", stop);
			            return SWPAR_INVALIDARG;
				}
			}
			else
			{
				SWPA_FILE_PRINT("Err: no stop bit value [%d]!\n", SWPAR_INVALIDARG);
				return SWPAR_INVALIDARG;
			}
			
			return _setup_port(pinfo->fd, baudrate, databits, parity, stop);
		}			
		break;
		
		case SWPA_FILE_SET_READ_TIMEOUT :
		{
			pinfo->read_timeout = *(int *)args;
			return SWPAR_OK;
		}
		break;
		
		case SWPA_FILE_SET_WRITE_TIMEOUT :
		{
			pinfo->write_timeout = *(int *)args;
			return SWPAR_OK;
		}
		break;

		default:
			SWPA_FILE_PRINT("Err: invalid cmd for comm file! [%d]\n", SWPAR_INVALIDARG);
			return SWPAR_INVALIDARG;
	}

	return SWPAR_OK;	
}





