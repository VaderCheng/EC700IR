#ifndef CSWEXTDEVCONTROL_H
#define CSWEXTDEVCONTROL_H
#include "SWCameraControlParameter.h"

class CSWExtDevControl
{
public:
    CSWExtDevControl();
    virtual ~CSWExtDevControl();
    // ��ʼ���ⲿ�豸����
    void InitialExtDevParam( LPCAMERAPARAM_INFO pCameraParam );
    // �л��˹�Ƭ
    HRESULT FilterSwitch( DWORD dwFilterType );
	// ���ú�ƼӺ�����
	HRESULT SetRedLightRect(SW_RECT* pRect, INT iCount);
	// 设置电网同步模式
	HRESULT SetExternSync(INT iMode, INT iRelayUs);
	// ���ô���ץ����
	HRESULT SetCaptureEdge(int nValue);
	// ��ȡ����ץ����
	HRESULT GetCaptureEdge(int *pnValue);

	HRESULT GetBarrierState(int *pnValue);
protected:
private:
	// Ӳ���ֳ�����
	int m_iFlashDifferentLaneExt;
};

#endif // CSWEXTDEVCONTROL_H
