//�������ƣ�typedef.h
//�� �� �ţ�1.0
//��    �ܣ��ṩ�Գ�����ʹ�õ����Ͷ��塣���ڼ�д������ȷ����
//�� �� �ˣ�lrn
//����ʱ�䣺2005-03-01


//�������Ͷ���
#ifndef _TYPEDEF_H
#define _TYPEDEF_H



typedef char             int8;
typedef short            int16;
typedef long             int32;
typedef unsigned char    uint8;
typedef unsigned char    byte;
typedef unsigned short   uint16;
typedef unsigned long    uint32;

//����ʹ�õ������ִ�
#ifdef WIN32
    #define PRN    Debug_Var   //�������
#else
    #define PRN    printf
#endif

//_DEBUG��TYPEDEF.H�ж��壬WINDOWS�²��ö��壬�ɵ��԰汾������
//#define _DEBUG

//����Ϊ���Ժ�������궨�塣
#ifdef _DEBUG

#ifdef WIN32
    #define PRND    Debug_Var   //�������
#else
    #define PRND    printf
#endif

#define PRNF    Debug_For   //�ֽ������ӡ

#else  //_DEBUG

#define PRND      //�������
#define PRNF      //�ֽ������ӡ
 
#endif

#endif
