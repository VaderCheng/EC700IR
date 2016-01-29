// ���ļ��������ΪWINDOWS-936

#include "transcoding.h"
#include "FreetypeUtils.h"

// ע�⣡����������freetype��ͷ�ļ�·��
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#pragma comment(lib, "lib/freetype2410MT.lib")
#endif

/////////debug
#ifdef WIN32
// Windows���ĺ�
#ifdef FT_DEBUG
#define debug printf
#define error_msg printf
#else
#define debug
#define error_msg
#endif

#else
// linux���ĺ�
#ifdef FT_DEBUG
#define debug(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define error_msg(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#define error_msg(fmt, ...) printf(fmt, ##__VA_ARGS__)
#endif

#endif
//////////// debug end

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define FT_FLOOR(X)    ((X & -64) / 64)
#define FT_CEIL(X)    (((X + 63) & -64) / 64)

// ���������ο���SDL_ttf����
struct _FT_Font
{
    FT_Face face;	// ������۶���
    int height;	    // �����
    int baseline;   // ����
};

/**
 * ��ʼ��FreeType��
 *
 * @return
 *          0    ��ʼ��OK
 *          -1   ��ʼ������
 */
int FT_Init(bool& fInited, FT_Library& libFT)
{
    int status = 0;

    if (!fInited)
    {
        FT_Error error = FT_Init_FreeType(&libFT);
        if (error)
        {
            error_msg("FT_Init_FreeType failed, error: 0x%x\n", error);
            status = -1;
			return status;
        }
    }

    // test
    int major = 0;
    int minor = 0;
    int patch = 0;
    FT_Library_Version(libFT, &major, &minor, &patch);
    debug("Init: version: %d.%d.%d\n", major, minor, patch);
	
	fInited = true;
    return status;
}

/**
 * ����FT
 *
 */
void FT_Quit(bool& fInited, FT_Library& libFT)
{
    if (fInited)
    {
        FT_Done_FreeType(libFT);
		libFT = 0;
    }

	fInited = false;
}

/**
 * �ر��ֿ⣬�ͷ�����ṹ��
 *
 */
void FT_CloseFont(FT_Font* font)
{
    int error;
    if (font)
    {
        if (font->face)
        {
            error = FT_Done_Face(font->face);
            if (error)
            {
                error_msg("FT_Done_Face error, 0x%X.\n", error);
            }
        }
        free(font);
        font = NULL;
    }
}

/**
 * ���ֿ��ļ�
 *
 * @param pFontFile �ֿ��ļ���
 * @param iFontSize  �����С
 *
 * @return
 *          �ɹ�����FT_Font�ṹ�壬������NULL
 */
FT_Font* FT_OpenFont(FT_Library& libFT, const char* pFontFile, int iFontSize)
{
    FT_Font* font;
    FT_Face face;
    FT_Error error;
    FT_Fixed scale;

    font = (FT_Font *)calloc(1, sizeof(FT_Font));
    if (font == NULL)
    {
        error_msg("malloc for font failed!\n");
        return NULL;
    }

    // ����һ���ֿ��ļ�
    error = FT_New_Face(libFT, pFontFile, 0, &font->face); // face����ΪFT����
    if (error)
    {
        error_msg("FT_New_Face %s failed, error: 0x%x\n", pFontFile, error);
        FT_CloseFont(font);
        return NULL;
    }
    face = font->face; // ���ݵ��Ƕ���ָ��

    // �趨ΪUNICODE��Ĭ��Ҳ��
    error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    if (error)
   {
       error_msg("FT_Select_Charmap %s failed, error: 0x%x\n", pFontFile, error);
       FT_CloseFont(font);
       return NULL;
   }

    // ������
    if (FT_IS_SCALABLE(face))
    {
        // ���������С
        // ע����СҪ��64��Ĭ��DPIΪ72
        error = FT_Set_Char_Size(face, 0, iFontSize << 6, 0, 0);
        if (error)
        {
            error_msg("Set char size failed, error: 0x%x\n", error);
            FT_CloseFont(font);
            return NULL;
        }

      scale = face->size->metrics.y_scale;
      int ascent  = FT_CEIL(FT_MulFix(face->ascender, scale));
      int descent = FT_CEIL(FT_MulFix(face->descender, scale));
      font->height  = ascent - descent;
      font->baseline = font->height + descent;

      //int underline = FT_CEIL(FT_MulFix(face->underline_position, scale));
      //debug("underline_position: %d %d\n", underline, FT_CEIL(FT_MulFix(face->height, scale)));
    }

    return font;
}

// ������ع��ߺ���
int FT_GetFontHeight(FT_Font* font)
{
    return font->height;
}

int FT_GetFontBaseline(FT_Font* font)
{
    return font->baseline;
}

/**
 * ȡ�����ַ��ĵ���
 *
 * @param font              ����ṹ��
 * @param pUnicodeChar      unicode��ʽ���ַ�
 * @param pBitmap           �ַ����󻺳���
 * @param iBitmapWidth      ������
 * @param iBitmapHeight     ����߶�
 * @param iStartX           �ַ������X��㿪ʼcopy
 * @param iStartY           �ַ������Y��㿪ʼcopy
 * @param iFontStride[OUT]  �ַ��Ŀ���
 *
 * @return  -1������   0������
 *
 * @note
 *       ʹ��iFontStride��ԭ����ÿ���ַ��Ŀ�Ȳ���ͬ��
         �������ȹ涨�����w��ȳ�����ǿ���i��Ȳ����ǿ�
 */
int FT_GetCharBitmap(
    FT_Font* font,
    const unsigned short* pUnicodeChar,
    unsigned char* pBitmap,
    int iBitmapWidth,
    int iBitmapHeight,
    int iStartX,
    int iStartY,
    int& iFontStride
    )
{
    FT_GlyphSlot  slot = font->face->glyph;
    FT_UInt glyph_index;
    FT_Error error;
    FT_Bitmap *bm;
    unsigned short iPlaceUnicodeChar = 0x25a1;  // ��ѡ�����򡰡���

    int rows;
    int width;
    int top;
    int left;

    if (pBitmap == NULL)
    {
        error_msg("pBitmap is NULL\n");
        return -1;
    }

    glyph_index = FT_Get_Char_Index(font->face, *pUnicodeChar); // �ҵ��ַ�����
    if(glyph_index == 0)
    {
        // �����ֿ��ļ���û�е��ַ�ʱ��ʹ�á���������
        // ʹ�ÿհ׷��򡰡������滻û�е��ַ�
        error_msg("For char UNICODE 0x%04X, got glyph_index = 0, changed to UNICODE: 0x%04X.\n", *pUnicodeChar, iPlaceUnicodeChar);
        glyph_index = FT_Get_Char_Index(font->face, iPlaceUnicodeChar);
        if(glyph_index == 0)
        {
            // ���û������ķ�����ʹ���ײ�����
            error_msg("For char UNICODE 0x%04X, got glyph_index = 0 again, ", iPlaceUnicodeChar);
            iPlaceUnicodeChar = 0x56d7; // ��ѡ�ײ�����
            error_msg("changed to UNICODE: 0x%04X.\n", iPlaceUnicodeChar);
            glyph_index = FT_Get_Char_Index(font->face, iPlaceUnicodeChar);
            if(glyph_index == 0)
            {
                error_msg("For char UNICODE 0x%04X, got glyph_index = 0 once again, ", iPlaceUnicodeChar);
                iPlaceUnicodeChar = 0x53e3; // ��ѡ���֡��ڡ�
                error_msg("changed to UNICODE: 0x%04X.\n", iPlaceUnicodeChar);
                glyph_index = FT_Get_Char_Index(font->face, iPlaceUnicodeChar);
                if(glyph_index == 0)
                {
                    error_msg("For char UNICODE 0x%04X, trying other char but got glyph_index = 0, failed!!!!!, ", *pUnicodeChar);
                    return -1;
                }
            }
        }
    }

    // װ������ͼ�����β�(�õ���ģ)
    error = FT_Load_Glyph(font->face, glyph_index, FT_LOAD_DEFAULT);
    if (error)
    {
        error_msg("FT_Load_Glyph error, 0x%x.\n", error);
        return -1;
    }

    // �Ӵ֣��ݲ�Ҫ
    //FT_Outline_Embolden( &(slot->outline), 1<<6);

    // ת��Ϊλͼ����
    if (slot->format != FT_GLYPH_FORMAT_BITMAP)
    {
        // ת����λͼ
        // �ڶ�������Ϊ��Ⱦģʽ
        // FT_RENDER_MODE_NORMAL��Ⱦ��һ���������Ŀ����(256���Ҷ�)λͼ
        // FT_RENDER_MODE_MONO��ȾΪ1λλͼ���ڰ�λͼ��
        error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
        if (error)
        {
            error_msg("FT_Render_Glyph error, 0x%x.\n", error);
            return -1;
        }
    }

    bm = &slot->bitmap;
    rows = slot->bitmap.rows;
    width = slot->bitmap.width;
    iFontStride = (int)slot->metrics.horiAdvance>>6;    // ��ʵ���

    // �����ڽ�����һ���ַ��������
    if (iBitmapWidth == 0)
    {
        iBitmapWidth = iFontStride;
    }

    if (iBitmapHeight == 0)
    {
        iBitmapHeight = font->height;
    }

    // �����߾�
    left = ((rows - font->height) >= 0) ? 0 : slot->bitmap_left / 2;
    top = font->baseline - slot->bitmap_top;
    rows = MIN(rows, font->height);

    // ��ֹԽ��
    //int x_max = MIN(iStartX + left + width, iBitmapWidth);
    //int y_max = MIN(iStartY + top + rows, iBitmapHeight);
    //int i, j, p, q;

    ////����������1�ֽڱ�ʾ1���أ�Ϊ1��ʾ��Ҫ��ʾ�ĵ�
    //for (i = MAX(iStartY + top, 0), p = 0; i < y_max; i++, p++)
    //{
    //    for (j = MAX(iStartX + left, 0), q = 0; j < x_max; j++, q++)
    //    {
    //        pBitmap[i * iBitmapWidth + j] = (bm->buffer[p * width + q] ? 1 : 0);
    //    }
    //}

    // �ƺ����ַ�����������������
    //int x = 0;
    //int y = 0;
    for (int i = 0; i < rows && (i + top + iStartY) < iBitmapHeight; i++)
    {
        for (int j = 0; j < width && (j + left + iStartX) < iBitmapWidth; j++)
        {
            //x = j + left + iStartX;
            //y = i + top + iStartY;
            //int v = 0;
            //if (bm->buffer[/*i * bm->pitch + */j / 8] & (0x80 >> (x&7)))
            //    v = 1;
            //else
            //    v = 0;
            pBitmap[(i + top + iStartY) * iBitmapWidth + (j + left + iStartX)] = (bm->buffer[i * width + j] ? 1 : 0);
        }
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////////

CTTF2Bitmap::CTTF2Bitmap()
            :m_cFont(NULL),
            m_iCharWidth(0),
            m_iCharHeight(0),
            m_fAdjustWidth(false),
            m_iBitmapWidth(0),
            m_iBitmapHeight(0),
            m_pbBitmapBuffer(NULL),
            m_iBitmapDataSize(0),
            m_iMaxBlocks(0),
            m_piBlockWidths(NULL),
            m_fNeedGenerateFontData(false),
            m_pbAsciiFontData(NULL)
{
#ifdef FT_DEBUG
	m_iRealTextNum = 0;
#endif
	m_fInitialized = false;
	m_sLibrary = 0;
}

CTTF2Bitmap::~CTTF2Bitmap()
{
    Destroy();
}

HRESULT CTTF2Bitmap::Init(const char* pFontFile, int iFontSize, int iMaxBitmapWidth)
{

    m_iBitmapWidth = iMaxBitmapWidth;

    if (m_iBitmapWidth == 0)
    {
        m_fAdjustWidth = true;
    }

    // m_iMaxColumnsΪ��ֱ����Ŀ�ĸ�������һ���������ж��ٸ�'\t'
    if (m_fAdjustWidth)
    {
        m_iMaxBlocks = 50; // �㹻��
    }
    else
    {
        m_iMaxBlocks = (m_iBitmapWidth) / (iFontSize / 2);
    }

    m_piBlockWidths = new int[m_iMaxBlocks];

    if (FT_Init(m_fInitialized, m_sLibrary) < 0)
    {
        error_msg("FT_Init failed!!\n");
        return E_FAIL;
    }

    m_cFont = FT_OpenFont(m_sLibrary, pFontFile, iFontSize);
    if (m_cFont == NULL)
    {
        error_msg("FT_OpenFont failed!!\n");
        return E_FAIL;
    }

    m_iCharWidth = iFontSize;
    m_iCharHeight = FT_GetFontHeight(m_cFont);

    m_fNeedGenerateFontData = true;

    return S_OK;
}

void CTTF2Bitmap::Destroy()
{
    if (m_pbBitmapBuffer != NULL)
    {
        delete[] m_pbBitmapBuffer;
        m_pbBitmapBuffer = NULL;
        m_iBitmapDataSize = 0;
    }
    if (m_piBlockWidths != NULL)
    {
        delete[] m_piBlockWidths;
        m_piBlockWidths = NULL;
    }

    if (m_pbAsciiFontData != NULL)
    {
        delete[] m_pbAsciiFontData;
        m_pbAsciiFontData = NULL;
    }

    if (m_cFont != NULL)
    {
        FT_CloseFont(m_cFont);
        m_cFont = NULL;
        FT_Quit(m_fInitialized, m_sLibrary);
    }
}

HRESULT CTTF2Bitmap::ExpandBitmapBuffer(int iTargetSize)
{
    if (iTargetSize <= 0)
    {
        error_msg("expand size = %d.\n", iTargetSize);
        return E_FAIL;
    }

    if (m_pbBitmapBuffer != NULL && m_iBitmapDataSize != 0)
    {
        if (iTargetSize <= m_iBitmapDataSize)
        {
            return S_OK;
        }
        else
        {
            delete[] m_pbBitmapBuffer;
            m_pbBitmapBuffer = NULL;
        }
    }
    else if (
            (m_pbBitmapBuffer != NULL && m_iBitmapDataSize == 0)
            ||
            (m_pbBitmapBuffer == NULL && m_iBitmapDataSize != 0)
            )
    {
        error_msg("State error m_pbBitmapBuffer = %p, m_iBitmapDataSize = %d.\n", m_pbBitmapBuffer, m_iBitmapDataSize);
    }

    m_pbBitmapBuffer = new BYTE[iTargetSize];
    if (m_pbBitmapBuffer)
    {
        m_iBitmapDataSize = iTargetSize;
    }
    else
    {
        error_msg("Allocate m_pbBitmapBuffer fail.\n");
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CTTF2Bitmap::GenerateBitmap(const char* pText, const Encoding cEncoding,
    BYTE** ppbBitmap, int& iBitmapDataSize, int& iBitmapWidth, int& iBitmapHeight)
{
    int ret = -1;
    int iNumRows = 0;
    int iColSpacing = 0;

    if(m_cFont == NULL)
    {
        error_msg("m_cFont is NULL\n");
        return E_FAIL;
    }

    // ת���ַ���
    int iTextLen = (int)strlen(pText);
    unsigned short* pUnicodeText = new unsigned short[iTextLen];
    if(pUnicodeText == NULL)
    {
        error_msg("Allocate pUnicodeText failed.\n");
        return E_FAIL;
    }
    int iUnicodeLen = 0;
#ifdef FT_DEBUG
    m_iRealTextNum = 0;
#endif
    switch (cEncoding)
    {
        case ENCODE_UTF8:
            iUnicodeLen = utf8_to_unicode(pUnicodeText, pText, iTextLen);
            break;
        case ENCODE_GB2312:
            iUnicodeLen = gb2312_to_unicode(pUnicodeText, pText, iTextLen);
            break;
        default:
            return E_FAIL;
    }

    // ɨ�����֣���ȷ���м�������
    ScanTextForRowCol(pUnicodeText, iUnicodeLen, iNumRows, iColSpacing);

    m_iBitmapHeight = m_iCharHeight * iNumRows;

    // ��������
    iBitmapWidth = m_iBitmapWidth;
    iBitmapHeight = m_iBitmapHeight;
    iBitmapDataSize = iBitmapWidth * iBitmapHeight;

    debug("m_iCharHeight = %d, iNumRows = %d.\n", m_iCharHeight, iNumRows);
    debug("m_iBitmapWidth = %d.\n", m_iBitmapWidth);
    debug("m_iBitmapHeight = %d.\n", m_iBitmapHeight);
    debug("iBitmapDataSize = %d.\n", iBitmapDataSize);

    if ( S_OK != ExpandBitmapBuffer(iBitmapDataSize))
    {
        error_msg("ExpandBitmapBuffer failed.\n");
        return E_FAIL;
    }

    // ÿ����ȡ����ջ���������ֹ�����ݸ���
    memset(m_pbBitmapBuffer, 0, iBitmapDataSize);

    int rowIndex = 0;
    int columnIndex = 0;

    int iStartX = 0;
    int iStartY = 0;
    int iFontStride = 0;
    for ( int n = 0; n < iUnicodeLen; n++ )
    {
        // ���У�X��0��Y�Ƶ���һ��
        if (pUnicodeText[n] == '\n')
        {
            iStartX = 0;
            iStartY = (++rowIndex) * m_iCharHeight;
            columnIndex = 0;
            continue;
        }
        // ˮƽ���룬X���ƣ�Y����
        if (pUnicodeText[n] == '\t')
        {
            iStartX = 0;
            for (int i = 0; i <= columnIndex; i++)
                iStartX += m_piBlockWidths[i] + iColSpacing;
            columnIndex++;
            continue;
        }

        ret = FT_GetCharBitmap(m_cFont, &pUnicodeText[n],
                        m_pbBitmapBuffer, m_iBitmapWidth, m_iBitmapHeight,
                        iStartX, iStartY, iFontStride);
        if (ret < 0)
        {
            error_msg("FT_GetCharBitmap failed.\n");
            delete[] pUnicodeText;
            pUnicodeText = NULL;
            return E_FAIL;
        }

#ifdef FT_DEBUG
        m_iRealTextNum++;
#endif
        iStartX += iFontStride;
    }

    *ppbBitmap = m_pbBitmapBuffer;

    delete[] pUnicodeText;
    pUnicodeText = NULL;

    return S_OK;
}

HRESULT CTTF2Bitmap::GenerateAsciiBitmap(const char* pAscii, BYTE** ppbBitmap,
        int& iBitmapDataSize, int& iBitmapWidth, int& iBitmapHeight)
{
    if(m_cFont == NULL)
    {
        error_msg("m_cFont is NULL\n");
        return E_FAIL;
    }

    // ת���ַ���
    int iTextLen = (int)strlen(pAscii);
    unsigned short* pUnicodeText = new unsigned short[iTextLen + 1];
    int iUnicodeLen = ascii_to_unicode(pUnicodeText, pAscii, iTextLen);;

    pUnicodeText[iUnicodeLen] = '\0';

    if (m_fNeedGenerateFontData)
    {
        printf("Need generate a new ASCII font database.\n");
        if ( S_OK != GenerateAsciiFont())
        {
            error_msg("GenerateAsciiFont fail.\n");
            return E_FAIL;
        }
        m_fNeedGenerateFontData = false;
    }

    // ɨ�����֣���ȷ���м�������
    int iNumRows = 0;
    int iColSpacing = 0;
    if ( S_OK != ScanAsciiTextForRowCol(pUnicodeText, iUnicodeLen, iNumRows, iColSpacing))
    {
        error_msg("Error in ScanAsciiTextForRowCol.\n");
    }

    m_iBitmapHeight = m_iCharHeight * iNumRows;

    // ��������
    iBitmapWidth = m_iBitmapWidth;
    iBitmapHeight = m_iBitmapHeight;
    iBitmapDataSize = iBitmapWidth * iBitmapHeight;

    if ( S_OK != ExpandBitmapBuffer(iBitmapDataSize))
    {
        error_msg("ExpandBitmapBuffer failed.\n");
        return E_FAIL;
    }

#ifdef FT_DEBUG
    m_iRealTextNum = 0;
#endif

    // ÿ����ȡ����ջ���������ֹ�����ݸ���
    memset(m_pbBitmapBuffer, 0, iBitmapDataSize);

    int rowIndex = 0;
    int columnIndex = 0;

    int iStartX = 0;
    int iStartY = 0;
    int iFontStride = 0;    //= m_iCharWidth / 2;
    //int iAsciiCharSize = m_iCharWidth / 2 * m_iCharHeight;

    for ( int n = 0; n < iUnicodeLen; n++ )
    {
        // ���У�X��0��Y�Ƶ���һ��
        if (pUnicodeText[n] == '\n')
        {
            iStartX = 0;
            iStartY = (++rowIndex) * m_iCharHeight;
            columnIndex = 0;
            continue;
        }
        // ˮƽ���룬X���ƣ�Y����
        if (pUnicodeText[n] == '\t')
        {
            iStartX = 0;
            for (int i = 0; i <= columnIndex; i++)
                iStartX += m_piBlockWidths[i] + iColSpacing;
            columnIndex++;
            continue;
        }

        // ���
		int index = pUnicodeText[n] - 0x20;
		char * chtmp = (char *)m_pbAsciiFontData + m_cAsciiAttr[index].offset;

		iFontStride = m_cAsciiAttr[index].width;

        //debug("font: %d[%c] width: %d offset: %d\n", pUnicodeText[n]-0x20, pUnicodeText[n], iFontStride, m_cAsciiAttr[index].offset);

        // ����ת��
        // old
        //for (int i = iStartY, p = 0; i < m_iBitmapHeight; i++, p++)
        //{
        //    for (int j = iStartX, q = 0; j < m_iBitmapWidth; j++, q++)
        //    {
        //        m_pbBitmapBuffer[i*m_iBitmapWidth + j] = chtmp[p*iFontStride + q];
        //    }
        //}
        // ����������ѭ��������
        for (int i = 0; i < m_iCharHeight && (i + iStartY) < m_iBitmapHeight; i++)
        {
            for (int j = 0; j < iFontStride && (j + iStartX) < m_iBitmapWidth; j++)
            {
                m_pbBitmapBuffer[(i + iStartY) * m_iBitmapWidth + (j + iStartX)] = chtmp[i*iFontStride + j];
            }
        }
#ifdef FT_DEBUG
        m_iRealTextNum++;
#endif

        iStartX += iFontStride;
    }

    *ppbBitmap = m_pbBitmapBuffer;

    delete[] pUnicodeText;
    pUnicodeText = NULL;

    return S_OK;
}

// ����95���ɴ�ӡ��ASCII��(����0~31��127���ASCII)����
// ���������е�ʸ��������ASCII�����ȫ�ǵ�һ�룬��Щ����ڰ�ǣ���ЩС�ڰ�ǣ�������ʱ�԰��Ϊ׼��
HRESULT CTTF2Bitmap::GenerateAsciiFont()
{
    int ret = -1;
    int iTextLen = 0;
    int iUnicodeTextLen = 0;
    unsigned short* pUnicodeText = NULL;
    int iWidth = 0;
    int iHeight = 0;
    int iCharSize = 0;	// һ���ַ��Ĵ�С
    int offset = 0;
    int iTotalAsciiSize = 0;
    BYTE* pBuffer = NULL;
    BYTE* pIncreaseBuffer = NULL;
    const char pAscii[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";    // 95��

    iTextLen = (int)strlen(pAscii);
    pUnicodeText = new unsigned short[iTextLen];
    iUnicodeTextLen = ascii_to_unicode(pUnicodeText, pAscii, iTextLen);

    iWidth = m_iCharWidth / 2;				// ��ʱÿ���ַ����Ϊ���
    iHeight = m_iCharHeight;
    iCharSize = iWidth * iHeight;
    iTotalAsciiSize = iCharSize * iTextLen; // �ܴ�С
    printf("pre-alloc memory buffer size is %d.\n", iTotalAsciiSize);

    if (m_pbAsciiFontData != NULL)
    {
        error_msg("memory leak fixed!.\n");
        delete [] m_pbAsciiFontData;
        m_pbAsciiFontData = NULL;
    }

    m_pbAsciiFontData = new BYTE[iTotalAsciiSize]; // Ԥ�����ɺû���
    if (m_pbAsciiFontData == NULL)
    {
        error_msg("memory insufficient!.\n");
        return E_FAIL;
    }

    memset(m_pbAsciiFontData, 0, iTotalAsciiSize);

    memset(m_cAsciiAttr, 0, 95*sizeof(AsciiFontAttr));

    pBuffer = m_pbAsciiFontData;

    int ss = 0;
    for (int i = 0; i < iUnicodeTextLen; i++)
    {
        if (pBuffer >= m_pbAsciiFontData + iTotalAsciiSize - 128*iHeight) // Ԥ����һ���ַ����ڴ�; ������ַ����128����
        {
            debug("Overflow pBuffer = %p, m_pbAsciiFontData = %p, iHeight = %d\n", pBuffer, m_pbAsciiFontData, iHeight);
            debug("need increase buffer size to %d.\n", offset+128*iHeight);
            iTotalAsciiSize = offset+128*iHeight;
            pIncreaseBuffer = new BYTE[iTotalAsciiSize];
            if (pIncreaseBuffer == NULL)
            {
                error_msg("memory insufficient, %s :line %d.\n", __func__, __LINE__);
            }
            memcpy(pIncreaseBuffer, m_pbAsciiFontData, offset); // ���������ɵ���������
            if (iTotalAsciiSize == 1)
                delete m_pbAsciiFontData;
            else
                delete[] m_pbAsciiFontData;
            pBuffer += pIncreaseBuffer - m_pbAsciiFontData;
            m_pbAsciiFontData = pIncreaseBuffer;
            pIncreaseBuffer = NULL;
            debug("increase done.\n");
        }

        int iFontStride = 0;
        ret = FT_GetCharBitmap(m_cFont, &pUnicodeText[i], pBuffer, 0, 0, 0, 0, iFontStride);
        if (ret < 0)
        {
            error_msg("FT_GetCharBitmap failed.\n");
            return E_FAIL;
        }
        iCharSize = iFontStride * iHeight;
		m_cAsciiAttr[i].width = iFontStride;
		ss+=iFontStride;
		m_cAsciiAttr[i].offset = offset;
		pBuffer += iCharSize;
        offset += iCharSize;

        debug("debug-: %d width: %d charsize: %d offset: %d\n", i, m_cAsciiAttr[i].width, iCharSize, m_cAsciiAttr[i].offset);
    }

    printf("total width: %d, calc width: %d; total size %d byte(s).\n", ss, 95*m_iCharWidth/2, iTotalAsciiSize);

	delete[] pUnicodeText;
    return S_OK;
}

// ��'\t'��'\n'Ϊ�ֿ��־��ɨ���ַ������ó�ÿ�鳤�ȣ��̶��õ���������Ҫ����
HRESULT CTTF2Bitmap::ScanTextForRowCol(unsigned short* pUnicodeText, int iUnicodeLen,
                                       int& iNumRows, int& iColSpacing)
{
    memset(m_piBlockWidths, 0, m_iMaxBlocks * sizeof(int));

    int iNumColumns = 0;    // һ�п�ĸ���
    int numColumnsCurRow = 0; // ��ǰ�п�ĸ���
    int columnWidth = 0;

    for (int i = 0; i < iUnicodeLen; i++) {
        switch (pUnicodeText[i]) {
        case '\n':
            m_piBlockWidths[numColumnsCurRow] = MAX(m_piBlockWidths[numColumnsCurRow], columnWidth);
            numColumnsCurRow++;
            if (numColumnsCurRow >= m_iMaxBlocks)
                return E_FAIL;
            iNumRows++;
            iNumColumns = MAX(iNumColumns, numColumnsCurRow);
            numColumnsCurRow = 0;
            columnWidth = 0;
            break;
        case '\t':
            m_piBlockWidths[numColumnsCurRow] = MAX(m_piBlockWidths[numColumnsCurRow], columnWidth);
            numColumnsCurRow++;
            if (numColumnsCurRow >= m_iMaxBlocks)
                return E_FAIL;
            columnWidth = 0;
            break;
        default:
            if (pUnicodeText[i] <= 0x80)
            {
                columnWidth += m_iCharWidth/ 2;
                if (m_iCharWidth % 2)
                    columnWidth += 1;   // note����ֹ�����СΪ����ʱ���ֽض��ַ�������
            }

            else
                columnWidth += m_iCharWidth;
            if (i == iUnicodeLen - 1) {
                m_piBlockWidths[numColumnsCurRow] = MAX(m_piBlockWidths[numColumnsCurRow], columnWidth);
                numColumnsCurRow++;
                if (numColumnsCurRow >= m_iMaxBlocks)
                    return E_FAIL;
                iNumRows++;
                iNumColumns = MAX(iNumColumns, numColumnsCurRow);
            }
            break;
        }
    }
    int textWidth = 0;
    for (int i = 0; i < iNumColumns; i++)
        textWidth += m_piBlockWidths[i];

    // �Զ�������
    if (m_fAdjustWidth)
    {
        m_iBitmapWidth = textWidth + m_iCharWidth * (iNumColumns - 1);
        iColSpacing = m_iCharWidth; // ��һ���ַ���Ϊ����ռ�
    }
    else
    {
        if ((m_iBitmapWidth - textWidth) <= 0)
            iColSpacing = 0;
        else
            iColSpacing = (m_iBitmapWidth - textWidth) / iNumColumns;
    }
    debug("font height: %d text width: %d bitmap width: %d space: %d col: %d\n",
        FT_GetFontHeight(m_cFont), textWidth, m_iBitmapWidth, iColSpacing, iNumColumns);

    return S_OK;
}

HRESULT CTTF2Bitmap::ScanAsciiTextForRowCol(unsigned short* pUnicodeText, int iUnicodeLen, int& iNumRows, int& iColSpacing)
{
    memset(m_piBlockWidths, 0, m_iMaxBlocks * sizeof(int));

    int iNumMaxBlocks = 0;    // һ�п�ĸ���(�������е����ֵ),ռ�������
    int numBlocksCurRow = 0; // ��ǰ�еĿ����
    int blockWidth = 0; // һ����Ŀ��

    int index;
    int iFontStride = 0;

    for (int i = 0; i < iUnicodeLen; i++) {
        switch (pUnicodeText[i]) {
        case '\n':
            m_piBlockWidths[numBlocksCurRow] = MAX(m_piBlockWidths[numBlocksCurRow], blockWidth);
            numBlocksCurRow++;
            if (numBlocksCurRow >= m_iMaxBlocks)
                return E_FAIL;
            iNumRows++;
            iNumMaxBlocks = MAX(iNumMaxBlocks, numBlocksCurRow);
            numBlocksCurRow = 0;
            blockWidth = 0;
            break;
        case '\t':
            m_piBlockWidths[numBlocksCurRow] = MAX(m_piBlockWidths[numBlocksCurRow], blockWidth);
            numBlocksCurRow++;
            if (numBlocksCurRow >= m_iMaxBlocks)
                return E_FAIL;
            blockWidth = 0;
            break;
        default:
            index = pUnicodeText[i] - 0x20;
            if (index >= 0)
            {
                iFontStride = m_cAsciiAttr[index].width;
            }
            else
            {
                iFontStride = m_iCharWidth;
            }

            if (pUnicodeText[i] <= 0x80)
                blockWidth += iFontStride;
            else
                blockWidth += m_iCharWidth;
            if (i == iUnicodeLen - 1) {
                m_piBlockWidths[numBlocksCurRow] = MAX(m_piBlockWidths[numBlocksCurRow], blockWidth);
                numBlocksCurRow++;
                if (numBlocksCurRow >= m_iMaxBlocks)
                    return E_FAIL;
                iNumRows++;
                iNumMaxBlocks = MAX(iNumMaxBlocks, numBlocksCurRow);
            }
            break;
        }
    }
    int textWidth = 0;
    for (int i = 0; i < iNumMaxBlocks; i++)
        textWidth += m_piBlockWidths[i];

    // �Զ�������
    if (m_fAdjustWidth)
    {
        m_iBitmapWidth = textWidth + m_iCharWidth * (iNumMaxBlocks - 1);
        iColSpacing = m_iCharWidth; // ��һ���ַ���Ϊ����ռ�
    }
    else
    {
        if ((m_iBitmapWidth - textWidth) <= 0)
            iColSpacing = 0;
        else
            iColSpacing = (m_iBitmapWidth - textWidth) / iNumMaxBlocks;
    }
    debug("font height: %d text width: %d bitmap width: %d space: %d col: %d\n",
        FT_GetFontHeight(m_cFont), textWidth, m_iBitmapWidth, iColSpacing, iNumMaxBlocks);

    return S_OK;
}

#ifdef FT_DEBUG
void CTTF2Bitmap::PrintDebugInfo()
{
    printf("bitmap buffer: %dB(%.1fKB) = %d*%d font width: %d font height: %d\n",
            m_iBitmapDataSize, m_iBitmapDataSize/1024.0, m_iBitmapWidth, m_iBitmapHeight, m_iCharWidth, m_iCharHeight);
    printf("real text num: %d\n", m_iRealTextNum);
}
#endif
