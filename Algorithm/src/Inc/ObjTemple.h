#ifndef _OBJTEMPLE_INCLUDED__
#define _OBJTEMPLE_INCLUDED__

// ģ��ƥ�����ݽṹ

#include "swimageobj.h"
#include "hvbaseobj.h"

using namespace HiVideo;

#define MAX_OBJMODEL_WIDTH		300			// ģ������ȣ����أ�
#define MAX_OBJMODEL_HEIGHT		200			// ģ�����߶ȣ����أ�

class CObjTemple
{
public:
	CObjTemple();
	~CObjTemple();

	HRESULT UpdateTemple(HV_COMPONENT_IMAGE *pImg, bool fFastMem = FALSE);
	HRESULT CalcMeanVar();			// ����ģ�Ͳ���

	void Clear();

	IReferenceComponentImage *m_pObjImg;		// ģ������

	float m_fltVar;			// ģ��Var
	float m_fltMean;		// ģ��Mean

protected:
	int m_nWidth;			// ģ�Ϳ��
	int m_nHeight;			// ģ�͸߶�
private:
	CFastMemAlloc m_cFastStack;

};

#endif // _OBJTEMPLE_INCLUDED__
