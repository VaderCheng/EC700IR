
#ifndef _TESTER_IN_AUTOTEST_H_
#define _TESTER_IN_AUTOTEST_H_

#include "SWBaseType.h"
#include "drv_device.h"

class CTester
{
public:
	
	CTester();

	virtual ~CTester();

	static HRESULT SetForceStopFlag(const BOOL fFlag);

	static BOOL GetForceStopFlag();
		
	/**
	* @brief �DNAND FLASH�Ķ�д���ԣ���DDR��һ������д��NAND FLASH�У�����
			  ����DDR����һ�ռ䣬���бȽϡ�ÿ��block�������ԡ�
			
	* @retval S_OK��ʾSPI FLASH���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestFlash();
	

	/**
	* @brief ��I2C EEPROM��д���ԣ���дEEPROM��ÿһҳ��һ������8ҳ

	* @retval S_OK��ʾSPI FLASH��ID���ɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestEEPROM();
	

	/**
	* @brief ��RTC���ԣ���ʱ��仯��ȡRTCʱ��ֵ
	
	* @retval S_OK:  �ɹ����в���	
	* @retval E_FAIL: ����ʧ��
	*/
	static HRESULT TestRTC();

	
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
	* @brief �����ڲ��ԡ�
					
	* @retval S_OK��ʾ���ڲ��Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestUART();




	/**
	* @brief ����SD���������ݶ�д
					
	* @retval S_OK��ʾ���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestSDcard();



	/**
	* @brief ��DM8127 to FPGA IO TEST
	
	* @retval S_OK��ʾIO���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT TestDM8127FPGAIO();





	/**
	* @brief �����������л������Ʋ���

	* @param [in] TestNum: �л������Դ�����
	* @retval �ޡ�
	*/
	static void Product_SwitcherTest(BYTE TestNum);



	/**
	* @brief ����������DC��Ȧ������Ʋ���

	* @param [in] TestNum: �л������Դ�����
	* @retval �ޡ�
	*/
	static void Product_DcCtl_TEST(BYTE TestNum);


	/**
	* @brief �����������������ģʽ��
	
	* @retval S_OK��ʾIO���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT Ball_IntoTestMode();


	/**
	* @brief ������˳���������ģʽ��
	
	* @retval S_OK��ʾIO���Գɹ�.
	* @retval E_FAIL��ʾӲ���ײ��������.
	*/
	static HRESULT Ball_StopTestMode();



private:
	
	
	/**
* @brief ����ָ��Ŀ¼�µĽ���ָ����С�����ݶ�д
				
* @retval S_OK��ʾ���Գɹ�.
* @retval E_FAIL��ʾӲ���ײ��������.
*/
	static HRESULT CheckSDmmcReadWrite(const CHAR* szDir, const DWORD dwSize);
	
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
	* @brief ��AT88SC�������Ļ������������������I2CͨѶ

	* @param [in] : TestByte : ����Ĳ�������
	* @retval S_OK:  �ɹ����в���	
	* @retval E_FAIL: ����ʧ��
	*/
	static HRESULT TestAT88SCOp(const BYTE bTestData);

	/**
	* @brief �����������л����������

	* @param [in] ctl: �л���.���ؿ��ơ�
	*				0�� OFF�� 1��ON ��
	* @retval �ޡ�
	*/
	static void Product_SwitcherCtl(BYTE ctl);

	/**
	* @brief ����������DC��Ȧ�������

	* @param [in] ctl: DC��Ȧ.���ؿ��ơ�
	*				0�� OFF�� 1��ON ��
	* @retval �ޡ�
	*/
	static void Product_DcCtl(BYTE ctl);
};


#endif //_TESTER_IN_AUTOTEST_H_

