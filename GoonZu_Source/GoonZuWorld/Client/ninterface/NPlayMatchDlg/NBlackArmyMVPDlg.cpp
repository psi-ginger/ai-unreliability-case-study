#include "..\Client\Client.h"
#include "./NBlackArmyMVPDlg.h"
#include "..\resource.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../../Client/InterfaceMgr/Interface/ComboBox.h"
#include "../../../Client/InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../common/Char/CharManager/CharManager.h"
#include "../CommonLogic/BlackArmy/BlackArmy.h"

#include "MsgType-System.h"

extern cltCommonLogic* pclClient;

NBlackArmyMVPDlg::NBlackArmyMVPDlg()
{

	m_pCombo_MapName = NULL;
	m_pList_MVP = NULL;
	m_pButton_close = NULL;
	m_pStatic_MapName = NULL;
	m_pStatic_WarLose = NULL;

}

NBlackArmyMVPDlg::~NBlackArmyMVPDlg()
{

	NDelete(m_pCombo_MapName);
	NDelete(m_pList_MVP);
	NDelete(m_pButton_close);
	NDelete(m_pStatic_MapName);
	NDelete(m_pStatic_WarLose);

}

void NBlackArmyMVPDlg::Create()
{
	cltClient* pclclient = (cltClient*)pclClient;
	// 대화상자가 없는 상태라면 만든다. 
	if( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NPlayMatchDlg/Dlg_NBlackArmy_MVP.ddf");
		file.CreatePopupDialog( this, NBLACKARMYMVP_DLG, "dialog_blackarmy_mvp", NBlackArmyMVPStaticProc );

		m_pCombo_MapName = new CComboBox( this );
		m_pList_MVP = new CList( this );
		m_pButton_close = new CButton ( this );
		m_pStatic_MapName = new CStatic ( this );
		m_pStatic_WarLose = new CStatic ( this );

		file.CreateControl( m_pCombo_MapName, NBLACKARMY_MVP_DLG_COMBO_MAPNAME, "combobox_MAPNAME" );
		file.CreateControl( m_pList_MVP, NBLACKARMY_MVP_DLG_LIST_MVP, "listbox_NONAME1" );
		file.CreateControl( m_pButton_close, NBLACKARMY_MVP_DLG_BUTTON_CLOSE, "button_close" );
		file.CreateControl( m_pStatic_MapName, NBLACKARMY_MVP_DLG_STATIC_MAPNAME, "statictext_mapname");
		file.CreateControl( m_pStatic_WarLose, NBLACKARMY_MVP_DLG_STATIC_WARLOSE, "statictext_warlose");

		m_pList_MVP->SetBorder(true);
		m_pList_MVP->SetBKColorUse(true);

		m_pCombo_MapName->SetBorder(true);
		m_pCombo_MapName->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr(5398);
		m_pList_MVP->SetColumn(0,50,pText);
		pText = GetTxtFromMgr(1586);	
		m_pList_MVP->SetColumn(1,100,pText);
		pText = GetTxtFromMgr(1556);
		m_pList_MVP->SetColumn(2,150,pText);
		m_pList_MVP->Refresh();

		m_pStatic_WarLose->SetFontColor( RGB( 255, 0, 0 ) );
		m_pStatic_WarLose->SetFontWeight( FW_BOLD );
		pText = GetTxtFromMgr(5874);
		m_pStatic_WarLose->SetText(pText);

		LoadMapName();

		m_pCombo_MapName->SetCursel(0);

	}
	else
	{
		DeleteDialog();
	}
}

void NBlackArmyMVPDlg::Close()
{
	DeleteDialog();
}

void CALLBACK NBlackArmyMVPDlg::NBlackArmyMVPStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	NBlackArmyMVPDlg *pThis = (NBlackArmyMVPDlg*)pControllerMgr;
	pThis->NBlackArmyMVPProc( nEvent, nControlID, pControl);
}

void CALLBACK NBlackArmyMVPDlg::NBlackArmyMVPProc( UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
	case NBLACKARMY_MVP_DLG_BUTTON_CLOSE:
		{
			Close();
		}
		break;

	case NBLACKARMY_MVP_DLG_COMBO_MAPNAME:
		{
			SI32 selectindex = m_pCombo_MapName->GetSelectedIndex();

			if (selectindex >= 0)
			{
				cltGameMsgRequest_BlackArmy_RankInfo clRankInfo(selectindex);
				cltMsg clMsg(GAMEMSG_REQUEST_BLACKARMY_RANKINFO, sizeof(clRankInfo), (BYTE*)&clRankInfo);

				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}

	}

}

void NBlackArmyMVPDlg::LoadMapName()
{
	m_pCombo_MapName->Clear();

	for (SI32 i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
	{

		TCHAR* pText = pclClient->pclMapManager->GetMapName( BLACKARMY_MAP_STARTINDEX + i );
		TCHAR buf[256];

		StringCchCopy(buf, 256, pText);

		stComboBoxItemData ComboBoxItemData;
		ComboBoxItemData.Init();
		ComboBoxItemData.Set( buf );					

		m_pCombo_MapName->AddItem( &ComboBoxItemData );

	}
}

void NBlackArmyMVPDlg::SetRanks(SI32 warmapindex, SI32 *scores, TCHAR **players)
{
	TCHAR rankplayers[MAX_WARRANK_PERSON_PER_MAP][MAX_PLAYER_NAME];

	memcpy(rankplayers, players, sizeof(TCHAR) * MAX_WARRANK_PERSON_PER_MAP * MAX_PLAYER_NAME);

	m_pList_MVP->Clear();

	bool bWarWin = false;

	for (SI32 i = 0; i < MAX_WARRANK_PERSON_PER_MAP; i++)
	{
		stListBoxItemData Data;

		TCHAR szrank[128] = TEXT("");
		TCHAR szscores[128] = TEXT("");
		TCHAR szplayer[128] = TEXT("");

		if (scores[i] > 0)
		{
			StringCchPrintf(szrank, 128, TEXT("%d"), i + 1);
			StringCchPrintf(szscores, 128, TEXT("%d"), scores[i]);
			StringCchPrintf(szplayer, 128, TEXT("%s"), rankplayers[i]);

			bWarWin = true;
		}
		
		Data.Set( 0, szrank );
		Data.Set( 1, szscores );
		Data.Set( 2, szplayer );

		m_pList_MVP->AddItem( &Data );

	}

	if (bWarWin == false)
	{
		m_pStatic_WarLose->Show(true);
		m_pStatic_WarLose->Refresh();
	}
	else
	{
		m_pStatic_WarLose->Show(false);
	}

	m_pList_MVP->Refresh();

}

