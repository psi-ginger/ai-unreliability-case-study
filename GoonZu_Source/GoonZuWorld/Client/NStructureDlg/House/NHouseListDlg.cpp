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
#include "../CommonLogic/MsgType-House.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NHouseListDlg::NHouseListDlg()
{
}

NHouseListDlg::~NHouseListDlg()
{
	
}

void NHouseListDlg::InitDlg()
{
 	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);

	SetControlMap( LIST_HOUSELIST, TEXT("listbox_houselist") );
	SetControlMap( BUTTON_ENTER, TEXT("button_enter") );
	SetControlMap( BUTTON_CANCELLATION_HOUSE, TEXT("button_recall_house") );
	
	CButton *pButton = (CButton*)m_pControlMap[ BUTTON_CANCELLATION_HOUSE ];
	pButton->Enable(FALSE);
	pButton = (CButton*)m_pControlMap[ BUTTON_ENTER ];
	pButton->Enable(FALSE);
	// 리스트 
	CList *pList = (CList*)m_pControlMap[ LIST_HOUSELIST ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(1695);
	pList->SetColumn( 0, 53, pText );
	pText = GetTxtFromMgr(1696);
	pList->SetColumn( 1, 115, pText );//사용자리스트 폭조절.[2007.06.29 성웅]
	pText = GetTxtFromMgr(1697);
	pList->SetColumn( 2, 148, pText );// 폭조절 by LeeKH 2007.07.30

	pList->Refresh();

	//-------------------------------------
	// 서버로 시전리스트를 요청한다. 
	//-------------------------------------
	cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITLIST, pHouseManager->siVillageUnique );
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );

	pHouseManager->siSelectedHouseUnitNumber = -1;

}

void NHouseListDlg::Action()
{
	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);

	if( pHouseManager->bUpdateHouseUnitListShowSwitch == true )
	{
		pHouseManager->bUpdateHouseUnitListShowSwitch = false;
		
		// 시전리스트를 보여준다. 
		ShowHouseUnitList();
	}
}


void NHouseListDlg::ShowHouseUnitList()
{
	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_HOUSELIST ];
	stListBoxItemData itemdata;

	SI32 i;
	TCHAR buffer[256];

	// 리스트뷰를 모두 지운다. 
	pList->Clear();

	for( i = 0; i < MAX_HOUSEUNIT_NUMBER; i++ )
	{
		itemdata.Init();

		StringCchPrintf(buffer, 256, TEXT("%d"), i+1);

		itemdata.siParam[0] = i;
		itemdata.Set( 0, buffer );

		if( pHouseManager->clClientUnit[i].clPerson.GetPersonID() )
		{
			itemdata.Set( 1, (TCHAR*)pHouseManager->clClientUnit[i].clPerson.GetName() );
			if ( ConstServiceArea_China != pclClient->siServiceArea )
			{		
				TCHAR* pText;
				pText = GetTxtFromMgr(7532);
				itemdata.Set( 2, pText );
			}
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(1691);
			itemdata.Set( 1, pText );
		}

		pList->AddItem( &itemdata );
	}

	pList->Refresh();
}

void NHouseListDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltHouseManager *pHouseManager = ((cltHouseManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_HOUSELIST ];

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

				if( !_tcscmp(pclClient->pclCM->CR[ 1 ]->GetName() , pText )) 
				{
					pEnerButton->Enable(TRUE);
					pCancellationButton->Enable(TRUE);
				}
				else
				{
					pEnerButton->Enable(FALSE);
					pCancellationButton->Enable(FALSE);
				}
			}
			break;
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
		if(pHouseManager->siSelectedHouseUnitNumber >=0)
		{
			// 대상이 되는 캐릭터의 아이디를 구한다. 
			SI32 id = 1;

			if(id)
			{
				pHouseManager->clClientUnit[pHouseManager->siSelectedHouseUnitNumber].clStorageInfo.Init();

				cltClient *pclclient = (cltClient *)pclClient;

				if( pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG] == NULL )
					pclclient->CreateInterface( NHOUSEUSERINFOBASE_DLG ); 

				if( pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG] )
				{
					((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->SetPersonItemInfo(&pclCM->CR[1]->pclCI->clCharItem);
					((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->SetHouseUserInfoBase(&pHouseManager->clClientUnit[pHouseManager->siSelectedHouseUnitNumber], &pclCM->CR[id]->pclCI->clCharSkill,
						pHouseManager->siVillageUnique, pHouseManager->siSelectedHouseUnitNumber);

					((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->InitHouse();
				}
				else	// 시전열고 있으면 닫혔다가 다시 열어준다
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