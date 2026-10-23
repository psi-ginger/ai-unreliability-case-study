#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-Mining.h"
#include "..\CommonLogic\MsgType-System.h"
#include "..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\Interface\NotifyMsgDlg\NotifyMsgDlg.h"
#include "..\Interface\SoundOnOffDlg\SoundOnOffDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../NInterface/NMiningDlg/NMiningDlg.h"
#include "..\Interface\DrawChat\DrawChat.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h" 
#include "Char\CharManager\CharManager.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_MININGGETPRODUCTS(cltMsg* pclMsg, SI32 id)
{
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MININGGETPRODUCTSNOTIFY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_MiningGetProductsNotify *pclInfo = (cltGameMsgResponse_MiningGetProductsNotify*)pclMsg->cData;

	if ( pclInfo->siCharUnique < 1 )
	{
		return;
	}

	if ( pclInfo->siItemUnique < 1 )
	{
		return;
	}
	
	id = cltCharCommon::CharUniqueInfo[ pclInfo->siCharUnique ];

	if ( id < 1 || pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}


	cltCharClient *pclCharClient = (cltCharClient *)pclCM->CR[id];

	if ( pclCharClient == NULL )
	{
		return;
	}

	TCHAR* ItemName = (TCHAR*)pclItemManager->GetName( pclInfo->siItemUnique );

	if ( ItemName == NULL || _tcslen( ItemName ) < 1 )
	{
		return;
	}

	TCHAR buffer[256];
	TCHAR* pText = GetTxtFromMgr(3222);
	StringCchPrintf(buffer, 256, pText, ItemName);
	pclCharClient->pclDrawChat->Set(pclDrawChatSurface, buffer, false, PRODUCTION_CHATTING );

	return;
}