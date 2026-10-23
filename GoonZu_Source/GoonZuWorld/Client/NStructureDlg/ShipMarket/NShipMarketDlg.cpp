#include "NShipMarketDlg.h"

#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"

extern cltCommonLogic *pclClient;


NShipMarketDlg::NShipMarketDlg()
{

}

NShipMarketDlg::~NShipMarketDlg()
{

}

void NShipMarketDlg::Create()
{
	CreateDlg( NSHIPMARKET_DLG, pclClient->pclShipMarketManager );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	char* pText = GetTxtFromMgr(3489);
	AddUserTabDialog( 0, pText, 80, new NShipMarketUserDlg, "NInterface/Data/NStructureDlg/DLG_ShipMarketUserDLG.ddf", "dialog_shipmarket_user" );
	pText = GetTxtFromMgr(3490);
	AddUserTabDialog( 1, pText, 80, new NShipMarketBuyDlg, "NInterface/Data/NStructureDlg/DLG_ShipMarketBuyDLG.ddf", "dialog_shipmarket_buy" );
	pText = GetTxtFromMgr(3491);
	AddUserTabDialog( 2, pText, 80, new NStrInfoDlg, "NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf", "dialog_strinfo" );	

	// 행수 메뉴
	SetTabInfo( 1, 1 );
	pText = GetTxtFromMgr(3442);
	AddChiefTabDialog( 0, pText, 60, new NShipMarketChiefDlg, "NInterface/Data/NStructureDlg/DLG_ShipMarketChiefDlg.ddf", "dialog_shipmarketchief" );
	pText = GetTxtFromMgr(3443);
	AddChiefTabDialog( 1, pText, 60, new NStrPayTaxDlg, "NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf", "dialog_strpaytax" );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 2, pText, 60, new NStrRepairDlg, "NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf", "dialog_strrepair" );
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 3, pText, 60, new NStrResignChiefDlg, "NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf", "dialog_resignchief" );
 
	InitDlg();

	SetActionTimer( 200 );
	SetTimerOnOff( true );
}

void NShipMarketDlg::GreetingMsg()
{
	char* pText = GetTxtFromMgr(1819);
	SetMainExplain( pText );
}

void NShipMarketDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, char *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 선박 팔기
	case 1: // 선박 지정 팔기
		((NShipMarketUserDlg*)m_pUserChildDlg[ 0 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	}
}

