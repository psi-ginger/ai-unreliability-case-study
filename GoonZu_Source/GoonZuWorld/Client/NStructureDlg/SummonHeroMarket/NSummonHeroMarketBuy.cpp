#include "..\..\Server\Server.h"
#include "Char\CharManager\CharManager.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Market.h"
#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-SummonHeroMarket.h"

#include "NSummonHeroMarketDlg.h"

#include <tchar.h>
#include "../../InterfaceMgr/Interface/BUTTON.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../../CommonLogic/MsgType-Structure.h"

#include "../../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

NSummonHeroMarketBuyDlg::NSummonHeroMarketBuyDlg()
{
}

NSummonHeroMarketBuyDlg::~NSummonHeroMarketBuyDlg()
{
}

void NSummonHeroMarketBuyDlg::InitDlg()
{
	SetControlMap(	IMAGESTATIC_PORTRAIT,  TEXT("imagestatic_portrait") );
	SetControlMap(	BUTTON_BUY	,TEXT("button_buy"));
	SetControlMap(	BUTTON_FIRST	,TEXT("button_first"));
	SetControlMap(	BUTTON_NEXT	,TEXT("button_next"));
	SetControlMap(	BUTTON_SELECTORDER	,TEXT("button_selectorder"));
	SetControlMap(	BUTTON_REFRESH	,TEXT("button_refresh"));
	SetControlMap(	LISTBOX_SUMMON	,TEXT("listbox_summon"));
	SetControlMap(	LISTBOX_EXPLAIN	,TEXT("listbox_explain"));
	SetControlMap(	COMBOBOX_KIND	,TEXT("combobox_kind"));


	//CButton *pButton = (CButton *)m_pControlMap[ BUTTON_SELECTORDER ];
	//pButton->Enable( true );

	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;

	pSummonHeroManager->m_siKind = -1;

	pSummonHeroManager->m_siStartPos = -1;
	pSummonHeroManager->siSelectedIndex = -1;

	int i;

	pSummonHeroManager->bShowResponseOrderSwitch = FALSE;
	
	// 리스트 멤버
	CList *pList_summon = (CList*)m_pControlMap[ LISTBOX_SUMMON ];
	CList *pList_explain = (CList*)m_pControlMap[ LISTBOX_EXPLAIN ];
	if( pList_summon == NULL || pList_explain == NULL ) return; 

	{
		int width[]		= { 120, 102, 18 };

		TCHAR* pText[3];

		pText[0] = GetTxtFromMgr(848);
		pText[1] = GetTxtFromMgr(1678);
		pText[2] = NULL;

		for( i = 0; i < 3; ++i ) 
		{ 
			pList_summon->SetColumn( i, width[i], pText[i] );
		}

        pList_summon->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
		pList_summon->SetBorder( true );
		pList_summon->SetBKColorUse( true );
		pList_summon->Refresh();
	}

	{
		int width[] = { 102, 120, 18 };

		TCHAR* pText[3];

		pText[0] = GetTxtFromMgr(1780);
		pText[1] = GetTxtFromMgr(1565);
		pText[2] = NULL;

		for( i = 0; i < 3; ++i ) 
		{ 
			pList_explain->SetColumn( i, width[i], pText[i] );
		}

		pList_explain->SetBorder( true );
		pList_explain->SetBKColorUse( true );
		pList_explain->Refresh();

		SI32 id = 1;

		cltMsg clMsg( GAMEMSG_REQUEST_SUMMONHEROMARKET_ORDERLIST, 0 );
		pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );

		AddSummonKind();
	}		
}

void NSummonHeroMarketBuyDlg::Action()
{
	SI32 id = 1;
	
	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;

	// 리스트 멤버
	CList *pList_summon = (CList*)m_pControlMap[ LISTBOX_SUMMON ];
	CList *pList_explain = (CList*)m_pControlMap[ LISTBOX_EXPLAIN ];
	if( pList_summon == NULL || pList_explain == NULL ) return; 

	if(pSummonHeroManager->bShowResponseOrderSwitch == TRUE )
	{
		pSummonHeroManager->bShowResponseOrderSwitch = FALSE;

		ShowResponseOrder();
	}

	if(pSummonHeroManager->bSelectedSwitch == TRUE)
	{
		ShowResponseExplain();

		pSummonHeroManager->bSelectedSwitch = FALSE;

		pSummonHeroManager->siTotalResponseOrderNumber = 0;

		pList_summon->Clear();
		pList_summon->Refresh();
		
		cltGameMsgRequest_SummonHeroMarketOrderList sendMsg( pSummonHeroManager->m_siStartPos, pSummonHeroManager->m_siKind, pSummonHeroManager->m_siSelectList );

		if( pSummonHeroManager->m_siKind > 0 || pSummonHeroManager->m_siSelectList == 1 ) {
			cltMsg clMsg( GAMEMSG_REQUEST_SUMMONHEROMARKET_ORDERLIST, sizeof( sendMsg ), (BYTE*)&sendMsg );
			pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );
		}

		pSummonHeroManager->m_siSelectList = 0;
	}	
}

void NSummonHeroMarketBuyDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;
	// 리스트 멤버
	CList *pList_summon = (CList*)m_pControlMap[ LISTBOX_SUMMON ];
	CList *pList_explain = (CList*)m_pControlMap[ LISTBOX_EXPLAIN ];
	CComboBox *pComboBox_kind = (CComboBox*)m_pControlMap[ COMBOBOX_KIND ];
	if( pList_summon == NULL || pList_explain == NULL || pComboBox_kind == NULL ) return; 

	SI32 id = 1;

	CASE_CONTROL( LISTBOX_SUMMON )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
		{
			TCHAR *pchTmp = NULL;
			SI16 siRow = -1, siCol = -1;
			
			pList_summon->GetSelectedIndex( &siRow, &siCol );
			if( siRow == -1 || siCol == -1 ) return;
			stListBoxItemData *pstListBoxItemData = NULL;
			pstListBoxItemData = pList_summon->GetItemData( siRow, 0 );
			if( pstListBoxItemData == NULL ) return;
			pSummonHeroManager->siSelectedIndex = pstListBoxItemData->siParam[0];	

			if( pSummonHeroManager->siSelectedIndex >= 0 )
				ShowResponseExplain( );
		}
		break;
		default :
			{
				pSummonHeroManager->siSelectedIndex = -1;
				pList_explain->Clear();
				pList_explain->Refresh();
			}
			break;
		}
	}
	//else CASE_CONTROL( LISTBOX_EXPLAIN )
	//{
	//	switch( nEvent )
	//	{
	//		case EVENT_LISTBOX_SELECTION:
	//		{
	//		}
	//		break;
	//	}
	//}
	else CASE_CONTROL( BUTTON_SELECTORDER )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				pSummonHeroManager->m_siSelectList = 1;
				pSummonHeroManager->m_siStartPos = -1;
				pSummonHeroManager->bSelectedSwitch = true;
			}
			break;
		}
	}
	else CASE_CONTROL( BUTTON_FIRST )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				pSummonHeroManager->m_siSelectList = 0;
				pSummonHeroManager->m_siStartPos = -1;
				pSummonHeroManager->bSelectedSwitch = true;
			}
			break;
		}
	}
	else CASE_CONTROL( BUTTON_NEXT )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				pSummonHeroManager->m_siSelectList = 0;
				pSummonHeroManager->bSelectedSwitch = true;
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

				if( pclClient->pclCM->CR[ id ]->pclCI->clIP.GetLevel() < LIMIT_LEVEL_HATCH_DOLL ) {
					TCHAR* pText = GetTxtFromMgr(1753);
					StringCchPrintf( buf, 256, pText, LIMIT_LEVEL_HATCH_DOLL );
					SetMainExplain( buf );
					break;
				}

				if( pSummonHeroManager->siSelectedIndex < 0 )break;

				if( pSummonHeroManager->clResponseOrder[ pSummonHeroManager->siSelectedIndex ].siPersonID == 0 )break;

				// 같은 국가 코드가 아니면 살 수가 없다.
				if ( pclClient->pclCM->CR[id]->pclKI->siDefaultCountry != 
					pclClient->pclKindInfoSet->pclKI[pSummonHeroManager->clResponseOrder[ pSummonHeroManager->siSelectedIndex ].clSummon.siKind]->siDefaultCountry )
				{
					TCHAR* pText = GetTxtFromMgr(5569);
					MStrCpy(buf,pText,256);
					SetMainExplain( buf );
					break;
				}

				cltGameMsgRequest_SummonHeroMarketBuy sendMsg( pSummonHeroManager->clResponseOrder[ pSummonHeroManager->siSelectedIndex ].siIndex );

				cltMsg clMsg( GAMEMSG_REQUEST_SUMMONHEROMARKET_BUY, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );

			}
			break;
		}
	}
	else CASE_CONTROL( BUTTON_REFRESH )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			pSummonHeroManager->m_siSelectList = 0;
			pSummonHeroManager->m_siStartPos = -1;
			pSummonHeroManager->bSelectedSwitch = true;
			break;
		}
	}
	else CASE_CONTROL( COMBOBOX_KIND )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{
				SI32 index = pComboBox_kind->GetSelectedIndex();

				if ( index > -1 )
				{
					TCHAR strMonsterName[ 128 ] = TEXT("");
					TCHAR* pchTmp = NULL;
					pchTmp = pComboBox_kind->GetText( index );
					if(pchTmp == NULL ) return;
					if( _tcslen( pchTmp ) >= 128 ) return;
					_tcscpy( strMonsterName, pchTmp );

					pSummonHeroManager->m_siKind = pclClient->pclKindInfoSet->FindKindFromName( strMonsterName );

					pSummonHeroManager->m_siSelectList = 0;
					pSummonHeroManager->m_siStartPos = -1;
					pSummonHeroManager->bSelectedSwitch = true;
				}
			}
			break;
		}
	}

}

void NSummonHeroMarketBuyDlg::ShowResponseOrder( )
{
	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;
	// 리스트 멤버
	CList *pList_summon = (CList*)m_pControlMap[ LISTBOX_SUMMON ];
	CList *pList_explain = (CList*)m_pControlMap[ LISTBOX_EXPLAIN ];
	CComboBox *pComboBox_kind = (CComboBox*)m_pControlMap[ COMBOBOX_KIND ];
	if( pList_summon == NULL || pList_explain == NULL || pComboBox_kind == NULL ) return; 

	int i;
	TCHAR buffer[256 ];
	TCHAR pricestring[64];

	// 기존의 리스트를 삭제한다. 
	pList_summon->Clear();
	pList_summon->Refresh();


	bool bcheck[MAX_SUMMONHEROMARKET_RESPONSE_ORDER_NUMBER];
	for( i = 0; i < MAX_SUMMONHEROMARKET_RESPONSE_ORDER_NUMBER; i++ )
	{
		bcheck[i] = false;
	}

	SI32 addnum = 0 ;
	while(addnum < pSummonHeroManager->siTotalResponseOrderNumber)
	{
		GMONEY minprice	= 5000000000;
		SI32 minindex	= -1;	

		for( i = 0; i < pSummonHeroManager->siTotalResponseOrderNumber; i++ )
		{
			if( bcheck[i] == false && pSummonHeroManager->clResponseOrder[i].siPrice <= minprice)
			{
				minprice	= pSummonHeroManager->clResponseOrder[i].siPrice;
				minindex	= i;
			}
		}
		
		if(minindex >= 0)
		{
			bcheck[minindex] = true;

			int len = _tcslen( pSummonHeroManager->clResponseOrder[minindex].clSummon.szName );
			
			if( len > 0 ) {
				StringCchPrintf( buffer, 256, TEXT("[%s]%s"), 
					(TCHAR*)pclClient->pclKindInfoSet->pclKI[ pSummonHeroManager->clResponseOrder[minindex].clSummon.siKind ]->GetName(),
					pSummonHeroManager->clResponseOrder[minindex].clSummon.szName );
			} else {
				TCHAR* pText = GetTxtFromMgr(1781);
				StringCchPrintf( buffer, 256, pText,
					(TCHAR*)pclClient->pclKindInfoSet->pclKI[ pSummonHeroManager->clResponseOrder[minindex].clSummon.siKind ]->GetName() );
			}


			// 소환영웅
			stListBoxItemData stTmpListBoxItemData;
			stTmpListBoxItemData.Init();
			stTmpListBoxItemData.Set( 0, buffer );
			stTmpListBoxItemData.siParam[0] = minindex;
			// 가격 
			TCHAR* pText = GetTxtFromMgr(611);
			g_JYLibFunction.SetNumUnit(pSummonHeroManager->clResponseOrder[minindex].siPrice, pricestring, 64, pText);

			//stTmpListBoxItemData.Init();
			stTmpListBoxItemData.Set( 1, pricestring );
			pList_summon->AddItem( &stTmpListBoxItemData );

			addnum++;
		}
	}
	pList_summon->Refresh();
	pSummonHeroManager->siSelectedIndex = -1;
}

void NSummonHeroMarketBuyDlg::ShowResponseExplain()
{

	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;
	// 리스트 멤버
	CList *pList_summon = (CList*)m_pControlMap[ LISTBOX_SUMMON ];
	CList *pList_explain = (CList*)m_pControlMap[ LISTBOX_EXPLAIN ];
	CComboBox *pComboBox_kind = (CComboBox*)m_pControlMap[ COMBOBOX_KIND ];
	if( pList_summon == NULL || pList_explain == NULL || pComboBox_kind == NULL ) return; 

	TCHAR buffer[256];

	pList_explain->Clear();
	pList_explain->Refresh();

	TCHAR* pText[13];

	pText[0] = GetTxtFromMgr(1782);
	pText[1] = GetTxtFromMgr(1556);
	pText[2] = GetTxtFromMgr(1506);
	pText[3] = GetTxtFromMgr(1319);
	pText[4] = GetTxtFromMgr(1783);
	pText[5] = GetTxtFromMgr(1784);
	pText[6] = GetTxtFromMgr(1785);
	pText[7] = GetTxtFromMgr(1786);
	pText[8] = GetTxtFromMgr(1787);
	pText[9] = GetTxtFromMgr(1788);
	pText[10] = GetTxtFromMgr(1789);
	pText[11] = GetTxtFromMgr(1790);
	pText[12] = GetTxtFromMgr(1791);

	//for( int i = 0; i < 13; i++ )
	//{
	//	stListBoxItemData stTmpListBoxItemData;
	//	stTmpListBoxItemData.Init();
	//	stTmpListBoxItemData.Set( 0, pText[ i ] );
	//	pList_explain->AddItem( &stTmpListBoxItemData );
	//}	
	//pList_explain->Refresh();

	if( pSummonHeroManager->siSelectedIndex < 0 ) return;

	cltSummon *pclSummon = &(pSummonHeroManager->clResponseOrder[ pSummonHeroManager->siSelectedIndex ].clSummon);

	stListBoxItemData stTmpListBoxItemData;
	// 종류
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 0 ] );
	stTmpListBoxItemData.Set( 1, (TCHAR*)pclClient->pclKindInfoSet->pclKI[ pclSummon->siKind ]->GetName() );
	pList_explain->AddItem( &stTmpListBoxItemData );

	// 이름
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 1 ] );
	stTmpListBoxItemData.Set( 1, pclSummon->szName );
	pList_explain->AddItem( &stTmpListBoxItemData );

	// 레벨
	_itot( pclSummon->clIP.GetLevel(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 2 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_explain->AddItem( &stTmpListBoxItemData );

	// 경험치
	
	_i64tot( pclSummon->clIP.GetCurExp(), buffer, 10 );// SI32->SI64 경험치 오버플로우때문에 수정
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 3 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_explain->AddItem( &stTmpListBoxItemData );

	// 체력
	
	_itot( pclSummon->clIP.GetLife(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[4  ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_explain->AddItem( &stTmpListBoxItemData );

	// 마법력

	_itot( pclSummon->clIP.GetMana(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 5 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_explain->AddItem( &stTmpListBoxItemData );

	// 근력
	_itot( pclSummon->clIP.GetStr(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 6 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_explain->AddItem( &stTmpListBoxItemData );

	// 순발력

	_itot( pclSummon->clIP.GetDex(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 7 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_explain->AddItem( &stTmpListBoxItemData );

	// 도력

	_itot( pclSummon->clIP.GetMag(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 8 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_explain->AddItem( &stTmpListBoxItemData );

	// 생명력

	_itot( pclSummon->clIP.GetVit(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 9] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_explain->AddItem( &stTmpListBoxItemData );

	// 보너스

	_itot( pclSummon->clIP.GetBonus(), buffer, 10 );
	stTmpListBoxItemData.Init();
	stTmpListBoxItemData.Set( 0, pText[ 10 ] );
	stTmpListBoxItemData.Set( 1, buffer );
	pList_explain->AddItem( &stTmpListBoxItemData );

	SI32 i = 0;
	// 부적
	for( i = 0; i < 2; ++i )
	{
		stTmpListBoxItemData.Init();
		stTmpListBoxItemData.Set( 0, pText[ 11 + i ] );
		if( pclSummon->clScroll.siUnique[i] )
		{
			stTmpListBoxItemData.Set( 0, (TCHAR *)pclClient->pclItemManager->GetName( pclSummon->clScroll.siUnique[i] ) );

			StringCchPrintf( buffer, 256, TEXT("%0.1f%%"), (float)(pclSummon->clScroll.siPercent[i] / 10.0));
			stTmpListBoxItemData.Set( 1, buffer );
		}
		else
		{
			stTmpListBoxItemData.Set( 0, TEXT("") );
			stTmpListBoxItemData.Set( 1, TEXT("") );
		}

		pList_explain->AddItem( &stTmpListBoxItemData );
	}

	pList_explain->Refresh();

	DrawSummonPortrait();
}

void NSummonHeroMarketBuyDlg::DrawSummonPortrait()
{
	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;
	// 리스트 멤버
	CList *pList_summon = (CList*)m_pControlMap[ LISTBOX_SUMMON ];
	CList *pList_explain = (CList*)m_pControlMap[ LISTBOX_EXPLAIN ];
	CComboBox *pComboBox_kind = (CComboBox*)m_pControlMap[ COMBOBOX_KIND ];
	CImageStatic *pImageStatic_portrait = (CImageStatic*)m_pControlMap[ IMAGESTATIC_PORTRAIT ];
	if( pList_summon == NULL || pList_explain == NULL || pImageStatic_portrait == NULL ) return; 

	if( pSummonHeroManager->siSelectedIndex < 0 ) return;

	cltSummon *pclSummon = &(pSummonHeroManager->clResponseOrder[ pSummonHeroManager->siSelectedIndex ].clSummon);

	SI32 siSummonKind = pclSummon->siKind;

	if( siSummonKind == 0 ) return;

	if( pSummonHeroManager->m_siPortraitSummonKind != siSummonKind ) 
	{
		TCHAR strFileName[ MAX_PATH ] = TEXT("");
		//sprintf( strFileName, TEXT("Interface/Portrait/%s.bmp"),	pclClient->pclKindInfoSet->pclKI[ siSummonKind ]->szCode );

		SI16 tmpSelectedIndex = siSummonKind;

		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(tmpSelectedIndex, szPortrait, sizeof(szPortrait), &siPortraitFont);
		pImageStatic_portrait->SetFileName( szPortrait );
		pImageStatic_portrait->SetFontIndex( siPortraitFont );

		pSummonHeroManager->m_siPortraitSummonKind = siSummonKind;
	}
}

void NSummonHeroMarketBuyDlg::AddSummonKind() 
{
	// 비변사 매니져
	cltSummonHeroMarketManager *pSummonHeroManager = ((cltSummonHeroMarketManager *)m_pStrManager);
	if( pSummonHeroManager == NULL ) return;
	// 리스트 멤버
	CList *pList_summon = (CList*)m_pControlMap[ LISTBOX_SUMMON ];
	CList *pList_explain = (CList*)m_pControlMap[ LISTBOX_EXPLAIN ];
	CComboBox *pComboBox_kind = (CComboBox*)m_pControlMap[ COMBOBOX_KIND ];
	CImageStatic *pImageStatic_portrait = (CImageStatic*)m_pControlMap[ IMAGESTATIC_PORTRAIT ];
	if( pList_summon == NULL || pList_explain == NULL || pImageStatic_portrait == NULL ) return; 

	pComboBox_kind->Clear();

	cltKindInfo* pclki = NULL;

	for ( SI32 j = 1 ; j < MAX_KIND_NUMBER ; j ++ )
	{
		for ( SI32 i = 10; i < MAX_KIND_NUMBER ; ++i )
		{
			pclki = pclClient->pclKindInfoSet->pclKI[ i ];

			if( pclki == NULL || pclki->siSummonRank == 0 || pclki->bSummonMall == false ) 
				continue;

			if ( pclki->siSortLevel <= 0 || pclki->siSortLevel >= MAX_SORT_LEVEL ) continue ;

			if ( pclki->siSortLevel == j )
			{
				TCHAR* pname = (TCHAR*)pclki->GetName();
				stComboBoxItemData stTmpComboBoxItemData;
				stTmpComboBoxItemData.Init();
				stTmpComboBoxItemData.Set( pname );
				pComboBox_kind->AddItem( &stTmpComboBoxItemData );
				break ;
			}
		}
	}

	pComboBox_kind->Refresh();

	return;
}

