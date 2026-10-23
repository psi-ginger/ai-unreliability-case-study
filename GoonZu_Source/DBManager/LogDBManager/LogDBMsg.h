#ifndef _LOGDBMSG_H_
#define _LOGDBMSG_H_

#include "itzbase.h"

#include "../../GoonZuWorld/Common/CommonHeader.h"
#include "../../GoonZuWorld/Lib/commonutil.h"

#include "../../GoonZuWorld/common/Item/ItemCommon/ItemUnit.h"
#include "../../GoonZuWorld/common/Char/ParametaBox/parametabox.h"

class cltSummonScroll
{
public:
	SI16 siUnique[MAX_SCROLL_SUMMON_PER_KIND];
	SI16 siPercent[MAX_SCROLL_SUMMON_PER_KIND];

	cltSummonScroll()
	{
		memset(this, 0, sizeof(cltSummonScroll));
	}
};

class cltSummon{
public:

	SI08		siUnique;
	SI16		siKind;							// 소환수의 종류 
	TCHAR		szName[MAX_SUMMON_NAME];		// 이름 
	SI08		siStatus;
	SI08		siAttackType;
	SI08		siIdentity;
	SI32		siTakeInMinute;

	cltPIPInfoCommon	clIP;
	cltSummonScroll clScroll;

public:

	cltSummon()
	{
		memset(this, 0, sizeof(cltSummon));
	}
};

//--------------------------------
// 건강 클래스. (핵심) 
//--------------------------------
class cltNewLog_HealthInfo{
public:
	//------------------------
	// 기초 상태 
	//------------------------
	SI16 siHungry		;			// 배고픔.		(0~ 256)	기본은 128	

	//-------------------------
	// 기관 상태 
	//-------------------------
	SI08 siInnerOrg1	;		// 소화계 (-100 ~ 100)
	SI08 siInnerOrg2	;		// 순환계 
	SI08 siInnerOrg3	;		// 호흡계 
	SI08 siInnerOrg4	;		// 신경계 

	//------------------------
	// 질병 상태 
	//------------------------
	UI08 uiOrg1Disease	;		// 소화계 질병.			
	UI08 uiOrg2Disease	;		// 순환계 질병. 
	UI08 uiOrg3Disease	;		// 호흡계 질병. 
	UI08 uiOrg4Disease	;		// 신경계 질병. 

	//-----------------------
	// 종합 건강 지수.
	//-----------------------
	UI08 uiHealthPoint;		// 0 ~ 100	:100이면 능력치를 모두 발휘할 수 있따. 

	cltNewLog_HealthInfo()		{		Init();			}

	void Init()
	{
		//------------------------
		// 기초 상태 
		//------------------------
		siHungry		= 0;			// 배고픔.		

		//-------------------------
		// 기관 상태 
		//-------------------------
		siInnerOrg1		= 0;		// 소화계 
		siInnerOrg2		= 0;		// 순환계 
		siInnerOrg3		= 0;		// 호흡계 
		siInnerOrg4		= 0;		// 신경계 

		//------------------------
		// 질병 상태 
		//------------------------
		uiOrg1Disease	= 0;		// 소화계 질병.			
		uiOrg2Disease	= 0;		// 순환계 질병. 
		uiOrg3Disease	= 0;		// 호흡계 질병. 
		uiOrg4Disease	= 0;		// 신경계 질병. 

		//-----------------------
		// 종합 건강 지수.
		//-----------------------
		uiHealthPoint	= 100;
	}
};

#define MAX_DBLOG_SIZE_DATE				24 // DBLog szDate 최대
#define MAX_DBLOG_SIZE_COMMAND			50 // DBLog szCommand 최대
#define MAX_DBLOG_SIZE_CATEGORY			512 // DBLog szCommand 최대
#define MAX_DBLOG_SIZE_LOGTEXT			512 // DBLog szLogTxt 최대
#define MAX_DBLOG_SIZE_SERVERNAME		16 // DBLog szServerName
#define MAX_DBLOG_SIZE_ITEMUNIQUE		10 // DBLog szItemUnique

#define MAX_DBLOG_SIZE_LOGINID			50 // DBLog szLoginID 최대
#define MAX_DBLOG_SIZE_LOGINPW			50 // DBLog szLoginPW 최대

#define MAX_STLOGDATA_PER_PACKET		10

#define MAX_IPSIZE						16	// IP 문자열 크기

enum {
		DBMSG_REQUEST_TEST = 0,
		DBMSG_RESPONSE_TEST,

		DBMSG_REQUEST_LOG,
		DBMSG_RESPONSE_LOG,

		DBMSG_REQUEST_TOOLSLOG,
		DBMSG_RESPONSE_TOOLSLOG,

		DBMSG_REQUEST_NEWLOG,
		DBMSG_RESPONSE_NEWLOG,

		DBMSG_END,
};

// 검색 기준 값
enum {
		FIND_ACCOUNTID = 1,	// 계정명
		FIND_CHARACTER		// 캐릭터
};

//=============================================================================
//	CLASS   X
//! @brief  
//=============================================================================
struct sDBRequest_Test
{
	sPacketHeader	packetHeader;

	UI16			usPacketUnique;
	UI16			usFrontCharID;

};

struct sDBResponse_Test
{
	sPacketHeader	packetHeader;

	UI16			usPacketUnique;	
	UI16			usFrontCharID;

};

//=============================================================================
//	CLASS   stLOGData
//! @brief  
//=============================================================================
class stLOGData
{
public:
	char				szDate[MAX_DBLOG_SIZE_DATE];
	char				szCommand[MAX_DBLOG_SIZE_COMMAND];
	char				szServerName[MAX_DBLOG_SIZE_SERVERNAME];

	char				szPersonName[MAX_PLAYER_NAME];
	char				szDstPersonName[MAX_PLAYER_NAME];
	char				szItemUnique[MAX_DBLOG_SIZE_ITEMUNIQUE];
	char				szLogTxt[MAX_DBLOG_SIZE_LOGTEXT];

	void Init()
	{
		szDate[0]			= '\0';
		szCommand[0]		= '\0';
		szPersonName[0]		= '\0';
		szDstPersonName[0]	= '\0';
		szItemUnique[0]		= '\0';
		szLogTxt[0]			= '\0';
	}

	void Set(stLOGData * stlogdata)
	{
		memcpy ( this, stlogdata, sizeof( stLOGData ));
	}
};


//=============================================================================
//	CLASS   X
//! @brief  
//=============================================================================
struct sDBRequest_Log
{
	sPacketHeader		packetHeader;
	UI16				usCharID;

	char				szLoginID[MAX_DBLOG_SIZE_LOGINID];			// 로그인 ID
	char				szLoginPW[MAX_DBLOG_SIZE_LOGINPW];			// 로그인 PW

	char				szCommand[MAX_DBLOG_SIZE_COMMAND];			// 검색 타입 문자열						
	char				szCategory[MAX_DBLOG_SIZE_CATEGORY];		// 카테고리 데이터 - 'Cmd1','Cmd2','Cmd3' 와 같은 문자열
	char				szServerName[MAX_DBLOG_SIZE_SERVERNAME];	// 서버 이름

	char				szPersonName[MAX_PLAYER_NAME];				// 찾을 캐릭터(SOURCE)의 이름
	char				szDstPersonName[MAX_PLAYER_NAME];			// 찾을 캐릭터(DESTINATION)의 이름
	char				szItemUnique[MAX_DBLOG_SIZE_ITEMUNIQUE];	// 검색 아이템

	char				szStartDate[MAX_DBLOG_SIZE_DATE];			// 검색시작할 날짜 ex:2004-08-10 10:30
	char				szEndDate[MAX_DBLOG_SIZE_DATE];				// 검색 끝낼 날짜 ex:2004-08-10 10:31

	char				szLogText[MAX_DBLOG_SIZE_LOGTEXT];

	sDBRequest_Log()
	{
		packetHeader.usSize = sizeof( sDBRequest_Log );
		packetHeader.usCmd	= DBMSG_REQUEST_LOG;

		szLoginID[0]		= '\0';
		szLoginPW[0]		= '\0';

		szCommand[0]		= '\0';
		szCategory[0]		= '\0';
		szServerName[0]		= '\0';

		szPersonName[0]		= '\0';
		szDstPersonName[0]	= '\0';
		szItemUnique[0]		= '\0';

		szStartDate[0]		= '\0';
		szEndDate[0]		= '\0';

		szLogText[0]		= '\0';

		usCharID			= 0;
	}

	sDBRequest_Log(	UI16 CharID, char* loginid, char* loginpw, 
					char* Command, char* Category, char* ServerName, 
					char* PersonName, char* DstPersonName, char* ItemUnique, char* LogText,
					char* StartDate, char* EndDate)
	{
		packetHeader.usSize = sizeof( sDBRequest_Log );
		packetHeader.usCmd	= DBMSG_REQUEST_LOG;

		szLoginID[0]		= '\0';
		szLoginPW[0]		= '\0';

		szCommand[0]		= '\0';
		szCategory[0]		= '\0';
		szServerName[0]		= '\0';

		szPersonName[0]		= '\0';
		szItemUnique[0]		= '\0';

		szStartDate[0]		= '\0';
		szEndDate[0]		= '\0';

		szLogText[0]		= '\0';

		usCharID			= CharID;

		if(loginid)			MStrCpy(szLoginID,		loginid,		MAX_DBLOG_SIZE_LOGINID);
		if(loginpw)			MStrCpy(szLoginPW,		loginpw,		MAX_DBLOG_SIZE_LOGINPW);		

		if(Command)			MStrCpy(szCommand,		Command,		MAX_DBLOG_SIZE_COMMAND);
		if(Category)		MStrCpy(szCategory,		Category,		MAX_DBLOG_SIZE_CATEGORY);		
		if(ServerName)		MStrCpy(szServerName,	ServerName,		MAX_DBLOG_SIZE_SERVERNAME);

		if(PersonName)		MStrCpy(szPersonName,	PersonName,		MAX_PLAYER_NAME);
		if(DstPersonName)	MStrCpy(szDstPersonName,DstPersonName,	MAX_PLAYER_NAME);
		if(ItemUnique)		MStrCpy(szItemUnique,	ItemUnique,		MAX_DBLOG_SIZE_ITEMUNIQUE);

		if(StartDate)		MStrCpy(szStartDate,	StartDate,		MAX_DBLOG_SIZE_DATE);
		if(EndDate)			MStrCpy(szEndDate,		EndDate,		MAX_DBLOG_SIZE_DATE);

		if(LogText)			MStrCpy(szLogText,		LogText,		MAX_DBLOG_SIZE_LOGTEXT);
	}

	void Set(sDBRequest_Log *pLog)
	{
		if(pLog == NULL)			return;

		memcpy(this, pLog, sizeof(*this));
	}
};

struct sDBResponse_Log
{
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	stLOGData			stLogData[MAX_STLOGDATA_PER_PACKET];

	sDBResponse_Log()
	{
		ZeroMemory( this, sizeof( sDBResponse_Log ) );
	}
};

//=============================================================================
//	CLASS   stNewLOGData
//! @brief  
//=============================================================================
class stNewLOGData
{
public:

	//TCHAR				m_szDate[20];
	SYSTEMTIME			m_stTime;
	TCHAR				m_szIP[16];
	SI16				m_siCommandType;
	SI16				m_siCommandIndex;
	UI08				m_uiReason;
	SI32				m_siIDNum;
	TCHAR				m_szPersonName[MAX_PLAYER_NAME];
	SI32				m_siPersonID;
	SI32				m_siLevel;
	SI64				m_siExp;
	GMONEY				m_siMoney;
	GMONEY				m_siMoneySum;
	cltItem				m_clItem;
	SI32				m_siItemSum;
	cltSummon			m_clSummon;
	SI32				m_siMonKind;
	TCHAR				m_szTargetPersonName[MAX_PLAYER_NAME];
	SI32				m_siTargetPersonID;
	SI32				m_siVillageUnique;
	SI16				m_siNpcKind;
	cltNewLog_HealthInfo	m_clHealthInfo;
	SI32				m_siRPPoint;
	SI32				m_siFameExp;
	SI64				m_siParam1;
	SI64				m_siParam2;
	SI64				m_siParam3;
	SI64				m_siParam4;
	SI64				m_siParam5;
	TCHAR				m_szCharParam1[50];
	TCHAR				m_szCharParam2[50];

	void Init()
	{
		memset(this, 0, sizeof(stNewLOGData));
	}

	void Set(stNewLOGData * stlogdata)
	{
		memcpy ( this, stlogdata, sizeof( stNewLOGData ));
	}
};


//=============================================================================
//	CLASS   X
//! @brief  sDBRequest_NewLog
//=============================================================================
struct sDBRequest_NewLog
{
	sPacketHeader		packetHeader;
	UI16				usCharID;

	char				szLoginID[MAX_DBLOG_SIZE_LOGINID];			// 로그인 ID
	char				szLoginPW[MAX_DBLOG_SIZE_LOGINPW];			// 로그인 PW

	UI08				uiCommandType;
	UI08				uiCommandIndex;
	char				szServerName[MAX_DBLOG_SIZE_SERVERNAME];	// 서버 이름

	UI32				uiFindType;									// 검색 기준 종류
	char				szPersonName[MAX_PLAYER_NAME];				// 찾을 캐릭터(SOURCE)의 이름
	char				szAccountID[MAX_PLAYER_NAME];				// 계정 명
	SI16				siItemUnique;								// 검색 아이템
	SI32				siItemSID;									// 검색 아이템

	char				szStartDate[MAX_DBLOG_SIZE_DATE];			// 검색시작할 날짜 ex:2004-08-10 10:30
	char				szEndDate[MAX_DBLOG_SIZE_DATE];				// 검색 끝낼 날짜 ex:2004-08-10 10:31

	char				szDetailOption[2048];						// 상세 검색 조건

	char				szIP[MAX_IPSIZE];

	sDBRequest_NewLog()
	{
		packetHeader.usSize = sizeof( sDBRequest_NewLog );
		packetHeader.usCmd	= DBMSG_REQUEST_NEWLOG;

		szLoginID[0]		= '\0';
		szLoginPW[0]		= '\0';

		uiCommandType		= 0;
		uiCommandIndex		= 0;
		szServerName[0]		= '\0';

		uiFindType			= 0;
		szPersonName[0]		= '\0';
		szAccountID[0]		= '\0';
		siItemUnique		= 0;
		siItemSID			= 0;

		szStartDate[0]		= '\0';
		szEndDate[0]		= '\0';

		usCharID			= 0;

		szDetailOption[0]	= '\0';

		szIP[0]		= '\0';
	}

	void Set(sDBRequest_NewLog *pLog)
	{
		if(pLog == NULL)			return;

		memcpy(this, pLog, sizeof(*this));
	}
};

struct sDBResponse_NewLog
{
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	stNewLOGData		stLogData[MAX_STLOGDATA_PER_PACKET];

	sDBResponse_NewLog()
	{
		ZeroMemory( this, sizeof( sDBResponse_NewLog ) );
	}
};

//=============================================================================
//	CLASS   X
//! @brief  sDBRequest_ToolsLog
//=============================================================================
struct sDBRequest_ToolsLog
{
	sPacketHeader		packetHeader;
	
	char szServerName[10];	// 서버이름 예) CNC0101
	char szCommand[50];		// 실행한 명령
	char szAccountID[20];	// 계정
	char szCharName[20];	// 캐릭터 이름
	char szLog[256];		// 세부 설명


	sDBRequest_ToolsLog()
	{
		packetHeader.usSize = sizeof( sDBRequest_ToolsLog );
		packetHeader.usCmd	= DBMSG_REQUEST_TOOLSLOG;

		szServerName[0] = '\0';
		szCommand[0] = '\0';
		szAccountID[0] = '\0';
		szCharName[0] = '\0';
		szLog[0] = '\0';
	}

	sDBRequest_ToolsLog( char* ServerName, char* Command, char* AccountID, char* CharName, char* Log )
	{
		packetHeader.usSize = sizeof( sDBRequest_ToolsLog );
		packetHeader.usCmd	= DBMSG_REQUEST_TOOLSLOG;

		memcpy( szServerName, ServerName, 9);
		szCommand[9] = NULL;
		memcpy( szCommand,	  Command, 50);
		szCommand[49] = NULL;
		memcpy( szAccountID, AccountID, 20);
		szAccountID[19] = NULL;
		memcpy( szCharName,  CharName, 20);
		szCharName[19] = NULL;
		memcpy( szLog,		  Log, 256);
		szLog[255] = NULL;
	}
};


#endif
