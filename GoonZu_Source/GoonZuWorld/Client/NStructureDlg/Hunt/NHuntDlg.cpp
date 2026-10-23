#include "NHuntDlg.h"

#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"

extern cltCommonLogic *pclClient;


NHuntDlg::NHuntDlg()
{

}

NHuntDlg::~NHuntDlg()
{

}

void NHuntDlg::Create()
{

	CreateDlg( NHUNT_DLG, pclClient->pclHuntManager );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	TCHAR* pTitle = GetTxtFromMgr(3843);
	AddUserTabDialog( 0, pTitle, 80, new NHuntUserDlg, TEXT("NInterface/Data/NStructureDlg/DLG_HuntUser.ddf"), TEXT("dialog_huntuser") );
	pTitle = GetTxtFromMgr(3844);
	AddUserTabDialog( 2, pTitle, 80, new NStrInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );

	//_LEON_HUNTMAP
	// 행수 메뉴
//#if defined(_DEBUG)
	SetTabInfo( 2, 1 );
//#else
//	SetTabInfo( 1, 1 );
//#endif
	TCHAR* pText = GetTxtFromMgr(5056);
	AddChiefTabDialog(	 0, pText, 80, new NHuntChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_HuntChief.ddf"), TEXT("dialog_huntchief") );
	pText = GetTxtFromMgr(4712);
	AddChiefTabDialog( 1,pText, 60, new NStrPayTaxDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf"), TEXT("dialog_strpaytax") );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 2, pText, 60, new NStrRepairDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair") );
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 3, pText, 60, new NStrResignChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );
//#if defined(_DEBUG)
	SetTabInfo( 2, 2 );
	pText = GetTxtFromMgr(5508);
	AddChiefTabDialog( 4, pText, 140, new NHuntChangeMapDlg, TEXT("NInterface/Data/NStructureDlg/DLG_Hunt_ChangeMap.ddf"), TEXT("dialog_huntchangemap") );
//#endif

	void LoadLineUpInfoList();
	void LoadHuntStg();

	InitDlg();

	SetActionTimer( 200 );
	SetTimerOnOff( true );
}

void NHuntDlg::GreetingMsg()
{
	TCHAR* pText = GetTxtFromMgr(2153);
	SetMainExplain(pText);
}

void NHuntDlg::VDirectDrawToBackScreen(LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY)
{
	if(((NHuntChangeMapDlg*)m_pChiefChildDlg[ 4 ])->IsShow() == true )
		((NHuntChangeMapDlg*)m_pChiefChildDlg[ 4 ])->VDirectDrawToBackScreen( lpBackScreen, ScreenRX, ScreenRY);
}

void NHuntDlg::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE* pData, SI16 DataLen)
{
	switch(MsgIndex)
	{
	case 10:	// 수렵장 지도 변경
		{
			((NHuntChangeMapDlg*)m_pChiefChildDlg[ 4 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen);
		}
		break;
	}
}


