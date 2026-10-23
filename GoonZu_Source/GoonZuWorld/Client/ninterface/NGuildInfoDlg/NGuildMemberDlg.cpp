#include "../../Client.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "NGuildMemberDlg.h"
#include "../../common/Char/CharManager/CharManager.h"

#include "MsgType-Guild.h"
#include "Msg/MsgType-Web.h"
#include "MSG/MsgType-ItemMall.h"

CNGuildMemberDlg::CNGuildMemberDlg()
{
	Initialize();
}	

CNGuildMemberDlg::~CNGuildMemberDlg()
{
	Destroy();
}

void CNGuildMemberDlg::Initialize()
{
	for( SI16 i=0; i < MAX_GUILD_MEMBER_NUM ; i++ )
	{
		m_siMemberPersonID[i] = 0;
		m_bConnected[i] = FALSE;
	}
	m_RefreshClock =0;
}

void CNGuildMemberDlg::Destroy()
{

}

void CNGuildMemberDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NGuildInfo/DLG_NGuildMember.ddf"));

	file.CreatePopupDialog( this, NGUILDMEMBER_DLG, TEXT("dialog_guildmember"),StaticNGuildMemberDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NGUILDMEMBER_STATIC_GUILDNAME, this)  , NGUILDMEMBER_STATIC_GUILDNAME, TEXT("statictext_guildname"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST , NGUILDMEMBER_LIST_MEMBER, this)  , NGUILDMEMBER_LIST_MEMBER , TEXT("listbox_memberlist"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGUILDMEMBER_BUTTON_GUILDINFO, this)  , NGUILDMEMBER_BUTTON_GUILDINFO , TEXT("button_guildinfo"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGUILDMEMBER_BUTTON_WHISPER, this)  , NGUILDMEMBER_BUTTON_WHISPER , TEXT("button_whisper"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGUILDMEMBER_BUTTON_WARP, this)  , NGUILDMEMBER_BUTTON_WARP , TEXT("button_warp"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGUILDMEMBER_BUTTON_KOMJI, this)  , NGUILDMEMBER_BUTTON_KOMJI , TEXT("button_komji"));

	// 길드명 표시
	TCHAR* pText = GetTxtFromMgr(6745);
	TCHAR buffer[256];
	StringCchPrintf( buffer, 256, pText, pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.szGuildName );
	(m_InterfaceFactory.GetStatic(NGUILDMEMBER_STATIC_GUILDNAME))->SetText( buffer, DT_LEFT | DT_VCENTER | DT_SINGLELINE );

	CList* pList = m_InterfaceFactory.GetList(NGUILDMEMBER_LIST_MEMBER);


	pList->SetBorder(true);
	pList->SetBKColorUse(true);


	pText = GetTxtFromMgr(3299);
	pList->SetColumn( 0, 160, pText );		// 이름
	pText = GetTxtFromMgr(1611);
	pList->SetColumn( 1, 150, pText );		// 현재 위치

	pList->Refresh();

	cltMsg clMsg(GAMEMSG_REQUEST_GUILD_MEMBERLIST,0 );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

	m_RefreshClock = pclClient->CurrentClock;

	m_InterfaceFactory.GetButton(NGUILDMEMBER_BUTTON_WARP)->Enable( false );


	//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
	if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
	{
		m_InterfaceFactory.GetButton(NGUILDMEMBER_BUTTON_KOMJI)->Show(false);
	}
}

void CNGuildMemberDlg::Hide()
{
	CControllerMgr::Show(false);

}

void CNGuildMemberDlg::Show()
{
	CControllerMgr::Show(true);

}

void CALLBACK CNGuildMemberDlg::StaticNGuildMemberDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNGuildMemberDlg *pThis = (CNGuildMemberDlg*) pControllerMgr;
	pThis->NGuildMemberDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNGuildMemberDlg::NGuildMemberDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				DeleteDialog();
			}
		}
		break;
	case NGUILDMEMBER_BUTTON_GUILDINFO:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				pclclient->CreateInterface(NGUILDINFO_DLG);
			}
		}
		break;
	case NGUILDMEMBER_BUTTON_WHISPER:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				SI32 selectedIndex = GetSelectedListIndex();

				if ( selectedIndex < 0 || selectedIndex >= MAX_GUILD_MEMBER_NUM ) return;

				SI32 personID = m_siMemberPersonID[selectedIndex];
				if ( personID )
				{
					pclclient->RequestSecretChatByPersonID(personID);
				}
			}
		}
		break;
	case NGUILDMEMBER_BUTTON_WARP:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				WarpToGuildMember();
			}
		}
		break;
	case NGUILDMEMBER_BUTTON_KOMJI:
		{
			if (EVENT_BUTTON_CLICKED == nEvent )
			{
				SI32 selectedIndex = GetSelectedListIndex();
				if ( selectedIndex < 0 || selectedIndex >= MAX_GUILD_MEMBER_NUM ) return;

				if ( m_siMemberPersonID[selectedIndex] > 0)
				{
					CList* pList = m_InterfaceFactory.GetList(NGUILDMEMBER_LIST_MEMBER);

					TCHAR buffer[MAX_PLAYER_NAME] = TEXT("");
					StringCchCopy(buffer, MAX_PLAYER_NAME, pList->GetText( selectedIndex, 0 ) );
					cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( buffer );
					cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
					pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );
				}
			}
		}
	//  PCK - 07.10.24 : 길드원을 선택할 시에 순간이동 버튼을 활성화 시킨다.
	case NGUILDMEMBER_LIST_MEMBER:
		{
			switch (nEvent)
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI32 selectedIndex = GetSelectedListIndex();
					if ( selectedIndex < 0 || selectedIndex >= MAX_GUILD_MEMBER_NUM ) return;
					SI32 personID = m_siMemberPersonID[selectedIndex];
					
					// 접속되어 있는 길드원을 선택할때만 순간이동 버튼을 활성화 시킨다.	
					if(personID > 0)
					{
						if(m_bConnected[selectedIndex])
						{
							m_InterfaceFactory.GetButton(NGUILDMEMBER_BUTTON_WARP)->Enable( true );
						}
						else
						{
							m_InterfaceFactory.GetButton(NGUILDMEMBER_BUTTON_WARP)->Enable( false );
						}
					}
				}
				break;
			}
		}
		break;
	}
}

void CNGuildMemberDlg::Action()
{
	// 30초에 한번씩 새로 요청
	if ( TABS(pclClient->CurrentClock - m_RefreshClock ) > 30 * 1000 )
	{
		cltMsg clMsg(GAMEMSG_REQUEST_GUILD_MEMBERLIST,0 );
		pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

		m_RefreshClock = pclClient->CurrentClock;
	}
}

void CNGuildMemberDlg::SetMemberList(cltGameMsgResponse_Guild_MemeberList* pclinfo )
{
	CList*				pList = m_InterfaceFactory.GetList(NGUILDMEMBER_LIST_MEMBER);
	stListBoxItemData	itemData;

	for ( SI32 i=0; i < MAX_GUILD_MEMBER_NUM; i++ )
	{
		if ( pclinfo->siPersonID[i] == 0 ) break;

		itemData.Init();

		m_siMemberPersonID[i] = pclinfo->siPersonID[i];
		m_bConnected[i] = pclinfo->bConnected[i];

		itemData.siSmallIconFileIndex[ 0 ] = 0;
		if ( pclinfo->bConnected[i] )
		{
			itemData.siSmallIconFontIndex[ 0 ] = 12; // 로그온 이미지 폰트 번호
			itemData.Set( 1, pclClient->pclMapManager->GetMapName(pclinfo->siMapIndex[i]));
		}
		else
		{
			itemData.siSmallIconFontIndex[ 0 ] = 11;
		}

		itemData.Set( 0, pclinfo->szName[i] );

		pList->AddItem( &itemData );
	}

	pList->Refresh();
}
SI16 CNGuildMemberDlg::GetSelectedListIndex()
{
	CList*	pList = m_InterfaceFactory.GetList(NGUILDMEMBER_LIST_MEMBER);

	SI16 row = 0, col = 0;
	pList->GetSelectedIndex(&row, &col);

	return row;
}
void CNGuildMemberDlg::WarpToGuildMember()
{
	cltClient* pclclient = (cltClient*)pclClient;

	SI32 selectedIndex = GetSelectedListIndex();
	if ( selectedIndex < 0 || selectedIndex >= MAX_GUILD_MEMBER_NUM ) return;
	SI32 personID = m_siMemberPersonID[selectedIndex];

	if ( personID > 0 )
	{
		cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

		TCHAR TEMP[256];
		TCHAR* pTitle = NULL;
		TCHAR* pText = NULL;
		
		if( pclchar->CanFreeWarp( 0 ) == false )
		{
			// 순간이동이용권을 보유하고 있는지 확인한다. 
			SI32 itemunique = ITEMUNIQUE(24000) ;  // 순간이동이용권
			SI32 pos = pclchar->pclCI->clCharItem.FindItemInv(itemunique);
			if(pos < 0)
			{
				pTitle = GetTxtFromMgr(1413);
				pText = GetTxtFromMgr(2460);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

				return ;
			}
			pText = GetTxtFromMgr(2461);
			pTitle = GetTxtFromMgr(1418);
			StringCchPrintf(TEMP, 256, pText, m_InterfaceFactory.GetList(NGUILDMEMBER_LIST_MEMBER)->GetText(selectedIndex, 0));
		}
		else
		{
			pTitle = GetTxtFromMgr(3189);
			pText = GetTxtFromMgr(3191);
			StringCchPrintf(TEMP, 256, pText, m_InterfaceFactory.GetList(NGUILDMEMBER_LIST_MEMBER)->GetText(selectedIndex, 0));
		}

/*		if ( !pclchar->pclCI->clBI.bUseNoLimitTicketWarp &&
			pclclient->pclCM->CR[1]->pclCI->clBI.uiPoliceSwitch == false &&
			pclclient->pclCM->CR[1]->pclCI->clBI.uiAngelSwitch == false &&
			pclclient->pclCM->CR[1]->pclCI->clBI.bPCRoom == false &&
			pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_KING  &&
			pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER1 &&
			pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER2 &&
			pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER3 &&
			pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER4 &&
			pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER5 &&
			pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER6 &&
			( pclclient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER) &&  
			pclclient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FRIENDSHIP) ) == false
			) 
		{
			// 순간이동이용권을 보유하고 있는지 확인한다. 
			SI32 itemunique = ITEMUNIQUE(24000) ;  // 순간이동이용권
			SI32 pos = pclchar->pclCI->clCharItem.FindItemInv(itemunique);
			if(pos < 0)
			{
				pTitle = GetTxtFromMgr(1413);
				pText = GetTxtFromMgr(2460);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

				return ;
			}
			pText = GetTxtFromMgr(2461);
			pTitle = GetTxtFromMgr(1418);
			StringCchPrintf(TEMP, 256, pText, m_InterfaceFactory.GetList(NGUILDMEMBER_LIST_MEMBER)->GetText(selectedIndex, 0));
		}
		// 수정 - PCK (07.10.25) : 순간이동정액권 사용시에 메시지가 다르게 출력하도록 추가함.
		else
		{
			pTitle = GetTxtFromMgr(3189);
			pText = GetTxtFromMgr(3191);
			StringCchPrintf(TEMP, 256, pText, m_InterfaceFactory.GetList(NGUILDMEMBER_LIST_MEMBER)->GetText(selectedIndex, 0));
		}
*/
		// 감옥에서는 워프 불가 
		if( pclchar->clInnerStatus.IsStatus(INNERSTATUS_PRISON))	
		{
			pTitle = GetTxtFromMgr(1413);
			pText = GetTxtFromMgr(2259);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
			return  ;
		}	

		// 영자나라에 있으면 불가능
		if ( pclchar->GetMapIndex() == MAPINDEX_MASTERWORLD &&
			pclchar->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
		{
			return ;
		}

		//// 목표 마을이공성중이면 워프 불가능 
		//SI16 personvillageunique = pclClient->pclCM->CR[personID]->pclCI->clBI->siHomeVillage; // 목표 케릭터의 마을 유니크
		//if (pclClient->pclVillageManager->IsVillageAtWar() == true) {
		//	return;
		//}


		stMsgBox MsgBox;
		MsgBox.Set( this, pTitle, TEMP, MSGBOX_TYPE_YESNO, 0 );
		BYTE TempBuffer[ 128 ];
		SI16 Size = sizeof( personID );
		memcpy( TempBuffer, &personID, Size );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
	}

}
void CNGuildMemberDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 친구로 이동
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 personID = *((SI32*)pData);
				cltGameMsgRequest_FriendWarp clFriendWarp( personID, FRIENDWARP_MODE_GUILD );
				cltMsg clMsg( GAMEMSG_REQUEST_FRIENDWARP, sizeof( clFriendWarp ), (BYTE*)&clFriendWarp );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}