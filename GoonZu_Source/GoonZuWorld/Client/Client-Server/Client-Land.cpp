#include "..\Client.h"
#include "..\Client\Music\Music.h"
#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/Msg/MsgType-Land.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_LAND_UNITLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_LandUnitList *pclinfo = (cltGameMsgResponse_LandUnitList *)pclMsg->cData;

	// 받은 정보를 농장 관리자에게 전달한다. 
	for( int i = 0; i < MAX_LANDUNIT_NUMBER; i++)
	{
		pclLandManager->clClientUnit[i].clPerson.Set(&pclinfo->clPerson[i]);
	}
	
	// 받은 정보를 화면에 표시하도록 한다. 
	pclLandManager->bUpdateLandUnitListShowChiefSwitch = TRUE;
	pclLandManager->bUpdateLandUnitListShowSwitch		= TRUE;

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_LAND_UNITDETAIL(cltMsg* pclMsg, SI32 id)
{

	// 받은 정보를 확인한다. 
	cltGameMsgResponse_LandUnitListDetail* pclinfo = (cltGameMsgResponse_LandUnitListDetail*)pclMsg->cData;
	
	// 받은 정보를 시전 관리자에게 전달한다. 
	for( int i = 0;i < MAX_LANDUNIT_NUMBER; i++ )
	{
		pclLandManager->clClientUnit[i].clContract.Set(&pclinfo->clContract[i]);
	}

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_LAND_APPLYERLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_LandApplyerList *pclinfo = (cltGameMsgResponse_LandApplyerList *)pclMsg->cData;

	for( int i = 0; i < MAX_APPLY_PER_LAND; i++ ) 
	{
		pclLandManager->clClientApplyer[ i ].Set( &pclinfo->clLandApplyer[ i ] );
	}

	pclLandManager->UpdateLandApplyerListShowSwitch = true;


}

void cltClient::DoMsg_GAMEMSG_RESPONSE_LAND_RENT(cltMsg* pclMsg, SI32 id)
{

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_LAND_DELAPPLYER(cltMsg* pclMsg, SI32 id)
{
	// 신청자 명단을 모두 삭제한다. 
	SI32 siSlot = (SI32)*pclMsg->cData;

	for(int i = 0; i < MAX_APPLY_PER_LAND;i++)
	{
		if(siSlot >=0 && siSlot == i)
		{
			pclLandManager->clClientApplyer[i].Init();
		}		
	}

	// 받은 정보를 화면에 표시하도록 한다. 
	pclLandManager->UpdateLandApplyerListShowSwitch = TRUE;
}