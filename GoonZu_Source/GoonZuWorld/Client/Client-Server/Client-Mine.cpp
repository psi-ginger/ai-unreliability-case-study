#include "..\Client.h"
#include "..\Client\Music\Music.h"
#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/Msg/MsgType-Mine.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_MINE_UNITLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_MineUnitList *pclinfo = (cltGameMsgResponse_MineUnitList *)pclMsg->cData;

	// 받은 정보를 광산 관리자에게 전달한다. 
	for( int i = 0; i < MAX_MINEUNIT_NUMBER; i++)
	{
		pclMineManager->clClientUnit[i].clPerson.Set(&pclinfo->clPerson[i]);
	}
	
	// 받은 정보를 화면에 표시하도록 한다. 
	pclMineManager->bUpdateMineUnitListShowChiefSwitch = TRUE;
	pclMineManager->bUpdateMineUnitListShowSwitch		= TRUE;

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MINE_UNITDETAIL(cltMsg* pclMsg, SI32 id)
{

	// 받은 정보를 확인한다. 
	cltGameMsgResponse_MineUnitListDetail* pclinfo = (cltGameMsgResponse_MineUnitListDetail*)pclMsg->cData;
	
	// 받은 정보를 시전 관리자에게 전달한다. 
	for( int i = 0;i < MAX_MINEUNIT_NUMBER; i++ )
	{
		pclMineManager->clClientUnit[i].clContract.Set(&pclinfo->clContract[i]);
	}

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MINE_APPLYERLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_MineApplyerList *pclinfo = (cltGameMsgResponse_MineApplyerList *)pclMsg->cData;

	for( int i = 0; i < MAX_APPLY_PER_MINE; i++ ) 
	{
		pclMineManager->clClientApplyer[ i ].Set( &pclinfo->clMineApplyer[ i ] );
	}

	pclMineManager->UpdateMineApplyerListShowSwitch = true;


}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MINE_RENT(cltMsg* pclMsg, SI32 id)
{

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MINE_DELAPPLYER(cltMsg* pclMsg, SI32 id)
{
	// 신청자 명단을 모두 삭제한다.
	SI32 siSlot = (SI32)*pclMsg->cData;

	for(int i = 0; i < MAX_APPLY_PER_MINE;i++)
	{
		if(siSlot >= 0 && siSlot == i)
		{
			pclMineManager->clClientApplyer[i].Init();
		}	
	}

	// 받은 정보를 화면에 표시하도록 한다. 
	pclMineManager->UpdateMineApplyerListShowSwitch = TRUE;
}