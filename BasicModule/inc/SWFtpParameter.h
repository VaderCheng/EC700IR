#ifndef _FTP_PRARAM_H_
#define _FTP_PRARAM_H_

typedef struct _tagFtpParam
{
    BOOL fFtpEnable;             		 /**< FTPʹ�� */
    CHAR szFtpIP[32];              		 /**< FTP������IP */
	CHAR szUserName[32];              		 /**< FTP�û��� */
	CHAR szPassWord[32];              		 /**< FTP���� */
	INT iTimeOut;						
   	

    _tagFtpParam()
    {
        fFtpEnable = FALSE;
		swpa_strcpy(szFtpIP ,	"172.18.10.100");
        swpa_strcpy(szUserName , "anonymous");
		swpa_strcpy(szPassWord , "yetpasswd");
		iTimeOut = 5;
    }
}FTP_PARAM;



#endif
