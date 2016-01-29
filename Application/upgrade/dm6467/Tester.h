
#ifndef _TESTER_IN_AUTOTEST_H_
#define _TESTER_IN_AUTOTEST_H_

#include "drv_device.h"

class CTester
{
public:
	
	CTester();

	virtual ~CTester();
	
	/*
	static HRESULT TestDM6467DDR();
	*/

	
	static HRESULT TestFlash(const DRV_DEVICE_TYPE dwFlashID, const DWORD dwAddr, const DWORD dwSectorCount, const DWORD dwSectorSize);
	
	/**
	* @brief �DSPI FLASH�Ķ�д���ԣ���DDR��һ������д��SPI FLASH�У�����
			  ����DDR����һ�ռ䣬���бȽϡ�ÿ�������������ԡ�
			
	* @retval S_OK��ʾSPI FLASH���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestDM6467Flash();
	

	/**
	* @brief ��I2C EEPROM��д���ԣ���дEEPROM��ÿһҳ��һ������8ҳ

	* @retval S_OK��ʾSPI FLASH��ID���ɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestDM6467EEPROM();
	

	/**
	* @brief ��RTC���ԣ���ʱ��仯��ȡRTCʱ��ֵ
	
	* @retval S_OK:  �ɹ����в���	
	* @retval E_FAIL: ����ʧ��
	*/
	static HRESULT TestDM6467RTC();

	
	/**
	* @brief ��LM75���ԣ���ȡ��ǰ�¶�
	
	* @retval S_OK:  �ɹ����в���	
	* @retval E_FAIL: ����ʧ��
	*/
	static HRESULT TestLM75();
	

	
	/**
	* @brief ��AT88SC�Ǽ��ܶ�д����
	
	* @retval S_OK:  �ɹ����в���	
	* @retval E_FAIL: ����ʧ��
	*/
	static HRESULT TestAT88SC();
	

	/**
	* @brief ��DM6467�˵�VPIF����

	* @retval S_OK��ʾVPIF���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestVPIF();
	
	
	/**
	* @brief ����USB�洢�豸����U�̡�USB to SataӲ�̽������ݶ�д
					
	* @retval S_OK��ʾ���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestUSB();
	

	/**
	* @brief ������0������1����ػ����ԡ�
					
	* @retval S_OK��ʾ���ڻػ����Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestUART();
	

	
	/**
	* @brief �DSPI FLASH�Ķ�д���ԣ���DDR��һ������д��SPI FLASH�У�����
			  ����DDR����һ�ռ䣬���бȽϡ�ÿ�������������ԡ�
			
	* @retval S_OK��ʾSPI FLASH���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestDM368Flash();
	

	/**
	* @brief ��DM6467T to FPGA IO TEST
	
	* @retval S_OK��ʾIO���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestDM6467FPGAIO();
	

	/**
	* @brief ��DM6467T to DM368 IO TEST
	
	* @retval S_OK��ʾIO���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestDM6467DM368IO();
	

	/**
	* @brief ��DM368 to FPGA IO TEST,DM6467 ͨ��USB���������֪DM368���в��ԣ������ؽ��
	
	* @retval S_OK��ʾ���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestDM368FPGAIO();
	
	/**
	* @brief �����������л����������
	@param [in] ctl: �л���.���ؿ��ơ�
						0�� OFF�� 1��ON ��
	* @retval �ޡ�

	*/
   static void ProductSwitcherCtl(BYTE ctl);


	/**
	* @brief �����������л������Ʋ���
	@param [in] TestNum: �л������Դ�����

	* @retval �ޡ�
	*/
	static void ProductSwitcherTest(BYTE TestNum);


	/**
	* @brief ����������DC��Ȧ�������
	@param [in] ctl: DC��Ȧ.���ؿ��ơ�
						0�� OFF�� 1��ON ��
	* @retval �ޡ�

	*/
	static void ProductDcCtl(BYTE ctl);

	/**
	* @brief ����������DC��Ȧ������Ʋ���
	@param [in] TestNum: �л������Դ�����

	* @retval �ޡ�

	*/
	static void ProductDcCtlTEST(BYTE TestNum);

	/**
	* @brief ��Ӳ�̵�Դ����
	@param [in] ctl: Ӳ�̵�Դ���ؿ��ơ�
						0�� OFF�� 1��ON ��
	* @retval �ޡ�

	*/
	static void HdPowerCtl(BYTE ctl);

	
	static HRESULT DeinitVPIF();

	static HRESULT InitVPIF();

	static HRESULT SetForceStopFlag(const BOOL fFlag);

	static HRESULT GetForceStopFlag();


private:
	
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
	static HRESULT SetupUARTPort(INT iFD, INT iSpeed, INT iDataBits, INT iParity, INT iStopBits);

	/**
	* @brief �رմ���

	* @param [in] fd: The file description returned by open_port().
	* @retval S_OK : ��ʾ�رճɹ�.
	* @retval E_FAIL : ��ʾӲ���ײ��������.
	*/
	static HRESULT CloseUARTPort(INT iFD);

	/**
	* @brief : ��һ�����ڡ�

	* @param [in] szDevice : The serial port device name, eg, /dev/ttyS0��/dev/ttyUSB0

	* @retval Return fd if success, otherwise will return -1 with some msg.
	*/
	static INT OpenUARTPort(const CHAR* szDevice);

	/**
	* @brief DM6467T to FPGA IO CHECK

	* @param [in] Testdata: ����Ĳ�������,ֻ�е�3λ��Ч��
	* @retval S_OK : ��ʾIO���Գɹ�.
	* @retval E_FAIL : ��ʾӲ���ײ��������.
	*/
	static HRESULT CheckIODM6467ToFPGA(BYTE bTestdata);

	/**
	* @brief DM6467T to DM368 IO CHECK

	* @param [in] Testdata: ����Ĳ�������,ֻ�е�2λ��Ч��
	* @retval S_OK : ��ʾIO���Գɹ�.
	* @retval E_FAIL : ��ʾӲ���ײ��������.
	*/
	static HRESULT CheckIODM6467ToDM368(BYTE bTestdata);

	 
	/**
	* @brief ����ָ��Ŀ¼�µĽ���ָ����С�����ݶ�д
					
	* @retval S_OK��ʾ���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT CheckUSBReadWrite(const CHAR* szDir, const DWORD dwSize);


	/**
	* @brief ��AT88SC�������Ļ������������������I2CͨѶ

	* @param [in] : TestByte : ����Ĳ�������
	* @retval S_OK:  �ɹ����в���	
	* @retval E_FAIL: ����ʧ��
	*/
	static HRESULT TestAT88SCOp(const BYTE bTestData);

	
	static HRESULT SetDM368ToIOTestMode();




private:

};


#endif //_TESTER_IN_AUTOTEST_H_

