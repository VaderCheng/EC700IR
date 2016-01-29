
#ifndef _FILEQUEUE_TABLE_H_
#define _FILEQUEUE_TABLE_H_

#include "hvutils.h"
#include <deque>


typedef struct _STORAGEINFO
{
    WORD16 wRepeatTimes;
    WORD16 wBlockCnt;
} STORAGEINFO;


typedef struct _RECORDNODE{
    DWORD32 dwYear;
    DWORD32 dwMonth;
    DWORD32 dwDay;
    DWORD32 dwHour;

    //block range in HDD
    DWORD32 dwDataStartID;
    DWORD32 dwDataEndID;

    //file range
    DWORD32 dwFileStartIndex;
    DWORD32 dwFileEndIndex;

    //��¼file��block��Ӧ��ϵ��ѹ����
    //��[ռ�ÿ�����ͬ���ļ�����][ռ�õĿ���]�����Ľṹ����ѹ���洢
    std::deque<STORAGEINFO*> dqInfo;
    DWORD32 dwInfoLen;

}RECORDNODE;


class CFileQueueTable
{
public:
    CFileQueueTable(){}
    virtual ~CFileQueueTable(){};
    virtual HRESULT LoadTable(BYTE8* pbBuf, const DWORD32 dwLen);

    virtual HRESULT SaveTable(BYTE8* pbBuf, const DWORD32 dwLen);

    virtual HRESULT PrintTable();

    virtual HRESULT ReadTable(DWORD32 dwYear, DWORD32 dwMonth, DWORD32 dwDay,
                   DWORD32 dwHour, DWORD32 dwMinute, DWORD32 dwSecond,
                   DWORD32 dwCarID, DWORD32 &dwStartIndex, DWORD32 &dwEndIndex)=0;

    virtual HRESULT WriteTable(DWORD32 dwYear, DWORD32 dwMonth, DWORD32 dwDay,
               DWORD32 dwHour, DWORD32 dwMinute, DWORD32 dwSecond,
               DWORD32 dwSize, DWORD32 &dwCarID, DWORD32 &dwStartIndex, DWORD32 &dwEndIndex)=0;

    virtual DWORD32 GetFileCount(DWORD32 dwYear, DWORD32 dwMonth, DWORD32 dwDay, DWORD32 dwHour)=0;


protected:

    HRESULT Clear();

    DWORD32 m_dwTRICKNUMBER;// = 0xEFDCCDFE;  ��־һ����¼�Ŀ�ʼ

    DWORD32 m_dwTotalBlockCount; // ������
    DWORD32 m_dwOneBlockSize;    //�����С
    DWORD32 m_dwRemained;       //ʣ����ÿ���
    DWORD32 m_dwOffset;         //��һ�����ÿ��ƫ����
    DWORD32 m_dwThreshHold;     //Ԥ����Ŀǰ����

    std::deque<RECORDNODE*> m_dqRecords; //�����¼��˫���
    //DWORD32 m_dwRecordLen;    //�����ɻ��ɵ���deque.size()��ȡ

};


#endif //_FILEQUEUE_TABLE_H_

