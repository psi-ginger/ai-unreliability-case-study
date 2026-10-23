#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "../../common/Order/order.h"
#include "../../common/JWLib/IsCorrectID/IsCorrectID.h"
#include "../../../DBManager/GameDBManager_World/DBMsg.h"

#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "../../CommonLogic/MsgType-HalloweenSpiritBox.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_HALLOWEENSPIRITBOX_OK(cltMsg* pclMsg, SI32 id)
{
	cltGameRequest_HalloweenSpiritBox* pclInfo = (cltGameRequest_HalloweenSpiritBox*)pclMsg->cData;
	if(NULL == pclInfo)
		return;

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )
		return;

	// 다른 스피릿 박스 아이템도 사용 해야 하니까 예외처리 삭제
	//if(pclInfo->m_siDelItemUinque != ITEMUNIQUE(13356))	// 할로윈스피릿박스 가 아니면 리턴
	//	return;

	//if(!(pclInfo->m_siAddItemUinque == ITEMUNIQUE(13626) || pclInfo->m_siAddItemUinque == ITEMUNIQUE(13627)))	// 할로윈 스피릿 근거리, 원거리가 아니면 리턴
	//	return;

	// 적절한 아이템 유니크 인지 검사 
	if(pclInfo->m_siDelItemUinque <= 0 )	return	;
	if(pclInfo->m_siAddItemUinque <= 0 )	return	;

	// 아이템 타입 검사 

	SI32 siItemType_Box = pclItemManager->GetItemType( pclInfo->m_siDelItemUinque )		;		// 해당 아이템 타입을 가지고 온다
	SI32 siItemType_Sprit = pclItemManager->GetItemType( pclInfo->m_siAddItemUinque )	;	

	if ( siItemType_Box != ITEMTYPE_BOX || siItemType_Sprit != ITEMTYPE_PREMIUM )	return	;	// 해당 아이템 타입이 박스가 아니거나 프리미엄이 아니면 리턴 

	// DB로 '상자 열기' 요청

	SI32 siItemPosition = pclChar->pclCI->clCharItem.GetItemPos( pclInfo->m_siDelItemUinque );	// 바뀔 아이템 위치
	if(siItemPosition == -1)
		return;

	cltItem clDelItem;
	clDelItem.Set(&pclChar->pclCI->clCharItem.clItem[siItemPosition]);
	clDelItem.siItemNum = 1;


	cltItem clAddItem;
	SI16	brareswitch		= 0;
	UI08	uiDateUseDay	= 0;
	pclItemManager->MakeRandItemUnique(pclInfo->m_siAddItemUinque, &clAddItem, 0, 0, &brareswitch, 0, 0, 0, 0, 0, uiDateUseDay);


	SI16	invpos	= 0;
	SI32	rtnvalue= 0;

	cltItem cltoitem;
	if(pclChar->CanAddInv(INVMODE_ALL, &clAddItem, &invpos, &cltoitem, &rtnvalue) == false)
	{
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_TREASUREBOX_FAILEDOPEN_FULLINVEN, 0, 0, pclChar->GetID());
		return;
	}

	sDBRequest_OpenTreasureBox clMsg(	pclChar->pclCI->GetPersonID(), 
		siItemPosition, 
		&clDelItem, 
		0, 
		NULL, 
		invpos, 
		&clAddItem, 
		pclItemManager->CanPile(clAddItem.siUnique), 
		clAddItem.IsCreateSID(pclItemManager),
		false);
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
}
