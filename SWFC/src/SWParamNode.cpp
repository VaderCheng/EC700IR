#include "SWParamNode.h"
//#include "assert.h"

//#include "hvutils.h"

#define RTN_IF_FAILED(fun) { HRESULT hr = fun; if(FAILED(hr)) return hr; }

//CParamNodeʵ��
CSWParamNode::CSWParamNode(WORD nFlag)
    :m_nFlag(nFlag)
    ,m_nInfoMask(NI_NONE)
    ,m_pSubNode(NULL)
{
    m_strName = "NULL";
	if ( nFlag & NT_SECTION )
	{
		m_pSubNode = new CSWParamNodeEnum;
		//assert(m_pSubNode);
	}
	// ��ʼ��ֻ�����
	this->SetRWFlag( 0 );
}

CSWParamNode::CSWParamNode(CSWParamNode& src)
{
	Assign(src);
}

CSWParamNode::~CSWParamNode()
{
	Clear();
}

HRESULT CSWParamNode::Clear()
{
//	SAFE_DELETE(m_pSubNode);
	//m_pParent = NULL;
	m_nFlag = NT_NULL;
	m_nInfoMask = NI_NONE;

	m_strName.Clear();
	m_Val.Clear();
	m_DefaultVal.Clear();
	m_MinVal.Clear();
	m_MaxVal.Clear();
	m_Rank.Clear();
	m_strComment.Clear();
	m_strChName.Clear();

	return S_OK;
}

HRESULT CSWParamNode::ReInit() //�س�ʼ��
{
	if(m_nFlag & NT_SECTION)
	{
//		SAFE_DELETE(m_pSubNode);
		m_pSubNode = new CSWParamNodeEnum;
//		assert(m_pSubNode);
	}
	else if(m_nFlag & NT_KEY)
	{
		m_nFlag &= ~NF_INFOMASK;
		m_nInfoMask = NI_NONE;

		m_Val.Clear();
		m_DefaultVal.Clear();
		m_MinVal.Clear();
		m_MaxVal.Clear();
		m_Rank.Clear();
		m_strComment.Clear();
		m_strChName.Clear();
	}
	return S_OK;
}


//�ӽ�����
HRESULT CSWParamNode::AddSubNode(CSWParamNode* pNode)
{
	if (!pNode) return E_POINTER;

	if ( !IsSectionNode() ) return S_FALSE;

	//pNode->m_pParent = this;

	return m_pSubNode->EnumBase_Add(pNode);
}

HRESULT CSWParamNode::RemoveSubNode( LPCSTR szNodeName, PARAM_NODE_TYPE nType )
{
	if (!szNodeName) return E_POINTER;

	if( !IsSectionNode() ) return S_FALSE;

	CSWParamNode* pNode = GetSubNode( szNodeName, nType );

	return m_pSubNode->EnumBase_Remove(pNode);
}

CSWParamNode* CSWParamNode::GetSubNode( LPCSTR szNodeName, PARAM_NODE_TYPE nType )
{
	if ( !szNodeName ||
		  !IsSectionNode() )
	{
		return NULL;
	}

	CSWParamNode* pNode;
	BOOL fFound = FALSE;
	m_pSubNode->EnumBase_Reset();
	while(S_OK == m_pSubNode->EnumBase_Next(1, &pNode, NULL))
	{
		if( pNode->m_strName == szNodeName &&
			( (pNode->m_nFlag & 0x0003) == nType ) )
		{
			fFound = TRUE;
			break;
		}
	}

	return fFound?pNode:NULL;
}

CSWParamNode* CSWParamNode::GetNextSubNode( CSWParamNode* pNode )
{
	if( !IsSectionNode() || NULL == m_pSubNode) return NULL;

	if(pNode == NULL)
	{
		m_pSubNode->EnumBase_Reset();
	}

	CSWParamNode* pTmp;
	HRESULT  hr = m_pSubNode->EnumBase_Next(1, &pTmp, NULL);

	return (hr == S_OK)?pTmp:NULL;
}

//��������
HRESULT CSWParamNode::OnEnum(IEnumNodeCallBack* pCallBack)
{
	if (pCallBack == NULL) return E_POINTER;

	pCallBack->OnEnumNode(this);

	if (!IsSectionNode()) return S_OK;

	CSWParamNode* pNode = NULL;
	while( pNode = GetNextSubNode(pNode) )
	{
		if(pNode->IsNullNode()) continue;
		pNode->OnEnum(pCallBack);
	}

	//���ؽ�����־
	CSWParamNode cNullNode(NT_NULL);
	pCallBack->OnEnumNode(&cNullNode);

	return S_OK;
}

//����
CSWParamNode& CSWParamNode::Assign( const CSWParamNode& cSrcNode)
{
	Clear(); //���Դ�ڵ�Ϊ��,��Ŀ��ڵ�Ҳ�ᱻ�ÿ�

	CSWParamNode* pSrcNode = const_cast<CSWParamNode*>(&cSrcNode);

	m_strName = pSrcNode->m_strName;

	if(pSrcNode->IsSectionNode() &&
		pSrcNode->m_pSubNode )
	{
		m_nFlag |= NT_SECTION;
		m_pSubNode = new CSWParamNodeEnum;
//		assert(m_pSubNode);

		CSWParamNode* pNode = NULL;
		CSWParamNode* pNewNode = NULL;
		while( pNode = pSrcNode->GetNextSubNode(pNode) )
		{
			if(pNode->IsNullNode()) continue;
			pNewNode = new CSWParamNode;
			pNewNode->Assign(*pNode);
			AddSubNode(pNewNode);
		}
	}
	else if(pSrcNode->IsKeyNode())
	{
		m_nFlag |= NT_KEY;
		m_Val = pSrcNode->m_Val;

		if(pSrcNode->HasExtInfo())
		{
			m_nFlag |= NF_INFOMASK;
			m_nInfoMask = pSrcNode->m_nInfoMask;
			m_DefaultVal = pSrcNode->m_DefaultVal;
			m_MinVal = pSrcNode->m_MinVal;
			m_MaxVal = pSrcNode->m_MaxVal;
			m_Rank = pSrcNode->m_Rank;
			m_strComment = pSrcNode->m_strComment;
			m_strChName = pSrcNode->m_strChName;
		}
	}

	return *this;
}

//�ϲ�,��fReplaceΪFALSE��ֻ����׷��
CSWParamNode& CSWParamNode::AddNew( const CSWParamNode& cSrcNode, BOOL fReplace)
{
	CSWParamNode* pSrcNode = const_cast<CSWParamNode*>(&cSrcNode);

	if ( m_strName != pSrcNode->m_strName )	//������ͬ���ڵ���ܺϲ�
	{
		return *this;
	}

	//����Ƕ���Key�ڵ㲢���Ǹ�����ʽ��ֵ
	if ( IsKeyNode() )
	{
		if (fReplace && pSrcNode->IsKeyNode() )
		{
			m_Val = pSrcNode->m_Val;

			//���Դ�ڵ�û�и�����Ϣ,��ֻ���и�ֵ
			if(pSrcNode->HasExtInfo())
			{
				WORD dwFlag = pSrcNode->m_nInfoMask;

				if(dwFlag & NI_DEFAULT_VAL)
				{
					SetDefaultValue(pSrcNode->m_DefaultVal);
				}

				if(dwFlag & NI_SCALE)
				{
					SetScale(pSrcNode->m_MinVal, pSrcNode->m_MaxVal);
				}

				if(dwFlag & NI_COMMENT)
				{
					SetComment( (LPCSTR)pSrcNode->m_strComment);
				}

				if(dwFlag & NI_CHNAME)
				{
					SetChName((LPCSTR)pSrcNode->m_strChName);
				}

				if(dwFlag & NI_RANK)
				{
					SetRank(pSrcNode->m_Rank);
				}
			}
		}

		return *this;
	}

	//ֻ�ж���Section�ڵ�Ž��кϲ�
	if ( !IsSectionNode() ||
		 !pSrcNode->IsSectionNode() )
	{
		return *this;
	}

	CSWParamNode* pSrcSubNode = NULL;
	CSWParamNode* pSubNode;
	PARAM_NODE_TYPE nType;

	while( pSrcSubNode = pSrcNode->GetNextSubNode(pSrcSubNode) )
	{
		if(pSrcSubNode->IsNullNode()) continue;
		nType = (PARAM_NODE_TYPE)(pSrcSubNode->m_nFlag&0x0003);
		if ( pSubNode = this->GetSubNode((LPCSTR)(pSrcSubNode->m_strName), nType)  )
		{
			pSubNode->AddNew( *pSrcSubNode, fReplace);	//����ҵ�ƥ����ӽڵ�, ����кϲ�
		}
		else
		{
			pSubNode = new CSWParamNode;
			if(pSubNode)
			{
				*pSubNode = *pSrcSubNode;
				AddSubNode(pSubNode);
			}
		}
	}
	return *this;
}
HRESULT CSWParamNode::SaveToStream(CSWStream* pStream, BOOL fSimpMode)
{
	if ( !pStream) return E_POINTER;

	HRESULT hr(E_FAIL);
	if(IsNullNode())
	{
		//�����NULL�ڵ�ֻд���־ֵ(���ڴ洢������־)
		hr = pStream->Write(&m_nFlag, sizeof(m_nFlag), NULL);
	}
	else if (IsKeyNode())
	{
		hr = SaveAsKeyNode(pStream, fSimpMode);
	}
	else if (IsSectionNode())
	{
		hr = SaveAsSectionNode(pStream, fSimpMode);
	}

	return hr;
}
//�洢����
//SimpMode: flag name value
//AdvMode: flag infomask name value [defaultvalue] [scale] [comment] [chname] [rank]
HRESULT CSWParamNode::SaveAsKeyNode(CSWStream* pStream, BOOL fSimpMode)
{
    CSWString strName = CSWString( (const CHAR*)m_strName);
	if( !pStream || strName.Compare("") ==0) return E_INVALIDARG;

	WORD nFlag = fSimpMode ? (m_nFlag &~ NF_INFOMASK) : m_nFlag;
    // ����ڵ���
	RTN_IF_FAILED(pStream->Write(&nFlag, sizeof(nFlag), NULL));
	// ����ֻ�����
	INT IsOnlyRead = 0 ; this->GetRWFlag( IsOnlyRead );
	RTN_IF_FAILED(pStream->Write(&IsOnlyRead, sizeof(IsOnlyRead), NULL));

	BOOL fWriteInfoMask = !fSimpMode && (nFlag & NF_INFOMASK);

	if( fWriteInfoMask ){
		RTN_IF_FAILED(pStream->Write(&m_nInfoMask, sizeof(m_nInfoMask), NULL));
	}

	// ���ֺ�ֵ�ر�
	RTN_IF_FAILED(m_strName.WriteToStream(pStream));
	RTN_IF_FAILED(m_Val.WriteToStream(pStream));

	if( !fWriteInfoMask ) return S_OK;


	if( m_nInfoMask & NI_DEFAULT_VAL){
		RTN_IF_FAILED(m_DefaultVal.WriteToStream(pStream));
	}

	if(	 m_nInfoMask & NI_SCALE){
		RTN_IF_FAILED(m_MinVal.WriteToStream(pStream));
		RTN_IF_FAILED(m_MaxVal.WriteToStream(pStream));
	}

	if(m_nInfoMask & NI_COMMENT){
		RTN_IF_FAILED(m_strComment.WriteToStream(pStream));
	}

	if(m_nInfoMask & NI_CHNAME){
		RTN_IF_FAILED(m_strChName.WriteToStream(pStream));
	}

	if(m_nInfoMask & NI_RANK){
		RTN_IF_FAILED(m_Rank.WriteToStream(pStream));
	}

	return S_OK;
}
HRESULT CSWParamNode::SaveAsSectionNode(CSWStream* pStream, BOOL fSimpMode)
{
    CSWString strName = CSWString( (const CHAR*)m_strName);
	if ( !pStream || strName.Compare("") ==0 ) return E_INVALIDARG;


    RTN_IF_FAILED(pStream->Write(&m_nFlag, sizeof(m_nFlag), NULL));

    RTN_IF_FAILED(m_strName.WriteToStream(pStream));

	HRESULT hr(S_OK);
	CSWParamNode* pNode;
	m_pSubNode->EnumBase_Reset();
	while(S_OK == m_pSubNode->EnumBase_Next(1, &pNode, NULL) )
	{
		//ͨ��Assign�п��ܽ�ĳ�ڵ���ΪNULL,�洢ʱ������Щ�ڵ�
		if(pNode->IsNullNode()) continue;

		hr = pNode->SaveToStream(pStream, fSimpMode);
		if(FAILED(hr)) break;
	}

	if ( !FAILED(hr) )
	{
		CSWParamNode cNode(NT_NULL);
		hr = cNode.SaveToStream(pStream, fSimpMode);	//д��һ���սڵ���Ϊ������־
	}

	return hr;
}
/**
 * @brief ���ļ����лָ����ݽ��
 * @param [in] pStream : �������ļ���
 * @return - S_OK : �ɹ� - E_FAIL : ʧ��
 */
HRESULT CSWParamNode::RestoreFromStream(CSWStream *pStream)
{
	if ( !pStream ) return E_INVALIDARG;

    // ���ԭ������
	// this->Clear();

	RTN_IF_FAILED(pStream->Read(&m_nFlag, sizeof(m_nFlag), NULL));

	HRESULT hr = E_FAIL;

	if( (m_nFlag & 0x03) == NT_NULL) //�ս��ֻ��Ϊ������־,ʵ�ʲ������в����пսڵ�
	{
		hr = S_FALSE;
	}
	else if(m_nFlag & NT_KEY)
	{// ��ȡ�ڵ�ֵ
		hr = this->ReadAsKeyNode(pStream, m_nFlag);
	}
	else if(m_nFlag & NT_SECTION)
	{// ��ȡ���ڵ�ֵ
		hr = this->ReadAsSectionNode(pStream, m_nFlag);
	}

	return hr;
}
/**
 * @brief ��ȡKEY�������
 * @param [in] pStream : �������ļ���
 * @param [in] nFlag : ����־
 * @return - S_OK : �ɹ� - E_FAIL : ʧ��
 */
HRESULT CSWParamNode::ReadAsKeyNode(CSWStream* pStream, WORD nFlag)
{
	if(!pStream) return E_INVALIDARG;

	// ��ȡ��ǰ��д���
	INT IsOnlyRead = 0 ;
	RTN_IF_FAILED(pStream->Read(&IsOnlyRead, sizeof(IsOnlyRead), NULL));
    this->SetRWFlag( IsOnlyRead );

	if(nFlag & NF_INFOMASK)
	{
		RTN_IF_FAILED(pStream->Read(&m_nInfoMask,sizeof(m_nInfoMask),NULL));
	}

	m_strName.ReadFromStream(pStream);
	m_Val.ReadFromStream(pStream);

	if(m_nInfoMask & NI_DEFAULT_VAL)
	{
		m_DefaultVal.ReadFromStream(pStream);
	}

	if(m_nInfoMask & NI_SCALE)
	{
		m_MinVal.ReadFromStream(pStream);
		m_MaxVal.ReadFromStream(pStream);
	}

	if(m_nInfoMask & NI_COMMENT)
	{
		m_strComment.ReadFromStream(pStream);
	}

	if(m_nInfoMask & NI_CHNAME)
	{
		m_strChName.ReadFromStream(pStream);
	}

	if (m_nInfoMask & NI_RANK)
	{
		m_Rank.ReadFromStream(pStream);
	}

	return S_OK;
}
/**
 * @brief ��ȡSection�������
 * @param [in] pStream : �������ļ���
 * @return - S_OK : �ɹ� - E_FAIL : ʧ��
 */
HRESULT CSWParamNode::ReadAsSectionNode( CSWStream* pStream, WORD nFlag )
{
	if(!pStream) return E_INVALIDARG;

	SAFE_DELETE(m_pSubNode);
	m_pSubNode =  new CSWParamNodeEnum;
	if (m_pSubNode == NULL) return E_OUTOFMEMORY;

	RTN_IF_FAILED(m_strName.ReadFromStream(pStream));
//	RTN_IF_FAILED(m_strChName.ReadFromStream(pStream));

	HRESULT hr(E_FAIL);
	while( true )
	{
		CSWParamNode* pNode = new CSWParamNode;
		if ( pNode == NULL)
		{
			hr = E_OUTOFMEMORY;
			break;
		}

		hr = pNode->RestoreFromStream(pStream);
		if (hr == S_OK)
		{
			hr = m_pSubNode->EnumBase_Add(pNode);
			continue;
		}
		else if (hr == S_FALSE)	//����������־
		{
			hr = S_OK;
		}

		//�������߶�ȡʧ��ֱ������
		SAFE_DELETE(pNode);
		break;
	}

	return hr;

}
HRESULT CSWParamNode::ToXml( CSWString &strXml )
{
    HRESULT hr = S_OK;
 	if( (m_nFlag & 0x03) == NT_NULL) //�ս��ֻ��Ϊ������־,ʵ�ʲ������в����пսڵ�
	{
		hr = S_FALSE;
	}
	else if(m_nFlag & NT_KEY)
	{// ��ȡ�ڵ�ֵ
		hr = this->KeyNodeToXml( strXml );
	}
	else if(m_nFlag & NT_SECTION)
	{// ��ȡ���ڵ�ֵ
		hr = this->SectionNodeToXml( strXml );
	}

	return hr;
}


HRESULT CSWParamNode::SectionNodeToXml( CSWString &strXml )
{
    CSWString strSectionXml;

    CSWString strName = CSWString( (const CHAR*)m_strName);

    if( 0 == strName.Compare( "HvParam" )){
        strSectionXml.Append( "<HvParam>\r\n" );
    }
    else if( strName.Compare("NULL") == 0) return E_FAIL;
    else{
        strSectionXml.Append("<SECTION name=\"");
        strSectionXml.Append(m_strName);
        strSectionXml.Append("\" cname=\"");
        strSectionXml.Append( m_strChName );
        strSectionXml.Append("\">\r\n" );
    }

    CSWParamNode* pKeyNode = NULL; HRESULT hr = S_OK;
    while( (pKeyNode = this->GetNextSubNode( pKeyNode )) != NULL )
     {
        hr = pKeyNode->ToXml( strSectionXml );
     }

    strXml.Append(strSectionXml);

    if( 0 == strName.Compare( "HvParam" )){
        strXml.Append( "</HvParam>\r\n" );
    }
    else
        strXml.Append("</SECTION>\r\n");

    return hr;
}

HRESULT CSWParamNode::KeyNodeToXml( CSWString &strXml )
{
    CSWString strNodeName = CSWString( (const CHAR*)m_strName);
    if( strNodeName.Compare("NULL") == 0) return E_FAIL;

    // ����key���
    strXml.Append("<KEY name=\"");
    strXml.Append( m_strName );
    strXml.Append("\">\r\n" );

    CHAR *buffer = NULL;
    if( m_Val.GetValLen() < 512 )
        buffer = new CHAR[512];
    else
        buffer = new CHAR[m_Val.GetValLen() + 1];


    CSWString strValue ,strDefaultValue, strMin , strMan
    , strChName , strRank , strComment , strType;

    switch( m_Val.vt )
    {
        case _PARAM_VAR::VT_INT:
            swpa_sprintf( buffer , "%d", (INT)m_Val);
            strType = "INT" ;
			break;
        case _PARAM_VAR::VT_FLOAT:
             swpa_sprintf( buffer , "%f", (FLOAT)m_Val);
             strType = "FLOAT" ;
			 break;
        case _PARAM_VAR::VT_STR:
             swpa_sprintf( buffer , "%s", (LPCSTR)m_Val);
             strType = "STR" ;
			 break;
		case _PARAM_VAR::VT_ENUM:
			swpa_sprintf( buffer, "%d", (INT)m_Val);
			strType = "ENUM" ;
			break;
		case _PARAM_VAR::VT_DATASET:
			swpa_sprintf( buffer, "%d", (INT)m_Val);
			strType = "SET" ;
			break;
        default:
            break;
    }

    strValue = CSWString( buffer);

    // ���ֵ
    strXml.Append("<VALUE type=\"" );
    strXml.Append(strType  + "\">");
    strXml.Append( strValue + "</VALUE>");

    strXml.Append("<CHNAME type=\"STR\">" );
    strXml.Append( m_strChName );
    strXml.Append( "</CHNAME>");

	const CHAR* pszTemp = (const CHAR*)m_strComment;
	if( pszTemp != NULL && swpa_strlen(pszTemp) > 0 )
	{
		strXml.Append("<COMMENT type=\"STR\">" );
		strXml.Append( m_strComment );
		strXml.Append( "</COMMENT>");
	}

	if( (m_nInfoMask & NI_DEFAULT_VAL) == NI_DEFAULT_VAL )
	{
		switch( m_DefaultVal.vt )
		{
		case _PARAM_VAR::VT_INT:
			swpa_sprintf( buffer , "%d", (INT)m_DefaultVal);
			strDefaultValue=CSWString(buffer);
			break;
		case _PARAM_VAR::VT_FLOAT:
			swpa_sprintf( buffer , "%f", (FLOAT)m_DefaultVal);
			strDefaultValue=CSWString(buffer);
			break;
		case _PARAM_VAR::VT_STR:
			swpa_sprintf( buffer , "%s", (LPCSTR)m_DefaultVal);
			strDefaultValue=CSWString(buffer);
			break;
		case _PARAM_VAR::VT_ENUM:
			swpa_sprintf( buffer, "%d", (INT)m_DefaultVal);
			strDefaultValue = CSWString(buffer);
			break;
		case _PARAM_VAR::VT_DATASET:
			swpa_sprintf( buffer, "%d", (INT)m_DefaultVal);
			strDefaultValue = CSWString(buffer);
			break;
		default:
			break;
		}
		// ���Ĭ��ֵ
		strXml.Append("<DEFAULT type=\"" );

		if( m_Val.vt == _PARAM_VAR::VT_ENUM ||  _PARAM_VAR::VT_DATASET ==  m_Val.vt )
		{
			strXml.Append("INT\">");
		}
		else
		{
			strXml.Append(strType + "\">");
		}

		strXml.Append( strDefaultValue + "</DEFAULT>");
	}

	// zhaopy
	// ֻ��INT UINT FLOAT ��������С���ֵ��

	if( (m_nInfoMask & NI_SCALE) == NI_SCALE )
	{
		// ��ȡ��Сֵ
		swpa_sprintf( buffer , "%d", (INT)m_MinVal);
		strMin = CSWString( buffer);

		strXml.Append("<MIN type=\"" );
		strXml.Append("INT\">");
		strXml.Append( CSWString(buffer) + "</MIN>");

		// ��ȡ���ֵ
		swpa_sprintf( buffer , "%d", (INT)m_MaxVal);
		strMin = CSWString( buffer);

		strXml.Append("<MAX type=\"" );
		strXml.Append("INT\">");
		strXml.Append( CSWString(buffer) + "</MAX>");
	}

    // ��ȡ�ȼ�
    swpa_sprintf( buffer , "%d", (INT)m_Rank);
    strRank = CSWString( buffer);

    strXml.Append("<RANK type=\"" );
    strXml.Append("INT\">");
    strXml.Append( CSWString(buffer) + "</RANK>");

    const CHAR *szEnum = m_strEnum;
    if( szEnum != NULL )
    {
         strXml.Append("<ENUM type=\"" );
         strXml.Append("STR\">");
         strXml.Append( CSWString(szEnum) + "</ENUM>");
    }

    const CHAR *strSet = m_strSet;
    if( strSet != NULL )
    {
         strXml.Append("<SET type=\"" );
         strXml.Append("STR\">");
         strXml.Append( CSWString(strSet) + "</SET>");
    }

    INT IsOnlyRead = 0 ; this->GetRWFlag(IsOnlyRead);
    strXml.Append("<ONLYREAD type=\"" );
    strXml.Append("INT\">");
    swpa_sprintf( buffer , "%d", IsOnlyRead );
    strXml.Append( CSWString(buffer) + "</ONLYREAD>");

    // �ýڵ����
    strXml.Append("</KEY>\r\n");

    delete []buffer;
    return S_OK;
}




