#ifndef _AUTO_LINK_PARAM_H_
#define _AUTO_LINK_PARAM_H_

typedef struct _tagAutoLinkParam
{
    BOOL fAutoLinkEnable;                /**< ��������ʹ�� */
    char szAutoLinkIP[32];               /**< �������ӷ�����IP */
    int iAutoLinkPort;                   /**< �������ӷ������˿� */

    _tagAutoLinkParam()
    {
        fAutoLinkEnable = FALSE;
        iAutoLinkPort = 0;
        strcpy(szAutoLinkIP , "172.18.10.100");
    }
}AUTO_LINK_PARAM;



#endif
