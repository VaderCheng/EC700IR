#ifndef _HVPARAM_STORE_H
#define _HVPARAM_STORE_H

#include "HvInterface.h"
#include "HvUtils.h"
#include "HvParamNode.h"

#define MAX_SECTION_LEN		32
#define MAX_STRING_LEN		4096
#define MAX_BIN_LEN			8192
#define MAX_COMMENT_LEN		64
#define MAX_CHNAME_LEN		64

static const char* HV_PARAM_NAME = "HvParam";

//�ڵ�ö�ٸ�����
template <class T>
class CEnumNodeProxy : public IEnumNodeCallBack
{
public:
	typedef HRESULT (T::*EnumFunc)(CParamNode*);

	//IEnumNodeCallBack
	HRESULT STDMETHODCALLTYPE OnEnumNode(CParamNode* pNode)
	{
		if (m_pOwner && m_pEnumFunc) (m_pOwner->*m_pEnumFunc)(pNode);
			
		return S_OK;
	}

public:
	HRESULT SetEnumFunc(EnumFunc pfn)
	{
		m_pEnumFunc = pfn;
		return S_OK;
	}

	CEnumNodeProxy(T* pOwner, EnumFunc pfn = NULL)
		:m_pOwner(pOwner)
		,m_pEnumFunc(pfn)
	{
	}
	
protected:
	T* m_pOwner;
	EnumFunc m_pEnumFunc;
};

//CParamStore ����
class CParamStore : public HvCore::IHvParam
				  , public HvCore::IHvParam2
{
public:
	// IUnknown
	STDMETHOD(QueryInterface)( 
		const IID& iid, 
		void** ppv
		)
	{
		HRESULT hr = S_OK;
		if ( iid == HvCore::IID_IUnknown )
		{
			*ppv = static_cast<IHvParam*>(this); 
			AddRef();
		}
		else if( iid == HvCore::IID_IHvParam )
		{
			*ppv = static_cast<IHvParam*>(this); 
			AddRef();
		}
		else if( iid == HvCore::IID_IHvParam2)
		{
			*ppv = static_cast<IHvParam2*>(this);
			AddRef();
		}
		else
		{
			*ppv = NULL;
			hr = E_NOINTERFACE;
		}
		return hr;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		return 1;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		return 0;
	}

public: //IHvParam
	STDMETHOD(GetInt)(
		const char* szSection,
		const char* szKey,
		int* pVal,
		int iDefault
		);

	STDMETHOD(GetFloat)(
		const char* szSection,
		const char* szKey,
		float* pVal,
		float fltDefault
		);

	STDMETHOD(GetString)(
		const char* szSection,
		const char* szKey,
		char* szRet,
		int nLen
		);

	STDMETHOD(GetBin)(
		const char* szSection,
		const char* szKey,
		void* pBuf,
		int* pLen
		);

	STDMETHOD(SetInt)(
		const char* szSection,
		const char* szKey,
		int iVal
		);

	STDMETHOD(SetFloat)(
		const char* szSection,
		const char* szKey,
		float fltVal
		);

	STDMETHOD(SetString)(
		const char* szSection,
		const char* szKey,
		const char* szString
		);

	STDMETHOD(SetBin)(
		const char* szSection,
		const char* szKey,
		const void* pBuf,
		int nBufLen
		);

	STDMETHOD(Save)();

public: //IHvParam2, �ڶ�ȡʱͬʱд�븽����Ϣ
	STDMETHOD(GetInt)(
		const char* szSection,
		const char* szKey,
		int* pVal,
		int nDefault,
		int nMin,
		int nMax,
		const char* szChName,
		const char* szComment,
		BYTE8	nRank
		);

	STDMETHOD(GetFloat)(
		const char* szSection,
		const char* szKey,
		float* pVal,
		float fltDefault,
		float fltMin,
		float fltMax,
		const char* szChName,
		const char* szComment,
		BYTE8	nRank
		);

	STDMETHOD(GetString)(
		const char* szSection,
		const char* szKey,
		char* szRet,
		int nLen,
		const char* szChName,
		const char* szComment,
		BYTE8 nRank
		);

	STDMETHOD(GetBin)(
		const char* szSection,
		const char* szKey,
		void* pBuf,
		int* pLen,
		const char* szChName,
		const char* szComment,
		BYTE8 nRank
		);

	STDMETHOD(Save)(
		DWORD32 dwFlag
		);

public:
	CParamStore();
	CParamStore(CParamStore& src);
	~CParamStore();

	HRESULT Initialize(
		HvCore::IHvStream* pStream, 
		BOOL fLoad, 
		BOOL fAutoAdd = TRUE,
		BOOL fClearMode = FALSE
		);

	HRESULT Load();
	HRESULT Clear();

	HRESULT SetStorage(HvCore::IHvStream* pStream);
	HRESULT EnableAutoAdd(BOOL fAutoAdd);
	HRESULT SetClearMode(BOOL fClear);

	HRESULT SaveTo(HvCore::IHvStream* pStream, BOOL fSimpMode = FALSE);
	HRESULT LoadFrom(HvCore::IHvStream* pStream);

	DWORD32 m_dwCookie;

public: //������ȡ���ϲ�
	//�ϲ�ģʽ
	typedef enum _APPEND_MODE
	{
		MODE_REPLACE_KEY = 0,				//��дͬ��(��)�ڵ�
		MODE_ADD_NEW = 1,					//ֻ�����(��)�ڵ�
		MODE_REPLACE_SECTION = 2		//��д�νڵ�,����������ɾ��
	}
	APPEND_MODE;

	//��ȡָ���Ľڵ�,�������Ӹ��ڵ㵽ָ���ڵ������·��
	HRESULT Extract(const char* szSection, const char* szKey, CParamStore* pDestStore);
	//�ϲ�������,Դ������Ӧ�����Ӹ��ڵ㵽ָ���ڵ������·��
	HRESULT Inject(const char* szSection, const char* szKey, CParamStore* pSrcStore, DWORD32 dwMode = 0);

	//ɾ��������
	HRESULT RemoveSection(const char* szSection);
	//ɾ����������
	HRESULT RemoveKey(const char* szSection, const char* szKey);

public: //�ϲ�����
	//����
	CParamStore& Assign(const CParamStore& cSrcStore);
	//iSrc = NULLʱ�����
	CParamStore& Assign(const int& iSrc);
	//׷��
	CParamStore& AddNew(const CParamStore& cSrcStore, BOOL fReplace);

	//����
	CParamStore& operator =(const CParamStore& cSrcStore);
	//iSrc = NULLʱ�����
	CParamStore& operator =(const int& iSrc);
	//׷��,����ͬ���ڵ�
	CParamStore& operator +=(const CParamStore& cSrcStore);
	//׷��,����ͬ���ڵ�
	CParamStore& operator |=(const CParamStore& cSrcStore);

public: //�ײ����
	//���������ڵ�
	CParamNode* CreateNode(const char* szName, PARAM_NODE_TYPE nType);
	//ȡ�öνڵ�
	CParamNode* GetSectionNode(const char* szSection, BOOL fCreate = FALSE);
	//ͨ���νڵ�ȡ�ü��ڵ�
	CParamNode* GetKeyNode( CParamNode* pSectionNode, const char* szKey, BOOL fCreate = FALSE);
	//ͨ������ȡ�ü��ڵ�
	CParamNode* GetKeyNode( const char* szSection, const char* szKey, BOOL fCreate = FALSE);
	//����
	HRESULT EnumParam(IEnumNodeCallBack* pEnumCallBack);

protected:
	CParamNode* m_pRoot;
	BOOL m_fAutoAdd;
	HvCore::IHvStream* m_pStorage;

	CParamStore* m_pClone;	//����ģʽʹ�õĸ���
};

#endif
