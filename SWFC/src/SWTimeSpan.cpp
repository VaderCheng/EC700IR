///////////////////////////////////////////////////////////
// CSWTimeSpan.cpp
// Implementation of the Class CSWTimeSpan
// Created on:   28-����-2013 14:09:52
// Original author: zhouy
///////////////////////////////////////////////////////////

#include "SWTimeSpan.h"

/**
 * @brief ���캯��
 */
CSWTimeSpan::CSWTimeSpan(){
	//swpa_memset(&m_cDateTime, 0, sizeof(m_cDateTime));
	swpa_memset(&m_cTime, 0, sizeof(m_cTime));
}

/**
 * @brief ��������
 */
CSWTimeSpan::~CSWTimeSpan(){

}

/**
 * @brief ʱ���ʽת��
 * 
 * @param [in] szFmt : ʱ��ת���ĸ�ʽ���ַ���
 * @return
 * - ת�����ʱ��CSWString��
 */
CSWString CSWTimeSpan::Format(char* szFmt){

    //todo:
    CSWString t;
	return t;
}

/**
 * @brief ���ش�CSWTimeSpan�����е�����������
 * 
 * @return
 * - ����������
 */
INT CSWTimeSpan::GetDays(){

	return m_cTime.sec / 3600 / 24;
}

/**
 * @brief ���ش�CSWTimeSpan�����е�Сʱ��
 * 
 * @return
 * - ������Сʱ��
 */
INT CSWTimeSpan::GetHours(){

	return m_cTime.sec / 3600;
}

/**
 * @brief ���ش�CSWTimeSpan�����еķ�����
 * 
 * @return
 * - ������
 */
INT CSWTimeSpan::GetMinutes(){

	return m_cTime.sec / 60;
}

/**
 * @brief ���ش�CSWTimeSpan�����е�����
 * 
 * @return
 * - ����
 */
INT CSWTimeSpan::GetSeconds(){


	return m_cTime.sec;
}


/**
 * @brief ���ش�CSWTimeSpan�����еĺ�����
 * 
 * @return
 * - ������
 */
INT CSWTimeSpan::GetMSeconds(){

	INT iMsec = m_cTime.sec * 1000 + m_cTime.msec;
	return iMsec >= 0 ? iMsec : -1;
}


/**
 * @brief ��������CSWTimeSpan֮��ļ�������
 * 
 * @param [in] Span ��ȥ�Ķ���
 * @return
 * - �������������
 */
CSWTimeSpan& CSWTimeSpan::operator-=(CSWTimeSpan& Span){

    if ( m_cTime.sec > Span.m_cTime.sec )
    {
        if ( m_cTime.msec >= Span.m_cTime.msec )
        {
            m_cTime.sec = m_cTime.sec - Span.m_cTime.sec;
            m_cTime.msec = m_cTime.msec - Span.m_cTime.msec;
        }
        else
        {
            m_cTime.sec = m_cTime.sec - 1 - Span.m_cTime.sec;
            m_cTime.msec = m_cTime.msec + 1000 - Span.m_cTime.msec;
        }
    }
    else if ( m_cTime.sec == Span.m_cTime.sec )
    {
        m_cTime.sec = 0;

        if ( m_cTime.msec >= Span.m_cTime.msec )
        {
            m_cTime.msec = m_cTime.msec - Span.m_cTime.msec;
        }
        else
        {
            m_cTime.msec = Span.m_cTime.msec - m_cTime.msec;
        }
    }
    else
    {
        if ( Span.m_cTime.msec >= m_cTime.msec )
        {
            m_cTime.sec = Span.m_cTime.sec - m_cTime.sec;
            m_cTime.msec = Span.m_cTime.msec - m_cTime.msec;
        }
        else
        {
            m_cTime.sec = Span.m_cTime.sec - 1 - m_cTime.sec;
            m_cTime.msec = Span.m_cTime.msec + 1000 - m_cTime.msec;
        }
    }

	return (*this);
}

/**
 * @brief ��������CSWTimeSpan֮��ļӷ�����
 * 
 * @param [in] Span ���ϵĶ���
 * @return
 * - ���Ӷ��������
 */
CSWTimeSpan& CSWTimeSpan::operator+=(CSWTimeSpan& Span){

    m_cTime.sec += Span.m_cTime.sec;
    m_cTime.msec += Span.m_cTime.msec;
    if ( m_cTime.msec >= 1000 )
    {
        m_cTime.sec += 1;
        m_cTime.msec -= 1000;
    }

	return (*this);
}

/**
 * @brief ��������CSWTimeSpan֮��Ĳ���������
 * 
 * @param [in] Span ���������ڱȽϵĶ���
 * @return
 * - TRUE : �ö�����Span�������ߵ�Time Spanֵ�����
 * - FALSE : �ö�����Span�������ߵ�Time Spanֵ���
 */
BOOL CSWTimeSpan::operator!=(CSWTimeSpan& Span){

    if ( m_cTime.sec != Span.m_cTime.sec
        || m_cTime.msec != Span.m_cTime.msec )
    {
        return TRUE;
    }
	return FALSE;
}

/**
 * @brief ��������CSWTimeSpan֮��ĸ�ֵ����
 * 
 * @param [in] Span : ��ֵ�Ķ���
 * @return
 * - ����ֵ���������
 */
CSWTimeSpan& CSWTimeSpan::operator=(CSWTimeSpan Span){

    m_cTime.sec = Span.m_cTime.sec;
    m_cTime.msec = Span.m_cTime.msec;
	return (*this);
}

/**
 * @brief ��������CSWTimeSpan֮��ĵ�������
 * 
 * @param [in] Span �������ڱȽϵĶ���
 * @return
 * - TRUE : �ö�����Span�������ߵ�Time Spanֵ���
 * - FALSE : �ö�����Span�������ߵ�Time Spanֵ�����
 */
BOOL CSWTimeSpan::operator==(CSWTimeSpan& Span){

	if ( m_cTime.sec == Span.m_cTime.sec
        && m_cTime.msec == Span.m_cTime.msec )
    {
        return TRUE;
    }
	return FALSE;
}

