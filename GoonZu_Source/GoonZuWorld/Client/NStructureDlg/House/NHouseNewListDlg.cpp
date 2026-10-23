#include "NHouseDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"
#include "../Client/NStructureDlg/House/UserInfo/NHouseUserInfoBaseDlg.h"
#include "../Client/NStructureDlg/House/NHouseExtensionDlg.h"
#include "../Client/NStructureDlg/House/NHouseNameChangeDlg.h"
#include "../CommonLogic/MsgType-House.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NHouseNewListDlg::NHouseNewListDlg()
{
}

NHouseNewListDlg::~NHouseNewListDlg()
{

}

void NHouseNewListDlg::InitDlg()
{
	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);

	SetControlMap( LIST_HOUSELIST, TEXT("listbox_houselist") );
	SetControlMap( BUTTON_ENTER, TEXT("button_enter") );
	SetControlMap( BUTTON_CANCELLATION_HOUSE, TEXT("button_recall_house") );
	// PCK - 내창고 목록 새로 추가 (08.07.09)
	SetControlMap( LIST_MYHOUSELIST, TEXT("listbox_myhouselist"));

	SetControlMap( BUTTON_CHANGEHOUSENAME, TEXT("button_ChangName"));
	SetControlMap( BUTTON_EXTENSION, TEXT("button_Period"));
		
	CButton *pButton = (CButton*)m_pControlMap[ BUTTON_CANCELLATION_HOUSE ];
	pButton->Enable(FALSE);
	pButton = (CButton*)m_pControlMap[ BUTTON_ENTER ];
	pButton->Enable(FALSE);
	CButton* pChangeNameButton = (CButton *)m_pControlMap[BUTTON_CHANGEHOUSENAME];
	CButton* pExtensionButton = (CButton *)m_pControlMap[BUTTON_EXTENSION];
	pChangeNameButton->Enable(FALSE);
	pExtensionButton->Enable(FALSE);
	// 리스트 
	CList *pList = (CList*)m_pControlMap[ LIST_HOUSELIST ];
	CList *pMyList = (CList*)m_pControlMap[ LIST_MYHOUSELIST ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	pMyList->SetBorder(true);
	pMyList->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(1695);
	pList->SetColumn( 0, 53, pText );
	pMyList->SetColumn( 0, 53, pText );
	pText = GetTxtFromMgr(1696);
	pList->SetColumn( 1, 115, pText );//사용자리스트 폭조절.[2007.06.29 성웅]
	pText = GetTxtFromMgr(40372);
	pMyList->SetColumn( 1, 100, pText );
	pText = GetTxtFromMgr(1697);
	pList->SetColumn( 2, 148, pText );// 폭조절 by LeeKH 2007.07.30
	pText = GetTxtFromMgr(8763);
	pMyList->SetColumn( 2, 115, pText );
	pText = GetTxtFromMgr(1697);
	pMyList->SetColumn( 3, 68, pText );

	pList->Refresh();
	pMyList->Refresh();

	//-------------------------------------
	// 서버로 시전리스트를 요청한다. 
	//-------------------------------------
	cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITLIST, pHouseManager->siVillageUnique );
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );

	pHouseManager->siSelectedHouseUnitNumber = -1;

}

void NHouseNewListDlg::Action()
{
	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);

	if( pHouseManager->bUpdateHouseUnitListShowSwitch == true )
	{
		pHouseManager->bUpdateHouseUnitListShowSwitch = false;

		// 시전리스트를 보여준다. 
		ShowHouseUnitList();
	}
}


void NHouseNewListDlg::ShowHouseUnitList()
{
	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_HOUSELIST ];
	CList *pMyList = (CList*)m_pControlMap[ LIST_MYHOUSELIST ];
	stListBoxItemData itemdata;
	stListBoxItemData stMyItemData;

	SI32 i;
	TCHAR buffer[256];

	// 리스트뷰를 모두 지운다. 
	pList->Clear();
	pMyList->Clear();

	for( i = 0; i < MAX_HOUSEUNIT_NUMBER; i++ )
	{
		itemdata.Init();
		stMyItemData.Init();

		StringCchPrintf(buffer, 256, TEXT("%d"), i+1);

		itemdata.siParam[0] = i;
		itemdata.Set( 0, buffer );

		stMyItemData.siParam[0] = i;
		stMyItemData.Set( 0, buffer );

		// [지연-090525] 일단 사용자가 있는지 체크.
		if( pHouseManager->clClientUnit[i].clPerson.GetPersonID() )
		{
			// 서버에(GAMEMSG_REQUEST_HOUSEUNITLIST)시전리스트를 요청하면 판매중인 물건인지에 대한 정보까지 houseUnit에 포함되어 날라온다 
			itemdata.Set( 1, (TCHAR*)pHouseManager->clClientUnit[i].clPerson.GetName() );

			if( pHouseManager->clClientUnit[i].bSell == true )
			{
				TCHAR* pText = NULL;
				pText = GetTxtFromMgr(30104);
				itemdata.Set( 2, pText );
				stMyItemData.Set( 3, pText );
			}
			else // 사용자가 있다는건 임대중이라는 것이고, 판매중인진 위에서 체크했으므로 여기까지 왔다는 것은 무조건 임대중이라는 뜻.
			{
				TCHAR* pText = NULL;
				pText = GetTxtFromMgr(7532);
				itemdata.Set( 2, pText );
				stMyItemData.Set( 3, pText );
			}

			//사용자가 나라면, 내것을 보여주는 리스트에도 추가하고, 사용자가 내가 아니라면 전체 리스트에만 추가.
			if( pHouseManager->clClientUnit[i].clPerson.GetPersonID() == pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
			{
				stMyItemData.Set( 1 , (TCHAR*)pHouseManager->clClientUnit[i].m_szName);

				cltDate& rclDate = pHouseManager->clClientUnit[i].clContract.clDate;
				
				TCHAR szDate[MAX_PATH] = {0,};
				
				SI32 siYear = pHouseManager->clClientUnit[i].clContract.siRentPeriod + rclDate.uiYear;

				StringCchPrintf(szDate, MAX_PATH , GetTxtFromMgr(3808), 
					siYear , 
					rclDate.uiMonth , 
					rclDate.uiDay ,
					rclDate.uiHour);
				
				stMyItemData.Set( 2 , szDate);

				pMyList->AddItem( &stMyItemData );
			}

			pList->AddItem( &itemdata );
		}
		else // 사용자가 없을때
		{
			TCHAR* pText = NULL;
			pText = GetTxtFromMgr(1691);	// 사용자가 없는 빈 창고라는 뜻. 1번 컬럼에만 "빈창고" 라는 문자열을 넣어준다.
			itemdata.Set( 1, pText );

			pList->AddItem( &itemdata );
		}
	}

	
    // 기존----------------------------------------------------------------------------------------
	//for( i = 0; i < MAX_HOUSEUNIT_NUMBER; i++ )
	//{ 
	//	itemdata.Init();

	//	StringCchPrintf(buffer, 256, TEXT("%d"), i+1);

	//	itemdata.siParam[0] = i;
	//	itemdata.Set( 0, buffer );

	//	if( pHouseManager->clClientUnit[i].clPerson.GetPersonID() )
	//	{
	//		// PersonID가 현재 자기 캐릭터와 같으면 내창고 목록에도 띄워준다. - PCK(08.07.09)
	//		if(pHouseManager->clClientUnit[i].clPerson.GetPersonID() == pclClient->pclCM->CR[1]->pclCI->GetPersonID())
	//		{
	//			itemdata.Set( 1, (TCHAR*)pHouseManager->clClientUnit[i].clPerson.GetName() );

	//			if ( ConstServiceArea_China != pclClient->siServiceArea )
	//			{		
	//				TCHAR* pText;
	//				pText = GetTxtFromMgr(7532);
	//				itemdata.Set( 2, pText );
	//			}

	//			pMyList->AddItem( &itemdata);
	//		}

	//		itemdata.Set( 1, (TCHAR*)pHouseManager->clClientUnit[i].clPerson.GetName() );
	//		if ( ConstServiceArea_China != pclClient->siServiceArea )
	//		{		
	//			TCHAR* pText;
	//			pText = GetTxtFromMgr(7532);
	//			itemdata.Set( 2, pText );
	//		}
	//	}
	//	else
	//	{
	//		TCHAR* pText = GetTxtFromMgr(1691);
	//		itemdata.Set( 1, pText );
	//	}

	//	pList->AddItem( &itemdata );
	//}
	// 기존----------------------------------------------------------------------------------------

	pList->Refresh();
	pMyList->Refresh();
}

void NHouseNewListDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_HOUSELIST ];
	CList *pMyList = (CList*)m_pControlMap[ LIST_MYHOUSELIST ];
	cltClient* pclclient = (cltClient*)pclClient;

	CASE_CONTROL( LIST_HOUSELIST )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			{			
				int index = pList->GetSelectedIndex();

				if( index < 0 ) return;

				stListBoxItemData *pItemData = pList->GetItemData( index, 0 );

				pHouseManager->siSelectedHouseUnitNumber = pItemData->siParam[0];

				TCHAR* pText = pItemData->strText[1]; 
				CButton* pEnerButton = (CButton *)m_pControlMap[BUTTON_ENTER];
				CButton* pCancellationButton = (CButton *)m_pControlMap[BUTTON_CANCELLATION_HOUSE];
				CButton* pChangeNameButton = (CButton *)m_pControlMap[BUTTON_CHANGEHOUSENAME];
				CButton* pExtensionButton = (CButton *)m_pControlMap[BUTTON_EXTENSION];
				
				if( !_tcscmp(pclClient->pclCM->CR[ 1 ]->GetName() , pText )) 
				{
					pEnerButton->Enable(TRUE);
					pCancellationButton->Enable(TRUE);
					pChangeNameButton->Enable(TRUE);
					pExtensionButton->Enable(TRUE);
				}
				else
				{
					pEnerButton->Enable(FALSE);
					pCancellationButton->Enable(FALSE);
					pChangeNameButton->Enable(FALSE);
					pExtensionButton->Enable(FALSE);
				}

				pMyList->ClearSelectIndex();
			}
			break;
		}
	}
	CASE_CONTROL( LIST_MYHOUSELIST )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			{
				int index = pMyList->GetSelectedIndex();

				if( index < 0 ) return;

				stListBoxItemData *pItemData = pMyList->GetItemData( index, 0 );

				pHouseManager->siSelectedHouseUnitNumber = pItemData->siParam[0];

				TCHAR* pText = pItemData->strText[1]; 
				CButton* pEnerButton = (CButton *)m_pControlMap[BUTTON_ENTER];
				CButton* pCancellationButton = (CButton *)m_pControlMap[BUTTON_CANCELLATION_HOUSE];
				CButton* pChangeNameButton = (CButton *)m_pControlMap[BUTTON_CHANGEHOUSENAME];
				CButton* pExtensionButton = (CButton *)m_pControlMap[BUTTON_EXTENSION];
			
				if( pHouseManager->siSelectedHouseUnitNumber >= 0 && pHouseManager->siSelectedHouseUnitNumber < MAX_HOUSEUNIT_NUMBER) 
				{
					pEnerButton->Enable(TRUE);
					pCancellationButton->Enable(TRUE);
					pChangeNameButton->Enable(TRUE);
					pExtensionButton->Enable(TRUE);
				}
				else
				{
					pEnerButton->Enable(FALSE);
					pCancellationButton->Enable(FALSE);
					pChangeNameButton->Enable(FALSE);
					pExtensionButton->Enable(FALSE);
				}

				pList->ClearSelectIndex();
			}
			break;
		}
	}
	else CASE_CONTROL( BUTTON_CHANGEHOUSENAME)
	{	
		cltHouseUnit& rclHouseUnit = pHouseManager->clClientUnit[pHouseManager->siSelectedHouseUnitNumber];

		if(pclclient->m_pDialog[NHOUSENAMECHANGE_DLG ] != NULL)
			((CNHouseNameChangeDlg*)pclclient->m_pDialog[NHOUSENAMECHANGE_DLG])->DeleteDialog();

		pclclient->CreateInterface(NHOUSENAMECHANGE_DLG);
		if(pclclient->m_pDialog[NHOUSENAMECHANGE_DLG ] != NULL)
		{
			((CNHouseNameChangeDlg*)pclclient->m_pDialog[NHOUSENAMECHANGE_DLG])->SetVillageUnique(pHouseManager->siVillageUnique);
			((CNHouseNameChangeDlg*)pclclient->m_pDialog[NHOUSENAMECHANGE_DLG])->SetHouseUnitIndex(pHouseManager->siSelectedHouseUnitNumber);
			((CNHouseNameChangeDlg*)pclclient->m_pDialog[NHOUSENAMECHANGE_DLG])->SetOldHouseName(rclHouseUnit.m_szName);
		}
	}
	else CASE_CONTROL( BUTTON_EXTENSION)
	{
		cltRentContract& pclContract = pclclient->pclHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract;

		NTCHARString1024 pBody(GetTxtFromMgr(40370));	

		TCHAR szMoney[MAX_PATH] = {0,};
		TCHAR szCYear[MAX_PATH] = {0,};
		TCHAR szYear[MAX_PATH] = {0,};
		TCHAR szMonth[MAX_PATH] = {0,};
		TCHAR szDay[MAX_PATH] = {0,};
		TCHAR szHour[MAX_PATH] = {0,};

		StringCchPrintf(szMoney, MAX_PATH, TEXT("%d"), pclContract.siRentFee);
		StringCchPrintf(szCYear, MAX_PATH, TEXT("%d"), pclContract.siRentPeriod);
		StringCchPrintf(szYear, MAX_PATH, TEXT("%d"), pclclient->clClientDate.uiYear + pclContract.siRentPeriod);
		StringCchPrintf(szMonth, MAX_PATH, TEXT("%d"), pclclient->clClientDate.uiMonth);
		StringCchPrintf(szDay, MAX_PATH, TEXT("%d"), pclclient->clClientDate.uiDay);
		StringCchPrintf(szHour, MAX_PATH, TEXT("%d"), pclclient->clClientDate.uiHour);

		pBody.Replace("#money#", szMoney);
		pBody.Replace("#cyear#", szCYear);
		pBody.Replace("#year#", szYear);
		pBody.Replace("#month#", szMonth);
		pBody.Replace("#day#", szDay);
		pBody.Replace("#hour#", szHour);

		if(pclclient->m_pDialog[NHOUSEEXTENSION_DLG ] != NULL)
			((CNHouseExtensionDlg*)pclclient->m_pDialog[NHOUSEEXTENSION_DLG])->DeleteDialog();

		pclclient->CreateInterface(NHOUSEEXTENSION_DLG);
		if(pclclient->m_pDialog[NHOUSEEXTENSION_DLG ] != NULL)
		{
			((CNHouseExtensionDlg*)pclclient->m_pDialog[NHOUSEEXTENSION_DLG])->SetExtensionInfoText(pBody);
			((CNHouseExtensionDlg*)pclclient->m_pDialog[NHOUSEEXTENSION_DLG])->SetVillageUnique(pHouseManager->siVillageUnique);
			((CNHouseExtensionDlg*)pclclient->m_pDialog[NHOUSEEXTENSION_DLG])->SetHouseUnitIndex(pHouseManager->siSelectedHouseUnitNumber);
		}
	}
	else CASE_CONTROL( BUTTON_CANCELLATION_HOUSE )
	{		
		if(pHouseManager->siSelectedHouseUnitNumber >=0)
		{
			cltGameMsgRequest_HouseCancellationPenaltyMoney clinfo(pHouseManager->siVillageUnique, pHouseManager->siSelectedHouseUnitNumber);
			cltMsg clMsg(GAMEMSG_REQUEST_HOUSECANCELLATIONPENALTYMONEY, sizeof(clinfo), (BYTE*) &clinfo );
			pclClient->pclCM->CR[ 1 ]->SendNetMsg((sPacketHeader*)&clMsg);

			CButton *pButton = (CButton*)m_pControlMap[ BUTTON_CANCELLATION_HOUSE ];
			pButton->Enable(FALSE);
		}

	}
	else CASE_CONTROL( BUTTON_ENTER )
	{
		if(pHouseManager->siSelectedHouseUnitNumber >= 0 && pHouseManager->siSelectedHouseUnitNumber < MAX_HOUSEUNIT_NUMBER )
		{
			//대상이 되는 캐릭터의 아이디를 구한다.
			SI32 id = 1;

			if(id)
			{
				//하우스 매니저 안의 하우스 유닛이 이젠 판매 정보도 담고 있기 때문에 해당 변수로 판매 중인지 체크 가능.
				if(pHouseManager->clClientUnit[pHouseManager->siSelectedHouseUnitNumber].bSell == true)
				{
					// 접근이 불가능 하다는 메세지.

					TCHAR* pTitle = GetTxtFromMgr(30145);
					TCHAR* pText  = GetTxtFromMgr(30103);

					cltClient *pclclient = (cltClient*) pclClient;

					stMsgBox MsgBox;
					MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_OK, 0 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}

				else // 판매중인 창고가 아니면 이전과 같음.
				{
					pHouseManager->clClientUnit[pHouseManager->siSelectedHouseUnitNumber].clStorageInfo.Init();

					cltClient *pclclient = (cltClient *) pclClient;

					if(pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG] == NULL )
						pclclient->CreateInterface( NHOUSEUSERINFOBASE_DLG );

					if(pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG] )
					{
						((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->SetPersonItemInfo(&pclCM->CR[1]->pclCI->clCharItem);
						((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->SetHouseUserInfoBase(&pHouseManager->clClientUnit[pHouseManager->siSelectedHouseUnitNumber], &pclCM->CR[id]->pclCI->clCharSkill,
							pHouseManager->siVillageUnique, pHouseManager->siSelectedHouseUnitNumber);

							((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->InitHouse();
					}

					else // 시전열고 있으면 닫혔다가 다시 열어준다.
					{
						pclclient->CreateInterface( NHOUSEUSERINFOBASE_DLG );

						((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->SetPersonItemInfo(&pclCM->CR[1]->pclCI->clCharItem);
						((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->SetHouseUserInfoBase(&pHouseManager->clClientUnit[pHouseManager->siSelectedHouseUnitNumber], &pclCM->CR[id]->pclCI->clCharSkill,
							pHouseManager->siVillageUnique, pHouseManager->siSelectedHouseUnitNumber);
						((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->InitHouse();
					}
					
					GameMsgRequest_HouseUnitStgInfo clinfo(pHouseManager->siVillageUnique, pHouseManager->siSelectedHouseUnitNumber);
					cltMsg clMsg(GAMEMSG_REQUEST_STGINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}
	}
	

	//기존---------------------------------------------------------------------------------------------------------
	//	if(pHouseManager->siSelectedHouseUnitNumber >=0)
	//	{
	//		// 대상이 되는 캐릭터의 아이디를 구한다. 
	//		SI32 id = 1;

	//		if(id)
	//		{
	//			pHouseManager->clClientUnit[pHouseManager->siSelectedHouseUnitNumber].clStorageInfo.Init();

	//			cltClient *pclclient = (cltClient *)pclClient;

	//			if( pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG] == NULL )
	//				pclclient->CreateInterface( NHOUSEUSERINFOBASE_DLG ); 

	//			if( pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG] )
	//			{
	//				((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->SetPersonItemInfo(&pclCM->CR[1]->pclCI->clCharItem);
	//				((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->SetHouseUserInfoBase(&pHouseManager->clClientUnit[pHouseManager->siSelectedHouseUnitNumber], &pclCM->CR[id]->pclCI->clCharSkill,
	//					pHouseManager->siVillageUnique, pHouseManager->siSelectedHouseUnitNumber);

	//				((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->InitHouse();
	//			}
	//			else	// 시전열고 있으면 닫혔다가 다시 열어준다
	//			{
	//				pclclient->CreateInterface( NHOUSEUSERINFOBASE_DLG );

	//				((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->SetPersonItemInfo(&pclCM->CR[1]->pclCI->clCharItem);
	//				((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->SetHouseUserInfoBase(&pHouseManager->clClientUnit[pHouseManager->siSelectedHouseUnitNumber], &pclCM->CR[id]->pclCI->clCharSkill,
	//					pHouseManager->siVillageUnique, pHouseManager->siSelectedHouseUnitNumber);
	//				((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->InitHouse();
	//			}

	//			GameMsgRequest_HouseUnitStgInfo clinfo(pHouseManager->siVillageUnique, pHouseManager->siSelectedHouseUnitNumber);
	//			cltMsg clMsg(GAMEMSG_REQUEST_STGINFO, sizeof(clinfo), (BYTE*)&clinfo);
	//			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	//		}

	//	}
    //기존---------------------------------------------------------------------------------------------------------

}