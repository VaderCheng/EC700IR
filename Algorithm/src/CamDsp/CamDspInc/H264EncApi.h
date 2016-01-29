#ifndef _H264ENCAPI_H_
#define _H264ENCAPI_H_

/*
 * If you are using an evaluation version of this codec, there will be a limit of 
 * encoding up to 54000 frames in the usage of the encoder. 
 */

#include "swBaseType.h"

#include "h264venc.h"
#include "ih264fhdvenc.h"
#include "h264fhdvenc_tii.h"

#ifdef __cplusplus
extern "C" {
#endif

// H.264����������ṹ
typedef struct tagH264EncHandle
{
	H264FHDVENC_Handle            handle;  /* Handle to the Encoder instance       - XDAIS */
	IH264FHDVENC_Fxns             fxns;    /* Function table for the H.264 Encoder - XDAIS */

	H264FHDVENC_InArgs            inargs;  /* Input Parameter to the process call  - XDM */
	H264FHDVENC_OutArgs           outargs; /* Ouput parameters from process call   - XDM */
	IVIDEO1_BufDescIn             inobj;   /* Input Buffer description array       - XDM */
	XDM_BufDesc                   outobj;  /* Output Buffer description array      - XDM */
	H264FHDVENC_Status            status;  /* Stores the status of process call    - XDM */
	H264FHDVENC_Params            params;  /* Instance creation Parameter          - XDM */
	H264FHDVENC_DynamicParams     dynamicparams; /* Dynamic Parameter struct       - XDM */
	IH264FHDVENC_VUIDataStructure vui_params;

	XDAS_Int32                    rgBufSizes[2];
	XDAS_Int8*                    rgBufs[2]; 
} H264EncHandle;

/* alg_malloc.c */
void InitH264OnChipRAM();

/**
* @brief		��H.264������
* @param[out]	pHandle					���صı��������
* @param[in]	dwIntraFrameInterval	I֡���
* @param[in]	dwTargetBitRate			Ŀ�������
* @param[in]	dwFrameRate				����֡��
* @param[in]	dwInputWidth			Դͼ���
* @param[out]	dwInputHeight			Դͼ�߶�
* @param[out]	dwOutputBufSize			�����������С
* @return		�ɹ� !NULL ʧ�� NULL
*/
int H264Enc_Open(
	H264EncHandle* pHandle,
	DWORD32 dwIntraFrameInterval,
	DWORD32 dwTargetBitRate,
	DWORD32 dwFrameRate,
	DWORD32 dwInputWidth,
	DWORD32 dwInputHeight,
	DWORD32 dwOutputBufSize
);

/**
* @brief		����H.264����
* @param[in]	pHandle					���������
* @param[in]	pbSrcYUV422SP_Y			������ͼƬY����
* @param[in]	pbSrcYUV422SP_UV		������ͼƬUV����
* @param[out]	pbH264BitStream			����󻺳���ָ��
* @param[out]	pdwFrameLen				������֡����
* @param[out]	pdwFrameType			������֡���ͣ�I/P��
* @return		�ɹ� 0 ʧ�� -1
*/
int H264Enc_Encode(
	H264EncHandle* pHandle,
	const PBYTE8 pbSrcYUV422SP_Y,
	const PBYTE8 pbSrcYUV422SP_UV,
	PBYTE8 pbH264BitStream,
	PDWORD32 pdwFrameLen,
	PDWORD32 pdwFrameType
);

/**
* @brief		�ر�H.264������
* @param[in]	pHandle					���������
* @return		0
*/
int H264Enc_Close(H264EncHandle* pHandle);

#ifdef __cplusplus
}
#endif

#endif
