/* ==========================================================================
	설명   :		CityHallState 관련 Client 메세지 함수

	작성일 :		05/05/25
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/


#include "Client.h"
#include "Music\Music.h"
#include "..\CommonLogic\MsgType.h"

//-------------------------------------
// Common
//-------------------------------------
#include "../Resource.h"

#include "MsgType-Structure.h"

#include "Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "Interface\NotifyMsgDlg\NotifyMsgDlg.h"

#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

//#if	defined(_CITYHALL_BANKRUPTCY)	
void cltClient::DoMsg_GAMEMSG_RESPONSE_NOTIFYCLIENTABOUTCITYHALLSTATE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_NotifyClientAboutCityHallState* pclinfo = (cltGameMsgResponse_NotifyClientAboutCityHallState*)pclMsg->cData;

	if(pclinfo == NULL ) return;

	cltVillageInfo *pVillInfo = NULL;

	pVillInfo = pclVillageManager->pclVillageInfo[ pclinfo->siVillageUnique ];

	if( pVillInfo == NULL ) return;

	TCHAR buffer[256];
	TCHAR buffer2[512];

	TCHAR* pTitle = NULL;
	TCHAR* pText1 = NULL;
	TCHAR* pText2 = NULL;

	switch( pclinfo->uiCityHallState )
	{
	case CITYHALL_BEING_BANKRUPT:
		pText1  = GetTxtFromMgr(5175);
		break;
	case CITYHALL_BEING_INITIALIZED:
		pText1  = GetTxtFromMgr(5179);
		break;
	case CITYHALL_BEING_STABLE:
		pText1  = GetTxtFromMgr(5171);
		break;
	}

	switch( pclinfo->uiAnnouncementType )
	{
	case cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_DECLARE_WAR:
		{
			pTitle = GetTxtFromMgr(5172);
			pText2  = GetTxtFromMgr(5173);

			memset(buffer, 0, sizeof(buffer));
			memset(buffer2, 0, sizeof(buffer2));

            StringCchPrintf(buffer, 256, TEXT("%s\r%s"), pText1, pText2 );
			StringCchPrintf(buffer2, 512, buffer, pVillInfo->szName );
		
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer2 );
		}
		break;
	case cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_STOCK_TRADE:
		{
			pTitle = GetTxtFromMgr(5177);
			pText2  = GetTxtFromMgr(5176);

			memset(buffer, 0, sizeof(buffer));
			memset(buffer2, 0, sizeof(buffer2));

			// [수정 : 황진성 2007.9.3]
			// 주식 거래소_파산한 마을의 주식을 거래(판매,구매)할 때 
			// 활성화 되는 경고 팝업창의 텍스트가 잘못 표시되는것은
			// "\r"로 인한 음표표시 때문 일단 음표표시가 안나오게 하려고 "\r" 제거. 
			StringCchPrintf(buffer, 256, TEXT("%s %s"), pText1, pText2 );
            //StringCchPrintf(buffer, 256, TEXT("%s\r%s"), pText1, pText2 );
			StringCchPrintf(buffer2, 512, buffer, pVillInfo->szName );
		
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer2 );
		}
		break;
	case cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_GENERALMEETING_SUGGEST:
		{
			pTitle = GetTxtFromMgr(4004);
			pText2  = GetTxtFromMgr(1549);

			memset(buffer, 0, sizeof(buffer));
			memset(buffer2, 0, sizeof(buffer2));

            StringCchPrintf(buffer, 256, TEXT("%s\r%s"), pText1, pText2 );
			StringCchPrintf(buffer2, 512, buffer, pVillInfo->szName );
		
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer2 );
		}
		break;
	case cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_CITYHALLCHIEF_RESIGN:
		{
			pTitle = GetTxtFromMgr(1546);
			pText2  = GetTxtFromMgr(1549);

			memset(buffer, 0, sizeof(buffer));
			memset(buffer2, 0, sizeof(buffer2));

            StringCchPrintf(buffer, 256, TEXT("%s\r%s"), pText1, pText2 );
			StringCchPrintf(buffer2, 512, buffer, pVillInfo->szName );
		
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer2 );
		}
		break;
	case cltGameMsgResponse_NotifyClientAboutCityHallState::ANNOUNCEMENT_TYPE_FAIL_BIDSTOCK:
		{
			pTitle = GetTxtFromMgr(1471);
			pText2  = GetTxtFromMgr(5181);

			memset(buffer, 0, sizeof(buffer));
			memset(buffer2, 0, sizeof(buffer2));

            StringCchPrintf(buffer, 256, TEXT("%s\r%s"), pText1, pText2 );
			StringCchPrintf(buffer2, 512, buffer, pVillInfo->szName );
		
			m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer2 );
		}
		break;
	default:
		{

		}
		break;
	}

}
//#endif