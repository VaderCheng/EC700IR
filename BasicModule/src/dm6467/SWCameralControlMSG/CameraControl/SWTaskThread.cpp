
#include "SWTaskThread.h"
#include "SWTask.h"


CSWTaskThread::CSWTaskThread()
: m_bThreadIsStatus( true )
, m_bStopThread( false )
{

}

CSWTaskThread::~CSWTaskThread()
{
    //dtor
}

/**
 * @brief �߳������г��򣬸��߳������ظú���ʵ���书�ܡ�
 * @return - S_OK : �ɹ� - E_FAIL : ʧ��
 */
HRESULT CSWTaskThread::Run( )
{
    CSWTask* theTask = NULL; m_bThreadIsStatus = false;
    while (true)
    {
        theTask = this->WaitForTask();

        // ����ȴ��߳�Ϊ�գ����˳�����
        if (theTask == NULL ){ m_bThreadIsStatus = true; return S_OK; }

		BOOL doneProcessingEvent = false;
		while( !doneProcessingEvent )
		{
			INT theTimeout = theTask->Run();

            // С�������˳����߳�
			if( theTimeout < 0 ){
				delete theTask; theTask = NULL;
				doneProcessingEvent = true;
			}
			else if( theTimeout == 0 ){
                 doneProcessingEvent = compare_and_store( CSWTask::kAlive , 0 , &theTask->m_uEvents);

			     if( doneProcessingEvent ) theTask = NULL;
			}
			else {
                // ���򽫸�������������
				theTask->SetTimeValue(CSWTask::Milliseconds() + theTimeout);

				m_objTaskLink.Push( theTask );
				doneProcessingEvent = true;
			}
        }
	}

    return S_OK;
}
CSWTask* CSWTaskThread::WaitForTask( )
{
	while (true)
    {
		SInt64 theCurrentTime = CSWTask::Milliseconds();
		CSWTask *theTask = (CSWTask*)m_objTaskLink.Pop( 100 ); // ������100����
		if( theTask != NULL ){
			if( theTask->m_sLastRunTimeMilli < theCurrentTime )
			{
				return theTask;
            }
            else if( this->IsStopThread() ) return theTask;
		}
        else if ( this->IsStopThread()) return NULL;

		INT theTimeout = 0 ;
		if( theTask != NULL ) // ��ȡ�����¼�
			theTimeout = theTask->m_sLastRunTimeMilli - theCurrentTime;
		// �����������������10���룬��Ѱ����������
		if( theTimeout > 10 ){ swpa_thread_sleep_ms( 10 ); m_objTaskLink.Push( theTask ); theTask = NULL;}

		if( theTask ) return theTask;
	}
}

