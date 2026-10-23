#ifndef _MSGTYPE_H
#define _MSGTYPE_H

#include "itzbase.h"

enum {
	MSG_REQUEST_TEST1 = 0,
	MSG_REQUEST_TEST2,
	
	MSG_RESPONSE_TEST1,
	MSG_RESPONSE_TEST2
};


struct sRequest_Test1
{
	sPacketHeader	packetHeader;

	SI32			a;

};


struct sRequest_Test2
{
	sPacketHeader	packetHeader;

	UI16			a;
};

struct sResponse_Test1
{
	sPacketHeader	packetHeader;

	char			a;
};

struct sResponse_Test2
{
	sPacketHeader	packetHeader;

	char			a;
};



#endif