/** ==================================================================
 *  @file   SW_Alg_Aewb.h
 *
 *	@date	2015.08.05
 *
 *  @author   lujy
 * ===================================================================*/

#ifndef _SW_ALG_AEWB_H_
#define _SW_ALG_AEWB_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void (*SW_2A_PLATPORM_PRINTF)(const char *string);

typedef void* (*SW_2A_PLATPORM_MEM_ALLOC)(int nSize);

typedef void (*SW_2A_PLATPORM_MEM_FREE)(void *Point);

typedef int (*SW_2A_PLATPORM_SPI_READ)(unsigned int addr,unsigned int *value);

typedef int  (*SW_2A_PLATPORM_SPI_WRITE)(unsigned int addr,unsigned int value);

typedef int  (*SW_2A_PLATPORM_SPI_WRITE_Mult)(unsigned int *addr,unsigned int *value,unsigned int count);

typedef void (*SW_2A_PLATPORM_SET_DENOISE_STATUS)(unsigned int Tnf_status,unsigned int Snf_status);

typedef void (*SW_2A_PLATPORM_SET_PWM_DC)(unsigned int duty,unsigned int ConstDuty);


typedef struct {
    SW_2A_PLATPORM_PRINTF Printf;

    SW_2A_PLATPORM_MEM_ALLOC Malloc;

    SW_2A_PLATPORM_MEM_FREE Free;

    SW_2A_PLATPORM_SPI_READ Spi_Read;

    SW_2A_PLATPORM_SPI_WRITE Spi_Write;
    
    SW_2A_PLATPORM_SPI_WRITE_Mult Spi_Write_Mult;

    SW_2A_PLATPORM_SET_DENOISE_STATUS Set_Denoise_status;

    SW_2A_PLATPORM_SET_PWM_DC Set_PWM_DC;    
}SW_2A_Platform_Operator;

typedef enum SW_2A_CMD_e
{
    //Set
    SW2A_CMD_SET_SHUTTER = 0,//���ÿ���
    SW2A_CMD_SET_GAIN,//��������
    SW2A_CMD_SET_RGB_GAIN,//����RGB����
    SW2A_CMD_SET_AE_STATE,//����AEʹ��
    SW2A_CMD_SET_AE_THRESHOLD,//����AE��׼ֵ
    SW2A_CMD_SET_AE_EXP_RANGE,//���ÿ��ŷ�Χ
    SW2A_CMD_SET_AE_ZONE,//���ò������
    SW2A_CMD_SET_AE_GAIN_RANGE,//�������淶Χ
    SW2A_CMD_SET_SCENE,//���ó���ģʽ
    SW2A_CMD_SET_AWB_STATE,//����AWBʹ��
    SW2A_CMD_SET_WDR_STRENGHT,//����wdrǿ��
    SW2A_CMD_SET_COLOR_ENABLE,//����ͼ����ǿʹ��
    SW2A_CMD_SET_SATURATION,//���ñ��Ͷ�
    SW2A_CMD_SET_CONTRAST,//���öԱȶ�
    SW2A_CMD_SET_TRAFFIC_LIGHT_ENHANCE_REGION,//���ú�ƼӺ�����
    SW2A_CMD_SET_TRAFFIC_LIGHT_LUM_TH,//���ú�ƼӺ���ֵ
    SW2A_CMD_SET_GAMMA_ENABLE,//����gammaʹ��
    SW2A_CMD_SET_GAMMA,//����gammaֵ
    SW2A_CMD_SET_AUTO_DC_STATE,//ʹ���Զ�dc��Ȧ
    SW2A_CMD_SET_AC_SYNC_STATE,//����ͬ��ʹ��
    SW2A_CMD_SET_AC_SYNC_PARAM,//����ͬ����ʱ
    SW2A_CMD_SET_IO_ARG,//ץ�Ĳ���
    SW2A_CMD_SET_EDGE_MOD,//������
    SW2A_CMD_SET_GRAY_IMAGE_STATE,//�ڰ�ͼ
    SW2A_CMD_SET_WORK_MODE,//����ģʽ
    SW2A_CMD_SET_SOFT_CAPTURE,//����ץ��
    SW2A_CMD_SET_FLASH_GATE,//���˻�����
    SW2A_CMD_SET_CAPT_SHUTTER,//ץ�Ŀ���
    SW2A_CMD_SET_CAPT_GAIN,//ץ������
    SW2A_CMD_SET_CAPT_SHARPEN,//ץ�����
    SW2A_CMD_SET_CAPT_AWB_ENABLE,//ץ���Զ���ƽ��ʹ��
    SW2A_CMD_SET_CAPT_RGB,//ץ��RGB����
    SW2A_CMD_SET_BACK_LIGHT,//����
    SW2A_CMD_SET_AWB_MODE,//��ƽ��ģʽ
    SW2A_CMD_SET_TNF_STATUS,//ʱ���뿪��
    SW2A_CMD_SET_SNF_STATUS,//�����뿪��
	SW2A_CMD_SET_EDGE_ENHANCE,//��Ե��ǿ
	SW2A_CMD_SET_NIGHT_DELAY,//��������״̬�ӳ�ʱ�䣬816������Ч
	SW2A_CMD_SET_REG,//����fpga�Ĵ���-------[31 - 20]:addr,[19 - 0]:value
    //Get
    SW2A_CMD_GET_SHUTTER,
    SW2A_CMD_GET_GAIN,
    SW2A_CMD_GET_RGB_GAIN,
    SW2A_CMD_GET_AE_STATE,
    SW2A_CMD_GET_AE_THRESHOLD,
    SW2A_CMD_GET_AE_EXP_RANGE,
    SW2A_CMD_GET_AE_ZONE,
    SW2A_CMD_GET_AE_GAIN_RANGE,
    SW2A_CMD_GET_AWB_STATE,
    SW2A_CMD_GET_WDR_STRENGHT,
    SW2A_CMD_GET_COLOR_ENABLE,
    SW2A_CMD_GET_SATURATION,
    SW2A_CMD_GET_CONTRAST,
    SW2A_CMD_GET_GAMMA_ENABLE,
    SW2A_CMD_GET_GAMMA,
    SW2A_CMD_GET_AUTO_DC_STATE,
    SW2A_CMD_GET_AC_SYNC_STATE,
    SW2A_CMD_GET_AC_SYNC_PARAM,
    SW2A_CMD_GET_IO_ARG,
    SW2A_CMD_GET_EDGE_MOD,
    SW2A_CMD_GET_GRAY_IMAGE_STATE,
    SW2A_CMD_GET_DAY_LIGHT,
    SW2A_CMD_GET_EDGE_ENHANCE,
	SW2A_CMD_GET_REG,//��ȡfpga�Ĵ���-------[31 - 20]:addr(����),[31 - 0]:value(����)
    SW2A_CMD_END
}SW_2A_CMD;
/* ===================================================================
 *  @func     SW_2A_Get_Version                                               
 *                                                                    
 *  @desc     Function does the following                             
 *                                                                    
 *  @modif    This function modifies the following structures         
 *                                                                    
 *  @inputs   This function takes the following inputs                
 *            <argument name>                                         
 *            Description of usage                                    
 *            <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @outputs  <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @return   Return value of this function if any                    
 *  ==================================================================
 */                                                                   
char * SW_2A_Get_Version();

/* ===================================================================
 *  @func     SW_2A_Init                                               
 *                                                                    
 *  @desc     Function does the following                             
 *                                                                    
 *  @modif    This function modifies the following structures         
 *                                                                    
 *  @inputs   This function takes the following inputs                
 *            <argument name>                                         
 *            Description of usage                                    
 *            <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @outputs  <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @return   Return value of this function if any                    
 *  ==================================================================
 */                                                                   
int SW_2A_Init(SW_2A_Platform_Operator Operator);

/* ===================================================================
 *  @func     SW_2A_Ctrl                                               
 *                                                                    
 *  @desc     Function does the following                             
 *                                                                    
 *  @modif    This function modifies the following structures         
 *                                                                    
 *  @inputs   This function takes the following inputs                
 *            <argument name>                                         
 *            Description of usage                                    
 *            <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @outputs  <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @return   Return value of this function if any                    
 *  ==================================================================
 */                                                                   
int SW_2A_Ctrl(unsigned cmd, void *data,int len);

/* ===================================================================
 *  @func     SW_2A_Process                                               
 *                                                                    
 *  @desc     Function does the following                             
 *                                                                    
 *  @modif    This function modifies the following structures         
 *                                                                    
 *  @inputs   This function takes the following inputs                
 *            <argument name>                                         
 *            Description of usage                                    
 *            <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @outputs  <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @return   Return value of this function if any                    
 *  ==================================================================
 */                                                                   
int SW_2A_Process(void *pH3aMem);

/* ===================================================================
 *  @func     SW_2A_Release                                               
 *                                                                    
 *  @desc     Function does the following                             
 *                                                                    
 *  @modif    This function modifies the following structures         
 *                                                                    
 *  @inputs   This function takes the following inputs                
 *            <argument name>                                         
 *            Description of usage                                    
 *            <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @outputs  <argument name>                                         
 *            Description of usage                                    
 *                                                                    
 *  @return   Return value of this function if any                    
 *  ==================================================================
 */                                                                   
int SW_2A_Release(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // _SW_ALG_AEWB_H_
