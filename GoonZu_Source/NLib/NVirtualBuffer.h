//-----------------------------------------------------------------------------
// 2005/12/21 이제완
//-----------------------------------------------------------------------------

#ifndef _NVIRTUALBUFFER_H
#define _NVIRTUALBUFFER_H

#include "NDataTypes.h"

class NVirtualBuffer
{
public:
	NVirtualBuffer();
	~NVirtualBuffer();

	void			CreateVirtualBuffer( DWORD dwMaxSize );
	BYTE*			Alloc( DWORD dwSize );

	BYTE*			GetBasePtr() { return m_pBasePtr; }

	DWORD			m_dwPageSize;					// 페이지 크기 4096

	BYTE*			m_pBasePtr;						// 베이스 포인터
	BYTE*			m_pCurPtr;						// 다음 Commit 위치

	DWORD			m_dwReservedSize;				// 총 예약된 버퍼 크기
	DWORD			m_dwCommittedSize;				// Committed 된 버퍼 크기

	DWORD			m_dwAllocSize;					// 실제 사용하기 위해 할당된 버퍼 크기
	
};


#endif