//---------------------------------
// 2003/8/8 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

//-------------------------------
// Common
//-------------------------------
#include "..\Common\Map\Map.h"
#include "..\Common\Order\Order.h"
#include "..\Common\GameMaster\GameMaster.h"
#include "..\Common\StockDistributionMgr\StockDistributionMgr.h"
#include "..\Common\VoteMgr\VoteMgr.h"
#include "../Common/Skill/Skill-Manager.h"
#include "..\Common\Event\Hunting\Hunting.h"
#include "../common/Horse/Horse.h"
#include "../common/Util/Util.h"
#include "..\Common\AutoHunting\AutoHunting.h"
#include "../common/Event/MofuMofu/MofuMofu.h"

#include "Char\CharManager\CharManager.h"

#include "..\..\..\Server\AuthServer\AuthMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-System.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Market.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Vote.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Stock.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Summon.h"
#include "..\..\DBManager\GameDBManager_World\DBGameLogMsg.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-IPGClient.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Guild.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Cityhall.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-PostOffice.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Village.h"

#include "../CommonLogic/Msg/MsgType-Village.h"

#include "..\CommonLogic\MsgType-System.h"
#include "..\CommonLogic\BlackArmy\BlackArmy.h"
#include "..\CommonLogic\CityHall\CityHall.h"
#include "..\COmmonLogic\Koinobory\Koinobory.h"
#include "..\Commonlogic\santaraccoon\santaraccoon.h"
#include "..\Commonlogic\KoinoboryFamily\KoinoboryFamily.h"
#include "..\CommonLogic\Msg\MsgType-WorldMoney.h"

#include "../Lib/HashTable/HashTable_StringInt.h"

#include "Notice\Notice.h"
#include "MsgType-Person.h"
#include "MsgRval-Define.h"
#include "Msg\MsgType-Summon.h"

#include "Minister/MinisterMgr.h"

#include "../../AdminSystem/MasterServer/MasterMsg.h"

#include "../CommonLogic/Msg/MsgType-AutoMacro.h"
#include "GachaManager/GachaManager.h"
#include "GawiBawiBoMgr/GawiBawiBoMgr_Server.h"


#include "../../Server/GZFtpMgr/GZFtpMgr.h"
#include "../CommonLogic/ValentineEvent/ValentineEvent.h"

#define	MAX_UPPERCENT	1000
#define	MAX_DOWNPERCENT	0.001
#define MAXMAKENUM_RACCOONDODGE	100 


//----------------------------------------------
// 종합적인 사용자 캐릭터 정보 관리 
//----------------------------------------------

extern cltCommonLogic* pclClient;

// 운영자 명령을 처리한다. 
BOOL cltServer::OperateGameMasterOrder(TCHAR* pstring, SI32 serverid)
{
	SI32 charunique = 0;
	bool bnotenoughmasterlevel = false;

	SI32 charunique1 = 0;
	SI32 charunique2 = 0;

	TCHAR order[256];
	_stscanf(&pstring[1], TEXT("%s"), order);
	
	if(lstrlen(pstring) > 1000)
	{
		return false;
	}

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[serverid];
	// 명령자의 운영자 권한 등급을 구한다. 
	SI32 gamemasterlevel = pclchar->GetGameMasterLevel();
	
	// 0 이하이고 군주도 아니라면 아무것도 할 수 없다. 
	bool bkingswitch = false;
	
	if(pclchar->pclCI->clBI.uiGRank == GRANK_KING)
	{
		if(pclchar->pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique == ITEMUNIQUE(23500)	// 적의 
			|| pclchar->pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique == ITEMUNIQUE(23505) )	// 면복.
		{
#ifdef _CHINA
			bkingswitch = false;
//#elif _NHNChina
#else
			bkingswitch = true;
#endif

		}
	}

	bool policewitch = false;
	
	if(pclchar->pclCI->clBI.uiPoliceSwitch)	// 나한대인지 체크
	{
		if(pclchar->pclCI->clCharItem.clItem[PERSONITEM_HAT].siUnique == ITEMUNIQUE(23490))	// 경찰모자 
		{
			policewitch = true;
		}
	}


	bool minister5switch = false;

	if(pclchar->pclCI->clBI.uiGRank == GRANK_MINISTER5)	// 형조판서. 
	{
		if(pclchar->pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique == ITEMUNIQUE(23510))	// 관복 
		{
#ifdef _CHINA
			minister5switch = true;
//#elif _NHNChina
#else
			minister5switch = true;
#endif
		}
	}

	
	// PlayerGM 추가 - by LEEKH 2007.05.03
	if(gamemasterlevel < BASIC_PLAYERMASTER_LEVEL && bkingswitch == false && minister5switch == false && policewitch == false)
	{
		// 클라이언트로 운영자 권한이 없음을 통보한다. 
		cltMsg clMsg(GAMEMSG_RESPONSE_GAMEMASTERLEVEL, 0);
		pclCM->CR[serverid]->SendNetMsg((sPacketHeader*)&clMsg);
		return FALSE;
	}

	// 무효한 명령어이면 리턴한다. 
	SI32 gmunique = pclGMManager->FindUniqueFromOrder(order);
	if(gmunique <= 0)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(0, SRVAL_INVALIDGMORDER,  0, 0,  serverid);

		return FALSE;
	}

	// 운영자 명령을 모두 기록에 남긴다. 
	pclLog->FilePrint(TEXT("Config\\GameMaster.log"), TEXT("User:[%s] [%s]"), pclCM->CR[serverid]->GetName(), &pstring[1]);


	// 워프 메시지 이면. (좌표기준으로 이동. ) 
	if(_tcscmp(order, TEXT("wm")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 mapindex, x, y;
			TCHAR mapname[128];
			ZeroMemory(mapname, 128);
			

			_stscanf(&pstring[1], TEXT("%s %d %s %d %d"), order, &charunique, mapname, &x, &y);

			if(( x < 0)||(y < 0))
				return FALSE;
			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;
			if(_tcslen( mapname ) <= 0)	return	FALSE	;

			ConvertUnderLineToSpace(mapname, 128);

			// GM권한 100, 101은 charunique 0만 사용하도록
			// GM권한 90이상  은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_PLAYERMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}
			
			mapindex = pclMapManager->GetMapIndexFromName(mapname);
			
			// 워프 가능한 곳인지 확인한다. 
			if(mapindex < 0 || mapindex >= MAX_MAP_NUMBER)return FALSE;
			if(pclMapManager->pclMapArray[mapindex] == NULL)return FALSE;
			if(pclMapManager->pclMapArray[mapindex]->IsInMapArea(x, y) == false)return FALSE;
			
			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;
		
			if(id)
			{
				OrderStop(id, BY_COMPUTER);

				cltWarpInfo clWarpInfo(1, mapindex, false, false, x, y, -1, -1);
				pclCM->CR[id]->SetWarpInfo(&clWarpInfo, 900);
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	if(_tcscmp(order, TEXT("move")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			// flag이 0 이면 케릭터 이름 
			// flag이 1 이면 케릭터	D
			TCHAR charname[MAX_PLAYER_NAME];
			SI32 siFlag	=	0	;

			_stscanf(&pstring[1], TEXT("%s %d %s %d"), order, &charunique, charname,&siFlag );

			// GM권한 100, 101은 charunique 0만 사용하도록
			// GM권한 90 이상 은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_PLAYERMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;
			if(siFlag > 1 )		return	FALSE	;
			if(_tcslen( charname ) <= 0)	return	FALSE	;

			switch( siFlag ) 
			{
			case 0:	// 이름 일때
				{
					SI32 toid = pclCM->GetIDFromName(charname);
					if ( IsPC(toid) )	//이름일때
					{
						if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

						SI32 id;
						if(charunique == 0)	{	id = serverid;		}
						else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
						if(id == 0)return FALSE;

						SI32 x , y , mapindex ;
						x = pclCM->CR[toid]->GetX();
						y = pclCM->CR[toid]->GetY();
						mapindex = pclCM->CR[toid]->GetMapIndex() ;
						if( IsPC(id) )
						{
							OrderStop(id, BY_COMPUTER);

							cltWarpInfo clWarpInfo(1, mapindex, false, false, x, y, -1, -1);
							pclCM->CR[id]->SetWarpInfo(&clWarpInfo, 900);
						}
					}

				}
				break;
			case 1:
				{
					SI32 siTarget = _tstoi(charname)	;

					if ( pclCM->IsValidID( siTarget) ) 
					{
						SI32 id;
						if(charunique == 0)	{	id = serverid;		}
						else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
						if(id == 0)return FALSE;

						SI32 x , y , mapindex ;
						x = pclCM->CR[siTarget]->GetX();
						y = pclCM->CR[siTarget]->GetY();
						mapindex = pclCM->CR[siTarget]->GetMapIndex() ;
						if( IsPC(id) )
						{
							OrderStop(id, BY_COMPUTER);

							cltWarpInfo clWarpInfo(1, mapindex, false, false, x, y, -1, -1);
							pclCM->CR[id]->SetWarpInfo(&clWarpInfo, 900);
						}
					}
					else
					{
						return	FALSE	;
					}

				}
				break;
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// AutoUnblock 모드 변경
	else if(_tcscmp(order, TEXT("autounblock")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siMode = 0;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &siMode);

			if ( siMode == 1)	// On
			{
				bAutoUnblock = true;
			}
			else	// Off
			{
				bAutoUnblock = false;
			}

			SaveAutoUnblockToFile( bAutoUnblock, siVersion );
						
			SendServerInfo(0);
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 흑의군단 이벤트 발생 (중국)
	else if(_tcscmp(order, TEXT("blackwar")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclBlackWarManager->MakeWar(CurrentClock, sTime.wHour );
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 흑의군단 이벤트 발생 (기존)
	else if(_tcscmp(order, TEXT("blackarmy")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclBlackArmyManager->MakeWar(CurrentClock);
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 흑의군단 이벤트 종료(중국쪽 - 모든맵) 
	else if(_tcscmp(order, TEXT("blackwarend")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);
		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{		
			pclBlackWarManager->SetLeftTime( 5000 );
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 흑의군단 이벤트 종료 (기존- 특정맵)
	else if(_tcscmp(order, TEXT("blackarmyend")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);
		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{		
			pclBlackArmyManager->AllWarEnd();
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 전쟁 신청 여부 스위치
	else if(_tcscmp(order, TEXT("warapply")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 ApplySwitch = 0;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &ApplySwitch);

			if (ApplySwitch == 1)
			{
				pclBlackArmyManager->bApplySwitch = true;
			}
			else
			{
				ApplySwitch = 0;

				pclBlackArmyManager->bApplySwitch = false;
			}

			SendServerResponseMsg(0, SRVAL_BLACKARMY_WARAPPLY,	ApplySwitch, 0, pclchar->GetCharUnique());
            
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 워프 (마을로의 이동. ) 
	else if(_tcscmp(order, TEXT("wv")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR villagename[128];
			_stscanf(&pstring[1], TEXT("%s %d %s"), order, &charunique, villagename);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}
			
			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;
			if(_tcslen( villagename ) <= 0)	return	FALSE	;

			// 마을의 이름을 토대로 MapIndex를 찾는다. 
			SI32 ref = pclVillageManager->FindVillageUniqueFromName(villagename);
			if(ref >=0)
			{
				SI32 mapindex		= pclVillageManager->pclVillageInfo[ref]->siVillageUnique;
				SI32 maptypeunique	= pclMapManager->pclMapArray[mapindex]->siMapUnique;
				SI32 x				= pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
				SI32 y				= pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;
				
				if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

				SI32 id;
				if(charunique == 0)	{	id = serverid;		}
				else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
				if(id == 0)return FALSE;
				
				OrderStop(id, BY_COMPUTER);
				
				cltWarpInfo clWarpInfo(1, mapindex, false, false, x, y, -1, -1);
				pclCM->CR[id]->SetWarpInfo(&clWarpInfo, 1000);
				
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}

	}
	//KHY - 0530 명나라 상인 구매제한. - 구매 제한 금액을 재설정 한다.
	else if(_tcscmp(order, TEXT("buylimitmoneyset")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			GMONEY num = 0		;	// 지정금액
			float fBuylimitmoneySetPrecentup = 0	;	// 증가율 
			float fBuylimitmoneySetPrecentdown = 0	;	// 증가율 

			// 최고 권한인 102 여야 한다.
			if(gamemasterlevel < BASIC_GAMEMASTER_LEVEL + 2)													return FALSE;

			_stscanf(&pstring[1], TEXT("%s %I64d %f %f"), order, &num, &fBuylimitmoneySetPrecentup, &fBuylimitmoneySetPrecentdown );
			if ( num >=0 && fBuylimitmoneySetPrecentup > 0 &&  fBuylimitmoneySetPrecentdown > 0) // 돈과 증감율을 모두 입력 했을때.
			{
				// 업 금액에 최대 최소 값 사이가 아닐때
				if( fBuylimitmoneySetPrecentup > MAX_UPPERCENT || fBuylimitmoneySetPrecentup < MAX_DOWNPERCENT)		return FALSE;
				// 다운  금액에 최대 최소 값 사이가 아닐때
				if( fBuylimitmoneySetPrecentdown > MAX_UPPERCENT || fBuylimitmoneySetPrecentdown < MAX_DOWNPERCENT)	return FALSE;

				gForeignMerchantBuyLimit	=	num	;
				fForeignMerchantPercentUP	=	fBuylimitmoneySetPrecentup		;
				fForeignMerchantPercentDOWN =	fBuylimitmoneySetPrecentdown	;
			}
			else if( num > 0 && fBuylimitmoneySetPrecentup == 0 &&  fBuylimitmoneySetPrecentdown == 0 )//돈만 입력 했을때 
			{
				gForeignMerchantBuyLimit		=	num	;
			}
			else if ( (num <= 0) || (fBuylimitmoneySetPrecentup < 0) || (fBuylimitmoneySetPrecentdown < 0) )
			{
				if ( 0 > num )								gForeignMerchantBuyLimit	= 0;
				if ( 0 > fBuylimitmoneySetPrecentup )		fForeignMerchantPercentUP	= 0;
				if ( 0 > fBuylimitmoneySetPrecentdown )		fForeignMerchantPercentDOWN	= 0;
			}

			sDBRequest_SetForeignMerchantBuyLimit clMsg( gForeignMerchantBuyLimit,fForeignMerchantPercentUP,fForeignMerchantPercentDOWN);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	
			SendServerResponseCharMsg2( 0, SRVAL_BUYLIMITMONEYSET_FOREIGNMERCHANT, SI64ToString(gForeignMerchantBuyLimit), REAL32ToString( fForeignMerchantPercentUP ), REAL32ToString( fForeignMerchantPercentDOWN ), serverid );
		}
	
	}
	//KHY - 0530 명나라 상인 구매제한. - 현재 구매제한 금액을 보여준다..
	else if(_tcscmp(order, TEXT("buylimitmoneyview")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			// 최고 권한인 102 여야 한다.
			if(gamemasterlevel < BASIC_GAMEMASTER_LEVEL + 2)
				return FALSE;
		
			_stscanf(&pstring[1], TEXT("%s"), order);

			//SendServerResponseMsg(0, SRVAL_BUYLIMITMONEYVIEW_FOREIGNMERCHANT, gForeignMerchantBuyLimit,0,serverid);			

			SendServerResponseCharMsg2( 0, SRVAL_BUYLIMITMONEYVIEW_FOREIGNMERCHANT, SI64ToString(gForeignMerchantBuyLimit), REAL32ToString( fForeignMerchantPercentUP ), REAL32ToString( fForeignMerchantPercentDOWN ), serverid );
		}
	
	}
	// 아이템을 만든다. 
	else if(_tcscmp(order, TEXT("ci")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			
			TCHAR itemname[128];
			ZeroMemory(itemname, 128);
			SI32 num = 0;

			_stscanf(&pstring[1], TEXT("%s %d %s %d"), order, &charunique, itemname, &num);

			if(num < 0)
				return FALSE;
			else if(num == 0)	
				num = 1;
			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;
			if(_tcslen( itemname ) <= 0)	return	FALSE	;

			ConvertUnderLineToSpace( itemname, 128 );

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}
			
			if(num <= 0)	num = 1;
			if(num > MAX_ITEM_PILE_NUMBER)	num = MAX_ITEM_PILE_NUMBER;
			
			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
			if (ref <= 0)	return FALSE;

			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
			pclCM->GiveItem(id, itemunique, num, GIVEITEM_MODE_PERSON, INPUTITEMPERSON_OPTION_MASTER,usedate, pclchar->GetID() );

		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// [기형] 휴면 계정 시스템 - 휴면 포인트 주기
	else if(_tcscmp(order, TEXT("givedp")) == 0)
	{
		SI32 siCharunique;
		SI32 siDormancyPoint;
		_stscanf(&pstring[1], TEXT("%s %d %d"), order, &siCharunique, &siDormancyPoint);

		// GM권한 100, 101은 charunique 0만 사용하도록
		if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
		{
			if (siCharunique != 0)
				return FALSE;
		}

		if(siCharunique !=0 && pclCM->IsValidID(siCharunique)== FALSE)	return FALSE;

		SI32 siId;
		if(siCharunique == 0)	{	siId = serverid;		}
		else				{	siId = pclCM->GetIDFromCharUnique(siCharunique);			}
		if(siId == 0)return FALSE;

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			CDormancySystem *pclDormancySystem = this->pclDormancySystem;
			if( NULL != pclDormancySystem)
			{
				CDormancyPointRewardServer *pclRewardServer = (CDormancyPointRewardServer *)pclDormancySystem->GetDormancyPointReward();
				if( NULL != pclRewardServer)
				{
					pclRewardServer->SendDBDormancyInfo(pclCM->GetCharServer(siId), siDormancyPoint);
				}
			}
				
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// [기형] 휴면 계정 시스템 - 휴면 계정 부여하기
	else if(_tcscmp(order, TEXT("setdormancyaccount")) == 0)
	{
		SI32 siCharunique;
		SI32 siDormancyState;
		_stscanf(&pstring[1], TEXT("%s %d %d"), order, &siCharunique, &siDormancyState);

		// GM권한 100, 101은 charunique 0만 사용하도록
		if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
		{
			if (siCharunique != 0)
				return FALSE;
		}

		if(siCharunique !=0 && pclCM->IsValidID(siCharunique)== FALSE)	return FALSE;

		SI32 siId;
		if(siCharunique == 0)	{	siId = serverid;		}
		else				{	siId = pclCM->GetIDFromCharUnique(siCharunique);			}
		if(siId == 0)return FALSE;

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			CDormancySystem *pclDormancySystem = this->pclDormancySystem;
			if( NULL != pclDormancySystem)
			{
				CDormancyPointRewardServer *pclRewardServer = (CDormancyPointRewardServer *)pclDormancySystem->GetDormancyPointReward();
				if(NULL != pclRewardServer)
				{
					pclRewardServer->SendDBChangeDormancyState(pclCM->GetCharServer(siId), DATA_TO_BOOL(siDormancyState));
				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// [기형] 배틀로얄 시작 시키기
	else if(_tcscmp(order, TEXT("battleroyal")) == 0)
	{
		_stscanf(&pstring[1], TEXT("%s"), order);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if( NULL == m_pclBattleRoyalMgr)
				return FALSE;
			if( FALSE == m_pclBattleRoyalMgr->OperatorGMBattleRoyal())
				return FALSE;
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}

	// [기형] 배틀로얄 다음 단계로 넘기기
	else if(_tcscmp(order, TEXT("battleroyalnext")) == 0)
	{
		_stscanf(&pstring[1], TEXT("%s"), order);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if( NULL == m_pclBattleRoyalMgr)
				return FALSE;
			m_pclBattleRoyalMgr->SetNextStep();
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}


	// [기형] 배틀로얄 모드 변경
	else if(_tcscmp(order, TEXT("brmode")) == 0)
	{
		SI32 siMode = 0;
		_stscanf(&pstring[1], TEXT("%s %d"), order, &siMode);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if( NULL == m_pclBattleRoyalMgr)
				return FALSE;
			if( FALSE == m_pclBattleRoyalMgr->SetGMGameMode(siMode))
				return FALSE;
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}

	// [기형] 발렌타인 이벤트 시작, 종료
	else if(_tcscmp(order, TEXT("ccstore")) == 0)
	{
		_stscanf(&pstring[1], TEXT("%s"), order);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			CValentineEventServerMgr* pclValentineEventServerMgr = (CValentineEventServerMgr*)m_pclValentineEventMgr;
			if( NULL == pclValentineEventServerMgr)
				return FALSE;
			BOOL bReturn = pclValentineEventServerMgr->OperationGMCommandEventState();

			if( EVALENTINEEVENT_GMCOMMAND_RETURN_ENDVALENTINE  == bReturn)
			{
				return FALSE;
			}
			else
			{
				TCHAR szText[32] = {0,};
				if( EVALENTINEEVENT_GMCOMMAND_RETURN_HOLDON  == bReturn)
				{
					StringCchCopy( szText , 32, TEXT("event start"));
				}
				else if(EVALENTINEEVENT_GMCOMMAND_RETURN_CLOSE  == bReturn)
				{
					StringCchCopy( szText , 32, TEXT("event close"));
				}

				cltGameMsgResponse_Notice clNotice(order, szText, true, true);
				cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

				pclCM->CR[serverid]->SendNetMsg((sPacketHeader*)&clMsg);

			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}

	// [기형] 발렌타인 아이템 판매점에서 원하는 만큼 사기
	else if(_tcscmp(order, TEXT("ccpurchase")) == 0)
	{
		SI32 siNPCCharUnique = 0;
		SI32 siItemUnique = 0;
		SI32 siItemCount = 0;

		_stscanf(&pstring[1], TEXT("%s %d %d %d"), order, &siNPCCharUnique, &siItemUnique, &siItemCount);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			CValentineEventServerMgr* pclValentineEventServerMgr = (CValentineEventServerMgr*)m_pclValentineEventMgr;
			if( NULL == pclValentineEventServerMgr)
				return FALSE;
			if(false ==  pclValentineEventServerMgr->OperationGMCommandBuyPresent(siNPCCharUnique, siItemUnique, siItemCount))
				return FALSE;
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}

	// [기형] 발렌타인 이벤트 판매점 재고 수량 확인
	else if(_tcscmp(order, TEXT("ccinventory")) == 0)
	{
		SI32 siNPCCharUnique = 0;
		SI32 siItemUnique = 0;
		
		_stscanf(&pstring[1], TEXT("%s %d %d"), order, &siNPCCharUnique, &siItemUnique);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			CValentineEventServerMgr* pclValentineEventServerMgr = (CValentineEventServerMgr*)m_pclValentineEventMgr;
			if( NULL == pclValentineEventServerMgr)
				return FALSE;

			SI32 siRemainedItemCount = pclValentineEventServerMgr->GetRemainedItemCount(siNPCCharUnique, siItemUnique);
			
			TCHAR szText[32] = {0,};
			StringCchPrintf( szText , 32, TEXT("%d"), siRemainedItemCount);

			cltGameMsgResponse_Notice clNotice(order, szText, true, true);
			cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

			pclCM->CR[serverid]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}

	// [기형] 버프 효과 고정
	else if(_tcscmp(order, TEXT("ccbuf")) == 0)
	{
		_stscanf(&pstring[1], TEXT("%s"), order);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			CValentineEventServerMgr* pclValentineEventServerMgr = (CValentineEventServerMgr*)m_pclValentineEventMgr;
			if( NULL == pclValentineEventServerMgr)
				return FALSE;
			BOOL bReturn = pclValentineEventServerMgr->OperationGMCommandBufState();

			if( EVALENTINEEVENT_GMCOMMAND_RETURN_ENDVALENTINE  == bReturn)
			{
				return FALSE;
			}
			else
			{
				TCHAR szText[32] = {0,};
				if( EVALENTINEEVENT_GMCOMMAND_RETURN_BUF_STATIC  == bReturn)
				{
					StringCchCopy( szText , 32, TEXT("enable default"));
				}
				else if(EVALENTINEEVENT_GMCOMMAND_RETURN_BUF_RAND  == bReturn)
				{
					StringCchCopy( szText , 32, TEXT("enable random"));
				}

				cltGameMsgResponse_Notice clNotice(order, szText, true, true);
				cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

				pclCM->CR[serverid]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}


	else if(_tcscmp(order, TEXT("ri")) == 0)		// remove item
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR itemname[128];
			ZeroMemory(itemname, 128);

			SI32 num = 0;

			_stscanf(&pstring[1], TEXT("%s %d %s %d"), order, &charunique, itemname, &num);

			if(num <= 0)
				return FALSE;

			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;
			if(_tcslen( itemname ) <= 0)	return	FALSE	;

			ConvertUnderLineToSpace( itemname, 128 );

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(num <= 0)	num = 1;
			if(num > MAX_ITEM_PILE_NUMBER)	num = MAX_ITEM_PILE_NUMBER;

			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			cltItem clRemoveItem;
			cltCharServer* pclchar = pclCM->GetCharServer( id );
			for ( SI32 i = PERSONITEM_INV0 ; i < MAX_ITEM_PER_PERSON ; i ++ )
			{
				if( pclchar->pclCI->clCharItem.clItem[i].siUnique == itemunique &&
					pclchar->pclCI->clCharItem.clItem[i].siItemNum >= num )
				{
					clRemoveItem.Set(&pclchar->pclCI->clCharItem.clItem[i]);
					clRemoveItem.SetItemNum( num );
					sDBRequest_UseItem useitem(id, pclchar->pclCI->GetPersonID(), i, &clRemoveItem, USEITEM_REASON_NONE);
					pclGameDBProcess->SendMsg(DBSELECT_BASE, (sPacketHeader *)&useitem);

					break;
				}
			}
		}
	}
	else if(_tcscmp(order, TEXT("rp")) == 0)		// remove position
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 pos = 0;

			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &charunique, &pos);

			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;
			if(pos <= 0 ) return FALSE;
			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			if( pos < 0 || pos >= MAX_ITEM_PER_PERSON )		return FALSE;

			sDBRequest_UseItem useitem(id, pclCM->CR[id]->pclCI->GetPersonID(), pos, &pclCM->CR[id]->pclCI->clCharItem.clItem[pos], USEITEM_REASON_NONE);
			pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
		}
	}
	//KHY - 0613 - 운영자 GM 명령어. 해당 아이템을, 해당 개수만큼 만들기위한 모든 재료를 받는다
	else if(_tcscmp(order, TEXT("makeitemmaterials")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			
			TCHAR itemname[128];
			ZeroMemory(itemname, 128);
			SI32 num = 0;

			_stscanf(&pstring[1], TEXT("%s %d %s %d"), order, &charunique, itemname, &num);

			if(num <= 0)
				return FALSE;

			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;
			if(_tcslen( itemname ) <= 0)	return	FALSE	;

			ConvertUnderLineToSpace( itemname, 128 );

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}
			
			if(num <= 0)	num = 1;
			// 재료아이템의 중첩 제한. 50개.
			if(num > 50)	num = 50;
			
			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
			if (ref <= 0)	return FALSE;

			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;

			//아이템 Unique를 근거로, 재료들의 상세 정보를 구한다. 
			for(SI32 i = 0;i < MAX_MATERIAL_NUMBER;i++)
			{
				SI32 materialunique = pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.clMaterial[i].siUnique;
				SI32 materialneednum = pclClient->pclItemManager->pclItemInfo[ref]->clProductionInfo.clMaterial[i].siNeedNum;
				
				if(materialunique)
				{
					//원하는 아이템 개수에 따라 각각의 재료아이템 수 증가.
					materialneednum = materialneednum * num;
					//아이템을 보낸다.
					pclCM->GiveItem(id, materialunique, materialneednum, GIVEITEM_MODE_PERSON, INPUTITEMPERSON_OPTION_MASTER,usedate, pclchar->GetID() );
				}
			}
			//아이템을 보낸다.
//			pclCM->GiveItem(id, itemunique, num, GIVEITEM_MODE_PERSON, INPUTITEMPERSON_OPTION_MASTER,usedate, pclchar->GetID() );

		}
		else
		{
			bnotenoughmasterlevel = true;
		}
		
	}
	else if(_tcscmp(order, TEXT("maketimereduce")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			_stscanf(&pstring[1], TEXT("%s %d"), order, &charunique);

			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			cltCharServer* pclchar = (cltCharServer*)pclCM->GetChar(id);

			if(pclchar)
			{
				pclchar->bMakeTimeReduce = !pclchar->bMakeTimeReduce;
			}

			if( pclchar->bMakeTimeReduce )
				SendServerResponseMsg(0, SRVAL_GAMEMASTER_MAKETIMEREDUCE, 1, 0, serverid);
			else
				SendServerResponseMsg(0, SRVAL_GAMEMASTER_MAKETIMEREDUCE, 0, 0, serverid);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	// 아이템을 만든다. 
	else if(_tcscmp(order, TEXT("cin")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siItemUnique;
			SI32 num = 0;

			_stscanf(&pstring[1], TEXT("%s %d %d %d"), order, &charunique, &siItemUnique, &num);

			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;
			if(siItemUnique < 0 )	return	FALSE	;


			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(num <= 0)	num = 1;
			if(num > MAX_ITEM_PILE_NUMBER)	num = MAX_ITEM_PILE_NUMBER;

			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( siItemUnique );
			if (ref <= 0)	return FALSE;

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)		return FALSE;

			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
			pclCM->GiveItem(id, siItemUnique, num, GIVEITEM_MODE_PERSON, INPUTITEMPERSON_OPTION_MASTER,usedate, pclchar->GetID() );

		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	// 레벨 20이상 모든 유저들에게 아이템을 제공한다. 
	else if(_tcscmp(order, TEXT("ciadult")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			
			TCHAR itemname[128];
			ZeroMemory(itemname, 128);

			SI32 num = 0;
			_stscanf(&pstring[1], TEXT("%s %s %d"), order, itemname, &num);


			if(num <= 0) return FALSE;

			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;
			if(_tcslen( itemname ) <= 0)	return	FALSE	;

			ConvertUnderLineToSpace( itemname, 128 );

			if(num <= 0)	num = 1;
			if(num >1000)	num = 1000;
			
			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			//cyj pclCM->GiveItem(0, itemunique, num, GIVEITEM_MODE_ADULT, INPUTITEMPERSON_OPTION_MASTER,0);
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
			if (ref <= 0)	return FALSE;

			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
			pclCM->GiveItem(0, itemunique, num, GIVEITEM_MODE_ADULT, INPUTITEMPERSON_OPTION_MASTER,usedate);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
		
	}
	// 접속중인 모든 유저들에게 아이템을 제공한다. 
	else if(_tcscmp(order, TEXT("ciall")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			
			TCHAR itemname[128];
			ZeroMemory(itemname, 128);
			SI32 num = 0;
			_stscanf(&pstring[1], TEXT("%s %s %d"), order, itemname, &num);

			if(num <= 0)
				return FALSE;
			// 보완코드 추가
			if(_tcslen( itemname ) <= 0)	return	FALSE	;

			ConvertUnderLineToSpace( itemname , 128);

			if(num <= 0)	num = 1;
			if(num >1000)	num = 1000;
			
			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
			if (ref <= 0)	return FALSE;

			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
			//cyj pclCM->GiveItem(0, itemunique, num, GIVEITEM_MODE_ALL, INPUTITEMPERSON_OPTION_MASTER,0);
			pclCM->GiveItem(0, itemunique, num, GIVEITEM_MODE_ALL, INPUTITEMPERSON_OPTION_MASTER, usedate);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
		
	}
	// 입력된 레벨 이상 모든 유저들에게 아이템을 제공한다. 
	else if(_tcscmp(order, TEXT("cilevel")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR itemname[128];
			ZeroMemory(itemname, 128);
			SI32 num = 0;
			SI32 level = 0;
			_stscanf(&pstring[1], TEXT("%s %s %d %d"), order, itemname, &num, &level);

			if(num <= 0 || level <=0)
				return FALSE;

			// 보완코드 추가
			if(_tcslen( itemname ) <= 0)	return	FALSE	;

			ConvertUnderLineToSpace( itemname, 128 );

			if(num <= 0)	num = 1;
			if(num >1000)	num = 1000;

			if(level <= 0)					level = 0;
			if(level > MAX_CHARACTER_LEVEL)	level = MAX_CHARACTER_LEVEL;

			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
			if (ref <= 0)	return FALSE;

			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
			pclCM->GiveItem(0, itemunique, num, GIVEITEM_MODE_MINLEVEL, INPUTITEMPERSON_OPTION_MASTER, usedate, 0, level);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	//KHY -0702- 접속중인 모든 유저들에게, 예약한 시간에 아이템을 제공한다. 
	else if(_tcscmp(order, TEXT("citimeall")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			
			TCHAR itemname[128];
			ZeroMemory(itemname, 128);
			SI32 num = 0;
			UI16 time_hour = 0;
			UI16 time_min = 0;
			
			_stscanf(&pstring[1], TEXT("%s %s %d %d %d"), order,itemname,&time_hour,&time_min, &num);

			if(num <= 0 || time_hour< 0 || time_min < 0)
				return FALSE;
			// 보완코드 추가			
			if(_tcslen( itemname ) <= 0)	return	FALSE	;

			ConvertUnderLineToSpace( itemname, 128 );

			if(num != 0 )
			{
				if(num <= 0)	num = 1;
				if(num >1000)	num = 1000;

				cltServer* pclserver = (cltServer*)pclClient;

				SI16 hour = (SI16)pclserver->sTime.wHour;
				SI16 minute = (SI16)pclserver->sTime.wMinute;

				// 현재시간보다 작으면 실패.
				if(time_hour == hour )
				{
					if(time_min <= minute)
						return FALSE;
				}
				else if(time_hour <= hour )
						return FALSE;
				
				// 아이템 이름을 근거로 Unique를 구한다.
				SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
				if(itemunique <= 0)
				{
					itemunique = _ttoi( itemname );
					if(itemunique <= 0)return FALSE;
				}
				SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
				if (ref <= 0)	return FALSE;

				UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;

				pclGMManager->giveItemTime(time_hour,time_min,itemunique,num,usedate);

				pclGMManager->timeSwitch = TRUE;
			}
			//갯수가 0개일때 예약취소 
			else
				pclGMManager->timeSwitch = FALSE;
			//cyj pclCM->GiveItem(0, itemunique, num, GIVEITEM_MODE_ALL, INPUTITEMPERSON_OPTION_MASTER,0);
			//pclCM->GiveItem(0, itemunique, num, GIVEITEM_MODE_ALL, INPUTITEMPERSON_OPTION_MASTER, usedate);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
		
	}
	// 접속중인 모든 유저들중 마을을 체크하여 아이템을 제공한다.
	else if(_tcscmp(order, TEXT("civillage")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{

			TCHAR itemname[128] = { '\0', };
			SI32 num = 0;
			SI32 villageunique = 0;
			TCHAR villagename[128] = { '\0', };

			_stscanf(&pstring[1], TEXT("%s %s %d %s"), order, itemname, &num, villagename);

			if(num <= 0)
				return FALSE;

			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;
			if(_tcslen( itemname ) <= 0)	return	FALSE	;
			if (_tcslen( villagename) <= 0 ) return FALSE ;

			ConvertUnderLineToSpace( itemname, 128 );

			if(num <= 0)	num = 1;
			if(num >1000)	num = 1000;

			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			// 마을 이름에 번호를 입력한 경우
			SI32 input = _tstoi(villagename);
			if( villagename[0] >= '0' && villagename[0] <= '9' && input > 0 )
			{
				villageunique = input;
			}
			else
			{
				if( _tcslen(villagename) <= 0 )
				{
					// 마을 주민이기만 하면, 아이템 지급
					villageunique = 0;
				}
				else
				{
					// 특정 마을 주민에게만 아이템 지급
					SI32 ref = pclVillageManager->FindVillageUniqueFromName(villagename);
					if( ref <= 0 )		return false;

					villageunique = ref;
				}
			}

			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
			pclCM->GiveItem(0, itemunique, num, GIVEITEM_MODE_VILLAGER, INPUTITEMPERSON_OPTION_MASTER, usedate, 0, 0, villageunique);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}

	
	// 아이템의 내구도를 조정한다. 
	else if(_tcscmp(order, TEXT("maxdurability")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			
			TCHAR itemname[128];
			ZeroMemory(itemname, 128);
			SI32 dur = 0;
			_stscanf(&pstring[1], TEXT("%s %d %s %d"), order, &charunique, itemname, &dur);

			if(dur <= 0)
				return FALSE;
			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;
			if(_tcslen( itemname ) <= 0)	return	FALSE	;

			ConvertUnderLineToSpace( itemname, 128 );

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			SI32 siMaxDurability = (SI32)pclClient->GetGlobalValue( TEXT("GV_MaxDurability") );
			if(dur <= 0)	dur = 1;
			if(dur > siMaxDurability)	dur = siMaxDurability;
			
			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;



			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;
			
			cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
			// 해당 아이템을 찾는다. 
			SI32 pos = pclchar->pclCI->clCharItem.FindItemInv(itemunique);
			if(pos >= 0)
			{
				// 내구도가 있는 아이템이어야 한다. 
				if(pclchar->pclCI->clCharItem.clItem[pos].CanPile(pclItemManager) == true)return FALSE;

				cltItem clitem;

				clitem.Set( &pclchar->pclCI->clCharItem.clItem[pos]);
				clitem.SetMaxDurability(pclItemManager, dur);
				if(clitem.GetDurability() > clitem.GetMaxDurability(pclItemManager))
				{
					clitem.SetDurability(pclItemManager, dur);
				}

				// DBManager에게 내구도를 소모할 것을 요청한다. 
				sDBRequest_WasteDurability clMsg(pclchar->GetID(), pclchar->pclCI->GetPersonID(), pos, &clitem);
				SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				cltGameMsgResponse_PersonItemInfo clinfo(pos, PERSONITEMINFO_OPTION_WASTE, 0, 0, &clitem, 0);
				cltMsg clMsg2(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg2);



			}

			
			
			//pclchar->GiveItem(itemunique, num, INPUTITEMPERSON_OPTION_MASTER);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
		
	}

	//  NPC를 생성한다. 
	else if(_tcscmp(order, TEXT("cm")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			
			TCHAR npcname[128];
			SI32 num		= 1;
			//TCHAR xbox[128];
			//SI32 xboxnum	= 0;
			SI32 level = 1;
			
			memset(npcname, 0, sizeof(npcname));
			//memset(xbox, 0, sizeof(xbox));
			_stscanf(&pstring[1], TEXT("%s %d %s %d %d"), order, &charunique, npcname, &num, &level);

			if(num <= 0 || level <= 0)
				return FALSE;
			// 보완코드 추가
			if(charunique < 0 )	return	FALSE	;
			if(_tcslen( npcname ) <= 0)	return	FALSE	;

			ConvertUnderLineToSpace( npcname, 128 );

			// GM권한 100, 101은 charunique 0만 사용하도록
			// GM권한 90이상  은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_PLAYERMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(num <= 0)	num = 1;
			if(num > 50)	num = 50;

			// NPC이름을 근거로 NPC의 Kind를 찾는다. 
			SI32 kind = pclKindInfoSet->FindKindFromName(npcname);
			if(kind ==0)return FALSE;
			
			
			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;
			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			SI32 CreatedCharNum = 0;
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
			SI32 count = 0;
			

			while(count < num)
			{
				count++;

				SI32 x, y;
				if(pclchar->pclMap->FindEmptyAreaForCharLocate(kind, pclchar->GetX(), pclchar->GetY(), 20, &x, &y, true) == false)
				{
					continue;
				}

				pclchar->SetInitProtectSwitch();

				cltInnerStatusInfo clinnerstatus;
				clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

				SI32 siSummonID = SetNPCChar(kind, VILLAGE_NONE, NULL, level, 0, x, y, pclchar->GetMapIndex(), NPCITEMMODE_GM, &clinnerstatus );
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if(_tcscmp(order, TEXT("cminfo")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{

			TCHAR npcname[128];
			SI32 num		= 1;
			SI32 level		= 1;

			memset(npcname, 0, sizeof(npcname));
			_stscanf(&pstring[1], TEXT("%s %d %s %d %d"), order, &charunique, npcname, &num, &level);

			if(num <= 0 || level <= 0)
				return FALSE;

			ConvertUnderLineToSpace( npcname , 128);

			// GM권한 100, 101은 charunique 0만 사용하도록
			// GM권한 90이상  은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_PLAYERMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(num <= 0)	num = 1;
			if(num > 50)	num = 50;

			// NPC이름을 근거로 NPC의 Kind를 찾는다. 
			SI32 kind = pclKindInfoSet->FindKindFromName(npcname);
			if(kind ==0)return FALSE;


			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;
			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			SI32 CreatedCharNum = 0;
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
			SI32 count = 0;


			while(count < num)
			{
				count++;

				SI32 x, y;
				if(pclchar->pclMap->FindEmptyAreaForCharLocate(kind, pclchar->GetX(), pclchar->GetY(), 20, &x, &y, true) == false)
				{
					continue;
				}

				pclchar->SetInitProtectSwitch();

				cltInnerStatusInfo clinnerstatus;
				clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

				SI32 siSummonID = SetNPCChar(kind, VILLAGE_NONE, NULL, level, 0, x, y, pclchar->GetMapIndex(), NPCITEMMODE_GM, &clinnerstatus );
				//SendServerResponseMsg(pclchar->GetCharID(), SRVAL_SUCCESS_CMINFO,  siSummonID, 0, pclchar->GetCharID());
                if(siSummonID > 0)
				{
					cltCharServer* pclnewchar = (cltCharServer*)pclCM->CR[siSummonID];
					TCHAR filename[256];
					wsprintf(filename, "charinfo.txt");
					FILE* fp = fopen(filename, "a");
					if(fp)
					{
						fprintf(fp, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", level, npcname, 
							pclnewchar->clPB.GetTotalMinDamage(),
							pclnewchar->clPB.GetTotalMaxDamage(),
							pclnewchar->clPB.GetCriticalRate(),
							pclnewchar->clPB.GetTotalAC(),
							pclnewchar->clPB.pclIP->GetStr(),
							pclnewchar->clPB.pclIP->GetDex(),
							pclnewchar->clPB.pclIP->GetVit(),
							pclnewchar->clPB.pclIP->GetMag(),
							pclnewchar->clPB.pclIP->GetCurExp());
						fclose(fp);
					}

				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 멀티 유저 체크
	else if(_tcscmp(order, TEXT("checkmulti")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclCheckProcessManager->SendOrderToClient( pclchar->pclCI->GetPersonID(), CHECKPROCESS_TYPE_MULTICLIENT );
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 메크로 & 프로세스 체크
	else if(_tcscmp(order, TEXT("checkmacro")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR szModuleName[64] = { '\0', };

			_stscanf( &pstring[1], TEXT("%s %s"), order, szModuleName );

			// 모듈 이름이 없을때는 macrolist에서
			if ( _tcscmp(szModuleName, TEXT("")) == 0 )
			{
				pclCheckProcessManager->SendOrderToClient( pclchar->pclCI->GetPersonID(), CHECKPROCESS_TYPE_MACRO );
			}
			// 있을때는 설정한 이름으로
			else
			{
				pclCheckProcessManager->SendOrderToClient( pclchar->pclCI->GetPersonID(), CHECKPROCESS_TYPE_MODULENAME, szModuleName );
			}

		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}

	//  공지를 띄운다.
	else if(_tcscmp(order, TEXT("delperson")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[serverid];
			if(pclchar != NULL)
			{
				sDBRequest_PersonTotalInfoDelete clMsg( serverid, pclchar->pclCI->clBI.siIDNum, pclchar->pclCI->clBI.GetIndex(), pclchar->pclCI->GetPersonID(), TEXT(""), pclchar->GetSession()->GetIP() );
				pclGameDBProcess->SendMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg);

				pclchar->bCloseSocketSwitch = true;
				pclchar->dwCloseSocketTimer = 5;
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	//  공지를 띄운다.
	else if(_tcscmp(order, TEXT("nt")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR msgtext[1024];
			TCHAR* pnotice = &pstring[1];
			StringCchCopy(msgtext, 1024, &pnotice[3] );

			TCHAR* pText = GetTxtFromMgr(5482); 

			// 운영자가 하는것이라면 채팅창에도 나타나게한다.
			//KHY - 0129 - 하단 채팅창에 공지 출력여부를 결정한다.
			cltGameMsgResponse_Notice clNotice(pText, msgtext, true);
			cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

			pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);			
			//SendNoticeMsg(-1, 0, 0, msgtext);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 타이머 공지. 
	else if(_tcscmp(order, TEXT("nc")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			/*TCHAR szMsg[1024]={0,};
			TCHAR szMsgtext[1024]={0,};
			TCHAR szInterval[1024]={0,} ;

			TCHAR* pnotice = &pstring[1];
			StringCchCopy(szMsg, 1024, &pnotice[3] );
			
			_stscanf(szMsg, TEXT("%s"),szInterval);
			
			SI32 size = _tcslen(szInterval);
			if(size < 1024-1)
			{
				StringCchCopy(szMsgtext, 1024, &szMsg[size] );
			}*/
			TCHAR szMsg[1024]={0,};
			TCHAR szMsgtext[1024]={0,};
			TCHAR szInterval[1024]={0,} ;
			TCHAR szCount[1024]={0,} ;

			TCHAR* pnotice = &pstring[1];
			StringCchCopy(szMsg, 1024, &pnotice[3] );

			_stscanf(szMsg, TEXT("%s %s"),szInterval , szCount );

			SI32 size = _tcslen(szInterval) + _tcslen(szCount)+1 ;
			if(size < 1024-1)
			{
				StringCchCopy(szMsgtext, 1024, &szMsg[size] );
			}

			SI32 siInterval;
			SI32 siCount ;

			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &siInterval, &siCount);

			//cyj nc명령에서 분으로 넣도록 수정
			DWORD dwinterval = siInterval * 1200;
			
			if( (dwinterval <= 0) || (!_tcscmp(szMsgtext,TEXT(""))) || (siCount <= 0) )
			{
				return FALSE;
			}

			pclNoticeManager->Add(dwinterval, szMsgtext, siCount);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	// 타이머 공지 삭제. 
	else if(_tcscmp(order, TEXT("ncdel")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			
			pclNoticeManager->DelAllNotice();
		}
		else
		{
			// 클라이언트로 운영자 권한이 없음을 통보한다. 
			cltMsg clMsg(GAMEMSG_RESPONSE_GAMEMASTERLEVEL, 0);
			pclCM->CR[serverid]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
	// 로그인 공지 on./off
	else if(_tcscmp(order, TEXT("ntlogin")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclNoticeManager->OnOffLogInNotice();
		}
		else
		{
			// 클라이언트로 운영자 권한이 없음을 통보한다. 
			cltMsg clMsg(GAMEMSG_RESPONSE_GAMEMASTERLEVEL, 0);
			pclCM->CR[serverid]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
		
	// 투표 초기화
	else if(_tcscmp(order, TEXT("voteinit")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 personid = pclCM->CR[serverid]->pclCI->GetPersonID();

			if ( personid > 0 )
			{
				sDBRequest_VoteInit clMsg( personid, serverid );
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
		}
		else
		{
			// 클라이언트로 운영자 권한이 없음을 통보한다. 
			cltMsg clMsg(GAMEMSG_RESPONSE_GAMEMASTERLEVEL, 0);
			pclCM->CR[serverid]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	// 투표 실시 
	else if(_tcscmp(order, TEXT("votestart")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclVoteMgr->SetVoteStatus(true);
		}
		else
		{
			// 클라이언트로 운영자 권한이 없음을 통보한다. 
			cltMsg clMsg(GAMEMSG_RESPONSE_GAMEMASTERLEVEL, 0);
			pclCM->CR[serverid]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	// 투표 실시 중단 
	else if(_tcscmp(order, TEXT("voteend")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclVoteMgr->SetVoteStatus(false);
		}
		else
		{
			// 클라이언트로 운영자 권한이 없음을 통보한다. 
			cltMsg clMsg(GAMEMSG_RESPONSE_GAMEMASTERLEVEL, 0);
			pclCM->CR[serverid]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	// [영훈] 투표 개표 시작
	else if (_tcscmp(order, TEXT("votecount")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if(pclCM->CR[serverid])
			{
				sDBRequest_VoteKingAggregate clMsg( serverid, VOTE_STATUS_TYPE_ONAIR );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg);
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}

	
	//  모두 강퇴 시킨다. (운영자 포함) 
	else if(_tcscmp(order, TEXT("out")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			// 정보를 DB에 모두 저장한다. 
			pclCM->CloseAllSocket();
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 사용자의 로그인을 제한한다. 
	else if(_tcscmp(order, TEXT("block")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclFrontAuthProcess->m_refreshInfo.state = 2;
		}
		else
		{
			bnotenoughmasterlevel = true;
		}


	}
	// 사용자의 로그인을 허용한다. 
	else if(_tcscmp(order, TEXT("unblock")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclFrontAuthProcess->m_refreshInfo.state = 0;
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	// 특정 사용자의 로그인을 제한한다. 
	else if(_tcscmp(order, TEXT("userblock")) == 0)
	{
		TCHAR personname[128];
		ZeroMemory(personname, 128);
		SI32 day;
		_stscanf(&pstring[1], TEXT("%s %s %d"), order, personname, &day);

		if(day < 0 || day > 365)
			return FALSE;

		ConvertUnderLineToSpace( personname, 128 );

		TCHAR temp[ MAX_BLOCKREASON_SIZE ];
		StringCchPrintf(temp, MAX_BLOCKREASON_SIZE, TEXT("%s %s %d"), order, personname, day);
		SI32 startpos = lstrlen( temp );
		TCHAR* pstartpos = &pstring[1] + startpos;

		TCHAR text[1024];
		MStrCpy( text, pstartpos, MAX_BLOCKREASON_SIZE );
		text[ MAX_BLOCKREASON_SIZE - 1 ] = '\0';


		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 blockmode = 1;
			if(day == 0)blockmode = 2;	

			sDBRequest_BlockUser clMsg(serverid, blockmode, personname, day, text);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 특정 사용자의 로그인을 허용한다. 
	else if(_tcscmp(order, TEXT("userunblock")) == 0)
	{
		TCHAR personname[128];
		_stscanf(&pstring[1], TEXT("%s %s"), order, personname);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR* pText = GetTxtFromMgr(5409);
			sDBRequest_BlockUser clMsg(serverid, 0, personname, 0, pText);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
		
	}
	// 특정 사용자에게 경고를 누적한다.
	else if( _tcscmp(order, TEXT("warn")) == 0 )
	{
		TCHAR personname[128];
		SI32 day;
		_stscanf(&pstring[1], TEXT("%s %s"), order, personname);

		TCHAR temp[ MAX_BLOCKREASON_SIZE ];
		StringCchPrintf(temp, MAX_BLOCKREASON_SIZE, TEXT("%s %s"), order, personname);
		SI32 startpos = lstrlen( temp );
		TCHAR* pstartpos = &pstring[1] + startpos;

		TCHAR text[1024];
		MStrCpy( text, pstartpos, MAX_BLOCKREASON_SIZE );
		text[ MAX_BLOCKREASON_SIZE - 1 ] = '\0';


		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 blockmode = sDBRequest_BlockUser::WARNING_TO_BLOCKING;
			day = 0;

			sDBRequest_BlockUser clMsg(serverid, blockmode, personname, day, text);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 특정 사용자에게 돈을 준다. 
	else if(_tcscmp(order, TEXT("money")) == 0)
	{
		SI32 charunique;
		GMONEY money;
		_stscanf(&pstring[1], TEXT("%s %d %I64d"), order, &charunique, &money);

		// GM권한 100, 101은 charunique 0만 사용하도록
		if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
		{
			if (charunique != 0)
				return FALSE;
		}

		if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

		SI32 id;
		if(charunique == 0)	{	id = serverid;		}
		else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
		if(id == 0)return FALSE;

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if(money < 0)money = 0;
			if(money > 5000000000)money = 5000000000;

			SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
			if(personid)
			{
				sDBRequest_ChangeMoney clMsg(id, personid, CHANGE_MONEY_REASON_GAMEMASTER, money);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
				
				// Log에 기록한다. 
				pclLog->FilePrint(TEXT("GameMaster.txt"), TEXT("돈 입금 대상:[%s], 금액:[%d]"), pclCM->CR[id]->GetName(), money);
				
				//  돈 입금 성공 여부를 알려준다
				{
					cltGameMsgResponse_GMGiveMoney clinfo((TCHAR*)pclCM->CR[id]->GetName(), money);
					cltMsg clMsg(GAMEMSG_RESPONSE_GMGIVEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
					pclCM->CR[serverid]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	// 특정 사용자의 채팅을 제한한다. 
	else if(_tcscmp(order, TEXT("mute")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 charunique = 0;
			SI32 timebyminute = 0;
			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &charunique, &timebyminute);

			SI32 id = pclCM->GetIDFromCharUnique(charunique);
			if( pclCM->IsValidID(id) == false )			return FALSE;
			cltCharServer* pclchar = pclCM->GetCharServer(id);
			if( pclchar == NULL	)						return FALSE;

			// 입력하지 않으면 기본 30분
			if( timebyminute <= 0 )		timebyminute = 30;

			// 채팅블록이 걸려있는 캐릭터이면 해제된다.
			if( pclchar->m_stMuteTime.wYear > 0 )
			{
				ZeroMemory( &pclchar->m_stMuteTime, sizeof(pclchar->m_stMuteTime) );
			}
			else
			{
				SYSTEMTIME stMuteTime;		ZeroMemory(&stMuteTime, sizeof(stMuteTime));
				TimeAddMinute(&sTime, timebyminute, &stMuteTime);
				memcpy(&pclchar->m_stMuteTime, &stMuteTime, sizeof(pclchar->m_stMuteTime));
			}

			sDBRequest_SetMuteTime clMsg(serverid, pclchar->pclCI->GetPersonID(), &pclchar->m_stMuteTime);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 채팅이 블록된 사용자들을 전송한다.
	else if(_tcscmp(order, TEXT("mutelist")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			_stscanf(&pstring[1], TEXT("%s"), order);

			cltGameResponse_MuteList clNames;

			SI32	siCount = 0;
			for( SI32 i=1; i<MAX_PERSON_NUMBER; i++ )
			{
				if( pclCM->IsValidID(i) == false )		continue;
				cltCharServer* pclchar = pclCM->GetCharServer(i);
				if( pclchar == NULL )					continue;

				if( siCount >= 100 )					continue;
				if( pclchar->m_stMuteTime.wYear > 0 )
				{
					clNames.m_szPersonNames[siCount] = pclchar->GetName();
					siCount++;
				}
			}
			clNames.m_siCount = siCount;

			cltCharServer* pclcharmaster = pclCM->GetCharServer(serverid);
			if(pclcharmaster)
			{
				cltMsg clMsg(GAMEMSG_RESPONSE_MUTELIST, sizeof(clNames), (BYTE*)&clNames);
				pclcharmaster->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 사용자 정보를 확보한다. 
	else if(_tcscmp(order, TEXT("user")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR personname[128];
			_stscanf(&pstring[1], TEXT("%s %s"), order, personname);
			
			bool bconnectswitch = false;
			
			if(pclCM->GetPersonIDFromName(personname))
			{
				bconnectswitch = true;
			}
			
			sDBRequest_UserCharInfo clMsg(serverid, personname, bconnectswitch, pclchar->pclCI->GetPersonID());
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 사용자 정보를 확보한다. 
	else if(_tcscmp(order, TEXT("useritem")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR personname[128];
			_stscanf(&pstring[1], TEXT("%s %s"), order, personname);

			sDBRequest_UserCharItemInfo clMsg( serverid, personname, pclchar->pclCI->GetPersonID() );
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}

	// 해당사용자를 강제 접속시킨다.
	else if(_tcscmp(order, TEXT("userkick")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR personname[128];
			_stscanf(&pstring[1], TEXT("%s %s"), order, personname);
			bool bconnectswitch = false;

			SI32 id = pclCM->GetIDFromName(personname);			
			if ( pclCM->IsValidID( id ) )
			{
				bconnectswitch = true;
				cltCharServer* pclnewchar = (cltCharServer*)(pclCM->CR[id]);
				pclnewchar->bCloseSocketSwitch = true;
				pclnewchar->dwCloseSocketTimer = 5;
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if(_tcscmp(order, TEXT("account") ) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR accountid[128];
			_stscanf(&pstring[1], TEXT("%s %s"), order, accountid);
			
			sDBRequest_AccountInfo clMsg( serverid, accountid );
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 사용자를 소환한다. 
	else if(_tcscmp(order, TEXT("recall")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel || bkingswitch || minister5switch)
		{
			TCHAR personname[128];
			_stscanf(&pstring[1], TEXT("%s %s"), order, personname);
			
			bool bconnectswitch = false;

			// 인스턴스 던전에선 운영자만 실행할 수 있다.
			bool bInstanceDungeonExecute = true;
			if( pclClient->pclMapManager->IsInstanceMap(pclClient->pclCM->CR[serverid]->GetMapIndex()) )
			{
				if( pclClient->pclCM->CR[serverid]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
				{
					bInstanceDungeonExecute = false;
				}
			}
			
			SI32 recallid = pclCM->GetIDFromName(personname);
			if(bInstanceDungeonExecute && recallid)
			{
				// [2007.12.31 - 김영훈 추가 : 군주나 5대신이 운영자를 소환했을때 소환이 안되도록 막는다(임시)]
				if ( (bkingswitch || minister5switch) 
					&& (pclCM->CR[recallid]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL) )
				{
					return FALSE;
				}

				SI32 mapindex			= pclCM->CR[serverid]->GetMapIndex();
				SI32 siPersonMapindex	= pclCM->CR[recallid]->GetMapIndex();
				bool bfailswitch		= false;
				// 운영자에 의해 소환되는 게 아니라면, 
				if(gamemasterlevel < BASIC_GAMEMASTER_LEVEL+1)
				{	
					//감옥에 갇혀 있는 상태라면.. 
					if(pclCM->CR[recallid]->clInnerStatus.IsStatus(INNERSTATUS_PRISON))
					{
						bfailswitch = true;
					}
					// 영자나라에 있으면 불가능
					if ( pclCM->CR[recallid]->GetMapIndex() == MAPINDEX_MASTERWORLD )
					{
						bfailswitch = true;
					}
					/*
					if ( pclCM->CR[serverid]->GetMapIndex() >= MAPINDEX_DAEMADO || 
						 pclCM->CR[serverid]->GetMapIndex() == MAPINDEX_DAEMADOGATE )
					{
						bfailswitch = true ;
					}
					*/

					// 출입 가능 레벨이 안된다면 실패
					//if ( pclMapManager->GetLevelLimit(mapindex) > pclCM->CR[recallid]->pclCI->clIP.GetLevel() )
					if( pclMapManager->IsEnterLevel(mapindex, pclCM->CR[recallid]->pclCI->clIP.GetLevel()) == false )
					{
						bfailswitch = true;
					}
					// 군주나 대신인 경우
					if ( bkingswitch || minister5switch )
					{
						// PK 맵이라면 실패
						if ( pclMapManager->IsPkMap(mapindex) == true )
						{
							bfailswitch = true;
						}
						// [영훈] 개인상점을 개설한 캐릭터가 개인상점 지역에서 개인상점이 아닌 지역으로 이동된다면 리콜불가능
						else if ( (pclMapManager->IsCanMarketOpenMap( siPersonMapindex ) == true)
							&& (pclMapManager->IsCanMarketOpenMap( mapindex ) == false)
							)
						{
							if ( (pclCM->CR[recallid]->bPersonalShopSwitch == true)
								|| (pclCM->CR[recallid]->bPremiumPersonalShop == true)
								)
							{
								bfailswitch = true;
							}
						}
						// [영훈] 워프 불가지역으로는 리콜 못함
						else if ( pclMapManager->IsBlockWarp( mapindex ) == true )
						{
							bfailswitch = true;
						}
						// [영훈] 인던으로 리콜 못함
						else if ( pclMapManager->IsInstanceMap( mapindex ) == true )
						{
							bfailswitch = true;
						}
					}	// if ( bkingswitch || minister5switch ) 종료
				}

				if(bfailswitch == false)
				{
					
					OrderStop(recallid, BY_COMPUTER);
					
					cltWarpInfo clWarpInfo(1, mapindex, false, false, pclCM->CR[serverid]->GetX(), pclCM->CR[serverid]->GetY(), -1, -1);
					pclCM->CR[recallid]->SetWarpInfo(&clWarpInfo, 1700);
					
					// 군주/ 형조판서가 실행한 것이라면 군주복의 내구도를 소모시킨다. 
					if(bkingswitch || minister5switch)
					{
						//pclchar->DecreaseEquipDur(PERSONITEM_DRESS, 5);
						
					}
				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 시세 정보를 설정한다. 
	else if(_tcscmp(order, TEXT("setprice")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR itemname[128];
			ZeroMemory(itemname, 128);
			SI32 price;
			_stscanf(&pstring[1], TEXT("%s %s %d"), order, itemname, &price);

			if(price <= 0)
				return FALSE;

			ConvertUnderLineToSpace( itemname, 128 );

			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			if(price <= 0 || price > 100000000)return FALSE;

			// 가격을 설정한다. 
			cltItemPriceUnit* pclprice = pclItemManager->pclItemPrice->GetPrice(pclItemManager, itemunique);
			if(pclprice == NULL)return FALSE;

			if(pclprice->siItemNum < 100)
			{
				pclprice->siItemNum = 100;
			}

			pclItemManager->pclItemPrice->SetPrice(pclItemManager, itemunique, price, pclprice->siItemNum, true);

			// DB에 저장한다. 
			const TCHAR* pszItemName = pclClient->pclItemManager->GetName(pclprice->siUnique);
			sDBRequest_SetItemPrice clMsg(pclprice->siUnique, pclprice->siPrice, pclprice->fPrice, pclprice->siItemNum, pszItemName);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 소환수 스테미너를 설정한다
	else if(_tcscmp(order, TEXT("setstamina")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siCharUnique	= 0;
			SI32 siStamina		= 0;

			_stscanf( &pstring[1], TEXT("%s %d %d"), order, &siCharUnique, &siStamina );

			SI32 siCharID = 0;
			if ( 0 == siCharUnique )
			{
				siCharID = serverid;
			}
			else
			{
				siCharID = pclCM->GetIDFromCharUnique( siCharUnique );
			}
			
			if ( false == pclCM->IsValidID(siCharID) )
			{
				return FALSE;
			}

			cltCharServer* pclchar = pclCM->GetCharServer( siCharID );
			if ( NULL == pclchar )
			{
				return FALSE;
			}

			SI32 siSummonID = pclchar->GetSummonID();
			if ( false == pclCM->IsValidID(siSummonID) )
			{
				return FALSE;
			}

			cltCharServer* pclSummonChar = pclCM->GetCharServer( siSummonID );
			if ( false == pclSummonChar )
			{
				return FALSE;
			}

			SI32 siSummonSkill	= pclchar->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_SUMMON, 0, 0, &pclchar->pclCI->clCharItem.clItem[0], siCharID );
			
			SI32 siMaxStamana	= GetMaxStaminaValue( siSummonSkill, pclSummonChar->GetKind() );

			// 스테미너 최대값은 설정 가능한 최대값 이상일 수 없다 최소값은 1
			if ( siMaxStamana < siStamina )
			{
				siStamina = siMaxStamana;
			}
			else if ( 0 >= siStamina )
			{
				siStamina = 1;
			}
			
			// 스테미너를 설정해주고
			pclSummonChar->clPB.pclIP->SetLuk( siStamina );
			pclSummonChar->clPB.clTotalBA.SetLuk( siStamina );
			// 정보를 업데이트 한다
			pclchar->SetUpdateSwitch( UPDATE_SUMMON, true, 0 );

		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if(_tcscmp(order, TEXT("stock"))== 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR villagename[128];
			SI32 totalstock, personstock;
			GMONEY	price;
			_stscanf(&pstring[1], TEXT("%s %s %d %d %I64d"), order, villagename, &totalstock, &personstock, &price);

			// 마을의 이름을 토대로 MapIndex를 찾는다. 
			SI32 ref = pclVillageManager->FindVillageUniqueFromName(villagename);
			if(ref >=0)
			{
				if (  pclStockDistributionMgr->IsAvailable( ref ) )
				{
					sDBRequest_MaxStockDistributionNum clMsg( ref, totalstock, personstock, price, false, 1, 0 );
					pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
				}
			}
		}
	}
	// 공성 발발. 
	else if(_tcscmp(order, TEXT("villagewar")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{

			bForceStopVillageWarProcess = !bForceStopVillageWarProcess;

			if(bForceStopVillageWarProcess == false)
				SendServerResponseMsg(0, SRVAL_GAMEMASTER_VILLAGEWAR, 1, 0, serverid);
			else
				SendServerResponseMsg(0, SRVAL_GAMEMASTER_VILLAGEWAR, 0, 0, serverid);
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 개인상점 
	else if(_tcscmp(order, TEXT("pshop")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{

			bForcePersonalShopSwitch = !bForcePersonalShopSwitch;
			// 클라이언트로 서버의 응답값을 보낸다. 
			SendServerResponseMsg(0, SRVAL_PERSONALSHOP,  bForcePersonalShopSwitch , 0, serverid);
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	
	// 게임시간 빠르게 가도록 설정
	else if(_tcscmp(order, TEXT("fasttime")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			bForceTimeSwitch = !bForceTimeSwitch;
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 종량제 시스템 시간을 빨리 소모하도록
	else if(_tcscmp(order, TEXT("fastuse")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 charid = 0;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &charunique);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			cltTimeMeterRateInfo* pclTimeMeterRateInfo = &pclClient->pclCM->CR[id]->pclCI->clBI.clTimeMeterRateInfo;
			if(pclTimeMeterRateInfo != NULL)
			{
				if( pclTimeMeterRateInfo->m_siUseTimePerSec == 1 )
					pclTimeMeterRateInfo->m_siUseTimePerSec = 60 * 60;	// 초당 1시간씩 소모한다.
				else
					pclTimeMeterRateInfo->m_siUseTimePerSec = 1;		// 초당 1초씩 소모한다.
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// [영훈] 낚시 이벤트 On/Off
	else if (_tcscmp(order, TEXT("fishingevent")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			bEventToggleSwitch[EVENT_TOGGLE_SWITCH_FISHINGEVENT] = !bEventToggleSwitch[EVENT_TOGGLE_SWITCH_FISHINGEVENT];

			SI16 siPara2 = (bEventToggleSwitch[EVENT_TOGGLE_SWITCH_FISHINGEVENT]==true?1:0);
			SendServerResponseMsg( 0, SRVAL_NOTICE_EVENT_TOGGLE_SWITCH, EVENT_TOGGLE_SWITCH_FISHINGEVENT, siPara2, serverid );

			SendServerInfo(0);
			
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// [영훈] 시스템 자동구매 On/Off
	else if (_tcscmp(order, TEXT("systembuy")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			bEventToggleSwitch[EVENT_TOGGLE_SWITCH_SYSTEMBUY] = !bEventToggleSwitch[EVENT_TOGGLE_SWITCH_SYSTEMBUY];

			SI16 siPara2 = (bEventToggleSwitch[EVENT_TOGGLE_SWITCH_SYSTEMBUY]==true?1:0);
			SendServerResponseMsg( 0, SRVAL_NOTICE_EVENT_TOGGLE_SWITCH, EVENT_TOGGLE_SWITCH_SYSTEMBUY, siPara2, serverid );

			SendServerInfo(0);

		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 무적 모드 
	else if(_tcscmp(order, TEXT("god")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			_stscanf(&pstring[1], TEXT("%s %d"), order, &charunique);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}
			
			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;
			
			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;
			
			pclCM->CR[id]->bGodMode = !pclCM->CR[id]->bGodMode;

			if( pclCM->CR[id]->pclCI->clBI.GetKind() == GetUniqueFromHash(TEXT("KIND_BERSERKER")) )
			{
				pclCM->CR[id]->bGodMode =  true;
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 무적 모드 
	else if(_tcscmp(order, TEXT("onekill")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			_stscanf(&pstring[1], TEXT("%s %d"), order, &charunique);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			pclCM->CR[id]->bOneKill = !pclCM->CR[id]->bOneKill;

			if( pclCM->CR[id]->pclCI->clBI.GetKind() == GetUniqueFromHash(TEXT("KIND_BERSERKER")) )
			{
				pclCM->CR[id]->bOneKill =  true;
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 라쿤 피하기 이벤트 다음 계의 라운드 시간을 설정 한다
	else if(_tcscmp(order, TEXT("dodgetime")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siNextRoundTime	= 0 ;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &siNextRoundTime);

			if (siNextRoundTime > 0 )
			{
				pclRacoonDodgeEventMgr->SetRoundTime( siNextRoundTime );
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 라쿤 피하기 이벤트 다음 계의 라운드 시간을 설정 한다
	else if(_tcscmp(order, TEXT("dodgenumber")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siStage = 0 	;
			SI32 siMonsternum = 0 ;
			_stscanf(&pstring[1], TEXT("%s %d %d "), order, &siStage , &siMonsternum );


			if ( siStage > 0 && siMonsternum > 0 && siMonsternum < MAXMAKENUM_RACCOONDODGE )
			{
				pclRacoonDodgeEventMgr->SetMonster(siMonsternum, siStage-1 );
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// [지연] 복불복 이벤트를 위한 GM명령어들
	else if(_tcscmp(order, TEXT("bbbshowtype")) == 0)			// 몹 타입 보이기
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siBBBShowType = 0;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &siBBBShowType );

			if ( pclClient->IsCountrySwitch(Switch_BokBulBok) )
			{
				/////////////////////////////////////////////////////////
				if ( 0 == siBBBShowType )
				{
					m_pServer_BBBMgr->ShowMonsterType( serverid, false );
				}
				else if ( 1 == siBBBShowType )
				{
					m_pServer_BBBMgr->ShowMonsterType( serverid, true );
				}

				else
				{
					bnotenoughmasterlevel = true;			
				}
				/////////////////////////////////////////////////////////
			}
		}
		
	}
	else if(_tcscmp(order, TEXT("bbbplaytime")) == 0)			// 플레이 시간 늘리기
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siPlayTime = 0;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &siPlayTime );

			if ( 0 < siPlayTime )
			{
				m_pServer_BBBMgr->SetPlayTime( siPlayTime );
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	else if(_tcscmp(order, TEXT("bbbnoticetime")) == 0)			// 공지 시간 늘리기
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siNoticeTime = 0;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &siNoticeTime );

			if ( 0 < siNoticeTime )
			{
				m_pServer_BBBMgr->Set20mNoticeTime( siNoticeTime );
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	//////////////////////////////////////////////////////////////////////////

	else if(_tcscmp(order, TEXT("gachalock")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			// GM 102 레벨만 사용 가능 
			if(gamemasterlevel != BASIC_GAMEMASTER_LEVEL+2)
			{
				return FALSE;
			}
			if ( bGachaLock == false )
			{
				bGachaLock = true	;
				SendServerResponseCharMsg2(0, SRVAL_GAMEMASTER_GACHALOCK, TEXT("Lock"), "", "", serverid);
			}
			else 
			{
				bGachaLock = false	;
				SendServerResponseCharMsg2(0, SRVAL_GAMEMASTER_GACHALOCK, TEXT("UnLock"), "", "", serverid);
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}	
	else if(_tcscmp(order,TEXT("setgachaRare")) == 0)
	{
		SI32 siitemunique = 0;
		_stscanf(&pstring[1], TEXT("%s %I32d"), order, &siitemunique);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			// GM 102 레벨만 사용 가능 
			if(gamemasterlevel != BASIC_GAMEMASTER_LEVEL+2)
			{
				return FALSE;
			}

			if ( bGachaLock == false )	
			{
				// 가차 lock 이 안걸린 상태에서 시도 했다.
				SendServerResponseCharMsg2(0, SRVAL_GAMEMASTER_GACHA_NOT_LOCK_CHANGE_UNIQUE, "", "", "", serverid);
				return FALSE ; // 가챠가 lock이 안걸려 있다
			}
			else if ( bGachaLock == true )	// lock가 걸려 있을때 
			{
				if ( siitemunique > 0 )
				{
					bool bCorrectItemUnique = false ;
					for ( SI32 siSlot = 0 ; siSlot < MAX_CHANGERARE_PRODUCT; siSlot++)
					{
						if(siSlot < MAX_CHANGERARE_PRODUCT )
						{
							SI32 siGetItemUniqeu = pclGachaManager->m_GacharareTimePeriod[siSlot].ItemUnique	;
							if( siitemunique ==  siGetItemUniqeu )
							{
								bCorrectItemUnique	= true	;
							}
						}
					}
					if ( bCorrectItemUnique == true )
					{
						if(siitemunique > 0 )
						{
							if(pclGachaManager!=NULL)
							{
								pclGachaManager->m_GachaProduct[0][0].ItemUnique = siitemunique	;
								pclGachaManager->m_GachaProduct[1][0].ItemUnique = siitemunique	;
								pclGachaManager->m_GachaProduct[2][0].ItemUnique = siitemunique	;
								siGMChangeGachaRareUnique	=	siitemunique	;
								// 입력 하고 나면 자동으록 lock 해제 
								bGachaLock	=	false	;

							}
						}
					}
					else
						return FALSE	;
				}
				else if ( siitemunique == 0)
				{
					// 기존 리스트에 있는 걸로 셋팅 
					SetGachaRareItem_Time();
				}
			}		
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if(_tcscmp(order, TEXT("goonzuday")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			_stscanf(&pstring[1], TEXT("%s %d"), order);
            
			cltEventTime* pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_Week");
			if(pclEventTime == NULL)			return false;

			if(pclEventTime->m_bEventActivated)
			{
				pclEventTime->SetActivated( false , true);

				pclMonsterAttackEvent->MonsterAttack_EventCount_Reset();		// [지연] : 몬스터 어택 이벤트 횟수 초기화
				pclEventHunting->HuntingEventCount_Reset();		// [지연] : 라티우스 대수렵제 이벤트 횟수 초기화

				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_CollectionItem2_0");
				if(pclEventTime)			pclEventTime->m_bEventActivated = false;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_CollectionItem2_3");
				if(pclEventTime)			pclEventTime->m_bEventActivated = false;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_MakeExpBonus150_6");
				if(pclEventTime)			pclEventTime->m_bEventActivated = false;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_HuntExpBonus150_9");
				if(pclEventTime)			pclEventTime->m_bEventActivated = false;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_MakeExpBonus150_12");
				if(pclEventTime)			pclEventTime->m_bEventActivated = false;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_HuntExpBonus150_15");
				if(pclEventTime)			pclEventTime->m_bEventActivated = false;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_HuntItem2_18");
				if(pclEventTime)			pclEventTime->m_bEventActivated = false;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_MakeExpBonus200_21");
				if(pclEventTime)			pclEventTime->m_bEventActivated = false;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_HuntExpBonus200_21");
				if(pclEventTime)			pclEventTime->m_bEventActivated = false;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_Week_Reward");
				if(pclEventTime)			pclEventTime->m_bEventActivated = false;

				SendServerResponseMsg(0, SRVAL_GAMEMASTER_GOONZUDAY, 0, 0, serverid);
			}
			else
			{
				pclEventTime->SetActivated( true, true );
	
				pclMonsterAttackEvent->MonsterAttack_EventCount_Reset();		// [지연] : 몬스터 어택 이벤트 횟수 초기화
				pclEventHunting->HuntingEventCount_Reset();						// [지연] : 라티우스 대수렵제 이벤트 횟수 초기화

				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_CollectionItem2_0");
				if(pclEventTime)			pclEventTime->m_bEventActivated = true;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_CollectionItem2_3");
				if(pclEventTime)			pclEventTime->m_bEventActivated = true;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_MakeExpBonus150_6");
				if(pclEventTime)			pclEventTime->m_bEventActivated = true;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_HuntExpBonus150_9");
				if(pclEventTime)			pclEventTime->m_bEventActivated = true;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_MakeExpBonus150_12");
				if(pclEventTime)			pclEventTime->m_bEventActivated = true;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_HuntExpBonus150_15");
				if(pclEventTime)			pclEventTime->m_bEventActivated = true;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_HuntItem2_18");
				if(pclEventTime)			pclEventTime->m_bEventActivated = true;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_MakeExpBonus200_21");
				if(pclEventTime)			pclEventTime->m_bEventActivated = true;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_HuntExpBonus200_21");
				if(pclEventTime)			pclEventTime->m_bEventActivated = true;
				pclEventTime = pclEventTimeManager->GetEvent("GoonzuDay_Week_Reward");
				if(pclEventTime)			pclEventTime->m_bEventActivated = true;


				SendServerResponseMsg(0, SRVAL_GAMEMASTER_GOONZUDAY, 1, 0, serverid);			
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// [영훈] 퀘스트 카운트 값 설정
	else if(_tcscmp(order, TEXT("questcount")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siSetCount = 0;
			SI32 siID		= 0;


			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &charunique, &siSetCount);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if ( (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL) && (gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1) )
			{
				if ( charunique != 0 )
				{
					return FALSE;
				}
			}

			if ( (charunique !=0) && (pclCM->IsValidID( charunique ) == FALSE) )
			{
				return FALSE;
			}
			
			if	( 0 == charunique )	
			{	
				siID = serverid;
			}
			else
			{	
				siID = pclCM->GetIDFromCharUnique(charunique);			
			}

			if ( 0 == siID )
			{
				return FALSE;
			}

			if ( siSetCount < 0 )
			{
				siSetCount = 0;
			}

			cltCharServer* pclchar = (cltCharServer*)pclCM->GetChar(siID);
			if ( NULL == pclchar )
			{
				return FALSE;
			}


			pclchar->siQuestCount = siSetCount;

			SendServerResponseMsg(0, SRVAL_GAMEMASTER_QUESTCOUNT, siSetCount, 0, serverid);

		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}

	// questinfo
	else if(_tcscmp(order, TEXT("questinfo")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 questtype;
			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &charunique, &questtype);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}
			
			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;
			
			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			if(questtype < 0 || questtype >= MAX_QUEST_TYPE)return false;

			
			cltGameMsgResponse_QuestuserInfo clinfo(questtype, &pclCM->CR[id]->pclCI->clQuestInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_QUESTUSERINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);
	

		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	
	// 경험이 증가 
	else if(_tcscmp(order, TEXT("incexp")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 increaseExp;
			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &charunique, &increaseExp);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}
			
			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;
			
			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;
			
			// DO SOMETHING
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
			pclchar->IncreaseExp(increaseExp);
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 해당 Level로의 경험이 증가 
	else if(_tcscmp(order, TEXT("setexplevel")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 setlevel;
			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &charunique, &setlevel);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}
			
			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;
			
			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			if(setlevel <= 0 || setlevel > MAX_CHARACTER_LEVEL) return FALSE;
			//====================================================
			// DO SOMETHING
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
			if(setlevel <= pclchar->pclCI->clIP.GetLevel()) return FALSE;	// Level이 떨어질수는 없다.

			// 현재와의 경험치 차이를 구한다.
			SI64 increaseExp = GetRequiredExpForLevel(setlevel) - pclchar->pclCI->clIP.GetCurExp();// SI32->SI64 경험치 오버플로우때문에 수정
			if(increaseExp<=0) return FALSE;

			// 현재와의 레벨 차이를 구한다. - tys
			SI32 increaseLevel = setlevel - pclchar->pclCI->clIP.GetLevel();

			pclchar->IncreaseExp(increaseExp);

			for(SI32 i=0 ; i<increaseLevel ; i++)
				pclchar->IncreaseExp(1);

		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	else if(_tcscmp(order, TEXT("setdurability")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 pos;
			SI32 durability;
			_stscanf(&pstring[1], TEXT("%s %d %d %d"), order, &charunique, &pos, &durability);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if( pos < 0 || pos >= MAX_ITEM_PER_PERSON )						return FALSE;
			if( charunique !=0 && pclCM->IsValidID(charunique)== FALSE )	return FALSE;

			if( durability < 0 )		durability = 0;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			//====================================================
			// DO SOMETHING
			cltCharServer* pclchar = (cltCharServer*)pclCM->GetCharServer( id );
			if( pclchar )
			{
				cltItem* pclitem = &pclchar->pclCI->clCharItem.clItem[ pos ];
				if( pclitem && pclitem->siUnique > 0 )
				{
					if( durability > pclitem->GetMaxDurability( pclItemManager ) )
						durability = pclitem->GetMaxDurability( pclItemManager );

					pclitem->SetDurability( pclItemManager, durability );

					sDBRequest_WasteDurability clMsg( id, pclchar->pclCI->GetPersonID(), pos, pclitem );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

					cltGameMsgResponse_PersonItemInfo clItemInfo( pos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0 );
					cltMsg clMsg2(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clItemInfo), (BYTE*)&clItemInfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg2);
				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	else if(_tcscmp(order, TEXT("setmaxdurability")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 pos;
			SI32 maxdurability;
			_stscanf(&pstring[1], TEXT("%s %d %d %d"), order, &charunique, &pos, &maxdurability);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if( pos < 0 || pos >= MAX_ITEM_PER_PERSON )						return FALSE;
			if( charunique !=0 && pclCM->IsValidID(charunique)== FALSE )	return FALSE;

			if( maxdurability < 0 )		maxdurability = 0;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			//====================================================
			// DO SOMETHING
			cltCharServer* pclchar = (cltCharServer*)pclCM->GetCharServer( id );
			if( pclchar )
			{
				SI32 siMaxDurability = (SI32)GetGlobalValue( TEXT("GV_MaxDurability") );

				cltItem* pclitem = &pclchar->pclCI->clCharItem.clItem[ pos ];
				if( pclitem && pclitem->siUnique > 0 )
				{
					if( maxdurability > siMaxDurability )
						maxdurability = siMaxDurability;

					pclitem->SetMaxDurability( pclItemManager, maxdurability );

					sDBRequest_WasteDurability clMsg( id, pclchar->pclCI->GetPersonID(), pos, pclitem );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

					cltGameMsgResponse_PersonItemInfo clItemInfo( pos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0 );
					cltMsg clMsg2(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clItemInfo), (BYTE*)&clItemInfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg2);
				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	else if(_tcscmp(order, TEXT("setdurability")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 pos;
			SI32 durability;
			_stscanf(&pstring[1], TEXT("%s %d %d %d"), order, &charunique, &pos, &durability);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if( pos < 0 || pos >= MAX_ITEM_PER_PERSON )						return FALSE;
			if( charunique !=0 && pclCM->IsValidID(charunique)== FALSE )	return FALSE;

			if( durability < 0 )		durability = 0;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			//====================================================
			// DO SOMETHING
			cltCharServer* pclchar = (cltCharServer*)pclCM->GetCharServer( id );
			if( pclchar )
			{
				cltItem* pclitem = &pclchar->pclCI->clCharItem.clItem[ pos ];
				if( pclitem && pclitem->siUnique > 0 )
				{
					if( durability > pclitem->GetMaxDurability( pclItemManager ) )
						durability = pclitem->GetMaxDurability( pclItemManager );

					pclitem->SetDurability( pclItemManager, durability );

					sDBRequest_WasteDurability clMsg( id, pclchar->pclCI->GetPersonID(), pos, pclitem );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

					cltGameMsgResponse_PersonItemInfo clItemInfo( pos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0 );
					cltMsg clMsg2(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clItemInfo), (BYTE*)&clItemInfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg2);
				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	else if(_tcscmp(order, TEXT("setmaxdurability")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 pos;
			SI32 maxdurability;
			_stscanf(&pstring[1], TEXT("%s %d %d %d"), order, &charunique, &pos, &maxdurability);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if( pos < 0 || pos >= MAX_ITEM_PER_PERSON )						return FALSE;
			if( charunique !=0 && pclCM->IsValidID(charunique)== FALSE )	return FALSE;

			if( maxdurability < 0 )		maxdurability = 0;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			//====================================================
			// DO SOMETHING
			cltCharServer* pclchar = (cltCharServer*)pclCM->GetCharServer( id );
			if( pclchar )
			{
				SI32 siMaxDurability = (SI32)GetGlobalValue( TEXT("GV_MaxDurability") );

				cltItem* pclitem = &pclchar->pclCI->clCharItem.clItem[ pos ];
				if( pclitem && pclitem->siUnique > 0 )
				{
					if( maxdurability > siMaxDurability )
						maxdurability = siMaxDurability;

					pclitem->SetMaxDurability( pclItemManager, maxdurability );

					sDBRequest_WasteDurability clMsg( id, pclchar->pclCI->GetPersonID(), pos, pclitem );
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

					cltGameMsgResponse_PersonItemInfo clItemInfo( pos, PERSONITEMINFO_OPTION_NONE, 0, 0, pclitem, 0 );
					cltMsg clMsg2(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clItemInfo), (BYTE*)&clItemInfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg2);
				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	// 해당 스킬 레벨로의 경험이 증가 
	else if(_tcscmp(order, TEXT("setskill")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 setlevel;
			TCHAR skillname[128] ;
			ZeroMemory(skillname, 128);
			SI32 skillunique ;

			_stscanf(&pstring[1], TEXT("%s %d %s %d"), order, &charunique, skillname, &setlevel);
			
			if((setlevel <= 0)||(setlevel > MAX_SKILL_LEVEL))
				return FALSE;

			ConvertUnderLineToSpace( skillname , 128);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			skillunique = pclSkillManager->FindUniqueFromSkillName(skillname) ;
			if ( skillunique <= 0 ) return false ;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			//====================================================
			// DO SOMETHING
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

			SI32 maxlevel = pclSkillManager->GetMaxLevel(skillunique) + MAX_SKILL_LEVEL + 1 ;
			if(setlevel <= 0 || setlevel >= maxlevel ) return FALSE;

			// Level이 떨어질수는 없다.
			SI32 totallevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(skillunique,0, 0) ;
			SI32 baselevel = pclchar->pclCI->clCharSkill.GetSkillLevel(skillunique);

			if ( baselevel < pclSkillManager->GetMaxLevel(skillunique) )
			{
				SI32 uplevel = 0 ;
				bool over = false ;
				if ( setlevel > pclSkillManager->GetMaxLevel(skillunique) )
				{
					uplevel = pclSkillManager->GetMaxLevel(skillunique) - baselevel ;
					over = true ;
				}
				else
				{
					uplevel = setlevel - baselevel ;
					over = false ;
				}

				if ( uplevel <= 0 ) return false ;

				pclchar->pclCI->clCharSkill.IncreaseSkillLevel(skillunique,uplevel);

				sDBRequest_IncreaseMasterSkill clMsg(id, pclchar->pclCI->GetPersonID(), skillunique, uplevel, pclSkillManager->GetMaxLevel(skillunique));
				SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				if ( over )
				{
					// 현재와의 경험치 차이를 구한다.
					SI32 increaseExp = GetRequiredSkillExpForLevel(setlevel-pclSkillManager->GetMaxLevel(skillunique)) - pclchar->pclCI->clCharSkill.GetSkillExp(skillunique);
					if(increaseExp<=0) return FALSE;

					pclchar->SetSkillUpdate(skillunique,increaseExp);
				}
				else
				{
					pclchar->SetUpdateSwitch(UPDATE_SKILL, true, 0);
					SendServerResponseMsg(0, SRVAL_SETSKILL,  skillunique, setlevel, id);
				}
			}
			else
			{
				// 현재와의 경험치 차이를 구한다.
				SI32 uplevel = setlevel - pclSkillManager->GetMaxLevel(skillunique) ;
				if(uplevel <= 0 || uplevel >= MAX_SKILL_LEVEL + 1 ) return FALSE;
				SI32 increaseExp = GetRequiredSkillExpForLevel(uplevel) - pclchar->pclCI->clCharSkill.GetSkillExp(skillunique);
				if(increaseExp<=0) return FALSE;

				pclchar->SetSkillUpdate(skillunique,increaseExp);
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	else if(_tcscmp(order, TEXT("setexpskill")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 setlevel;
			TCHAR skillname[128] ;
			ZeroMemory(skillname, 128);
			SI32 skillunique ;

			_stscanf(&pstring[1], TEXT("%s %d %s %d"), order, &charunique, skillname, &setlevel);

			if((setlevel <= 0)||(setlevel > MAX_SKILL_LEVEL))
				return FALSE;

			ConvertUnderLineToSpace( skillname , 128);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			skillunique = pclSkillManager->FindUniqueFromSkillName(skillname) ;
			if ( skillunique <= 0 ) return false ;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			//====================================================
			// DO SOMETHING
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

			SI32 maxlevel = pclSkillManager->GetMaxExpLevel(skillunique);
			SI32 curlevel = pclchar->pclCI->clCharSkill.GetSkillExpLevel(skillunique);
			if(setlevel <= curlevel || setlevel > maxlevel ) return FALSE;

			//pclchar->pclCI->clCharSkill.IncreaseSkillExpLevel( setlevel );
			//pclchar->SendDBSkillExp();

			//setlevel += 30;

			// 현재와의 경험치 차이를 구한다.
			//SI32 increaseExp = GetRequiredSkillExpForLevel(setlevel-pclSkillManager->GetMaxLevel(skillunique)) - pclchar->pclCI->clCharSkill.GetSkillExp(skillunique);
			SI32 increaseExp = GetRequiredSkillExpForLevel(setlevel) - pclchar->pclCI->clCharSkill.GetSkillExp(skillunique);
			if(increaseExp<=0) return FALSE;

			pclchar->SetSkillUpdate(skillunique,increaseExp);
		}
	}

	// [지연] 요술램프 퀘스트 단어 조합 테스트 위한 gm명령어
	//if ( _tcscmp(order, TEXT("lw")) == 0 )
	//{
	//	// GM 레벨 체크
	//	SI32 ref = pclGMManager->FindUniqueFromOrder(order);

	//	if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
	//	{
	//		// 램프 퀘스트의 완료 조건을 채워서 디비에 메세지 날려야 하는데.. 아직 몰라서 패스.
	//		cltQuestInfoCommon questInfo;
	//		questInfo.Init();
	//		
	//		questInfo.clQuestClearConditionInfo.siFlag;
	//		questInfo.clQuestClearConditionInfo.siPara1;
	//		questInfo.clQuestClearConditionInfo.siPara2;
	//		
	//		bool b = true;
	//		sDBRequest_SetSpecialQuestInfo clMsg(pclchar->pclCI->GetPersonID(), &(pclchar->pclCI->clQuestInfo.clSpecialQuestInfo), &b);
	//		SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
	//	}
	//}
	
	//----------------------------------------------------------------------------------------------
	// else if 가 너무 많아 컴파일러 오류가 발생하여 한번 나눠 줌
	// by LEEKH - 2007.10.24
	//----------------------------------------------------------------------------------------------

	if(_tcscmp(order, TEXT("incskillexp")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR skillname[128] ;
			ZeroMemory(skillname, 128);
			SI32 skillunique ;
			SI32 incExp;

			_stscanf(&pstring[1], TEXT("%s %d %s %d"), order, &charunique, skillname, &incExp);

			if(incExp <= 0)
				return FALSE;

			ConvertUnderLineToSpace( skillname , 128);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			skillunique = pclSkillManager->FindUniqueFromSkillName(skillname) ;
			if ( skillunique <= 0 ) return false ;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			pclchar->SetSkillUpdate(skillunique,incExp);
		}
	}
	else if(_tcscmp(order, TEXT("instancemaptime")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 time;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &time);

			SI32 id = serverid;
			if( pclCM->IsValidID(id) == false )		return FALSE;

			if(time <= 0)			time = 1;

			SI32 siMapIndex = pclchar->GetMapIndex();
			if(pclMapManager->IsInstanceMap(siMapIndex) == false)		return FALSE;
			cltMapCommon* pclMap = pclMapManager->GetMapPointer(siMapIndex);
			if(pclMap == NULL)											return FALSE;

			// 밀리세컨드로 변경
			time = time * 60 * 1000;

			SI32 siTmp = TABS(pclMap->m_dwInstanceMapStartTime - CurrentClock);
			siTmp += time;

			// 인던의 최대 시간을 조정한다. - 흐른시간 + 입력으로 받은 나머지 시간
			pclMap->m_dwInstanceMapMaxTime = siTmp;
		}
	}
	else if(_tcscmp(order, TEXT("newusernum")) == 0)
	{

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			sDBRequest_DBSystemInfo clMsg( serverid, pclCM->CR[serverid]->pclCI->GetPersonID(), DBSYSTEMINFO_TODAY_NEWUSERNUM,sTime );
			pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);		
		}
	}
	else if(_tcscmp(order, TEXT("getminister")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclMinisterMgr->InitializeDB();
			SendServerResponseMsg(0, SRVAL_MINISTER_SUCCESS_GETMINISTER,  0, 0, serverid);
		}
	}
	else if(_tcscmp(order, TEXT("wealthrank")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			sDBRequest_DBSystemInfo clMsg( serverid, pclCM->CR[serverid]->pclCI->GetPersonID(), DBSYSTEMINFO_CALCULATE_WEALTHRANK,sTime);
			pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);		

			SendServerResponseMsg(0, SRVAL_DBSYSTEMINFO,  DBSYSTEMINFO_CALCULATE_WEALTHRANK, 1 , pclCM->CR[serverid]->GetCharUnique());
		}
	}
	else if(_tcscmp(order, TEXT("totalmoney")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			sDBRequest_DBSystemInfo clMsg( serverid, pclCM->CR[serverid]->pclCI->GetPersonID(), DBSYSTEMINFO_CALCULATE_TOTALMONEY,sTime);
			pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);		

			SendServerResponseMsg(0, SRVAL_DBSYSTEMINFO,  DBSYSTEMINFO_CALCULATE_TOTALMONEY, 1 , pclCM->CR[serverid]->GetCharUnique());
		}
	}
	else if(_tcscmp(order, TEXT("tradeserver")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR szOption[128] = { '\0' };
			_stscanf( &pstring[1], TEXT("%s %s"), order, szOption );

			if (_tcscmp(szOption, TEXT("on")) == 0)
			{
				bForceStopTradeProcess = false;
			}
			else if (_tcscmp(szOption, TEXT("off")) == 0)
			{
				bForceStopTradeProcess = true;
			}
			else if (_tcscmp(szOption, TEXT("view")) == 0)
			{
				if ( pclNTradeServerProcess && pclNTradeServerProcess->GetTradeServerServersSession() )
				{	
					SI16 siState = (SI16)pclNTradeServerProcess->GetTradeServerServersSession()->GetState();

					SendServerResponseMsg( 0, SRVAL_NOTICE_TRADESERVER_STATE, siState, 0, serverid );
				}
				else
				{
					SendServerResponseMsg( 0, SRVAL_NOTICE_TRADESERVER_STATE, -1, 0, serverid );
				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	else if(_tcscmp(order, TEXT("giveitem")) == 0)
	{

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			int minlevel, maxlevel, num;
			TCHAR itemname[128];
			ZeroMemory(itemname, 128);
			TCHAR msg[ 256 ];
	
			msg[ 0 ] = NULL;

			_stscanf(&pstring[1], TEXT("%s %d %d %s %d %s"), 
				order, &minlevel, &maxlevel, itemname, &num, msg );

			if(num <= 0)
				return FALSE;

			ConvertUnderLineToSpace( itemname, 128 );

			int i, l, c;
			l = _tcslen( pstring );
			c = 0;
			for( i = 1; i < l; ++i ) {
				if( pstring[ i ] == ' ' ) c++;

				if( c == 5 ) {
					StringCchCopy( msg, 256, &pstring[ i + 1 ] );
					break;
				}
			}

			if(num <= 0)	num = 1;
			if(num >1000)	num = 1000;
			
			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}


			// 아이템 만들기 
			cltItem	clItem;
			SI16 brareswitch = 0;
			if( pclItemManager->MakeRandItemUnique( itemunique, &clItem, 0, 0, &brareswitch ) == false ) return false;
			
			// 개수를 조정한다. 
			// 중첩 가능한 아이템은 원하는 개수만큼 만들 수 있다. 
			if(clItem.CanPile( pclItemManager ) == true)
			{
				clItem.SetItemNum(num);
			}
			// 중첩 불가 아이템은 1개만 만들 수 있다. 
			else
			{
				clItem.SetItemNum(1);
			}

			sDBRequest_GiveItem clMsg( serverid, minlevel, maxlevel, &clItem, msg, &pclTime->clDate, clItem.IsCreateSID(pclItemManager) );
			pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&clMsg);

			// LEEKH MODIFY 2007.10.25
			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			//{
			//	// [로그추가 : 황진성 2007. 10. 18] // 시스템에서 아이템 주기.
			//	// param1 == 최소레벨, 
			//	// param2 == 최대레벨, 
			//	// pszParam1 == 메시지
			//	cltServer * pclServer = (cltServer*)pclClient;
			//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SYSTEM, LOGCOMMAND_INDEX_SYSTEM_GIVE_ITEM, 
			//									0, pclchar, NULL, 0, 0, 0, &clItem, 0, 0, 0, 0, 0, 0, 
			//									minlevel, maxlevel, 0, 0, 0, msg, NULL);
			//}
 		}
		else
		{
			bnotenoughmasterlevel = true;			
		}

	}
	else if ( _tcscmp(order, TEXT("giveitemone") ) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			int num;
			TCHAR itemname[128];
			ZeroMemory(itemname, 128);
			TCHAR msg[ 256 ];
			TCHAR charname[MAX_PLAYER_NAME];

			msg[ 0 ] = NULL;

			_stscanf(&pstring[1], TEXT("%s %s %d %s %s"), 
				order, itemname, &num, charname, msg );

			if(num <= 0)
				return FALSE;

			ConvertUnderLineToSpace( itemname , 128);

			int i, l, c;
			l = _tcslen( pstring );
			c = 0;
			for( i = 1; i < l; ++i ) {
				if( pstring[ i ] == ' ' ) c++;

				if( c == 5 ) {
					StringCchCopy( msg, 256, &pstring[ i + 1 ] );
					break;
				}
			}

			if(num <= 0)	num = 1;
			if(num >1000)	num = 1000;

			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			// 네잎클로버 지급의 경우 현재사용하고 있는 네잎클로버가 아니면 지급할 수 없다. - by LEEKH 2008-08-26
			if(itemunique == ITEMUNIQUE(2755) || itemunique == ITEMUNIQUE(2754))
			{
				if(itemunique != siFourLeafUnique)
				{
					return FALSE;
				}
			}

			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
			if (ref <= 0)	return FALSE;

			UI16 usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;

			// 아이템 만들기 
			cltItem	clItem;
			SI16 brareswitch = 0;
			if( pclItemManager->MakeRandItemUnique( itemunique, &clItem, 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate ) == false ) return false;

			// 개수를 조정한다. 
			// 중첩 가능한 아이템은 원하는 개수만큼 만들 수 있다. 
			if(clItem.CanPile( pclItemManager ) == true)
			{
				clItem.SetItemNum(num);
			}
			// 중첩 불가 아이템은 1개만 만들 수 있다. 
			else
			{
				clItem.SetItemNum(1);
			}

			SendPostOfficeItemToPerson(charname, &clItem, msg );
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}
	}
	else if ( _tcscmp(order, TEXT("giftnt") ) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{			
			TCHAR RecvGiftName[ 32 ] = TEXT("");
			TCHAR MasterNoticeMsg[ 128 ] =TEXT("");
			_stscanf(&pstring[1], TEXT("%s %s"), order, RecvGiftName );
			
			if ( _tcslen( RecvGiftName ) < 1 )
			{

			}
			else
			{
				_tcsncpy( MasterNoticeMsg, &pstring[ 9 + _tcslen( RecvGiftName ) ], 127 );
				MasterNoticeMsg[ 127 ] = '\0';

				cltGameMsgResponse_GiftNT clinfo( RecvGiftName, MasterNoticeMsg );
				cltMsg clMsg(GAMEMSG_RESPONSE_GIFTNT, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
			}
		}
	}
	else if ( _tcscmp(order, TEXT("summonkill") ) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{			
			SI32 siSummonIndex = 0;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &siSummonIndex);

			SI32 siSummonID = pclchar->GetSummonID();	
			if(pclCM->IsValidID(siSummonID))
			{
					// DB에 소환수를 설정한다.
				pclchar->SendDBSummon(SETSUMMON_REASON_DIE, siSummonIndex);
			}
		}
	}
	else if ( _tcscmp(order, TEXT("summonevolve") ) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{			
			TCHAR npcname[128];
			memset(npcname, 0, sizeof(npcname));

			_stscanf(&pstring[1], TEXT("%s %s"), order, npcname);
			ConvertUnderLineToSpace( npcname , 128);

			// 소환수를 회수되었을떄만 진행한다.
			SI32 siSummonID = pclchar->GetSummonID();
			if(pclCM->IsAlive(siSummonID))
			{
				cltSummon *pclSummon = pclchar->pclCI->clSummonInfo.GetSummon(pclCM->CR[siSummonID]->GetSummonIndex());
				// 일단 소환수가 있어야 바꿀수 있다.
				if(pclSummon) 
				{
					// NPC이름을 근거로 NPC의 Kind를 찾는다. 
					SI32 kind = pclKindInfoSet->FindKindFromName(npcname);
					if(kind > 0 && kind < MAX_KIND_NUMBER)	
					{
						//cltKindInfo* pclki = pclKindInfoSet->pclKI[kind];
						cltKindInfo* pclki = NULL;
						if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
						{
							pclki = pclClient->pclKindInfoSet->pclSummonKI[kind];
						}
						else
						{
							pclki = pclClient->pclKindInfoSet->pclKI[kind];
						}

						if(pclki != NULL && pclki->siSummonRank > 0)
						{
							//---------------------------------------------
							// 게임에서 소환수를 지운다. 
							//---------------------------------------------
							pclCM->CR[siSummonID]->DeleteInCell();
							pclCM->DeleteChar(siSummonID);
							
							//====================================================
							// 새로 진화할 소환수를 설정한다.
							//====================================================
							cltPIPInfoCommon	clipinfo;
							cltPIPInfo			clipinfo2;
							//cltKindInfo* pclki = pclKindInfoSet->pclKI[kind];
							cltKindInfo* pclki = NULL;
							cltGMCommandKindInfo* pclGMki = NULL;

							if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
							{
								pclki = pclClient->pclKindInfoSet->pclSummonKI[kind];
								pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[kind];
							}
							else
							{
								pclki = pclClient->pclKindInfoSet->pclKI[kind];
								pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[kind];
							}

							clipinfo.SetBasicAbility(&pclki->clBA);	
							
							clipinfo2.Set(&clipinfo);
							cltParametaBox clPB;
							clPB.Init(pclItemManager, &clipinfo2, NULL, pclki, pclGMki);
							
							bool bmonsterhero = false;
							// 진화 대상이 아이템몰에서 판매하는 소환영웅이면, 
							if(pclki->bSummonMall)	
							{
								bmonsterhero = true;
							}

							clPB.Action(NULL, 0, PERSONITEM_WEAPON1, NULL, NULL, NULL, NULL, NULL, pclSummon->siAttackWearStoneNum,pclSummon->siDefenseWearStoneNum, GAMEMODE_SERVER, true);
							
							clipinfo.SetLife(clPB.GetMaxLife());	// 기본적인 것만 저장한다.( 사실 PBBOX 를 사용해서 풀로 채워야한다)
							clipinfo.SetMana(clPB.GetMaxMana());
							
							cltSummonScroll clscroll(pclki->siSummonScroll[0], 0, pclki->siSummonScroll[1], 0);

							cltSummonItemInfo clItemInfo;
							clItemInfo.Init();
							
							//==========================================
							// DB에 소환수를 설정한다.
							//==========================================
							cltSummon clSummon(	pclSummon->siSummonID, 
												kind, 
												pclSummon->szName, 
												SUMMON_STATUS_TAKEIN,	
												SUMMON_ATTACKTYPE_AVOID,	
												SUMMON_IDENTITY_NORMAL, 
												0, 
												0, 
												0,
												&clipinfo,				
												&clscroll,
												pclSummon->m_siTranformStamina );
							pclchar->SendDBSummon( SETSUMMON_REASON_GMEVOLVE, pclCM->CR[siSummonID]->GetSummonIndex(), &clSummon );
							
							//==========================================
							// 주인이 갖고 있는 소환수의 정보를 설정한다.
							//==========================================
							pclSummon->Init();
							pclchar->siChildCharUnique  = 0;
							
							//==========================================
							// 주인이 갖고 있는 소환수의 정보를 설정한다.
							//==========================================
							pclSummon->Set(&clSummon);
							{
								cltGameMsgResponse_Summon clInfo(pclchar->GetCharUnique(), pclCM->CR[siSummonID]->GetSummonIndex(), pclSummon);
								cltMsg clMsg(GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo);
								pclchar->SendNetMsg((sPacketHeader*)&clMsg);
							}
							pclchar->siChildCharUnique = 0;
							{
								// 주인한테도 정보를 갱신시킨다.
								cltGameMsgResponse_ChangeSummonStatus clInfo(pclchar->GetCharUnique(), pclchar->siChildCharUnique, pclchar->siSummonIndex);
								cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESUMMONSTATUS, sizeof(clInfo), (BYTE*)&clInfo);
								pclchar->SendNetMsg((sPacketHeader*)&clMsg);
							}
						}
					}
				}
			}
		}
	}
	//// 매크로 캐락분석 파일을 생성 한다 해당 케릭터에 .
	else if(_tcscmp(order, TEXT("recordaction")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR TargetCharName[128];
			memset(TargetCharName, 0, sizeof(TargetCharName));
			_stscanf(&pstring[1], TEXT("%s %s"), order, TargetCharName);	// 오더 , 케릭터 이름

			if(_tcslen( TargetCharName ) <= 0)	return	FALSE	;
			if( TargetCharName == NULL )	return FALSE	;

			SI32 siTargetCharID = pclCM->GetIDFromName(TargetCharName)	;
			cltCharServer* pclTargetCharServer = pclCM->GetCharServer(siTargetCharID)	;

			if (pclCM->IsValidID( siTargetCharID ) == FALSE )							
			{
				return	FALSE;
			}
			time_t now	;
			time( &now );
			cltGameMsgRequest_RecordAction	clinfo( pclchar->pclCI->GetPersonID(), 
				pclTargetCharServer->pclCI->GetPersonID(),
				pclTargetCharServer->GetSession()->m_szIP,
				now, TRUE );
			// 해당 유저에게 패킷을 쏜다 
			cltMsg clMsg(GAMEMSG_REQUEST_RECORD_ACTION, sizeof(clinfo), (BYTE*)&clinfo);
			pclTargetCharServer->SendNetMsg((sPacketHeader*)&clMsg);

			SendServerResponseMsg(0, SRVAL_RECORD_MOUSEACTION_START, 0, 0, serverid );

		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 매크로 유저 정보를 가지고 온다 .
	else if(_tcscmp(order, TEXT("getrecordaction")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR TargetCharName[128];
			CHAR	szFilePath[ MAX_PATH ] = TEXT("goonzu.gma")	;


			memset(TargetCharName, 0, sizeof(TargetCharName));
			_stscanf(&pstring[1], TEXT("%s %s"), order, TargetCharName);	// 오더 , 케릭터 이름

			if(_tcslen( TargetCharName ) <= 0)	return	FALSE	;
			if( TargetCharName == NULL )	return FALSE	;

			SI32 siTargetCharID = pclCM->GetIDFromName(TargetCharName)	;
			cltCharServer* pclTargetCharServer = pclCM->GetCharServer(siTargetCharID)	;

			if( pclTargetCharServer == NULL )	return	FALSE;

			cltGameMsgGZFTPProtocol::__initialize_data	data;

			SI32 siGMPersonID = pclchar->pclCI->GetPersonID()	;
			SI32 siTargetPersonID = pclTargetCharServer->pclCI->GetPersonID()	;

			data.siyourPersonID		=	siGMPersonID	;
			data.siotherPersonID	=	siTargetPersonID	;

			strncpy( data.szSenderName,TargetCharName, sizeof( data.szSenderName ) - 1 );		

			strncpy( data.szFilePath, szFilePath, sizeof( data.szFilePath ) - 1 );

			data.boption_delete		=	TRUE;

			cltGameMsgGZFTPProtocol	clinfo(	cltGameMsgGZFTPProtocol::GZFTP_INITIALIZE,siGMPersonID,siTargetPersonID,
				( BYTE* )&data, sizeof( data ) );		
			cltMsg clMsg( GAMEMSG_GZFTP_PROTOCOL, sizeof(clinfo),(BYTE*)&clinfo );
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp(order, TEXT("prison") ) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel/* || minister5switch */)
		{
			TCHAR personname[MAX_PLAYER_NAME];
			SI32 time;
			TCHAR reason[MAX_PRISON_REASON_SIZE];

			_stscanf(&pstring[1], TEXT("%s %s %d %s"), order, personname,&time,reason);

			if ( _tcslen(personname) <= 0 || _tcslen(personname) > MAX_PLAYER_NAME )
			{
				SendServerResponseMsg(0, SRVAL_PRISON_FAIL_NAMEERROR,  0, 0,  serverid);
				return FALSE ;
			}
			if ( time <= 0 || time > MAX_PRISON_SECOND )
			{
				SendServerResponseMsg(0, SRVAL_PRISON_FAIL_TIMEERROR,  0, 0,  serverid);
				return FALSE ;
			}
			time *= 60 ;
			if ( _tcslen(reason) <= 0 || _tcslen(reason) > MAX_PRISON_REASON_SIZE )
			{
				SendServerResponseMsg(0, SRVAL_PRISON_FAIL_REASONERROR,  0, 0,  serverid);
				return FALSE ;
			}

			cltGuiltyInfo pclGuiltyInfo ;
			pclGuiltyInfo.Init();
			pclGuiltyInfo.SetTotalPrisonSecond(time);
			pclGuiltyInfo.SetLeftPrisonSecond(time);
			pclGuiltyInfo.SetPrisonReason(reason);
			
			SI32 curdate = pclTime->clDate.GetDateVary();
			sDBRequest_PrisonUser clMsg(serverid,curdate, personname,
										pclCM->CR[serverid]->pclCI->GetPersonID(),&pclGuiltyInfo);
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp(order, TEXT("free") ) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR charname[MAX_PLAYER_NAME];

			_stscanf(&pstring[1], TEXT("%s %s"), order, charname);

			if ( _tcslen(charname) <= 0 || _tcslen(charname) > MAX_PLAYER_NAME )
			{
				SendServerResponseMsg(0, SRVAL_PRISON_FAIL_NAMEERROR,  0, 0,  serverid);
				return FALSE ;
			}

			SI32 personid, id;
			if( pclHashTable->Find( charname, personid, id, 140) == false)return FALSE;

			pclCM->CR[id]->pclCI->clGuiltyInfo.SetLeftPrisonSecond(0);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp(order, TEXT("fourleafset") ) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if( gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel )
		{			
			SI32 siUnique = 0;

			_stscanf( &pstring[1], TEXT("%s %d"), order, &siUnique );
			//siUnique = _ttoi(Unique);

			// 같은 유니크가 설정되면, 실패로 처리한다.
			if(siUnique == siFourLeafUnique)
			{
				return FALSE;
			}

			// 사용되는 네잎클로버가 아니면 실패 시킨다.
			if(siUnique == ITEMUNIQUE(2755) || siUnique == ITEMUNIQUE(2754))
			{
				sDBRequest_SetFourLeaf clMsg( siUnique );
				SendDBMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);

				SendServerResponseMsg(0, SRVAL_GAMEMASTER_FOURLEAF, siFourLeafUnique, 0, serverid);
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp(order, TEXT("fourleafview") ) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if( gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel )
		{
			SendServerResponseMsg(0, SRVAL_GAMEMASTER_FOURLEAF, siFourLeafUnique, 0, serverid);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp(order, TEXT("mofulandstart") ) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);
		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR villagename[128];

			_stscanf(&pstring[1], TEXT("%s %s"), order, villagename);

			// 마을의 이름을 토대로 MapIndex를 찾는다. 
			SI32 villageunique = pclVillageManager->FindVillageUniqueFromName(villagename);

			if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER
				|| pclVillageManager->pclVillageInfo[villageunique] == NULL)return false;

			pclMofuMofuEvent->Start(1000, villageunique);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp(order, TEXT("mofulandend") ) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);
		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			_stscanf(&pstring[1], TEXT("%s"), order);

			pclMofuMofuEvent->End();
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if(_tcscmp(order, TEXT("hp")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 hp;

			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &charunique, &hp);

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			SI32 siMaxLife = pclCM->CR[id]->clPB.GetMaxLife();
			if(siMaxLife < hp)		hp = siMaxLife;
			pclCM->CR[id]->SetLife(hp);

			cltGameMsgResponse_Hit clHitInfo(pclCM->CR[id]->GetCharUnique(), 1, 0, pclCM->CR[id]->GetLife(), 0, 0, false, false, false, 0 );
			cltMsg clMsg(GAMEMSG_RESPONSE_HIT, sizeof(clHitInfo), (BYTE*)&clHitInfo);
			((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	else if ( _tcscmp(order, TEXT("huntingstart") ) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{

			TCHAR villagename[128];
			
			_stscanf(&pstring[1], TEXT("%s %s"), order, villagename);

			// 마을의 이름을 토대로 MapIndex를 찾는다. 
			SI32 villageunique = pclVillageManager->FindVillageUniqueFromName(villagename);

			if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER
			|| pclVillageManager->pclVillageInfo[villageunique] == NULL)return false;

			pclEventHunting->Start(villageunique);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp(order, TEXT("huntingend") ) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclEventHunting->End();
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
/*	else if ( _tcscmp(order, TEXT("gogustart") ) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{

			TCHAR villagename[128];
			
			_stscanf(&pstring[1], TEXT("%s %s"), order, villagename);

			// 마을의 이름을 토대로 MapIndex를 찾는다. 
			SI32 villageunique = pclVillageManager->FindVillageUniqueFromName(villagename);

			if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER
			|| pclVillageManager->pclVillageInfo[villageunique] == NULL)return false;

			pclEventGogu->Start(villageunique, false);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}*/
	/*else if ( _tcscmp(order, TEXT("goguend") ) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclEventGogu->End();
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}*/
	else if ( _tcscmp(order, TEXT("rareitemrate")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 attack = 0 ;
			SI32 mag = 0 ;
			SI32 skill = 0 ;
			SI32 dur = 0 ;

			_stscanf(&pstring[1], TEXT("%s %d %d %d %d %d"), order, &charunique,&attack,&mag,&skill,&dur);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			pclCM->CR[id]->SetRareItemRate(0, 0, 0, 0);
			pclCM->CR[id]->SetRareItemRate( attack, mag, skill, dur );
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp(order, TEXT("changerare")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 charunique = 0 ;
			TCHAR itemname[MAX_ITEM_NAME_LENGTH] = TEXT("") ;
			UI32 rarelevel = 0 ;

			_stscanf(&pstring[1], TEXT("%s %d %s %d"), order, &charunique, itemname, &rarelevel);

			if(rarelevel <= 0)
				return FALSE;

			ConvertUnderLineToSpace( itemname, MAX_ITEM_NAME_LENGTH );

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname) ;
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			if ( pclItemManager->CanPile(itemunique) == true ) return FALSE ;
			if ( charunique !=0 && pclCM->IsValidID(charunique)== FALSE ) return FALSE ;

			switch(rarelevel)
			{
			case 1: rarelevel = RARELEVEL_1; break;
			case 2: rarelevel = RARELEVEL_2; break;
			case 3: rarelevel = RARELEVEL_3; break;
			case 4: rarelevel = RARELEVEL_4; break;
			case 5: rarelevel = 0 ; break ;
			default: rarelevel = RARELEVEL_2; break;
			}

			SI32 id = 0 ;
			if ( charunique == 0 ){	id = serverid ; }
			else { id = pclCM->GetIDFromCharUnique(charunique) ; }
			if ( id == 0 ) return FALSE ;

			cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];
			if ( pclchar == NULL ) return FALSE ;

			for ( int i = 0 ; i < MAX_ITEM_PER_PERSON ; i ++ )
			{
				cltItem* pclItem = &pclCM->CR[id]->pclCI->clCharItem.clItem[i] ;
				if ( pclItem->siUnique == itemunique &&
					 pclItem->clItemCommon.clCommonInfo.uiRareInfo != rarelevel )
				{
					pclItem->clItemCommon.clCommonInfo.uiRareInfo = rarelevel ;

					SI32 FromPersonID = pclchar->pclCI->GetPersonID();

					//cyj 명품으로 변경시 PersonID추가
					pclItem->siMakerPersonID = FromPersonID;

					sDBRequest_WasteDurability clMsg(id, FromPersonID, i, pclItem);
					SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

					cltGameMsgResponse_PersonItemInfo clinfo(i, PERSONITEMINFO_OPTION_MASTER, 0, 0, pclItem, 0);
					cltMsg clMsg2(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg2);

					break;
				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp(order, TEXT("sthorse")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 charunique = 0 ;
			SI32 status[4] = {10,10,10,10};

			_stscanf(&pstring[1], TEXT("%s %d %d %d %d %d"), order, &charunique,
				   &status[0], &status[1],&status[2],&status[3]);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if ( charunique !=0 && pclCM->IsValidID(charunique)== FALSE ) return FALSE ;
			SI32 id = 0 ;
			if ( charunique == 0 ){	id = serverid ; }
			else { id = pclCM->GetIDFromCharUnique(charunique) ; }
			if ( id == 0 ) return FALSE ;

			cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];

			for ( int i = 0 ; i < 4 ; i ++ )
			{
				if ( status[i] <= 0 ) status[i] = 10 ;

				status[i] *= 100 ;
			}

			cltHorse* pclHorse = pclchar->pclCI->clHorseInfo.GetCurrentShowHorse();
			if(pclHorse != NULL && pclHorse->siHorseUnique > 0)
			{
				SI32 horseuniqque = pclHorse->siHorseUnique;

				pclHorse->siMoveSpeed	= min(pclHorseManager->GetFullSpeed( horseuniqque ), status[0]);
				pclHorse->siStr			= min(pclHorseManager->GetFullStr( horseuniqque ), status[1]);
				pclHorse->siDex			= min(pclHorseManager->GetFullDex( horseuniqque ), status[2]);
				pclHorse->siMag			= min(pclHorseManager->GetFullMag( horseuniqque ), status[3]);

				//KHY - 1001 - 기승동물 5마리 보유 수정.
				// DB에 저장한다. 
				//pclchar->UpdateHorseToDB(this, pclchar->pclCI->clHorseInfo.siCurrentShowHorse, &pclchar->pclCI->clHorseInfo.clHorse, SETHORSE_REASON_GAMEMASTER);
				pclchar->UpdateHorseToDB(this, pclchar->pclCI->clHorseInfo.siCurrentShowHorse, pclHorse, SETHORSE_REASON_GAMEMASTER);

				pclchar->SetUpdateSwitch(UPDATE_HORSE, true, 0);
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp(order, TEXT("horselife")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 life;
			SI32 charunique = 0 ;

			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &charunique,
				   &life);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if ( charunique !=0 && pclCM->IsValidID(charunique)== FALSE ) return FALSE ;
			SI32 id = 0 ;
			if ( charunique == 0 ){	id = serverid ; }
			else { id = pclCM->GetIDFromCharUnique(charunique) ; }
			if ( id == 0 ) return FALSE ;

			cltHorse* pclHorse = pclchar->pclCI->clHorseInfo.GetCurrentShowHorse();

			if(pclHorse != NULL && pclHorse->siHorseUnique > 0)
			{
				SI32 fulllife = pclHorseManager->GetFullLife(pclHorse->siHorseUnique);
				if(life <= 0 || life > fulllife)return FALSE;


				cltCharServer* pclchar = (cltCharServer* )pclCM->CR[id];

				SI32 curdate = pclTime->clDate.GetDateVary();

				//KHY - 1001 - 기승동물 5마리 보유 수정.
				//현재 active되어 있는 말을 가져온다. - active변수 필요.
				//pclHorseManager->SetLife(&pclchar->pclCI->clHorseInfo.clHorse, curdate, life);
				pclHorseManager->SetLife(pclHorse, curdate, life);

				// DB에 저장한다. 
				//pclchar->UpdateHorseToDB(this, pclchar->pclCI->clHorseInfo.siCurrentShowHorse, &pclchar->pclCI->clHorseInfo.clHorse, SETHORSE_REASON_GAMEMASTER);
				pclchar->UpdateHorseToDB(this, pclchar->pclCI->clHorseInfo.siCurrentShowHorse, pclHorse, SETHORSE_REASON_GAMEMASTER);

				pclchar->SetUpdateSwitch(UPDATE_HORSE, true, 0);
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	
	else if ( _tcscmp(order, TEXT("questinit")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 questunique;
			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &charunique, &questunique);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}
			
			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;
			
			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;
			

			cltQuestInfo* pclquestinfo = pclQuestManager->GetQuestInfo(questunique);
			if(pclquestinfo == NULL)return FALSE;

			SI32 bit = pclQuestManager->GetBitFromUnique(pclquestinfo->siType, questunique);

			// 사용자의 퀘스트 정보를 수정한다. 
			pclCM->CR[id]->pclCI->clQuestInfo.clUnit[pclquestinfo->siType].SetBit(bit, 0);

			// DB에 퀘스트의 상태를 저장한다. 
			sDBRequest_SetQuestInfo clMsg(	pclCM->CR[id]->pclCI->GetPersonID(), 
											pclquestinfo->siType, 
											&pclCM->CR[id]->pclCI->clQuestInfo.clUnit[pclquestinfo->siType]);
			SendDBMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);
										
			// 클라이언트에 퀘스트의 상태를 통보한다. 
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_QUEST, true, 0);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp(order, TEXT("questpass")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 questunique;
			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &charunique, &questunique);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;


			cltQuestInfo* pclquestinfo = pclQuestManager->GetQuestInfo(questunique);
			if(pclquestinfo == NULL)return FALSE;

			SI32 bit = pclQuestManager->GetBitFromUnique(pclquestinfo->siType, questunique);

			// 사용자의 퀘스트 정보를 수정한다. 
			pclCM->CR[id]->pclCI->clQuestInfo.clUnit[pclquestinfo->siType].SetBit(bit, 1);
			pclCM->CR[id]->pclCI->clQuestInfo.clUnit[pclquestinfo->siType].siCurQuestStep = QUEST_STEP_CLEAR;

			// DB에 퀘스트의 상태를 저장한다. 
			sDBRequest_SetQuestInfo clMsg(	pclCM->CR[id]->pclCI->GetPersonID(), 
											pclquestinfo->siType, 
											&pclCM->CR[id]->pclCI->clQuestInfo.clUnit[pclquestinfo->siType]);
			SendDBMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);

			// 클라이언트에 퀘스트의 상태를 통보한다. 
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_QUEST, true, 0);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp(order, TEXT("questclear")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 questtype;
			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &charunique, &questtype);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}
			
			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;
			
			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;
			

			if(questtype < 0 || questtype >= MAX_QUEST_TYPE)return false;

			// 사용자의 퀘스트 정보를 수정한다. 
			pclCM->CR[id]->pclCI->clQuestInfo.clUnit[questtype].siQuestState = 0xffffffff;

			// DB에 퀘스트의 상태를 저장한다. 
			sDBRequest_SetQuestInfo clMsg(	pclCM->CR[id]->pclCI->GetPersonID(), 
											questtype, 
											&pclCM->CR[id]->pclCI->clQuestInfo.clUnit[questtype]);
			SendDBMsg( DBSELECT_SUB, (sPacketHeader *)&clMsg);
			
			// 클라이언트에 퀘스트의 상태를 통보한다. 
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_QUEST, true, 0);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if( _tcscmp(order, TEXT("setminister")) == 0 )
	{

		if( gamemasterlevel >= BASIC_GAMEMASTER_LEVEL+1 )
		{
			TCHAR ministerbuffer[16] = TEXT("") ;
			TCHAR ministername[MAX_PLAYER_NAME] = TEXT("") ;

			_stscanf(&pstring[1], TEXT("%s %s %s"), order, ministerbuffer, ministername );
			ConvertUnderLineToSpace( ministerbuffer, 16);

			SI32 grank = 0 ;
			TCHAR* pText[7];
			pText[0] = GetTxtFromMgr(599);
			pText[1] = GetTxtFromMgr(5075);
			pText[2] = GetTxtFromMgr(5076);
			pText[3] = GetTxtFromMgr(5077);
			pText[4] = GetTxtFromMgr(5078);
			pText[5] = GetTxtFromMgr(5079);
			pText[6] = GetTxtFromMgr(5074);

			if (!_tcscmp(ministerbuffer,pText[0])) 
			{
				grank = GRANK_KING ;
			}
			else if (!_tcscmp(ministerbuffer,pText[1])) 
			{
				
				grank = GRANK_MINISTER1 ;
			}
			else if (!_tcscmp(ministerbuffer,pText[2])) 
			{
				
				grank = GRANK_MINISTER2 ;
			}
			/*cyj 예조판서 제외
			else if (!_tcscmp(ministerbuffer,pText[3])) 
			{
				
				grank = GRANK_MINISTER3 ;
			}
			*/
			else if (!_tcscmp(ministerbuffer,pText[4]))
			{
				
				grank = GRANK_MINISTER4 ;
			}
			else if (!_tcscmp(ministerbuffer,pText[5]))
			{
				
				grank = GRANK_MINISTER5 ;
			}
			else if (!_tcscmp(ministerbuffer,pText[6]))
			{
				
				grank = GRANK_MINISTER6 ;
			}
			else return FALSE ;

			SI32 personid = pclCM->CR[serverid]->pclCI->GetPersonID() ;
			if ( personid <= 0 ) return FALSE ;

			sDBRequest_SetMinister clMsg(grank,personid,ministername,true,0);
			SendDBMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp( order, TEXT("sethorseprice") ) == 0 )
	{
		if( gamemasterlevel >= BASIC_GAMEMASTER_LEVEL+1 )
		{
			SI32 HorsePrice = 0;

			_stscanf(&pstring[1], TEXT("%s %d"), order, &HorsePrice );

			
			sDBRequest_SetCurrentHorsePrice clSetCurrentHorsePrice( 1, HorsePrice );
			SendDBMsg( DBSELECT_BASE, (sPacketHeader *)&clSetCurrentHorsePrice );
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp( order, TEXT("refreshhouse") ) == 0 )
	{
		if( gamemasterlevel >= BASIC_GAMEMASTER_LEVEL+1 )
		{			
			RefreshHouseUnit();
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp( order, TEXT("questviewer") ) == 0 )
	{
		if( gamemasterlevel >= BASIC_GAMEMASTER_LEVEL+1 )
		{			
			
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp( order, TEXT("refreshinvest") ) == 0 )
	{
		if( gamemasterlevel >= BASIC_GAMEMASTER_LEVEL+1 )
		{			
			RefreshInvestList();
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if ( _tcscmp( order, TEXT("refreshstock") ) == 0 ) 
	{
		if( gamemasterlevel >= BASIC_GAMEMASTER_LEVEL+1 )
		{			
			RefreshStock();
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 로그인 공지 on./off
	else if(_tcscmp(order, TEXT("ntadmin")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR msgtext[256];
			TCHAR* pnotice = &pstring[1];
			StringCchCopy(msgtext, 256, &pnotice[8] );

			cltGameMsgResponse_NoticeAdmin clNoticeAdmin(msgtext);
			cltMsg clMsg(GAMEMSG_RESPONSE_NOTICEADMIN, sizeof(clNoticeAdmin), (BYTE*)&clNoticeAdmin);

			pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true ;
		}
	}
	//cyj PC방관련 삭제
#ifdef _DEBUG
	else if(_tcscmp(order, TEXT("setpcroom")) == 0 ) 
	{
		if( gamemasterlevel >= BASIC_GAMEMASTER_LEVEL+1 )
		{
			_stscanf(&pstring[1], TEXT("%s %d"), order, &charunique );

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			/*
			//====================================================
			// DO SOMETHING
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

			pclchar->pclCI->clBI.uiGMMode = !pclchar->pclCI->clBI.uiGMMode;
			*/
			pclchar->pclCI->clBI.bPCRoom = true;

			SendCharUpdate( pclchar->GetCharUnique(), false );

		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
#endif
	
	else if(_tcscmp(order, TEXT("getserverinfo")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			GetServerInfo();

			pclCM->siIncLifeUnit = siRegenerateLifeValue;
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 기간제 아이템을 만든다. 
	else if(_tcscmp(order, TEXT("ciday")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{

			TCHAR itemname[128];
			ZeroMemory(itemname, 128);
			SI32 usedate = 0;

			/*
			if( IsWhereServiceArea(ConstServiceArea_English | ConstServiceArea_NHNChina | ConstServiceArea_EUROPE) )
			{
				_stscanf(&pstring[1], TEXT("%s %d %s %d"), order, &charunique, itemname, &usedate);
				if ( pclClient->siServiceArea != ConstServiceArea_Japan )
					ConvertUnderLineToSpace( itemname );
			}
			else
			{
				_stscanf(&pstring[1], TEXT("%s %d %s %d"), order, &charunique, itemname, &usedate);
			}
			*/
			_stscanf(&pstring[1], TEXT("%s %d %s %d"), order, &charunique, itemname, &usedate);

			if(usedate <= 0 )
				return FALSE;

			ConvertUnderLineToSpace( itemname, 128 );

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			pclCM->GiveItem(id, itemunique, 1, GIVEITEM_MODE_PERSON, INPUTITEMPERSON_OPTION_MASTER,usedate);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	// 모든 유저에게 기간제 아이템을 날짜 설정해서 제공한다.
	else if(_tcscmp(order, TEXT("cidayall")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{

			TCHAR itemname[128];
			ZeroMemory(itemname, 128);
			SI32 usedate = 0;
			_stscanf(&pstring[1], TEXT("%s %s %d"), order, itemname, &usedate);

			if(usedate <= 0 )
				return FALSE;

			ConvertUnderLineToSpace( itemname, 128 );

			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			if ( usedate <= 0 ) return FALSE ;

			pclCM->GiveItem(0, itemunique, 1, GIVEITEM_MODE_ALL, INPUTITEMPERSON_OPTION_MASTER,usedate);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 레벨 20이상 모든 유저들에게 기간제 아이템을 날자 설정해서 제공한다. 
	else if(_tcscmp(order, TEXT("cidayadult")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{

			TCHAR itemname[128];
			ZeroMemory(itemname, 128);
			SI32 usedate = 0;
			_stscanf(&pstring[1], TEXT("%s %s %d"), order, itemname, &usedate);

			if(usedate <= 0 )
				return FALSE;

			ConvertUnderLineToSpace( itemname, 128 );

			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			if ( usedate <= 0 ) return FALSE ;

			pclCM->GiveItem(0, itemunique, 1, GIVEITEM_MODE_ADULT, INPUTITEMPERSON_OPTION_MASTER,usedate);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	//역참으로 모든 사용자에게 기간제 아이템을 사용날자를 설정해서 역참으로 보내준다.
	else if(_tcscmp(order, TEXT("givedayitem")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR itemname[128] = TEXT("") ;
			TCHAR msg[256] = TEXT("") ;
			SI32 usedate = 0 ;

			msg[ 0 ] = NULL;

			_stscanf(&pstring[1], TEXT("%s %s %d %s"), 
				order, itemname, &usedate, msg );

			if(usedate <= 0 )
				return FALSE;

			ConvertUnderLineToSpace( itemname, 128 );

			int i, l, c;
			l = _tcslen( pstring );
			c = 0;
			for( i = 1; i < l; ++i ) {
				if( pstring[ i ] == ' ' ) c++;

				if( c == 5 ) {
					StringCchCopy( msg, 256, &pstring[ i + 1 ] );
					break;
				}
			}

			if ( usedate <= 0 ) return FALSE ;

			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemname );
				if(itemunique <= 0)return FALSE;
			}

			// 아이템 만들기 
			cltItem	clItem;
			SI16 brareswitch = 0;
			if( pclItemManager->MakeRandItemUnique( itemunique, &clItem, 0, 0, &brareswitch ,0,0,0,0,0,usedate) == false ) return false;

			// 중첩 가능한 아이템은 안된다.
			if(clItem.CanPile( pclItemManager ) == true)
			{
				return FALSE;
			}
			// 중첩 불가 아이템은 1개만 만들 수 있다. 
			else
			{
				clItem.SetItemNum(1);
			}

			sDBRequest_GiveItem clMsg( serverid, 0, MAX_CHARACTER_LEVEL, &clItem, msg, &pclTime->clDate, clItem.IsCreateSID(pclItemManager) );
			pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&clMsg);

			// LEEKH MODIFY 2007.10.25
			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			//{
			//	// [로그추가 : 황진성 2007. 10. 18] // 아이템 주기.
			//	// param1 == 아이템 기간, pszParam1 == 메시지
			//
			//	cltServer * pclServer = (cltServer*)pclClient;
			//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SYSTEM, LOGCOMMAND_INDEX_SYSTEM_GIVE_ITEM, 
			//									0, (cltCharServer*)pclCM->CR[serverid], NULL, 0, 0, 0, &clItem, 0, 0, 0, 0, 0, 0, 
			//									usedate, 0, 0, 0, 0, msg, NULL);
			//// TEMP LEEKH --------------------------------------------------------------------------
			//}
		}
		else
		{
			bnotenoughmasterlevel = true;			
		}

	}
	//cyj GM마크 On/Off추가 => GM모드 변경
	else if(_tcscmp(order, TEXT("gmmode")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if(pclCM->CR[serverid])
			{
				SI32 gmmode = 0 ;

				_stscanf(&pstring[1], TEXT("%s %d"), order, &gmmode );
				
				//[추가 : 황진성 2007. 11. 27 gmmode5 추가.]
				if(gmmode == 5)
				{
					// gmmode 5일경우는 죽어선 안된다( 죽는 동작이 없으므로)
					pclCM->CR[serverid]->bGodMode =  true;

					pclCM->CR[serverid]->pclCI->clBI.uiGMMode = (UI08)gmmode;

					pclCM->CR[serverid]->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );
				}
				else if (gmmode == 4)
				{
					// gmmode 4일경우는 죽어선 안된다( 죽는 동작이 없으므로)
					pclCM->CR[serverid]->bGodMode =  true;

					pclCM->CR[serverid]->pclCI->clBI.SetKind( pclClient->GetUniqueFromHash(TEXT("KIND_BERSERKER")) );

					cltWarpInfo clWarpInfo(1, pclCM->CR[serverid]->GetMapIndex(), false, false, pclCM->CR[serverid]->GetX(), pclCM->CR[serverid]->GetY(), -1, -1);
					((cltCharServer*)pclCM->CR[serverid])->SetWarpInfo(&clWarpInfo, 2100);

				}
				else
				{

					if (gmmode < 0 || gmmode > 2)
						gmmode = 0;
					//[추가 : 황진성 2007. 11. 30 gmmode 5 에서 god이 되면 풀기위해 추가.]
					if(0 == gmmode)
					{
						if(true == pclCM->CR[serverid]->bGodMode)
							pclCM->CR[serverid]->bGodMode = false;
					}

					pclCM->CR[serverid]->pclCI->clBI.uiGMMode = (UI08)gmmode;

					/*
					if(pclCM->CR[serverid]->pclCI->clHorseInfo.GetCurrentShowHorse() != NULL )
					{
						if(pclCM->CR[serverid]->pclCI->clHorseInfo.siCurrentHorseIndex == -1)
						{
							((cltCharServer*)pclCM->CR[serverid])->pclCI->clHorseInfo.Init();
							//((cltCharServer*)pclCM->CR[serverid])->pclCI->clHorseInfo.siCurrentShowHorse = 0;
						 	//((cltCharServer*)pclCM->CR[serverid])->pclCI->clHorseInfo.
							((cltCharServer*)pclCM->CR[serverid])->SetUpdateSwitch(UPDATE_HORSE, true, 0);
						}
					}
					
					*/
					pclCM->CR[serverid]->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );

				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	//cyj 군주 후보자 투표집계 명령 추가
	else if(_tcscmp(order, TEXT("voteaggregate")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if(pclCM->CR[serverid])
			{
				sDBRequest_VoteKingAggregate clMsg( serverid, VOTE_STATUS_TYPE_RESULT );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg);
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	// 선거 유세기간 설정
	else if(_tcscmp(order, TEXT("votecampagin")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			// 현재 모드에 따라서 토글시킴
			if ( true == bVoteCampaign )
			{
				bVoteCampaign = false;
				SendServerResponseMsg( 0, SRVAL_GAMEMASTER_VOTECAMPAGIN, 0, 0, serverid );
			}
			else
			{
				bVoteCampaign = true;
				SendServerResponseMsg( 0, SRVAL_GAMEMASTER_VOTECAMPAGIN, 1, 0, serverid );
			}
			
			SendServerInfo(0);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	else if(_tcscmp(order, TEXT("voteresult")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if(pclCM->CR[serverid])
			{
				sDBRequest_VoteKingAggregate clMsg( serverid, VOTE_STATUS_TYPE_WRITE_RESULT );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg);
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}

	//cyj 보너스 타임 시작 메뉴 추가
	else if(_tcscmp(order, TEXT("sbonus")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 bonusPercent = 0;

			_stscanf(&pstring[1], TEXT("%s %d"), 
				order, &bonusPercent );

			if (bBonusTimeSwitch == FALSE)
			{
				bBonusTimeSwitch = TRUE;
				bGMBonusTimeSwitch = 1;
				bonusPercent  -= 100;

				if( bonusPercent > 0 )
				{
					// 1 ~  200 사이의 값만 유효하므로 200을 넘으면 200이 된다
					if( bonusPercent > 300 )
					{
						bonusPercent = 300;
					}
					siGMBonusTimePercent = bonusPercent;
				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	//cyj 보너스 타임 종료 메뉴 추가
	else if(_tcscmp(order, TEXT("ebonus")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if (bBonusTimeSwitch == TRUE)
			{
				bBonusTimeSwitch = FALSE;
				bGMBonusTimeSwitch = 1;
				siGMBonusTimePercent = 0;
				bBonusTimeGoonzu = false;
			}

		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	//cyj 제조보너스 타임 시작 메뉴 추가
	else if(_tcscmp(order, TEXT("smakebonus")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 bonusPercent = 0;

			_stscanf(&pstring[1], TEXT("%s %d"), 
				order, &bonusPercent );

			if (bBonus2TimeSwitch == FALSE)
			{
				bBonus2TimeSwitch = TRUE;
				bGMBonusTimeSwitch = 2;
				bonusPercent  -= 100;

				if( bonusPercent > 0 )
				{
					// 1 ~  200 사이의 값만 유효하므로 200을 넘으면 200이 된다
					if( bonusPercent > 200 )
					{
						bonusPercent = 200;
					}
					siGMBonusTime2Percent = bonusPercent; 
				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	//cyj 제조보너스 타임 종료 메뉴 추가
	else if(_tcscmp(order, TEXT("emakebonus")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if (bBonus2TimeSwitch == TRUE)
			{
				bBonus2TimeSwitch = FALSE;
				bGMBonusTimeSwitch = 2;
				siGMBonusTime2Percent = 0;
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	else if(_tcscmp(order, TEXT("eventenable")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR szEventName[128];
			SI32  siEnable = 0;

			_stscanf(&pstring[1], TEXT("%s %s %d"), order, szEventName, &siEnable);

			cltEventTime* pclEvent = pclClient->pclEventTimeManager->GetEvent(szEventName);
			if( pclEvent == NULL )				return false;


            pclEvent->SetActivated( (siEnable == 1), true );

			NTCHARString32 kEnable;
			if(siEnable == 1)		kEnable = "enable";
			else					kEnable = "disable";

			SendServerResponseCharMsg2(0, SRVAL_GAMEMASTER_EVENTENABLE, szEventName, (TCHAR*)kEnable, "", serverid);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	//cyj 자기자신의 마을유니크를 설정한다..
	else if(_tcscmp(order, TEXT("setvillage")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if(pclCM->CR[serverid] && pclCM->CR[serverid]->pclCI->clBI.uiGMMode == GMMODE_ATB_GMSHOW)
			{
				SI32 villageunique = 0;

				_stscanf(&pstring[1], TEXT("%s %d"), 
					order, &villageunique );

				if (villageunique < 0 || villageunique > MAX_VILLAGE_NUMBER)
					villageunique = 0;

                pclCM->CR[serverid]->pclCI->clBI.siHomeVillage = (SI08)villageunique;
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	//cyj 자동배당 실시
	else if(_tcscmp(order, TEXT("autodividend")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 tempVillageUnique = 0;

			_stscanf(&pstring[1], TEXT("%s %d"), 
				order, &tempVillageUnique );

			cltStrInfo *pStrInfo = pclCityHallManager->GetStrInfo( tempVillageUnique );
			if(pStrInfo == NULL)
				return FALSE;

			// 배당될 총 금액
			SI64 tempDividendMoney = pStrInfo->clCityhallStrInfo.clProfitMoney.itMoney * 9 / 10;

			if( tempDividendMoney < MIN_AUTODIVIEND_MONEY )
				return FALSE;

			// 1. 주주가 먹을 분량
			if( pStrInfo->clCityhallStrInfo.siTotalStockAmount <= 0 )
				return FALSE;
			cltMoney tempStockDividendMoney;
			tempStockDividendMoney.Set( tempDividendMoney * 55 / 100 );

			if( tempStockDividendMoney.itMoney / pStrInfo->clCityhallStrInfo.siTotalStockAmount <=0 )
				return FALSE;

			// 2. 주민이 먹을 분량
			if( pStrInfo->clCityhallStrInfo.siResidentNum <= 0 )
				return FALSE;

			cltMoney tempResidentDividendMoney;
			tempResidentDividendMoney.Set( tempDividendMoney * 3 / 10 );

			if( tempResidentDividendMoney.itMoney / pStrInfo->clCityhallStrInfo.siResidentNum <=0 )
				return FALSE;

			// 3. 왕실 세금 납부
			cltMoney tempTaxDividendMoney;
			tempTaxDividendMoney.Set( tempDividendMoney * 9 / 100 );

			if( tempTaxDividendMoney.itMoney <= 0 )
				return FALSE;

			// 4. 대행수 월급
			SI64 tempChiefDividendMoney = tempDividendMoney * 5 / 100;
			// 5. 군주 월급
			SI64 tempKingDividendMoney = tempDividendMoney * 1 / 100;

			SI64 tempAllMoney = tempStockDividendMoney.itMoney + tempResidentDividendMoney.itMoney + tempTaxDividendMoney.itMoney +
				tempChiefDividendMoney + tempKingDividendMoney;

			sDBRequest_AutoPayDividend clMsg(1, tempVillageUnique, &tempStockDividendMoney, &tempResidentDividendMoney, &tempTaxDividendMoney,
				tempChiefDividendMoney, tempKingDividendMoney, 1, pclTime->clDate.uiYear, &pclTime->clDate );

			pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_DIVIDEND, tempResidentDividendMoney.itMoney);
			pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_DIVIDEND, tempChiefDividendMoney);
			pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_DIVIDEND, tempKingDividendMoney);

			pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	//cyj 자동배당 중지 설정
	else if(_tcscmp(order, TEXT("stopautodividend")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			bAutoPayDividend = !bAutoPayDividend;
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	else if(_tcscmp(order,TEXT("setstockoffer")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR VillageName[128];
			SI32 VillageUnique = 0 ;

			SI32 MaxStockOfferNum = 0;			
            SI32 DeviededOnePerson = 0;
			SI32 StockPrice = 0;

			_stscanf(&pstring[1], TEXT("%s %s %d %d %d"), order, VillageName
				,&MaxStockOfferNum, &DeviededOnePerson, &StockPrice);

			VillageUnique = pclVillageManager->FindVillageUniqueFromName( VillageName );
			if(VillageUnique < 0 ) return FALSE;
			
			// [영훈] 초보자 마을 : 초보자 마을은 주식공모 안되게
			if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
			{
				if ( VillageUnique == Const_Beginner_Village )
				{
					return FALSE;
				}
			}

			//todo:
			sDBRequest_SetStockOffering clMsg( VillageUnique, MaxStockOfferNum, DeviededOnePerson, StockPrice );
			pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );

		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	else if(_tcscmp(order,TEXT("endstockoffer")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR VillageName[128];
			SI32 VillageUnique = 0 ;

			_stscanf(&pstring[1], TEXT("%s %s"), order, VillageName );

			VillageUnique = pclVillageManager->FindVillageUniqueFromName( VillageName );
			if(VillageUnique < 0 ) return FALSE;

			//todo:

			sDBRequest_EndStockOffering clMsg( VillageUnique );
			pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	else if(_tcscmp(order,TEXT("lotterystock")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR VillageName[128];
			SI32 VillageUnique = 0 ;

			_stscanf(&pstring[1], TEXT("%s %s"), order, VillageName);

			VillageUnique = pclVillageManager->FindVillageUniqueFromName( VillageName );
			if(VillageUnique < 0 ) return FALSE;
			//todo:


			if( !pclStockOfferingManager->bLottery)
			{
				pclStockOfferingManager->bLottery = TRUE;

				pclStockOfferingManager->siVillageUnique = VillageUnique;
				pclStockOfferingManager->siCount = 0;

				sDBRequest_GetStockOfferingOrderList clMsg( VillageUnique );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	else if(_tcscmp(order,TEXT("skoinobory")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclKoinoboryFamilyManager->StartEvent();
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	else if(_tcscmp(order,TEXT("ekoinobory")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			pclKoinoboryFamilyManager->EndEvent();
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	else if(_tcscmp(order,TEXT("makehorse")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{

			SI32 HorseUnique = 0 ;


			_stscanf(&pstring[1], TEXT("%s %d"), order, &HorseUnique);

			if (HorseUnique <= 0) return FALSE;

			SI32 speed			= 0;
			SI32 sistr, sidex, simag;

			TCHAR* pText = GetTxtFromMgr(667);
			TCHAR horsename[128];
			StringCchCopy(horsename, 128, pText);

			switch (HorseUnique)
			{
			// 갈색마,흑마,백마는 대만만 초기능력치 다르게 설정함 (PCK- 2007.09.11)
			// 갈색마
			case 1 :
				if(pclClient->IsCountrySwitch(Switch_HorseAbility))
				{
					speed = 1400;
					sistr = 500;
					sidex = 500;
					simag = 500;
				}
				else
				{
					speed = 500 + (300  - (rand() % 600));
					sistr	= 50 + rand() % 10;
					sidex	= 50 + rand() % 10;
					simag	= 50 + rand() % 10;
				}
				break;

			// 흑마
			case 2 :
				pText = GetTxtFromMgr(2806);
				StringCchCopy(horsename, 128, pText);

				if(pclClient->IsCountrySwitch(Switch_HorseAbility))
				{
					speed = 1400;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;
				}
				else
				{
					speed = 800;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;
				}
				break;


			// 백마
			case 3 :
				pText = GetTxtFromMgr(2805);
				StringCchCopy(horsename, 128, pText);

				if(pclClient->IsCountrySwitch(Switch_HorseAbility))
				{
					speed = 1400;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;
				}
				else
				{
					speed = 800;
					sistr	= 500;
					sidex	= 500;
					simag	= 500;
				}
				break;

			// 적토마	
			case 4 :
				pText = GetTxtFromMgr(2753);
				StringCchCopy(horsename, 128, pText);

				speed = 800;
				sistr	= 500;
				sidex	= 500;
				simag	= 500;

				break;
			// 루돌프
			case 5 :
				pText = GetTxtFromMgr(3370);
				StringCchCopy(horsename, 128, pText);

				speed = 800;
				sistr	= 500;
				sidex	= 500;
				simag	= 500;

				break;

				// 천리마
			case 7 :
				pText = GetTxtFromMgr(2791);
				StringCchCopy(horsename, 128, pText);

				speed = 800;
				sistr	= 500;
				sidex	= 500;
				simag	= 500;

				break;
				// 철기마
			case 8 :
				pText = GetTxtFromMgr(5174);
				StringCchCopy(horsename, 128, pText);

				speed = 800;
				sistr	= 500;
				sidex	= 500;
				simag	= 500;

				break;
			case HORSEUNIQUE_WOLF:
				pText = GetTxtFromMgr(5622);
				StringCchCopy(horsename, 128, pText);

				speed = 2000;
				sistr	= 5000;
				sidex	= 5500;
				simag	= 4500;

				break;

				// 얼룩마
			case 10 :
				pText = GetTxtFromMgr(2769);
				StringCchCopy(horsename, 128, pText);

				speed = 800;
				sistr	= 500;
				sidex	= 500;
				simag	= 500;

				break;

				// 구미호
			case 11 :
				pText = GetTxtFromMgr(6418);
				StringCchCopy(horsename, 128, pText);

				speed = 800;
				sistr	= 500;
				sidex	= 500;
				simag	= 500;

				break;
			//KHY - 0622 - 군주S 초보자용  망아지 추가.
			case HORSEUNIQUE_FOAL :
				pText = GetTxtFromMgr(7505);
				StringCchCopy(horsename, 128, pText);

				speed = 1400;
				sistr	= 500;
				sidex	= 500;
				simag	= 500;

				break;
			//KHY - 0911 - 호랑이 추가.
			case HORSEUNIQUE_TIGER:
				pText = GetTxtFromMgr(5394);
				StringCchCopy(horsename, 128, pText);

				speed = 2000;
				sistr	= 5500;
				sidex	= 5000;
				simag	= 4500;

				break;
				// 체험용 백마 : 손성웅-2009.03.17
			case HORSEUNIQUE_WHITE_TEST:
				pText = GetTxtFromMgr(9897);
				StringCchCopy(horsename, 128, pText);

				speed = 1500;
				sistr	= 2000;
				sidex	= 2000;
				simag	= 2000;

				break;
			

			// 기본
			default :
				HorseUnique = 1;
				speed = 500 + (300  - (rand() % 600));
				sistr	= 50 + rand() % 10;
				sidex	= 50 + rand() % 10;
				simag	= 50 + rand() % 10;
				break;

			}

			//------------------------------
			// 성별 결정. 
			//------------------------------
			SI32 sisex = ANIMAL_MALE;
			if(rand() % 5 == 0)
			{
				sisex = ANIMAL_FEMALE;
			}

			cltServer *pclserver = (cltServer*)pclClient;

			SI32 curhour = pclserver->pclTime->clDate.GetHourVary();

			SI32 randomvalue = rand() % 10000;
			TCHAR randomstr[128]=TEXT("");
			_itot(randomvalue, randomstr, 10);
			StringCchCat(horsename, 128, randomstr);

			cltDate cldate;
			cldate.MakeFullDateFromDateVary(0);

			cltHorse clHorse(HorseUnique, horsename, &pclserver->pclTime->clDate, sisex, 1, 0,
				sistr, sidex, simag, speed, curhour,0,&cldate,0
				,&cldate,0
				,&cldate,0
				,&cldate,0
				,0);

			if(pclserver->pclTime->clDate.uiMonth == 0)
			{
				TCHAR* pText = GetTxtFromMgr(2807);
				if (pclClient->GameMode == GAMEMODE_SERVER)
					pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), pText, 
					pclserver->pclTime->clDate.uiYear, pclserver->pclTime->clDate.uiMonth, pclserver->pclTime->clDate.uiDay);
			}


			// 내 소유의 말을 추가한다. 
			SI32 index;
			if(pclchar->pclCI->clHorseInfo.Add(&clHorse, &index) == true)
			{
				//KHY - 1001 - 기승동물 5마리 보유 수정.
				pclchar->pclCI->clHorseInfo.SetCurrentShowHorseIndex(index);
				// DB에 말을 추가한다.  // index,
				pclchar->UpdateHorseToDB((cltServer*)pclClient, index, &clHorse, SETHORSE_REASON_MAKE);

				// 주인과 이웃에게 말 정보를 보낸다. 
				pclchar->SendNeighbourMyHorseStatus(index);

				//cyj 말 생산하면 말인벤토리에 있는 아이템 정보 가져오도록 추가
				sDBRequest_PersonItemInfoGet clMsg(pclchar->siID, pclchar->pclCI->GetPersonID());
				pclserver->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg);

			}

		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	else if(_tcscmp(order, TEXT("setworldmoneyprice"))== 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI64 price = 0 ;
			sscanf(&pstring[1], "%s %I64d", order, &price);

			sDBRequest_WorldMoney_SetAveragePrice clMsg(price);
			pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader*)&clMsg);

			SendServerResponseMsg( 0, SRVAL_SUCCESS_WORLDMONEY_SETPRICE, 0, 0, serverid );
		}
		else
		{
			bnotenoughmasterlevel = true ;
		}
	}
	else if(_tcscmp(order, TEXT("setservertime"))== 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siYear		= 0;
			SI32 siMonth	= 0;
			SI32 siDay		= 0;

			SI32 siHour		= 0;
			SI32 siMinute	= 0;
			SI32 siSecond	= 0;

			SYSTEMTIME sysTime;
			
			bool bYearCheck		= false;
			bool bMonthCheck	= false;
			bool bDayCheck		= false;

			bool bHourCheck		= false;
			bool bMinuteCheck	= false;
			bool bSecondCheck	= false;

						
			sscanf(&pstring[1], "%s %d-%d-%d %d:%d:%d", order, &siYear, &siMonth, &siDay, &siHour, &siMinute, &siSecond);

			// 년도 값 체크
			bYearCheck		= ((siYear	 >= 2000)	&&	(siYear		<= 2100));
			// 달 값 체크
			bMonthCheck		= ((siMonth	 >= 1)		&&	(siMonth	<= 12));
			// 날 값 체크
			bDayCheck		= ((siDay	 >= 1)		&&	(siDay		<= 31));

			// 시간 값 체크
			bHourCheck		= ((siHour	 >= 0)		&&	(siHour		<= 23));
			// 분 값 체크
			bMinuteCheck	= ((siMinute >= 0)		&&	(siMinute	<= 59));
			// 초 값 체크
			bSecondCheck	= ((siSecond >= 0)		&&	(siSecond	<= 59));


			if ( bYearCheck && bMonthCheck && bDayCheck && bHourCheck && bMinuteCheck && bSecondCheck )
			{
				GetLocalTime( &sysTime );

				sysTime.wYear		= (UI16)siYear;
				sysTime.wMonth		= (UI16)siMonth;
				sysTime.wDay		= (UI16)siDay;

				sysTime.wHour		= (UI16)siHour;
				sysTime.wMinute		= (UI16)siMinute;
				sysTime.wSecond		= (UI16)siSecond;

				if ( SetLocalTime( &sysTime ) == FALSE )
				{
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			bnotenoughmasterlevel = true ;
		}

	}

	else if(_tcscmp(order,TEXT("worldmoney")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI64 num = 0 ;
			SI64 price = 0 ;
			SI64 total = 0 ;
			sscanf(&pstring[1], "%s %I64d %I64d %I64d", order, &num, &price , &total);

			pclDistributionWorldMoney->Set(TRUE,num,price,total,CurrentClock,600000);

			cltGameMsgResponse_SetWorldMoney_BuyDlg clinfo( num, price ,total);
			cltMsg clMsg(GAMEMSG_RESPONSE_SETWORLDMONEY_BUYDLG, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true ;
		}

	}
	else if(_tcscmp(order,TEXT("sttradetime")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if ( bTradeServer )
			{
				bTradeServer = false ;
				SendServerResponseMsg(0,SRVAL_SUCCESS_SET_TRADESERVER,0,0,pclCM->CR[serverid]->GetCharUnique());
			}
			else
			{
				bTradeServer = true ;
				SendServerResponseMsg(0,SRVAL_SUCCESS_SET_TRADESERVER,1,0,pclCM->CR[serverid]->GetCharUnique());
			}
		}

	}
	else if(_tcscmp(order,TEXT("settradeservertime")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 start = 0;
			SI32 end = 0;
			sscanf(&pstring[1], "%s %d %d", order, &start, &end);

			siTradeServerStartTime = start ;
			siTradeServerEndTime = end ;

			SendServerResponseMsg(0,SRAVL_SUCCESS_TRADESERVER_SETTIME,start,end,pclCM->CR[serverid]->GetCharUnique());
		}
	}
	else if(_tcscmp(order,TEXT("enchantsuccess")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 success = 0;
			sscanf(&pstring[1], "%s %d", order, &success);

			if ( success > 0 )
			{
				pclchar->bSuccessEnchant = true;
			}
			else
			{
				pclchar->bSuccessEnchant = false;
			}

		}
	}
	else if(_tcscmp(order, TEXT("giveworldmoney"))== 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI64 worldmoney = 0 ;
			sscanf(&pstring[1], "%s %d %I64d", order, &charunique, &worldmoney);

			// GM권한 100, 101은 charunique 0만 사용하도록
			if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
			{
				if (charunique != 0)
					return FALSE;
			}

			if ( worldmoney <= 0 )
				return FALSE ;

			if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

			SI32 id = 0 ;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
			if(personid)
			{
				sDBRequest_WorldMoney_Set_Char clMsg(id,personid,worldmoney);
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

				SendServerResponseMsg( 0, SRVAL_SUCCESS_WORLDMONEY_SET_CHAR, 0, 0, serverid );
			}
			else
			{
				SendServerResponseMsg( 0, SRVAL_FAIL_WORLDMONEY_SET_CHAR, id, 0, serverid );
			}
		}
		else
		{
			bnotenoughmasterlevel = true ;
		}
	}
	/* [영훈] tradeserver로 명령어 변경
	else if(_tcscmp(order, TEXT("stoptradeprocess"))== 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			bForceStopTradeProcess = true;
		}
		else
		{
			bnotenoughmasterlevel = true ;
		}
	}
	*/
	else if(_tcscmp(order, TEXT("requestmacro"))== 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI64 worldmoney = 0 ;
			TCHAR szSuspectName[MAX_PLAYER_NAME];

			sscanf(&pstring[1], "%s %s", order, szSuspectName);

			// 잘못된 이름이다.
			if ( _tcslen(szSuspectName) <= 0 || !_tcscmp(szSuspectName,TEXT("")) )
			{
				SendServerResponseMsg(0, SRVAL_AUTOHUNTING_FAIL_NAMEERROR,  0, 0, serverid);
				return FALSE;
			}

			cltCharServer *pclCharRequest = (cltCharServer *)pclCM->CR[serverid];
			if ( pclCharRequest == NULL ) return FALSE;

			// 없는 캐릭터이다.
			SI32 suspectid = pclCM->GetIDFromName(szSuspectName);
			if ( suspectid <= 0 )
			{
				SendServerResponseMsg(0, SRVAL_AUTOHUNTING_FAIL_NAMEERROR,  0, 0, serverid);
				return FALSE;
			}
			cltCharServer *pclCharSuspect = (cltCharServer *)pclCM->CR[suspectid];
			if ( pclCharSuspect == NULL ) return FALSE;

			TCHAR szName[MAX_PLAYER_NAME] = TEXT("") ;
			StringCchCopy(szName, MAX_PLAYER_NAME, pclCharRequest->GetName());

			// 사냥을 하지 않거나 사냥터가 아니다.
			if ( pclCharSuspect->IsAutoHunting() == false )
			{
				SendServerResponseMsg(0, SRVAL_AUTOHUNTING_FAIL_NOTHUNTING,  0, 0, serverid);
				return FALSE;
			}

			CMinisterHyungzo *pclminister = (CMinisterHyungzo *)pclMinisterMgr->GetMinister(MINISTER_HYUNGZO);
			if ( pclminister == NULL ) return FALSE;
			SI32 TimeLimit	= min(300, max(20, pclminister->m_siSuspectTime)) ;
			SI16 PrisonTime = min(60, max(5, pclminister->m_siPrisonTime)) ;
			SI16 penalty = pclminister->m_siPrisonerPenalty ;

			// 서버 오류

			const TCHAR* Txt = pclItemManager->GetRandomItemName();
			if ( Txt == NULL || _tcslen(Txt) <= 0 )
			{
				SendServerResponseMsg(0, SRVAL_AUTOHUNTING_FAIL_SERVERERROR,  0, 0, serverid);
				return FALSE;
			}

			// 이미 신고된 캐릭터이다.

			if ( pclClient->pclAutoHuntingManager->IsRequest(szSuspectName) == true )
			{
				SendServerResponseMsg(0, SRVAL_AUTOHUNTING_FAIL_ISREQUEST,  0, 0, serverid);
				return FALSE;
			}

			// 서버 오류
			SI32 slot = 0 ;
			if ( pclAutoHuntingManager->Set(Txt,szName,szSuspectName,
				&slot,pclCharRequest->pclCI->GetPersonID()) == false )
			{
				SendServerResponseMsg(0, SRVAL_AUTOHUNTING_FAIL_SERVERERROR,  0, 0, serverid);
				return FALSE;
			}

			cltGameMsgResponse_AutoHunting clinfo1(TimeLimit,slot,PrisonTime,penalty,Txt);
			cltMsg clMsg1(GAMEMSG_RESPONSE_AUTOHUNTING, 	sizeof(clinfo1), (BYTE*)&clinfo1);
			pclCM->CR[suspectid]->SendNetMsg((sPacketHeader*)&clMsg1);

			cltGameMsgResponse_AutoHunting_TimeUpdate clinfo2(0,TimeLimit);
			cltMsg clMsg2(GAMEMSG_RESPONSE_AUTOHUNTING_TIMEUPDATE, sizeof(clinfo2), (BYTE*)&clinfo2);
			pclCM->CR[suspectid]->SendNetMsg((sPacketHeader*)&clMsg2);

			// 의뢰 성공
			if ( IsPC(serverid) )
				SendServerResponseMsg(0,SRVAL_AUTOHUNTING_SUCCESS_REQUEST,0,0,serverid);


		}
		else
		{
			bnotenoughmasterlevel = true ;
		}
	}
	//[추가 : 황진성 2007. 12. 3 => 4차오토프로그램 방지 체크 명령어.]
	else if(_tcscmp(order, TEXT("macrocheck")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 Type = 0 ;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &Type );

			for ( SI32 i=0; i<MAX_PERSON_NUMBER; ++i )
			{
				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[i];
				if ( pclClient->pclCM->IsValidID(i) )
				{			
					cltGameMsgResponse_AutoMacroCheck AutoMacrocheck(Type);
					cltMsg clMsg(GAMEMSG_RESPONSE_AUTOMACRO_CHECK, sizeof(AutoMacrocheck), (BYTE*)&AutoMacrocheck);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}
	}

	else if(_tcscmp(order, TEXT("macromon"))== 0)
	{ 

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if((gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel) || policewitch )
		{
			// NPC이름을 근거로 NPC의 Kind를 찾는다. 
			SI32 kind = pclClient->GetUniqueFromHash(TEXT("KIND_JOKER")) ;

			SI32 id = serverid;
			if(id == 0)return FALSE;

			SI32 CreatedCharNum = 0;
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
			SI32 count = 0;

			SI32 x, y;

			while(count < 1)
			{
				if(pclchar->pclMap->FindEmptyAreaForCharLocate(kind, pclchar->GetX(), pclchar->GetY(), 20, &x, &y, true) == false)
				{
					continue;
				}

				pclchar->SetInitProtectSwitch();

				cltInnerStatusInfo clinnerstatus;
				clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

				// 도마돈은 100레벨이다.
				SI32 siSummonID = SetNPCChar(kind, VILLAGE_NONE, NULL, 100, 0, x, y, pclchar->GetMapIndex(), NPCITEMMODE_GM, &clinnerstatus );
				if ( siSummonID )
				{
					((cltCharServer*)pclCM->CR[ siSummonID])->dwTimerClock = CurrentClock;
					count ++ ;

				}
			}

		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if(_tcscmp(order, TEXT("autobonustime"))== 0)
	{ 
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 success = 0;
			sscanf(&pstring[1], "%s %d", order, &success);

			if ( success == 0 )
			{
				bDoNotAutoBonus = true;
			}
			else
			{
				bDoNotAutoBonus = false;
			}

		}
	}
	else if(_tcscmp(order, TEXT("givefp")) == 0)
	{
		SI32 charunique;
		SI32 recommandPoint;
		_stscanf(&pstring[1], TEXT("%s %d %d"), order, &charunique, &recommandPoint);

		// GM권한 100, 101은 charunique 0만 사용하도록
		if (gamemasterlevel >= BASIC_GAMEMASTER_LEVEL && gamemasterlevel <= BASIC_GAMEMASTER_LEVEL + 1)
		{
			if (charunique != 0)
				return FALSE;
		}


		if(charunique !=0 && pclCM->IsValidID(charunique)== FALSE)	return FALSE;

		SI32 id;
		if(charunique == 0)	{	id = serverid;		}
		else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
		if(id == 0)return FALSE;

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			sDBRequest_GiveFP clMsg( pclCM->CR[id]->pclCI->GetPersonID(), recommandPoint, RPGIVE_REASON_FROMGM);
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}

	else if(_tcscmp(order, TEXT("guildclose")) == 0)
	{

		TCHAR guildName[MAX_GUILD_NAME];

		_stscanf(&pstring[1], TEXT("%s %s"), order, guildName);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);
		SI32 guildUnique = 0;

		for ( SI32 i = 0 ; i<MAX_GUILD_NUM; i++ )
		{
			if ( _tcscmp ( pclClient->pclGuildManager->clUnit[i].szName , guildName ) == 0 )
			{
				guildUnique = pclClient->pclGuildManager->clUnit[i].siUnique;
			}
		}

		if ( guildUnique <= 0 ) return FALSE;

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel )
		{
			sDBRequest_Guild_DestroyGuild clMsg( pclchar->pclCI->GetPersonID(), pclchar->GetID(),guildUnique , 0, 1);
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if(_tcscmp(order, TEXT("settppoint")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 updateTeacherPoint = 0;
			SI32 charUnique = 0;
			sscanf(&pstring[1], "%s %d %d", order, &charUnique, &updateTeacherPoint);

			SI32 id = 0;
			if( charUnique == 0 )	
			{	id = serverid;		}
			else
			{
				id = pclCM->GetIDFromCharUnique( charUnique );
			}
			if( id == 0 )
				return FALSE;

		//	sDBRequest_SetTeacherPoint clSetTeacherPoint( pclCM->CR[id]->pclCI->clBI.clFatherInfo.clFather.GetPersonID(), updateTeacherPoint );
		//	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clSetTeacherPoint);
		}
	}
	else if(_tcscmp(order, TEXT("kingguri")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 type = 0;
			sscanf( &pstring[1], "%s %d", order, &type );
			
			if( 0 == type )
			{
				pclKingGuriEventManager->End();
			}
			else if( 1== type )
			{
				pclKingGuriEventManager->MakeKingGuri();
			}
		}
	}
	else if(_tcscmp(order, TEXT("limituser")) == 0)
	{
		// kkm-
		if( ConstServiceArea_China == siServiceArea )
		{
			SI32 ref = pclGMManager->FindUniqueFromOrder(order);

			if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
			{
				SI32 limitMaxUser = 0;
				sscanf(&pstring[1], "%s %d", order, &limitMaxUser);

				siLimitMaxUserNumber = limitMaxUser;
			}
			else
			{
				bnotenoughmasterlevel = true;
			}
		}
		// -kkm
	}
	// 마을 주민들에게 아이템을 준다. (역참)	 
	else if(_tcscmp(order, TEXT("givevillageitem")) == 0) // 
	{
		// [영진]
	
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR	villageName[256];
			villageName[0] = 0;
			SI32	villageUnique = 0;			
			TCHAR	itemName[256];
			itemName[0] = 0;
			SI32	itemNum = 0;
			SI32	useDate = 0;
			TCHAR	msg[256];
			msg[0] = 0;

			sscanf( &pstring[1] , "%s %s %s %d %d %s" , order , villageName , itemName , &itemNum , &useDate , msg );
			
			// 마을 이름을 근거로 Unique를 구한다.
			villageUnique = pclVillageManager->GetMapIndexFromName(villageName);
            
			// 입력된 값의 유효성 체크
			if(villageUnique <= 0 || villageUnique >= MAX_VILLAGE_NUMBER ) return FALSE;

			if(itemNum <= 0 )return FALSE;
			if( useDate <= 0 ) return FALSE;

			ConvertUnderLineToSpace( itemName, 256 );

			if(itemNum <= 0)	itemNum = 1;
			if(itemNum > MAX_ITEM_PILE_NUMBER)	itemNum = MAX_ITEM_PILE_NUMBER;

			

			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemName);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemName );
				if(itemunique <= 0)return FALSE;
			}

			// 아이템 만들기 
			cltItem	clItem;
			SI16 brareswitch = 0;
			if( pclItemManager->MakeRandItemUnique( itemunique, &clItem, 0, 0, &brareswitch ,0,0,0,0,0,useDate) == false ) return FALSE;

			// 중첩 가능한 아이템이 아니면 갯수는 1개로 고정
			if(clItem.CanPile( pclItemManager ) == false)
			{
				clItem.SetItemNum(1);
			}
			else
			{
				clItem.SetItemNum(itemNum);
			}

			sDBRequest_GiveVillageItem clMsg( pclchar->pclCI->GetPersonID(), villageUnique ,  &clItem , msg, useDate );
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

			// LEEKH MODIFY 2007.10.25
			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
			//{
			//	// [로그추가 : 황진성 2007. 10. 11] // 마을주민에게 아이템 주기
			//	// param1 == 사용기간
			//	// charparam1 == 
			//	cltServer * pclServer = (cltServer*)pclClient;
			//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CITYHALL, LOGCOMMAND_INDEX_CITYHALL_VILLAGEGIVEITEM, 
			//									0, pclchar, NULL, 0, 0, 0, &clItem, 0, 0, 0, villageUnique, 0, 0, 
			//									useDate, 0, 0, 0, 0, msg, NULL);
			//}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	} 
	else if(_tcscmp(order, TEXT("itemsupply")) == 0) // 
	{
		// [영진]

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR	itemName[256];
			itemName[0] = 0;
			SI32	itemNum = 0;
			SI32	useDate = 0;
			TCHAR	msg[256];
			msg[0] = 0;

			sscanf( &pstring[1] , "%s %s %d %d %s" , order , itemName , &itemNum , &useDate , msg );

			if( itemName[0] == 0 ) return FALSE;
			if(itemNum <= 0 )return FALSE;
			if( useDate <= 0 ) return FALSE;
			if( msg[0] == 0) return FALSE;

			ConvertUnderLineToSpace( itemName, 256 );

			if(itemNum <= 0)	itemNum = 1;
			if(itemNum > MAX_ITEM_PILE_NUMBER)	itemNum = MAX_ITEM_PILE_NUMBER;

			// 아이템 이름을 근거로 Unique를 구한다.
			SI32 itemunique = pclItemManager->FindUniqueFromName(itemName);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( itemName );
				if(itemunique <= 0)return FALSE;
			}

			// 아이템 만들기 
			cltItem	clItem;
			SI16 brareswitch = 0;
			if( pclItemManager->MakeRandItemUnique( itemunique, &clItem, 0, 0, &brareswitch ,0,0,0,0,0,useDate) == false ) return FALSE;

			// 중첩 가능한 아이템이 아니면 갯수는 1개로 고정
			if(clItem.CanPile( pclItemManager ) == false)
			{
				clItem.SetItemNum(1);
			}
			else
			{
				clItem.SetItemNum(itemNum);
			}
			
			bool bResult = pclGMManager->SetSupplyItem(&clItem);
			
			if( bResult == false )
			{
				return FALSE;				
			}

			pclGMManager->SetSupplyMsg( msg );
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	else if(_tcscmp(order, TEXT("itemsupplystop")) == 0) // 
	{
		// [영진]

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			cltItem clItem;
			clItem.Init();

			pclGMManager->SetSupplyItem( &clItem );			
		}
		else
		{
			bnotenoughmasterlevel = true;
		}

	}
	else if(_tcscmp(order, TEXT("mofuenemycountset")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if( gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel )
		{
			SI32 count = 0;
			sscanf( &pstring[1] , "%s %d" , order , &count );

			count = count / 3 * 3;  // 3의 배수로 보정

			if ( count < 30 )		// 30 마리 이하는 설정 불가
			{
				SendServerResponseMsg( 0, SRVAL_MOFUMOFUEVENT_FAIL_SETENEMYCOUNT, 0, 0, serverid );
			}

			if ( pclMofuMofuEvent->GetCurrentEventVillage() >= 0 )
			{
				SendServerResponseMsg( 0, SRVAL_MOFUMOFUEVENT_FAIL_SETENEMYCOUNT, 1, 0, serverid );
			}
			cltMofuMofuEvent::m_siMaxEnemyCount = count;

			SendServerResponseMsg( 0, SRVAL_MOFUMOFUEVENT_SUCCESS_SETENEMYCOUNT, count, 0, serverid );
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if(_tcscmp(order, TEXT("build")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if( gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel )
		{
			SI32	villageUnique	= 0;
			SI32	slot			= 0;
			SI32	structureUnique = 0;
			TCHAR	szVillageName[64];
			TCHAR	szStructureName[64];
			sscanf( &pstring[1] , "%s %s %d %s" , order, szVillageName, &slot, szStructureName );

			NTCHARString64 kVillageName(szVillageName);
			kVillageName.Replace("_", " ");

			// 마을 이름을 기준으로 마을 유니크를 찾는다.
			villageUnique = pclClient->pclVillageManager->FindVillageUniqueFromName(kVillageName);
			// 마을 이름을 기준으로 마을 유니크를 찾지 못했으면, 입력한 숫자를 마을 유니크로 사용한다.
			if( villageUnique <= 0 )
			{
				villageUnique = _ttoi(kVillageName);
			}
			if( villageUnique < 0 || villageUnique >= MAX_VILLAGE_NUMBER )	return FALSE;

			NTCHARString64 kStructureName(szStructureName);
			kStructureName.Replace("_", " ");
			cltVillageStructureInfo* pclVillageStructureInfo = NULL;

			// 건물의 이름을 기준으로 건물 유니크를 찾는다.
			for(SI32 i=0; i<MAX_VILLAGE_STRUCTURE_NUMBER; i++)
			{
				pclVillageStructureInfo = pclClient->pclVillageStructureSet->pclVillageStructureInfo[i];
				if( pclVillageStructureInfo == NULL )			continue;
				if( _tcscmp(pclVillageStructureInfo->szName, kStructureName) == 0 )
				{
					structureUnique = pclVillageStructureInfo->siUnique;
					break;
				}
			}
            
			// 이름으로 찾지 못하면, 입력된 숫자를 건물유니크로 사용한다.
			if( structureUnique <= 0 )
			{
				structureUnique = _ttoi(kStructureName);
			}

			// 건물 유니크를 기준으로 건물 정보를 찾는다.
			for(SI32 i=0; i<MAX_VILLAGE_STRUCTURE_NUMBER; i++)
			{
				pclVillageStructureInfo = pclClient->pclVillageStructureSet->pclVillageStructureInfo[i];
				if( pclVillageStructureInfo == NULL )			continue;
				if( pclVillageStructureInfo->siUnique == structureUnique )
				{
					break;
				}
			}
			if( pclVillageStructureInfo == NULL )				return FALSE;

			// 건물을 건설한다.
			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			if( id )
			{
				sDBRequest_BuildStructure sendMsg( id, villageUnique, slot, structureUnique, 0, BUILD_BY_MASTER, pclVillageStructureInfo->siNeedVillageScore );
				SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&sendMsg);
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if(_tcscmp(order, TEXT("bankruptcy")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if( gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel )
		{
			if( bBankruptcy )
			{
				SI32 type = 0;
				sscanf( &pstring[1] , "%s %d" , order , &type );

				SI32 id;
				if(charunique == 0)	{	id = serverid;		}
				else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
				if(id == 0)return FALSE;

				if( id )
				{
					if( 0 == type )
					{
						SI32 startDate = pclTime->clDate.GetDateVary();
						sDBRequest_VillageBankruptcy sendMsg( id, 0, sDBRequest_VillageBankruptcy::BANKRUPTY );
						SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&sendMsg);
					}
					else if( 1 == type )
					{
						SI32 startDate = pclTime->clDate.GetDateVary();
						sDBRequest_VillageBankruptcy sendMsg( id, startDate, sDBRequest_VillageBankruptcy::BANKRUPTY );
						SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&sendMsg);
					}
					else if( 2 == type )
					{
						SI32 startDate = pclTime->clDate.GetDateVary();
						sDBRequest_VillageBankruptcy sendMsg( 0, 0, sDBRequest_VillageBankruptcy::RECOVERY );
						SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&sendMsg);
					}
				}
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if(_tcscmp(order, TEXT("usercount")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if( gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel )
		{
			SI32 mapindex = 0;
			sscanf( &pstring[1] , "%s %d" , order , &mapindex );

			SI32 id;
			if(charunique == 0)	{	id = serverid;		}
			else				{	id = pclCM->GetIDFromCharUnique(charunique);			}
			if(id == 0)return FALSE;

			if( id )
			{
				if ( mapindex <= 0 )
				{
					mapindex = pclchar->GetMapIndex();
				}
				SI32 count = pclClient->pclMapManager->GetUserCount( mapindex );
				SendServerResponseMsg( 0, SRVAL_RETURN_USERCOUNT, count, 0, serverid );
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 패킷을 사용을 제한한다. - 시스템 제한
	else if(_tcscmp(order, TEXT("packetenable")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siCmd = 0;
			SI32 siEnable = 0;
			_stscanf(&pstring[1], TEXT("%s %d %d"), order, &siCmd, &siEnable);

			if( siCmd < 0 && siCmd >= GAMEMSG_END )		return FALSE;
			if( m_pbUseMsg == NULL )					return FALSE;

			m_pbUseMsg[siCmd] = (bool)(siEnable != 0);

			if( m_pbUseMsg[siCmd] )
				SendServerResponseMsg(0, SRVAL_GAMEMASTER_SYSTEM_PACKET_ENABLE, siCmd, 1, serverid);
			else
				SendServerResponseMsg(0, SRVAL_GAMEMASTER_SYSTEM_PACKET_ENABLE, siCmd, 0, serverid);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 패킷압축 최소 크기를 설정한다
	else if(_tcscmp(order, TEXT("packetcompsize")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siSize = 0;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &siSize);

			if( siSize < 0 )		siSize = 0;
			if( siSize > 11000 )	siSize = 11000;

			m_uiCompressMinSize = siSize;
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 밀봉상태의 아이템을 우체국으로 지급
	else if(_tcscmp(order, TEXT("postsealup")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR	szItemname[128];
			ZeroMemory(szItemname, 128);
			SI32	siUseDay = 1;
			TCHAR	szPersonname[MAX_PLAYER_NAME];

			_stscanf(&pstring[1], TEXT("%s %s %s %d %d"), order, szPersonname, szItemname, &siUseDay);

			if(siUseDay <= 0 )return FALSE;

			ConvertUnderLineToSpace( szItemname , 128);

			if( siUseDay < 1 )			return FALSE;

			SI32 itemunique = pclItemManager->FindUniqueFromName(szItemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( szItemname );
				if(itemunique <= 0)		return FALSE;
			}

			// 아이템 만들기 
			cltItem	clItem;
			SI16 brareswitch = 0;
			if( pclItemManager->MakeRandItemUnique( itemunique, &clItem, 0, 0, &brareswitch, 0, 0, 0, 0, 0, siUseDay ) == false )	return FALSE;

			if(clItem.CanPile( pclItemManager ) == true)
			{
				// 중첩가능한 아이템은 기간아이템이 될수 없으므로 생성할 수 없다.
				return FALSE;
			}
			// 중첩 불가이기 때문에 무조건 하나만 지급.
			clItem.SetItemNum( 1 );

			// 밀봉이 풀린 아이템도 다시 밀봉한다.
			clItem.uiDateYear	= 0;
			clItem.uiDateMonth	= 0;
			clItem.uiDateDay	= 0;

			sDBRequest_PostOfficeMasterSendItem clMsg( 0, 0, szPersonname, &clItem, &pclTime->clDate );
			// 실행한 운영자의 캐릭터ID 설정
			clMsg.usCharID = serverid;
			clMsg.siReason = 100;

			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	// 밀봉이 풀린상태의 아이템을 우체국으로 지급
	else if(_tcscmp(order, TEXT("postsealoff")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			TCHAR	szItemname[128];
			ZeroMemory(szItemname, 128);
			SI32	siUseDay = 1;
			TCHAR	szPersonname[MAX_PLAYER_NAME];

			_stscanf(&pstring[1], TEXT("%s %s %s %d %d"), order, szPersonname, szItemname, &siUseDay);

			if(siUseDay <= 0 ) return FALSE;

			ConvertUnderLineToSpace( szItemname, 128 );

			if( siUseDay < 1 )			return FALSE;

			SI32 itemunique = pclItemManager->FindUniqueFromName(szItemname);
			if(itemunique <= 0)
			{
				itemunique = _ttoi( szItemname );
				if(itemunique <= 0)		return FALSE;
			}

			// 아이템 만들기 
			cltItem	clItem;
			SI16 brareswitch = 0;
			if( pclItemManager->MakeRandItemUnique( itemunique, &clItem, 0, 0, &brareswitch, 0, 0, 0, 0, 0, siUseDay ) == false )	return FALSE;

			if(clItem.CanPile( pclItemManager ) == true)
			{
				// 중첩가능한 아이템은 기간아이템이 될수 없으므로 생성할 수 없다.
				return FALSE;
			}
			// 중첩 불가이기 때문에 무조건 하나만 지급.
			clItem.SetItemNum( 1 );

			// 시간을 설정해서 밀봉을 해제한다.
			clItem.uiDateYear	= sTime.wYear - 2000;
			clItem.uiDateMonth	= sTime.wMonth;
			clItem.uiDateDay	= sTime.wDay;

			sDBRequest_PostOfficeMasterSendItem clMsg( 0, 0, szPersonname, &clItem, &pclTime->clDate );
			// 실행한 운영자의 캐릭터ID 설정
			clMsg.usCharID = serverid;
			clMsg.siReason = 100;

			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if(_tcscmp(order, TEXT("prisontimeset")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if( gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel )
		{
			SI32 prisontime = 0;
			sscanf( &pstring[1], "%s %d", order, &prisontime );

			CMinisterHyungzo *pclminister = (CMinisterHyungzo*)pclMinisterMgr->GetMinister(MINISTER_HYUNGZO);
			if ( pclminister == NULL ) return FALSE;

			if ( prisontime < 5 || prisontime > 60 )
				return FALSE;

			sDBRequest_SetHyungzoInfo clMsg(pclchar->GetID(), pclchar->pclCI->GetPersonID(),
				pclminister->m_szGreeting, pclminister->m_siLimitStealNum,
				pclminister->m_siSuspectTime,prisontime,pclminister->m_siPrisonerPenalty);

			((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	//[추가 : 황진성 2008. 2. 20 => 주주총회 발동 가능.]
	else if(_tcscmp(order, TEXT("cityhallon")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siViilageNumber = 0;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &siViilageNumber );
			if( 0 < siViilageNumber && siViilageNumber < MAX_VILLAGE_NUMBER)
			{
				sDBRequest_GeneralMeeting_Suggest_On_Off sendMsg( siViilageNumber, true );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
			}
		}
	}
	//[추가 : 황진성 2008. 2. 20 => 주주총회 발동 불가.]
	else if(_tcscmp(order, TEXT("cityhalloff")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);
		
		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siViilageNumber = 0;
			_stscanf(&pstring[1], TEXT("%s %d"), order, &siViilageNumber );
			if( 0 < siViilageNumber && siViilageNumber < MAX_VILLAGE_NUMBER)
			{
				sDBRequest_GeneralMeeting_Suggest_On_Off sendMsg( siViilageNumber, false );
				pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
			}
		}
	}
	//[추가 : 황진성 2008. 2. 20 => 주주총회 발동 발동 가능과 불가능 여부 표시.]
	else if(_tcscmp(order, TEXT("cityhallinfo")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI32 siViilageNumber = 0;
			
			SI32 id = 0;
			if(charunique == 0)	
			{	
				id = serverid;		
			}
			else
			{	
				id = pclCM->GetIDFromCharUnique(charunique);		
			}
			
			if(id == 0)
				return FALSE;
			
			//SendServerResponseMsg(0, SRVAL_GENERAL_MEETING_SUGGEST_ON_OFF_INFO, 0, 0, pclCM->CR[id]->GetCharUnique());

			cltVillageInfo* pVillageInfo = NULL;
			cltGameMsgResponse_VillageGeneralMeeting clinfo;
			for( SI32 VillageUnique = 1; VillageUnique < MAX_VILLAGE_NUMBER; ++VillageUnique )
			{
				pVillageInfo = pclClient->pclVillageManager->pclVillageInfo[VillageUnique];
				if(NULL == pVillageInfo)    
				{
					continue;
				}

				clinfo.m_siVillageGeneralMeeting[VillageUnique] = pclClient->pclVillageManager->pclVillageInfo[VillageUnique]->clVillageVary.m_bGeneralMeetingSuggest;	
			}

			// 클라이언트로 메시지를 보낸다. 
			cltMsg clMsg(GAMEMSG_RESPONSE_VILLAGEGENERALMEETING, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
	else if(_tcscmp(order, TEXT("serverquestclear")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if( gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel )
		{
			((cltServer*)pclClient)->pclServerEventManager->GetServerEvent_Quest()->QuestClearForGM();
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	else if(_tcscmp(order, TEXT("structurerepair")) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if( gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel )
		{
			SI32 villageUnique = 0;
			SI32 StrDura = 0;
			SI32 ranktype = 0;
			TCHAR szVillageName[64];
			TCHAR szStructureName[64];
			sscanf( &pstring[1] , "%s %s %s %d" , order, szVillageName, szStructureName, &StrDura );

			NTCHARString64 kVillageName(szVillageName);
			kVillageName.Replace("_", " ");

			villageUnique = pclClient->pclVillageManager->FindVillageUniqueFromName(kVillageName);

			if( villageUnique <= 0 )
			{
				villageUnique = _ttoi(kVillageName);
			}

			NTCHARString64 kStructureName(szStructureName);
			kStructureName.Replace("_", " ");

			ranktype = pclClient->pclVillageStructureSet->GetRankTypeFromStructureName(kStructureName);
			if( ranktype <= 0 )		return false;
			if( StrDura < 0 )		return false;

			if( StrDura >= 10000 )	StrDura = 10000;

			sDBRequest_ChangeStrDura sendMsg( villageUnique, ranktype, StrDura );
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&sendMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}
	//KHY - 1022 - NpcRate 값을 설정한다. (임시로)
	else if(_tcscmp(order,TEXT("setnpcrate")) == 0)
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SI64 setnpcrate = 0 ;
			sscanf(&pstring[1], "%s %I64d ", order, &setnpcrate);

			if(setnpcrate >= 100)
			{
				pclItemManager->clItemStatistic.siNPCPriceRate =setnpcrate ;

				SendServerInfo(0);

				pclNPCItemInfo->SetNPCItemInfo(setnpcrate);
			}
			else
				return false;
		}
		else
		{
			bnotenoughmasterlevel = true ;
		}

	}
	//[진성] 마을 점수 증가.
	else if(_tcscmp(order,TEXT("addvillagescore")) == 0)
	{
		SI32 siViilageUnique = 0;
		SI32 siViilageScore  = 0;
		_stscanf(&pstring[1], TEXT("%s %d %d"), order, &siViilageUnique, &siViilageScore );
		
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			if( 0 < siViilageUnique && siViilageUnique < MAX_VILLAGE_NUMBER)
			{
				sDBRequest_Add_Village_Score clMsg( siViilageUnique, siViilageScore );
				pclClient->SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
			}
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}

	//	[종호] BalanceCurrency 사용한도금액 설정
	else if(_tcscmp(order,TEXT("setbm")) == 0)
	{
		GMONEY siMoney = 0;
		_stscanf(&pstring[1], TEXT("%s %I64d"), order, &siMoney);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			//[종호] 통화량 조절 - 사용가능 통화량 저장
			sDBRequest_SetBalanceCanUseCurrency clMsg(siMoney);
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}

	//	[종호] BalanceCurrency 사용한도금액 확인
	else if(_tcscmp(order,TEXT("getbm")) == 0)
	{
		GMONEY siMoney = 0;
		_stscanf(&pstring[1], TEXT("%s"), order);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			// 잔여 통화량을 요청한다.
			//sDBRequest_GetBalanceCanUseCurrency clMsgReq(pclCM->CR[1]->GetCharUnique(), true);
			sDBRequest_GetBalanceCanUseCurrency clMsgReq(serverid, true);
			pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsgReq);		
		}
		else
		{
			bnotenoughmasterlevel = true;
		}
	}

	
	//	[종호_GAWIBAWIBO] 가위바위보 연승 Toggle
	else if(_tcscmp(order,TEXT("srpwin")) == 0)
	{
		_stscanf(&pstring[1], TEXT("%s"), order);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			GetGawiBawiBoMgr()->ToggleAlwaysWin();
			SendServerResponseMsg(0, SRVAL_RETURN_GAWIBAWIBO_SRPWIN,  GetGawiBawiBoMgr()->IsAlwaysWin(), 0, pclCM->CR[serverid]->GetCharUnique());
		}
		else
			bnotenoughmasterlevel = true;
	}

	//	[종호_GAWIBAWIBO] 가위바위보 연승 제한
	else if(_tcscmp(order,TEXT("srplimit")) == 0)
	{
		SI32 siLimitWin = 0;
		_stscanf(&pstring[1], TEXT("%s %d"), order, &siLimitWin);

		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			GetGawiBawiBoMgr()->SetLimitWin(siLimitWin);
			SendServerResponseMsg(0, SRVAL_RETURN_GAWIBAWIBO_SRPLIMIT,  (SI16)GetGawiBawiBoMgr()->GetLimitWin(), 0, pclCM->CR[serverid]->GetCharUnique());
		}
		else
			bnotenoughmasterlevel = true;
	}

	//	[진성] 소환수 변신 스테미너 채우기.
	else if( _tcscmp( order,TEXT("setsummontransformtime") ) == 0 )
	{
		SI32 siTime = 0;
		_stscanf(&pstring[1], TEXT("%s %d" ), order, &siTime);

		cltCharServer* pclChar = pclCM->GetCharServer( serverid );		
		if( NULL == pclChar ) return false;
		
		cltSummon* pclTranformSummon = pclChar->pclCI->clSummonInfo.GetSummon( pclChar->siSummonIndex );
		if( NULL == pclTranformSummon ) return false;
		pclTranformSummon->m_siTranformStamina = siTime;
	
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			cltGameMsgResponse_Summon clInfo( pclChar->GetCharUnique(), pclChar->siSummonIndex, pclTranformSummon );
			cltMsg clMsg( GAMEMSG_RESPONSE_SUMMONINFO, sizeof(clInfo), (BYTE*)&clInfo );
			pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
		}
		else
		{
			bnotenoughmasterlevel = true;
		}	
	}
	
	// [춘기] 몬스터 생성 창
	else if( _tcscmp( order,TEXT("cmt") ) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SendServerResponseMsg( 0, SRVAL_RETURN_MONCREATE_DLG, 0, 0, serverid );
		}
		else
		{
			bnotenoughmasterlevel = true;
		}	
	}

	// [춘기] 장비 제조 창
	else if( _tcscmp( order,TEXT("seq") ) == 0 )
	{
		SI32 ref = pclGMManager->FindUniqueFromOrder(order);

		if(gamemasterlevel >= pclGMManager->pclGMInfo[ref]->siNeedGMLevel)
		{
			SendServerResponseMsg( 0, SRVAL_RETURN_EQUIPMENTCREATE_DLG, 0, 0, serverid );
		}
		else
		{
			bnotenoughmasterlevel = true;
		}			
	}
	
	// 중국에서는 운영자 명령어 실행시 툴스 컴퓨터로 옮긴다.
#ifdef _CHINA
	sRequest_Send_ToolsLog_MasterServer		sendMsg( szServerID, order, pclCM->CR[serverid]->pclCI->clBI.szAccountID,
		                                            (TCHAR*)pclCM->CR[serverid]->GetName(), pstring );
	pclAgentProcess->GetAgentSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );
#endif

	if(bnotenoughmasterlevel)
	{
		// 클라이언트로 운영자 권한이 없음을 통보한다. 
		cltMsg clMsg(GAMEMSG_RESPONSE_GAMEMASTERLEVEL, 0);
		pclCM->CR[serverid]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	else
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			if( NULL != pclchar)
			{
				// [로그추가 : 박기형 2009. 11. 3] GM명령어
				// charparam1 == 사용 명령어 문자열
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_USEGMCOMMAND, 
					0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
					0, 0, 0, 0, 0, pstring, NULL);	
			}
		}
	}


	return TRUE;
	
}


