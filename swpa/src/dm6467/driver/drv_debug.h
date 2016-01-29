/**
 * @file   drv_debug.h
 * @author lijj
 * @date   2013-02-28
 *
 * @brief  ���Ժ�
 *
 * @note   �ڰ������ļ�ǰ���붨��_DRV_DEBUG_
 *         1��DRV_PRINT����ӡ�����ڸ��ٴ���ִ������
 *         2��DRV_ERR����ӡ��������ʾ������Ϣ
 *         3��DRV_DEBUG����ѡ��ӡ�����ڵ���
 *
 * @todo
 *         1��Ϊ������ԣ������д�ӡ��Ϣ�м��Ϻ���������ʽʹ��ʱ���Ƿ�Ӧȥ��??
 *
 */
#ifndef DRV_DEBUG_H
#define DRV_DEBUG_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "drv_errno.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// ��ӡ��
//#define DRV_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)

// �����ʾ��ǰ׺
#define DRV_PRINT(fmt, ...) printf("[%s @ %s]: " fmt, __func__, "drvlib", ##__VA_ARGS__)

/// debug������Ϣ����Ҫ����_DRV_DEBUG_��
#ifdef _DRV_DEBUG_
#define DRV_DEBUG(fmt, ...) printf("[DEBUG %s().%d @ %s]: " fmt, __func__, __LINE__, "drvlib", ##__VA_ARGS__)
#else
#define DRV_DEBUG(fmt, ...)
#endif

////////////////////////////////////////////////////////////////////////////

/// ��ӡ������Ϣ
#define DRV_ERR(fmt, ...) printf("[%s().%d @ %s]: " fmt, __func__, __LINE__, "drvlib", ##__VA_ARGS__)

/// ��ӡϵͳ���ô�����Ϣ(ֻ����ϵͳ���ó����ʹ��)
#define DRV_SYSERR(fmt, ...) printf("[%s().%d @ %s | ERR msg: %s]: " fmt, __func__, __LINE__, "drvlib", strerror(errno), ##__VA_ARGS__)

////////////////////////////////////////////////////////////////////////////
/// ��λ����
#ifndef DEBUG_MARK
#define DEBUG_MARK printf("================%s %d\n", __func__, __LINE__);
#endif

///��ӡ����int ������ֵ
#define DRV_PRINT_INT(int_var) 		DRV_PRINT("%s = %d\n", #int_var, int_var)

///��ӡ����unsigned int ������ֵ
#define DRV_PRINT_UINT(uint_var)    DRV_PRINT("%s = %d\n", #uint_var, uint_var)

///��ӡ����long ������ֵ
#define DRV_PRINT_LONG(long_var)    DRV_PRINT("%s = %d\n", #long_var, long_var)

///��ӡ����unsigned long ������ֵ
#define DRV_PRINT_ULONG(ulong_var) 	DRV_PRINT("%s = %d\n", #ulong_var, ulong_var)

///��ӡ����float ������ֵ
#define DRV_PRINT_FLOAT(float_var) 	DRV_PRINT("%s = %f\n", #float_var, float_var)

///��ӡ����char ������ֵ
#define DRV_PRINT_CHAR(char_var) 	DRV_PRINT("%s = %c\n", #char_var, char_var)

///��ӡ����char* ������ֵ
#define DRV_PRINT_STRING(string_var)    DRV_PRINT("%s = %s\n", #string_var, string_var)

///////////////////////////////////////////////////////////
// ����_DRV_CHECK_ARG_�������ʾ����������Ϣ

/// ��ӡ�����Ϸ����ж���Ϣ����Ҫ����_DRV_CHECK_ARG_��
#ifdef _DRV_CHECK_ARG_
#define DRV_ARG_DEBUG(fmt, ...) DRV_PRINT(fmt, ##__VA_ARGS__)
#else
#define DRV_ARG_DEBUG(fmt, ...)
#endif

/// ������
#define DRV_ARG_CHECK(x) \
do {\
if (!(x)) \
{ \
	DRV_ARG_DEBUG("%s().%d Check Arg failed: [%s].\n",__func__, __LINE__, #x);\
    return DRV_INVALIDARG;  \
}\
}while (0)


#ifdef __cplusplus
}
#endif

#endif // DRV_DEBUG_H
