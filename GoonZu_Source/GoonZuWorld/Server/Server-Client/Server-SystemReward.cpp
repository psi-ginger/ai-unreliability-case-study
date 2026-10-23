// tys -

#include "../Server/Server.h"
#include "Char/CharManager/CharManager.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Person.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_SYSTEMREWARDLIST(cltMsg* pclMsg, SI32 id)
{
	cltRequestRewardList* pclinfo = (cltRequestRewardList*)pclMsg->cData;

	if ( pclCM->IsValidID(id) == false ) 
		return ;

	if(pclinfo->m_siCurPage < 0) return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);

    sDBRequest_SystemRewardList	clMsg(pclchar->pclCI->GetPersonID(), (UI16)pclchar->GetCharUnique(), sTime, pclinfo->m_siCurPage);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_RECIEVESYSTEMREWARD(cltMsg* pclMsg, SI32 id )
{
	cltSystemRewardInfo* pclinfo = (cltSystemRewardInfo*)pclMsg->cData;

	if ( pclCM->IsValidID(id) == false )	return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	if ( pclchar == NULL )
		return;

	SI16 invpos = 0;
	cltItem cltoitem;
	SI32 rtnvalue = 0;

	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	pclchar->ItemUseReserveAction();        

	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	pclchar->BulletUseReserveAction();

	if ( pclchar->CanAddInv( INVMODE_ALL, &pclinfo->m_clItem, &invpos, &cltoitem, &rtnvalue ) )
	{
        sDBRequest_RecieveSystemReward clMsg( pclchar->pclCI->GetPersonID(), pclchar->GetID(), pclinfo->m_siIndex, invpos, pclItemManager->CanPile(pclinfo->m_clItem.siUnique), pclinfo->m_clItem.IsCreateSID(pclItemManager) );
		pclGameDBProcess->SendMsg((sPacketHeader*)&clMsg); 		
	}
	else
	{
		return;		// 인벤토리가 부족해;;
	}
}