/*
 * SWCOMTestDevice.h
 *
 *  Created on: 2013��12��4��
 *      Author: qinjj
 */

#ifndef __SW_COMTESTDEVICE_H__
#define __SW_COMTESTDEVICE_H__
#include "SWBaseDevice.h"
/*
 *@brief ���ڲ���ģ��
**/
class CSWCOMTestDevice : public CSWBaseDevice
{
	CLASSINFO(CSWCOMTestDevice, CSWBaseDevice)
public:
	CSWCOMTestDevice(DEVICEPARAM *pParam);
	virtual ~CSWCOMTestDevice();
	virtual HRESULT Run(VOID);
};

#endif /* __SW_COMTESTDEVICE_H__ */
