#ifndef __CSR_I_RADAR_H__
#define __CSR_I_RADAR_H__
#include "RadarBase.h"
/**
˵��:�����״�Э�飬���״���Ҫ������λ�����������ٶȵ������ʽΪ���ֽڵķ�ʽ
     ���ٶ�����ٶ�ֵ���������0���߲����������ֻ����Ӵ��ڽ��յ��ٶ�ֵ
*/
class CCSRIRadar : public CRadarBase
{
public:
	CCSRIRadar();
	virtual ~CCSRIRadar();
	virtual bool Initialize(void);
protected:
	virtual const char* GetName()
	{
		return "CCSRIRadar";
	}
	virtual HRESULT Run(void* pvParam);
};
#endif
