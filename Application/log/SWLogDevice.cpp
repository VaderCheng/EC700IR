#include "SWFC.h"
#include "SWLogDevice.h"

CSWList<CSWLogDevice *>CSWLogDevice::m_lstLog;
CSWTelnet CSWLogDevice::m_cTelnet;
CSWTelnet CSWLogDevice::m_cDspTelnet;

CSWLogDevice::CSWLogDevice()
{
    m_lstLog.AddTail(this);
}

CSWLogDevice::~CSWLogDevice()
{
}

VOID CSWLogDevice::Log(LPCSTR szLog)
{    
    //������־
    if(FAILED(m_cTelnet.IsValid()))
    {
        m_cTelnet.Create();
    }
    
    //DSPר��������־
    if(FAILED(m_cDspTelnet.IsValid()))
    {
        m_cDspTelnet.Create(2324);
    }  
    
    //�����ַ���
    INT iLevel = -1;
    int index = 0;
    for(LPCSTR ch = szLog; *ch != '\0'; ch++)
    {
        if(*ch == '|')
        {
            index++;
        }
        else if(index == 1)
        {
            if(*ch >= '0' && *ch <= '9')
            {
                if(iLevel < 0)
                {
                    iLevel = 0;
                }
                iLevel = 10 * iLevel + (*ch - '0');
            }
        }
        else if(index == 2)
        {
            szLog = ch + 1;
            break;
        }
    }

    if (iLevel == SW_LOG_LV_DSP)
    {		
        //DSP��־���������
        m_cDspTelnet.Log(2, szLog);
    }
    else
    {
        //���������
        m_cTelnet.Log(2, szLog);
        
        //�������Ӧ���豸
        SW_POSITION pos = m_lstLog.GetHeadPosition();
        while(m_lstLog.IsValid(pos) && iLevel >= 0)
        {
            CSWLogDevice* pDev = (CSWLogDevice *)m_lstLog.GetNext(pos);
			// ��dsp��־���ʱ����ʱ��ֹ���������
			// ����ռ�ô�������Դ���ർ��dsp��־��ʧ
			// ʵ����������������ڵ���־���Ѿ�ͨ��23�˿ڵ�telnet���
			if (DEBUG == pDev->GetLevel())
			{
				static DWORD dwCount = 0;
				if (!m_cDspTelnet.IsLogging() && DEBUG >= iLevel)
				{
					pDev->ShowLog(szLog);
					dwCount = 0;
				}
				else if (dwCount++ % 1000 == 0)
				{
					printf("------  Warning: DSP log Client connected, pause serial log output!  ------\n");
					dwCount = 1;
				}
			}
            else if(pDev->GetLevel() >= iLevel)
            {
                pDev->ShowLog(szLog);
            }
        }
    }
}

