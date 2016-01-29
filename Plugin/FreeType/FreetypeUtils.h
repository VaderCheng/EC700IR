/****************************************************************************
                 ʹ��Freetype��ȡʸ���ֿ��ַ�����

ʹ��freetypeע�����
1��ʹ��IDE��Makefile���ú�freetype���ͷ�ļ�·����
2��freetypeʹ�õ���unicode�룬��˱��뽫��unicode�ַ�����ת����

���ܣ�
1��֧��GB2312��UTF8�����ʽ������(ǰ��ʹ�ò��������ʹ��ת���㷨)�������UTF8�����뱣֤���ֱ����ʽΪUTF8�������������롣
2��֧���ҽ�֧��\n��\t�����ַ����ֱ��Ӧ���С��ж��롣
3��֧��ָ������λͼ�������Ӧ�����ַ�ʽ��

log:
�Ƚ�ASCII���ɵ��󣬴洢�ڻ�������ʹ�ò���õ���Ӧ��ASCII�롣ȱ�㣺�˷��ڴ�
****************************************************************************/

#ifndef __FREETYPEUTILS_H__
#define __FREETYPEUTILS_H__

#include "SWFC.h"

#include <ft2build.h>

#include FT_FREETYPE_H


// �ַ��洢�����ʽ
enum Encoding {
    ENCODE_UTF8,
    ENCODE_GB2312
};

// ������
typedef struct _FT_Font FT_Font;

class CTTF2Bitmap
{
public:
    CTTF2Bitmap();
    ~CTTF2Bitmap();

public:
    /**
    * ��ʼ��
    *
    * @param pFontFile         ʸ���ֿ��ļ�(.ttf��.ttc�ļ�)
    * @param iFontSize         �����С
    * @param iMaxBitmapWidth   ���ɵ���λͼ��������󳤶ȣ���Ϊ0����ʾ�ó����Զ���������
    * @return  E_FAIL������   S_OK������
    *
    */
    HRESULT Init(const char* pFontFile, int iFontSize, int iMaxBitmapWidth);

    /**
    * �ͷ��ڴ棬�ر��ֿ�
    *
    * @note
    *       ��ʽ�ͷ��ڴ棬�粻���ô˺��������������������ͷ��ڴ�
    */
    void Destroy();

    /**
    * ��������λͼ����
    *
    * @param pText                 ��Ҫת���ɵ�����ַ���
    * @param cEncoding             pText�ַ������õı����ʽ
    * @param ppbBitmap[OUT]        ���ɵĵ���λͼ�Ļ�����
    * @param iBitmapDataSize[OUT]  ��������С
    * @param iBitmapWidth[OUT]     ��������
    * @param iBitmapHeight[OUT]    ��������
    *
    * @return S_OK: ����  E_FAIL������
    *
    * @note
    *       ����ע��pText�ı����ʽ��Ŀǰ��֧��GB2312��UTF8������벻��ȷ����������롣
    */
    HRESULT GenerateBitmap(const char* pText, const Encoding cEncoding,
            BYTE** ppbBitmap, int& iBitmapDataSize, int& iBitmapWidth, int& iBitmapHeight);

    // ���Ͻӿ����ƣ�Ŀǰֻ��������ʱ����Ϣ�������ַ�δȫ������������
    HRESULT GenerateAsciiBitmap(const char* pAscii, BYTE** ppbBitmap,
            int& iBitmapDataSize, int& iBitmapWidth, int& iBitmapHeight);
    int GetFontSize() {return m_iCharWidth; }
    int GetFontHeight() { return m_iCharHeight; }

#ifdef FT_DEBUG
    int GetRealTextNum() { return m_iRealTextNum; }
    void PrintDebugInfo();
#endif

private:
    HRESULT ScanTextForRowCol(unsigned short* pUnicodeText, int iUnicodeLen, int& iNumRows, int& iColSpacing);
    HRESULT ScanAsciiTextForRowCol(unsigned short* pUnicodeText, int iUnicodeLen, int& iNumRows, int& iColSpacing);

    // �����ַ�����!"#$%&'()*+,-./0123456789:���ĵ���ÿ���ַ������洢
    // �޷�����ȫ��ASCII�룬��Ϊÿ��ASCII�ĵ����С����ͬ������ͳһ��
	HRESULT GenerateAsciiFont();

    void ChangeMatrix(char* pString, BYTE* pAsciiFontData, int iAsciiWidth, int iAsciiHeight,
        BYTE* pBitmap, int& iBitmapSize, int& iWidth, int& iHeight);

    HRESULT ExpandBitmapBuffer(int iTargetSize);

private:
    FT_Font* m_cFont;       // ������

    int m_iCharWidth;       // �ַ���
    int m_iCharHeight;      // �ַ�ʵ�ʵĸ�(���ʼ����FontSizeֵ��һ�����)

    bool m_fAdjustWidth;    // ����Ӧ���־

	bool m_fInitialized;
	FT_Library    m_sLibrary;

    int m_iBitmapWidth;     // λͼ��
    int m_iBitmapHeight;    // λͼ��
    BYTE* m_pbBitmapBuffer;// λͼ������ָ��
    int m_iBitmapDataSize;  // λͼ�����ܴ�С

    int m_iMaxBlocks;      // һ�ж���������(��\t������)
    int* m_piBlockWidths;  // һ��ÿ�εĿ�

    //unsigned short* m_pUnicodeText; // Unicode��������(���û���������ת������)
    //int m_iUnicodeTextLen;  // unicode����

#ifdef FT_DEBUG
    int m_iRealTextNum;     // ʵ�����ָ���
#endif

	//////////////////////////////////////////////

    typedef struct _AsciiFontAttr
    {
        int width;
        int offset;
    } AsciiFontAttr;

    bool m_fNeedGenerateFontData;
    BYTE* m_pbAsciiFontData;   // �洢ASCII����󻺳���

    AsciiFontAttr m_cAsciiAttr[95]; // ����ÿ��ASCII��ȼ�ƫ����
};

#endif // __FREETYPEUTILS_H__
