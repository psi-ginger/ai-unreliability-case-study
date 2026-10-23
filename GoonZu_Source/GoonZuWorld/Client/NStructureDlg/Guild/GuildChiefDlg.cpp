#include "GuildDlg.h"

#include "../../InterfaceMgr/Interface/List.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/MsgType-Guild.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NGuildChiefDlg::NGuildChiefDlg()
{
}

NGuildChiefDlg::~NGuildChiefDlg()
{

}

void NGuildChiefDlg::InitDlg()
{
	SetControlMap( EDIT_RENT_FEE, TEXT("editbox_feerate"));
	SetControlMap( EDIT_FEE_RATE, TEXT("editbox_rent_fee"));

	SetControlMap( BUTTON_SETUP, TEXT("button_setup") );

	SetControlMap( LIST_APPLYLIST, TEXT("listbox_application") );

	SetControlMap( BUTTON_ACCEPT, TEXT("button_setup") );
	SetControlMap( STATIC_RATE, TEXT("statictext_NONAME2") );

	// 리스트 
	CList *pListApply = (CList*)m_pControlMap[ LIST_APPLYLIST ];
	
	
	pListApply->SetBorder(true);
	pListApply->SetBKColorUse(true);

	pListApply->Refresh();

	CEdit *pEdit = (CEdit*)m_pControlMap[ EDIT_FEE_RATE ];
	pEdit->SetText("1,000,000");
	pEdit->Refresh();
	// 계약 조건을 보여준다. 
	ShowRentContract();
}

void NGuildChiefDlg::Action()
{
	CEdit* pEdit = (CEdit*)m_pControlMap[EDIT_RENT_FEE];
	CButton* pButton = (CButton*)m_pControlMap[BUTTON_SETUP];

	SI32 villageunique = pclClient->pclCM->CR[1]->pclMap->siVillageUnique ;

	if ( pclClient->pclCM->CR[1]->pclCI->clCharRank.siType == RANKTYPE_GUILD &&
		 pclClient->pclCM->CR[1]->pclMap->siVillageUnique == pclClient->pclCM->CR[1]->pclCI->clCharRank.siVillageUnique )
	{
		// [영훈] 초보자 마을 : 임대설정 버튼 막기
		if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
		{
			if ( Const_Beginner_Village == villageunique )
			{
				pEdit->Enable(false);
				pButton->Enable(false);
				return;
			}
		}

		pEdit->Enable(true);
		pButton->Enable(true);
	}
	else
	{
		pEdit->Enable(false);
		pButton->Enable(false);
	}
}

void NGuildChiefDlg::ShowRentContract()
{
	cltGuildManager *pGuildManager = ((cltGuildManager *)m_pStrManager);
	CEdit* pEdit = (CEdit*)m_pControlMap[EDIT_RENT_FEE];

	TCHAR buffer[128] = "";
	sprintf(buffer,"%d",pGuildManager->clClientStrInfo.clGuildStrInfo.clFee.GetFeeRate() / 10 );

	pEdit->SetText(buffer);
}

void NGuildChiefDlg::ShowGuildUnitListInChief()
{
}

void NGuildChiefDlg::ShowGuildApplyerListInChief()
{
}

void NGuildChiefDlg::ShowSelectedGuildUnitExplain( int index )
{
}


void NGuildChiefDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltGuildManager *pGuildManager = ((cltGuildManager *)m_pStrManager);

	// 리스트 
	CList *pListApply = (CList*)m_pControlMap[ LIST_APPLYLIST ];

	CASE_CONTROL( BUTTON_SETUP )
	{
		CEdit* pEdit = (CEdit*)m_pControlMap[EDIT_RENT_FEE];

		SI32 fee = _tstoi(pEdit->GetText());

		if ( fee <= 0 || fee > 10 )
		{		
			// [추가 : 황진성 2008. 1. 18 => 길드 사무소 수수료 설정 초과시 설명창에 알림.]
			SetMainExplain(GetTxtFromMgr(7878));								
			return ;
		}

		cltGameMsgRequest_Guild_SetStrFeeRate clinfo(pclClient->pclCM->CR[1]->pclMap->siVillageUnique,fee);
		cltMsg clMsg(GAMEMSG_REQUEST_GUILD_SETSTRFEERATE, sizeof(clinfo), (BYTE*)&clinfo);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
}