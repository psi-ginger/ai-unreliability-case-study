#include "NLandDlg.h"

#include "../../InterfaceMgr/Interface/List.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

#include "Server.h"
#include "../Server/GameDBProcess/GameDBProcess.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NLandChiefDlg::NLandChiefDlg()
{
}

NLandChiefDlg::~NLandChiefDlg()
{
	
}

void NLandChiefDlg::InitDlg()
{

	cltLandManager *pLandManager = ((cltLandManager *)m_pStrManager);

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

	pLandManager->CurrentSelectedLandUnitSlot	= -1;
	pLandManager->CurrentSelectedApplyerSlot	= -1;

	cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
	if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) )
	{
		//-------------------------------------
		// 서버로 농장리스트를 요청한다. 
		//-------------------------------------
		pLandManager->RequestLandUnitList();
		
		// 게임서버로 단위농장의 상세 정보 리스트를 요청한다. 
		pLandManager->RequestLandUnitListDetail();

		//------------------------------------------
		// 서버로 농장임차 희망자 리스트를 요청한다. 
		//------------------------------------------
		pLandManager->RequestLandRentApplyerList();
	}

}

void NLandChiefDlg::Action()
{

	cltLandManager *pLandManager = ((cltLandManager *)m_pStrManager);

	if( pLandManager->bUpdateLandUnitListShowChiefSwitch )
	{
		pLandManager->bUpdateLandUnitListShowChiefSwitch = false;

		// 농장 유니트 정보를 보여준다. 
		ShowLandUnitListInChief();
	}

	if( pLandManager->UpdateRentContractShowSwitch )
	{
		pLandManager->UpdateRentContractShowSwitch = FALSE;

		// 계약 조건을 보여준다. 
		ShowRentContract();
	}

	if( pLandManager->UpdateLandApplyerListShowSwitch )
	{
		pLandManager->UpdateLandApplyerListShowSwitch = FALSE;

		// 임차 희망자 리스트를 보여준다. 
		ShowLandApplyerListInChief();
	}
}

void NLandChiefDlg::ShowRentContract()
{

}

void NLandChiefDlg::ShowLandUnitListInChief()
{
	cltLandManager *pLandManager = ((cltLandManager *)m_pStrManager);
	// 리스트 
	CList *pListRent = (CList*)m_pControlMap[ LIST_RENTLIST ];
	CList *pListApply = (CList*)m_pControlMap[ LIST_APPLYLIST ];
	stListBoxItemData itemdata;

	SI32 i;
	TCHAR buffer[256];

	// 기존 리스트를 지운다. 
	pListRent->Clear();

	for(i = 0;i < MAX_LANDUNIT_NUMBER;i++)
	{
		if( pLandManager->clClientUnit[i].clPerson.GetPersonID() == 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1824);
			StringCchPrintf(buffer, 256, TEXT("%d %s"), i+1, pText);			
		}
		else
		{
			StringCchPrintf(buffer, 256, TEXT("%d %s"), i+1, pLandManager->clClientUnit[i].clPerson.GetName() );
		}

		itemdata.Init();
		itemdata.Set( 0, buffer );

		pListRent->AddItem( &itemdata );
	}

	pListRent->Refresh();
}

void NLandChiefDlg::ShowLandApplyerListInChief()
{
	cltLandManager *pLandManager = ((cltLandManager *)m_pStrManager);
	// 리스트 
	CList *pListRent = (CList*)m_pControlMap[ LIST_RENTLIST ];
	CList *pListApply = (CList*)m_pControlMap[ LIST_APPLYLIST ];
	stListBoxItemData itemdata;

	SI32 i;
	TCHAR buffer[256];

	// 기존 리스트를 지운다. 
	pListApply->Clear();

	for(i = 0;i < MAX_APPLY_PER_LAND;i++)
	{
		if(pLandManager->clClientApplyer[i].clPerson.GetPersonID() == 0)
		{
			TCHAR* pText = GetTxtFromMgr(1692);
			StringCchPrintf(buffer, 256, TEXT("%d %s"), i+1, pText);			
		}
		else
		{
			StringCchPrintf(buffer, 256, TEXT("%d %s"), i+1, pLandManager->clClientApplyer[i].clPerson.GetName());
		}

		itemdata.Init();
		itemdata.Set( 0, buffer );

		pListApply->AddItem( &itemdata);
	}

	pListApply->Refresh();

}

void NLandChiefDlg::ShowSelectedLandUnitExplain( int index )
{
	
	cltLandManager *pLandManager = ((cltLandManager *)m_pStrManager);
	// 리스트 
	CList *pListRent = (CList*)m_pControlMap[ LIST_RENTLIST ];
	CList *pListApply = (CList*)m_pControlMap[ LIST_APPLYLIST ];

	TCHAR buffer[256];
	TCHAR date[128];
	if(index < 0)return;

	// 계약 만기일을 구한다. 
	cltDate cldate;
	cldate.Set(&pLandManager->clClientUnit[index].clContract.clDate);
	cldate.AddMonth(pLandManager->clClientUnit[index].clContract.siRentPeriod*12);
	
	cldate.GetDateText(date);

	TCHAR* pText = GetTxtFromMgr(1825);

	//사용자가 없으면 사용자 없음 이라는 메세지 표시

	TCHAR * name = (TCHAR*)pLandManager->clClientUnit[index].clPerson.GetName();

	if( pLandManager->clClientUnit[index].clPerson.GetPersonID() <= 0 || name == NULL || name[0] == 0 )
	{
		name = GetTxtFromMgr(7533);
	}

	/*if(pLandManager->clClientUnit[index].clPerson.GetName()==NULL)
			
		TCHAR* pText = GetTxtFromMgr(7533);

		StringCchPrintf(buffer, 256, pText, index+1, 
			pText,
			pLandManager->clClientUnit[index].clContract.siRentFee);
		StringCchCat( buffer, 256, date );
		SetEditText( EDIT_EXPLAIN, buffer);
	
	else*/
	StringCchPrintf(buffer, 256, pText, index+1, 
		name,
		pLandManager->clClientUnit[index].clContract.siRentFee);
	StringCchCat( buffer, 256, date );
	SetEditText( EDIT_EXPLAIN, buffer);

	
	
}


void NLandChiefDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltLandManager *pLandManager = ((cltLandManager *)m_pStrManager);

	// 리스트 
	CList *pListRent = (CList*)m_pControlMap[ LIST_RENTLIST ];
	CList *pListApply = (CList*)m_pControlMap[ LIST_APPLYLIST ];


	TCHAR buffer[ 1024 ];

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
				pLandManager->CurrentSelectedLandUnitSlot = index;

				if( index < 0 ) return;

				ShowSelectedLandUnitExplain( index );
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
				pLandManager->CurrentSelectedApplyerSlot = index;

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
		SI32 fee = _tstoi( GetEditText( EDIT_RENT_FEE ) );

		if(fee < MIN_LAND_RENT_FEE)
		{
			TCHAR* pText = GetTxtFromMgr(1682);

			StringCchPrintf(buffer, 1024, pText, MIN_LAND_RENT_FEE);
			SetMainExplain(buffer);
			return;
		}

		if(fee > MAX_LAND_RENT_FEE) 
		{
			TCHAR* pText = GetTxtFromMgr(1683);

			StringCchPrintf(buffer, 1024, pText, MAX_LAND_RENT_FEE );
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

		cltRentContractPacket sendMsg( pLandManager->siVillageUnique, &clcontract);
		
		cltMsg clMsg(GAMEMSG_REQUEST_LAND_CHANGECONTRACT, sizeof(cltRentContractPacket), (BYTE*)&sendMsg);
		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	}

	else CASE_CONTROL( BUTTON_REFRESH_RENTLIST )
	{
		//-------------------------------------
		// 서버로 농장리스트를 요청한다. 
		//-------------------------------------
		pLandManager->RequestLandUnitList();
	
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

		// 게임서버로 단위농장의 상세 정보 리스트를 요청한다. 
		pLandManager->RequestLandUnitListDetail();
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
		// 서버로 농장임차 희망자 리스트를 요청한다. 
		//------------------------------------------
		pLandManager->RequestLandRentApplyerList();
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
		// 선택된 농장유니트가 빈곳인지 확인한다.
		//------------------------------------------
		if( pLandManager->CurrentSelectedLandUnitSlot < 0)
		{
			TCHAR* pText = GetTxtFromMgr(1820);
			SetMainExplain( pText);
			return;
		}
		
		if( pLandManager->clClientUnit[ pLandManager->CurrentSelectedLandUnitSlot].clPerson.GetPersonID() )
		{
			TCHAR* pText = GetTxtFromMgr(1821);
			SetMainExplain(pText);
			return;
		}

		//--------------------------------------
		// 선택된 희망자가 유효한지 확인한다. 
		//--------------------------------------
		if( pLandManager->CurrentSelectedApplyerSlot < 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1687);
			SetMainExplain(pText);
			return;
		}

		SI32 rentpersonid = pLandManager->clClientApplyer[pLandManager->CurrentSelectedApplyerSlot].clPerson.GetPersonID();

		//----------------------------
		// 서버로 임대를 요청한다. 
		//----------------------------
		cltRequestRent clrent(rentpersonid, 
			pLandManager->siVillageUnique, 
			pLandManager->CurrentSelectedLandUnitSlot);

		cltMsg clMsg(GAMEMSG_REQUEST_LAND_RENT, sizeof(cltRequestRent), (BYTE*)&clrent);
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

		cltMsg clMsg(GAMEMSG_REQUEST_LAND_DELAPPLYER, sizeof(SI32), (BYTE*)&pLandManager->CurrentSelectedApplyerSlot); 
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

	}
}