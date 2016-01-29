#ifndef _HVDEVICECOMMDEF_H__
#define _HVDEVICECOMMDEF_H__

/* ͼƬ���� */
#define IMAGE_TYPE_UNKNOWN          0xffff0100	/**< δ֪ */
#define IMAGE_TYPE_JPEG_NORMAL      0xffff0101	/**< Jpeg��ͨͼ */
#define IMAGE_TYPE_JPEG_CAPTURE     0xffff0102	/**< Jpegץ��ͼ */
#define IMAGE_TYPE_JPEG_LPR         0xffff0103	/**< Jpeg�������� */

/* ��Ƶ���� */
#define VIDEO_TYPE_UNKNOWN          0xffff0200	/**< δ֪ */
#define VIDEO_TYPE_H264_NORMAL_I    0xffff0201	/**< H.264ʵʱ��Ƶ��I֡ */
#define VIDEO_TYPE_H264_NORMAL_P    0xffff0202	/**< H.264ʵʱ��Ƶ��P֡ */
#define VIDEO_TYPE_H264_HISTORY_I   0xffff0203	/**< H.264Ӳ��¼����Ƶ��I֡ */
#define VIDEO_TYPE_H264_HISTORY_P   0xffff0205	/**< H.264Ӳ��¼����Ƶ��I֡ */
#define VIDEO_TYPE_JPEG_HISTORY     0xffff0204	/**< JPEGӲ��¼����Ƶ�� */

/* ʶ�������� */
#define RECORD_TYPE_UNKNOWN         0xffff0300	/**< δ֪ */
#define RECORD_TYPE_NORMAL          0xffff0301	/**< ʵʱ��� */
#define RECORD_TYPE_HISTORY         0xffff0302	/**< ��ʷ��� */
#define RECORD_TYPE_STRING          0xffff0303	/**< �¼������Ϣ */
#define RECORD_TYPE_INLEFT          0xffff0304	/**< ���Ƶ����뿪��ʶ */

/* ����״̬ */
#define CONN_STATUS_UNKNOWN         0xffff0400	/**< δ֪ */
#define CONN_STATUS_NORMAL          0xffff0401	/**< ���� */
#define CONN_STATUS_DISCONN         0xffff0402	/**< �Ͽ� */
#define CONN_STATUS_RECONN          0xffff0403	/**< ������ */
#define CONN_STATUS_RECVDONE        0xffff0404	/**< ��ʷ���ݽ������ */

/* �豸���� */
#define DEV_TYPE_UNKNOWN		0x00000000 /**< δ֪�豸 */
#define DEV_TYPE_HVCAM_200W		0x00000001 /**< 200��һ��� */
#define DEV_TYPE_HVCAM_500W		0x00000002 /**< 500��һ��� */
#define DEV_TYPE_HVSIGLE		0x00000004 /**< ����2.0ʶ���� */
#define DEV_TYPE_HVCAM_SINGLE	0x00000008 /**< ����2.0ʶ���� */
#define DEV_TYPE_ALL            0x0000ffff	/**< �����豸 */

#endif

