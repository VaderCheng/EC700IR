#include "platerecogparam.h"

PlateRecogParam g_rgPlateRecogParam[PRM_MaxMode]=
{
	{
		// �շ�վ
		PRM_TOLLGATE,
		PR_SPEED_NORMAL,
		MD_TOP2BOTTOM,			// g_MovingDirection

		0,						// g_cnDetStep, ���岻ʹ��
		{0, 30, 95, 90},		// g_rectDetectionArea
		{0, 0, 100, 90},		// g_rectTrackArea

		0.5f,					// g_fltTrackInflateX
		0.5f,					// g_fltTrackInflateY
		3,						// g_nContFrames_EstablishTrack;
		15,						// g_nMissFrames_EndTrack;
		10,						// g_nMissFrames_EndTrackQuick;
		0,						// g_nObservedFrames_ConsiderAsTrack, ���岻ʹ��;
		0.15f,					// g_fltThreshold_StartTrack
		0.12f,					// g_fltThreshold_BeObservation
		0.0f,					// g_fltMinConfidenceForVote, ���岻ʹ�ã�

		////for plate detection
		6,						// g_nDetMinScaleNum
		14,						// g_nDetMaxScaleNum
		30,						// g_nDetMinStdVar
		6,						// g_nMinStopPlateDetNum
		false,					// g_fSubscanPredict
		0,						// g_nSubscanPredictMinScale
		0,						// g_nSubscanPredictMaxScale

		0.075f,					// g_kfltPlateInflateRate
		0.2f,					// g_kfltPlateInflateRateV
		0.2f,					// g_kfltHighInflateArea
		0.15f,					// g_kfltPlateInflateRate2
		0.3f,					// g_kfltPlateInflateRateV2

		false,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		200,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		false,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		    //���Ӿ��쿪��
		0,						//m_fUseTemplet;				// ģ��ƥ�俪��

		0,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		0,						//m_iDetectReverseEnable;		//�������м�⿪��
		0,						//m_iDetectOverYellowLineEnable;//����ѹ���߼�⿪��
		0,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		// ����·���� - ��ͷ
		PRM_HIGHWAY_HEAD,
		PR_SPEED_FAST,
		MD_TOP2BOTTOM,			// g_MovingDirection

		0,						// g_cnDetStep, ���岻ʹ��
		{0, 15, 100, 55},		// g_rectDetectionArea
		{0, 10, 100, 90},		// g_rectTrackArea

		0.5f,					// g_fltTrackInflateX
		1.6f,					// g_fltTrackInflateY
		2,						// g_nContFrames_EstablishTrack;
		3,						// g_nMissFrames_EndTrack;
		0,						// g_nMissFrames_EndTrackQuick;
		0,						// g_nObservedFrames_ConsiderAsTrack, ���岻ʹ��;
		0.08f,					// g_fltThreshold_StartTrack
		0.08f,					// g_fltThreshold_BeObservation
		0.0f,					// g_fltMinConfidenceForVote, ���岻ʹ�ã�

		////for plate detection
		5,						// g_nDetMinScaleNum, 85~170
		9,						// g_nDetMaxScaleNum
		30,						// g_nDetMinStdVar
		0,						// g_nMinStopPlateDetNum
		true,					// g_fSubscanPredict
		-1,						// g_nSubscanPredictMinScale
		2,						// g_nSubscanPredictMaxScale

		0.05f,					// g_kfltPlateInflateRate
		0.10f,					// g_kfltPlateInflateRateV
		0.0f,					// g_kfltHighInflateArea
		0.0f,					// g_kfltPlateInflateRate2
		0.0f,					// g_kfltPlateInflateRateV2

		true,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		200,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		false,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		0,						//m_fUseTemplet;			// ģ��ƥ�俪��

		0,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		1,						//m_iDetectReverseEnable;		//�������м�⿪��
		1,						//m_iDetectOverYellowLineEnable;//����ѹ���߼�⿪��
		1,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		// ����·���� - ��β
		PRM_HIGHWAY_TAIL,
		PR_SPEED_FAST,
		MD_BOTTOM2TOP,			//g_MovingDirection

		0,						// g_cnDetStep, ���岻ʹ��
		{0, 40, 100, 90},		// g_rectDetectionArea
		{0, 20, 100, 100},		// g_rectTrackArea

		0.5f,					// g_fltTrackInflateX
		1.6f,					// g_fltTrackInflateY
		2,						// g_nContFrames_EstablishTrack;
		6,						// g_nMissFrames_EndTrack;
		0,						// g_nMissFrames_EndTrackQuick;
		0,						// g_nObservedFrames_ConsiderAsTrack, ���岻ʹ��;
		0.08f,					// g_fltThreshold_StartTrack
		0.08f,					// g_fltThreshold_BeObservation
		0.0f,					// g_fltMinConfidenceForVote, ���岻ʹ�ã�

		////for plate detection
		7,						// g_nDetMinScaleNum
		13,						// g_nDetMaxScaleNum
		16,						// g_nDetMinStdVar
		0,						// g_nMinStopPlateDetNum
		true,					// g_fSubscanPredict
		-2,						// g_nSubscanPredictMinScale
		1,						// g_nSubscanPredictMaxScale

		0.05f,					// g_kfltPlateInflateRate
		0.10f,					// g_kfltPlateInflateRateV
		0.0f,					// g_kfltHighInflateArea
		0.0f,					// g_kfltPlateInflateRate2
		0.0f,					// g_kfltPlateInflateRateV2

		true,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		200,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		false,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		0,						//m_fUseTemplet;			// ģ��ƥ�俪��

		0,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		1,						//m_iDetectReverseEnable;		//�������м�⿪��
		1,						//m_iDetectOverYellowLineEnable;//����ѹ���߼�⿪��
		1,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		// �ƶ����� - ���
		PRM_PATROL_LEFT,
		PR_SPEED_FAST,
		MD_LEFT2RIGHT,			// g_MovingDirection

		0,						// g_cnDetStep, ���岻ʹ��
		{0, 0, 50, 100},		// g_rectDetectionArea	//�ڵ��Ƚ϶�
		{0, 0, 80, 100},		// g_rectTrackArea

		0.75f,					// g_fltTrackInflateX
		0.5f,					// g_fltTrackInflateY
		2,						// g_nContFrames_EstablishTrack;
		3,						// g_nMissFrames_EndTrack;
		0,						// g_nMissFrames_EndTrackQuick;
		0,						// g_nObservedFrames_ConsiderAsTrack, ���岻ʹ��;
		0.08f,					// g_fltThreshold_StartTrack
		0.08f,					// g_fltThreshold_BeObservation
		0.0f,					// g_fltMinConfidenceForVote, ���岻ʹ�ã�

		////for plate detection
		5,						// g_nDetMinScaleNum
		12,						// g_nDetMaxScaleNum
		60,						// g_nDetMinStdVar
		0,						// g_nMinStopPlateDetNum
		true,					// g_fSubscanPredict
		-3,						// g_nSubscanPredictMinScale
		1,						// g_nSubscanPredictMaxScale

		0.05f,					// g_kfltPlateInflateRate
		0.15f,					// g_kfltPlateInflateRateV
		0.0f,					// g_kfltHighInflateArea
		0.0f,					// g_kfltPlateInflateRate2
		0.0f,					// g_kfltPlateInflateRateV2

		////for plate segmentation
		true,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		200,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		false,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		0,						//m_fUseTemplet;			// ģ��ƥ�俪��

		0,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		0,						//m_iDetectReverseEnable;		//�������м�⿪��
		0,						//m_iDetectOverYellowLineEnable;//����ѹ���߼�⿪��
		0,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		// �ƶ����� - �Ҳ�
		PRM_PATROL_RIGHT,
		PR_SPEED_FAST,
		MD_RIGHT2LEFT,			// g_MovingDirection

		0,						// g_cnDetStep, ���岻ʹ��
		{50, 0, 100, 100},		// g_rectDetectionArea	//�ڵ��Ƚ϶�
		{20, 0, 100, 100},		// g_rectTrackArea

		0.75f,					// g_fltTrackInflateX
		0.5f,					// g_fltTrackInflateY
		2,						// g_nContFrames_EstablishTrack;
		3,						// g_nMissFrames_EndTrack;
		0,						// g_nMissFrames_EndTrackQuick;
		0,						// g_nObservedFrames_ConsiderAsTrack, ���岻ʹ��;
		0.08f,					// g_fltThreshold_StartTrack
		0.08f,					// g_fltThreshold_BeObservation
		0.0f,					// g_fltMinConfidenceForVote, ���岻ʹ�ã�

		////for plate detection
		5,						// g_nDetMinScaleNum
		12,						// g_nDetMaxScaleNum
		60,						// g_nDetMinStdVar
		0,						// g_nMinStopPlateDetNum
		true,					// g_fSubscanPredict
		-3,						// g_nSubscanPredictMinScale
		1,						// g_nSubscanPredictMaxScale

		0.05f,					// g_kfltPlateInflateRate
		0.15f,					// g_kfltPlateInflateRateV
		0.0f,					// g_kfltHighInflateArea
		0.0f,					// g_kfltPlateInflateRate2
		0.0f,					// g_kfltPlateInflateRateV2

		true,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		200,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		false,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		0,						//m_fUseTemplet;			// ģ��ƥ�俪��

		0,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		0,						//m_iDetectReverseEnable;		//�������м�⿪��
		0,						//m_iDetectOverYellowLineEnable;//����ѹ���߼�⿪��
		0,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		//������
		PRM_PORTABLE,
		PR_SPEED_NORMAL,		// g_PlateRecogSpeed
		MD_NOTSURE,				// g_MovingDirection

		0,						// g_cnDetStep, ���岻ʹ��
		{0, 0, 100, 100},		// g_rectDetectionArea
		{0, 0, 100, 100},		// g_rectTrackArea

		1.0f,					// g_fltTrackInflateX
		1.0f,					// g_fltTrackInflateY
		3,						// g_nContFrames_EstablishTrack;
		3,						// g_nMissFrames_EndTrack;
		0,						// g_nMissFrames_EndTrackQuick;
		0,						// g_nObservedFrames_ConsiderAsTrack, ���岻ʹ��;
		0.12f,					// g_fltThreshold_StartTrack
		0.10f,					// g_fltThreshold_BeObservation
		0.0f,					// g_fltMinConfidenceForVote, ���岻ʹ�ã�

		////for plate detection
		8,						// g_nDetMinScaleNum
		18,						// g_nDetMaxScaleNum
		60,						// g_nDetMinStdVar
		4,						// g_nMinStopPlateDetNum
		false,					// g_fSubscanPredict
		0,						// g_nSubscanPredictMinScale
		0,						// g_nSubscanPredictMaxScale

		0.075f,					// g_kfltPlateInflateRate
		0.15f,					// g_kfltPlateInflateRateV
		0.2f,					// g_kfltHighInflateArea
		0.15f,					// g_kfltPlateInflateRate2
		0.3f,					// g_kfltPlateInflateRateV2

		false,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		1000,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		false,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		200,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		18,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		0,						//m_fUseTemplet;			// ģ��ƥ�俪��

		0,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		0,						//m_iDetectReverseEnable;		//�������м�⿪��
		0,						//m_iDetectOverYellowLineEnable;//����ѹ���߼�⿪��
		0,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		// ���Ӿ���
		PRM_ELECTRONIC_POLICE,
		PR_SPEED_FAST,
		MD_BOTTOM2TOP,			//g_MovingDirection

		0,						// g_cnDetStep, ���岻ʹ��
		{2, 60, 98, 100},		// g_rectDetectionArea
		{2, 40, 98, 100},		// g_rectTrackArea

		0.5f,					// g_fltTrackInflateX
		1.6f,					// g_fltTrackInflateY
		2,						// g_nContFrames_EstablishTrack;
		6,						// g_nMissFrames_EndTrack;
		0,						// g_nMissFrames_EndTrackQuick;
		0,						// g_nObservedFrames_ConsiderAsTrack, ���岻ʹ��;
		0.08f,					// g_fltThreshold_StartTrack
		0.08f,					// g_fltThreshold_BeObservation
		0.0f,					// g_fltMinConfidenceForVote, ���岻ʹ�ã�

		////for plate detection
		6,						// g_nDetMinScaleNum
		10,						// g_nDetMaxScaleNum
		16,						// g_nDetMinStdVar
		0,						// g_nMinStopPlateDetNum
		true,					// g_fSubscanPredict
		-2,						// g_nSubscanPredictMinScale
		1,						// g_nSubscanPredictMaxScale

		0.05f,					// g_kfltPlateInflateRate
		0.10f,					// g_kfltPlateInflateRateV
		0.0f,					// g_kfltHighInflateArea
		0.0f,					// g_kfltPlateInflateRate2
		0.0f,					// g_kfltPlateInflateRateV2

		true,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		200,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		false,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		1,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		1,						//m_fUseTemplet;			// ģ��ƥ�俪��

		0,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		1,						//m_iDetectReverseEnable;		//�������м�⿪��
		1,						//m_iDetectOverYellowLineEnable;//����ѹ���߼�⿪��
		1,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		// ��ͨ�¼���� - ��ͷ
		PRM_TRAFFICE_EVENT_HEAD,
		PR_SPEED_FAST,
		MD_TOP2BOTTOM,			// g_MovingDirection

		0,						// g_cnDetStep, ���岻ʹ��
 		{0, 0, 100, 50},		// g_rectDetectionArea
		{0, 0, 100, 100},		// g_rectTrackArea

		0.5f,					// g_fltTrackInflateX
		1.6f,					// g_fltTrackInflateY
		2,						// g_nContFrames_EstablishTrack;
		3,						// g_nMissFrames_EndTrack;
		0,						// g_nMissFrames_EndTrackQuick;
		0,						// g_nObservedFrames_ConsiderAsTrack, ���岻ʹ��;
		0.08f,					// g_fltThreshold_StartTrack
		0.08f,					// g_fltThreshold_BeObservation
		0.0f,					// g_fltMinConfidenceForVote, ���岻ʹ�ã�

		////for plate detection
		5,						// g_nDetMinScaleNum, 85~170
		9,						// g_nDetMaxScaleNum
		30,						// g_nDetMinStdVar
		0,						// g_nMinStopPlateDetNum
		true,					// g_fSubscanPredict
		-1,						// g_nSubscanPredictMinScale
		2,						// g_nSubscanPredictMaxScale

		0.05f,					// g_kfltPlateInflateRate
		0.10f,					// g_kfltPlateInflateRateV
		0.0f,					// g_kfltHighInflateArea
		0.0f,					// g_kfltPlateInflateRate2
		0.0f,					// g_kfltPlateInflateRateV2

		true,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		200,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		false,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		0,						//m_fUseTemplet;			// ģ��ƥ�俪��

		1,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		1,						//m_iDetectReverseEnable;		//�������м�⿪��
		1,						//m_iDetectOverYellowLineEnable;//����ѹ���߼�⿪��
		1,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		// ��ͨ�¼���� - ��β
		PRM_TRAFFICE_EVENT_TAIL,
		PR_SPEED_FAST,
		MD_BOTTOM2TOP,			//g_MovingDirection

		0,							// g_cnDetStep, ���岻ʹ��
		{0, 40, 100, 80},		// g_rectDetectionArea
		{0, 20, 100, 100},		// g_rectTrackArea

		0.5f,					// g_fltTrackInflateX
		1.6f,					// g_fltTrackInflateY
		2,						// g_nContFrames_EstablishTrack;
		6,						// g_nMissFrames_EndTrack;
		0,						// g_nMissFrames_EndTrackQuick;
		0,						// g_nObservedFrames_ConsiderAsTrack, ���岻ʹ��;
		0.08f,					// g_fltThreshold_StartTrack
		0.08f,					// g_fltThreshold_BeObservation
		0.0f,					// g_fltMinConfidenceForVote, ���岻ʹ�ã�

		////for plate detection
		7,						// g_nDetMinScaleNum
		13,						// g_nDetMaxScaleNum
		16,						// g_nDetMinStdVar
		0,						// g_nMinStopPlateDetNum
		true,					// g_fSubscanPredict
		-2,						// g_nSubscanPredictMinScale
		1,						// g_nSubscanPredictMaxScale

		0.05f,					// g_kfltPlateInflateRate
		0.10f,					// g_kfltPlateInflateRateV
		0.0f,					// g_kfltHighInflateArea
		0.0f,					// g_kfltPlateInflateRate2
		0.0f,					// g_kfltPlateInflateRateV2

		true,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		200,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		false,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		0,						//m_fUseTemplet;			// ģ��ƥ�俪��

		1,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		1,						//m_iDetectReverseEnable;		//�������м�⿪��
		1,						//m_iDetectOverYellowLineEnable;//����ѹ���߼�⿪��
		1,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		//ץ��ʶ��
		PRM_HVC,
		PR_SPEED_NORMAL,
		MD_TOP2BOTTOM,			// g_MovingDirection

		0,							// g_cnDetStep, ���岻ʹ��
		{0, 0, 100, 100},		// g_rectDetectionArea
		{0, 0, 100, 100},		// g_rectTrackArea

		0.5f,					// g_fltTrackInflateX
		0.5f,					// g_fltTrackInflateY
		3,						// g_nContFrames_EstablishTrack;
		15,						// g_nMissFrames_EndTrack;
		10,						// g_nMissFrames_EndTrackQuick;
		0,						// g_nObservedFrames_ConsiderAsTrack, ���岻ʹ��;
		0.15f,					// g_fltThreshold_StartTrack
		0.12f,					// g_fltThreshold_BeObservation
		0.0f,					// g_fltMinConfidenceForVote, ���岻ʹ�ã�

		////for plate detection
		6,						// g_nDetMinScaleNum
		14,						// g_nDetMaxScaleNum
		30,						// g_nDetMinStdVar
		6,						// g_nMinStopPlateDetNum
		false,					// g_fSubscanPredict
		0,						// g_nSubscanPredictMinScale
		0,						// g_nSubscanPredictMaxScale

		0.075f,					// g_kfltPlateInflateRate
		0.2f,					// g_kfltPlateInflateRateV
		0.2f,					// g_kfltHighInflateArea
		0.15f,					// g_kfltPlateInflateRate2
		0.3f,					// g_kfltPlateInflateRateV2

		false,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		400,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		false,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		0,						//m_fUseTemplet;			// ģ��ƥ�俪��

		0,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		0,						//m_iDetectReverseEnable;		//�������м�⿪��
		0,						//m_iDetectOverYellowLineEnable;//����ѹ���߼�⿪��
		0,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		// ͣ����˫·
		PRM_PARK,
		PR_SPEED_NORMAL,
		MD_NOTSURE,			// g_MovingDirection

		12,						// g_cnDetStep
		{20, 20, 80, 80},		// g_rectDetectionArea
		{0, 0, 100, 100},		// g_rectTrackArea

		0.5f,					// g_fltTrackInflateX
		0.5f,					// g_fltTrackInflateY
		3,						// g_nContFrames_EstablishTrack;
		15,						// g_nMissFrames_EndTrack;
		10,						// g_nMissFrames_EndTrackQuick;
		5,						// g_nObservedFrames_ConsiderAsTrack;
		0.15f,					// g_fltThreshold_StartTrack
		0.12f,					// g_fltThreshold_BeObservation
		0.4f,					// g_fltMinConfidenceForVote

		////for plate detection
		6,						// g_nDetMinScaleNum
		12,						// g_nDetMaxScaleNum
		30,						// g_nDetMinStdVar
		6,						// g_nMinStopPlateDetNum
		false,					// g_fSubscanPredict
		0,						// g_nSubscanPredictMinScale
		0,						// g_nSubscanPredictMaxScale

		0.075f,					// g_kfltPlateInflateRate
		0.2f,					// g_kfltPlateInflateRateV
		0.2f,					// g_kfltHighInflateArea
		0.15f,					// g_kfltPlateInflateRate2
		0.3f,					// g_kfltPlateInflateRateV2

		false,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		200,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		true,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		0,						//m_fUseTemplet;				// ģ��ƥ�俪��

		0,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		1,						//m_iDetectReverseEnable;			//�������м�⿪��
		0,						//m_iDetectOverYellowLineEnable;		//����ѹ���߼�⿪��
		0,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		// ͣ������·
		PRM_PARK_SINGLE,
		PR_SPEED_NORMAL,
		MD_NOTSURE,			// g_MovingDirection

		12,						// g_cnDetStep
		{20, 20, 80, 80},		// g_rectDetectionArea
		{0, 0, 100, 100},		// g_rectTrackArea

		0.5f,					// g_fltTrackInflateX
		0.5f,					// g_fltTrackInflateY
		3,						// g_nContFrames_EstablishTrack;
		15,						// g_nMissFrames_EndTrack;
		10,						// g_nMissFrames_EndTrackQuick;
		5,						// g_nObservedFrames_ConsiderAsTrack;
		0.15f,					// g_fltThreshold_StartTrack
		0.12f,					// g_fltThreshold_BeObservation
		0.4f,					// g_fltMinConfidenceForVote

		////for plate detection
		6,						// g_nDetMinScaleNum
		12,						// g_nDetMaxScaleNum
		30,						// g_nDetMinStdVar
		6,						// g_nMinStopPlateDetNum
		false,					// g_fSubscanPredict
		0,						// g_nSubscanPredictMinScale
		0,						// g_nSubscanPredictMaxScale

		0.075f,					// g_kfltPlateInflateRate
		0.2f,					// g_kfltPlateInflateRateV
		0.2f,					// g_kfltHighInflateArea
		0.15f,					// g_kfltPlateInflateRate2
		0.3f,					// g_kfltPlateInflateRateV2

		false,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		200,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		true,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		0,						//m_fUseTemplet;			// ģ��ƥ�俪��

		0,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		1,						//m_iDetectReverseEnable;		//�������м�⿪��
		0,						//m_iDetectOverYellowLineEnable;//����ѹ���߼�⿪��
		0,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		// ��������ץ��
		PRM_CAP_FACE,
		PR_SPEED_FAST,
		MD_TOP2BOTTOM,			// g_MovingDirection

		0,							// g_cnDetStep, ���岻ʹ��
		{0, 15, 100, 55},		// g_rectDetectionArea
		{0, 10, 100, 90},		// g_rectTrackArea

		0.5f,					// g_fltTrackInflateX
		1.6f,					// g_fltTrackInflateY
		2,						// g_nContFrames_EstablishTrack;
		3,						// g_nMissFrames_EndTrack;
		0,						// g_nMissFrames_EndTrackQuick;
		0,						// g_nObservedFrames_ConsiderAsTrack, ���岻ʹ��;
		0.08f,					// g_fltThreshold_StartTrack
		0.08f,					// g_fltThreshold_BeObservation
		0.0f,					// g_fltMinConfidenceForVote, ���岻ʹ�ã�

		////for plate detection
		5,						// g_nDetMinScaleNum, 85~170
		9,						// g_nDetMaxScaleNum
		30,						// g_nDetMinStdVar
		0,						// g_nMinStopPlateDetNum
		true,					// g_fSubscanPredict
		-1,						// g_nSubscanPredictMinScale
		2,						// g_nSubscanPredictMaxScale

		0.05f,					// g_kfltPlateInflateRate
		0.10f,					// g_kfltPlateInflateRateV
		0.0f,					// g_kfltHighInflateArea
		0.0f,					// g_kfltPlateInflateRate2
		0.0f,					// g_kfltPlateInflateRateV2

		true,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		200,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		false,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		0,						//m_fUseTemplet;				// ģ��ƥ�俪��

		0,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		1,						//m_iDetectReverseEnable;			//�������м�⿪��
		1,						//m_iDetectOverYellowLineEnable;		//����ѹ���߼�⿪��
		1,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
	{
		// ���������
		PRM_COVER_CTRL,
		PR_SPEED_FAST,
		MD_TOP2BOTTOM,			// g_MovingDirection

		0,							// g_cnDetStep, ���岻ʹ��
		{0, 15, 100, 55},		// g_rectDetectionArea
		{0, 10, 100, 90},		// g_rectTrackArea

		0.5f,					// g_fltTrackInflateX
		1.6f,					// g_fltTrackInflateY
		2,						// g_nContFrames_EstablishTrack;
		3,						// g_nMissFrames_EndTrack;
		0,						// g_nMissFrames_EndTrackQuick;
		0,						// g_nObservedFrames_ConsiderAsTrack, ���岻ʹ��;
		0.08f,					// g_fltThreshold_StartTrack
		0.08f,					// g_fltThreshold_BeObservation
		0.0f,					// g_fltMinConfidenceForVote, ���岻ʹ�ã�

		////for plate detection
		5,						// g_nDetMinScaleNum, 85~170
		9,						// g_nDetMaxScaleNum
		30,						// g_nDetMinStdVar
		0,						// g_nMinStopPlateDetNum
		true,					// g_fSubscanPredict
		-1,						// g_nSubscanPredictMinScale
		2,						// g_nSubscanPredictMaxScale

		0.05f,					// g_kfltPlateInflateRate
		0.10f,					// g_kfltPlateInflateRateV
		0.0f,					// g_kfltHighInflateArea
		0.0f,					// g_kfltPlateInflateRate2
		0.0f,					// g_kfltPlateInflateRateV2

		true,					// g_fSegPredict

		60,						// g_nMinPlateWidth, ����ʶ�����С���ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		200,					// g_nMaxPlateWidth, ����ʶ�������ƿ�ȣ��Էָ��Ľ��Ϊ׼��
		false,					// g_fBestResultOnly, ���ͼ�����ж��ʶ������ֻ�����õ�

		400,					// g_nMinDownSampleWidthInSeg, �ָ�ʱDownSample����С���
		52,						// g_nMinDownSampleHeightInSeg, �ָ�ʱDownSample����С�߶�

		0,						//m_fUsedTrafficLight;		//���Ӿ��쿪��
		0,						//m_fUseTemplet;				// ģ��ƥ�俪��

		0,						//m_iDetectCarStopEnable;		//����ֹͣ��⿪��
		1,						//m_iDetectReverseEnable;			//�������м�⿪��
		1,						//m_iDetectOverYellowLineEnable;		//����ѹ���߼�⿪��
		1,						//m_iDetectCrossLineEnable;		//�������߼�⿪��
	},
};
