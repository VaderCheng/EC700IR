#ifndef _OBJMODEL_INCLUDED__
#define _OBJMODEL_INCLUDED__

// ģ��ƥ����

#include "ObjTemple.h"
#include "hist.h" // ֱ��ͼ���� added by zoul, 20110815

using namespace signalway; // �й�ֱ��ͼ�Ľṹ�Ͳ�������signalway���ֿռ���

class CObjMatch
{
public:
	CObjMatch();
	~CObjMatch();

	void Clear();

	HRESULT UpdateTemple(HV_COMPONENT_IMAGE *pImg, HiVideo::CRect rc, bool fFastMem = FALSE);
	HRESULT PrepareIntegral(HV_COMPONENT_IMAGE *pImg);			// ����ͼ����

	HRESULT GetBestMatch(HV_COMPONENT_IMAGE *pImg, HiVideo::CRect *rcNect, 
		bool fUseHistMatch, float fltThreld, bool fNewMethod, int iDelayCount, int iMaxDiff, bool fIsDay);		// ģ��ƥ�䣬�������λ

	HRESULT TempleMatch(HV_COMPONENT_IMAGE *pImg, CObjTemple *pTemple, HiVideo::CRect *rcNect, 
		bool fUseHistMatch, float fltThreld, int iDelayCount, int iMaxDiff, bool fIsDay);		// ģ��ƥ�䣬�������λ��

	static int X_SCAN_STEP;
	static int Y_SCAN_STEP;
protected:
	CObjTemple *m_pTmpOrg;			// ԭʼ��ģ�ͣ���ʱֻ����1��scale��

private:
	CFastMemAlloc m_cFastStack;

	// ����ͼʹ�õı���
	WORD16 				m_nItgImageWidth;				//����ͼ���.
	WORD16 				m_nItgImageHeight;				//����ͼ�߶�.
	WORD16 				m_nItgStrideWidth;				//����ͼ�洢Stride���.

	PDWORD32 			m_pdwItgImage;					//һ�����ͼ����.
	PDWORD32 			m_pdwItgSqImage;				//ƽ������ͼ����.

	WORD16				m_nItgBufStrideWidth;			//����ͼ�ڴ�ߴ�
	WORD16				m_nItgBufMaxHeight;

	PDWORD32			m_pdwItgBuf;					//����ͼ�ڴ�ָ��
	PDWORD32			m_pdwItgSqBuf;

	bool                m_bHistSaved ;                  // ģ���ֱ��ͼ�Ƿ񱻱��棻 added by zoul, 20110817
	histogram           m_histMatched ;                 // ֱ��ͼ�ṹ�� added by zoul, 20110815
	// zhaopy
	histogram			m_histBase;
	int					m_iDiffCount;
	static const int    MAX_DIFF = 18;
	static const int	CMP_DIFFCOUNT = 8;
	float				m_rgdbDiff[CMP_DIFFCOUNT];

	histogram			m_histBaseOld;
	//ֱ��ͼ����, �̶�Ϊ16*16
	int	m_rgHistBase[16*16];
	int m_nHistBaseType; //0Ϊ��άֱ��ͼ,1Ϊһάֱ��ͼ
};

#endif // _OBJMODEL_INCLUDED__
