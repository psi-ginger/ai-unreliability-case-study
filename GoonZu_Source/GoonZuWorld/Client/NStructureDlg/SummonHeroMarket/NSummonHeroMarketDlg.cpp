#include "NSummonHeroMarketDlg.h"

#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"

extern cltCommonLogic *pclClient;


NSummonHeroMarketDlg::NSummonHeroMarketDlg()
{

}

NSummonHeroMarketDlg::~NSummonHeroMarketDlg()
{

}

void NSummonHeroMarketDlg::Create()
{
	CreateDlg( NSUMMONHEROMARKET_DLG, pclClient->pclSummonHeroMarketManager );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	TCHAR* pText = GetTxtFromMgr(3502);
	AddUserTabDialog( 0, pText, 80, new NSummonHeroMarketUserDlg, TEXT("NInterface/Data/NStructureDlg/DLG_SummonHeroMarketUserDLG.ddf"), TEXT("dialog_summonmarket_user") );
	pText = GetTxtFromMgr(3503);
	AddUserTabDialog( 1, pText, 80, new NSummonHeroMarketBuyDlg, TEXT("NInterface/Data/NStructureDlg/DLG_SummonHeroMarketBuyDLG.ddf"), TEXT("dialog_summonmarket_buy") );
	pText = GetTxtFromMgr(3504);
	AddUserTabDialog( 2, pText, 80, new NStrInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );	

	// 행수 메뉴
	SetTabInfo( 1, 1 );
	pText = GetTxtFromMgr(3442);
	AddChiefTabDialog( 0, pText, 60, new NSummonHeroMarketChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_SummonHeroMarketChiefDlg.ddf"), TEXT("dialog_summonmarketchief") );
	pText = GetTxtFromMgr(3443);
	AddChiefTabDialog( 1, pText, 60, new NStrPayTaxDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf"), TEXT("dialog_strpaytax") );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 2, pText, 60, new NStrRepairDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair") );
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 3, pText, 60, new NStrResignChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );
 
	InitDlg();

	SetActionTimer( 200 );
	SetTimerOnOff( true );
}

void NSummonHeroMarketDlg::GreetingMsg()
{
	TCHAR* pText = GetTxtFromMgr(1852);
	SetMainExplain( pText );
}

void NSummonHeroMarketDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 소환영웅 팔기
	case 1: // 소환영웅 지정 팔기
		((NSummonHeroMarketUserDlg*)m_pUserChildDlg[ 0 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	}
}
