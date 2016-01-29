#pragma once
#include "swobjbase.h"
#include "swimage.h"
#include "trackerdef.h"
#include "swimageobj.h" 

#include "DspLinkCmd.h"

#include "svBase/svBase.h"
#include "svIPDetApi/svIPDetApi.h"

class CCarDetector
{
public:
    CCarDetector();
    ~CCarDetector();

    HRESULT Init();

    HRESULT DetectCar(
        PROCESS_ONE_FRAME_PARAM* pParam,
        HV_COMPONENT_IMAGE imgFrame,    // ԭ����ͼ
        svIPDetApi::DET_ROI* rgDetRect,             // ���򻺴�
        int nMaxDetRect,                // ��������
        int* pnDetRect                  // ���صļ쵽����
        );

private:
    bool m_fInit;
    svIPDetApi::CDetCtrl* m_pDetCtrl;
    static const int MAX_DET_MODEL_NUM = 2;
    svIPDetApi::DET_PARAM m_rgDetParam[MAX_DET_MODEL_NUM];

    static const int MAX_DET_ROI = 100; 
    svIPDetApi::DET_ROI* m_rgDetRoi[MAX_DET_ROI];
};



