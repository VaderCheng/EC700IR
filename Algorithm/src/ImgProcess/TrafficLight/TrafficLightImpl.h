#pragma once
#include ".\trafficlightinfo.h"
#include "tracker.h"

#define MAX_ERROR_COUNT 500
#define MAX_LIGHT_TYPE_COUNT 32
#define STATUS_BUF_LEN 100
#define MAX_IO_LIGHT_COUNT 8

class CTrafficLightImpl
{
public:
	CTrafficLightImpl(void);
	~CTrafficLightImpl(void);
public:
	//���ú��̵Ʋ���
	HRESULT SetTrafficLightParam(TRAFFIC_LIGHT_PARAM tlpTrafficLightParam);
	//���ýӿڶ����ص�����
	HRESULT SetTrafficLightCallback(ITracker *pCallback);
	//pSceneImageԭʼͼƬ,iCurrentStatus��ǰʶ����ĺ��̵Ƶ�״̬,iLastStatus����ȶ�ʶ���״̬,fNight��ǰ�����ϻ��ǰ���
	HRESULT RecognizeTrafficLight(HV_COMPONENT_IMAGE* pSceneImage, int* iCurrentStatus, int* iLastStatus, bool fNight);
	//ȡ�ö�Ӧ�����ĳ���,iPos��������ֵ
	bool GetLightScene(int iPos, TRAFFICLIGHT_SCENE* pts);
	//��ȡ��ƿ�ʼʱ��
	bool GetRedLightStartTime(LIGHT_REALTIME* plrRedLightTime);
	//��ȡ�̵�ʱ��
	bool GetGreenLightStartTime(LIGHT_TICK* pGreenTick);
	//��ȡָ������ĺ��̵���Ϣ��,iIndex���̵Ƶ�������ֵ
	CTrafficLightInfo* GetTrafficLightInfo(int iIndex);
private:
	int InitTrafficLightParam();
	HRESULT PrintLightStatus(const LIGHT_TEAM_STATUS& ltsInfo , char* pLightStatus,int iLightStatusLen);
	//ͨ��IOȡ��ǰ���̵�״̬
	DWORD32 GetIOStatus(int iTeam, BYTE8 bLevel);
	//����������״̬�����س���״̬���
	int TransitionScene(const LIGHT_TEAM_STATUS& ltsInfo, const int& iSceneCount, const int& iLastOkLightStatus, bool fFlag);
	//����ǵ���״̬�볡���ǰ��
	int GetSceneNumber(const LIGHT_TEAM_STATUS& ltsInfo, const int& iSceneCount);
	bool GetSceneInfo(int iPos, SCENE_INFO* pts);
	void SetSceneInfo(int iPos, const SCENE_INFO& ts);
	void SetTrafficLightType(int iTeam, int iPos, _TRAFFICLIGHT_POSITION tpPos, _TRAFFICLIGHT_LIGHT_STATUS tlsStatush);
	bool GetTrafficLightType(int iTeam, int iPos, TRAFFICLIGHT_TYPE* ltLightType);	
	void SetSceneStatus(int nPos, int nStatus);
	int GetSceneStatus(int nPos);
	
	HRESULT UpdateRedLightPos();
private:
	ITracker *m_pCallback;
	TRAFFIC_LIGHT_PARAM m_tlpTrafficLightParam;
	
	CTrafficLightInfo m_rgLightInfo[MAX_TRAFFICLIGHT_COUNT];
	
	//���ڻص����
	HV_RECT m_rgLightRect[MAX_TRAFFICLIGHT_COUNT];	//����λ��
	DWORD32 m_rgLightStatus[MAX_TRAFFICLIGHT_COUNT]; //����״̬
	TRAFFICLIGHT_SCENE m_rgLightScene[MAX_SCENE_COUNT];
	SCENE_INFO m_rgsiInfo[MAX_SCENE_COUNT];
	int m_iLightTypeCount;
	TRAFFICLIGHT_TYPE m_rgLightType[MAX_LIGHT_TYPE_COUNT];
	
	//���IO���̵���Ϣ
	TRAFFICLIGHT_TYPE m_rgIOLight[MAX_IO_LIGHT_COUNT];
	//��ǰ����
	int m_iLastLightStatus;
	//ʵʱ���̵�״̬
	int m_iCurLightStatus;
	//��һ����Ч�ĺ��̵�״̬
	int m_iLastOkLightStatus;
	//����ת������
	bool m_fCheckSpeed;
	//������Ч������
	int m_iErrorSceneCount;
	//ÿ��������̵ƿ�ʼʱ��(��ת,ֱ��,��ת,��ͷ��
	LIGHT_REALTIME m_redrealtime;
	//�̵ƿ�ʼ���ʱ��
	LIGHT_TICK m_greentick;
	//����ӳ�ʱ��
	LIGHT_TICK m_ltRedRelay;
	int m_iRedLightCount;
	HiVideo::CRect m_rgRedLightRect[MAX_TRAFFICLIGHT_COUNT * 2];	//��Ƶ���λ��
	int m_rgiSceneStatus[STATUS_BUF_LEN];
};
