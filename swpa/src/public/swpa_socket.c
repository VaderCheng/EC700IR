#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>			/* See NOTES */
#include <sys/un.h>


#include "swpa_socket.h"
#include "swpa.h"
#include "swpa_private.h"
#include "string.h"
#include <signal.h>

#ifdef SWPA_SOCKET
#define SWPA_SOCKET_PRINT(fmt, ...) SWPA_PRINT("[%s:%d]"fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define SWPA_SOCKET_CHECK(arg)      {if (!(arg)){SWPA_PRINT("[%s:%d]Check failed : %s [%d]\n", __FILE__, __LINE__, #arg, SWPAR_INVALIDARG);return SWPAR_INVALIDARG;}}
#else
#define SWPA_SOCKET_PRINT(fmt, ...)
#define SWPA_SOCKET_CHECK(arg)
#endif

/**
* @brief �����׽���
* @param [out] skt �׽��ֽṹ��ָ��
* @param [in] attr �׽�������
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_create(SWPA_SOCKET_T* skt, const SWPA_SOCKET_ATTR_T* attr)
{
    sigset_t signal_mask;
    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGPIPE);
    int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);

    SWPA_SOCKET_CHECK(skt != NULL && attr != NULL);
    SWPA_SOCKET_PRINT("skt=0x%08x\n", (unsigned long)skt);
    SWPA_SOCKET_PRINT("attr=0x%08x\n", (unsigned long)attr);
    int nskt = socket(attr->af, attr->type, attr->protocol);
    if(!nskt){
        return SWPAR_FAIL;
    }
    int opt = 1;
    setsockopt(nskt, SOL_SOCKET, SO_REUSEADDR ,&opt, sizeof(&opt));

    *skt = nskt;
    return SWPAR_OK;
}

/**
* @brief ɾ���׽���
* @param [in] skt �׽��ֽṹ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_delete(SWPA_SOCKET_T skt)
{
    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET);
    SWPA_SOCKET_PRINT("skt=0x%08x\n", skt);

    int iRet = shutdown( skt , 2 );
    iRet = close(skt);
    if(iRet != 0) return SWPAR_FAIL;

    return SWPAR_OK;
}

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
)
{
    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET && addr != NULL);
    SWPA_SOCKET_PRINT("skt=%d\n", skt);
    SWPA_SOCKET_PRINT("addr=0x%08x\n", (unsigned long)addr);
    SWPA_SOCKET_PRINT("addrlen=%d\n", addrlen);

    struct sockaddr_in objAddr;

    bzero(&objAddr,sizeof(objAddr));
    objAddr.sin_family=AF_INET;
    objAddr.sin_port=htons(addr->port);
    if( addr->ip[0] == 0 )
        objAddr.sin_addr.s_addr = INADDR_ANY;
    else
        objAddr.sin_addr.s_addr = inet_addr(addr->ip) ;

    addrlen = sizeof(objAddr);


    int iret = bind(skt, (struct sockaddr*)&objAddr, addrlen);

    if(iret != 0)
    {
        return SWPAR_FAIL;
    }

    return SWPAR_OK;
}

int swpa_socket_bind_un(
    SWPA_SOCKET_T skt,
    const struct SWPA_SOCKADDR_UN* addr,
    unsigned int addrlen
)
{
    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET && addr != NULL);
    SWPA_SOCKET_PRINT("skt=%d\n", skt);
    SWPA_SOCKET_PRINT("addr=0x%08x\n", (unsigned long)addr);
    SWPA_SOCKET_PRINT("addrlen=%d\n", addrlen);

    struct sockaddr_un objAddr;

    bzero(&objAddr,sizeof(objAddr));
    objAddr.sun_family= SWPA_AF_UNIX;
    strcpy(objAddr.sun_path, addr->sun_path);
	
    addrlen = strlen(objAddr.sun_path) + sizeof(objAddr.sun_family);

	unlink(objAddr.sun_path);

    int iret = bind(skt, (struct sockaddr*)&objAddr, addrlen);

    if(iret != 0)
    {
        return SWPAR_FAIL;
    }

    return SWPAR_OK;
}

/**
* @brief �����׽���
* @param [in] skt �׽��ֽṹ��
* @param [in] backlog �׽��ּ����������������󳤶�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_listen(SWPA_SOCKET_T skt, int backlog)
{
    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET);
    SWPA_SOCKET_PRINT("skt=%d\n", skt);
    SWPA_SOCKET_PRINT("backlog=%d\n", backlog);
    int iret = listen(skt, backlog);
    if(iret != 0)
    {
        return SWPAR_FAIL;
    }
    return SWPAR_OK;
}

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
)
{
    if( addr == NULL ) return SWPAR_FAIL;

    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET && addr != NULL && addrlen != NULL && outSock != NULL);
    SWPA_SOCKET_PRINT("skt=%d\n", skt);
    SWPA_SOCKET_PRINT("addr=0x%08x\n", (unsigned int)addr);
    SWPA_SOCKET_PRINT("addrlen=0x%08x\n", (unsigned int)addrlen);
    SWPA_SOCKET_PRINT("outSock=0x%08x\n", (unsigned int)outSock);

    struct sockaddr_in objaddr;
    bzero(& objaddr,sizeof( objaddr ));
    *addrlen = sizeof(objaddr);

    int nResult = accept(skt,  (struct sockaddr*)&objaddr, (socklen_t*)addrlen);

    if( nResult == -1 )
    {
        return SWPAR_FAIL;
    }

    if( outSock != NULL ) *outSock = nResult;

    return SWPAR_OK;
}

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
)
{
    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET && addr != NULL);
    SWPA_SOCKET_PRINT("skt=%d\n", skt);
    SWPA_SOCKET_PRINT("addr=0x%08x\n", (unsigned int)addr);
    SWPA_SOCKET_PRINT("addrlen=%d\n", addrlen);

    struct sockaddr_in objaddr;

    bzero(& objaddr,sizeof( objaddr));
    objaddr.sin_family=AF_INET;
    objaddr.sin_port=htons(addr->port);
    objaddr.sin_addr.s_addr = inet_addr(addr->ip);
    addrlen = sizeof(  objaddr );

    int iret = connect(skt, (struct sockaddr*)&objaddr, addrlen);
    if(iret != 0)
    {
    	  SWPA_SOCKET_PRINT("connect failed,%s\n", strerror(errno));
        return SWPAR_FAIL;
    }
    return SWPAR_OK;
}

int swpa_socket_connect_un(
    SWPA_SOCKET_T skt,
    const struct SWPA_SOCKADDR_UN* addr,
    unsigned int addrlen
)
{
    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET && addr != NULL);
    SWPA_SOCKET_PRINT("skt=%d\n", skt);
    SWPA_SOCKET_PRINT("addr=0x%08x\n", (unsigned int)addr);
    SWPA_SOCKET_PRINT("addrlen=%d\n", addrlen);

    struct sockaddr_un objaddr;

    bzero(& objaddr,sizeof( objaddr));
    objaddr.sun_family = SWPA_AF_UNIX;
	strcpy(objaddr.sun_path, addr->sun_path);
	
    addrlen = strlen(objaddr.sun_path) + sizeof(objaddr.sun_family);;

    int iret = connect(skt, (struct sockaddr*)&objaddr, addrlen);
    if(iret != 0)
    {
    	  SWPA_SOCKET_PRINT("connect failed,%s\n", strerror(errno));
        return SWPAR_FAIL;
    }
    return SWPAR_OK;
}

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
)
{
    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET && buf != NULL
    && len > 0);
    SWPA_SOCKET_PRINT("skt=%d\n", skt);
    SWPA_SOCKET_PRINT("buf=0x%08x\n", (unsigned int)buf);
    SWPA_SOCKET_PRINT("len=%d\n", len);
    SWPA_SOCKET_PRINT("succ_send_len=0x%08x\n", (unsigned int)succ_send_len);

    int length = send(skt, buf, len, MSG_NOSIGNAL);

    if( succ_send_len != NULL )  *succ_send_len = length;

    if( length < 0){
         return SWPAR_FAIL;
    }

    return SWPAR_OK;
}

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
)
{
    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET && buf != NULL
    && len > 0 && succ_send_len != NULL
    && dest_addr != NULL && addrlen > 0);
    SWPA_SOCKET_PRINT("skt=%d\n", skt);
    SWPA_SOCKET_PRINT("buf=0x%08x\n", (unsigned int)buf);
    SWPA_SOCKET_PRINT("len=%d\n", len);
    SWPA_SOCKET_PRINT("succ_send_len=0x%08x\n", (unsigned int)succ_send_len);
    SWPA_SOCKET_PRINT("dest_addr=0x%08x\n", (unsigned int)dest_addr);
    SWPA_SOCKET_PRINT("addrlen=%d\n", addrlen);

    struct sockaddr_in objaddr;

    bzero(& objaddr,sizeof( objaddr));
    objaddr.sin_family=AF_INET;
    objaddr.sin_port=htons(dest_addr->port);
    objaddr.sin_addr.s_addr = inet_addr(dest_addr->ip);
    addrlen = sizeof(  objaddr );

    int length = sendto(skt, buf, len, 0,  (struct sockaddr*)&objaddr, addrlen);
    if( succ_send_len != NULL ) *succ_send_len = length ;

    if( length < 0){
        return SWPAR_FAIL;
    }

    return SWPAR_OK;
}

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
)
{
    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET && buf != NULL);
    SWPA_SOCKET_PRINT("skt=%d\n", skt);
    SWPA_SOCKET_PRINT("buf=0x%08x\n", (unsigned int)buf);
    SWPA_SOCKET_PRINT("len=0x%08x\n", (unsigned int)len);

    int length = recv(skt, buf, *len, 0);
    if( len != NULL ) *len = length;

    if(length <= 0){
        return SWPAR_FAIL;
    }



    return SWPAR_OK;
}

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
)
{
    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET && buf != NULL
    && len != NULL && *len > 0 && src_addr != NULL
    && addrlen != NULL && *addrlen > 0);


    SWPA_SOCKET_PRINT("skt=%d\n", skt);
    SWPA_SOCKET_PRINT("buf=0x%08x\n", (unsigned int)buf);
    SWPA_SOCKET_PRINT("len=0x%08x\n", (unsigned int)len);
    SWPA_SOCKET_PRINT("src_addr=0x%08x\n", (unsigned int)src_addr);
    SWPA_SOCKET_PRINT("addrlen=0x%08x\n", (unsigned int)addrlen);

    struct sockaddr_in dest_addr;

    bzero(&dest_addr,sizeof(dest_addr));
    /*dest_addr.sin_family=AF_INET;
    dest_addr.sin_port=htons(src_addr->port);
    dest_addr.sin_addr.s_addr = htonl(inet_addr(src_addr->ip)) ;*/
    *addrlen = sizeof( dest_addr );

    int length = recvfrom(skt, buf, *len, 0, (struct sockaddr*)&dest_addr, (socklen_t*)addrlen);
    if( len != NULL ) *len = length;

    if(length < 0){
		return SWPAR_FAIL;
    }

    swpa_strcpy(src_addr->ip, inet_ntoa(dest_addr.sin_addr));
    src_addr->port =  ntohs(dest_addr.sin_port);

    return SWPAR_OK;
}

/**
* @brief �����׽���
* @param [in] skt �׽��ֽṹ��
* @param [in] cmd ������������
* @param [in] ... �����������
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_ioctl(SWPA_SOCKET_T skt, int cmd,void* argv)
{
    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET && argv != NULL);
    SWPA_SOCKET_PRINT("skt=%d\n", skt);
    SWPA_SOCKET_PRINT("cmd=%d\n", cmd);
    SWPA_SOCKET_PRINT("argv=0x%08x\n", (unsigned int)argv);
    int iret = ioctl(skt, cmd, argv);
    if(iret != 0)
    {
        return SWPAR_FAIL;
    }
    return SWPAR_OK;
}
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
                    ,const void * optval, int optlen )
{
    if(  skt < 0 ) return SWPAR_FAIL;
    int iref = setsockopt( skt , level , optname , (char*)optval , optlen);

    if( iref < 0 ) return SWPAR_FAIL;

    return SWPAR_OK;
}

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
                    ,const void * optval, int *optlen )
{
    if(  skt < 0 ) return SWPAR_FAIL;

    int iref = getsockopt( skt , level , optname , (void*)optval , optlen);

    if( iref != 0 ) return SWPAR_FAIL;

    return SWPAR_OK;
}
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
)
{
    SWPA_SOCKET_CHECK(skt != INVALID_SOCKET && addr != NULL
    && addrlen != NULL && *addrlen > 0);
    SWPA_SOCKET_PRINT("skt=%d\n", skt);
    SWPA_SOCKET_PRINT("addr=0x%08x\n", addr);
    SWPA_SOCKET_PRINT("*addrlen=%d\n", *addrlen);


    struct sockaddr_in dest_addr;
    bzero(&dest_addr,sizeof(dest_addr));
    socklen_t socklen = sizeof( dest_addr );

    int iret = getpeername(skt, (struct sockaddr*)&dest_addr, &socklen);
    if(iret != 0)
    {
        return SWPAR_FAIL;
    }
    swpa_strcpy(addr->ip, inet_ntoa(dest_addr.sin_addr));
    addr->port =  ntohs(dest_addr.sin_port);

    return SWPAR_OK;
}

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
)
{
    struct sockaddr_in dest_addr;
    bzero(&dest_addr,sizeof(dest_addr));
    socklen_t socklen = sizeof( dest_addr );

    int iret = getsockname(skt, (struct sockaddr*)&dest_addr, &socklen);
    if(iret != 0)
    {
        return SWPAR_FAIL;
    }

    swpa_strcpy(addr->ip, inet_ntoa(dest_addr.sin_addr));
    addr->port =  ntohs(dest_addr.sin_port);

    return SWPAR_OK;
}

/**
* @brief �������ַ�ṹ��תΪ���ʮ�����ַ�����ʽ
* @param [in] addr �����ַ�ṹ��
* @param [out] str �ַ��������ַ��������"192.168.255.255"
* @param [in] len ������str�ĳ��ȡ�����Ϊ16�ֽ�
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_addr2string(const struct SWPA_SOCKADDR* addr, char* str, int len)
{
    SWPA_SOCKET_CHECK(addr != NULL
    && str != NULL && len > 0);
    SWPA_SOCKET_PRINT("addr=0x%08x\n", (unsigned int)addr);
    SWPA_SOCKET_PRINT("str=0x%08x\n", (unsigned int)str);
    SWPA_SOCKET_PRINT("len=%d\n", len);
    struct sockaddr_in* addr_in;
    addr_in = (struct sockaddr_in*)addr;
    char szTmpBuf[32] = {0};
    sprintf(szTmpBuf, "%s", (char*)inet_ntoa(addr_in->sin_addr));
    int iStrLen = (int)strlen(szTmpBuf);
    if(iStrLen >= len)
    {
        return SWPAR_OUTOFMEMORY;
    }
    memcpy(str, szTmpBuf, iStrLen);
    *(str + iStrLen) = '\0';

    return SWPAR_OK;
}

/**
* @brief �����ʮ�����ַ�����ʽ�������ַתΪ�ṹ��
* @param [in] str �ַ��������ַ
* @param [out] addr �����ַ�ṹ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_socket.h
*/
int swpa_socket_string2addr(const char* str, unsigned short sin_port, struct SWPA_SOCKADDR* addr)
{
    SWPA_SOCKET_CHECK(str != NULL && addr != NULL);
    SWPA_SOCKET_PRINT("str=%s\n", str);
    SWPA_SOCKET_PRINT("sin_port=%d\n", sin_port);
    SWPA_SOCKET_PRINT("addr=0x%08x\n", (unsigned int)addr);
    struct sockaddr_in* addr_in;
    addr_in = (struct sockaddr_in*)addr;
    memset(addr_in, 0, sizeof(struct sockaddr_in));
    addr_in->sin_addr.s_addr = inet_addr(str);
    addr_in->sin_port = htons(sin_port);
    addr_in->sin_family = AF_INET;
    return SWPAR_OK;
}
/**
*
* @brief ��ȡ��ô�����
* @retval ���ص�ǰ������
* @see swpa_socket.h
*/
int swpa_socket_get_lastererrocode()
{
    return errno;
}

