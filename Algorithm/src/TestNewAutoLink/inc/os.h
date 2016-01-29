#ifndef _OS_H_
#define _OS_H_

#include "HvUtils.h"

namespace QTCPIP {
	//��ָ�붨��
#ifndef NULL
	const int NULL = 0;
#endif
	//
	//�����������Ͷ���
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef unsigned int DWORD;
	//
	//��󻺴��
#ifdef CHIP_6455
	const int MAX_PACKET_COUNT = 2000;
#else
	const int MAX_PACKET_COUNT = 500;
#endif
	
	//Ԥ����ϵͳ�Ļ���������ݷ��Ͳ���ռ��������棬�����ݷ�����ռ����󻺴���MAX_PACKET_COUNT-30
	const int RESERVE_PACKET_COUNT = 30;
	//
	//WORD�����ֽ���
	inline WORD* ReverseWORD( WORD *dest, const WORD *source ) {
		*dest = ( *source >> 8 ) | ( *source << 8 );
		return dest;
	}
	//
	//DWORD�����ֽ���
	inline WORD* ReverseDWORD( WORD *dest, const WORD *source ) {
		dest[ 0 ] = ( source[ 1 ] >> 8 ) | ( source[ 1 ] << 8 );
		dest[ 1 ] = ( source[ 0 ] >> 8 ) | ( source[ 0 ] << 8 );
		return dest;
	}
	//
	//����DWORD
	inline WORD* CopyDWORD( WORD *dest, const WORD *source ) {
		_mem4( dest ) = _mem4_const( source );
		return dest;
	}
	//
	//������̫����ַ
	inline WORD* CopyEtherAddr( WORD *dest, const WORD *source ) {
		_mem4( dest ) = _mem4_const( source );
		_amem2( dest + 2 ) = _amem2_const( source + 2 );
		return dest;
	}
	//
	//�Ƚ�DWORD
	inline bool EqualDWORD( const WORD *dest, const WORD *source ) {
		return _mem4_const( dest ) == _mem4_const( source );
	}

	inline BYTE *OS_memcpy( BYTE *restrict dest, const BYTE *restrict source, int count )
	{
		return ( BYTE * )std::memcpy( dest, source, count );		
	//���µ�д����Debug�²�����������.
	/*	
		BYTE *pbTemp = dest;
		int iCount2 = count >> 3;
		for ( int i = 0; i < iCount2; i++ )
		{
			_memd8( dest ) = _memd8_const( source );
			dest += 8;
			source += 8;
		}
		if ( count & 4 )
		{
			_mem4( dest ) = _mem4_const( source );
			dest += 4;
			source += 4;
		}
		if ( count & 2 )
		{
			_mem2( dest ) = _mem2_const( source );
			dest += 2;
			source += 2;
		}
		if ( count & 1 )
		{
			*dest = *source;
		}
		return pbTemp;
	*/
	}
	
	//�����ź���
	inline void PostSem( DWORD SemHandle ) {
		SEM_post( reinterpret_cast< SEM_Handle >( SemHandle ) );
	}
	//
	//�ȴ��ź���
	inline void PendSem( DWORD SemHandle ) {
		SEM_pend( reinterpret_cast< SEM_Handle >( SemHandle ), SYS_FOREVER );
	}
	//
	//ȡ��ϵͳʱ��(����)
	inline DWORD GetMS() {
		return static_cast< DWORD >( PRD_getticks() );
	}
	//
	//ȡ��ʱ����(����)
	inline DWORD GetInterval( DWORD begin_time ) {
		return GetMS() - begin_time;
	}
	//
	//
	//��������
	inline void Sleep( DWORD ms ) {
		TSK_sleep( ms );
	}
	//
	//ȡ��һ�������
	inline DWORD OS_Random() {
		return GetMS();
	}
	//
}

#endif
