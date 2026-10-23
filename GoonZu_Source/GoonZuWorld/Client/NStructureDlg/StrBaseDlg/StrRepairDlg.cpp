#include "StrRepairDlg.h"
#include "StrInventory.h"

#include "../../Client/InterfaceMgr/InterfaceMgr.h"
#include "Char\CharManager\CharManager.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../InterfaceMgr/Interface/ListView.h"

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NStrRepairDlg::NStrRepairDlg()
{
	m_siNeedRepairMoney = 0;
}

NStrRepairDlg::~NStrRepairDlg()
{
}

void NStrRepairDlg::InitDlg()
{
	SetControlMap( EDIT_STRDURA, TEXT("editbox_strdura") );
	SetControlMap( EDIT_BASEMONEY, TEXT("editbox_basemoney") );
	SetControlMap( EDIT_NEEDREPAIR, TEXT("editbox_needrepair") );
	SetControlMap( EDIT_REPAIR, TEXT("editbox_repair") );

	SetControlMap( BUTTON_REPAIR, TEXT("button_repair") );

	// [추가 : 황진성 2007. 12. 24 건물수리창 인터 페이스 수정.]
	SetControlMap( STATIC_GOLD_REPAIR,	TEXT("statictext_gold_repair") );
	SetControlMap( STATIC_SET_REPAIR,	TEXT("statictext_set_repair") );
	SetControlMap( EDITBOX_GOLD_REPAIR, TEXT("editbox_gold_repair") );
	SetControlMap( EDITBOX_SET_REPAIR,	TEXT("editbox_set_repair") );
	SetControlMap( BUTTON_SET_REPAIR,	TEXT("button_set_repair") );

	SetControlMap( STATIC_NONAME1,	TEXT("statictext_NONAME1") );
	SetControlMap( STATIC_NONAME2,	TEXT("statictext_NONAME2") );
	SetControlMap( STATIC_NONAME3,	TEXT("statictext_NONAME3") );
	SetControlMap( STATIC_NONAME10,	TEXT("statictext_NONAME10") );
	SetControlMap( STATIC_NONAME11,	TEXT("statictext_NONAME11") );
	SetControlMap( STATIC_NONAME12,	TEXT("statictext_NONAME12") );

	SetControlMap( OUTLINE_NONAME2,	TEXT("outline_NONAME2") );
	SetControlMap( OUTLINE_NONAME3,	TEXT("outline_NONAME3") );

	CEdit*	pEdit_Repair = (CEdit*)m_pControlMap[ EDIT_REPAIR ];
}

void NStrRepairDlg::Action()
{

	TCHAR buf[ 256 ] = {'\0'};

	g_JYLibFunction.SetNumUnit(m_pStrManager->clClientStrInfo.clCommonInfo.siStrDura, buf, 256 );
	SetEditText( EDIT_STRDURA, buf );

	TCHAR* pText = GetTxtFromMgr(611);
	g_JYLibFunction.SetNumUnit( m_pStrManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney, buf, 256, pText );
	SetEditText( EDIT_BASEMONEY, buf );

	_itot( m_pStrManager->clClientStrInfo.clCommonInfo.siRepairStrDura, buf, 10 );
	SetEditText( EDIT_NEEDREPAIR, buf );
}

void NStrRepairDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CASE_CONTROL(EDIT_REPAIR ) 
	{
		switch( nEvent )
		{
		case EVENT_EDITBOX_CHANGE:
			{
				TCHAR buf[ 256 ] = {'\0'};
				TCHAR buf2[ 256 ] = {'\0'};
				// 수리시 필요한 돈
				cltDate *pclCurrentDate = &((cltClient *)pclClient)->clClientDate;
				SI64 tempNeedRepairMoney = _atoi64( GetEditText( EDIT_REPAIR ) );

				if(tempNeedRepairMoney > 1000)
				{
					tempNeedRepairMoney = 1000;
					SetEditText( EDIT_REPAIR, "1000" );
				}
				else if(4 < strlen(GetEditText( EDIT_REPAIR )))
				{
					tempNeedRepairMoney = 0;
					SetEditText( EDIT_REPAIR, "0" );
				}

				//KHY - 1102 - 마을 수리비 재조정. - 서버에서도 다시 계산한다.
				if (pclClient->IsCountrySwitch(Switch_RepairMoney) )
				{
					SI32 tempRepairAmount = atoi( GetEditText( EDIT_REPAIR ) );
					
					if( pclCurrentDate->uiYear >= NEW_REPAIR_YEAR )
						tempNeedRepairMoney *= 50 * NEW_REPAIR_YEAR;
//						tempNeedRepairMoney *= (140 * (NEW_REPAIR_YEAR/4));
					else if((pclCurrentDate->uiYear >= 0) &&(pclCurrentDate->uiYear < NEW_REPAIR_YEAR)) 
					{
						tempNeedRepairMoney *= 50 * (pclCurrentDate->uiYear);
//						tempNeedRepairMoney *= (140 * (pclCurrentDate->uiYear/4));
					}

					m_siNeedRepairItem = tempRepairAmount;
				}
				else
				{
					if( pclCurrentDate->uiYear >= REPAIR_YEAR )
						tempNeedRepairMoney *= 1000;
					else if( pclCurrentDate->uiYear >= REPAIR_YEAR - 1 )
						tempNeedRepairMoney *= 1000 * 8 / 10;
					else if( pclCurrentDate->uiYear >= REPAIR_YEAR - 2 )
						tempNeedRepairMoney *= 1000 * 4 / 10;
					else if( pclCurrentDate->uiYear >= REPAIR_YEAR - 3 )
						tempNeedRepairMoney *= 1000 * 2 / 10;
					else
						tempNeedRepairMoney *= 100;
				}
				
				m_siNeedRepairMoney = tempNeedRepairMoney;

				TCHAR* pText = GetTxtFromMgr(611);
				itoa(tempNeedRepairMoney, buf, 10);
				g_JYLibFunction.SetNumUnit( m_siNeedRepairMoney, buf, 256, pText );
				SetEditText( EDITBOX_GOLD_REPAIR,  buf);
				
				TCHAR* pText2  = GetTxtFromMgr(2219);
				SI32   ItemNum = atoi(GetEditText( EDIT_REPAIR ));
				g_JYLibFunction.SetNumUnit( ItemNum, buf2, 256,  pText2 );
				SetEditText( EDITBOX_SET_REPAIR,  buf2);
			}
			break;
		}
	}
	// [추가 : 황진성 2007. 12. 24 => 건물 수리를 수리 세트로 수리수리.]
	CASE_CONTROL( BUTTON_SET_REPAIR ) 
	{
		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		// 주총이면 아무행수나 수리가능하게
		if( pclClient->pclCityHallManager->clGeneralMeetingInfo.siMatterType > 0 ) 
		{ 
			TCHAR* pText = NULL;
			if( pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage != pclClient->pclCM->CR[1]->GetCurrentVillageUnique() )
			{
				pText = GetTxtFromMgr(2452);

				SetMainExplain(pText);
				return;
			}
			if( pclCM->CR[1]->pclCI->clCharRank.siType == 0 || pclCM->CR[1]->pclCI->clCharRank.siType > RANKTYPE_NEWMARKET )
			{
				pText = GetTxtFromMgr(1440);

				SetMainExplain(pText);
				return;
			}
		}
		else
		{
			SI32 id = 1;
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[id]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText = NULL;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		//KHY - 1102 - 마을 수리비 재조정.  - 수리세트 삭제.
		if (pclClient->IsCountrySwitch(Switch_RepairMoney) )
		{
			TCHAR* pText = NULL;
			//KHY - 1102 -필요한 수리량이 1000 이면 수리 해선 안된다.
			if(m_pStrManager->clClientStrInfo.clCommonInfo.siRepairStrDura > 1000)
			{
				pText = GetTxtFromMgr(7219);
				SetMainExplain(pText);
				return;
			}
			//// 내구도가 10000 이상이면 수리할 필요가 없다.
			//if(m_pStrManager->clClientStrInfo.clCommonInfo.siStrDura >= 10000)
			//{
			//	TCHAR* pText = GetTxtFromMgr(7219);
			//	SetMainExplain(pText);
			//	return;
			//}
			SI32 RepairAmount = atoi( GetEditText( EDIT_NEEDREPAIR ) );
			// 내구도 양을 입력해라는 표시.
			if( 1000 == RepairAmount)
			{
				TCHAR* pText = GetTxtFromMgr(5988);
				SetMainExplain(pText);
				return;
			}
		
			SI32 tempRepairAmount = atoi( GetEditText( EDIT_REPAIR ) );

			// 내구도 양을 입력해라는 표시.
			if( tempRepairAmount == 0)
			{
				TCHAR* pText = GetTxtFromMgr(7807);
				SetMainExplain(pText);
				return;
			}
			
			SI32 itemunique		  = ITEMUNIQUE(8074);  // 건물 수리 세트  - 서버에서 한번 더 체크한다. -바꿔줄때 함께 바꿔줘야한다.
			if( pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) < tempRepairAmount) //수리하고자 하는 포인트보다 수리 세트가 적다면. - 실패.
			{

				pText   = GetTxtFromMgr(7216);
				SetMainExplain(pText);

				return;
			}

			if( tempRepairAmount <= 0 || tempRepairAmount > 1000 )
			{
				TCHAR* pText = GetTxtFromMgr(5987);
				SetMainExplain(pText);
				return;
			}

			cltGameMsgRequest_RepairStructure sendMsg( m_pStrManager->siRankType, tempRepairAmount, true );

			cltMsg clMsg( GAMEMSG_REQUEST_REPAIRSTRUCTURE, sizeof( sendMsg ), (BYTE *)&sendMsg );
			pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

			SetEditText( EDIT_REPAIR, "" );

			SetEditText( EDITBOX_GOLD_REPAIR,  "");
			SetEditText( EDITBOX_SET_REPAIR,   "");

			if( !(RANKTYPE_NEWMARKET == pclCM->CR[1]->pclCI->clCharRank.siType) )
			{
				// 성공을 표시.
				pText = GetTxtFromMgr(7808);
				SetMainExplain(pText);
			}
		}
	}

	else CASE_CONTROL( BUTTON_REPAIR ) 
	{
		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		// 주총이면 아무행수나 수리가능하게
		if( pclClient->pclCityHallManager->clGeneralMeetingInfo.siMatterType > 0 ) 
		{ 
			TCHAR* pText = NULL;
			if( pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage != pclClient->pclCM->CR[1]->GetCurrentVillageUnique() )
			{
				pText = GetTxtFromMgr(2452);

				SetMainExplain(pText);
				return;
			}
			if( pclCM->CR[1]->pclCI->clCharRank.siType == 0 || pclCM->CR[1]->pclCI->clCharRank.siType > RANKTYPE_NEWMARKET )
			{
				pText = GetTxtFromMgr(1440);

				SetMainExplain(pText);
				return;
			}
		}
		else
		{
			SI32 id = 1;
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[id]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText = NULL;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}


		SI32 tempRepairAmount = atoi( GetEditText( EDIT_REPAIR ) );

		// 내구도 양을 입력해라는 표시.
		if( tempRepairAmount == 0)
		{
			TCHAR* pText = GetTxtFromMgr(7807);
			SetMainExplain(pText);
			return;
		}

		if( tempRepairAmount <= 0 || tempRepairAmount > 1000 )
		{
			TCHAR* pText = GetTxtFromMgr(5987);
			SetMainExplain(pText);
			return;
		}

		//KHY - 1102 -필요한 수리량이 0 이면 수리 해선 안된다.
		if(m_pStrManager->clClientStrInfo.clCommonInfo.siRepairStrDura > 1000)
		{
			TCHAR* pText = GetTxtFromMgr(7219);
			SetMainExplain(pText);
			return;
		}
		//// 내구도가 10000 이상이면 수리할 필요가 없다.
		//if(m_pStrManager->clClientStrInfo.clCommonInfo.siStrDura >= 10000)
		//{
		//	TCHAR* pText = GetTxtFromMgr(7219);
		//	SetMainExplain(pText);
		//	return;
		//}
		

		SI32 tempLimitRepairAmount = m_pStrManager->clClientStrInfo.clCommonInfo.siRepairStrDura + tempRepairAmount;
		if( tempLimitRepairAmount > 1000 )
		{
			TCHAR* pText = GetTxtFromMgr(5988);
			SetMainExplain(pText);
			return;
		}

		if( m_siNeedRepairMoney > m_pStrManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney )
		{
			TCHAR* pText = GetTxtFromMgr(5989);
			SetMainExplain(pText);
			return;
		}
				
		cltGameMsgRequest_RepairStructure sendMsg( m_pStrManager->siRankType, tempRepairAmount );

		cltMsg clMsg( GAMEMSG_REQUEST_REPAIRSTRUCTURE, sizeof( sendMsg ), (BYTE *)&sendMsg );
		pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

		SetEditText( EDIT_REPAIR, "" );

		SetEditText( EDITBOX_GOLD_REPAIR,  "");
		SetEditText( EDITBOX_SET_REPAIR,   "");

		if( !(RANKTYPE_NEWMARKET == pclCM->CR[1]->pclCI->clCharRank.siType) )
		{
			// 성공을 표시.
			TCHAR* pText = GetTxtFromMgr(7808);
			SetMainExplain(pText);
		}
	}

}

