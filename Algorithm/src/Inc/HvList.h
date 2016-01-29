/*
 *	(C)��Ȩ���� 2010 ������·���Ƽ���չ���޹�˾
 */

/**
* @file		HvList.h
* @version	1.0
* @brief	CHvListģ����Ķ����Լ�ʵ��
* @author	Shaorg
* @date		2010-7-26
*/

#ifndef _HVLIST_H_
#define _HVLIST_H_

#include <list>
using namespace std;

namespace HiVideo
{
	typedef int HVPOSITION;  /**< CHvList��λ������*/

	/*! @class	CHvList
	* @brief	����STL�е�listģ����ʵ�֣�������MFC�е�CList�ࡣ
	*/
	template<class T>
	class CHvList
	{
	public:
		/**
		* @brief	Ĭ�Ϲ��캯��
		*/
		CHvList()
		{
		};

		/**
		* @brief	��������
		*/
		virtual ~CHvList()
		{
		};

	public:
		/**
		* @brief	��ͷ�ڵ㴦����һ���µ�Ԫ��
		* @param	item	�����Ԫ��
		* @return	ͷ�ڵ��λ�ã�������һ�����ݴ��ڵ�λ�ã�
		*/
		HVPOSITION AddHead(const T& item)
		{
			m_list.push_front(item);
			return 0;
		};

		/**
		* @brief	��β�ڵ㴦����һ���µ�Ԫ��
		* @param	item	�����Ԫ��
		* @return	β�ڵ��λ�ã��������һ�����ݴ��ڵ�λ�ã�
		*/
		HVPOSITION AddTail(const T& item)
		{
			m_list.push_back(item);
			return (int)m_list.size()-1;
		};

		/**
		* @brief	��ȡͷ�ڵ��λ��
		* @return	�ɹ���0��ʧ�ܣ�-1
		*/
		HVPOSITION GetHeadPosition()
		{
			return m_list.empty() ? -1 : 0;
		};

		/**
		* @brief	��ȡβ�ڵ��λ��
		* @return	�ɹ���β�ڵ��λ�ã�ʧ�ܣ�����-1
		*/
		HVPOSITION GetTailPosition()
		{
			return m_list.empty() ? -1 : ((int)m_list.size()-1);
		};

		/**
		* @brief	��ȡָ��λ�õ�Ԫ�ص����õ�ͬʱ����λ�ú���һλ
		* @param	rPosition	ָ����λ��
		* @return	����λ��rPosition��Ԫ�ص����ã�֮��rPosition�����һλ,����Ƶ��Ƿ�λ�ã���rPosition����Ϊ-1��
		* @details	��������rPositionС��0���򷵻�λ��ͷ�ڵ�Ԫ�ص����ã�
		*			��������rPosition���ڵ���β�ڵ��λ�ã��򷵻�λ��β�ڵ�Ԫ�ص����ã�
		*			ͬʱ��rPosition��������Ϊ-1��
		* @warning	�ڵ��ô˺���֮ǰ����ȷ��list��Ϊ�ա�
		*/
		T& GetNext(HVPOSITION& rPosition)
		{
			HVPOSITION rOldPos = rPosition;

			//��rPosition�Ϸ��Ҳ�Ϊβ�ڵ��λ��
			rPosition = ( 0 <= rPosition && rPosition < ((int)m_list.size()-1) ) ? (rPosition+1) : -1;

			//��rOldPos���ںϷ�λ�ã����ߴ���ĩβʱ����rOldPosλ�õ�Ԫ���ҵ��������ظ�Ӧ�ò㡣
			if ( rPosition != -1 || ((int)m_list.size()-1) == rOldPos )
			{
				return GetAt(rOldPos);
			}
			else
			{
				//rPosition�ڷǷ�λ��ʱ�Ĵ���
				if ( rOldPos < 0 )
				{
					rOldPos = 0;
				}
				else if ( rOldPos > ( (int)m_list.size()-1 ) )
				{
					rOldPos = ( (int)m_list.size()-1 );
				}
				return GetAt(rOldPos);
			}
		};

		/**
		* @brief	��ȡָ��λ�õ�Ԫ�ص����õ�ͬʱ����λ��ǰ��һλ
		* @param	rPosition	ָ����λ��
		* @return	����λ��rPosition��Ԫ�ص����ã�֮��rPosition���ǰһλ,����Ƶ��Ƿ�λ�ã���rPosition����Ϊ-1��
		* @details	��������rPositionΪ�Ƿ�ֵ������ͬGetNext()
		* @see		GetNext()
		* @warning	�ڵ��ô˺���֮ǰ����ȷ��list��Ϊ�ա�
		*/
		T& GetPrev(HVPOSITION& rPosition)
		{
			HVPOSITION rOldPos = rPosition;

			//���rPosition�Ϸ��Ҳ�Ϊͷ�ڵ��λ��
			rPosition = ( 0 < rPosition && rPosition <= ((int)m_list.size()-1) ) ? (rPosition-1) : -1;

			if ( rPosition != -1 || 0 == rOldPos )
			{
				return GetAt(rOldPos);
			}
			else
			{
				//rPosition�ڷǷ�λ��ʱ�Ĵ���
				if ( rOldPos < 0 )
				{
					rOldPos = 0;
				}
				else if ( rOldPos > ( (int)m_list.size()-1 ) )
				{
					rOldPos = ( (int)m_list.size()-1 );
				}
				return GetAt(rOldPos);
			}
		};

		/**
		* @brief	ɾ��ͷ�ڵ�
		* @return	���Ƴ���ͷ�ڵ��е�Ԫ��
		* @warning	�ڵ��ô˺���֮ǰ����ȷ��list��Ϊ�ա�
		*/
		T RemoveHead()
		{
			class list<T>::iterator it_head = m_list.begin();
			T delItem = (*it_head);

			m_list.pop_front();
			return delItem;
		};

		/**
		* @brief	ɾ��β�ڵ�
		* @return	���Ƴ���β�ڵ��е�Ԫ��
		* @warning	�ڵ��ô˺���֮ǰ����ȷ��list��Ϊ�ա�
		*/
		T RemoveTail()
		{
			class list<T>::reverse_iterator it_tail = m_list.rbegin();
			T delItem = (*it_tail);

			m_list.pop_back();
			return delItem;
		};

		/**
		* @brief	ɾ��ָ��λ�õĽڵ�
		* @param	rPosition	ָ����λ��
		* @return	void
		*/
		void RemoveAt(const HVPOSITION& rPosition)
		{
			if ( 0 <= rPosition && rPosition < (int)m_list.size() )
			{
				int nPos = 0;
				for ( class list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it )
				{
					if ( nPos == rPosition )
					{
						m_list.erase(it);
						return ;
					}
					++nPos;
				}
			}
		};

		/**
		* @brief	ɾ�����нڵ�
		* @return	void
		*/
		void RemoveAll()
		{
			m_list.clear();
		};

		/**
		* @brief	�ж�ָ����λ���Ƿ�Ϊ�Ϸ�λ��
		* @param	rPosition	ָ����λ��
		* @return	�Ϸ���true���Ƿ���false
		*/
		bool IsValidPos(const HVPOSITION& rPosition)
		{
			return ( 0 <= rPosition && rPosition < (int)m_list.size() ) ? true : false;
		};

		/**
		* @brief	�ж�list�Ƿ�Ϊ��
		* @return	Ϊ�գ�true����Ϊ�գ�false
		*/
		bool IsEmpty()
		{
			return m_list.empty();
		};

		/**
		* @brief	�ж�list�Ƿ������������Ѿ������ٲ���Ԫ�أ�
		* @return	����Ϊfalse��
		* @detail	�ó�Ա������Ŀ����Ϊ�˼��ݾɰ汾
		*/
		bool IsFull()
		{
			//���������ʵ����Ϊ�˿��Ǽ����ԣ�������Կ���ȥ����
			//��Ϊ����STL��list��ʵ�ֵģ������ڴ��Ƕ�̬�ڶ��Ϸ���ģ��������迼���Ƿ�����������ڴ������ˣ�
			return false;
		};

		/**
		* @brief	��ȡlist�ĳ���
		* @return	list�ĳ���
		*/
		int GetSize()
		{
			return (int)m_list.size();
		};
       /**
		* @brief	��ȡlist�б�
		* @return	list�б�
		*/
		const list<T>& GetList() const
		{
			return m_list;
		}
	private:
		//ע����Ϊ�ڲ�˽�к�����Position�ĺϷ����Լ�m_list�ķǿ����ɵ��ú�����֤��
		T& GetAt(const HVPOSITION& rPosition)
		{
		    int nPos;

			//���if�ж������Ż���Ϊ������λ�����м�֮ǰ��������ң����м�֮��ķ�����ҡ�
			if ( rPosition <= ((int)m_list.size()/2) )
			{
				nPos = 0;
				for ( class list<T>::iterator it = m_list.begin(); it != m_list.end(); ++it )
				{
					if ( nPos == rPosition )
					{
						return (*it);
					}
					++nPos;
				}
			}
			else
			{
			    nPos = 0;
				for ( class list<T>::reverse_iterator it_r = m_list.rbegin(); it_r != m_list.rend(); ++it_r )
				{
				    HVPOSITION rPosition_r = m_list.size()-1 - rPosition;
					if ( nPos == rPosition_r )
					{
						return (*it_r);
					}
					++nPos;
				}
			}
			return *m_list.end();
		};

	private:
		list<T> m_list;
	};
}

#endif
