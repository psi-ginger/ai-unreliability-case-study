#include ".\dormancypointreward.h"

#include "../../Client/InterfaceMgr/DialogController.h"
#include "../../Client/ninterface/NDormancyAccount/NDormancyPointDlg.h"
#include "../common/char/CharClient/Char-Client.h"
#include "../../Client/Client.h"
#include "../common/char/CharManager/CharManager.h"
#include "../MsgType.h"
#include "../msg/MsgType-DormancyAccount.h"

CDormancyPointRewardClient::CDormancyPointRewardClient(void) : CDormancyPointReward()
{
	// 클라이언트 객체
	m_pclClient = (cltClient *)pclClient;
}

CDormancyPointRewardClient::~CDormancyPointRewardClient(void)
{
	m_pclClient = NULL;
}

// 휴면계정상태 일 경우 휴면 포인트 다이얼로그를 생성한다.
BOOL	CDormancyPointRewardClient::CreatePointDlg()
{
	// 클라이언트 캐릭터
	cltCharClient *pclCharClient = (cltCharClient *)m_pclClient->pclCM->CR[1];

	if( NULL == pclCharClient)
		return FALSE;

	// 휴면 포인트 다이얼로그
	CNDormancyPointDlg* pclDialog = (CNDormancyPointDlg*)m_pclClient->m_pDialog[ NDORMANCYPOINT_DLG ];
	if( NULL == pclDialog )
	{
		m_pclClient->CreateInterface(NDORMANCYPOINT_DLG, TRUE);
	}
	
	return TRUE;
}

// 포인트 아이템 구입 게임서버로 요청
BOOL	CDormancyPointRewardClient::BuyPointItemRequest(SI32 siItemUnique)
{
	// 클라이언트 캐릭터
	cltCharClient *pclCharClient = (cltCharClient *)m_pclClient->pclCM->CR[1];	

	if( NULL == pclCharClient)
		return FALSE;

	cltGameMsgRequest_DormancyBuyPoinItem clInfo(siItemUnique);
	cltMsg clMsg( GAMEMSG_REQUEST_DORMANCY_BUYPOINTITEM, sizeof(clInfo), (BYTE*)&clInfo );
	m_pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	return TRUE;
}

// 휴면 포인트 정보 수신
BOOL	CDormancyPointRewardClient::RecvDormancyInfo(cltCharClient*	pclChar, SI32 siPoint, SI32 siReaminedPlayTime, BOOL bDormancyState,SI16 siStartedYear, SI16 siStartedMonth, SI16 m_siStartedDay)
{
	if( NULL == pclChar)
		return FALSE;

	CDormancyInfo &pclDormancyInfo = pclChar->pclCI->clDormancyInfo;
	
	// 데이터 입력 
	pclDormancyInfo.SetDormancyPoint(siPoint);
	pclDormancyInfo.SetDormancyState(bDormancyState);
	pclDormancyInfo.SetRemainedDormancyPlayTime(siReaminedPlayTime);
	pclDormancyInfo.GetStartedDate()->wYear = siStartedYear;
	pclDormancyInfo.GetStartedDate()->wMonth = siStartedMonth;
	pclDormancyInfo.GetStartedDate()->wDay = m_siStartedDay;

	CNDormancyPointDlg* pclDialog = (CNDormancyPointDlg*)m_pclClient->m_pDialog[ NDORMANCYPOINT_DLG ];

	if( FALSE == bDormancyState )
	{
		if(NULL != pclDialog)
			pclDialog->Destroy();
		return FALSE;
	}

	if( NULL == pclDialog )
	{
		m_pclClient->CreateInterface(NDORMANCYPOINT_DLG, TRUE);
		pclDialog = (CNDormancyPointDlg*)m_pclClient->m_pDialog[ NDORMANCYPOINT_DLG ];
		if(NULL == pclDialog )
			return FALSE;
	}
	else
	{
		pclDialog->Show(TRUE);
	}

	// 정보 갱신
	pclDialog->RefreshDormancyInfo();

	return TRUE;
}