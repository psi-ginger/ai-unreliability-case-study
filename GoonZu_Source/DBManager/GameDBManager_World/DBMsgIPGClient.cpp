#include "DBGameProtocol.h"
#include "DBMsg-IPGClient.h"

#include <objbase.h>
#import "bxpgincli.tlb" named_guids

using namespace BXPGINCLILib;

extern IPGTXCli *g_pIPGTXCli;

void DBGameProtocol::GiveCash()
{
	sDBRequest_GiveCash *pRecvMsg = (sDBRequest_GiveCash *)m_pPacket;

	if( givecashcount > 1000 ) return;

	givecashbuffer[ givecashcount ].personid = pRecvMsg->siPersonID;
	memcpy( givecashbuffer[ givecashcount ].id, pRecvMsg->szUserID, MAX_PLAYER_NAME );
	givecashbuffer[ givecashcount ].id[ MAX_PLAYER_NAME - 1 ] = NULL;
	givecashbuffer[ givecashcount ].amount = pRecvMsg->siAmount;

	givecashcount++;

	return;

}
