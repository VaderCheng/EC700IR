#ifndef _ICHARQUOTIETYMATRIX_H_
#define _ICHARQUOTIETYMATRIX_H_

//�ַ�Ȩֵ����ӿ�
class ICharQuotietyMatrix
{
public:
	STDMETHOD( Initialize )() = 0;
	STDMETHOD( GetQuotiety )( int iCharIndex, int iCharPos, float &fltQuotiety ) = 0;
};

#endif
