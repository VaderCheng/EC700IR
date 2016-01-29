#ifndef __SW_CAMERA_DATA_PDU_H__
#define __SW_CAMERA_DATA_PDU_H__
class CSWCameraDataPDU : public CSWObject
{
	CLASSINFO(CSWCameraDataPDU, CSWObject)
public:
	CSWCameraDataPDU();
	virtual ~CSWCameraDataPDU();
	void SetLightType(DWORD dwLightType){m_dwLightType = dwLightType;}
	DWORD GetLightType(void){return m_dwLightType;}
	
	void SetCplStatus(DWORD dwCplStatus){m_dwCplStatus = dwCplStatus;}
	DWORD GetCplStatus(void){return m_dwCplStatus;}
	
	void SetPluseLevel(DWORD dwLevel){m_dwPluseLevel = dwLevel;}
	DWORD GetPluseLevel(void){return m_dwPluseLevel;}

	void SetAvgPlateY(int nPlateY) {m_nAvgPlateY = nPlateY;}
	int GetAvgPlateY() {return m_nAvgPlateY;}

	void SetWDRLevel(int nWDRLevel) {m_nWDRLevel = nWDRLevel;}
	int GetWDRLevel() {return m_nWDRLevel;}
private:
	DWORD m_dwLightType;	//����ȼ�
	DWORD m_dwCplStatus;	//ƫ�⾵״̬
	DWORD m_dwPluseLevel;	//Ƶ����������ȼ�
	int m_nAvgPlateY;    //ƽ����������
	int m_nWDRLevel;    //WDR�ȼ�
};
#endif
