#ifndef  CSWMD5EncryptFilter_H_
#define  CSWMD5EncryptFilter_H_

#include "SWFC.h"
#include "SWBaseFilter.h"
#include "SWMessage.h"

/* POINTER defines a generic pointer type */
typedef unsigned char * POINTER;

/* UINT2 defines a two byte word */
//typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;


/* MD5 context. */
typedef struct {
    UINT4 state[4];                                   /* state (ABCD) */
    UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

void MD5Init (MD5_CTX *context);
void MD5Update (MD5_CTX *context, unsigned char *input, unsigned int inputLen);
void MD5UpdaterString(MD5_CTX *context,const char *string);
int MD5FileUpdateFile (MD5_CTX *context,char *filename);
void MD5Final (unsigned char digest[16], MD5_CTX *context);
void MDString (char *string,unsigned char digest[], unsigned char* pbPublicKey, int wKeyLen);
void MDImage (unsigned char *pbImgData,int iSize, unsigned char digest[], unsigned char* pbPublicKey, int wKeyLen);
int MD5File (char *filename,unsigned char digest[16]);


class CSWMD5EncryptFilter: public CSWObject, CSWMessage
{
	CLASSINFO(CSWMD5EncryptFilter, CSWObject)
public:
    CSWMD5EncryptFilter();
    virtual ~CSWMD5EncryptFilter();

protected:
    /**
     *@breif ��ʼ��
     *@param [in] pbPublicKey ��Կ
     *@param [in] iKeyLen ��Կ����
     *@return �ɹ�����S_OKʧ�ܷ���E_FAIL
     */
    HRESULT InitMD5EncryptFilter(BOOL fEnable, CHAR* pbPublicKey, int iKeyLen);

    /**
     *@breif ����
     *@param [in] wParam, ��
     *@param [in] lParam, ��
     *@return �ɹ�����S_OKʧ�ܷ���E_FAIL
     */
    HRESULT OnMD5EncryptProcess(WPARAM wParam, LPARAM lParam);
    
    HRESULT OnSetEncryptParam(WPARAM wParam, LPARAM lParam);
    
    HRESULT OnGetEncryptCode(WPARAM wParam, LPARAM lParam);

    //�Զ���ӳ���
    SW_BEGIN_DISP_MAP(CSWMD5EncryptFilter, CSWObject)
        SW_DISP_METHOD(InitMD5EncryptFilter, 3)
    SW_END_DISP_MAP()

    //��Ϣӳ���
    SW_BEGIN_MESSAGE_MAP(CSWMD5EncryptFilter, CSWMessage)
        SW_MESSAGE_HANDLER(MSG_MD5_ENCRYPT_PROCESS, OnMD5EncryptProcess)
        SW_MESSAGE_HANDLER(MSG_SET_ENCRYPT_PARAM, OnSetEncryptParam)
        SW_MESSAGE_HANDLER(MSG_GET_ENCRYPT_CODE, OnGetEncryptCode)
    SW_END_MESSAGE_MAP()

private:
    char m_szPublicKey[1024];	//�ַ�����key,��ʱδʹ��
	BYTE m_bKey[16];			//�����Ƶ�key���̶�ֵ
    int  m_iKeyLen;
    BOOL m_fEnable;
};
REGISTER_CLASS(CSWMD5EncryptFilter)
#endif