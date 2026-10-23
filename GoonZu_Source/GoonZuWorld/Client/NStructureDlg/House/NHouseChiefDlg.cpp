#include "NHouseDlg.h"

#include "../../InterfaceMgr/Interface/List.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

#include "Server.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NHouseChiefDlg::NHouseChiefDlg()
{
}

NHouseChiefDlg::~NHouseChiefDlg()
{
	
}

void NHouseChiefDlg::InitDlg()
{

	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);

	SetControlMap( EDIT_RENT_FEE, TEXT("editbox_rent_fee") );
	SetControlMap( EDIT_RENT_PERIOD, TEXT("editbox_rent_period") );

	SetControlMap( EDIT_EXPLAIN, TEXT("editbox_explain") );

	SetControlMap( LIST_RENTLIST, TEXT("listbox_rentlist") );
	SetControlMap( LIST_APPLYLIST, TEXT("listbox_applylist") );

	SetControlMap( BUTTON_SETUP, TEXT("button_setup") );

	SetControlMap( BUTTON_REFRESH_RENTLIST, TEXT("button_refresh_rentlist") );
	SetControlMap( BUTTON_REFRESH_APPLYLIST, TEXT("button_refresh_applylist") );

	SetControlMap( BUTTON_RENT, TEXT("button_rent") );
	SetControlMap( BUTTON_CLEAR_LIST, TEXT("button_clear_list") );

	// 리스트 
	CList *pListRent = (CList*)m_pControlMap[ LIST_RENTLIST ];
	CList *pListApply = (CList*)m_pControlMap[ LIST_APPLYLIST ];

	pListRent->SetBorder(true);
	pListRent->SetBKColorUse(true);

	pListApply->SetBorder(true);
	pListApply->SetBKColorUse(true);


	pListRent->SetColumn( 0, 148, TEXT("") );
	pListApply->SetColumn( 0, 148, TEXT("") );

	pListRent->Refresh();
	pListApply->Refresh();

	// 계약 조건을 보여준다. 
	ShowRentContract();

	pHouseManager->CurrentSelectedHouseUnitSlot	= -1;
	pHouseManager->CurrentSelectedApplyerSlot	= -1;

	cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
	if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) )
	{
		//-------------------------------------
		// 서버로 시전리스트를 요청한다. 
		//-------------------------------------
		pHouseManager->RequestHouseUnitList();
		
		// 게임서버로 단위시전의 상세 정보 리스트를 요청한다. 
		pHouseManager->RequestHouseUnitListDetail();

		//------------------------------------------
		// 서버로 시전임차 희망자 리스트를 요청한다. 
		//------------------------------------------
		pHouseManager->RequestHouseRentApplyerList();
	}

}

void NHouseChiefDlg::Action()
{

	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);

	if( pHouseManager->bUpdateHouseUnitListShowChiefSwitch )
	{
		pHouseManager->bUpdateHouseUnitListShowChiefSwitch = false;

		// 시전 유니트 정보를 보여준다. 
		ShowHouseUnitListInChief();
	}

	if( pHouseManager->UpdateRentContractShowSwitch )
	{
		pHouseManager->UpdateRentContractShowSwitch = FALSE;

		// 계약 조건을 보여준다. 
		ShowRentContract();
	}

	if( pHouseManager->UpdateHouseApplyerListShowSwitch )
	{
		pHouseManager->UpdateHouseApplyerListShowSwitch = FALSE;

		// 임차 희망자 리스트를 보여준다. 
		ShowHouseApplyerListInChief();
	}
}

void NHouseChiefDlg::ShowRentContract()
{

}

void NHouseChiefDlg::ShowHouseUnitListInChief()
{
	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);
	// 리스트 
	CList *pListRent = (CList*)m_pControlMap[ LIST_RENTLIST ];
	CList *pListApply = (CList*)m_pControlMap[ LIST_APPLYLIST ];
	stListBoxItemData itemdata;

	SI32 i;
	TCHAR buffer[256];

	// 기존 리스트를 지운다. 
	pListRent->Clear();

	for(i = 0;i < MAX_HOUSEUNIT_NUMBER;i++)
	{
		if( pHouseManager->clClientUnit[i].clPerson.GetPersonID() == 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1691);
			StringCchPrintf(buffer, 256, TEXT("%d %s"), i+1, pText);
		}
		else
		{
			StringCchPrintf(buffer, 256, TEXT("%d %s"), i+1, pHouseManager->clClientUnit[i].clPerson.GetName() );
		}

		itemdata.Init();
		itemdata.Set( 0, buffer );

		pListRent->AddItem( &itemdata );
	}

	pListRent->Refresh();
}

void NHouseChiefDlg::ShowHouseApplyerListInChief()
{
	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);
	// 리스트 
	CList *pListRent = (CList*)m_pControlMap[ LIST_RENTLIST ];
	CList *pListApply = (CList*)m_pControlMap[ LIST_APPLYLIST ];
	stListBoxItemData itemdata;

	SI32 i;
	TCHAR buffer[256];

	// 기존 리스트를 지운다. 
	pListApply->Clear();

	for(i = 0;i < MAX_APPLY_PER_HOUSE;i++)
	{
		if(pHouseManager->clClientApplyer[i].clPerson.GetPersonID() == 0)
		{
			TCHAR* pText = GetTxtFromMgr(1692);
			StringCchPrintf(buffer, 256, TEXT("%d %s"), i+1, pText);			
		}
		else
		{
			StringCchPrintf(buffer, 256, TEXT("%d %s"), i+1, pHouseManager->clClientApplyer[i].clPerson.GetName());
		}

		itemdata.Init();
		itemdata.Set( 0, buffer );

		pListApply->AddItem( &itemdata);
	}

	pListApply->Refresh();

}
//bts창고메세지.

void NHouseChiefDlg::ShowSelectedHouseUnitExplain( int index )
{
	
	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);
	// 리스트 
	CList *pListRent = (CList*)m_pControlMap[ LIST_RENTLIST ];
	CList *pListApply = (CList*)m_pControlMap[ LIST_APPLYLIST ];

	TCHAR buffer[256];
	TCHAR date[128];
	if(index < 0)return;

	// 계약 만기일을 구한다. 
	cltDate cldate;
	cldate.Set(&pHouseManager->clClientUnit[index].clContract.clDate);
	cldate.AddMonth(pHouseManager->clClientUnit[index].clContract.siRentPeriod*12);
	
	cldate.GetDateText(date);

	TCHAR* pText = GetTxtFromMgr(1693);
	StringCchPrintf(buffer, 256, pText, index+1, 
		pHouseManager->clClientUnit[index].clPerson.GetName(),
		pHouseManager->clClientUnit[index].clContract.siRentFee);

	StringCchCat( buffer, 256, date );

	SetEditText( EDIT_EXPLAIN, buffer );
	
}


void NHouseChiefDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);

	// 리스트 
	CList *pListRent = (CList*)m_pControlMap[ LIST_RENTLIST ];
	CList *pListApply = (CList*)m_pControlMap[ LIST_APPLYLIST ];


	TCHAR buffer[ 1024 ];

	CASE_CONTROL( EDIT_RENT_FEE )
	{
		switch( nEvent ) {
		case EVENT_EDITBOX_CHANGE:
			{
				NTCHARString512	kRawPrice(GetEditText( EDIT_RENT_FEE ));
				kRawPrice.ReplaceAll(",", "");	// , 삭제
				TCHAR		szPrice[64] = { '\0', };
				GMONEY		siPrice = _tstoi64(kRawPrice);
				if( siPrice > pclClient->GetGlobalValue("GV_FeeLimit"))
					siPrice = pclClient->GetGlobalValue("GV_FeeLimit");

				if( kRawPrice.Length() > 0 )
					g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));
				SetEditText( EDIT_RENT_FEE, szPrice );
			}
			break;
		}
	}
	

	CASE_CONTROL( LIST_RENTLIST )
	{
		switch( nEvent )
		{
			case EVENT_LISTBOX_SELECTION:
			{

				// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
				{
					cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
					if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE ) return;
				}

				SI16 index = pListRent->GetSelectedIndex();
				pHouseManager->CurrentSelectedHouseUnitSlot = index;

				if( index < 0 ) return;

				ShowSelectedHouseUnitExplain( index );
			}
			break;
		}

	}

	else CASE_CONTROL( LIST_APPLYLIST )
	{
		switch( nEvent )
		{
			case EVENT_LISTBOX_SELECTION:
			{
				SI16 index = pListApply->GetSelectedIndex();
				pHouseManager->CurrentSelectedApplyerSlot = index;

			}
			break;
		}
	}

	else CASE_CONTROL( BUTTON_SETUP )
	{

		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		SI32 id = 1;

		//--------------------------------------
		// 월 임대료를 확인한다. 
		//--------------------------------------

		NTCHARString512	kRawPrice(GetEditText( EDIT_RENT_FEE ));
		kRawPrice.ReplaceAll(",", "");	// , 삭제
		TCHAR		szPrice[64] = { '\0', };
		GMONEY		siPrice = _tstoi64(kRawPrice);
		SI64 fee = siPrice ;

		if(fee < MIN_HOUSE_RENT_FEE)
		{
			TCHAR* pText = GetTxtFromMgr(1682);

			StringCchPrintf(buffer, 1024, pText, MIN_HOUSE_RENT_FEE);
			SetMainExplain(buffer);
			return;
		}

		if(fee > MAX_HOUSE_RENT_FEE) 
		{
			TCHAR* pText = GetTxtFromMgr(1683);

			StringCchPrintf(buffer, 1024, pText, MAX_HOUSE_RENT_FEE );
			SetMainExplain(buffer);
			return;
		}


		//--------------------------------------
		// 계약 기간을 확인한다. 
		//--------------------------------------
		SI32 period	= _tstoi( GetEditText( EDIT_RENT_PERIOD ) );

		if( period <=0 || period > MAX_RENT_PERIOD_YEAR )
		{
			TCHAR* pText = GetTxtFromMgr(1684);
			SetMainExplain(pText);
			return;
		}

		//-------------------------------------
		// 서버로 계약조건을 보내서 승인 받는다. 
		//--------------------------------------
		cltRentContract clcontract;
		clcontract.Set(fee, period);

		cltRentContractPacket sendMsg( pHouseManager->siVillageUnique, &clcontract);
		
		cltMsg clMsg(GAMEMSG_REQUEST_CHANGEHOUSECONTRACT, sizeof(cltRentContractPacket), (BYTE*)&sendMsg);
		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	}

	else CASE_CONTROL( BUTTON_REFRESH_RENTLIST )
	{
		//-------------------------------------
		// 서버로 시전리스트를 요청한다. 
		//-------------------------------------
		pHouseManager->RequestHouseUnitList();
	
		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		// 게임서버로 단위시전의 상세 정보 리스트를 요청한다. 
		pHouseManager->RequestHouseUnitListDetail();
	}

	else CASE_CONTROL( BUTTON_REFRESH_APPLYLIST )
	{

		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		//------------------------------------------
		// 서버로 시전임차 희망자 리스트를 요청한다. 
		//------------------------------------------
		pHouseManager->RequestHouseRentApplyerList();
	}

	else CASE_CONTROL( BUTTON_RENT )
	{
		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);

			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}


		SI32 id = 1;

		//------------------------------------------
		// 선택된 시전유니트가 빈곳인지 확인한다.
		//------------------------------------------
		if( pHouseManager->CurrentSelectedHouseUnitSlot < 0)
		{
			TCHAR* pText = GetTxtFromMgr(1685);
			SetMainExplain( pText);
			return;
		}
		
		if( pHouseManager->clClientUnit[ pHouseManager->CurrentSelectedHouseUnitSlot].clPerson.GetPersonID() )
		{
			TCHAR* pText = GetTxtFromMgr(1686);
			SetMainExplain(pText);
			return;
		}

		//--------------------------------------
		// 선택된 희망자가 유효한지 확인한다. 
		//--------------------------------------
		if( pHouseManager->CurrentSelectedApplyerSlot < 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1687);
			SetMainExplain(pText);
			return;
		}

		SI32 rentpersonid = pHouseManager->clClientApplyer[pHouseManager->CurrentSelectedApplyerSlot].clPerson.GetPersonID();

		//----------------------------
		// 서버로 임대를 요청한다. 
		//----------------------------
		cltRequestRent clrent(rentpersonid, 
			pHouseManager->siVillageUnique, 
			pHouseManager->CurrentSelectedHouseUnitSlot);

		cltMsg clMsg(GAMEMSG_REQUEST_HOUSERENT, sizeof(cltRequestRent), (BYTE*)&clrent);
		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	else CASE_CONTROL( BUTTON_CLEAR_LIST )
	{
		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		cltMsg clMsg(GAMEMSG_REQUEST_DELHOUSEAPPLYER, sizeof(SI32), (BYTE*)&pHouseManager->CurrentSelectedApplyerSlot); 
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

	}
}