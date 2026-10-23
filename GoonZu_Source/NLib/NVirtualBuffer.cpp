//-----------------------------------------------------------------------------
// 2005/12/21 이제완
//-----------------------------------------------------------------------------

#include "NVirtualBuffer.h"
#include <math.h>

NVirtualBuffer::NVirtualBuffer()
{
	m_pBasePtr = NULL;
	m_pCurPtr = NULL;

	m_dwPageSize = 0;

	m_dwReservedSize = 0;
	m_dwCommittedSize = 0;

	m_dwAllocSize = 0;
}

NVirtualBuffer::~NVirtualBuffer()
{
	if( m_pBasePtr )
	{
		VirtualFree( m_pBasePtr, 0, MEM_RELEASE );
		m_pBasePtr = NULL;
	}

	m_dwPageSize = 0;
	
	m_dwReservedSize = 0;
	m_dwCommittedSize = 0;

	m_dwAllocSize = 0;
}


void NVirtualBuffer::CreateVirtualBuffer( DWORD dwMaxSize )
{

	SYSTEM_INFO si;
	GetSystemInfo( &si );

	m_dwPageSize = si.dwPageSize;
	
	m_dwReservedSize = (DWORD)ceil( (double)dwMaxSize / m_dwPageSize ) * m_dwPageSize;

	m_pBasePtr = (BYTE*)VirtualAlloc( NULL, m_dwReservedSize, MEM_RESERVE, PAGE_READWRITE );

	m_pCurPtr = m_pBasePtr;

	m_dwCommittedSize = 0;
	m_dwAllocSize = 0;
}

BYTE* NVirtualBuffer::Alloc( DWORD dwSize )
{
	BYTE *pRet = NULL;

	// 총 예약된 버퍼 크기 보다 크다면 할당 실패
	if( m_dwReservedSize < m_dwAllocSize + dwSize )
	{
		return NULL;
	}

	// 추가로 commit 하기 위한 크기 계산 
	int iNeedSize = m_dwAllocSize + dwSize - m_dwCommittedSize;

	// 추가로 commit 해야 한다면,
	if( iNeedSize > 0 )
	{
		DWORD dwCommitSize = (UINT)ceil( (double)iNeedSize / m_dwPageSize ) * m_dwPageSize;

		VirtualAlloc( m_pCurPtr, dwCommitSize, MEM_COMMIT, PAGE_READWRITE );

		m_pCurPtr += dwCommitSize;

		m_dwCommittedSize += dwCommitSize;
	}

	pRet = m_pBasePtr + m_dwAllocSize;

	m_dwAllocSize += dwSize;

	return pRet;

}
