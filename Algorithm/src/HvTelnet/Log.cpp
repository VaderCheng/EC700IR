/*
 * Log.cpp
 *
 *  Created on: May 10, 2011
 *      Author: root
 */

#include "Log.h"

//���ƣ�ֻ��������һ��telnet���ӡ�

#include <stdio.h>
#include <string.h>
#include "hvutils.h"
using namespace std;

#define MAX_LIST_LEN 256

Log::Log()
{
    if ( 0 != pthread_mutex_init(&m_WriteMutex, NULL) )
    {
        HV_Trace(5, "<HvTelnet> pthread_mutex_init is failed!\n");
    }
}

Log::~Log()
{
    pthread_mutex_destroy(&m_WriteMutex);
}

void Log::Lock()
{
    pthread_mutex_lock(&m_WriteMutex);
}

void Log::Unlock()
{
    pthread_mutex_unlock(&m_WriteMutex);
}

// ���ܼ�������strInfo�е�strsrcȫ���滻Ϊstrdst
static void string_replace(string& strInfo, const string& strsrc, const string& strdst)
{
    string::size_type pos=0;
    string::size_type srclen=strsrc.size();
    string::size_type dstlen=strdst.size();
    while ( (pos=strInfo.find(strsrc, pos)) != string::npos)
    {
        strInfo.replace(pos, srclen, strdst);
        pos += dstlen;
    }
}

void Log::Write(int nLevel, char* szData, bool bLock)
{
    Lock();
    if ( m_LogData.size() < MAX_LIST_LEN )
    {
        LogData t;
        t.nLevel = nLevel;
        t.strData = szData;

        // ����Ϣ�ַ����еĻ��з��滻ΪWindows���ģ��Ա����ܹ���Windows��������ʾ��
        string_replace(t.strData, "\n", "\r\n");

        m_LogData.push_back(t);
    }
    Unlock();
}

unsigned int Log::Read(
    IN OUT int* pLevel,
    OUT char* szData,
    IN OUT unsigned int* pPos
)
{
    int iRetLen = 0;

    Lock();
    if ( m_LogData.size() > 0 )
    {
        LogData t = m_LogData.front();
        *pLevel = t.nLevel;
        strcpy(szData, t.strData.c_str());
        m_LogData.pop_front();

        iRetLen = strlen(szData);
    }
    *pPos = 0;
    Unlock();

    return iRetLen;
}
