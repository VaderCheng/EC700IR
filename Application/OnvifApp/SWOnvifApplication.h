/**
* @file
* @brief 
*
*/

#ifndef _SW_ONVIF_APPLICATION_H_
#define _SW_ONVIF_APPLICATION_H_

#include "SWApplication.h"
#include "SWThread.h"
#include "SWMessage.h"
#include "onvifLib.h"


//ONVIF �������
typedef struct _ONVIF_PARAM_INFO
{
     int iOnvifEnable;
     int iAuthenticateEnable;                                   //��Ȩ 0 ��ʹ��  1 ʹ��
     _ONVIF_PARAM_INFO()
     {
          iOnvifEnable = 0;
          iAuthenticateEnable = 0;
     }
}ONVIF_PARAM_INFO;


class CSWOnvifApp : public CSWApplication,CSWMessage
{
	CLASSINFO(CSWOnvifApp, CSWApplication);
public:
	
	/**
	* @brief 
	*/
	CSWOnvifApp();

	/**
	* @brief 
	*/
	virtual ~CSWOnvifApp();

	virtual HRESULT InitInstance(const WORD wArgc, const CHAR** szArgv);
	virtual HRESULT ReleaseInstance();

	virtual HRESULT Run();
	
	static HRESULT Breath(VOID)
	{
		return HeartBeat();
	}

protected:

	/**
     *@brief ��ȡ�����̨����
     *@return �ɹ�����S_OK,����ֵΪ�������
     */
	HRESULT GetPTZInfo(VOID);

	/**
     *@brief ���������̨����
     *@param [PVOID] pvBuffer PTZ���Ʋ���
     *@param [PVOID] iSize ������С
     *@return �ɹ�����S_OK,����ֵΪ�������
     */
	HRESULT OnSetPTZInfo(PVOID pvBuffer, INT iSize);

	/**
     *@brief �����û�������
     *@param [PVOID] pvBuffer �û�������
     *@param [PVOID] iSize ������С
     *@return �ɹ�����S_OK,����ֵΪ�������
     */
	HRESULT OnSetUSERInfo(PVOID pvBuffer, INT iSize);

	/**
     *@brief ��ȡ�û�������
     *@param [PVOID] pvBuffer �û�������
     *@param [PVOID] iSize �û�����
     *@return �ɹ�����S_OK,����ֵΪ�������
     */
	static int OnGetUSERInfo(void* pSize, void* pvArg);

	HRESULT OnSetEnableInfo(PVOID pvBuffer, INT iSize);

    HRESULT OnOnvifRestart(PVOID pvBuffer, INT iSize);


	
	//Զ����Ϣӳ�亯��
    SW_BEGIN_REMOTE_MESSAGE_MAP(CSWOnvifApp, CSWMessage)
		SW_REMOTE_MESSAGE_HANDLER(MSG_APP_REMOTE_SET_PTZ_INFO, OnSetPTZInfo)
		SW_REMOTE_MESSAGE_HANDLER(MSG_APP_REMOTE_SET_USER_INFO, OnSetUSERInfo)
		SW_REMOTE_MESSAGE_HANDLER(MSG_ONVIF_SET_PARAM, OnSetEnableInfo)
		SW_REMOTE_MESSAGE_HANDLER(MSG_ONVIF_APP_RESTART, OnOnvifRestart)
    SW_END_REMOTE_MESSAGE_MAP();

	
private:	
	INT GetVersion(VOID);

	static INT PTZControlCallback(const int iCmdID, void* pvArg);
	static INT UserManageCallback(const int iCmdID, void* pvArg);

	ONVIF_PTZ_PARAM m_sPTZParam;
	
	static UserArray_t g_psUserlist[8];
	static int	m_UserSize;
	int m_fEnable;
	int m_fAuthentEnable;

	bool m_fIsGetParam;

	CSWMutex m_cMutex;


};





#endif //_SW_ONVIF_APPLICATION_H_

