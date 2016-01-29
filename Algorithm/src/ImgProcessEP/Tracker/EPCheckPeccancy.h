/// @file
/// @brief �羯Υ��
/// @author ganzz
/// @date 11/6/2013 16:28:40
/// 
/// �޸�˵��:
/// [11/6/2013 16:28:40 ganzz] ����汾
/// 

#pragma once

#include "EPTrackInfo.h"

bool CheckReverseRun(
    CEPTrackInfo* pEPObj
    );

void CheckRunType(
    CEPTrackInfo* pEPObj,
    CRoadInfo* pRoadInfo,
    svEPApi::EP_PARAM* pEPParam
    );
// �����Ƿ�Խ��
bool CheckCrossLine(
    CEPTrackInfo* pEPObj,
    CRoadInfo* pRoadInfo,
    svEPApi::EP_PARAM* pEPParam,
    int* pnRoadNum,
    ACTIONDETECT_PARAM_TRACKER* pParam
    );

// �����Ƿ�ѹ��
bool CheckPressRoadLine(
    CEPTrackInfo* pEPObj,
    CRoadInfo* pRoadInfo,
    float fltOverLineSensitivity,
    int nStopLinePos,  // ��������
    int* pnRoadNum,
    ACTIONDETECT_PARAM_TRACKER* pParam
    );
