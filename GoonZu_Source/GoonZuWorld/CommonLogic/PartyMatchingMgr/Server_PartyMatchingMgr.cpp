// 파티매칭
#include "Server_PartyMatchingMgr.h"
#include "./../Server/Server.h"

#include "../../CommonLogic/PartyMatchingMgr/Defines_PartyMatchingMgr.h"
#include "../../common/PartyMgr/PartyMgr.h"





CServer_PartyMatchingMgr::CServer_PartyMatchingMgr()
{
	m_dwWaitListRefreshTime = 0;

	for ( SI16 i = 0; i < MAX_PARTYQUEST_CATEGORY_NUM; ++i )
	{
		m_aPMList[i].RemoveAll();
	}
}

CServer_PartyMatchingMgr::~CServer_PartyMatchingMgr()
{
	for ( SI16 i = 0; i < MAX_PARTYQUEST_CATEGORY_NUM; ++i )
	{
		m_aPMList[i].RemoveAll();
	}
}

void CServer_PartyMatchingMgr::Action()
{
	cltServer* pServer = (cltServer*)pclClient;

	// 5초마다 주기적으로 대기자 리스트 전체를 한번씩 검사해서 없어진 유저를 찾아내 제거 한다.
	if ( m_dwWaitListRefreshTime < pServer->CurrentClock )
	{
		_Refresh_UserList();
		m_dwWaitListRefreshTime = pServer->CurrentClock + 5000;		// 5초마다 갱신 (클라로 알려주진 않음)
	}
}

void CServer_PartyMatchingMgr::_Refresh_UserList()
{
	SI16 siCount = 0;
	NkRbTreeMapPos pos = NULL;

	//////////////////////////////////////////////////////////////////////////
	for ( SI16 k = 0; k < MAX_PARTYQUEST_CATEGORY_NUM; ++k )
	{
		pos = m_aPMList[k].GetBeginPos();
		siCount = m_aPMList[k].GetCount();

		for ( SI16 i = 0; i < siCount; ++i )
		{
			if ( false == m_aPMList[k].InvalidPos(pos) )
			{
				NkRbTreeMapPos nextpos = m_aPMList[k].GetNextPos(pos);

				if ( NULL != m_aPMList[k].GetDataPtr(pos) )
				{
					SI32 siLeaderID = pclClient->pclCM->GetIDFromCharUnique(m_aPMList[k].GetData(pos).m_siLeaderID);

					if ( NULL == pclClient->pclCM->GetCharServer( siLeaderID ) )
					{
						m_aPMList[k].Remove( m_aPMList[k].GetData(pos).m_siLeaderID );
					}
				}

				pos = nextpos;
			}
		}
	}
}

void CServer_PartyMatchingMgr::MsgProcess_Request_PartyMatchingLevel( SI32 siCharID, SI16 siLevelType )
{
	cltServer* pServer = (cltServer*)pclClient;
	cltCharServer* pCharServer = (cltCharServer*)pServer->pclCM->GetCharServer( siCharID );
	if ( NULL == pCharServer ) return;
	if ( PARTYMATCHING_GROUP_01 > siLevelType || PARTYMATCHING_GROUP_10 < siLevelType ) return;

	_sendpacket_PartyMatchingList( siCharID, m_aPMList[siLevelType] );
}

void CServer_PartyMatchingMgr::MsgProcess_Request_PartyMatchingCreate(SI32 siCharID, CInfo_PartyMatching& rInfo)
{
	cltServer* pServer = (cltServer*)pclClient;
	if ( NULL == pServer ) return;
	if ( NULL == pServer->m_pServer_PartyMatchingMgr ) return;
	if ( PARTYMATCHING_GROUP_01 > rInfo.m_siParentQuest || PARTYMATCHING_GROUP_10 < rInfo.m_siParentQuest ) return;

	cltCharServer* pCharServer = (cltCharServer*)pServer->pclCM->GetCharServer( siCharID );
	if ( NULL == pCharServer ) return;

	SI32 siCharServerUnique = pCharServer->GetCharUnique();

	SI32 siPersonID = -1;
	siPersonID = pCharServer->pclCI->GetPersonID();
	if ( 1 > siPersonID ) return;

	rInfo.m_siLeaderID = siCharServerUnique;

	m_aPMList[rInfo.m_siParentQuest].Insert( siCharServerUnique, rInfo );
	_sendpacket_PartyMatchingList( siCharID, m_aPMList[rInfo.m_siParentQuest] );
	_sendpacket_PartyMatchingCreate( siCharID, true );
}

void CServer_PartyMatchingMgr::Remove_PartyMatching(SI32 siLeaderID)
{
	cltServer* pServer = (cltServer*)pclClient;
	if ( NULL == pServer ) return;
	if ( 0 > siLeaderID ) return;

	// 리더가 서버에서 사라진 경우도 있을수 있기 때문에 캐릭터의 정보를 찾진 않고 모든 파티매칭 리스트를 검색한다.
	for ( SI16 i = 0; i < MAX_PARTYQUEST_CATEGORY_NUM; ++i )
	{
		m_aPMList[i].Remove( siLeaderID );
	}
}

SI32 CServer_PartyMatchingMgr::Get_Category(SI32 siLeaderUnique) 
{
	for ( SI16 i = 0; i < MAX_PARTYQUEST_CATEGORY_NUM; ++i )
	{
		NkRbTreeMapPos pos = NULL;
		pos = m_aPMList[i].FindPos( siLeaderUnique );
		if ( false == m_aPMList[i].InvalidPos(pos) )
		{
			return m_aPMList[i].GetData( pos ).m_siParentQuest;
		}
	}

	return -1;
}

SI32 CServer_PartyMatchingMgr::Get_Mission(SI32 siLeaderUnique)
{
	for ( SI16 i = 0; i < MAX_PARTYQUEST_CATEGORY_NUM; ++i )
	{
		NkRbTreeMapPos pos = NULL;
		pos = m_aPMList[i].FindPos( siLeaderUnique );
		if ( false == m_aPMList[i].InvalidPos(pos) )
		{
			return m_aPMList[i].GetData( pos ).m_siChildQuest;
		}
	}

	return -1;
}

TCHAR* CServer_PartyMatchingMgr::GetPartyName(SI32 siLeaderUnique)
{
	for ( SI16 i = 0; i < MAX_PARTYQUEST_CATEGORY_NUM; ++i )
	{
		NkRbTreeMapPos pos = NULL;
		pos = m_aPMList[i].FindPos( siLeaderUnique );
		if ( false == m_aPMList[i].InvalidPos(pos) )
		{
			return m_aPMList[i].GetData( pos ).m_strPartyName;
		}
	}

	return NULL;
}

bool CServer_PartyMatchingMgr::IsExist(SI32 siLeaderUnique)
{
	for ( SI16 i = 0; i < MAX_PARTYQUEST_CATEGORY_NUM; ++i )
	{
		NkRbTreeMapPos pos = NULL;
		pos = m_aPMList[i].FindPos( siLeaderUnique );
		if ( false == m_aPMList[i].InvalidPos(pos) )
		{
			return true;
		}
	}

	return false;
}




/////////////////

//-----------------------------------------------------------------------------------------------
// 패킷 전송 함수들 : 서버 -> 클라
//-----------------------------------------------------------------------------------------------

// 레벨 구간별 파티 매칭 리스트를 클라로 전송
void CServer_PartyMatchingMgr::_sendpacket_PartyMatchingList (SI32 siCharID, const PMLIST& rPMList )
{
	cltServer* pServer = (cltServer*)pclClient;
	if ( NULL == pServer ) return;

	SI16 siTotalPartyMatchingListCount = rPMList.GetCount();
	SI16 siTotalPageCount = siTotalPartyMatchingListCount / MAX_PARTYMATCHING_LIST_SEND_SIZE + 1;
	NkRbTreeMapPos pCurrentPos = rPMList.GetBeginPos();

	for ( SI16 i = 0; i < siTotalPageCount; ++i )
	{
		cltGameResponse_PartyMatchingLevel clInfo;

		for ( SI16 k = 0; k < MAX_PARTYMATCHING_LIST_SEND_SIZE; ++k )
		{
			if ( true == rPMList.InvalidPos(pCurrentPos) ) break;

			clInfo.AddPartyMatchingInfo( (i+k), rPMList.GetData(pCurrentPos) );
			pCurrentPos = rPMList.GetNextPos( pCurrentPos );
		}

		if ( 0 == i ) clInfo.bNeedClearOldList = true;
		else clInfo.bNeedClearOldList = false;

		cltMsg clMsg( GAMEMSG_RESPONSE_PARTYMATCHING_LEVEL, sizeof(clInfo), (BYTE*)&clInfo );
		pServer->pclCM->GetCharServer( siCharID )->SendNetMsg( (sPacketHeader*)&clMsg );
	}
}

// 파티매칭 등록이 성공적으로 이루어 졌다는 패킷을 클라로 전송
void CServer_PartyMatchingMgr::_sendpacket_PartyMatchingCreate(SI32 siCharID, bool bSuccess)
{
	cltServer* pServer = (cltServer*)pclClient;
	if ( NULL == pServer ) return;

	cltGameResponse_PartyMatchingCreate clInfo( bSuccess );
	cltMsg clMsg( GAMEMSG_RESPONSE_CREATECOMPLETEPARTY, sizeof(clInfo), (BYTE*)&clInfo );
	pServer->pclCM->GetCharServer( siCharID )->SendNetMsg( (sPacketHeader*)&clMsg );
}










