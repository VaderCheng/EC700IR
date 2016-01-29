/*
 * SWLPRVenusEPoliceParameter.h
 *
 *  Created on: 2013��12��27��
 *      Author: zhaopy
 */

#ifndef __SW_LPR_VENUS_EPOLICE_PARAMETER_H__
#define __SW_LPR_VENUS_EPOLICE_PARAMETER_H__
#include "SWLPRParameter.h"

class CSWLPRVenusEPoliceParameter  : public CSWLPRParameter
{
	CLASSINFO(CSWLPRVenusEPoliceParameter, CSWLPRParameter)
public:
	CSWLPRVenusEPoliceParameter();
	virtual ~CSWLPRVenusEPoliceParameter();

protected:
    virtual void ResetParam(VOID);

	virtual HRESULT Initialize(CSWString strFilePath);

	virtual HRESULT InitSystem(VOID);
	/**
	 *@brief
	 */
	virtual HRESULT InitTracker(VOID);

	/**
	 *@brief
	 */
	virtual HRESULT InitHvDsp(VOID);

	/**
	 *
	 */
	virtual HRESULT InitIPTCtrl(VOID);
	
	virtual HRESULT InitOuterCtrl(VOID);

	virtual HRESULT InitScaleSpeed(VOID);
	/**
	*
	*/
	virtual HRESULT InitCamApp(VOID);

	virtual HRESULT InitCamera(VOID);

	VOID LoadTrafficLightParam(VOID);
	/**
	*
	*/
	virtual HRESULT InitCharacter(VOID);

	/**
    *
    */
    virtual HRESULT InitGB28181(VOID);


    // ���μ�����
    void DeleteTrackerParam(VOID);
    // �ָ�Ĭ��

	
	/**
    *
    */	
	virtual HRESULT InitAutoReboot(VOID);

	/*
	 *@biref Init ONVIF Param
	 *@return On success, S_OK is returned 
	 */
	virtual HRESULT InitONVIF(VOID); 
};
REGISTER_CLASS(CSWLPRVenusEPoliceParameter)
#endif /* __SW_LPR_VENUS_EPOLICE_PARAMETER_H__ */
