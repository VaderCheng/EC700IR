/// @file
/// @brief ƽ̨��־����
/// @author liaoy
/// @date 2013/8/26 10:46:40
///
/// �޸�˵��:
/// [2013/8/26 10:46:40 liaoy] ����汾

#pragma once

/// WINDOWSƽ̨
#define SV_PLATFORM_WIN         1

/// DSPƽ̨
#define SV_PLATFORM_DSP         2

/// DSPƽ̨
#define SV_PLATFORM_LINUX       3

#ifdef WIN32
/// ��ǰƽ̨
#define SV_RUN_PLATFORM         SV_PLATFORM_WIN

/// ��ֹDSPƽ̨��#pragma����
#pragma warning(disable : 4068)
#elif defined LINUX
#define SV_RUN_PLATFORM         SV_PLATFORM_LINUX
#else
/// ��ǰƽ̨
#define SV_RUN_PLATFORM         SV_PLATFORM_DSP
#endif
