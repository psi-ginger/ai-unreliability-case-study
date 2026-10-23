#include "../Client.h"
#include "Char\CharManager\CharManager.h"
#include "../CommonLogic/MsgType-System.h"
#include "../CommonLogic/Msg/MsgType-AutoMacro.h"

//[추가 : 2007. 12. 3 => 4차오토프로그램 체크 클라이언트( S -> C ) 메시지 처리부분.]
void cltClient::DoMsg_GAMEMSG_RESPONSE_AUTOMACRO_CHECK(cltMsg* pclMsg, SI32 id )
{  
	cltGameMsgResponse_AutoMacroCheck* pclinfo = (cltGameMsgResponse_AutoMacroCheck*)pclMsg->cData;		

	if(true == clAutoMacroManager.Undate())
	{
		SI32			personid = pclClient->pclCM->CR[1]->clPB.pclCI->clBI.clPerson.GetPersonID();
		TCHAR*			pIP		 = pclClient->pclCM->CR[1]->GetSession()->GetIP();

		cltGameMsgRequest_AutoMacroOn clinfo(personid, pclinfo->m_siType, "OK군주SPlus.exe", pIP, true);
		
		cltMsg clMsg(GAMEMSG_REQUEST_AUTOMACRO_ON, sizeof(clinfo), (BYTE*)&clinfo);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	}    
}

//[추가 : 황진성 2007. 12. 10 => 오토프로그램 발견 공지 메시지.]
void cltClient::DoMsg_GAMEMSG_RESPONSE_AUTOMACRO_ON_OTHER(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgReponse_AutoMacroOnOther* pclinfo = (cltGameMsgReponse_AutoMacroOnOther*)pclMsg->cData;		
	
	if(1 == pclinfo->m_siCount)
	{
		TCHAR	szMsgText[256] = {'\0', };
        TCHAR*	pText		   = GetTxtFromMgr(7709);
		StringCchPrintf(szMsgText, sizeof(szMsgText), pText, pclinfo->m_szName);
		pclMessage->SetMsg( szMsgText, 0, RGB(255, 255, 0));
	}
	else
	{
	//	clAutoMacroManager.ShutDown();
	}
}
