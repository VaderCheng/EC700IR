/// @file
/// @brief �羯ģ����������
/// @author ganzz
/// @date 10/31/2013 15:31:21
/// 
/// �޸�˵��:
/// [10/31/2013 15:31:21 ganzz] ����汾
/// 

#pragma once

namespace svEPData
{

struct EP_DET_MODEL_DATA
{
    unsigned char* pbData;
    int nDataLen;
};

extern EP_DET_MODEL_DATA g_cEPDetModDaySmall;
extern EP_DET_MODEL_DATA g_cEPDetModDayBig;
extern EP_DET_MODEL_DATA g_cEPDetModNightSmall;
extern EP_DET_MODEL_DATA g_cEPDetModNightBig;

}