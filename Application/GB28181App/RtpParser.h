#ifndef _RTPPARSER_H_ZHANGYANJIE
#define _RTPPARSER_H_ZHANGYANJIE

#include "Types.h"
#include <list>

//#include "PS_Packet_Packaging.h"
#include "SWPSMuxer.h"


//namespace NAMEA_SH
//{

/* ֡���Ͷ���*/
enum PackTypeFlag
{
	PACK_TYPE_FRAME_P=0,
	PACK_TYPE_FRAME_I,
	PACK_TYPE_FRAME_B,
	PACK_TYPE_FRAME_AUDIO,
	PACK_TYPE_FRAME_JEPG,
	PACK_TYPE_FRAME_MOTION,
	PACK_TYPE_FRAME_NUM,
	PACK_TYPE_FRAME_NULL=0xff,/*������������*/
};

#define UDP_PACKET_SIZE 2*1024

typedef struct
{
	char buf[UDP_PACKET_SIZE];
	int len;
}PACKET_NODE_T;

typedef std::list<PACKET_NODE_T*> PACKET_BUF_LIST;
class CPSPackaging;

class CRtpParser
{
public:
	CRtpParser();
	virtual ~CRtpParser();

	int SetSize(int w,int h);
	int SetFPS(int nfps);
	//!���ô��ģʽ
	//!flag:1-udp�����0-tcp���
	int SetPackMode(int flag);
    int SetSSrc(int ssrc)
    {
        if(ssrc !=0)
        {
            m_iRtpSsrc = ssrc;
            return 0;
        }
        return -1;
    }
    int GetSSrc()
    {
        return m_iRtpSsrc;
    }
	int clear();
	//!pFrame-һ֡����
	//!len:֡��
	//flag:֡��־
	int AddFrame(const char* pFrame, const int len, const PackTypeFlag ucFrameType);
	//!��ȡ����rtp����Ŀ
	int GetPacketCount();
	//!�ɹ�-���ذ�����
	int GetRtpPacket(char* pOutBuf, int size);
protected:
	void PlaySendFrame(uchar iFrameType, char* buf, int len,char* fui = NULL,char* fuh = NULL, bool bMarker = true);
	/* ý���������*/
	int sendFU_A(int iCh,unsigned int frameType, char* fui,char* fuh,char* data, int len, bool bMarker);

	unsigned short getSequenceNum();
	void setTimestamp(int fps);
private:
	int m_ucFrameRate;
	int m_packet_num;
	//CPSPackaging *m_pPsPackt;
	CSWPSMuxer *m_pPsMuxer;

	int m_pic_width;
	int m_pic_height;
	PACKET_BUF_LIST m_Packet_list;

	int m_rtpModel; //rtp over rtsp 2,other 0
	unsigned short m_usSeq;
	unsigned long m_iTimeStep;
	unsigned long m_iRtpSsrc;
	bool m_bFindFirstIFrame;
};

//}

#endif
