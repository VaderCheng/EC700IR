#ifndef __SW_CLASS_FACTORY_H__
#define __SW_CLASS_FACTORY_H__
#include "SWLog.h"
/**
 *@brief ����һ������������Ļ���
 */
class CSWCreateClass : public CSWObject
{
	CLASSINFO(CSWCreateClass, CSWObject)
public: 
	CSWCreateClass();	
	virtual ~CSWCreateClass();
	/**
	 *@brief ȡ�ö�������
	 */
	virtual LPCSTR GetObjectName(void);
	/**
	 *@brief ��������
	 */
	virtual CSWObject* CreateObject(const char *szclsName);
};

/**
 *@brief �๤��
 */
class CSWClassFactory : public CSWCreateClass
{
	CLASSINFO(CSWClassFactory, CSWCreateClass)
public:	
	CSWClassFactory();
	virtual ~CSWClassFactory();
	/**
	 *@brief ���������ƴ�������
	 */
	virtual CSWObject *CreateObject(LPCSTR lpzclsName);
	/**
	 *@brief ������ӵ��๤����
	 */
	HRESULT Add(CSWCreateClass *pClass);
private:	
	CSWList<CSWCreateClass *>m_lstClass;
};
extern CSWClassFactory theClassFactory;

/**
 *@brief ע���ൽ�๤����
 */
#define REGISTER_CLASS(cls)\
	class cls##_Factory_Registration : public CSWCreateClass\
	{\
		CLASSINFO(cls##_Factory_Registration, CSWCreateClass)\
	public:\
		cls##_Factory_Registration()\
		{\
			theClassFactory.Add(this);\
		}\
		virtual ~cls##_Factory_Registration()\
		{\
		}\
		virtual LPCSTR GetObjectName(void)\
		{\
			return #cls;\
		}\
		virtual CSWObject* CreateObject(const char *szclsName)\
		{\
			return new cls;\
		}\
		VOID Print(VOID)\
		{\
			SW_TRACE_DEBUG("load class %s...\n", #cls);\
		}\
	}cls##_Factory_Registration_Instance;\
	void cls##_Factory_Registration_Initialize()\
	{\
		extern cls##_Factory_Registration cls##_Factory_Registration_Instance;\
		cls##_Factory_Registration* pTmp = &cls##_Factory_Registration_Instance;\
		theClassFactory.Add(pTmp);\
		pTmp->Print();\
	}

/**
 *@brief �����ൽ�๤��
 */
#define LOAD_CLASS(cls) extern void cls##_Factory_Registration_Initialize(); cls##_Factory_Registration_Initialize();

/**
 *@brief ��������
 */	
#define CREATE_OBJECT(clsName) theClassFactory.CreateObject(clsName);

/**
 *@brief ����Filter����
 */
#define CREATE_FILTER(clsName) (CSWBaseFilter *)CREATE_OBJECT(clsName) 

/**
 *��ʼ���๤��
 */
void ClassInitialize();
#endif
