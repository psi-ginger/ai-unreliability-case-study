#include ".\bbbwaitlistmanager.h"

#include "../Server/Server.h"
#include "../Char/CharManager/CharManager.h"

#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/Msg/MsgType-BokBulBok.h"





CBBBWaitListManager::CBBBWaitListManager(void)
{
	m_dwWaitListRefreshTime = 0;
}

CBBBWaitListManager::~CBBBWaitListManager(void)
{
	m_WaitList.clear();
}



//////////////////////////////////////////////////////////////////////////
bool CBBBWaitListManager::IsFull()
{
	SI16 siListLength = m_WaitList.size();
	if ( MAX_RESERVATION_NUM > siListLength )
	{
		return false;
	}

	return true;
}

bool CBBBWaitListManager::AddUser(SI32 siUserID, TCHAR* pszUserName)
{
	if ( true == _AlreadyExist( pszUserName ) ) return true; // 대기자 예약창에 이미 존재하고 있는 캐릭터라면

	if ( m_WaitList.size() < MAX_RESERVATION_NUM )
	{
		_BBBWaitUserData pUserData;

		pUserData.siUserID = siUserID;
		MStrCpy( pUserData.szUserName, pszUserName, MAX_CHARACTER_NAME_LENGTH );

		m_WaitList.push_back( pUserData );
		return true;
	}
	else
	{
		return false;
	}
}

void CBBBWaitListManager::RemoveUser(SI32 siUserID)
{
	m_WLitr = m_WaitList.begin();

	while ( m_WaitList.end() != m_WLitr )
	{
		if ( siUserID == (*m_WLitr).siUserID )
		{
			WAITLIST::iterator itr = m_WLitr;
			++m_WLitr;
			m_WaitList.erase( itr );
			break;
		}
		else
		{
			++m_WLitr;
		}
	}
}

void CBBBWaitListManager::GetWaitList(_BBBWaitUserData* pWaitList)
{
	m_WLitr = m_WaitList.begin();

	int i = 0;
	while ( m_WaitList.end() != m_WLitr )
	{
		MStrCpy( pWaitList[i].szUserName, (*m_WLitr).szUserName, MAX_CHARACTER_NAME_LENGTH );

		++m_WLitr;
		++i;
	}
}

bool CBBBWaitListManager::_AlreadyExist(TCHAR* pszUserName)
{
	m_WLitr = m_WaitList.begin();

	while ( m_WaitList.end() != m_WLitr )
	{
		if ( 0 == _tcscmp( (*m_WLitr).szUserName, pszUserName ) )
		{
			return true;
		}

		++m_WLitr;
	}

	return false;
}

void CBBBWaitListManager::SubAction()
{
	cltServer* pServer = (cltServer*)pclClient;

	// 5초마다 주기적으로 대기자 리스트 전체를 한번씩 검사해서 없어진 유저를 찾아내 제거 한다.
	if ( m_dwWaitListRefreshTime < pServer->CurrentClock )
	{
		_Refresh_UserList();
		m_dwWaitListRefreshTime = pServer->CurrentClock + 5000;		// 5초마다 갱신 (클라로 알려주진 않음)
	}
}

void CBBBWaitListManager::_Refresh_UserList()
{	
	cltServer* pclServer = (cltServer*)pclClient;

	m_WLitr = m_WaitList.begin();
	while ( m_WaitList.end() != m_WLitr )
	{
		cltCharServer* pclChar = pclServer->pclCM->GetCharServer( (*m_WLitr).siUserID);

		if ( NULL == pclChar )
		{
			WAITLIST::iterator itr = m_WLitr;
			++m_WLitr;
			m_WaitList.erase( itr );
		}
		else
		{
			const TCHAR* pszCharName = pclChar->GetName();
			if ( 0 != _tcscmp(pszCharName, (*m_WLitr).szUserName) )
			{
				WAITLIST::iterator itr = m_WLitr;
				++m_WLitr;
				m_WaitList.erase( itr );
			}
			else
			{
				++m_WLitr;
			}
		}
	}
}

bool CBBBWaitListManager::WaitUserExist()
{
	m_WLitr = m_WaitList.begin();

	if ( m_WaitList.end() != m_WLitr )
	{
		cltServer* pServer = (cltServer*)pclClient;

		cltCharServer* pCharServer = pServer->pclCM->GetCharServer( (*m_WLitr).siUserID );
		if ( NULL != pCharServer )
		{
			const TCHAR* pszCharName = pCharServer->GetName();
			if ( 0 == _tcscmp( pszCharName, (*m_WLitr).szUserName ) )
			{
				return true;
			}
		}
		else	// 대기 리스트엔 있는데 게임 서버에 존재하지 않는 캐릭이면 리스트에서 삭제 (로갓 했든 뭘 했든 해서 나간캐릭이니까)
		{
			// 한 캐릭만 체크하지 말고 그냥 한방에 리스트 전부다 검사.
			_Refresh_UserList();
		}
	}

	// _Refresh_UserList() 함수 호출후에도 대기리스트에 유저가 있을수 있지만 일단 현재 루프에선 false 리턴하고 다음 루프때 체크 하도록 한다.
	return false;
}

void CBBBWaitListManager::Send_EnterMapQuestion()
{
	m_WLitr = m_WaitList.begin();

	if ( m_WaitList.end() != m_WLitr )
	{
		cltServer* pServer = (cltServer*)pclClient;

		cltCharServer* pCharServer = pServer->pclCM->GetCharServer( (*m_WLitr).siUserID );
		if ( NULL != pCharServer )
		{
			const TCHAR* pszCharName = pCharServer->GetName();
			if ( 0 == _tcscmp( pszCharName, (*m_WLitr).szUserName ) )
			{
				// 클라에 복불복 게임맵으로 워프하겠냐고 메세지 보냄
				cltGameMsgResponse_BBB_WarpMapQuestion clinfo( (*m_WLitr).siUserID );
				cltMsg clMsg( GAMEMSG_RESPONSE_BBB_WARPMAPQUESTION, sizeof(clinfo), (BYTE*)&clinfo);
				pCharServer->SendNetMsg((sPacketHeader*)&clMsg);

				// 그리고 이 1순위 대기자는 대기리스트에서 제거
				m_WaitList.erase( m_WLitr );
			}
		}
		else	// 대기 리스트엔 있는데 게임 서버에 존재하지 않는 캐릭이면 리스트에서 삭제 (로갓 했든 뭘 했든 해서 나간캐릭이니까)
		{
			// 한 캐릭만 체크하지 말고 그냥 한방에 리스트 전부다 검사.
			_Refresh_UserList();
		}
	}
}

SI32 CBBBWaitListManager::Get1stUserID()
{
	m_WLitr = m_WaitList.begin();

	if ( m_WaitList.end() != m_WLitr )
	{
		cltServer* pServer = (cltServer*)pclClient;

		cltCharServer* pCharServer = pServer->pclCM->GetCharServer( (*m_WLitr).siUserID );
		if ( NULL != pCharServer )
		{
			const TCHAR* pszCharName = pCharServer->GetName();
			if ( 0 == _tcscmp( pszCharName, (*m_WLitr).szUserName ) )
			{
				return (*m_WLitr).siUserID;
			}
		}
		else	// 대기 리스트엔 있는데 게임 서버에 존재하지 않는 캐릭이면 리스트에서 삭제 (로갓 했든 뭘 했든 해서 나간캐릭이니까)
		{
			// 한 캐릭만 체크하지 말고 그냥 한방에 리스트 전부다 검사.
			_Refresh_UserList();
		}
	}

	// _Refresh_UserList() 함수 호출후에도 대기리스트에 유저가 있을수 있지만 일단 현재 루프에선 false 리턴하고 다음 루프때 체크 하도록 한다.
	return 0;
}

