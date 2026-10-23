#include "NBankDlg.h"

#include "../../InterfaceMgr/Interface/List.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Bank.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NBankInfoDlg::NBankInfoDlg()
{
}

NBankInfoDlg::~NBankInfoDlg()
{
}

void NBankInfoDlg::InitDlg()
{

	SetControlMap( EDIT_STRDURA, TEXT("editbox_strdura") );
	SetControlMap( EDIT_CHIEF, TEXT("editbox_chief") );
	SetControlMap( EDIT_MONEY, TEXT("editbox_money") );
	SetControlMap( EDIT_TOTAL_INPUT_MONEY, TEXT("editbox_total_input_money") );
	SetControlMap( EDIT_PROFIT, TEXT("editbox_profit") );
	SetControlMap( LIST_TAXLIST, TEXT("listbox_taxlist") );

	CList *pList = (CList *)m_pControlMap[ LIST_TAXLIST ];
	stListBoxItemData itemdata;

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR *pText[ 4 ];
	pText[ 0 ] = GetTxtFromMgr( 1659 );
	pText[ 1 ] = GetTxtFromMgr( 1660 );
	pText[ 2 ] = GetTxtFromMgr( 1661 );
	pText[ 3 ] = GetTxtFromMgr( 1662 );

	pList->SetColumn( 0, 48, pText[ 0 ] );
	pList->SetColumn( 1, 96, pText[ 1 ] );
	pList->SetColumn( 2, 96, pText[ 2 ] );
	pList->SetColumn( 3, 96, pText[ 3 ] );
	pList->Refresh();

	m_bShowTaxInfo_local = FALSE;

	ShowBankInfo();

}

void NBankInfoDlg::Action()
{
	ShowBankInfo();
}

void NBankInfoDlg::ShowBankInfo()
{
	TCHAR buffer[256]= TEXT("");

	// 건물 내구도를 표시한다
	SI32 siDura = m_pStrManager->clClientStrInfo.clCommonInfo.siStrDura;
	TCHAR dura[128] = TEXT("");

	g_JYLibFunction.SetNumUnit( siDura, dura, 128 );
    
	StringCchPrintf( buffer, 256, TEXT("%s/%s"), dura, TEXT("10,000") );
	SetEditText( EDIT_STRDURA, buffer );

	// 행수의 이름을 표시한다. 
	if( m_pStrManager->UpdateChiefSwitch == true)
	{
		m_pStrManager->UpdateChiefSwitch = false;

		// 장을 보여준다. 
		if(m_pStrManager->siVillageUnique > 0)
		{

			cltSimplePerson *pclPerson = &pclClient->pclVillageManager->pclVillageInfo[ m_pStrManager->siVillageUnique ]->clVillageVary.clRankPerson[RANKTYPE_BANK];
			
			if( pclPerson ) {

				if( pclPerson->GetPersonID() ) {

					StringCchPrintf( buffer, 256, TEXT("%s"), pclPerson->GetName() );
				
				} else {

					TCHAR* pText = GetTxtFromMgr(1287);
					StringCchCopy( buffer, 256, pText );
				}

				SetEditText( EDIT_CHIEF, buffer );
			}
		}
	}

	TCHAR* pText = GetTxtFromMgr(611);

	g_JYLibFunction.SetNumUnit( m_pStrManager->clClientStrInfo.clCommonInfo.clMoney.itMoney, buffer, 256, pText );
	SetEditText( EDIT_MONEY, buffer );

	
	g_JYLibFunction.SetNumUnit( m_pStrManager->clClientStrInfo.clBankStrInfo.clTotalInputMoney.itMoney, buffer, 256, pText );
	SetEditText( EDIT_TOTAL_INPUT_MONEY, buffer );

	// 수익금 
	__int64 margin = m_pStrManager->clClientStrInfo.clCommonInfo.clMoney.itMoney - 
				     m_pStrManager->clClientStrInfo.clBankStrInfo.clTotalInputMoney.itMoney;
	
	g_JYLibFunction.SetNumUnit( margin, buffer, 256, pText);
	
	SetEditText( EDIT_PROFIT, buffer );

	// 세금 표시 
	CList *pList = (CList *)m_pControlMap[ LIST_TAXLIST ];
	stListBoxItemData itemdata;

	cltTaxInfo *pTaxInfo = 
		&m_pStrManager->clClientStrInfo.clCommonInfo.clTaxInfo;

	int count = pTaxInfo->GetCount();

	if( m_bShowTaxInfo_local == FALSE && count > 0 ) {
		m_bShowTaxInfo_local = TRUE;

		SI64 due, paid;
		SI32 i;

		pList->Clear();

		for( i = 0; i < count; i++ )
		{
			itemdata.Init();

			due  = pTaxInfo->clTaxUnitInfo[ count - i - 1 ].clDueTax.itMoney;
			paid = pTaxInfo->clTaxUnitInfo[ count - i - 1 ].clPaidTax.itMoney;

			_itot( pTaxInfo->clTaxUnitInfo[ count - i - 1 ].usYear, buffer, 10 );
			itemdata.Set( 0, buffer );

			g_JYLibFunction.SetNumUnit( due, buffer, 256, pText );
			itemdata.Set( 1, buffer );

			if( due == 0 ) {
				g_JYLibFunction.SetNumUnit( paid, buffer, 256, pText );
			} else {
				int t;
				double d;
				
				d = (double)(paid * 100 / due);
				t = (int)paid;

				g_JYLibFunction.SetNumUnit( t, buffer, 256, pText );
			}

			itemdata.Set( 2, buffer );

			
			if( due - paid < 0 ) {
				StringCchCopy( buffer, 256, TEXT("0") );
			} else {
				g_JYLibFunction.SetNumUnit( due - paid, buffer, 256, pText );
			}
			
			itemdata.Set( 3, buffer );

			pList->AddItem( &itemdata );
		}

		pList->Refresh();

	}
}


void NBankInfoDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{

}