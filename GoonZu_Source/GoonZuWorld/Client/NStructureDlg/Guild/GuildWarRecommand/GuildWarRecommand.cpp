#include "GuildWarRecommand.h"
#include "InterfaceMgr/InterfaceMgr.h"
#include "InterfaceMgr/InterfaceFile.h"
#include "CommonLogic.h"
#include "../../../../Client/Client.h"
#include "../../../../CommonLogic/Guild/Guild.h"
#include "../../../../CommonLogic/MsgType-Guild.h"
#include "../../../../common/Char/CharManager/CharManager.h"
#include "../GuildDlg.h"

extern cltCommonLogic* pclClient;
CNGuildWarRecommand::CNGuildWarRecommand()
{
	Initialize();
}

CNGuildWarRecommand::~CNGuildWarRecommand()
{
	Destroy();
}

void CNGuildWarRecommand::Initialize()
{

}

void CNGuildWarRecommand::Destroy()
{

}

void CNGuildWarRecommand::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NStructureDlg/Guild/DLG_GuildWarRecommand.ddf"));
	file.CreatePopupDialog( this, NGUILDWARRECOMMAND_DLG, TEXT("dialog_recommandguild"), StaticGuildWarRecommandDlgProc);	

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST ,NGUILDWARRECOMMAND_LIST_GUILD , this)  , NGUILDWARRECOMMAND_LIST_GUILD , TEXT("listbox_guildlist"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGUILDWARRECOMMAND_BUTTON_INPUT , this)  , NGUILDWARRECOMMAND_BUTTON_INPUT , TEXT("button_input"));

	CList* pList = m_InterfaceFactory.GetList(NGUILDWARRECOMMAND_LIST_GUILD);

	pList->SetBorder( true );
	pList->SetBKColorUse( true );

	TCHAR* pText = GetTxtFromMgr(6548); //  길드명
	pList->SetColumn( 0, 100, pText );
	pText = GetTxtFromMgr(6549); // 길드 마스터
    pList->SetColumn( 1, 80, pText );
	pText = GetTxtFromMgr(6550);	// 길드레벨
	pList->SetColumn( 2, 35, pText );
	pText = GetTxtFromMgr(6551); // 길드원수
	pList->SetColumn( 3, 35, pText );

	pList->Refresh();


	cltMsg clMsg(GAMEMSG_REQUEST_GUILDWAR_RECOMMANDLIST, 0 );
	pclclient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
}


void CNGuildWarRecommand::StaticGuildWarRecommandDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNGuildWarRecommand *pThis = (CNGuildWarRecommand*)pControllerMgr;
	pThis->NGuildWarRecommandDlgProc( nEvent, nControlID, pControl );
}
void CNGuildWarRecommand::NGuildWarRecommandDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient* pclclient = (cltClient*)pclClient;
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NGUILDWARRECOMMAND_BUTTON_INPUT:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				InputGuildName();
			}
		}
		break;
	default:
		break;

	}
}


void CNGuildWarRecommand::SetGuildList( cltGameMsgResponse_GuildWar_RecommandList* pclinfo )
{
	CList* pList = m_InterfaceFactory.GetList(NGUILDWARRECOMMAND_LIST_GUILD);
	pList->Clear();
	stListBoxItemData itemData;

	for ( SI32 i = 0; i < MAX_RECOMMANDGUILDLIST; i++ )
	{
		if ( pclinfo->siLevel[i] > 0 )
		{
			TCHAR buf[16] = {0,};
			itemData.Init();
			itemData.Set( 0, pclinfo->szGuildName[i] );
			itemData.Set( 1, pclinfo->szMasterName[i] );
			_itot( pclinfo->siLevel[i], buf, 10 );
			itemData.Set( 2, buf  );
			_itot( pclinfo->siMemberCount[i], buf, 10);
			itemData.Set( 3, buf );

			pList->AddItem( &itemData );
		}
	}
	pList->Refresh();
}

void CNGuildWarRecommand::InputGuildName()
{
	cltClient* pclclient = (cltClient*)pclClient;
	
	CList* pList = m_InterfaceFactory.GetList(NGUILDWARRECOMMAND_LIST_GUILD);
	SI32 selectedIndex = pList->GetSelectedIndex();
	if ( selectedIndex < 0  )
		return;
	if( pclclient->m_pDialog[ NGUILDSTR_DLG ] == NULL ) 
	{
		pclclient->CreateInterface( NGUILDSTR_DLG );
		((NGuildDlg*)pclclient->m_pDialog[NGUILDSTR_DLG])->SetActiveTab( 3 );
	}

	NGuildWarListDlg* pDialog = ((NGuildWarListDlg*)((NGuildDlg*)pclclient->m_pDialog[NGUILDSTR_DLG])->m_pUserChildDlg[3]);
	if ( pDialog )
	{
		pDialog->SetGuildName( pList->GetText(selectedIndex, 0 ) );
	}

	DeleteDialog();
}
