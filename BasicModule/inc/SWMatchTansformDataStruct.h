#ifndef __SW_MATCH_TRANSFORM_DATA_STRUCT_H__
#define __SW_MATCH_TRANSFORM_DATA_STRUCT_H__

enum
{
	E_SIG_RADAR_SPEED = 0,
	E_SIG_COIL_SPEED
};
//�ź�
typedef struct tagSIGNAL
{
	DWORD dwTime;       //�ź�ʱ��
	DWORD dwSpeed;      //�ź��ٶ�
	DWORD dwSpeedType;	//�ٶ�����
	DWORD dwDirection;	//�״﷽��
	CSWImage* pImage;   //�źŸ�����ͼƬ
	BOOL fIsMatch;		//�Ƿ��Ѿ�ƥ�����
	tagSIGNAL()
	{
		swpa_memset(this, 0, sizeof(*this));
	}
}SIGNAL;
//�źŲ���
typedef struct tagSIGNAL_PARAM
{
	DWORD dwRoadID;		  //�ź������ĳ���,0xFFΪȫ����
	DWORD dwType;       //�ź�����
	DWORD dwSignalID;	  //ͬһ���źŵ�����
	BOOL    fTrigger;   //�Ƿ�Ҫ����ץ��,������Ƶ����źţ��ض�����ץ��
	BOOL    fRecognize; //�Ƿ�ʶ��
	DWORD dwCondition;  //ƥ��������0:���źſ��п��ޣ�ֻҪ�г��Ƽ��ɳ��ƣ�1:�����д��źźͳ��Ʋ��ܳ����,2:ֻҪ�д��źţ��ͱض�����
	SIGNAL  signal[6]; //���ź����͵��ź�����
	
	tagSIGNAL_PARAM()
	{
		swpa_memset(this, 0, sizeof(*this));
	}
}SIGNAL_PARAM;

#define DETECT_AREA_COUNT_MAX	32			//�����������
#define DETECT_AREA_POS_COUNT_MAX 32		//һ��Ԥ��λ���ĵ���	
#define PRESET_COUNT_MAX 256				//���Ԥ��λ��	

typedef struct _PECCANCY_PARKING_AREA
{
	BOOL fPeccancyAreaEnable;	//Υ������ʹ��
	INT iPresetNum;				//��ǰԤ��λֵ 
	INT iDetAreaNum;			//���������
	INT iDetAreaPosCount;	//[DETECT_AREA_COUNT_MAX];		//�����������
	INT rgiPosX[DETECT_AREA_POS_COUNT_MAX];				//�������X����
	INT rgiPosY[DETECT_AREA_POS_COUNT_MAX];				//�������Y����
//	INT rgiWeekDay[7];				//��ʾ����
	INT iBeginTime[2];				//��ʼ [0]ʱ[1]��
	INT iEndTime[2];				//���� [0]ʱ[1]��
	INT iLimitTime;				//����ͣ��ʱ��  ��λ����
}PECCANCY_PARKING_AREA;

typedef struct _PECCANCY_PARKING_PARAM
{
	BOOL fPeccancyPresetEnable[PRESET_COUNT_MAX];
	INT iNightUseLedLight;
	FLOAT fltConfidenceFilter;		//���Ŷȹ�����
	PECCANCY_PARKING_AREA cPeccancyParkingArea[DETECT_AREA_COUNT_MAX];
}PECCANCY_PARKING_PARAM;

//ƥ�����
typedef struct tagMATCHPARAM
{
	BOOL  fEnable;             //���ܿ�ʹ�ܿ���
	DWORD dwSignalKeepTime;    //�źű���ʱ��
	DWORD dwPlateKeepTime;     //���Ʊ���ʱ��
	DWORD dwMatchMinTime;      //����ʱ��-�ź�ʱ�����С���
	DWORD dwMatchMaxTime;      //�ź�ʱ��-����ʱ��������
	DWORD dwSignalNum;         //�ź�����
	SIGNAL_PARAM  signal[6];  //�����źŵ�����

	BOOL fCaptureRecong;		// ץ��ͼ��ʶ�𿪹�
	int iDetectorAreaLeft;
	int iDetectorAreaRight;
	int iDetectorAreaTop;
	int iDetectorAreaBottom;
	int iDetectorMinScaleNum;
	int iDetectorMaxScaleNum;
	PECCANCY_PARKING_PARAM cPeccancyParkingParam;	//Υ��ͣ����ز���
	
	tagMATCHPARAM()
	{
		swpa_memset(this, 0, sizeof(*this));
		fEnable = TRUE;
		fCaptureRecong = FALSE;
		dwPlateKeepTime = 1500;
		dwSignalKeepTime = 2000;
		dwMatchMaxTime = 1500;
		dwMatchMinTime = 1500;
	}
}MATCHPARAM;

//��Ȧ�����ṹ��
typedef struct tagCOIL_PARAM
{
	INT iTowCoilInstance;     //������Ȧ�ľ���
	INT iCoilEgdeType;        //�����ش��������½��ش���
	BYTE bTrigger;            //����ץ�ĵ���Ȧ,��λ������
	                          //λ0: ------��1����Ȧ����ץ��
	                          //λ1: ------��2����Ȧ����ץ��
	                          //λ2: ------��3����Ȧ����ץ��
	                          //λ3: ------��4����Ȧ����ץ��...
	
	tagCOIL_PARAM()
	{
		iTowCoilInstance = 50;
		iCoilEgdeType = 0;
		bTrigger = 0;
	}
}COIL_PARAM;

//�ⲿ�豸����
typedef struct tagDEVICEPARAM
{
	INT iBaudrate;
	INT iDeviceType;          //�豸����,"0:��;1:�����״�;2:�ս�������;3:���͵º��̵�ת����;4:����ά���״�;5:�Ĵ������״�"
	INT iRoadNo;			        // ����������
	INT iCommType;			      // �������ͣ�0��RS232 1��RS485
	INT iComNo2RoadNo1;	//����1��Ӧ����
	INT iComNo2RoadNo2;	//����2��Ӧ����
	INT iComNo2RoadNo3;	//����3��Ӧ����
	INT iComNo2RoadNo4;	//����4��Ӧ����
	
	COIL_PARAM cCoilParam;     //����������
	tagDEVICEPARAM()
	{
		iBaudrate = 9600;
		iDeviceType = 0;
		iRoadNo = 0;
		//����ֻ֧��RS485
		iCommType = 1;
		iComNo2RoadNo1=0;
		iComNo2RoadNo2=1;
		iComNo2RoadNo1=0;
		iComNo2RoadNo2=0;
	}
}DEVICEPARAM;
#endif

