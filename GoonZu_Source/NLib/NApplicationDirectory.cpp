
#include <assert.h>
#include ".\napplicationdirectory.h"




NApplicationDirectory* NApplicationDirectory::ms_kThis = NULL;

//////////////////////////////////////////////////////////////////////////


NApplicationDirectory::NApplicationDirectory(void)
{
	ZeroMemory( m_pTree, sizeof(m_pTree) );
	m_iTreeLevel = 0;
}

NApplicationDirectory::~NApplicationDirectory(void)
{
}

bool NApplicationDirectory::CreateInstance()
{
	if( ms_kThis )
		return true; // 이미 생성됨

	ms_kThis = new NApplicationDirectory;

	if( ms_kThis )
	{
		ms_kThis->Initialize();
		return true;
	}

	return false;
}
void NApplicationDirectory::DestroyInstance()
{
	if( ms_kThis )
	{
		delete ms_kThis;
		ms_kThis = NULL;
	}
}

void NApplicationDirectory::Initialize()
{
	HMODULE hModule = NULL;
	TCHAR szFileName[512] = {""};
	TCHAR* pEnd = NULL;

	GetModuleFileName( hModule, szFileName, 512 );

	m_kApplicationDirectory.Format("%s", szFileName );

	m_iTreeLevel = 0;
	for( int i = 0; szFileName[i] != '\0'; i++ ) 
	{
		if( *( szFileName + i ) == '\\' ) 
		{
			assert( m_iTreeLevel < CON_TREELEVEL_MAX );

			m_pTree[m_iTreeLevel++] = (m_kApplicationDirectory.GetString() + i);
			pEnd = m_kApplicationDirectory.GetString() + i;
		}
	}

	*(pEnd) = _T('\0');
	m_iTreeLevel--;

	SetCurrentDirectory( m_kApplicationDirectory.GetString() );
}

LPCTSTR NApplicationDirectory::GetApplicationDirectory()
{
	assert( ms_kThis ); // 인스턴스 생성해야함!!!!

	return ms_kThis->m_kApplicationDirectory.GetString();
}

LPCTSTR NApplicationDirectory::MakeRelativePath( LPCTSTR pszPathName )
{
	assert( ms_kThis ); // 인스턴스 생성해야함!!!!
	
	if( !pszPathName ) return NULL;

	int iTreelevel = ms_kThis->m_iTreeLevel;
	TCHAR* pCheck = (TCHAR*)pszPathName;
	TCHAR* pChildPath = (TCHAR*)pszPathName;

	if( *pCheck == '.' )
	{
		int ret = _tcsncmp(pCheck, "..\\", 3);
		if( ret == 0 )
		{
			while( ret == 0 )
			{
				pChildPath = pCheck + 3;
				iTreelevel--;
				pCheck += 3;
				ret = _tcsncmp(pCheck, "..\\", 3);

				if( iTreelevel == 0 ) ret = -1;
			}
		}
		else if( _tcsncmp( pCheck, ".\\", 3) )
		{
			pChildPath = pCheck + 2;
		}
	}
	else if( *pCheck == '\\' )
	{
		pChildPath = pCheck + 1;
	}

	TCHAR szDir[512] = "";

	if( iTreelevel != ms_kThis->m_iTreeLevel )
	{
		*(ms_kThis->m_pTree[iTreelevel]) = _T('\0');
		_tcscpy( szDir, ms_kThis->m_kApplicationDirectory );
		*(ms_kThis->m_pTree[iTreelevel]) = _T('\\');
	}
	else
	{
		_tcscpy( szDir, ms_kThis->m_kApplicationDirectory );
	}

	ms_kThis->m_kMakePath.Format( "%s\\%s", szDir, pChildPath );

	return ms_kThis->m_kMakePath.GetString();
}