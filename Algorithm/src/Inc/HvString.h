/*
 *	(C)��Ȩ���� 2010 ������·���Ƽ���չ���޹�˾
 */

/**
* @file		HvString.h
* @version	1.0
* @brief	CHvString��Ķ���
* @author	Shaorg
* @date		2010-7-26
*/

#ifndef _HVSTRING_H_
#define _HVSTRING_H_

#include <string>
using namespace std;

namespace HiVideo
{
	/*! @class	CHvString
	* @brief	����STL�е�string��ʵ�֣�������MFC�е�CString�ࡣ
	*/
	class CHvString
	{
	public:
		/**
		* @brief	Ĭ�Ϲ��캯��
		*/
		CHvString();

		/**
		* @brief	���캯��
		* @param	szString	CHvString��ʼ�����ֵ
		*/
		CHvString(const char* szString);

		/**
		* @brief	��������
		*/
		virtual ~CHvString();

	public:
		/**
		* @brief	��ȡ�ַ���������ָ��
		* @return	ָ���ַ��������ĳ�ָ��
		* @warning	�÷��ص�ָ��ֻ��������ȡ��ָ������ݣ����������ı���ָ������ݡ�
		*/
		const char* GetBuffer();

		/**
		* @brief	�ж��ַ����Ƿ�Ϊ��
		* @return	Ϊ�գ�true����Ϊ�գ�false
		*/
		bool IsEmpty();

		/**
		* @brief	����CHvString�ַ�����ĩβ
		* @param	str	�����ӵ��ַ���
		* @return	void
		*/
		void Append(const CHvString& str);

		/**
		* @brief	���ɸ�ʽ���ַ���
		* @param	szFormat	�������ɵ��ַ���ԭ�ͣ����пɰ�����ʽ����
		* @param	...			����������滻��ʽ�������ݣ��������szFormat�еĸ�ʽ������ȷ����
		* @return	�ɹ����������ɵ��ַ����ĳ��ȣ�ʧ�ܣ�-1
		*/
		int Format(const char* szFormat, ...);

		/**
		* @brief	�����ַ�����ָ��λ��
		* @param	iIndex	�ַ��������λ�ã�ע��Ϊ0��ʾ���뵽��ǰ�ˣ�
		* @param	psz		������ַ���
		* @return	�ɹ������ַ����ĳ��ȣ�ʧ�ܣ�ԭ�ַ����ĳ���
		*/
		int Insert(int iIndex, const char* psz);

		/**
		* @brief	��ȡ�ַ����ĳ���
		* @return	�ַ������ȡ�ע���ó��Ȳ�������β��\0�ַ�
		*/
		int GetLength();

		/**
		* @brief	�Ƴ�����ָ�����ַ�
		* @param	ch	Ҫ�Ƴ����ַ�
		* @return	�Ƴ����ַ�����
		*/
		int Remove(char ch);

		/**
		* @brief	��ָ��λ�ÿ�ʼ����Ѱָ���ַ���
		* @param	pszSub	Ҫ��Ѱ���ַ���
		* @param	iStart	��Ѱ����ʼλ��
		* @return	�ҵ����ַ�����λ��������ע��������λ�ô�0��ʼ��δ�ҵ�ʱ����-1
		*/
		int Find(const char* pszSub, int iStart = 0);

		/**
		* @brief	��ȡ������ĸСд������ַ���
		* @return	��ĸСд������ַ���
		*/
		CHvString MakeLower();

		/**
		* @brief	��ȡ������ĸ��д������ַ���
		* @return	��ĸ��д������ַ���
		*/
		CHvString MakeUpper();

		/**
		* @brief	��ȡ���ָ�����ȵ��Ӵ�
		* @nCount	��ȡ���ַ����ĳ���
		* @return	��ȡ�����ַ���
		*/
		CHvString Left(int nCount);

		/**
		* @brief	��ȡ�Ӵ�
		* @param	iFirst	���Ӵ�����ʼλ�ã�ע��0Ϊ�ʼ��λ�ã�
		* @param	nCount	���Ӵ��ĳ���
		* @return	��ȡ�����ַ���
		*/
		CHvString Mid(int iFirst, int nCount = 0);

	public:
		/**
		* @brief	����ת������
		* @return	ָ���ַ��������ĳ�ָ��
		* @see		GetBuffer()
		*/
		operator const char*();

	public:
		/**
		* @brief	��CHvStringֵ����
		*/
		CHvString& operator =(const CHvString& str);

		/**
		* @brief	���ַ���ֵ����
		*/
		CHvString& operator =(const char* pchar);

		/**
		* @brief	����CHvString�ַ�������
		* @see		Append()
		*/
		CHvString& operator +=(const CHvString& str);

		/**
		* @brief	�����ַ�������
		*/
		CHvString& operator +=(const char* pchar);

		/**
		* @brief	�����ַ�����
		*/
		CHvString& operator +=(const char ch);

		/**
		* @brief	����CHvString�ַ�������
		*/
		CHvString operator +(const CHvString& str);

		/**
		* @brief	�����ַ�������
		*/
		CHvString operator +(const char* pchar);

		/**
		* @brief	ȡָ��λ���ַ�����
		* @param	iIndex	λ��
		* @return	ȡ�����ַ�
		*/
		char operator [](int iIndex);

		bool operator !=(const char* pchar);
		bool operator !=(const CHvString& str);
		bool operator ==(char* pchar);
		bool operator ==(const CHvString& str);
		bool operator >=(const CHvString& str);
		bool operator <=(const CHvString& str);

	private:
		string m_str;

		//ͨ������Format�������ɵ��ַ������������󳤶ȡ�
		//ע���������ó���������������ȫ����Ӧ�ò㱣֤��
		static const int FORMAT_BUFMAXLEN = 256;
	};
}

#endif
