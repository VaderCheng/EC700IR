/// @file
/// @brief �羯ģ����������
/// @author ganzz
/// @date 10/31/2013 15:31:21
/// 
/// �޸�˵��:
/// [10/31/2013 15:31:21 ganzz] ����汾
/// 

#pragma once

//#define BIKE_DET_MODE  // �ǻ��������ģʽ

namespace swTgVvdApp
{

struct TG_DET_MODE_INFO
{
    unsigned char* pData;
    int nDataLen;
};

extern TG_DET_MODE_INFO g_cTgDetMode_DaySmall;
extern TG_DET_MODE_INFO g_cTgDetMode_DayLarge;
extern TG_DET_MODE_INFO g_cTgDetMode_DuskSmall;
extern TG_DET_MODE_INFO g_cTgDetMode_DuskLarge;
extern TG_DET_MODE_INFO g_cTgDetMode_DuskSmallEx;
extern TG_DET_MODE_INFO g_cTgDetMode_DuskLargeEx;
extern TG_DET_MODE_INFO g_cTgDetMode_NightSmall;
extern TG_DET_MODE_INFO g_cTgDetMode_NightLarge;

}