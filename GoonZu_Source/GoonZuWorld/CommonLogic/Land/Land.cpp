#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "Land.h"
#include "..\..\Resource.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/msg/MsgType-Land.h"

#include "../CityHall/TaxpaymentListDlg.h"
#include "../Server/Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Land.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;


//----------------------------------------------
// cltLand
//----------------------------------------------
cltLand::cltLand(SI32 sivillageunique) : cltStructureBasic(sivillageunique)
{
	for( int i = 0; i < MAX_LANDUNIT_NUMBER; ++i ) {
		clUnit[ i ].Init();
	}

	 InitRentApplyer();
}


cltLand::~cltLand()
{

}




cltLandManager::cltLandManager(cltCharManager* pclcm, SI32 gamemode, SI32 Landraitid, TCHAR *strFileName ) : cltStructureMngBasic(MAX_STOCK_TAB_NUMBER, pclcm, Landraitid, strFileName )
{
	

	if(gamemode == GAMEMODE_SERVER)
	{

	}
	else
	{

	}

	
}


cltLandManager::~cltLandManager()
{
	
	
}


// 최초 환영 인사말표시..
void cltLandManager::GreetingMsg(HWND hDlg)
{
	TCHAR* pText = GetTxtFromMgr(1829);
	SetMainExplain(hDlg, pText);
}

void cltLandManager::Do_WM_INITDIALOG(SI32 charunique)
{
	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);
}


// 농장을 신설한다.
BOOL cltLandManager::CreateLand(SI32 siunique)
{
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltLand(siunique);
		return TRUE;
	}

	return FALSE;
}

// 임대료를 처리하는등의 액션을 취한다. 
void cltLandManager::Action( cltTime* pcltime, clock_t CurrentClock )
{
	if(pcltime == NULL)return ;

	// DB 초기화 완료될때까지 기다림
	if( ((cltServer *)pclClient)->bCompleteDBInitAll == FALSE ) return;

	bool bdaychangedswitch = pcltime->IsDayChanged();

	//----------------------------------------
	// 매일 임대료을 처리한다. 
	//----------------------------------------
	if(bdaychangedswitch)
	{
		SI32 i, j;
		
		cltLand* pclLand ;
		
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			pclLand = (cltLand*)pclStruct[i];
			if(pclLand)
			{
				for(j = 0;j < MAX_LANDUNIT_NUMBER;j++)
				{
					SI32 personid = pclLand->clUnit[j].clPerson.GetPersonID(); 
					
					if(personid <= 0)continue;
					
					// 계약 일자가 오늘과 같은 날이어야 한다. (매월 계약일에 지불된다.)
					if( (SI32)pcltime->clDate.uiDay != pclLand->clUnit[j].clContract.clDate.GetDay())continue;
					
					// 임대료를 구한다. 
					GMONEY rentfee		= pclLand->clUnit[j].clContract.siRentFee;

					// 마을의 유니크를 구한다. 
					SI32 villageunique	= i;

					// 단위 주택의 위치를 구한다. 
					SI32 Landindex		= j;
					
					// 전장에서 임대료를 시전으로 옮길것을 DBManager에게 요청한다. 
					sDBRequest_LandPayRentFee clMsg(0, personid, rentfee, villageunique, Landindex);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

					((cltServer *)pclClient)->pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, rentfee);

					// 계약 기간이 종료되었는지 확인한다. 
					if(pclLand->clUnit[j].clContract.IsEnd(&pcltime->clDate) == true)
					{
						// DB매니저에게 시전을 회수할 것을 요청한다. 
						sDBRequest_LandRecall clMsg(personid, LANDRECALL_REASON_CONTRACTEND, villageunique, Landindex);
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
						cldate.Set(&pclLand->clUnit[j].clContract.clDate);
						cldate.AddMonth(pclLand->clUnit[j].clContract.siRentPeriod * 12);
						
						SI32 endvary	= cldate.GetDateVary();
						if(endvary <= curvary + 90)
						{
							// 쪽지를 보낸다. 
							cltServer* pclserver = (cltServer*)pclClient;
							cltLetterMsg_LandRecallWarning clletter(villageunique, Landindex, (endvary - curvary) / 30);
							pclserver->SendLetterMsg( personid, (cltLetterHeader*)&clletter );
						}
						
					}

				}
				
			}
		}

	}
}
BOOL cltLandManager::IsFarmer( SI32 villageunique, SI32 personid )
{
	cltLand *pclLand = (cltLand*)pclStruct[ villageunique ];

	for( int i = 0; i < MAX_LANDUNIT_NUMBER; i++ )
	{		
		if( pclLand->clUnit[ i ].clPerson.GetPersonID() == personid ) return TRUE;
	}

	return FALSE;
}



// 같은 계약조건인지 확인한다. 
BOOL cltLand::IsSameContract( const cltRentContract* pclcontract )
{
	if(clStrInfo.clLandStrInfo.clRentContract.IsSame(pclcontract) == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}

// 임차 신청한다. 
BOOL cltLand::AddApplyer( const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract )
{
	SI32 i;
	for( i = 0; i < MAX_APPLY_PER_LAND; i++ ) {

		if(clApplyer[i].clPerson.GetPersonID() == 0)
		{
			clApplyer[i].Set(pclapplyer, pclcontract);
			return TRUE;
		}
	}

	return FALSE;
}

// 임차 신청자를 삭제한다. 
BOOL cltLand::DeleteApplyer( SI32 personid )
{
	SI32 i;

	for(i = 0;i < MAX_APPLY_PER_LAND;i++)
	{
		if(clApplyer[i].clPerson.GetPersonID() == personid)
		{
			clApplyer[i].Init();
			return TRUE;
		}
	}

	return FALSE;
}

// 빈집인지 여부를 확인한다. 
BOOL cltLand::IsEmpty(SI32 index)
{
	if(clUnit[index].clPerson.GetPersonID() ) return FALSE;

	return TRUE;
}

//PersonID를 가지고 임차 희망자의 정보를 구한다. 
cltRentApplyer* cltLand::GetLandApplyerFromPersonID(SI32 personid)
{
	SI32 i;

	for(i = 0;i < MAX_APPLY_PER_LAND;i++)
	{
		if(clApplyer[i].clPerson.GetPersonID() == personid)
		{
			
			return &clApplyer[i];
		}
	}
	return NULL;
}

// 임차인을 농장에 등록한다.
BOOL cltLand::SetRent( SI32 index, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract )
{
	clUnit[ index ].clPerson.Set( pclapplyer );
	clUnit[ index ].clContract.Set( pclcontract );

	return TRUE;
}

// 특정 Person이 단위 농장의 주인인지 확인한다. 
bool cltLand::IsLandHost(SI32 personid, SI32 landunitindex )
{
	if( personid == 0 )return false;
	if( landunitindex < 0 || landunitindex >= MAX_LANDUNIT_NUMBER ) return false;

	if( clUnit[ landunitindex ].clPerson.GetPersonID() == personid )return true;

	return false;
}

// 각 농장의 임대료를 징수한다.
void cltLand::GetRentFee()
{
	SI32 i;

	// 단위 농장을 검색하여 임대료를 징수한다. 
	for( i = 0; i < MAX_LANDUNIT_NUMBER; i++ )
	{
		// 임대가 되어 있는 물건만 검색한다.
		SI32 personid = clUnit[i].clPerson.GetPersonID();
		if( personid <= 0 )continue;

		// 월세를 확인한다. 
		GMONEY fee = clUnit[i].clContract.siRentFee;
	}
}



// 게임서버로 단위농장의 리스트를 요청한다. 
void cltLandManager::RequestLandUnitList()
{
	SI32 id = 1;
	if(id == 0)return;

	cltMsg clMsg(GAMEMSG_REQUEST_LAND_UNITLIST, 0);
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

// 게임서버로 단위농장의 상세 정보 리스트를 요청한다. 
void cltLandManager::RequestLandUnitListDetail()
{
	SI32 id = 1;
	if(id == 0)return;

	cltMsg clMsg(GAMEMSG_REQUEST_LAND_UNITDETAIL, 0);
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}


// 게임서버로 임차희망자 리스트를 요청한다. 
void cltLandManager::RequestLandRentApplyerList()
{
	SI32 id = 1;
	if(id == 0)return;

	cltMsg clMsg(GAMEMSG_REQUEST_LAND_APPLYERLIST, 0);
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}


