#include "NFeastDlg.h"

#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"

extern cltCommonLogic *pclClient;


NFeastDlg::NFeastDlg()
{

}

NFeastDlg::~NFeastDlg()
{

}

void NFeastDlg::Create()
{

	CreateDlg( NFEAST_DLG, pclClient->pclFeastManager );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	TCHAR* pText = NULL;
	
	//[진성] 파티홀. 파티홀로 변경. 2008. 3. 10
	if( pclClient->IsCountrySwitch( Switch_PartyHall ) )
	{
		pText = GetTxtFromMgr(3496);
		AddUserTabDialog( 0, pText, 65, new NPartyHallInfoDlg,		 TEXT("NInterface/Data/NStructureDlg/DLG_PartyHallInfo.ddf"),		 TEXT("dialog_feastuser") );
		pText = GetTxtFromMgr(3500);
		AddUserTabDialog( 1, pText, 65, new NPartyHallPresentDlg,	 TEXT("NInterface/Data/NStructureDlg/DLG_PartyHallPresent.ddf"),	 TEXT("dialog_partyhall_present") );
		pText = GetTxtFromMgr(8145);
		AddUserTabDialog( 2, pText, 65, new NPartyHallReservationDlg, TEXT("NInterface/Data/NStructureDlg/DLG_PartyHallReservation.ddf"), TEXT("dialog_feastresv") );
	}  
	else
	{
		pText = GetTxtFromMgr(3496);
		AddUserTabDialog( 0, pText, 65, new NFeastUserDlg, TEXT("NInterface/Data/NStructureDlg/DLG_FeastUser.ddf"), TEXT("dialog_feastuser") );
		pText = GetTxtFromMgr(3497);
		AddUserTabDialog( 1, pText, 65, new NFeastChatDlg, TEXT("NInterface/Data/NStructureDlg/DLG_FeastChat.ddf"), TEXT("dialog_feastchat") );
		pText = GetTxtFromMgr(3498);
		AddUserTabDialog( 2, pText, 65,new NFeastResvDlg, TEXT("NInterface/Data/NStructureDlg/DLG_FeastResv.ddf"), TEXT("dialog_feastresv") );
	}
	
	
	
	
	pText = GetTxtFromMgr(3499);
	//연회장정보버튼 글자 잘림 수정 폭수정.[성우]

	if ( ConstServiceArea_China == pclClient->siServiceArea )
	{
		AddUserTabDialog( 3, pText, 80, new NStrInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );
	}
	else
	{
		AddUserTabDialog( 3, pText, 65, new NStrInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );
	}



	// 행수 메뉴
	SetTabInfo( 2, 1 );
		
	//[진성] 파티홀. 파티홀로 변경. 2008. 3. 10
	if( pclClient->IsCountrySwitch( Switch_PartyHall ) )
	{
		pText = GetTxtFromMgr(8338);
		AddChiefTabDialog( 0, pText, 80, new NPartyHallChief2Dlg, TEXT("NInterface/Data/NStructureDlg/DLG_PartyHallChief2.ddf"), TEXT("dialog_feastchief2") );
		pText = GetTxtFromMgr(8339);
		AddChiefTabDialog( 1, pText, 80, new NPartyHallChiefDlg,  TEXT("NInterface/Data/NStructureDlg/DLG_PartyHallChief.ddf"),  TEXT("dialog_feastchief") );
	} 
	else 
	{
		pText = GetTxtFromMgr(3500);
		AddChiefTabDialog( 0, pText, 80, new NFeastChiefDlg,  TEXT("NInterface/Data/NStructureDlg/DLG_FeastChief.ddf"),  TEXT("dialog_feastchief") );
		pText = GetTxtFromMgr(3501);
        AddChiefTabDialog( 1, pText, 80, new NFeastChief2Dlg, TEXT("NInterface/Data/NStructureDlg/DLG_FeastChief2.ddf"), TEXT("dialog_feastchief2") );
	}
	
	SetTabInfo( 2, 2 );
	pText = GetTxtFromMgr(3443);
	AddChiefTabDialog( 2, pText, 80, new NStrPayTaxDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf"), TEXT("dialog_strpaytax") );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 3, pText, 80, new NStrRepairDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair") );
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 4, pText, 80, new NStrResignChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );
 
	InitDlg();

	SetActionTimer( 200 );
	SetTimerOnOff( true );
}

void NFeastDlg::GreetingMsg()
{
	TCHAR* pText = GetTxtFromMgr(1844);
	SetMainExplain(pText);
}

void NFeastDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 내자시폭죽이용권 사용
		((NFeastChatDlg*)m_pUserChildDlg[1])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	case 1:
		((NPartyHallReservationDlg*)m_pUserChildDlg[2])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;

	}
}
