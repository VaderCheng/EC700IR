#include "AppUtils.h"

#if SV_RUN_PLATFORM == SV_PLATFORM_DSP
#include "SwCore.h"
//extern void Venus_OutputDebug(char* szMsg);
extern int Dsp_DebugInfo(char* szfmt, ...);
// ƽ̨���ӿ�
extern PLATFORM_FUCTION_LIST* g_pFuction;

#elif SV_RUN_PLATFORM == SV_PLATFORM_WIN
#include "svDbgUtils/svDbgUtilsDll.h"
extern char g_szLogPath[256];

#endif

namespace swTgApp
{

    HRESULT Svresult2Hresult(sv::SV_RESULT svrt)
    {
        HRESULT hr;

        switch(svrt)
        {
        case sv::RS_S_OK:
            hr = S_OK;
            break;

        case sv::RS_S_FALSE:
            hr = S_FALSE;
            break;

        case sv::RS_E_FAIL:
            hr = E_FAIL;
            break;

        case sv::RS_E_OUTOFMEMORY:
            hr = E_OUTOFMEMORY;
            break;

        case sv::RS_E_INVALIDARG:
            hr = E_INVALIDARG;
            break;

        case sv::RS_E_UNEXPECTED:
            hr = E_UNEXPECTED;
            break;

        case sv::RS_E_OBJNOINIT:
            hr = E_OBJ_NO_INIT;
            break;

        case sv::RS_E_NOTIMPL:
            hr = E_NOTIMPL;
            break;

        default :
            hr = E_UNEXPECTED;
            break;
        }

        return hr;
    }

    static int svTraceCallback(
        const char* szInfo,     ///< �ַ���
        int nLen                ///< �ַ�������+1
        )
    {
#if SV_RUN_PLATFORM == SV_PLATFORM_WIN
        //printf(szInfo);
        //utdPrintLog(szInfo);

#if 1  // ������ļ�
        static FILE* s_fileSVTrace = NULL;
        static char s_LogPath[256] = {0};

        if(g_szLogPath[0] != 0
            && strcmp(g_szLogPath, s_LogPath) != 0)
        {
            s_fileSVTrace = fopen(g_szLogPath, "w");

            if(s_fileSVTrace != NULL)
            {
                strcpy(s_LogPath, g_szLogPath);
            }
        }

        if(s_fileSVTrace)
        {
            fwrite(szInfo, 1, strlen(szInfo), s_fileSVTrace);
            fflush(s_fileSVTrace);
        }
        else
        {
            printf(szInfo);
        }

#endif
#else

    //Venus_OutputDebug((char*)szInfo);
    Dsp_DebugInfo("%s",(char*)szInfo);

#endif

        return 0;
    }

#if SV_RUN_PLATFORM == SV_PLATFORM_DSP

    static unsigned int svGetSysTimeCallBack()
    {
        return GetSystemTick();
    }

    /// ���볣���ڴ�
    static void* svMemAllocCallBack(
        int nSize  ///< ���볤��
        )
    {
        return HV_AllocMem(nSize);
    }

    /// ����Ƭ���ڴ�
    static void* svFastMemAllocCallBack(
        int nSize  ///< ���볤��
        )
    {
        return HV_AllocFastMem(nSize);
    }

    /// �ͷ��ڴ�
    static void svMemFreeCallBack(
        void* pAddr,                    ///< Ҫ�ͷŵĵ�ַ
        int nSize                       ///< �ڴ泤��
        )
    {
        HV_FreeMem(pAddr, nSize);
    }

    /// �ͷ�Ƭ���ڴ�
    static void svFastMemFreeCallBack(
        void* pAddr,                    ///< Ҫ�ͷŵĵ�ַ
        int nSize                       ///< �ڴ泤��
        )
    {
        HV_FreeFastMem(pAddr, nSize);
    }

	/// Cache��Ч
	static int svCacheInvaildCallBack(
	    void* pAddr,                    ///< Cache��Ч����ʼ��ַ
	    int nSize                       ///< Cache��Ч�ĳ��ȣ�-1Ϊ������Cache����
	    )
	{
		//sv::utTrace("===sv_Cache_InvaildCallBack %d %d\n", g_pFuction, g_pFuction->pfCacheInv);
		if( g_pFuction != NULL && g_pFuction->pfCacheInv != NULL )
		{
			g_pFuction->pfCacheInv(pAddr, nSize, 0x8, 1);

		}
	    return 0;
	}

	/// Cache��Ч
	static int svCacheWriteBackCallBack(
	    void* pAddr,                    ///< Cache��Ч����ʼ��ַ
	    int nSize                       ///< Cache��Ч�ĳ��ȣ�-1Ϊ������Cache����
	    )
	{
		//sv::utTrace("===sv_Cache_WriteBackCallBack %d %d\n", g_pFuction, g_pFuction->pfCacheWbInv);
		if( g_pFuction != NULL && g_pFuction->pfCacheWbInv != NULL )
		{
			g_pFuction->pfCacheWbInv(pAddr, nSize, 0x8, 1);
		}
	    return 0;
	}

	/// Cache��Ч
	static int svCacheWriteBackInvaildCallBack(
	    void* pAddr,                    ///< Cache��Ч����ʼ��ַ
	    int nSize                       ///< Cache��Ч�ĳ��ȣ�-1Ϊ������Cache����
	    )
	{
		//sv::utTrace("===sv_Cache_WriteBackInvaildCallBackCallBack %d %d\n", g_pFuction, g_pFuction->pfCacheWbInv);
		if( g_pFuction != NULL && g_pFuction->pfCacheWbInv != NULL )
		{
			g_pFuction->pfCacheWbInv(pAddr, nSize, 0x8, 1);//
		}
	    return 0;
	}

	/// ��һ��DMA����������DMA�������������0��ʾʧ�ܡ���Ч���
	static int svDmaOpenCallBack(void)
	{
		//sv::utTrace("===sv_Dma_OpenCallBack %d %d\n", g_pFuction, g_pFuction->pfDMAOpen);
		if( g_pFuction != NULL && g_pFuction->pfDMAOpen != NULL )
		{
			return g_pFuction->pfDMAOpen();
		}
		else return 0;
	}

	/// DMA����������0��ʾ�ɹ�
    static int svDmaCopyCallBack(
        int nHandle,   ///< SV_CALLBACK_DMA_OPEN �򿪵ľ��
        void* pDst,    ///< Ҫ������Դ���ݵ�ַ
        void* pSrc,    ///< Ŀ���ַ
        int nLen       ///< Ҫ���������ݳ���
        )
    {

		//sv::utTrace("===sv_Dma_CopyCallBack %d %d\n", g_pFuction, g_pFuction->pfDMACopy);
		if( g_pFuction != NULL && g_pFuction->pfDMACopy != NULL )
		{
			if (((unsigned int)pSrc & 0x10000000) == 0x10000000)
			{
				pSrc = (void*)((0x0fffffff & (unsigned int)pSrc) | (0x40000000));
			}
			return g_pFuction->pfDMACopy(nHandle, pSrc, pDst, nLen);
		}
		else return 0;
	    //memcpy(pDst, pSrc, nLen);
	    //return 0;
    }

    /// �ȴ�DMA��������������0��ʾ�ɹ����ú���Ϊ����������
    static int svDmaWaitCallBack(
        int nHandle            ///< SV_CALLBACK_DMA_OPEN �򿪵ľ��
        )
    {

		//sv::utTrace("===sv_Dma_WaitCallBack %d %d\n", g_pFuction, g_pFuction->pfDMAWait);
		if( g_pFuction != NULL && g_pFuction->pfDMAWait != NULL )
		{
			return g_pFuction->pfDMAWait(nHandle);
		}
		else return 0;
	    //return 0;
	}

    /// DMA�رպ������ͷ�DMA���
    static int svDmaCloseCallBack(
        int nHandle            ///< SV_CALLBACK_DMA_OPEN �򿪵ľ��
        )
    {
		//sv::utTrace("===sv_Dma_CloseCallBack %d %d\n", g_pFuction, g_pFuction->pfDMAClose);
		if( g_pFuction != NULL && g_pFuction->pfDMAClose != NULL )
		{
			return g_pFuction->pfDMAClose(nHandle);
		}
		else return 0;
	    //return 0;
    }

#endif

    HRESULT SetSVCallBack()
    {
        sv::utSetTraceCallBack_TXT(svTraceCallback);

#if SV_RUN_PLATFORM == SV_PLATFORM_DSP
        sv::SetCallBack_GetSysTime(svGetSysTimeCallBack);

        RTN_HR_IF_SVFAILED((sv::SV_RESULT)sv::SetCallBack_MemAlloc(svMemAllocCallBack));
        RTN_HR_IF_SVFAILED((sv::SV_RESULT)sv::SetCallBack_FastMemAlloc(svFastMemAllocCallBack));
        RTN_HR_IF_SVFAILED((sv::SV_RESULT)sv::SetCallBack_MemFree(svMemFreeCallBack));
        RTN_HR_IF_SVFAILED((sv::SV_RESULT)sv::SetCallBack_FastMemFree(svFastMemFreeCallBack));


        // cache
        RTN_HR_IF_SVFAILED((sv::SV_RESULT)sv::SetCallBack_CacheInvaild(svCacheInvaildCallBack));
        RTN_HR_IF_SVFAILED((sv::SV_RESULT)sv::SetCallBack_CacheWriteBack(svCacheWriteBackCallBack));
        RTN_HR_IF_SVFAILED((sv::SV_RESULT)sv::SetCallBack_CacheWriteBackInvaild(svCacheWriteBackInvaildCallBack));
 
        // dma
        RTN_HR_IF_SVFAILED((sv::SV_RESULT)sv::SetCallBack_DmaOpen(svDmaOpenCallBack));
        RTN_HR_IF_SVFAILED((sv::SV_RESULT)sv::SetCallBack_DmaCopy(svDmaCopyCallBack));
        RTN_HR_IF_SVFAILED((sv::SV_RESULT)sv::SetCallBack_DmaWait(svDmaWaitCallBack));
        RTN_HR_IF_SVFAILED((sv::SV_RESULT)sv::SetCallBack_DmaClose(svDmaCloseCallBack));
#endif

        return S_OK;
    }

}
