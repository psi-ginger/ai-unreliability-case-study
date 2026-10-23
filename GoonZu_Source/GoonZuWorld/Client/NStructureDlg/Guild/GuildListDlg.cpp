#include "GuildDlg.h"

#include "../../InterfaceMgr/Interface/List.h"

#include "Char/CharManager/CharManager.h"
#include "../../../Server/Rank/Rank.h"

#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/MsgType-Structure.h"

#include "../../../Lib/JYLibFunction.h"
#include "../../../Client/Music/Music.h"
#include "../../../CommonLogic/MsgType-Guild.h"

#include "../../../Client/Client.h"

#include "InterfaceMgr/InterfaceMgr.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NGuildListDlg::NGuildListDlg()
{
	siSelectedListIndex = -1 ;
	siSelectedGuildUnique = 0 ;

	for ( SI32 i = 0 ; i < MAX_GUILD_PER_VILLAGE ; i ++ )
		siMemberNum[i] = 0 ;
}

NGuildListDlg::~NGuildListDlg()
{

}

void NGuildListDlg::InitDlg()
{
	SetControlMap( LIST_GUILDLIST, TEXT("listbox_guildstrlist") );
	SetControlMap( BUTTON_GUILDENTER, TEXT("button_enter"));
	SetControlMap( BUTTON_GUILDMAKE, TEXT("button_guildmake"));
	SetControlMap( BUTTON_GUILDJOIN, TEXT("button_guildjoin"));

	// 리스트 
	CList *pList = (CList*)m_pControlMap[ LIST_GUILDLIST ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(6548);
	pList->SetColumn( 0, 90, pText );		// 길드명
	pText = GetTxtFromMgr(6549);
	pList->SetColumn( 1, 130, pText );		// 길드마스터
	pText = GetTxtFromMgr( 6550 );
	pList->SetColumn( 2, 30, pText );		// 길드 레벨
	pText = GetTxtFromMgr( 6551 );
	pList->SetColumn( 3, 65, pText );		// 길드원수
	pList->SetColumn( 4, 15, TEXT("") );
	// 리스트의 칸의 폭이 맞지 않아 폭 수정.[2007.07.05]
	pList->Refresh();

	((CButton*)m_pControlMap[BUTTON_GUILDMAKE])->Enable( false );
	((CButton*)m_pControlMap[BUTTON_GUILDJOIN])->Enable( false );

	cltGameMsgRequest_Guild_GetGuildList cllist(pclClient->pclCM->CR[1]->pclMap->siVillageUnique);
	cltMsg clMsg(GAMEMSG_REQUEST_GUILD_GETGUILDLIST, sizeof(cllist), (BYTE*)&cllist);
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
}

void NGuildListDlg::Action()
{
	cltGuildManager *pGuildManager = ((cltGuildManager *)m_pStrManager);
}

void NGuildListDlg::ShowGuildUnitList()
{
	cltGuildManager *pGuildManager = ((cltGuildManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_GUILDLIST ];
	stListBoxItemData itemdata;

	SI32 i;
	TCHAR buffer[256];

	// 리스트뷰를 모두 지운다. 
	pList->Clear();

	for( i = 0; i < MAX_GUILD_PER_VILLAGE; i++ )
	{
		itemdata.Init();

		StringCchPrintf(buffer, 256, TEXT("%d"), i+1);


		if( pGuildManager->clUnit[i].siUnique != 0 )
		{
			itemdata.Set( 0, (TCHAR*)pGuildManager->clUnit[i].szName );
			itemdata.Set( 1, (TCHAR*)pGuildManager->clUnit[i].clMember[0].szCharName );
			StringCchPrintf(buffer, 256, TEXT("%d"),pGuildManager->clUnit[i].clGPoint.siLevel);
			itemdata.Set( 2, buffer );

			// 길드원/최대인원
			// 길드가 승인 대기중일때는 "승인대기" 표시
			if( pGuildManager->clUnit[i].uiConfirm == GUILDSTATUS_NORMAL )
			{
				StringCchPrintf(buffer, 256, TEXT("%d/%d"),siMemberNum[i],pGuildManager->clUnit[i].GetLimitMemberNum());
			}
			else if ( pGuildManager->clUnit[i].uiConfirm == GUILDSTATUS_CHANGEVILLAGE && 
				pGuildManager->clUnit[i].siVillageUnique == pclClient->pclCM->CR[1]->pclMap->siVillageUnique )
			{
				StringCchPrintf(buffer, 256, TEXT("%d/%d"),siMemberNum[i],pGuildManager->clUnit[i].GetLimitMemberNum());
			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(6602);
				StringCchPrintf(buffer, 256, pText);
			}
			itemdata.Set( 3, buffer );
		}
		else
		{
			//빈길드는 아예 표시하지 않는다..
			//중간에 빈길드라고 생기는 경우도 없어지겠지..
			//TCHAR* pText = GetTxtFromMgr(6561);
			//itemdata.Set( 0, pText );
		}

		pList->AddItem( &itemdata );
	}

	pList->Refresh();
}

void NGuildListDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	cltGuildManager *pGuildManager = ((cltGuildManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_GUILDLIST ];

	CASE_CONTROL( LIST_GUILDLIST )
	{
		if ( nEvent == EVENT_LISTBOX_SELECTION )
		{
			((CButton*)m_pControlMap[BUTTON_GUILDENTER])->Enable(false);

			CList *pList = (CList*)m_pControlMap[ LIST_GUILDLIST ];

			SI16 siRow = -1, siCol = -1;

			pList->GetSelectedIndex(&siRow,&siCol);
			if( siRow == -1 || siCol == -1 )
				return;

			siSelectedListIndex = siRow ;
			stListBoxItemData* pBox = pList->GetItemData(siSelectedListIndex);
			if ( pBox == NULL )
				return ;

			bool bFindGuildUnique = false;
			
			for ( SI32 i = 0 ; i < MAX_GUILD_PER_VILLAGE ; i ++ )
			{
				if ( _tcscmp(pBox->strText[0], pGuildManager->clUnit[i].szName) == 0)
				{
					siSelectedGuildUnique = pGuildManager->clUnit[i].siUnique ;
					bFindGuildUnique = true;

					bool isMyGuild = pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex == siSelectedGuildUnique;
					bool isRegulerMember = pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siType != GUILDRANKTYPE_STANDING;

					if ( isMyGuild && isRegulerMember )
					{
						if ( pGuildManager->clUnit[i].uiConfirm == GUILDSTATUS_NORMAL )
						{
							((CButton*)m_pControlMap[BUTTON_GUILDENTER])->Enable(true);
						}
						else if ( pGuildManager->clUnit[i].uiConfirm == GUILDSTATUS_CHANGEVILLAGE
							&& pGuildManager->clUnit[i].siVillageUnique == pclClient->pclCM->CR[1]->pclMap->siVillageUnique )
						{
							((CButton*)m_pControlMap[BUTTON_GUILDENTER])->Enable( true );
						}

					}
					break;
				}
			}
			// 찾지 못했을경우 다시 선택된 길드 유니크 0으로!!
			if (bFindGuildUnique)
			{
				// 기입 승인대기중인 길드는 입장할수 없다.
				if (pGuildManager->clUnit[i].uiConfirm == GUILDSTATUS_NORMAL )
                    ((CButton*)m_pControlMap[BUTTON_GUILDMAKE])->Enable( true );
				else
					((CButton*)m_pControlMap[BUTTON_GUILDMAKE])->Enable( false );

				((CButton*)m_pControlMap[BUTTON_GUILDJOIN])->Enable( true );
			}
			else
			{
				siSelectedGuildUnique = 0;
				((CButton*)m_pControlMap[BUTTON_GUILDMAKE])->Enable( false );
				((CButton*)m_pControlMap[BUTTON_GUILDJOIN])->Enable( false );
			}
		}
	}
	CASE_CONTROL( BUTTON_GUILDENTER )
	{
		if ( nEvent == EVENT_BUTTON_CLICKED )
		{
			if ( siSelectedGuildUnique <= 0 )
				return ;

			if ( siSelectedListIndex < 0 )
				return ;

			if ( siSelectedGuildUnique != pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex )
				return ;

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siType == GUILDRANKTYPE_STANDING )
				return ;

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.uiConfirm == GUILDSTATUS_STANDING )
				return ;

			cltGameMsgRequest_Guild_GetGuildInfo clInfo(siSelectedGuildUnique);
			cltMsg clMsg(GAMEMSG_REQUEST_GUILD_GETGUILDINFO, sizeof(clInfo), (BYTE*)&clInfo);
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
	CASE_CONTROL( BUTTON_GUILDMAKE ) // 생성(Make) 아니다. 가입(Join)이다!
	{
		if ( nEvent == EVENT_BUTTON_CLICKED )
		{
			if ( siSelectedGuildUnique <= 0 || siSelectedListIndex < 0)
				return ;

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex != 0 )
			{

				TCHAR* pTitle = GetTxtFromMgr(6524);
				TCHAR* pText = GetTxtFromMgr(6601);

				pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pText);

				return ;
			}

			TCHAR buffer[256] = "" ;
			TCHAR guildname[13] = "" ;
			stListBoxItemData* pBox = pList->GetItemData(siSelectedListIndex);
			if ( pBox == NULL )
				return ;

			MStrCpy(guildname,pBox->strText[0],13);

			TCHAR * pTitle = GetTxtFromMgr(6524);
			TCHAR * pText = GetTxtFromMgr(6558);
			StringCchPrintf(buffer, 256, pText,guildname);

			stMsgBox MsgBox;
			// 현재 dialog가 탭 창 같은 Child Dialog일때
			CControllerMgr *pParent = NULL;
			pParent = this->GetParentDialog();
			if(pParent == NULL ) return;
			MsgBox.Set( pParent, pTitle,buffer, MSGBOX_TYPE_YESNO, 3 );

			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof( siSelectedGuildUnique );
			memcpy( TempBuffer, &siSelectedGuildUnique, sizeof( siSelectedGuildUnique ) );
			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
		}
	}
	CASE_CONTROL( BUTTON_GUILDJOIN ) // 가입(Join) 아니다. 탈퇴(Leave)다!
	{
		if ( nEvent == EVENT_BUTTON_CLICKED )
		{
			if ( pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex <= 0 )
				return ;

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siType == GUILDRANKTYPE_MASTER )
			{
				TCHAR * pTitle = GetTxtFromMgr(6525);
				TCHAR * pText = GetTxtFromMgr(6560);
				pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
				return ;
			}

			TCHAR buffer[256] = "" ;
			TCHAR guildname[13] = "" ;
			MStrCpy(guildname,pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.szGuildName,13);

			TCHAR * pTitle = GetTxtFromMgr(6525);
			TCHAR * pText = GetTxtFromMgr(6559);
			sprintf(buffer,pText,guildname);

			stMsgBox MsgBox;
			// 현재 dialog가 탭 창 같은 Child Dialog일때
			CControllerMgr *pParent = NULL;
			pParent = this->GetParentDialog();
			if(pParent == NULL ) return;
			MsgBox.Set( pParent, pTitle,buffer, MSGBOX_TYPE_YESNO, 4 );

			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof( pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex );
			memcpy( TempBuffer, &pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex, sizeof( pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex ) );
			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
		}
	}
}