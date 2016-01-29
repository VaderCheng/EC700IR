// ���ļ������ʽ������WIN936
#ifndef _SWPARAM_NODE_XXXX_H
#define _SWPARAM_NODE_XXXX_H

#include "SWEnumBase.h"
#include "swpa_mem.h"
#include "SWFileStream.h"
#include "SWString.h"

typedef struct _PARAM_VAR
{
    //0x00~0xff
    enum  _VAR_TYPE
    {
        VT_NULL = 0,
        VT_INT = 10,
        VT_UINT = 11,
        VT_ENUM = 12,
        VT_DATASET = 13,
        VT_FLOAT = 20,
        VT_DOUBLE = 21,
        VT_BIN = 100,
        VT_STR = 101,
        VT_BYTE = 200
    }
    vt;

    union
    {
        BYTE Byt;
        INT Int;
        UINT uInt;
        FLOAT Flt;
        DOUBLE Dbl;
        struct
        {
            void* pBuf;
            WORD nLen;
        } Bin;
    }
    val;

    _PARAM_VAR()
    {
        vt = VT_NULL;
    }
}
PARAM_VAR;

class CSWParamVar : public PARAM_VAR
{
public:
    CSWParamVar(){}

    ~CSWParamVar()
    {
        Clear();
    }

    CSWParamVar(const INT& src)
    {
        vt = VT_INT;
        val.Int = src;
    }
    CSWParamVar(const UINT& src)
    {
        vt = VT_UINT;
        val.uInt = src;
    }
    CSWParamVar(const FLOAT& src)
    {
        vt = VT_FLOAT;
        val.Flt = src;
    }
    CSWParamVar(const DOUBLE& src)
    {
        vt = VT_DOUBLE;
        val.Dbl = src;
    }
    CSWParamVar(const BYTE& src)
    {
        vt = VT_BYTE;
        val.Byt = src;
    }
    CSWParamVar(LPCSTR szSrc)
    {
        SetString(szSrc);
    }
    CSWParamVar(const void* pBuf, const WORD& nLen)
    {
        SetBin(pBuf, nLen);
    }



public:
    CSWParamVar& operator=(const INT& src)
    {
        if (vt == VT_INT)
        {
            val.Int = src;
        }
		else if( vt == VT_ENUM )
		{
			val.Int = src;
		}
        else
        {
            Clear();
            vt = VT_INT;
            val.Int = src;
        }
        return *this;
    }
    CSWParamVar& operator=(const UINT& src)
    {
        if (vt == VT_UINT)
        {
            val.uInt = src;
        }
        else
        {
            Clear();
            vt = VT_UINT;
            val.uInt = src;
        }
        return *this;
    }
    CSWParamVar& operator=(const FLOAT& src)
    {
        if (vt == VT_FLOAT)
        {
            val.Flt = src;
        }
        else
        {
            Clear();
            vt = VT_FLOAT;
            val.Flt = src;
        }
        return *this;
    }
    CSWParamVar& operator=(const DOUBLE& src)
    {
        if (vt == VT_DOUBLE)
        {
            val.Dbl =(DOUBLE)src;
        }
        else
        {
            Clear();
            vt = VT_DOUBLE;
            val.Dbl = src;
        }
        return *this;
    }
    CSWParamVar& operator=(const BYTE& src)
    {
        if (vt == VT_BYTE)
        {
            val.Byt = src;
        }
        else
        {
            Clear();
            vt = VT_BYTE;
            val.Byt = src;
        }
        return *this;
    }
    CSWParamVar& operator=(LPCSTR szSrc)
    {
        SetString(szSrc);
        return *this;
    }
    CSWParamVar& operator=(const CSWParamVar& cSrcVar)
    {
        Clear();
        if ( cSrcVar.vt == VT_INT)
        {
            vt = VT_INT;
            val.Int = cSrcVar.val.Int;
        }
        else if ( cSrcVar.vt == VT_UINT)
        {
            vt = VT_UINT;
            val.uInt = cSrcVar.val.uInt;
        }
        else if ( cSrcVar.vt == VT_FLOAT)
        {
            vt = VT_FLOAT;
            val.Flt = cSrcVar.val.Flt;
        }
        else if ( cSrcVar.vt == VT_DOUBLE)
        {
            vt = VT_DOUBLE;
            val.Dbl = cSrcVar.val.Dbl;
        }
        else if ( cSrcVar.vt == VT_BIN)
        {
            SetBin(cSrcVar.val.Bin.pBuf, cSrcVar.val.Bin.nLen);
        }
        else if (cSrcVar.vt == VT_STR)
        {
            SetString((LPCSTR)cSrcVar);
        }
		else if (cSrcVar.vt == VT_ENUM)
		{
			vt = VT_ENUM;
			val.Int = cSrcVar.val.Int;
		}
		else if( cSrcVar.vt == VT_DATASET )
		{
		    vt = VT_DATASET;
		}

        return *this;
    }

public:
    operator INT() const
    {
        if (vt == VT_INT)
        {
            return val.Int;
        }
        if (vt == VT_UINT)
        {
            return (INT)val.uInt;
        }
        else if (vt == VT_FLOAT)
        {
            return (INT)val.Flt;
        }
        else if (vt == VT_DOUBLE)
        {
            return (INT)val.Dbl;
        }
        else if (vt == VT_BYTE)
        {
            return (INT)val.Byt;
        }
		else if( vt == VT_ENUM )
		{
			return val.Int;
		}
		else if( vt == VT_DATASET )
		{
			return val.Int;
		}
        else
        {
            return 0;
        }
    }
    operator UINT() const
    {
        if (vt == VT_INT)
        {
            return (UINT)val.Int;
        }
        if (vt == VT_UINT)
        {
            return val.uInt;
        }
        else if (vt == VT_FLOAT)
        {
            return (UINT)val.Flt;
        }
        else if (vt == VT_DOUBLE)
        {
            return (UINT)val.Dbl;
        }
        else if (vt == VT_BYTE)
        {
            return (UINT)val.Byt;
        }
        else
        {
            return 0;
        }
    }
    operator FLOAT() const
    {
        if (vt == VT_INT)
        {
            return (FLOAT)val.Int;
        }
        else if (vt == VT_UINT)
        {
            return (FLOAT)val.uInt;
        }
        else if (vt == VT_FLOAT)
        {
            return val.Flt;
        }
        else if (vt == VT_DOUBLE)
        {
            return (FLOAT)val.Dbl;
        }
        else if (vt == VT_BYTE)
        {
            return (FLOAT)val.Byt;
        }
        else
        {
            return 0;
        }
    }
    operator DOUBLE() const
    {
        if (vt == VT_INT)
        {
            return (DOUBLE)val.Flt;
        }
        else if (vt == VT_UINT)
        {
            return (DOUBLE)val.uInt;
        }
        else if (vt == VT_FLOAT)
        {
            return (DOUBLE)val.Flt;
        }
        else if (vt == VT_DOUBLE)
        {
            return val.Dbl;
        }
        else if (vt == VT_BYTE)
        {
            return (DOUBLE)val.Byt;
        }
        else
        {
            return 0;
        }
    }
    operator LPCSTR() const
    {
        if (vt == VT_STR)
        {
            return (LPCSTR)val.Bin.pBuf;
        }
        else
        {
            return '\0';
        }
    }

    BOOL operator==(const CSWParamVar& cDest)
    {
        if (vt != cDest.vt) return FALSE;
        if (vt == VT_INT)
        {
            return (val.Int == cDest.val.Int);
        }
        else if (vt == VT_UINT)
        {
            return (val.uInt == cDest.val.uInt);
        }
        else if (vt == VT_FLOAT || vt == VT_DOUBLE)
        {
            return FALSE;	//���������Ƚ�
        }
        else if (vt == VT_BYTE)
        {
            return (val.Byt == cDest.val.Byt);
        }
        else if (vt == VT_STR)
        {
            return IsEqualString((LPCSTR)(*this), (LPCSTR)cDest);
        }
        else if (vt == VT_BIN)
        {
            if (val.Bin.nLen != cDest.val.Bin.nLen) return FALSE;

            return (swpa_memcmp(val.Bin.pBuf, cDest.val.Bin.pBuf, val.Bin.nLen) == 0);
        }
		else if( vt == VT_ENUM )
		{
			 return (val.Int == cDest.val.Int);
		}
        else
        {
            return FALSE;
        }
    }

    BOOL operator!=(const CSWParamVar& cDest)
    {
        return !(*this == cDest);
    }

    BOOL operator==(LPCSTR pszDest)
    {
        if ( vt != VT_STR) return FALSE;
        return IsEqualString((const char*)val.Bin.pBuf, pszDest);
    }

    BOOL operator==(const INT& iDest)
    {
        if ( vt != VT_INT) return FALSE;
        return ( val.Int == iDest );
    }

    BOOL operator==(const UINT& nDest)
    {
        if ( vt != VT_UINT) return FALSE;
        return ( val.uInt == nDest );
    }

    BOOL operator==(const BYTE& bDest)
    {
        if ( vt != VT_BYTE) return FALSE;
        return ( val.Byt == bDest );
    }

public:
    BOOL IsNull()
    {
        return (vt == VT_NULL);
    }

    HRESULT SetString(LPCSTR szSrc)
    {
        if ( szSrc == NULL ) return E_INVALIDARG;
        Clear();	//����ΪVT_NULL
        UINT nLen = (UINT)strlen(szSrc) + 1;
        HRESULT hr = AllocBuf(nLen);
        if (hr == S_OK)
        {
            swpa_memcpy((char*)val.Bin.pBuf, szSrc, nLen);
            *((char*)val.Bin.pBuf + nLen - 1) ='\0';
            vt = VT_STR; //ֻ�з���ɹ��Ż�ı�TYPE
        }
        return hr;
    }

    HRESULT SetBin(const void* pBuf, WORD nLen)
    {
        if ( pBuf == NULL ) return E_INVALIDARG;
        Clear(); //����ΪVT_NULL
        HRESULT hr = AllocBuf(nLen);
        if (hr == S_OK)
        {
            swpa_memcpy(val.Bin.pBuf, pBuf, nLen);
            vt = VT_BIN; //ֻ�з���ɹ��Ż�ı�TYPE
        }
        return hr;
    }

    HRESULT GetBin(void** ppBuf, WORD* pLen)
    {
        if (!ppBuf) return E_POINTER;
        if (vt == VT_BIN)
        {
            *ppBuf = val.Bin.pBuf;
            if (pLen) *pLen = val.Bin.nLen;
        }
        else
        {
            *ppBuf = NULL;
            if (pLen) *pLen = 0;
        }
        return (*ppBuf)?S_OK:E_FAIL;
    }

    DWORD GetValLen()
    {
        DWORD dwLen = 0;

        switch (vt)
        {
        case VT_NULL:
            dwLen = 0;
            break;
        case VT_INT:
            dwLen = sizeof(INT);
            break;
        case VT_UINT:
            dwLen = sizeof(UINT);
            break;
        case VT_FLOAT:
            dwLen = sizeof(FLOAT);
            break;
        case VT_DOUBLE:
            dwLen = sizeof(DOUBLE);
            break;
        case VT_BYTE:
            dwLen = sizeof(BYTE);
            break;
        case VT_STR:
            dwLen = val.Bin.nLen - 1; //��������β'\0'
            break;
        case VT_BIN:
            dwLen = val.Bin.nLen;
            break;
		case VT_ENUM:
			dwLen = sizeof(INT);
			break;
		case VT_DATASET:
			dwLen = sizeof(INT);
			break;
        default:
            dwLen = 0;
            break;
        }
        return dwLen;
    }
   //S_OK : д��ɹ�
    //����: д��ʧ��
    HRESULT WriteToStream(CSWStream* pStream)
    {
        if (vt == VT_NULL) return E_FAIL; //������д��������ֵ

        HRESULT hr;

        //д�������Ϣ
        BYTE nType = (BYTE)vt;
        WORD nValLen = GetValLen();

        if ( S_OK != pStream->Write(&nType, sizeof(BYTE), NULL) ){
            return E_FAIL;
        }

        //д������
        if (vt == VT_INT){
            hr = pStream->Write(&val.Int, nValLen, NULL);
        }
        else if (vt == VT_UINT){
            hr = pStream->Write(&val.uInt, nValLen, NULL);
        }
        else if (vt == VT_FLOAT){
            hr = pStream->Write(&val.Flt, nValLen, NULL);
        }
        else if (vt == VT_DOUBLE){
            hr = pStream->Write(&val.Dbl, nValLen, NULL);
        }
        else if (vt == VT_BYTE){
            hr = pStream->Write(&val.Byt, nValLen, NULL);
        }
        else if (vt == VT_STR || vt == VT_BIN) { //д���ִ�ʱnValLen = nBufLen -1
            if ( S_OK == pStream->Write(&nValLen, sizeof(WORD), NULL))
            {
                if( nValLen > 0 ) pStream->Write( val.Bin.pBuf, nValLen, NULL);

                hr = S_OK;
            }
            else{
                hr = E_FAIL;
            }
        }
		else if( vt == VT_ENUM )
		{
			hr = pStream->Write(&val.Int, nValLen, NULL);
		}
		else if( vt == VT_DATASET )
		{
			hr = pStream->Write(&val.Int, nValLen, NULL);
		}
        else{
            hr = E_FAIL; //δִ��д�����
        }

        return hr;
    }

    // S_OK : ��ȡ�ɹ�
    // ���� : ��ȡʧ��(ֵ��Ч)
    HRESULT ReadFromStream(CSWStream* pStream)
    {
        HRESULT hr(E_FAIL); this->Clear();

        BYTE nType = 0;

        if (S_OK != pStream->Read( &nType, sizeof(BYTE), NULL) ){
            return E_FAIL;
        }

        if (nType == VT_INT){
            hr = pStream->Read(&val.Int, sizeof(INT), NULL);
            vt = VT_INT;
        }
        else if (nType == VT_UINT){
            hr = pStream->Read(&val.uInt, sizeof(UINT), NULL);
            vt = VT_UINT;
        }
        else if (nType == VT_FLOAT){
            hr = pStream->Read(&val.Flt, sizeof(FLOAT), NULL);
            vt = VT_FLOAT;
        }
        else if (nType == VT_DOUBLE){
            hr = pStream->Read(&val.Dbl, sizeof(DWORD), NULL);
            vt = VT_DOUBLE;
        }
        else if (nType == VT_BYTE){
            hr = pStream->Read(&val.Byt, sizeof(BYTE), NULL);
            vt = VT_BYTE;
        }
        else if (nType == VT_STR){
            //���ַ�������
            WORD nStrLen;
            hr = pStream->Read(&nStrLen, sizeof(WORD), NULL);

            if ( hr == S_OK && nStrLen > 0){
                //���仺��
                BYTE* pBuf = (BYTE*)swpa_mem_alloc(nStrLen + 1);
                if (NULL == pBuf){
                    hr = E_OUTOFMEMORY;
                }
                else{
                    swpa_memset(pBuf, 0, nStrLen + 1);
                    //��������
                    hr = pStream->Read(pBuf, nStrLen, NULL);
					if (hr == S_OK){
	                    //�����ַ���
	                    hr = SetString((LPCSTR)pBuf);
	                }
					swpa_mem_free(pBuf);
                }
            }
        }
        else if (nType == VT_BIN){
            //��BIN���ݳ���
            WORD nBufLen;
            hr = pStream->Read(&nBufLen, sizeof(WORD), NULL);

            if ( hr == S_OK){
                //���仺��
                BYTE* pBuf = (BYTE*)swpa_mem_alloc(nBufLen);
                if (NULL == pBuf){
                    hr = E_OUTOFMEMORY;
                }
                else{
                    swpa_memset(pBuf, 0, nBufLen);

                    //��������
                    hr = pStream->Read(pBuf, nBufLen, NULL);
					if (hr == S_OK){
	                    //���ö���������
	                    hr = SetBin(pBuf, nBufLen);
	                }
					swpa_mem_free(pBuf);
                }
            }
        }
		else if( nType == VT_ENUM )
		{
			hr = pStream->Read(&val.Int, sizeof(INT), NULL);
			vt = VT_ENUM;
		}
		else if( nType == VT_DATASET )
		{
			hr = pStream->Read(&val.Int, sizeof(INT), NULL);
			vt = VT_ENUM;
		}
        else{
            hr = E_FAIL; //��Ч���Ͳ��������
        }

        if ( hr != S_OK) Clear();


        return hr;
    }
public:
    void Clear()
    {
        if ( (vt == VT_BIN || vt == VT_STR) &&
                val.Bin.pBuf != NULL )
        {
			swpa_mem_free(val.Bin.pBuf);
            val.Bin.pBuf = NULL;
            val.Bin.nLen = 0;
        }
        vt = VT_NULL;
    }

protected:
    HRESULT AllocBuf(WORD nLen)
    {
        val.Bin.pBuf = swpa_mem_alloc( nLen );//new INT[(nLen + sizeof(INT) - 1) / sizeof(INT)];
        if (val.Bin.pBuf != NULL)
        {
            val.Bin.nLen = nLen;
            swpa_memset(val.Bin.pBuf, 0, nLen);
        }
        return (val.Bin.pBuf == NULL)?E_OUTOFMEMORY:S_OK;
    }

    BOOL IsEqualString(LPCSTR str1, LPCSTR str2)
    {
        BOOL fSame = TRUE;
        LPCSTR p1 = str1;
        LPCSTR p2 = str2;

        while ( *p1 !='\0' && *p2 != '\0' )
        {
            if ( *p1 != *p2)
            {
                if ( ( (*p1 > 'a' && *p1 < 'z') && (*p1 - *p2) == 32)  ||
                        ( (*p1 > 'A' && *p1 < 'Z') && (*p2 - *p1) == 32)  )
                {
                    //���ֻ�Ǵ�Сд���첻������
                }
                else
                {
                    fSame = FALSE;
                    break;
                }
            }
            p1++;
            p2++;
        }

        if ( (*p1 == '\0') || (*p2 == '\0') )
        {
            fSame = (*p1 == *p2);
        }

        return fSame;
    }
};

//�ڵ�����
//��4λ
typedef enum _PARAM_NODE_TYPE
{
    NT_NULL = 0x0000,
    NT_SECTION = 0x0001,
    NT_KEY = 0x0002
}
PARAM_NODE_TYPE;

//�ڵ��־λ
//��5~8λ
typedef enum _PARAM_NODE_FLAG
{
    NF_NORMAL= 0x0000,
    NF_INFOMASK = 0x0010		//����Ϣ��־λ
}
PARAM_NODE_FLAG;

//������Ϣ,��12λ��ʾ,ǰ4λ����
typedef enum _PARAM_NODE_INFO
{
    NI_NONE = 0x0000,
    NI_DEFAULT_VAL = 0x0001,	    //Ĭ��ֵ
    NI_SCALE = 0x0002,				//��Χ
    NI_COMMENT = 0x0004,			//ע��
    NI_CHNAME = 0x008,			    //������
    NI_RANK	= 0x0010,				//����
    NI_ENUM	= 0x0011,				//ö��

    NI_ALL = 0x0FFF					//ȫ����Ч
}
_PARAM_NODE_INFO;

class CSWParamNodeEnum;
interface IEnumNodeCallBack;

//CParamNode ����
class CSWParamNode
{
public:
    CSWParamNode(WORD nFlag = NT_NULL);
    CSWParamNode(CSWParamNode& src);
    ~CSWParamNode();

public: //��־λ�ж�
    BOOL IsSectionNode()
    {
        return (m_nFlag & NT_SECTION);
    }

    BOOL IsKeyNode()
    {
        return (m_nFlag & NT_KEY);
    }

    BOOL IsNullNode()
    {
        return (m_nFlag & 0x0003) == NT_NULL;
    }

    BOOL HasExtInfo()
    {
        return (m_nFlag & NF_INFOMASK);
    }

public: //ֵ����
    template<typename T>
    HRESULT SetValue(const T& value)
    {
        m_Val = value;
        return S_OK;
    }
    HRESULT SetEnumValue(INT value)
    {
        m_Val = value;
        m_Val.vt = PARAM_VAR::VT_ENUM;

        return S_OK;
    }
    HRESULT SetSetValue(INT value)
    {
        m_Val = value;
        m_Val.vt = PARAM_VAR::VT_DATASET;

        return S_OK;
    }
    HRESULT SetValue(LPCSTR szValue)
    {
        return m_Val.SetString(szValue);
    }

    HRESULT SetValue(void* pBuf, WORD nLen)
    {
        return m_Val.SetBin(pBuf, nLen);
    }

    template<typename T>
    HRESULT SetDefaultValue(const T& value)
    {
        m_DefaultVal = value;
        m_nInfoMask |= NI_DEFAULT_VAL;
        m_nFlag |= NF_INFOMASK;
        return S_OK;
    }

    HRESULT SetDefaultValue(LPCSTR szDefault)
    {
        m_DefaultVal.SetString(szDefault);
        m_nInfoMask |= NI_DEFAULT_VAL;
        m_nFlag |= NF_INFOMASK;
        return S_OK;
    }

    HRESULT SetDefaultValue(void* pBuf, WORD nLen)
    {
        m_DefaultVal.SetBin(pBuf, nLen);
        m_nInfoMask |= NI_DEFAULT_VAL;
        m_nFlag |= NF_INFOMASK;
        return S_OK;
    }

    template<typename T>
    HRESULT SetScale(const T& Min, const T& Max)
    {
        m_MinVal = Min;
        m_MaxVal = Max;
        m_nInfoMask |= NI_SCALE;
        m_nFlag |= NF_INFOMASK;
        return S_OK;
    }

    HRESULT SetComment( LPCSTR szComment )
    {
        m_strComment = szComment;
        m_nInfoMask |= NI_COMMENT;
        m_nFlag |= NF_INFOMASK;
        return S_OK;
    }

    HRESULT SetChName( LPCSTR szChName)
    {
        m_strChName = szChName;
        m_nInfoMask |= NI_CHNAME;
        m_nFlag |= NF_INFOMASK;
        return S_OK;
    }

    template<typename T>
    HRESULT SetRank( const T& Rank )
    {
        m_Rank = Rank;
        m_nInfoMask |= NI_RANK;
        m_nFlag |= NF_INFOMASK;

        return S_OK;
    }

    HRESULT SetRWFlag( INT IsOnlyRead )
    {
        m_IsOnlyRead = IsOnlyRead;
    }

    HRESULT GetRWFlag( INT &IsOnlyRead )
    {
        IsOnlyRead = m_IsOnlyRead ;
    }
    HRESULT SetMaxLen( INT iMaxLen )
    {
        m_MaxVal = iMaxLen;
    }

    HRESULT SetEnum( LPCSTR szValueEnum )
    {
       m_strEnum = szValueEnum; m_Val.vt = PARAM_VAR::VT_ENUM;
    }
    HRESULT SetSet( LPCSTR szValueSet)
    {
       m_strSet = szValueSet;m_Val.vt = PARAM_VAR::VT_DATASET;
    }

public:
    //�洢, SimpMode��ֻ����ֵ,�����渽����Ϣ
    HRESULT SaveToStream(CSWStream* pStream, BOOL fSimpMode);

    HRESULT RestoreFromStream(CSWStream* pStream);

    HRESULT ToXml( CSWString &strXml );

public:	//�ӽڵ����
    //���
    HRESULT AddSubNode(CSWParamNode* pNode);
    //ɾ��
    HRESULT RemoveSubNode( LPCSTR szNodeName, PARAM_NODE_TYPE nType );
    //ȡ��ָ�����ƺ����͵��ӽڵ�
    CSWParamNode* GetSubNode( LPCSTR szNodeName, PARAM_NODE_TYPE nType );
    //ȡ����һ���ӽڵ�
    CSWParamNode* GetNextSubNode( CSWParamNode* pNode );
    //����
    HRESULT OnEnum(IEnumNodeCallBack* pCallBack);

public:	//�ϲ�����
    CSWParamNode& Assign( const CSWParamNode& cSrcNode);	//����
    CSWParamNode& AddNew( const CSWParamNode& cSrcNode, BOOL fReplace); //�ϲ�

    CSWParamNode& operator =(const CSWParamNode& cSrcNode) //����
    {
        return Assign(cSrcNode);
    }

public:
    HRESULT ReInit(); //�س�ʼ��

protected:
    HRESULT Clear();	//����,����������ת��ʱ

    HRESULT SaveAsKeyNode(CSWStream* pStream, BOOL fSimpMode);
    HRESULT SaveAsSectionNode(CSWStream* pStream, BOOL fSimpMode);

    HRESULT ReadAsKeyNode(CSWStream* pStream, WORD nFlag);
    HRESULT ReadAsSectionNode( CSWStream* pStream, WORD nFlag );

private:
    HRESULT KeyNodeToXml( CSWString &strXml );

    HRESULT SectionNodeToXml( CSWString &strXml );

public:	//������Ϣ
    WORD m_nFlag;
    WORD m_nInfoMask;
    DWORD m_IsOnlyRead;

    CSWParamVar m_strName;
    CSWParamVar m_Val;
    CSWParamVar m_DefaultVal;
    CSWParamVar m_MinVal;
    CSWParamVar m_MaxVal;
    CSWParamVar m_Rank;
    CSWParamVar m_strComment;
    CSWParamVar m_strChName;
    CSWParamVar m_strEnum;
    CSWParamVar m_strSet;

    CSWParamNode* m_pParent;



protected:	 //�ӽڵ�
    CSWParamNodeEnum* m_pSubNode;
};


//CParamNodeEnum ����
class CSWParamNodeEnum : public CSWEnumBase<CSWParamNode*>
{
public:
    ~CSWParamNodeEnum()
    {
        EnumBase_Reset();
        while (S_OK == EnumBase_Remove())
        {
            //SystemDog();
        }
    }
protected:
    virtual HRESULT OnDelNode(NODE* pNode)
    {
//        SAFE_DELETE(pNode->Dat);
        return S_OK;
    }
};

interface IEnumNodeCallBack
{
    STDMETHOD(OnEnumNode)(CSWParamNode* pNode) = 0;
};

#endif

