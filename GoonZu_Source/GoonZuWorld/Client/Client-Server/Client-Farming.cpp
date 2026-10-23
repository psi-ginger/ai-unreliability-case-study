#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-Farming.h"
#include "..\CommonLogic\MsgType-System.h"
#include "..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\Interface\NotifyMsgDlg\NotifyMsgDlg.h"
#include "..\Interface\SoundOnOffDlg\SoundOnOffDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"


//#include "..\Interface\FarmingDlg\FarmingDlg.h"
#include "..\NInterface\NFarmingDlg\NFarmingDlg.h"
#include "..\Interface\DrawChat\DrawChat.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h" 
#include "Char\CharManager\CharManager.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_FARMINGGETPRODUCTS(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_FarmingGetProducts *pclInfo = (cltGameMsgResponse_FarmingGetProducts*)pclMsg->cData;
	
	if ( pclInfo->siInvPos > 0 )
	{
		pclCM->CR[1]->pclCI->clCharItem.clItem[ pclInfo->siInvPos ].Set( &pclInfo->clItem );
	}

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FARMINGGETPRODUCTSNOTIFY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_FarmingGetProductsNotify *pclInfo = (cltGameMsgResponse_FarmingGetProductsNotify*)pclMsg->cData;

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

	TCHAR buffer[256];
	
	TCHAR *ItemName = (TCHAR*)pclItemManager->GetName( pclInfo->siItemUnique );
	
	if ( ItemName == NULL || _tcslen( ItemName ) < 1 )
	{
		return;
	}
	
	TCHAR* pText = GetTxtFromMgr(8385);
	StringCchPrintf(buffer, 256, pText, ItemName);
	pclCharClient->pclDrawChat->Set(pclDrawChatSurface, buffer, false, PRODUCTION_CHATTING );

	return;
}