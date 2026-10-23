
#include "..\..\Server\Server.h"
#include "Char\CharManager\CharManager.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Market.h"
//
#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-HorseMarket.h"

#include "NHorseMarketDlg.h"

#include <tchar.h>
#include "../../InterfaceMgr/Interface/BUTTON.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../../CommonLogic/MsgType-Structure.h"

#include "../../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

NHorseMarketBuyDlg::NHorseMarketBuyDlg()
{
}

NHorseMarketBuyDlg::~NHorseMarketBuyDlg()
{
}

void NHorseMarketBuyDlg::InitDlg()
{

	SetControlMap(	BUTTON_BUY	,TEXT("button_buy"));
	SetControlMap(	BUTTON_REFRESH	,TEXT("button_refresh"));
	SetControlMap(	LISTBOX_HORSE	,TEXT("listbox_horse"));
	SetControlMap(	EDITBOX_EXPLAIN	,TEXT("editbox_explain"));

	SetControlMap(	COMBOBOX_KIND	,TEXT("combobox_kind"));
	SetControlMap(	COMBOBOX_PRICE	,TEXT("combobox_price"));

	CComboBox *pComboKind = (CComboBox *)m_pControlMap[ COMBOBOX_KIND ];
	CComboBox *pComboPrice = (CComboBox *)m_pControlMap[ COMBOBOX_PRICE ];
	stComboBoxItemData itemdata;


	// 사복시 매니져
	cltHorseMarketManager *pHorseMarketManager = ((cltHorseMarketManager *)m_pStrManager);
	if( pHorseMarketManager == NULL ) return;

	pHorseMarketManager->siHorseUnique = 1;
	pHorseMarketManager->siHorsePriceType = 0;
	
	pHorseMarketManager->siSelectedIndex = -1;

	pHorseMarketManager->bShowResponseOrderSwitch = FALSE;

	int i;	


	// horse type 
	for( i = 0; i < MAX_HORSE_TYPE_INFO; i++ )
	{
		cltHorseTypeInfo *pclHorseTypeInfo = pclClient->pclHorseManager->pclHorseTypeInfo[ i ];
		if( pclHorseTypeInfo ) {
			
			switch(pclClient->siServiceArea)
			{
			case ConstServiceArea_Korea:	if( (pclHorseTypeInfo->siServiceArea & SERVICEAREA_ATB_KOREA)		== 0 )	continue;	break;
			case ConstServiceArea_China:	if( (pclHorseTypeInfo->siServiceArea & SERVICEAREA_ATB_CHINA)		== 0 )	continue;	break;
			case ConstServiceArea_Japan:	if( (pclHorseTypeInfo->siServiceArea & SERVICEAREA_ATB_JAPAN)		== 0 )	continue;	break;
			case ConstServiceArea_English:	if( (pclHorseTypeInfo->siServiceArea & SERVICEAREA_ATB_ENGLISH)		== 0 )	continue;	break;
			case ConstServiceArea_Taiwan:	if( (pclHorseTypeInfo->siServiceArea & SERVICEAREA_ATB_TAIWAN)		== 0 )	continue;	break;
			case ConstServiceArea_USA:		if( (pclHorseTypeInfo->siServiceArea & SERVICEAREA_ATB_USA)			== 0 )	continue;	break;
			case ConstServiceArea_NHNChina:	if( (pclHorseTypeInfo->siServiceArea & SERVICEAREA_ATB_NHNCHINA)	== 0 )	continue;	break;
			case ConstServiceArea_EUROPE:	if( (pclHorseTypeInfo->siServiceArea & SERVICEAREA_ATB_EUROPE)		== 0 )	continue;	break;
			}

			if( pclHorseTypeInfo->bNotTrade )				continue;
			
			itemdata.Init();
			itemdata.Set( pclHorseTypeInfo->szSpecies );
			pComboKind->AddItem( &itemdata );
		}
	}

	// horse price
	for( i = 0; i < 7; ++i )
	{
		itemdata.Init();
		itemdata.Set( GetTxtFromMgr(1745 + i) );

		pComboPrice->AddItem( &itemdata );
	}

	// 리스트 멤버
	CList *pList = (CList*)m_pControlMap[ LISTBOX_HORSE ];

	{
		int width[]		= { 120, 102, 18 };

		TCHAR* pText[3];

		pText[0] = GetTxtFromMgr(1752);
		pText[1] = GetTxtFromMgr(1678);
		pText[2] = NULL;

		for( i = 0; i < 3; ++i ) 
		{ 
			pList->SetColumn( i, width[i], pText[i] );
		}

        pList->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
		pList->SetBorder( true );
		pList->SetBKColorUse( true );
		pList->Refresh();
	}

	pComboKind->SetCursel( 0 );
	pComboPrice->SetCursel( 0 );

	pHorseMarketManager->bSelectedSwitch = TRUE;
}

void NHorseMarketBuyDlg::Action()
{
	SI32 id = 1;
	
	// pHorseMarketManager 매니져
	cltHorseMarketManager *pHorseMarketManager = ((cltHorseMarketManager *)m_pStrManager);
	if( pHorseMarketManager == NULL ) return;

	// 리스트 멤버
	CList *pList = (CList*)m_pControlMap[ LISTBOX_HORSE ];

	if(pHorseMarketManager->bShowResponseOrderSwitch == TRUE )
	{
		pHorseMarketManager->bShowResponseOrderSwitch = FALSE;

		ShowResponseOrder();
	}

	if(pHorseMarketManager->bSelectedSwitch == TRUE)
	{

		pHorseMarketManager->bSelectedSwitch = FALSE;

		pHorseMarketManager->siTotalResponseOrderNumber = 0;

		pList->Clear();
		pList->Refresh();
		
		cltGameMsgRequest_HorseMarketOrderList sendMsg( 
			pHorseMarketManager->siHorseUnique, 
			pHorseMarketManager->siHorsePriceType );

		cltMsg clMsg( GAMEMSG_REQUEST_HORSEMARKET_ORDERLIST, sizeof( sendMsg ), (BYTE*)&sendMsg );
		pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );

	}	
}

void NHorseMarketBuyDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	TCHAR buf[ 1024 ];

	// pHorseMarketManager 매니져
	cltHorseMarketManager *pHorseMarketManager = ((cltHorseMarketManager *)m_pStrManager);
	if( pHorseMarketManager == NULL ) return;
	// 리스트 멤버
	CList *pList = (CList*)m_pControlMap[ LISTBOX_HORSE ];

	CComboBox *pComboKind = (CComboBox *)m_pControlMap[ COMBOBOX_KIND ];
	CComboBox *pComboPrice = (CComboBox *)m_pControlMap[ COMBOBOX_PRICE ];

	SI32 id = 1;

	CASE_CONTROL( LISTBOX_HORSE )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
		{
			TCHAR *pchTmp = NULL;
			SI16 siRow = -1, siCol = -1;
			
			pList->GetSelectedIndex( &siRow, &siCol );
			if( siRow == -1 || siCol == -1 ) return;
			stListBoxItemData *pstListBoxItemData = NULL;
			pstListBoxItemData = pList->GetItemData( siRow, 0 );
			if( pstListBoxItemData == NULL ) return;
			pHorseMarketManager->siSelectedIndex = pstListBoxItemData->siParam[0];	

			if( pHorseMarketManager->siSelectedIndex >= 0 ) {
				pclClient->pclHorseManager->GetText( &pHorseMarketManager->clResponseOrder[ pHorseMarketManager->siSelectedIndex ].clHorse, buf, 1024 );
				SetEditText( EDITBOX_EXPLAIN, buf );
			}
		}
		break;
		//default :
		//	{
		//		pHorseMarketManager->siSelectedIndex = -1;
		//		SetEditText( EDITBOX_EXPLAIN, TEXT("") );
		//	}
		//	break;
		}
	}

	else CASE_CONTROL( BUTTON_REFRESH )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				pHorseMarketManager->bSelectedSwitch = true;
			}
			break;
		}
	}
	else CASE_CONTROL( BUTTON_BUY )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				TCHAR buf[ 256 ];

				if( pclClient->pclCM->CR[ 1 ]->pclCI->clIP.GetLevel() < MIN_LEVEL_FOR_HORSE ) {
					TCHAR* pText = GetTxtFromMgr(1753);
					StringCchPrintf( buf, 256, pText, MIN_LEVEL_FOR_HORSE );
					SetMainExplain( buf );
					break;
				}

				if( pHorseMarketManager->siSelectedIndex < 0 )break;

				if( pHorseMarketManager->clResponseOrder[ pHorseMarketManager->siSelectedIndex ].siPersonID == 0 )break;

				cltGameMsgRequest_HorseMarketBuyHorse sendMsg( pHorseMarketManager->clResponseOrder[ pHorseMarketManager->siSelectedIndex ].siIndex, 
															   pHorseMarketManager->clResponseOrder[ pHorseMarketManager->siSelectedIndex ].siPersonID );

				cltMsg clMsg( GAMEMSG_REQUEST_HORSEMARKET_BUYHORSE, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );
			}
			break;
		}
	}

	else CASE_CONTROL( COMBOBOX_KIND )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{
				SI32 index = pComboKind->GetSelectedIndex();

				if ( index > -1 )
				{
					TCHAR strMonsterName[ 128 ] = TEXT("");
					TCHAR* pchTmp = NULL;
					pchTmp = pComboKind->GetText( index );
					if(pchTmp == NULL ) return;
					if( _tcslen( pchTmp ) >= 128 ) return;
					_tcscpy( strMonsterName, pchTmp );


					pHorseMarketManager->siHorseUnique = pclClient->pclHorseManager->GetHorseUniqueFromString( strMonsterName );
					pHorseMarketManager->bSelectedSwitch = true;
				}
			}
			break;
		}
	}

	else CASE_CONTROL( COMBOBOX_PRICE )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{
				SI32 index = pComboPrice->GetSelectedIndex();

				if ( index > -1 )
				{
					pHorseMarketManager->siHorsePriceType = index; 
					pHorseMarketManager->bSelectedSwitch = true;
				}
			}
			break;
		}
	}


}

void NHorseMarketBuyDlg::ShowResponseOrder( )
{
	// pHorseMarketManager 매니져
	cltHorseMarketManager *pHorseMarketManager = ((cltHorseMarketManager *)m_pStrManager);
	if( pHorseMarketManager == NULL ) return;
	
	// 리스트 멤버
	CList *pList = (CList*)m_pControlMap[ LISTBOX_HORSE ];
	
	int i;
	TCHAR buffer[256 ];
	TCHAR pricestring[64];

	// 기존의 리스트를 삭제한다. 
	pList->Clear();
	pList->Refresh();

	bool bcheck[MAX_HORSEMARKET_RESPONSE_ORDER_NUMBER];
	for( i = 0; i < MAX_HORSEMARKET_RESPONSE_ORDER_NUMBER; i++ )
	{
		bcheck[i] = false;
	}

	SI32 addnum = 0 ;
	while(addnum < pHorseMarketManager->siTotalResponseOrderNumber)
	{
		GMONEY minprice	= 5000000000;
		SI32 minindex	= -1;	

		for( i = 0; i < pHorseMarketManager->siTotalResponseOrderNumber; i++ )
		{
			if( bcheck[i] == false && pHorseMarketManager->clResponseOrder[i].siPrice <= minprice)
			{
				minprice	= pHorseMarketManager->clResponseOrder[i].siPrice;
				minindex	= i;
			}
		}
		
		if(minindex >= 0)
		{
			bcheck[minindex] = true;

			int len = _tcslen( pHorseMarketManager->clResponseOrder[minindex].clHorse.szName );
			
			if( len > 0 ) {
				StringCchCopy( buffer, 256, pHorseMarketManager->clResponseOrder[minindex].clHorse.szName );
			} else {
				TCHAR* pText = GetTxtFromMgr(1754);
				StringCchCopy( buffer, 256, pText );
			}

			// horse
			stListBoxItemData stTmpListBoxItemData;
			stTmpListBoxItemData.Init();
			stTmpListBoxItemData.Set( 0, buffer );
			stTmpListBoxItemData.siParam[0] = minindex;
			
			// 가격 
			TCHAR* pText = GetTxtFromMgr(611);
			g_JYLibFunction.SetNumUnit(pHorseMarketManager->clResponseOrder[minindex].siPrice, pricestring, 64, pText);

			//stTmpListBoxItemData.Init();
			stTmpListBoxItemData.Set( 1, pricestring );
			pList->AddItem( &stTmpListBoxItemData );

			addnum++;
		}
	}
	pList->Refresh();
	pHorseMarketManager->siSelectedIndex = -1;
}

void NHorseMarketBuyDlg::ShowResponseExplain()
{

}
