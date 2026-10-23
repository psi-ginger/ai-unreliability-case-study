#include "NMineDlg.h"

#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"

extern cltCommonLogic *pclClient;


NMineDlg::NMineDlg()
{

}

NMineDlg::~NMineDlg()
{

}

void NMineDlg::Create()
{

	CreateDlg( NMINE_DLG, pclClient->pclMineManager, 3 );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	TCHAR* pText = GetTxtFromMgr(3506);
	AddUserTabDialog( 0, pText, 80, new NMineListDlg, TEXT("NInterface/Data/NStructureDlg/DLG_MineList.ddf"), TEXT("dialog_minelist") );
	pText = GetTxtFromMgr(3507);
	AddUserTabDialog( 1, pText, 80, new NMineUserDlg, TEXT("NInterface/Data/NStructureDlg/DLG_MineUser.ddf"), TEXT("dialog_mineuser") );
	pText = GetTxtFromMgr(3508);
	AddUserTabDialog( 2, pText, 80, new NStrInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );
	

	// 행수 메뉴
	SetTabInfo( 1, 1 );
	pText = GetTxtFromMgr(3509);
	AddChiefTabDialog( 0, pText, 60, new NMineChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_MineChief.ddf"), TEXT("dialog_minechief") );
	pText = GetTxtFromMgr(3443);
	AddChiefTabDialog( 1, pText, 50, new NStrPayTaxDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf"), TEXT("dialog_strpaytax") );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 2, pText, 60, new NStrRepairDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair") );
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 3, pText, 60, new NStrResignChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );

	InitDlg();

	SetActionTimer( 200 );
	SetTimerOnOff( true );

}

void NMineDlg::GreetingMsg()
{
	TCHAR* pText = GetTxtFromMgr(3250);
	SetMainExplain(pText);
}

void NMineDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// NMarketUserSellDlg 세금납부
		((NMineUserDlg*)m_pUserChildDlg[ 1 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	}
}