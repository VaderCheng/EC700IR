// ���ļ����������WINDOWS-936��ʽ
/*******************************************************************************
 ���ڵ����С��ʶ��
 һ����˵������ȡģ������ÿ8���������1���ֽڣ�ȡģʽ��ʽ�ֺ���������ֽ���
 �ֵ�λ��ǰ�͸�λ��ǰ����ˣ�����4��ȡģ��ʽ�����ļ�ʹ�õķ�ʽ������ȡģ(��
 ��ע��д��������ʽ��)����λ��ǰ��Ϊ�˷���DSP��ʾ���Ὣ���ַ�ʽ�ĵ���ת����ÿ
 ����ռ1�ֽڵ��µĵ��󣬴������ң������������У�Ϊ1����ʾ��Ϊ0�߲���ʾ��
 ����Ϊ�ַ���1����12*12����(����ʾ����������ʵ�����)��
 000001000000
 000111000000
 000001000000
 000001000000
 000001000000
 000001000000
 000001000000
 000001000000
 000001000000
 000001000000
 000001000000
 000111111000

 �����˽⣬��ʹ��������������������˽����֪ʶ��
*******************************************************************************/
#ifndef _CHARACTEROVERLAP_H
#define _CHARACTEROVERLAP_H

#include "CameraController.h"
#include "HvTime.h"
#include "DspLinkCmd.h"
#include "HvDspLinkApi.h"

// ʱ����ʾ�ַ�����Ϊstrlen("2012-04-19 09:02:10:999  ") = 25
#define TIMEINFO_LEN 25

// nλ(���ֽ�)����
#define ALIGN(x, n) (((x)+(n)-1)&~((n)-1))

// ע�����к�ʹ�ú������棬���ӿ���չ��
//#define CHARACTER_WIDTH 24                          // 24����
//#define CHARACTER_HALFWIDTH (CHARACTER_WIDTH/2)     // ��ǿ�(24����Ϊ12)
//#define CHARACTER_HEIGHT ALIGN(CHARACTER_WIDTH, 8)  // ��(24����Ϊ24)
//#define CHARACTER_BYTE ((ALIGN(CHARACTER_WIDTH, 8))/8)  // �������ռ�õ��ֽ���
//#define FULLWIDTH_SIZE (ALIGN(CHARACTER_WIDTH, 8)*CHARACTER_WIDTH/8)    // ȫ��ռ�ÿռ��ֽ�
//#define HALFWIDTH_SIZE (ALIGN(CHARACTER_WIDTH, 8)*CHARACTER_WIDTH/2/8)  // ���ռ�ÿռ��ֽ�

// ��ȡ����������Ժ���
// ע��iFontSizeָ���������С�����ǵ������ݵĴ�С��
// �������СΪ24����ָ������ĵ���Ϊ24x24

// ��ȡһ����������İ�ǿ�
inline int GetFontHalfWidth(int iFontSize)
{
    return iFontSize / 2;
}

// ��ȡһ����������ĸ�
// ע����������Ϊ�������Ը߱�����8��������
inline int GetFontHeight(int iFontSize)
{
    return ALIGN(iFontSize, 8);
}

// ��ȡһ�������������ռ���ֽ���
inline int GetFontHeightByte(int iFontSize)
{
    return ALIGN(iFontSize, 8) / 8;
}

// ��ȡһ����������İ�Ǵ�С
inline int GetFontHalfWidthSize(int iFontSize)
{
    return ALIGN(iFontSize, 8) * iFontSize / 2 / 8;
}

// ��ȡһ�����������ȫ�Ǵ�С
inline int GetFontFullWidthSize(int iFontSize)
{
    return ALIGN(iFontSize, 8) * iFontSize / 8;
}

class CCharacterOverlap : public ICharacterOverlap
{
public:
    CCharacterOverlap();
    virtual ~CCharacterOverlap();

    virtual HRESULT EnableCharacterOverlap(int iValue);
    virtual HRESULT SetCharacterOverlap(char* pLattice);
    virtual HRESULT SaveCharacterInfo(void);
    virtual HRESULT SetCharacterLightness(int iIsFixedLight);
    HRESULT MakeDateTimeString(BYTE8* pDateTimeString, DWORD32 dwTimeLow, DWORD32 dwTimeHigh);

private:
    STRING_OVERLAY_PARAM m_cParam;
    STRING_OVERLAY_DATA m_cData;
    STRING_OVERLAY_RESPOND m_cRespond;
};

#endif // _CHARACTEROVERLAP_H
