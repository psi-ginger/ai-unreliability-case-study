#include "stdafx.h"
#include "MinDataMgr.h"
#include "MinDataObj.h"

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
	
	char FolderPath[ MAX_PATH ] = "";
	DWORD dwBufferLen = MAX_PATH - 1;
	char Buffer[MAX_PATH];

	m_siMinFileNum = 0;	

	::GetCurrentDirectory( dwBufferLen, Buffer );
	strcat( Buffer, "\\MinData\\*.Min" );

	WIN32_FIND_DATA FileFind;
	HANDLE hSearch = ::FindFirstFile( Buffer, &FileFind );

	while(  hSearch != INVALID_HANDLE_VALUE )
	{
		memset( FolderPath, 0, MAX_PATH );

		strcpy( FolderPath, "./MinData/" );
		strcat( FolderPath, FileFind.cFileName );

		CMinDataObj *pTemp = new CMinDataObj();
		pTemp->LoadMinData( FolderPath );
		PushBack( pTemp );
		++m_siMinFileNum;
		
		if ( ::FindNextFile( hSearch, &FileFind ) == 0 )
		{
			break;
		}
	}

	::FindClose( hSearch );

	m_bIsFindBefore = true;

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

CMinDataObj *CMinDataMgr::GetSameMinDataObjPtr( char *strFileName )
{
	CMinDataObj *pTemp = m_pFirst;

	while( pTemp )
	{
		if ( pTemp->IsSameMinFile( strFileName ) )
		{
			return pTemp;
		}

		pTemp = pTemp->GetNext();
	}

	return NULL;
}

char *CMinDataMgr::GetFileName( int siIndex )
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
