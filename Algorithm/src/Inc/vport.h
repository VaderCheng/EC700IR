/* 
    ���ĵ�Ϊ��Ƶ�˿����ú궨��ͽӿ�����.
    ���л���2004��10��15��10:24�����޸�.
*/
#ifndef _VPORT_H
#define _VPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "std.h"
#include "csl.h"
#include <iom.h>
#include "edc.h"

/* �й�VP�˿ڵ��������ú궨��. */
/*------------------------------------------------------------------------------------------------------------------------------------*/
/* VP ��Ƶ������ģʽ. */
#define VPORT_FLDOP_FLD1              	0x1    //��1 CAP/DIS ONLY.
#define VPORT_FLDOP_FLD2              	0x2    //��2 CAP/DIS ONLY.
#define VPORT_FLDOP_FRAME             	0x4    //֡ CAP/DIS.
#define VPORT_FLDOP_PROGRESSIVE       	0x5

/* VP�˿ڲ���ģʽ. */
#define VPORT_MODE_BT656_8BIT        	0x0
#define VPORT_MODE_BT656_10BIT       	0x1
#define VPORT_MODE_RAW_8BIT          	0x2
#define VPORT_MODE_RAW_10BIT         	0x3
#define VPORT_MODE_YCBCR_8BIT        	0x4
#define VPORT_MODE_YCBCR_10BIT       	0x5
#define VPORT_MODE_RAW_16BIT         	0x6
#define VPORT_MODE_RAW_20BIT	     	0x7
                                     	
/* VP�˿��ⲿ���ż���. */            	
#define VPORT_POLARITY_ACTIVE_HIGH   	0x00
#define VPORT_POLARITY_ACTIVE_LOW    	0x11
                
/* VP�˿ڴ�ֱ����ģʽ,���ڲ����������С1/2,������ʾ�����ǷŴ�2. */
#define VPORT_SCALING_DISABLE        	0x0
#define VPORT_SCALING_ENABLE         	0x1

/* VP�˿�ɫ���ز�����������,���ڲ�׽�������� 4:2:2 => 4:2:0,������ʾ������4:2:0 => 4:2:2. */
#define VPORT_RESMPL_DISABLE         	0x0
#define VPORT_RESMPL_ENABLE          	0x1          
                                           
/* VP�˿���Ƶ֡�����Ƿֿ�����,�����ۺϵ�һ��. */                                      
#define VPORT_FLDS_SEPARATED         	0
#define VPORT_FLDS_MERGED				1
/*------------------------------------------------------------------------------------------------------------------------------------*/





/* VP�˿�IOM����������ؽӿںͿ�������궨�� */
/*------------------------------------------------------------------------------------------------------------------------------------*/
/* IOM�豸����������֧�ֵĿ������. */
#define VPORT_CMD_RESET                 0x10000001  //VP�˿ڸ�λ����.
#define VPORT_CMD_CONFIG_PORT           0x10000002  //VP�˿���������.
#define VPORT_CMD_CONFIG_CHAN           0x10000003  //VP�˿�ͨ����������.
#define VPORT_CMD_START                 0x10000004  //����VP�˿ڲ���.
#define VPORT_CMD_STOP                  0x10000005  //ֹͣVP�˿ڲ���.
#define VPORT_CMD_SET_VINTCB            0x10000006  //����VP�˿��ж�Callback����.
#define VPORT_CMD_DUND_RECOVER          0x10000007  //ǿ��VP����ʾFIFO����״̬�ָ�.
#define VPORT_CMD_COVR_RECOVER          0x10000007  //ǿ��VP�Ӳ�׽FIFO����״̬�ָ�.
#define VPORT_CMD_SET_BUFFNUM			0x10000008  //���û���������.
#define VPORT_CMD_GET_BUFFNUM			0x10000009  //���û���������.

/* EDC������������� */
#define VPORT_CMD_EDC_BASE              0x80000000
#define VPORT_CMD_EDC_CONFIG			( VPORT_CMD_EDC_BASE + EDC_CONFIG )		//config��������.		Video Interface IC Contorl Command.
#define VPORT_CMD_EDC_RESET				( VPORT_CMD_EDC_BASE + EDC_RESET )		//reset��������.        Video Interface IC Contorl Command.
#define VPORT_CMD_EDC_START				( VPORT_CMD_EDC_BASE + EDC_START )		//start��������.        Video Interface IC Contorl Command.
#define VPORT_CMD_EDC_STOP				( VPORT_CMD_EDC_BASE + EDC_STOP )		//stop��������.         Video Interface IC Contorl Command.
#define VPORT_CMD_EDC_GET_CONFIG		( VPORT_CMD_EDC_BASE + EDC_GET_CONFIG )	//getconfig��������.    Video Interface IC Contorl Command.
#define VPORT_CMD_EDC_GET_STATUS		( VPORT_CMD_EDC_BASE + EDC_CONFIG )		//getstatus��������.    Video Interface IC Contorl Command.


#define VPORT_MAX_NUM_FRMBUFS           20          // ��Ƶ֡��󻺳�������

/* ����Callback����ԭ��.����һ��ͨ��/�豸ʵ������,Ӧֻ��һ��VP�˿�ȫ���ж�Callback����,�����ε���(������ǰ������)ֻ�������һ��Ϊ׼. */
typedef void ( *VPORT_IntCallBack )( Arg id, Arg mask );

/* VP�˿��ж����α�־����. */
#define VPORT_INT_COVR                  0x0002      //��׽FIFO�����ж�.
#define VPORT_INT_CCMP                  0x0004      //��׽����ж�.
#define VPORT_INT_SERR                  0x0008      //ͬ�������ж�.
#define VPORT_INT_VINT1                 0x0010      //��1�ж�.
#define VPORT_INT_VINT2                 0x0020      //��2�ж�.
#define VPORT_INT_SFD                   0x0040      //�̳�����ж�.
#define VPORT_INT_LFD                   0x0080      //��������ж�.
#define VPORT_INT_STC                   0x0400      //ϵͳʱ���ж�.
#define VPORT_INT_TICK                  0x0800      //ʱ�����ж�.
#define VPORT_INT_DUND                  0x1000      //��ʾFIFO�����ж�.
#define VPORT_INT_DCMP                  0x2000      //��ʾ����ж�.
#define VPORT_INT_DCNA                  0x4000      //��ʾ��ɵ���û�л�Ӧ�ж�.

/* ��Ƶ�˿�Callback�����������ݽṹ. */                                           
typedef struct VPORT_VIntCbParams{
    Int                 cbArg;          //��ǰ���ж�Callback����Ϊ�ξ����ͨ�������豸ʵ��.
    VPORT_IntCallBack   vIntCbFxn;      //ָ������Callback����.
    Uint16              vIntMask;       //ʹ�ø��ж�Callback�������ж���������.
    Uint16              vIntLine;       //��ˮƽ���ж�ˮƽ��ֵ.
    Int                 irqId;          //�ж�ID.    
} VPORT_VIntCbParams;


/* VP�˿��������ݽṹ. */
typedef struct VPORT_PortParams{
    Bool                dualChanEnable; //VP�˿�FIFO�Ƿ�ʹ��˫ͨ��.
    Uns                 vc1Polarity;    //VP�˿ڿ�������1����.
    Uns                 vc2Polarity;    //VP�˿ڿ�������2����.
    Uns                 vc3Polarity;    //VP�˿ڿ�������3����.
    EDC_Fxns*           edcTbl[2];      //�ⲿEDC���ƺ�����ָ��,Ϊÿ��ͨ������һ��EDC����������.
} VPORT_PortParams;
/*------------------------------------------------------------------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* _VPORT_H */
