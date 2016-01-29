/**
 * @file   common_protocol.h
 * @author 
 * @date   Tue Jun 17 09:20:40 2014
 * 
 * @brief  api for serial port device of posix.
 * 
 * 
 */

#ifndef __COMMON_PROTOCOL_H
#define __COMMON_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif


#define _DEBUG

// ģ�⣬��ʵ�ʴ���
//#define __LL_DEBUG

// ����ʱ��ӡ����
//#define __DUMP_CMD

// ��6�����ڲ����Եȼ�
#define DEFAULT_RANK 7

#define com_debug(rank, fmt, ...) \
    do {\
        if (rank <= DEFAULT_RANK) \
        {\
            printf(fmt, ##__VA_ARGS__); \
        }\
    }while(0)

#define com_print(fmt, ...) \
    do {\
        printf(fmt, ##__VA_ARGS__); \
    }while(0)


#define _LOCK(_lock)	\
if (0 !=_lock && 0 != swpa_sem_pend(&_lock, -1))\
{\
	com_print("Err: failed to lock %s! [%d]\n", #_lock, -1);\
	return -1;	\
}\
//printf("[zydebug] lock 0x%x @ %s:L%d\n", _lock, __FUNCTION__, __LINE__);


#define _UNLOCK(_lock)	\
/*printf("[zydebug] unlock 0x%x @ %s:L%d\n", _lock, __FUNCTION__, __LINE__);*/ \
if (0 !=_lock && 0 != swpa_sem_post(&_lock))\
{\
	com_print("Err: failed to unlock %s! [%d]\n", #_lock, -1);\
	return -1;	\
}


#define _CREATE_LOCK(_lock) \
\
if (0 ==_lock && 0 != swpa_sem_create(&_lock, 1, 1))\
{\
	com_print("Err: failed to create lock %s! [%d]\n", #_lock, -1);\
	return -1;	\
}\


#define _DELETE_LOCK(_lock) \
if (0 !=_lock && 0 != swpa_sem_delete(&_lock))\
{\
	com_print("Err: failed to delete lock %s! [%d]\n", #_lock, -1);\
	return -1;	\
}



/// �ɹ�
#define COM_SUCCESS       0
/// ʧ��
#define COM_FAILURE       -1
/// ��ʱ
#define COM_TIMEOUT       -6
/// ����ִ��ʧ��
#define COM_CMDERROR      -3

/* size of the local packet buffer */
#define COM_INPUT_BUFFER_SIZE     32

/// ����Э��ӿ�
typedef struct _COM_interface
{
    /// �����豸����
    const char* device;
    /// �豸��ַ
    uint32_t address;
    /// �㲥��־
    uint32_t broadcast;

    /// �����豸������
    int port_fd;
    /// ���ڽ������ݻ�����
    unsigned char ibuf[COM_INPUT_BUFFER_SIZE];
    /// ���ڽ������ݻ�������С
    uint32_t bytes;
    /// ���ڽ������ݰ�����
    uint32_t type;
} COMInterface_t;

typedef struct _COM_interface *pCOMInterface_t;

/// ������հ��ṹ��
typedef struct _COM_packet
{
    /// �����������
    unsigned char bytes[32];
    /// �������������С
    uint32_t length;
} COMPacket_t;
/** 
 * �򿪴���
 * 
 * @param device_name [in] �����豸����
 * @param baudrate [in] ������
 * @param data_bits [in] ����λ
 * @param parity [in] ��żУ����
 * @param stop_bits [in] ֹͣλ
 * 
 * @return �ɹ��������豸������  ʧ�ܣ�-1
 */
int32_t com_open_serial(const char *device_name, int baudrate, int data_bits, int parity, int stop_bits);
/** 
 * �رմ���
 * 
 * @param fd [in]  �����豸������
 * 
 * @return �ɹ���0  ʧ�ܣ�-1
 */
int32_t com_close_serial(int fd);
/** 
 * ��ȡ���ڻ�����ʣ�������
 * 
 * @param fd [in] �����豸������
 * @param buffer [out] ������
 * @param buffer_size [out] ��������С
 * 
 * @return 
 */
int32_t com_unread_bytes(int fd, unsigned char *buffer, int *buffer_size);
/** 
 * 
 * 
 * @param fd [in] �����豸������
 * @param buffer [in] Ҫ���͵����ݵĻ�����
 * @param buffer_size [in] ��������С
 * 
 * @return 
 */
int32_t com_write_packet_data(int fd, unsigned char *buffer, int buffer_size);

/** 
 * 
 * 
 * @param fd [in] �����豸������
 * @param buffer [out] ������
 * @param buffer_size [in/out] ��������С(����������ʾʵ�ʶ�ȡ��������)
 * @param terminator [in] ������(eg: 0xff)
 * 
 * @return 
 */
int32_t com_get_packet_terminator(int fd, unsigned char *buffer, int *buffer_size, int terminator);
/** 
 * 
 * 
 * @param fd [in] �����豸������
 * @param buffer [out] ������
 * @param buffer_size [in/out] (����������ʾʵ�ʶ�ȡ��������)
 * 
 * @return 
 */
int32_t com_get_packet(int fd, unsigned char *buffer, int *buffer_size);

#ifdef __cplusplus
};
#endif

#endif /* __COMMON_PROTOCOL_H */
