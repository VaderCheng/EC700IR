

#include "SWFC.h"
#include "SWLog.h"
#include "drv_device.h"
#include "Tester.h"
#include "SWUtils.h"

#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "libmtd.h"
#include "linux/types.h"
#include "mtd/mtd-user.h"


#define FPGA_ID_ADR   0x900     //
#define FPGA_VERSION  0x56425F02      //VT_02



static BOOL _fForceStop = FALSE;


CTester::CTester()
{}

CTester::~CTester()
{}


HRESULT CTester::SetForceStopFlag(const BOOL fFlag)
{
	SW_TRACE_NORMAL("Info: SetForceStopFlag to %s \n", fFlag ? "TRUE" : "FALSE");
	_fForceStop = fFlag;
}

BOOL CTester::GetForceStopFlag()
{
	//SW_TRACE_NORMAL("Info: ForceStopFlag = %s \n", _fForceStop ? "TRUE" : "FALSE");
	return _fForceStop;
}
	



/**
* @brief �DNAND FLASH�Ķ�д���ԣ���DDR��һ������д��NAND FLASH�У�����
		  ����DDR����һ�ռ䣬���бȽϡ�ÿ��block�������ԡ�
		
* @retval S_OK��ʾSPI FLASH���Գɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::TestFlash()
{
	/*	
	��дNAND FLASH ��block�����в���:
	�� �����NAND FLASH����Ϊ512K�ֽڣ�FLASH������Ϊ512MB�����Ա������򣩣�
		����ʱ���������򻮷�Ϊ4��block��ÿ��blockΪ64K�֣�128KB����
	�� �ȶԱ���block���в���������
		Ȼ����block�е�ÿ����Ԫ��16λ��ÿ��block��64K��16λ��Ԫ��д�����Ӧ�ĵ�ַƫ������
		д��һ��block�󣬰�block�ڵ�ÿ����Ԫ��������д������ݽ��бȽϡ�
	*/
	
	struct mtd_dev_info mtd = {
		0,0,0,0,"","",0,0,0,0,0,0,0,0,0
	};
	libmtd_t mtd_desc;
    static const char	*mtd_device = "/dev/mtd11";	//����ר�÷���
	int fd = -1;
	int ret = S_OK;
	unsigned char write_mode = MTD_OPS_RAW;
	
	//const DWORD dwTotalSize = 256*1024*1024;
	const int pagelen = 2048;	//?��ֵ��
	const DWORD blockSize = pagelen * 64;
	//const DWORD dwTestblockstart = 0xFD80000;	//Ӳ����ַ��
	//const DWORD dwTestAddr = 0xFD80000;
	const DWORD dwTestblockstart = 0x0;	//������ʼ
	const DWORD dwTestAddr = 0x0;
	
	const DWORD dwTestSize = 512*1024;//512KByte
	//DWORD dwLen = dwTestSize;
	DWORD dwblockCount = dwTestSize/blockSize;
	DWORD mtdoffset = 0;
	
	
	//unsigned char writebuf[128 *1024];
	//unsigned char readbuf[128 *1024];
	unsigned char *writebuf = (unsigned char *)swpa_mem_alloc(128 *1024);
	if (NULL == writebuf)
	{
		SW_TRACE_NORMAL("Err: malloc write buf failed!");
		return E_OUTOFMEMORY;
	}
	unsigned char *readbuf = (unsigned char *)swpa_mem_alloc(128 *1024);
	if (NULL == readbuf)
	{
		SW_TRACE_NORMAL("Err: malloc read buf failed!");
		SAFE_MEM_FREE(writebuf);
		return E_OUTOFMEMORY;
	}
	unsigned short *wr_ptr16 = (unsigned short* )(writebuf);
	//unsigned short *rd_ptr16 = (unsigned short* )(readbuf);
	unsigned int i = 0,j=0;
	
	//create test data
	for(i = 0; i < ( 64 * 1024); i++)
	{
		wr_ptr16[i] = i; 
	}
	
	
	SW_TRACE_NORMAL("Info: Testing NAND Flash...............\n");
	
	/* Open the device */
	if ((fd = open(mtd_device, O_RDWR)) < 0)
	{
		SW_TRACE_NORMAL("%s\n", mtd_device);
		//close(fd);	
		SAFE_MEM_FREE(readbuf);
		SAFE_MEM_FREE(writebuf);
		return E_FAIL;
	}
	
	mtd_desc = libmtd_open();
	if (!mtd_desc)
	{
		SW_TRACE_NORMAL("can't initialize libmtd\n");
		//libmtd_close(mtd_desc);
		close(fd);	
		SAFE_MEM_FREE(readbuf);
		SAFE_MEM_FREE(writebuf);
		return E_FAIL;
	}
	
	/* Fill in MTD device capability structure */
	if (mtd_get_dev_info(mtd_desc, mtd_device, &mtd) < 0)
	{
		SW_TRACE_NORMAL("mtd_get_dev_info failed");
		libmtd_close(mtd_desc);
		close(fd);
		SAFE_MEM_FREE(readbuf);
		SAFE_MEM_FREE(writebuf);
		return E_FAIL;
	}
	
	SW_TRACE_NORMAL("Info: mtd oobsize %d,subpagesize %d,ebsize %d,miniosize %d\n",
		mtd.oob_size,mtd.subpage_size,mtd.eb_size,mtd.min_io_size);
	
	//erase block	
	//@eb_size: eraseblock size
	
	for (i = dwTestblockstart; i < dwTestblockstart + dwTestSize; i += mtd.eb_size) {
		//skip bad?
	
		if (mtd_erase(mtd_desc, &mtd, fd, i / mtd.eb_size)) {
			SW_TRACE_NORMAL("%s: MTD Erase failure\n", mtd_device);
			ret = E_FAIL;
			goto closeall;		
		}
	}
	
	// begin test NAND FLASH write and read.
	for(i = 0 ; i < dwblockCount; i++)
	{

		if (GetForceStopFlag())
		{
			return S_OK;
		}
		
		/*  write test data to a block*/
		j = 0;
		mtdoffset = dwTestAddr + blockSize * i;
		while ( j < blockSize)
		{
			//@min_io_size: minimum input/output unit size
			if( mtd_write(mtd_desc, &mtd, fd, mtdoffset / mtd.eb_size,
				mtdoffset % mtd.eb_size,&writebuf[j],mtd.min_io_size,NULL,0,write_mode) )
			{	
				SW_TRACE_NORMAL("%s: MTD write failure\n", mtd_device);
				ret = E_FAIL;	
				goto closeall;
			}
								
			mtdoffset += mtd.min_io_size;
			j += pagelen;						
		}
		
		/* read test data from a block*/
		memset(readbuf, 0x00, blockSize);	
		j = 0;
		mtdoffset = dwTestAddr + blockSize * i;
		while ( j < blockSize)
		{
			if( mtd_read(&mtd, fd, mtdoffset / mtd.eb_size, mtdoffset % mtd.eb_size, &readbuf[j], mtd.min_io_size))
			{
				SW_TRACE_NORMAL("MTD read failure.\n");
				ret = E_FAIL;
				goto closeall;
			}

			mtdoffset += mtd.min_io_size;
			j += pagelen;
		}

		if (GetForceStopFlag())
		{
			return S_OK;
		}
		
		/* cheeck the data */
		for(j = 0 ; j < blockSize ; j++)
		{
			if( readbuf[j] != writebuf[j] )
			{
				SW_TRACE_NORMAL("data check failure.\n");
				ret = E_FAIL;
				goto closeall;
			}
		}	
	
	}
	
closeall:
	libmtd_close(mtd_desc);
	close(fd);	
	
	SAFE_MEM_FREE(readbuf);
	SAFE_MEM_FREE(writebuf);

    swpa_utils_shell("flash_eraseall /dev/mtd11", NULL);

	if(ret == E_FAIL)
	{	
		SW_TRACE_NORMAL("Info: NAND Flash Test -- NG!\n");
	}else{
		SW_TRACE_NORMAL("Info: NAND Flash Test -- OK!\n");
	}
			
	return ret;
}


/**
* @brief ��I2C EEPROM��д���ԣ���дEEPROM��ÿһҳ��һ������8ҳ

* @retval S_OK��ʾSPI FLASH��ID���ɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::TestEEPROM()
{
	/*
	�����I2C EEPROM����Ϊ2KB��������Ϊ128KB����
	����ʱ����Ϊ8ҳ��ÿҳΪ256�ֽڣ�
	��ÿҳ���ж�д���ԣ���ÿҳ��ÿ����Ԫд��õ�Ԫ��Ӧ�ĵ�ַƫ������
	д��һҳ��һһ������ҳ��ÿ����Ԫ�����Ƿ���ȷ��
	*/
	HRESULT hRet = S_OK;
	const DWORD dwTotalSize = 128*1024;
	const DWORD dwTestSize = 1*1024;
	//DWORD dwLen = dwTestSize;
	DWORD dwAddr = dwTotalSize - dwTestSize;  //the start address 
	const DWORD dwPageSize = 256; 
	DWORD dwPageCount = dwTestSize/dwPageSize;

	PBYTE pbWriteBuf = (PBYTE) swpa_mem_alloc(dwPageSize);
	if (NULL == pbWriteBuf)
	{
		SW_TRACE_NORMAL("Err: no memory for pbWriteBuf\n");
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
		SW_TRACE_NORMAL("Err: no memory for pbReadBuf\n");
		return E_OUTOFMEMORY;
	}	

	
	SW_TRACE_NORMAL("Info: Testing I2C EEPROM...\n");
	
	for (DWORD dwPage = 0; dwPage < dwPageCount; dwPage++)
	{
		UINT uLen = dwPageSize;
		DWORD dwPageStart = dwAddr + dwPage*dwPageSize;
		DWORD dwPageEnd = dwPageStart + dwPageSize - 1;

		if (GetForceStopFlag())
		{
			return S_OK;
		}

		//д����
		SW_TRACE_NORMAL("Info: writing EEPROM (%#x~%#x)\n", dwPageStart, dwPageEnd);
		if (0 != drv_eeprom_write(DEVID_IIC_EEPROM, dwPageStart, pbWriteBuf, &uLen)
			|| dwPageSize != uLen)
		{
			SW_TRACE_NORMAL("Err: failed to write EEPROM (%#x ~ %#x)\n", dwPageStart, dwPageEnd);
			hRet = E_FAIL;
			goto FreeMem;
		}

		if (GetForceStopFlag())
		{
			return S_OK;
		}
		
		//������
		SW_TRACE_NORMAL("Info: reading EEPROM (%#x~%#x)\n", dwPageStart, dwPageEnd);
		swpa_memset(pbReadBuf, 0, dwPageSize);
		uLen = dwPageSize;
		if (0 != drv_eeprom_read(DEVID_IIC_EEPROM, dwPageStart, pbReadBuf, &uLen)
			|| dwPageSize != uLen)
		{
			SW_TRACE_NORMAL("Err: failed to read EEPROM (%#x ~ %#x)\n", dwPageStart, dwPageEnd);
			hRet = E_FAIL;
			goto FreeMem;
		}

		if (0 != swpa_memcmp(pbReadBuf, pbWriteBuf, dwPageSize))
		{
			SW_TRACE_NORMAL("Err: EEPROM (%#x ~ %#x) test failed!\n", dwPageStart, dwPageEnd);
			hRet = E_FAIL;
			goto FreeMem;
		}
		SW_TRACE_NORMAL("Info: EEPROM (%#x ~ %#x) OK!\n", dwPageStart, dwPageEnd);
	}
	
	SW_TRACE_NORMAL("Info: I2C EEPROM Test -- OK!\n");	

FreeMem:

	SAFE_MEM_FREE(pbReadBuf);
	SAFE_MEM_FREE(pbWriteBuf);

	return hRet;
}


/**
* @brief ��RTC���ԣ���ʱ��仯��ȡRTCʱ��ֵ

* @retval S_OK:  �ɹ����в���	
* @retval E_FAIL: ����ʧ��
*/
HRESULT CTester::TestRTC()
{
	/*
	(1)����RTC�ĳ�ʼʱ�䣻
	(2)ÿ��һ���ȡһ��RTCʱ�䣬��ʱ���Ƿ���ȷ������
	*/
	
	SW_TRACE_NORMAL("Info: Testing DM8127 RTC...\n");

	HRESULT hr = S_OK;
	SWPA_TIME sTestTime = {0x1, 0x1};
	SWPA_TIME sCurTime;
	DWORD dwPrevTick=0, dwTick=0;

	if (SWPAR_OK != swpa_datetime_gettime(&sCurTime))
	{
		SW_TRACE_DEBUG("Err: get time failed!\n");
		SW_TRACE_NORMAL("Info: DM8127 RTC Test ..... NG!\n");		
		return E_FAIL;
	}

	if (SWPAR_OK != swpa_datetime_settime(&sTestTime))
	{
		SW_TRACE_DEBUG("Err: set time failed!\n");
		SW_TRACE_NORMAL("Info: DM8127 RTC Test ............ NG!\n");		
		return E_FAIL;
	}
	
	dwPrevTick = swpa_datetime_gettick();

	for (DWORD i=0; i < 5; i++)
	{		
		if (GetForceStopFlag())
		{
			SW_TRACE_DEBUG("Info: Test is forced to stop!\n");
			break;
		}

		SWPA_TIME sPrevTime = sTestTime;
		
		swpa_thread_sleep_ms(1000);

		if (SWPAR_OK != swpa_datetime_gettime(&sTestTime))
		{
			SW_TRACE_DEBUG("Err: get time failed!\n");
			hr = E_FAIL;
			break;
		}

		if (1000 > (sTestTime.sec*1000+sTestTime.msec)-(sPrevTime.sec*1000+sPrevTime.msec))
		{
			hr = E_FAIL;
			break;
		}
	}

	dwTick = swpa_datetime_gettick();

	sCurTime.sec += (sCurTime.msec + dwTick - dwPrevTick) / 1000;
	sCurTime.msec = (sCurTime.msec + dwTick - dwPrevTick) % 1000;

	if (SWPAR_OK != swpa_datetime_settime(&sCurTime))
	{
		SW_TRACE_DEBUG("Err: set time failed!\n");
		SW_TRACE_NORMAL("Info: DM8127 RTC Test .............................. NG!\n");		
		return E_FAIL;
	}
	
	SW_TRACE_NORMAL("Info: DM8127 RTC Test ............................................................................ %s!\n",
		FAILED(hr) ? "NG" : "OK");	
	
	return hr;
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
	
	SW_TRACE_NORMAL("Info: Testing LM75...\n");
	
	INT iTemperature = 0;
	if (0 != drv_device_get_temperature(DEVID_IIC_LM75, &iTemperature))
	{
		SW_TRACE_NORMAL("Info: LM75 Test -- NG!\n");
		return E_FAIL;
	}
	
	SW_TRACE_NORMAL("Info: The temperature is %03d C\n", iTemperature);	
	
	SW_TRACE_NORMAL("Info: LM75 Test -- OK!\n");

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

	swpa_thread_sleep_ms(200);
	
	//SW_TRACE_NORMAL("info: dwLen = %u\n", dwLen);
	if (0 != drv_sec_std_write(DEVID_IIC_AT88SC, uAddr, &bData, &dwLen)
		|| sizeof(bData) != dwLen)
	{
		//SW_TRACE_NORMAL("info: dwLen = %u\n", dwLen);
		SW_TRACE_NORMAL("Err: failed to write IIC AT88SC @ %#X\n", uAddr);
		return E_FAIL;
	}
	//SW_TRACE_NORMAL("info: dwLen = %u\n", dwLen);

	swpa_thread_sleep_ms(100);
	
	bData = ~bData;
	if (0 != drv_sec_std_read(DEVID_IIC_AT88SC, uAddr, &bData, &dwLen)
		|| sizeof(bData) != dwLen
		|| bTestData != bData
		)
	{
		SW_TRACE_NORMAL("Err: failed to read IIC AT88SC @ %#x\n", uAddr);
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
	
	SW_TRACE_NORMAL("Info: Testing AT88SC...\n");
	//if (0 != drv_sec_crypt_init(DEVID_IIC_AT88SC))
	//{
	//	SW_TRACE_NORMAL("Err: failed to init IIC AT88SC\n");
	//	return E_FAIL;
	//}

	if (FAILED(TestAT88SCOp(0x00))
		||FAILED(TestAT88SCOp(0xFF))
		||FAILED(TestAT88SCOp(0xAA))
		||FAILED(TestAT88SCOp(0x55))
		)
	{
		SW_TRACE_NORMAL("Info: AT88SC Test -- NG!\n");
		return E_FAIL;
	}	
	
	SW_TRACE_NORMAL("Info: AT88SC Test -- OK!\n");

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
        SW_TRACE_NORMAL("Err: Unable to open port %s\n", szDevice);
	}

    /* block */
    ret = fcntl(fd, F_SETFL, 0);
    if (ret < 0)
	{
        //My_Txtout("fcntl\n");
        SW_TRACE_NORMAL("Err: fcntl ret %d\n", ret);
	}
    ret = isatty(STDIN_FILENO);
    if (ret == 0)
	{
        //My_Txtout("Standard input is not a terminal device.\n");
        SW_TRACE_NORMAL("Err: Standard input is not a terminal device.\n");
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
        SW_TRACE_NORMAL("Err: Unable to close the port.\n");
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
        SW_TRACE_NORMAL("Err: Unable to get UART Port attribute\n");
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
            SW_TRACE_NORMAL("ERr: Unsupported baud rate.\n");
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
        SW_TRACE_NORMAL("Err: Unsupported data bits.\n");
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
        SW_TRACE_NORMAL("Err: Unsupported parity bits.");
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
        SW_TRACE_NORMAL("Err: Unsupported stop bits.");
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
        SW_TRACE_NORMAL("Err: Unable to setup the port.");
        return E_FAIL;
	}
	
    return S_OK; /* everything is OK! */
}


	/**
	* @brief �����ڲ��ԡ�
					
	* @retval S_OK��ʾ���ڲ��Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
HRESULT CTester::TestUART()
{
	/*
	���ܴ��ڻػ����������������ж�
	*/
	
	SW_TRACE_NORMAL("Info: Testing UART...\n");

	DWORD Count_temp;
	INT fd0,fd1,ret;
		
	BYTE Tx_Buffer[35]= 
		{0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
		 0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x90,0xA0,0xB0,0xC0,0xD0,0xE0,0xF0,
		 0x00,0xFF,0xAA,0x55,'\n'};
		 
	BYTE Rx_Buffer[512];
	
	volatile BYTE *Tx_ptr,*Rx_ptr;

	
	SW_TRACE_NORMAL("Info: UART Closed Loop Testing...\n");
	
	 //open uart1
	fd1 = OpenUARTPort("/dev/ttyS1");   
	if( fd1 < 0 ) return E_FAIL;
	//setup the port
	if( SetupUARTPort(fd1, 115200, 8, 'N', 1) != S_OK) return E_FAIL;

	
	
	//---------UART1 send, UART1 received-----------------------------------
	
	//drv_device_set_rs485_direction(DEVID_RS485_1, 1);     //UART1 send dir

	swpa_thread_sleep_ms(10);//mdelay(10);
	

	ret = write(fd1, Tx_Buffer, 34);                              //UART1 send data
	if( ret < 0)
	{
		SW_TRACE_NORMAL("Err: UART1 Send Data Fail!!\n");

		CloseUARTPort(fd1);
    	return E_FAIL;
	}

	swpa_thread_sleep_ms(100);//mdelay(100); 
	
	ret = read(fd1, Rx_Buffer, 512);                               //UART1 received data 
	if( ret < 0)
	{
		SW_TRACE_NORMAL("Err: UART1 Read Data Fail!!\n");

		CloseUARTPort(fd1);
    	return E_FAIL;
	}   
    
	Tx_ptr = Tx_Buffer;
	Rx_ptr = Rx_Buffer;

    Count_temp = 34;
    
    while(Count_temp)
    {	
		if(*Tx_ptr != *Rx_ptr)
        {
    		SW_TRACE_NORMAL("Err: UART Test -- NG!\n");

			CloseUARTPort(fd1);
			return E_FAIL;
		}
		
		Count_temp--;
		Tx_ptr++;
		Rx_ptr++;
	}
	
	
	CloseUARTPort(fd1);
	
	SW_TRACE_NORMAL("Info: UART Test -- OK!\n");
	
}
	
/**
* @brief ����ָ��Ŀ¼�µĽ���ָ����С�����ݶ�д
				
* @retval S_OK��ʾ���Գɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
HRESULT CTester::CheckSDmmcReadWrite(const CHAR* szDir, const DWORD dwSize)
{
	if (NULL == szDir)
	{
		SW_TRACE_NORMAL("Err: NULL == szDir!\n");
		return E_INVALIDARG;
	}

	CHAR szFileName[256] = {0};
	swpa_strncpy(szFileName, szDir, sizeof(szFileName));
	swpa_strcat(szFileName, "testsdmmc");

	if (SWPAR_OK != swpa_utils_file_exist(szFileName))
	{
		if (SWPAR_OK != swpa_utils_file_create(szFileName))
		{
			SW_TRACE_NORMAL("Err: failed to create %s under %s\n", szFileName, szDir);
			return E_FAIL;
		}
	}

	
	INT fd = swpa_file_open(szFileName, "w");
	if (0 > fd)
	{
		SW_TRACE_NORMAL("Err: failed to open file %s\n", szFileName);
		swpa_utils_file_delete(szFileName);
		return E_FAIL;
	}

	
	PBYTE pbReadBuf = (PBYTE)swpa_mem_alloc(dwSize);
	PBYTE pbWriteBuf = (PBYTE)swpa_mem_alloc(dwSize);
	if (NULL == pbReadBuf || NULL == pbWriteBuf)
	{
		SW_TRACE_NORMAL("Err: no memory for pbReadBuf and pbWriteBuf\n");
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
		SW_TRACE_NORMAL("Err: failed to write file %s\n", szFileName);
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
		SW_TRACE_NORMAL("Err: failed to open file %s\n", szFileName);
		swpa_mem_free(pbReadBuf);
		swpa_mem_free(pbWriteBuf);
		swpa_utils_file_delete(szFileName);
		return E_FAIL;
	}

	
	if (SWPAR_OK != swpa_file_read(fd, pbReadBuf, (INT)dwSize, &iRetSize)
		|| (INT)dwSize != iRetSize)
	{
		SW_TRACE_NORMAL("Err: failed to read file %s\n", szFileName);
		swpa_file_close(fd);
		swpa_mem_free(pbReadBuf);
		swpa_mem_free(pbWriteBuf);
		swpa_utils_file_delete(szFileName);
		return E_FAIL;
	}

	if (0 != swpa_memcmp(pbReadBuf, pbWriteBuf, dwSize))
	{
		SW_TRACE_NORMAL("Err: pbReadBuf != pbWriteBuf\n");
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
	* @brief ����SD���������ݶ�д
					
	* @retval S_OK��ʾ���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
HRESULT CTester::TestSDcard()
{
	
	/*
	��SD�������豸�������ݶ�д
	��SD�����ں��Զ��������̣�����Ҫ�ڳ����й��ش��̣�Ȼ�������fwrite֮��ĺ������ж�д�ˡ�
	*/
	
	SW_TRACE_NORMAL("Info: Testing SD card...\n");
	
	const CHAR* szPartition = "/dev/mmcblk0p1";
	const CHAR* szDir = "/mnt/swtestsd/";

	INT iStatus = -1;
	if (SWPAR_OK != swpa_device_get_sdcard_status(&iStatus))
	{
		SW_TRACE_NORMAL("Err: failed to get SD card status\n");
		return E_FAIL;
	}

	if (0 != iStatus
		|| (!swpa_utils_file_exist("/dev/mmcblk0")))
	{
		SW_TRACE_NORMAL("Err: no SD card plugged in!\n");
		return E_FAIL;
	}
	else if (!swpa_utils_file_exist("/dev/mmcblk0p1"))
	{
		/*if (SWPAR_OK != swpa_hdd_fdisk("/dev/mmcblk0", 8, 0))
		{
			SW_TRACE_NORMAL("Err: failed to fdisk SD card!\n");
			return E_FAIL;
		}*/
		swpa_hdd_fdisk("/dev/mmcblk0", 8, 0);
		swpa_thread_sleep_ms(10*1000);
		
		swpa_utils_shell("mkfs.ext3 -T largefile /dev/mmcblk0p1", 0);
		SW_TRACE_NORMAL("Info: Formatting SD card...\n");
		swpa_thread_sleep_ms(5*60*1000);
	}
	
	if (SWPAR_OK != swpa_utils_dir_exist(szDir))
	{
		if (SWPAR_OK != swpa_utils_dir_create(szDir))
		{
			SW_TRACE_NORMAL("Err: failed to create %s\n", szDir);
			return E_FAIL;
		}
	}	

	if (SWPAR_OK != swpa_hdd_mount(szPartition, szDir, NULL))//if (SWPAR_OK != swpa_sdmmc_mount(szPartition, szDir, NULL))
	{

		SW_TRACE_NORMAL("Err: failed to mount %s to %s\n", szPartition, szDir);
		return E_FAIL;
	}

	LONGLONG llAvailSize = 0;
	if (SWPAR_OK != swpa_hdd_get_availsize(szPartition, &llAvailSize))//if (SWPAR_OK != swpa_sdmmc_get_availsize(szPartition, &llAvailSize))
	{
		SW_TRACE_NORMAL("Err: failed to get available size of %s \n", szPartition);
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
		SW_TRACE_NORMAL("Err: no enough space under %s \n", szPartition);
		swpa_hdd_umount(szPartition);//swpa_sdmmc_umount(szPartition);
		swpa_utils_dir_remove(szDir);
		return E_FAIL;
	}

	if (FAILED(CheckSDmmcReadWrite(szDir, dwSize)))
	{
		SW_TRACE_NORMAL("Err: SD card Test -- NG!\n");
		swpa_hdd_umount(szPartition);//swpa_sdmmc_umount(szPartition);
		swpa_utils_dir_remove(szDir);
		return E_FAIL;
	}

	if (SWPAR_OK != swpa_hdd_umount(szPartition))//if (SWPAR_OK != swpa_sdmmc_umount(szPartition))//todo: always failed!!
	{
		SW_TRACE_NORMAL("Err: failed to umount %s \n", szPartition);
		return E_FAIL;
	}

	if (SWPAR_OK != swpa_utils_dir_remove(szDir))
	{
		SW_TRACE_NORMAL("Err: failed to remove %s\n", szDir);
		return E_FAIL;
	}
	
	SW_TRACE_NORMAL("Info: SD card Test -- OK!\n");

	return S_OK;
	
}



	/**
	* @brief ��DM8127 to FPGA IO TEST
	
	* @retval S_OK��ʾIO���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
HRESULT CTester::TestDM8127FPGAIO()
{	
	int temp = 0;
	unsigned int temp32 = 0,addr = 0;
	
	SW_TRACE_NORMAL("Info: Testing DM8127 to FPGA IO ........\n");
	
	addr = FPGA_ID_ADR | ( 1 << 12)|(1 << 15);
	drv_fpga_read_single(DEVID_SPI_FPGA, addr, &temp32);

	if (FPGA_VERSION != temp32)
	{
        SW_TRACE_NORMAL("Err: Failed to Get FPGA Version,Current:0x%x !\n",temp32);    //FPGA Version
		return E_FAIL;
	}

	//----------------------------------------------------------
		drv_gpio_set_output(DEVID_GPIO3,15,0);        //SOFT_TRIG = 0;
		swpa_thread_sleep_ms(10);//mdelay(10);
		drv_gpio_get_input(DEVID_GPIO3, 22, &temp);   
		
		if(0x01 != temp)
		{
			SW_TRACE_NORMAL("Info: DM8127 to FPGA IO TEST --NG!\n");
			return E_FAIL;
			
		}
 //-------------------------------------------------------------
	
		drv_gpio_set_output(DEVID_GPIO3,15,1);        //SOFT_TRIG = 0;
		swpa_thread_sleep_ms(10);//mdelay(10);
		drv_gpio_get_input(DEVID_GPIO3, 22, &temp);   
		
		if(0x00 != temp)
		{
			SW_TRACE_NORMAL("Info: DM8127 to FPGA IO TEST --NG!\n");
			return E_FAIL;
			
		}
 //-------------------------------------------------------------
	
		SW_TRACE_NORMAL("Info: DM8127 to FPGA IO TEST --OK!\n");
		return S_OK;	
}



/**
* @brief �����������л����������

* @param [in] ctl: �л���.���ؿ��ơ�
					0�� OFF�� 1��ON ��
* @retval �ޡ�
*/
void CTester::Product_SwitcherCtl(BYTE ctl)
{

	switch(ctl)
	{
		case 0:
			drv_gpio_set_output(DEVID_GPIO2,25,0);
			drv_gpio_set_output(DEVID_GPIO2,27,1);
			swpa_thread_sleep_ms(1500);
			drv_gpio_set_output(DEVID_GPIO2,25,0);
			drv_gpio_set_output(DEVID_GPIO2,27,0);
		break;
		
		case 1:
			drv_gpio_set_output(DEVID_GPIO2,25,1);
			drv_gpio_set_output(DEVID_GPIO2,27,0);	
			swpa_thread_sleep_ms(1500);
			drv_gpio_set_output(DEVID_GPIO2,25,0);
			drv_gpio_set_output(DEVID_GPIO2,27,0);	
		break;
		
		default:
		break;	
	}	
}

/**
* @brief �����������л������Ʋ���

* @param [in] TestNum: �л������Դ�����
* @retval �ޡ�
*/
void CTester::Product_SwitcherTest(BYTE TestNum)
{

	unsigned int i =0 ;
	SW_TRACE_NORMAL("Info: Testing SwitcherCtl...\n");
	for( i = 0 ; i < TestNum ; i++)
	{
		Product_SwitcherCtl(1);   //  ON
		if (GetForceStopFlag())
		{
			return;
		}
		swpa_thread_sleep_ms(1000);//mdelay(1000);
		Product_SwitcherCtl(0);  //  OFF
		if (GetForceStopFlag())
		{
			return;
		}
		swpa_thread_sleep_ms(1000);//mdelay(1000);
	}
		
}


/**
* @brief ����������DC��Ȧ�������

* @param [in] ctl: DC��Ȧ.���ؿ��ơ�
					0�� OFF�� 1��ON ��
* @retval �ޡ�
*/
void CTester::Product_DcCtl(BYTE ctl)
{

	//unsigned int temp32 =0 ;
	switch(ctl)
	{
		case 0:
			//drv_gpio_set_output(DEVID_GPIO1,22,0);   //IR0
			//drv_gpio_set_output(DEVID_GPIO1,21,1);   //IR1
			
			drv_autoiris_pwm(DEVID_PWM, 0);
		break;
		
		case 1:
			//drv_gpio_set_output(DEVID_GPIO1,22,1);    //IR0
			//drv_gpio_set_output(DEVID_GPIO1,21,0);    //IR1
			
			drv_autoiris_pwm(DEVID_PWM, 1000);
		break;
		
		default:
		break;	
		
	}	

}


/**
* @brief ����������DC��Ȧ������Ʋ���

* @param [in] TestNum: �л������Դ�����?
* @retval �ޡ�
*/
void CTester::Product_DcCtl_TEST(BYTE TestNum)
{
	unsigned int i = 0 ;
	
	for( i = 0 ; i < TestNum ; i++)
	{
		Product_DcCtl(1);   //  ON
		if (GetForceStopFlag())
		{
			return;
		}
		swpa_thread_sleep_ms(500);//mdelay(500);
		Product_DcCtl(0);  //  OFF
		if (GetForceStopFlag())
		{
			return;
		}
		swpa_thread_sleep_ms(500);//mdelay(500);

	}

}



	/**
	* @brief �����������������ģʽ��
	
	* @retval S_OK��ʾIO���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
HRESULT CTester::Ball_IntoTestMode()
{
	
	int temp = 0;
	unsigned int temp32 = 0,addr = 0,i=0,j=0;
	INT fd0,fd1,ret;
	
	SW_TRACE_NORMAL("Info: Hight speed Ball into TestMode...\n");

	ret = swpa_camera_ball_set_wiper();    //������ˢ
	if( ret < 0)
	{
		SW_TRACE_NORMAL("Err: set wiper Fail!!\n");
        return E_FAIL;
	}
	
	
	ret = swpa_camera_ball_set_defog_fan(1); //�������ȼ�����
	if( ret < 0)
	{
		SW_TRACE_NORMAL("Err: set defog fan fail!!\n");
    	return E_FAIL;
	}
		
    //����LED����
    ret = swpa_camera_ball_set_led_mode(0x2, 0x2);  //ǿ�ƴ򿪺���LED��
    if( ret < 0)
    {
        SW_TRACE_NORMAL("Err: set infrared led Fail!!\n");
        return E_FAIL;
    }
		
	for(i =0; i < 3;i++)
	{

		ret = swpa_camera_ball_set_led_power(0,0,10);  // ��Զ����飻
		if( ret < 0)
		{
			SW_TRACE_NORMAL("Err: set infrared power Fail!!\n");
            return E_FAIL;
		}
		
		swpa_thread_sleep_ms(2000);
			
		ret = swpa_camera_ball_set_led_power(10,0,0);  // �򿪽�����飻
		if( ret < 0)
		{
			SW_TRACE_NORMAL("Err: set infrared power Fail!!\n");
            return E_FAIL;
		}
		
		swpa_thread_sleep_ms(2000);
		
	}
	
	ret = swpa_camera_ball_set_led_mode(0x3, 0x2);  //ǿ�ƹرպ���LED��
	if( ret < 0)
	{
        SW_TRACE_NORMAL("Err: set infrared led Fail!!\n");
    	return E_FAIL;
	}
		
	ret = swpa_camera_ball_set_defog_fan(0); //�رշ��ȼ�����
	if( ret < 0)
	{
		SW_TRACE_NORMAL("Err: set defog fan fail!!\n");
    	return E_FAIL;
	}

#if 0
    ret = swpa_camera_ball_restore_factory_settings();//�ָ�����Ĭ��
	if( ret < 0)
	{
		SW_TRACE_NORMAL("Err: restore_factory_settings fail!!\n");
    	return E_FAIL;
	}
    swpa_thread_sleep_ms(2000);
#endif

//    SW_TRACE_NORMAL("!!!!!!!!!!!!!!!!!!!!!!!!WANGNC: JUST DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	ret = swpa_camera_ball_factorytest_start();   //�����ϻ�����ģʽ
	if( ret < 0)
	{
		SW_TRACE_NORMAL("Err: swpa_camera_ball_factorytest_start Fail!!\n");
        return E_FAIL;
	}
	
	return S_OK;
		
}



	/**
	* @brief ������˳���������ģʽ��
	
	* @retval S_OK��ʾIO���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
HRESULT CTester::Ball_StopTestMode()
{
	
	int temp = 0,ret;
	unsigned int temp32 = 0,addr = 0;
	
	

	
	SW_TRACE_NORMAL("Info: Hight speed Ball Stop TestMode...\n");
	
	
	ret = swpa_camera_ball_factorytest_stop( );   //�˳��ϻ�ģʽ
	
	if( ret < 0)
	{
		SW_TRACE_NORMAL("Err: swpa_camera_ball_factorytest_stop Fail!!\n");

    	return E_FAIL;
	}
	
	
	

	return S_OK;
		
}




