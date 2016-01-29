///////////////////////////////////////////////////////////
//  CSWBasePin.h
//  Implementation of the Class CSWBasePin
//  Created on:      27-����-2013 16:29:45
//  Original author: huanggc
///////////////////////////////////////////////////////////

#if !defined(EA_D326A1EB_5AD6_445a_90BC_65F4369EF27C__INCLUDED_)
#define EA_D326A1EB_5AD6_445a_90BC_65F4369EF27C__INCLUDED_

#include "SWObject.h"

/**
 * @brief ���������
 */
class CSWBaseGraph;
class CSWBaseFilter;
class CSWBasePin;

enum PIN_STATE{PIN_NONE, PIN_RUNNING, PIN_PAUSED, PIN_STOPPED};
enum FILTER_STATE{FILTER_NONE, FILTER_RUNNING, FILTER_PAUSED, FILTER_STOPPED};

class CSWBasePin : virtual public CSWObject
{
	CLASSINFO(CSWBasePin,CSWObject)
	friend class CSWBaseGraph;
	friend class CSWBaseFilter;
public:	
	/**
	 * @brief ���캯������Ҫ�������ָ�������뻹�����Pin
	 * @param [in] pFilter Filterָ��
	 * @param [in] dwDir ����0��ʾ�����1��ʾ����
	 */
	CSWBasePin(CSWBaseFilter* pFilter, DWORD dwDir);
	virtual ~CSWBasePin();
	/**
	 * @brief ��Ӵ˽ӿڵ����ݸ�ʽ��ֻ�з��ϴ˸�ʽ�����ݵ�Ԫ���ܾ����˽ӿ�
	 * @param [in] dwFormat ����Э�鵥Ԫ��ʽ
   * @return �ɹ�����S_OK,����ֵΪ�������
	 */
	virtual HRESULT AddObject(DWORD dwObjectID);
	/**
	 * @brief ������һ��Pin,����Ҫ��ֻ������ܹ���������,���������ӷ�ʽ���ǲ������
	 * @param [in] pIn ����
	 * @return �ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Connect(CSWBasePin* pIn);
	/**
	 * @brief ��������
	 * @param [in] pdu����Э�鵥Ԫ
	 * @return �ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Deliver(CSWObject* obj);
	/**
	 * @brief ���Filterָ��
	 * @return �ɹ�����Filterָ�룬ʧ�ܷ���NULL
	 */
	virtual CSWBaseFilter* GetFilter();
	/**
	 * @brief ������Ӵ�Pin����һ��Pin
	 * @return �ɹ�����Pinָ�룬ʧ�ܷ���NULL
	 */
	virtual CSWBasePin* GetPin();
	/**
	 * @brief ����Pin��״̬
	 * @return ����Pin��״̬
	 */
	virtual PIN_STATE GetState();
	/**
	 * @brief ��ͣPin
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
	 * @brief ����Pin
	 * @return �ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Run();
	/**
	 * @brief ֹͣPin
	 * @return �ɹ�����S_OK������ֵΪ�������
	 */
	virtual HRESULT Stop();

private:
	PIN_STATE m_State;
	DWORD m_dwDirection;
	CSWList<DWORD>m_lstObjectID;
	CSWList<DWORD>m_lstConnectID;
	CSWBaseFilter* m_pFilter;
	CSWBasePin* m_pIn;	
};
#endif // !defined(EA_D326A1EB_5AD6_445a_90BC_65F4369EF27C__INCLUDED_)

