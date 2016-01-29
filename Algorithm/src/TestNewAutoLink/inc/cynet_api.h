//
//                 CAMYU CORPORATION
//  This file is supplied for Camyu Network Digital Camera 
//
//  File:
//    CYNET_API.h
//
//  Purpose:
//
//  Version:
//    1.2.2
//
#if (RUN_PLATFORM == PLATFORM_WINDOWS)
#define CAMERA_INFO_LEN 0x160    /* ���������� */

/* 
 ����������Ŷ��壬���ע��//��*�ű�ʾ�˲����Ŷ������������Ч
 */

//������Ϣ��  (0x00--------0x1f)
#define  ID   			0x00 	//�豸��(0--���豸��!0--���豸)
#define  WIDTH			0x01	//������ˮƽ������(���)
#define  HEIGHT			0x02    //��������ֱ������(���)
#define  BITS			0x03 	//ͼ�����ݵ�A/Dλ��	
#define  COLOR			0x04    //ͼ�����ݸ�ʽ(0--�Ҷȣ�1--Bayer_RG��2--Bayer_GR��3--Bayer_BG��5--RGB��6--YUV422)
#define  HEART_BEAT     0x0F	//*����*

#define  SOFT_VERSION  	0x12	//�̼�����汾

//��λ��Ϣ��  (0x20--------0x2f)
#define  GAIN_UNIT     	0x20	//��ǰ����ֵ
#define  OFFSET_UNIT    0x21	//��ǰƫ��ֵ
#define  FREQ_UNIT    	0x22	//��ǰ֡Ƶֵ
#define  TIME_UNIT     	0x23	//��ǰ�ع�ʱ��ֵ


//�������Ʒ�Χ��Ϣ��  (0x30--------0x4f)
#define  MAX_GAIN		0x30	//�������ֵ
#define  MIN_GAIN		0x31	//������Сֵ
#define  MAX_OFFSET		0x32	//ƫ�����ֵ
#define  MIN_OFFSET		0x33	//ƫ����Сֵ
#define  MAX_FREQ       0x34	//֡Ƶ���ֵ
#define  MIN_FREQ       0x35	//֡Ƶ��Сֵ
#define  MAX_TIME       0x36	//�ع�ʱ�����ֵ
#define  MIN_TIME       0x37	//�ع�ʱ����Сֵ
#define  MAX_BALANCEMODE  0x38	//��ƽ�ⷽʽѡ�����ֵ
#define  MAX_AGCLIMIT   0x39	//AGC����ֵ���ֵ
#define  MAX_AGCSELECT  0x3a	//AGCȡ������ѡ�����ֵ
//---- ֻ����Ϣ��End -----

#define  PROTECT_AREA   0x8f  // 0--0x13f����Ϊ��������������������  

//---- 2. �ɱ�̲������궨��(��/д) -----
//��ò���������ֵ = CYUSB_ReadCameraParam(������);
//�޸Ĳ�����CYUSB_UpdateCameraParam(������,����ֵ);
#define  GAIN      		0x90	//��ǰ����ֵ
#define  OFFSET      	0x91	//��ǰƫ��ֵ
#define  FREQ      		0x92	//��ǰ֡Ƶֵ
#define  TIME      		0x93	//��ǰ�ع�ʱ��ֵ
#define  SYNC			0x94	//ͬ����ʽ(0--�ⴥ����1--���δ���)
//Ԥ��n������λ��.......


#define  AGCMODE		0xb0	//������Ʒ�ʽ(1--�Զ�(AGC)��0--�ֶ�)
#define  AGCLIMIT		0xb1	//AGC����ֵ
#define  AGCSELECT      0xb2    //AGCȡ������ѡ��
#define  AGCTIME		0xb3	//AGC�������ӿ���(1--��,0--��)
//Ԥ��n������λ��.......

#define  GAIN_R       	0xc9	//��ǰR����ֵ
#define  GAIN_G			0xca	//��ǰG����ֵ
#define  GAIN_B			0xcb	//��ǰB����ֵ
#define  BALANCEMODE    0xd2    //��ƽ��У����ʽѡ�� 0--��ֹ��ƽ��  1---���ΰ�ƽ��  2---�Զ���ƽ�����.
//Ԥ��n������λ��.......

#define  AUTOGRAY		0xe0	//�Զ��Ҷ���ǿ(1--�Զ���0--�ֶ�)
#define  GRID	  		0xe1	//���س��ֵ (����㣬1/2��㣬1/4���)
  #define GRID_1B1  0 //�����
  #define GRID_1B2  1 //1/2���
  #define GRID_1B4  2 //1/4���
                                      
//Ԥ��n������λ��.......


/* ������������� */
#define  IP_ADDR		0x120	// �����IP��ַ
#define  GATEWAY		0x121	// ���������
#define	 NETMASK		0x122	// �����ַ����
#define  CTRL_PORT      0x123   // ���ƶ˿�
#define  TRAN_PORT		0x124	// ����˿�
#define	 SERVER_IP		0x125	// ������IP��ַ
#define  SERVER_PORT	0x126   // ��������˿�(0 ��ץ��)     
#define  TF_COUNT		0x127   // Trigger Frame Count ����ץ��֡��
#define  JPEG_SET		0x128   // *���λ(0--��ѹ����1--ѹ��)����16λ��ʾJPEGѹ��Ʒ��
#define  TRIG_SET		0x129	// �����ź���������λus( 0us ~ 65ms ) 		
								// �ع�ͬ�����( 0--������� 1--��Ч֡ʱ��� )
								// ����֡��� 0���������� 1��2��3�����n֡���
#define  AI_LEVEL		0x12A	// *Auto-Iris Level �Զ���Ȧ�ȼ�
#define  RADAR_SET_0	0x12B   // ���û���״����ݳ�ʱ����������20ms�����δ���ģʽʱ��Ч
#define  RADAR_SET_1	0x12C	// Ԥ��	
#define  RADAR_SET_2	0x12D	// �����״ﴮ��ͬ��ͷ�ֽڶ��壬32B�����ֳ�4���ֽڣ�����ʹ��4��ͬ���ֽ�
                                // ��������յ�ץ�Ĵ����󣬽���RS232���ݣ�������յ�RS232�����ֽ���4��ͬ���ֽ�
                                // ������һ��ƥ��ʱ����¼����2������Ƕ��֡��Ϣͷ�ش�

#define  CYNET_YEAR			0x130
#define  CYNET_MONTH			0x131
#define  CYNET_DAY			0x132
#define  CYNET_HOUR			0x133
#define  CYNET_MINUTE			0x134
#define  CYNET_SECOND			0x135   // д��������ϵͳʱ�� 

/* ���������� */
#define  UPGRADE_FIRMWARE	0xFF0001 // �����̼�����
#define  SOFT_RESET		0xFF0002 // ��λ����
#define  SYNC_PARAM		0xFF0003 // ͬ����������
#define  SAVE_PARAM		0xFF0004 // ��������������

/* ����ֵ */
#define RET_ERR	0
#define RET_OK	1

/* SDK API */
extern "C" {
/*****************************************************************************/
/*                             ��ʼ����                                      */
/* ����: ��                                                                  */
/*****************************************************************************/
void __declspec(dllexport) CYNET_Open(void);


/*****************************************************************************/
/*                        �ͷ�������Դ���رտ�                               */
/* ����: ��                                                                  */
/*****************************************************************************/
void __declspec(dllexport) CYNET_Close(void);

/*****************************************************************************/
/* �Թ���������������������������                                          */
/* $������Ŀ���������5����û���յ�������Զ��Ͽ�����                      */
/* $���Ҫ��ʱ�䱣�ֿ������ӣ���ʱ���øú���                                 */
/* ����: ��                                                                  */
/*****************************************************************************/
void __declspec(dllexport) CYNET_SendHeartBeatPackage(void);

/*****************************************************************************/
/*                        ���ָ��IP�����                                   */
/* ����: ip    ׼����ӵ�����������IP��ַ                                  */
/*       portc ׼����ӵ����������Ŀ��ƶ˿�                                */
/*       ports ׼����ӵ����������Ĵ���˿�                                */
/*****************************************************************************/
int __declspec(dllexport) CYNET_AddCamera(char *ip, USHORT portc, USHORT ports);


/*****************************************************************************/
/*                        ɾ��ָ��IP�����                                   */
/* ����: ip    ׼���ӿ���ɾ�����������IP��ַ                                */
/*****************************************************************************/
int __declspec(dllexport) CYNET_DelCamera(char *ip);


/*****************************************************************************/
/*             ѡ��ָ��IP��������Ժ���������Ե�ǰѡ�е������              */
/* ����: ip    Ҫѡ�е��������IP��ַ                                        */
/*****************************************************************************/
int __declspec(dllexport) CYNET_SelectCamera(char *ip);


/*****************************************************************************/
/*                       ѡ��������ɼ���ʼ/ֹͣ                             */
/* ����: bCapStart    TRUE  ��ʼ�ɼ�                                         */
/*                    FALSE ֹͣ�ɼ�                                         */
/*****************************************************************************/
int __declspec(dllexport) CYNET_CapStart(BOOL bCapStart);


/*****************************************************************************/
/*                       �ȴ����������ɼ�ͼ��                              */
/* ����: pData    ��������ڴ�����Ҫ�㹻���һ֡BAYER����                    */
/*       pLen     ��Ųɼ����ݵĳ���                                         */
/*****************************************************************************/
int __declspec(dllexport) CYNET_GetFrame(char *pData,int *pLen,char *ip=NULL);


/*****************************************************************************/
/*                     ѡ�����������ͨ����/�ر�                           */
/* ����: bCtrlStart   TRUE  �򿪿���ͨ��                                     */
/*                    FALSE �رտ���ͨ��                                     */
/*****************************************************************************/
int __declspec(dllexport) CYNET_CtrlStart(BOOL bCtrlStart);


/*****************************************************************************/
/*                         ��ȡ���������                                    */
/* ����: nName   ���������                                                  */
/* ����: ��ȡ�Ĳ���ֵ                                                        */
/*****************************************************************************/
ULONG __declspec(dllexport) CYNET_ReadCameraParam(int nName);


/*****************************************************************************/
/*                         �������������                                    */
/* ����: nName   ���������                                                  */
/*       nParam  ���ò���1                                                   */
/*       nParam  ���ò���2                                                   */
/*****************************************************************************/
int __declspec(dllexport) CYNET_UpdateCameraParam(int nName, ULONG nParam,ULONG nParam2=0);


/*****************************************************************************/
/*                         �������������                                    */
/* ����: nName   ������                                                  */
/*       nBuf    �������ݻ�����ָ��                                          */
/*****************************************************************************/
int __declspec(dllexport) CYNET_SendCommand(int nCmd, char *nBuf=NULL);

/*****************************************************************************/
/*                         �õ����������ĵ�λ                                */
/* ע:   ʵ��ֵ = ����ֵ x ��λ                                              */ 
/*       ��ȡ���浥λʱ�����ص�nB�����0����ʾdB��1��ʾ����                  */
/*****************************************************************************/
double __declspec(dllexport) CYNET_GetTimeUnit(); //��λ����
double __declspec(dllexport) CYNET_GetFrameUnit();//��λ֡/��
double __declspec(dllexport) CYNET_GetGainUnit(int *nB); //��λnB (0 - dB 1 - ����)
int	   __declspec(dllexport) CYNET_GetOffsetUnit(); //��λ�Ҷ�ֵ


/*****************************************************************************/
/*                 ��׼��ɫ�ؽ�(Bayerת����RGB)                              */
/* ����: pRGB    �û������������ת�����RGBͼ�����ݣ�RGB��������Bayer��3����*/
/*               ע������㹻���ڴ���                                        */
/*       pBayer  ָ��Bayer��ʽͼ��Ļ�����                                   */
/*       w       ͼ���                                                      */
/*       h       ͼ���                                                      */
/*****************************************************************************/
void __declspec(dllexport) CYNET_BayerToRGB(BYTE* pRGB,BYTE* pBayer,int w,int h);
void __declspec(dllexport) CYNET_BayerToRGB_12bit(USHORT* pRGB,USHORT* pBayer,int w,int h);

/*****************************************************************************/
/*                 ��������ɫ�ؽ�(Bayerת����RGB)                            */
/* ����: pRGB    �û������������ת�����RGBͼ�����ݣ�RGB��������Bayer��3����*/
/*               ע������㹻���ڴ���                                        */
/*       pBayer  ָ��Bayer��ʽͼ��Ļ�����                                   */
/*       w       ͼ���                                                      */
/*       h       ͼ���                                                      */
/*****************************************************************************/
void __declspec(dllexport) CYNET_BayerToRGB_HQI(BYTE* pRGB,BYTE* pBayer,int w,int h);
void __declspec(dllexport) CYNET_BayerToRGB_HQI_12bit(USHORT* pRGB,USHORT* pBayer,int w,int h);

/*****************************************************************************/
/*                             �����ƽ��У��                                */
/* ����: pRGB    ָ��RGBͼ������                                             */
/*       nW      ͼ���                                                      */
/*       nH      ͼ���                                                      */
/*****************************************************************************/
void __declspec(dllexport) CYNET_WhiteBalance(unsigned char *pRGB,int nW,int nH);
void __declspec(dllexport) CYNET_WhiteBalance_12bit(unsigned short *pRGB,int nW,int nH);

/*****************************************************************************/
/*                             ����JPEG�ļ�                                  */
/* ����: lpSrcBuffer  ָ��JPEG����Դ                                         */
/*       dwSrcSize    ����Դ����                                             */
/*               ע������㹻���ڴ���                                        */
/*       lpDstBuffer  ָ��Ŀ���ڴ�                                           */
/*       dwDstSize    ���ڻ�ý�����ͼ�����ݳ���                           */
/*       lpdwWidth    ���ͼ���                                             */
/*       lpdwHeight   ���ͼ���                                             */
/*       lpBits       ���ͼ������λ��                                       */
/*       dwReserve1   ���һ��ͼ�����ݵĳ���                                 */
/*****************************************************************************/
BOOL __declspec(dllexport) CYNET_JpegDecode(BYTE *lpSrcBuffer, DWORD dwSrcSize,
					                        BYTE *lpDstBuffer, DWORD *dwDstSize,
					                        DWORD *lpdwWidth, DWORD *lpdwHeight,
					                        DWORD *lpBits, DWORD *dwReserve1);
}

/*****************************************************************************/
/*                             ͼ��֡ͷ��Ϣ                                  */
/*****************************************************************************/
#endif
typedef struct
{
	unsigned short w;       // ͼ���
	unsigned short h;       // ͼ���
	
	unsigned short ad;      // ͼ��λ�� 8~16bits
	unsigned short hi;      // ͼ�����ʹ��� 1-Bayer 2-JPEG
	
	unsigned long  len;     // ͼ�����ݳ���

	unsigned long ip;		// �ɼ���ǰ֡�������IP
	unsigned long frame_count;	//�ܹ��Ĵ���֡��Ŀ
	unsigned long trigger_info; //��16λ������������16λ��ǰ�����ĵڼ�֡
	
	unsigned long r4;		// ����֡ʱ���������δ������ʱ��(us)
	unsigned long r5;
	unsigned long r6;

} FRAME_HEADER; // ֡ͷ


