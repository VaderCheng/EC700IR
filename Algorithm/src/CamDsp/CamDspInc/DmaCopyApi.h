#ifndef _DMACOPYAPI_H_
#define _DMACOPYAPI_H_

typedef void* DMA_HANDLE;

#ifdef __cplusplus
extern "C" {
#endif

// ��ʼ��DMA
void DmaInit();

// ��һ��DMA���
DMA_HANDLE DmaOpen();

// ����DMA��1Dģʽ����
int DmaCopy1D(DMA_HANDLE hDma, unsigned char* pbDst, unsigned char* pbSrc, int iSize);

// ����DMA��2Dģʽ����
int DmaCopy2D(
	DMA_HANDLE hDma, 
	unsigned char* pbDst, 
	int iDstStride, 
	unsigned char* pbSrc, 
	int iSrcStride, 
	int iElementSize, 
	int iElementsNum
);

// �ȴ���������
int DmaWaitFinish(DMA_HANDLE hDma);

// �ر�һ��DMA���
int DmaClose(DMA_HANDLE hDma);

// ж��DMA
void DmaExit();

// ������ǰ����͸����DMA��װģʽ
int HV_dmasetup_dm6467();
int HV_dmawait_dm6467(int iHandle);
int HV_dmacpy1D_dm6467(unsigned char* pbDst, unsigned char* pbSrc, int iSize);
int HV_dmacpy2D_dm6467(unsigned char* pbDst, int iDstStride, unsigned char* pbSrc, int iSrcStride, int iWidth, int iHeight);

// Comment by Shaorg��DMAʹ��ע������
// 1���ڴ����ƣ�Ҫ�������ڴ治����ջ�ϣ�һ�ο�����󳤶�Ϊ64KB������128�ֽڶ��롣
// 2��Cacheһ�������⣺����ʹ��csl_cache.hͷ�ļ��е���غ�����֤Cache��һ���ԡ�
// 3����Ҫ��H264Enc_Open����ִ��֮ǰ��ʼ������DMAͨ��������򿪵�DMAͨ�����в��ֿ�����ʹ��ʱ�����쳣��

#ifdef __cplusplus
}
#endif

#ifndef SAFE_CLOSE_DMA
#define SAFE_CLOSE_DMA(h)						\
	if (h)										\
	{											\
		DmaClose(h);							\
		h = NULL;								\
	}
#endif

#endif
