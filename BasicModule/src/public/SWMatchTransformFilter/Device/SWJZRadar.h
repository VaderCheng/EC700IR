/*
 * SWJZRadar.h
 *
 *  Created on: 2013��12��4��
 *      Author: qinjj
 */

#ifndef __SW_JZRADAR_H__
#define __SW_JZRADAR_H__
#include "SWBaseDevice.h"
/*
 *@brief �Ĵ������״����
**/
class CSWJZRadar : public CSWBaseDevice
{
	CLASSINFO(CSWJZRadar, CSWBaseDevice)
public:
	CSWJZRadar(DEVICEPARAM *pParam);
	virtual ~CSWJZRadar();
	virtual HRESULT Run(VOID);
//private:
	//HRESULT JZRadarInit(VOID);
};

#endif /* __SW_JZRADAR_H__ */
