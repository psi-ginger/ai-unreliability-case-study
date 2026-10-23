#include "NSummonMarketDlg.h"

#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"

extern cltCommonLogic *pclClient;


NSummonMarketDlg::NSummonMarketDlg()
{

}

NSummonMarketDlg::~NSummonMarketDlg()
{

}

void NSummonMarketDlg::Create()
{
	CreateDlg( NSUMMONMARKET_DLG, pclClient->pclSummonMarketManager );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	TCHAR* pText = GetTxtFromMgr(6159);
	AddUserTabDialog( 0, pText, 80, new NSummonMarketUserDlg, TEXT("NInterface/Data/NStructureDlg/DLG_SummonMarketUserDLG.ddf"), TEXT("dialog_summonmarket_user") );
	pText = GetTxtFromMgr(6161);
	AddUserTabDialog( 1, pText, 80, new NSummonMarketBuyDlg, TEXT("NInterface/Data/NStructureDlg/DLG_SummonMarketBuyDLG.ddf"), TEXT("dialog_summonmarket_buy") );
	pText = GetTxtFromMgr(3487);
	//AddUserTabDialog( 2, TEXT("장예원정보"), 80, new NSummonMarketInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_SummonMarketInfoDLG.ddf"), TEXT("dialog_summonmarket_info") );
	AddUserTabDialog( 2, pText, 80, new NStrInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );	

	// 행수 메뉴
	SetTabInfo( 1, 1 );
	pText = GetTxtFromMgr(3442);
	AddChiefTabDialog( 0, pText, 60, new NSummonMarketChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_SummonMarketChiefDlg.ddf"), TEXT("dialog_summonmarketchief") );
	pText = GetTxtFromMgr(3443);
	AddChiefTabDialog( 1,pText, 60, new NStrPayTaxDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf"), TEXT("dialog_strpaytax") );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 2, pText, 60, new NStrRepairDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair" ));
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 3, pText, 60, new NStrResignChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );
 
	InitDlg();

	SetActionTimer( 200 );
	SetTimerOnOff( true );
}

void NSummonMarketDlg::GreetingMsg()
{
	TCHAR* pText = GetTxtFromMgr(1801);
	SetMainExplain( pText );
}

void NSummonMarketDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 소환수 팔기
	case 1:	// 소환수 지정 팔기
		((NSummonMarketUserDlg*)m_pUserChildDlg[ 0 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	}
}