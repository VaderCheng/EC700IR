/// @file
/// @brief ��ѧ��������
/// @author liaoy
/// @date 2013/8/26 10:46:40
///
/// �޸�˵��:
/// [2013/8/26 10:46:40 liaoy] ����汾

#pragma once

#include "sv_basetype.h"

namespace sv
{
    /// ��Сֵ
#define SV_MIN(x,y)     ((x)>(y)?(y):(x))

    /// ���ֵ
#define SV_MAX(x,y)     ((x)>(y)?(x):(y))

    /// ���½�����
#define SV_LIMT(x,l,h)  ((x)<(l)?(l):((x)>(h)?(h):(x)))

    /// ����ֵ
#define SV_ABS(x)       ((x)>0?(x):-(x))

    /// ��ƽ����
    SV_UINT16 mathSqrt(
        SV_UINT32 nRoot     ///< nRoot ��������, 0<=nRoot<=65535*65535=4294836225(0xFFFE0001)
    );

    /// ��16λ������exp(x), xΪ����
    /// @return ָ��ֵ(16λ����)
    /// @note ������� < 2
    SV_UINT32 mathExpPos(
        SV_UINT32 nNum      ///< 16λ������, 0<=x<11 => 0<= nNum=(SV_UINT32)(x*65536)<=726817
    );

    /// ��16λ������exp(x), xΪ����
    /// @return ָ��ֵ(16λ����)
    /// @note ������� < 2
    SV_UINT32 mathExpNeg(
        SV_UINT32 nNum       ///< 16λ������x����ֵ, -16=<x<0 => dwNum=(SV_UINT32)(-x * 65536) <=1048576
    );

} // sv
