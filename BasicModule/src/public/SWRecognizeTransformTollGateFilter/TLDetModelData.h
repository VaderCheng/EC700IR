/// @file
/// @brief �羯ģ����������
/// @author ganzz
/// @date 10/31/2013 15:31:21
/// 
/// �޸�˵��:
/// [10/31/2013 15:31:21 ganzz] ����汾
/// 

#pragma once

namespace svTLDetData
{

struct TL_DET_MODEL_DATA
{
    unsigned char* pbData;
    int nDataLen;
};

extern TL_DET_MODEL_DATA g_cTLDetModDaySmall;
extern TL_DET_MODEL_DATA g_cTLDetModNightSmall;

}