//---------------------------------
// 2004/8/13 김태곤
//---------------------------------

#include "../../../NLib/NUtil.h"
#include "Event.h"

#include "../../Server/Server.h"
#include "../../CommonLogic/CommonLogic.h"

#include "../Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgType-System.h"
#include "../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../DBManager/GameDBManager_World/DBMsg-PostOffice.h"
#include "../DBManager/GameDBManager_World/DBMsg-Person.h"


extern cltCommonLogic* pclClient;

cltEvent::cltEvent()
{
	Init();
}
cltEvent::~cltEvent()
{
}
void cltEvent::Init()
{
	siVillageUnique	= 0;
	dwLeftClock		= 0;			// 이벤트 잔여시간. 
	dwStartClock	= 0;			// 이벤트 시작 시간. 
	dwLastInfoClock	= 0;
	siHuntmapIndex	= 0;

	bEventSwitch = false;
}

bool cltEventTime::SetActivated(bool bActivated, bool bSendAllChar /* = false */)
{
	m_bEventActivated = bActivated;

	// 전 클라이언트에게 바뀐 상황을 알린다.
	cltGameMsgResponse_EventTime_Update sendMsg(m_szEventName, m_bEventActivated);
	cltMsg clMsg( GAMEMSG_REQUEST_EVENTTIME_UPDATE, sizeof(sendMsg), (BYTE*)&sendMsg );

	if(bSendAllChar)
	{
		SI32 index = 0;
		SI32 id = 0;
		while(id = pclClient->pclCM->GetCharOrder(index))
		{
			index++;
			if(IsPC(id) == false)	continue;

			cltCharServer* pclchar = pclClient->pclCM->GetCharServer(id);
			if(pclchar)
			{
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}

	return m_bEventActivated;
}

cltEventTimeManager::cltEventTimeManager()
{
#ifdef _SAFE_MEMORY
	m_pEventTimes.ZeroMem();
#else
	ZeroMemory(m_pEventTimes, sizeof(m_pEventTimes));
#endif
	m_siCount = 0;
	eventmapindex	= 0;

	m_kUpdateTimer_Action.Init(1 * 1000);		// 1초 업데이트
	m_kUpdateTimer_Action.SetActive(true, ::GetTickCount());

	pclHashTableString = new NHashTableString< int >;
	pclHashTableString->CreateHashTableString( MAX_EVENTTIME_COUNT, MAX_EVENTTIME_COUNT, 64 );

#ifdef _SAFE_MEMORY
	siMonsterID.ZeroMem();
	m_HuntmapIndexByResource.ZeroMem();
#else
	ZeroMemory(siMonsterID, sizeof(siMonsterID));
	ZeroMemory(m_HuntmapIndexByResource,sizeof(m_HuntmapIndexByResource));
#endif
}

cltEventTimeManager::~cltEventTimeManager()
{
	SI32	i;
	for (i=0; i<MAX_EVENTTIME_COUNT; i++)
	{
		cltEventTime* pclEventTime = m_pEventTimes[i];
		if( pclEventTime == NULL)							continue;		

		delete pclEventTime;
		m_pEventTimes[i] = NULL;
	}

	NDelete(pclHashTableString);
}

cltEventTime* cltEventTimeManager::GetEvent( TCHAR* pszEventName )
{
	if( pszEventName == NULL )		return NULL;

	SI32 siNum = 0;
	BOOL bFind = pclHashTableString->Find(pszEventName, siNum);
	if(bFind == false)						return NULL;	
	if(siNum < 0)							return NULL;

	cltEventTime* pclEventTime = m_pEventTimes[siNum];
	if( pclEventTime == NULL)							return NULL;

	return pclEventTime;
}

SI32 cltEventTimeManager::ConvertEventExecuteTypeStringToEnum(TCHAR* pszEventExecuteType)
{
	if     ( _tcscmp(pszEventExecuteType, "EXEC_NONE") == 0 )								return EXEC_NONE;

	else if( _tcscmp(pszEventExecuteType, "EXEC_PERIOD_COLLECTIONITEM") == 0 )				return EXEC_PERIOD_COLLECTIONITEM;
	else if( _tcscmp(pszEventExecuteType, "EXEC_PERIOD_HUNTITEM") == 0 )					return EXEC_PERIOD_HUNTITEM;
	else if( _tcscmp(pszEventExecuteType, "EXEC_PERIOD_MAKEEXPBONUS") == 0 )				return EXEC_PERIOD_MAKEEXPBONUS;
	else if( _tcscmp(pszEventExecuteType, "EXEC_PERIOD_HUNTEXPBONUS") == 0 )				return EXEC_PERIOD_HUNTEXPBONUS;

	else if( _tcscmp(pszEventExecuteType, "EXEC_SHOT_GIVEITEM") == 0 )						return EXEC_SHOT_GIVEITEM;
	else if( _tcscmp(pszEventExecuteType, "EXEC_SHOT_MASTERNOTICE") == 0 )					return EXEC_SHOT_MASTERNOTICE;

	else if( _tcscmp(pszEventExecuteType, "EXEC_SHOT_MONSTER_APPEAR_IN_VILLAGE") == 0 )		return EXEC_SHOT_MONSTER_APPEAR_IN_VILLAGE;
	else if( _tcscmp(pszEventExecuteType, "EXEC_SHOT_MONSTER_APPEAR_OUT_VILLAGE") == 0 )	return EXEC_SHOT_MONSTER_APPEAR_OUT_VILLAGE;
	else if( _tcscmp(pszEventExecuteType, "EXEC_SHOT_MONSTER_APPEAR_RANDOM_VILLAGE") == 0 )	return EXEC_SHOT_MONSTER_APPEAR_RANDOM_VILLAGE;
	//[성웅] 몬스터 출현 이벤트 - 마을에 무작위 출현.	=> 2008-9-17
	else if( _tcscmp(pszEventExecuteType, "EXEC_SHOT_MONSTER_APPEAR_RANDOM_HUNTMAP") == 0 )	return EXEC_SHOT_MONSTER_APPEAR_RANDOM_HUNTMAP;
	// [성웅] 길드 사냥터 소유 길드전 매니저.
	else if( _tcscmp(pszEventExecuteType, "EXEC_SHOT_GUILDWAR") == 0 )						return EXEC_SHOT_GUILDWAR;

	//[지연] 군주데이미션 : 지정 시간에 접속중인 유저에게 미션 부여
	else if( _tcscmp(pszEventExecuteType, "EXEC_SHOT_GIVEMISSION") == 0 )					return EXEC_SHOT_GIVEMISSION;
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 새로운 타입은 이 위로 추가해 주세요
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
	else 
	{
		NTCHARString256 strText( pszEventExecuteType );
		NTCHARString64	strCaption( _T(" 이벤트 타입설정 오류 ") );


		strText += _T(" 타입은 존재하지 않는 타입입니다");
		
		MessageBox(NULL, strText, strCaption, MB_ICONWARNING | MB_OK );
	}
#endif


	return -1;
}

void cltEventTimeManager::Init()
{
	// 하드코딩 추가

	// 한국에서만 발동 되도록 한다.
/*	if ( pclClient->IsWhereServiceArea( ConstServiceArea_Korea ) == true )
	{	
		// 설날 라쿤이벤트 기간 (2008.01.31 ~ 2008.02.10)
		AddEventByTime( TEXT("NewYear_RaccoonTime"), 2008, 1, 31, 0, 0, 2008, 2, 10, 23, 59, true );
		
		UI16 uiEventHour[]	= {2, 6, 10, 14, 18, 22};
		UI16 uiCount		= (UI16)(sizeof(uiEventHour)/sizeof(UI16));
		for (UI16 i=0; i<uiCount; i++)
		{
			// 카이트 라쿤 소환 오전/오후 2,6,10시 정각에 발동(총 6번)
			AddShotEvent( TEXT("NewYear_KiteRaccoon"), -1, -1, -1, uiEventHour[i], 0, true, TEXT("NewYear_RaccoonTime") );
			// 50분이 되면 이벤트 종료 메시지 알림
			AddShotEvent( TEXT("NewYear_KiteRaccoonEnd"), -1, -1, -1, uiEventHour[i], 50, true, TEXT("NewYear_RaccoonTime") );
		}
	}

	// 한국과 대만에서만 발동 되도록 한다.
	if ( pclClient->IsWhereServiceArea( (ConstServiceArea_Korea | ConstServiceArea_Taiwan) ) == true )
	{
		// 설날 네잎클로버 이벤트 기간 (2008.02.06 ~ 2008.02.10)
		AddEventByTime( TEXT("NewYear_FourLeafTime"), 2008, 2, 6, 0, 0, 2008, 2, 10, 23, 59, true );
		// 설날 네잎클로버 이벤트 시간 설정
		AddShotEvent( TEXT("NewYear_FourLeaf"), -1, -1, -1, 20, 8, true, TEXT("NewYear_FourLeafTime") );

		// 설날 전투 보너스 타임 추가
		AddEventByTime( TEXT("NewYear_AttackBonusTime"), 2008, 2, 6, 0, 0, 2008, 2, 6, 23, 59, true );
		AddEventByTime( TEXT("NewYear_AttackBonusTime"), 2008, 2, 8, 0, 0, 2008, 2, 8, 23, 59, true );
		AddEventByTime( TEXT("NewYear_AttackBonusTime"), 2008, 2, 10, 0, 0, 2008, 2, 10, 23, 59, true );
		// 설날 제조 보너스 타임 추가
		AddEventByTime( TEXT("NewYear_ManufactureBonusTime"), 2008, 2, 7, 0, 0, 2008, 2, 7, 23, 59, true );
		AddEventByTime( TEXT("NewYear_ManufactureBonusTime"), 2008, 2, 9, 0, 0, 2008, 2, 9, 23, 59, true );

		// 대만에서는 11일날 하루 더 발동되도록 한다
		if ( pclClient->IsWhereServiceArea( ConstServiceArea_Taiwan ) == true )
		{
			AddEventByTime( TEXT("NewYear_ManufactureBonusTime"), 2008, 2, 11, 0, 0, 2008, 2, 11, 23, 59, true );
		}
	}*/

	LoadEventTimePeriod();
	LoadEventTimeWeek();
	LoadEventTimeShot();
	LoadHuntmapInfo();
}

void cltEventTimeManager::LoadEventTimePeriod()
{
	BOOL bLoadFile = FALSE;
	NDataLoader	dataloader;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/EventTimePeriod.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/EventTimePeriod.txt") );
	}

	if (!bLoadFile)
	{
		return;
	}

	TCHAR szEventName[64]			=	TEXT("");
	TCHAR szEventExecuteType[64]	=	TEXT("");
//	TCHAR szParent[64]				=	TEXT("");

	SI32 sYear, sMonth, sDay, sHour, sMin;
	SI32 eYear, eMonth, eDay, eHour, eMin;

	SI32 siActive;

	SI32 siBonusAmount = 0;
	SI32 siTextMgr = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szEventName,		64,
		NDATA_MBSTRING,	szEventExecuteType,	64,
//		NDATA_MBSTRING,	szParent,			64,
		NDATA_INT32,	&sYear,				4,
		NDATA_INT32,	&sMonth,			4,
		NDATA_INT32,	&sDay,				4,
		NDATA_INT32,	&sHour,				4,
		NDATA_INT32,	&sMin,				4,
		NDATA_INT32,	&eYear,				4,
		NDATA_INT32,	&eMonth,			4,
		NDATA_INT32,	&eDay,				4,
		NDATA_INT32,	&eHour,				4,
		NDATA_INT32,	&eMin,				4,
		NDATA_INT32,	&siActive,			4,
		NDATA_INT32,	&siBonusAmount,		4,
		NDATA_INT32,	&siTextMgr,			4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI16 index = 0;

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData( true ))	
		{
			SI32 siEventExecuteType = ConvertEventExecuteTypeStringToEnum(szEventExecuteType);

			AddEventByTime( szEventName, sYear, sMonth, sDay, sHour, sMin, eYear, eMonth, eDay, eHour, eMin, DATA_TO_BOOL(siActive) );

			cltEventTime* pclEventTime = GetEvent(szEventName);
			if(pclEventTime)
			{
				pclEventTime->m_siEventExecuteType = siEventExecuteType;

				pclEventTime->m_siBonusAmount		= siBonusAmount;
				pclEventTime->m_siTextMgr			= siTextMgr;
			}
		}
	}
}

void cltEventTimeManager::LoadEventTimeWeek()
{
	BOOL bLoadFile = FALSE;
	NDataLoader	dataloader;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/EventTimeWeek.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/EventTimeWeek.txt") );
	}

	if (!bLoadFile)
	{
		return;
	}

	TCHAR szEventName[64]			=	TEXT("");
	TCHAR szEventExecuteType[64]	=	TEXT("");
//	TCHAR szParent[64]				=	TEXT("");

	SI32 sWeek, sHour, sMin;
	SI32 eWeek, eHour, eMin;

	SI32 siActive;

	SI32 siBonusAmount;
	SI32 siTextMgr;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szEventName,		64,
		NDATA_MBSTRING,	szEventExecuteType,	64,
//		NDATA_MBSTRING,	szParent,			64,
		NDATA_INT32,	&sWeek,				4,
		NDATA_INT32,	&sHour,				4,
		NDATA_INT32,	&sMin,				4,
		NDATA_INT32,	&eWeek,				4,
		NDATA_INT32,	&eHour,				4,
		NDATA_INT32,	&eMin,				4,
		NDATA_INT32,	&siActive,			4,
		NDATA_INT32,	&siBonusAmount,		4,
		NDATA_INT32,	&siTextMgr,			4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI16 index = 0;

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData())	
		{
			SI32 siEventExecuteType = ConvertEventExecuteTypeStringToEnum(szEventExecuteType);

			AddEventByWeek(szEventName, sWeek, sHour, sMin, eWeek, eHour, eMin, DATA_TO_BOOL(siActive));			

			cltEventTime* pclEventTime = GetEvent(szEventName);
			if(pclEventTime)
			{
				pclEventTime->m_siEventExecuteType	= siEventExecuteType;

				pclEventTime->m_siBonusAmount		= siBonusAmount;
				pclEventTime->m_siTextMgr			= siTextMgr;
			}
		}
	}
}

void cltEventTimeManager::LoadEventTimeShot()
{
	BOOL bLoadFile = FALSE;
	NDataLoader	dataloader;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/EventTimeShot.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/EventTimeShot.txt") );
	}

	if (!bLoadFile)
	{
		return;
	}

	TCHAR szEventName[64]			=	TEXT("");
	TCHAR szEventExecuteType[64]	=	TEXT("");
	TCHAR szParent[64]				=	TEXT("");

	SI32 Year, Month, Day, Hour, Min;

	SI32 siActive;

	SI32 siItemUnique;
	SI32 siItemNum;
	
	//------------------------------
	//[진성] 캐릭터 출현 정보.
	//------------------------------
	TCHAR szCharKind[64] = TEXT("");			
	SI32 siCharNum;	
	SI32 siCharLevel;	
	SI32 siCharLife;	
	SI32 siVillage;	
	SI32 siPosX;		
	SI32 siPosY;
	SI32 siHuntmapIndex;
	//------------------------------

	SI32 siTextMgr;
	
	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szEventName,		64,
		NDATA_MBSTRING,	szEventExecuteType,	64,
		NDATA_MBSTRING,	szParent,			64,
		NDATA_INT32,	&Year,				4,
		NDATA_INT32,	&Month,				4,
		NDATA_INT32,	&Day,				4,
		NDATA_INT32,	&Hour,				4,
		NDATA_INT32,	&Min,				4,
		NDATA_INT32,	&siActive,			4,
		NDATA_INT32,	&siItemUnique,		4,
		NDATA_INT32,	&siItemNum,			4,
		NDATA_MBSTRING,	&szCharKind,		64,
		NDATA_INT32,	&siCharNum,			4,
		NDATA_INT32,	&siCharLevel,		4,
		NDATA_INT32,	&siCharLife,		4,
		NDATA_INT32,	&siVillage,			4,
		NDATA_INT32,	&siPosX,			4,
		NDATA_INT32,	&siPosY,			4,
		NDATA_INT32,	&siTextMgr,			4,
		NDATA_INT32,	&siHuntmapIndex,	4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI16 index = 0;

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData())	
		{
			SI32 siEventExecuteType = ConvertEventExecuteTypeStringToEnum(szEventExecuteType);

			AddShotEvent(szEventName, Year, Month, Day, Hour, Min, DATA_TO_BOOL(siActive), szParent);

			cltEventTime* pclEventTime = GetEvent(szEventName);
			if(pclEventTime)
			{
				pclEventTime->m_siEventExecuteType	= siEventExecuteType;
				pclEventTime->m_siItemUnique		= siItemUnique;	
				pclEventTime->m_siItemNum			= siItemNum;	
				
				pclEventTime->m_siCharKind			= pclClient->GetUniqueFromHash( TEXT(szCharKind) );	
				pclEventTime->m_siCharNum			= siCharNum;
				pclEventTime->m_siCharLevel			= siCharLevel;	
				pclEventTime->m_siCharLife			= siCharLife;	
				pclEventTime->m_siVillage			= siVillage;
				pclEventTime->m_siPosX				= siPosX;	
				pclEventTime->m_siPosY				= siPosY;

				pclEventTime->m_siTextMgr			= siTextMgr;
				pclEventTime->m_siHuntmapIndex		= siHuntmapIndex;
			}
		}
	}
}
void cltEventTimeManager::LoadHuntmapInfo()
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/EventMapInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/EventMapInfo.txt") );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile HuntmapInfo"),TEXT("Error"));
		return;
	}

	TCHAR maptypename[256] = TEXT(""), mapunique[256] = TEXT("");
	SI32 ptextnum, mapinx;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&ptextnum,   4,		
			NDATA_MBSTRING,	mapunique,	256,
			NDATA_INT32,	&mapinx,	4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );


	SI16 index = 0 ;

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData())	
		{
			if ( mapunique[ 0 ] != NULL)
			{
				if(index > MAX_HUNTMAP_INDEX)
				{
#ifdef _DEBUG
					MessageBox(NULL, "Data/EventMapInfo.txt 개수 초과", "ERROR", MB_OK);
#endif
					continue;
				}
				
				m_HuntmapIndexByResource[ index ]= mapinx;
				index++;
				if( 0 == mapinx )					break;
				SetEventMapIndex( index );
			}
			
		}
	}
}
// 이벤트 맵인포의 맵인덱스의 총 합을 기록한다.
void cltEventTimeManager::SetEventMapIndex( SI16 sieventmapindex )
{
	eventmapindex =  sieventmapindex	;

}
// 이벤트 맵인포의 맵인덱스의 총 합을 가지고 온다.
SI16 cltEventTimeManager::GetEventMapIndex()
{
	return eventmapindex ;

}

// 중복등록 허용
bool cltEventTimeManager::AddEventByTime(TCHAR* pszEventName, SI16 siSYear, SI16 siSMonth, SI16 siSDay, SI16 siSHour, SI16 siSMinute,
										 SI16 siEYear, SI16 siEMonth, SI16 siEDay, SI16 siEHour, SI16 siEMinute, bool Activated )
{
	if( pszEventName == NULL || _tcslen(pszEventName) <= 0 )			return false;

	SI32 siNum = 0;
	bool bFind = pclHashTableString->Find(pszEventName, siNum);
	if(bFind == true)
	{
#ifdef _DEBUG
		MsgBox("ERROR", "[%s]이벤트는 이미 등록되어 있습니다.", pszEventName);
#endif
		return false;
	}

	if( m_siCount >= MAX_EVENTTIME_COUNT )								return false;

	m_pEventTimes[m_siCount] = new cltEventTime();
	if( m_pEventTimes[m_siCount] == NULL)								return false;

	StringCchCopy( m_pEventTimes[m_siCount]->m_szEventName, sizeof(m_pEventTimes[m_siCount]->m_szEventName), pszEventName );

	m_pEventTimes[m_siCount]->m_siEventType			= EVENTTYPE_PERIOD;
	m_pEventTimes[m_siCount]->m_siEventExecuteType	= EXEC_NONE;
	m_pEventTimes[m_siCount]->m_bEventActivated		= Activated;

	m_pEventTimes[m_siCount]->m_StartTime.wYear		= ( siSYear		<0	? MAX_UI16 : siSYear );
	m_pEventTimes[m_siCount]->m_StartTime.wMonth	= ( siSMonth	<0	? MAX_UI16 : siSMonth );
	m_pEventTimes[m_siCount]->m_StartTime.wDay		= ( siSDay		<0	? MAX_UI16 : siSDay );
	m_pEventTimes[m_siCount]->m_StartTime.wHour		= ( siSHour		<0	? MAX_UI16 : siSHour );
	m_pEventTimes[m_siCount]->m_StartTime.wMinute	= ( siSMinute	<0	? MAX_UI16 : siSMinute );
	m_pEventTimes[m_siCount]->m_StartTime.wDayOfWeek= 10;

	m_pEventTimes[m_siCount]->m_EndTime.wYear		= ( siEYear		<0	? MAX_UI16 : siEYear );
	m_pEventTimes[m_siCount]->m_EndTime.wMonth		= ( siEMonth	<0	? MAX_UI16 : siEMonth );
	m_pEventTimes[m_siCount]->m_EndTime.wDay		= ( siEDay		<0	? MAX_UI16 : siEDay );
	m_pEventTimes[m_siCount]->m_EndTime.wHour		= ( siEHour		<0	? MAX_UI16 : siEHour );
	m_pEventTimes[m_siCount]->m_EndTime.wMinute		= ( siEMinute	<0	? MAX_UI16 : siEMinute );
	m_pEventTimes[m_siCount]->m_EndTime.wDayOfWeek	= 10;

	// 마치는 시각이 23시 59분이면, 하루 종일에 해당하는 시간으로 변경한다.
	// [ 23:59:59.999 ]
	if(	m_pEventTimes[m_siCount]->m_EndTime.wHour	== 23 &&
		m_pEventTimes[m_siCount]->m_EndTime.wMinute	== 59 )
	{
		m_pEventTimes[m_siCount]->m_EndTime.wHour	= 23;
		m_pEventTimes[m_siCount]->m_EndTime.wMinute = 59;
		m_pEventTimes[m_siCount]->m_EndTime.wSecond = 59;
		m_pEventTimes[m_siCount]->m_EndTime.wMilliseconds = 999;
	}

	// 마치는 시각이 24시 00분이면, 하루 종일에 해당하는 시간으로 변경한다.
	// [ 23:59:59.999 ]
	if(	m_pEventTimes[m_siCount]->m_EndTime.wHour	== 24 &&
		m_pEventTimes[m_siCount]->m_EndTime.wMinute	== 0 )
	{
		m_pEventTimes[m_siCount]->m_EndTime.wHour	= 23;
		m_pEventTimes[m_siCount]->m_EndTime.wMinute = 59;
		m_pEventTimes[m_siCount]->m_EndTime.wSecond = 59;
		m_pEventTimes[m_siCount]->m_EndTime.wMilliseconds = 999;
	}

	pclHashTableString->Insert( pszEventName, m_siCount );

	m_siCount++;

	return true;
}

bool cltEventTimeManager::AddEventByWeek( TCHAR* pszEventName, SI16 siSWeek, SI16 siSHour, SI16 siSMinute, SI16 siEWeek, SI16 siEHour, SI16 siEMinute, bool Activated )
{
	if( pszEventName == NULL || _tcslen(pszEventName) <= 0 )			return false;

	SI32 siNum = 0;
	bool bFind = pclHashTableString->Find(pszEventName, siNum);
	if(bFind == true)
	{
#ifdef _DEBUG
		MsgBox("ERROR", "[%s]이벤트는 이미 등록되어 있습니다.", pszEventName);
#endif
		return false;
	}

	if( m_siCount >= MAX_EVENTTIME_COUNT )								return false;
	if( siSWeek < 0 || siSWeek > 6 || siEWeek < 0 || siEWeek > 6 )		return false;

	m_pEventTimes[m_siCount] = new cltEventTime();
	if( m_pEventTimes[m_siCount] == NULL)								return false;

	StringCchCopy( m_pEventTimes[m_siCount]->m_szEventName, sizeof(m_pEventTimes[m_siCount]->m_szEventName), pszEventName );

	m_pEventTimes[m_siCount]->m_siEventType			= EVENTTYPE_PERIOD;
	m_pEventTimes[m_siCount]->m_siEventExecuteType	= EXEC_NONE;
	m_pEventTimes[m_siCount]->m_bEventActivated		= Activated;

	m_pEventTimes[m_siCount]->m_StartTime.wDayOfWeek= siSWeek;
	m_pEventTimes[m_siCount]->m_StartTime.wHour		= ( siSHour		< 0 ? MAX_UI16 : siSHour );
	m_pEventTimes[m_siCount]->m_StartTime.wMinute	= ( siSMinute	< 0 ? MAX_UI16 : siSMinute );

	m_pEventTimes[m_siCount]->m_StartTime.wYear		= MAX_UI16;
	m_pEventTimes[m_siCount]->m_StartTime.wMonth	= MAX_UI16;
	m_pEventTimes[m_siCount]->m_StartTime.wDay		= MAX_UI16;

	m_pEventTimes[m_siCount]->m_EndTime.wDayOfWeek	= siEWeek;
	m_pEventTimes[m_siCount]->m_EndTime.wHour		= ( siEHour		< 0 ? MAX_UI16 : siEHour );
	m_pEventTimes[m_siCount]->m_EndTime.wMinute		= ( siEMinute	< 0 ? MAX_UI16 : siEMinute );

	m_pEventTimes[m_siCount]->m_EndTime.wYear		= MAX_UI16;
	m_pEventTimes[m_siCount]->m_EndTime.wMonth		= MAX_UI16;
	m_pEventTimes[m_siCount]->m_EndTime.wDay		= MAX_UI16;

	// 마치는 시각이 23시 59분이면, 하루 종일에 해당하는 시간으로 변경한다.
	// [ 23:59:59.999 ]
	if(	m_pEventTimes[m_siCount]->m_EndTime.wHour	== 23 &&
		m_pEventTimes[m_siCount]->m_EndTime.wMinute	== 59 )
	{
		m_pEventTimes[m_siCount]->m_EndTime.wHour	= 23;
		m_pEventTimes[m_siCount]->m_EndTime.wMinute = 59;
		m_pEventTimes[m_siCount]->m_EndTime.wSecond = 59;
		m_pEventTimes[m_siCount]->m_EndTime.wMilliseconds = 999;
	}

	// 마치는 시각이 24시 00분이면, 하루 종일에 해당하는 시간으로 변경한다.
	// [ 23:59:59.999 ]
	if(	m_pEventTimes[m_siCount]->m_EndTime.wHour	== 24 &&
		m_pEventTimes[m_siCount]->m_EndTime.wMinute	== 0 )
	{
		m_pEventTimes[m_siCount]->m_EndTime.wHour	= 23;
		m_pEventTimes[m_siCount]->m_EndTime.wMinute = 59;
		m_pEventTimes[m_siCount]->m_EndTime.wSecond = 59;
		m_pEventTimes[m_siCount]->m_EndTime.wMilliseconds = 999;
	}

	pclHashTableString->Insert( pszEventName, m_siCount );

	m_siCount++;

	return true;
}

bool cltEventTimeManager::AddShotEvent( TCHAR* pszEventName, SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, SI16 siMinute, bool Activated, TCHAR* pszParentName /*= NULL*/ )
{
	if( pszEventName == NULL || _tcslen(pszEventName) <= 0 )			return false;

	SI32 siNum = 0;
	bool bFind = pclHashTableString->Find(pszEventName, siNum);
	if(bFind == true)
	{
#ifdef _DEBUG
		MsgBox("ERROR", "[%s]이벤트는 이미 등록되어 있습니다.", pszEventName);
#endif
		return false;
	}

	if( m_siCount >= MAX_EVENTTIME_COUNT )								return false;

	m_pEventTimes[m_siCount] = new cltEventTime();
	if( m_pEventTimes[m_siCount] == NULL)								return false;

	StringCchCopy( m_pEventTimes[m_siCount]->m_szEventName, sizeof(m_pEventTimes[m_siCount]->m_szEventName), pszEventName );

	m_pEventTimes[m_siCount]->m_siEventType			= EVENTTYPE_SHOT;
	m_pEventTimes[m_siCount]->m_siEventExecuteType	= EXEC_NONE;
	m_pEventTimes[m_siCount]->m_bEventActivated		= Activated;

	m_pEventTimes[m_siCount]->m_StartTime.wYear		= ( siYear	< 0 ? MAX_UI16 : siYear );
	m_pEventTimes[m_siCount]->m_StartTime.wMonth	= ( siMonth < 0 ? MAX_UI16 : siMonth );
	m_pEventTimes[m_siCount]->m_StartTime.wDay		= ( siDay	< 0 ? MAX_UI16 : siDay );
	m_pEventTimes[m_siCount]->m_StartTime.wHour		= ( siHour	< 0 ? MAX_UI16 : siHour );
	m_pEventTimes[m_siCount]->m_StartTime.wMinute	= ( siMinute< 0 ? MAX_UI16 : siMinute );
	m_pEventTimes[m_siCount]->m_StartTime.wDayOfWeek= 10;

	if(pszParentName)		StringCchCopy( m_pEventTimes[m_siCount]->m_szParentEventName, sizeof(m_pEventTimes[m_siCount]->m_szParentEventName), pszParentName );

	pclHashTableString->Insert( pszEventName, m_siCount );

	m_siCount++;

	return true;
}

bool cltEventTimeManager::InEventTime( TCHAR* pszEventName, SYSTEMTIME* pCurTime )
{
	if( pszEventName == NULL || _tcslen(pszEventName) <= 0 )			return false;
	if( pCurTime == NULL )												return false;

	SI32 siNum = -1;
	BOOL bFind;

	bFind = pclHashTableString->Find(pszEventName, siNum);
	if(bFind == false)						return false;	
	if(siNum < 0)							return false;

	if( m_pEventTimes[siNum] != NULL &&
		_tcscmp(m_pEventTimes[siNum]->m_szEventName, pszEventName) == 0 )
	{
		// 이벤트가 켜져있지 않으면 false
		if( m_pEventTimes[siNum]->m_bEventActivated == false )		return false;

		bool	bCheckPass = true;

		SYSTEMTIME	tmSTime;	memcpy(&tmSTime, &(m_pEventTimes[siNum]->m_StartTime), sizeof(tmSTime));
		SYSTEMTIME	tmETime;	memcpy(&tmETime, &(m_pEventTimes[siNum]->m_EndTime), sizeof(tmETime));

		if( tmSTime.wDayOfWeek != 10 || tmETime.wDayOfWeek != 10 )
		{
			if( pCurTime->wDayOfWeek < tmSTime.wDayOfWeek ||
				tmETime.wDayOfWeek < pCurTime->wDayOfWeek )
			{
				bCheckPass = false;
			}
		}

		if(bCheckPass)
		{
			// 0으로 설정된 값은 무시되는 값으로 오늘 데이터로 채운다.
			// ex) Year, Month, Day 가 없는 경우 매일매일 가동되는 이벤트로,
			// 오늘 날짜를 사용하여 시간만 체크한다.
			if( tmSTime.wYear	== MAX_UI16 )		tmSTime.wYear = pCurTime->wYear;
			if( tmETime.wYear	== MAX_UI16 )		tmETime.wYear = pCurTime->wYear;

			if( tmSTime.wMonth	== MAX_UI16 )		tmSTime.wMonth = pCurTime->wMonth;
			if( tmETime.wMonth	== MAX_UI16 )		tmETime.wMonth = pCurTime->wMonth;

			if( tmSTime.wDay	== MAX_UI16 )		tmSTime.wDay = pCurTime->wDay;
			if( tmETime.wDay	== MAX_UI16 )		tmETime.wDay = pCurTime->wDay;

			if( tmSTime.wHour	== MAX_UI16 )		tmSTime.wHour = pCurTime->wHour;
			if( tmETime.wHour	== MAX_UI16 )		tmETime.wHour = pCurTime->wHour;


			// 일반적인 시간 체크
			if( TimeInBounds(pCurTime, &tmSTime, &tmETime) )
				return true;
		}
	}

	return false;
}

SI08 cltEventTimeManager::GetEventType( TCHAR* pszEventName )
{
	SI32 siNum = -1;
	BOOL bFind;

	bFind = pclHashTableString->Find(pszEventName, siNum);
	if(bFind == false)						return EVENTTYPE_NONE;	
	if(siNum < 0)							return EVENTTYPE_NONE;

	cltEventTime* pclEventTime = m_pEventTimes[siNum];
	if( pclEventTime == NULL)							return EVENTTYPE_NONE;

	if( _tcscmp(m_pEventTimes[siNum]->m_szEventName, pszEventName) == 0 )
	{
		return pclEventTime->m_siEventType;
	}

	return EVENTTYPE_NONE;
}
bool cltEventTimeManager::IsEventStartTiem( TCHAR* pszEventName,SYSTEMTIME* pCurTime )
{
	// 시간과 분만 체크 합니다.
	if( pszEventName == NULL || _tcslen(pszEventName) <= 0 )			return false;
	if( pCurTime == NULL )												return false;

	SI32 siNum = -1;
	BOOL bFind;

	bFind = pclHashTableString->Find(pszEventName, siNum);
	if(bFind == false)						return false;	
	if(siNum < 0)							return false;

	if( m_pEventTimes[siNum] != NULL &&
		_tcscmp(m_pEventTimes[siNum]->m_szEventName, pszEventName) == 0 )
	{
		// 이벤트가 켜져있지 않으면 false
		if( m_pEventTimes[siNum]->m_bEventActivated == false )		return false;

		bool	bCheckPass = true;

		SYSTEMTIME	tmSTime;	memcpy(&tmSTime, &(m_pEventTimes[siNum]->m_StartTime), sizeof(tmSTime));

		if(bCheckPass)
		{
			if ( tmSTime.wHour	== pCurTime->wHour &&	tmSTime.wMinute	== pCurTime->wMinute ) return true;
		}
	}

	return false;
}
bool cltEventTimeManager::IsEventStartTiemOverMinute( TCHAR* pszEventName,SYSTEMTIME* pCurTime , SI32 siOvertime )
{
	// 시간과 분만 체크 합니다.
	if( pszEventName == NULL || _tcslen(pszEventName) <= 0 )			return false;
	if( pCurTime == NULL )												return false;

	SI32 siNum = -1;
	BOOL bFind;

	bFind = pclHashTableString->Find(pszEventName, siNum);
	if(bFind == false)						return false;	
	if(siNum < 0)							return false;

	if( m_pEventTimes[siNum] != NULL &&
		_tcscmp(m_pEventTimes[siNum]->m_szEventName, pszEventName) == 0 )
	{
		// 이벤트가 켜져있지 않으면 false
		if( m_pEventTimes[siNum]->m_bEventActivated == false )		return false;

		bool	bCheckPass = true;

		SYSTEMTIME	tmSTime;	memcpy(&tmSTime, &(m_pEventTimes[siNum]->m_StartTime), sizeof(tmSTime));

		if(bCheckPass)
		{
			// 0으로 설정된 값은 무시되는 값으로 오늘 데이터로 채운다.
			// ex) Year, Month, Day 가 없는 경우 매일매일 가동되는 이벤트로,
			// 오늘 날짜를 사용하여 시간만 체크한다.
			if( tmSTime.wHour	== MAX_UI16 )		tmSTime.wHour = pCurTime->wHour;
			if( tmSTime.wMinute	== MAX_UI16 )		tmSTime.wHour = pCurTime->wMinute;
			if ( pCurTime->wHour == tmSTime.wHour && pCurTime->wMinute+siOvertime == tmSTime.wMinute) 	return true;
		}
	}

	return false;
}
bool cltEventTimeManager::IsEventEndTiem( TCHAR* pszEventName, SYSTEMTIME* pCurTime )
{
	// 시간과 분만 체크 합니다.
	if( pszEventName == NULL || _tcslen(pszEventName) <= 0 )			return false;
	if( pCurTime == NULL )												return false;

	SI32 siNum = -1;
	BOOL bFind;

	bFind = pclHashTableString->Find(pszEventName, siNum);
	if(bFind == false)						return false;	
	if(siNum < 0)							return false;

	if( m_pEventTimes[siNum] != NULL &&
		_tcscmp(m_pEventTimes[siNum]->m_szEventName, pszEventName) == 0 )
	{
		// 이벤트가 켜져있지 않으면 false
		if( m_pEventTimes[siNum]->m_bEventActivated == false )		return false;

		bool	bCheckPass = true;

		SYSTEMTIME	tmETime;	memcpy(&tmETime, &(m_pEventTimes[siNum]->m_EndTime), sizeof(tmETime));

		if(bCheckPass)
		{
			// 0으로 설정된 값은 무시되는 값으로 오늘 데이터로 채운다.
			// ex) Year, Month, Day 가 없는 경우 매일매일 가동되는 이벤트로,
			// 오늘 날짜를 사용하여 시간만 체크한다.
			if( tmETime.wHour	== MAX_UI16 )		tmETime.wHour = pCurTime->wHour;
			if( tmETime.wMinute	== MAX_UI16 )		tmETime.wHour = pCurTime->wMinute;
			if ( pCurTime->wHour == tmETime.wHour && pCurTime->wMinute == tmETime.wMinute) 	return true;
		}
	}

	return false;
}

void cltEventTimeManager::Action()
{
	SI32	i;
	if(m_kUpdateTimer_Action.IsTimed(::GetTickCount()) == false)		return;

	SYSTEMTIME	tm;		GetLocalTime(&tm);

	for (i=0; i<m_siCount; i++)
	{
		cltEventTime* pclEventTime = m_pEventTimes[i];
		if( pclEventTime == NULL)							continue;		

		if( pclEventTime->m_siEventType == EVENTTYPE_PERIOD )
		{
			if( InEventTime( pclEventTime->m_szEventName, &tm) )
			{
				switch( pclEventTime->m_siEventExecuteType )
				{
				case EXEC_PERIOD_COLLECTIONITEM:	EventPeriod_CollectionItem(  pclEventTime );	break;
				case EXEC_PERIOD_HUNTITEM:			EventPeriod_HuntItem(  pclEventTime );			break;
				case EXEC_PERIOD_MAKEEXPBONUS:		EventPeriod_MakeExpBonus(  pclEventTime );		break;
				case EXEC_PERIOD_HUNTEXPBONUS:		EventPeriod_HuntExpBonus(  pclEventTime );		break;
				}
			}
			else
			{
				pclEventTime->m_kUpdateTimer_Event.SetActive(false, ::GetTickCount());
			}
		}
		else if( pclEventTime->m_siEventType == EVENTTYPE_SHOT )
		{
			if( _tcslen(pclEventTime->m_szParentEventName) > 0 )
			{
				// 부모는 항상 기간이어야 한다.
				if( GetEventType( pclEventTime->m_szParentEventName ) != EVENTTYPE_PERIOD )	continue;

				// 부모의 타임안에 들어오지 않으면, 처리하지 않는다.
				if( InEventTime( pclEventTime->m_szParentEventName, &tm) == false )	continue;
			}

			SYSTEMTIME	tmTime;	memcpy(&tmTime, &(pclEventTime->m_StartTime), sizeof(tmTime));
			SYSTEMTIME	tmCur;	GetLocalTime(&tmCur);

			// 0으로 설정된 값은 무시되는 값으로 오늘 데이터로 채운다.
			// ex) Year, Month, Day 가 없는 경우 매일매일 가동되는 이벤트로,
			// 오늘 날짜를 사용하여 시간만 체크한다.
			if( tmTime.wYear	== MAX_UI16 )	tmTime.wYear	= tmCur.wYear;
			if( tmTime.wMonth	== MAX_UI16 )	tmTime.wMonth	= tmCur.wMonth;
			if( tmTime.wDay		== MAX_UI16 )	tmTime.wDay		= tmCur.wDay;
			if( tmTime.wHour	== MAX_UI16 )	tmTime.wHour	= tmCur.wHour;


			if ( tmTime.wYear	== tmCur.wYear	&&
				tmTime.wMonth	== tmCur.wMonth &&
				tmTime.wDay		== tmCur.wDay	&&
				tmTime.wHour	== tmCur.wHour	&&
				tmTime.wMinute	== tmCur.wMinute )
			{
				if( pclEventTime->m_bEventShoted == false )
				{
					// 이벤트가 켜져있지 않으면 처리하지 않는다.
					if( pclEventTime->m_bEventActivated == true )
					{
						EventAction( pclEventTime );
						pclEventTime->m_bEventShoted = true;
					}
				}
			}
			else
			{
				if ( EXEC_SHOT_GIVEMISSION != pclEventTime->m_siEventExecuteType )
				{
					pclEventTime->m_bEventShoted = false;
				}
			}
		}
	}
}

// 임시 하드코딩
bool cltEventTimeManager::EventAction( cltEventTime* pclEventTime )
{
	cltServer*	pclserver			= (cltServer*)pclClient;
	bool		bKiteRaccoonEvent	= false;

	
	switch( pclEventTime->m_siEventExecuteType )
	{
	case EXEC_SHOT_GIVEITEM:
		{
			EventAction_GiveItem( pclEventTime );
		}
		break;
	case EXEC_SHOT_MASTERNOTICE:
		{
			EventAction_MasterNotice( pclEventTime );
		}
		break;
	case EXEC_SHOT_GIVEMISSION:
		{
			EventAction_GoonzuDay_GiveMission( pclEventTime );
		}
		break;
	case EXEC_SHOT_MONSTER_APPEAR_IN_VILLAGE:
	case EXEC_SHOT_MONSTER_APPEAR_OUT_VILLAGE:
	case EXEC_SHOT_MONSTER_APPEAR_RANDOM_VILLAGE:
		{
			EventAction_MonsterAppear( pclEventTime->m_siEventExecuteType, pclEventTime );
			EventAction_MonsterAppearNotice( pclEventTime->m_siEventExecuteType, pclEventTime );
		}
		break;
	case EXEC_SHOT_MONSTER_APPEAR_RANDOM_HUNTMAP:
		{
			EventAction_MonsterAppear( pclEventTime->m_siEventExecuteType, pclEventTime );
		}
	case EXEC_SHOT_GUILDWAR:// [성웅] 길드 사냥터 소유 길드전 매니저.
		{

		}
	case EXEC_NONE:
		{
			// 우선 각각의 어떤 이벤트가 발생했는지를 체크 함
			if ( _tcscmp(pclEventTime->m_szEventName, TEXT("NewYear_KiteRaccoon")) == 0 )
			{
				bKiteRaccoonEvent = true;
			}
			// 카이트 라쿤이 이벤트가 종료되었다는 메시지 알림
			else if ( _tcscmp(pclEventTime->m_szEventName, TEXT("NewYear_KiteRaccoonEnd")) == 0 )
			{
				cltGameMsgResponse_KiteRaccoonInfo clKiteRaccoonInfo( KITERACCOON_EVENT_END );
				cltMsg clMsg( GAMEMSG_RESPONSE_KITERACCOONINFO, sizeof(clKiteRaccoonInfo), (BYTE*)&clKiteRaccoonInfo );

				pclserver->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );

				return true;
			}
			else if ( _tcscmp(pclEventTime->m_szEventName, TEXT("NewYear_FourLeaf")) == 0 )
			{
				// 네잎 클로버 지급은 간단하기 때문에 이곳에서 바로 처리
				SI32 siItemUnique	= 2755;	// 네잎 클로버
				SI16 siItemNum		= 1;	// 지급 갯수
				SI32 siItemRef		= pclserver->pclItemManager->FindItemRefFromUnique( siItemUnique );
				UI16 uiDateUseDay	= pclserver->pclItemManager->pclItemInfo[siItemRef]->clItem.uiDateUseDay;


				pclserver->pclCM->GiveItem( 0, siItemUnique, siItemNum, GIVEITEM_MODE_ALL, INPUTITEMPERSON_OPTION_EVENT, uiDateUseDay );

				return true;
			}

			// 라쿤이벤트 시간이라면
			if ( bKiteRaccoonEvent == true ) 
			{
				cltInnerStatusInfo	clinnerstatus;

				SI32				siX				= 29;	// 몬스터가 나오는 X 좌표
				SI32				siY				= 29;	// 몬스터가 나오는 Y 좌표
				SI32				siLevel			= 50;	// 몬스터의 레벨
				SI32				siUnique		= 0;	// 생성되야하는 몬스터의 유니크 
				SI32				siID			= 0;	// 생성되는 몬스터의 아이디

				// 모든 PC와 우호적, 사람과는 적대적. 
				clinnerstatus.clEnemyType.Set( ENEMYTYPE_ALLPC, 0 );

				// 현재 소환해야될 몬스터의 유니크 값을 얻어온다
				siUnique = pclserver->GetUniqueFromHash(TEXT("KIND_KITERACCOON"));

				// 혹시 모를 오류를 방지하기 위해 0이면 리턴
				if ( siUnique == 0 ) return false;

				// 생성된 모든 마을안에 몬스터를 소환한다
				for ( UI16 i=0; i<MAX_VILLAGE_NUMBER; i++ )
				{
					// 마을이 없다면 바로 컨티뉴
					if( pclserver->pclVillageManager->pclVillageInfo[i] == NULL ) continue;

					siID = 0;

					// 해당 유니크의 몬스터를 마을에 소환한다.
					siID = pclserver->SetNPCChar(siUnique, i, NULL, siLevel, 0, siX, siY, i, NPCITEMMODE_NORMAL, &clinnerstatus);

					// 시간 제한(50분이 지나면 죽는다)이 있는 몬스터 설정			
					if ( pclserver->pclCM->IsValidID( siID ) )
					{
						((cltCharServer*)pclserver->pclCM->CR[siID])->dwTimerClock	= pclserver->CurrentClock;
					}
				}

				// 카이트 라쿤이 생성되었다는 메시지 보냄
				cltGameMsgResponse_KiteRaccoonInfo clKiteRaccoonInfo( KITERACCOON_EVENT_START, siUnique );
				cltMsg clMsg( GAMEMSG_RESPONSE_KITERACCOONINFO, sizeof(clKiteRaccoonInfo), (BYTE*)&clKiteRaccoonInfo );

				pclserver->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );

				return true;
			}
		}
		break;
	}

	return false;
}

bool cltEventTimeManager::EventPeriod_CollectionItem( cltEventTime* pclEventTime )
{
	if( pclEventTime->m_kUpdateTimer_Event.IsActivated() == false )
	{
		pclEventTime->m_kUpdateTimer_Event.Init(10 * 60 * 1000);
		pclEventTime->m_kUpdateTimer_Event.SetActive(true, ::GetTickCount());
	}

	if( pclEventTime->m_kUpdateTimer_Event.IsTimed( ::GetTickCount()) )
	{
		NTCHARString128 msgtext;
		if(pclEventTime->m_siTextMgr > 0)
		{
			msgtext = GetTxtFromMgr(pclEventTime->m_siTextMgr); 

			TCHAR	szTimeStr[64];
			StringCchPrintf(szTimeStr, sizeof(szTimeStr), TEXT("%02d:%02d~%02d:%02d"),	pclEventTime->m_StartTime.wHour, pclEventTime->m_StartTime.wMinute,
				pclEventTime->m_EndTime.wHour, pclEventTime->m_EndTime.wMinute );
			msgtext.Replace("#time#", szTimeStr);
			msgtext.Replace("#bonus#", SI32ToString(pclEventTime->m_siBonusAmount));

			TCHAR* pText = GetTxtFromMgr(5482); 

			// 운영자가 하는것이라면 채팅창에도 나타나게한다.
			//KHY - 0129 - 하단 채팅창에 공지 출력여부를 결정한다.
			cltGameMsgResponse_Notice clNotice(pText, msgtext, true);
			cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

			pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
		}
	}

	return false;
}

bool cltEventTimeManager::EventPeriod_HuntItem( cltEventTime* pclEventTime )
{
	if( pclEventTime->m_kUpdateTimer_Event.IsActivated() == false )
	{
		pclEventTime->m_kUpdateTimer_Event.Init(5 * 60 * 1000);
		pclEventTime->m_kUpdateTimer_Event.SetActive(true, ::GetTickCount());
	}

	if( pclEventTime->m_kUpdateTimer_Event.IsTimed( ::GetTickCount()) )
	{
		NTCHARString128 msgtext;
		if(pclEventTime->m_siTextMgr > 0)
		{
			msgtext = GetTxtFromMgr(pclEventTime->m_siTextMgr); 

			TCHAR	szTimeStr[64];
			StringCchPrintf(szTimeStr, sizeof(szTimeStr), TEXT("%02d:%02d~%02d:%02d"),	pclEventTime->m_StartTime.wHour, pclEventTime->m_StartTime.wMinute,
				pclEventTime->m_EndTime.wHour, pclEventTime->m_EndTime.wMinute );
			msgtext.Replace("#time#", szTimeStr);
			msgtext.Replace("#bonus#", SI32ToString(pclEventTime->m_siBonusAmount));

			TCHAR* pText = GetTxtFromMgr(5482); 

			// 운영자가 하는것이라면 채팅창에도 나타나게한다.
			//KHY - 0129 - 하단 채팅창에 공지 출력여부를 결정한다.
			cltGameMsgResponse_Notice clNotice(pText, msgtext, true);
			cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

			pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
		}
	}

	return false;
}

bool cltEventTimeManager::EventPeriod_MakeExpBonus( cltEventTime* pclEventTime )
{
	if( pclEventTime->m_kUpdateTimer_Event.IsActivated() == false )
	{
		pclEventTime->m_kUpdateTimer_Event.Init(5 * 60 * 1000);
		pclEventTime->m_kUpdateTimer_Event.SetActive(true, ::GetTickCount());
	}

	if( pclEventTime->m_kUpdateTimer_Event.IsTimed( ::GetTickCount()) )
	{
		NTCHARString128 msgtext;
		if(pclEventTime->m_siTextMgr > 0)
		{
			msgtext = GetTxtFromMgr(pclEventTime->m_siTextMgr); 

			TCHAR	szTimeStr[64];
			StringCchPrintf(szTimeStr, sizeof(szTimeStr), TEXT("%02d:%02d~%02d:%02d"),	pclEventTime->m_StartTime.wHour, pclEventTime->m_StartTime.wMinute,
																						pclEventTime->m_EndTime.wHour, pclEventTime->m_EndTime.wMinute );
			msgtext.Replace("#time#", szTimeStr);
			msgtext.Replace("#bonus#", SI32ToString(pclEventTime->m_siBonusAmount + 100));

			TCHAR* pText = GetTxtFromMgr(5482); 

			// 운영자가 하는것이라면 채팅창에도 나타나게한다.
			//KHY - 0129 - 하단 채팅창에 공지 출력여부를 결정한다.
			cltGameMsgResponse_Notice clNotice(pText, msgtext, true);
			cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

			pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
		}
	}

	return false;
}

bool cltEventTimeManager::EventPeriod_HuntExpBonus( cltEventTime* pclEventTime )
{
	if( pclEventTime->m_kUpdateTimer_Event.IsActivated() == false )
	{
		pclEventTime->m_kUpdateTimer_Event.Init(5 * 60 * 1000);
		pclEventTime->m_kUpdateTimer_Event.SetActive(true, ::GetTickCount());
	}

	if( pclEventTime->m_kUpdateTimer_Event.IsTimed( ::GetTickCount()) )
	{
		NTCHARString128 msgtext;
		if(pclEventTime->m_siTextMgr > 0)
		{
			msgtext = GetTxtFromMgr(pclEventTime->m_siTextMgr); 

			TCHAR	szTimeStr[64];
			StringCchPrintf(szTimeStr, sizeof(szTimeStr), TEXT("%02d:%02d~%02d:%02d"),	pclEventTime->m_StartTime.wHour, pclEventTime->m_StartTime.wMinute,
																						pclEventTime->m_EndTime.wHour, pclEventTime->m_EndTime.wMinute );
			msgtext.Replace("#time#", szTimeStr);
			msgtext.Replace("#bonus#", SI32ToString(pclEventTime->m_siBonusAmount + 100));

			TCHAR* pText = GetTxtFromMgr(5482); 

			// 운영자가 하는것이라면 채팅창에도 나타나게한다.
			//KHY - 0129 - 하단 채팅창에 공지 출력여부를 결정한다.
			cltGameMsgResponse_Notice clNotice(pText, msgtext, true);
			cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

			pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
		}
	}

	return false;
}

bool cltEventTimeManager::EventAction_GiveItem( cltEventTime* pclEventTime )
{
	cltServer* pclserver = (cltServer*)pclClient;

	SI32 siItemRef		= pclserver->pclItemManager->FindItemRefFromUnique( pclEventTime->m_siItemUnique );	
	UI16 uiDateUseDay	= pclserver->pclItemManager->pclItemInfo[siItemRef]->clItem.uiDateUseDay;

	// 레벨 제한 아이템 지급
	if( pclEventTime->m_siCharLevel > 0)
	{
		pclserver->pclCM->GiveItem( 0, pclEventTime->m_siItemUnique, pclEventTime->m_siItemNum, GIVEITEM_MODE_MINLEVEL, INPUTITEMPERSON_OPTION_EVENT, uiDateUseDay, 0, pclEventTime->m_siCharLevel );
	}
	else if( pclEventTime->m_siVillage > 0 )
	{
		if( pclEventTime->m_siVillage < 1000 )
		{
			pclserver->pclCM->GiveItem( 0, pclEventTime->m_siItemUnique, pclEventTime->m_siItemNum, GIVEITEM_MODE_VILLAGER, INPUTITEMPERSON_OPTION_EVENT, uiDateUseDay, 0, 0, pclEventTime->m_siVillage );
		}
		else
		{
			pclserver->pclCM->GiveItem( 0, pclEventTime->m_siItemUnique, pclEventTime->m_siItemNum, GIVEITEM_MODE_VILLAGER, INPUTITEMPERSON_OPTION_EVENT, uiDateUseDay, 0, 0, 0 );
		}
	}
	else
	{
		pclserver->pclCM->GiveItem( 0, pclEventTime->m_siItemUnique, pclEventTime->m_siItemNum, GIVEITEM_MODE_ALL, INPUTITEMPERSON_OPTION_EVENT, uiDateUseDay );
	}

	if( _tcscmp(pclEventTime->m_szParentEventName, "HalloweenEvent") == 0 )
	{
		// 할로윈 이벤트 아이템 배포는 특별한 창을 띄워준다.
		cltGameMsgResponse_Notice_Quset clNotice( 0, 0, HALLOWEENSUPRISE,0,0 );
		cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_QUEST, sizeof( clNotice ), (BYTE*)&clNotice );
		pclClient->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
	}

	return true;
}

bool cltEventTimeManager::EventAction_MasterNotice( cltEventTime* pclEventTime )
{
	NTCHARString128 msgtext;
	if(pclEventTime->m_siTextMgr > 0)
	{
		msgtext = GetTxtFromMgr(pclEventTime->m_siTextMgr); 

		TCHAR* pText = GetTxtFromMgr(5482); 

		// 운영자가 하는것이라면 채팅창에도 나타나게한다.
		//KHY - 0129 - 하단 채팅창에 공지 출력여부를 결정한다.
		cltGameMsgResponse_Notice clNotice(pText, msgtext, true);
		cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

		pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);			
	}

	return true;
}

bool cltEventTimeManager::EventAction_MonsterAppearNotice( SI16 siType, cltEventTime* pclEventTime )
{
	cltServer*	pclserver = (cltServer*)pclClient;
	
	if( 0 >= pclEventTime->m_siCharKind )	return false;

	NTCHARString128 msgtext;
	
	switch( siType )
	{
		case EXEC_SHOT_MONSTER_APPEAR_IN_VILLAGE:
			{

			}
			break;
		case EXEC_SHOT_MONSTER_APPEAR_OUT_VILLAGE:
			{

			}
			break;
		case EXEC_SHOT_MONSTER_APPEAR_RANDOM_VILLAGE:
			{
				if(pclEventTime->m_siTextMgr > 0)
				{
					for( SI32 Count = 0; Count < pclEventTime->m_siCharNum; ++Count )
					{
						if( 0 >= siMonsterID[Count] )	return false;

						cltCharServer* pclChar = pclClient->pclCM->GetCharServer( siMonsterID[Count] );
						if(NULL == pclChar) continue;

						msgtext = GetTxtFromMgr( pclEventTime->m_siTextMgr ); 
						msgtext.Replace( TEXT("#monstername#"), pclChar->GetName() );
						msgtext.Replace( TEXT("#posx#"),		SI32ToString( pclChar->GetX() ) );
						msgtext.Replace( TEXT("#posy#"),		SI32ToString( pclChar->GetY() ) );

						cltGameMsgResponse_Notice clNotice( GetTxtFromMgr(5482), msgtext, true );
						cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice );

						pclClient->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
					}
				}
			}
			break;
		case EXEC_SHOT_MONSTER_APPEAR_RANDOM_HUNTMAP:
			{

			}
			break;
		case EXEC_SHOT_GUILDWAR:// [성웅] 길드 사냥터 소유 길드전 매니저.
			{

			}break;
	}

	return true;
}

bool cltEventTimeManager::EventAction_MonsterAppear( SI16 siType, cltEventTime* pclEventTime )
{
	cltServer*	pclserver = (cltServer*)pclClient;

	if( 0 >= pclEventTime->m_siCharKind )	return false;

#ifdef _SAFE_MEMORY
	siMonsterID.ZeroMem();
#else
	ZeroMemory(siMonsterID, sizeof(siMonsterID));
#endif
	SI32	siMonsterCount = 0;

	switch( siType )
	{
		case EXEC_SHOT_MONSTER_APPEAR_IN_VILLAGE:
		case EXEC_SHOT_MONSTER_APPEAR_OUT_VILLAGE:
			{
				cltInnerStatusInfo	clinnerstatus;
				clinnerstatus.clEnemyType.Set( ENEMYTYPE_ALLPC, 0 );	// 모든 PC와 우호적, 사람과는 적대적. 

				SI32	siX		= 0;	// 몬스터가 나오는 X 좌표
				SI32	siY		= 0;	// 몬스터가 나오는 Y 좌표
				SI32	siID	= 0;	// 생성되는 몬스터의 아이디
				
				// 생성된 모든 마을안에 몬스터를 소환한다
				for( UI16 i = 0; i < MAX_VILLAGE_NUMBER; i++ )
				{
					// 설정된 생성 수많큼 생성.
					for( UI16 j = 0; j < pclEventTime->m_siCharNum; j++ )
					{
						// 마을이 없다면 바로 컨티뉴
						if( pclserver->pclVillageManager->pclVillageInfo[i] == NULL ) continue;

						siID = 0;

						if( EXEC_SHOT_MONSTER_APPEAR_IN_VILLAGE == siType )
						{
							siID = pclserver->SetNPCChar(	pclEventTime->m_siCharKind, 
															i, 
															NULL, 
															( 0 < pclEventTime->m_siCharLevel ) ? pclEventTime->m_siCharLevel : (rand()%100) + 1, 
															pclEventTime->m_siCharLife, 
															pclEventTime->m_siPosX, 
															pclEventTime->m_siPosY, 
															i, 
															NPCITEMMODE_NORMAL, 
															&clinnerstatus );
						}
						else if( EXEC_SHOT_MONSTER_APPEAR_OUT_VILLAGE == siType )
						{
							siID = pclserver->SetNPCChar(	pclEventTime->m_siCharKind, 
															VILLAGE_NONE, 
															NULL, 
															( 0 < pclEventTime->m_siCharLevel ) ? pclEventTime->m_siCharLevel : (rand()%100) + 1, 
															pclEventTime->m_siCharLife, 
															pclEventTime->m_siPosX, 
															pclEventTime->m_siPosY, 
															0, 
															NPCITEMMODE_NORMAL, 
															&clinnerstatus );
						}
					
						// 시간 제한이 있는 몬스터 설정			
						if( pclserver->pclCM->IsValidID( siID ) )
						{
							siMonsterID[siMonsterCount++] = siID;
							((cltCharServer*)pclserver->pclCM->CR[siID])->dwTimerClock	= pclserver->CurrentClock;
						}
					}
				}
			}
			break;

		case EXEC_SHOT_MONSTER_APPEAR_RANDOM_VILLAGE:
			{
				cltInnerStatusInfo	clinnerstatus;
				clinnerstatus.clEnemyType.Set( ENEMYTYPE_ALLPC, 0 );	// 모든 PC와 우호적, 사람과는 적대적. 

				SI32	siX		= 0;	// 몬스터가 나오는 X 좌표
				SI32	siY		= 0;	// 몬스터가 나오는 Y 좌표
				SI32	siID	= 0;	// 생성되는 몬스터의 아이디

				bool siAppearVillage[MAX_VILLAGE_NUMBER];
				ZeroMemory( siAppearVillage, sizeof(siAppearVillage) );

// 일단은 모든 마을 주변에 다 나오게 한다. 낭중에 삭제하자.ㅋㅋ
#ifdef _DEBUG
//				memset(siAppearVillage, 1, sizeof(siAppearVillage));
#endif
				UI16 i;
				for( i = 1; i <= pclEventTime->m_siCharNum;  ++i)
				{
					if( MAX_VILLAGE_NUMBER <= i ) break;

					siAppearVillage[i] = true;	
				}

				// 캐릭터 출현마을을 렌덤하게 설정한다.(3번 섞는다.)
				SI32 siChangeVillageIndex = 0;
				for(SI32 Count = 0; Count < 3; ++Count)
				{
					for( i = 0; i < MAX_VILLAGE_NUMBER;  ++i)
					{
						siChangeVillageIndex = rand() % MAX_VILLAGE_NUMBER;

						if( pclserver->pclVillageManager->pclVillageInfo[i] == NULL )						continue;	
						if( pclserver->pclVillageManager->pclVillageInfo[siChangeVillageIndex] == NULL )	continue; 
						if( i == siChangeVillageIndex )														continue; 

						siAppearVillage[i] ^= siAppearVillage[siChangeVillageIndex] ^=	siAppearVillage[i] ^= siAppearVillage[siChangeVillageIndex];																
					}
				}
				
				UI16 CreateNpcNum;
				// 생성된 모든 마을안에 몬스터를 소환한다
				for( UI16 i = 0; i < MAX_VILLAGE_NUMBER; i++ )
				{
					CreateNpcNum = 1;
					for( UI16 CreateNpc = 0; CreateNpc < CreateNpcNum; CreateNpc++ )
					{
						// 마을이 없다면 바로 컨티뉴
						if( pclserver->pclVillageManager->pclVillageInfo[i] == NULL )	continue;	
						if( false == siAppearVillage[i] )								continue;						

						siID = 0;

						siX = pclClient->pclVillageManager->pclVillageInfo[i]->PosX + 3;
						siY = pclClient->pclVillageManager->pclVillageInfo[i]->PosY + 3;

						siID = pclserver->SetNPCChar(	pclEventTime->m_siCharKind, 
														VILLAGE_NONE, 
														NULL, 
														( 0 < pclEventTime->m_siCharLevel ) ? pclEventTime->m_siCharLevel : (rand()%100) + 1, 
														pclEventTime->m_siCharLife, 
														siX, 
														siY, 
														0, 
														NPCITEMMODE_NORMAL, 
														&clinnerstatus );

						// 시간 제한이 있는 몬스터 설정			
						if( pclserver->pclCM->IsValidID( siID ) )
						{
							siMonsterID[siMonsterCount++] = siID;

							((cltCharServer*)pclserver->pclCM->CR[siID])->dwTimerClock	= pclserver->CurrentClock;

							CreateNpcNum = 1;
						}
						else
						{
							CreateNpcNum = 4;	// 생성에 실패하면 다시 시도.
						}
					}
				}
			}
			break;
		case EXEC_SHOT_MONSTER_APPEAR_RANDOM_HUNTMAP:
			{
				cltServer* pclserver = (cltServer*)pclClient;
				cltGate clGate					;
				SI32	Kind			=	0	;	// 소환할 몬스터 카인드
				SI32	siX				=	0	;	// 소환할 X 좌표 
				SI32	siY				=	0	;	// 소환할 Y 좌표
				SI32	siMapIndex		=	0	;	// 소환할 맵 인덱스
				SI32	siTempMapIndex	=	0	;	// 소환할 맵 인덱스
				SI32	siIndex			=	0	;
				SI32	X				=	0	;	// 소환할 최종 X 좌표 
				SI32	Y				=	0	;	// 소환할 최종 Y 좌표
				SI16	siEventmapIndex	=	0	;	// 이벤트 맵 인덱스의 최종 갯수

				Kind		=	pclEventTime->m_siCharKind					;	// 소환할 몬스터 카인드를 가지고 온다.
				 siEventmapIndex = GetEventMapIndex();
				while ( siMapIndex == 0 ) 
				{
					siTempMapIndex = rand()%siEventmapIndex				;
					siMapIndex	=	m_HuntmapIndexByResource[ siTempMapIndex ]	;
					break	;
					
				}
				while( pclserver->pclMapManager->pclGateManager->FindGate(siIndex, &clGate) == true )
				{
					siIndex++	;
					if( clGate.DstMapIndex == siMapIndex )
					{
						siX =	clGate.DstX + rand()%10	 ;
	
						// 랜덤을 위해서 
						switch( rand()%5) 
						{
						case 1:
							{
								siY	=	clGate.DstY + rand()%5	 ;
							}
							break;
						case 2:
							{
								siY	=	clGate.DstY + rand()%30	 ;
							}
							break;
						case 3:
							{
								siY	=	clGate.DstY - rand()%10	 ;
							}
							break;
						case 4:
							{
								siY	=	clGate.DstY + rand()%15	 ;
							}
							break;
						case 5 :
							{
								siY	=	clGate.DstY - rand()% 30	 ;
							}
							break;
						}
						break;
					}
					
				}
				cltMapCommon* pclmap	= pclserver->pclMapManager->GetMapPointer( siMapIndex );
				//// 만약 빈 케릭터 로케이션이 아니면 그냥 케이트 입구로 
				if(pclmap->FindEmptyAreaForCharLocate( Kind , siX, siY, 100, &X, &Y, false ) == false)
				{
					// 만약 못찾으면 
					pclmap->FindEmptyAreaForCharLocate( Kind , clGate.DstX, clGate.DstY, 10, &X, &Y, false );
					break;
				}
				//// 만약 빈 케릭터 로케이션이 아니면 그냥 케이트 입구로 
				//if(pclmap->FindEmptyAreaForCharLocate( Kind , siX, siY, 150, &X, &Y, false ) == false)
				//{
				//	siX = clGate.DstX	;
				//	siY = clGate.DstY	;
				//	pclmap->FindEmptyAreaForCharLocate( Kind , clGate.DstX, clGate.DstY, 150, &X, &Y, false );
				//}
				TCHAR * mapname = pclserver->pclMapManager->GetMapName(siMapIndex);
				SI32 id = pclserver->SetNPCChar(Kind, VILLAGE_NONE, NULL, 1, 500, X, Y, siMapIndex, NPCITEMMODE_NORMAL, NULL );

				// 시간 제한이 있는 몬스터 설정			
				if( pclserver->pclCM->IsValidID( id ) )
				{
					((cltCharServer*)pclserver->pclCM->CR[id])->dwTimerClock	= pclserver->CurrentClock;
				}
				cltGameMsgResponse_Notice_Quset clNotice( Kind ,siMapIndex,HALLOWEENQUEST,X,Y );
				cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_QUEST, sizeof( clNotice ), (BYTE*)&clNotice );
				pclClient->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );

			}
			break	;
		case EXEC_SHOT_GUILDWAR:// [성웅] 길드 사냥터 소유 길드전 매니저.
			{

			}break;
	}
	
	return true;
}

// [지연] 군주데이미션 : 미션 부여
void cltEventTimeManager::EventAction_GoonzuDay_GiveMission( cltEventTime* pEventTime )
{
	if ( NULL == pEventTime ) return;
	cltServer* pServer = (cltServer*)pclClient;

	//if ( pEventTime->m_dwExtraQuestStartTime <= pServer->CurrentClock )
	{
		// 퀘스트 발동 : 접속중인 모든 유저에게 부여 (접속중이 아닌 유저에 대해선 로그인 할때 퀘스트 부여)
		SI32 id = 0;
		SI32 index = 0;

		while( id = pServer->pclCM->GetCharOrder(index) )
		{
			index++;

			cltCharServer* pclchar = (cltCharServer*)pServer->pclCM->CR[id];
			if ( NULL == pclchar ) continue;

			GiveMission( pclchar );
		}
	}
}

void cltEventTimeManager::GiveMission(cltCharServer* pChar)
{
	if ( NULL == pChar ) return;
	cltServer* pServer = (cltServer*)pclClient;

	if( pChar->GetSession() )
	{
		if ( true == pChar->Make_GoonzuDayMission() )
		{
			// 군주데이미션을 정상적으로 부여 했다면 디비에 해당 정보를 저장한다 (군주데이 미션 부여 여부 저장)
			SYSTEMTIME stCurrentTime;
			GetLocalTime( &stCurrentTime );

			// 디비에 군주데이미션 부여 정보를 저장
			sDBRequest_GoonzuDayMissionInfo_Set clSndInfo( pChar->GetID(), pChar->pclCI->GetPersonID(), stCurrentTime.wYear, stCurrentTime.wMonth, stCurrentTime.wDay, 1 );
			pServer->SendDBMsg( DBSELECT_SYSTEM, (sPacketHeader*)&clSndInfo);					
		}
	}
}