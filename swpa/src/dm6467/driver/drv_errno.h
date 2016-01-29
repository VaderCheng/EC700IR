/**
 * @file   drv_errno.h
 * @author lijj
 * @date   2013-02-28
 *
 * @brief  ������API�����붨��
 *
 */
#ifndef DRV_ERRNO_H
#define DRV_ERRNO_H

//����ֵ����
///�ɹ�
#define DRV_OK          ( 0)

///ʧ��
#define DRV_FAIL        (-1)

///�Ƿ�����
#define DRV_INVALIDARG  (-2)

///û��ʵ��
#define DRV_NOTIMPL	    (-3)

///�ڴ治��
#define DRV_OUTOFMEMORY (-4)

///û�г�ʼ��
#define DRV_NOTINITED   (-5)

#define DRV_TIMEOUT     (-6)

#endif // DRV_ERRNO_H
