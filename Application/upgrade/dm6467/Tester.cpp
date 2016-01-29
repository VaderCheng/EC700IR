

#include "SWFC.h"
#include "swpa.h"
#include "drv_device.h"
#include "drv_vpif.h"
#include "Tester.h"

#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


#define PRINT SW_TRACE_DEBUG


#define  FPGA_VERSION    0000   


#define CPLD_GP_REG1          0x0C
#define CPLD_GP_REG2          0x0D    

#define BUFFERS_COUNT 3 // ����������
//#define Kernel_alloc		 0 
		
#define IMAGE_WIDTH    1936
#define IMAGE_HEIGHT   1452//1080
#define BYTES_PER_LINE IMAGE_WIDTH//1920
		
#define MAX_IMAGE_BUFFER 1 


static BOOL _fForceStop = FALSE;



CTester::CTester()
{}

CTester::~CTester()
{}


HRESULT CTester::SetForceStopFlag(const BOOL fFlag)
{
	PRINT("Info: SetForceStopFlag to %s \n", fFlag ? "TRUE" : "FALSE");
	_fForceStop = fFlag;
}

BOOL CTester::GetForceStopFlag()
{
	//PRINT("Info: ForceStopFlag = %s \n", _fForceStop ? "TRUE" : "FALSE");
	return _fForceStop;
}
	


HRESULT CTester::TestFlash(const DRV_DEVICE_TYPE dwFlashID, const DWORD dwAddr, const DWORD dwSectorCount, const DWORD dwSectorSize)
{
	PBYTE pbWriteBuf = (PBYTE) swpa_mem_alloc(dwSectorSize);
	if (NULL == pbWriteBuf)
	{
		PRINT("Err: no memory for pbWriteBuf\n");
		return E_OUTOFMEMORY;
	}
	for (DWORD i = 0; i < dwSectorSize; i++)
	{
		pbWriteBuf[i] = i%256;
	}
	PBYTE pbReadBuf = (PBYTE) swpa_mem_alloc(dwSectorSize);
	if (NULL == pbReadBuf)
	{
		SAFE_MEM_FREE(pbWriteBuf);
		PRINT("Err: no memory for pbReadBuf\n");
		return E_OUTOFMEMORY;
	}

	if (GetForceStopFlag())
	{
		PRINT("Info: Test is forced to stop!\n");
		SAFE_MEM_FREE(pbWriteBuf);
		SAFE_MEM_FREE(pbReadBuf);
		return S_OK;
	}
	
	for (DWORD dwSector=0; dwSector < dwSectorCount; dwSector++)
	{
		UINT uLen = dwSectorSize;
		DWORD dwSectorStart = dwAddr + dwSector*dwSectorSize;
		DWORD dwSectorEnd = dwSectorStart + dwSectorSize - 1;

		if (GetForceStopFlag())
		{
			PRINT("Info: Test is forced to stop!\n");
			break;
		}

		//��������
		//PRINT("Info: erasing Flash (%#x~%#x)\n", dwSectorStart, dwSectorEnd);
		if (0 != drv_flash_erase(dwFlashID, dwSectorStart, &uLen)
			|| dwSectorSize != uLen)
		{
			SAFE_MEM_FREE(pbWriteBuf);
			SAFE_MEM_FREE(pbReadBuf);
			PRINT("Err: failed to erase Flash (%#x~%#x)\n", dwSectorStart, dwSectorEnd);
			return E_FAIL;
		}

		if (GetForceStopFlag())
		{
			PRINT("Info: Test is forced to stop!\n");
			break;
		}

		//д����		
		//PRINT("Info: writing Flash (%#x~%#x)\n", dwSectorStart, dwSectorEnd);
		uLen = dwSectorSize;
		if (0 != drv_flash_write(dwFlashID, dwSectorStart, pbWriteBuf, &uLen)
			|| dwSectorSize != uLen)
		{
			SAFE_MEM_FREE(pbWriteBuf);
			SAFE_MEM_FREE(pbReadBuf);
			PRINT("Err: failed to write Flash (%#x~%#x)\n", dwSectorStart, dwSectorEnd);
			return E_FAIL;
		}

		if (GetForceStopFlag())
		{
			PRINT("Info: Test is forced to stop!\n");
			break;
		}

		//������
		swpa_memset(pbReadBuf, 0, dwSectorSize);
		uLen = dwSectorSize;
		//PRINT("Info: reading Flash (%#x~%#x)\n", dwSectorStart, dwSectorEnd);
		if (0 != drv_flash_read(dwFlashID, dwSectorStart, pbReadBuf, &uLen)
				|| dwSectorSize != uLen)
		{
			SAFE_MEM_FREE(pbWriteBuf);
			SAFE_MEM_FREE(pbReadBuf);
			PRINT("Err: failed to read Flash (%#x~%#x)\n", dwSectorStart, dwSectorEnd);
			return E_FAIL;
		}

		if (GetForceStopFlag())
		{
			PRINT("Info: Test is forced to stop!\n");
			break;
		}

		//PRINT("Info: comparing Flash (%#x~%#x)\n", dwSectorStart, dwSectorEnd);
		if (0 != swpa_memcmp(pbWriteBuf, pbReadBuf, dwSectorSize))
		{
			SAFE_MEM_FREE(pbWriteBuf);
			SAFE_MEM_FREE(pbReadBuf);
			PRINT("Err: pbWriteBuf != pbReadBuf\n");
			return E_FAIL;
		}
		//PRINT("Info: Flash (%#x~%#x) OK\n", dwSectorStart, dwSectorEnd);
		if (GetForceStopFlag())
		{
			PRINT("Info: Test is forced to stop!\n");
			break;
		}
	}

	SAFE_MEM_FREE(pbWriteBuf);
	SAFE_MEM_FREE(pbReadBuf);

	return S_OK;
}



/**
* @brief �DSPI FLASH�Ķ�д���ԣ���DDR��һ������д��SPI FLASH�У�����
		  ����DDR����һ�ռ䣬���бȽϡ�ÿ�������������ԡ�
		
* @retval S_OK��ʾSPI FLASH���Գɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::TestDM6467Flash()
{
	//NOTE: ��Ʒ��û���ṩ��ȡFlash ID�Ľӿں������ʲ������²���:
	//��ȡSPI FLASH��ID�򣬿��Ƿ�Ϊ0xEF��0x40��0x19

	
	/*	
	��дSPI FLASH �����������в���:
	�� �����SPI FLASH����Ϊ1M�ֽڣ�FLASH������Ϊ32MB����������1MB����
		����ʱ���������򻮷�Ϊ16��������ÿ������Ϊ64K�ֽڣ�
	�� �ȶԱ����������в���������
		Ȼ���������е�ÿ����Ԫ��8λ��ÿ��������64K��8λ��Ԫ��д�����Ӧ�ĵ�ַƫ������
		д��һ�������󣬰������ڵ�ÿ����Ԫ��������д������ݽ��бȽϡ�
	*/
	const DWORD dwTotalSize = 32*1024*1024;
	const DWORD dwTestSize = 512*1024;
	DWORD dwLen = dwTestSize;
	DWORD dwAddr = dwTotalSize - dwTestSize;  //the start address 
	const DWORD dwSectorSize = 64*1024; //64KB
	DWORD dwSectorCount = dwTestSize/dwSectorSize;

	PRINT("Info: Testing DM6467 Flash...\n");

	if (FAILED(TestFlash(DEVID_SPI_FLASH_DM6467, dwAddr, dwSectorCount, dwSectorSize)))
	{
		PRINT("Info: DM6467 Flash Test ............................................................................ NG!\n");
		return E_FAIL;
	}
	
	PRINT("Info: DM6467 Flash Test ............................................................................ OK!\n");
	
	return S_OK;
	
}



/**
* @brief ��I2C EEPROM��д���ԣ���дEEPROM��ÿһҳ��һ������8ҳ

* @retval S_OK��ʾSPI FLASH��ID���ɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::TestDM6467EEPROM()
{
	/*
	�����I2C EEPROM����Ϊ2KB��������Ϊ128KB����
	����ʱ����Ϊ8ҳ��ÿҳΪ256�ֽڣ�
	��ÿҳ���ж�д���ԣ���ÿҳ��ÿ����Ԫд��õ�Ԫ��Ӧ�ĵ�ַƫ������
	д��һҳ��һһ������ҳ��ÿ����Ԫ�����Ƿ���ȷ��
	*/
	
	const DWORD dwTotalSize = 128*1024;
	const DWORD dwTestSize = 1*1024;
	DWORD dwLen = dwTestSize;
	DWORD dwAddr = dwTotalSize - dwTestSize;  //the start address 
	const DWORD dwPageSize = 256; 
	DWORD dwPageCount = dwTestSize/dwPageSize;

	PBYTE pbWriteBuf = (PBYTE) swpa_mem_alloc(dwPageSize);
	if (NULL == pbWriteBuf)
	{
		PRINT("Err: no memory for pbWriteBuf\n");
		return E_OUTOFMEMORY;
	}
	for (DWORD i = 0; i < dwPageSize; i++)
	{
		pbWriteBuf[i] = i%256;
	}
	PBYTE pbReadBuf = (PBYTE) swpa_mem_alloc(dwPageSize);
	if (NULL == pbReadBuf)
	{
		SAFE_MEM_FREE(pbWriteBuf);
		PRINT("Err: no memory for pbReadBuf\n");
		return E_OUTOFMEMORY;
	}

	if (GetForceStopFlag())
	{
		SAFE_MEM_FREE(pbWriteBuf);
		SAFE_MEM_FREE(pbReadBuf);
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}

	
	PRINT("Info: Testing DM6467 EEPROM...\n");
	
	for (DWORD dwPage = 0; dwPage < dwPageCount; dwPage++)
	{
		UINT uLen = dwPageSize;
		DWORD dwPageStart = dwAddr + dwPage*dwPageSize;
		DWORD dwPageEnd = dwPageStart + dwPageSize - 1;

		if (GetForceStopFlag())
		{
			PRINT("Info: Test is forced to stop!\n");
			break;
		}

		//д����
		//PRINT("Info: writing EEPROM (%#x~%#x)\n", dwPageStart, dwPageEnd);
		if (0 != drv_eeprom_write(DEVID_IIC_EEPROM, dwPageStart, pbWriteBuf, &uLen)
			|| dwPageSize != uLen)
		{
			PRINT("Err: write EEPROM (%#x ~ %#x) ............................................................................ failed\n", dwPageStart, dwPageEnd);
			SAFE_MEM_FREE(pbWriteBuf);
			SAFE_MEM_FREE(pbReadBuf);
			return E_FAIL;
		}

		if (GetForceStopFlag())
		{
			PRINT("Info: Test is forced to stop!\n");
			break;
		}

		//������
		//PRINT("Info: reading EEPROM (%#x~%#x)\n", dwPageStart, dwPageEnd);
		swpa_memset(pbReadBuf, 0, dwPageSize);
		uLen = dwPageSize;
		if (0 != drv_eeprom_read(DEVID_IIC_EEPROM, dwPageStart, pbReadBuf, &uLen)
			|| dwPageSize != uLen)
		{
			PRINT("Err: read EEPROM (%#x ~ %#x) ............................................................................ failed\n", dwPageStart, dwPageEnd);
			SAFE_MEM_FREE(pbWriteBuf);
			SAFE_MEM_FREE(pbReadBuf);
			return E_FAIL;
		}

		if (GetForceStopFlag())
		{
			PRINT("Info: Test is forced to stop!\n");
			break;
		}

		if (0 != swpa_memcmp(pbReadBuf, pbWriteBuf, dwPageSize))
		{
			PRINT("Err: EEPROM (%#x ~ %#x) ............................................................................ test failed!\n", dwPageStart, dwPageEnd);
			SAFE_MEM_FREE(pbWriteBuf);
			SAFE_MEM_FREE(pbReadBuf);
			return E_FAIL;
		}

		if (GetForceStopFlag())
		{
			PRINT("Info: Test is forced to stop!\n");
			break;
		}
		//PRINT("Info: EEPROM (%#x ~ %#x) OK!\n", dwPageStart, dwPageEnd);
	}

	SAFE_MEM_FREE(pbWriteBuf);
	SAFE_MEM_FREE(pbReadBuf);
			
	PRINT("Info: DM6467 EEPROM Test ............................................................................ OK!\n");	

	return S_OK;
}




/**
* @brief ��RTC���ԣ���ʱ��仯��ȡRTCʱ��ֵ

* @retval S_OK:  �ɹ����в���	
* @retval E_FAIL: ����ʧ��
*/
HRESULT CTester::TestDM6467RTC()
{
	/*
	(1)����RTC�ĳ�ʼʱ�䣻
	(2)ÿ��һ���ȡһ��RTCʱ�䣬��ʱ���Ƿ���ȷ������
	*/
	
	PRINT("Info: Testing DM6467 RTC...\n");

	SWPA_TIME sTime = {0x1, 0x1};


	//swpa_watchdog_enable(0);
	
	if (SWPAR_OK != swpa_datetime_settime(&sTime))
	{
		PRINT("Err: set time............................................................................ failed!\n");
		return E_FAIL;
	}

	for (DWORD i=0; i < 5; i++)
	{		
		if (GetForceStopFlag())
		{
			PRINT("Info: Test is forced to stop!\n");
			break;
		}

		SWPA_TIME sPrevTime = sTime;
		
		swpa_watchdog_handshake();
		swpa_thread_sleep_ms(1000);
		swpa_watchdog_handshake();

		if (GetForceStopFlag())
		{
			PRINT("Info: Test is forced to stop!\n");
			break;
		}

		if (SWPAR_OK != swpa_datetime_gettime(&sTime))
		{
			PRINT("Err: get time............................................................................ failed!\n");
			return E_FAIL;
		}

		if (1000 >= (sTime.sec*1000+sTime.msec)-(sPrevTime.sec*1000+sPrevTime.msec))
		{
			PRINT("Err: DM6467 RTC Test ............................................................................ NG!\n");
			return E_FAIL;
		}
	}

	
	PRINT("Info: DM6467 RTC Test ............................................................................ OK!\n");	

	return S_OK;
}




/**
* @brief ��LM75���ԣ���ȡ��ǰ�¶�

* @retval S_OK:  �ɹ����в���	
* @retval E_FAIL: ����ʧ��
*/
HRESULT CTester::TestLM75()
{
	/*
	��ȡLM75���¶�ֵ�����Ƿ�������Χ���¶������
	*/
	
	PRINT("Info: Testing LM75...\n");
	
	INT iTemperature = 0;
	if (0 != drv_device_get_temperature(DEVID_IIC_LM75, &iTemperature))
	{
		PRINT("Info: LM75 Test ............................................................................ NG!\n");
		return E_FAIL;
	}
	
	PRINT("Info: The temperature is %03d C\n", iTemperature);	
	
	PRINT("Info: LM75 Test ............................................................................ OK!\n");

	return S_OK;
}





/**
* @brief ��AT88SC�������Ļ������������������I2CͨѶ

* @param [in] : TestByte : ����Ĳ�������
* @retval S_OK:  �ɹ����в���	
* @retval E_FAIL: ����ʧ��
*/
HRESULT CTester::TestAT88SCOp(const BYTE bTestData)
{
	const UINT uAddr = 0x0A; //	���ñ�׼д�ӿڣ���0x0a��ַдһ���ֽڵ����ݣ����ñ�׼���ӿڣ�����0x0a��ַ�����Ƿ���֮ǰд���ֵ
	BYTE bData = bTestData;
	DWORD dwLen = sizeof(bData);

	swpa_thread_sleep_ms(100);

	if (GetForceStopFlag())
	{
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}
	
	//PRINT("info: dwLen = %u\n", dwLen);
	if (0 != drv_sec_std_write(DEVID_IIC_AT88SC, uAddr, &bData, &dwLen)
		|| sizeof(bData) != dwLen)
	{
		//PRINT("info: dwLen = %u\n", dwLen);
		PRINT("Err: failed to write IIC AT88SC @ %#X\n", uAddr);
		return E_FAIL;
	}
	//PRINT("info: dwLen = %u\n", dwLen);

	if (GetForceStopFlag())
	{
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}
	
	bData = ~bData;
	if (0 != drv_sec_std_read(DEVID_IIC_AT88SC, uAddr, &bData, &dwLen)
		|| sizeof(bData) != dwLen
		|| bTestData != bData
		)
	{
		PRINT("Err: failed to read IIC AT88SC @ %#x\n", uAddr);
		return E_FAIL;
	}

	return S_OK;
	
}



/**
* @brief ��AT88SC�Ǽ��ܶ�д����

* @retval S_OK:  �ɹ����в���	
* @retval E_FAIL: ����ʧ��
*/
HRESULT CTester::TestAT88SC()
{
	/*
	��AT88SC��ͨ�üĴ���д��������ݣ�Ȼ��������Ƚϡ�
	*/
	
	PRINT("Info: Testing AT88SC...\n");

	if (FAILED(TestAT88SCOp(0x00))
		||FAILED(TestAT88SCOp(0xFF))
		||FAILED(TestAT88SCOp(0xAA))
		||FAILED(TestAT88SCOp(0x55))
		)
	{
		PRINT("Info: AT88SC Test ............................................................................ NG!\n");
		return E_FAIL;
	}
		
	
	PRINT("Info: AT88SC Test ............................................................................ OK!\n");

	return S_OK;
}





HRESULT CTester::InitVPIF()
{
	INT iWidth, iHeight;
	INT i = 0;

	DeinitVPIF();
	
	//--------��ʼ����Ƶ�ɼ��������ں˷����ڴ�ķ�ʽ------
	if (0 != drv_capture_init(0, &iWidth, &iHeight))
	{
			PRINT("Err: failed to init VPIF\n");			
			return E_FAIL;
	}
	
	//-----������Ƶ������-------------------------
	for (i =0 ; i < BUFFERS_COUNT ; i++)
	{
		if (0 != drv_capture_put_buffer( NULL ))
		{ 
			PRINT("Err: failed to alloc image buffer\n");	
			drv_capture_stop();
			drv_capture_release();
			return E_FAIL; 
		}
		
	}
	
	//----������Ƶ�ɼ�---------------------------
	if (0 != drv_capture_start())
	{
		PRINT("Err: failed to start capture \n" );
		drv_capture_stop();
		drv_capture_release();
		return E_FAIL;
	}

	return S_OK;
}


HRESULT CTester::DeinitVPIF()
{	
	drv_capture_stop();
	drv_capture_release();

	return S_OK;
}



/**
* @brief ��DM6467�˵�VPIF����

* @retval S_OK��ʾVPIF���Գɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::TestVPIF()
{
	/*
	��CPU��DM6467T������FPGA�����ͼ�񣬲�ͨ�����ͼ�����ݵ������������ж��Ƿ����ͨ����
	*/
	
	PRINT("Info: Testing VPIF...\n");

	DWORD h,w;
	V4L2_BUFFER Buf;	
	INT i = 0;
	
	//static unsigned char image_buffer[MAX_IMAGE_BUFFER][BYTES_PER_LINE*IMAGE_HEIGHT*2];

	unsigned char * image_buffer = NULL;
	
	//------��ȡ��Ƶ����--------------------
	for (i=0; i< MAX_IMAGE_BUFFER; i++)
	{
    PRINT("Info: Getting image...\n");		
		if (0 != drv_capture_get_buffer( &Buf ))
		{
			PRINT("Err: failed to get buffer \n" );
			//DeinitVPIF();
			return E_FAIL;
			
		}
		image_buffer = (unsigned char*) Buf.virt_addr;
		//swpa_memcpy((unsigned char *)image_buffer[i], pBuf->virt_addr, BYTES_PER_LINE*2*IMAGE_HEIGHT);		
	}

	if (GetForceStopFlag())
	{
		drv_capture_put_buffer( &Buf );
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}

  PRINT("Info: Comparing image...\n");
	if ( NULL != image_buffer )
	{
		//-----ͼ������check------------
		for (i=0; i<MAX_IMAGE_BUFFER; i++)
		{		
			for (h = 0; h < 40; h++)
			{
				for ( w = 0; w < IMAGE_WIDTH; w++)
				{
					if ( image_buffer[h*BYTES_PER_LINE + w] != 0x55 )
					{
						PRINT("Err: VPIF Test ............................................................................ NG!\n" );
						return E_FAIL;
					}
					
					if ( image_buffer[ ( h + IMAGE_HEIGHT )*BYTES_PER_LINE + w ] != 0x55 )
					{
						PRINT("Err: VPIF Test ............................................................................ NG!\n" );
						return E_FAIL;
					}				
								
				}
								
			}
			
			for (;h < 80;h++)
			{
				for ( w = 0; w < IMAGE_WIDTH; w++)
				{
					if ( image_buffer[h*BYTES_PER_LINE + w] != 0xAA )
					{
						PRINT("Err: VPIF Test ............................................................................ NG!\n" );
						return E_FAIL;
					}
					
					if ( image_buffer[ ( h + IMAGE_HEIGHT )*BYTES_PER_LINE + w ] != 0xAA )
					{
						PRINT("Err: VPIF Test ............................................................................ NG!\n" );
						return E_FAIL;
					}				
								
				}
							
			}
			
		}
	}
	

	if (0 != drv_capture_put_buffer( &Buf ))
	{
		PRINT("Err: failed to put buffer \n" );
		//DeinitVPIF();
		return E_FAIL;
			
	}		
	
	PRINT("Info: VPIF Test ............................................................................ OK!\n");

	return S_OK;
}



/**
* @brief ����ָ��Ŀ¼�µĽ���ָ����С�����ݶ�д
				
* @retval S_OK��ʾ���Գɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::CheckUSBReadWrite(const CHAR* szDir, const DWORD dwSize)
{
	if (NULL == szDir)
	{
		PRINT("Err: NULL == szDir!\n");
		return E_INVALIDARG;
	}

	CHAR szFileName[256] = {0};
	swpa_strncpy(szFileName, szDir, sizeof(szFileName));
	swpa_strcat(szFileName, "testusb");

	if (SWPAR_OK != swpa_utils_file_exist(szFileName))
	{
		if (SWPAR_OK != swpa_utils_file_create(szFileName))
		{
			PRINT("Err: failed to create %s under %s\n", szFileName, szDir);
			return E_FAIL;
		}
	}

	
	INT fd = swpa_file_open(szFileName, "w");
	if (0 > fd)
	{
		PRINT("Err: failed to open file %s\n", szFileName);
		swpa_utils_file_delete(szFileName);
		return E_FAIL;
	}

	
	PBYTE pbReadBuf = (PBYTE)swpa_mem_alloc(dwSize);
	PBYTE pbWriteBuf = (PBYTE)swpa_mem_alloc(dwSize);
	if (NULL == pbReadBuf || NULL == pbWriteBuf)
	{
		PRINT("Err: no memory for pbReadBuf and pbWriteBuf\n");
		swpa_file_close(fd);
		swpa_utils_file_delete(szFileName);
		return E_FAIL;
	}

	swpa_memset(pbReadBuf, 0, dwSize);
	swpa_memset(pbWriteBuf, 0, dwSize);

	for (DWORD i=0; i<dwSize; i++)
	{
		pbWriteBuf[i] = i%256;
	}

	swpa_file_seek(fd, 0, SWPA_SEEK_SET);
	INT iRetSize = 0;
	if (SWPAR_OK != swpa_file_write(fd, pbWriteBuf, (INT)dwSize, &iRetSize)
		|| (INT)dwSize != iRetSize)
	{
		PRINT("Err: failed to write file %s\n", szFileName);
		swpa_file_close(fd);
		swpa_mem_free(pbReadBuf);
		swpa_mem_free(pbWriteBuf);
		swpa_utils_file_delete(szFileName);
		return E_FAIL;
	}

	swpa_file_close(fd);

	fd = swpa_file_open(szFileName, "r");
	if (0 > fd)
	{
		PRINT("Err: failed to open file %s\n", szFileName);
		swpa_mem_free(pbReadBuf);
		swpa_mem_free(pbWriteBuf);
		swpa_utils_file_delete(szFileName);
		return E_FAIL;
	}

	
	if (SWPAR_OK != swpa_file_read(fd, pbReadBuf, (INT)dwSize, &iRetSize)
		|| (INT)dwSize != iRetSize)
	{
		PRINT("Err: failed to read file %s\n", szFileName);
		swpa_file_close(fd);
		swpa_mem_free(pbReadBuf);
		swpa_mem_free(pbWriteBuf);
		swpa_utils_file_delete(szFileName);
		return E_FAIL;
	}

	if (0 != swpa_memcmp(pbReadBuf, pbWriteBuf, dwSize))
	{
		PRINT("Err: pbReadBuf != pbWriteBuf\n");
		swpa_file_close(fd);
		swpa_mem_free(pbReadBuf);
		swpa_mem_free(pbWriteBuf);
		swpa_utils_file_delete(szFileName);
		return E_FAIL;
	}

	swpa_file_close(fd);
	swpa_mem_free(pbReadBuf);
	swpa_mem_free(pbWriteBuf);
	swpa_utils_file_delete(szFileName);

	return S_OK;
}


/**
* @brief ����USB�洢�豸����U�̡�USB to SataӲ�̽������ݶ�д
				
* @retval S_OK��ʾ���Գɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::TestUSB()
{
	/*
	��USB�����豸�������ݶ�д
	����U�̻�Ӳ�̺��ں��Զ��������̣�����Ҫ�ڳ����й��ش��̣�Ȼ�������fwrite֮��ĺ������ж�д�ˡ�
	*/
	
	PRINT("Info: Testing USB...\n");
	
	const CHAR* szPartition = "/dev/sda1";
	const CHAR* szDir = "/mnt/swtestusb/";

	HdPowerCtl(1);
		
	if (SWPAR_OK != swpa_utils_file_exist(szPartition))
	{
		PRINT("Err: No USB mass storage device %s\n", szPartition);
		HdPowerCtl(0);
		return E_FAIL;
	}

	if (SWPAR_OK != swpa_utils_dir_exist(szDir))
	{
		if (SWPAR_OK != swpa_utils_dir_create(szDir))
		{
			PRINT("Err: failed to create %s\n", szDir);
			HdPowerCtl(0);
			return E_FAIL;
		}
	}	

	if (SWPAR_OK != swpa_hdd_mount(szPartition, szDir, NULL))
	{

		PRINT("Err: failed to mount %s to %s\n", szPartition, szDir);
		HdPowerCtl(0);
		return E_FAIL;
	}

	LONGLONG llAvailSize = 0;
	if (SWPAR_OK != swpa_hdd_get_availsize(szPartition, &llAvailSize))
	{
		PRINT("Err: failed to get available size of %s \n", szPartition);
		HdPowerCtl(0);
		return E_FAIL;
	}

	DWORD dwSize = 0;
	if (llAvailSize > 1*(LONGLONG)1024*1024*1024)//larger than 1GB available, then test 10MB
	{
		dwSize = 10*1024*1024;
	}
	else if (llAvailSize > 1*(LONGLONG)1024*1024)//larger than 1MB available, then test 1MB
	{
		dwSize = 1*1024*1024;
	}
	else if (llAvailSize > 1*(LONGLONG)1024)//larger than 1KB available, then test 1KB
	{
		dwSize = 1*1024;
	}
	else
	{
		PRINT("Err: no enough space under %s \n", szPartition);
		swpa_hdd_umount(szPartition);
		swpa_utils_dir_remove(szDir);
		HdPowerCtl(0);
		return E_FAIL;
	}

	if (GetForceStopFlag())
	{
		swpa_hdd_umount(szPartition);
		swpa_utils_dir_remove(szDir);
		HdPowerCtl(0);
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}

	if (FAILED(CheckUSBReadWrite(szDir, dwSize)))
	{
		PRINT("Err: USB Test ............................................................................ NG!\n");
		swpa_hdd_umount(szPartition);
		swpa_utils_dir_remove(szDir);
		HdPowerCtl(0);
		return E_FAIL;
	}

	if (GetForceStopFlag())
	{
		swpa_hdd_umount(szPartition);
		swpa_utils_dir_remove(szDir);
		HdPowerCtl(0);
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}

	if (SWPAR_OK != swpa_hdd_umount(szPartition))//todo: always failed!!
	{
		PRINT("Err: failed to umount %s \n", szPartition);
		HdPowerCtl(0);
		return E_FAIL;
	}

	if (SWPAR_OK != swpa_utils_dir_remove(szDir))
	{
		PRINT("Err: failed to remove %s\n", szDir);
		HdPowerCtl(0);
		return E_FAIL;
	}

	HdPowerCtl(0);
	
	PRINT("Info: USB Test ............................................................................ OK!\n");

	return S_OK;
}




/**
* @brief : ��һ�����ڡ�

* @param [in] szDevice : The serial port device name, eg, /dev/ttyS0��/dev/ttyUSB0

* @retval Return fd if success, otherwise will return -1 with some msg.
*/
INT CTester::OpenUARTPort(const CHAR* szDevice)
{
    INT fd = -1; /* File descriptor for the port, we return it. */
    INT ret;

    fd = open(szDevice, O_RDWR | O_NOCTTY | O_NDELAY);    
    if (fd == -1)
	{
        PRINT("Err: Unable to open port %s\n", szDevice);
	}

    /* block */
    ret = fcntl(fd, F_SETFL, 0);
    if (ret < 0)
	{
        //My_Txtout("fcntl\n");
        PRINT("Err: fcntl ret %d\n", ret);
	}
    ret = isatty(STDIN_FILENO);
    if (ret == 0)
	{
        //My_Txtout("Standard input is not a terminal device.\n");
        PRINT("Err: Standard input is not a terminal device.\n");
	}        
  
    return fd;
}


/**
* @brief �رմ���

* @param [in] fd: The file description returned by open_port().
* @retval S_OK : ��ʾ�رճɹ�.
* @retval E_FAIL : ��ʾӲ���ײ��������.
*/
HRESULT CTester::CloseUARTPort(INT fd)
{
    if (close(fd) < 0)
    {    		
        //My_Txtout("Unable to close the port.\n"); 
        PRINT("Err: Unable to close the port.\n");
        return E_FAIL;
	}
	
    return S_OK;
}

 
/**
* @brief �Դ��ڽ�������

* @param [in] fd: The file description returned by open_port().
* @param [in] speed    : The baud rate
* @param [in] data_bits : The data bits
* @param [in] parity    : The parity bits
* @param [in] stop_bits : The stop bits
* @retval S_OK : ��ʾ�������óɹ�.
* @retval E_FAIL : ��ʾӲ���ײ��������. 				
*/
HRESULT CTester::SetupUARTPort(INT fd, INT speed, INT data_bits, INT parity, INT stop_bits)
{
    INT speed_arr[] = {B115200, B9600, B38400, B19200, B4800};
    INT name_arr[] = {115200, 9600, 38400, 19200, 4800};
    struct termios opt;
    INT ret= E_FAIL;
    INT i=0;
    INT len=0;

    ret = tcgetattr(fd, &opt);		/* get the port attr */
    if (ret < 0)
    {
        PRINT("Err: Unable to get UART Port attribute\n");
        return E_FAIL;
	}

    opt.c_cflag |= (CLOCAL | CREAD); /* enable the receiver, set local mode */
    opt.c_cflag &= ~CSIZE;			/* mask the character size bits*/

    /* baud rate */
    len = sizeof(speed_arr) / sizeof(INT);
    for (i = 0; i < len; i++)
    {
        if (speed == name_arr[i])
        {
            cfsetispeed(&opt, speed_arr[i]);
            cfsetospeed(&opt, speed_arr[i]);
        }
        if (i == len)
        {
            PRINT("ERr: Unsupported baud rate.\n");
            return E_FAIL;
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
        PRINT("Err: Unsupported data bits.\n");
		return E_FAIL;
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
        PRINT("Err: Unsupported parity bits.");
        return E_FAIL;
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
        PRINT("Err: Unsupported stop bits.");
        return E_FAIL;
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
        PRINT("Err: Unable to setup the port.");
        return E_FAIL;
	}
	
    return S_OK; /* everything is OK! */
}





/**
* @brief ������0������1����ػ����ԡ�
				
* @retval S_OK��ʾ���ڻػ����Գɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::TestUART()
{
	/*
	2�����ܴ����໥�����������ݣ��ػ����������������ж�
	*/
	
	PRINT("Info: UART Closed Loop Testing...\n");

	DWORD Count_temp;
	INT fd0,fd1,ret;
		
	BYTE Tx_Buffer[35]= 
		{0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
		 0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x90,0xA0,0xB0,0xC0,0xD0,0xE0,0xF0,
		 0x00,0xFF,0xAA,0x55,'\n'};
		 
	BYTE Rx_Buffer[512];
	
	volatile BYTE *Tx_ptr,*Rx_ptr;

	
	 //open uart0
	fd0 = OpenUARTPort("/dev/ttyS0");   
	if( fd0 < 0 ) return E_FAIL;
	//setup the port0
	if( SetupUARTPort(fd0, 115200, 8, 'N', 1) != S_OK) return E_FAIL;

	
	//open uart1
	fd1 = OpenUARTPort("/dev/ttyS1");
	if( fd1 < 0 ) return E_FAIL;    
	//setup the port1
	if( SetupUARTPort(fd1, 115200, 8, 'N', 1) != S_OK) return E_FAIL;
	
	
	//---------UART0 send, UART1 received-----------------------------------
	
	drv_device_set_rs485_direction(DEVID_RS485_1, 1);     //UART0 send dir
	drv_device_set_rs485_direction(DEVID_RS485_2, 0); //UART1 received  dir
	swpa_thread_sleep_ms(10);//mdelay(10);
	
	ret = write(fd0, Tx_Buffer, 34);                              //UART0 send data
	if( ret < 0)
	{
		PRINT("Err: UART0 Send Data Fail!!\n");
		CloseUARTPort(fd0);
		CloseUARTPort(fd1);
    	return E_FAIL;
	}

	if (GetForceStopFlag())
	{
		CloseUARTPort(fd0);
		CloseUARTPort(fd1);
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}

	swpa_thread_sleep_ms(100);//mdelay(100); 
	
	ret = read(fd1, Rx_Buffer, 512);                               //UART1 received data 
	if( ret < 0)
	{
		PRINT("Err: UART1 Read Data Fail!!\n");
		CloseUARTPort(fd0);
		CloseUARTPort(fd1);
    	return E_FAIL;
	}

	if (GetForceStopFlag())
	{
		CloseUARTPort(fd0);
		CloseUARTPort(fd1);
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}
    
	Tx_ptr = Tx_Buffer;
	Rx_ptr = Rx_Buffer;

    Count_temp = 34;
    
    while(Count_temp)
    {	
		if(*Tx_ptr != *Rx_ptr)
        {
    		PRINT("Err: UART Test ............................................................................ NG!\n");
			CloseUARTPort(fd0);
			CloseUARTPort(fd1);
			return E_FAIL;
		}
		
		Count_temp--;
		Tx_ptr++;
		Rx_ptr++;
	}

	if (GetForceStopFlag())
	{
		CloseUARTPort(fd0);
		CloseUARTPort(fd1);
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}

	
	//---------UART1 send, UART0 received-----------------------------------
  	/*
  	swpa_memset(Rx_Buffer, 0, sizeof(Rx_Buffer));
  	
	drv_device_set_rs485_direction(DEVID_RS485_2, 1);     //UART1 send dir
	drv_device_set_rs485_direction(DEVID_RS485_1, 0); //UART0 received  dir

	swpa_thread_sleep_ms(10);//mdelay(10);
	
	ret = write(fd1, Tx_Buffer, 34);                              //UART1 send data
	if( ret < 0)
	{
		PRINT("Err: UART1 Send Data Fail!!\n");
		CloseUARTPort(fd0);
		CloseUARTPort(fd1);
    	return E_FAIL;
	}

	if (GetForceStopFlag())
	{
		CloseUARTPort(fd0);
		CloseUARTPort(fd1);
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}
	
	swpa_thread_sleep_ms(100);//mdelay(100); 
	
	ret = read(fd0, Rx_Buffer, 512);                               //UART1 received data 
	if( ret < 0)
	{
		PRINT("Err: UART0 Read Data Fail!!\n");
		CloseUARTPort(fd0);
		CloseUARTPort(fd1);
    	return E_FAIL;
	}   

	if (GetForceStopFlag())
	{
		CloseUARTPort(fd0);
		CloseUARTPort(fd1);
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}
	
	Tx_ptr = Tx_Buffer;
	Rx_ptr = Rx_Buffer;

    Count_temp = 34;
    
    while(Count_temp)
    {	
		if(*Tx_ptr != *Rx_ptr)
        {
    		PRINT("Err: UART Test ............................................................................ NG!\n");
			CloseUARTPort(fd0);
			CloseUARTPort(fd0);
			return E_FAIL;
		}
		
		Count_temp--;
		Tx_ptr++;
		Rx_ptr++;
	}
	*/
		
	CloseUARTPort(fd0);
	CloseUARTPort(fd1);
	
	PRINT("Info: UART Test ............................................................................ OK!\n");

	return S_OK;
}




/**
* @brief �DSPI FLASH�Ķ�д���ԣ���DDR��һ������д��SPI FLASH�У�����
		  ����DDR����һ�ռ䣬���бȽϡ�ÿ�������������ԡ�
		
* @retval S_OK��ʾSPI FLASH���Գɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::TestDM368Flash()
{
	/*	
	��CPU��DM6467T��ͨ��CPLDʹ��ͨ���󣨲�����Ϻ�ԭĬ��ͨ������
	��DM368�˵�SPI FLASH���ж�д���ԣ�û�з���ʹ�õĿռ䣩�����£�
	(1) ��ȡSPI FLASH��ID�򣬿��Ƿ�Ϊ0xEF��0x40��0x18��
	(2) ��дSPI FLASH �����������в��ԣ�
		�� �����SPI FLASH����Ϊ2M�ֽڣ�FLASH������Ϊ16MB����������2MB����
			����ʱ���������򻮷�Ϊ32��������ÿ������Ϊ64K�ֽڣ�
	  	�� �ȶԱ����������в���������Ȼ���������е�ÿ����Ԫ��8λ��ÿ��������64K��8λ��Ԫ��
	  		д�����Ӧ�ĵ�ַƫ������
	  		д��һ�������󣬰������ڵ�ÿ����Ԫ��������д������ݽ��бȽϡ�
	*/

	
	PRINT("Info: Testing DM368 Flash...\n");	
	
	//NOTE: ��Ʒ��û���ṩ��ȡFlash ID�Ľӿں������ʲ������²���:
	//(1) ��ȡSPI FLASH��ID�򣬿��Ƿ�Ϊ0xEF��0x40��0x18��


	//(2) ��дSPI FLASH ������
	const DWORD dwTotalSize = 16*1024*1024;
	const DWORD dwTestSize = 512*1024; //Note: shorten 2MB to 1MB to save testing time
	DWORD dwLen = dwTestSize;
	DWORD dwAddr = dwTotalSize - dwTestSize;  //the start address 
	const DWORD dwSectorSize = 64*1024; //64KB
	DWORD dwSectorCount = dwTestSize/dwSectorSize;

	PRINT("Info: Testing DM368 Flash...\n");

	if (FAILED(TestFlash(DEVID_SPI_FLASH_DM368, dwAddr, dwSectorCount, dwSectorSize)))
	{
		PRINT("Info: DM368 Flash Test ............................................................................ NG!\n");
		return E_FAIL;
	}
	
	PRINT("Info: DM368 Flash Test ............................................................................ OK!\n");

	return S_OK;
}



/**
* @brief DM6467T to FPGA IO CHECK

* @param [in] Testdata: ����Ĳ�������,ֻ�е�3λ��Ч��
* @retval S_OK : ��ʾIO���Գɹ�.
* @retval E_FAIL : ��ʾӲ���ײ��������.
*/
HRESULT CTester::CheckIODM6467ToFPGA(BYTE Testdata)
{
	BYTE temp8 = 0;
	INT DataTemp = 0;
	DataTemp = Testdata;
	
	//---------------------------------------------------------------------
		
	//drv_gpio_set_direction(DEVID_GPIO,1, GPIO_DIR_INPUT);
	//drv_gpio_set_direction(DEVID_GPIO,38, GPIO_DIR_OUTPUT);
	
	//---------------------------------------------------------------------
	
	drv_cpld_read(DEVID_SPI_CPLD, CPLD_GP_REG2, &temp8);
	
	temp8 = ( temp8 & 0x03 ) | ( ( DataTemp & 0x03 ) << 2 );
	drv_cpld_write(DEVID_SPI_CPLD, CPLD_GP_REG2, temp8);   //���� FPGA_CPLD_GP3,FPGA_CPLD_GP2		
	drv_gpio_set_output(DEVID_GPIO,38, ( DataTemp & 0x04 ) >> 2);  // ����SOFT_TRIG  
	               		
	swpa_thread_sleep_ms(10);//mdelay(10);
		
	drv_cpld_read(DEVID_SPI_CPLD, CPLD_GP_REG2, &temp8);

	drv_gpio_get_input(DEVID_GPIO,1, &DataTemp);
	
	
	temp8 &= 0x03;
	temp8 |= ( ( DataTemp & 0x01 ) << 2 ) ;     //����VD�ź�
	
	DataTemp = (~Testdata) & 0x07;
	if( DataTemp != temp8 ) 
	{
		return E_FAIL;
	}
	
	return S_OK;
		
}




/**
* @brief ��DM6467T to FPGA IO TEST

* @retval S_OK��ʾIO���Գɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::TestDM6467FPGAIO()
{
	/*
	*/
	
	PRINT("Info: Testing DM6467 to FPGA IO...\n");

	DWORD temp32 = 0;
	BYTE temp8 = 0;
	BYTE szVersion[32];


	if ( 0 != drv_fpga_get_version(DEVID_SPI_FPGA, szVersion, sizeof(szVersion)) )
	{
		PRINT("Err: Failed to get FPGA Version !\n");
		return E_FAIL;
	}

	/* //todo: check with wu, why this?
	temp32 = *( (DWORD* )szVersion );

	if (FPGA_VERSION != temp32)
	{
		PRINT("Err: Failed to Get FPGA Version !\n");    //FPGA Version
		return E_FAIL;
	}
	*/

	//-----------------------------------------------------------

	if (FAILED(CheckIODM6467ToFPGA(0x00))||
		FAILED(CheckIODM6467ToFPGA(0x05))||
		FAILED(CheckIODM6467ToFPGA(0x02))||
		FAILED(CheckIODM6467ToFPGA(0x07))
		)
	{
		PRINT("Err: DM6467 to FPGA IO Test ............................................................................ NG!\n");
		return E_FAIL;
	}

	
	PRINT("Info: DM6467 to FPGA IO Test ............................................................................ OK!\n");

	return S_OK;
}





/**
* @brief DM6467T to DM368 IO CHECK

* @param [in] Testdata: ����Ĳ�������,ֻ�е�2λ��Ч��
* @retval S_OK : ��ʾIO���Գɹ�.
* @retval E_FAIL : ��ʾӲ���ײ��������.
*/
HRESULT CTester::CheckIODM6467ToDM368(BYTE Testdata)
{
	BYTE temp8 = 0, DataTemp = 0;
	
	DataTemp = Testdata;
	
	drv_cpld_read(DEVID_SPI_CPLD, CPLD_GP_REG1, &temp8);
	drv_cpld_write(DEVID_SPI_CPLD, CPLD_GP_REG1, (temp8 & 0x07) | ( (DataTemp & 0x03) << 3 )); //����368_GP4_OUT,368_GP3_OUT 
	swpa_thread_sleep_ms(10);//mdelay(10);
	
	switch(Testdata & 0x03)
	{
		case 0x00:
			DataTemp = 0x00;
		break;
		
		case 0x01:
			DataTemp = 0x02;
		break;
		
		case 0x02:
			DataTemp = 0x05;
		break;
		
		case 0x03:
			DataTemp = 0x07;
		break;
		
		default:
		break;
		
	}
	
	drv_cpld_read(DEVID_SPI_CPLD, CPLD_GP_REG1, &temp8);
	temp8 &= 0x07;	
	
	if( DataTemp != temp8 )
	{
		return E_FAIL;
	}
	
	return S_OK;
		
}


HRESULT CTester::SetDM368ToIOTestMode()
{
	CSWTCPSocket cTestSock;
	INT iCountDown = 16;

	while (FAILED(cTestSock.Create()) && 0 < iCountDown--)
	{
		PRINT("Err: failed to create socket to DM368, trying again...\n");
		CSWApplication::Sleep(500);

		if (GetForceStopFlag())
		{			
			PRINT("Info: Test is forced to stop!\n");
			return S_OK;
		}
	}

	if (GetForceStopFlag())
	{			
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}

	if ( 0 >= iCountDown)
	{
		PRINT("Err: failed to create socket to DM368\n");
		return E_FAIL;
	}
	
	const CHAR* szDm368IP = "123.123.219.219";
	const WORD  wDm368Port = 2133;
	
	cTestSock.SetRecvTimeout(4000);
	cTestSock.SetSendTimeout(4000);
	
	iCountDown = 16;
	while (FAILED(cTestSock.Connect(szDm368IP, wDm368Port)) && 0 < iCountDown--)
	{
		if (GetForceStopFlag())
		{			
			PRINT("Info: Test is forced to stop!\n");
			return S_OK;
		}
		
		PRINT("Err: failed to connect to %s:%d, trying again...\n", szDm368IP, wDm368Port);
		CSWApplication::Sleep(500);
	}

	
	if (GetForceStopFlag())
	{			
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}


	if ( 0 >= iCountDown)
	{
		PRINT("Err: failed to create socket to DM368\n");
		return E_FAIL;
	}
	
	// �����ͷ
	typedef struct tag_CameraCmdHeader
	{
		DWORD dwID;
		DWORD dwInfoSize;
	}
	CAMERA_CMD_HEADER;

	
	// ��Ӧ��ͷ
	typedef struct tag_CameraResponseHeader
	{
		DWORD dwID;
		DWORD dwInfoSize;
		INT   iReturn;
	}
	RESPONSE_CMD_HEADER;

	

	CAMERA_CMD_HEADER sHeader;
	sHeader.dwID = 0XFFCC00F0;
	sHeader.dwInfoSize = 0;

	iCountDown = 16;
	while (FAILED(cTestSock.Send(&sHeader, sizeof(sHeader), NULL)) && 0 < iCountDown)
	{		
		if (GetForceStopFlag())
		{			
			PRINT("Info: Test is forced to stop!\n");
			return S_OK;
		}	

		PRINT("Err: failed to send CMD(%#X) to %s:%d, trying again...\n", sHeader.dwID, szDm368IP, wDm368Port);
		CSWApplication::Sleep(200);
		iCountDown --;
	}

	if (GetForceStopFlag())
	{			
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}

	if ( 0 >= iCountDown)
	{
		PRINT("Err: failed to send CMD(%#X) to %s:%d\n", sHeader.dwID, szDm368IP, wDm368Port);
		return E_FAIL;
	}

	RESPONSE_CMD_HEADER sResponse;
	swpa_memset(&sResponse, 0, sizeof(sResponse));
	iCountDown = 16;
	while (FAILED(cTestSock.Read(&sResponse, sizeof(sResponse), NULL)) && 0 < iCountDown )
	{
		if (GetForceStopFlag())
		{			
			PRINT("Info: Test is forced to stop!\n");
			return S_OK;
		}
		
		PRINT("Err: failed to read CMD(%#X) response from %s:%d, trying again...\n", sHeader.dwID, szDm368IP, wDm368Port);
		CSWApplication::Sleep(200);
		iCountDown --;
	}

	if (GetForceStopFlag())
	{			
		PRINT("Info: Test is forced to stop!\n");
		return S_OK;
	}

	if ( 0 >= iCountDown)
	{
		PRINT("Err: failed to recv CMD(%#X) response from %s:%d\n", sHeader.dwID, szDm368IP, wDm368Port);
		return E_FAIL;
	}
	
	return ( sResponse.dwID != sHeader.dwID ) ? E_FAIL : S_OK;

}



/**
* @brief ��DM6467T to DM368 IO TEST

* @retval S_OK��ʾIO���Գɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::TestDM6467DM368IO()
{
	/*
	*/
	
	PRINT("Info: Testing DM6467 to DM368 IO...\n");

	if (FAILED(SetDM368ToIOTestMode()))
	{
		PRINT("Err: failed to send CMD to DM368 for starting IO Test!\n");
		return E_FAIL;	
	}
		
	if( FAILED(CheckIODM6467ToDM368(0x00))||
		FAILED(CheckIODM6467ToDM368(0x01))||
		FAILED(CheckIODM6467ToDM368(0x02))||
		FAILED(CheckIODM6467ToDM368(0x03))
		)
	{
		PRINT("Err: DM6467 to DM368 IO Test ............................................................................ NG!\n");
		return E_FAIL;	
	}
	
	PRINT("Info: DM6467 to DM368 IO Test ............................................................................ OK!\n");

	return S_OK;
	
}




/**
* @brief ��DM368 to FPGA IO TEST,DM6467 ͨ��USB���������֪DM368���в��ԣ������ؽ��

* @retval S_OK��ʾ���Գɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::TestDM368FPGAIO()
{
	/*
	*/
	
	PRINT("Info: Testing DM368 to FPGA IO...\n");

	if (FAILED(SetDM368ToIOTestMode()))
	{
		PRINT("Err: DM368 to FPGA IO Test ............................................................................ NG\n");
		return E_FAIL;
	}
	
	PRINT("Info: DM368 to FPGA IO Test ............................................................................ OK!\n");

	return S_OK;
}


/**
* @brief �����������л����������
@param [in] ctl: �л���.���ؿ��ơ�
					0�� OFF�� 1��ON ��
* @retval �ޡ�

*/
void CTester::ProductSwitcherCtl(BYTE ctl)
{

	 unsigned int temp32 =0 ;
	 FPGA_STRUCT FPGA_INFO;
	 FPGA_INFO.count = 1;
	 FPGA_INFO.regs[0].addr = (0x908) | ( 1 << 12 );

	if(ctl)
		temp32 |= ( 2 << 0 );
	else
		temp32 |= ( 1 << 0 );
		
	FPGA_INFO.regs[0].data = temp32;
	drv_fpga_write( DEVID_SPI_FPGA,&FPGA_INFO);

	if (GetForceStopFlag())
	{			
		PRINT("Info: Test is forced to stop!\n");
		return;
	}
	
	swpa_thread_sleep_ms(500);//mdelay(500);

	if (GetForceStopFlag())
	{			
		PRINT("Info: Test is forced to stop!\n");
		return;
	}

	FPGA_INFO.regs[0].data = 0x00;
	drv_fpga_write( DEVID_SPI_FPGA,&FPGA_INFO);

}

/**
* @brief �����������л������Ʋ���
@param [in] TestNum: �л������Դ�����

* @retval �ޡ�
*/

void CTester::ProductSwitcherTest(BYTE TestNum)
{

	unsigned int i =0 ;
	PRINT("Info: Testing SwitcherCtl...\n");
	for( i = 0 ; i < TestNum ; i++)
	{
		ProductSwitcherCtl(1);   //  ON
		if (GetForceStopFlag())
		{			
			PRINT("Info: Test is forced to stop!\n");
			return ;
		}
		swpa_thread_sleep_ms(1000);//mdelay(1000);
		if (GetForceStopFlag())
		{			
			PRINT("Info: Test is forced to stop!\n");
			return ;
		}
		ProductSwitcherCtl(0);  //  OFF
		if (GetForceStopFlag())
		{			
			PRINT("Info: Test is forced to stop!\n");
			return ;
		}
		swpa_thread_sleep_ms(1000);//mdelay(1000);
		if (GetForceStopFlag())
		{			
			PRINT("Info: Test is forced to stop!\n");
			return ;
		}
	}
		
}


/**
* @brief ����������DC��Ȧ�������
@param [in] ctl: DC��Ȧ.���ؿ��ơ�
					0�� OFF�� 1��ON ��
* @retval �ޡ�

*/
void CTester::ProductDcCtl(BYTE ctl)
{

	 unsigned int temp32 =0 ;
	 FPGA_STRUCT FPGA_INFO;
	 FPGA_INFO.count = 1;
	 FPGA_INFO.regs[0].addr = (0x907) | ( 1 << 12 );

	if(ctl)
		temp32 &= ~( 3 << 0 );
	else
		temp32 |= ( 3 << 0 );
			
	FPGA_INFO.regs[0].data = temp32;
	drv_fpga_write(DEVID_SPI_FPGA,&FPGA_INFO);
	
	
}



/**
* @brief ����������DC��Ȧ������Ʋ���
@param [in] TestNum: �л������Դ�����?

* @retval �ޡ�

*/
void CTester::ProductDcCtlTEST(BYTE TestNum)
{
	
	unsigned int i = 0 ;
	
	for( i = 0 ; i < TestNum ; i++)
	{
		ProductDcCtl(1);   //  ON
		if (GetForceStopFlag())
		{			
			PRINT("Info: Test is forced to stop!\n");
			return ;
		}
		swpa_thread_sleep_ms(500);//mdelay(500);
		if (GetForceStopFlag())
		{			
			PRINT("Info: Test is forced to stop!\n");
			return ;
		}
		ProductDcCtl(0);  //  OFF
		if (GetForceStopFlag())
		{			
			PRINT("Info: Test is forced to stop!\n");
			return ;
		}
		swpa_thread_sleep_ms(500);//mdelay(500);
		if (GetForceStopFlag())
		{			
			PRINT("Info: Test is forced to stop!\n");
			return ;
		}

	}

}


/**
* @brief ��Ӳ�̵�Դ����
@param [in] ctl: Ӳ�̵�Դ���ؿ��ơ�
					0�� OFF�� 1��ON ��
* @retval �ޡ�

*/

void CTester::HdPowerCtl(BYTE ctl)
{
	
	unsigned int temp32 = 0;
	 FPGA_STRUCT FPGA_INFO;
	 FPGA_INFO.count = 1;
	 FPGA_INFO.regs[0].addr = (0x90E) | ( 1 << 12 );
	 
	if(ctl)
		temp32 |= ( 1 << 0 );
	else
		temp32 &= ~( 1 << 0 );

	FPGA_INFO.regs[0].data = temp32;
	drv_fpga_write(DEVID_SPI_FPGA,&FPGA_INFO);

}

