/**
* @file SWVPIFSourceFilter.h 
* @brief VPIF�ɼ�Filter
* @copyright Signalway All Rights Reserved
* @author zhaopy
* @date 2013-04-02
* @version 1.0
*/

#ifndef _SWVPIF_SOURCEFILTER_H_
#define _SWVPIF_SOURCEFILTER_H_

#include "SWBaseFilter.h"
#include "SWMessage.h"

class CSWVPIFSourceFilter : public CSWBaseFilter
{
	CLASSINFO(CSWVPIFSourceFilter, CSWBaseFilter)
public:
	CSWVPIFSourceFilter();
	virtual ~CSWVPIFSourceFilter();

	/**
   *@brief ��ʼ��
   * pvParam [in] : Ŀǰ����ΪNULL��������
   */
	virtual HRESULT Initialize(PVOID pvParam);
	virtual HRESULT Run();
	virtual HRESULT Stop();
public:
	/**
   *@brief ����֡��
   */
	HRESULT SetFps(INT iFps);

	static VOID OnProcessProxy(PVOID pvParam);
protected:	
	/**
   *@brief �ɼ����߳�
   */
	HRESULT OnProcess();
		
	VOID Clear();
protected:
	//�Զ���ӳ���
	SW_BEGIN_DISP_MAP(CSWVPIFSourceFilter, CSWBaseFilter)
		SW_DISP_METHOD(SetFps, 1)
		SW_DISP_METHOD(Initialize, 1)
	SW_END_DISP_MAP()
private:
	static const DWORD MAX_IMAGE_COUNT = 3;		// VPIF API����Ҫ��ʼҪ���ٸ��ڴ��

	int m_iFps;	// ֡�ʣ� -1 �����ı�֡�ʡ�

	int m_iWidth; // ͼƬ��
	int m_iHeight;// ͼƬ��
	CSWMemoryFactory* m_pMemoryFactory; 

	CSWThread* m_pThread; // �ɼ��߳�
	CSWList<CSWImage*> m_lstImage; // ͼƬ����
	BOOL m_fInitialized;
};
REGISTER_CLASS(CSWVPIFSourceFilter)

#endif

