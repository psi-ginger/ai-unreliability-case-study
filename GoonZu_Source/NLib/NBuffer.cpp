//-----------------------------------------------------------------------------
// 2005/11/19 ¿Ã¡¶øœ
//-----------------------------------------------------------------------------


#include "NBuffer.h"

NBuffer::NBuffer()
{
	m_pBuffer = NULL;
	m_dwBufferSize = 0;
}

NBuffer::~NBuffer()
{
	NDelete_Array( m_pBuffer );
}

void NBuffer::CreateBuffer( int size )
{

	if( m_pBuffer ) delete [] m_pBuffer;

	m_pBuffer = new BYTE[ size ];

	m_dwBufferSize = size;
}

void NBuffer::ClearBuffer()
{
	if( m_pBuffer ) {
	
		ZeroMemory( m_pBuffer, m_dwBufferSize );
	}
}