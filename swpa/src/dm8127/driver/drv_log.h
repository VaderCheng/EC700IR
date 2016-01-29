/**
 * @file   drv_log.h
 * @author lujy
 * @date   2013-11-12
 *
 * @brief  д��־�ӿ�
 *
 * @note   ���ӿڵ���syslog����־д��ϵͳ��־�ļ���(/var/log/messages��/var/log/syslog)\n
 *         ע�⣺Ϊ�˱�֤ÿһ���ַ���ǰ�������ڵ���Ϣ������һ��drv_logֻ��дһ���ַ���\n
 *
 * 
 */

#ifndef DRV_LOG_H
#define DRV_LOG_H

#include <syslog.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief ��ӡ��־����
 *
 * @param log_src [in]   : ��־Դ
 * @param log_level [in] : ��־�ȼ�
 * @param fmt [in]       : �ɱ����
 * @param args [in]      : �ɱ����������
 *
 * @note ��Ч������ĺ�����
 * void drv_log(const char* log_src, int log_level, const char *fmt, ...)
 */
#define drv_log(log_src, log_level, fmt, args...)  \
{ \
    syslog(LOG_USER|LOG_NOTICE, "| %d | %s | " fmt, log_level, log_src, ##args); \
}

#ifdef __cplusplus
}
#endif

#endif // DRV_LOG_H
