/// @file
/// @brief �㷨��������Ͷ���
/// @author liaoy
/// @date 2013/8/26 10:46:40
///
/// �޸�˵��:
/// [2013/8/26 10:46:40 liaoy] ����汾

#pragma once

#include "sv_config.h"

/// ����ֵ��
#ifndef TRUE
#define TRUE 1
#endif

/// ����ֵ��
#ifndef FALSE
#define FALSE 0
#endif

/// ��ָ��
#ifndef NULL
#define NULL 0
#endif

/// restrict�ؼ���, DSPƽ̨Ĭ����
#if SV_RUN_PLATFORM == SV_PLATFORM_WIN
#define restrict
#elif SV_RUN_PLATFORM == SV_PLATFORM_LINUX
#define restrict __restrict
#endif

namespace sv
{
    /// 8λ����
    typedef signed char          SV_INT8;
    /// 16λ����
    typedef short               SV_INT16;
    /// 32λ����
    typedef int                 SV_INT32;
    /// 64λ����
    typedef long long           SV_INT64;
    /// 8λ�޷�������
    typedef unsigned char       SV_UINT8;
    /// 16λ�޷�������
    typedef unsigned short      SV_UINT16;
    /// 32λ�޷�������
    typedef unsigned int        SV_UINT32;
    /// 64λ�޷�������
    typedef unsigned long long  SV_UINT64;
    /// 32λ�����ȸ���
    typedef float               SV_FLOAT;
    /// 64λ˫���ȸ���
    typedef double              SV_DOUBLE;
    /// ������
    typedef int                 SV_BOOL;
    /// ���ַ���
    typedef unsigned short      SV_WCHAR;

} // sv

