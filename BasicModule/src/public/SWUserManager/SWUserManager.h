/*
 * SWUserManager.h
 *
 *  Created on: 2014��1��6��
 *      Author: qinjj
 */

#ifndef __SW_USERMANAGER_H__
#define __SW_USERMANAGER_H__
#include "SWObject.h"
#include "SWMessage.h"

#define ACOUNT_NUM				8 		///< How many acounts which are stored in system.
#define USER_LEN_MAX			32 		///< Maximum of acount username length.
#define MIN_USER_LEN			4 		///< Maximum of acount username length.
#define PASSWORD_LEN_MAX		16 		///< Maximum of acount password length.
#define MIN_PASSWORD_LEN		4 		///< Maximum of acount password length.
#define PASSWORD_ZERO           0       ///< minimum length of password>

/**
* @brief IPCAM user account data.
*/
typedef struct _Acount_t
{
	CHAR	user[USER_LEN_MAX];			///< username
	char	password[PASSWORD_LEN_MAX];	///< password
	DWORD	authority;					///< user authority

    _Acount_t()
    {
        memset(user, '\0', USER_LEN_MAX);
        memset(password, '\0', PASSWORD_LEN_MAX);
        authority = 0x1;
    };

}Acount_t;

class CSWUserManager: public CSWMessage, public CSWObject {
	CLASSINFO(CSWUserManager, CSWObject)
public:
	CSWUserManager();
	virtual ~CSWUserManager();

protected:
	/**
	* @brief MSG_USER_LOGIN �û���¼У��
	* @param [in] wParam ��¼�û��ṹ��ָ��
	* @param [out] lParam ��¼�û�Ȩ�ޣ�-1��¼ʧ��
	* @return �ɹ�����S_OK������ֵ����ʧ��
	*/
	HRESULT OnUserLogin(WPARAM wParam,LPARAM lParam);

	/**
	* @brief MSG_USER_GETLIST ��ȡ�û���Ϣ�б�
	* @param [in] wParam ����
	* @param [out] lParam ��¼�û��б�ṹ������ָ��
	* @return �ɹ�����S_OK������ֵ����ʧ��
	*/
	HRESULT OnUserGetList(WPARAM wParam,LPARAM lParam);

	/**
	* @brief MSG_USER_GETLIST ��ȡ�û���Ϣ�б��Լ�����
	* @param [in] wParam ����
	* @param [out] lParam ��¼�û��б�ṹ������ָ��
	* @return �ɹ�����S_OK������ֵ����ʧ��
	*/
	HRESULT OnGetListUsername_Passwd(WPARAM wParam,LPARAM lParam);


	/**
	* @brief MSG_USER_ADD �����û�
	* @param [in] wParam �����û���Ϣ�������û���Ϣ�ṹ������
	* @param [out] lParam ����
	* @return �ɹ�����S_OK������ֵ����ʧ��
	*/
	HRESULT OnUserAdd(WPARAM wParam,LPARAM lParam);

	/**
	* @brief MSG_USER_DEL ɾ���û�
	* @param [in] wParam �����û���Ϣ��ɾ���û���Ϣ�ṹ������
	* @param [out] lParam ����
	* @return �ɹ�����S_OK������ֵ����ʧ��
	*/
	HRESULT OnUserDell(WPARAM wParam,LPARAM lParam);

	/**
	* @brief MSG_USER_CHANGE �û����
	* @param [in] wParam �����û���Ϣ��ǰ�û������û���Ϣ�ṹ������
	* @param [out] lParam ����
	* @return �ɹ�����S_OK������ֵ����ʧ��
	*/
	HRESULT OnUserChange(WPARAM wParam,LPARAM lParam);

	/**
	* @brief MSG_USER_RESTORE_DEFAULT �û���Ϣ�ָ�Ĭ��
	* @param [in] wParam ����
	* @param [out] lParam ����
	* @return �ɹ�����S_OK������ֵ����ʧ��
	*/
	HRESULT OnUserRetoreDefault(WPARAM wParam,LPARAM lParam);

	/**
     *@brief �û�����
     *@param [PVOID] pvBuffer �û����Ʋ���:1����ȡ�û��б�2�������û���3���޸��û����룻4��ɾ���û�
     *@param [PVOID] iSize ������С
     *@return �ɹ�����S_OK,����ֵΪ�������
     */
	HRESULT OnUserManage_ONVIF(PVOID pvBuffer, INT iSize);

	/**
	 *@brief ��ȡ�û���������
	 *@param [in] wParam �û���
	 *@param [out] lParam �û�����
     *@return �ɹ�����S_OK,����ֵΪ�������
	 */
	HRESULT OnUserGetPassword(WPARAM wParam, LPARAM lParam);


	//��Ϣӳ��궨��
	SW_BEGIN_MESSAGE_MAP(CSWUserManager, CSWMessage)
	   SW_MESSAGE_HANDLER(MSG_USER_LOGIN, OnUserLogin)
	   SW_MESSAGE_HANDLER(MSG_USER_GETLIST, OnUserGetList)
	   SW_MESSAGE_HANDLER(MSG_USER_ADD, OnUserAdd)
	   SW_MESSAGE_HANDLER(MSG_USER_DEL, OnUserDell)
	   SW_MESSAGE_HANDLER(MSG_USER_CHANGE, OnUserChange)
	   SW_MESSAGE_HANDLER(MSG_USER_RESTORE_DEFAULT, OnUserRetoreDefault)
		SW_MESSAGE_HANDLER(MSG_USER_GETPASSWORD, OnUserGetPassword)
	SW_END_MESSAGE_MAP()


	

	//Զ����Ϣӳ�亯��
    SW_BEGIN_REMOTE_MESSAGE_MAP(CSWUserManager, CSWMessage)
		SW_REMOTE_MESSAGE_HANDLER(MSG_USER_MANAGE, OnUserManage_ONVIF)
    SW_END_REMOTE_MESSAGE_MAP();

private:
   	//�û���Ϣ���ݶ�ͷ��
#define USER_DATA_SYNC 0x47
#define USER_DATA_VALID	0xD1

   	typedef struct _tUserInfoHead
   	{
		BYTE bSync;		//ͬ����־
	   	BYTE bValid;		//������Ч��־
	   	WORD	wDataLen;	//��Ч���ݳ���
	   	_tUserInfoHead()
	   	{
		   	bSync = USER_DATA_SYNC;
		   	bValid = USER_DATA_VALID;
		   	wDataLen = 0;
		};
   	}tUserInfoHead;

   	Acount_t m_tUserInfo[ACOUNT_NUM];		//�û���Ϣ
   	Acount_t m_tSuperUser;				//�����û����û���������̶��������޸ģ������û��б���

   	BOOL m_fLoadInfo;

   	HRESULT LoadUserInfo(VOID);			//Read from EEPROM
   	HRESULT CommitUserInfo(VOID);		//Write to EEPROM

   	VOID RestoreDefault(VOID);

	BOOL UserInfoCheck(Acount_t *pCount);
   	BOOL AdminCheck(Acount_t *pCount);
	BOOL SuperUserCheck(Acount_t *pCount);
   
};
REGISTER_CLASS(CSWUserManager)
#endif /* __SW_USERMANAGER_H__ */
