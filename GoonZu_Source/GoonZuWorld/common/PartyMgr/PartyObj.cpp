#include "PartyObj.h"
#include "..\Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "..\CommonLogic\CommonLogic.h"
#include "..\CommonLogic\MsgType-System.h"
#include "..\CommonLogic\Msg\MsgType-Party.h"

extern cltCommonLogic *pclClient;

CPartyObj::CPartyObj( SI32 PartyIndex ) : CObj()
{
	m_siPartyIndex = PartyIndex;
	Initialize();

	m_dwStartTime	= 0;
	m_dwElapseTime	= 0;
	m_dwEndTime		= 0;
}

CPartyObj::~CPartyObj()
{
	m_dwStartTime	= 0;
	m_dwElapseTime	= 0;
	m_dwEndTime		= 0;

	Destroy();
}
	
void CPartyObj::Initialize()
{
	for ( SI32 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		m_stPartyUserList[ i ].Initialize();
	}

	TCHAR* pText = GetTxtFromMgr(3098);
	StringCchCopy ( m_strPartyName, MAX_PARTY_NAME_LENGTH, pText );

	bIsPartyStaus = false;		// 파티중이 아님

	m_siPartyUserNum = 0;
	
	m_siItemGetPattern = ITEM_GETPATTERN_ROTATION; //파티획득막음
	//m_siItemGetPattern = ITEM_GETPATTERN_NORMAL;
//	m_siItemGetPersonIndex = 0;

	ZeroMemory(&m_PartyQuestInfo, sizeof(PartyQuest_UnitData));
	
	return;
}

void CPartyObj::Destroy()
{
	return;
}

bool CPartyObj::IsEmptyUserListExist()
{
	for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( ! m_stPartyUserList[ i ].bPartyStatus )
		{
			return true;
		}
	}

	return false;
}

// 리턴 값이 -1 이면 파티유저 다 찬 상태
SI16 CPartyObj::GetEmptyPartyUserListIndex()
{
	for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( ! m_stPartyUserList[ i ].bPartyStatus )
		{
			return i;
		}
	}

	return -1;
}

SI32 CPartyObj::GetPartyIndex()
{
	return m_siPartyIndex;
}


bool CPartyObj::IsSamePartyIndex( SI32 PartyIndex )
{
	if ( ! bIsPartyStaus )				// 파티 중이 아니라면 체크할 필요 없음.
	{
		return false;
	}

	if ( PartyIndex == m_siPartyIndex )
	{
		return true;
	}

	return false;
}

SI16 CPartyObj::GetSamePersonIDIndex( SI32 PersonID )
{
	for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stPartyUserList[ i ].IsSamePersonID( PersonID ) )
		{
			return i;
		}
	}

	return -1;
}

bool CPartyObj::IsPartyLeader( SI32 PersonID )
{
	for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stPartyUserList[ i ].bIsPartyLeader && m_stPartyUserList[ i ].IsSamePersonID( PersonID ) )
		{
			return true;
		}
	}

	return false;
}


SI32 CPartyObj::GetPartyLeaderUnique()
{
	for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if(m_stPartyUserList[i].bIsPartyLeader)
			return m_stPartyUserList[i].siCharUnique;
	}
	return 0;
}

SI16 CPartyObj::GetChangedLeaderIndex()
{
	for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stPartyUserList[ i ].bPartyStatus )
		{
			m_stPartyUserList[ i ].bIsPartyLeader = true;		// 파티장으로 임명
			return i;
		}
	}

	return -1;
}

SI32 CPartyObj::CreateParty( SI32 PersonID, SI32 CharUnique, SI32 Level )
{
	m_stPartyUserList[ 0 ].Join( PersonID, CharUnique, true, Level );		// 파티 생성자는 제일 위에 위치하므로...
	
//	m_siItemGetPersonIndex = 0;
	    
	bIsPartyStaus = true;

	++m_siPartyUserNum;

	return GetPartyIndex();
}

SI16 CPartyObj::JoinParty( SI32 PersonID, SI32 CharUnique, SI32 Level )
{
	SI16 EmptyListIndex = GetEmptyPartyUserListIndex();

	if ( EmptyListIndex < 0 )
	{
		return -1;
	}
	
	SI16 SamePersonIndex = GetSamePersonIDIndex( PersonID );

	if ( SamePersonIndex < 0 )
	{
		m_stPartyUserList[ EmptyListIndex ].Join( PersonID, CharUnique, false, Level );		// 파티장이 아니므로, false
		
		++m_siPartyUserNum;

		return EmptyListIndex;
	}
	else
	{
		m_stPartyUserList[ SamePersonIndex ].Join( PersonID, CharUnique, false, Level );

		return SamePersonIndex;
	}	
}

SI16 CPartyObj::LeaveParty( SI32 PersonID, SI32 *pRet )
{
	SI16 SamePersonIDIndex = GetSamePersonIDIndex( PersonID );

	if ( SamePersonIDIndex < 0 )
	{
		return -1;
	}

	--m_siPartyUserNum;

	// 파티원이 더이상 없을 경우는 파티를 삭제해야 하므로
	if ( m_siPartyUserNum < 2 )
	{
		*pRet = SamePersonIDIndex;
		return PARTY_DELETE;
	}

	// 파티장이라면 파티장 넘겨 주어야 하므로..
	if ( m_stPartyUserList[ SamePersonIDIndex ].bIsPartyLeader )
	{
		bool bFind = false;

		for ( SI16 i = 1; i < MAX_PARTY_USER_NUM; ++i )
		{
			if ( m_stPartyUserList[i].bPartyStatus && m_stPartyUserList[i].bIsPartyLeader == false )
			{
				SI32 CharUnique = m_stPartyUserList[i].siCharUnique;

				if ( CharUnique < 1 )
				{
					continue;
				}

				SI32 id = pclClient->pclCM->GetIDFromCharUnique( CharUnique );
				
				if ( id < 1 )
				{
					continue;
				}
				else
				{
					bFind = true;

					*pRet = i;
					m_stPartyUserList[ i ].bIsPartyLeader = true;	// 파티장 바뀌었음.


					// SamePersonIDIndex 는 0 임
					m_stPartyUserList[ SamePersonIDIndex ].Initialize();
					m_stPartyUserList[ 0 ].Set( &m_stPartyUserList[ i ] );
					m_stPartyUserList[ i ].Initialize();
					break;
				}
			}
		}

		if ( bFind )
		{
			return PARTY_CHANGE_LEADER;
		}
		else
		{
			*pRet = SamePersonIDIndex;
			return PARTY_DELETE;
		}
	}

	m_stPartyUserList[ SamePersonIDIndex ].Initialize();
	*pRet = SamePersonIDIndex;

	return SamePersonIDIndex;
}

SI16 CPartyObj::ForceLeaveParty( SI32 PersonID )
{
	SI16 SamePersonIDIndex = GetSamePersonIDIndex( PersonID );

	if ( SamePersonIDIndex < 0 )
	{
		return -1;
	}

	m_stPartyUserList[ SamePersonIDIndex ].Initialize();

	--m_siPartyUserNum;

	return SamePersonIDIndex;
}

stPartyUser *CPartyObj::GetPartyUser( SI16 UserListIndex )
{
	return &m_stPartyUserList[ UserListIndex ];
}

SI16 CPartyObj::GetPartyUserListIndex( SI32 CharUnique )
{
	for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stPartyUserList[ i ].bPartyStatus && m_stPartyUserList[ i ].siCharUnique == CharUnique )
		{
			return i;
		}
	}

	return -1;
}

SI32 CPartyObj::GetCharUnique( SI32 PersonID )
{
	for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stPartyUserList[ i ].bPartyStatus && m_stPartyUserList[ i ].siPersonID == PersonID )
		{
			return m_stPartyUserList[ i ].siCharUnique;
		}
	}

	return -1;
}

SI16 CPartyObj::GetPartyUserNum()
{
	return m_siPartyUserNum;
}

SI32 CPartyObj::GetItemGetCharUnique(SI32 * PartyList, SI16 arayMaxIndex)	
{	//[영진] 순환획득 방식에서 아이템을 가져갈 파티원의 캐릭터유니크를 얻는다.
	// 인자는 몹과 같은 맵에 있는 파티원의 리스트이다.
	
	if(PartyList == NULL || arayMaxIndex <= 0 || arayMaxIndex > MAX_PARTY_USER_NUM)
		return -1;	

    for(SI16 i = 0; i< MAX_PARTY_USER_NUM; i++)	
	{	
		if( m_stPartyUserList[i].bPartyStatus && m_stPartyUserList[i].bItemGetChance) // 파티중이고,아이템을 획득할 기회가 남아있고
		{			
			for(SI16 j = 0; j <  arayMaxIndex; j++) //아이템 획득 유효 리스트 안에 포함되어 있다면
			{
				if(PartyList[j] ==  m_stPartyUserList[i].siCharUnique)
				{  // CharUnique 를 리턴하고 아이템 먹을 기회를 없앤다.
					m_stPartyUserList[i].bItemGetChance = false;
					return m_stPartyUserList[i].siCharUnique ;
				}
			}			
		}	
	}
	
	// 유효한 파티원 목록중 아이템을 먹을 기회를 가진 사람이 없었다. 유효파티원의 아이템 획득기회 초기화
	
	SI32 returnVal = -1;

	for(SI16 i = 0; i< MAX_PARTY_USER_NUM; i++)	
	{	
		if( m_stPartyUserList[i].bPartyStatus && m_stPartyUserList[i].bItemGetChance == false) // 파티중이고 획득기회가 없다면
		{			
			for(SI16 j = 0; j <  arayMaxIndex; j++) //아이템 획득 유효 리스트 안에 포함되어 있다면
			{
				if(PartyList[j] ==  m_stPartyUserList[i].siCharUnique)
				{
					m_stPartyUserList[i].bItemGetChance = true;

					// 리턴해줄 캐릭터 유니크값이 아직 설정되어 있지 않다면 하나 넣어줌 그리고 다시 false
					if( returnVal <= 0)
					{
						m_stPartyUserList[i].bItemGetChance = false;
						returnVal = m_stPartyUserList[i].siCharUnique;					
					}

                    break;
				}
			}			
		}	
	}

	// 기회의 초기화가 끝났다
	// 리턴해줄 값이 캐릭터 유니크 값이 설정되었다.

	return returnVal;	
}


TCHAR *CPartyObj::GetPartyName()
{
	if ( ! bIsPartyStaus )				// 파티 중이 아니라면 체크할 필요 없음.
	{
		return NULL;
	}

	return m_strPartyName;
}

bool CPartyObj::SetPartyName( TCHAR *PartyName )
{
	if ( PartyName == NULL || _tcslen( PartyName ) < 1 )
	{
		return false;
	}

	MStrCpy( m_strPartyName, PartyName, MAX_PARTY_NAME_LENGTH );

	return true;
}

void CPartyObj::SendPartyChat( SI32 SendCharUnique, TCHAR *Name, TCHAR *ChatData )
{
	if ( Name == NULL || _tcslen( Name ) < 1 || ChatData == NULL || _tcslen( ChatData ) < 1 )
	{
		return;
	}

	cltGameMsgResponse_Chat clChat(SendCharUnique, CHATMODE_PARTY, (TCHAR*)Name, ChatData );
	cltMsg clMsg(GAMEMSG_RESPONSE_CHAT, clChat.siDataSize, (BYTE*)&clChat);

	for ( SI32 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{		
		if ( m_stPartyUserList[ i ].bPartyStatus && pclClient->pclCM->IsValidID( m_stPartyUserList[ i ].siCharUnique ) )
		{
			pclClient->pclCM->CR[ m_stPartyUserList[ i ].siCharUnique ]->SendNetMsg( (sPacketHeader*)&clMsg );
		}
	}


	return;
}

void CPartyObj::IncreaseExp( SI32 PersonID, SI32 Exp )
{
	for ( SI32 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stPartyUserList[ i ].bPartyStatus && m_stPartyUserList[ i ].siPersonID == PersonID )
		{
			m_stPartyUserList[ i ].siExp += Exp;
		}
	}
	return;
}

void CPartyObj::NotifyExp()
{
	for ( SI32 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stPartyUserList[ i ].bPartyStatus && pclClient->pclCM->IsValidID( m_stPartyUserList[ i ].siCharUnique ) )
		{
			for ( int j = 0; j < MAX_PARTY_USER_NUM; ++j )
			{
				if ( m_stPartyUserList[ j ].bPartyStatus && pclClient->pclCM->IsValidID( m_stPartyUserList[ j ].siCharUnique ) )
				{
					cltGameMsgResponse_Party_NotifyExp clParty_NotifyExp( m_stPartyUserList[ j ].siCharUnique, j, m_stPartyUserList[ j ].siExp );
					cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_NOTIFYEXP, sizeof(clParty_NotifyExp), (BYTE*)&clParty_NotifyExp);

					pclClient->pclCM->CR[ m_stPartyUserList[ i ].siCharUnique ]->SendNetMsg( (sPacketHeader*)&clMsg );
				}
			}
		}
	}
	return;
}

void CPartyObj::NotifyMovePosToPartyMember(SI32 MovedCharUnique, SI16 Xpos, SI16 Ypos, BOOL bWarpSwitch)
{
	WRITE_MEMLOG( 140, 1, 0, 0 );

	if(pclClient->pclCM->IsValidID(MovedCharUnique) == false) return;

	if(pclClient->pclCM->CR[MovedCharUnique]->siPartyIndex != m_siPartyIndex ) return;

    cltCharPos clCharPos( pclClient->pclCM->CR[MovedCharUnique]->GetMapIndex(), Xpos , Ypos);

 	for(SI16 i = 0; i < MAX_PARTY_USER_NUM ; i++)
	{
		if( m_stPartyUserList[i].bPartyStatus && pclClient->pclCM->IsValidID(m_stPartyUserList[i].siCharUnique) )
		{
			//  워프로 인해 위치 알려주는게 아니라면 ( 이동으로 인한 위치알림이라면)
			if( bWarpSwitch == FALSE)
			{
 				if( m_stPartyUserList[i].siCharUnique == MovedCharUnique)
 					continue;

				//다른맵이면 위치를 알려줄 필요가 없다.
				if(pclClient->pclCM->CR[m_stPartyUserList[i].siCharUnique]->GetMapIndex() != pclClient->pclCM->CR[MovedCharUnique]->GetMapIndex() ) 
					continue;
				
				// 가시영역 안에 있다면 위치를 이미 알고 있으므로 보낼필요가 없다. (가시영역 대충 -> 15 으로 잡음)
				if(max(TABS(pclClient->pclCM->CR[m_stPartyUserList[i].siCharUnique]->GetX() - pclClient->pclCM->CR[MovedCharUnique]->GetX()),
					TABS(pclClient->pclCM->CR[m_stPartyUserList[i].siCharUnique]->GetY() - pclClient->pclCM->CR[MovedCharUnique]->GetY())) < 20)
						continue;				

			}

			cltGameMsgResponse_Party_PartyUserPos clinfo( MovedCharUnique ,&clCharPos);
			cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_PARTYUSERPOS, sizeof(clinfo),(BYTE*)&clinfo);
			pclClient->pclCM->CR[m_stPartyUserList[i].siCharUnique]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
	
	return;
}

void CPartyObj::NotifyGetIemToPartyMember(SI32 ItemGetCharUnique,cltItem * pclItem)
{	
	WRITE_MEMLOG( 140, 2, 0, 0 );

	if(pclClient->pclCM->IsValidID(ItemGetCharUnique) == false) return;

	if(pclClient->pclCM->CR[ItemGetCharUnique]->siPartyIndex != m_siPartyIndex ) return;
	
	if(pclItem == NULL) return;


	
	for ( SI32 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stPartyUserList[ i ].bPartyStatus && pclClient->pclCM->IsValidID( m_stPartyUserList[ i ].siCharUnique ) )
		{
			//자신에게는 보내지 않는다
			if( m_stPartyUserList[i].siCharUnique == ItemGetCharUnique)
				continue;
			
			cltGameMsgResponse_Party_NotifyItemGet clinfo(  ItemGetCharUnique , pclItem );
			cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_NOTIFYITEMGET,sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->pclCM->CR[m_stPartyUserList[i].siCharUnique]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

void CPartyObj::NotifyItemGetPatternToPartyMember(SI16 ItemGetPattern )
{
	cltGameMsgResponse_Party_ChangeItemGetPattern clinfo(ItemGetPattern);
	cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_CHANGEITEMGETPATTERN,sizeof(clinfo),(BYTE*)&clinfo);

	for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stPartyUserList[ i ].bPartyStatus && pclClient->pclCM->IsValidID( m_stPartyUserList[ i ].siCharUnique ) )
		{
			pclClient->pclCM->CR[ m_stPartyUserList[ i ].siCharUnique ]->SendNetMsg((sPacketHeader*)&clMsg );
		}
	}
}

void CPartyObj::NotifyLevelUpToPatryMember(SI32 LevelUpCharUnique, SI32 Level)
{
	cltGameMsgResponse_Party_NotifyLevelUp clinfo(LevelUpCharUnique,Level);
	cltMsg clMsg(GAMEMSG_RESPONSE_PARTY_NOTIFYLEVELUP,sizeof(clinfo),(BYTE*)&clinfo);

	for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
	{
		if ( m_stPartyUserList[ i ].bPartyStatus && pclClient->pclCM->IsValidID( m_stPartyUserList[ i ].siCharUnique ) )
		{
			pclClient->pclCM->CR[ m_stPartyUserList[ i ].siCharUnique ]->SendNetMsg((sPacketHeader*)&clMsg );
		}
	}
}

