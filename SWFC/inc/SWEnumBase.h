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

#ifndef _SWENUMBASE_H_
#define _SWENUMBASE_H_

#include <list>
#include "SWObject.h"
#include "SWObjbase.h"

using namespace std;

/////////////////////////////

typedef unsigned long ULONG, *PULONG;

//ö��������
template<class T>
class CSWEnumBase
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
    HRESULT EnumBase_Next(
		ULONG cCount,	//�������
		T* rgDat,	//Ŀ������
		ULONG *pcFetched	//ʵ��ȡ�ø���
		)
	{
		if (pcFetched != NULL) *pcFetched = 0;

		if ( rgDat == NULL ) return S_FALSE;

		ULONG cCopied = 0;
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

	HRESULT EnumBase_Skip(
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
	HRESULT EnumBase_Reset(void)
	{
		m_pCur = m_pHead;
		return S_OK;
	}

public:
	//���
	//����ֵ S_OK: ��ӳɹ�
	//					E_OUTOFMEMORY: �����½ڵ�ʧ��
	HRESULT EnumBase_Add(const T& dat)
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
	HRESULT EnumBase_FindNode(const T& dat)
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
	HRESULT EnumBase_Remove(void)
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
	HRESULT EnumBase_Remove(const T& dat)
	{
		HRESULT hr = EnumBase_FindNode(dat);	//��λ�ڵ�
		if (S_OK == hr)
		{
			hr = EnumBase_Remove();	//��λ�ɹ���ɾ����ǰ�ڵ�
		}
		return hr;
	}
public:
	CSWEnumBase()
		:m_pHead(NULL)
		,m_pCur(NULL)
	{
	}

	virtual ~CSWEnumBase()
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
		return (memcpy((void *)&dat1,&dat2,sizeof(T) ) == 0);
	}
	virtual HRESULT OnDelNode(NODE* pNode)
	{
		return S_OK;
	}
	NODE* m_pHead;
	NODE* m_pCur;
};




/*__inline void* HV_memcpy(void* dest, const void* src, size_t len)
{
    return memcpy(dest, src, len);
}

__inline void* HV_memset(void* p, int val, size_t len)
{
    return memset(p, val, len);
}

__inline int HV_memcmp(const void* p1, const void* p2, size_t len)
{
    return memcmp(p1, p2, len);
}*/


#endif

