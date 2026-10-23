//---------------------------------
// 2003/9/19 김태곤
//---------------------------------

#include "..\..\Client\Client.h"

#include "UnderMouseObject.h"

//----------------------------
// CommonLogic
//----------------------------
#include "../../CommonLogic/MsgType-Structure.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "..\..\Common\SystemNPC\SystemNPC.h"
#include "..\..\Common\SystemNPC\ForeignMerchantNPC\ForeignMerchantNPC.h"
#include "Char\CharManager\CharManager.h"
#include "Map\FieldObject\FieldObject.h"

#include "../../Common/Agriculture/Mining/MiningMgr.h"

//------------------------------
// Client
//------------------------------
#include "..\Client\ImageInterface/VillageNotice/VillageNotice.h"
#include "../Client/ninterface/nRightMouseUserListDlg/NRightMouseUserListDlg.h"
#include "../Client/DrawSequence/DrawSequence.h"

#include "Help\Help.h"



extern SHORT	GameRealX;
extern SHORT	GameRealY;

extern cltCommonLogic* pclClient;

//---------------------------------------------------------
// cltUnderMouseObject
//---------------------------------------------------------

void cltUnderMouseObject::Init()
{
	ID						= 0;
	siUnderMouseLockChar	= 0;
	siStructureUnique		= 0;
	siNPCUnique				= 0;
	siGateUnique			= 0;
	siVillageUnique			= 0;
	siDeskUnique			= 0;
	siTileAtb				= 0;

	MapX					= 0;
	MapY					= 0;

	NewIDSelectedClock		= 0;	// 새로운 캐릭터를 선택한 시간. 

	pclDrawSequence = new CDrawSequence();
};

cltUnderMouseObject::~cltUnderMouseObject()
{
	NDelete(pclDrawSequence);
}

// 지상에서 캐릭터를 찾는다. 
BOOL cltUnderMouseObject::FindLandChar(cltMapClient* pclMap , OUT SI32 * newid, OUT SI32 * underUserCnt   )
{
	cltClient * pclclient = (cltClient*) pclClient;
	
	// 지상에서 캐릭터를 찾는다. 
	SI32 range, i, j;
	SI16 length     = 0;
	SI32 min_length = 10000;
	SI32 id			= 0;
	SI32 index		= 0;
	SI32 playerCnt = 0; // 마우스 아래에 있는 플레이어의 카운트 [영진] 여러명의 플레이어중 골라서 메뉴를 열수있도록 수를 헤아림
	
	*newid		= 0;		// 찾은 캐릭터의 ID
	*underUserCnt = 0;		// 겹쳐있는 캐릭터의 개수

	// 다이얼로그 처리
	if( pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG] == NULL )
	{
		pclclient->CreateInterface(NRIGHTMOUSEUSERLIST_DLG);
		pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->Show(FALSE);
	}

	// 유저 목록 다이얼로그가 닫혀 있을때만 목록을 업데이트 해둔다. 열리는순간은 보여주기만 한다.
	if( pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->IsShow() == false )
	{
		((NRightMouseUserListDlg*)pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG])->Initialize();
	}

/*	index = 0;
	// 근처에 있는 캐릭터들의 리스트 생성
	while(id = pclCM->GetCharOrder(index))
	{
		index++;

		if(pclCM->IsValidID(id))
		{
			length = max( TABS(pclCM->CR[id]->GetX() - MapX), TABS(pclCM->CR[id]->GetY() - MapY));
			
			if(length > 3)continue;				// 너무 먼 거리에 있는 캐릭터는 연관짓지 않는다.
			if(id == 1 && pclCM->CR[1]->siSelectedCharIndex == -1)continue;	// 사용자이어서는 안된다. 
			if(pclCM->IsAlive(id) == false)continue;	// 살아 있어야 한다. 
			if(pclCM->CR[id]->pclKI->IsAtb(ATB_HORSE) == true)continue;	// 말은 검사하지 않는다. 

			cltDrawObjData cldata(DRAWOBJ_TYPE_CHAR, id);
			pclDrawSequence->Push(pclCM->CR[id]->GetX(), pclCM->CR[id]->GetY(), 1, 1, &cldata);
		}
	}

	min_length = 10000;
	cltDrawObjData *pcldata = pclDrawSequence->Pop();	
	while( pcldata )
	{
		if(pcldata->siType == DRAWOBJ_TYPE_CHAR)
		{
			SI32 siCharID = pcldata->siID;
			SI16 siLen = 0;

			// 캐릭터 박스 안에 들어오는 경우
			if(pclCM->CR[siCharID]->IsAbsPtInObj(GameRealX, GameRealY, siLen) == TRUE)
			{
				// 그림 그리는 순서로 정렬되었으므로
				// 나중에 찾은 캐릭터로 무조건 변경한다.
				*newid  = siCharID;

				if( pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->IsShow() == false )
				{
					if( IsPC(pclCM->CR[siCharID]->GetCharUnique()) )
					{
						*underUserCnt += 1; // 마우스 밑에 있는 플레이어의 카운트 증가
						((NRightMouseUserListDlg*)pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG])->AddUser(id, pclCM->CR[siCharID]->GetKind() , pclCM->CR[siCharID]->pclCI->clIP.GetLevel(), (TCHAR*)pclCM->CR[siCharID]->GetName() ); 
					}
				}
			}
		}

		pcldata = pclDrawSequence->Pop();
	}
*/

	while(id = pclCM->GetCharOrder(index))
	{
		index++;

		if(pclCM->IsValidID(id))
		{
			length = max( TABS(pclCM->CR[id]->GetX() - MapX), TABS(pclCM->CR[id]->GetY() - MapY));

			// 너무 먼 거리에 있는 캐릭터는 연관짓지 않는다.
			if(length > 3)continue;
			if(id == 1 && pclCM->CR[1]->siSelectedCharIndex == -1)continue;	// 사용자이어서는 안된다. 
			if(pclCM->IsAlive(id) == false)continue;	// 살아 있어야 한다. 
			if(pclCM->CR[id]->pclKI->IsAtb(ATB_HORSE) == true)continue;	// 말은 검사하지 않는다. 
			

			if(pclCM->CR[id]->IsAbsPtInObj(GameRealX, GameRealY, length)==TRUE)
			{
				
				if(length <= min_length)	
				{
					min_length = length;
					
					if(*newid  == 0)	//id 가 선택되어 있지 않다면
					{
						*newid  = id;
					}
					
					if( pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->IsShow() == false )
					{
						if( IsPC(*newid) && pclCM->CR[id]->pclKI->IsAtb(ATB_SYSTEMNPC) == false && pclCM->CR[id]->pclKI->IsAtb(ATB_MONSTER) == false ) // NPC가 아닌 플레이어라면
						{
							*underUserCnt += 1; // 마우스 밑에 있는 플레이어의 카운트 증가
							((NRightMouseUserListDlg*)pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG])->AddUser(id, pclCM->CR[id]->GetKind() , pclCM->CR[id]->pclCI->clIP.GetLevel(), (TCHAR*)pclCM->CR[id]->GetName() ); 
						}
					}
				}
			}

		}

	}

	for(range = 0 ; range <= 2 ; range++)
	{
		for(i=MapY - range;i <= MapY + range; i++)
			for(j=MapX - range;j <= MapX + range; j++)
			{
				if(i==(MapY-range) || i== (MapY+range) || j==(MapX-range) || j==(MapX+range)){}
				else continue;
				
				// 지도의 범위내에 존재하는지 검사한다. 
				if(pclMap->IsInMapArea(j, i) == false) continue;

				// 유효한 캐릭터의 아이디를 찾아야 한다. 
				id = pclMap->GetIDMap(j, i);
				if(id == 1 && pclCM->CR[1]->siSelectedCharIndex == -1)continue;	// 사용자이어서는 안된다. 
				if(pclCM->IsAlive(id) == false)continue;	// 살아 있어야 한다. 
				if(pclCM->CR[id]->pclKI->IsAtb(ATB_HORSE) == true)continue;	// 말은 검사하지 않는다. 
				
				if(pclCM->CR[id]->IsAbsPtInObj(GameRealX, GameRealY, length)==TRUE)
				{

					if(length < min_length)
					{
						min_length = length;
						*newid      = id;

						TCHAR buffer[256];
						StringCchPrintf(buffer, 256, TEXT("id:%d charunique:%d------------"), *newid, pclCM->CR[id]->GetCharUnique()); 
						pclClient->pclMessage->SetMsg(buffer);

					}
				}
			}
			
	}

	return TRUE;
}


// FieldObject를 찾는다. 찾으면 그 FOUnique를 리턴한다. 
SI32 cltUnderMouseObject::FindFieldObject(cltMapClient* pclMap)
{

	// FOUnique가 무효하면 실패. 
	SI32 founique = pclMap->GetFieldObject_Unique(MapX, MapY);
	if( founique <= 0)return 0;

	return founique;
}


// 아이템을 찾는다. 찾으면 true 리턴. 
bool cltUnderMouseObject::FindItem(cltMapClient* pclMap, SI32* pitemunique, SI32* pitemindex)
{

	cltItemMapInfo* pclinfo = pclMap->GetItemMap(MapX, MapY);
	if(pclinfo == NULL)return false;

	*pitemunique	= pclinfo->uiItemUnique;
	*pitemindex		= pclinfo->uiItemIndex;

	return true;
}

// 건축물을 찾는다. 
SI32 cltUnderMouseObject::FindStructure(cltMapClient* pclMap)
{
	if(pclMap == NULL)				return 0;

	// 마을의 아니면 리턴한다. 
	if(pclMap->siVillageUnique <= 0)return 0;

	// 건물의 영역인지 확인한다. (땅의 위치만 보고 확인한다.)
	SI32 slot = pclMap->GetStructureSlot(pclMap->siVillageUnique, MapX, MapY);
	if(slot < 0)return 0;
	
	// 해당 슬롯의 시작 좌표륵 구한다. 
	SI32 slotstartx, slotstarty;
	if(pclMap->GetSlotStartPos(pclMap->siVillageUnique, slot, &slotstartx, &slotstarty) == false)return 0;

	// 슬롯에 건물이 있는 것이 아니라면 리턴한다. 
	SI32 structureunique = pclGame->pclVillageManager->pclVillageInfo[pclMap->siVillageUnique]->clVillageVary.clStructureInfo.Get(slot);
	if(structureunique <= 0) return 0;

	// 오브젝트가 있는 곳만 유효하다. 
	if(pclMap->GetFieldObject_Unique(MapX, MapY) == 0)return 0;

	// 오브젝트의 중심 위치를 구한다.
	SI32 centerx, centery;
	if(pclMap->GetFieldObjectCenterPos(MapX, MapY, &centerx, &centery) == false)return 0;

	// 중심 오브젝트가 keyobject인 경우에만 유효하다.
	if(pclGame->pclVillageStructureSet->IsKeyStructure(centerx-slotstartx , centery-slotstarty, structureunique) == false) return 0;
	
	return structureunique;
}

// Gate를 찾는다. 
SI32 cltUnderMouseObject::FindGate(cltMapClient* pclMap)
{
	// 오브젝트가 있는 곳만 유효하다. 
	SI32 founique = pclMap->GetFieldObject_Unique(MapX, MapY);
	if(founique <= 0)return 0;

	SI32 ref = pclMap->pclFieldObjectManager->FindRefFromUniqueMinus(founique);
	if(ref < 0)return 0;

	SI32 type = pclMap->pclFieldObjectManager->pclInfo[ref]->dwType;
	
	if(type != FOTYPE_GATE)return 0;

	SI32 gateunique = pclMap->GetFieldObjectPara(MapX, MapY);
	if(gateunique <= 0)return  0;

	return gateunique ;
}

// 마을을  찾는다. 
SI32 cltUnderMouseObject::FindVillage(cltMapClient* pclMap)
{
	// 오브젝트가 있는 곳만 유효하다. 
	SI32 founique = pclMap->GetFieldObject_Unique(MapX, MapY);
	if(founique <= 0)return 0;

	SI32 ref = pclMap->pclFieldObjectManager->FindRefFromUniqueMinus(founique);
	if(ref < 0)return 0;

	SI32 type = pclMap->pclFieldObjectManager->pclInfo[ref]->dwType;
	
	if(type != FOTYPE_VILLAGE)return 0;

	SI32 villageunique = pclMap->GetFieldObjectPara(MapX, MapY);
	if(villageunique <= 0)return  0;

	return villageunique ;
}

// 마우스 아래 객체를 파악한다. 
void cltUnderMouseObject::GetUnderMouseObject(cltMapClient* pclMap, SI32 screenmousex, SI32 screenmousey)
{
	static DWORD dwLastCheckLockClock = 0;

	// 마우스가 지도위에 있는 것이 아니라면 , 
	if(pclMap->IsInMapArea(MapX, MapY) == false) return ;
	
	//-------------------------------------
	// 마우스 밑에 있는 캐릭터를 구한다. 
	//-------------------------------------
	SI32 newid = 0;
	SI32 underUserCnt = 0;

	// 지상에서 찾는다. 
	FindLandChar(pclMap,&newid, &underUserCnt);

    

	if(newid == 0 && siUnderMouseLockChar)
	{
		newid = siUnderMouseLockChar;
	}
	
	// 새로운 아이디를 선택한 시간을 구한다. 
	if((newid == 0) || newid != GetUnderMouseID())
	{
		NewIDSelectedClock = pclGame->CurrentClock;
	}

	// 말은 선택하지 않는다. 
	if(newid && pclClient->pclCM->CR[newid]->pclKI->IsAtb(ATB_HORSE))
	{
		newid = 0;
	}

	//cyh 운영자가 HIDE모드일때는 선택하지 않는다
	if (newid > 0)
	{
		if(pclClient->pclCM->CR[newid]->pclCI->clBI.uiGMMode == GMMODE_ATB_HIDE)
		{
			newid = 0;
			
		}
	}
	//일정시간 이상 캐릭터를 선택하고 있는 것이면, 
	if(TABS(NewIDSelectedClock - pclGame->CurrentClock) > 500)
	{
		// 다시 요청하지 않도록 한다. 
		NewIDSelectedClock = pclGame->CurrentClock;

		//캐릭터의 이름이 유효한지 확인한다. 
		cltCharClient* pclChar = (cltCharClient*)pclGame->pclCM->CR[newid];

		if( newid > 0 && IsPC(pclChar->GetCharUnique()))
		{
			// 이름이 무효하면, 
			if(_tcscmp(pclChar->GetName(), TEXT("")) == 0)
			{
				// 유효하지 않다면 서버에 이름을 요청한다. 
				cltGameMsgRequest_GetName clInfo( pclChar->GetCharUnique() );
				cltMsg clNormalMsg(GAMEMSG_REQUEST_NAME, sizeof(clInfo), (BYTE*)&clInfo);

				pclGame->SendMsgToServer((sPacketHeader*)&clNormalMsg);
			}
		}
	}

	// 마우스 밑의 캐릭터를 설정한다. 
	SetUnderMouseID(newid);
	// 마우스 밑의 캐릭터 수를 설정한다.
	SetUnderUserCnt( underUserCnt );

	// 마우스 밑의 캐릭터를 Lock한다. 
	if(newid && siUnderMouseLockChar != newid)
	{
		siUnderMouseLockChar	= newid;
		dwLastCheckLockClock	= pclClient->CurrentClock;
	}
	else if(siUnderMouseLockChar)
	{
		if(TABS(pclClient->CurrentClock - dwLastCheckLockClock) > 2000)
		{
			siUnderMouseLockChar = 0;
		}
	}
	
	
	//-----------------------------------
	// 마우스 밑에 있는 건물을 찾는다. 
	//-----------------------------------
	SI32 structureunique = 0;
	structureunique = FindStructure(pclMap);

	siStructureUnique = structureunique;
	
	// 도움말을 설정한다.
	if(siStructureUnique)
	{
		TCHAR buffer[256];
		SI32 ref = pclGame->pclVillageStructureSet->FindRefFromUnique(siStructureUnique);

		if(ref >= 0)
		{
			StringCchPrintf(buffer, 256, pclGame->pclVillageStructureSet->pclVillageStructureInfo[ref]->szName);
			
			// 초보자이면, 
			if(pclCM->CR[1]->pclCI->clIP.GetLevel() < 10)
			{
				TCHAR* pText = GetTxtFromMgr(1119);
				
				StringCchCat(buffer, 256, pText);
			}

			
			SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
			if( index >= 0 )
			{
				pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
			}
			else
			{
				pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
			}
		}
	}

	//-----------------------------------
	// 마우스 밑에 있는 마을을 찾는다. 
	//-----------------------------------
	SI32 villageunique = 0;
	villageunique	= FindVillage(pclMap);
	siVillageUnique	= villageunique;
	
	cltClient *pclclient = (cltClient *)pclClient;

	// 마을 안내문을 그릴 수 있게 한다.
	if ( siVillageUnique > 0 )
	{
		if ( pclclient->pclVillageNotice->Set( siVillageUnique, screenmousex, screenmousey) )
		{
			cltGameMsgRequest_VillageNotice clInfo( siVillageUnique );		
			cltMsg clMsg(GAMEMSG_REQUEST_VILLAGENOTICE, sizeof(clInfo), (BYTE*)&clInfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);	
		}
	}
	// 마을 안내문을 그릴 수 없게 한다.
	else
	{
		pclclient->pclVillageNotice->Set( 0, screenmousex, screenmousey);
	}


	//-----------------------------------
	// 마우스 밑에 있는 Gate를 찾는다. 
	//-----------------------------------
	SI32 gateunique = 0;
	gateunique = FindGate(pclMap);

	siGateUnique = gateunique;
	
	// 도움말을 설정한다.
	if(siGateUnique)
	{
		TCHAR buffer[256] = { '\0', };

		if( siGateUnique < INSTANCEMAPGATE_START )
		{
			StringCchPrintf(buffer, 256, TEXT("%s"), pclGame->pclMapManager->pclGateManager->clGate[siGateUnique].szName);
		
			SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
			if( index >= 0 )
			{
				pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
			}
			else
			{
				pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
			}
		}
	}


	//-----------------------------
	// 마우스 밑에 있는 아이템을 찾는다.
	//-----------------------------
	SI32 itemunique;
	SI32 itemindex;
	if(FindItem(pclMap, &itemunique, &itemindex) == true)
	{
		siItemUnique	= itemunique;
		siItemIndex		= itemindex;
		siItemX			= MapX;
		siItemY			= MapY;

		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclGame->pclItemManager;
		SI32 ref = pclitemmanager->FindItemRefFromUnique(siItemUnique);
		if(ref > 0)
		{
			TCHAR buffer[256];
			StringCchPrintf(buffer, 256, pclitemmanager->pclItemInfo[ref]->GetName());
			
			SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
			if( index >= 0 )
			{
				pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
			}
			else
			{
				pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
			}
		}

	}
	else
	{
		siItemUnique	= 0;
		siItemIndex		= 0;
	}

	
	//-------------------------------
	// 마우스 밑에 있는 책상을 찾는다. 
	//-------------------------------
	SI32 deskunique	= FindFieldObject(pclMap);
	siDeskUnique = 0;
	if(deskunique == FOUNIQUE(30))			// 병조판서
	{
		siDeskUnique	= deskunique;
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(1120);
		

		StringCchCopy(buffer, 256, pText);

		SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
		if( index >= 0 )
		{
			pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
		}
		else
		{
			pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
		}
	}
	else if(deskunique == FOUNIQUE(31))		// 공조판서
	{
		siDeskUnique	= deskunique;
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(1121);
		

		StringCchCopy(buffer, 256, pText);

		SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
		if( index >= 0 )
		{
			pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
		}
		else
		{
			pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
		}
	}
	else if(deskunique == FOUNIQUE(32))		// 형조판서
	{
		siDeskUnique	= deskunique;
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(1122);
		

		StringCchCopy(buffer, 256, pText);

		SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
		if( index >= 0 )
		{
			pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
		}
		else
		{
			pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
		}
	}
	else if(deskunique == FOUNIQUE(34))		// 호조판서
	{
		siDeskUnique	= deskunique;
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(1123);
		

		StringCchCopy(buffer, 256, pText);

		SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
		if( index >= 0 )
		{
			pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
		}
		else
		{
			pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
		}
	}
	else if(deskunique == FOUNIQUE(35))		// 이조판서
	{
		siDeskUnique	= deskunique;
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(1124);
		

		StringCchCopy(buffer, 256, pText);

		SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
		if( index >= 0 )
		{
			pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
		}
		else
		{
			pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
		}
	}
	else if(deskunique == FOUNIQUE(39) )		// 군주의자
	{
		siDeskUnique	= deskunique;
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(599); // 군주


		StringCchCopy(buffer, 256, pText);
		SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
		if( index >= 0 )
		{
			pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
		}
		else
		{
			pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
		}
	}
	else if(deskunique == FOUNIQUE(303) )		// 선거게시판
	{
		
 		siDeskUnique	= deskunique;
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(6998); // 선거게시판


		StringCchCopy(buffer, 256, pText);

		SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
		if( index >= 0 )
		{
			pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
		}
		else
		{
			pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
		}	

	}
	else if(deskunique == FOUNIQUE(301) )		// 역대군주목록
	{

		siDeskUnique	= deskunique;
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(7021); // 역대군주목록


		StringCchCopy(buffer, 256, pText);

		SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
		if( index >= 0 )
		{
			pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
		}
		else
		{
			pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
		}	

	}
	//[추가 : 황진성 2008. 1. 10 => 인던 현재 상황정보 보여주는 게시판 설명.]
	else if ( deskunique == FOUNIQUE(305) ) 
	{
		siDeskUnique = deskunique;
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(7830); // 여기에 게시판 관련 정보로...

		StringCchCopy(buffer, 256, pText);

		SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
		if( index >= 0 )
		{
			pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
		}
		else
		{
			pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
		}	
	}

	else if ( deskunique == FOUNIQUE(307) ) //랭킹.명예의 전당 개시판
	{
		siDeskUnique = deskunique;
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(7854); // 여기에 게시판 관련 정보로...

		StringCchCopy(buffer, 256, pText);

		SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
		if( index >= 0 )
		{
			pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
		}
		else
		{
			pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
		}	
	}
	else if ( deskunique == FOUNIQUE(9468) ) // 예식장
	{
		siDeskUnique = deskunique;
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(30098);

		StringCchCopy(buffer, 256, pText);

		SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
		if( index >= 0 )
		{
			pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
		}
		else
		{
			pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
		}	
	}

	/*cyj 예조판서 삭제
	else if(deskunique == 5640)
	{
		siDeskUnique	= deskunique;
		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(1125);
		

		StringCchCopy(buffer, 256, pText);

		SI32 index = pclGame->pclHelpManager->GetHelp(buffer);
		if( index >= 0 )
		{
			pclGame->pclHelpManager->SetPos(index, screenmousex, screenmousey);
		}
		else
		{
			pclGame->pclHelpManager->Add(buffer, screenmousex, screenmousey);
		}
	}
	*/
		
	//-----------------------------------
	// 마우스 밑의 타일 유니크를 찾는다. 
	//-----------------------------------
	siTileAtb = pclMap->GetMouseTileAtb(MapX, MapY);


	return ;
}
