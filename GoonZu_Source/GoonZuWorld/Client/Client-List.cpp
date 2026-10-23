#include "Client.h"
#include "..\CommonLogic\MsgType.h"
#include "..\CommonLogic\Msg\MsgType-List.h"
#include "..\CommonLogic\PostOffice\PostOffice.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_LIST_DELETEALL( cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ListDeleteAll* pclInfo = (cltGameMsgResponse_ListDeleteAll *)pclMsg->cData;

	switch( pclInfo->siListID )
	{
	case LISTID_POSTOFFICE_RECVITEM:
		{
			pclPostOfficeManager->ListRecvItem_DeleteAll();
		}
		break;
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_LIST_DELITEM( cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ListDelItem* pclInfo = (cltGameMsgResponse_ListDelItem *)pclMsg->cData;

	switch( pclInfo->siListID )
	{
	case LISTID_POSTOFFICE_RECVITEM:
		{
			pclPostOfficeManager->ListRecvItem_DelItem( pclInfo->lParam );
		}
		break;
	}
}

