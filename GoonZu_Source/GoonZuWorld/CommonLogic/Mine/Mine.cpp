#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "Mine.h"
#include "..\..\Resource.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/msg/MsgType-Mine.h"

#include "../CityHall/TaxpaymentListDlg.h"
#include "../Server/Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Mine.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;


//----------------------------------------------
// cltMine
//----------------------------------------------
cltMine::cltMine(SI32 sivillageunique) : cltStructureBasic(sivillageunique)
{
	for( int i = 0; i < MAX_MINEUNIT_NUMBER; ++i ) {
		clUnit[ i ].Init();
	}

	 InitRentApplyer();
}


cltMine::~cltMine()
{

}




cltMineManager::cltMineManager(cltCharManager* pclcm, SI32 gamemode, SI32 portraitid, TCHAR *strFileName ) : cltStructureMngBasic(MAX_STOCK_TAB_NUMBER, pclcm, portraitid, strFileName )
{
	

	if(gamemode == GAMEMODE_SERVER)
	{

	}
	else
	{

	}


}


cltMineManager::~cltMineManager()
{
	

}


// 최초 환영 인사말표시..
void cltMineManager::GreetingMsg(HWND hDlg)
{
	TCHAR* pText = GetTxtFromMgr(3250);
	SetMainExplain(hDlg, pText);
}

void cltMineManager::Do_WM_INITDIALOG(SI32 charunique)
{
	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);
}


// 광산을 신설한다.
BOOL cltMineManager::CreateMine(SI32 siunique)
{
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltMine(siunique);
		return TRUE;
	}

	return FALSE;
}

// 임대료를 처리하는등의 액션을 취한다. 
void cltMineManager::Action( cltTime* pcltime, clock_t CurrentClock )
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
		
		cltMine* pclMine ;
		
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			pclMine = (cltMine*)pclStruct[i];
			if(pclMine)
			{
				for(j = 0;j < MAX_MINEUNIT_NUMBER;j++)
				{
					SI32 personid = pclMine->clUnit[j].clPerson.GetPersonID(); 
					
					if(personid <= 0)continue;
					
					// 계약 일자가 오늘과 같은 날이어야 한다. (매월 계약일에 지불된다.)
					if( (SI32)pcltime->clDate.uiDay != pclMine->clUnit[j].clContract.clDate.GetDay())continue;
					
					// 임대료를 구한다. 
					GMONEY rentfee		= pclMine->clUnit[j].clContract.siRentFee;

					// 마을의 유니크를 구한다. 
					SI32 villageunique	= i;

					// 단위 주택의 위치를 구한다. 
					SI32 Mineindex		= j;
					
					// 전장에서 임대료를 광산으로 옮길것을 DBManager에게 요청한다. 
					sDBRequest_MinePayRentFee clMsg(0, personid, rentfee, villageunique, Mineindex);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

					// 계약 기간이 종료되었는지 확인한다. 
					if(pclMine->clUnit[j].clContract.IsEnd(&pcltime->clDate) == true)
					{
						// DB매니저에게 광산을 회수할 것을 요청한다. 
						sDBRequest_MineRecall clMsg(personid, MINERECALL_REASON_CONTRACTEND, villageunique, Mineindex);
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
						cldate.Set(&pclMine->clUnit[j].clContract.clDate);
						cldate.AddMonth(pclMine->clUnit[j].clContract.siRentPeriod * 12);
						
						SI32 endvary	= cldate.GetDateVary();
						if(endvary <= curvary + 90)
						{
/*
							// 쪽지를 보낸다. 
							cltServer* pclserver = (cltServer*)pclClient;
							cltLetterMsg_MineRecallWarning clletter(villageunique, Mineindex, (endvary - curvary) / 30);
							pclserver->SendLetterMsg( personid, (cltLetterHeader*)&clletter );
*/
						}
						
					}

				}
				
			}
		}

	}
}
BOOL cltMineManager::IsMiner( SI32 villageunique, SI32 personid )
{
	cltMine *pclMine = (cltMine*)pclStruct[ villageunique ];

	for( int i = 0; i < MAX_MINEUNIT_NUMBER; i++ )
	{		
		if( pclMine->clUnit[ i ].clPerson.GetPersonID() == personid ) return TRUE;
	}

	return FALSE;
}



// 같은 계약조건인지 확인한다. 
BOOL cltMine::IsSameContract( const cltRentContract* pclcontract )
{
	if(clStrInfo.clMineStrInfo.clRentContract.IsSame(pclcontract) == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}

// 임차 신청한다. 
BOOL cltMine::AddApplyer( const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract )
{
	SI32 i;
	for( i = 0; i < MAX_APPLY_PER_MINE; i++ ) {

		if(clApplyer[i].clPerson.GetPersonID() == 0)
		{
			clApplyer[i].Set(pclapplyer, pclcontract);
			return TRUE;
		}
	}

	return FALSE;
}

// 임차 신청자를 삭제한다. 
BOOL cltMine::DeleteApplyer( SI32 personid )
{
	SI32 i;

	for(i = 0;i < MAX_APPLY_PER_MINE;i++)
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
BOOL cltMine::IsEmpty(SI32 index)
{
	if(clUnit[index].clPerson.GetPersonID() ) return FALSE;

	return TRUE;
}

//PersonID를 가지고 임차 희망자의 정보를 구한다. 
cltRentApplyer* cltMine::GetMineApplyerFromPersonID(SI32 personid)
{
	SI32 i;

	for(i = 0;i < MAX_APPLY_PER_MINE;i++)
	{
		if(clApplyer[i].clPerson.GetPersonID() == personid)
		{
			
			return &clApplyer[i];
		}
	}
	return NULL;
}

// 임차인을 광산에 등록한다.
BOOL cltMine::SetRent( SI32 index, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract )
{
	clUnit[ index ].clPerson.Set( pclapplyer );
	clUnit[ index ].clContract.Set( pclcontract );

	return TRUE;
}

// 특정 Person이 단위 광산의 주인인지 확인한다. 
bool cltMine::IsMineHost(SI32 personid, SI32 Mineunitindex )
{
	if( personid == 0 )return false;
	if( Mineunitindex < 0 || Mineunitindex >= MAX_MINEUNIT_NUMBER ) return false;

	if( clUnit[ Mineunitindex ].clPerson.GetPersonID() == personid )return true;

	return false;
}

// 각 광산의 임대료를 징수한다.
void cltMine::GetRentFee()
{
	SI32 i;

	// 단위 광산을 검색하여 임대료를 징수한다. 
	for( i = 0; i < MAX_MINEUNIT_NUMBER; i++ )
	{
		// 임대가 되어 있는 물건만 검색한다.
		SI32 personid = clUnit[i].clPerson.GetPersonID();
		if( personid <= 0 )continue;

		// 월세를 확인한다. 
		GMONEY fee = clUnit[i].clContract.siRentFee;
	}
}


// 게임서버로 단위농장의 리스트를 요청한다. 
void cltMineManager::RequestMineUnitList()
{
	SI32 id = 1;
	if(id == 0)return;

	cltMsg clMsg(GAMEMSG_REQUEST_MINE_UNITLIST, 0);
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

// 게임서버로 단위농장의 상세 정보 리스트를 요청한다. 
void cltMineManager::RequestMineUnitListDetail()
{
	SI32 id = 1;
	if(id == 0)return;

	cltMsg clMsg(GAMEMSG_REQUEST_MINE_UNITDETAIL, 0);
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}


// 게임서버로 임차희망자 리스트를 요청한다. 
void cltMineManager::RequestMineRentApplyerList()
{
	SI32 id = 1;
	if(id == 0)return;

	cltMsg clMsg(GAMEMSG_REQUEST_MINE_APPLYERLIST, 0);
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}


