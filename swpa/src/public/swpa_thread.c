#include "swpa.h"
#include "swpa_private.h"
#include "swpa_thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#ifdef SWPA_THREAD
#define SWPA_THREAD_PRINT(fmt, ...) SWPA_PRINT("[%s:%d]"fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define SWPA_THREAD_CHECK(arg)      {if (!(arg)){SWPA_PRINT("[%s:%d]Check failed : %s [%d]\n", __FILE__, __LINE__, #arg, SWPAR_INVALIDARG);return SWPAR_INVALIDARG;}}
#else
#define SWPA_THREAD_PRINT(fmt, ...)
#define SWPA_THREAD_CHECK(arg)
#endif

typedef struct tga_THREAD
{
    pthread_t thread;
    void *(*start_routine)(void*);
    void* arg;
    
}THREAD_HANDLE;

typedef struct tga_THREAD_ATTR
{
    int stack_size;
    int priotiry;
}THREAD_ATTR_HANDLE;

typedef struct tga_PROCESS
{
    pid_t processid;
}PROCESS_HANDLE;

typedef struct tga_PROCESS_ATTR
{
    int x;
}PROCESS_ATTR_HANDLE;

void *thread_func(void *pv)
{
	THREAD_HANDLE *handle = (THREAD_HANDLE *)pv;
	handle->start_routine(handle->arg);
	//swpa_mem_free(handle);
	return 0;
}

/**
* @brief �����߳�
* @param [out] thread �߳̽ṹ��ָ��
* @param [in] stacksize �߳�ջ��С���ֽڣ�
* @param [in] priority �߳����ȼ���Ŀǰ��Ч
* @param [in] start_routine �̺߳�����ע���ú����Ĳ�����Ϊarg
* @param [in] arg �̺߳�������
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_thread.h
*/
int swpa_thread_create(
	int* thread,
	const int stacksize,
	const int priority,
	void* (*start_routine)(void*),
	void* arg
	)
{
	SWPA_THREAD_CHECK(thread != NULL && start_routine != NULL);
	SWPA_THREAD_PRINT("thread=0x%08x\n", (unsigned int)thread);
	SWPA_THREAD_PRINT("stacksize=%d\n", (int)stacksize);
	SWPA_THREAD_PRINT("priority=%d\n", (int)priority);
	SWPA_THREAD_PRINT("start_routine=0x%08x\n", (unsigned int)start_routine);
	SWPA_THREAD_PRINT("arg=0x%08x\n", (unsigned int)arg);
	pthread_attr_t attrThread;
	pthread_attr_init (&attrThread);
	if (stacksize > 0)
	{
		pthread_attr_setstacksize(&attrThread, stacksize);
	}
	THREAD_HANDLE* handle = (THREAD_HANDLE*)swpa_mem_alloc(sizeof(THREAD_HANDLE));
	if( handle == NULL )
	{
		return SWPAR_OUTOFMEMORY;
	}

	handle->start_routine = start_routine;
	handle->arg = arg;

	int iret = pthread_create(&handle->thread, &attrThread, thread_func, handle);
	if(iret != 0)
	{
		perror("Err: failed to create thread");
		pthread_attr_destroy(&attrThread);
		swpa_mem_free(handle);
		return SWPAR_FAIL;
	}

	pthread_attr_destroy(&attrThread);

	(*thread) = (int)handle;
	return SWPAR_OK;
}



/**
* @brief �����߳�
* @param [in] thread �߳̽ṹ��ָ���ַ
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_thread.h
*/
int swpa_thread_detach(int thread)
{
	SWPA_THREAD_CHECK(thread != 0);
    SWPA_THREAD_PRINT("thread=%d\n", thread);
    THREAD_HANDLE* handle = (THREAD_HANDLE *)thread;
	int iret = pthread_detach(handle->thread);
	if (0 != iret)
	{
		perror("Err: failed to detach thread");
		return SWPAR_FAIL;
	}
    return SWPAR_OK;
}



/**
* @brief �����߳�
* @param [in] thread �߳̽ṹ��ָ���ַ
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_thread.h
*/
int swpa_thread_destroy(int thread)
{
	SWPA_THREAD_CHECK(thread != 0);
    SWPA_THREAD_PRINT("thread=%d\n", thread);
    THREAD_HANDLE* handle = (THREAD_HANDLE *)thread;
    swpa_mem_free(handle);
    return SWPAR_OK;
}




/**
* @brief �˳���ǰ�߳�
* @param [out] retval �߳��˳���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_thread.h
* @note �ú����ȼ������̺߳�����ʹ��return������
*/
int swpa_thread_exit(void* retval)
{
    SWPA_THREAD_CHECK(retval != NULL);
    SWPA_THREAD_PRINT("retval=0x%08x\n", retval);
    pthread_exit(retval);

    return SWPAR_OK;
}

/**
* @brief �ȴ��߳��˳�
* @param [in] thread �߳̽ṹ��
* @param [out] retval �̺߳�������ֵ
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_thread.h
*/
int swpa_thread_join(int thread, void** retval)
{
    SWPA_THREAD_CHECK(thread != 0);
    SWPA_THREAD_CHECK(retval != NULL);
    SWPA_THREAD_PRINT("thread=%d\n", thread);
    SWPA_THREAD_PRINT("retval=0x%08x\n", (unsigned int)retval);
    THREAD_HANDLE* handle = (THREAD_HANDLE *)thread;
    int iret = pthread_join(handle->thread, NULL);
	if (0 != iret)
	{
		perror("Err: failed to join thread");
		return SWPAR_FAIL;
	}
    return SWPAR_OK;
}

/**
* @brief ȡ���߳�ִ��
* @param [in] thread �߳̽ṹ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_thread.h
*/
int swpa_thread_cancel(int thread)
{
    SWPA_THREAD_CHECK(thread != 0);
    SWPA_THREAD_PRINT("thread=%d\n", thread);
    THREAD_HANDLE* handle = (THREAD_HANDLE *)thread;
    int iret = pthread_cancel(handle->thread);
	if (0 != iret)
	{
		perror("Err: failed to cancel thread");
		return SWPAR_FAIL;
	}
    return SWPAR_OK;
}

/**
* @brief ��ȡ�߳�ID��
* @param [in] thread �߳̽ṹ��
* @param [out] id �߳�ID��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_thread.h
*/
int swpa_thread_getid(int thread, int* id)
{
    SWPA_THREAD_CHECK(thread != 0);
    SWPA_THREAD_CHECK(id != NULL);
    SWPA_THREAD_PRINT("thread=%d\n", thread);
    SWPA_THREAD_PRINT("id=0x%08x\n", (unsigned int)id);
    THREAD_HANDLE* handle = (THREAD_HANDLE *)thread;
    *id = (int)(handle->thread);
    return SWPAR_OK;
}

/**
* @brief �߳�����
* @param [in] ms ��������ʱ�䡣��λ������
* @retval ��
* @see swpa_thread.h
*/
void swpa_thread_sleep_ms(unsigned int ms)
{
    SWPA_THREAD_PRINT("ms=%d\n", ms);
    struct timespec sleeptime;
    struct timespec returntime;
    sleeptime.tv_sec = ms / 1000;
    sleeptime.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&sleeptime, &returntime);

}

/**
* @brief ��������
* @param [out] process ���̽ṹ��
* @param [in] attr ��������
* @param [in] filename ��ִ���ļ���
* @param [in] cmd �����в���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_thread.h
*/
int swpa_process_create(
    int* process,
    const int* attr,
    char* filename,
    char* cmdv[]
)
{
    SWPA_THREAD_CHECK(process != NULL && filename != NULL);
    SWPA_THREAD_PRINT("process=0x%08x\n", (unsigned int)process);
    SWPA_THREAD_PRINT("attr=0x%08x\n", (unsigned int)attr);
    SWPA_THREAD_PRINT("filename=%s\n", filename);
    SWPA_THREAD_PRINT("cmdv=0x%08x\n", (unsigned int)cmdv);
    PROCESS_HANDLE* handle = (PROCESS_HANDLE *)swpa_mem_alloc(sizeof(PROCESS_HANDLE));
	if( handle == NULL )
	{
		return SWPAR_OUTOFMEMORY;
	}
    int iret = fork();
    if(iret < 0)
    {
    	perror("Err: failed to fork");
		swpa_mem_free(handle);
        return SWPAR_FAIL;
    }
    else if(iret == 0)
    {
    	char *szCmd[100], *ch;
    	int pos = 0, index = 0;
    	swpa_memset(szCmd, 0, sizeof(szCmd));
    	for(ch = filename; *ch != '\0'; ch++)
    	{
    		if(*ch != ' ')
    		{
    			if(!szCmd[pos])
    			{
    				szCmd[pos] = (char *)swpa_mem_alloc(255);
    			}
    			szCmd[pos][index++] = *ch;
    		}
    		else if(index > 0)
    		{
    			szCmd[pos][index] = '\0';
    			pos++;
    			index = 0;
    		}
    	}
    	if(szCmd[pos])
    	{
    		szCmd[pos][index] = '\0';
    	}
        execvp(szCmd[0], szCmd);
        printf("swpa_process_create error:%s\n", strerror(errno));
        for(index = 0; index < pos + 1; index++)
        {
        	if(NULL != szCmd[index])
        	{
        		swpa_mem_free(szCmd[index]);
        	}
        }
        swpa_mem_free(handle);
		return SWPAR_FAIL;
    }
    else
    {
        handle->processid = iret;
        (*process) = (int)handle;
    }
    return SWPAR_OK;
}


/**
* @brief ��ֹ����
* @param [in] process ���̽ṹ��
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_thread.h
*/
int swpa_process_kill(int process)
{
	PROCESS_HANDLE* handle = (PROCESS_HANDLE *)process;
	kill(handle->processid, SIGINT);
	swpa_mem_free(handle);
	return SWPAR_OK;
}


/**
* @brief �ȴ�����
* @param [in] process ���̽ṹ��
* @param [in] timeout �ȴ���ʱ����λ������
* @param [out] ikill_code ����kill���˳���
* @param [out] exit_code ���������˳����˳���
* @retval 0 �ɹ�
* @retval -1 ʧ��
* @see swpa_thread.h
* @note ��timeoutΪ-1ʱ����ʾ�����ȴ�����timeout����0ʱ������ʾ��ʱ�ȴ���
*/
int swpa_process_wait(int process, int timeout, int* ikill_code, int *exit_code)
{
	PROCESS_HANDLE* handle = (PROCESS_HANDLE *)process;
	int status;
	if(timeout < 0)
	{
		waitpid(handle->processid, &status, WUNTRACED);
		if(ikill_code)
		{
			*ikill_code = WTERMSIG(status);
		}
		if(exit_code)
		{
			*exit_code = WEXITSTATUS(status);
		}
		return SWPAR_OK;
	}
	else
	{
		int now = swpa_datetime_gettick();
		while(swpa_datetime_gettick() - now < timeout)
		{
			if(0 < waitpid(handle->processid, &status, WNOHANG))
			{
				if(ikill_code)
				{
					*ikill_code = WTERMSIG(status);
				}
				if(exit_code)
				{
					*exit_code = WEXITSTATUS(status);
				}
				return SWPAR_OK;
			}
			swpa_thread_sleep_ms(50);
		}
	}
	return SWPAR_FAIL;
}





