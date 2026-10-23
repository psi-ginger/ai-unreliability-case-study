#include "NBankDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"

extern cltCommonLogic *pclClient;

NBankDlg::NBankDlg()
{

}

NBankDlg::~NBankDlg()
{

}

void NBankDlg::Create()
{
	CreateDlg( NBANK_DLG, pclClient->pclBankManager, 1 );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	TCHAR* pText = GetTxtFromMgr(3440);
	AddUserTabDialog( 0, pText, 80, new NBankUserDlg, TEXT("NInterface/Data/NStructureDlg/DLG_BankUserDlg.ddf"), TEXT("dialog_bankuser") );
	pText = GetTxtFromMgr(3441);
	AddUserTabDialog( 1, pText, 80, new NBankInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_BankInfoDlg.ddf"), TEXT("dialog_bankinfo") );

	// 행수 메뉴
	SetTabInfo( 1, 1 );
	pText = GetTxtFromMgr(3442);
	AddChiefTabDialog( 0, pText, 60, new NBankChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_BankChiefDlg.ddf"), TEXT("dialog_bankchief") );
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

void NBankDlg::GreetingMsg()
{
	TCHAR* pText = GetTxtFromMgr(1452);
	SetMainExplain( pText );
}

void NBankDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 통장 해지 // NBankUserDlg
		((NBankUserDlg*)m_pUserChildDlg[ 0 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	case 1:	// 전액 출금 // NBankUserDlg
		((NBankUserDlg*)m_pUserChildDlg[ 0 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	}
}
