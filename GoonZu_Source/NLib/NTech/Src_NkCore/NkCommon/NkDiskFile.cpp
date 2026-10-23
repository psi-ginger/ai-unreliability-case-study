#include "NkDisk.h"
#include "NkDiskFile.h"


NkDiskFile::NkDiskFile( NkDisk& kDisk )
{
	m_pkDisk = &kDisk;

	m_pkFileNode = NULL;
	m_dwCurrentPos = 0;	
}

BOOL NkDiskFile::OpenFile( const char* szFileName )
{
	if( !m_pkDisk->IsReadOnly() ) return FALSE;

	NkDiskNode_File* pkFileNode = m_pkDisk->FindFile( szFileName );

	if( !pkFileNode ) return FALSE;

	m_pkFileNode = pkFileNode;
	m_dwCurrentPos = 0;

	return TRUE;
}

void NkDiskFile::CloseFile()
{
	m_pkFileNode = NULL;
	m_dwCurrentPos = 0;	
}


BOOL NkDiskFile::ReadFile( LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead )
{
	if( !m_pkFileNode )
	{
		*lpNumberOfBytesRead = 0;
		return FALSE;
	}
	
	if( m_dwCurrentPos >= m_pkFileNode->m_dwFileSize )
	{
		*lpNumberOfBytesRead = 0;
		return FALSE;
	}

	if( m_dwCurrentPos + dwNumberOfBytesToRead > m_pkFileNode->m_dwFileSize )
	{
		dwNumberOfBytesToRead = m_pkFileNode->m_dwFileSize - m_dwCurrentPos;
	}

	DWORD dwRead;
	m_pkDisk->ReadFileData( m_pkFileNode, m_dwCurrentPos, lpBuffer, dwNumberOfBytesToRead, &dwRead );

	m_dwCurrentPos += dwRead;
	*lpNumberOfBytesRead = dwRead;

	return TRUE;
}

DWORD NkDiskFile::SetFilePointer( LONG lDistanceToMove, DWORD dwMoveMethod )
{
	if( !m_pkFileNode ) return 0;

	switch( dwMoveMethod )
	{
	case FILE_BEGIN:
		{
			m_dwCurrentPos = lDistanceToMove;			
		}
		break;

	case FILE_CURRENT:
		{
			m_dwCurrentPos += lDistanceToMove;
		}
		break;
	}
	return m_dwCurrentPos;
}

DWORD NkDiskFile::GetFileSize()
{
	if( !m_pkFileNode ) return 0;
	return m_pkFileNode->m_dwFileSize;
}
