#include "NChoiseItemGameStartDlg.h"
#include "NRecievedItemDlg.h"
#include "NGachaDlgNew.h"
#include "NOpenBoxDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Msg/msgType-Gacha.h"
#include "../../../common/Char/CharManager/CharManager.h"
#include "../../../Client/Music/music.h"

#include "../../lib/MD5/md5.h"
#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"

CNRecievedItemDlg::CNRecievedItemDlg()
{

}

CNRecievedItemDlg::~CNRecievedItemDlg()
{
	Destroy();
}


void CNRecievedItemDlg::Destroy()
{
	cltClient * pclclient = (cltClient*) pclClient;
	if(pclclient->m_pDialog[NGACHANEW_DLG])
		((CNGachaDlgNew*)pclclient->m_pDialog[NGACHANEW_DLG])->SetExplain2(GetTxtFromMgr(7022));
}

void CNRecievedItemDlg::Create()
{
	if( IsCreate() )
	{
        return;
	}

	CInterfaceFile file;
	
	file.LoadFile("Ninterface/Data/NGachaDlg/DLG_NRecievedItemDlg.ddf");
	file.CreatePopupDialog( this , RECIEVED_ITEM_DLG, "dialog_recieved_item" , StaticNRecievedItemDlgProc );

	if( g_bFullScreen)	MovePos( 80, 40 );
	else		 		MovePos( 45, 20 );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	RECIEVED_IMAGESTATIC_BEST_ITEM,		this), RECIEVED_IMAGESTATIC_BEST_ITEM,		TEXT("imagestatic_bestitem"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		RECIEVED_ITEM_BUTTON_CLOSE,			this), RECIEVED_ITEM_BUTTON_CLOSE,			TEXT("button_close")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		RECIEVED_ITEM_BUTTON_RESTART,		this), RECIEVED_ITEM_BUTTON_RESTART,		TEXT("button_restart")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		RECIEVED_ITEM_BUTTON_INIT,			this), RECIEVED_ITEM_BUTTON_INIT,			TEXT("button_init")				);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		RECIEVED_ITEM_EDIT_RECIEVED_ITEM,	this), RECIEVED_ITEM_EDIT_RECIEVED_ITEM,	TEXT("editbox_recieved_item")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		RECIEVED_ITEM_EDIT_ITEMNAME,		this), RECIEVED_ITEM_EDIT_ITEMNAME,			TEXT("editbox_item_name")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LISTVIEW,	RECIEVED_ITEM_LISTVIEW_ITEM,		this), RECIEVED_ITEM_LISTVIEW_ITEM,			TEXT("listview_item")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	RECIEVED_IMAGESTATIC_BACKGROUND,	this), RECIEVED_IMAGESTATIC_BACKGROUND,		TEXT("imagestatic_background")	);

	m_InterfaceFactory.GetEdit(RECIEVED_ITEM_EDIT_RECIEVED_ITEM)->SetTextPrintOption(DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	m_InterfaceFactory.GetEdit(RECIEVED_ITEM_EDIT_RECIEVED_ITEM)->Refresh();

	m_InterfaceFactory.GetEdit(RECIEVED_ITEM_EDIT_ITEMNAME)->SetTextPrintOption(DT_VCENTER | DT_CENTER);
	m_InterfaceFactory.GetEdit(RECIEVED_ITEM_EDIT_ITEMNAME)->Refresh();

	m_InterfaceFactory.GetListView(RECIEVED_ITEM_LISTVIEW_ITEM)->Enable(false);

	SetTopMostWindow( true );

	cltClient * pclclient = (cltClient*) pclClient;
	if(pclclient->m_pDialog[NGACHANEW_DLG])
		((CNGachaDlgNew*)pclclient->m_pDialog[NGACHANEW_DLG])->SetExplain2(GetTxtFromMgr(7023));
}

void CALLBACK CNRecievedItemDlg::StaticNRecievedItemDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNRecievedItemDlg *pThis = (CNRecievedItemDlg *)pControllerMgr;
	pThis->NRecievedItemDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK CNRecievedItemDlg::NRecievedItemDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;
	
	switch(nControlID)
	{
		case RECIEVED_ITEM_BUTTON_CLOSE:
		{
			if( pclclient->m_pDialog[ NGACHANEW_DLG ] )
				((CNGachaDlgNew*)pclclient->m_pDialog[ NGACHANEW_DLG ])->DeleteDialog();
		}
		break;
		case RECIEVED_ITEM_BUTTON_RESTART:
			{
				//[진성] 인벤토리가 다 차있으면 가차 창을 닫아 버린다.
				cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
				if(false == pclchar->IsBlankInv(1, INVMODE_ALL) )
				{
					if( pclclient->m_pDialog[ NGACHANEW_DLG ] )
					{
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(6634) );
						((CNGachaDlgNew*)pclclient->m_pDialog[ NGACHANEW_DLG ])->Destroy();
						return;
					}
				}

				if( pclclient->m_pDialog[ NGACHANEW_DLG ] )
				{
					if(false == CheckHaveKey(((CNGachaDlgNew*)pclclient->m_pDialog[NGACHANEW_DLG])->GetGachaUseKeyKind()) )
						return;
				}
				else
				{
					return;
				}


				if ( NULL == pclclient->m_pDialog[NOPENBOX] )
				{							
					pclclient->CreateInterface( NOPENBOX );
					
					((CNOpenBoxDlg*) pclclient->m_pDialog[NOPENBOX])->SetGachaUseKeyKind( ((CNGachaDlgNew*)pclclient->m_pDialog[NGACHANEW_DLG])->GetGachaUseKeyKind() );
				}
				else
				{
					pclclient->DestroyInterface( pclclient->m_pDialog[NOPENBOX] );
				}
				DeleteDialog(); 
			}
			break;
		case RECIEVED_ITEM_BUTTON_INIT:
		case COMMON_DIALOG_CLOSEBUTTON:
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

void CNRecievedItemDlg::Action()
{
	

}

void CNRecievedItemDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if( ! IsShow() )
		return;

	cltClient *pclclient = ( cltClient * )pclClient;
}


void CNRecievedItemDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if( ! IsShow() )
		return;

	cltClient *pclclient = ( cltClient * )pclClient;


}

void CNRecievedItemDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{


}

void CNRecievedItemDlg::SetRecieveItem( cltItem * pclItem , UI16 GachaCnt , bool BestProduct  )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	if( pclclient->m_pDialog[ NGACHANEW_DLG ] )
		((CNGachaDlgNew*)pclclient->m_pDialog[ NGACHANEW_DLG ])->SetGachaCount( GachaCnt );

	CListView*		pListView   = NULL;
	CEdit*			pEdit		= NULL;
	TCHAR*			pItemName   = NULL;

	SI32			GlobalImageIndex = -1;
	SI32			GlobalImageFont	 = -1;

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;

	if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclItem->siUnique, &GlobalImageIndex, &GlobalImageFont ) )
	{
		pclClient->pclItemManager->GetExplain( pclItem, ToolTipMsg );
		pItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclItem->siUnique );

		ListViewItemData.Init();
		ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	pclItem->siItemNum, pItemName, ToolTipMsg );

		pListView = m_InterfaceFactory.GetListView( RECIEVED_ITEM_LISTVIEW_ITEM ); 
		pListView->DeleteAllItems();
		pListView->InsertItem( &ListViewItemData );   

		pEdit = m_InterfaceFactory.GetEdit( RECIEVED_ITEM_EDIT_RECIEVED_ITEM ); 
		pEdit->SetText( GetTxtFromMgr( 7026 ) );

		pEdit = m_InterfaceFactory.GetEdit( RECIEVED_ITEM_EDIT_ITEMNAME ); 
		pEdit->SetText( pItemName );

		if(true == BestProduct)
		{
			// 최고 상품은 큰 이미지로 그린다. 
			TCHAR* pFileName    = NULL;
			SI32   siImageIndex = -1;
			cltItemManagerClient* pItemManager = (cltItemManagerClient*)pclClient->pclItemManager;
			if( pItemManager->GetItemImageBigFileName( pclItem->siUnique, &pFileName, &siImageIndex ) )
			{
				m_InterfaceFactory.GetImageStatic(RECIEVED_IMAGESTATIC_BEST_ITEM)->SetFileName( pFileName );
				m_InterfaceFactory.GetImageStatic(RECIEVED_IMAGESTATIC_BEST_ITEM)->SetFontIndex( siImageIndex );
			}	
		}
	}

	if(pclclient->m_pDialog[NGACHANEW_DLG])
	{
		if(false == CheckHaveKey(((CNGachaDlgNew*)pclclient->m_pDialog[NGACHANEW_DLG])->GetGachaUseKeyKind()) )
		{
			m_InterfaceFactory.GetEdit( RECIEVED_ITEM_BUTTON_RESTART )->Enable( false ); 
		}
	}
} 

bool CNRecievedItemDlg::CheckHaveKey( SI32 GachaUseKeyKind )
{
	SI32 KeyItemUnique = 0;
	SI32 KeyNeedNum    = 1;

	if( GachaUseKeyKind == GACHA_USE_GOLDKEY )
	{
		KeyItemUnique = GACHA_GOLDKEY_ITEMUNIQUE;
	}
	else if( GachaUseKeyKind == GACHA_USE_SILVERKEY )
	{
		KeyItemUnique = GACHA_SILVERKEY_ITEMUNIQUE;
	}
	else if( GachaUseKeyKind == GACHA_USE_BRONZEKEY )
	{
		KeyItemUnique = GACHA_BRONZEKEY_ITEMUNIQUE;
	}
	else if( GachaUseKeyKind == GACHA_USE_KEYPIECE )
	{
		KeyItemUnique = GACHA_KEYPIECE_ITEMUNIQUE;
		KeyNeedNum = 10;
	}
	else 
	{
		return false;
	}

	SI32 keyItemNum =	pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( KeyItemUnique );

	if( GachaUseKeyKind == GACHA_USE_BRONZEKEY && keyItemNum < KeyNeedNum )
	{
		// 이벤트 키 개수를 체크한다.
		KeyItemUnique = GACHA_BRONZEKEY_EVENT_ITEMUNIQUE;
		keyItemNum	  =	pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( KeyItemUnique );
	}

	// 열쇠가 모자르다면..
	if( keyItemNum < KeyNeedNum )
	{
		return false;
	}
	else
	{
		return true;
	}

}

