#include <CommonLogic.h>
#include "..\..\Server\Server.h"
#include "Char\CharCommon\Char-Common.h"

#include "..\..\Common\Util\Control\Control.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\Resource.h"

#include "SummonMarket.h"
//#include "..\..\DBManager\GameDBManager_World\DBMsg-Market.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-SummonMarket.h"

#include "../CityHall/TaxpaymentListDlg.h"

extern cltCommonLogic* pclClient;

void cltSummonMarketManager::ShowMySummonInfo( HWND hDlg )
{
	if( siMySummonSelectedIndex < 0 || 
		siMySummonSelectedIndex >= MAX_SUMMON_OWN ) return;
/*
	char buf1[ 1024 ], buf2[ 1024 ];
	char scrbuf[ MAX_SCROLL_SUMMON_PER_KIND ][ 128 ];

	buf1[ 0 ] = NULL;
	buf2[ 0 ] = NULL;

	cltSummon *pclSummon = pclClient->pclCM->CR[ 1 ]->pclCI->clSummonInfo.GetSummon( siMySummonSelectedIndex );

	if( pclSummon == NULL ) return;

	if( pclSummon->siKind != 0 ) {

		char* pText = GetTxtFromMgr(1799);
		sprintf( buf1, 
				 pText,
				 (char*)pclClient->pclKindInfoSet->pclKI[ pclSummon->siKind ]->GetName(),
				 pclSummon->szName,
				 pclSummon->clIP.GetLevel(),
				 pclSummon->clIP.GetCurExp(),
				 pclSummon->clIP.GetLife(),
				 pclSummon->clIP.GetMana()
				 );

	
	
		for(SI32 i = 0 ; i<MAX_SCROLL_SUMMON_PER_KIND; ++i )
		{
			scrbuf[ i ][ 0 ] = NULL;

			if(pclSummon->clScroll.siUnique[i])
			{
				sprintf(scrbuf[i], "%s : %0.1f%%", 
					pclClient->pclItemManager->GetName( pclSummon->clScroll.siUnique[i] ),
					(float)(pclSummon->clScroll.siPercent[i] / 10.0));
			}
		}

		pText = GetTxtFromMgr(1800);

		sprintf( buf2,
				 pText,
				 pclSummon->clIP.GetStr(),
				 pclSummon->clIP.GetDex(),
				 pclSummon->clIP.GetMag(),
				 pclSummon->clIP.GetVit(),
				 pclSummon->clIP.GetBonus(),
				 scrbuf[ 0 ],
				 scrbuf[ 1 ]
				 );
	}

	SetDlgItemText( hDlg, IDC_EDIT_DETAILINFO, buf1 );
	SetDlgItemText( hDlg, IDC_EDIT_DETAILINFO2, buf2 );
*/
	char buffer[256];

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_DELETEALLITEMS, 0, 0 );

//	char *text[] = { "종류", "이름", "레벨", "경험치", "체력", "마법력",
//					 "근력", "순발력", "도력", "생명력", "보너스", "부적1", "부적2" };

	char* pText[13];

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

	LVITEM lvi;

	for( int i = 0; i < 13; i++ )
	{

		lvi.iItem = i;

		lvi.mask = LVIF_TEXT;
		lvi.iSubItem = 0;
		lvi.pszText = pText[i]; // text[ i ];	

		SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_INSERTITEM, 0, (LPARAM)&lvi );
	}	

	cltSummon *pclSummon = pclClient->pclCM->CR[ 1 ]->pclCI->clSummonInfo.GetSummon( siMySummonSelectedIndex );

	// 종류
	lvi.iItem = 0;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = (char*)pclClient->pclKindInfoSet->pclKI[ pclSummon->siKind ]->GetName();

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 이름
	lvi.iItem = 1;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = pclSummon->szName;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );


	// 레벨

	itoa( pclSummon->clIP.GetLevel(), buffer, 10 );

	lvi.iItem = 2;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 경험치
	
	itoa( pclSummon->clIP.GetCurExp(), buffer, 10 );

	lvi.iItem = 3;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 체력
	
	itoa( pclSummon->clIP.GetLife(), buffer, 10 );

	lvi.iItem = 4;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 마법력

	itoa( pclSummon->clIP.GetMana(), buffer, 10 );

	lvi.iItem = 5;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 근력

	itoa( pclSummon->clIP.GetStr(), buffer, 10 );

	lvi.iItem = 6;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 순발력

	itoa( pclSummon->clIP.GetDex(), buffer, 10 );

	lvi.iItem = 7;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 도력

	itoa( pclSummon->clIP.GetMag(), buffer, 10 );

	lvi.iItem = 8;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 생명력

	itoa( pclSummon->clIP.GetVit(), buffer, 10 );

	lvi.iItem = 9;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 보너스

	itoa( pclSummon->clIP.GetBonus(), buffer, 10 );

	lvi.iItem = 10;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 부적
	for( i = 0; i < 2; ++i )
	{
		lvi.iItem = 11 + i;

		if( pclSummon->clScroll.siUnique[i] )
		{
			lvi.iSubItem = 0;
			lvi.pszText = (char *)pclClient->pclItemManager->GetName( pclSummon->clScroll.siUnique[i] );

			SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

			sprintf( buffer, "%0.1f%%", (float)(pclSummon->clScroll.siPercent[i] / 10.0));

			lvi.iSubItem = 1;
			lvi.pszText = buffer;

			SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );
		}
		else
		{
			/*
			lvi.iSubItem = 0;
			lvi.pszText = "";
			SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

			lvi.iSubItem = 1;
			lvi.pszText = "";
			SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );
			*/
		}
	}

}


void cltSummonMarketManager::ShowMyOrderInfo( HWND hDlg )
{
	if( siMyOrderSelectedIndex < 0 ||
		siMyOrderSelectedIndex >= MAX_SUMMONMARKET_ORDER_PER_PERSON ) return;


	char buffer[256];

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_DELETEALLITEMS, 0, 0 );

//	char *text[] = { "종류", "이름", "레벨", "경험치", "체력", "마법력",
//					 "근력", "순발력", "도력", "생명력", "보너스", "부적1", "부적2" };

	char* pText[13];

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

	LVITEM lvi;

	for( int i = 0; i < 13; i++ )
	{

		lvi.iItem = i;

		lvi.mask = LVIF_TEXT;
		lvi.iSubItem = 0;
		lvi.pszText = pText[i]; // text[ i ];	

		SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_INSERTITEM, 0, (LPARAM)&lvi );
	}	

	cltSummon *pclSummon = &clMyOrder[ siMyOrderSelectedIndex ].clSummon;

	// 종류
	lvi.iItem = 0;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = (char*)pclClient->pclKindInfoSet->pclKI[ pclSummon->siKind ]->GetName();

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 이름
	lvi.iItem = 1;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = pclSummon->szName;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );


	// 레벨

	itoa( pclSummon->clIP.GetLevel(), buffer, 10 );

	lvi.iItem = 2;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 경험치
	
	itoa( pclSummon->clIP.GetCurExp(), buffer, 10 );

	lvi.iItem = 3;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 체력
	
	itoa( pclSummon->clIP.GetLife(), buffer, 10 );

	lvi.iItem = 4;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 마법력

	itoa( pclSummon->clIP.GetMana(), buffer, 10 );

	lvi.iItem = 5;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 근력

	itoa( pclSummon->clIP.GetStr(), buffer, 10 );

	lvi.iItem = 6;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 순발력

	itoa( pclSummon->clIP.GetDex(), buffer, 10 );

	lvi.iItem = 7;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 도력

	itoa( pclSummon->clIP.GetMag(), buffer, 10 );

	lvi.iItem = 8;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 생명력

	itoa( pclSummon->clIP.GetVit(), buffer, 10 );

	lvi.iItem = 9;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 보너스

	itoa( pclSummon->clIP.GetBonus(), buffer, 10 );

	lvi.iItem = 10;
	lvi.mask = LVIF_TEXT;
	lvi.iSubItem = 1;
	lvi.pszText = buffer;

	SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

	// 부적
	for( i = 0; i < 2; ++i )
	{
		lvi.iItem = 11 + i;

		if( pclSummon->clScroll.siUnique[i] )
		{
			lvi.iSubItem = 0;
			lvi.pszText = (char *)pclClient->pclItemManager->GetName( pclSummon->clScroll.siUnique[i] );

			SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

			sprintf( buffer, "%0.1f%%", (float)(pclSummon->clScroll.siPercent[i] / 10.0));

			lvi.iSubItem = 1;
			lvi.pszText = buffer;

			SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );
		}
		else
		{
			/*
			lvi.iSubItem = 0;
			lvi.pszText = "";
			SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );

			lvi.iSubItem = 1;
			lvi.pszText = "";
			SendDlgItemMessage( hDlg, IDC_LIST_DETAILINFO, LVM_SETITEM, 0, (LPARAM)&lvi );
			*/
		}
	}

/*

	char buf1[ 1024 ], buf2[ 1024 ];
	char scrbuf[ MAX_SCROLL_SUMMON_PER_KIND ][ 128 ];

	buf1[ 0 ] = NULL;
	buf2[ 0 ] = NULL;

	cltSummon *pclSummon = pclClient->pclCM->CR[ 1 ]->pclCI->clSummonInfo.GetSummon( siMySummonSelectedIndex );

	if( pclSummon == NULL ) return;

	if( pclSummon->siKind != 0 ) {

		char* pText = GetTxtFromMgr(1799);
		sprintf( buf1, 
				 pText,
				 (char*)pclClient->pclKindInfoSet->pclKI[ pclSummon->siKind ]->GetName(),
				 pclSummon->szName,
				 pclSummon->clIP.GetLevel(),
				 pclSummon->clIP.GetCurExp(),
				 pclSummon->clIP.GetLife(),
				 pclSummon->clIP.GetMana()
				 );

	
	
		for(SI32 i = 0 ; i<MAX_SCROLL_SUMMON_PER_KIND; ++i )
		{
			scrbuf[ i ][ 0 ] = NULL;

			if(pclSummon->clScroll.siUnique[i])
			{
				sprintf(scrbuf[i], "%s : %0.1f%%", 
					pclClient->pclItemManager->GetName( pclSummon->clScroll.siUnique[i] ),
					(float)(pclSummon->clScroll.siPercent[i] / 10.0));
			}
		}

		pText = GetTxtFromMgr(1800);

		sprintf( buf2,
				 pText,
				 pclSummon->clIP.GetStr(),
				 pclSummon->clIP.GetDex(),
				 pclSummon->clIP.GetMag(),
				 pclSummon->clIP.GetVit(),
				 pclSummon->clIP.GetBonus(),
				 scrbuf[ 0 ],
				 scrbuf[ 1 ]
				 );
	}

	SetDlgItemText( hDlg, IDC_EDIT_DETAILINFO, buf1 );
	SetDlgItemText( hDlg, IDC_EDIT_DETAILINFO2, buf2 );
*/
}






// 수수료 정보를 보여준다.
BOOL cltSummonMarketManager::ShowFee(HWND hDlg)
{
	char buffer[256];
	float feerate = (float)clClientStrInfo.clSummonMarketStrInfo.clFee.GetFeeRate() / 10;

	sprintf(buffer, "%3.1f%%", feerate);
	SetDlgItemText(hDlg, IDC_STATIC_FEERATE, buffer);

	return TRUE;
}

// 실제로 지불해야 할 금액을 보여준다. 
BOOL cltSummonMarketManager::ShowRealFee(HWND hDlg, SI32 price)
{
	char buffer[256];

	sprintf(buffer, "%d", clClientStrInfo.clSummonMarketStrInfo.clFee.CalcFee( price ));
	SetDlgItemText(hDlg, IDC_EDIT_REALFEE, buffer);

	return TRUE;
}




void cltSummonMarketManager::ShowMySummonList(HWND hDlg)
{

	SendDlgItemMessage( hDlg, IDC_LIST_MYSUMMON, LVM_DELETEALLITEMS, 0, 0 );

	LVITEM lvi;

	int index = 0;

	cltSummon *pclSummon = NULL;

	for( int i = 0; i < MAX_SUMMON_OWN; i++ )
	{

		pclSummon = &pclCM->CR[ 1 ]->pclCI->clSummonInfo.clSummon[ i ];

		if( pclSummon->siKind != 0 ) {

			if( pclClient->pclKindInfoSet->pclKI[ pclSummon->siKind ]->bSummonMall == true ) continue;

			lvi.iItem = index;

			lvi.mask = LVIF_TEXT | LVIF_PARAM;
			lvi.iSubItem = 0;
			lvi.pszText = (char *)pclClient->pclKindInfoSet->pclKI[ pclSummon->siKind ]->GetName();
			lvi.lParam = i;			

			SendDlgItemMessage( hDlg, IDC_LIST_MYSUMMON, LVM_INSERTITEM, 0, (LPARAM)&lvi );

			lvi.mask = LVIF_TEXT;
			lvi.iSubItem = 1;
			lvi.pszText = pclSummon->szName;
			SendDlgItemMessage( hDlg, IDC_LIST_MYSUMMON, LVM_SETITEM, 0, (LPARAM)&lvi );

			index++;
		}
	}	

	siMySummonSelectedIndex = -1;
}

void cltSummonMarketManager::ShowMyOrderList(HWND hDlg)
{
	ShowMySummonList( hDlg );

	SendDlgItemMessage( hDlg, IDC_LIST_MYORDER, LVM_DELETEALLITEMS, 0, 0 );

	LVITEM lvi;

	int index = 0;

	for( int i = 0; i < MAX_SUMMONMARKET_ORDER_PER_PERSON; i++ )
	{
		if( clMyOrder[ i ].siPersonID != 0 ) {

			lvi.iItem = index;

			lvi.mask = LVIF_TEXT | LVIF_PARAM;
			lvi.iSubItem = 0;
			lvi.pszText = (char *)pclClient->pclKindInfoSet->pclKI[ clMyOrder[ i ].clSummon.siKind ]->GetName();
			lvi.lParam = i;			

			SendDlgItemMessage( hDlg, IDC_LIST_MYORDER, LVM_INSERTITEM, 0, (LPARAM)&lvi );

			lvi.mask = LVIF_TEXT;
			lvi.iSubItem = 1;
			lvi.pszText = clMyOrder[ i ].clSummon.szName;
			SendDlgItemMessage( hDlg, IDC_LIST_MYORDER, LVM_SETITEM, 0, (LPARAM)&lvi );

			index++;
		}	
	}

	siMyOrderSelectedIndex = -1;
}