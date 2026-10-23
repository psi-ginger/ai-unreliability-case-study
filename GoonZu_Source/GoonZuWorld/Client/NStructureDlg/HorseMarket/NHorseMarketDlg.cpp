#include "NHorseMarketDlg.h"

#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"

extern cltCommonLogic *pclClient;


NHorseMarketDlg::NHorseMarketDlg()
{

}

NHorseMarketDlg::~NHorseMarketDlg()
{

}

void NHorseMarketDlg::Create()
{
	CreateDlg( NHORSEMARKET_DLG, pclClient->pclHorseMarketManager );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	TCHAR* pText = GetTxtFromMgr(3478);
	AddUserTabDialog( 0, pText, 90,new NHorseMarketUserDlg, TEXT("NInterface/Data/NStructureDlg/DLG_HorseMarketUserDlg.ddf"), TEXT("dialog_horsemarket_user") );
	
	pText = GetTxtFromMgr(3479);
	AddUserTabDialog( 1, pText, 90,new NHorseMarketBuyDlg, TEXT("NInterface/Data/NStructureDlg/DLG_HorseMarketBuyDlg.ddf"), TEXT("dialog_horsemarket_buy") );

	pText = GetTxtFromMgr(3480);
	AddUserTabDialog( 2, pText, 90,new NStrInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );	


	//각메뉴의 넢이 조절은 pText 옆의 숫자.
	//90이상 넘으면 안될듯.

	//m_pChiefTabButton[0]->Move()
	// 행수 메뉴
	SetTabInfo( 1, 1 );
	pText = GetTxtFromMgr(3442);
	AddChiefTabDialog( 0, pText, 67, new NHorseMarketChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_HorseMarketChiefDlg.ddf"), TEXT("dialog_horsemarketchief") );
	pText = GetTxtFromMgr(3443);
	AddChiefTabDialog( 1, pText, 67, new NStrPayTaxDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf"), TEXT("dialog_strpaytax") );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 2, pText, 67, new NStrRepairDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair") );
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 3, pText, 67, new NStrResignChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );



	InitDlg();

	SetActionTimer( 200 );
	SetTimerOnOff( true );
}

void NHorseMarketDlg::GreetingMsg()
{
	TCHAR* pText = GetTxtFromMgr(1766);
	SetMainExplain( pText );
}

void NHorseMarketDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 말 팔기
		((NHorseMarketUserDlg*)m_pUserChildDlg[ 0 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	}
}