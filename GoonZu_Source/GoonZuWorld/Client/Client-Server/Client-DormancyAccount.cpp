#include "../Client.h"
#include "../Music\Music.h"
#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/Msg/MsgType-DormancyAccount.h"

#include "Char/CharManager/CharManager.h"

#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"


void cltClient::DoMsg_GAMEMSG_RESPONSE_DORMANCY_TAKEOUTEQUIPITEMINBOX(cltMsg* pclMsg, SI32 id)
{
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DORMANCY_REFRESHPOINTINFO(cltMsg* pclMsg, SI32 id)
{
	if( NULL == pclDormancySystem)
		return;
	CDormancyPointRewardClient *pclDormancyClient = (CDormancyPointRewardClient *)pclDormancySystem->GetDormancyPointReward();
	if( NULL == pclDormancyClient)
		return;
	
	cltGameMsgResponse_DormancyRefreshPointInfo* pclinfo = (cltGameMsgResponse_DormancyRefreshPointInfo*)pclMsg->cData;
	
	cltCharClient *pclChar= pclCM->GetCharClient(id);
	
	if( NULL == pclChar)
		return;

	// 휴면 정보 수신
	pclDormancyClient->RecvDormancyInfo(pclChar, pclinfo->m_siPoint, pclinfo->m_siRemainedTime, pclinfo->m_bDormancyState , pclinfo->m_siStartedYear, pclinfo->m_siStartedMonth, pclinfo->m_siStartedDay);

}
