//---------------------------------
// 2004/1/14 김태곤
//---------------------------------

#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"

#include "..\..\Common\Map\Map.h"
#include "..\..\Common\PrivateTradeOrderMgr\PrivateTradeOrderMgr.h"


#include "..\../../Server/AuthServer/AuthMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-System.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Web.h"

#include "..\..\CommonLogic\Msg\MsgType-PrivateTrade.h"
#include "..\..\CommonLogic\MsgType-System.h"
#include "..\..\CommonLogic\Msg\MsgType-Web.h"

void cltServer::DoMsg_DBMSG_RESPONSE_NEWS(sPacketHeader* pPacket)
{
	sDBResponse_News* pclMsg = (sDBResponse_News* )pPacket;

	if(pclMsg->siResult == 0)return ;

	// 클라이언트로 뉴스를 보낸다. 
	cltGameMsgResponse_News	clinfo(pclMsg->szNewsTitle, pclMsg->szNewspaperName, pclMsg->szNewURL,CAPTIONKIND_NEWS);
	cltMsg clMsg(GAMEMSG_RESPONSE_NEWS, sizeof(clinfo), (BYTE*)&clinfo);
	//pclCM->SendNewsMsg((BYTE*)&clMsg, CAPTIONKIND_NEWS);

	PushSpecialNews( pclMsg->szNewsTitle, pclMsg->szNewspaperName ,pclMsg->szNewURL, CAPTIONKIND_NEWS,CAPTIONKIND_NEWSCHAT);
}

// 뉴스 정보를 버퍼에 넣는다.
bool cltServer::PushSpecialNews(TCHAR * szTitle,TCHAR * szName, TCHAR * szURL, SI32 NewsKind,SI32 SendKind)
{
	if ( szTitle == NULL || szName == NULL ) return false ;
	if ( siSpecialNewsBufferIndex < 0 ) return false ;

	if ( siSpecialNewsBufferIndex >= MAX_SPECIAL_NEWS_BUFFER_SIZE )
	{
		if ( szNewsTitleBuffer[0][0] == NULL && szNewsNameBuffer[0][0] == NULL )
		{
			siSpecialNewsBufferIndex = 0 ;
		}
		else return false ;
	}

	MStrCpy(szNewsTitleBuffer[siSpecialNewsBufferIndex],szTitle,MAX_NEWS_TITLE_LENGTH);
	MStrCpy(szNewsNameBuffer[siSpecialNewsBufferIndex],szName,MAX_NEWSPAPER_NAME_LENGTH);
	MStrCpy(szNewsURLBuffer[siSpecialNewsBufferIndex],szURL,MAX_PATH);
	siSpecialNewsKindBuffer[siSpecialNewsBufferIndex] = NewsKind ;
	siSpecialSendKindBuffer[siSpecialNewsBufferIndex] = SendKind ;
	siSpecialNewsBufferIndex ++ ;

	return true;
}

bool cltServer::PopSpecialNews()
{
	if ( siSpecialNewsBufferIndex < 0 || siSpecialNewsBufferIndex > MAX_SPECIAL_NEWS_BUFFER_SIZE )
		return false ;

	for ( SI32 i = siSpecialNewsSendIndex ; i < siSpecialNewsSendIndex + 20 ; i ++ )
	{
		if ( i >= MAX_SPECIAL_NEWS_BUFFER_SIZE ) continue ;

		if ( szNewsTitleBuffer[i][0] != NULL && szNewsNameBuffer[i][0] != NULL )
		{
			cltGameMsgResponse_News	clinfo(szNewsTitleBuffer[i], szNewsNameBuffer[i], szNewsURLBuffer[i],siSpecialNewsKindBuffer[i]);
			cltMsg clMsg(GAMEMSG_RESPONSE_NEWS, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->SendNewsMsg((sPacketHeader*)&clMsg, siSpecialSendKindBuffer[i]);

			ZeroMemory(szNewsTitleBuffer[i],MAX_NEWS_TITLE_LENGTH);
			ZeroMemory(szNewsNameBuffer[i],MAX_NEWSPAPER_NAME_LENGTH);
			ZeroMemory(szNewsURLBuffer[i],MAX_PATH);
			siSpecialNewsKindBuffer[i] = CAPTIONKIND_NEWS ;
			siSpecialSendKindBuffer[i] = CAPTIONKIND_NEWS ;
		}
	}

	siSpecialNewsSendIndex = siSpecialNewsSendIndex + 20 ;

	if ( siSpecialNewsSendIndex > siSpecialNewsBufferIndex )
	{
		siSpecialNewsBufferIndex = siSpecialNewsSendIndex ;
	}

	if ( siSpecialNewsSendIndex >= MAX_SPECIAL_NEWS_BUFFER_SIZE )
	{
		siSpecialNewsSendIndex = 0 ;
	}

	if ( siSpecialNewsBufferIndex >= MAX_SPECIAL_NEWS_BUFFER_SIZE )
	{
		siSpecialNewsBufferIndex = 0 ;
	}

	return true ;
}