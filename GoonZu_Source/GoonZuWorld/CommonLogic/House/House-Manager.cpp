//---------------------------------
// 2003/6/26 김태곤
//---------------------------------
//-------------------------------
// CommonLogic
//--------------------------------
#include <CommonLogic.h>
#include "MsgType-Structure.h"
#include "CityHall/TaxpaymentListDlg.h"
#include "House.h"
#include "MsgRval-Define.h"

#include "..\..\GoonZuWorld\Resource.h"

//--------------------------------
// Common
//--------------------------------
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

//---------------------------------
// Server
//---------------------------------
#include "Server.h"
#include "Statistics\Statistics.h"


//---------------------------------
// DBManager
//---------------------------------
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-House.h"


//--------------------------------
// Lib
//--------------------------------
#include "../../Lib/HashTable/HashTable_StringInt.h"



extern cltCommonLogic* pclClient;

cltHouseManager::cltHouseManager(cltCharManager* pclcm, SI32 gamemode, SI32 ranktype, TCHAR *strFileName ) : cltStructureMngBasic(MAX_HOUSE_TAB_NUMBER, pclcm, ranktype, strFileName )
{
	UpdateRentContractShowSwitch		= FALSE;
	bUpdateHouseUnitListShowChiefSwitch	= false;
	bUpdateHouseUnitListShowSwitch		= false;
	UpdateHouseVarySwitch				= FALSE;
	UpdateHouseApplyerListShowSwitch	= FALSE;

	CurrentSelectedHouseUnitSlot	= -1;
	CurrentSelectedApplyerSlot		= -1;
	siSelectedHouseUnitNumber		= -1;

	hImageList						= NULL;

}

cltHouseManager::~cltHouseManager()
{
}

// 시전을 신설한다.
SI32 cltHouseManager::CreateHouse(SI32 siunique)
{
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltHouse(siunique);
		return siunique;
	}
	
	return 0;
}


// 최초 환영 인사말표시 . 
void cltHouseManager::GreetingMsg(HWND hDlg)
{
	TCHAR* pText = GetTxtFromMgr(1698);
	SetMainExplain(hDlg, pText);
}

void cltHouseManager::Do_WM_INITDIALOG(SI32 charunique)
{
	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);
}

// 특정 시전의 계약조건을 변경한다. 
BOOL cltHouseManager::SetRentContract(SI32 villageunique, const cltRentContract* pclcontract)
{
	if(pclStruct[villageunique])
	{
		return pclStruct[villageunique]->SetRentContract(pclcontract);
	}
	return FALSE;
}

// 특정 마을시전의 특정 시전 슬롯을 얻어온다.
cltHouseUnit* cltHouseManager::GetHouseUnit(SI32 villageunique, SI32 houseunitslot )
{
	if(pclStruct[villageunique])
	{
		return pclStruct[villageunique]->GetHouseUnit(houseunitslot);
	}
	return NULL;
}

// 시전을 회수한다. 
void cltHouseManager::RecallHouse(SI32 villageunique, SI32 houseindex)
{
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)return ;

	cltHouse* pclhouse = (cltHouse*)pclStruct[villageunique];
	if(pclhouse == NULL)return ;

	// 단위시전을 초기화한다. 
	pclhouse->clUnit[houseindex].Init();

}

BOOL cltHouseManager::IsSameContract(SI32 sivillageunique, const cltRentContract*pclcontract)
{
	return pclStruct[sivillageunique]->IsSameContract(pclcontract);
}

// 임차신청자를 등록한다. 
BOOL cltHouseManager::AddApplyer(SI32 sivillageunique, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract )
{
	return pclStruct[sivillageunique]->AddApplyer(pclapplyer, pclcontract);
}

// 임차 신청자를 삭제한다. 
BOOL cltHouseManager::DeleteApplyer(SI32 sivillageunique, SI32 personid)
{
	return pclStruct[sivillageunique]->DeleteApplyer(personid);
}

// 빈집인지 여부를 확인한다. 
BOOL cltHouseManager::IsEmpty(SI32 sivillageunique, SI32 slot)
{
	return pclStruct[sivillageunique]->IsEmpty(slot);
}

//PersonID를 가지고 희망자의 정보를 구한다. 
cltRentApplyer* cltHouseManager::GetHouseApplyerFromPersonID(SI32 sivillageunique, SI32 personid)
{
	return pclStruct[sivillageunique]->GetHouseApplyerFromPersonID(personid);
}

// 임차인을 시전에 등록한다.
BOOL cltHouseManager::SetRent(SI32 sivillageunique, SI32 slot, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract, const TCHAR* pszHouseName)
{
	if( NULL != pszHouseName)
	{
		cltHouseUnit* pclHouseUnit = GetHouseUnit(sivillageunique, slot);
		StringCchCopy(pclHouseUnit->m_szName, MAX_PLAYER_NAME, pszHouseName);
	}
	
	return pclStruct[sivillageunique]->SetRent(slot, pclapplyer, pclcontract);
}

// 특정 Person이 단위 시전의 주인인지 확인한다. 
bool cltHouseManager::IsHouseHost(SI32 personid, SI32 villageunique, SI32 houseunitslot)
{
	if(villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)return false;

	if(pclStruct[villageunique] == NULL)return false;

	return pclStruct[villageunique]->IsHouseHost(personid, houseunitslot);

}

// 창고에  아이템을 추가할 수 있는지 확인한다. 
bool cltHouseManager::CanAddStg(SI32 villageunique, SI32 houseunitslot, cltItemManagerCommon* pclItemManager, cltItem* pclitem, SI32* pitempos)
{
	if(villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)return false;

	if(pclStruct[villageunique] == NULL)return false;

	return pclStruct[villageunique]->CanAddStg(houseunitslot, pclItemManager, pclitem, pitempos);

}

// 아이템을 창고에 넣는다.
void cltHouseManager::SetItemStg(SI32 villageunique, SI32 houseunitslot, SI32 stgpos, cltItem* pclitem)
{
	if(villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)return ;

	if(pclStruct[villageunique] == NULL)return ;

	pclStruct[villageunique]->SetItemStg(houseunitslot, stgpos, pclitem);

}

// 창고 아이템의 포인터를 구한다. 
cltItem* cltHouseManager::GetStgItemPointer(SI32 villageunique, SI32 houseunitslot, SI32 stgpos)
{
	if(villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)return NULL;

	if(pclStruct[villageunique] == NULL)return NULL;

	return pclStruct[villageunique]->GetStgItemPointer(houseunitslot, stgpos);
}

// 창고의 포인터를 구한다. 
cltHouseUnitStorageInfo* cltHouseManager::GetStgPointer(SI32 villageunique, SI32 houseunitslot)
{
	if(villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)return NULL;

	if(pclStruct[villageunique] == NULL)return NULL;

	return pclStruct[villageunique]->GetStgPointer(houseunitslot);
}


// 임대료를 처리하는등의 액션을 취한다. 
void cltHouseManager::Action( cltTime* pcltime, clock_t CurrentClock )
{
	if(pcltime == NULL)return ;

	// DB 초기화 완료될때까지 기다림
	if( ((cltServer *)pclClient)->bCompleteDBInitAll == FALSE ) return;

	bool bdaychangedswitch = pcltime->IsDayChanged();
	
	static DWORD dwHouseStableHorseClock = 0;
	cltServer *pclserver = ( cltServer *)pclClient;

	//----------------------------------------
	// 말 관련 조치를 취한다.
	//----------------------------------------	
	if ( TABS( CurrentClock - dwHouseStableHorseClock ) > 30000 )
	{
		dwHouseStableHorseClock = CurrentClock;
		
		SI32 i, j, k;
		cltHouse* pclHouse = NULL;

		for( i = 0; i < MAX_VILLAGE_NUMBER; ++i )
		{
			pclHouse = (cltHouse*)pclStruct[i];
			if ( pclHouse )
			{
				for(j = 0;j < MAX_HOUSEUNIT_NUMBER; ++j)
				{
					SI32 id, PersonID;

					if ( pclHouse->clUnit[j].clPerson.szName != NULL && _tcslen( pclHouse->clUnit[j].clPerson.szName ) > 0 )
					{
						if ( pclserver->pclHashTable->Find( pclHouse->clUnit[j].clPerson.szName, PersonID, id, 300 ) == false  )
						{
							continue;
						}
					}
					else
					{
						continue;
					}
					
					for ( k = 0; k < MAX_HOUSEUNIT_HORSENUMBER; ++k  )
					{
						if ( pclHouse->clUnit[j].clStorageInfo.clHorse[ k ].siHorseUnique > 0 )
						{
							cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

							SI32 horseaction = pclchar->CharHorseAction( &pclHouse->clUnit[j].clStorageInfo.clHorse[ k ] , false);
							if(horseaction == HORSEACTION_UPDATE)
							{
								sDBRequest_HouseUnitHorseInfoSet clMsg( id, PersonID, i, j, k, &pclHouse->clUnit[j].clStorageInfo.clHorse[ k ] );
								pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader*)&clMsg );
							}
							
							SI32 ItemUnique = 0;
							SI32 ItemNum = 0;
							SI16 l = 0;
							if ( pclserver->pclHorseManager->GetHorseFoodNeed(&pclHouse->clUnit[j].clStorageInfo.clHorse[ k ],  &ItemUnique, &ItemNum ) )
							{
								SI16 ItemIndex = -1;
								cltItem clItem;
								clItem.Init();
								
								bool bFindHorseFeed = false;
								
								for ( l = 0; l < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++l )
								{
									if ( pclHouse->clUnit[j].clStorageInfo.clItem[ l ].siUnique == ITEMUNIQUE(3855) )
									{
										if ( pclHouse->clUnit[j].clStorageInfo.clItem[ l ].siItemNum < ItemNum )
										{
											ItemNum = pclHouse->clUnit[j].clStorageInfo.clItem[ l ].siItemNum;
											
											clItem.siUnique = ITEMUNIQUE(3855);
											clItem.siItemNum = 1;
										}
										else
										{
											clItem.siUnique = ITEMUNIQUE(3855);
											clItem.siItemNum = 1;
										}

										bFindHorseFeed = true;

										break;
									}
								}

								if ( bFindHorseFeed == false )
								{
									for ( l = 0; l < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++l )
									{
										if ( pclHouse->clUnit[j].clStorageInfo.clItem[ l ].siUnique == ITEMUNIQUE(3850) )
										{
											if ( pclHouse->clUnit[j].clStorageInfo.clItem[ l ].siItemNum < ItemNum )
											{
												ItemNum = pclHouse->clUnit[j].clStorageInfo.clItem[ l ].siItemNum;

												clItem.siUnique = ITEMUNIQUE(3850);
												clItem.siItemNum = 1;
											}
											else
											{
												clItem.siUnique = ITEMUNIQUE(3850);
												clItem.siItemNum = 1;
											}

											bFindHorseFeed = true;

											break;
										}
									}
								}

								if ( bFindHorseFeed == false )
								{
									for ( l = 0; l < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++l )
									{
										if ( pclHouse->clUnit[j].clStorageInfo.clItem[ l ].siUnique == ItemUnique )
										{
											if ( pclHouse->clUnit[j].clStorageInfo.clItem[ l ].siItemNum < ItemNum )
											{
												ItemNum = pclHouse->clUnit[j].clStorageInfo.clItem[ l ].siItemNum;
												
												clItem.siUnique = ItemUnique;
												clItem.siItemNum = ItemNum;
											}
											else
											{
												clItem.siUnique = ItemUnique;
												clItem.siItemNum = ItemNum;
											}
											break;
										}
									}
									
									if ( l == MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER )
									{
										continue;
									}
								}
								
								ItemIndex = l;

								if ( pclserver->pclHorseManager->GiveFood(&pclHouse->clUnit[j].clStorageInfo.clHorse[ k ],  &clItem ) )
								{
									sDBRequest_HouseUnitHorseInfoSet clMsg( id, PersonID, i, j, k, &pclHouse->clUnit[j].clStorageInfo.clHorse[ k ] );
									pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader*)&clMsg );
									
									sDBRequest_HouseUnitHorseEatFood clMsg1( id, PersonID, i, j, ItemIndex, clItem.siUnique, clItem.siItemNum );
									pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader*)&clMsg1 );
								}
							}
						}
						else
						{
							continue;
						}
					}
				}
			}
		}
	}
	


	//----------------------------------------
	// 매일 임대료을 처리한다. 
	//----------------------------------------
	if(bdaychangedswitch)
	{
		SI32 i, j;
		
		cltHouse* pclHouse ;
		
		// 통계를 위한 준비를 한다. 
		cltServer* pclserver = (cltServer*)pclClient;
		pclserver->pclStatisticsManager->clDailyStatistics.siTotalHouseNumber	= 0;
		pclserver->pclStatisticsManager->clDailyStatistics.siRentHouseNumber		= 0;

		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			pclHouse = (cltHouse*)pclStruct[i];
			if(pclHouse)
			{
				for(j = 0;j < MAX_HOUSEUNIT_NUMBER;j++)
				{
					SI32 personid = pclHouse->clUnit[j].clPerson.GetPersonID(); 

					//  전체 시전 개수를 구한다. 
					pclserver->pclStatisticsManager->clDailyStatistics.siTotalHouseNumber++;
					
					if(personid <= 0)continue;

					// 임대된 시전 개수를 구한다. 
					pclserver->pclStatisticsManager->clDailyStatistics.siRentHouseNumber++;

					
					// 계약 일자가 오늘과 같은 날이어야 한다. (매월 계약일에 지불된다.)
					if( pcltime->clDate.uiDay != (UI32)pclHouse->clUnit[j].clContract.clDate.GetDay())continue;
					
					// 임대료를 구한다. 
					GMONEY rentfee		= pclHouse->clUnit[j].clContract.siRentFee;
					// 마을의 유니크를 구한다. 
					SI32 villageunique	= i;
					// 단위 주택의 위치를 구한다. 
					SI32 houseindex		= j;
					
					// 전장에서 임대료를 시전으로 옮길것을 DBManager에게 요청한다. 
					sDBRequest_PayRentFee clMsg(0, personid, rentfee, villageunique, houseindex);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

					pclserver->pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, rentfee);

					// 계약 기간이 종료되었는지 확인한다. 
					if(pclHouse->clUnit[j].clContract.IsEnd(&pcltime->clDate) == true)
					{
						// DB매니저에게 시전을 회수할 것을 요청한다. 
						sDBRequest_RecallHouse clMsg(personid, HOUSERECALL_REASON_CONTRACTEND, villageunique, houseindex);
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);						
					}
					else
					{
						//----------------------------------------------
						// 계약 종료 임박 쪽지. 
						//----------------------------------------------
						// 현재 시간 변수. 
						SI32 curvary	= pcltime->clDate.GetDateVary();
						
						// 계약 종료일 변수. 
						cltDate cldate;
						cldate.Set(&pclHouse->clUnit[j].clContract.clDate);
						cldate.AddMonth(pclHouse->clUnit[j].clContract.siRentPeriod * 12);
						
						SI32 endvary	= cldate.GetDateVary();
						if(endvary <= curvary + 90)
						{
							// 쪽지를 보낸다. 
							cltServer* pclserver = (cltServer*)pclClient;
							cltLetterMsg_HouseRecallWarning clletter(villageunique, houseindex, (endvary - curvary)/30);
							pclserver->SendLetterMsg(personid, (cltLetterHeader*)&clletter);
						}
						
					}

				}
				
			}
		}

	}
}


// 게임서버로 단위시전의 리스트를 요청한다. 
void cltHouseManager::RequestHouseUnitList()
{
	SI32 id = 1;
	if(id == 0)return;

	cltMsg clMsg(GAMEMSG_REQUEST_HOUSEUNITLIST, siVillageUnique);
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

// 게임서버로 단위시전의 상세 정보 리스트를 요청한다. 
void cltHouseManager::RequestHouseUnitListDetail()
{
	SI32 id = 1;
	if(id == 0)return;

	cltMsg clMsg(GAMEMSG_REQUEST_HOUSEUNITLIST_DETAIL, siVillageUnique);
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}


// 게임서버로 임차희망자 리스트를 요청한다. 
void cltHouseManager::RequestHouseRentApplyerList()
{
	SI32 id = 1;
	if(id == 0)return;

	cltMsg clMsg(GAMEMSG_REQUEST_HOUSEAPPLYERLIST, siVillageUnique);
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}


// 서버 리턴값 처리 함수. 
void cltHouseManager::ServerRtnValOperate()
{
	// 먼저 서버로부터 받은 메시지를 복사해 둔다. 
	SI32 returnval = siServerResponseValue;

	cltStructureMngBasic::ServerRtnValOperate();
	cltClient* pclclient = (cltClient*)pclClient;

	switch(returnval)
	{
		// 임대가 성공한 경우 주택리스트와 임대신청자 리스트를 서버에 요청한다. 
	case SRVAL_HOUSE_SUCCESSHOUSERENT:
		{
	
			// 단위주택 리스트 요청. 
			RequestHouseUnitList();
			// 임차 희망자 명단 요청. 
			RequestHouseRentApplyerList();
		}	
		break;
	case SRVAL_HOUSE_SUCCESSCANCELLATION:
		{
		
			RequestHouseUnitList();
		}
		break;
	}
}

