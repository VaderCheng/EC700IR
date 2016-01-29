#pragma once

#ifndef _SCALESPEEDIMPL_INCLUDED__
#define _SCALESPEEDIMPL_INCLUDED__

#include "ScaleSpeed.h"

class CScaleSpeedImpl : public IScaleSpeed
{
public:
	CScaleSpeedImpl();
	virtual ~CScaleSpeedImpl();

	STDMETHOD(InitScaleSpeed)(
		SCALESPEED_CFG_PARAM *pScaleSpeedParam
		);

	STDMETHOD(SetHvParam)(
		HvCore::IHvParam2* pHvParam
		);

	STDMETHOD(SetFrameProperty)(
		const FRAME_PROPERTY& cFrameProperty
		);

	virtual float STDMETHODCALLTYPE CalcCarSpeed(		// ���س����ٶ�
		CRect rcBegin,				// ��ʼλ������
		CRect rcEnd,				// ����λ������
		DWORD32 dwDruTime,			// ʱ��(����)
		bool fAdjustDistance,	// �Ƿ�ͨ�����ƿ�Ƚ����������
		PLATE_TYPE PlateType,		//��������
		float &fltScale			// ���ز���������ʵ�ʾ���ı���
	);

	// �·���
	virtual float STDMETHODCALLTYPE CalcCarSpeedNewMethod(	// ���س����ٶ�
		CRect rcBegin,				// ��ʼλ������
		CRect rcEnd,				// ����λ������
		DWORD32 dwDruTime,			// ʱ��(����)
		bool fAdjustDistance,	// �Ƿ�ͨ�����ƿ�Ƚ����������
		PLATE_TYPE PlateType,		//��������
		float &fltScale			// ���ز���������ʵ�ʾ���ı���
		);

	//������Ļ����
	virtual float STDMETHODCALLTYPE CalcActureDistance(HV_POINT point1, HV_POINT point2);
	HRESULT SetVideoID(int iVideoID);
	bool GetActurePoint(HV_POINT ptOrg, HV_POINT& ptDest, float fltDistance);
private:
	HvCore::IHvParam2* m_pHvParam;

	BOOL m_fInitialized;
	float *m_prgfltPixelsDisTable;			// ��Ļ���ؾ�����ձ�


	// ����ʹ�õĲ���
	float m_fltCameraHigh;			// ������߶�	= 7.50f;
	float m_fltDistanceAll;			// �ܾ��� = 75.00f;
	float m_fltDistanceLeft;		// �����������Χ�ڵľ��� = 25.00f;
	float m_fltAdjustCoef;			// �ٶȵ���ϵ��
	float m_fltTransMatrix[11];		// ����ͼ�����굽�ռ�����ı任����

	// ���Է���ѡ���־
	int m_nFlagMethods;			// ѡ���������任�ķ�ʽ��Ĭ��Ϊ0��0:�ɷ�����1:�·�����������任�������

	// ����
	void Clear();				// �ͷ�����Ŀռ�

	// ʹ�õ�һЩ�м����
	float m_fltX0;
	float m_fltY0;

	//zhaopy
	DWORD32 m_dwHeight;
	// �м�Ƕ�
	float m_anBeta;					// �Ƕ� atan(m_fltDistanceLeft / m_fltCameraHigh)
	float m_anACF;

	// �м����
	float m_disCG;
	float m_disAC;

	void CalcActurePoint(HV_POINT poScreen, float &fltCD, float &fltW);		// ������Ļ�������ʵ������
	int m_iVideoID;
};

#endif // _SCALESPEEDIMPL_INCLUDED__
