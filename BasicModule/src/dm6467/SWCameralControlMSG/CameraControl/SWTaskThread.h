#ifndef CSWTASKTHREAD_H
#define CSWTASKTHREAD_H
#include "SWFC.h"
#include "SWObjectList.h"


class CSWTask;
class CSWTaskThreadPool;

class CSWTaskThread : public CSWThread
{
CLASSINFO(CSWTaskThread, CSWThread)
public:
    CSWTaskThread();
    virtual ~CSWTaskThread();

    // �����߳�ֹͣ״̬��Ĭ��Ϊ��ֹͣ״̬
    void SendStopThread() { m_bStopThread = true; }
    // �߳��Ƿ���Ҫֹֹͣͣ
    BOOL IsStopThread() { return m_bStopThread; }
    // �߳��ͷ��Ѿ��˳����߳�һ����������Զ����ø�״̬��
    // ���ΪTRUE�����߳��Ѿ��˳�������Ϊ���ڻ�Ծ�߳�
    BOOL IsExited( ){ return m_bThreadIsStatus;}

    /**
     * @brief �߳������г��򣬸��߳������ظú���ʵ���书�ܡ�
     * @return - S_OK : �ɹ� - E_FAIL : ʧ��
     */
    virtual HRESULT Run();

private:

	CSWTask* WaitForTask( );
	// �洢�����б�
	CSWObjectList m_objTaskLink;
private:
    // �߳��Ƿ��˳�״̬
    BOOL m_bThreadIsStatus;
    // �߳��Ƿ���Ҫֹͣ״̬
    BOOL m_bStopThread;

	friend class CSWTask;
	friend class CSWTaskThreadPool;
};

#endif // CSWTASKTHREAD_H

