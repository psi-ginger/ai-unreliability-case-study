#include ".\CBattleRoyalClient.h"

#include "../Client/Client.h"
#include "Char/CharManager/CharManager.h"
#include "../Client/ninterface/NbattleRoyal/NBattleRoyalResultDlg.h"
#include "../Client/ninterface/NbattleRoyal/NBattleRoyalMoveDlg.h"
#include "../Client/InterfaceMgr/DialogResource.h"
#include "../MsgType.h"
#include "../msg/MsgType-BattleRoyal.h"

CBattleRoyalClient::CBattleRoyalClient(void)
{
	m_pclClient = (cltClient*) pclClient;

	m_pclGameMode = NULL;
	m_pclReward = NULL;
	m_pclStadium = NULL;
}

CBattleRoyalClient::~CBattleRoyalClient(void)
{
	/*if( NULL != m_pclGameMode )
	{
		delete m_pclGameMode;
		m_pclGameMode = NULL;
	}

	if( NULL != m_pclReward )
	{
		delete m_pclReward;
		m_pclReward = NULL;
	}

	if( NULL != m_pclStadium )
	{
		delete m_pclStadium;
		m_pclStadium = NULL;
	}*/
}

BOOL	CBattleRoyalClient::Create()
{
	/*if( NULL != m_pclGameMode)
		return FALSE;
	if( NULL != m_pclReward)
		return FALSE;
	if( NULL != m_pclStadium)
		return FALSE;

	m_pclGameMode = new CBattleRoyalGameMode();
	if( NULL == m_pclGameMode)
	{
		return FALSE;
	}

	m_pclReward = new CBattleRoyalReward();
	if( NULL == m_pclReward)
	{
		return FALSE;
	}

	m_pclStadium = new CBattleRoyalStadiumClient();
	if( NULL == m_pclStadium)
	{
		return FALSE;
	}*/
	
	return TRUE;
}

// 결과 알리기
BOOL	CBattleRoyalClient::NoticeResult(bool bResult, TCHAR* pszWinnerName, SI32 siMapIndex)
{
	// 공지창
	SI32 siDlg = 0;

	if(m_pclClient->m_pDialog[NBATTLEROYALRESULT1_DLG ] == NULL)
		siDlg = NBATTLEROYALRESULT1_DLG ;
	else
		siDlg = NBATTLEROYALRESULT2_DLG ;
	m_pclClient->CreateInterface(siDlg);

	TCHAR* pszText = NULL;
	SI32 siIndex = 0;
	if ( true == bResult )
	{
		pszText = GetTxtFromMgr(30416);
	}	
	else
	{
		pszText = GetTxtFromMgr(30417);
		siIndex = 4;
	}

	TCHAR*	pszTitle  = NULL;
	if( MAPINDEX_BATTLEROYAL_MABRIUM == siMapIndex )
		pszTitle = GetTxtFromMgr(30404);
	else
		pszTitle = GetTxtFromMgr(30405);

	NTCHARString256	pBody(pszText);	

	pBody.Replace("#name#", pszWinnerName);

	((CNBattleRoyalResultDlg*)m_pclClient->m_pDialog[siDlg])->SetResult(bResult);
	SI32 siHeight = ((CNBattleRoyalResultDlg*)m_pclClient->m_pDialog[siDlg])->GetHeight();
	SI32 siY = ((CNBattleRoyalResultDlg*)m_pclClient->m_pDialog[siDlg])->GetY();

	((CNBattleRoyalResultDlg*)m_pclClient->m_pDialog[siDlg])->SetTitle(pszTitle);
	((CNBattleRoyalResultDlg*)m_pclClient->m_pDialog[siDlg])->SetDescription(pBody);

	if(siDlg == NBATTLEROYALRESULT2_DLG)
	{
		((CNBattleRoyalResultDlg*)m_pclClient->m_pDialog[siDlg])->SetY(siY + siHeight+ 5);
	}

	return TRUE;
}
// 시간 요청
BOOL	CBattleRoyalClient::RequestTime()
{
	//cltGameMsgRequest_BattleRoyalRemainedTime clInfo;
	cltMsg clMsg( GAMEMSG_REQUEST_BATTLEROYAL_REMAINEDTIME, 0);
	m_pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	return TRUE;
}
// 시간 보여주기
BOOL	CBattleRoyalClient::ShowTime(SI32 siTime)
{
	const SI32 HOURTOMINUTE = 60;
	const SI32 MAXNUMSIZE = 4;

	TCHAR*	pszTitle = GetTxtFromMgr(30418);
	NTCHARString1024 pBody(GetTxtFromMgr(30419));	
	
	SI32 siHour =  siTime / HOURTOMINUTE;
	SI32 siMin =  siTime % HOURTOMINUTE;

	TCHAR szHour[MAXNUMSIZE] = {0,};
	TCHAR szMin[MAXNUMSIZE] = {0,};

	StringCchPrintf(szHour, MAXNUMSIZE ,"%d" , siHour);
	StringCchPrintf(szMin, MAXNUMSIZE ,"%d" , siMin);

	pBody.Replace("#hour#", szHour);
	pBody.Replace("#minute#", szMin);

	if(m_pclClient->m_pDialog[NBATTLEROYALTIME_DLG ] == NULL)
		m_pclClient->CreateInterface(NBATTLEROYALTIME_DLG);

	((CNBattleRoyalResultDlg*)m_pclClient->m_pDialog[NBATTLEROYALTIME_DLG])->SetTitle(pszTitle);
	((CNBattleRoyalResultDlg*)m_pclClient->m_pDialog[NBATTLEROYALTIME_DLG])->SetDescription(pBody);

	return TRUE;
}
void	CBattleRoyalClient::SetGameMode(SI32 siMode)
{
	m_siMode = siMode;
	
	TCHAR *pszText = NULL;

	switch(m_siMode)
	{
	case BATTLEROYALGAMEMODE_RESTRICTTOUSEHEALITEM:
		{
			pszText = GetTxtFromMgr(30409);
			break;
		}
	case BATTLEROYALGAMEMODE_ONEKILL:
		{
			pszText = GetTxtFromMgr(30410);
			break;
		}
	case BATTLEROYALGAMEMODE_POWERUPONEUSER:
		{
			pszText = GetTxtFromMgr(30411);
			break;
		}
	case BATTLEROYALGAMEMODE_NORMAL:
	default:
		{
			pszText = GetTxtFromMgr(30412);
			break;
		}
	}

	pclClient->pclMessage->SetBattleRoyalGameModeMsg(pszText);

}
void	CBattleRoyalClient::HideTimerForGamingState()
{
	pclClient->bTimeCount = false ;
}

void	CBattleRoyalClient::ShowTimerForGamingState(SI32 siTime)
{
	pclClient->bTimeCount = true ;
	pclClient->siTimeSet	=	siTime;	// 초단위
	pclClient->dwWriteSetTime	=	pclClient->CurrentClock	;	// 시간을 저장한 시간
}

BOOL	CBattleRoyalClient::IsTransform(cltCharClient* pclChar)
{
	if( NULL == pclChar)
		return FALSE;
	
	if( (pclChar->siBufTransformKind == pclClient->GetUniqueFromHash("KIND_BattleRoyalSword"))
		|| (pclChar->siBufTransformKind == pclClient->GetUniqueFromHash("KIND_BattleRoyalSword2"))
		|| (pclChar->siBufTransformKind == pclClient->GetUniqueFromHash("KIND_BattleRoyalRifle"))
		|| (pclChar->siBufTransformKind == pclClient->GetUniqueFromHash("KIND_BattleRoyalRifle2"))
		|| (pclChar->siBufTransformKind == pclClient->GetUniqueFromHash("KIND_BattleRoyalKatar"))
		|| (pclChar->siBufTransformKind == pclClient->GetUniqueFromHash("KIND_BattleRoyalKatar2")) )
	{
		return TRUE;
	}

	SI32 siKind = pclChar->GetKind();

	if( (siKind == pclClient->GetUniqueFromHash("KIND_BattleRoyalSword"))
		|| (siKind == pclClient->GetUniqueFromHash("KIND_BattleRoyalSword2"))
		|| (siKind == pclClient->GetUniqueFromHash("KIND_BattleRoyalRifle"))
		|| (siKind == pclClient->GetUniqueFromHash("KIND_BattleRoyalRifle2"))
		|| (siKind == pclClient->GetUniqueFromHash("KIND_BattleRoyalKatar"))
		|| (siKind == pclClient->GetUniqueFromHash("KIND_BattleRoyalKatar2")) )
	{
		return TRUE;
	}

	return FALSE;
}

void	CBattleRoyalClient::ShowWarpButton()
{
	if(m_pclClient->m_pDialog[NBATTLEROYALMOVE_DLG ] == NULL)
		m_pclClient->CreateInterface(NBATTLEROYALMOVE_DLG);
	
	if( NULL != m_pclClient->m_pDialog[NBATTLEROYALMOVE_DLG ] )
	{
		((CNBattleRoyalMoveDlg*)m_pclClient->m_pDialog[NBATTLEROYALMOVE_DLG])->SetFlash(true);
	}
}