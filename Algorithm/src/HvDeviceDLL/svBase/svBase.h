/// @file
/// @brief svBase����
/// @author liaoy
/// @date 2013/8/26 10:46:40
/// @note ֻ�������ڲ�ʵ��,���ɶ��ⷢ��
///
/// �޸�˵��:
/// [2013/8/26 10:46:40 liaoy] ����汾

#pragma once

#include "sv_basetype.h"
#include "sv_error.h"
#include "sv_memory.h"
#include "sv_math.h"
#include "sv_image.h"
#include "sv_point.h"
#include "sv_rect.h"
#include "sv_utils.h"
#include "sv_callback.h"
#include "sv_dma_cache.h"

/// svBase�����ռ�
namespace sv
{
    /// ��ȡȡ�汾��Ϣ
    const char* GetRevInfo();

    // �����ı�����ص�
    void utSetTraceCallBack_TXT(
        TRACE_CALLBACK_TXT hCallBack
    );

} // sv
