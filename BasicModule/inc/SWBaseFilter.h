///////////////////////////////////////////////////////////
//  CSWBaseFilter.h
//  Implementation of the Class CSWBaseFilter
//  Created on:      27-����-2013 16:29:44
//  Original author: huanggc
///////////////////////////////////////////////////////////

#if !defined(EA_F720ABF8_1CAB_4d3e_A68C_33C28371966D__INCLUDED_)
#define EA_F720ABF8_1CAB_4d3e_A68C_33C28371966D__INCLUDED_

#include "SWBasePin.h"
#include "SWBaseGraph.h"
#include "SWClassFactory.h"
/**
 * @brief ����ģ��Filter�Ļ��࣬ʵ����ģ�������ӡ����ݷ��͵ȵ�
 */
 
class CSWBaseGraph;
class CSWBaseFilter;
class CSWBasePin;

class CSWBaseFilter : public CSWObject
{
	CLASSINFO(CSWBaseFilter,CSWObject)
	friend class CSWBaseGraph;
	friend class CSWBasePin;
public:
	/**
	 * @brief ���캯������Ҫָ����Filter��������ĸ���
	 */
	CSWBaseFilter(DWORD dwInCount, DWORD dwOutCount);
	virtual ~CSWBaseFilter();
	/**
	 *@brief ģ��ĳ�ʼ������
	 *@param [in] pvParam ģ������ṹ��ָ��
	 *@return �ɹ�����S_OK������ֵΪ�������
	 */
	 virtual HRESULT Initialize(PVOID pvParam);
	/**
	 * @brief ����Filter������
	 * @param [in] pszName ����Filter������
	 * @return S_OK��ʾ�ɹ�,����ֵΪ�������
	*/
	virtual HRESULT SetName(LPCSTR pszName);
	/**
	 * @brief ��ȡFilter������
	 * @return �ɹ�����Filter�����ƣ�ʧ�ܷ���NULL
	*/
	virtual LPCSTR GetName(void);
	/**
	 * @brief ��������
	 * @param [in] pdu����Э�鵥Ԫ
	 * @return �ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Deliver(CSWObject* obj);
	/**
	 * @brief ���AM��ָ��
	 * @return ����AMָ��
	 */
	virtual CSWBaseGraph* GetGraph();
	/**
	 * @brief ��������ָ��
	 * @param [in] n �ڼ�������
	 * @return �ɹ���������ָ�룬���󷵻�NULL
	 */
	virtual CSWBasePin* GetIn(DWORD n);
	/**
	 * @brief ������������
	 * @return �������������
	 */
	virtual DWORD GetInCount();	
	/**
	 *@brief ��������Pin
	 *@param [in] n�ڼ�������
	 *@return ��������Pinָ��
	 */
	virtual CSWBasePin* CreateIn(DWORD n);
	/**
	 * @brief ��������ָ��
	 * @param [in] n �ڼ������
	 * @return �ɹ��������ָ�룬���󷵻�NULL
	 */
	virtual CSWBasePin* GetOut(DWORD n);
	/**
	 * @brief ������������
	 * @return �������������
	 */
	virtual DWORD GetOutCount();
	/**
	 *@brief �������Pin
	 *@param [in] n�ڼ������
	 *@return ��������Pinָ��
	 */
	virtual CSWBasePin* CreateOut(DWORD n);
	/**
	 * @brief ����Filter��״̬
	 */
	virtual FILTER_STATE GetState();
protected:
	/**
	 * @brief ��ͣFilter
	 * @return �ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Pause();
	/**
	 * @brief ������������ݰ�
	 * @param [in] pdu �������ݰ�
	 * @�ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Receive(CSWObject* obj);
	/**
	 * @brief ����Filter
	 * @return �ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Run();
	/**
	 * @brief ����AM��������
	 * @param [in] dwID  dwID
	 * @param [in] wParam �������1
	 * @param [in] lParam �������2
	 * @return Э���Զ��巵������
	 */
	virtual HRESULT SendCommand(DWORD dwID, DWORD wParam = 0, DWORD lParam = 0);
	/**
	 * @brief ֹͣFilter
	 * @return �ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Stop();
	
private:
	FILTER_STATE m_fsState;
	DWORD m_dwInCount;
	DWORD m_dwOutCount;
	CSWString m_strName;
	CSWBaseGraph *m_pGraph;
	CSWBasePin  **m_pIn;
	CSWBasePin  **m_pOut;
};
#endif // !defined(EA_F720ABF8_1CAB_4d3e_A68C_33C28371966D__INCLUDED_)

