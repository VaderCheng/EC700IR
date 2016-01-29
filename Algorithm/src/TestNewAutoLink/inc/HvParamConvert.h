#ifndef _PARAM_CONVERT_H
#define _PARAM_CONVERT_H

#include "tinyxml.h"
#include "hvinterface.h"

extern HRESULT ConvertParam2Xml(
								BYTE8* pbParamBuf,
								UINT nBufLen,
								TiXmlDocument* pDoc,
								INT nRank,
								BOOL fShowExtInfo,
								BOOL fShowRank
								);

extern HRESULT ConvertXml2Param(
								TiXmlDocument* pDoc, 
								BYTE8* pbParamBuf,	
								UINT& nBufLen,	//�����С,����ʱΪʵ�ʲ������ݳ���
								BOOL fSimpMode
								);

#endif
