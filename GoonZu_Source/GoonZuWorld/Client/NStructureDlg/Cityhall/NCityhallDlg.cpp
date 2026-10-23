#include "NCityhallDlg.h"

#include "NVillageTreeDlg.h"
#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"

extern cltCommonLogic *pclClient;


NCityhallDlg::NCityhallDlg()
{

}

NCityhallDlg::~NCityhallDlg()
{
	cltClient* pclclient = (cltClient*)pclClient;

    if(pclclient != NULL && pclclient->m_pDialog[ NVILLAGETREE_DLG ] != NULL 
		&& ((NVillageTreeDlg*)pclclient->m_pDialog[ NVILLAGETREE_DLG ])->IsCreate() == true )
		((NVillageTreeDlg*)pclclient->m_pDialog[ NVILLAGETREE_DLG ])->DeleteDialog();

	if( pclclient->m_pDialog[ NGENERALMEETING2_DLG ] )
	{
		pclclient->m_pDialog[ NGENERALMEETING2_DLG ]->DeleteDialog();
	}
}

void NCityhallDlg::Create()
{

	CreateDlg( NCITYHALL_DLG, pclClient->pclCityHallManager );


	// 사용자 메뉴
	SetTabInfo( 2, 1 );
	TCHAR* pText = GetTxtFromMgr(3461);
	AddUserTabDialog( 0, pText, 80, new NCityhallInfoDlg, 
		TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_Info.ddf"), TEXT("dialog_cityhallinfo") );

	 pText = GetTxtFromMgr(3462);
	AddUserTabDialog( 1, pText, 80, new NCityhallInfo2Dlg, 
		TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_Info2.ddf"), TEXT("dialog_cityhallinfo2") );
	
	// 주주 명부
	pText = GetTxtFromMgr(3463);
	AddUserTabDialog( 2, pText, 80, new NCityhallStockHolderDlg, TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_StockHolder.ddf"), TEXT("dialog_cityhallstockholder") );


	 pText = GetTxtFromMgr(3464);
	SetTabInfo( 2, 2 );
	AddUserTabDialog( 3, pText, 80, new NCityhallResidentDlg, 
		TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_Resident.ddf"), TEXT("dialog_cityhallresident") );

	// KHY - 주주 총회 버튼 확장
	pText = GetTxtFromMgr(3465);
	AddUserTabDialog( 4, pText, 160, new NCityhallGeneralMeetingDlg, TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_GeneralMeeting.ddf"), TEXT("dialog_cityhallgeneralmeeting") );

	
	if(pclClient->IsCountrySwitch(Switch_DonateDlg)) //KHY - 기부금제도 국가제한.
	{
		pText = GetTxtFromMgr(10163);
		AddUserTabDialog( 5, pText, 100, new NCityhallDonateDlg, TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_Donate.ddf"), TEXT("dialog_donate") );
	}
#ifdef _CHINA
	SetTabInfo( 2, 3 );

	pText = GetTxtFromMgr(10204);
	 AddUserTabDialog( 6, pText, 80, new NCityhallUserMenuEmpireDlg, TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_Empire.ddf"), TEXT("dialog_Empire") );
#endif

	// 배당관련탭 통합
	// pText = GetTxtFromMgr(5254);
	//AddUserTabDialog( 5, pText, 80, new NCityhallDividendListDlg, 
	//	TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_DividedListDlg.ddf"), TEXT("dialog_cityhalldividedlist") );
	

	// 행수 메뉴
	SetTabInfo( 2, 1 );
	 pText = GetTxtFromMgr(3466);
	//cyj 행수메뉴 탭사이즈도 64에서 80으로 변경(마을지도 변경 메뉴 삭제로 인하여)
	AddChiefTabDialog( 0, pText, 60, new NCityhallChiefDlg, 
		TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_Chief.ddf"), TEXT("dialog_cityhallchief") );

	pText = GetTxtFromMgr(3467);
	AddChiefTabDialog( 1, pText, 60, new NCityhallCapitalDlg, 
		TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_Capital.ddf"), TEXT("dialog_cityhallcapital") );

	if ( ConstServiceArea_China == pclClient->siServiceArea )
	{
		pText = GetTxtFromMgr(3468);
		AddChiefTabDialog( 2, pText, 50, new NCityhallGodDlg, 
			TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_God.ddf"), TEXT("dialog_cityhallgod") );

		pText = GetTxtFromMgr(5998);
		AddChiefTabDialog( 3, pText, 100, new NCityhallSetBaseMoneyDlg, 
			"NInterface/Data/NStructureDlg/DLG_Cityhall_SetBaseMoney.ddf", "dialog_cityhallsetbasemoney" );
	}
	else
	{
		pText = GetTxtFromMgr(3468);
		AddChiefTabDialog( 2, pText, 60, new NCityhallGodDlg, 
			TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_God.ddf"), TEXT("dialog_cityhallgod") );

		pText = GetTxtFromMgr(5998);
		AddChiefTabDialog( 3, pText, 90, new NCityhallSetBaseMoneyDlg, 
			"NInterface/Data/NStructureDlg/DLG_Cityhall_SetBaseMoney.ddf", "dialog_cityhallsetbasemoney" );
	}

	SetTabInfo( 2, 2 );

	pText = GetTxtFromMgr(3469);
	AddChiefTabDialog( 4, pText, 90, new NCityhallEventDlg, 
		TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_Event.ddf"), TEXT("dialog_cityhallevent") );

	//pText = GetTxtFromMgr(1536);	

	//cyj 관청의 마을지도 변경 메뉴 임시 삭제
	//pText = GetTxtFromMgr(5052);
	//AddChiefTabDialog( 4, pText, 128, new NCityhallChangeMapDlg, 
	//	TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_ChangeMap.ddf"), TEXT("dialog_cityhallchangemap") );

	pText = GetTxtFromMgr(3444);	
	AddChiefTabDialog( 5, pText, 90, new NStrRepairDlg, 
		TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair") );

	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 6, pText, 90, new NStrResignChiefDlg, 
		TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );

#ifdef _CHINA
	SetTabInfo( 2, 3 );

	pText = GetTxtFromMgr(10212);
	AddChiefTabDialog( 7, pText, 80, new NCityhallChiefMenuEmpireDlg, TEXT("NInterface/Data/NStructureDlg/DLG_Cityhall_Empire2.ddf"), TEXT("dialog_Empire2") );
#endif

	// [영훈] 초보자 마을 : 관청 버튼들의 각종 제한사항
	if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
	{
		if ( Const_Beginner_Village == pclClient->pclCityHallManager->siVillageUnique )
		{
			//-- 유저 텝버튼
			DisableTabButton( 2 );	// 주주 명부
			DisableTabButton( 4 );	// 주주 총회
			if ( pclClient->IsCountrySwitch(Switch_DonateDlg) )
			{
				DisableTabButton( 5 );	// 기부제도
			}
			
			//-- 관리자 텝버튼
			DisableTabButton( 1, true );
			DisableTabButton( 2, true );
			DisableTabButton( 3, true );
			DisableTabButton( 4, true );
			DisableTabButton( 5, true );
			DisableTabButton( 6, true );
			if ( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
			{
				DisableTabButton( 7, true );
			}
		}
	}




	InitDlg(TRUE);	// CityHall창 열었을 때만 마을 상태(파산, 정상,.. ) 보여줌

	SetActionTimer( 200 );
	SetTimerOnOff( true );

}

void NCityhallDlg::GreetingMsg()
{
	// 마을의 Unique를 구한다. 
	TCHAR* pText = GetTxtFromMgr(1610);
	SetMainExplain(pText);
}

void NCityhallDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	// User Tab
	case 0:
	case 1: // NCityhallGeneralMeetingDlg
		((NCityhallGeneralMeetingDlg*)m_pUserChildDlg[ 4 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	case 2:
	case 3: // NCityhallResidentDlg
		((NCityhallResidentDlg*)m_pUserChildDlg[ 3 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	// Chief Tab		
	case 20:	// NCityhallEventDlg
		((NCityhallEventDlg*)m_pChiefChildDlg[ 4 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	case 21:	//NCityhallGodDlg
	case 22:
	case 23:
	case 24:
	case 25:
		((NCityhallGodDlg*)m_pChiefChildDlg[ 2 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	case 26:	// NCityhallChangeMapDlg
		((NCityhallChangeMapDlg*)m_pChiefChildDlg[ 4 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	case 30:
		((NCityhallChiefDlg*)m_pChiefChildDlg[0])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	case 31:
		((NCityhallDonateDlg*)m_pUserChildDlg[5])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	}
}
