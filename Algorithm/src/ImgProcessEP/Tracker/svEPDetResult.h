#pragma once
#include "svEPDetApi/svEPDetApi.h"
struct RECTA;  // ǰ���������ϵļ���� 

namespace svEPDetApi
{
    // ����Э��ṹ��ͷ
    // �������ݽ��ڽṹ��β��������˳���������У�DET_ROI*nDetROICnt��RECTA*nPlateROICnt
    struct EP_DET_API_RESULT_HEAD
    {
        int nCheckID;  // �ϼ�ID
        unsigned int nFrameTime;
        int nDetROICnt;
        int nPlateROICnt;
    }; 

    // ���õ���������װ��BUF
    sv::SV_RESULT MakeResultBuf(
        int nFrameTime,
        DET_ROI* rgROI,  
        int nDetROICnt, 
        RECTA* rgRectA, 
        int nPlateCnt,
        void* pBuf,    // ��Ϊ�գ�Ϊ��ʱ pBufLen ��ֱ�ӷ������ݳ���
        int nMaxBufLen,
        int* pBufLen   // �������ݳ���
        );
    
    // ����װ�������ݲ�ֵ���������
    sv::SV_RESULT SeparateResultBuf(
        void* pBuf,    //
        int nBufLen,
        unsigned int* pFrameTime,
        DET_ROI* rgROI, 
        int nMaxROICnt,
        int* pDetROICnt, 
        RECTA* rgPlateROI, 
        int nMaxPlateROICnt,
        int* pPlateCnt
        );
    // ����װ�������ݲ�ֵ�ָ���ϣ����ù�ϵ��ֱ��ʹ��Դ����
    sv::SV_RESULT SeparateResultRefPoint(
        void* pBuf,    //
        int nBufLen,
        unsigned int* pFrameTime,
        DET_ROI** prgROI,  
        int* pDetROICnt, 
        RECTA** prgPlateROI, 
        int* pPlateCnt
        );
}
