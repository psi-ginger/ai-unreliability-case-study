#include "NPostOfficeDlg.h"



#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"

#include "../common/Char/CharManager/CharManager.h"



extern cltCommonLogic *pclClient;


NPostOfficeDlg::NPostOfficeDlg()
{

}

NPostOfficeDlg::~NPostOfficeDlg()
{
	
}

void NPostOfficeDlg::Create()
{

	CreateDlg( NPOSTOFFICE_DLG, pclClient->pclPostOfficeManager, 5 );

	// 사용자 메뉴
	SetTabInfo( 2, 1 );
	TCHAR* pText = GetTxtFromMgr(3470);
	AddUserTabDialog( 0, pText, 80, new NPostOfficeSendItemDlg, TEXT("NInterface/Data/NStructureDlg/DLG_PostOfficeSendItem.ddf"), TEXT("dialog_postoffice_senditem") );
	pText = GetTxtFromMgr(3471);
	AddUserTabDialog( 1, pText, 80, new NPostOfficeSendMoneyDlg, TEXT("NInterface/Data/NStructureDlg/DLG_PostOfficeSendMoney.ddf"), TEXT("dialog_postoffice_sendmoney") );
	pText = GetTxtFromMgr(3472);
	AddUserTabDialog( 2, pText, 80, new NStrInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );	
	SetTabInfo( 2, 2 );
	pText = GetTxtFromMgr(3473);
	 if (pclClient->IsCountrySwitch(Switch_reply)	) {//답장하기 [2007.08.17 손성웅]
		 AddUserTabDialog( 3, pText, 80, new NPostOfficeRecvDlg, TEXT("NInterface/Data/NStructureDlg/DLG_PostOfficeRecv_New.ddf"), TEXT("dialog_postoffice_recv") );
	 }
	 else{
		 AddUserTabDialog( 3, pText, 80, new NPostOfficeRecvDlg, TEXT("NInterface/Data/NStructureDlg/DLG_PostOfficeRecv.ddf"), TEXT("dialog_postoffice_recv") );
	 }
	
	if( pclClient->IsCountrySwitch(Switch_Postoffice_Group_Message) )
	{
		//[진성] 단체 메시지 보내기. 버튼추가 => 2008-8-4
		pText = GetTxtFromMgr(8758);
		AddUserTabDialog( 4, pText, 80, new NNewPostOfficeSendItemDlg, TEXT("NInterface/Data/NStructureDlg/DLG_PostOfficeSendItem.ddf"), TEXT("dialog_postoffice_senditem") );

		pText = GetTxtFromMgr(3474);
		AddUserTabDialog( 5, pText, 80, new NPostOfficeRecordDlg, TEXT("NInterface/Data/NStructureDlg/DLG_PostOfficeRecord.ddf"), TEXT("dialog_postoffice_record") );
	}
	else
	{
		pText = GetTxtFromMgr(3474);
		AddUserTabDialog( 4, pText, 80, new NPostOfficeRecordDlg, TEXT("NInterface/Data/NStructureDlg/DLG_PostOfficeRecord.ddf"), TEXT("dialog_postoffice_record") );
	}
	

	

	// 행수 메뉴
	SetTabInfo( 1, 1 );
	pText = GetTxtFromMgr(3442);
	AddChiefTabDialog( 0, pText, 68, new NPostOfficeChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_PostOfficeChief.ddf"), TEXT("dialog_postoffice_chief") );
	pText = GetTxtFromMgr(3443);
	AddChiefTabDialog( 1, pText, 68, new NStrPayTaxDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf"), TEXT("dialog_strpaytax") );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 2, pText, 65, new NStrRepairDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair") );
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 3, pText, 65, new NStrResignChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );
	//글씨가 잘리는 이유로 버튼 폭 조절.[2007.07.18 손성웅]

	InitDlg();

	SetActionTimer( 200 );
	SetTimerOnOff( true );

	pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_OPENPOSTOFFICEDIALOG );
}

void NPostOfficeDlg::GreetingMsg()
{	
	TCHAR* pText = GetTxtFromMgr(1738);
    SetMainExplain(pText);
	
}

void NPostOfficeDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 물품보내기
	case 1:
		((NPostOfficeSendItemDlg*)m_pUserChildDlg[ 0 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		
		//물품을 보낸후 물품전송기록 다이얼로그의 데이터 서버로부터 갱신
		//NPostOfficeRecordDlg 의 2번 버튼(레코드받기버튼)의 메시지핸들러 호출
		((NPostOfficeRecordDlg*)m_pUserChildDlg[4])->NDlgTabProc(NULL,
			((NPostOfficeRecordDlg*)m_pUserChildDlg[4])->m_nControlID[2],NULL); 

		break;
	case 2:	// 송금하기
	case 3:
		((NPostOfficeSendMoneyDlg*)m_pUserChildDlg[ 1 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;

	//[진성] 단체 메시지 보내기 => 2008-8-5
	case 10:
	case 11:
		((NNewPostOfficeSendItemDlg*)m_pUserChildDlg[ 4 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
	}
}
