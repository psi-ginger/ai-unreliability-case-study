#include "NStockDlg.h"
#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"

extern cltCommonLogic *pclClient;


NStockDlg::NStockDlg()
{

}

NStockDlg::~NStockDlg()
{

}

void NStockDlg::Create()
{
	CreateDlg( NSTOCK_DLG, pclClient->pclStockManager, 4 );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	TCHAR* pText = GetTxtFromMgr(3446);
	AddUserTabDialog( 0, pText, 65, new NStockUserDlg, TEXT("NInterface/Data/NStructureDlg/DLG_StockUser.ddf"), TEXT("dialog_stockuser") );
	if( pclClient->IsCountrySwitch(Switch_AddCapitalRemove) )		// by LEEKH 2007.07.16
	{
		// 증자된 주식 구매 불가
	}
	else
	{
		pText = GetTxtFromMgr(3447);
		AddUserTabDialog( 1, pText, 65, new NStockUser2Dlg, TEXT("NInterface/Data/NStructureDlg/DLG_StockUser2.ddf"), TEXT("dialog_stockuser2") );
	}

	pText = GetTxtFromMgr(3448);
	AddUserTabDialog( 2, pText, 65, new NStrInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );
	pText = GetTxtFromMgr(5994);
	AddUserTabDialog( 3, pText, 65, new NStockPublicSubscriptionDlg, TEXT("NInterface/Data/NStructureDlg/DLG_StockPublicSubscription.ddf"), TEXT("dialog_stockpublic") );
	

	// 행수 메뉴
	SetTabInfo( 1, 1 );
	pText = GetTxtFromMgr(3442);
	AddChiefTabDialog( 0, pText, 65, new NStockChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_StockChief.ddf"), TEXT("dialog_stockchief") );
	pText = GetTxtFromMgr(3443);
	AddChiefTabDialog( 1, pText, 65, new NStrPayTaxDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf"), TEXT("dialog_strpaytax") );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 2, pText, 65, new NStrRepairDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair") );
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 3, pText, 65, new NStrResignChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );
	//주식버튼폭 조절 60->65 [2007.06.29 성웅]
	InitDlg();

	SetActionTimer( 200 );
	SetTimerOnOff( true );
}

void NStockDlg::GreetingMsg()
{
	TCHAR* pText = GetTxtFromMgr(1499);
	SetMainExplain( pText );

}

void NStockDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0: // 주식 사기
	case 1:	// 주식 팔기
		((NStockUserDlg*)m_pUserChildDlg[ 0 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	case 10:	// 주식 입찰 선정
		((NStockUser2Dlg*)m_pUserChildDlg[ 1 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	}
}