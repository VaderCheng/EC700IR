#ifndef _HV_ENUM_H
#define _HV_ENUM_H

#include "HvUtils.h"

#include "swObjBase.h"

//ö��������
template<class T>
class CHvEnumBase
{	
public:
	//�ڵ㶨��
	typedef struct _NODE
	{
		_NODE* pPre;
		_NODE* pNext;

		T Dat;

		_NODE()
			:pPre(NULL)
			,pNext(NULL)
		{
		}
	} NODE;

	//ȡ����
	//����ֵ  S_OK:  ��ȡָ�������ڵ�ɹ�
	//					S_FALSE:  ����
	//��ע: �����ȶ�λm_pCur,��ͨ��Reset Skip FindNode
	STDMETHOD(EnumBase_Next)( 
		ULONG cCount,	//�������
		T* rgDat,	//Ŀ������
		ULONG *pcFetched	//ʵ��ȡ�ø���
		)
	{
		if (pcFetched != NULL) *pcFetched = 0;

		if ( rgDat == NULL ) return S_FALSE; 

		ULONG cCopied(0);
		//��ǰ�ڵ�ָ�벻ΪNULL�ҿ�����С����Ҫ��
		while(m_pCur != NULL && cCopied < cCount)
		{
			*rgDat = m_pCur->Dat;
			m_pCur = m_pCur->pNext;

			rgDat++;
			cCopied++;
		}

		if ( pcFetched != NULL ) *pcFetched = cCopied;

		return (cCount == cCopied)?S_OK:S_FALSE;
	}

	//����ָ�������ڵ�
	//����ֵ  S_OK: ����ָ�������ڵ�ɹ�
	//				   S_FALSE: ����

	STDMETHOD(EnumBase_Skip)( 
		ULONG cCount
		)
	{
		ULONG cSkipped(0);
		while(m_pCur != NULL && cSkipped < cCount)
		{
			m_pCur = m_pCur->pNext;
			cSkipped++;
		}
		return (cCount == cSkipped)?S_OK:S_FALSE;
	}

	//��������ָ��
	//����ֵ S_OK 
	STDMETHOD(EnumBase_Reset)(void)
	{
		m_pCur = m_pHead;
		return S_OK;
	}

public:
	//���
	//����ֵ S_OK: ��ӳɹ�
	//					E_OUTOFMEMORY: �����½ڵ�ʧ��
	STDMETHOD(EnumBase_Add)(const T& dat)
	{
		NODE* pNewNode = new NODE;
		if (pNewNode == NULL) return E_OUTOFMEMORY;

		pNewNode->Dat = dat;

		NODE* pTail(m_pHead);

		if (pTail == NULL)
		{
			m_pHead = pNewNode;
			m_pCur = m_pHead;
		}
		else
		{
			//����β
			while(pTail->pNext != NULL) 
			{ 
				pTail = pTail->pNext;
			}
			pNewNode->pPre = pTail;
			pTail->pNext=pNewNode;	
		}

		return S_OK;
	}

	//Ѱ��ָ���ڵ�
	//����ֵ S_OK: �ҵ�����λ�ɹ�,��ǰ�ڵ�ָ��ָ��ƥ��Ľڵ�
	//					S_FALSE: �Ҳ���,���ı䵱ǰ�ڵ�ָ��
	STDMETHOD(EnumBase_FindNode)(const T& dat)
	{
		NODE* pNode(m_pHead);
		NODE TempNode;
		BOOL fFound = FALSE; 

		while( pNode != NULL )
		{
			if (IsMatch(dat,pNode->Dat))
			{
				m_pCur = pNode;
				fFound = TRUE;
				break;
			}
			pNode = pNode->pNext;
		}

		return fFound?S_OK:S_FALSE;
	}

	//ɾ����ǰ�ڵ�
	//����ֵ	S_OK:	 ɾ���ɹ�,��ǰ�ڵ�ָ��ָ����һ���ڵ�(��β��Ϊ��)
	//					S_FALSE: ��ǰ�ڵ�ָ��Ϊ��(�ѵ���β)
	STDMETHOD(EnumBase_Remove)(void)
	{
		if (m_pCur == NULL) return S_FALSE;

		NODE* pPre = m_pCur->pPre;
		NODE* pNext = m_pCur->pNext;

		OnDelNode(m_pCur);

		delete m_pCur;
		m_pCur = pNext;

		if (pPre == NULL && pNext == NULL)		//ΨһԪ��
		{
			m_pHead = NULL;
		}
		else if (pPre != NULL && pNext == NULL)	//ɾ��β
		{
			pPre->pNext = NULL;
		}
		else if ( pPre == NULL && pNext != NULL)		//ɾ��ͷ
		{
			m_pHead = pNext;
			pNext->pPre = NULL;
		}
		else	//ɾ���м�
		{
			pPre->pNext = pNext;
			pNext->pPre = pPre;
		}

		return S_OK;
	}

	//ɾ��ָ���ڵ�
	//����ֵ S_OK: �ҵ���ɾ���ڵ�ɹ�
	//					S_FALSE: �Ҳ�����δ��ɾ��
	STDMETHOD(EnumBase_Remove)(const T& dat)
	{
		HRESULT hr = EnumBase_FindNode(dat);	//��λ�ڵ�
		if (S_OK == hr)
		{
			hr = EnumBase_Remove();	//��λ�ɹ���ɾ����ǰ�ڵ�
		}
		return hr;		
	}
public:
	CHvEnumBase()
		:m_pHead(NULL)
		,m_pCur(NULL)
	{
	}

	virtual ~CHvEnumBase()
	{
		NODE *pDel,*pNode(m_pHead);
		while(pNode != NULL)
		{
			pDel = pNode;	//����Ҫɾ���Ľڵ�ָ��
			pNode = pNode->pNext;	//ָ����һ��
			delete pDel;	//ɾ��
		}
	}

	BOOL GetHead( T* pRet)
	{
		if (!pRet) return FALSE;

		m_pCur = m_pHead;

		if (m_pCur == NULL) return FALSE;

		*pRet = m_pCur->Dat;

		return TRUE;
	}

	BOOL GetTail( T* pRet)
	{
		if (!pRet) return FALSE;

		EnumBase_Reset();

		if (m_pCur == NULL) return FALSE;

		while ( m_pCur->pNext != NULL)
		{
			m_pCur = m_pCur->pNext;
		}

		*pRet = m_pCur->Dat;

		return TRUE;
	}

	BOOL IsEmpty()
	{
		return (m_pHead == NULL);
	}

protected:
	//�ж�ƥ�����,��������Զ���
	virtual BOOL IsMatch(const T& dat1, const T& dat2)
	{
		return (HV_memcmp(&dat1,&dat2,sizeof(T) ) == 0);
	}
	virtual HRESULT OnDelNode(NODE* pNode)
	{
		return S_OK;
	}
	NODE* m_pHead;
	NODE* m_pCur;
};

template<class T> 
class CSimpStack
{
public:
	BOOL Push(const T& dat)
	{
		return S_OK == m_StackStore.EnumBase_Add(dat);
	}
	BOOL Pop(T* pRet)
	{
		BOOL fRet = FALSE;
		if ( TRUE == m_StackStore.GetTail(pRet) )
		{
			m_StackStore.EnumBase_Remove(); //��ʱpCurָ�����һ��NODE
			fRet = TRUE;
		}
		return fRet;
	}
	BOOL GetTop(T* pRet)
	{
		if ( !m_StackStore.IsEmpty() )
		{
			return m_StackStore.GetTail(pRet);
		}
		else
		{
			return FALSE;
		}
	}
	BOOL IsEmpty()
	{
		return m_StackStore.IsEmpty();
	}

protected:
	CHvEnumBase<T> m_StackStore;
};

#endif// _HV_ENUM_H

