#include "NGuildDungeonDlg.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../common/Item/ItemCommon/Item-GuildDungeon.h"
#include "../../Client/Client.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"


extern cltCommonLogic* pclClient;

CNGuildDungeonDlg::CNGuildDungeonDlg()
{
	Init();
}

CNGuildDungeonDlg::~CNGuildDungeonDlg()
{
	Destroy();
}

void CNGuildDungeonDlg::Init()
{
	m_siItemUnique = 0;

}

void CNGuildDungeonDlg::Destroy()
{

}

void CNGuildDungeonDlg::Create()
{
	if ( IsCreate() ) return;

	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/NPlayMatchDlg/Dlg_NGuildDungeon.ddf"));
	file.CreatePopupDialog( this, NGUILDDUNGEON_DLG, "dialog_guilddungeon", StaticGuildDungeonDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST , NGUILDDUNGEON_LIST_DUNGEONLIST, this) ,NGUILDDUNGEON_LIST_DUNGEONLIST, TEXT("listbox_dungeonlist"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGUILDDUNGEON_BUTTON_APPLY, this)  , NGUILDDUNGEON_BUTTON_APPLY , TEXT("button_apply"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC , NGUILDDUNGEON_STATIC_NONAME1, this)  , NGUILDDUNGEON_STATIC_NONAME1 , TEXT("statictext_NONAME1"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC , NGUILDDUNGEON_STATIC_ITEMIMAGE, this)  , NGUILDDUNGEON_STATIC_ITEMIMAGE , TEXT("statictext_itemimage"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC , NGUILDDUNGEON_STATIC_ITEMNAME, this)  , NGUILDDUNGEON_STATIC_ITEMNAME , TEXT("statictext_itemname"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST , NGUILDDUNGEON_LIST_ITEMLIST, this)  , NGUILDDUNGEON_LIST_ITEMLIST , TEXT("listbox_itemlist"));


	CList* pList = m_InterfaceFactory.GetList(NGUILDDUNGEON_LIST_DUNGEONLIST);

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(6727);
	pList->SetColumn(0,80,pText);
	pText = GetTxtFromMgr(6728);	
	pList->SetColumn(1,100,pText);
	pText = GetTxtFromMgr(6729);
	pList->SetColumn(2,50,pText);
	pText = GetTxtFromMgr(6730);
	pList->SetColumn(3,100,pText);
	pList->Refresh();

	pList->Enable( true );


	pList = m_InterfaceFactory.GetList(NGUILDDUNGEON_LIST_ITEMLIST);
	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	pText = GetTxtFromMgr(6598);
	pList->SetColumn(0, 126, pText);
	pText = GetTxtFromMgr(4166);
	pList->SetColumn(1, 100, pText);
	pList->Refresh();

	ShowDungeonList();
}

void CALLBACK CNGuildDungeonDlg::StaticGuildDungeonDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNGuildDungeonDlg *pThis = (CNGuildDungeonDlg*)pControllerMgr;
	pThis->NGuildDungeonDlgProc( nEvent, nControlID, pControl);
}

void CNGuildDungeonDlg::NGuildDungeonDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NGUILDDUNGEON_BUTTON_APPLY:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				pclclient->CreateInterface(NGUILDDUNGEONAPPLY_DLG);
			}
		}
		break;
	case NGUILDDUNGEON_LIST_DUNGEONLIST:
		{
			if ( EVENT_LISTBOX_SELECTION == nEvent )
			{
				ShowItemList();
			}
		}
		break;
	default:
		break;
	}
}

void CNGuildDungeonDlg::Action()
{
}

void CNGuildDungeonDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if ( m_siItemUnique > 0 )
	{
		SI32 itemType = 0;
		SI32 fontIndex = -1;
		if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_siItemUnique, &itemType, &fontIndex ) )
		{
			TSpr* pSpr = pclClient->GetGlobalSpr(itemType);
			
			CStatic* pStatic = m_InterfaceFactory.GetStatic(NGUILDDUNGEON_STATIC_ITEMIMAGE);

			GP.PutSpr( pSpr,ScreenRX + GetX() + pStatic->GetX()+8, ScreenRY+GetY()+pStatic->GetY(), fontIndex );
		}
	}
}

void CNGuildDungeonDlg::ShowDungeonList()
{
	cltMsg clMsg(GAMEMSG_REQUEST_GUILDDUNGEON_STATUS, 0 );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);	
}

void CNGuildDungeonDlg::SetList(cltGameMsgResponse_GuildDungeon_Status* pclinfo )
{


	CList* pList = m_InterfaceFactory.GetList(NGUILDDUNGEON_LIST_DUNGEONLIST);
	stListBoxItemData itemData;

	TCHAR buffer[128];
	SI32  index = 0;

	pList->Clear();

	for ( SI32 i=0; i<MAX_GUILDDUNGEON; i++ )
	{
		if ( pclinfo->siMapIndex[i] <= 0 ) continue;

		itemData.Init();

		TCHAR* mapName = pclClient->pclMapManager->GetMapName( pclinfo->siMapIndex[i] );
		itemData.Set(0, mapName );
		itemData.siParam[0] = pclinfo->siMapIndex[i];

		itemData.Set(1, pclinfo->szGuildName[i] );

		_i64tot(pclinfo->siExp[i], buffer, 10 );// SI32->SI64 경험치 오버플로우때문에 수정
		itemData.Set(2, buffer );

		_i64tot(pclinfo->siMoney[i], buffer, 10 );
		itemData.Set(3, buffer );

		pList->AddItem(&itemData);
	}

	pList->Refresh();
}

void CNGuildDungeonDlg::ShowItemList()
{
	CStatic* pStatic = m_InterfaceFactory.GetStatic(NGUILDDUNGEON_STATIC_ITEMNAME);
	CList* pList = m_InterfaceFactory.GetList(NGUILDDUNGEON_LIST_ITEMLIST);
	CList* pDungeonList = m_InterfaceFactory.GetList(NGUILDDUNGEON_LIST_DUNGEONLIST);
	pList->Clear();

	pStatic->SetText(TEXT(""));
	m_siItemUnique = 0;

	SI32 selectedIndex = pDungeonList->GetSelectedIndex();

	if ( selectedIndex < 0 )
		return;

	stListBoxItemData itemData =  pDungeonList->GetItemData( selectedIndex, 0 );

	SI32 siMapIndex = itemData.siParam[0];

	for ( SI16 i=0 ; i < MAX_GUILDDUNGEON; i++ )
	{
		if ( pclClient->pclGuildDungeonItemMgr->m_pclItem[i] == NULL)
		{
			continue;
		}

		if ( pclClient->pclGuildDungeonItemMgr->m_pclItem[i]->m_siMapIndex == siMapIndex )
		{
			stListBoxItemData itemData2;
			TCHAR buffer[128];

			for ( SI16 j=0 ; j < 5; j++ )
			{
				if ( j == 4 )
				{
					StringCchPrintf( buffer, 128, TEXT("%d ~"), pclClient->pclGuildDungeonItemMgr->m_pclItem[i]->m_siCondition[j] );
				}
				else
				{
					StringCchPrintf( buffer, 128, TEXT("%d ~ %d"), pclClient->pclGuildDungeonItemMgr->m_pclItem[i]->m_siCondition[j] + 1, pclClient->pclGuildDungeonItemMgr->m_pclItem[i]->m_siCondition[j+1] );
				}

				itemData2.Set( 0, buffer );
				_itot( pclClient->pclGuildDungeonItemMgr->m_pclItem[i]->m_siItemNum[j], buffer, 10 );
				itemData2.Set( 1, buffer );

				pList->AddItem( &itemData2 );

			}
			

			m_siItemUnique = pclClient->pclGuildDungeonItemMgr->m_pclItem[i]->m_siItemUnique;
			TCHAR* itemName = (TCHAR*)pclClient->pclItemManager->GetName( m_siItemUnique );
			pStatic->SetText(itemName, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		}
	}
	pList->Refresh();
}