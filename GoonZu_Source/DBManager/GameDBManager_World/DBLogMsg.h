#ifndef _DBLOGMSG_H
#define _DBLOGMSG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "itzbase.h"
#include "../../GoonZuWorld/Common/CommonHeader.h"
#include "../../GoonZuWorld/Common/Char/CharCommon/Char-Common.h"
#include "../../GoonZuWorld/Common/Char/ParametaBox/ParametaBox.h"
#include "../../GoonZuWorld/Common/Char/CharCommon/Char-Common.h"


enum {
	DBLOGMSG_QUERYSTRING = 1
,	DBLOGMSG_REQUEST_PCROOM_LOG
,	DBLOGMSG_REQUEST_SETHAPPYBEANCLEARINFO_LOG
,	DBLOGMSG_REQUEST_SETEVENTCOUPONINFO_LOG     //KHY - 0906 - 게임방 이벤트 쿠폰.
,	DBLOGMSG_REQUEST_NEWLOG						//LKH - 새로운 로그
,	DBLOGMSG_END
};

class sDBLogMsg_QueryString
{
public:
	sPacketHeader		packetHeader;

	SI32				siQueryLength;
	SI32				siCommandLength;

	SI32				siPersonID;
	SI32				siDstPersonID;
	SI32				siItemUnique;

	char				szCommand[ 50 ];
	char				szTime[24];
	char				szQuery[ 512 ];

	sDBLogMsg_QueryString( char *command, SI32 personid, SI32 dstpersonid, SI32 itemunique, char *query ) 
	{
		siCommandLength = siQueryLength = 0;

		szCommand[ 0 ] = szQuery[ 0 ] = NULL;

		if( command == NULL ) return;
		if( query == NULL ) return;

		int commandlen = strlen( command );
		if( commandlen >= 50 ) return;

		int querylen = strlen( query );
		if( querylen >= 500 ) return;

		siPersonID		= personid;
		siDstPersonID	= dstpersonid;
		siItemUnique	= itemunique;

		SYSTEMTIME st;
		GetLocalTime(&st);
		sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

		packetHeader.usSize = sizeof( packetHeader ) + 32 + 50 + 24 + querylen + 8;
		packetHeader.usCmd = DBLOGMSG_QUERYSTRING;

		siCommandLength = commandlen;
		siQueryLength	= querylen;

		strcpy( szCommand, command );
		strcpy( szQuery, query );
	}

	char*				GetQuery() { return szQuery; };
	char*				GetCommand() { return szCommand; };
	SI32				GetCommandLength() { return siCommandLength; };
	SI32				GetQueryLength() { return siQueryLength; };

};


class sDBLogMsg_NewLog
{
public:
	sPacketHeader		packetHeader;

	SI32				m_siUseLog;
	TCHAR				m_szServerName[50];

	//TCHAR				m_szDate[20];
	SYSTEMTIME			m_stTime;
	TCHAR				m_szIP[32];
	SI16				m_siCommandType;
	SI16				m_siCommandIndex;
	UI08				m_uiReason;
	SI32				m_siIDNum;
	TCHAR				m_szPersonName[MAX_PLAYER_NAME];
	SI32				m_siPersonID;
	SI32				m_siLevel;
	SI64				m_siExp;		// SI32->SI64 경험치 오버플로우때문에 수정	
	GMONEY				m_siMoney;
	GMONEY				m_siMoneySum;
	cltItem				m_clItem;
	SI32				m_siItemSum;
	cltSummon			m_clSummon;
	SI32				m_siMonKind;
	SI32				m_siTargetPersonID;
	SI32				m_siVillageUnique;
	SI16				m_siNpcKind;
	cltHealthInfo		m_clHealthInfo;
	SI32				m_siRPPoint;
	SI32				m_siFameExp;
	SI64				m_siParam1;
	SI64				m_siParam2;
	SI64				m_siParam3;
	SI64				m_siParam4;
	SI64				m_siParam5;
	TCHAR				m_szCharParam1[50];
	TCHAR				m_szCharParam2[50];

	sDBLogMsg_NewLog(  ) 
	{
		ZeroMemory( this, sizeof( sDBLogMsg_NewLog ) );

		packetHeader.usSize = sizeof( sDBLogMsg_NewLog );
		packetHeader.usCmd = DBLOGMSG_REQUEST_NEWLOG;
	}
};


#endif
