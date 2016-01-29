/**
* @file swpa_flash_file.c
* @brief ʵ��FLASH��صĲ�������
*
* 
*/


#include "swpa_file_private.h"
#include "swpa_flash_file.h"

#ifdef __SWPA_FILE_VERIFY
#include "swpa_flash_verify.h"
#else
#include "drv_device.h"
#include "drv_device_flash_hal.h"
#endif


#define __FLASH_SEG_NAME_LEN 64



typedef struct _FLASH_FILE_INFO
{
	unsigned int dev_id; // �ļ����ڵ��豸ID
	unsigned int addr;	//�ļ�����ʼ��ַ
	unsigned int len;	//�ļ��ĳ���
	unsigned int seg_len;	//�ļ����ڶεĳ���
	unsigned int offset;	//�ļ��ڲ�ƫ��ָ��
	unsigned int file_id;	//�ļ��ڲ�ƫ��ָ��
} FLASH_FILE_INFO;



//FLASH�ϴ洢�εĶ���
typedef struct _FLASH_SEG_DEF
{
	char * 			name; // ����
	unsigned int 	addr;	//����ʼ��ַ
	unsigned int 	len;	//�γ���	
	unsigned int	permission;
	unsigned int	dev_id; //�����豸��ID
	
} FLASH_SEG_DEF;

//FLASH�����ļ���Ϣ
typedef struct _FLASH_SEG_FILE_INFO
{
	//unsigned int 	addr;	//����ʼ��ַ
	unsigned int 	len;	//�ļ�����
	unsigned int	permission;
	
} FLASH_SEG_FILE_INFO;



#define DM6467_FILEINFO_ADDR			F_DM6467_ADDR_RESERVED//RESERVED_ADDR
#define DM6467_FILEINFO_LEN				0x400
#define DM368_FILEINFO_ADDR				F_DM368_ADDR_RESERVED//RESERVED_ADDR
#define DM368_FILEINFO_LEN				0x400



#define PERMISSION_READ			0x1<<0
#define PERMISSION_WRITE		0x1<<1



//FLASH �η���������²�driver lib��drv_device_flash_hal.h��FLASH�Ļ�������
static FLASH_SEG_DEF _flash_seg_info[] = 
{
// -- NAME --				-- ADDR --					-- LEN -- 				-- PERMISSION --							-- DEV ID --
	//DM6467
	{"UBOOT", 			F_DM6467_ADDR_UBOOT, 		F_DM6467_LEN_UBOOT,			PERMISSION_READ,						DEVID_SPI_FLASH_DM6467},
	{"KERNEL_BAK", 		F_DM6467_ADDR_KERNEL_BAK, 	F_DM6467_LEN_KERNEL_BAK,	PERMISSION_READ | PERMISSION_WRITE,		DEVID_SPI_FLASH_DM6467},	
	{"ROOTFS_BAK", 		F_DM6467_ADDR_ROOTFS_BAK, 	F_DM6467_LEN_ROOTFS_BAK,	PERMISSION_READ | PERMISSION_WRITE,		DEVID_SPI_FLASH_DM6467},
	{"FFGA_ROM", 		F_DM6467_ADDR_FPGA_ROM, 	F_DM6467_LEN_FPGA_ROM,		PERMISSION_READ | PERMISSION_WRITE,		DEVID_SPI_FLASH_DM6467},	
	{"KERNEL", 			F_DM6467_ADDR_KERNEL, 		F_DM6467_LEN_KERNEL,		PERMISSION_READ | PERMISSION_WRITE,		DEVID_SPI_FLASH_DM6467},
	{"ROOTFS", 			F_DM6467_ADDR_ROOTFS, 		F_DM6467_LEN_ROOTFS,		PERMISSION_READ | PERMISSION_WRITE,		DEVID_SPI_FLASH_DM6467},
	{"ARM_APP", 		F_DM6467_ADDR_ARM_APP, 		F_DM6467_LEN_ARM_APP,		PERMISSION_READ | PERMISSION_WRITE,		DEVID_SPI_FLASH_DM6467},
	{"DSP_APP", 		F_DM6467_ADDR_DSP_APP, 		F_DM6467_LEN_DSP_APP,		PERMISSION_READ | PERMISSION_WRITE,		DEVID_SPI_FLASH_DM6467},
	{"RESERVED", 		F_DM6467_ADDR_RESERVED, 	F_DM6467_LEN_RESERVED,		PERMISSION_READ | PERMISSION_WRITE,		DEVID_SPI_FLASH_DM6467},
	{"APP_LOG", 		F_DM6467_ADDR_APP_LOG, 		F_DM6467_LEN_APP_LOG,		PERMISSION_READ | PERMISSION_WRITE,		DEVID_SPI_FLASH_DM6467},		
	{"SYS_LOG", 		F_DM6467_ADDR_SYS_LOG, 		F_DM6467_LEN_SYS_LOG,		PERMISSION_READ | PERMISSION_WRITE,		DEVID_SPI_FLASH_DM6467},

	//DM368
	{"UBL", 			F_DM368_ADDR_UBL,			F_DM368_LEN_UBL,			PERMISSION_READ | PERMISSION_WRITE, 	DEVID_SPI_FLASH_DM368},
	{"PARAMS", 			F_DM368_ADDR_PARAMS,		F_DM368_LEN_PARAMS,			PERMISSION_READ | PERMISSION_WRITE, 	DEVID_SPI_FLASH_DM368},
	{"UBOOT", 			F_DM368_ADDR_UBOOT,			F_DM368_LEN_UBOOT,			PERMISSION_READ | PERMISSION_WRITE, 	DEVID_SPI_FLASH_DM368},
	{"KERNEL", 			F_DM368_ADDR_KERNEL,		F_DM368_LEN_KERNEL,			PERMISSION_READ | PERMISSION_WRITE, 	DEVID_SPI_FLASH_DM368},
	{"ROOTFS", 			F_DM368_ADDR_ROOTFS,		F_DM368_LEN_ROOTFS,			PERMISSION_READ | PERMISSION_WRITE, 	DEVID_SPI_FLASH_DM368},
	{"RESERVED", 		F_DM368_ADDR_RESERVED,		F_DM368_LEN_RESERVED,		PERMISSION_READ | PERMISSION_WRITE, 	DEVID_SPI_FLASH_DM368},
	{"APP_LOG", 		F_DM368_ADDR_APP_LOG,		F_DM368_LEN_APP_LOG,		PERMISSION_READ | PERMISSION_WRITE, 	DEVID_SPI_FLASH_DM368},
	{"SYS_LOG", 		F_DM368_ADDR_SYS_LOG,		F_DM368_LEN_SYS_LOG,		PERMISSION_READ | PERMISSION_WRITE, 	DEVID_SPI_FLASH_DM368},
};


//������
#define FLASH_SEG_NUM 	(sizeof(_flash_seg_info) / sizeof(_flash_seg_info[0]))




static int _flash_init(const unsigned int dev_id)
{
	int ret = 0;
	unsigned int filecount = 0;
	unsigned int crc32 = 0;
	unsigned int size = 0;
	const unsigned int info_addr = ((0 == dev_id) ? DM6467_FILEINFO_ADDR : DM368_FILEINFO_ADDR);
	const unsigned int info_len  = ((0 == dev_id) ? DM6467_FILEINFO_LEN : DM368_FILEINFO_LEN);

	//read crc32 stored in flash
	size = sizeof(crc32);
	ret = drv_flash_read(dev_id, 
		info_addr+sizeof(filecount), 
		(unsigned char*)&crc32, 
		&size);
	if (SWPAR_OK != ret || size != sizeof(crc32))
	{
		SWPA_FILE_PRINT("Err: failed to get flash crc32! [%d]\n", SWPAR_FAIL );
		return SWPAR_FAIL;
	}

	//calculat the new crc32
	filecount = FLASH_SEG_NUM;
	unsigned int new_crc32 = swpa_utils_calc_crc32(0, (unsigned char*)&filecount, sizeof(filecount));

	//if not matched, do init 
	if (new_crc32 != crc32)
	{	
		size = sizeof(filecount);
		ret = drv_flash_write(dev_id, 
			info_addr, 
			(unsigned char*)&filecount, 
			&size);
		if (SWPAR_OK != ret || size != sizeof(filecount))
		{
			SWPA_FILE_PRINT("Err: failed (ret = %d) to write flash file count! [%d]\n", ret, SWPAR_FAIL );
			return SWPAR_FAIL;
		}

		size = sizeof(new_crc32);
		ret = drv_flash_write(dev_id, 
			info_addr+sizeof(filecount), 
			(unsigned char*)&new_crc32, 
			&size);
		if (SWPAR_OK != ret || size != sizeof(new_crc32))
		{
			SWPA_FILE_PRINT("Err: failed (ret = %d) to write flash crc32! [%d]\n", ret, SWPAR_FAIL );
			return SWPAR_FAIL;
		}

		unsigned int buf_size = info_len - sizeof(filecount) - sizeof(new_crc32);
		unsigned char * buf = (unsigned char*)swpa_mem_alloc(buf_size);
		if (NULL == buf)
		{
			SWPA_FILE_PRINT("Err: no memory for buf [%d]\n", SWPAR_OUTOFMEMORY);
			return SWPAR_OUTOFMEMORY;
		}
		swpa_memset(buf, 0, buf_size);
 
		int i = 0;
		for (i=0; i<FLASH_SEG_NUM; i++)
		{
			FLASH_SEG_FILE_INFO info;
			info.permission = _flash_seg_info[i].permission;
			info.len = (PERMISSION_WRITE & _flash_seg_info[i].permission) ? 0 : _flash_seg_info[i].len;

			swpa_memcpy(buf+i*sizeof(info), &info, sizeof(info));
		}

		size = buf_size;
		ret = drv_flash_write(
			dev_id, 
			info_addr + sizeof(filecount) + sizeof(new_crc32), 
			(unsigned char*)buf, 
			&size);
		if (0 != ret || size != buf_size) 
		{
			SWPA_FILE_PRINT("Err: drv_flash_write() failed: %d ![%d]\n", ret, SWPAR_FAIL );
			swpa_mem_free(buf);
			buf = NULL;
			return SWPAR_FAIL;
		}

		swpa_mem_free(buf);
		buf = NULL;
		
	}
	
	return SWPAR_OK;
}



static int _flash_get_file_size(const unsigned int dev_id, const unsigned int file_id)
{
	int ret = SWPAR_OK;
	
	FLASH_SEG_FILE_INFO info;
	unsigned int len = sizeof(info);

	const unsigned int info_addr = (0 == dev_id) ? DM6467_FILEINFO_ADDR : DM368_FILEINFO_ADDR;

	ret = drv_flash_read(dev_id, 
		info_addr+sizeof(unsigned int/*filecount*/)+sizeof(unsigned int/*crc32*/)+file_id*sizeof(info),
		(unsigned char*)&info,
		&len);
	if (0 != ret || len != sizeof(info))
	{
		SWPA_FILE_PRINT("Err: drv_flash_read() failed: %d ![%d]\n", ret, SWPAR_FAIL );
		return SWPAR_FAIL;
	}

	return info.len;	
}


static int _flash_save_file_size(const unsigned int dev_id, const unsigned int file_id, const unsigned int size)
{
	int ret = SWPAR_OK;
	
	FLASH_SEG_FILE_INFO info;
	info.len = size;
	unsigned int len = sizeof(info);

	const unsigned int info_addr = (0 == dev_id) ? DM6467_FILEINFO_ADDR : DM368_FILEINFO_ADDR;

	ret = drv_flash_write(dev_id, 
		info_addr+sizeof(unsigned int/*filecount*/)+sizeof(unsigned int/*crc32*/)+file_id*sizeof(info),
		(unsigned char*)&info,
		&len);
	if (0 != ret || len != sizeof(info))
	{
		SWPA_FILE_PRINT("Err: drv_flash_read() failed: %d ![%d]\n", ret, SWPAR_FAIL );
		return SWPAR_FAIL;
	}

	return info.len;	
}






/**
* @brief ��FLASH�ļ�,��Ҫ�ǳ�ʼ��SWPA_FILEHEADER�ṹ��
*
* 
* @param [in] filename : FLASH�豸���ļ���
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
int swpa_flash_file_open(
	const char *filename, 
	const char *mode
)
{
	int ret = 0;
	SWPA_FILEHEADER * pheader = NULL;
	FLASH_FILE_INFO * pinfo = NULL;
	int dev_id = 0;
	char segname[64] = {0};
	int i;
		
	//������Ч�Լ��
	SWPA_FILE_CHECK(NULL != filename);    	//filenameָ��ǿ�
	SWPA_FILE_CHECK(0 != swpa_strcmp(filename, ""));//filename�ǿ��ַ���
	SWPA_FILE_CHECK(NULL != mode);			//modeָ��ǿ�
	SWPA_FILE_CHECK(0 != swpa_strcmp(mode, ""));	//mode�ǿ��ַ���

	//��ӡ����
	SWPA_FILE_PRINT("filename=%s\n", filename);
	SWPA_FILE_PRINT("mode=%s\n", mode);

	//��ȡ�豸ID
	swpa_sscanf(filename, "%d/%s", &dev_id, segname);
	if (0 == dev_id)
	{
		dev_id = DEVID_SPI_FLASH_DM6467;
	}
	else if (1 == dev_id)
	{
		dev_id = DEVID_SPI_FLASH_DM368;
	}
	else 
	{
		return SWPAR_INVALIDARG;
	}

	if (SWPAR_OK != _flash_init(dev_id))
	{
		ret = SWPAR_FAIL;
		SWPA_FILE_PRINT("Err: failed to init flash! [%d]\n", ret);
		return ret;
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
	
	pinfo = (FLASH_FILE_INFO *)swpa_mem_alloc(sizeof(*pinfo));
	if (NULL == pinfo)
	{
		ret = SWPAR_OUTOFMEMORY;
	    SWPA_FILE_PRINT("Err: No enough memory for pinfo! [%d]\n", ret);
		
		goto _ERR_HANDLING;
	}

	ret = swpa_file_parse_mode(pheader, mode);
	if (SWPAR_OK != ret)
	{
		SWPA_FILE_PRINT("Err: swpa_file_parse_mode() failed! [%d]\n", ret);		
		goto _ERR_HANDLING; 
		
	}

	for (i=0; i<FLASH_SEG_NUM; i++)
	{
		if (0==swpa_strncmp(_flash_seg_info[i].name, segname, swpa_strlen(_flash_seg_info[i].name))
			&& dev_id == _flash_seg_info[i].dev_id)
		{
			if (SWPA_FILE_IS_APPEND_MODE(pheader) ||
				SWPA_FILE_IS_WRITE_MODE(pheader))
			{
				if (!(PERMISSION_WRITE ^ _flash_seg_info[i].permission))
				{
					ret = SWPAR_INVALIDARG;
					SWPA_FILE_PRINT("Err: this file is not permitted to write! [%d]\n", ret);		
					goto _ERR_HANDLING; 
				}
			}
			
			pinfo->addr 	= _flash_seg_info[i].addr;
			pinfo->len  	= _flash_get_file_size(dev_id, i); 
			pinfo->offset 	= (SWPA_FILE_IS_APPEND_MODE(pheader)) ? pinfo->len : 0;	//0;
			pinfo->dev_id 	= dev_id;
			pinfo->seg_len	= _flash_seg_info[i].len;
			pinfo->file_id	= i;
			
			break;
		}
	}
	
	//û���ҵ���Ӧ�Ķ�����������
	if (FLASH_SEG_NUM <= i)
	{
		ret = SWPAR_INVALIDARG;
		SWPA_FILE_PRINT("Err: %s not found! [%d]\n", filename, ret);		
		
		goto _ERR_HANDLING;
	}

	pheader->device_param = (int)pinfo;			
	pheader->file_open 	= swpa_flash_file_open;
	pheader->file_close = swpa_flash_file_close;
	pheader->file_seek 	= swpa_flash_file_seek;
	pheader->file_eof 	= swpa_flash_file_eof;
	pheader->file_tell 	= swpa_flash_file_tell;
	pheader->file_ioctl = swpa_flash_file_ioctl;
	pheader->file_read 	= swpa_flash_file_read;
	pheader->file_write	= swpa_flash_file_write;

	
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
* @brief �ر�flash�豸�ļ�
*
* 
* @param [in] fd �ļ�������
* @retval SWPAR_OK :  �ɹ�
*/
int swpa_flash_file_close(
	int fd
)
{
	int ret = SWPAR_OK;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	FLASH_FILE_INFO * pinfo = NULL;
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);    	//fd ���벻����0
	

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);	
	

	SWPA_FILE_CHECK(0 != pheader->device_param);

	pinfo = (FLASH_FILE_INFO *)(pheader->device_param);
	SWPA_FILE_CHECK(NULL != pinfo);		//pinfo�ǿ�

	//�ر��豸
	if (SWPA_FILE_IS_WRITE_MODE(pheader) ||
		SWPA_FILE_IS_APPEND_MODE(pheader))
	{
		_flash_save_file_size(pinfo->dev_id, pinfo->file_id, pinfo->len);
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
* @brief ��flash�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf �����ݻ�����
* @param [in] size ��������С
* @param [out] ret_size ���ض��������ݴ�С
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
int swpa_flash_file_read(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
)
{
	int ret = SWPAR_OK;
	unsigned int bytes = 0;
	FLASH_FILE_INFO * pinfo = NULL;
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
	

	pinfo = (FLASH_FILE_INFO *)(pheader->device_param);
	SWPA_FILE_CHECK(NULL != pinfo);		//pinfo�ǿ�


	if (!SWPA_FILE_IS_READ_MODE(pheader))
	{
		SWPA_FILE_PRINT("Err: file is not in read mode![%d]\n", SWPAR_FAIL);
		return SWPAR_FAIL;
	}
	
	//do read
	bytes = (unsigned int)size;
	if (pinfo->offset+size >= pinfo->seg_len)
	{
		bytes = pinfo->seg_len - pinfo->offset;
	}	
	ret = drv_flash_read(pinfo->dev_id, pinfo->addr + pinfo->offset, (unsigned char *)buf, &bytes);
	if (0 != ret) 
	{
		SWPA_FILE_PRINT("Err: drv_flash_read() failed: %d ![%d]\n", ret, SWPAR_FAIL);
		return SWPAR_FAIL;
	}

	//�����ļ��ڲ���дָ��
	pinfo->offset += bytes;
	pinfo->offset %= (pinfo->seg_len+1);

	if (NULL != ret_size)
	{
		*ret_size = bytes;
	}
	
	return ret;
}




/**
* @brief дflash�ļ�����
*
* 
* @param [in] fd �ļ�������
* @param [in] buf д���ݻ�����
* @param [in] size ��������С
* @param [out] ret_size ����д������ݴ�С
* @retval SWPAR_OK :  �ɹ�
* @retval SWPAR_FAIL :  ʧ��
*/
int swpa_flash_file_write(
	int fd, 
	void* buf, 
	int size, 
	int* ret_size
)
{
	int ret = SWPAR_OK;
	unsigned int bytes = 0;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	FLASH_FILE_INFO * pinfo = NULL;
	
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
	

	pinfo = (FLASH_FILE_INFO *)pheader->device_param;
	SWPA_FILE_CHECK(NULL != pinfo);		//pinfo�ǿ�

	//SWPA_FILE_CHECK((unsigned int)size <= pinfo->len); //buf �Ĵ�С���벻�����ļ�����

	if (!SWPA_FILE_IS_APPEND_MODE(pheader)
		&& !SWPA_FILE_IS_WRITE_MODE(pheader))
	{
		SWPA_FILE_PRINT("Err: file is not in write mode![%d]\n", SWPAR_FAIL);
		return SWPAR_FAIL;
	}

	//do write
	bytes = (unsigned int)size;
	if (pinfo->offset+size >= pinfo->seg_len)
	{
		bytes = pinfo->seg_len - pinfo->offset;
	}
	ret = drv_flash_write(
		pinfo->dev_id, 
		pinfo->addr + pinfo->offset, 
		(unsigned char *)buf, 
		&bytes
	);
	if (0 != ret) 
	{
		SWPA_FILE_PRINT("Err: drv_flash_write() failed: %d ![%d]\n", ret, SWPAR_FAIL);
		return SWPAR_FAIL;
	}

	//�����ļ��ڲ���дָ��
	pinfo->offset += bytes;
	pinfo->offset %= (pinfo->seg_len+1);

	if (pinfo->len < pinfo->offset)
	{
		pinfo->len = pinfo->offset;
	}


	if (NULL != ret_size)
	{
		*ret_size = bytes;
	}
	
	return ret;
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
* @retval SWPAR_INVALIDARG : �����Ƿ�
*/
int swpa_flash_file_seek(
	int fd, 
	int offset, 
	int pos
)
{
	int ret = SWPAR_OK;
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	FLASH_FILE_INFO * pinfo = NULL;
	
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		// fd ���벻����0
	//SWPA_FILE_CHECK(0 != offset);		//offset����ȡ����ֵ
	SWPA_FILE_CHECK(SWPA_SEEK_SET == pos || SWPA_SEEK_CUR == pos || SWPA_SEEK_END == pos);		//pos

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);
	SWPA_FILE_PRINT("offset=%d\n", offset);
	SWPA_FILE_PRINT("pos=%d\n", pos);

	pinfo = (FLASH_FILE_INFO *) pheader->device_param;

	switch (pos)
	{
	case SWPA_SEEK_SET:
		if (offset>=0 && (unsigned int)offset <= pinfo->len)
		{
			pinfo->offset = offset;
		}
		else
		{
			SWPA_FILE_PRINT("Err: offset(%d) out of range! (file len = %d)\n", offset, pinfo->len);
			return SWPAR_FAIL;
		}
	break;

	case SWPA_SEEK_CUR:
		if (pinfo->offset+offset >= 0 && pinfo->offset+offset <= pinfo->len)
		{
			pinfo->offset += offset;
		}
		else
		{
			SWPA_FILE_PRINT("Err: offset(%d) out of range! (file len = %d)\n", offset, pinfo->len);
			return SWPAR_FAIL;
		}
	break;

	case SWPA_SEEK_END:
		if (offset<=0 && pinfo->len + offset >= 0)
		{
			pinfo->offset = pinfo->len + offset;
		}
		else
		{
			SWPA_FILE_PRINT("Err: offset(%d) out of range! (file len = %d)\n", offset, pinfo->len);
			return SWPAR_FAIL;
		}
	break;
	
		
	default:
		return SWPAR_INVALIDARG;
	}
	
	
	return ret;
}


/**
* @brief �ж��Ƿ����ļ�β��
* @param [in] fd �ļ�������
* @retval 0 : û�е��ļ�β����
* @retval -1 : �����ļ�β��
* @attention ��������ķ���ֵ�Ƚ����⣬������Դ�
*/
int swpa_flash_file_eof(
	int fd
)
{
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	FLASH_FILE_INFO * pinfo = NULL;
	
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		// fd ���벻����0

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);

	pinfo = (FLASH_FILE_INFO *) pheader->device_param;
	
	return (pinfo->offset == pinfo->len) ? (-1) : (0);
}



/**
* @brief ��õ�ǰ�ļ�ָ��λ��
*
* 
* @param [in] fd �ļ�������
* @retval �ļ�ָ��λ��: �ɹ�
* @retval SWPAR_INVALIDARG : �����Ƿ�
* @retval SWPAR_FAIL : ʧ��
* @attention ��������ķ���ֵ�Ƚ����⣬������Դ�
*/
int swpa_flash_file_tell(
	int fd
)
{
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	FLASH_FILE_INFO * pinfo = NULL;
	
	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);		// fd ���벻����0

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);

	pinfo = (FLASH_FILE_INFO *) pheader->device_param;


	return (pinfo->offset < 0 || pinfo->offset > pinfo->len) ? (SWPAR_FAIL) : (pinfo->offset);
}




/**
* @brief ���ļ��������Ŀ���
*
*
* Ŀǰ��֧�ֻ�ȡ�ļ����size
* 
* @param [in] fd �ļ�������
* @param [in] cmd �����ʶ��Ŀǰ�Ѷ����������
* - SWPA_FILE_IOCTL_FLASH_GET_MAX_SIZE : ��ȡ�ļ����size
* @param [in] args �����־����
* @retval SWPAR_OK : �ɹ�
* @retval SWPAR_FAIL : ʧ��
*/
int swpa_flash_file_ioctl(
	int fd, 
	int cmd, 
	void* args
)
{
	SWPA_FILEHEADER * pheader = (SWPA_FILEHEADER *)fd;
	FLASH_FILE_INFO * pinfo = (FLASH_FILE_INFO *)pheader->device_param;	

	
	//������Ч�Լ��
	SWPA_FILE_CHECK(0 != fd);    	//fd ���벻����0
	//SWPA_CHECK(0 < cmd);    	//cmdȡֵû���޶�����mark��
	//SWPA_CHECK(NULL != args);    	//args �ڲ�ͬcmd�¿���ΪNULL����mark��

	//������ӡ
	SWPA_FILE_PRINT("fd=%d\n", fd);	
	SWPA_FILE_PRINT("cmd=%d\n", cmd);	
	SWPA_FILE_PRINT("args=0x%08x\n", (unsigned int)args);
	
	switch (cmd)
	{
		case SWPA_FILE_IOCTL_GET_MAX_SIZE:
		{
			unsigned int *psize = (unsigned int *)args;
			if (NULL == psize)
			{
				SWPA_FILE_PRINT("Err: invalid cmd arg (NULL) ! [%d]\n", SWPAR_INVALIDARG);
				return SWPAR_INVALIDARG;
			}
			
			*psize = pinfo->seg_len;
		}
		break;
		
		default:
			SWPA_FILE_PRINT("Err: invalid cmd for flash file! [%d]\n", SWPAR_INVALIDARG);
			return SWPAR_INVALIDARG;
	}

	return SWPAR_OK;
}




