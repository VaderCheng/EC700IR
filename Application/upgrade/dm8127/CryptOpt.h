#ifndef __CRYPT_OPT_H__
#define __CRYPT_OPT_H__
//����оƬ�������
class CCryptOpt
{
public:

	//��ʼ�����ܹ���
	static int CryptInit(void);

	//��дAT88(��NC��KEY)
	static int InitCryptConfig(unsigned char* nc, unsigned char* key);

	//���ܶ�
	static int CryptRead(unsigned int unAddr,unsigned char *pBuf,unsigned int *Len);
	//int CryptWrite()

	//�۶���˿
	static int FuseWrite(void);

	//��ȡ��˿״̬
	static int FuseGetStatus(unsigned char *pvalue);

	//��������
	static int DecryptUserData(unsigned char *data, unsigned int len);

	//�򵥽������ݣ����ڽ����ں��ļ�ϵͳ��
	static int SimpleDecryptData(unsigned char *data, unsigned int len,unsigned long *pos);

private:
	static int GetNcKey(unsigned int (*rgdwNC)[4], unsigned int (*prgdwKey)[4]);

	static int xxtea_decrypt(
		unsigned char* pbIn ,			//����
		unsigned int nLenght ,		//���볤��
		unsigned int* pKey			//128λ��Կ
		);

	static int crypt(unsigned char *addr, unsigned int len, unsigned long* pos);

};
#endif /*__CRYPT_OPT_H__*/