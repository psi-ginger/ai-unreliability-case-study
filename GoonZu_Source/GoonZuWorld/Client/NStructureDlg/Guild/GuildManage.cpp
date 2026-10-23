#include "GuildDlg.h"

#include "Char/CharManager/CharManager.h"

#include "InterfaceMgr/InterfaceMgr.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NGuildManageDlg::NGuildManageDlg()
{
	m_siSelectListIndex = -1 ;
	m_siSelectGuildUnique = 0 ;
}

NGuildManageDlg::~NGuildManageDlg()
{

}

void NGuildManageDlg::InitDlg()
{
	SetControlMap( LIST_APPLICATION, TEXT("listbox_application"));
	SetControlMap( BUTTON_ACCEPT, TEXT("button_accept") );
	SetControlMap( BUTTON_REJECT, TEXT("button_reject") );

	// 리스트 
	CList *pListApply = (CList*)m_pControlMap[ LIST_APPLICATION ];

	pListApply->SetBorder(true);
	pListApply->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(1695);
	pListApply->SetColumn( 0, 25, pText );		// 주소
	pText = GetTxtFromMgr(6548);
	pListApply->SetColumn( 1, 125, pText );		// 길드명
	pText = GetTxtFromMgr(6549);
	pListApply->SetColumn( 2, 90, pText );		// 길드마스터
	//pText = GetTxtFromMgr( 1 );
	pListApply->SetColumn( 3, 86, TEXT("") );		// 시간

	pListApply->Refresh();
}
void NGuildManageDlg::Action()
{
}
void NGuildManageDlg::NDlgTabProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;
	cltGuildManager *pGuildManager = ((cltGuildManager *)m_pStrManager);

	CList *pList = (CList*)m_pControlMap[ LIST_APPLICATION ];

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( pclchar == NULL )
		return ;

	CASE_CONTROL( LIST_APPLICATION )
	{
		if ( nEvent == EVENT_LISTBOX_SELECTION )
		{
			SI16 siRow = -1, siCol = -1;
			pList->GetSelectedIndex(&siRow,&siCol);
			if( siRow == -1 || siCol == -1 )
				return ;

			m_siSelectListIndex = siRow ;
			stListBoxItemData* pBox = pList->GetItemData(m_siSelectListIndex);
			if ( pBox == NULL )
				return ;

			m_siSelectGuildUnique = 0;
			for ( SI32 i = 0 ; i < MAX_GUILD_PER_VILLAGE ; i ++ )
			{
				if ( !strcmp(pBox->strText[1],pGuildManager->clUnit[i].szName) )
				{
					m_siSelectGuildUnique = pGuildManager->clUnit[i].siUnique ;
					break;
				}
			}
		}
	}
	CASE_CONTROL( BUTTON_ACCEPT ) // 생성 승인
	{
		if ( nEvent == EVENT_BUTTON_CLICKED )
		{
			if ( pclchar->pclMap->siVillageUnique == pclchar->pclCI->clCharRank.siVillageUnique &&
				 pclchar->pclCI->clCharRank.siType == RANKTYPE_GUILD )
			{
				TCHAR buffer[256] = "" ;

				if ( m_siSelectGuildUnique <= 0 && m_siSelectListIndex < 0 )
					return ;

				TCHAR guildname[13] = "" ;
				stListBoxItemData* pBox = pList->GetItemData(m_siSelectListIndex);
				if ( pBox == NULL )
					return ;

				MStrCpy(guildname,pBox->strText[1],13);

				TCHAR * pTitle = GetTxtFromMgr(6536);
				TCHAR * pText = GetTxtFromMgr(6556);
				StringCchPrintf(buffer, 256, pText,guildname);

				stMsgBox MsgBox;
				// 현재 dialog가 탭 창 같은 Child Dialog일때
				CControllerMgr *pParent = NULL;
				pParent = this->GetParentDialog();
				if(pParent == NULL ) return;
				MsgBox.Set( pParent, pTitle,buffer, MSGBOX_TYPE_YESNO, 1 );

				BYTE TempBuffer[ 128 ];
				SI16 Size = sizeof( m_siSelectGuildUnique );
				memcpy( TempBuffer, &m_siSelectGuildUnique, sizeof( m_siSelectGuildUnique ) );
				pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
			}
		}
	}
	CASE_CONTROL( BUTTON_REJECT ) // 생성 최소
	{
		if ( nEvent == EVENT_BUTTON_CLICKED )
		{
			if ( pclchar->pclMap->siVillageUnique == pclchar->pclCI->clCharRank.siVillageUnique &&
				pclchar->pclCI->clCharRank.siType == RANKTYPE_GUILD )
			{
				TCHAR buffer[256] = "" ;

				if ( m_siSelectGuildUnique <= 0 && m_siSelectListIndex < 0 )
					return ;

				TCHAR guildname[13] = "" ;
				stListBoxItemData* pBox = pList->GetItemData(m_siSelectListIndex);
				if ( pBox == NULL )
					return ;

				MStrCpy(guildname,pBox->strText[1],13);

				TCHAR * pTitle = GetTxtFromMgr(6537);
				TCHAR * pText = GetTxtFromMgr(6557);
				StringCchPrintf(buffer, 256, pText,guildname);


				stMsgBox MsgBox;
				// 현재 dialog가 탭 창 같은 Child Dialog일때
				CControllerMgr *pParent = NULL;
				pParent = this->GetParentDialog();
				if(pParent == NULL ) return;
				MsgBox.Set( pParent, pTitle,buffer, MSGBOX_TYPE_YESNO, 2 );

				BYTE TempBuffer[ 128 ];
				SI16 Size = sizeof( m_siSelectGuildUnique );
				memcpy( TempBuffer, &m_siSelectGuildUnique, sizeof( m_siSelectGuildUnique ) );
				pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
			}
		}
	}
}

void NGuildManageDlg::RefreshList()
{
	cltGuildManager *pGuildManager = ((cltGuildManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_APPLICATION ];
	stListBoxItemData itemdata;

	SI32 i;
	TCHAR buffer[256];

	// 리스트뷰를 모두 지운다. 
	pList->Clear();

	for( i = 0; i < MAX_GUILD_PER_VILLAGE; i++ )
	{
		itemdata.Init();

		StringCchPrintf(buffer, 256, TEXT("%d"), i+1);

		itemdata.siParam[0] = i;
		itemdata.Set( 0, buffer );

		SI32 currentVillageUnique = pclClient->pclCM->CR[1]->pclMap->siVillageUnique; 
		bool isNewGuild = pGuildManager->clUnit[i].uiConfirm == GUILDSTATUS_STANDING;
		bool isChangeGuild = ( pGuildManager->clUnit[i].uiConfirm == GUILDSTATUS_CHANGEVILLAGE ) 
							&& ( pGuildManager->clUnit[i].siMoveVillageUnique == currentVillageUnique );

		//KHY - 0705 - 기존 길드명 표시.
		if( (pGuildManager->clUnit[i].siUnique != 0) )
		{
			itemdata.Set( 1, (TCHAR*)pGuildManager->clUnit[i].szName );
			itemdata.Set( 2, (TCHAR*)pGuildManager->clUnit[i].clMember[0].szCharName );	

			if (isChangeGuild )
			{
				TCHAR* pText = GetTxtFromMgr(6674);
				itemdata.Set( 3, pText );
			}
			else if( pGuildManager->clUnit[i].uiConfirm == GUILDSTATUS_STANDING)
			{
				TCHAR* pText = GetTxtFromMgr(6675);
				itemdata.Set( 3, pText );
			}
		}
		else
		{
			TCHAR imsibuffer[128] = "" ;
						
			itemdata.Set( 0, imsibuffer );
			itemdata.Set( 1, imsibuffer );
			itemdata.Set( 2, imsibuffer );
			itemdata.Set( 3, imsibuffer );
			//TCHAR* pText = GetTxtFromMgr(6561);
			//itemdata.Set( 1, pText );
		}

		pList->AddItem( &itemdata );
	}

	pList->Refresh();
}
