//---------------------------------
// 2004/2/18 김태곤
//---------------------------------


#include <CommonLogic.h>
#include "..\..\Server\Server.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

//---------------------------------------
// Server
//---------------------------------------
#include "..\..\Server\Statistics\Statistics.h"
#include "..\..\Resource.h"

#include "Hunt.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Market.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Hunt.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg\MsgType-Hunt.h"

#include "../../Common/Item/ItemCommon/ItemUnit.h"
#include "../../Common/Char/CharCommon/Char-Common.h"

#include "../CityHall/TaxpaymentListDlg.h"
#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "../../common/Event/event.h"

extern cltCommonLogic* pclClient;


//-----------------------------------------------
// cltHunt
//-----------------------------------------------
cltHunt::cltHunt(SI32 sivillageunique) : cltStructureBasic(sivillageunique)
{
	m_PreClock = 0;

	siLeftMonNum	= 0;
	siLeftUserNum	= 0;
	dwLastCheckClock		= 0;
	dwLastGiveExpClock	= 0;

}

cltHunt::~cltHunt()
{
	
}

void cltHunt::AddLeftMon(SI32 kind, SI16* pdata)
{
	SI32 i;


	for(i = 0;i < MAX_HUNT_LINEUP_NUMBER;i++)
	{
		if(pdata[i * 2] == kind)
		{
			pdata[i * 2 + 1]++;
			return ;
		}
	}

	for(i = 0;i < MAX_HUNT_LINEUP_NUMBER;i++)
	{
		if(pdata[i * 2] == 0)
		{
			pdata[i * 2]		= kind;
			pdata[i * 2 + 1]	= 1;
			return ;
		}
	}
	
}

// 수렵장에 남은 몬스터 정보를 확보한다.
void cltHunt::CheckLeftMonNum(SI16* pdata)
{

	SI32 id;
	SI32 index = 0;
	SI32 mapindex = siVillageUnique + 200;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		if (pclClient->pclCM->IsAlive(id) == FALSE)										continue;	// 살아 있어야 하고, 
		if (IsPC(id) )																	continue;	// 사용자가 아니어야 하고, 
		if (pclClient->pclCM->CR[id]->GetMapIndex() != mapindex)						continue;	// 수렵장의 지도에 있어야 한다.
		if (pclClient->pclCM->CR[id]->clIdentity.siIdentity == IDENTITY_PERSON_SUMMON)	continue;	// 소환수 여서는 안된다.
		if (pclClient->pclCM->CR[id]->clIdentity.siIdentity == IDENTITY_SOULGUARD)		continue;	// 수호정령

		SI32 kind		= pclClient->pclCM->CR[id]->GetKind();

		if( kind == pclClient->GetUniqueFromHash(TEXT("KIND_JOKER")) ) continue;


		SI32 itemunique = pclClient->pclKindInfoSet->pclKI[kind]->siDollUnique;

		AddLeftMon(itemunique, pdata);
	}
	

}

// 사용자를 내 쫓는다. 
void cltHunt::GoOutUser()
{
	// 모두 수렵장에서 내 보낸다. 
	SI32 id;
	SI32 index = 0;
	SI32 mapindex = siVillageUnique + 200;

	if(pclClient->pclMapManager->pclMapArray[siVillageUnique] == NULL)return ;

	SI32 maptypeunique = pclClient->pclMapManager->pclMapArray[siVillageUnique]->siMapUnique;
	if(maptypeunique <= 0)return ;

	// 사용자 수를 초기화한다. 
	siLeftUserNum = 0;

	SI32 x = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
	SI32 y = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;

	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		if(pclClient->pclCM->IsAlive(id))
		{
			// 사용자는 모두 내보낸다. 
			if(IsPC(id) && pclClient->pclCM->CR[id]->GetMapIndex() == mapindex)
			{
				cltWarpInfo clWarpInfo(1, siVillageUnique, false, false, x, y, -1, -1);
				pclClient->pclCM->CR[id]->SetWarpInfo(&clWarpInfo, 800);
				pclClient->pclCM->CR[id]->Warp();

				// 사유를 알린다. 
				((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_HUNT_NOTENOUGHMON,  0, 0, id);

			}
		}
	}


}
// 몬스터를 배치한다. 
bool cltHunt::SetMonster()
{

	SI32 i;
	SI32 x, y;

	siLeftMonNum	= 0;

	cltServer* pclserver = (cltServer*)pclClient;
	

	SI32 kind;
	SI32 maxkindnum;
	SI32 level;
	
	SI32 monindex = 0;
	cltHuntLineUp* pcllineup = clStrInfo.clHuntStrInfo.clHuntLineUp;
	
	_SYSTEMTIME sTime;
	GetLocalTime(&sTime);

	for(i = 0;i < MAX_HUNT_LINEUP_NUMBER;i++)
	{
		kind		= pcllineup[i].siLineUpKind;
		maxkindnum	= pcllineup[i].siLineUpKindNum;
		level		= pcllineup[i].siLineUpKindLevel;

		//	이벤트 기간끝나면 칠면조 배치 안시킨다.
		if(!pclClient->pclEventTimeManager->InEventTime(TEXT("Chuseok_Monster_Appear"), &sTime))
		{
			if(kind == pclClient->GetUniqueFromHash(TEXT("KIND_TURKEY")))
			{
				continue;
			}
		}

		siLeftMonNum += maxkindnum;

		SI32 curmonnum  = 0;
		while(curmonnum < maxkindnum)
		{
			SI32 mapindex = siVillageUnique + 200;
			
			x = rand() % pclserver->pclMapManager->pclMapArray[mapindex]->GetXsize();
			y = rand() % pclserver->pclMapManager->pclMapArray[mapindex]->GetYsize();
			
			SI32 dx, dy;
			if(pclserver->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(kind, x, y, 10, &dx, &dy, true) == false)continue;
			
			// NPC를 만들고 아이템도 준다. 
			cltInnerStatusInfo clinnerstatus;
			clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);
			
			if(pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, 1, 0, dx, dy, mapindex, NPCITEMMODE_HUNT, &clinnerstatus ) > 0)
			{
				curmonnum++;
			}
			
		}
		monindex++;
	}

	return true;

}



void cltHunt::Action()
{		
	
	// 마을 상세 정보 보기에서, 수렵장 정보를 보여줄 때 입장 가능한지 아니한지를 알려주기 위해서...
	if( TABS(pclClient->CurrentClock - m_PreClock) > 60000 )			// 60000 = 60초
	{
		m_PreClock = pclClient->CurrentClock;

		if( pclClient->pclHuntManager->IsHuntLineUpMonsterReady( siVillageUnique ) == false )
		{
			clStrInfo.clHuntStrInfo.siHuntStatus = HUNT_ENTER_ITEMINSUFFICIENY;
		}
		else
		{
			for( SI32 i = 0; i < MAX_HUNT_LINEUP_NUMBER; ++i )
			{
				if ( clStrInfo.clHuntStrInfo.clHuntLineUp[i].siLineUpKind > 0 )
				{
					break;
				}
			}

			if ( i == MAX_HUNT_LINEUP_NUMBER )
			{
				clStrInfo.clHuntStrInfo.siHuntStatus = HUNT_ENTER_ITEMINSUFFICIENY;
			}
			else
			{
				clStrInfo.clHuntStrInfo.siHuntStatus = HUNT_ENTER_ENABLESTATUS;
			}
		}
	}
	

	if(siLeftUserNum == 0)
	{
		if(siLeftMonNum)
		{
			// 남은 몬스터는 창고에 다시 넣는다. 
			SI16 siLeftMonInfo[MAX_HUNT_LINEUP_NUMBER * 2];
			ZeroMemory(siLeftMonInfo, sizeof(siLeftMonInfo));
			CheckLeftMonNum(siLeftMonInfo);
			
			pclClient->pclHuntManager->RecoverDollItem(siVillageUnique, siLeftMonInfo);
			
			// mapindex에 있는 모든 npc를 삭제한다. 
			pclClient->pclCM->DeleteAllNPCInMapIndex(siVillageUnique + 200);
			
			// 몬스터를 모두 삭제한다. 
			siLeftMonNum	= 0;
			
		}
	}

	// 사용자가 존재하고 몬스터는 모두 처치 되었다면, 
	if(siLeftUserNum && siLeftMonNum == 0)
	{
		// 몬스터가 부족하면 강퇴. 
		if(pclClient->pclHuntManager->IsHuntLineUpMonsterReady(siVillageUnique) == true)
		{
			// 수렵장의 기존 몬스터를 모두 제거하고 새롭게 몬스터를 만든다. 
			if(pclClient->pclHuntManager->SetNewMonster(siVillageUnique) == true)
			{
				// 수렵장 창고의 재고를 소모시킨다. 
				pclClient->pclHuntManager->WasteStgDoll(siVillageUnique);
			}
		}
		else
		{
			// 재고 부족으로 사용자를 내보낸다. 
			GoOutUser();
			return ;
		}

	}

	//-------------------------------------------------
	// 행수에게 경험치를 준다. (10분 간격) 
	//-------------------------------------------------
	if(TABS(pclClient->CurrentClock - dwLastGiveExpClock)  >= 600000)
	{
		dwLastGiveExpClock = pclClient->CurrentClock;

		// 경험치를 수렵장 행수에게 준다. 
		// 사용자의 접속 여부와 관계없이 경험치를 준다.
		if(clStrInfo.clHuntStrInfo.siChiefExp)
		{
			cltServer* pclserver = (cltServer*)pclClient;
			cltRank* pclrank = pclserver->pclRankManager->GetRank(RANKTYPE_HUNT, siVillageUnique);
			if(pclrank)
			{
				SI32 huntchiefpersonid = pclrank->clPerson.siPersonID;
				
				if(huntchiefpersonid)
				{
					pclClient->pclCM->IncExp(huntchiefpersonid, clStrInfo.clHuntStrInfo.siChiefExp, CHANGE_EXP_REASON_HUNTCHIEF);
					
					// 경험치를 초기화한다. 
					clStrInfo.clHuntStrInfo.siChiefExp = 0;
				}
			}
		}
		
		// 수렵장에 지급해야 할 돈이 있는가 ?
		if(clStrInfo.clHuntStrInfo.siTempMoney)
		{

			sDBRequest_ChangeStrMoney sendMsg( CHANGESTRMONEY_HUNT, 0, siVillageUnique, RANKTYPE_HUNT,  clStrInfo.clHuntStrInfo.siTempMoney);
			((cltServer *)pclClient)->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&sendMsg );

	
			// 지급할 돈을 초기화 한다. 
			clStrInfo.clHuntStrInfo.siTempMoney = 0;
		}
	}
}


cltHuntManager::cltHuntManager(cltCharManager* pclcm, SI32 gamemode, SI32 ranktype, TCHAR *strFileName ) : cltStructureMngBasic(MAX_HUNT_TAB_NUMBER, pclcm, ranktype, strFileName )
{	
	
	m_hHuntChiefDlg = NULL;
	m_hHuntUserDlg = NULL;

	m_hImageList = NULL;
	
	
	m_pclMyItem = new cltItem[ MAX_ITEM_PER_PERSON ];
	m_pclHuntStg = new cltHuntStg[ MAX_HUNT_STG_NUMBER ];
	memset( m_pclHuntStg, 0, sizeof( cltHuntStg ) * MAX_HUNT_STG_NUMBER );

	memset( m_strListSelectedMonsterName, 0, 64 );

	// 수렵장 맵
#ifdef _SAFE_MEMORY
	m_siHuntMapUnique.ZeroMem();
#else
	memset( m_siHuntMapUnique, 0, sizeof(m_siHuntMapUnique) );
#endif
	m_siHuntMapNumber = 0;
	m_siHuntMapNumber = 0;
	
	siHuntAddRate = 0;


}

cltHuntManager::~cltHuntManager()
{
	m_siInventorySelectedIndex = -1;
	m_siHuntStorageSelectedIndex = -1;
	
	if ( m_pclMyItem )
	{
		delete [] m_pclMyItem;
	}

	if( m_pclHuntStg )
	{
		delete m_pclHuntStg;
		m_pclHuntStg = NULL;
	}
}

// 최초 환영 인사말표시 . 
void cltHuntManager::GreetingMsg(HWND hDlg)
{
	TCHAR* pText = GetTxtFromMgr(2153);
	SetMainExplain(hDlg, pText);
}

void cltHuntManager::Do_WM_INITDIALOG(SI32 charunique)
{
	
	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);
}

// 수렵장을 신설한다.
BOOL cltHuntManager::CreateHunt(SI32 siunique)
{
	
	// 수렵장을 만든다. 
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltHunt(siunique);
		return TRUE;
	}
		
	return FALSE;
}

// 수렵장의 몬스터를 모두 제거하고 다시 만든다. 
bool cltHuntManager::SetNewMonster(SI32 villageunique)
{

	// mapindex에 있는 모든 npc를 삭제한다. 
	pclClient->pclCM->DeleteAllNPCInMapIndex(villageunique + 200);

	// 새롭게 npc를 배치한다.
	cltHunt* pclhunt = (cltHunt*)pclStruct[villageunique];
	if(pclhunt == NULL)return false;

	return pclhunt->SetMonster();
}


void cltHuntManager::Action()
{
	SI32 i;

	//----------------------------------------------
	// 수렵장 어드벤티지를 계산한다. 
	//----------------------------------------------
	siHuntAddRate	= 0;
	SI32 targetrate = 30;	// 수렵장 이용자의 목표 비율. 

	// DB 초기화 완료될때까지 기다림
	if( ((cltServer *)pclClient)->bCompleteDBInitAll == FALSE ) return;


	cltServer* pclserver = (cltServer*)pclClient;
	if(pclserver && pclserver->pclStatisticsManager && pclserver->pclStatisticsManager->clDailyStatistics.siDungeonUserNumber)
	{
		// 수렵장 이용자의 비율. 
		SI32 rate = pclserver->pclStatisticsManager->clDailyStatistics.siHuntUserNumber * 100  / pclserver->pclStatisticsManager->clDailyStatistics.siDungeonUserNumber;
		
		// 부족비율. . 
		if(targetrate > rate)
		{
			SI32 shortrate = (targetrate - rate) * 100 / targetrate;
			
			if(shortrate)
			{
				siHuntAddRate = 80 * shortrate / 100;
			}
		}
	}
	
	// 수렵장별 몬스터 수를 파악한다. 
	static DWORD dwLastCheckClcok = 0;
	if(TABS(pclClient->CurrentClock - dwLastCheckClcok) > 5000)
	{
		dwLastCheckClcok = pclClient->CurrentClock;

		// 수렵장 잔여 몬스터를 초기화한다. 
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			cltHunt* pclhunt = (cltHunt*)pclStruct[i];
			if(pclhunt == NULL)continue;
			
			pclhunt->siLeftMonNum = 0;
			pclhunt->siLeftUserNum = 0;
		}

		SI32 id;
		SI32 index = 0;
		while(id = pclClient->pclCM->GetCharOrder(index))
		{
			index++;
			
			// 사망한 캐릭터는 무효. 
			if(pclClient->pclCM->IsAlive(id) == false)continue;
			
			SI32 mapindex = pclClient->pclCM->CR[id]->GetMapIndex();
			// 전투 능력이 없거나 수렵장에 있지 않으면 무효. 
			if(pclCM->CR[id]->IsAtb(ATB_ATTACK)==false || pclClient->pclMapManager->IsHuntMap(mapindex) == false)continue;

			cltHunt* pclhunt = (cltHunt*)pclStruct[mapindex - 200];
	
			if(IsPC(id))
			{
				pclhunt->siLeftUserNum++;
			}
			else
			{
				// 소환수가 아니면, 
				if(pclCM->CR[id]->clIdentity.siIdentity != IDENTITY_PERSON_SUMMON)
				{
					pclhunt->siLeftMonNum++;
				}
			}
		}
	}


	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		cltHunt* pclhunt = (cltHunt*)pclStruct[i];
		if(pclhunt == NULL)continue;

		pclhunt->Action();

	}

}


bool cltHuntManager::IsMyInventoryChanged()
{
	for ( int j = PERSONITEM_INV0; j < MAX_ITEM_PER_PERSON; ++j )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_pclMyItem[ j ];
		BYTE* dst = (BYTE*)&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ j ];

		for(int i = 0;i < size; i++)
		{
			if( src[i] != dst[i] )
			{
				return true;
			}
		}
	}

	return false;
}

void cltHuntManager::LoadMyInventory()
{

	/*
	for ( int i = PERSONITEM_INV0; i < PERSONITEM_SUMMON1EQUIP0; ++i )
	{
		m_pclMyItem[i] = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ];
	}
	
	if( m_hHuntChiefDlg )
	{
		ListView_DeleteAllItems( GetDlgItem( m_hHuntChiefDlg, IDC_LIST_INVENTORY ) );
		
		SetInventoryInListView( pclClient->pclItemManager, m_hHuntChiefDlg, IDC_LIST_INVENTORY, NULL, &pclClient->pclCM->CR[1]->pclCI->clCharItem );
	}
	*/
	return;
}

bool cltHuntManager::IsHuntStorageInventoryChanged()
{
	for ( int i = 0; i < MAX_HUNT_STG_NUMBER; ++i )
	{
		if ( m_pclHuntStg[ i ].siItemNum != clClientStrInfo.clHuntStrInfo.clHuntStg[ i ].siItemNum || m_pclHuntStg[ i ].siItemUnique != clClientStrInfo.clHuntStrInfo.clHuntStg[ i ].siItemUnique )
		{
			return true;
		}
	}	
	
	return false;
}

void cltHuntManager::LoadHuntStorageInventory()
{
	/*
	SendMessage( m_hMonsterComboList, CB_RESETCONTENT, 0, 0 );

	for ( int i = 0; i < MAX_HUNT_STG_NUMBER; ++i )
	{
		m_pclHuntStg[i].Set( &clClientStrInfo.clHuntStrInfo.clHuntStg[ i ] );
		if ( m_pclHuntStg[i].siItemUnique != 0 )
		{
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( m_pclHuntStg[i].siItemUnique );
			if(ref > 0)
			{
				SI32 monkind = pclClient->pclItemManager->pclItemInfo[ref]->siMakeKind;
				TCHAR *strMonsterName = (TCHAR*)pclClient->pclKindInfoSet->GetName( monkind );

				if ( strMonsterName != NULL )
				{
					SendMessage( m_hMonsterComboList, CB_ADDSTRING, 0, (LPARAM)strMonsterName );
				}
			}

			
		}
	}

	// 리스트 뷰에 그려주는 부분
	if( m_hHuntChiefDlg )
	{
		ListView_DeleteAllItems( GetDlgItem( m_hHuntChiefDlg, IDC_LIST_HUNTSTORAGE ) );
		SetInventoryInListView( pclClient->pclItemManager, m_hHuntChiefDlg, IDC_LIST_HUNTSTORAGE, NULL, m_pclHuntStg, MAX_HUNT_STG_NUMBER, true );
	}
	*/
	return;
}

void cltHuntManager::LoadLineUpInfoList()
{
	if( pStrBaseDlg ) {
		bUpdateLineUpInfo = TRUE;
	
	} else {

		ListView_DeleteAllItems( m_hHuntLineUpInfoList );

		memset( m_strListSelectedMonsterName, 0, 64 );
		
		LVITEM LI;

		SI16 Count = 0;
		LI.mask = LVIF_TEXT; // LVIF_IMAGE;
		LI.state = 0;
		LI.stateMask = 0;
		//LI.iImage = -1;
		TCHAR Buffer[ 32 ] = TEXT("");

		for ( int i = 0; i < MAX_HUNT_LINEUP_NUMBER; ++i )
		{
			if ( clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind > 0 )
			{
				LI.iItem = Count++;			
				LI.iSubItem = 0;			
				TCHAR *MonsterName = (TCHAR*)pclClient->pclKindInfoSet->GetName( clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind );
				if ( MonsterName == NULL )
				{
					continue;
				}
				//strcpy( LI.pszText, MonsterName );
				LI.pszText = MonsterName;
				SendMessage( m_hHuntLineUpInfoList, LVM_INSERTITEM, 0, (LPARAM)&LI );
				
				LI.iSubItem = 1;			
				StringCchPrintf( Buffer, 32, TEXT("%d"), clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKindLevel );
				//strcpy( LI.pszText, Buffer );
				LI.pszText = Buffer;			

				SendMessage( m_hHuntLineUpInfoList, LVM_SETITEM, 0, (LPARAM)&LI );

				LI.iSubItem = 2;			
				StringCchPrintf( Buffer, 32,  TEXT("%d"), clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKindNum );
				//strcpy( LI.pszText, Buffer );
				LI.pszText = Buffer;			
				SendMessage( m_hHuntLineUpInfoList, LVM_SETITEM, 0, (LPARAM)&LI );
			}
		}
	}

	return;
}

// 수렵장 창고의 재고를 소모시킨다. 
void cltHuntManager::WasteStgDoll(SI32 villageunique)
{
	cltStrInfo* pclstr = GetStrInfo( villageunique );
	
	if( pclstr == NULL )
	{
		return;
	}

	SI16 siStorageItemUnique[ MAX_HUNT_LINEUP_NUMBER ];
	SI16 siStorageItemNum[  MAX_HUNT_LINEUP_NUMBER ];
	SI16 siStorageItemIndex[ MAX_HUNT_LINEUP_NUMBER ];

	memset( siStorageItemUnique, 0, sizeof( SI16 ) * MAX_HUNT_LINEUP_NUMBER );
	memset( siStorageItemNum, 0, sizeof( SI16 ) *  MAX_HUNT_LINEUP_NUMBER );
	memset( siStorageItemIndex, 0, sizeof( SI16 ) * MAX_HUNT_LINEUP_NUMBER );

	
	for ( SI16 i = 0; i < MAX_HUNT_LINEUP_NUMBER; ++i )
	{
		if ( pclstr->clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind > 0 )
		{
			for ( SI16 j = 0; j < MAX_HUNT_STG_NUMBER; ++j )
			{
				if ( pclstr->clHuntStrInfo.clHuntStg[ j ].siItemUnique > 0 )
				{
					SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( pclstr->clHuntStrInfo.clHuntStg[ j ].siItemUnique );
					if ( Ref > 0 )
					{
						SI32 monkind = pclClient->pclItemManager->pclItemInfo[Ref]->siMakeKind;

						if ( monkind == pclstr->clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind )
						{
							siStorageItemUnique[ i ] = pclstr->clHuntStrInfo.clHuntStg[ j ].siItemUnique;

							//cyj 인형아이템하나당 몬스터 10마리이므로
							siStorageItemNum[ i ] = pclstr->clHuntStrInfo.clHuntLineUp[ i ].siLineUpKindNum / 10;
							siStorageItemIndex[ i ] = j;
							break;
						}
					}
				}
			}
		}
	}

	sDBRequest_HuntStorageDecreaseDollItemNum clMsg( villageunique, siStorageItemUnique, siStorageItemIndex, siStorageItemNum );
	pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
}

bool cltHuntManager::IsHuntLineUpMonsterReady( SI32 VillageUnique )
{
	cltStrInfo* pclstr = GetStrInfo( VillageUnique );
	
	if( pclstr == NULL )
	{
		return false;
	}
	
	SI32 i = 0;
	for( ; i < MAX_HUNT_LINEUP_NUMBER; ++i )
	{
		if ( pclstr->clHuntStrInfo.clHuntLineUp[i].siLineUpKind > 0 )
		{
			break;
		}
	}

	if ( i == MAX_HUNT_LINEUP_NUMBER )
	{		
		return false;
	}


	for ( i =0; i < MAX_HUNT_LINEUP_NUMBER; ++i )
	{
		if ( pclstr->clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind > 0 )
		{
			bool bFindSameKind = false;
			for ( int j = 0; j < MAX_HUNT_STG_NUMBER; ++j )
			{
				SI32 StorageItemUnique = pclstr->clHuntStrInfo.clHuntStg[ j ].siItemUnique;

				if ( StorageItemUnique > 0 )
				{
					SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( StorageItemUnique );
					
					if ( Ref > 0 )
					{
						SI32 MonKind = pclClient->pclItemManager->pclItemInfo[ Ref ]->siMakeKind;
						if ( pclstr->clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind == MonKind )
						{
							bFindSameKind = true;
							if ( pclstr->clHuntStrInfo.clHuntLineUp[ i ].siLineUpKindNum > pclstr->clHuntStrInfo.clHuntStg[ j ].siItemNum )
							{
								return false;
							}
						}						
					}
				}
			}

			if ( bFindSameKind == false )
			{
				return false;
			}
		}
	}			

	return true;
}

void cltHuntManager::RecoverDollItem( SI32 VillageUnique, SI16 *pItemDataList )		// pItemDataList[0] = ItemUnique, pItemDataList[1] = ItemNum
{
	cltStrInfo* pclstr = GetStrInfo( VillageUnique );
	
	if( pclstr == NULL )
	{
		return;
	}

	for ( SI16 i = 0; i < MAX_HUNT_LINEUP_NUMBER; ++i )
	{
		if (pItemDataList[ i * 2] > 0)
		{
			//cyj 인형아이템하나당 몬스터 10마리 생성이므로
			pItemDataList[ i * 2 + 1] = pItemDataList[ i * 2 + 1] / 10;
		}
	}

	SI16 InputListIndex = -1;

	for ( SI16 i = 0; i < MAX_HUNT_LINEUP_NUMBER; ++i )
	{
		if ( pItemDataList[ i * 2 ] > 0 )
		{
			InputListIndex = -1;
			for ( SI16 j = 0; j < MAX_HUNT_STG_NUMBER; ++j )
			{
				// 아이템 넣을 곳을 찾았음.
				if ( pclstr->clHuntStrInfo.clHuntStg[ j ].siItemUnique == pItemDataList[ i * 2 ] )
				{
					// 아이템에 추가할수 있음.
					if ( pclstr->clHuntStrInfo.clHuntStg[ j ].siItemNum < MAX_ITEM_PILE_NUMBER )
					{
						if ( pclstr->clHuntStrInfo.clHuntStg[ j ].siItemNum + pItemDataList[ i * 2 + 1 ] > MAX_ITEM_PILE_NUMBER )
						{
							pItemDataList[ i * 2 + 1 ] = MAX_ITEM_PILE_NUMBER - pclstr->clHuntStrInfo.clHuntStg[ j ].siItemNum;
						}
						InputListIndex = j;
						break;
					}
					else
					{
						InputListIndex = -1;
						break;
					}
				}
				// 아이템 없는 빈 공간이면서, 아직까지 빈 공간을 못 찾은 경우
				else if ( InputListIndex < 0 && pclstr->clHuntStrInfo.clHuntStg[ j ].siItemUnique < 1 )
				{
					InputListIndex = j;
					continue;
				}
			}
			
			if ( InputListIndex > -1 )
			{
				sDBRequest_HuntLeftMonsterStgIn clMsg( VillageUnique, InputListIndex, pItemDataList[ i * 2 ], pItemDataList[ i * 2 + 1 ] );
				pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
			}
		}
	}
	
	return;
}