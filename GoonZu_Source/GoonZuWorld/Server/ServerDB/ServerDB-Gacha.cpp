//---------------------------------
// 영진 [07/04/17]
//---------------------------------

#include "Char/CharManager/CharManager.h"

#include "../Server/Server.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-Gacha.h"

#include "../../CommonLogic/Msg/MsgType-Gacha.h"
#include "../../CommonLogic/MsgType-System.h"

#include "../../Server/GachaManager/GachaManager.h"

#include "../../CommonLogic/MsgRval-Define.h"



void cltServer::DoMsg_DBMSG_RESPONSE_GACHABUY( sPacketHeader* pPacket )
{
	sDBResponse_GachaBuy* pclinfo = (sDBResponse_GachaBuy*)pPacket;

	SI32 id = pclinfo->usCharID ;
	if ( pclCM->IsValidID(id) == false ) return ;
	SI32 personid = pclinfo->siPersonID ;
	if ( pclCM->CR[id]->pclCI->GetPersonID() != personid ) return ;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	pclchar->bNowGachaBuy = false;

	if( pclinfo->siGetItemPos < 0 || pclinfo->siGetItemPos >= MAX_ITEM_PER_PERSON )		return;
	if( pclinfo->siKeyItemPos < 0 || pclinfo->siKeyItemPos >= MAX_ITEM_PER_PERSON )		return;

	if ( pclinfo->siResult != 1 )
	{
		if( pclinfo->siResult == -2 )
		{
			SendServerResponseMsg(0, SRVAL_GACHA_FAIL_KEYNEED, 0, 0, pclchar->GetID());
			return; 	
		}

		return ;
	}
	// 캐릭터의 가차 정보를 업데이트 한다.

	pclchar->pclCI->uiGachaCnt = pclinfo->uiGachaCnt;


	// 인벤토리를 업데이트 한다.

	// 예약 해지한다.
	pclchar->SetInputItemReserve( pclinfo->siGetItemPos , 0 );
	pclchar->SetInputItemReserve( pclinfo->siKeyItemPos , 0);

	pclchar->pclCI->clCharItem.clItem[pclinfo->siGetItemPos].Set(&pclinfo->clGetItem);
	pclchar->pclCI->clCharItem.clItem[pclinfo->siKeyItemPos].Set(&pclinfo->clKeyItem);

	// 클라이언트로 업데이트 된 정보를 보낸다. 
	{
		cltGameMsgResponse_PersonItemInfo clinfo(pclinfo->siGetItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclinfo->clGetItem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg);
	}
	{
		cltGameMsgResponse_PersonItemInfo clinfo(pclinfo->siKeyItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclinfo->clKeyItem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg);
	}

	// 최고 상품이 뽑힌것이라면 전체 알림
	if( pclinfo->bBestProduct )
	{
		((cltServer*)(pclClient))->SendRareItemMade(id, &pclinfo->clGetItem, RAREITEM_NOTICE_SWITCH_GACHA );
	}

	// 클라이언트로 가차 성공 정보를 보낸다.
	//[진성] 가차 개편. 가차로 받은 아이템 정보를 보내느걸로 수정. => 2008-9-2
	cltGameMsgResponse_GachaBuy clinfo( pclinfo->bBestProduct , &pclinfo->clGachaGetItem, pclinfo->uiGachaCnt );
	cltMsg clMsg(GAMEMSG_RESPONSE_GACHABUY, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	// 아이템 사용통계 추가
	pclItemManager->AddUseNum( pclinfo->clUseKeyItem.siUnique, pclinfo->clUseKeyItem.siItemNum );

	// 가차키 사용 로그
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
	{
		// [로그추가 : 황진성 2007. 10. 10] // 가차 열쇠로 아이템을 구입할 것을 요청. 
		// param1	== 사용한 키 위치
		// param2	== 가차 회수
		// param3	== 가차 Add Count

		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GACHA, LOGCOMMAND_INDEX_GACHA_BUYUSED, 
										0, (cltCharServer*)pclchar, NULL, 0, 0, 0, &pclinfo->clUseKeyItem, pclinfo->clKeyItem.siItemNum, 0, 0, 0, 0, 0,
										pclinfo->siKeyItemPos, pclinfo->uiGachaCnt, pclinfo->uiGachaCntAdd, 0, 0,
										NULL, NULL);
	}

	// 획득 아이템 로그
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
	{
		// [로그추가 : 황진성 2007. 10. 10] // 가차 열쇠로 아이템을 구입할 것을 요청. 
		// param1		== 얻은 아이템 위치, 
		// param2		== 가차 회수
		// param3		== 최고상품여부
		// param4       == 키아이템유니크
		// param5		== 가차 Add Count

		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GACHA, LOGCOMMAND_INDEX_GACHA_BUY, 
							0, (cltCharServer*)pclchar, NULL, 0, 0, 0, &pclinfo->clGachaGetItem, pclinfo->clGetItem.siItemNum, 0, 0, 0, 0, 0,
							pclinfo->siGetItemPos, pclinfo->uiGachaCnt, pclinfo->bBestProduct, pclinfo->clUseKeyItem.siUnique, pclinfo->uiGachaCntAdd,
							NULL, NULL);
	}
}