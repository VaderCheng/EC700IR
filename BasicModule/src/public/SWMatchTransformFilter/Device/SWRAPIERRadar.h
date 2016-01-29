/*
 * SWRAPIERRadar.h
 *
 *  Created on: 2013��12��4��
 *      Author: qinjj
 */

#ifndef __SW_RAPIERRADAR_H__
#define __SW_RAPIERRADAR_H__

#include "SWBaseDevice.h"

/*
 *@brief ����ά���״����
**/

class CSWRAPIERRadar : public CSWBaseDevice
{
	CLASSINFO(CSWRAPIERRadar, CSWBaseDevice)
public:
	CSWRAPIERRadar(DEVICEPARAM *pParam);
	virtual ~CSWRAPIERRadar();
	virtual HRESULT Run(VOID);

//private:
	//HRESULT RAPIERRadarInit(VOID);
};

#endif /* __SW_RAPIERRADAR_H__ */
