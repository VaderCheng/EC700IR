/**
* @file swpa_socket.h
* @brief �����׽���
* @author Shaorg
* @date 2013-02-26
* @version v0.1
* @note ������׼�����׽���socket��һ�麯����
*/

#ifndef _SWPA_SOCKET_H_
#define _SWPA_SOCKET_H_

#include <sys/socket.h>

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#define SWPA_SOCKET_T int
//#define SWPA_SOCKADDR sockaddr_in

// �����׽���
#define SWPA_SOCK_STREAM            SOCK_STREAM // 1
// �������׽���
#define SWPA_SOCK_DGRAM             SOCK_DGRAM // 2

//�����׽���
#define SWPA_AF_UNIX				AF_UNIX // 1
// ����Э��
#define SWPA_AF_INET                AF_INET // 2

//
#define SWPA_EAGAIN                 11

#define SWPA_SOL_SOCKET             SOL_SOCKET // 1


#define SWPA_SO_RCVTIMEO            SO_RCVTIMEO // 20
#define SWPA_SO_SNDTIMEO            SO_SNDTIMEO // 21
#define SWPA_SO_TIMEOUT             SO_TIMEOUT // 10060
#define SWPA_SO_BROADCAST           SO_BROADCAST // 6
#define SWPA_SO_REUSEADDR           SO_REUSEADDR // 2

#define SWPA_SO_SNDBUF				SO_SNDBUF // 7
#define SWPA_SO_RCVBUF				SO_RCVBUF // 8


typedef struct SWPA_SOCKADDR_UN
{
	int sun_family;
	char sun_path[128];
}SWPA_SOCKADDR_UN,*LPSWPA_SOCKADDR_UN;

typedef struct SWPA_SOCKADDR
{
    int port;
    char ip[32];
}SWPA_SOCKADDR,*LPSWPA_SOCKADDR;

typedef struct _SWPA_SOCKET_ATTR_T
{
    int af;
    int type;
    int protocol;
}SWPA_SOCKET_ATTR_T;

typedef struct SWPA_TIMEVAL
{
    long tv_sec;
    long tv_usec;

}SWPA_TIMEVAL;


#ifdef __cplusplus
extern "C"
{
#endif

/**
* @brief �����׽���
* @param [out] skt �׽��ֽṹ��ָ��
* @param [in] attr �׽�������
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_create(SWPA_SOCKET_T* skt, const SWPA_SOCKET_ATTR_T* attr);

/**
* @brief ɾ���׽���
* @param [in] skt �׽��ֽṹ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_delete(SWPA_SOCKET_T skt);

/**
* @brief ���׽���
* @param [in] skt �׽��ֽṹ��
* @param [in] addr Ҫ�󶨵������ַ
* @param [in] addrlen ��ַ�ṹ��ĳ���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_bind(
    SWPA_SOCKET_T skt,
    const struct SWPA_SOCKADDR* addr,
    unsigned int addrlen
);

/**
* @brief �󶨱����׽���
* @param [in] skt �׽��ֽṹ��
* @param [in] addr Ҫ�󶨵ı��ص�ַ
* @param [in] addrlen ��ַ�ṹ��ĳ���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_bind_un(
    SWPA_SOCKET_T skt,
    const struct SWPA_SOCKADDR_UN* addr,
    unsigned int addrlen
);

/**
* @brief �����׽���
* @param [in] skt �׽��ֽṹ��
* @param [in] backlog �׽��ּ����������������󳤶�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_listen(SWPA_SOCKET_T skt, int backlog);

/**
* @brief �����׽���
* @param [in] skt �׽��ֽṹ��
* @param [out] addr �Է������ַ
* @param [out] addrlen ��ַ�ṹ��ĳ���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_accept(
    SWPA_SOCKET_T skt,
    struct SWPA_SOCKADDR* addr,
    unsigned int* addrlen,
    SWPA_SOCKET_T *outSock
);

/**
* @brief �����׽���
* @param [in] skt �׽��ֽṹ��
* @param [out] addr Ŀ�������ַ
* @param [out] addrlen ��ַ�ṹ��ĳ���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_connect(
    SWPA_SOCKET_T skt,
    const struct SWPA_SOCKADDR* addr,
    unsigned int addrlen
);

/**
* @brief ���ӱ����׽���
* @param [in] skt �׽��ֽṹ��
* @param [out] addr Ŀ�걾�ص�ַ
* @param [out] addrlen ��ַ�ṹ��ĳ���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_connect_un(
    SWPA_SOCKET_T skt,
    const struct SWPA_SOCKADDR_UN* addr,
    unsigned int addrlen
);

/**
* @brief ���������ӵ��׽��ַ�������
* @param [in] skt �׽��ֽṹ��
* @param [in] buf ��������
* @param [in] len ���ݳ���
* @param [out] succ_send_len �ѳɹ����͵����ݳ���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_send(
    SWPA_SOCKET_T skt,
    const void* buf,
    unsigned int len,
    unsigned int* succ_send_len
);

/**
* @brief ���������ӵ��׽��ַ�������
* @param [in] skt �׽��ֽṹ��
* @param [in] buf ��������
* @param [in] len ���ݳ���
* @param [out] succ_send_len �ѳɹ����͵����ݳ���
* @param [in] dest_addr Ŀ�ĵ�ַ�ṹ��ָ��
* @param [in] addrlen Ŀ�ĵ�ַ�ṹ�峤��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_sendto(
    SWPA_SOCKET_T skt,
    const void* buf,
    unsigned int len,
    unsigned int* succ_send_len,
    const struct SWPA_SOCKADDR* dest_addr,
    unsigned int addrlen
);

/**
* @brief ���������ӵ��׽��ֽ�������
* @param [in] skt �׽��ֽṹ��
* @param [out] buf ���ݽ��ջ�����
* @param [in,out] len ����buf���������ȣ������ɹ����յ������ݳ��ȡ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_recv(
    SWPA_SOCKET_T skt,
    void* buf,
    unsigned int* len
);

/**
* @brief ���������ӵ��׽��ֽ�������
* @param [in] skt �׽��ֽṹ��
* @param [out] buf ���ݽ��ջ�����
* @param [in,out] len ����buf���������ȣ������ɹ����յ������ݳ��ȡ�
* @param [out] src_addr Դ������ַ�ṹ��ָ��
* @param [out] addrlen Դ������ַ�ṹ�峤��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_recvfrom(
    SWPA_SOCKET_T skt,
    void* buf,
    unsigned int* len,
    struct SWPA_SOCKADDR* src_addr,
    unsigned int* addrlen
);

/**
* @brief �����׽���
* @param [in] skt �׽��ֽṹ��
* @param [in] cmd ������������
* @param [in] ... �����������
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_ioctl(SWPA_SOCKET_T skt, int cmd, void* argv);

/**
* @brief �����׽���
* @param [in] skt �׽��ֽṹ��
* @param [in] level ���õ������
* @param [in] optname ���õ�ѡ��
* @param [in] optval ���������õ�ֵ
* @param [in] optlen Ϊoptval�ĳ���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_opt( SWPA_SOCKET_T skt, int level,int optname
                    ,const void * optval, int optlen );
/**
* @brief �����׽���
* @param [in] skt �׽��ֽṹ��
* @param [in] level ���õ������
* @param [in] optname ���õ�ѡ��
* @param [out] optval ��ѡ����õ�ֵ
* @param [in/out] optlen Ϊoptval�ĳ���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_getopt( SWPA_SOCKET_T skt, int level,int optname
                    ,const void * optval, int *optlen );
/**
* @brief ��ȡ�׽��ֵĶԷ������ַ
* @param [in] skt �׽��ֽṹ��
* @param [out] addr �Է��������ַ
* @param [out] addrlen ��ַ�ṹ��ĳ���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_getpeername(
    SWPA_SOCKET_T skt,
    struct SWPA_SOCKADDR* addr,
    unsigned int* addrlen
);

/**
* @brief ��ȡ�׽��ֵı��������ַ
* @param [in] skt �׽��ֽṹ��
* @param [out] addr �����������ַ
* @param [out] addrlen ��ַ�ṹ��ĳ���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_getsockname(
    SWPA_SOCKET_T skt,
    struct SWPA_SOCKADDR* addr,
    unsigned int* addrlen
);

/**
* @brief �������ַ�ṹ��תΪ���ʮ�����ַ�����ʽ
* @param [in] addr �����ַ�ṹ��
* @param [out] str �ַ��������ַ��������"192.168.255.255"
* @param [in] len ������str�ĳ��ȡ�����Ϊ16�ֽ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_addr2string(const struct SWPA_SOCKADDR* addr, char* str, int len);

/**
* @brief �����ʮ�����ַ�����ʽ�������ַתΪ�ṹ��
* @param [in] str �ַ��������ַ
* @param [out] addr �����ַ�ṹ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_string2addr(const char* str, unsigned short sin_port, struct SWPA_SOCKADDR* addr);

/**
*
* @brief ��ȡ��ô�����
* @retval ���ص�ǰ������
* @see swpa_socket.h
*/

int swpa_socket_get_lastererrocode( );

#ifdef __cplusplus
}
#endif

#endif

