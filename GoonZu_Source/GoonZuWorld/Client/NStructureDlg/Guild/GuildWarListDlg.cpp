#include "GuildDlg.h"

#include "InterfaceMgr/InterfaceMgr.h"
#include "../../../CommonLogic/MsgType-Guild.h"
#include "Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;

NGuildWarListDlg::NGuildWarListDlg()
{
	m_siFieldIndex = 0;
	for ( SI32 i=0; i< GUILDWAR_ROOMNUM ; i++ )
		m_clUnit[i].Init();

}

NGuildWarListDlg::~NGuildWarListDlg()
{

}

void NGuildWarListDlg::InitDlg()
{
	SetControlMap( COMBO_ROOM, TEXT("combobox_roomindex"));
	SetControlMap( LIST_WARLIST, TEXT("listbox_warlist"));
	SetControlMap( BUTTON_RECOMMAND, TEXT("button_recommand") );
	SetControlMap( EDIT_ENEMYGUILD, TEXT("editbox_enemyname") );
	SetControlMap( BUTTON_APPLY, TEXT("button_apply"));

	// 리스트 
	CList *pListApply = (CList*)m_pControlMap[ LIST_WARLIST ];

	pListApply->Clear();
	pListApply->SetBorder(true);
	pListApply->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(1641);
	pListApply->SetColumn( 0, 40, pText );		// 번호
	pText = GetTxtFromMgr(6684);
	pListApply->SetColumn( 1, 120, pText );		// 시간
	pText = GetTxtFromMgr(6685);
	pListApply->SetColumn( 2, 90, pText );		// 길드1
	pText = GetTxtFromMgr( 6686 );
	pListApply->SetColumn( 3, 90, pText);		// 길드2

	pListApply->Refresh();

	// 콤보박스 아이템 추가
	CComboBox* pCombo = (CComboBox*)m_pControlMap[ COMBO_ROOM ];
	pCombo->Clear();
	stComboBoxItemData itemData;

	if(pclClient->IsCountrySwitch( Switch_NewGuildHuntMapWar ) ) //바꼈으면
	{
		for ( SI16 i=0; i< GUILDWAR_FIELDNUM_OLD ; i++ )
		{
			if(i >= GUILDWAR_FIELDNUM_OLD) continue	;
			itemData.Init();
			pText = GetTxtFromMgr( 6696 + i);
			itemData.Set( pText );			
			pCombo->AddItem( &itemData );
		}
	}
	else// 안바꼈으면
	{
		for ( SI16 i=0; i< GUILDWAR_FIELDNUM ; i++ )
		{
			if(i >= GUILDWAR_FIELDNUM) continue	;
			itemData.Init();
			pText = GetTxtFromMgr( 6696 + i);
			itemData.Set( pText );			
			pCombo->AddItem( &itemData );
		}

	}
	pCombo->Refresh();
	pCombo->SetCursel(0);
	m_siFieldIndex = pCombo->GetSelectedIndex();
	

	cltGameMsgRequest_GuildWar_ReserveList clinfo(m_siFieldIndex);
	cltMsg clMsg(GAMEMSG_REQUEST_GUILDWAR_RESERVELIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}
void NGuildWarListDlg::Action()
{
}
void NGuildWarListDlg::NDlgTabProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;
	cltGuildManager *pGuildManager = ((cltGuildManager *)m_pStrManager);

	CList *pList = (CList*)m_pControlMap[ LIST_WARLIST ];

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
	if ( pclchar == NULL )
		return ;

	CASE_CONTROL( BUTTON_APPLY  )
	{
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			CList *pList = (CList*)m_pControlMap[ LIST_WARLIST ];

			SI16 siRow = -1, siCol = -1;

			pList->GetSelectedIndex(&siRow,&siCol);
			if( siRow == -1 || siCol == -1 )
				return;

			CEdit* pEdit = (CEdit*)m_pControlMap[ EDIT_ENEMYGUILD ];

			if ( !pEdit->GetText() || _tcscmp( pEdit->GetText(), TEXT("") ) == 0 )
			{
				TCHAR* pText = GetTxtFromMgr(6690);
				TCHAR* pTitle = GetTxtFromMgr(6694);

				((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return;
			}
			if ( pList->GetText( siRow, 2 ) && _tcscmp( pList->GetText( siRow, 2 ), TEXT("") )!= 0 )
			{
				TCHAR* pText = GetTxtFromMgr(6690);
				TCHAR* pTitle = GetTxtFromMgr(6694);

				((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return;
			}

			if ( pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.GetRankType() != GUILDRANKTYPE_MASTER )
			{
				TCHAR* pText = GetTxtFromMgr(6693);
				TCHAR* pTitle = GetTxtFromMgr(6694);

				((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return;
			}
			
			SI32 roomIndex = _tstoi( pList->GetText( siRow, 0 ) );

			cltGameMsgRequest_GuildWar_ApplyToEnemy clinfo( m_siFieldIndex, roomIndex, pEdit->GetText() );
			cltMsg clMsg(GAMEMSG_REQUEST_GUILDWAR_APPLYTOENEMY, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

			//cltGameMsgRequest_GuildWar_ApplyReserve clinfo( m_siFieldIndex, roomIndex, pEdit->GetText() );
			//cltMsg clMsg(GAMEMSG_REQUEST_GUILDWAR_APPLYRESERVE, sizeof(clinfo), (BYTE*)&clinfo);
			//pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

		}
	}
	CASE_CONTROL( COMBO_ROOM )
	{
		if ( EVENT_COMBOBOX_SELECTION_CHANGED == nEvent )
		{
			CComboBox* pCombo = (CComboBox*)m_pControlMap[ COMBO_ROOM ];
			m_siFieldIndex = pCombo->GetSelectedIndex();

			cltGameMsgRequest_GuildWar_ReserveList clinfo(m_siFieldIndex);
			cltMsg clMsg(GAMEMSG_REQUEST_GUILDWAR_RESERVELIST, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
	}
	CASE_CONTROL( BUTTON_RECOMMAND )
	{
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			pclclient->CreateInterface( NGUILDWARRECOMMAND_DLG );
		}
	}
}

void NGuildWarListDlg::RefreshList()
{
	CComboBox* pCombo = (CComboBox*)m_pControlMap[ COMBO_ROOM ];
	SI16 m_siFieldIndex = pCombo->GetSelectedIndex();
	if(m_siFieldIndex < 0)
	{
		return;
	}
	
	CList *pList = (CList*)m_pControlMap[ LIST_WARLIST ];
	
	stListBoxItemData itemdata;
	TCHAR buffer[256];

	pList->Clear();
	for ( SI32 i=0; i<GUILDWAR_ROOMNUM; i++ )
	{
		if ( m_clUnit[i].m_siRoomIndex == 0 ) break;

		itemdata.Init();
		StringCchPrintf( buffer, 256, TEXT("%d"), m_clUnit[i].m_siRoomIndex );
		itemdata.Set( 0, buffer );
		SI32 siHour = m_clUnit[i].m_siRoomIndex / 2;
		SI32 siMinute = m_clUnit[i].m_siRoomIndex % 2 * 30;
		StringCchPrintf( buffer, 256, TEXT("%02d : %02d"), siHour, siMinute );
		itemdata.Set( 1, buffer );
		itemdata.Set( 2, m_clUnit[i].m_szHomeGuildName );
		itemdata.Set( 3, m_clUnit[i].m_szAwayGuildName );

		pList->AddItem( &itemdata );

	}
	pList->Refresh();

	if ( pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.GetRankType() == GUILDRANKTYPE_MASTER )
	{
		((CList*)m_pControlMap[BUTTON_APPLY])->Enable( true );
	}

}
void NGuildWarListDlg::SetGuildName(TCHAR* guildName )
{
	CEdit* pEdit = (CEdit*)m_pControlMap[ EDIT_ENEMYGUILD ];
	if ( pEdit )
	{
		pEdit->SetText( guildName );
	}
}