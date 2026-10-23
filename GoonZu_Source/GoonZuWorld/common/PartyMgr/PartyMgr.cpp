#include "PartyMgr.h"
#include "PartyObj.h"

#include "..\SMLib\LibList\LibList.h"

#include "..\Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"


#include "..\CommonLogic\Msg\MsgType-Party.h"

#include "..\CommonLogic\CommonLogic.h"

#include "../common/Char/CharServer/Char-Server.h"
#include "../../Server/Server.h"
#include "../client/ninterface/npartydlg/NPartyDlg.h"

//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"

extern cltCommonLogic *pclClient;

CPartyMgr::CPartyMgr() : CLibListMgr()
{
	Initialize();
	CreateMaxEmptyObj();
}

CPartyMgr::~CPartyMgr()
{
	Destroy();
}

void CPartyMgr::Initialize()
{
	return;
}

void CPartyMgr::Destroy()
{
	return;
}

void CPartyMgr::CreateMaxEmptyObj()
{
	CPartyObj *pTemp = NULL;

	for ( int i = 0; i < MAX_PARTY_OBJ_NUM; ++i )
	{
		pTemp = new CPartyObj( i + 1 );		// 파티 각각에 대해 인덱스를 부여해서 좀 더 효과적으로 관리하기 위해서
		PushBackToEmptyList( pTemp );
	}

	return;
}

void CPartyMgr::Action()
{
	static clock_t PreClock = 0;

	if( TABS( pclClient->CurrentClock - PreClock ) < 10000 )
	{
		return;
	}

	PreClock = pclClient->CurrentClock;

	CPartyObj *pTemp = (CPartyObj *)GetExistList()->GetFirst();
	
	while( pTemp )
	{
		pTemp->NotifyExp();
			
		// 유효한 파티의 진행중인 파티퀘스트에 시간이 걸려있다면
		if(pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching))
		{
			cltServer* pServer = (cltServer*) pclClient;

			if(pTemp->GetPartyIndex() > 0 && pTemp->m_PartyQuestInfo.m_bGoingQuest == true && pTemp->m_PartyQuestInfo.m_siTimeLimit > 0)
			{
				// 시간종료
				if(TABS(pTemp->m_dwEndTime) <= TABS(pclClient->CurrentClock))
				{
					pServer->m_pPartyQuestMgr->DoGiveUpPartyQuest(pTemp->GetPartyIndex(), 0, PARTYQUEST_MESSAGE_TIMEOUT);
				}
				else
				{
					// 1분마다 남은시간을 보여줍니다.	
					if(TABS(pclClient->CurrentClock - pTemp->m_dwElapseTime) > 30 * 1000)
					{
						pServer->m_pPartyQuestMgr->UpdatePartyQuestTime(pTemp->GetPartyIndex(), &pTemp->m_PartyQuestInfo);
						pTemp->m_dwElapseTime = pclClient->CurrentClock;
					}
				}
			}
		}

		pTemp = (CPartyObj *)pTemp->GetNext();
	}

	return;
}

SI32 CPartyMgr::CreateParty( SI32 PersonID, SI32 CharUnique, SI32 Level )
{
	CPartyObj *pTemp = (CPartyObj *)PopFrontToEmptyListAndPushBackToExistList();

	if ( pTemp == NULL )		// 파티 개수 초과
	{
		return -1;
	}

	pTemp->Initialize();		// 기존에 쓰레기 값들을 완전 초기화 시킴.

	return pTemp->CreateParty( PersonID, CharUnique, Level );	// 파티 인덱스를 리턴해줌
}

/*
// 조인된 위치(m_stPartyUserList[index]의 index값)를 리턴함.
SI16 CPartyMgr::JoinParty( SI32 FromPersonID, SI32 CharUnique, SI32 PartyIndex )
{
	CPartyObj *pTemp = (CPartyObj *)GetExistList()->GetFirst();
	
	while( pTemp )
	{
		if ( pTemp->IsSamePartyIndex( PartyIndex ) )
		{			
			return pTemp->JoinParty( FromPersonID, CharUnique );
		}

		pTemp = (CPartyObj *)pTemp->GetNext();
	}

	return -1;			// 파티가 존재하지 않거나, 파티장이 아닌 사람이 넣을려고 했는 경우
}
*/

SI16 CPartyMgr::LeaveParty( SI32 PersonID, SI32 PartyIndex, SI32 *pRet )
{
	cltServer* pServer = (cltServer*) pclClient;

	if ( PartyIndex < 1 )
	{
		return -1;
	}

	CPartyObj *pTemp = (CPartyObj *)GetExistList()->GetFirst();
	
	while( pTemp )
	{
		if ( pTemp->IsSamePartyIndex( PartyIndex ) )
		{
			SI32 CharUnique = pTemp->GetCharUnique( PersonID );
			
			if ( CharUnique < 1 )
			{
				return -1;
			}
			
			SI32 id = pclClient->pclCM->GetIDFromCharUnique( CharUnique );

			if ( id < 1 )
			{
				return -1;
			}

			if ( pclClient->pclCM->CR[ id ]->pclCI->GetPersonID() != PersonID )
			{
				return -1;
			}

			SI16 MyPartyUserListIndex = pTemp->GetPartyUserListIndex( CharUnique );

			if ( MyPartyUserListIndex < 0 )			// 내가 파티원 리스트에 없음.
			{
				return -1;
			}

			SI16 Ret = pTemp->LeaveParty( PersonID, pRet );

			if ( Ret < 0 )
			{
				return -1;
			}

			SI32 siPartyIndex = pclClient->pclCM->CR[id]->siPartyIndex;

			pclClient->pclCM->CR[id]->siPartyIndex = 0;			// 캐릭터 정보 업데이트


			switch ( Ret )
			{
			case PARTY_DELETE:				// 파티 삭제
				{
					if(pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == true)
					{
						if(GetPartyObj(siPartyIndex)->m_PartyQuestInfo.m_bGoingQuest == true)
						{
							if(GetPartyObj(siPartyIndex)->GetPartyUserNum() <  pServer->m_pPartyQuestMgr->GetPartyQuestCategoryData(GetPartyObj(siPartyIndex)->m_PartyQuestInfo.m_siCategoryIndex)->m_siMinUser)
							{
								pServer->m_pPartyQuestMgr->DoGiveUpPartyQuest(siPartyIndex, id, PARTYQUEST_MESSAGE_FAILED_LESS_USER);
							}	
						}
					}	


					for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
					{
						if ( pTemp->GetPartyUser( i )->bPartyStatus )
						{
							SI32 ListUserCharUnique = pTemp->GetPartyUser( i )->siCharUnique;

							if ( ListUserCharUnique < 1 )
							{
								continue;
							}

							SI32 ListUserCharID = pclClient->pclCM->GetIDFromCharUnique( ListUserCharUnique );

							if ( ListUserCharID < 1 )
							{
								continue;
							}

							if ( pclClient->pclCM->IsValidID( ListUserCharID ) == TRUE )
							{
								if ( pclClient->pclCM->CR[ ListUserCharID ]->pclCI->GetPersonID() != pTemp->GetPartyUser( i )->siPersonID )
								{
									continue;
								}

								cltGameMsgResponse_Party_Delete clGameMsgResponse_Party_Delete( PartyIndex );
								cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_DELETE, sizeof( clGameMsgResponse_Party_Delete ), (BYTE*)&clGameMsgResponse_Party_Delete );
								pclClient->pclCM->CR[ ListUserCharID ]->SendNetMsg((sPacketHeader*)&clMsg );

								pclClient->pclCM->CR[ListUserCharID]->siPartyIndex = 0;

								InPartyDungeonDeleteParty( ListUserCharID );
							}
						}
					}

					
					/*
					// 유저가 나갔음을 알려줌
					cltGameMsgResponse_Party_DelUser clGameMsgResponse_Party_DelUser( *pRet, CharUnique );
					cltMsg clMsg1( GAMEMSG_RESPONSE_PARTY_DELUSER, sizeof( clGameMsgResponse_Party_DelUser ), (BYTE*)&clGameMsgResponse_Party_DelUser );
					pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1 );
					*/

					pTemp->Initialize();
					GetExistList()->Pop( pTemp );
					PushBackToEmptyList( pTemp );
					return PARTY_DELETE;
				}
				break;

			case PARTY_CHANGE_LEADER:		// 파티장 변경
				{
					SI32 NewLeaderUserListIndex = *pRet;
					SI32 NewLeaderPersonID = pTemp->GetPartyUser( 0 )->siPersonID;
					SI32 NewLeaderCharUnique = pTemp->GetPartyUser( 0 )->siCharUnique;

					// 이 안으로 올수 없지만... 혹시나 해서 예외처리
					if ( NewLeaderUserListIndex < 0 )
					{
						return -1;
					}

					for ( SI16 i = 0; i < MAX_PARTY_USER_NUM; ++i )
					{
						if ( pTemp->GetPartyUser( i )->bPartyStatus )
						{
							SI32 ListUserCharUnique = pTemp->GetPartyUser( i )->siCharUnique;

							if ( ListUserCharUnique < 1 )
							{
								continue;
							}

							SI32 ListUserCharID = pclClient->pclCM->GetIDFromCharUnique( ListUserCharUnique );

							if ( ListUserCharID < 1 )
							{
								continue;
							}

							if ( pclClient->pclCM->IsValidID( ListUserCharID ) == TRUE )
							{
								if ( pclClient->pclCM->CR[ ListUserCharID ]->pclCI->GetPersonID() != pTemp->GetPartyUser( i )->siPersonID )
								{
									continue;
								}

								// 유저가 나갔음을 알려줌
								cltGameMsgResponse_Party_DelUser clGameMsgResponse_Party_DelUser( MyPartyUserListIndex, CharUnique );
								cltMsg clMsg1( GAMEMSG_RESPONSE_PARTY_DELUSER, sizeof( clGameMsgResponse_Party_DelUser ), (BYTE*)&clGameMsgResponse_Party_DelUser );
								pclClient->pclCM->CR[ListUserCharID]->SendNetMsg((sPacketHeader*)&clMsg1 );

								// 방장이 바뀌었음을 알려줌
								cltGameMsgResponse_Party_ChangeLeader clGameMsgResponse_Party_ChangeLeader( NewLeaderUserListIndex, NewLeaderCharUnique );
								cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_CHANGELEADER, sizeof( clGameMsgResponse_Party_ChangeLeader ), (BYTE*)&clGameMsgResponse_Party_ChangeLeader );
								pclClient->pclCM->CR[ListUserCharID]->SendNetMsg((sPacketHeader*)&clMsg );
							}
						}
					}
					
					// 파티장이 나간경우
					if(pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == true)
					{
						if(GetPartyObj(siPartyIndex)->m_PartyQuestInfo.m_bGoingQuest == true)
						{
							pServer->m_pPartyQuestMgr->DoGiveUpPartyQuest(siPartyIndex, id, PARTYQUEST_MESSAGE_FAILED_LESS_USER);
						}
					}	


					// 자기 자신이 파티에서 나갔음을 알려줌
					cltGameMsgResponse_Party_DelUser clGameMsgResponse_Party_DelUser( MyPartyUserListIndex, CharUnique );
					cltMsg clMsg1( GAMEMSG_RESPONSE_PARTY_DELUSER, sizeof( clGameMsgResponse_Party_DelUser ), (BYTE*)&clGameMsgResponse_Party_DelUser );
					pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1 );


					return PARTY_CHANGE_LEADER;
				}
				break;

			default:						// 파티원 한명 빠짐
				{
					for ( SI32 i = 0; i < MAX_PARTY_USER_NUM; ++i )
					{
						if ( ! pTemp->GetPartyUser( i )->bPartyStatus )
						{
							continue;
						}

						SI32 ListUserCharUnique = pTemp->GetPartyUser( i )->siCharUnique;

						if ( ListUserCharUnique < 1 )
						{
							continue;
						}

						SI32 ListUserCharID = pclClient->pclCM->GetIDFromCharUnique( ListUserCharUnique );

						if ( ListUserCharID < 1 )
						{
							continue;
						}

						if ( pclClient->pclCM->IsValidID( ListUserCharID ) == TRUE )
						{
							if ( pclClient->pclCM->CR[ ListUserCharID ]->pclCI->GetPersonID() != pTemp->GetPartyUser( i )->siPersonID )
							{
								continue;
							}

							cltGameMsgResponse_Party_DelUser clGameMsgResponse_Party_DelUser( *pRet, CharUnique );
							cltMsg clMsg( GAMEMSG_RESPONSE_PARTY_DELUSER, sizeof( clGameMsgResponse_Party_DelUser ), (BYTE*)&clGameMsgResponse_Party_DelUser );
							pclClient->pclCM->CR[ListUserCharID]->SendNetMsg((sPacketHeader*)&clMsg );
						
						}
					}

					// 파티원이 3명 이하인 경우에는 파티퀘스트가 종료됩니다.
					if(pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == true)
					{
						if(GetPartyObj(siPartyIndex)->m_PartyQuestInfo.m_bGoingQuest == true)
						{
							if(GetPartyObj(siPartyIndex)->GetPartyUserNum() < pServer->m_pPartyQuestMgr->GetPartyQuestCategoryData(GetPartyObj(siPartyIndex)->m_PartyQuestInfo.m_siCategoryIndex)->m_siMinUser)
							{
								pServer->m_pPartyQuestMgr->DoGiveUpPartyQuest(siPartyIndex, id, PARTYQUEST_MESSAGE_FAILED_LESS_USER);
							}	
						}
					}	

					// 자기 자신이 파티에서 나갔음을 알려줌
					cltGameMsgResponse_Party_DelUser clGameMsgResponse_Party_DelUser( MyPartyUserListIndex, CharUnique );
					cltMsg clMsg1( GAMEMSG_RESPONSE_PARTY_DELUSER, sizeof( clGameMsgResponse_Party_DelUser ), (BYTE*)&clGameMsgResponse_Party_DelUser );
					pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg1 );

					return PARTY_USER_LEAVE;
				}
				break;
			}
		}

		pTemp = (CPartyObj *)pTemp->GetNext();
	}

	return -1;
}

/*
// 파티장이 파티원을 강제 퇴장 시킴
SI16 CPartyMgr::ForceLeaveParty( SI32 FromPersonID, SI32 ToPersonID, SI32 PartyIndex )
{
	if ( FromPersonID == ToPersonID )		// 자기 자신을 강퇴 시킬수 없음.
	{
		return -1;
	}

	CPartyObj *pTemp = (CPartyObj *)GetExistList()->GetFirst();
	
	while( pTemp )
	{
		// FromPersonID 가 파티장임을 확인
		if ( pTemp->IsSamePartyIndex( PartyIndex ) && pTemp->IsPartyLeader( FromPersonID ) )
		{
			SI16 ForcedLeaveUserListIndex = pTemp->ForceLeaveParty( ToPersonID );

			if (  ForcedLeaveUserListIndex < 0 )		// 없는 유저를 어찌 강퇴 시키리오.
			{
				return -1;
			}
			else
			{
				return ForcedLeaveUserListIndex;
			}
		}

		pTemp = (CPartyObj *)pTemp->GetNext();
	}
	
	return -1;	// 파티가 존재하지 않거나, 파티장이 아니므로 강퇴할수 없음.
}
*/


CPartyObj *CPartyMgr::GetPartyObj( SI32 PartyIndex )
{
	CPartyObj *pTemp = (CPartyObj *)GetExistList()->GetFirst();
	
	while( pTemp )
	{
		if ( pTemp->IsSamePartyIndex( PartyIndex ) )
		{
			return pTemp;
		}

		pTemp = (CPartyObj *)pTemp->GetNext();
	}

	return NULL;
}

// 파티번호에 FromPersondID 가 파티장인지 확인
bool CPartyMgr::IsPartyLeader( SI32 FromPersonID, SI32 PartyIndex )
{
	CPartyObj *pTemp = (CPartyObj *)GetExistList()->GetFirst();
	
	while( pTemp )
	{
		if ( pTemp->IsSamePartyIndex( PartyIndex ) && pTemp->IsPartyLeader( FromPersonID ) )
		{
			return true;
		}

		pTemp = (CPartyObj *)pTemp->GetNext();
	}

	return false;
}

bool CPartyMgr::IsPartyMember( SI32 siPersionID, SI32 siPartyIndex )
{
	CPartyObj* pclPartyObj = GetPartyObj( siPartyIndex );
	if ( NULL == pclPartyObj )
	{
		return false;
	}

	if ( 0 <= pclPartyObj->GetSamePersonIDIndex(siPersionID) )
	{
		return true;
	}

	return false;

}

bool CPartyMgr::IsEmptyUserListExist( SI32 PartyIndex )
{
	CPartyObj *pTemp = (CPartyObj *)GetExistList()->GetFirst();
	
	while( pTemp )
	{
		if ( pTemp->IsSamePartyIndex( PartyIndex ) && pTemp->IsEmptyUserListExist() )
		{
			return true;
		}

		pTemp = (CPartyObj *)pTemp->GetNext();
	}

	return false;
}

void CPartyMgr::DeleteParty( CPartyObj *pObj )
{
	pObj->Initialize();
	GetExistList()->Pop( pObj );
	PushBackToEmptyList( pObj );
	
	return;
}

UI32 CPartyMgr::GetPartyNum()
{
	return GetExistList()->GetObjNum();
}

UI32 CPartyMgr::GetPartyTotalUserNum()
{
	UI32 TotalPartyUserNum = 0;

	CPartyObj *pTemp = (CPartyObj *)GetExistList()->GetFirst();
	
	while( pTemp )
	{
		TotalPartyUserNum += pTemp->GetPartyUserNum();

		pTemp = (CPartyObj *)pTemp->GetNext();
	}

	return TotalPartyUserNum;
}

void CPartyMgr::IncreaseExp( SI32 PartyIndex, SI32 PersonID, SI32 Exp )
{
	CPartyObj *pTemp = GetPartyObj( PartyIndex );
	
	if ( pTemp == NULL )
	{
		return;
	}

	pTemp->IncreaseExp( PersonID, Exp );

	return;
}

void CPartyMgr::InPartyDungeonDeleteParty( SI32 siCharID )
{
	if ( pclClient->pclCM->IsValidID( siCharID ) == false )
	{
		return;
	}

	cltCharServer *pclChar = (cltCharServer*)pclClient->pclCM->CR[ siCharID ];
	if ( pclChar == NULL )
	{
		return;
	}
	
	SI32 siMapIndex	= pclChar->GetMapIndex();

	if ( pclClient->pclMapManager->IsPartyMap( siMapIndex ) == true )
	{
		if ( pclClient->pclCM->IsAlive( siCharID ) == FALSE )
		{
			pclChar->siKilledMode = 0;
		}	
		else
		{
			cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);
			pclChar->SetWarpInfo(&clWarpInfo, 56);
			pclChar->Warp();
		}
	}
	
}

SI32 CPartyMgr::CountQuestPalyUserNum( SI32 siPartyIndex, SI32 siClearCondition )
{
	CPartyObj *pclPartyObj = GetPartyObj( siPartyIndex );
	if ( NULL == pclPartyObj )
	{
		return 0;
	}

	SI32			siReturnCount	= 0;
	cltCharCommon*	pclchar			= NULL;
	stPartyUser*	pstUser			= NULL;
	
	for ( SI32 siIndex=0; siIndex<MAX_PARTY_USER_NUM; siIndex++ )
	{
		pstUser = pclPartyObj->GetPartyUser( siIndex );
		if ( NULL == pstUser )
		{
			continue;
		}

		if ( (false == pstUser->bPartyStatus) || (0 >= pstUser->siCharUnique) )
		{
			continue;
		}

		pclchar = pclClient->pclCM->GetChar( pstUser->siCharUnique );
		if ( NULL == pclchar )
		{
			continue;
		}

		if ( true == pclchar->pclCI->clQuestInfo.IsQuestGoing(siClearCondition) )
		{
			siReturnCount++;
		}
	}

	return siReturnCount;

}