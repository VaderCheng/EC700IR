/**
* @file FileQueueTable.h 
* @brief CFileQueue is used to save the result/video file mapping table
* 
* @copyright Signalway All Rights Reserved
* @author zhouy
* @date 2013-03-21
* @version 1.0
*/


#ifndef _FILEQUEUE_TABLE_H_
#define _FILEQUEUE_TABLE_H_

#include "SWList.h"
#include "SWFC.h"



typedef struct _STORAGEINFO
{
    WORD wRepeatTimes;
    WORD wBlockCnt;
} STORAGEINFO;


typedef struct _RECORDNODE{
    DWORD dwYear;
    DWORD dwMonth;
    DWORD dwDay;
    DWORD dwHour;

    //block range in HDD
    DWORD dwDataStartID;
    DWORD dwDataEndID;

    //file range
    DWORD dwFileStartIndex;
    DWORD dwFileEndIndex;

    //��¼file��block��Ӧ��ϵ��ѹ����
    //��[ռ�ÿ�����ͬ���ļ�����][ռ�õĿ���]�����Ľṹ����ѹ���洢
    CSWList<STORAGEINFO*, 4096> lstInfo;
    DWORD dwInfoLen;

}RECORDNODE;


class CFileQueueTable
{

public:
	
    CFileQueueTable(){}
    virtual ~CFileQueueTable(){};
    virtual HRESULT LoadTable(BYTE* pbBuf, const DWORD dwLen);

    virtual HRESULT SaveTable(BYTE* pbBuf, const DWORD dwLen);

    virtual HRESULT PrintTable();

    virtual HRESULT ReadTable(DWORD dwYear, DWORD dwMonth, DWORD dwDay,
                   DWORD dwHour, DWORD dwMinute, DWORD dwSecond,
                   DWORD dwCarID, DWORD &dwStartIndex, DWORD &dwEndIndex)=0;

    virtual HRESULT WriteTable(DWORD dwYear, DWORD dwMonth, DWORD dwDay,
               DWORD dwHour, DWORD dwMinute, DWORD dwSecond,
               DWORD dwSize, DWORD &dwCarID, DWORD &dwStartIndex, DWORD &dwEndIndex)=0;

    virtual DWORD GetFileCount(DWORD dwYear, DWORD dwMonth, DWORD dwDay, DWORD dwHour)=0;

	
	DWORD GetTotalCount()
	{
		return m_dwTotalBlockCount;
	}
	
	
	DWORD GetAvailableCount()
	{
		return m_dwRemained;
	}
	
	
	DWORD GetOffset()
	{
		return m_dwOffset;
	}

protected:

    HRESULT Clear();

    DWORD m_dwTRICKNUMBER;// = 0xEFDCCDFE;  ��־һ����¼�Ŀ�ʼ

    DWORD m_dwTotalBlockCount; // ������
    DWORD m_dwOneBlockSize;    //�����С
    DWORD m_dwRemained;       //ʣ����ÿ���
    DWORD m_dwOffset;         //��һ�����ÿ��ƫ����
    DWORD m_dwThreshHold;     //Ԥ����Ŀǰ����

    CSWList<RECORDNODE*, 40960> m_lstRecords; //�����¼��˫���
    //DWORD m_dwRecordLen;    //�����ɻ��ɵ���deque.size()��ȡ

};


#endif //_FILEQUEUE_TABLE_H_


