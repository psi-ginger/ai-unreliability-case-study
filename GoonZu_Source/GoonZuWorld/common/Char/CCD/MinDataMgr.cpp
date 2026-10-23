#include <Directives.h>
#include "MinDataMgr.h"
#include "MinDataObj.h"

#include "CommonLogic.h"


CMinDataMgr g_MinDataMgr;

CMinDataMgr::CMinDataMgr()
{
	m_pFirst = NULL;
	m_pLast = NULL;
	m_bIsFindBefore = false;

	Initialize();
}

CMinDataMgr::~CMinDataMgr()
{
	Destroy();
}

void CMinDataMgr::Initialize()
{
	m_pFirst = NULL;
	m_pLast = NULL;

	FindMinFiles();
}

void CMinDataMgr::Destroy()
{
	CMinDataObj *pTemp = NULL;

	while ( m_pFirst )
	{
		pTemp = m_pFirst;
		m_pFirst = m_pFirst->GetNext();
		
		delete pTemp;
		pTemp = NULL;
	}

	m_pFirst = NULL;
	m_pLast = NULL;
}

void CMinDataMgr::FindMinFiles()
{
	if ( m_bIsFindBefore )
	{
		Destroy();
	}
	
	TCHAR FolderPath[ _MAX_PATH ] = TEXT("");
	DWORD dwBufferLen			  = _MAX_PATH - 1;
	TCHAR Buffer[_MAX_PATH]		  = TEXT("");
	memset( Buffer, 0, _MAX_PATH );

	m_siMinFileNum = 0;	

	::GetCurrentDirectory( dwBufferLen, Buffer );

	int BufLen = _tcslen( Buffer );

	if( Buffer [ BufLen - 1 ] == '\\' )
	{
		StringCchCat( Buffer, _MAX_PATH, TEXT("CCD\\MinData\\*.Min") );
	}
	else
	{
		StringCchCat( Buffer, _MAX_PATH, TEXT("\\CCD\\MinData\\*.Min") );
	}
	
	WIN32_FIND_DATA FileFind;
	HANDLE hSearch = ::FindFirstFile( Buffer, &FileFind );

	if ( hSearch == INVALID_HANDLE_VALUE )
	{
//		if (pclClient->GameMode != GAMEMODE_SERVER)
//			MessageBox( NULL, TEXT("hSearch == INVALID_HANDLE_VALUE"), Buffer, MB_OK );
		return;
	}

	while(  hSearch != INVALID_HANDLE_VALUE )
	{
		memset( FolderPath, 0, _MAX_PATH );

		StringCchCopy( FolderPath, _MAX_PATH, TEXT("CCD\\MinData\\") );
		StringCchCat( FolderPath, _MAX_PATH, FileFind.cFileName );

		CMinDataObj *pTemp = new CMinDataObj();
		pTemp->LoadMinData( FolderPath );
		PushBack( pTemp );
		++m_siMinFileNum;
		
		if ( ::FindNextFile( hSearch, &FileFind ) == 0 )
		{
			m_bIsFindBefore = true;
			::FindClose( hSearch );
			break;
		}
	}

	if ( ! m_bIsFindBefore )
	{
		TCHAR TempBuf[ 128 ] = TEXT("");
		StringCchPrintf( TempBuf, 128, TEXT("MinFileNum = %d"), m_siMinFileNum );
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox( NULL, TEXT("Min File Not Exist"), TempBuf, MB_OK );
		return;
	}

	return;
}

CMinDataObj *CMinDataMgr::GetFirst()
{
	return m_pFirst;
}

void CMinDataMgr::SetFirst( CMinDataObj *pFirst )
{
	m_pFirst = pFirst;
	return;
}

CMinDataObj *CMinDataMgr::GetLast()
{
	return m_pLast;
}

void CMinDataMgr::SetLast( CMinDataObj *pLast )
{
	m_pLast = pLast;
	return;
}

void CMinDataMgr::PushBack( CMinDataObj *pNode )
{
	if ( m_pFirst )
	{
		m_pLast->SetNext( pNode );
		pNode->SetPre( m_pLast );
		m_pLast = pNode;
		m_pLast->SetNext( NULL );
	}
	else
	{
		m_pFirst = pNode;
		m_pLast = pNode;
	}

	return;
}

CMinDataObj *CMinDataMgr::GetSameMinDataObjPtr( TCHAR *strFileName )
{
	TCHAR UprFileName[ _MAX_PATH ] = TEXT("");
	TCHAR *TempUprFileName = tcsupr( strFileName );
	StringCchCopy( UprFileName, _MAX_PATH, TempUprFileName );

	CMinDataObj *pTemp = m_pFirst;

	while( pTemp )
	{
		if ( pTemp->IsSameMinFile( UprFileName ) )
		{
			return pTemp;
		}

		pTemp = pTemp->GetNext();
	}

	return NULL;
}

TCHAR *CMinDataMgr::GetFileName( int siIndex )
{
	if ( m_pFirst == NULL )
	{
		return NULL;
	}

	CMinDataObj *pTemp = m_pFirst;
	
	
	for ( int i = 0; i < siIndex; i ++ )
	{
		pTemp = pTemp->GetNext();

		if ( pTemp == NULL )
		{
			return NULL;
		}
	}
	
	return pTemp->GetFileName();
}

char CMinDataMgr::GetDelayTimeIndex( int siActionIndex, int CharacterKind )
{
	if ( m_pFirst == NULL )
	{
		return NULL;
	}

	CMinDataObj *pTemp = m_pFirst;
	
	
	for ( int i = 0; i < m_siMinFileNum; i ++ )
	{
		if ( pTemp->GetActionIndex() == siActionIndex && pTemp->GetCharacterKind() == CharacterKind )
		{
			return pTemp->GetDelayTimeIndex();
		}

		pTemp = pTemp->GetNext();

		if ( pTemp == NULL )
		{
			return -1;
		}
	}

	return -1;
	
}