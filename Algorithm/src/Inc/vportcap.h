/* 
    ���ĵ�Ϊ��Ƶ��׽�˿����ú궨��ͽӿ�����.
    ���л���2004��10��15��11:24�����޸�.
*/
#ifndef _VPORTCAP_H
#define _VPORTCAP_H

#ifdef __cplusplus
extern "C" {
#endif
#include "vport.h"



/* �й�VP ��׽�˿ڵ��������ú궨��. */
/*------------------------------------------------------------------------------------------------------------------------------------*/
/* ����ĳ��ź��Ƿ���Ҫ����. */
#define   VPORTCAP_FINV_DISABLE             0x0			
#define   VPORTCAP_FINV_ENABLE              0x1

/* VP�˿�ɨ���߼�������λλ��ģʽ�궨��. */
#define   VPORTCAP_VRST_START_VBLK          0x0 
#define   VPORTCAP_VRST_START_VSYNC         0x0            
#define   VPORTCAP_VRST_EAV_V0              0x0

#define   VPORTCAP_VRST_END_VBLK            0x1 
#define   VPORTCAP_VRST_END_VSYNC           0x1 
#define   VPORTCAP_VRST_EAV_V1              0x1


/* VP�˿����ؼ�������λλ��ģʽ�궨��. */
#define   VPORTCAP_HRST_EAV                 0x0 
#define   VPORTCAP_HRST_START_HBLK          0x0
#define   VPORTCAP_HRST_START_HSYNC         0x0 

#define   VPORTCAP_HRST_SAV                 0x1 
#define   VPORTCAP_HRST_END_HBLK            0x1
#define   VPORTCAP_HRST_END_HSYNC           0x1 

/* VP�˿ڳ����ģʽ���üĴ���. */
#define   VPORTCAP_FLDD_DISABLE             0x0             //ʹ��EAV,SAV�������������ź�.
#define   VPORTCAP_FLDD_ENABLE              0x1             //�ɳ��ź��������ŵ��߼��ó�.

/* VP��׽�ⲿ����ģʽ����. */
#define   VPORTCAP_EXC_DISABLE              0x0             //ʹ���������ϵ�EAV,SAV�������������ź�.   
#define   VPORTCAP_EXC_ENABLE               0x1             //��HCTL,VCTL,FID�ź�������������ȡ�����߼�.

/* VP��׽�˿�(10λ��ʽ)ʱ�İ���ʽ����. */
#define	   VPORTCAP_BPK_10BIT_ZERO_EXTENDED 0x0
#define    VPORTCAP_BPK_10BIT_SIGN_EXTENDED 0x1
#define    VPORTCAP_BPK_10BIT_DENSE         0x2


/* VP�������򻺳����������. */
#define 	VP_DEVICE_NEW_FRAME				0
#define 	VP_DEVICE_BUFFER				1
#define 	VP_DEVICE_ASYMMETRY_QUEUE		2

/* IOM�豸���������ⲿ�ӿ����ݽṹ���������. */
/*------------------------------------------------------------------------------------------------------------------------------------*/
/* VP�˿���Ϊ��׽�˿�ʹ��ʱ�������������ݽṹ����,��Щ�й����õĺ궨���Vport.h�ļ�. */
typedef struct {
    /* -------------------------------VP�˿ڹ�������.----------------------- */
    Int     cmode;                          //��Ƶ��׽ģʽ,
    Int     fldOp;                          //��׽��������,�ǲ�׽��1/2����֡,Bit2:����ָʾ�Ƿ���֡,Bit1:��2,Bit0:��1, Ϊ1��ʹ��Ϊ0�ǽ�ֹ.
    Int     scale;                          //ˮƽ��������
    Int     resmpl;                         //ɫ�����²�������,����4:2:2�ز�����4:2:0
    Int     bpk10Bit;                       //10λ��ģʽ����.
    Int     hCtRst;                         //ˮƽɨ���߼�������λģʽ����.
    Int     vCtRst;                         //��ֱ��(������)��������λģʽ����.
    Int     fldDect;                        //��Ƶ��׽�����ģʽ����.
    Int     extCtl;                         //��Ƶ��׽ʱ���߼���������.
    Int     fldInv;                         //���źŷ�������.
    
    Uint16  fldXStrt1;                      //��1 X��ʼ���ص�.
    Uint16  fldYStrt1;                      //��1 Y��ʼ���ص�.
    Uint16  fldXStrt2;                      //��2 X��ʼ���ص�.
    Uint16  fldYStrt2;                      //��2 Y��ʼ���ص�.
    Uint16  fldXStop1;                      //��1 X�������ص�.
    Uint16  fldYStop1;                      //��1 Y�������ص�. 
    Uint16  fldXStop2;                      //��2 X�������ص�.
    Uint16  fldYStop2;                      //��2 Y�������ص�.
    Uint16  thrld;                          //����DMA�¼�����ֵ(������Ϊ��λ).
    
    /* -------------------------------VP����֡����.------------------------- */
    Int     numFrmBufs;                     //ΪIOM�豸����������ٸ�֡������,����������Ƶ��׽����.
    Int     alignment;                      //֡����������segId�����õ��ڴ���е���ʼ.
    Int     mergeFlds;                      //�����Ƿ���Ҫ�ϲ���1/2������.
    Int     segId;                          //���������Ƶ��׽���ݵ��ڴ������.

    /* -------------------------------EDMA���ȼ�����.----------------------- */
    Int     edmaPri;                        //EDMA���ȼ�����.

    /* -------------------------------VP�˿�ID����.------------------------- */
    Int     irqId;                          //�ж�ID����
    
    /* -------------------------------VP������ģʽ------------------------- */
    Int		queueMode;						//����ָʾVP����������ģʽ.
    Int		iCtrlNumFrmBufs;				//���Գƻ���������������Ŀ.
} VPORTCAP_Params;

extern IOM_Fxns VPORTCAP_Fxns;




#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* _VPORTCAP_H */
