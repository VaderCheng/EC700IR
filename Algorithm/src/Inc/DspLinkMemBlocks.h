// ���ļ��������ΪWINDOWS-936

#ifndef _DSP_LINK_MEM_BLOCKS_H_
#define _DSP_LINK_MEM_BLOCKS_H_

#include "hvthread.h"
#include "swimage.h"

#define MAX_MEMBLOCKS_COUNT 8  // ���֧�ֵĹ����ڴ��������

typedef struct tagMemBlocksInfo
{
    DWORD32 dwBlockCount;
    DWORD32 dwBlockSize;

    tagMemBlocksInfo()
    {
        dwBlockCount = 0;
        dwBlockSize = 0;
    };
}
MemBlocksInfo;

typedef enum
{
    CAMAPP_FULLPLATFORM_HVCAM = 0,
    CAMAPP_FULLPLATFORM_HVCAM_500W_PIXEL,
    CAMAPP_FULLPLATFORM_OTHER,
    CAMAPP_FULLPLATFORM_OTHER_500W_PIXEL,
    CAMAPP_CAMERAONLY,
    CAMAPP_CAMERAONLY_500W_PIXEL,
    LPRAPP_SINGLE_BOARD_PLATFORM,
    LPRAPP_NORMAL,
    LPRAPP_EDDY,
    LPRAPP_CAP_FACE,
    LPRAPP_CAP_FACE_S,
    LPRAPP_CAP_FACE_500W,
    LPRAPP_EPOLICE_S,
    LPRAPP_EPOLICE_500W,
    LPRAPP_OTHER,
} SHARE_MEM_MODE;  // �����ڴ�ģʽ

/**
* ��DSPLink�����ڴ��ʼ��Ϊָ��ģʽ
*/
HRESULT InitDspLinkMemBlocks(SHARE_MEM_MODE t);

/**
* DSPLink�����ڴ��ʼ��
*/
HRESULT InitMemBlocks(
    MemBlocksInfo* rgcMemBlocksInfo,
    DWORD32 dwCount
);

/**
* DSPLink�����ڴ�����ӿ�
*/
#define CreateShareMemOnPool(pObj, dwLen)\
{\
    CreateShareMemOnPool2(pObj,dwLen);\
    if (NULL == pObj.addr)\
    {\
        HV_Trace(5, "[%s:%d:%s]CreateShareMemOnPool failed\n", __FILE__, __LINE__);\
    }\
}
void CreateShareMemOnPool2(DSPLinkBuffer *pObj, DWORD32 dwLen);

/**
* DSPLink�����ڴ��ͷŽӿ�
*/
void FreeShareMemOnPool(DSPLinkBuffer *pObj);

/**
* DSPLink�����ڴ��״̬��ʾ
*/
void ShareMemPoolStatusShow();

/**
* DSPLink�����ڴ�����ֻ࣬�ṩ�������ͷŹ����ڴ�ʹ��
*/
class CDspLinkMemBlock
{
public:
    CDspLinkMemBlock(DWORD32 dwBlockCount, DWORD32 dwBlockSize);
    ~CDspLinkMemBlock();
    static void Initialize(void);
    void AllocMem(DSPLinkBuffer *pObj);
    void FreeMem(DSPLinkBuffer *pObj);

private:
    DSPLinkBuffer *m_prgMemFrame;
    DWORD32 m_dwBlockCount;
    DWORD32 m_dwBlockSize;
    int m_iMemIndex;
    static bool m_fInitialize;
    friend void ShareMemPoolStatusShow();
};

#endif
