#ifndef _DBGAMELOGMSG_H
#define _DBGAMELOGMSG_H

#include "../../GoonZuWorld/Common/CommonHeader.h"
#include "../../GoonZuWorld/Common/Char/CharCommon/Char-Common.h"
#include "../../GoonZuWorld/Common/Char/ParametaBox/ParametaBox.h"
#include "../../GoonZuWorld/Common/Char/CharCommon/Char-Common.h"

#define MAX_STLOGDATA_PER_PACKET		10
#define MAX_RESPONSING_PERSONAL_LOG		10

#define LOGTYPE_PERSONAL_POSTOFFICE		0	// 개인 역참 거래
#define LOGTYPE_PERSONAL_ITEMMALL		1	// 아이템 몰 거래
#define LOGTYPE_PERSONAL_ALLITEM		2	// 사람과 아이템 관계 검색
#define LOGTYPE_PERSONAL_CHARLOGIN		3	// 캐릭터 로그인 검사
#define LOGTYPE_PERSONAL_ACCOUNTLOGIN	4	// 계정 로그인 검사

#define MAX_DBLOG_SIZE_DATE				24 // DBLog szDate 최대
#define MAX_DBLOG_SIZE_COMMAND			30 // DBLog szCommand 최대
#define MAX_DBLOG_SIZE_LOGTEXT			256 // DBLog szLogTxt 최대
#define MAX_DBLOG_SIZE_SERVERNAME		16 // DBLog szServerName

enum {
	DBLOGMSG_REQUEST_PERSONAL_LOG = 1,		// [준엽] 개인 로그 요청
	DBLOGMSG_RESPONSE_PERSONAL_LOG,
};

class stLOGData
{
public:
	char				szDate[MAX_DBLOG_SIZE_DATE];
	char				szCommand[MAX_DBLOG_SIZE_COMMAND];

	char				szPersonName[MAX_PLAYER_NAME];
	char				szDstPersonName[MAX_PLAYER_NAME];
	SI32				siItemUnique;
	char				szLogTxt[MAX_DBLOG_SIZE_LOGTEXT];

	void Init()
	{
		strcpy(szDate,"");
		strcpy(szCommand,"");

		strcpy(szPersonName,"");
		strcpy(szDstPersonName,"");
		siItemUnique = 0 ;
		strcpy(szLogTxt,"");
	}

	void Set(stLOGData * stlogdata)
	{
		memcpy ( this, stlogdata, sizeof( stLOGData ));
	}
};

// 개인 로그 요청
class sDBRequest_Personal_Log
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID; // 명령한 캐릭터의 ID

	SI32				siLogType; // 로그의 종류

	char				szServerName[MAX_DBLOG_SIZE_SERVERNAME]; // 서버 이름
	char				szPersonName[MAX_PLAYER_NAME]; // 찾을 캐릭터의 이름
	
	SI32				siItemUnique; // 검색 아이템

	char				szStartDate[MAX_DBLOG_SIZE_DATE]; // 검색시작할 날짜 ex:2004-08-10 10:30
	char				szEndDate[MAX_DBLOG_SIZE_DATE]; // 검색 끝낼 날짜 ex:2004-08-10 10:31

	sDBRequest_Personal_Log(UI16 charid, SI32 logtype,
		char * server,char * personname,SI32 itemunique,char *startdate,char *enddate)
	{

		packetHeader.usSize = sizeof( sDBRequest_Personal_Log );
		packetHeader.usCmd = DBLOGMSG_REQUEST_PERSONAL_LOG;

		usCharID = charid ;

		siLogType = logtype ;

		memcpy(szServerName,server,MAX_DBLOG_SIZE_SERVERNAME);
		memcpy(szPersonName,personname,MAX_PLAYER_NAME);

		siItemUnique = itemunique ;

		memcpy(szStartDate,startdate,MAX_DBLOG_SIZE_DATE);
		memcpy(szEndDate,enddate,MAX_DBLOG_SIZE_DATE);
	}
};

// 역참 로그 응답
class sDBResponse_Personal_Log
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	stLOGData			stLogData[MAX_STLOGDATA_PER_PACKET];

	sDBResponse_Personal_Log()
	{
		ZeroMemory( this, sizeof( sDBResponse_Personal_Log ) );
	}
};

#endif