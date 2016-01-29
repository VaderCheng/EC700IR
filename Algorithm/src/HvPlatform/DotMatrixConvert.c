#include <string.h>
#include "swbasetype.h"

#if 0
// �������ã������ʽת��
// ����˵����
// pStringDotMatrix��������LJJ����ʽ���ַ������ַ��߶ȹ��Ϊ32����λ�����أ�
// iDotMatrixWidth��pStringDotMatrix�Ŀ�ȣ���λ�����أ�
// rgbDotMatrixDsp��ת��������DSP��ʹ�õ��ַ������ʽ
void DotMatrixConvert(const char* pStringDotMatrix, int iDotMatrixWidth, PBYTE8 rgbDotMatrixDsp)
{
    int w = iDotMatrixWidth;
    char* p = (char*)pStringDotMatrix;

    // ��Ϊ�ַ��߶ȹ��Ϊ32���أ���������Ҫ����32�ֽ�
    memset(rgbDotMatrixDsp, 0, iDotMatrixWidth * 32);

    int i = 0;
    for (i = 0; i < w; ++i )
    {
        int iDstLine = 0;

        char c0 = p[0];
        char c1 = p[1];
        char c2 = p[2];
        char c3 = p[3];

        #define BIT0 0x01
        #define BIT1 0x02
        #define BIT2 0x04
        #define BIT3 0x08
        #define BIT4 0x10
        #define BIT5 0x20
        #define BIT6 0x40
        #define BIT7 0x80

        rgbDotMatrixDsp[w*iDstLine + i] = (c0&BIT0); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c0&BIT1); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c0&BIT2); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c0&BIT3); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c0&BIT4); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c0&BIT5); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c0&BIT6); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c0&BIT7); ++iDstLine;

        rgbDotMatrixDsp[w*iDstLine + i] = (c1&BIT0); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c1&BIT1); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c1&BIT2); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c1&BIT3); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c1&BIT4); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c1&BIT5); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c1&BIT6); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c1&BIT7); ++iDstLine;

        rgbDotMatrixDsp[w*iDstLine + i] = (c2&BIT0); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c2&BIT1); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c2&BIT2); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c2&BIT3); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c2&BIT4); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c2&BIT5); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c2&BIT6); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c2&BIT7); ++iDstLine;

        rgbDotMatrixDsp[w*iDstLine + i] = (c3&BIT0); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c3&BIT1); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c3&BIT2); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c3&BIT3); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c3&BIT4); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c3&BIT5); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c3&BIT6); ++iDstLine;
        rgbDotMatrixDsp[w*iDstLine + i] = (c3&BIT7); ++iDstLine;

        p += 4;
    }
}
#endif

/**
 * old_lattice: ԭ�������ݣ�ȡģ��ʽ������ʽ����λ��ǰ��8������ռ1�ֽ�
 * new_lattice: �µ������ݣ�ÿ������ռ1�ֽ�
 * width��      �µ����
 * height:      �µ����(ע���߱�����8��������)
 * ע��
 *     �ڴ��ɵ����߷���
 */
void DotMatrixConvert(unsigned char* old_lattice, unsigned char* new_lattice, int width, int height)
{
    unsigned char tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
    int offset_byte = 0;    // ������ռ�ֽ�ƫ��
    unsigned char* p = NULL;
    unsigned char* q = NULL;
    int i = 0;
    int j = 0;

    if (old_lattice == NULL || new_lattice == NULL)
        return;
    p = old_lattice;
    q = new_lattice;

    for(i = 0; i < width; i++)
    {
        for (j = 0; j < height/8; j++)
        {
            offset_byte = j*8;
            // һ���ֽ�
            /// ��λ��ǰ
            tmp1 = (*p & 0x01) >> 0;
            tmp2 = (*p & 0x02) >> 1;
            tmp3 = (*p & 0x04) >> 2;
            tmp4 = (*p & 0x08) >> 3;
            tmp5 = (*p & 0x10) >> 4;
            tmp6 = (*p & 0x20) >> 5;
            tmp7 = (*p & 0x40) >> 6;
            tmp8 = (*p & 0x80) >> 7;
            p++;
            // height/8���ֽ�
            q[width * (0+offset_byte) + i] = tmp1;
            q[width * (1+offset_byte) + i] = tmp2;
            q[width * (2+offset_byte) + i] = tmp3;
            q[width * (3+offset_byte) + i] = tmp4;
            q[width * (4+offset_byte) + i] = tmp5;
            q[width * (5+offset_byte) + i] = tmp6;
            q[width * (6+offset_byte) + i] = tmp7;
            q[width * (7+offset_byte) + i] = tmp8;
        }
    }
}

