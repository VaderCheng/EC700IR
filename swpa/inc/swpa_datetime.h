/**
* @file swpa_datetime.h
* @brief ����ʱ�����
* @copyright Signalway All Rights Reserved
* @author Shaorg
* @date 2013-02-26
* @version v0.1
* @note ������ȡ�Լ���������ʱ�����غ�����
*/

#ifndef _SWPA_DATETIME_H_
#define _SWPA_DATETIME_H_

/**
* @brief ʱ��ṹ��
* @see swpa_datetime.h
*/
typedef struct _SWPA_TIME
{
    unsigned int sec; /**< ��1970-01-01 00:00:00��ʼ��Ŀǰ������������ */
    unsigned int msec; /**< ���µĵ�ǰ������ */
} SWPA_TIME;

typedef struct _SWPA_DATETIME_TM
{
    short msec;
    short year;
    short month;
    short day;
    short hour;
    short min;
    short sec;
    short weeknum;
} SWPA_DATETIME_TM;

#ifdef __cplusplus
extern "C"
{
#endif

/**
* @brief ��ȡϵͳ��ǰ��ʱ�ӵδ���
* 
* @retval tick ʱ�ӵδ�
* @see swpa_datetime.h
*/
unsigned long swpa_datetime_gettick(void);

/**
* @brief ��ȡ��ǰʱ��
*
* @retval ��ǰʱ��
* @see swpa_datetime.h
* @note ��ʱ��Ϊһ����ֵ�����磺8���ʾ+8ʱ����������ʱ�䡣
*/
int swpa_datetime_gettimezone(void);

/**
* @brief ���õ�ǰʱ��
* @param [in] tz ��ǰʱ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_datetime.h
* @note ��ʱ��Ϊһ����ֵ�����磺8���ʾ+8ʱ����������ʱ�䡣
*/
int swpa_datetime_settimezone(int tz);

/**
* @brief ��ȡʱ��
* @param [out] t ʱ��ṹ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_datetime.h
* @note ��ʱ��ṹ�������1970-01-01 00:00:00��ʼ��Ŀǰ�������������Լ����µĵ�ǰ��������ע���뵱ǰ����ʱ����ء�
*/
int swpa_datetime_gettime(struct _SWPA_TIME* t);

/**
* @brief ����ʱ��
* @param [in] t ʱ��ṹ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_datetime.h
* @note ��ʱ��ṹ�������1970-01-01 00:00:00��ʼ��Ŀǰ�������������Լ����µĵ�ǰ��������ע���뵱ǰ����ʱ����ء�
*/
int swpa_datetime_settime(const struct _SWPA_TIME* t);

/**
* @brief ������ʱ��תΪ����ʱ��
* @param [in] t ʱ��ṹ��
* @param [out] tm ����ʱ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_datetime.h
*/
int swpa_datetime_time2tm(struct _SWPA_TIME t, struct _SWPA_DATETIME_TM* tm);

/**
* @brief ������ʱ��תΪ����ʱ��
* @param [in] tm ����ʱ��
* @param [out] t ʱ��ṹ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_datetime.h
*/
int swpa_datetime_tm2time(struct _SWPA_DATETIME_TM tm, struct _SWPA_TIME* t);

#ifdef __cplusplus
}
#endif

#endif

