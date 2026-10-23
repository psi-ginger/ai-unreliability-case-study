//---------------------------------
// 2006/2/20 √÷ø¨¡ÿ
//---------------------------------

#include "PersonNameManager.h"
#include <CommonLogic.h>

extern cltCommonLogic* pclClient;

unsigned int HashFunc(int key)
{
	return key;
}


cltPersonNameManager::cltPersonNameManager(SI32 arraySize, SI32 listPoolSize)
{
	m_pNameHashTable = new NHashTable<int, TCHAR*>;
	m_pNameHashTable->CreateHashTable(arraySize, listPoolSize, HashFunc );

	m_siListPoolSize = listPoolSize;
	m_siCurPos = 0;

	m_szPersonNames = new TCHAR[ m_siListPoolSize * MAX_PLAYER_NAME ];

	ZeroMemory(m_szPersonNames, sizeof(TCHAR) * m_siListPoolSize * MAX_PLAYER_NAME );

}

cltPersonNameManager::~cltPersonNameManager()
{
	NDelete_Array( m_szPersonNames );
	NDelete( m_pNameHashTable );
}

bool cltPersonNameManager::Insert( int id, TCHAR* pensonName )
{
	if (m_siCurPos + MAX_PLAYER_NAME < m_siListPoolSize * MAX_PLAYER_NAME && pensonName != NULL)
	{
		StringCchCopy(&m_szPersonNames[m_siCurPos], MAX_PLAYER_NAME, pensonName);

		m_pNameHashTable->Insert(id, &m_szPersonNames[m_siCurPos]);

		m_siCurPos = m_siCurPos + sizeof(TCHAR) * MAX_PLAYER_NAME;

		return true;
	}

	return false;

}

bool cltPersonNameManager::Find( int id, TCHAR* pensonName )
{
	TCHAR *pText;

	if (m_pNameHashTable->Find( id, &pText ) == true)
	{
        StringCchCopy(pensonName, 256, pText);

		return true;
	}
	return false;
}

bool cltPersonNameManager::Remove( int id )
{
	return m_pNameHashTable->Remove( id );
}



//////////////////////////////////////////////////////////////////////////


cltNewPersonNameManager::cltNewPersonNameManager(SI32 arraySize, SI32 listPoolSize)
{
	m_pNameHashTable = new NHashTable<int, cltPersonNameInfo*>;
	m_pNameHashTable->CreateHashTable(arraySize, listPoolSize, HashFunc );

	m_siListPoolSize = listPoolSize;
	m_siCurIndex = 0;

	m_pclPersonNameInfo = new cltPersonNameInfo[ m_siListPoolSize ];
}

cltNewPersonNameManager::~cltNewPersonNameManager()
{
	NDelete_Array( m_pclPersonNameInfo );
	NDelete( m_pNameHashTable );
}

bool cltNewPersonNameManager::Insert( int id, cltPersonNameInfo* PersonInfo )
{
	if(0 <= m_siCurIndex && m_siCurIndex < m_siListPoolSize )
	{
		m_pclPersonNameInfo[m_siCurIndex] = *PersonInfo;
		
		m_pNameHashTable->Insert( id, &m_pclPersonNameInfo[m_siCurIndex] );

		++m_siCurIndex;

		return true;
	}

	return false;

}

bool cltNewPersonNameManager::Find( int id, cltPersonNameInfo* _pPersonInfo )
{
	cltPersonNameInfo* pclPersonInfo;

	if (m_pNameHashTable->Find( id, &pclPersonInfo ) == true)
	{
		StringCchCopy(_pPersonInfo->szPersonName, MAX_PLAYER_NAME, pclPersonInfo->szPersonName);
		_pPersonInfo->siServerUnique = pclPersonInfo->siServerUnique;

		return true;
	}
	
	return false;
}

bool cltNewPersonNameManager::Remove( int id )
{
	return m_pNameHashTable->Remove( id );
}




