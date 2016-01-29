#ifndef _DSPCMD_API_
#define _DSPCMD_API_

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief ��ʼ��ͼƬʶ�������
* @return 0:�ɹ�; 1:�Ѿ�ִ�иò���; -1:ʧ��
*/
int InitDspCmdLib_Photo(int nPlateRecogParamIndex, void* pvParam);

/**
* @brief ��ʼ����Ƶ��ʶ�������
* @return 0:�ɹ�; 1:�Ѿ�ִ�иò���; -1:ʧ��
*/
int InitDspCmdLib_Video(int nPlateRecogParamIndex, void* pvParam);

/**
* @brief ����DSP����
* @param pszInbuf ��������
* @param nInlen �������ݵĳ���
* @param pszOutbuf �������
* @param nOutlen ������ݵĳ���
* @return ��
*/
void ProcessDspCmd(unsigned char *pbInbuf, unsigned int nInlen, unsigned char *pbOutbuf, unsigned int nOutlen);

#ifdef __cplusplus
}
#endif

#endif

