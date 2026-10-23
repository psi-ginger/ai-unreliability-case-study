#include "NLandDlg.h"

#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"

extern cltCommonLogic *pclClient;


NLandDlg::NLandDlg()
{

}

NLandDlg::~NLandDlg()
{

}

void NLandDlg::Create()
{

	CreateDlg( NLAND_DLG, pclClient->pclLandManager, 3 );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	TCHAR* pText = GetTxtFromMgr(3493);
	AddUserTabDialog( 0,pText, 80, new NLandListDlg, TEXT("NInterface/Data/NStructureDlg/DLG_LandList.ddf"), TEXT("dialog_landlist") );
    pText = GetTxtFromMgr(3494);
	AddUserTabDialog( 1, pText, 80, new NLandUserDlg, TEXT("NInterface/Data/NStructureDlg/DLG_LandUser.ddf"), TEXT("dialog_landuser") );
	pText = GetTxtFromMgr(3495);
	AddUserTabDialog( 2, pText, 80, new NStrInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );
	

	// 행수 메뉴
	SetTabInfo( 1, 1 );
	pText = GetTxtFromMgr(3442);
	AddChiefTabDialog( 0, pText, 69, new NLandChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_LandChief.ddf"), TEXT("dialog_landchief") );
	pText = GetTxtFromMgr(3443);

	AddChiefTabDialog( 1, pText, 65, new NStrPayTaxDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf"), TEXT("dialog_strpaytax") );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 2, pText, 65, new NStrRepairDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair") );
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 3, pText, 70, new NStrResignChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );
	//다이얼로그 버튼 크기 수정 [2006.06.29 성웅]
	InitDlg();

	SetActionTimer( 200 );
	SetTimerOnOff( true );

}

void NLandDlg::GreetingMsg()
{
	TCHAR* pText = GetTxtFromMgr(1829);
	SetMainExplain(pText);
}

void NLandDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// NMarketUserSellDlg 세금납부
		((NLandUserDlg*)m_pUserChildDlg[ 1 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	}
}