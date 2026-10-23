
#include "NCityhallDlg.h"

#include "../../InterfaceMgr/Interface/List.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgType-Structure.h"

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NCityhallDividendListDlg::NCityhallDividendListDlg()
{
}

NCityhallDividendListDlg::~NCityhallDividendListDlg()
{
	SetTimerOnOff( false );
}

void NCityhallDividendListDlg::InitDlg()
{

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	SetControlMap( LIST_DIVIDEND, TEXT("listbox_dividend") );

	CList *pList = (CList *)m_pControlMap[ LIST_DIVIDEND ];
	stListBoxItemData itemdata;

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR *pText[ 4 ];
	pText[ 0 ] = GetTxtFromMgr( 1636 );
	pText[ 1 ] = GetTxtFromMgr( 1637 );
	pText[ 2 ] = GetTxtFromMgr( 1638);
	pText[ 3 ] = GetTxtFromMgr( 1639 );

	pList->SetColumn( 0, 88, pText[ 0 ] );
	pList->SetColumn( 1, 66, pText[ 1 ] );
	pList->SetColumn( 2, 86, pText[ 2 ] );
	pList->SetColumn( 3, 96, pText[ 3 ] );
	pList->Refresh();

	m_bDispDividend = FALSE;

	m_dividendInfo.SetCount( 0 );

	// 배당 내역 요청
	SI32 CharUnique = pCityhallManager->CharUnique;
	SI32 siVillageUnique = pCityhallManager->siVillageUnique;

	SI32 id = 1;
	if(id)
	{
		cltGameMsgRequest_GetDividendList	clDividendList( siVillageUnique );

		cltMsg clMsg( GAMEMSG_REQUEST_GETDIVIDENDLIST, sizeof(cltGameMsgRequest_GetDividendList), (BYTE*)&clDividendList );
		
		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
	}

	SetTimerOnOff( true );
	SetActionTimer( 500 );
}

void NCityhallDividendListDlg::Action()
{
	// 세금 납부 내역 보여주기
	CList *pList = (CList *)m_pControlMap[ LIST_DIVIDEND ];

	if(pList == NULL ) return;

	int count = m_dividendInfo.GetCount();

	if( count > 0 && m_bDispDividend == FALSE ) {
		m_bDispDividend = TRUE;
			
		int		totstock;
		SI64	price;
		cltDate	*pclDate;

		SI32 i;
		TCHAR buffer[256];

		pList->Clear();

		TCHAR* pText = NULL;
		for( i = 0; i < count; i++ )
		{
			stListBoxItemData tmpListItem;
			tmpListItem.Init();

			// 배당일
			pclDate = &m_dividendInfo.clDividendUnitInfo[ count - i - 1 ].clDate;

			pText = GetTxtFromMgr(1508);
			StringCchPrintf( buffer, 256, pText, pclDate->uiYear, pclDate->uiMonth, pclDate->uiDay );
			tmpListItem.Set( 0, (TCHAR *)buffer);


			// 총 주식수
			totstock = m_dividendInfo.clDividendUnitInfo[ count - i - 1 ].siTotalStock;
			pText = GetTxtFromMgr(1594);
			StringCchPrintf( buffer, 256, pText, totstock );
			tmpListItem.Set( 1, (TCHAR *)buffer);

			// 1주당 배당금액
			price = m_dividendInfo.clDividendUnitInfo[ count - i - 1 ].clDividendPrice.itMoney;
			//pText = GetTxtFromMgr(1640);

			TCHAR		szPrice[64] = { '\0', };
			GMONEY		siPrice = price ;
			g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));
			NTCHARString512	kText(GetTxtFromMgr(1640));
			kText.Replace("#money#",szPrice);
			//m_pEdit_Quest_Fee->SetText( kText );

			//StringCchPrintf( buffer, 256, pText, price );
			tmpListItem.Set( 2, kText /*(TCHAR *)buffer*/);

			// 총 배당금액
			TCHAR	szTotalPrice[64]	= { '\0', };
			GMONEY	siTotalPrice		= (GMONEY)totstock * price;

			g_JYLibFunction.SetNumUnit(siTotalPrice, szTotalPrice, sizeof(szTotalPrice));
			NTCHARString512	kTotalPrice(GetTxtFromMgr(1640));
			kTotalPrice.Replace("#money#",szTotalPrice);
			
			tmpListItem.Set( 3, kTotalPrice);

			pList->AddItem( &tmpListItem );
		}

		pList->Refresh();
	}

}

void NCityhallDividendListDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{

}
