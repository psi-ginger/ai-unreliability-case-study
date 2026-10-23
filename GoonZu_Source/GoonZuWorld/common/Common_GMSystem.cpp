#include "CommonHeader.h"
#include "../Server/Server.h"

#include "../GMSystem/GMProtocol.h"

extern cltCommonLogic* pclClient;

void cltPBasicInfo::SendInfofromServer(cGMClient* pGMClient)
{
	cltServer* pclserver = (cltServer*)pclClient;						if( pclserver == NULL )		return;
    GMProtocol* pclGMProtocol = (GMProtocol*)pclserver->pclGMProtocol;	if( pclGMProtocol == NULL )	return;
	ITZSession* pSession = pGMClient->GetSession();

	pclGMProtocol->SendInfofromServer(pSession, "");
}
