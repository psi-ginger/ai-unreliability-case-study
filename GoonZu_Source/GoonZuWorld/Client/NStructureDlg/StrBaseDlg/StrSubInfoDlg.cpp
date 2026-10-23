#include "StrSubInfoDlg.h"

#include "../../InterfaceMgr/Interface/List.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Guild.h"
#include "../../CommonLogic/MsgType-Structure.h"

//#include "../../CommonLogic/Guild/Guild.h"
//#include "../../CommonLogic/Guild/GuildRankManager.h"


#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NStrSubInfoDlg::NStrSubInfoDlg()
{
	m_siVillageUnique = -1;
//	m_siUnitSlot = -1;
//	m_pGuildRank = NULL;
	
}

NStrSubInfoDlg::~NStrSubInfoDlg()
{
}

void NStrSubInfoDlg::InitDlg()
{
	cltClient* pclclient = (cltClient*)pclClient;

//	m_pGuildRank = pclclient->pclGuildRankManager;

	SetControlMap( BUTTON_REFRESH, TEXT("button_refresh") );
	SetControlMap( EDITBOX_BENEFIT,TEXT("editbox_benefit"));
	SetControlMap( EDITBOX_CAPITAL,TEXT("editbox_capital"));
	SetControlMap( EDITBOX_CHIEF,TEXT("editbox_chief"));
	SetControlMap( EDITBOX_EXP,TEXT("editbox_exp"));
	SetControlMap( EDITBOX_LEVEL,TEXT("editbox_level"));
//	SetControlMap( LISTBOX_GUILDLIST,TEXT("listbox_guildlist"));

/*	CList *pList = (CList *)m_pControlMap[ LISTBOX_GUILDLIST ];
	stListBoxItemData itemdata;

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR *pText[ 2 ];
	pText[ 0 ] = GetTxtFromMgr( 1556 );
	pText[ 1 ] = GetTxtFromMgr( 5271 );

	pList->SetColumn( 0, 144, pText[ 0 ] );
	pList->SetColumn( 1, 192, pText[ 1 ] );
	pList->Refresh();
	*/

	m_bShowTaxInfo = FALSE;

//	SendRequestGuildUnitInfo();
}
/*
BOOL NStrSubInfoDlg::SendRequestGuildUnitInfo()
{
	cltClient* pclclient = (cltClient*)pclClient;

	SI32 unitslot = pclclient->pclGuildStructManager->siSelectedGuildStrUnitNumber;
	SI32 villageunique = pclclient->pclGuildStructManager->siVillageUnique;

	if( unitslot < 0 || unitslot >= MAX_GUILDUNIT_NUMBER 
		|| villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER )
		return FALSE;

	m_siUnitSlot = unitslot;
	m_siVillageUnique = villageunique;
		
	// 길드 목록 요청
	cltGameMsgRequest_GuildUnitRank clGuildUnitRank( villageunique, unitslot );
	cltMsg clMsg( GAMEMSG_REQUEST_GUILDUNITRANK, sizeof(cltGameMsgRequest_GuildUnitRank), (BYTE*)&clGuildUnitRank );
	pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );

	return TRUE;
}
*/

void NStrSubInfoDlg::Action()
{
	if( m_pBaseDlg->m_dwActionCount % 3 ) return;

	//TCHAR buffer[256];

/*	if( m_pGuildRank->bUpdateGuildUnitInfoSwitch == TRUE )
	{
		m_pGuildRank->bUpdateGuildUnitInfoSwitch = FALSE;

		//단위 길드 정보를 보여준다.
		ShowGuildUnitListInfo();

		ShowGuildUnitInfo();
	}*/
}
/*
void NStrSubInfoDlg::ShowGuildUnitListInfo()
{
	CList *pList = (CList*)m_pControlMap[ LISTBOX_GUILDLIST ];
	stListBoxItemData itemdata;

	SI32 i;
	//TCHAR buffer[256];

	// 리스트뷰를 모두 지운다. 
	pList->Clear();	

	TCHAR name[MAX_PLAYER_NAME];
	TCHAR alias[MAX_GUILDRANK_NAME];

	for( i = 0; i<cltGuildRankManager::GUILDRANK_END; i++ )
	{
		itemdata.Init();

		itemdata.siParam[0] = i;

		switch( i )
		{
		case cltGuildRankManager::GUILDRANK_EMPTY:
		case cltGuildRankManager::GUILDRANK_END:
		case cltGuildRankManager::GUILDRANK_GENERAL:
			continue;
			break;
		}

		memcpy( name, (TCHAR*)m_pGuildRank->clRank[m_siVillageUnique][m_siUnitSlot][ i ].clPerson.GetName(), sizeof(name)) ;	// 단위길드 행수
		if( name == NULL || _tcslen(name) <= 0 )
			StringCchCopy(name, MAX_PLAYER_NAME, TEXT("NOTEXT"));
		memcpy( alias, (TCHAR*)m_pGuildRank->clRank[m_siVillageUnique][m_siUnitSlot][ i ].GetRankName(), sizeof(alias));

		itemdata.Set( 0, name );
		itemdata.Set( 1, alias );

		pList->AddItem( &itemdata );
	}

	pList->Refresh();
}
*/
/*
void NStrSubInfoDlg::ShowGuildUnitInfo()
{
	cltClient* pclclient = (cltClient*)pclClient;

	if( m_siUnitSlot < 0 || m_siUnitSlot >= MAX_GUILDUNIT_NUMBER 
		|| m_siVillageUnique < 0 || m_siVillageUnique >= MAX_VILLAGE_NUMBER )
		return ;

	if( m_pGuildRank == NULL) return;

	TCHAR tmpchar[ 50 ];
	// benefit
	StringCchPrintf( tmpchar, 50, TEXT("%d"), pclclient->pclGuildStructManager->clProfitMoney.itMoney );
	SetEditText( EDITBOX_BENEFIT, tmpchar );

	// capital
	StringCchPrintf( tmpchar, 50, TEXT("%d"), pclclient->pclGuildStructManager->clCapitalMoney.itMoney );
	SetEditText( EDITBOX_CAPITAL, tmpchar );

	// exp
	StringCchPrintf( tmpchar, 50, TEXT("%d"), pclclient->pclGuildStructManager->siExp );
	SetEditText( EDITBOX_EXP, tmpchar );

	// level
	StringCchPrintf( tmpchar, 50, TEXT("%d"), pclclient->pclGuildStructManager->siLevel );
	SetEditText( EDITBOX_LEVEL, tmpchar );

	// chief
	StringCchPrintf( tmpchar, 50, TEXT("%s"), m_pGuildRank->clRank[ m_siVillageUnique ][ m_siUnitSlot ][ cltGuildRankManager::GUILDRANK_CHIEF ].clPerson.szName );
	SetEditText( EDITBOX_CHIEF, tmpchar );

}*/

void NStrSubInfoDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CASE_CONTROL( BUTTON_REFRESH )
	{
//		SendRequestGuildUnitInfo();    
	}
}