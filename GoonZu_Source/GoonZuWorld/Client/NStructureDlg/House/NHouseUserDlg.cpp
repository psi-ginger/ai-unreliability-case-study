#include "NHouseDlg.h"

//#include "../../InterfaceMgr/Interface/ListView.h"
//#include "../StrBaseDlg/StrInventory.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NHouseUserDlg::NHouseUserDlg()
{
}

NHouseUserDlg::~NHouseUserDlg()
{
	
}

void NHouseUserDlg::InitDlg()
{

	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);

	SetControlMap( EDIT_RENT_FEE, TEXT("editbox_rent_fee") );
	SetControlMap( EDIT_RENT_PERIOD, TEXT("editbox_rent_period") );

	SetControlMap( EDIT_CONTRACT, TEXT("editbox_contract") );

	SetControlMap( BUTTON_APPLY, TEXT("button_apply") );
	SetControlMap( BUTTON_CANCEL_APPLY, TEXT("button_cancel_apply") );
    
	ShowRentContractInUser();


}

void NHouseUserDlg::ShowRentContractInUser()
{

	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);

	TCHAR buffer[1024];

	TCHAR		szPrice[64] = { '\0', };
	GMONEY		siPrice = pHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract.siRentFee ;
	g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));

	StringCchPrintf(buffer, 1024, TEXT("%s"), szPrice);
	SetEditText( EDIT_RENT_FEE, buffer );

	StringCchPrintf(buffer, 1024, TEXT("%d"), pHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract.siRentPeriod);
	SetEditText( EDIT_RENT_PERIOD, buffer );

	TCHAR* pText = GetTxtFromMgr(1703);
	StringCchPrintf(buffer, 1024, pText);
	SetEditText( EDIT_CONTRACT, buffer );

	if(pclClient->IsCountrySwitch(Switch_BasicStructureSet)){
		SI32 dura = pclClient->pclHouseManager->clClientStrInfo.clCommonInfo.siStrDura;//건물의 내구도를 가지고 온다
		if (dura <= 0)
		{
			CButton *pButton = (CButton *)m_pControlMap[ BUTTON_APPLY ];
			pButton->Enable(false);

		}
		else
		{
			CButton *pButton = (CButton *)m_pControlMap[ BUTTON_APPLY ];
			pButton->Enable(true);

		}

	}

	

}

void NHouseUserDlg::Action()
{
	if( m_pBaseDlg->m_dwActionCount % 4 ) return;

	ShowRentContractInUser();
}


void NHouseUserDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);
	CASE_CONTROL( EDIT_RENT_FEE )
	{
		switch( nEvent ) {
		case EVENT_EDITBOX_CHANGE:
			{
				NTCHARString512	kRawPrice(GetEditText( EDIT_RENT_FEE ));
				kRawPrice.ReplaceAll(",", "");	// , 삭제
				TCHAR		szPrice[64] = { '\0', };
				GMONEY		siPrice = _tstoi64(kRawPrice);
				if( kRawPrice.Length() > 0 )
					g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));
				SetEditText( EDIT_RENT_FEE, szPrice );
			}
			break;
		}
	}

	CASE_CONTROL( BUTTON_APPLY )
	{
		// 대상이 되는 캐릭터의 아이디를 구한다. 
		SI32 id = 1;

		//현제 계좌.[2006.06.08 성웅]
		BOOL bAccount = pclCM->CR[id]->pclCI->clBank.clAccountNum.IsValid();

		// 필요 자격 조건을 충족시키고 있는지 확인한다. 
		if( pclClient->pclCM->CR[id]->pclCI->clIP.GetLevel() < LEVELLIMIT_USE_HOUSE)
		{
			TCHAR buffer[256];
			TCHAR* pText = GetTxtFromMgr(1051);
			StringCchPrintf(buffer, 256, pText, LEVELLIMIT_USE_HOUSE);
			SetMainExplain( buffer);

			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			return;
		}

		//만약 계좌가 없다면 실패[2006.06.08 성웅]

		if( bAccount==FALSE)
		{			
			TCHAR* pText = GetTxtFromMgr(15241);
			
			SetMainExplain( pText);

			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			return;
		}
		//만약 계좌의 돈이 현제 임대료보다 작은지 검사하여 작으면 실패[2006.06.08 성웅]
		if( pclClient->pclCM->CR[id]->GetBankMoney() < pHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract.siRentFee)
		{
			TCHAR* pText = GetTxtFromMgr(7115);
			SetMainExplain( pText);
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			return;
		}
		//3개 이상을 사용하면 임차가 안되도록
			
		// 임대 기간을 확인한다.
		if( pHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract.siRentPeriod == 0)
		{
			TCHAR* pText = GetTxtFromMgr(1699);
			SetMainExplain(pText);
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
			return;
		}

		// 임대신청을 접수한다. 
		cltRentContractPacket sendMsg( pHouseManager->siVillageUnique, &pHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract);
		cltMsg clMsg(GAMEMSG_REQUEST_HOUSERENTAPPLY, sizeof(cltRentContractPacket), (BYTE*)&sendMsg);
		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	else CASE_CONTROL( BUTTON_CANCEL_APPLY )
	{
		/// 취소 신청을 서버에 보낸다. 
		cltGameMsgRequest_HouseRentApplyCancel clInfo(pHouseManager->siVillageUnique);
		cltMsg clMsg(GAMEMSG_REQUEST_HOUSERENTAPPLYCANCEL, sizeof(clInfo), (BYTE*)&clInfo);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void NHouseUserDlg::Show()
{
	NStrTabDlg::Show();

	pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_LEASECLICK );
}
