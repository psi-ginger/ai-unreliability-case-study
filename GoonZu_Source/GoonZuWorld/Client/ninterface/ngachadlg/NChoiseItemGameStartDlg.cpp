#include "NChoiseItemGameStartDlg.h"
#include "NOpenBoxDlg.h"
#include "NGachaDlgNew.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Msg/msgType-Gacha.h"
#include "../../../common/Char/CharManager/CharManager.h"
#include "../../../Client/Music/music.h"

#include "../../lib/MD5/md5.h"
#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"

CNChoiseItemGameStartDlg::CNChoiseItemGameStartDlg()
{

}

CNChoiseItemGameStartDlg::~CNChoiseItemGameStartDlg()
{
	Destroy();
}


void CNChoiseItemGameStartDlg::Destroy()
{

}

void CNChoiseItemGameStartDlg::Create()
{
	if( IsCreate() )
	{
		return;
	}

	CInterfaceFile file;

	file.LoadFile("Ninterface/Data/NGachaDlg/DLG_NChoiseItemGameStartDlg.ddf");
	file.CreatePopupDialog( this , NCHOISE_ITEM_GAME_START_DLG, "dialog_NChoiseItemGameStart" , StaticNChoiseItemGameStartDlgProc );

	if( g_bFullScreen)	MovePos( 80, 40 );
	else		 		MovePos( 45, 20 );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NCHOISE_ITEM_GAME_START_BUTTON_START,			this), NCHOISE_ITEM_GAME_START_BUTTON_START,			TEXT("button_choise_item_gamestart"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NCHOISE_ITEM_GAME_START_BUTTON_CLOSE,			this), NCHOISE_ITEM_GAME_START_BUTTON_CLOSE,			TEXT("button_close"));


	TCHAR szChoiseItem[128] = "";
	for(SI32 Count = 0; Count < MAX_CHOISE_ITEM; ++Count)
	{
		StringCchPrintf(szChoiseItem, sizeof(szChoiseItem),  "button_item%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM1 + Count,	this), NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM1 + Count, szChoiseItem);

		StringCchPrintf(szChoiseItem, sizeof(szChoiseItem),  "editbox_itemname%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT, NCHOISE_ITEM_GAME_START_EDIT_ITEMNAME0 + Count,	this), NCHOISE_ITEM_GAME_START_EDIT_ITEMNAME0 + Count, szChoiseItem);
		m_InterfaceFactory.GetEdit(NCHOISE_ITEM_GAME_START_EDIT_ITEMNAME0 + Count)->SetTextPrintOption(DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		m_InterfaceFactory.GetEdit(NCHOISE_ITEM_GAME_START_EDIT_ITEMNAME0 + Count)->Refresh(); 
	}

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NCHOISE_ITEM_GAME_START_EDIT_EXPLAIN,			this), NCHOISE_ITEM_GAME_START_EDIT_EXPLAIN,			TEXT("editbox_explain"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC, NCHOISE_ITEM_GAME_START_IMAGESTATIC_BACKGROUND,	this), NCHOISE_ITEM_GAME_START_IMAGESTATIC_BACKGROUND,	TEXT("imagestatic_background"));

	m_InterfaceFactory.GetEdit(NCHOISE_ITEM_GAME_START_EDIT_ITEMNAME0)->SetText((TCHAR*)pclClient->pclItemManager->GetName(GACHA_GOLDKEY_ITEMUNIQUE));
	m_InterfaceFactory.GetEdit(NCHOISE_ITEM_GAME_START_EDIT_ITEMNAME1)->SetText((TCHAR*)pclClient->pclItemManager->GetName(GACHA_SILVERKEY_ITEMUNIQUE)); 
	m_InterfaceFactory.GetEdit(NCHOISE_ITEM_GAME_START_EDIT_ITEMNAME2)->SetText((TCHAR*)pclClient->pclItemManager->GetName(GACHA_BRONZEKEY_ITEMUNIQUE));
	m_InterfaceFactory.GetEdit(NCHOISE_ITEM_GAME_START_EDIT_ITEMNAME3)->SetText((TCHAR*)pclClient->pclItemManager->GetName(GACHA_KEYPIECE_ITEMUNIQUE));

	m_InterfaceFactory.GetEdit(NCHOISE_ITEM_GAME_START_EDIT_EXPLAIN)->SetText(GetTxtFromMgr(7079));
	m_InterfaceFactory.GetEdit(NCHOISE_ITEM_GAME_START_EDIT_EXPLAIN)->SetFontSize( 15 ); 
	m_InterfaceFactory.GetEdit(NCHOISE_ITEM_GAME_START_EDIT_EXPLAIN)->SetTextPrintOption(DT_CENTER);
	m_InterfaceFactory.GetEdit(NCHOISE_ITEM_GAME_START_EDIT_EXPLAIN)->Refresh(); 

	if( CheckHaveKey( GACHA_USE_GOLDKEY ) )
		m_InterfaceFactory.GetButton( NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM1 )->Enable( true );
	else
		m_InterfaceFactory.GetButton( NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM1 )->Enable( false );

	if( CheckHaveKey( GACHA_USE_SILVERKEY )	)
		m_InterfaceFactory.GetButton( NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM2 )->Enable( true );
	else
		m_InterfaceFactory.GetButton( NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM2 )->Enable( false );

	if( CheckHaveKey( GACHA_USE_BRONZEKEY )	)
		m_InterfaceFactory.GetButton( NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM3 )->Enable( true );
	else
		m_InterfaceFactory.GetButton( NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM3 )->Enable( false );

	if( CheckHaveKey( GACHA_USE_KEYPIECE )	)
		m_InterfaceFactory.GetButton( NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM4 )->Enable( true );
	else
		m_InterfaceFactory.GetButton( NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM4 )->Enable( false );

	SetTopMostWindow( true );

	cltClient * pclclient = (cltClient*) pclClient;
	if(pclclient->m_pDialog[NGACHANEW_DLG])
		((CNGachaDlgNew*)pclclient->m_pDialog[NGACHANEW_DLG])->SetExplain2(GetTxtFromMgr(7078));
}

void CALLBACK CNChoiseItemGameStartDlg::StaticNChoiseItemGameStartDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNChoiseItemGameStartDlg *pThis = (CNChoiseItemGameStartDlg *)pControllerMgr;
	pThis->NChoiseItemGameStartDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK CNChoiseItemGameStartDlg::NChoiseItemGameStartDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case NCHOISE_ITEM_GAME_START_BUTTON_CLOSE:
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			cltClient * pclclient = (cltClient*) pclClient;
			if(pclclient->m_pDialog[NGACHANEW_DLG])
				((CNGachaDlgNew*)pclclient->m_pDialog[NGACHANEW_DLG])->SetExplain2(GetTxtFromMgr(7022));

			DeleteDialog();
		}
		break;
	case NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM1:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( true == CheckHaveKey( GACHA_USE_GOLDKEY ) )
					{
						m_siGachaUseKeyKind = GACHA_USE_GOLDKEY;
					}
				}
				break;
			}
		}
		break;
	case NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM2:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( true == CheckHaveKey( GACHA_USE_SILVERKEY ) )
					{
						m_siGachaUseKeyKind = GACHA_USE_SILVERKEY;
					}
				}
				break;
			}
		}
		break;
	case NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM3:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( true == CheckHaveKey( GACHA_USE_BRONZEKEY ) )
					{
						m_siGachaUseKeyKind = GACHA_USE_BRONZEKEY;
					}
				}
				break;
			}
		}
		break;
	case NCHOISE_ITEM_GAME_START_BUTTON_GIVEITEM4:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( true == CheckHaveKey( GACHA_USE_KEYPIECE ) )
					{
						m_siGachaUseKeyKind = GACHA_USE_KEYPIECE;
					}
				}
				break;
			}
		}
		break;
	case NCHOISE_ITEM_GAME_START_BUTTON_START:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( true == CheckHaveKey( m_siGachaUseKeyKind ) )
					{			
						if ( NULL == pclclient->m_pDialog[NOPENBOX] )
						{					
							pclclient->CreateInterface( NOPENBOX );
							((CNOpenBoxDlg*)pclclient->m_pDialog[NOPENBOX])->SetGachaUseKeyKind( m_siGachaUseKeyKind );
							((CNGachaDlgNew*)pclclient->m_pDialog[NGACHANEW_DLG])->SetGachaUseKeyKind( m_siGachaUseKeyKind );
							DeleteDialog();
						}
					}	
				}
				break;
			}
		}
		break;
	default:
		{
		}
		break;
	}

}

void CNChoiseItemGameStartDlg::Action()
{


}

void CNChoiseItemGameStartDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if( ! IsShow() )
		return;

	cltClient *pclclient = ( cltClient * )pclClient;
}


void CNChoiseItemGameStartDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if( ! IsShow() )
		return;

	cltClient *pclclient = ( cltClient * )pclClient;


}

void CNChoiseItemGameStartDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{


}


bool CNChoiseItemGameStartDlg::CheckHaveKey( SI32 GachaUseKeyKind )
{
	//사운드
	pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_STATUE_BUFF")), 0, 0);

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

void CNChoiseItemGameStartDlg::SetKeyItem( void )
{
	CListView*	pListView   = NULL;
	CEdit*		pEditText = NULL;
	TCHAR*		pItemName   = NULL;

	SI32			GlobalImageIndex = -1;
	SI32			GlobalImageFont	 = -1;

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;

	cltItem clItem[MAX_CHOISE_ITEM];
	ZeroMemory( clItem, sizeof(clItem) );
	clItem[0].siUnique = GACHA_GOLDKEY_ITEMUNIQUE; 
	clItem[1].siUnique = GACHA_SILVERKEY_ITEMUNIQUE;
	clItem[2].siUnique = GACHA_BRONZEKEY_ITEMUNIQUE;
	clItem[3].siUnique = GACHA_KEYPIECE_ITEMUNIQUE;

	for( SI32 Count = 0 ; Count < MAX_CHOISE_ITEM; Count++ )
	{
		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( clItem[Count].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			pclClient->pclItemManager->GetExplain( &clItem[Count], ToolTipMsg );
			pItemName = (TCHAR*)pclClient->pclItemManager->GetName( clItem[Count].siUnique );

			ListViewItemData.Init();
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	clItem[Count].siItemNum, pItemName, ToolTipMsg );

			pListView = m_InterfaceFactory.GetListView( NGACHANEW_LISTVIEW_GIVEITEM1 + Count );
			pListView->DeleteAllItems();
			pListView->InsertItem( &ListViewItemData );   

			pEditText  = m_InterfaceFactory.GetEdit( NGACHANEW_EDIT_GIVEITEM1 + Count );
			pEditText->SetText( pItemName );
		}
	}
}