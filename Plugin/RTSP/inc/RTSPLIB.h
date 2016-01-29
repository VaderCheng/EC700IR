#ifndef RTSPLIB_HH_INCLUDED
#define RTSPLIB_HH_INCLUDED

#include "SWOnvifRtspParameter.h"

#ifdef __cplusplus
extern "C" {
#endif


bool StartRTSP( int* piErr, RTSP_PARAM_INFO *cRtspParamInfo);

bool StopRTSP( int* piErr);

bool SendH264Data(unsigned char *pbData,int iDataSize );


/*
@brief: ����H264����
@note: ����ʽ���޻���
*/
bool SendData(unsigned char* pbData, const int iDataSize);


#ifdef __cplusplus
}
#endif

#endif // RTSPLIB_HH_INCLUDED
