//---------------------------------
// 2004/2/18 김태곤
//---------------------------------

#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-Friend.h"
#include "../NInterface/NFriendDlg/NFriendDlg.h"
#include "../NInterface/NFriendDlg/NNewFriendDlg.h"
#include "../NInterface/NFriendDlg/NFriendAddAcceptDlg.h"
#include "../ninterface/NFriendDlg/NFriendMemoDlg.h"
#include "../Interface/SoundOnOffDlg/SoundOnOffdlg.h"

#include "Char\CharManager\CharManager.h"


extern CSoundOnOffDlg g_SoundOnOffDlg;

void cltClient::DoMsg_GAMEMSG_RESPONSE_FRIENDALLLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_FriendAllList* pclInfo = (cltGameMsgResponse_FriendAllList*)pclMsg->cData;

	if(pclCM->CR[1]->pclFriendInfo == NULL)		return;

	pclCM->CR[1]->pclFriendInfo->Set(&pclInfo->clFriendInfo);

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FRIENDADD(cltMsg* pclMsg, SI32 id)
{
	// 관전 모드이면, 
	if(bWatchModeSwitch == true)return ;

	cltGameMsgResponse_FriendAdd* pclInfo = (cltGameMsgResponse_FriendAdd*)pclMsg->cData;
	
	if ( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ NFRIENDACCEPT_DLG ])
	{
		((CNFriendAddAcceptDlg*)m_pDialog[ NFRIENDACCEPT_DLG ])->PushAccpetList( pclInfo->siGroupID, pclInfo->siPersonID, pclInfo->siCharID, pclInfo->strFriendName );
	}
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FRIENDADDACCEPT(cltMsg* pclMsg, SI32 id)
{
	// 관전 모드이면, 
	if(bWatchModeSwitch == true)return ;

	cltGameMsgResponse_FriendAddAccept* pclInfo = (cltGameMsgResponse_FriendAddAccept*)pclMsg->cData;
	if ( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ NNEWFRIEND_DLG ] )
	{
		((CNNewFriendDlg*)m_pDialog[ NNEWFRIEND_DLG ])->FriendAddList(pclInfo->siFriendAddListIndex, pclInfo->siGroupID, pclInfo->siPersonID, pclInfo->siCharID, pclInfo->strCharName ,pclInfo->szAccountID);
	}
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FRIENDDEL(cltMsg* pclMsg, SI32 id)
{
	// 관전 모드이면, 
	if(bWatchModeSwitch == true)return ;

	cltGameMsgResponse_FriendDel* pclInfo = (cltGameMsgResponse_FriendDel*)pclMsg->cData;
	
	if ( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ NNEWFRIEND_DLG ])
	{
		((CNNewFriendDlg*)m_pDialog[ NNEWFRIEND_DLG ])->FriendDelList( pclInfo->siFriendListIndex, pclInfo->siPersonID );
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FRIENDLOGIN(cltMsg* pclMsg, SI32 id)
{
	// 관전 모드이면, 
	if(bWatchModeSwitch == true)return ;

	cltGameMsgResponse_FriendLogin* pclInfo = (cltGameMsgResponse_FriendLogin*)pclMsg->cData;

	if ( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ NNEWFRIEND_DLG ])
	{
		((CNNewFriendDlg*)m_pDialog[ NNEWFRIEND_DLG ])->FriendLogin( pclInfo->siGroupID, pclInfo->siPersonID, pclInfo->siCharID,pclInfo->szAccountID );
	}
	
	PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FRIEND_LOGIN")), 0, 0 );
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FRIENDLOGOUT(cltMsg* pclMsg, SI32 id)
{
	// 관전 모드이면, 
	if(bWatchModeSwitch == true)return ;

	cltGameMsgResponse_FriendLogOut* pclInfo = (cltGameMsgResponse_FriendLogOut*)pclMsg->cData;
	if ( g_SoundOnOffDlg.m_bFullScreenMode && m_pDialog[ NNEWFRIEND_DLG ])
	{
		((CNNewFriendDlg*)m_pDialog[ NNEWFRIEND_DLG ])->FriendLogOut( pclInfo->siPersonID );
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FRIENDLIST(cltMsg* pclMsg, SI32 id)
{
	// 친구의 이름등은 기존에 클라이언트가 가지고 있던것을 그대로 쓴다.
	// 현재 위치 , 레벨, 가입마을만 서버에서 받아온걸로 씀..

	cltGameMsgResponse_FriendList* pclInfo = (cltGameMsgResponse_FriendList*)pclMsg->cData;
	CNNewFriendDlg* pNewDialog = (CNNewFriendDlg*)m_pDialog[NNEWFRIEND_DLG];

	if(pclCM->CR[1]->pclFriendInfo == NULL)			return;

	for( SI32 siFriendCount = 0; siFriendCount < MAX_FRIEND_NUMBER; ++siFriendCount )
	{
		memcpy( &pclCM->CR[1]->pclFriendInfo->clFriend[siFriendCount], &pclInfo->clSimplePerson[siFriendCount], sizeof( cltSimplePerson ) );
	}
	
	stListBoxItemData tmpListBoxItemData;

	if(pNewDialog)	pNewDialog->m_pFriendList->Clear();
	
	SI16 siCount = 0;
	
	if(pNewDialog)	memcpy( pNewDialog->m_pFriendInfo, pclCM->CR[1]->pclFriendInfo, sizeof( cltFriendInfo ) );
	
	for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
	{
		cltSimplePerson *pSimplePersonInfo = &pclCM->CR[1]->pclFriendInfo->clFriend[i];
		
		tmpListBoxItemData.Init();

		if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
		{
			if(pNewDialog)	StringCchCopy(pNewDialog->m_szAccountID[i], MAX_PLAYER_NAME, pclCM->CR[1]->pclFriendInfo->szAccountID[i]);

			if ( pclCM->CR[1]->pclFriendInfo->bConnectStatus[ i ] )
			{
				tmpListBoxItemData.siSmallIconFileIndex[ 0 ] = 0;
				tmpListBoxItemData.siSmallIconFontIndex[ 0 ] = 12; // 로그온 이미지 폰트 번호
				tmpListBoxItemData.Set( 1, pclClient->pclMapManager->GetMapName(pclInfo->siMapIndex[i]));
				TCHAR buf[16];
				StringCchPrintf( buf, 16,"%d",  pclInfo->siLevel[i] );
				tmpListBoxItemData.Set( 2, buf);
				if ( pclInfo->siHomeMapIndex[i] > 0 )
				{
					tmpListBoxItemData.Set( 3, pclClient->pclMapManager->GetMapName(pclInfo->siHomeMapIndex[i]));
				}

				TCHAR* pText = NULL;
				if( pclInfo->siCharKind[i] == KIND_HERO3 || pclInfo->siCharKind[i] == KIND_HERO4 )
				{
					pText = GetTxtFromMgr(6882);
				}
				else
				{
					pText = GetTxtFromMgr(6883);
				}
				tmpListBoxItemData.Set( 4, pText );

				TCHAR rankBuffer[128] = {0,};
				if ( GetRankName( &pclInfo->clCharRank[i], rankBuffer, 128, pclInfo->uiGRank[i] ) == TRUE )
				{
					tmpListBoxItemData.Set( 5, rankBuffer );
				}
			}
			else
			{
				tmpListBoxItemData.siSmallIconFileIndex[ 0 ] = 0;
				tmpListBoxItemData.siSmallIconFontIndex[ 0 ] = 11; // 로그아웃 이미지 폰트 번호
			}

			TCHAR *Name = (TCHAR*)pSimplePersonInfo->GetName();
			if ( Name == NULL || _tcslen( Name ) < 1 )
			{
				continue;
			}

			// leon todo - Column 값이 1개에서 달라지면 아래 0을 고쳐야함
			tmpListBoxItemData.Set( 0, (TCHAR*)pSimplePersonInfo->GetName());
			siCount++;

			if(pNewDialog)
			{
				if(pNewDialog->m_siCurrentGroupID != FRIEND_GROUP_UNKNOWN)
				{
					if(pclCM->CR[1]->pclFriendInfo->siGroupID[i] == pNewDialog->m_siCurrentGroupID)
						pNewDialog->m_pFriendList->AddItem( &tmpListBoxItemData );
				}
				else
				{
					pNewDialog->m_pFriendList->AddItem( &tmpListBoxItemData );
				}
			}
		}
	}

	if(pNewDialog)	pNewDialog->m_pFriendList->Refresh();
	if(pNewDialog)	pNewDialog->SetGroupName();

	return;
}
void cltClient::DoMsg_GAMEMSG_RESPONSE_FRIENDMEMO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_FriendMemo* pclinfo = (cltGameMsgResponse_FriendMemo*)pclMsg->cData;

	cltClient* pclclient = (cltClient*)pclClient;

	pclclient->CreateInterface( NFRIENDMEMO_DLG );
	CNFriendMemoDlg* pDialog = (CNFriendMemoDlg*)pclclient->m_pDialog[NFRIENDMEMO_DLG];
	if ( pDialog )
	{
		pDialog->SetFriendMemo( pclinfo->szFriendMemo );
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FRIENDMEMOSET(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_FriendMemoSet* pclinfo = (cltGameMsgResponse_FriendMemoSet*)pclMsg->cData;

	TCHAR* pText = NULL;
	TCHAR* pTitle = GetTxtFromMgr(6778);

	if ( pclinfo->bSuccessSave )
	{
		pText = GetTxtFromMgr(6780);
	}
	else
	{
		pText = GetTxtFromMgr(6781);
	}


	((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FRIENDCHANGEGROUP(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_FriendChangeGroup* pclinfo = (cltGameMsgResponse_FriendChangeGroup*)pclMsg->cData;

	CNNewFriendDlg* pNewDialog = (CNNewFriendDlg*)m_pDialog[NNEWFRIEND_DLG];

	if(	pclinfo->siFriendListIndex < 0 ||
		pclinfo->siFriendListIndex >= MAX_FRIEND_NUMBER)		return;

	if(	pclinfo->siGroupID < 0 ||
		pclinfo->siGroupID > MAX_FRIEND_GROUP)					return;

	if( pclCM->CR[1]->pclFriendInfo == NULL )					return;

	cltSimplePerson *pSimplePersonInfo = &pclCM->CR[1]->pclFriendInfo->clFriend[pclinfo->siFriendListIndex];

	if( pSimplePersonInfo != NULL && 
		pSimplePersonInfo->GetPersonID() > 0 && pSimplePersonInfo->GetPersonID() == pclinfo->siFriendPersonID )
	{
		pclCM->CR[1]->pclFriendInfo->siGroupID[pclinfo->siFriendListIndex] = pclinfo->siGroupID;
		if(pNewDialog)		pNewDialog->LoadFriendList();
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FRIENDCHANGEGROUPNAME(cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_FriendChangeGroupName* pclinfo = (cltGameMsgResponse_FriendChangeGroupName*)pclMsg->cData;

	CNNewFriendDlg* pNewDialog = (CNNewFriendDlg*)m_pDialog[NNEWFRIEND_DLG];

	if( pclCM->CR[1]->pclFriendInfo == NULL )				return;

	if(pclinfo->bSuccessChange)
	{
		for(int i=0; i<MAX_FRIEND_GROUP; i++)
		{
			MStrCpy(pclCM->CR[1]->pclFriendInfo->szGroupName[i], pclinfo->szGroupName[i], MAX_FRIEND_GROUPNAME);
		}
		if(pNewDialog)		pNewDialog->LoadFriendList();
	}
}