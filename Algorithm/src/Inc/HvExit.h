// ���ļ����������WINDOWS-936��ʽ
#ifndef _HVEXIT_H_
#define _HVEXIT_H_

// HV_EXIT_CODE�淶��
// ����Ϊ1���ֽڣ�
// ���λ��0��ʾ���س������λ��1��ʾ��λ�豸��
// ��7λ������ʾ�˳�ԭ��

#define HEC_RESET_DEV 0x80 // �����ǣ���λ�豸

#define HEC_SUCC 0x00 // �����˳�
#define HEC_FAIL 0x01 // �쳣�˳�

#ifdef __cplusplus
extern "C" {
#endif

void HvExit(
    int iExitCode,
    const char* szExitInfo,
    const char* szFile,
    int nLine
);

#ifdef __cplusplus
}
#endif

#define HV_Exit(iExitCode, szExitInfo) HvExit(iExitCode, szExitInfo, __FILE__, __LINE__)

#endif

