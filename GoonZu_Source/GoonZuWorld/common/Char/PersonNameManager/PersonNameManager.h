//---------------------------------
// 2006/2/20 √÷ø¨¡ÿ
//---------------------------------

#ifndef _PERSONNAMEMANAGER_H
#define _PERSONNAMEMANAGER_H

#include <directives.h>
#include "..\..\CommonHeader.h"
#include "../NLib/NHashTable.h"

class cltPersonNameManager{
private:
public:

	NHashTable< int, TCHAR* >*	m_pNameHashTable;
	SI32 m_siListPoolSize;
	TCHAR* m_szPersonNames;
	SI32 m_siCurPos;

	cltPersonNameManager(SI32 arraySize, SI32 listPoolSize);
	~cltPersonNameManager();

	bool Insert( int id, TCHAR* personname);
	bool Remove( int id );

	bool Find( int id, TCHAR* personname );
};

class cltPersonNameInfo
{
public:
	cltPersonNameInfo()
	{
		ZeroMemory( this, sizeof(*this) );
	}
	
	cltPersonNameInfo(SI32	ServerUnique, TCHAR*	PersonName)
	{
		siServerUnique = ServerUnique;
		StringCchCopy( szPersonName, sizeof(szPersonName), PersonName );
	}

	SI32	siServerUnique;
	TCHAR	szPersonName[MAX_PLAYER_NAME];
};

class cltNewPersonNameManager
{
private:
public:

	NHashTable< int, cltPersonNameInfo* >*	m_pNameHashTable;
	SI32					m_siListPoolSize;
	cltPersonNameInfo*		m_pclPersonNameInfo;
	SI32					m_siCurIndex;
	
	cltNewPersonNameManager( SI32 arraySize, SI32 listPoolSize );
	~cltNewPersonNameManager();

	bool Insert( int id, cltPersonNameInfo* PersonInfo);
	bool Remove( int id );

	bool Find( int id, cltPersonNameInfo* PersonInfo );
};


#endif
