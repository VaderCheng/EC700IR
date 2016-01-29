///////////////////////////////////////////////////////////
// CSWDateTime.cpp
// Implementation of the Class CSWDateTime
// Created on:   28-����-2013 14:09:50
// Original author: zhouy
///////////////////////////////////////////////////////////

#include "SWDateTime.h"
#include "SWLog.h"

/**
 * @brief ���캯��
 * 
 * @param [in] sRealTime : ������ʼ���ö����ʵʱ�ṹ��
 */
CSWDateTime::CSWDateTime(const SWPA_DATETIME_TM* sRealTime){

    swpa_datetime_gettime(&m_cTime);

    if ( sRealTime != NULL )
    {
        SWPA_TIME t;
        if ( 0 == swpa_datetime_tm2time(*sRealTime, &t) )
        {
            m_cTime = t;
        }
    }
}

/**
 * @brief ���캯��
 * 
 * @param [in] sTime : ������ʼ���ö����ʱ��ṹ��
 */
CSWDateTime::CSWDateTime(const SWPA_TIME& sTime){

    m_cTime.sec = sTime.sec;
    m_cTime.msec = sTime.msec;
}

/**
 * @brief ���캯��,�˺���Ϊ���ʱ��ת������ʱ��
 * @param [in] dwTick ���ʱ��
 */
CSWDateTime::CSWDateTime(const DWORD dwTick)
{
	LONG lTick = GetSystemTick() - dwTick;
	
	swpa_datetime_gettime(&m_cTime);
	
	DWORD dwTickMsec = lTick % 1000;
	if( dwTickMsec > m_cTime.msec ) 
	{
		m_cTime.sec -= 1;
		m_cTime.msec = (m_cTime.msec + 1000 - dwTickMsec);
	}
	else
	{
		m_cTime.msec -= dwTickMsec;
	}
	m_cTime.sec -= (lTick/1000);
}

/**
 * @brief ���캯�����õ�ǰϵͳʱ���ʼ���ĺ���
 */
CSWDateTime::CSWDateTime(){

    swpa_datetime_gettime(&m_cTime);
}

/**
 * @brief ��������
 */
CSWDateTime::~CSWDateTime(){

}

/**
 * @brief ʱ���ʽת��
 * 
 * @param [in] szFmt : ʱ��ת���ĸ�ʽ���ַ���
 * @return
 * - ת�����ʱ��CSWString��
 */
CSWString CSWDateTime::Format(const char* szFmt){

    //todo:
    CSWString t;
	return t;
}

/**
 * @brief ��ȡ���󱣴��ʱ��ֵ
 * 
 * @param [out] psTime : ��ȡ����ʱ��
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ��ȡʧ��
 */
HRESULT CSWDateTime::GetTime(SWPA_TIME* psTime){

  *psTime = m_cTime;
	return S_OK;
}

/**
 * @brief ��ȡ���󱣴��ʱ��ֵ
 * 
 * @param [out] psRealTime : ʱ��ṹ��
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ��ȡʧ��
 */
HRESULT CSWDateTime::GetTime(SWPA_DATETIME_TM* psRealTime){

	SWPA_DATETIME_TM tm;
  if ( 0 == swpa_datetime_time2tm(m_cTime, &tm) )
  {
      *psRealTime = tm;
      return S_OK;
  }
	return E_FAIL;
}

/**
* @brief ��ȡ��
*/
INT CSWDateTime::GetYear(){
    
    SWPA_DATETIME_TM sRealTime;
    if ( S_OK == GetTime(&sRealTime) )
    {
        return sRealTime.year;
    }

    return -1;
}

/**
* @brief ��ȡ��
*/
INT CSWDateTime::GetMonth(){

    SWPA_DATETIME_TM sRealTime;
    if ( S_OK == GetTime(&sRealTime) )
    {
        return sRealTime.month;
    }

    return -1;
}

/**
* @brief ��ȡ��
*/
INT CSWDateTime::GetDay(){

    SWPA_DATETIME_TM sRealTime;
    if ( S_OK == GetTime(&sRealTime) )
    {
        return sRealTime.day;
    }

    return -1;
}


/**
* @brief ��ȡ����
*/
INT CSWDateTime::GetWeekday(){

    SWPA_DATETIME_TM sRealTime;
    if ( S_OK == GetTime(&sRealTime) )
    {
        return sRealTime.weeknum;
    }

    return -1;
}


/**
* @brief ��ȡʱ
*/
INT CSWDateTime::GetHour(){

    SWPA_DATETIME_TM sRealTime;
    if ( S_OK == GetTime(&sRealTime) )
    {
        return sRealTime.hour;
    }

    return -1;
}

/**
* @brief ��ȡ��
*/
INT CSWDateTime::GetMinute(){

    SWPA_DATETIME_TM sRealTime;
    if ( S_OK == GetTime(&sRealTime) )
    {
        return sRealTime.min;
    }

    return -1;
}

/**
* @brief ��ȡ��
*/
INT CSWDateTime::GetSecond(){

    SWPA_DATETIME_TM sRealTime;
    if ( S_OK == GetTime(&sRealTime) )
    {
        return sRealTime.sec;
    }

    return -1;
}

/**
* @brief ��ȡ����
*/
INT CSWDateTime::GetMSSecond(){

    SWPA_DATETIME_TM sRealTime;
    if ( S_OK == GetTime(&sRealTime) )
    {
        return sRealTime.msec;
    }

    return -1;
}

/**
 * @brief ���ö����ʱ��
 * 
 * @param [in] sTime : ʱ��ֵ�ṹ��
 * @param [in] fSetSysTime : �Ƿ���Ҫͬ������ϵͳʱ�ӣ�Ĭ��Ϊ��
 * @return
 * - S_OK  : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWDateTime::SetTime(const SWPA_TIME& sTime, const BOOL fSetSysTime){

	m_cTime = sTime;
    if (fSetSysTime)
   	{
		return (0 == swpa_datetime_settime(&sTime)) ? S_OK : E_FAIL;
    }
	return S_OK;
}

/**
 * @brief ���ö����ʱ��
 * 
 * @param [in] sRealTime : ʱ��ṹ��
 * @param [in] fSetSysTime : �Ƿ���Ҫͬ������ϵͳʱ�ӣ�Ĭ��Ϊ��
 * @return
 * - S_OK  : �ɹ�
 * - E_FAIL : ʧ��
 */
HRESULT CSWDateTime::SetTime(const SWPA_DATETIME_TM& sRealTime, const BOOL fSetSysTime){

    SWPA_TIME t;
    if ( 0 == swpa_datetime_tm2time(sRealTime, &t) )
    {
        return SetTime(t, fSetSysTime);
    }
	return E_FAIL;
}



CSWDateTime CSWDateTime::operator+(const CSWDateTime&  time){

    CSWDateTime cDateTimeResult;
    cDateTimeResult = m_cTime;
    cDateTimeResult += time;
    return cDateTimeResult;
}

CSWDateTime CSWDateTime::operator+(const SWPA_TIME& sTime){

    CSWDateTime cDateTimeResult;
    cDateTimeResult = m_cTime;
    cDateTimeResult += sTime;
    return cDateTimeResult;
}

CSWTimeSpan CSWDateTime::operator-(const CSWDateTime&  time){

    CSWTimeSpan cTimeSpanResult;
    CSWTimeSpan cTimeSpan;
    cTimeSpanResult.m_cTime = m_cTime;
    cTimeSpan.m_cTime = time.m_cTime;
    cTimeSpanResult -= cTimeSpan;
	return cTimeSpanResult;
}

CSWTimeSpan CSWDateTime::operator-(const SWPA_TIME& sTime){

	CSWTimeSpan cTimeSpan;
    CSWDateTime cDateTime(sTime);
    cTimeSpan = (*this) - cDateTime;
	return cTimeSpan;
}

CSWDateTime& CSWDateTime::operator=(const SWPA_DATETIME_TM& sRealTime){

    SWPA_TIME t;
    if ( 0 == swpa_datetime_tm2time(sRealTime, &t) )
    {
        m_cTime = t;
    }

	return (*this);
}

CSWDateTime& CSWDateTime::operator=(const SWPA_TIME& sTime){

    m_cTime = sTime;
	return (*this);
}

CSWDateTime& CSWDateTime::operator-=(const CSWTimeSpan&  Span){

    CSWTimeSpan cSpanTmp;
    CSWDateTime cTimeTmp;
    cTimeTmp.m_cTime = Span.m_cTime;
    cSpanTmp = (*this) - cTimeTmp;
    m_cTime = cSpanTmp.m_cTime;
	return (*this);
}


CSWDateTime& CSWDateTime::operator-=(const SWPA_TIME& sTime){

	if (m_cTime.sec * 1000 + m_cTime.msec >= sTime.sec * 1000 + sTime.msec)
	{
		m_cTime.sec -= sTime.sec;
	    m_cTime.msec -= sTime.msec;
	    if ( m_cTime.msec < 0 )
	    {
	        m_cTime.sec -= 1;
	        m_cTime.msec += 1000;
	    }
	}
	else
	{
		SW_TRACE_DEBUG("Err: Time operation overflowed!!\n");
	}
	
	return (*this);
}


CSWDateTime& CSWDateTime::operator+=(const CSWTimeSpan&  Span){

    m_cTime.sec += Span.m_cTime.sec;
    m_cTime.msec += Span.m_cTime.msec;
    if ( m_cTime.msec >= 1000 )
    {
        m_cTime.sec += 1;
        m_cTime.msec -= 1000;
    }
	return (*this);
}

CSWDateTime& CSWDateTime::operator+=(const CSWDateTime&  DateTime){

    m_cTime.sec += DateTime.m_cTime.sec;
    m_cTime.msec += DateTime.m_cTime.msec;
    if ( m_cTime.msec >= 1000 )
    {
        m_cTime.sec += 1;
        m_cTime.msec -= 1000;
    }
	return (*this);
}

CSWDateTime& CSWDateTime::operator+=(const SWPA_TIME& sTime){

    m_cTime.sec += sTime.sec;
    m_cTime.msec += sTime.msec;
    if ( m_cTime.msec >= 1000 )
    {
        m_cTime.sec += 1;
        m_cTime.msec -= 1000;
    }
	return (*this);
}

BOOL CSWDateTime::operator!=(const CSWDateTime&  DateTime){

    if ( m_cTime.sec != DateTime.m_cTime.sec
        || m_cTime.msec != DateTime.m_cTime.msec )
    {
        return TRUE;
    }
	return FALSE;
}

BOOL CSWDateTime::operator<(const CSWDateTime& DateTime){

    if ( m_cTime.sec < DateTime.m_cTime.sec )
    {
        return TRUE;
    }
    else if ( m_cTime.sec == DateTime.m_cTime.sec )
    {
        if ( m_cTime.msec < DateTime.m_cTime.msec )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

BOOL CSWDateTime::operator==(const CSWDateTime&  DateTime){

	if ( m_cTime.sec == DateTime.m_cTime.sec
        && m_cTime.msec == DateTime.m_cTime.msec )
    {
        return TRUE;
    }
	return FALSE;
}

BOOL CSWDateTime::operator>(const CSWDateTime&  DateTime){

	if ( m_cTime.sec > DateTime.m_cTime.sec )
    {
        return TRUE;
    }
    else if ( m_cTime.sec == DateTime.m_cTime.sec )
    {
        if ( m_cTime.msec > DateTime.m_cTime.msec )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

/**
 * @brief ���ö��󱣴��ʱ��
 * 
 * @param [in] dwTimeZone : ʱ��ֵ
 * @return
 * - S_OK : �ɹ�
 * - E_FAIL : ��ȡʧ��
 */
HRESULT CSWDateTime::SetTimeZone(const DWORD dwTimeZone){

	if ( 0 == swpa_datetime_settimezone(dwTimeZone) )
    {
	    return S_OK;
    }
    return E_FAIL;
}

/**
 * @brief ��ȡ���󱣴��ʱ��
 * 
 * @return
 * - ��ȡ����ʱ��ֵ
 */
DWORD CSWDateTime::GetTimeZone(){

	return swpa_datetime_gettimezone();
}

/**
 * @brief ��ȡϵͳʱ��tick��
 * 
 * @return
 * - ϵͳʱ��tick��
 */
DWORD CSWDateTime::GetSystemTick(){

    return swpa_datetime_gettick();
}


HRESULT CSWDateTime::TimeConvert(const DWORD dwRefTime, DWORD* pdwTimeHigh, DWORD* pdwTimeLow)
{
	if (NULL == pdwTimeHigh || NULL == pdwTimeLow)
	{
		//PRINT("Err: NULL == pdwTimeHigh || NULL == pdwTimeLow\n");
		return E_INVALIDARG;
	}

	CSWDateTime cTime(dwRefTime);
	SWPA_TIME sTime;
	cTime.GetTime(&sTime);

	UInt64 ui64Time = sTime.sec;
	ui64Time *= 1000;
	ui64Time += sTime.msec;
	
	*pdwTimeHigh =  (DWORD)(ui64Time >> 32);
	*pdwTimeLow = (DWORD)(( ui64Time << 32 ) >> 32);
	
	return S_OK;
}


HRESULT CSWDateTime::TimeConvert(const SWPA_TIME& sTime, DWORD* pdwTimeHigh, DWORD* pdwTimeLow)
{
	if (NULL == pdwTimeHigh || NULL == pdwTimeLow)
	{
		//PRINT("Err: NULL == pdwTimeHigh || NULL == pdwTimeLow\n");
		return E_INVALIDARG;
	}

	UInt64 ui64Time = (UInt64)sTime.sec * 1000 + (UInt64)sTime.msec;

	
	*pdwTimeHigh =  (DWORD)(ui64Time >> 32);
	*pdwTimeLow = (DWORD)(ui64Time & 0xFFFFFFFF);
	
	return S_OK;
}



HRESULT CSWDateTime::TimeConvert(const DWORD dwTimeHigh,  const DWORD dwTimeLow, SWPA_TIME* psTime)
{
	if (NULL == psTime)
	{
		//PRINT("Err: NULL == pdwTimeHigh || NULL == pdwTimeLow\n");
		return E_INVALIDARG;
	}

	UInt64 ui64Time = ((UInt64)dwTimeHigh)<<32 | ((UInt64)dwTimeLow);

	psTime->sec = ui64Time / 1000;
	psTime->msec = ui64Time % 1000;
	
	return S_OK;
}


