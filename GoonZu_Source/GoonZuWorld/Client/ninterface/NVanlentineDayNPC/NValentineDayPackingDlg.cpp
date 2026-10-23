#include "NValentineDayPackingDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-Quest.h"
#include "../../CommonLogic/Msg/MsgType-NPC.h"

NValentineDayPackingDlg::NValentineDayPackingDlg()
{
	
}

NValentineDayPackingDlg::~NValentineDayPackingDlg()
{
	
}

void NValentineDayPackingDlg::Destroy()
{ 


}

void NValentineDayPackingDlg::Create()
{
	if( IsCreate() ) return;

	CInterfaceFile file;

    file.LoadFile("Ninterface/Data/NValentineDay/NValentineDayPacking.ddf");
	file.CreatePopupDialog( this, VALENTINEDAY_PACKING_DLG, "dialog_valentinedaypacking" , StaticNValentineDayPackingDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LISTVIEW,  	VALENTINEDAY_PACKING_LISTVIEW_ITEM,		this),  VALENTINEDAY_PACKING_LISTVIEW_ITEM,		TEXT("listview_pakingitem")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	VALENTINEDAY_PACKING_STATIC_NONAME1,	this),  VALENTINEDAY_PACKING_STATIC_NONAME1,	TEXT("statictext_NONAME1")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	VALENTINEDAY_PACKING_STATIC_NONAME2,	this),  VALENTINEDAY_PACKING_STATIC_NONAME2,	TEXT("statictext_NONAME2")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		VALENTINEDAY_PACKING_EDITBOX_ITEM_NUM,	this),  VALENTINEDAY_PACKING_EDITBOX_ITEM_NUM,	TEXT("editbox_packingitemnum")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	VALENTINEDAY_PACKING_BUTTON_OK,			this),  VALENTINEDAY_PACKING_BUTTON_OK,			TEXT("button_ok")				);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	VALENTINEDAY_PACKING_BUTTON_CANCEL,		this),  VALENTINEDAY_PACKING_BUTTON_CANCEL,		TEXT("button_cancel")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	VALENTINEDAY_PACKING_OUTLINE_NONAME1,	this),  VALENTINEDAY_PACKING_OUTLINE_NONAME1,	TEXT("outline_NONAME1")			);
	
	m_InterfaceFactory.GetEdit( VALENTINEDAY_PACKING_EDITBOX_ITEM_NUM )->SetNumberMode( true ); 
	m_InterfaceFactory.GetEdit( VALENTINEDAY_PACKING_EDITBOX_ITEM_NUM )->Refresh();  

	SetItemInfo();
		
	SetX( 450 ); 
	SetY( 300 );
}

void NValentineDayPackingDlg::Action()
{

}

void NValentineDayPackingDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	TSpr* pspr = pclClient->GetGlobalSpr( GIMG_VALENTINEDAY_HEART );
	if( pspr ) 
	{
		GP.PutSprScreen( pspr, GetX(),		 GetY(),	  max(0, TABS(pclClient->CurrentClock / 100))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() +  50, GetY() - 20, max(0, TABS(pclClient->CurrentClock / 120))%pspr->GetImageNum() ); 
		GP.PutSprScreen( pspr, GetX() +  90, GetY() + 50, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() );  
		GP.PutSprScreen( pspr, GetX() + 100, GetY() + 10, max(0, TABS(pclClient->CurrentClock / 150))%pspr->GetImageNum() ); 
	}	
}

void NValentineDayPackingDlg::SetItemInfo( void )
{
	CListView* pListView = m_InterfaceFactory.GetListView( VALENTINEDAY_PACKING_LISTVIEW_ITEM );
	pListView->DeleteAllItems();

	SI32 				GlobalImageIndex	= -1;
	SI32 				GlobalImageFont		= -1;
	SI32 				StartPos			= 15;   //장착하고 있는 장비 제외하고 말, 사람, 가방에 있는 아이템만 출력 ^^* by tae yang
	SI32 				id					=  1;
	stListViewItemData  ListViewItemData;
	TCHAR*				ItemName			= NULL;
	if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( ITEMUNIQUE(13660), &GlobalImageIndex, &GlobalImageFont ) )
	{
		cltItem clItem;
		clItem.siUnique = ITEMUNIQUE(13660);

		ItemName = (TCHAR*)pclClient->pclItemManager->GetName( clItem.siUnique );
		if ( ItemName )
		{
			TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
			pclClient->pclItemManager->GetExplain( &clItem, ToolTipMsg );
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0, clItem.siItemNum, ItemName, ToolTipMsg );
			pListView->InsertItem( &ListViewItemData );
			pListView->SetItemNumShow( false );
		}
	}

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if( NULL == pclChar ) return;

	// 개수.
	m_InterfaceFactory.GetEdit(VALENTINEDAY_PACKING_EDITBOX_ITEM_NUM)->SetText( SI32ToString( pclChar->pclCI->clCharItem.GetItemNum( ITEMUNIQUE(13660) ) ) );
}

void CALLBACK NValentineDayPackingDlg::StaticNValentineDayPackingDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NValentineDayPackingDlg *pThis = (NValentineDayPackingDlg *)pControllerMgr;
	pThis->NValentineDayPackingDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NValentineDayPackingDlg::NValentineDayPackingDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case VALENTINEDAY_PACKING_EDITBOX_ITEM_NUM:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_CHANGE:	
					{
						cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
						if( NULL == pclChar ) return;
						
						SI32 siItemNum		= _ttoi(m_InterfaceFactory.GetEdit(VALENTINEDAY_PACKING_EDITBOX_ITEM_NUM)->GetText() );
						SI32 siInvenItemNum = pclChar->pclCI->clCharItem.GetItemNum( ITEMUNIQUE(13660) );

						if( siItemNum > siInvenItemNum || 0 > siItemNum )
						{
							m_InterfaceFactory.GetEdit(VALENTINEDAY_PACKING_EDITBOX_ITEM_NUM)->SetText( SI32ToString(siInvenItemNum) );
						}
					}
					break;
				}
			}
			break;
		case VALENTINEDAY_PACKING_BUTTON_OK:
			{
				cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
				if( NULL == pclChar ) return;

				SI32 siItemNum		= _ttoi(m_InterfaceFactory.GetEdit(VALENTINEDAY_PACKING_EDITBOX_ITEM_NUM)->GetText() );
				SI32 siInvenItemNum = pclChar->pclCI->clCharItem.GetItemNum( ITEMUNIQUE(13660) );
				if( siItemNum > siInvenItemNum || 0 >= siItemNum) 
				{
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(2237) );
					return;
				}

				cltItem	 clPackingItem;
				clPackingItem.siUnique  = ITEMUNIQUE(13660);
				clPackingItem.siItemNum = siItemNum;
				
				cltGameRequest_ValentineDay_Item_Packing clinfo( &clPackingItem );
				cltMsg clMsg( GAMEMSG_REQUEST_VALENTINEDAY_ITEM_PACKING, PACKET(clinfo) );
				((cltClient*)pclClient)->SendMsgToServer((sPacketHeader*)&clMsg);

				DeleteDialog();

			}
			break;
		case VALENTINEDAY_PACKING_BUTTON_CANCEL:
			{
				DeleteDialog();
			}
			break;
			
		default:
			{

			}
			break;
	}
}

