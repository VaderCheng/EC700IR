#ifndef _BASE64_H
#define _BASE64_H

//���������Ҫ��������泤��,�˳��Ȳ������������ַ����Ľ�β'\0'
inline unsigned int base64_code_size(const unsigned int data_size)
{
	return (data_size+2)/3*4;
}

typedef enum _B64ReultType{  
	b64Result_OK=0, 
	b64Result_CODE_SIZE_ERROR, 
	b64Result_DATA_SIZE_SMALLNESS,
	b64Result_CODE_ERROR
}
B64ReultType;

void base64_encode(
		const void* pdata,	//ԭʼ����
		const unsigned int data_size,	//ԭʼ���ݳ���
		void* out_pcode	//���BASE64����
		);

B64ReultType base64_decode(
				const void* pcode,	//BASE64����
				const unsigned int code_size,	//���볤��
				void* out_pdata,	//���
				const unsigned int data_size,	//������泤��
				unsigned int* out_pwrited_data_size	//ʵ���������
				);

//////////////////////////////////////////////////////////////////////////
//������

class CBase64Encoder
{
public:
	CBase64Encoder(const void* pData, unsigned int nDataLen);
	~CBase64Encoder();
public:
	const char* ToStr();
	unsigned int GetLen();
protected:
	char* m_pEncBuf;
	unsigned int m_nBufLen;
};

class CBase64Decoder
{
public:
	//pCode: ��'\0'��β��Base64�����ַ���
	CBase64Decoder(const char* pCode);
	~CBase64Decoder();
public:
	const void* ToBin();
	unsigned int GetLen();
protected:
	unsigned char* m_pDecBuf;
	unsigned int m_nBufLen;
	unsigned int m_nDecodeLen;
};

#endif
