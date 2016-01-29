#pragma once
#include "svTLDetApi/svTLDetApi.h"

namespace svTLDetApi
{
    // ����Э��ṹ��ͷ
    // �������ݽ��ڽṹ��β��������˳���������У�nAvgY, SV_PLATERECT*nPlateROICnt
    struct TL_DET_API_RESULT_HEAD
    {
        int nCheckID;  // �ϼ�ID
        int nPlateROICnt;
        int nDetROICnt; 
    }; 

    // ���õ���������װ��BUF
    sv::SV_RESULT MakeResultBuf(
        int nAvgY,
        SV_PLATERECT* rgPlateData, 
        int nPlateCnt,
        DET_ROI* rgROI,  
        int nDetROICnt, 
        void* pBuf,    // ��Ϊ�գ�Ϊ��ʱ pBufLen ��ֱ�ӷ������ݳ���
        int nMaxBufLen,
        int* pBufLen   // �������ݳ���
        );
    
    // ����װ�������ݲ�ֵ���������
    sv::SV_RESULT SeparateResultBuf(
        void* pBuf,    
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
        void* pBuf,    
        int nBufLen,
        int* pnAvgY,
        SV_PLATERECT** prgPlateROI, 
        int* pPlateCnt,
        DET_ROI** prgROI,  
        int* pDetROICnt
        );
}
