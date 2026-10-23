/* ==========================================================================
	클래스 :		CNNotifyGetExpAndItemDlg

	작성일 :		05/05/10
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/


#include "./NNotifyGetExpAndItemDlg.h"

#include "..\..\..\Common\Item\ItemCommon\ItemUnit.h"
#include "../../Client.h"

#include <CommonLogic.h>

#include "../../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../../Client/InterfaceMgr/Interface/ListView.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

extern cltCommonLogic* pclClient;


CNNotifyGetExpAndItemDlg::CNNotifyGetExpAndItemDlg()
{
	m_siCount = 0;
	m_bShow = false;

	m_pListView_item_image = NULL;
	m_pEdit_explain = NULL;	
}

CNNotifyGetExpAndItemDlg::~CNNotifyGetExpAndItemDlg()
{
	if(m_pListView_item_image)
		delete m_pListView_item_image;

	if(m_pEdit_explain)
		delete m_pEdit_explain;
}

void CNNotifyGetExpAndItemDlg::Create( )
{
	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient *)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NNotifyGetExpAndItemDlg/DLG_NNotifyGetExpAndItem.ddf"));

		file.CreatePopupDialog( this, NNOTIFYGETEXPANDITEM_DLG, TEXT("dialog_notifygetexpanditem"), NNNotifyGetExpAndItemStaticProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_notifygetexpanditem") );

		if(siTmp < 0 )
			siTmpWidth = DLG_BIG_POS_X;
		else
			siTmpWidth = file.m_control[ siTmp  ].width;

		if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
		{
			this->MovePos(/*DLG_BIG_POS_X -*/ siTmpWidth, DLG_BIG_POS_Y);
		}
		else
		{
			this->MovePos(/*DLG_SMALL_POS_X -*/ siTmpWidth, DLG_SMALL_POS_Y);
		}

		m_pListView_item_image	 = new 	CListView	( this )	;
		m_pEdit_explain	 = new 	CEdit	( this )	;

		file.CreateControl( 	m_pListView_item_image	, 	NNOTIFYGETEXPANDITEM_LISTVIEW_ITEM_IMAGE	, TEXT("listview_item_image") )	;
		file.CreateControl( 	m_pEdit_explain	, 	NNOTIFYGETEXPANDITEM_EDIT_EXPLAIN	, TEXT("editbox_explain") )	;

	}
	else
	{
		DeleteDialog();
	}

	return;
}

void CALLBACK CNNotifyGetExpAndItemDlg::NNNotifyGetExpAndItemStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
    CNNotifyGetExpAndItemDlg *pThis = (CNNotifyGetExpAndItemDlg*)pControllerMgr;
	pThis->NNotifyGetExpAndItemProc( nEvent, nControlID, pControl );
}

void CALLBACK CNNotifyGetExpAndItemDlg::NNotifyGetExpAndItemProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}
}

void CNNotifyGetExpAndItemDlg::Show( cltItem *pclItem, SI32 Exp, SI32 GP, GMONEY Money )
{
	if ( pclItem == NULL )
	{
		return;
	}	
	
	TCHAR szString[ 512 ] = TEXT("");
	TCHAR itemname[128] = TEXT("");

	if ( Money > 0 )
	{
		TCHAR* pText = GetTxtFromMgr(10054);
		StringCchPrintf( szString, 256, pText, Money, Exp );
	}
	else if(pclItem->GetFullName(pclClient->pclItemManager , itemname, 128) == true)
	{
		m_siCount = 0;

		SetNInventoryInListView( m_pListView_item_image, pclItem );

		TCHAR num[128]= TEXT(" ") ;
		TCHAR exp[128]= TEXT(" ") ;
		TCHAR* pText = GetTxtFromMgr(1890);
		StringCchPrintf( num, 128, TEXT("%d"), pclItem->GetItemNum() );
		StringCchPrintf( exp, 128, TEXT("%d"), Exp );
		Nsprintf(szString,pText,TEXT("item"),  itemname  ,TEXT("num"),  num  ,TEXT("exp"),  exp  ,NULL);
		//sprintf( szString, pText, itemname, pclItem->GetItemNum(), Exp );
	}

	if ( GP > 0 )
	{
		TCHAR buf[128] = {0,};
		TCHAR* pText = GetTxtFromMgr(6892);
		StringCchPrintf( buf, 128, pText, GP );
		StringCchCat( szString, 512, buf );
	}

	m_pEdit_explain->SetText( szString );

	Show( ); // m_bShow = true;

	return;
}

void CNNotifyGetExpAndItemDlg::Show()
{
	m_bShow = true;

	CDialogController::Show( true );
}

void CNNotifyGetExpAndItemDlg::Hide()
{
	m_siCount = 0;
	m_bShow = false;

	CDialogController::Show( false );
}

bool CNNotifyGetExpAndItemDlg::IsShow()
{
	return m_bShow;
}

void CNNotifyGetExpAndItemDlg::SetNInventoryInListView(CListView* in_pListView, cltItem* in_pCltItem )
{
	SI16 StartPos = 0;

	SI16 i = 0, j = 0;

	SI32 id = 1;

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;

	in_pListView->DeleteAllItems();

	StartPos = 0;/*PERSONITEM_INV0 + ( j * MAX_ITEM_PER_INVENTORY );*/
	in_pListView->SetStartItemPos( StartPos );

	if ( in_pCltItem->siUnique > 0 )
	{
		if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( in_pCltItem->siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( in_pCltItem->siUnique );

			if ( ItemName )
			{
				TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
				pclClient->pclItemManager->GetExplain( in_pCltItem, ToolTipMsg );

				stListViewItemData ListViewItemData;
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, /*i - StartPos*/0, in_pCltItem->siItemNum, ItemName, ToolTipMsg );
				in_pListView->SetStartItemPos( 0 );
				in_pListView->InsertItem( &ListViewItemData );
			}
		}
	}
}
void CNNotifyGetExpAndItemDlg::Show2( cltItem *pclItem, SI32 Exp, SI32 GP, GMONEY Money )
{
	if ( pclItem == NULL )
	{
		return;
	}	

	TCHAR szString[ 512 ] = TEXT("");
	TCHAR itemname[128] = TEXT("");

	if ( Money > 0 )
	{
		TCHAR* pText = GetTxtFromMgr(8098);
		StringCchPrintf( szString, 256, pText, Money, Exp );
	}
	else if(pclItem->GetFullName(pclClient->pclItemManager , itemname, 128) == true)
	{
		m_siCount = 0;

		SetNInventoryInListView( m_pListView_item_image, pclItem );

		TCHAR num[128]= TEXT(" ") ;
		TCHAR exp[128]= TEXT(" ") ;
		TCHAR* pText = GetTxtFromMgr(8097);
		StringCchPrintf( num, 128, TEXT("%d"), pclItem->GetItemNum() );
		StringCchPrintf( exp, 128, TEXT("%d"), Exp );
		Nsprintf(szString,pText,TEXT("item"),  itemname  ,TEXT("num"),  num  ,TEXT("exp"),  exp  ,NULL);
		//sprintf( szString, pText, itemname, pclItem->GetItemNum(), Exp );
	}

	if ( GP > 0 )
	{
		TCHAR buf[128] = {0,};
		TCHAR* pText = GetTxtFromMgr(6892);
		StringCchPrintf( buf, 128, pText, GP );
		StringCchCat( szString, 512, buf );
	}

	m_pEdit_explain->SetText( szString );

	Show( ); // m_bShow = true;

	return;
}

void CNNotifyGetExpAndItemDlg::MonsterAttackReward( SI32 ItemUnique, SI32 ItemNum, SI32 AddVillageScore )
{
	if (0 >= ItemUnique || 0 >= ItemNum)
	{
		return;
	}	

	NTCHARString64	kTitle( GetTxtFromMgr( 5614 ) );
	NTCHARString128	kText(  GetTxtFromMgr( 8119 ) );
	
	SetTitle(kTitle);

	kText.Replace("#item#",			 pclClient->pclItemManager->GetName(ItemUnique));
	kText.Replace("#num#",			 SI32ToString(ItemNum));
	kText.Replace("#villagepoint#",  SI32ToString(AddVillageScore));

	m_pEdit_explain->SetText(kText);

	cltItem clItem;
	clItem.siUnique  = ItemUnique;
	clItem.siItemNum = ItemNum;
	SetNInventoryInListView( m_pListView_item_image, &clItem );

	return;
}
void CNNotifyGetExpAndItemDlg::ShowDailyQuest2( cltItem *pclItem, SI32 Exp, SI32 GP, GMONEY Money )
{
	if ( pclItem == NULL )
	{
		return;
	}
	TCHAR szString[ 512 ] = TEXT("");
	TCHAR itemname[128] = TEXT("");

	if(pclItem->GetFullName(pclClient->pclItemManager , itemname, 128) == true) {
		m_siCount = 0;
		SetNInventoryInListView( m_pListView_item_image, pclItem );
		TCHAR num[128]= TEXT(" ") ;
		TCHAR exp[128]= TEXT(" ") ;
		TCHAR* pText = GetTxtFromMgr(8369);
		StringCchPrintf( num, 128, TEXT("%d"), pclItem->GetItemNum() );
		StringCchPrintf( exp, 128, TEXT("%d"), Exp );
		Nsprintf(szString,pText,TEXT("item"),  itemname  ,TEXT("num"),  num  ,TEXT("exp"),  exp ,NULL);
		//sprintf( szString, pText, itemname, pclItem->GetItemNum(), Exp );
		if ( GP > 0 )
		{
			TCHAR buf[128] = {0,};
			TCHAR* pText = GetTxtFromMgr(6892);
			StringCchPrintf( buf, 128, pText, GP );
			StringCchCat( szString, 512, buf );
		}

		m_pEdit_explain->SetText( szString );

		Show( ); // m_bShow = true;

		return;
	}
}

void CNNotifyGetExpAndItemDlg::ShowDailyQuest3( cltItem *pclItem, SI32 Exp, SI32 GP, GMONEY Money )
{
	if ( pclItem == NULL )
	{
		return;
	}
	TCHAR szString[ 512 ] = TEXT("");
	TCHAR itemname[128] = TEXT("");
	if(pclItem->GetFullName(pclClient->pclItemManager , itemname, 128) == true) {
		m_siCount = 0;
		SetNInventoryInListView( m_pListView_item_image, pclItem );
		TCHAR num[128]= TEXT(" ") ;
		TCHAR exp[128]= TEXT(" ") ;
		TCHAR* pText = GetTxtFromMgr(8370);
		StringCchPrintf( num, 128, TEXT("%d"), pclItem->GetItemNum() );
		StringCchPrintf( exp, 128, TEXT("%d"), Exp );
		Nsprintf(szString,pText,TEXT("item"),  itemname  ,TEXT("num"),  num  ,TEXT("exp"),  exp, NULL);
		//sprintf( szString, pText, itemname, pclItem->GetItemNum(), Exp );
		if ( GP > 0 )
		{
			TCHAR buf[128] = {0,};
			TCHAR* pText = GetTxtFromMgr(6892);
			StringCchPrintf( buf, 128, pText, GP );
			StringCchCat( szString, 512, buf );
		}
		m_pEdit_explain->SetText( szString );
		Show( ); // m_bShow = true;
		return;
	}
}

