#include "NContributionEvent_CommonDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/MsgType-Item.h"

SI32 NContributionEvent_CommonDlg::m_siControllID = -1;

NContributionEvent_CommonDlg::NContributionEvent_CommonDlg()
{
	m_bComplete = false;	
	m_siNPCKind = 0;
}


NContributionEvent_CommonDlg::~NContributionEvent_CommonDlg()
{
}

void NContributionEvent_CommonDlg::Create( SI16 _siControllID, CControllerMgr* _pChildControllerMgr, GUIChildCallBackEvent _pChildCallBack )
{
	if( IsCreate() )			return;
		
	if( 0 < m_siControllID ) 
		((cltClient*)pclClient)->m_pDialog[ m_siControllID ]->DeleteDialog();

	m_siControllID			= _siControllID;
	m_pChildControllerMgr	= _pChildControllerMgr;
	m_pChildCallBack		= _pChildCallBack;

	CInterfaceFile file;

	file.LoadFile("Ninterface/Data/NContributionEvent_CommonDlg/NContributionEvent_CommonDlg.ddf");

	file.CreatePopupDialog( this, _siControllID, "dialog_NContributionEvent", StaticNContributionEvent_CommonDlgProc );


	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT, NCONTRIBUTIONEVENT_COMMONDLG_EDIT_EXPLAIN,	this),  NCONTRIBUTIONEVENT_COMMONDLG_EDIT_EXPLAIN,	TEXT("editbox_explain")	);
	
	TCHAR szString[128];
	for( SI32 siCount = 0; CONTRIBUTION_ITEM_NUM > siCount; ++siCount )
	{
		StringCchPrintf( szString, sizeof(szString), "editbox_itemname%d",	siCount+1 );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT, 		NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNAME1+siCount,	this),  NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNAME1+siCount,	szString );
		
		StringCchPrintf( szString, sizeof(szString), "editbox_itemmaxnum%d", siCount+1 );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT, 		NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMMAXNUM1+siCount,	this),  NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMMAXNUM1+siCount,	szString );
		//m_InterfaceFactory.GetEdit( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMMAXNUM1+siCount )->SetTextPrintOption( DT_RIGHT );  
		m_InterfaceFactory.GetEdit( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMMAXNUM1+siCount )->Refresh();
		
		StringCchPrintf( szString, sizeof(szString), "editbox_itemnum%d",	siCount+1 );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT, 		NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+siCount,		this),  NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+siCount,		szString );
		//m_InterfaceFactory.GetEdit( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+siCount )->SetTextPrintOption( DT_RIGHT );  
		m_InterfaceFactory.GetEdit( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+siCount )->SetNumberMode( true ); 
		m_InterfaceFactory.GetEdit( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+siCount )->Refresh();  
		
		StringCchPrintf( szString, sizeof(szString), "listview_item%d",		siCount+1 );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(LISTVIEW, 	NCONTRIBUTIONEVENT_COMMONDLG_LISTVIEW_ITEM1+siCount,	this),  NCONTRIBUTIONEVENT_COMMONDLG_LISTVIEW_ITEM1+siCount,	szString );
	}

  	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 	NCONTRIBUTIONEVENT_COMMONDLG_BUTTON_CONSTRIBUTION,	this),  NCONTRIBUTIONEVENT_COMMONDLG_BUTTON_CONSTRIBUTION,	TEXT("button_contribution") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 	NCONTRIBUTIONEVENT_COMMONDLG_BUTTON_ACTION,			this),  NCONTRIBUTIONEVENT_COMMONDLG_BUTTON_ACTION,			TEXT("button_action") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,	NCONTRIBUTIONEVENT_COMMONDLG_OUTLINE1,				this),  NCONTRIBUTIONEVENT_COMMONDLG_OUTLINE1,				TEXT("outline_NONAME1") );

	// NCONTRIBUTIONEVENT_COMMONDLG_BUTTON_ACTION 액션버튼인데 그냥 다른 NPC로 만들어주는 버튼으로 사용한다. (확장시 사용하려고 했는데.. 그냥 하나의 고정된 처리를 하게함)
	m_InterfaceFactory.GetButton(NCONTRIBUTIONEVENT_COMMONDLG_BUTTON_ACTION)->Enable( false );
	SetContributeButtonText( GetTxtFromMgr(7652) );
}

void CALLBACK NContributionEvent_CommonDlg::StaticNContributionEvent_CommonDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NContributionEvent_CommonDlg *pThis = (NContributionEvent_CommonDlg *)pControllerMgr;
	pThis->NContributionEvent_CommonDlgProc( nEvent, nControlID, pControl );
}


void CALLBACK NContributionEvent_CommonDlg::NContributionEvent_CommonDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;
	SI32 siItemIndex = 0;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			m_siControllID = -1;
			m_pChildControllerMgr->DeleteDialog();
		}
		break;
	case NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:	
				{
					MakeItemNum( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1 );
				}
				break;
			}
		}
		break;
	case NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM2:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:	
				{
					MakeItemNum( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM2 );
				}
				break;
			}
		}
		break;
	case NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM3:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:	
				{
					MakeItemNum( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM3 );
				}
				break;
			}
		}
		break;
	case NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM4:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:	
				{
					MakeItemNum( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM4 );
				}
				break;
			}
		}
		break;
	case NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM5:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:	
				{
					MakeItemNum( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM5 );
				}
				break;
			}
		}
		break;
	case NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM6:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:	
				{
					MakeItemNum( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM6 );
				}
				break;
			}
		}
		break;
	case NCONTRIBUTIONEVENT_COMMONDLG_BUTTON_CONSTRIBUTION: 
		{
			switch( nEvent )
			{
				case EVENT_BUTTON_CLICKED:	
					{
						if( m_pChildCallBack )
						{
							m_pChildCallBack( m_pChildControllerMgr, EVENT_CONTRIBUTION_BUTTON_CLICK );
						}

						InputItem();
					}
			}
		}
		break;
	case NCONTRIBUTIONEVENT_COMMONDLG_BUTTON_ACTION: 
		{
			switch( nEvent )
			{
				case EVENT_BUTTON_CLICKED:	
					{
						if( m_pChildCallBack )
						{
							m_pChildCallBack( m_pChildControllerMgr, EVENT_ACTION_BUTTON_CLICK );
						}

						MakeNPC();
						
						
						m_siControllID = -1;
						m_pChildControllerMgr->DeleteDialog();
					}
			}
		}
		break;
				
	default:
		{

		}
		break;
	}

	
}

bool NContributionEvent_CommonDlg::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	static cltClient* pclclient = (cltClient*)pclClient;

	if( NULL == pclClickAndDrop )									return true;
	if( pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false )	return true;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;
	if( NULL == pclClickAndDrop_Item ) return true;

	switch ( pclClickAndDrop->m_siDstControlID )
	{
		default:	break;
	}

    return true;
}

void NContributionEvent_CommonDlg::SetExplainText( TCHAR* pszString )
{
	m_InterfaceFactory.GetEdit(NCONTRIBUTIONEVENT_COMMONDLG_EDIT_EXPLAIN)->SetText( pszString );
}

void NContributionEvent_CommonDlg::SetContributeButtonText( TCHAR* pszString )
{
	m_InterfaceFactory.GetButton(NCONTRIBUTIONEVENT_COMMONDLG_BUTTON_CONSTRIBUTION)->SetText( pszString );
}

void NContributionEvent_CommonDlg::SetActionButtonText( TCHAR* pszString )
{
	m_InterfaceFactory.GetButton(NCONTRIBUTIONEVENT_COMMONDLG_BUTTON_ACTION)->SetText( pszString );
}

void NContributionEvent_CommonDlg::StaticSetContributionItemList( SI32* _psiItemUnique,  SI16* _psiItemNum )
{
	static cltClient* pclclient = (cltClient*)pclClient;

	if( 0 > m_siControllID ) return;

	if( pclclient->m_pDialog[m_siControllID] )
		((NContributionEvent_CommonDlg*)pclclient->m_pDialog[m_siControllID])->SetContributionItemList( _psiItemUnique, _psiItemNum );
}

void NContributionEvent_CommonDlg::StaticSetContributionInputItemList( SI32* _psiInputItemUnique,  SI16* _psiInputItemNum )
{
	static cltClient* pclclient = (cltClient*)pclClient;

	if( 0 > m_siControllID ) return;

	if( pclclient->m_pDialog[m_siControllID] )
		((NContributionEvent_CommonDlg*)pclclient->m_pDialog[m_siControllID])->SetContributionInputItemList( _psiInputItemUnique, _psiInputItemNum );
}

void NContributionEvent_CommonDlg::SetContributionItemList( SI32* _psiItemUnique,  SI16* _psiItemNum )
{
	memcpy( m_siItemUnique, _psiItemUnique, sizeof(m_siItemUnique) );
	memcpy( m_siItemNum,	_psiItemNum,	sizeof(m_siItemNum) );

	cltItem clItem;
	for( SI32 siCount = 0; CONTRIBUTION_ITEM_NUM > siCount; ++siCount )
	{
		if( 0 >= m_siItemUnique[siCount] ) break;

		m_InterfaceFactory.GetEdit(NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNAME1+siCount)->SetText( (TCHAR*)pclClient->pclItemManager->GetName( m_siItemUnique[siCount] ) );
		m_InterfaceFactory.GetEdit(NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMMAXNUM1+siCount)->SetText( SI32ToString( m_siItemNum[siCount] ) );

		clItem.Init();
		clItem.siUnique  = m_siItemUnique[siCount];
		clItem.siItemNum = m_siItemNum[siCount];

		SI32	GlobalImageIndex = -1;
		SI32	GlobalImageFont	 = -1;
		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_siItemUnique[siCount], &GlobalImageIndex, &GlobalImageFont ) )
		{
			CListView* pListView = m_InterfaceFactory.GetListView( NCONTRIBUTIONEVENT_COMMONDLG_LISTVIEW_ITEM1 + siCount );
			pListView->DeleteAllItems();

			TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
			pclClient->pclItemManager->GetExplain( &clItem, ToolTipMsg );

			TCHAR* pItemName = (TCHAR*)pclClient->pclItemManager->GetName( clItem.siUnique );

			stListViewItemData	ListViewItemData;
			ListViewItemData.Init();
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	clItem.siItemNum, pItemName, ToolTipMsg );
			pListView->InsertItem( &ListViewItemData );
		}
	}
}

void NContributionEvent_CommonDlg::GetContributionItemList( SI32 _siNPCKind )
{
	m_siNPCKind = _siNPCKind;

	cltGameMsgRequest_Contribution_Item_List cltContributionItemList( m_siNPCKind );
	cltMsg clMsg( GAMEMSG_REQUEST_CONTRIBUTION_ITEM_LIST, PACKET(cltContributionItemList) );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg ); 
}

void NContributionEvent_CommonDlg::SetContributionInputItemList( SI32* _psiInputItemUnique,  SI16* _psiInputItemNum )
{
	_psiInputItemUnique ? memcpy( m_siInputItemUnique,	_psiInputItemUnique,	sizeof(m_siInputItemUnique) )	: 0;
	_psiInputItemNum	? memcpy( m_siInputItemNum,		_psiInputItemNum,		sizeof(m_siInputItemNum) )		: 0;
	
	cltItem clItem;
	for( SI32 siCount = 0; CONTRIBUTION_ITEM_NUM > siCount; ++siCount )
	{
		if( 0 >= m_siItemUnique[siCount] )								break;	
		if( m_siItemUnique[siCount] != m_siInputItemUnique[siCount] )	return;

		m_InterfaceFactory.GetEdit(NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNAME1+siCount)->SetText( (TCHAR*)pclClient->pclItemManager->GetName( m_siItemUnique[siCount] ) );
		m_InterfaceFactory.GetEdit(NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMMAXNUM1+siCount)->SetText( SI32ToString( m_siItemNum[siCount] - m_siInputItemNum[siCount] ) );

		clItem.Init();
		clItem.siUnique  = m_siItemUnique[siCount];
		clItem.siItemNum = m_siItemNum[siCount];

		SI32	GlobalImageIndex = -1;
		SI32	GlobalImageFont	 = -1;
		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_siItemUnique[siCount], &GlobalImageIndex, &GlobalImageFont ) )
		{
			CListView* pListView = m_InterfaceFactory.GetListView( NCONTRIBUTIONEVENT_COMMONDLG_LISTVIEW_ITEM1 + siCount );
			pListView->DeleteAllItems();

			TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
			pclClient->pclItemManager->GetExplain( &clItem, ToolTipMsg );

			TCHAR* pItemName = (TCHAR*)pclClient->pclItemManager->GetName( clItem.siUnique );

			stListViewItemData	ListViewItemData;
			ListViewItemData.Init();
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	clItem.siItemNum, pItemName, ToolTipMsg );
			pListView->InsertItem( &ListViewItemData );
		}
	}
	
	SetContributionUserItemList();
}

void NContributionEvent_CommonDlg::GetContributionInputItemList( SI32 NPCKind )
{

}

void NContributionEvent_CommonDlg::SetContributionUserItemList( void )
{
	m_bComplete = true;	// 기부 완성여부.

	SI32			siInvenPos = 0;
	SI32			siItemNum  = 0;
	cltPItemInfo*	pclCharItem = &pclClient->pclCM->GetCharClient(1)->pclCI->clCharItem;
	
	for( SI32 siCount = 0; CONTRIBUTION_ITEM_NUM > siCount; ++siCount )
	{
		if( 0 >= m_siItemUnique[siCount] || m_siItemUnique[siCount] != m_siInputItemUnique[siCount] ) break; 
		
		siInvenPos = pclCharItem->FindItemInv( m_siItemUnique[siCount] );
		if( 0 > siInvenPos ) 
		{
			if( 0 < m_siItemNum[siCount] - m_siInputItemNum[siCount] ) 
				m_bComplete = false;
			
			m_InterfaceFactory.GetEdit(NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+siCount)->SetText("0");
			continue;
		}
		
		siItemNum = min( pclCharItem->clItem[siInvenPos].siItemNum, m_siItemNum[siCount] - m_siInputItemNum[siCount] );
		if( 0 < siItemNum ) m_bComplete = false;

		m_InterfaceFactory.GetEdit(NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+siCount)->SetText( SI32ToString( siItemNum ) );
	}

	if( true == m_bComplete )
	{
		if( m_pChildCallBack )
		{
			m_pChildCallBack( m_pChildControllerMgr, EVENT_ACTION_CONTRIBUTION_COMPLETE );
		}

		// 다른 NPC로 만들어 줄 버튼을 활성화 시킨다.
		m_InterfaceFactory.GetButton(NCONTRIBUTIONEVENT_COMMONDLG_BUTTON_ACTION)->Enable( true );
	}
}

void NContributionEvent_CommonDlg::InputItem( void )
{
	if( 0 >= m_siNPCKind ) return;

	SI16 m_siInputItemNum[CONTRIBUTION_ITEM_NUM];
	ZeroMemory( m_siInputItemNum, sizeof(m_siInputItemNum) );
	
	SI32 m_siInputItemInvenPos[CONTRIBUTION_ITEM_NUM];
	ZeroMemory( m_siInputItemInvenPos, sizeof(m_siInputItemInvenPos) );

	for( SI32 siCount = 0; CONTRIBUTION_ITEM_NUM > siCount; ++siCount )
	{
		if( 0 >= m_siItemUnique[siCount] ) break;

		if( 0 == _tcslen(m_InterfaceFactory.GetEdit( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+siCount )->GetText()) )  
			m_InterfaceFactory.GetEdit( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+siCount )->SetText( "0" );
		
		m_siInputItemNum[siCount] = _ttoi( m_InterfaceFactory.GetEdit(NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+siCount)->GetText() );
	}

	cltClient*		pclclient	= (cltClient *)pclClient;
	SI32			siInvenPos  = 0;
	cltPItemInfo*	pclCharItem = &pclClient->pclCM->GetCharClient(1)->pclCI->clCharItem;
	for( SI32 siCount = 0; CONTRIBUTION_ITEM_NUM > siCount; ++siCount )
	{
		if( 0 >= m_siItemUnique[siCount] ) break;

		if( m_siInputItemNum[siCount] )
		{
			m_siInputItemInvenPos[siCount] = pclCharItem->FindItemInv( m_siItemUnique[siCount] );
			if( 0 > m_siInputItemInvenPos[siCount] ) 
			{
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(40811));
				return;
			}

			if( pclCharItem->clItem[m_siInputItemInvenPos[siCount]].siItemNum < m_siInputItemNum[siCount] )
			{
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(7656));
				return;
			}
		}
	}

	cltGameMsgRequest_Contribution_Item_Input ContributionItemInput( m_siNPCKind, m_siItemUnique, m_siInputItemNum, m_siInputItemInvenPos );
	cltMsg clMsg( GAMEMSG_REQUEST_CONTRIBUTION_ITEM_INPUT, PACKET(ContributionItemInput) );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg ); 
}


bool NContributionEvent_CommonDlg::IsComplete( void )
{
	return m_bComplete;
}

void NContributionEvent_CommonDlg::MakeNPC( void )
{
	cltGameMsgRequest_Contribution_Make_NPC	ContributionMakeNPC( m_siNPCKind );
	cltMsg clMsg( GAMEMSG_REQUEST_CONTRIBUTION_MAKE_NPC, PACKET(ContributionMakeNPC) );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg ); 
}


void NContributionEvent_CommonDlg::MakeItemNum( SI32 _siIndex )
{
	_siIndex -= NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1;

	if( 0 >= m_siItemUnique[_siIndex] ) 
		m_InterfaceFactory.GetEdit( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+_siIndex )->SetText( "0" );


	cltPItemInfo*	pclCharItem		= &pclClient->pclCM->GetCharClient(1)->pclCI->clCharItem;
	SI32			siItemInvenPos	= -1;
		
	siItemInvenPos = pclCharItem->FindItemInv( m_siItemUnique[_siIndex] );
	if( 0 > siItemInvenPos ) 
	{
		m_InterfaceFactory.GetEdit( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+_siIndex )->SetText( "0" );
	}
	else
	{
		SI32 siItemNum = min( pclCharItem->clItem[siItemInvenPos].siItemNum, m_siItemNum[_siIndex] - m_siInputItemNum[_siIndex] );
		
		if( siItemNum < _ttoi( m_InterfaceFactory.GetEdit( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+_siIndex )->GetText() ) )
		{
			m_InterfaceFactory.GetEdit( NCONTRIBUTIONEVENT_COMMONDLG_EDIT_ITEMNUM1+_siIndex )->SetText( SI16ToString( siItemNum ) );
		}
	}

	
	
}

