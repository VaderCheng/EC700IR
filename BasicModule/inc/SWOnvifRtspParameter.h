#ifndef __SW_ONVIF_RTSP_PARAMETER_H__
#define __SW_ONVIF_RTSP_PARAMETER_H__

#define RTSP_STRING_LEN 	128
#define RTSP_STREAM_MAX 	2			//Ŀǰ��������


//ONVIF �������
typedef struct _ONVIF_PARAM_INFO
{
	int iOnvifEnable;
	int iAuthenticateEnable;							//��Ȩ 0 ��ʹ��  1 ʹ��
	_ONVIF_PARAM_INFO()
	{
		iOnvifEnable = 0;
		iAuthenticateEnable = 0;
	}
}ONVIF_PARAM_INFO;


//RTSP �������
typedef enum _RTSP_STREAM_INDEX
{
	RTSP_STREAM_MAJOR = 0,
	RTSP_STREAM_MINOR
}RTSP_STREAM_INDEX;


typedef struct _RTSP_PARAM_INFO
{
	int iRTSPStreamNum;									//RTSP ������
	int rgCommunicationMode[RTSP_STREAM_MAX];			//ͨѶģʽ 0 ����  1 �鲥
	int iAuthenticateEnable;							//��Ȩ 0 ��ʹ��  1 ʹ��
	int iAutoControlBitrateEnable;						//�Զ��������� 0 ��ʹ�� 1 ʹ��
	int iRTSPServerPort;								//RTSP����˿ں�
	char rgStreamName[RTSP_STREAM_MAX][RTSP_STRING_LEN];//������
//	char rgUserName[RTSP_STRING_LEN];					//�û���
//	char rgPassword[RTSP_STRING_LEN];					//��Ȩ������

	_RTSP_PARAM_INFO()
	{
		iRTSPStreamNum			= 2;				//Ĭ�ϱ�����·����
		rgCommunicationMode[0]	= 0;
		rgCommunicationMode[1]	= 0;				//��·Ĭ�϶��ǵ���
		iAuthenticateEnable		= 0;				//Ĭ�ϲ�����Ȩ
		iAutoControlBitrateEnable = 1;
		iRTSPServerPort			= 554;				//Ĭ�϶˿ں�
		strcpy(rgStreamName[0], "h264ESVideoTest");
		strcpy(rgStreamName[1], "h264ESVideoTestSecond");
//		strcpy(rgUserName, "RTSPUserName");
//		strcpy(rgPassword, "RTSPPassword");
	}
}RTSP_PARAM_INFO;


typedef enum E_H264
{
	H264_STATUS_FIRST_STREAM_CONN = 0,		//����״ָ̬��
	H264_STATUS_FIRST_STREAM_REDUCE,		//�����ź�
	H264_STATUS_FIRST_STREAM_ASCEND,		//�����ź�
	H264_STATUS_SECOND_STREAM_CONN,
	H264_STATUS_SECOND_STREAM_REDUCE,
	H264_STATUS_SECOND_STREAM_ASCEND,
	H264_STATUS_AUTHENTICATE
} E_H264_STATUS;

typedef struct _H264_STATUS_HEADER
{
	E_H264_STATUS eH264StatusCmd;
	unsigned int iDataLen;
}H264_STATUS_HEADER;

#endif
