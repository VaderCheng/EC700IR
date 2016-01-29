/// @file
/// @brief ����ֵ����
/// @author liaoy
/// @date 2013/8/26 10:46:40
///
/// �޸�˵��:
/// [2013/8/26 10:46:40 liaoy] ����汾

#pragma once

/// �ɹ��жϺ�
#define SV_SUCCEEDED(r) ((r)>=0)

/// ʧ���жϺ�
#define SV_FAILED(r) ((r)<0)

namespace sv
{
    /// ����ֵ����
    enum SV_RESULT
    {
        RS_S_OK = (int)0x00000000,              ///< �ɹ�����ȷ
        RS_S_FALSE = (int)0x00000001,           ///< �ɹ�������ȷ
        RS_E_FAIL = (int)0x80004005,            ///< ʧ��
        RS_E_NOTIMPL = (int)0x80004001,         ///< ʧ��: δʵ��
        RS_E_OUTOFMEMORY = (int)0x8007000E,     ///< ʧ��: �ڴ治��
        RS_E_INVALIDARG = (int)0x80070057,      ///< ʧ��: ��������
        RS_E_OBJNOINIT = (int)0x8000000A,       ///< ʧ��: δ��ʼ��
        RS_E_UNEXPECTED = (int)0x8000FFFF       ///< ʧ��: �����쳣
    };

} // sv

