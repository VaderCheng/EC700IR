#include "stdio.h"
#include "swpa_hdd.h"
#include <sys/vfs.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <linux/fs.h>
#include <fcntl.h>

#include "swpa.h"
#include "swpa_private.h"
#include "swpa_command.h"


#ifdef SWPA_HDD
#define SWPA_HDD_PRINT(fmt, ...) SWPA_PRINT(fmt, ##__VA_ARGS__)
#define SWPA_HDD_CHECK(arg)      {if (!(arg)){SWPA_PRINT("[%s:%d]Check failed : %s [%d]\n", __FILE__, __LINE__, #arg, SWPAR_INVALIDARG);return SWPAR_INVALIDARG;}}
#else
#define	SWPA_HDD_PRINT(fmt, ...)
#define SWPA_HDD_CHECK(arg)
#endif


static int dev_fd = 3;

#define DEFAULT_SECTOR_SIZE 512
#define DEFAULT_SECTOR_SIZE_STR "512"

static int sector_size = DEFAULT_SECTOR_SIZE;

static void get_sectorsize(void)
{
	int arg;
	if (ioctl(dev_fd, BLKSSZGET, &arg) == 0)
		sector_size = arg;
	if (sector_size != DEFAULT_SECTOR_SIZE)
		printf("Note: sector size is %u (not " DEFAULT_SECTOR_SIZE_STR ")\n",	sector_size);
}



static unsigned int bb_BLKGETSIZE_sectors(int fd)
{
	unsigned long long v64;
	unsigned long longsectors;
	
	get_sectorsize();


	int ret = ioctl(fd, BLKGETSIZE64, &v64);
	if (ret == 0) {
		/* Got bytes, convert to 512 byte sectors */
		v64 >>= 9;
		if (v64 != (unsigned int)v64) {
 ret_trunc:
			/* Not only DOS, but all other partition tables
			 * we support can't record more than 32 bit
			 * sector counts or offsets
			 */
			SWPA_HDD_PRINT("device has more than 2^32 sectors, can't use all of them");
			v64 = (unsigned int)-1L;
		}

		return v64;
	}

	/* Needs temp of type long */
	if (ioctl(fd, BLKGETSIZE, &longsectors)) {
		/* Perhaps this is a disk image */
		long long sz = 0;
		sz = lseek(fd, 0, SEEK_END);
		longsectors = 0;
		if (sz > 0)
		{
			longsectors = (long long)sz / sector_size;
		}
		lseek(fd, 0, SEEK_SET);
	}

	if (sizeof(long) > sizeof(unsigned int)
	 && longsectors != (unsigned int)longsectors
	) {
		goto ret_trunc;
	}

	return longsectors;
}



/**
* @brief ���Ӳ�̵Ĵ�С
*
*
* @param [in] szDev �豸��������,��sda,sdb...,����Ϊϵͳ�ĵ�һ��Ӳ��
* @param [out] size �豸��С������˵���豸����Ӳ�̺�usb,����������Ӳ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
*/
int swpa_hdd_get_totalsize(const char* szDev, long long* size)
{
    SWPA_HDD_CHECK(size != NULL);

	char * devname = (char*)szDev;
	if (NULL == szDev)
	{
		devname = "/dev/sda";
	}

	if (swpa_strstr(devname, "/dev/") == devname)
	{
		unsigned int sectorcount = 0; 

		dev_fd = open(devname, O_RDONLY|O_NONBLOCK);

		if (0 > dev_fd)
		{
			SWPA_HDD_PRINT("Err: failed to open %s!\n", devname);
			return SWPAR_FAIL;
		}

		sectorcount = bb_BLKGETSIZE_sectors(dev_fd);

		*size = (long long)sectorcount<<9;// x 512
		
		close(dev_fd);
	}
	else
	{
		struct statfs fs;
    
        if(statfs(devname, &fs) < 0)
        {
            return SWPAR_FAIL;
        }
		
        *size = (long long)fs.f_bsize * (long long)fs.f_blocks;
	}
	
	
    return SWPAR_OK;
}




/**
* @brief ���Ӳ��Ŀ¼�Ŀ���������С
*
*
* @param [in] szDev �豸�������ƻ��豸��·����,��sda,sdb
* @param [out] size ����������С������˵���豸����Ӳ�̺�usb,����������Ӳ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
*/
int swpa_hdd_get_availsize(const char* szDev, long long* size)
{
	SWPA_HDD_CHECK(szDev != NULL);
	SWPA_HDD_CHECK(size != NULL);
	
	if (!swpa_utils_file_exist(szDev))
	{
		printf("Err: no device %s\n", szDev);
		return SWPAR_FAIL;
	}
	else
	{
		FILE* fp = fopen("/proc/mounts", "r");
		if (NULL == fp)
		{
			printf("Err: failed to read mount info\n");
			return SWPAR_FAIL;
		}

		char szline[256]  = {0};;
		while (!feof(fp))
		{
			fgets(szline, sizeof(szline)-1, fp);
			
			if (NULL != strstr(szline, szDev))
			{
				char* p_end = strstr(szline+strlen(szDev)+1, " ");
				int ilen = (unsigned int)p_end - (unsigned int)szline - strlen(szDev) - 1;
				char szmountpoint[64] = {0};
				struct statfs fs;
				strncpy(szmountpoint, szline+strlen(szDev)+1, ilen);
				if(statfs(szmountpoint, &fs) < 0)
				{
					printf("Err: failed to get fs info.\n");
					// ... ��Ȼû��close
					if( fp != NULL )
					{
						fclose(fp);
						fp = NULL;
					}
					return SWPAR_FAIL;
				}
				else
				{
					*size = (long long)fs.f_bsize * (long long)fs.f_bfree;
					// ... ��Ȼû��close
					if( fp != NULL )
					{
						fclose(fp);
						fp = NULL;
					}
					return SWPAR_OK;
				}
			}
		}

		// ... ��Ȼû��close
		if( fp != NULL )
		{
			fclose(fp);
			fp = NULL;
		}
	}

	printf("Err: failed to get mount info.\n");
	
	return SWPAR_FAIL;
}


/**
* @brief Ӳ�̷���
*
*
* @param [in] szDev �豸��������
* @param [in] size ������С������ָ�������С�����һ��ֵ����Ϊ0��������������
* @retval 0 �ɹ�
* @retval -1 ʧ��
*/
int swpa_hdd_fdisk(const char* szDev, long long size, ...)
{
	SWPA_HDD_CHECK(szDev != NULL && 0 < size);
	
	TiXmlDocument xmlDoc;
	xmlDoc.LinkEndChild(new TiXmlDeclaration("1.0", "GB2312", "yes"));
	TiXmlElement* pRoot = new TiXmlElement("LprApp");		
	
	char szTmp[255] = "";
	swpa_sprintf(szTmp, "%s", szDev);	
	
	swpa_va_list marker;
	swpa_va_start(marker, size);
	while(0 < size)
	{
		swpa_sprintf(szTmp + swpa_strlen(szTmp), " %lld", size);
		size = swpa_va_arg(marker, long long);
	}
	swpa_va_end(marker);
	
	TiXmlElement* pElement = new TiXmlElement("Command");
	pElement->SetAttribute("id", "fdisk_disk");
	pElement->SetAttribute("param", szTmp);
	pElement->SetAttribute("shell", "./command.sh");
	pRoot->LinkEndChild(pElement);
	xmlDoc.LinkEndChild(pRoot);
	
	if(SWPAR_OK == exec_command(xmlDoc)
		&& (pElement = xmlDoc.RootElement())
		&& (pElement = pElement->FirstChildElement("Command"))
		&& !swpa_strcmp(pElement->Attribute("id"), "fdisk_disk")
		&& !swpa_strcmp(pElement->Attribute("return"), "S_OK")
		)
	{
		return SWPAR_OK;
	}

	return SWPAR_FAIL;
}



/**
* @brief Ӳ�̸�ʽ��
*
*
* @param [in] szPartition ��������
* @retval 0 �ɹ�
* @retval -1 ʧ��
*/
int swpa_hdd_format(const char* szPartition,...)
{
	SWPA_HDD_CHECK(NULL != szPartition);
	
  TiXmlDocument xmlDoc;
	xmlDoc.LinkEndChild(new TiXmlDeclaration("1.0", "GB2312", "yes"));
	TiXmlElement* pRoot = new TiXmlElement("LprApp");		
	
	char szTmp[255] = "";
	int fFirst = 1;
	
	swpa_va_list marker;
	swpa_va_start(marker, szPartition);
	while(NULL != szPartition)
	{
		swpa_sprintf(szTmp + swpa_strlen(szTmp), "%s%s", fFirst ? "" : " ", szPartition);		
		szPartition = swpa_va_arg(marker, const char*);
		fFirst = 0;
	}
	swpa_va_end(marker);
	
	TiXmlElement* pElement = new TiXmlElement("Command");
	pElement->SetAttribute("id", "format_disk");
	pElement->SetAttribute("param", szTmp);
	pElement->SetAttribute("shell", "./command.sh");
	pRoot->LinkEndChild(pElement);
	xmlDoc.LinkEndChild(pRoot);
	
	if(SWPAR_OK == exec_command(xmlDoc)
		&& (pElement = xmlDoc.RootElement())
		&& (pElement = pElement->FirstChildElement("Command"))
		&& !swpa_strcmp(pElement->Attribute("id"), "format_disk")
		&& !swpa_strcmp(pElement->Attribute("return"), "S_OK")
		)
	{
		return SWPAR_OK;
	}
	return SWPAR_FAIL;
}

int swpa_hdd_command(int id, const char*szCmd)
{
	TiXmlDocument xmlDoc;
	xmlDoc.LinkEndChild(new TiXmlDeclaration("1.0", "GB2312", "yes"));
	TiXmlElement* pRoot = new TiXmlElement("LprApp");		
	TiXmlElement* pElement = new TiXmlElement("Command");
	pElement->SetAttribute("id", "disk_cmd");
	
	if(NULL == szCmd)
	{
		pElement->SetAttribute("param", id);
	}
	else
	{
		char szTmp[255];
		sprintf(szTmp, "%d %s", id, szCmd);
		pElement->SetAttribute("param", szTmp);
	}
	pElement->SetAttribute("shell", "./command.sh");
	pRoot->LinkEndChild(pElement);
	xmlDoc.LinkEndChild(pRoot);
	
	if(SWPAR_OK == exec_command(xmlDoc)
		&& (pElement = xmlDoc.RootElement())
		&& (pElement = pElement->FirstChildElement("Command"))
		&& !swpa_strcmp(pElement->Attribute("id"), "disk_cmd")
		&& !swpa_strcmp(pElement->Attribute("return"), "S_OK")
		)
	{
		return SWPAR_OK;
	}
	return SWPAR_FAIL;
}

int swpa_hdd_query(int id)
{
	int iret = 0;
	char szText[255];
	sprintf(szText, "/tmp/disk_cmd_%d.txt", id);
	if(swpa_utils_file_exist(szText))
	{
		int fd = open(szText, O_RDONLY);
		if(fd)
		{
			pread(fd, szText, sizeof(szText), 0);
			close(fd);
			iret = swpa_atoi(szText);
			if(iret == 2)
			{
				swpa_utils_file_delete(szText);
			}
		}
	}
	return iret;
}
/**
* @brief Ӳ�̼�⣬���Ӳ����2�У�һ���Ǽ򵥵ļ�⣬�����Ӽ��ɼ����ϡ�һ���ǻ��������޸��������Ҫ��ʱ�ܾá�
*
*
* @param [in] mode ���ģʽ��0�����򵥵ļ�⣬1�����������
* @retval 0 �ɹ�
* @retval -1 ʧ��
*/
int swpa_hdd_check(int mode)
{
	SWPA_HDD_CHECK(mode == 0 || mode == 1);
	
  TiXmlDocument xmlDoc;
	xmlDoc.LinkEndChild(new TiXmlDeclaration("1.0", "GB2312", "yes"));
	TiXmlElement* pRoot = new TiXmlElement("LprApp");		
	TiXmlElement* pElement = new TiXmlElement("Command");
	pElement->SetAttribute("id", "check_disk");
	pElement->SetAttribute("param", mode);
	pElement->SetAttribute("shell", "./command.sh");
	pRoot->LinkEndChild(pElement);
	xmlDoc.LinkEndChild(pRoot);
	
	if(SWPAR_OK == exec_command(xmlDoc)
		&& (pElement = xmlDoc.RootElement())
		&& (pElement = pElement->FirstChildElement("Command"))
		&& !swpa_strcmp(pElement->Attribute("id"), "check_disk")
		&& !swpa_strcmp(pElement->Attribute("return"), "S_OK")
		)
	{
		return SWPAR_OK;
	}
	return SWPAR_FAIL;
}


/**
* @brief Ӳ�̵Ĺ��أ����ڱ���Ӳ�̣�swpa_hdd_format�Ѿ������˹��صĹ��ܣ����Ƕ�������Ӳ�̣���Ҫ�ֶ�����
*
*
* @param [in] szPartition ��������
* @param [in] szDir ���ص�Ŀ¼
* @param [in] szParam ������صĲ��������û�ж���Ĳ�������NULL���ɡ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
*/
int swpa_hdd_mount(const char *szPartition, const char *szDir, const char *szParam)
{
	SWPA_HDD_CHECK(NULL != szPartition && NULL != szDir);
	
  TiXmlDocument xmlDoc;
	xmlDoc.LinkEndChild(new TiXmlDeclaration("1.0", "GB2312", "yes"));
	TiXmlElement* pRoot = new TiXmlElement("LprApp");		
	
	char szTmp[255] = "";
	swpa_strcat(szTmp, szPartition);
	swpa_sprintf(szTmp + swpa_strlen(szTmp), " %s", szDir);		
	
	if(NULL != szParam && swpa_strlen(szParam))
	{
		swpa_sprintf(szTmp + swpa_strlen(szTmp), " %s", szParam);		
	}
	
	TiXmlElement* pElement = new TiXmlElement("Command");
	pElement->SetAttribute("id", "mount_disk");
	pElement->SetAttribute("param", szTmp);
	pElement->SetAttribute("shell", "./command.sh");
	pRoot->LinkEndChild(pElement);
	xmlDoc.LinkEndChild(pRoot);
	
	if(SWPAR_OK == exec_command(xmlDoc)
		&& (pElement = xmlDoc.RootElement())
		&& (pElement = pElement->FirstChildElement("Command"))
		&& !swpa_strcmp(pElement->Attribute("id"), "mount_disk")
		&& !swpa_strcmp(pElement->Attribute("return"), "S_OK")
		)
	{
		return SWPAR_OK;
	}
	return SWPAR_FAIL;
}




/**
* @brief Ӳ�̵�ж��
*
*
* @param [in] szPartition ��������
* @retval 0 �ɹ�
* @retval -1 ʧ��
*/
int swpa_hdd_umount(const char *szPartition)
{
	SWPA_HDD_CHECK(NULL != szPartition);
	
	TiXmlDocument xmlDoc;
	xmlDoc.LinkEndChild(new TiXmlDeclaration("1.0", "GB2312", "yes"));
	TiXmlElement* pRoot = new TiXmlElement("LprApp");		
	TiXmlElement* pElement = new TiXmlElement("Command");
	pElement->SetAttribute("id", "umount_disk");
	pElement->SetAttribute("param", szPartition);
	pElement->SetAttribute("shell", "./command.sh");
	pRoot->LinkEndChild(pElement);
	xmlDoc.LinkEndChild(pRoot);
	
	if(SWPAR_OK == exec_command(xmlDoc)
		&& (pElement = xmlDoc.RootElement())
		&& (pElement = pElement->FirstChildElement("Command"))
		&& !swpa_strcmp(pElement->Attribute("id"), "umount_disk")
		&& !swpa_strcmp(pElement->Attribute("return"), "S_OK")
		)
	{
		return SWPAR_OK;
	}
	return SWPAR_FAIL;
}


