#ifndef __HV_IMAGE_H__
#define __HV_IMAGE_H__
/*
���ܣ�
1��CHVImageͼ�����࣬Ŀǰ��Ҫ����YUV422��RGB24,����ͼ���ʽ��δ��֤
Ϊ�˴����ϵ�ͳһ�������е�����ʵ����cpp�ļ��ϣ�Ŀ����Ϊ�˲����û�ֱ�ӷ�������
ͳһͨ��CHVImage::Create������һ��ͼ�����ͣ���ͼ��ת���ϣ�ͨ��Draw��������ת��
2��CRegion���������,ͼ�����о������õ�ͨ����ֵͼ������ͨ�ԣ�Ϊ�˷������ͼ�����ͨ�ԣ��ر�д����
3��CHVText��ͼ����д���֣���ʱΪ����ͼ���ϸ���������Ϣ��ͨ�����������ͼ����д������Ϣ
�������ƹ���
���ڣ�2010-08-10

����ֱ����CHVImage ������һ��ͼƬ��������CHVImage::Create����������ĳһ���͵�ͼƬ��Ȼ��ͨ��Draw��������ͼ���ʽת��
����: ��jpg�ļ���ѹ��YUV422��ʽ��Ȼ����ת��ΪYV12��ʽ

//����YUV422ͼƬ
CHVImage * yuv422 = CHVImage::Create(0,0,CHVImage::YUV422);
//����ͼƬ
if(yuv422->Load("test.jpg"))
{
//����YV12ͼƬ
CHVImage * yv12 = CHVImage::Create(yuv422->GetWidth(),yuv422->GetHeight(),CHVImage::YV12);
//ͨ��yuv422������Ƶ�yv12����ʵ�ָ�ʽת��
yuv422->Draw(yv12);
//��yv12����ʾ����
CHVText text = "����yv12ͼƬ";
text.SetFont("������",100).SetColor(YUYV(82,90,82,240))).Draw(10,10,yv12);
//��ʾyv12ͼ��
yv12->Draw(GetActiveWindow());
}

//Ҳ��������ͼ��ϲ������´���
CHVImage * img1 = CHVImage::Create(100,100,CHVImage::YUV422);
CHVImage * img2 = CHVImage::Create(100,100,CHVImage::YUV422);
CHVImage * img3 = CHVImage::Create(100,200,CHVImage::YUV422);
//��ͼ��img1��img2�ϲ���img3��
img1->Draw(img3,0,0,img1->GetWidth(),img1->GetHeight(),0,0);
img2->Draw(img3,0,0,img2->GetWidth(),img2->GetHeight(),img1->GetWidth(),0);

�޸���־��

2010-08-02  ����CHVImage�ඨ������ӿں���
2010-08-05  ���CHVRegion�࣬ͳһ����CHVImage����ͨ����
2010-08-06  ����CHVText�࣬ͳһ������ͼ����д������Ϣ
2010-08-10  �޸�CHVImage�࣬���ڴ���ʵ��YUV422��jpg��ѹ����ѹ
�޸�CHVImage�࣬���Bayer8ToYUV422��ʵ�֣�������Ҫ��ת����RGBȻ������RGB��YUV422��
�ú���ת��һ��2448*2050��ת����ʱ80���룬�����ϴﵽ12fps��Ҫ��
2010-08-11  Ϊ�˿�ƽ̨��ʵ��win32���еĺ����ͽṹ�壬�޸�Bayer8TOYUV422������ʹ�ò�����ת��Ч��ת��һ��2448*2050ͼ��ʱ60����
2010-08-11  CHVRegion����Clear�����������ͨ����,�޸�[]���������ȡֵ������ͨ������������������ڴ棬����һ���յľ���
2010-08-20  Ϊ��֧�ָ����YUV��ʽ��������YUY2,YVYU,UYVY,YUV444���࣬��������ijl51.lib��������windows���ܹ����ٽ�ѹJPEGͼƬ
2010-08-20  Ϊ������̳еķ��㣬������Resize����Ϊ���麯���������ľ�Ϊ�麯������������Ĵ�������δ�̳еĺ�����ʾû�иù���
2010-08-21  �����˸���YUV��ʽ��ת��;������ʾͼ�񵽴����ϵĺ���Draw(HWND)����Ҫ����CDirectDraw���л���
2010-08-23  �޸�YUV�궨�壬ԭ����YUYV�ĺ��޸�ΪYUV�꣬��Ϊһ�����ص��Ӧһ��YUVֵ������ĳ��YUV���͵ĺ�㣬���ຯ���Լ�ȥת��
2010-08-24  ����HSLȡ�ó�����ɫ��ȡֵ��Χ�������Ժ��ж�
2010-08-25  ���ӹ��˵�ĳЩ��ɫ���߽�����ĳЩ��ɫ�ĺ���Filter,����ͼ���������Adjust,����ͼ��Ŵ���С����Resample
2010-08-26  ����YV12��Y411��ͼ��ת������,�޸�YUV422����JPEGͼ�񷽷���ͨ���ȼ��ص�YUV444Ȼ����ת��
2010-08-27  ��������ָ��auto_ptr,���Խ�һ��ָ�����ͱ�������һ����ͨ�ı�����������������Զ��ͷ��ڴ�
2010-09-01  Ϊ���ܹ�����ǿ��4���ı�׼ͼ����Ҫ����ɨ���еĸ��ÿ�δ���һ��ɨ����,ĿǰYUV411�����д�������
2010-11-11  ����֧��IReferenceComponentImage�ӿ�,���ӽ�ѹͼ���ͬʱ������ת�Ĺ���
2010-11-12  ����������֮��̳еĹ�ϵ��ͨ��dcastת��ָ�룬ȷ����ָ��ת����ȷ
2010-11-23  ���Ӵ����ֵͼ�Ĺ��ܣ�Ϊ�˸�����Ĵ���ʶ�����Ķ�ֵͼ
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�궨��
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define dcast(cls, ptr)    (ptr && (ptr)->InternalIsDescendant(cls::Class()) ? (cls *)ptr : NULL)
#define CLASSINFO(cls, par) \
  public: \
  static const char * Class(){ return #cls; } \
  virtual bool InternalIsDescendant(const char * clsName) const {return strcmp(clsName, cls::Class()) == 0 || (strcmp(cls::Class(),par::Class()) && par::InternalIsDescendant(clsName));}
#ifdef min
#undef min
#endif
inline int min(int a, int b){ return ((a)<(b)?(a):(b));}
#ifdef max
#undef max
#endif
inline int max(int a, int b){return ((a)>(b)?(a):(b));}
#ifdef mid
#undef mid
#endif
inline int mid(int a, int min, int max){ if(a < min) a = min; if(a > max) a = max; return a;}
//RGB��ɫ�ռ�
#ifndef RGB
#define RGB(r,g,b)        ((DWORD)mid(r,0,255))|(((DWORD)mid(g,0,255))<<8)|(((DWORD)mid(b,0,255))<<16) 
#endif
#ifndef R
#define R(rgb)            ((BYTE) (rgb))
#endif
#ifndef G
#define G(rgb)            ((BYTE) (((WORD) (rgb)) >> 8))
#endif
#ifndef B
#define B(rgb)            ((BYTE) (((DWORD)(rgb)) >> 16))
#endif
//YUV��ɫ�ռ�
#ifndef YUV
#define YUV(y,u,v)        RGB(y,u,v)
#endif
#ifndef Y
#define Y(yuv)            R(yuv)
#endif
#ifndef U
#define U(yuv)            G(yuv)
#endif
#ifndef V
#define V(yuv)            B(yuv)
#endif
//HSL��ɫ�ռ�
#ifndef HSL
#define HSL(h,s,l)        RGB(mid(h,0,240),mid(s,0,240),mid(l,0,240))
#endif
#ifndef H
#define H(hsl)            R(hsl)
#endif
#ifndef S
#define S(hsl)            G(hsl)
#endif
#ifndef L
#define L(hsl)            B(hsl)
#endif
//ת������
#define RGB2YUV(rgb)      CHVImage::RGBtoYUV(rgb)
#define YUV2RGB(yuv)      CHVImage::YUVtoRGB(yuv)
#define HSL2RGB(hsl)      CHVImage::HSLtoRGB(hsl)
#define RGB2HSL(rgb)      CHVImage::RGBtoHSL(rgb)
#define YUV2HSL(yuv)      RGB2HSL(YUV2RGB(yuv))
#define HSL2YUV(hsl)      RGB2YUV(HSL2RGB(hsl))
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����ͷ�ļ�
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _MSC_VER
#include "swImageObj.h"
#include "gba-jpeg-decode.h"
#endif
#define                                 RGB_YUV_TABLE  //ͨ�����ת��RGB��YUV
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Windows����
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include <memory>
using namespace std;
#define XJPEG           //jpeg��Դ��
#define INTEL           //intel��jpeg��
#define DDRAW           //ʹ��ddraw����ͼƬ
#ifdef  DDRAW
class CDirectDraw;
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��Windows����
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
#include <std.h>
#include <string.h>
#include <mem.h>

#define BI_RGB   0

//����ָ��
template<class T>class auto_ptr
{
public:
	auto_ptr(T * ptr = NULL){m_Ptr = ptr;}
	virtual ~auto_ptr(){if(m_Ptr)delete m_Ptr;}
	T & operator *() {return *m_Ptr;}
	T * operator ->(){return m_Ptr;}
	operator T*()    {return m_Ptr;}
private:
	T * m_Ptr;
};
//���Ͷ���
typedef char           CHAR;
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned int   DWORD;
typedef long           LONG;
//�õ��Ľṹ��
typedef struct tagRGBQUAD
{ 
	BYTE    rgbBlue; 
	BYTE    rgbGreen; 
	BYTE    rgbRed; 
	BYTE    rgbReserved; 
} RGBQUAD; 

typedef struct tagBITMAPFILEHEADER
{
	WORD    bfType; 
	DWORD   bfSize; 
	WORD    bfReserved1; 
	WORD    bfReserved2; 
	DWORD   bfOffBits; 
} BITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER
{
	DWORD  biSize; 
	LONG   biWidth; 
	LONG   biHeight; 
	WORD   biPlanes; 
	WORD   biBitCount; 
	DWORD  biCompression; 
	DWORD  biSizeImage; 
	LONG   biXPelsPerMeter; 
	LONG   biYPelsPerMeter; 
	DWORD  biClrUsed; 
	DWORD  biClrImportant; 
} BITMAPINFOHEADER;

typedef struct tagRECT
{
	int left;
	int top;
	int right;
	int bottom;
}RECT;

typedef struct tagPOINT
{
	int x;
	int y; 
} POINT;
typedef struct tagLOGFONT
{
	LONG lfHeight; 
	LONG lfWidth; 
	LONG lfEscapement; 
	LONG lfOrientation; 
	LONG lfWeight; 
	BYTE lfItalic; 
	BYTE lfUnderline; 
	BYTE lfStrikeOut; 
	BYTE lfCharSet; 
	BYTE lfOutPrecision; 
	BYTE lfClipPrecision; 
	BYTE lfQuality; 
	BYTE lfPitchAndFamily; 
	CHAR lfFaceName[32];
} LOGFONT;

//���κ���
bool CopyRect(RECT * lprcDst, const RECT *lprcSrc);
bool EqualRect(const RECT *lprc1, const RECT *lprc2);
bool InflateRect(RECT * lprc, int dx, int dy);
bool IntersectRect(RECT * lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2);
bool IsRectEmpty(const RECT *lprc);
bool OffsetRect(RECT * lprc, int dx, int dy);
bool PtInRect(const RECT *lprc, POINT pt);
bool SetRect(RECT * lprc, int xLeft, int yTop, int xRight, int yBottom);
bool SetRectEmpty(RECT * lprc);
bool SubtractRect(RECT * lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2);
bool UnionRect(RECT * lprcDst, const RECT *lprcSrc1, const RECT *lprcSrc2);
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ඨ��
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CHVImage;
/*
ͼ������࣬����ָ������ɫѰ����ͨ����
*/
class CHVRegion
{
public:
	CHVRegion(CHVImage * image);
	virtual  ~CHVRegion(void);
	//������ɫcolor����ͼ�����ͨ����
	CHVRegion & Create(DWORD color, RECT * rc = NULL);
	//�����������
	CHVRegion & Clear(void);
	//�ϲ����ڻ��߽������ͨ����,intervalΪ2����ͨ�������ڵ����أ�С�ڸ�������ϲ�
	CHVRegion & Union(int interval=3);
	//�����е���ͨ�����������Ŀǰ��ˮƽ�Ĵ����Һʹ�ֱ���ϵ���2������
	CHVRegion & Sort(bool m_bHorizon = true);
	//����ͨ������color���ƾ��α߿�
	CHVRegion & Draw(DWORD color);
	//ɾ��ĳ����ͨ����
	int       Delete(int index);
	//�����ͨ����ľ�������
	operator RECT *(){return rgnRect;}
	//��õ�index����ͨ����ľ���
	RECT    & operator[](int index);
	//�����ͨ����ĸ���
	int       GetCount(void){return rgnCount;}
protected:
	//�ж���ͨ�����Ƿ�����
	bool     IsNeighbor(RECT rc1,RECT rc2,int interval);
private:
	CHVImage * image;
	int * buffer;
	int **rgn;
	RECT * rgnRect;
	int   rgnCount;
	int   rgnSize;
};
/*
ͼ�����࣬������һ���ӿ��࣬����ֱ�Ӷ��壬��Ҫ��CHVImage::Create��������Ӧ��ͼ���ʽ��Ŀǰ֧����Bayer8��RGB1��RGB4��RGB24�͸���YUV��ʽ��HSL��ʽ
*/
class CHVImage
#ifdef _SWIMAGEOBJ_INCLUDED__
	:public IReferenceComponentImage
#endif
{
public:
	CHVImage();
	virtual ~CHVImage();
#ifdef _SWIMAGEOBJ_INCLUDED__
	CLASSINFO(CHVImage,IReferenceComponentImage)
#else
	CLASSINFO(CHVImage,CHVImage)
#endif
		//ͼ������,ͼ��ĸ�ʽת��������draw������һ�ָ�ʽת��������һ�ָ�ʽ��Ҳ����ֱ�����þ�̬��������ת��
		enum HVImageType
	{
		NONE,     //�����ͣ���ʼֵ
		BIN,      //���ƶ�ֵͼ,Ҳ����RGB1��ʽ������
		RGB1,
		RGB4,
		RGB8,
		RGB15,
		RGB16,
		RGB24,
		RGB32,    //RGBͼ���ʽ
		YUV444,   //ԭʼ��YUVͼ���ʽ
		YUV422,   //YUV422��ʽ��Ŀǰ��˾�õ��Ǹø�ʽ��y u v ��ƽ���ʽ���
		YUY2,     //��׼��yuv422��ʽ��yuv�Դ���ĸ�ʽ���
		YVYU,     //ͬ�ϣ�ֻ��yuv����һ�����ѣ��Ƚ�����
		UYVY,     //ͬ�ϣ�ֻ��yuv����һ�����ѣ��Ƚ�����
		YV12,     //��׼��yuv420��ʽ��yuv��ƽ���ʽ���
		Y411,     //yuv411��ʽ��Ŀǰ��˾Ҳ�øø�ʽ��y u v ��ƽ���ʽ���
		HSL       //hsl��ʽ
	};
	//����ĳһ���͵�ͼ��
	static CHVImage * Create(int inWidth,int inHeight,HVImageType inType=NONE);
	//�趨�û��Զ������
	CHVImage & SetData(int data){m_iUserData = data; return *this;}
	//ȡ���û��Զ������
	int      GetData(void)   {return m_iUserData;}
	//ͼ������
	HVImageType GetType(void){return m_Type;}
	//ͼ����
	int      GetWidth(void)  {return m_iWidth;}
	//ͼ��߶�
	int      GetHeight(void) {return m_iHeight;}
	//һ��ɨ���ߵ��ֽ���
	int      GetStride(void) {return m_iStride;}
	//ͼ������
	BYTE   * GetBits(void)   {return m_bImage;}
	//ͼ���С
	int      GetSize(void)   {return m_iStride * m_iHeight;}
	//����ͼ���С������֮��ԭ����ͼ�����ݶ�ʧ��������������ͼ�����������Resample
	virtual  CHVImage & Resize(int width, int height){return *this;}
	//ȡ��ͼ��ĳһ�������ֵ
	virtual  DWORD GetPixel(int x,int y);
	virtual  CHVImage & GetPixel(int x, int y, RGBQUAD * color);
	//����ͼ��ĳһ�������ֵ
	virtual  CHVImage & PutPixel(int x,int y,DWORD color);
	virtual  CHVImage & PutPixel(int x, int y, RGBQUAD color);
	//����ֱ��
	virtual  CHVImage & Line(int x0,int y0, int x1,int y1,DWORD color);
	virtual  CHVImage & Line(POINT pt1,POINT pt2,DWORD color);
	//���ƾ���
	virtual  CHVImage & Rectangle(int x0,int y0, int x1,int y1,DWORD color);
	virtual  CHVImage & Rectangle(POINT topLeft,POINT rightBottom, DWORD color);
	virtual  CHVImage & Rectangle(RECT rc, DWORD color);
	//ȡ�ø�ͼ�����ͨ����ʵ����ȡ��֮����Ҫ����CHVRegion::Create��������ͨ����
	virtual  CHVRegion & GetRgn(void){return m_Rgn;}
#if defined(WIN32) && defined(DDRAW)
	//����DirectDrawʵ��
	CDirectDraw & GetDDraw(void){return *ddraw;}
	//�ڴ����ϻ�ͼ������YUV422��YV12��RGB��ͼ���ʽ����ֱ����ʾ�������ĸ�ʽ��Ҫת��
	virtual  CHVImage & Draw(HWND hWnd);
	//����ͼ����ʾ
	virtual  CHVImage & Invalidate(void);
#endif
	//ͼ�񿽱�����������ʵ�ָ���ͼ�θ�ʽ��ת����ͼ��Ľ�ȡ�ȵ�
	virtual  CHVImage & Draw(CHVImage * dstImage, RECT * dstRC = NULL, RECT * srcRC = NULL, int angle = 0, bool m_bFlip = false, bool m_bMirror = false);
	virtual  CHVImage & Draw(CHVImage * dstImage, int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, int srcWidth, int srcHeight, int angle = 0, bool m_bFlip = false, bool m_bMirror = false);
	//ͼ��ת��Ŀǰ���з�ת90�Ⱥ�180��
	virtual  CHVImage & Rotate(int angle=90);
	//ͼ��ת
	virtual  CHVImage & Flip(void);
	//ͼ����
	virtual  CHVImage & Mirror(void);
	//ͼ�����,ɫ��[-240,240]�����Ͷ�[-240,240]������[-240,240]
	virtual  CHVImage & Adjust(int addHue, int addSaturation, int addLight, RECT * rc = NULL);
	//ͳ��ɫ��[0,240]������[0,240]��ĳһ��Χ��ֵ,����ָ�������ƽ��ɫ��������
	virtual  CHVImage & GetAvgHueAndBright(int minHue, int maxHue, int * retAvgHue, int minLight = 0, int maxLight = 240, int * retAvgLight = NULL, RECT * rc = NULL);
	//ת���Ҷ�ͼƬ
	virtual  CHVImage & ToGray(RECT * rc = NULL);
	//ת����ֵͼ
	virtual  CHVImage & ToBin(RECT * rc = NULL);
	//���ݸ�����ɫ�ȷ�Χ[ɫ��Ϊһ��Բ����,����240������㿪ʼ��,����startHue=200,endHue=10]�����ȷ�Χ���ж�ֵͼ,�ڸ÷�Χ�ڵ�ֵ��Ϊ��ɫ������Ϊ��ɫ,m_bInvert=true��ڰ��෴
	virtual  CHVImage & ToBin(int startHue, int endHue, int minLight, int maxLight, bool m_bInvert = false, RECT * rc = NULL);
	//ͼ��Ŵ���С
	virtual CHVImage  & Resample(int newWidth, int newHeight, CHVImage **image = NULL);
	//���ļ�����ͼ��YUV��Ӧjpgͼ��,m_bIJL=true��ʾ��ijl��ѹ����ѹ,false���ʾ��libjpeg����ѹ����ѹ,����jpeg�ļ���Ч
	virtual  bool Load(const char * fname, int angle=0, bool m_bHalfHeight = false, bool m_bIJL = true);
	//���ڴ����ͼ��,�������Ƿ�Ҫ��תͼƬ,angle=0,90,180,270
	virtual  bool Load(const BYTE * bImage, int size, int angle=0, bool m_bHalfHeight = false, bool m_bIJL = true);
	//����Bayer8����
	virtual  bool LoadFromBayer8(BYTE * bayer8, int width, int height, int angle = 0, bool m_bHalfHeight = false);
	//����ͼ���ļ�
	virtual  bool Save(const char * fname, bool m_bIJL = true);
	//����ͼ���ڴ�
	virtual  bool Save(BYTE * bImage, int & size, bool m_bIJL = true);
public:
	//����һ��ɨ����
	virtual  bool CopyALine(int srcX,int srcY, CHVImage *dstImage,int dstX,int dstY, int width);
	//д��һ��ɨ���ߣ�m_bVert��ֱ����ˮƽ��Ĭ����ˮƽ, m_bNormal������[���ϵ���]���Ǵ��ҵ���[���µ���]
	virtual void WriteAScanLine(int x, int y, BYTE * line, int size, bool m_bVert = false, bool m_bNormal = true);
	//��ȡ�Ժ�ɨ����,scaleX �ǷŴ�����*16384[Ϊ�˷�����㣬����2��14�η�]
	virtual void ReadAScanLine(int x, int y, int scaleX, BYTE *line, int size, HVImageType type = NONE);
#ifdef _SWIMAGEOBJ_INCLUDED__
public:
	virtual  bool Load(IReferenceComponentImage * jpgImage, bool m_bHalfHeight = false, CItgArea* pItgArea = NULL,bool m_bTurn = false, bool m_bIJL = true);
	virtual  CHVImage & SetHalfHeight(bool m_bHalf = true);
	virtual  bool IsHalfHeight(void);
	virtual  IReferenceComponentImage  * Save(void);

	STDMETHOD(GetImage)(HV_COMPONENT_IMAGE* pImage)              {return m_nRefImage ? m_nRefImage->GetImage(pImage):E_FAIL;}
	STDMETHOD(Assign)(const HV_COMPONENT_IMAGE& imgInit)         {Resize(imgInit.iWidth, imgInit.iHeight); return m_nRefImage ? m_nRefImage->Assign(imgInit) :E_FAIL;}
	STDMETHOD(Convert)(const HV_COMPONENT_IMAGE& imgInit)        {return m_nRefImage ? m_nRefImage->Convert(imgInit):E_FAIL;}
	STDMETHOD(Serialize)(bool fOut)                              {return m_nRefImage ? m_nRefImage->Serialize(fOut) :E_FAIL;}
	virtual void AddRef()                                        {m_cRef++;}
	virtual void Release()                                       {m_cRef--; if(!m_cRef)delete this;}
	virtual void ClearYUV()                                      {if(m_nRefImage)m_nRefImage->ClearYUV();}
	virtual DWORD32 GetFrameNo()                                 {return m_nRefImage ? m_nRefImage->GetFrameNo()  :0;}
	virtual char *GetFrameName()                                 {return m_nRefImage ? m_nRefImage->GetFrameName():NULL;}
	virtual DWORD32 GetRefTime()                                 {return m_nRefImage ? m_nRefImage->GetRefTime()  :0;}
	virtual DWORD32 GetFlag()                                    {return m_nRefImage ? m_nRefImage->GetFlag()     :0;}
	virtual IReferenceComponentImage *GetLastRefImage()          {return m_nRefImage ? m_nRefImage->GetLastRefImage():NULL;}
	virtual IReferenceComponentImage *GetNextRefImage()          {return m_nRefImage ? m_nRefImage->GetNextRefImage():NULL;}
	virtual void SetFrameNo(DWORD32 dwFrameNo)                   {if(m_nRefImage)m_nRefImage->SetFrameNo(dwFrameNo);}
	virtual void SetFrameName(const char *lpszFrameName)         {if(m_nRefImage)m_nRefImage->SetFrameName(lpszFrameName);}
	virtual void SetRefTime(DWORD32 dwRefTime)                   {if(m_nRefImage)m_nRefImage->SetRefTime(dwRefTime);}
	virtual void SetFlag(DWORD32 dwFlag)                         {if(m_nRefImage)m_nRefImage->SetFlag(dwFlag);}
	virtual void SetLastRefImage(IReferenceComponentImage *pLast){if(m_nRefImage)m_nRefImage->SetLastRefImage(pLast);}
	virtual void SetNextRefImage(IReferenceComponentImage *pNext){if(m_nRefImage)m_nRefImage->SetNextRefImage(pNext);}
	virtual void SetImageContent(HV_COMPONENT_IMAGE& img)        {if(m_nRefImage)m_nRefImage->SetImageContent(img);}
	virtual void SetImageSize(const HV_COMPONENT_IMAGE& img)     {if(m_nRefImage)m_nRefImage->SetImageSize(img);}
protected:
	IReferenceComponentImage * m_nRefImage;
	int m_cRef;
#endif
public:
	//RGB��RGBQUADת��
	static inline DWORD RGBQUADtoRGB(RGBQUAD c);
	static inline RGBQUAD RGBtoRGBQUAD(DWORD rgb);
	//hue--ɫ��;saturation--���Ͷ�;value--����
	static inline void  HSLtoRGB(BYTE h, BYTE s, BYTE l, BYTE *r, BYTE *g, BYTE *b);
	static inline DWORD HSLtoRGB(DWORD hsl);
	static inline void  RGBtoHSL(BYTE r, BYTE g, BYTE b, BYTE *h, BYTE *s, BYTE *l);
	static inline DWORD RGBtoHSL(DWORD rgb);
	static inline void  YUVtoRGB(BYTE y, BYTE u, BYTE v, BYTE *r, BYTE *g, BYTE *b);
	static inline DWORD YUVtoRGB(DWORD yuv);
	static inline void  RGBtoYUV(BYTE r, BYTE g, BYTE b, BYTE *y, BYTE *u = NULL, BYTE *v = NULL);
	static inline DWORD RGBtoYUV(DWORD rgb);
protected:
	HVImageType m_Type;
	int      m_iWidth;
	int      m_iHeight;
	int      m_iStride;
	BYTE   * m_bImage;
	int      m_iUserData;
	//ͼ������
	CHVRegion  m_Rgn;
	//ͼ����ר��
	CHVImage * rotateImage;
	CHVImage * mirrorImage;
#if defined(WIN32) && defined(DDRAW)
	CDirectDraw * ddraw;
#endif
#ifdef RGB_YUV_TABLE
	static bool m_bInitTable;
	static int  V_r[256],U_g[256],V_g[256],U_b[256];
	static int  R_y[256],G_y[256],B_y[256],By_u[256],Ry_v[256];
#endif
};

/*
�ı������࣬ͨ�����������ͼ������ʾ����
*/
class CHVText : public CHVImage
{
public:
	CLASSINFO(CHVText,CHVImage)
		CHVText();
	//����
	CHVText(const char * text);
	//ʱ��
	CHVText(time_t t);
	virtual ~CHVText();
	//���ز���������ֵַ�������ʱ�����?
	CHVText & operator=(const char * text);
	CHVText & operator=(time_t t);
	CHVText & SetText(const char * text);
	CHVText & SetTime(time_t t);
	//�������ֵ���ɫ
	CHVText & SetTextColor(DWORD color=RGB(255,0,0));
	//���ñ���ɫ
	CHVText & SetBkColor(DWORD color=RGB(0,0,0));
	//���ñ���͸���ȣ�����50����͸����Ϊ50%
	CHVText & SetTransparent(int iTransparent = 50);
	//������������ͺʹ�С
	CHVText & SetFont(const char * fontName="������", int fontSize=0, int fontWeight=400/*FW_NORMAL*/, bool m_bItalic=FALSE, bool m_bUnderline=FALSE);
	//��ͼƬ����ʾ������Ϣ
	CHVText & Draw(int x, int y, CHVImage * image);
protected:
	//���ػ�����麯��
	virtual  CHVImage & Resize(int width, int height);
	virtual  DWORD GetPixel(int x,int y);
	virtual  CHVImage & PutPixel(int x,int y,DWORD color);
private:
	DWORD   textColor;
	DWORD   bkColor;
	int     m_iTransparent;
	LOGFONT logFont;
};
#endif

