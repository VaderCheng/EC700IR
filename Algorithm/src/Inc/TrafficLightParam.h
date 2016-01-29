#ifndef _TRAFFIC_LIGHT_PARAM_
#define _TRAFFIC_LIGHT_PARAM_

typedef struct _tagTrafficLightParam
{
    bool fEnhanceRed;
    int nLightCount;                                //��������
    char rgszLightPos[MAX_TRAFFICLIGHT_COUNT][64];  //������Ϣ

    _tagTrafficLightParam()
    {
        fEnhanceRed = true;
        nLightCount = 0;
        for (int i = 0; i < MAX_TRAFFICLIGHT_COUNT; i++)
        {
            strcpy(rgszLightPos[i], "(0,0,0,0),0,0,00");
        }
    }
}TRAFFIC_LIGHT_PARAM;


#endif
