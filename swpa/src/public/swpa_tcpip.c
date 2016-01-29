#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#ifdef CROSS_COMPILE
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long long   u64;
typedef unsigned char  u8;
#endif

#include <linux/sockios.h>
#include <linux/ethtool.h>
#include "stdio.h"
#include "swpa.h"
#include "swpa_private.h"
#include "swpa_tcpip.h"
#include "string.h"
#include "drv_device.h"

#ifdef SWPA_TCP
#define SWPA_TCP_PRINT(fmt, ...) SWPA_PRINT("[%s:%d]"fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define SWPA_TCP_CHECK(arg)      {if (!(arg)){SWPA_PRINT("[%s:%d]Check failed : %s [%d]\n", __FILE__, __LINE__, #arg, SWPAR_INVALIDARG);return SWPAR_INVALIDARG;}}
#else
#define SWPA_TCP_PRINT(fmt, ...)
#define SWPA_TCP_CHECK(arg)
#endif

#define BUFSIZE 8192

struct route_info
{
    char ifName[IF_NAMESIZE];
    u_int gateWay;
    u_int srcAddr;
    u_int dstAddr;
};

/**
* @brief ��ȡ������Ϣ
* @param [in] eth ��������������
* @param [out] ip ����IP��ַ������
* @param [in] len_ip ����IP��ַ���������ȡ�����Ϊ16�ֽڡ�
* @param [out] mask �������뻺����
* @param [in] len_mask �������뻺�������ȡ�����Ϊ16�ֽڡ�
* @param [out] gateway���ػ�����
* @param [in] gateway���ػ��������ȡ�����Ϊ16�ֽڡ�
* @param [out] mac ����MAC��ַ������
* @param [in] len_mac ����MAC��ַ���������ȡ�����Ϊ32�ֽڡ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_tcpip.h
*/
int swpa_tcpip_getinfo(
    const char* eth,
    char* ip,
    int len_ip,
    char* mask,
    int len_mask,
    char* gateway,
    int len_gateway,
    char* mac,   
    int len_mac
)
{
    SWPA_TCP_CHECK(eth != NULL && ip != NULL && mask != NULL && mac != NULL
    && len_ip > 0 && len_mask > 0 && len_mac > 0);
    SWPA_TCP_PRINT("eth=%s\n", eth);
    SWPA_TCP_PRINT("ip=0x%08x\n", (unsigned int)ip);
    SWPA_TCP_PRINT("len_ip=%d\n", len_ip);
    SWPA_TCP_PRINT("mask=0x%08x\n", (unsigned int)mask);
    SWPA_TCP_PRINT("len_mask=%d\n", len_mask);
	SWPA_TCP_PRINT("gateway=0x%08x\n", (unsigned int)gateway);
    SWPA_TCP_PRINT("gateway=%d\n", len_gateway);
    SWPA_TCP_PRINT("mac=0x%08x\n", mac);
    SWPA_TCP_PRINT("len_mac=%d\n", len_mac);

    int iret = -1;
    struct ifreq req;
    struct sockaddr_in* host = NULL;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
    {
        return SWPAR_FAIL;
    }
    bzero(&req, sizeof(struct ifreq));
    strcpy(req.ifr_name, eth);
    iret = ioctl(sockfd, SIOCGIFADDR, &req);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    host = (struct sockaddr_in*)&req.ifr_addr;
    strcpy(ip, inet_ntoa(host->sin_addr));

    bzero(&req, sizeof(struct ifreq));
    strcpy(req.ifr_name, eth);
    iret = ioctl(sockfd, SIOCGIFNETMASK, &req);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    host = (struct sockaddr_in*)&req.ifr_addr;
    strcpy(mask, inet_ntoa(host->sin_addr));

#if 0
	bzero(&req, sizeof(struct ifreq));
	strcpy(req.ifr_name, eth);
	iret = ioctl(sockfd, SIOCGIFBRDADDR, &req);
	if(iret < 0)
	{
		close(sockfd);
		return SWPAR_FAIL;
	}
	host = (struct sockaddr_in*)&req.ifr_addr;
	strcpy(gateway, inet_ntoa(host->sin_addr));
#else
	{
		char szIP[255], szMask[255], szGW[255];
		if(SWPAR_OK != swpa_device_read_ipinfo(szIP, 255, szMask, 255, szGW, 255))
		{
			close(sockfd);
			return SWPAR_FAIL;
		}
		else
		{	
			swpa_memset(gateway, 0, len_gateway);
			swpa_strncpy(gateway, szGW, len_gateway-1);
		}
	}
#endif
    bzero(&req, sizeof(struct ifreq));
    strcpy(req.ifr_name, eth);
    iret = ioctl(sockfd, SIOCGIFHWADDR, &req);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
    (unsigned char)req.ifr_hwaddr.sa_data[0],
    (unsigned char)req.ifr_hwaddr.sa_data[1],
    (unsigned char)req.ifr_hwaddr.sa_data[2],
    (unsigned char)req.ifr_hwaddr.sa_data[3],
    (unsigned char)req.ifr_hwaddr.sa_data[4],
    (unsigned char)req.ifr_hwaddr.sa_data[5]);
    close(sockfd);
    return SWPAR_OK;
}

/**
* @brief ����������Ϣ
* @param [in] eth ��������������
* @param [in] ip ����IP��ַ
* @param [in] mask ������������
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_tcpip.h
*/
int swpa_tcpip_setinfo(
    const char* eth,
    const char* ip,
    const char* mask,
    const char* gateway
)
{
    SWPA_TCP_CHECK(eth != NULL && ip != NULL && mask != NULL);
    SWPA_TCP_PRINT("eth=%s\n", eth);
    SWPA_TCP_PRINT("ip=%s\n", ip);
    SWPA_TCP_PRINT("mast=%s\n", mask);
	SWPA_TCP_PRINT("gateway=%s\n", gateway);

	// �����Ч��
	if( SWPAR_OK != swpa_tcpip_checkinfo(ip, mask, gateway) )
	{
		SWPA_TCP_PRINT("Invalid IP info!!\n");
		return SWPAR_INVALIDARG;
	}
#if 0
    int iret = -1;
    struct ifreq req;
    struct sockaddr_in* host = NULL;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
    {
        return SWPAR_FAIL;
    }
    bzero(&req, sizeof(struct ifreq));
    strcpy(req.ifr_name, eth);
    host = (struct sockaddr_in*)&req.ifr_addr;
    host->sin_family = AF_INET;
    if(inet_pton(AF_INET, ip, &(host->sin_addr)) != 1)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    if(ioctl(sockfd, SIOCSIFADDR, &req) < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
	
    bzero(&req, sizeof(struct ifreq));
    strcpy(req.ifr_name, eth);
    host = (struct sockaddr_in*)&req.ifr_addr;
    host->sin_family = AF_INET;
    if(inet_pton(AF_INET, mask, &(host->sin_addr)) != 1)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    if(ioctl(sockfd, SIOCSIFNETMASK, &req) < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
	
	  bzero(&req, sizeof(struct ifreq));
    strcpy(req.ifr_name, eth);
    host = (struct sockaddr_in*)&req.ifr_addr;
    host->sin_family = AF_INET;
    if(inet_pton(AF_INET, gateway, &(host->sin_addr)) != 1)
    {
        close(sockfd);
		perror("3:");
        return SWPAR_FAIL;
    }
    if(ioctl(sockfd, SIOCSIFBRDADDR, &req) < 0)
    {
        close(sockfd);
		perror("4:");
        return SWPAR_FAIL;
    }
#else
	char szCmd[255];
	swpa_sprintf(szCmd, "ifconfig eth0 %s netmask %s", ip, mask);
	swpa_utils_shell(szCmd, NULL);

	{
		char szIP[256]={0}, szMask[256]={0}, szGW[256]={0};
		if(SWPAR_OK == swpa_device_read_ipinfo(szIP, 255, szMask, 255, szGW, 255))
		{
			swpa_sprintf(szCmd, "route del default gw %s", szGW);
			swpa_utils_shell(szCmd, NULL);
		}
	}
	
	swpa_sprintf(szCmd, "route add default gw %s", gateway);
	swpa_utils_shell(szCmd, NULL);
#endif
	char szIP[255], szMask[255], szGW[255];
	if(swpa_device_read_ipinfo(szIP, 255, szMask, 255, szGW, 255) || swpa_strcmp(szIP, ip) || swpa_strcmp(szMask, mask) || swpa_strcmp(szGW, gateway))
	{
		return swpa_device_write_ipinfo(ip, mask, gateway);
	}
    return SWPAR_OK;
}

/**
* @brief ��ȡ������Ϣ
* @param [in] eth ��������������
* @param [out] ip ����IP��ַ������
* @param [in] len_ip ����IP��ַ���������ȡ�����Ϊ16�ֽڡ�
* @param [out] mask �������뻺����
* @param [in] len_mask �������뻺�������ȡ�����Ϊ16�ֽڡ�
* @param [out] gateway���ػ�����
* @param [in] gateway���ػ��������ȡ�����Ϊ16�ֽڡ�
* @param [out] mac ����MAC��ַ������
* @param [in] len_mac ����MAC��ַ���������ȡ�����Ϊ32�ֽڡ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_tcpip.h
*/
int swpa_tcpip_getinfo_with_id(
    const char* eth,
    int id,
    char* ip,
    int len_ip,
    char* mask,
    int len_mask,
    char* gateway,
    int len_gateway,
    char* mac,
    int len_mac
)
{
    SWPA_TCP_CHECK(eth != NULL && ip != NULL && mask != NULL && mac != NULL
    && len_ip > 0 && len_mask > 0 && len_mac > 0);
    SWPA_TCP_PRINT("eth=%s\n", eth);
    SWPA_TCP_PRINT("id=%d", id);
    SWPA_TCP_PRINT("ip=0x%08x\n", (unsigned int)ip);
    SWPA_TCP_PRINT("len_ip=%d\n", len_ip);
    SWPA_TCP_PRINT("mask=0x%08x\n", (unsigned int)mask);
    SWPA_TCP_PRINT("len_mask=%d\n", len_mask);
    SWPA_TCP_PRINT("gateway=0x%08x\n", (unsigned int)gateway);
    SWPA_TCP_PRINT("gateway=%d\n", len_gateway);
    SWPA_TCP_PRINT("mac=0x%08x\n", mac);
    SWPA_TCP_PRINT("len_mac=%d\n", len_mac);

    int iret = -1;
    struct ifreq req;
    char eth_name[256];
    struct sockaddr_in* host = NULL;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
    {
        return SWPAR_FAIL;
    }
    bzero(&req, sizeof(struct ifreq));
    sprintf(eth_name, "%s:%d", eth, id);
    SWPA_TCP_PRINT("eth_name=%s\n", eth_name);
    strcpy(req.ifr_name, eth_name);
    iret = ioctl(sockfd, SIOCGIFADDR, &req);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    host = (struct sockaddr_in*)&req.ifr_addr;
    strcpy(ip, inet_ntoa(host->sin_addr));

    bzero(&req, sizeof(struct ifreq));
    strcpy(req.ifr_name, eth_name);
    iret = ioctl(sockfd, SIOCGIFNETMASK, &req);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    host = (struct sockaddr_in*)&req.ifr_addr;
    strcpy(mask, inet_ntoa(host->sin_addr));

#if 0
    bzero(&req, sizeof(struct ifreq));
    strcpy(req.ifr_name, eth);
    iret = ioctl(sockfd, SIOCGIFBRDADDR, &req);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    host = (struct sockaddr_in*)&req.ifr_addr;
    strcpy(gateway, inet_ntoa(host->sin_addr));
#else
    {
        char szIP[255], szMask[255], szGW[255];
        if(SWPAR_OK != swpa_device_read_ipinfo(szIP, 255, szMask, 255, szGW, 255))
        {
            return SWPAR_FAIL;
        }
        else
        {
            swpa_memset(gateway, 0, len_gateway);
            swpa_strncpy(gateway, szGW, len_gateway-1);
        }
    }
#endif
    bzero(&req, sizeof(struct ifreq));
    strcpy(req.ifr_name, eth_name);
    iret = ioctl(sockfd, SIOCGIFHWADDR, &req);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
    (unsigned char)req.ifr_hwaddr.sa_data[0],
    (unsigned char)req.ifr_hwaddr.sa_data[1],
    (unsigned char)req.ifr_hwaddr.sa_data[2],
    (unsigned char)req.ifr_hwaddr.sa_data[3],
    (unsigned char)req.ifr_hwaddr.sa_data[4],
    (unsigned char)req.ifr_hwaddr.sa_data[5]);
    close(sockfd);
    return SWPAR_OK;
}

/**
* @brief ����������Ϣ
* @param [in] eth ��������������
* @param [in] ip ����IP��ַ
* @param [in] mask ������������
* @param [in] id ��IPͨ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_tcpip.h
*/
int swpa_tcpip_setinfo_with_id(
    const char* eth,
    const int  id,
    const char* ip,
    const char* mask,
    const char* gateway
)
{
    SWPA_TCP_CHECK(eth != NULL && ip != NULL && mask != NULL);
    SWPA_TCP_PRINT("eth=%s\n", eth);
    SWPA_TCP_PRINT("id=%d", id);
    SWPA_TCP_PRINT("ip=%s\n", ip);
    SWPA_TCP_PRINT("mast=%s\n", mask);
    SWPA_TCP_PRINT("gateway=%s\n", gateway);

    // �����Ч��
    if( SWPAR_OK != swpa_tcpip_checkinfo(ip, mask, gateway) )
    {
        SWPA_TCP_PRINT("Invalid IP info!!\n");
        return SWPAR_INVALIDARG;
    }
#if 0
    int iret = -1;
    struct ifreq req;
    struct sockaddr_in* host = NULL;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
    {
        return SWPAR_FAIL;
    }
    bzero(&req, sizeof(struct ifreq));
    strcpy(req.ifr_name, eth);
    host = (struct sockaddr_in*)&req.ifr_addr;
    host->sin_family = AF_INET;
    if(inet_pton(AF_INET, ip, &(host->sin_addr)) != 1)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    if(ioctl(sockfd, SIOCSIFADDR, &req) < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }

    bzero(&req, sizeof(struct ifreq));
    strcpy(req.ifr_name, eth);
    host = (struct sockaddr_in*)&req.ifr_addr;
    host->sin_family = AF_INET;
    if(inet_pton(AF_INET, mask, &(host->sin_addr)) != 1)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    if(ioctl(sockfd, SIOCSIFNETMASK, &req) < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }

      bzero(&req, sizeof(struct ifreq));
    strcpy(req.ifr_name, eth);
    host = (struct sockaddr_in*)&req.ifr_addr;
    host->sin_family = AF_INET;
    if(inet_pton(AF_INET, gateway, &(host->sin_addr)) != 1)
    {
        close(sockfd);
        perror("3:");
        return SWPAR_FAIL;
    }
    if(ioctl(sockfd, SIOCSIFBRDADDR, &req) < 0)
    {
        close(sockfd);
        perror("4:");
        return SWPAR_FAIL;
    }
#else
    char szCmd[255];
    swpa_sprintf(szCmd, "ifconfig eth0:%d %s netmask %s", id, ip, mask);
    swpa_utils_shell(szCmd, NULL);

    {
        char szIP[256]={0}, szMask[256]={0}, szGW[256]={0};
        if(SWPAR_OK == swpa_device_read_ipinfo(szIP, 255, szMask, 255, szGW, 255))
        {
            swpa_sprintf(szCmd, "route del default gw %s", szGW);
            swpa_utils_shell(szCmd, NULL);
        }
    }

    swpa_sprintf(szCmd, "route add default gw %s", gateway);
    swpa_utils_shell(szCmd, NULL);
#endif
    return SWPAR_OK;
}

/*static int ReadNlSock(
    int sockFd,
    char* bufPtr,
    int seqNum,
    int pId
)
{
    struct nlmsghdr* nlHdr = NULL;
    int readLen = 0, msgLen = 0;

    while (1)
    {
        if ( (readLen = recv(sockFd, bufPtr, BUFSIZE - msgLen, 0)) < 0 )
        {
            return -1;
        }

        nlHdr = (struct nlmsghdr *)bufPtr;

        if ( (NLMSG_OK(nlHdr, (unsigned int)readLen) == 0)
                || (nlHdr->nlmsg_type == NLMSG_ERROR) )
        {
            return -1;
        }

        if ( nlHdr->nlmsg_type == NLMSG_DONE )
        {
            break;
        }
        else
        {
            bufPtr += readLen;
            msgLen += readLen;
        }

        if ( (nlHdr->nlmsg_flags & NLM_F_MULTI) == 0 )
        {
            break;
        }

        if ( (nlHdr->nlmsg_seq != (unsigned int)seqNum)
                || (nlHdr->nlmsg_pid != (unsigned int)pId) )
        {
            break;
        }
    }

    return msgLen;
}

static int ParseRoutes(
    struct nlmsghdr* nlHdr,
    struct route_info* rtInfo,
    char* default_gateway
)
{
    int rtLen = 0;
    struct in_addr dst;
    struct in_addr gate;
    struct rtmsg* rtMsg = NULL;
    struct rtattr* rtAttr = NULL;

    rtMsg = (struct rtmsg*)NLMSG_DATA(nlHdr);

    if ( (rtMsg->rtm_family != AF_INET)
            || (rtMsg->rtm_table != RT_TABLE_MAIN) )
    {
        return -1;
    }

    rtAttr = (struct rtattr*)RTM_RTA(rtMsg);
    rtLen = RTM_PAYLOAD(nlHdr);
    for ( ; RTA_OK(rtAttr, rtLen); rtAttr = RTA_NEXT(rtAttr, rtLen) )
    {
        switch (rtAttr->rta_type)
        {
        case RTA_OIF:
            if_indextoname(*(int*)RTA_DATA(rtAttr), rtInfo->ifName);
            break;
        case RTA_GATEWAY:
            rtInfo->gateWay = *(u_int*)RTA_DATA(rtAttr);
            break;
        case RTA_PREFSRC:
            rtInfo->srcAddr = *(u_int*)RTA_DATA(rtAttr);
            break;
        case RTA_DST:
            rtInfo->dstAddr = *(u_int*)RTA_DATA(rtAttr);
            break;
        }
    }
    dst.s_addr = rtInfo->dstAddr;
    if (strstr((char*)inet_ntoa(dst), "0.0.0.0"))
    {
        gate.s_addr = rtInfo->gateWay;
        strcpy(default_gateway, (char*)inet_ntoa(gate));
    }

    return 0;
}*/

/**
* @brief ��ȡ����
* @param [in] eth ��������������
* @param [out] gw ��������IP��ַ������
* @param [in] len ��������IP��ַ����������
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_tcpip.h
*/
/*int swpa_tcpip_getgateway(const char* eth, char* gw, int len)
{
    SWPA_TCP_CHECK(eth != NULL && gw != NULL && len > 0);
    SWPA_PRINT_ULONG(eth);
    SWPA_PRINT_ULONG(gw);
    SWPA_PRINT_ULONG(len);
    static char szGatewayTemp[32] = {0};
    static char msgBuf[BUFSIZE] = {0};
    static struct route_info ri;
    int ret = -1;
    struct nlmsghdr* nlMsg = NULL;
    struct rtmsg* rtMsg = NULL;
    struct route_info* rtInfo = &ri;
    int lenp = 0, msgSeq = 0;
    int sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
    if (sock == -1)
    {
        return SWPAR_FAIL;
    }
    nlMsg = (struct nlmsghdr*)msgBuf;
    rtMsg = (struct rtmsg*)NLMSG_DATA(nlMsg);

    nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
    nlMsg->nlmsg_type = RTM_GETROUTE;
    nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
    nlMsg->nlmsg_seq = msgSeq++;
    nlMsg->nlmsg_pid = getpid();

    if ( send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0 )
    {
        close(sock);
        return SWPAR_FAIL;
    }

    if ( (lenp = ReadNlSock(sock, msgBuf, msgSeq, getpid())) < 0 )
    {
        close(sock);
        return SWPAR_FAIL;
    }

    printf("%s\n", msgBuf);
    if ( rtInfo != NULL )
    {
        for ( ; NLMSG_OK(nlMsg, (unsigned int)lenp); nlMsg = NLMSG_NEXT(nlMsg, lenp) )
        {
            memset(szGatewayTemp, 0, 32);
            memset(rtInfo, 0, sizeof(struct route_info));
            if ( 0 == ParseRoutes(nlMsg, rtInfo, szGatewayTemp) )
            {
                if ( strcmp(rtInfo->ifName, eth) == 0
                        && strcmp(szGatewayTemp, "0.0.0.0") != 0
                        && strlen(szGatewayTemp) > 0 )
                {
                    strcpy(gw, szGatewayTemp);
                    len = strlen(gw);
                    ret = 0;
                }
            }
        }
    }
    return SWPAR_OK;
}*/

/**
* @brief ��������
* @param [in] eth ��������������
* @param [in] gw ��������IP��ַ
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_tcpip.h
*/
int swpa_tcpip_setgateway(const char* eth, const char* gw)
{
    return SWPAR_NOTIMPL;
}

/**
* @brief ��ȡ����MTUֵ
* @param [in] eth ��������������
* @param [out] mtu ����MTUֵ
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_tcpip.h
*/
int swpa_tcpip_getmtu(const char* eth, unsigned int* mtu)
{
    SWPA_TCP_CHECK(eth != NULL && mtu != NULL);
    SWPA_TCP_PRINT("eth=%s\n", eth);
    SWPA_TCP_PRINT("mtu=0x%08x\n", (unsigned int)mtu);
    int iret = -1;
    struct ifreq ifr;
    int sockfd;
    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) <= 0)
    {
        return SWPAR_FAIL;
    }
    bzero(&ifr, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, eth, IFNAMSIZ);
    iret = ioctl(sockfd, SIOCGIFMTU, (void*)&ifr);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    *mtu = ifr.ifr_mtu;
    return SWPAR_OK;
}

/**
* @brief ��������MTUֵ
* @param [in] eth ��������������
* @param [in] mtu ����MTUֵ
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_tcpip.h
*/
int swpa_tcpip_setmtu(const char* eth, unsigned int mtu)
{
    SWPA_TCP_CHECK(eth != NULL);
    SWPA_TCP_PRINT("eth=%s\n", eth);
    SWPA_TCP_PRINT("mtu=%d\n", mtu);
    int iret = -1;
    struct ifreq ifr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
    {
        return SWPAR_FAIL;
    }
    bzero(&ifr, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, eth, IFNAMSIZ);
    ifr.ifr_ifru.ifru_mtu = mtu;
    iret = ioctl(sockfd, SIOCSIFMTU, &ifr);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    return SWPAR_OK;
}

/**
* @brief ʹ������������
* @param [in] eth ��������������
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_tcpip.h
*/
int swpa_tcpip_enable(const char* eth)
{
    SWPA_TCP_CHECK(eth != NULL);
    SWPA_TCP_PRINT("eth=%s\n", eth);
    int iret = -1;
    struct ifreq ifr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
    {
        return SWPAR_FAIL;
    }
    bzero(&ifr, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, eth, IFNAMSIZ);
    iret = ioctl(sockfd, SIOCGIFFLAGS, &ifr);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    ifr.ifr_flags |= IFF_UP;
    iret = ioctl(sockfd, SIOCSIFFLAGS, &ifr);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    return SWPAR_OK;
}

/**
* @brief ��ֹ����������
* @param [in] eth ��������������
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_tcpip.h
*/
int swpa_tcpip_disable(const char* eth)
{
    SWPA_TCP_CHECK(eth != NULL);
    SWPA_TCP_PRINT("eth=%s\n", eth);
    int iret = -1;
    struct ifreq ifr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1)
    {
        return SWPAR_FAIL;
    }
    bzero(&ifr, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, eth, IFNAMSIZ);
    iret = ioctl(sockfd, SIOCGIFFLAGS, &ifr);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    ifr.ifr_flags &= ~IFF_UP;
    iret = ioctl(sockfd, SIOCSIFFLAGS, &ifr);
    if(iret < 0)
    {
        close(sockfd);
        return SWPAR_FAIL;
    }
    return SWPAR_OK;
}

/**
* @brief ��ȡ��������������״̬
* @param [out] linked ����״̬��0���ѶϿ���1�������ӡ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_tcpip.h
*/
int swpa_tcpip_getphystatus(const char* eth, int* linked)
{
    SWPA_TCP_CHECK(eth != NULL && linked != NULL);
    SWPA_TCP_PRINT("eth=%s\n", eth);
    SWPA_TCP_PRINT("linked=0x%08x\n", (unsigned int)linked);
    int iret = -1;
    iret = drv_phy_get_status(DEVID_PHY, linked);
    if(iret < 0)
    {
        return SWPAR_FAIL;
    }
    return SWPAR_OK;
}



/**
* @brief IP��ַ���������롢���غϷ��Լ��
* @param [in] ip_str IP��ַ�ַ���
* @param [in] net_mask_str ���������ַ���
* @param [in] gateway_str �����ַ���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_tcpip.h
*/
int swpa_tcpip_checkinfo(const char *ip_str, const char *net_mask_str, const char *gateway_str)
{
	SWPA_TCP_CHECK(ip_str);
	SWPA_TCP_CHECK(net_mask_str);
	SWPA_TCP_CHECK(gateway_str);

	unsigned char byIp[4]; 
	unsigned char byMask[4];
	unsigned char byGateway[4];

	unsigned char *pbyIp = byIp;
	unsigned char *pbyMask = byMask;
	unsigned char *pbyGateway = byGateway;

	unsigned char byaIpAndMask[4] = {0};
	unsigned char byaGatewayAndMask[4] = {0};
	unsigned char byNum = 0;

	//�����ֽ���
	struct in_addr addr;
	inet_aton(ip_str, &addr);
	memcpy(byIp,&addr.s_addr,sizeof(byIp));

	inet_aton(net_mask_str, &addr);
	memcpy(byMask,&addr.s_addr,sizeof(byIp));

	inet_aton(gateway_str, &addr);
	memcpy(byGateway,&addr.s_addr,sizeof(byIp));

	for ( byNum=0; byNum<4; byNum++ )
	{
		byaIpAndMask[byNum]      = pbyIp[byNum] & pbyMask[byNum];
		byaGatewayAndMask[byNum] = pbyGateway[byNum] & pbyMask[byNum];

		if ( byaIpAndMask[byNum] != byaGatewayAndMask[byNum] )
		{
			SWPA_TCP_PRINT("swpa_tcpip_checkinfo:���ش���,���ر�����IP��ַ��ͬһ����\n");
			return SWPAR_FAIL;
		}
	}

	//����Ϸ��Լ��A\B\C������
#if 0	//todo:��ʱ���������飬���Բ�Ҫ��B���ַ����255.0.0.0����������
	if(pbyIp[0] <= 126)//A��
	{
		if(pbyMask[0] != 0xff)
		{
			SWPA_TCP_PRINT("swpa_tcpip_checkinfo:A���ַ������󣬱���Ϊ255.x.x.x\n");
			return SWPAR_FAIL;
		}

	}else if(pbyIp[0] <= 191)//B��
	{
		if((pbyMask[0] != 0xff) ||(pbyMask[1] != 0xff))
		{
			SWPA_TCP_PRINT("swpa_tcpip_checkinfo:B���ַ�������,����Ϊ255.255.x.x\n");
			return SWPAR_FAIL;
		}

	}else if(pbyIp[0] <= 223)//C��
	{
		if((pbyMask[0] != 0xff)||(pbyMask[1] != 0xff)||(pbyMask[2] != 0xff))
		{
			SWPA_TCP_PRINT("swpa_tcpip_checkinfo:C���ַ�������,����Ϊ255.255.255.x\n");
			return SWPAR_FAIL;
		}
	}else//D��
	{
		SWPA_TCP_PRINT("IP��ַ����ΪD��\n");
		return SWPAR_FAIL;
	}
#endif
	unsigned int unMask;
	unsigned int unIp;
	unsigned int unGateWay;
	memcpy(&unMask,pbyMask,4);
	memcpy(&unIp,pbyIp,4);
	memcpy(&unGateWay,pbyGateway,4);

	if(255 == pbyMask[3])
	{
		SWPA_TCP_PRINT("���������������\n");
		return SWPAR_FAIL;
	}

	//���������Լ��
	unsigned int unMaskTemp = ntohl(unMask);
	unsigned int unRightMask = 0xffffffff;
	int i = 0;
	for(i=0;i<32;i++)//û�п��Ǵ�С������
	{
		if((unMaskTemp & 0x1) == 0x1)
		{
			if(unRightMask != unMaskTemp)
			{
				SWPA_TCP_PRINT("�������,��������111000\n");
				return SWPAR_FAIL;	
			}
			break;
		}
		unMaskTemp = unMaskTemp>>1;
		unRightMask = unRightMask>>1;
	}

	//����ż��
	if(0 == (unMask & unIp))
	{
		SWPA_TCP_PRINT("������Ų���Ϊ��\n");
		return SWPAR_FAIL;
	}

	//�����ż��
	if(0 == ((~unMask)&unIp))//ȫ��
	{
		SWPA_TCP_PRINT("�����Ų���Ϊ��\n");
		return SWPAR_FAIL;
	}

	if(0 == ((~unMask)^((~unMask)&unIp)))//ȫһ
	{
		SWPA_TCP_PRINT("�����Ų���Ϊ�㲥\n");
		return SWPAR_FAIL;
	}

	//IP��ַ��������ͬ
	if(memcmp(pbyIp,pbyGateway,4)==0)
	{
		SWPA_TCP_PRINT("IP��ַ������������ͬ\n");
		return SWPAR_FAIL;
	}

	//����������

	//���������ż��
	if(0 == ((~unMask)&unGateWay))//ȫ��
	{
		SWPA_TCP_PRINT("���ش��������Ų���Ϊ��\n");
		return SWPAR_FAIL;
	}

	if(0 == ((~unMask)^((~unMask)&unGateWay)))//ȫһ
	{
		SWPA_TCP_PRINT("���ش������ز���Ϊ�㲥��ַ\n");
		return SWPAR_FAIL;
	}

	return SWPAR_OK;
}

int swpa_tcpip_ethtool_gset(const char* devname, eth_type* type, int* autoneg)
{
    struct ifreq ifr;
    int fd = 0;
    int err = -1;

    struct ethtool_cmd ecmd;
    struct ethtool_value edata;

    SWPA_TCP_CHECK(devname != NULL && autoneg != NULL);

    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, devname);

    *autoneg = AUTONEG_DISABLE; 

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        //perror("swpa_tcpip_ethtool_gset Cannot get control socket");
        return SWPAR_FAIL;
    }

    ecmd.cmd = ETHTOOL_GSET;
    ifr.ifr_data = (caddr_t)&ecmd;
    err = ioctl(fd, SIOCETHTOOL, &ifr);

    if (err < 0)
    {
        //perror("swpa_tcpip_ethtool_gset Cannot get device settings");
        close(fd);
        return SWPAR_FAIL;
    }

    if (ecmd.autoneg == AUTONEG_ENABLE)
        *autoneg = ecmd.autoneg;

    if (ecmd.speed == SPEED_100 && ecmd.duplex == DUPLEX_FULL)
    {
        *type = ETH_100_FULL;
    }
    else if (ecmd.speed == SPEED_100 && ecmd.duplex == DUPLEX_HALF)
    {
        *type = ETH_100_HALF;
    }
    else if (ecmd.speed == SPEED_1000 && ecmd.duplex == DUPLEX_FULL)
    {
        *type = ETH_1000_FULL;
    }
    else if (ecmd.speed == SPEED_10 && ecmd.duplex == DUPLEX_FULL)
    {
        *type = ETH_10_FULL;
    }
    else if (ecmd.speed == SPEED_10 && ecmd.duplex == DUPLEX_HALF)
    {
        *type = ETH_10_HALF;
    }
    else
    {
        *type = ETH_UNKONW;
    }

    close(fd);

    return SWPAR_OK;
}

int swpa_tcpip_ethtool_sset(const char* devname, eth_type type)
{
    int speed_wanted = -1;
    int duplex_wanted = -1;
    int autoneg_wanted = AUTONEG_ENABLE; // Ĭ��ʹ���Զ�Э��
    int advertising_wanted = -1;

    struct ethtool_cmd ecmd;
    struct ifreq ifr;
    int fd = 0;
    int err = -1;

    SWPA_TCP_CHECK(devname != NULL);

    if (type == ETH_100_FULL)
    {
        speed_wanted = SPEED_100;
        duplex_wanted = DUPLEX_FULL;
    }
    else if (type == ETH_100_HALF)
    {
        speed_wanted = SPEED_100;
        duplex_wanted = DUPLEX_HALF;
    }
    else if (type == ETH_1000_FULL)
    {
        speed_wanted = SPEED_1000;
        duplex_wanted = DUPLEX_FULL;
    }
    else if (type == ETH_10_FULL)
    {
        speed_wanted = SPEED_10;
        duplex_wanted = DUPLEX_FULL;
    }
    else if (type == ETH_10_HALF)
    {
        speed_wanted = SPEED_10;
        duplex_wanted = DUPLEX_HALF;
    }
    else if (type == ETH_AUTONEG)
    {
        autoneg_wanted = AUTONEG_ENABLE;
    }

    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, devname);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        //perror("ethtool_sset Cannot get control socket");
        return SWPAR_FAIL;
    }

    ecmd.cmd = ETHTOOL_GSET;
    ifr.ifr_data = (caddr_t)&ecmd;
    err = ioctl(fd, SIOCETHTOOL, &ifr);
    if (err < 0)
    {
        //perror("Cannot get current device settings");
        close(fd);
        return SWPAR_OK;
    }

    if (speed_wanted != -1)
        ecmd.speed = speed_wanted;
    if (duplex_wanted != -1)
        ecmd.duplex = duplex_wanted;
    if (autoneg_wanted != -1)
        ecmd.autoneg = autoneg_wanted;

    if ((autoneg_wanted == AUTONEG_ENABLE) && (advertising_wanted < 0))
    {
        if (speed_wanted == SPEED_10 && duplex_wanted == DUPLEX_HALF)
            advertising_wanted = ADVERTISED_10baseT_Half;
        else if (speed_wanted == SPEED_10 &&
             duplex_wanted == DUPLEX_FULL)
            advertising_wanted = ADVERTISED_10baseT_Full;
        else if (speed_wanted == SPEED_100 &&
             duplex_wanted == DUPLEX_HALF)
            advertising_wanted = ADVERTISED_100baseT_Half;
        else if (speed_wanted == SPEED_100 &&
             duplex_wanted == DUPLEX_FULL)
            advertising_wanted = ADVERTISED_100baseT_Full;
        else if (speed_wanted == SPEED_1000 &&
             duplex_wanted == DUPLEX_HALF)
            advertising_wanted = ADVERTISED_1000baseT_Half | ADVERTISED_Pause; // ǧ��ʹ����ͣ֡
        else if (speed_wanted == SPEED_1000 &&
             duplex_wanted == DUPLEX_FULL)
            advertising_wanted = ADVERTISED_1000baseT_Full | ADVERTISED_Pause; // ǧ��ʹ����ͣ֡
        else if (speed_wanted == SPEED_2500 &&
             duplex_wanted == DUPLEX_FULL)
            advertising_wanted = ADVERTISED_2500baseX_Full;
        else if (speed_wanted == SPEED_10000 &&
             duplex_wanted == DUPLEX_FULL)
            advertising_wanted = ADVERTISED_10000baseT_Full;
        else
            advertising_wanted = 0;
    }

    if (advertising_wanted != -1)
    {
        // �൱��ethtool eth0 autoneg on�����
        if (advertising_wanted == 0)
            ecmd.advertising = ecmd.supported &
                (ADVERTISED_10baseT_Half |
                 ADVERTISED_10baseT_Full |
                 ADVERTISED_100baseT_Half |
                 ADVERTISED_100baseT_Full |
                 ADVERTISED_1000baseT_Half |
                 ADVERTISED_1000baseT_Full |
                 ADVERTISED_2500baseX_Full |
                 ADVERTISED_10000baseT_Full |
                 ADVERTISED_Pause); // dm8127�ں��Ѿ�������̫��Ӳ�����أ�����������ͣ֡
        else
            ecmd.advertising = advertising_wanted;
    }

    ecmd.cmd = ETHTOOL_SSET;
    ifr.ifr_data = (caddr_t)&ecmd;
    err = ioctl(fd, SIOCETHTOOL, &ifr);
    if (err < 0)
    {
        //perror("Cannot set new settings");
        close(fd);
        return SWPAR_FAIL;
    }
    close(fd);

    return SWPAR_OK;
}
