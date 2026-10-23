#include ".\bbbeventmapmanager.h"








CBBBEventMapManager::CBBBEventMapManager(void)
{
	m_MapList.clear();
	m_MapList.resize( MAX_MAP_COUNT );

	for ( int i = 0; i < MAX_MAP_COUNT; ++i )
	{
		CBBBEventMap* pMap = new CBBBEventMap( i );
		m_MapList[i] = pMap;
	}
}

CBBBEventMapManager::~CBBBEventMapManager(void)
{
	for ( int i = 0; i < MAX_MAP_COUNT; ++i )
	{
		if ( NULL != m_MapList[i] )
		{
			delete m_MapList[i];
		}
	}

	m_MapList.clear();
	m_MapList.resize( MAX_MAP_COUNT );
}


//////////////////////////////////////////////////////////////////////////
bool CBBBEventMapManager::IsFull()
{
	for ( int i = 0; i < MAX_MAP_COUNT; ++i )
	{
		if ( true == m_MapList[i]->IsAvailable() )
		{
			return false;
		}
	}

	return true;
}

SI16 CBBBEventMapManager::AddUser(SI32 siPersonID, TCHAR* pszCharName)
{
	for ( int i = 0; i < MAX_MAP_COUNT; ++i )
	{
		if ( true == m_MapList[i]->IsAvailable() )
		{
			m_MapList[i]->EnterUser( siPersonID, pszCharName );
			return i;
		}
	}

	return -1;
}

void CBBBEventMapManager::RemoveUser(SI32 siPersonID)
{
	for ( int i = 0; i < MAX_MAP_COUNT; ++i )
	{
		if ( siPersonID == m_MapList[i]->GetUserID() )
		{
			m_MapList[i]->LeaveUser();
			break;
		}
	}
}

void CBBBEventMapManager::SubAction()
{
	for ( int i = 0; i < MAX_MAP_COUNT; ++i )
	{
		m_MapList[i]->DoTimeTask();
	}
}

void CBBBEventMapManager::StageFail(SI32 siPersonID)
{
	for ( int i = 0; i < MAX_MAP_COUNT; ++i )
	{
		if ( siPersonID == m_MapList[i]->GetUserID() )
		{
			m_MapList[i]->GameFail();
			break;
		}
	}
}

void CBBBEventMapManager::StageClear(SI32 siPersonID)
{
	for ( int i = 0; i < MAX_MAP_COUNT; ++i )
	{
		if ( siPersonID == m_MapList[i]->GetUserID() )
		{
			m_MapList[i]->GameClear();
			break;
		}
	}
}

void CBBBEventMapManager::SetTemporaryReserve(SI32 siPersonID)
{
	for ( int i = 0; i < MAX_MAP_COUNT; ++i )
	{
		if ( true == m_MapList[i]->IsAvailable() )
		{
			m_MapList[i]->SetTemporaryReserve( siPersonID );
		}
	}
}

void CBBBEventMapManager::ActiveTemporaryReserve(SI32 siPersonID)
{
	for ( int i = 0; i < MAX_MAP_COUNT; ++i )
	{
		if ( siPersonID == m_MapList[i]->GetUserID() )
		{
			m_MapList[i]->ActiveTemporaryReserve( siPersonID );
		}
	}
}

void CBBBEventMapManager::CancelTemporaryReserve(SI32 siPersonID)
{
	for ( int i = 0; i < MAX_MAP_COUNT; ++i )
	{
		if ( siPersonID == m_MapList[i]->GetUserID() )
		{
			m_MapList[i]->CancelTemporaryReserve( siPersonID );
		}
	}
}

void CBBBEventMapManager::SetPlayTime(SI32 siPlayTime)
{
	for ( int i = 0; i < MAX_MAP_COUNT; ++i )
	{
		m_MapList[i]->m_siStagePlayDelay = siPlayTime * 1000;
	}
}