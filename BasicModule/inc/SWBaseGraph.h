///////////////////////////////////////////////////////////
//  CSWBaseGraph.h
//  Implementation of the Class CSWBaseGraph
//  Created on:      27-����-2013 16:29:44
//  Original author: huanggc
///////////////////////////////////////////////////////////

#if !defined(EA_4086695D_55CD_4dc6_859F_96CE104EEE33__INCLUDED_)
#define EA_4086695D_55CD_4dc6_859F_96CE104EEE33__INCLUDED_

#include "SWObject.h"
#include "SWList.h"

/**
 * @brief AM������,��Ҫ����Filter�����ӡ����С�ֹͣ����ѯ�ȵȲ���
 * @brief �ƹ�����д
 * @brief 2013-02-27��������
 */
class CSWBaseGraph;
class CSWBaseFilter;
class CSWBasePin;

class CSWBaseGraph : public CSWObject
{
	CLASSINFO(CSWBaseGraph,CSWObject)
	friend class CSWBaseFilter;
	friend class CSWBasePin;
public:
	CSWBaseGraph();
	virtual ~CSWBaseGraph();
	/**
	 *@brief ȡ���������İ汾��
	 *@return �ɹ����ذ汾��ʧ�ܷ���-1
	 */
	static INT GetVersion(VOID);
	/**
	 * @brief ���һ��Filter��ֻ�����Filter��AMȥ����ʵ������
	 * @param [in] pszName ָ��Filter������
	 * @param [in] pFilter Filter����
	 * @return �ɹ�����S_OK,����ֵ����������
	 */
	virtual HRESULT AddFilter(LPCSTR pszName, CSWBaseFilter* pFilter);
	/**
	 * @brief ����2��Pin
	 * @param [in] pOut���Pin
	 * @param [in] pIn ����Pin
	 * @return �ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Connect(CSWBasePin* pOut, CSWBasePin* pIn);
	/**
   * @brief ��Ӧ���µ�Filter�������¼�
   * @param [in] pFilter �������¼���Filter
   * @param [in] dwID  dwID
   * @param [in] wParam �������1
   * @param [in] lParam �������2
   * @return Э���Զ��巵������
   */
	virtual HRESULT OnCommand(CSWBaseFilter* pFilter, DWORD dwID, DWORD wParam, DWORD lParam);
	/**
	 * @brief ��ͣ����Filter�����У������ʵ���ǵ�������������Filter��Pause����
	 * @return �ɹ�����S_OK������ֵ����������
	 */
	virtual HRESULT Pause();
	/**
	 * @brief ��ѯAM���Ƿ��д�Filter,ͨ��pszName��ѯ
	 * @param [in] pszName Filter������
	 * @param [out] pFilter  �����ѯ����Filter
	 * @return �ɹ�����S_OK,����ֵΪ�������
	 */
	virtual CSWBaseFilter* QueryFilter(LPCSTR pszName);
	/**
	 * @brief ɾ��Filter
	 * @param [in] pszName Filter����
	 * @return �ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT RemoveFilter(LPCSTR lpzName);
	/**
	 * @brief  �������е�Filter,���ڲ�ʵ���ǵ���������Filter��Run����
	 * @return �ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Run();
	/**
	 * @brief  ֹͣ���е�Filter,���ڲ�ʵ���ǵ���������Filter��Stop����
	 * @return �ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Stop();
	
	/**
	 * @brief ��ѯ����Filter��״̬
	 * @return ����Filter��״̬
	 */
	virtual FILTER_STATE GetState();

protected:
	CSWList<CSWBaseFilter *>m_lstFilter;
};
#endif // !defined(EA_4086695D_55CD_4dc6_859F_96CE104EEE33__INCLUDED_)

