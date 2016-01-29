#ifndef _CAMYU_LINK_OPT_H_
#define _CAMYU_LINK_OPT_H_
#include <swbasetype.h>
// ����˿ں�
#define CY_DATA_PORT 8888
// ���ƶ˿ں�
#define CY_CMD_PORT 8886
// ����Ӧ��
#define CY_THROB_ACK 0x55AA55AA

#define CY_CAMERA_INFO_LEN 0x160    /* ���������� */

/* 
����������Ŷ��壬���ע��//��*�ű�ʾ�˲����Ŷ������������Ч
���Jϵ��: DC-1335J DC-2035J 
*/

//��������������Ͷ���
#define DH_FRAME_I 10	// I֡
#define DH_FRAME_P 11	// P֡


//������Ϣ��  (0x00--------0x1f)
#define  CY_ID   			0x00 	//�豸��(0--���豸��!0--���豸)
#define  CY_WIDTH			0x01	//������ˮƽ������(���)
#define  CY_HEIGHT			0x02    //��������ֱ������(���)
#define  CY_BITS			0x03 	//ͼ�����ݵ�A/Dλ��	
#define  CY_COLOR			0x04    //ͼ�����ݸ�ʽ(0--�Ҷȣ�1--Bayer_RG��2--Bayer_GR��3--Bayer_BG��5--RGB��6--YUV422��7--JPEG)
#define  CY_HEART_BEAT     0x0F	//*����*

#define  CY_SOFT_VERSION  	0x12	//�̼�����汾
#define  CY_DEVICE_NO     	0x13    //��������

//��λ��Ϣ��  (0x20--------0x2f)
#define  CY_GAIN_UNIT     	0x20	//��ǰ����ֵ
#define  CY_OFFSET_UNIT    0x21	//��ǰƫ��ֵ
#define  CY_FREQ_UNIT    	0x22	//��ǰ֡Ƶֵ
#define  CY_TIME_UNIT     	0x23	//��ǰ�ع�ʱ��ֵ


//�������Ʒ�Χ��Ϣ��  (0x30--------0x4f)
#define  CY_MAX_GAIN		0x30	//�������ֵ
#define  CY_MIN_GAIN		0x31	//������Сֵ
#define  CY_MAX_OFFSET		0x32	//ƫ�����ֵ
#define  CY_MIN_OFFSET		0x33	//ƫ����Сֵ
#define  CY_MAX_FREQ       0x34	//֡Ƶ���ֵ
#define  CY_MIN_FREQ       0x35	//֡Ƶ��Сֵ
#define  CY_MAX_TIME       0x36	//�ع�ʱ�����ֵ
#define  CY_MIN_TIME       0x37	//�ع�ʱ����Сֵ
#define  CY_MAX_BALANCEMODE  0x38	//��ƽ�ⷽʽѡ�����ֵ
#define  CY_MAX_AGCLIMIT   0x39	//AGC����ֵ���ֵ
#define  CY_MAX_AGCSELECT  0x3a	//AGCȡ������ѡ�����ֵ
//---- ֻ����Ϣ��End -----

#define  CY_PROTECT_AREA   0x8f  // 0--0x13f����Ϊ��������������������  

//---- 2. �ɱ�̲������궨��(��/д) -----
//��ò���������ֵ = CYUSB_ReadCameraParam(������);
//�޸Ĳ�����CYUSB_UpdateCameraParam(������,����ֵ);
#define  CY_GAIN      		0x90	//��ǰ����ֵ
#define  CY_OFFSET      	0x91	//��ǰƫ��ֵ
#define  CY_FREQ      		0x92	//��ǰ֡Ƶֵ
#define  CY_TIME      		0x93	//��ǰ�ع�ʱ��ֵ
#define  CY_SYNC			0x94	//ͬ����ʽ(0--�ⴥ����1--���δ���)
//Ԥ��n������λ��.......

//add by�ƹ������������ץ������,2010-11-09
#define  TRI_EXP_TIME   0x103 // ���ģʽ��Ч(ץ��ʱ���õ��ع�ʱ��)
#define  TRI_GAIN       0x104 // ���ģʽ��Ч(ץ��ʱ���õ�����)
//end added


#define  CY_AGCMODE		0xb0 //������Ʒ�ʽ(1--�Զ�(AGC)��0--�ֶ�)
#define  CY_AGCLIMIT		0xb1 //AGC����ֵ (0-255)
#define  CY_AGCSELECT      0xb2 //AGCȡ������ѡ�� ��16λ��Ч���ֱ��Ӧ4x4��16������˳��Ϊ�����ң����ϵ���
#define  CY_AGCTIME        0xb3 //AGC����ӿ�������
#define  CY_AGC_GAIN_MAX   0xb4 //AGC������Χ 0 ~ Max dB
#define  CY_AGC_TIME_MIN   0xb5 //AGC����ӿ�������ʱ�����ӿ��ŵ�����Сֵ(us)
#define  CY_AGC_TIME_MAX   0xb6 //AGC����ӿ�������ʱ�����ӿ��ŵ������ֵ(us)
//Ԥ��n������λ��.......


#define  CY_GAIN_R         0xc9 //��ǰR����ֵ
#define  CY_GAIN_G			0xca //��ǰG����ֵ
#define  CY_GAIN_B			0xcb //��ǰB����ֵ
#define  CY_BALANCEMODE    0xd2 //��ƽ��У����ʽѡ�� 0--�ֶ���ƽ�� 1--�Զ���ƽ��.
//Ԥ��n������λ��.......

#define  CY_AUTOGRAY		0xe0 //*�Զ��Ҷ���ǿ(1--�Զ���0--�ֶ�)
#define  CY_GRID	  		0xe1 //���س��ֵ (����㣬1/2���)
#define  CY_GRID_1B1  0 //�����
#define  CY_GRID_1B2  1 //1/2���

//Ԥ��n������λ��.......

#define  CY_SOFT_TRIGGER	0xf0// ����������� 1-��ͨ��ץ�Ĳ��� 0-��ͨ��ץ�Ĳ���
#define  CY_TEST_IMAGE     0xf1 // ����ͼ��ѡ�� 1-����ͼ�� 0-����ͼ��
#define  CY_TIME_STAMP 	0xf2 // ��ʱ�������Ϊ��׼�����
//Ԥ��n������λ��.......


#define  CY_SERVER_IP      0x100 // ������IP(���ģʽ��Ч)
#define  CY_SERVER_PORT    0x101 // ������PORT(���ģʽ��Ч)
#define  CY_TRI_JPEG_Q     0x102 // ץ��ʱJPEG����(���ģʽ��Ч)

/* ������������� */
#define  CY_IP_ADDR		0x120 // �����IP��ַ
#define  CY_GATEWAY		0x121 // ���������
#define  CY_NETMASK		0x122 // �����ַ����
#define  CY_CTRL_PORT      0x123 // ���ƶ˿�
#define  CY_TRAN_PORT	    0x124 // ����˿�

#define  CY_TF_COUNT		0x125 // ����ץ��֡�� (1-3)
#define  CY_JPEG_SET		0x126 // JPEGѹ��Ʒ�� (0-100) 0--��� 100--����
#define  CY_EXP_OUT_EN     0x127 // �ع�ͬ�����( d0--ץ�Ĳ���� d1--Ƶ�� )
#define  CY_TRIG_SET		0x128 // ����֡��� 0���������� 1��2�����n֡���

#define  CY_RADAR_SET_0	0x12B // ���û���״����ݳ�ʱ����(��λms)
#define  CY_RADAR_SET_1	0x12C // *Ԥ��	
#define  CY_RADAR_SET_2	0x12D // �����״ﴮ��ͬ��ͷ�ֽڶ��壬32B�����ֳ�4���ֽڣ�����ʹ��4��ͬ���ֽ�
// ��������յ�ץ�Ĵ����󣬽���RS232���ݣ�������յ�RS232�����ֽ���4��ͬ���ֽ�
// ������һ��ƥ��ʱ����¼����n������Ƕ��֡��Ϣͷ�ش�
#define  CY_RADAR_SET_3	0x12E // ���ô��ڲ�����
#define  CY_RADAR_SET_4	0x12F // ����ͬ���ֺ���Ϣ���� 1~4�ֽ�

#define  CY_YEAR			0x130
#define  CY_MONTH		    0x131
#define  CY_DAY			0x132
#define  CY_HOUR			0x133
#define  CY_MINUTE		    0x134
#define  CY_SECOND		    0x135 // д��������ϵͳʱ�� 

#define  CY_TRIGGER_DELAY  0x136 // ץ����ʱ(us)
#define  CY_EXP_DELAY      0x137 // �ع���ʱ(us)
#define  CY_SELECT_IMAGE   0x13F // ѡͼ����
#define  CY_USER_INFO      0x140 // 0x140~0x14F �û���Ϣ 64Byte


/* ���������� */
#define  CY_SOFT_RESET		0xFF0002 // ��λ����
#define  CY_SYNC_PARAM		0xFF0003 // ͬ����������
#define  CY_SAVE_PARAM		0xFF0004 // ��������������

/*****************************************************************************/
/*                             ͼ��֡ͷ��Ϣ                                  */
/*****************************************************************************/
typedef struct
{
	WORD16 wWidth;       // ͼ���
	WORD16 wHeight;       // ͼ���

	WORD16 wBits;      // ͼ��λ�� 8~16bits
	WORD16 wFormat;      // ͼ�����ʹ���(0--�Ҷȣ�1--Bayer_RG��2--Bayer_GR��3--Bayer_BG��5--RGB��6--YUV422��7--JPEG)

	WORD16 wFrameType; // ֡����(0--��ͨ��1--ץ��ͼ��2--����֡��10--������I֡��11--������P֡)
	WORD16 wFrameRev;  // ����

	DWORD32  dwFirmwareVersion; //�̼�����汾
	DWORD32  dwDeviceNo; // �豸���

	DWORD32  dwLen;     // ͼ�����ݳ���

	DWORD32  dwSpeed;   // ˫��Ȧ����ֵ(us)
	DWORD32  dwRs232;   // ������Ϣ(1~4�ֽ�)	

	WORD16 wYear; // ͼ��ɼ�ʱ��
	WORD16 wMonth;
	WORD16 wDay;
	WORD16 wHour;
	WORD16 wMinute;
	WORD16 wSecond;

	DWORD32 dwIp;		// �ɼ���ǰ֡�������IP
	DWORD32 dwFrameCount;	//�ܹ���ץ��֡��Ŀ
	DWORD32 dwTriggerCount; //�ܹ��Ĵ�����
	DWORD32 dwTriggerIndex; //����������
	DWORD32 dwFrameNo; //֡��

	DWORD32 dwGain; //��ǰץ�Ĳ���
	DWORD32 dwTime; //�ع�ʱ��
	DWORD32 dwGainR; //������
	DWORD32 dwGainG; //������
	DWORD32 dwGainB; //������

	DWORD32 dwMode;  // ���������ģʽ
	DWORD32 dwJpegQ; // JPEGѹ��Ʒ��
	DWORD32 dwTd1;   // ץ����ʱ(��λus)
	DWORD32 dwTd2;   // �ع���ʱ(��λus)

	DWORD32 dwtrig_chl; //����ͨ��
	DWORD32 dwmsecond; //ms��ʱ
	DWORD32 dwyavg; //ƽ������
	DWORD32 dwmpeg_head;//MPEG4����ͷ��־
	DWORD32 dwSoftParam;//��������
	DWORD32 dwrev[7]; //��������

	BYTE8 rgbUserInfo[64]; // �û�����
} CY_FRAME_HEADER; // ֡ͷ

#endif
