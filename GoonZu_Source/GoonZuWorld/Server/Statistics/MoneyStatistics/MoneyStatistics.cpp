//---------------------------------
// 2008/05/22 이강형
//---------------------------------

#include "MoneyStatistics.h"

void cltMoneyStatistics::Action()
{
	if(m_kUpdateTimer_Action.IsTimed(::GetTickCount()) == false)		return;

	// 이전 데이터값 추가
	ReadStatistics();

	// 데이터 값 기록
	WriteStatistics();

	// 데이터 초기화
	if(m_Row)		ZeroMemory(m_Row, sizeof(SI64) * m_siColumnCount);
}


bool cltMoneyStatistics::CreateFile()
{
	NTCHARString<64>	FileName;
	FileName = "Config\\";
	FileName += m_kFileName;
	FileName += ".log";

	FILE*	fp = fopen(FileName, "w");

	if( fp == NULL )			return false;

	for(SI32 i=0; i<m_siColumnCount; i++)
	{
		fprintf(fp, "%s\t", (const TCHAR*)m_pColumnNames[i]);
	}
	fprintf(fp, "\n" );

	fclose(fp);

	return true;
}

bool cltMoneyStatistics::ChangeExistFileName()
{
	NTCHARString<64>	SrcFileName;
	SrcFileName = "Config\\";
	SrcFileName += m_kFileName;
	SrcFileName += ".log";

	SYSTEMTIME	stLocal;	GetLocalTime(&stLocal);

	TCHAR	szDateTime[32] = { 0, };
	StringCchPrintf(szDateTime, sizeof(szDateTime), "%04d%02d%02d_%02d%02d%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
	szDateTime[sizeof(szDateTime)-1] = '\0';

	NTCHARString<64>	DstFileName;
	DstFileName = "Config\\";
	DstFileName += m_kFileName;
	DstFileName += szDateTime;
	DstFileName += ".log";

	::MoveFile(SrcFileName, DstFileName);

	return true;
}


int cltMoneyStatistics::CheckColumn()
{
	NTCHARString<64>	FileName;
	FileName = "Config\\";
	FileName += m_kFileName;
	FileName += ".log";

	FILE*	fp = fopen(FileName, "r");
	if( fp == NULL )			return -1;

	TCHAR	szLine[1024];	
	if( fgets(szLine, sizeof(szLine), fp) == NULL)		return 0;

	SI32	siColumnCount = 0;

	TCHAR*	pTok = _tcstok(szLine, "\t");
	while(pTok)
	{
		siColumnCount++;
		pTok = _tcstok(NULL, "\t");
	}

	fclose(fp);

	if( siColumnCount - 1 != m_siColumnCount )		return 0;

	return 1;
}

bool cltMoneyStatistics::WriteStatistics()
{
	NTCHARString<64>	FileName;
	FileName = "Config\\";
	FileName += m_kFileName;
	FileName += ".log";

	FILE* fp = fopen(FileName, "r+");
	if( fp == NULL )			return false;

	// 파일 크기
	fseek(fp, 0, SEEK_END);
	SI32	siFileSize = ftell(fp);

	// 다시 처음으로
	fseek(fp, 0, SEEK_SET);

	SI32	siFilePos = 0;
	SI32	siFilePrev = 0;

	siFilePrev = siFilePos;	
	siFilePos = ftell(fp);

	// 첫행은 컬럼명이다.
	TCHAR	szLine[1024];	
	if( fgets(szLine, sizeof(szLine), fp) == NULL)		return false;

	bool bUnlimit = true;
	while(bUnlimit)
	{
		siFilePos = ftell(fp);
		if( fgets(szLine, sizeof(szLine), fp) == NULL )		break;

		siFilePrev = siFilePos;
	}

	SYSTEMTIME	stLocal;	GetLocalTime(&stLocal);
	TCHAR	szDateTime[16] = { 0, };
	StringCchPrintf(szDateTime, sizeof(szDateTime), "%04d-%02d-%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay );

	fseek(fp, siFilePrev, SEEK_SET);
	fgets(szLine, sizeof(szLine), fp);

	// 첫 컬럼을 읽고 날짜를 비교한다.
	TCHAR* pTok = _tcstok(szLine, "\t");
	if( pTok != NULL && _tcsncmp(pTok, szDateTime, sizeof(szDateTime)) == 0 )
	{
		siFilePos = siFilePrev;
	}
	else
	{
		siFilePos = siFileSize;
	}

	fseek(fp, siFilePos, SEEK_SET);

	fprintf(fp, "%s\t", szDateTime);
	for(SI32 i=1; i<m_siColumnCount; i++)
	{
		fprintf(fp, "%I64d\t", m_Row[i]);
	}
	fprintf(fp, "\n" );

	fclose(fp);

	return true;
}

bool cltMoneyStatistics::ReadStatistics()
{
	NTCHARString<64>	FileName;
	FileName = "Config\\";
	FileName += m_kFileName;
	FileName += ".log";

	FILE* fp = fopen(FileName, "r");
	if( fp == NULL )			return false;

	// 파일 크기
	fseek(fp, 0, SEEK_END);
	SI32	siFileSize = ftell(fp);

	// 다시 처음으로
	fseek(fp, 0, SEEK_SET);

	SI32	siFilePos = 0;
	SI32	siFilePrev = 0;

	siFilePrev = siFilePos;	
	siFilePos = ftell(fp);

	// 첫행은 컬럼명이다.
	TCHAR	szLine[1024];	
	if( fgets(szLine, sizeof(szLine), fp) == NULL)		return false;

	bool bUnlimit = true;
	while(bUnlimit)
	{
		siFilePos = ftell(fp);
		if( fgets(szLine, sizeof(szLine), fp) == NULL )		break;

		siFilePrev = siFilePos;
	}

	SYSTEMTIME	stLocal;	GetLocalTime(&stLocal);
	TCHAR	szDateTime[16] = { 0, };
	StringCchPrintf(szDateTime, sizeof(szDateTime), "%04d-%02d-%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay );

	fseek(fp, siFilePrev, SEEK_SET);
	fgets(szLine, sizeof(szLine), fp);

	// 첫 컬럼을 읽고 날짜를 비교한다.
	TCHAR* pTok = _tcstok(szLine, "\t");
	SI32 i=1;
	if( pTok != NULL && _tcsncmp(pTok, szDateTime, sizeof(szDateTime)) == 0 )
	{
		while( (pTok = _tcstok(NULL, "\t")) != NULL )
		{
			if( i < m_siColumnCount )
			{
				m_Row[i] += _tstoi64(pTok);
				i++;
			}
		}
	}

	fclose(fp);

	return true;
}

void cltAddMoneyStatistics::Init(SI32 siColumnCount)
{
	if( siColumnCount <= 0 )			return;

	m_pColumnNames = new NTCHARString<32>[siColumnCount];
	m_Row = new SI64[siColumnCount];

	if(m_pColumnNames == NULL)			return;
	if(m_Row == NULL)					return;

	ZeroMemory(m_Row, sizeof(SI64) * siColumnCount);

	m_siColumnCount = siColumnCount;

#ifdef _DEBUG
	m_kUpdateTimer_Action.Init(60 * 1000);
#else
	m_kUpdateTimer_Action.Init(10 * 60 * 1000);
#endif
	m_kUpdateTimer_Action.SetActive(true, ::GetTickCount());

	//===========================================================
	m_kFileName = "AddMoney";
	//===========================================================

	//===========================================================
	// 실제 이름 설정 - 컬럼이 추가되면, 이쪽에 추가한다.

	m_pColumnNames[ADDMONEY_STATISTIC_DATE] = "날짜";
	m_pColumnNames[ADDMONEY_STATISTIC_DAILYQUEST] = "데일리퀘스트";
	m_pColumnNames[ADDMONEY_STATISTIC_LEVEL20] = "20레벨지원금";
	m_pColumnNames[ADDMONEY_STATISTIC_ANKALANSTOCK30] = "앙카란주식배당";
	m_pColumnNames[ADDMONEY_STATISTIC_SKILLMONEY] = "기술지원금";
	m_pColumnNames[ADDMONEY_STATISTIC_EVENT] = "이벤트";
	m_pColumnNames[ADDMONEY_STATISTIC_FATHER] = "스승제자";	
	m_pColumnNames[ADDMONEY_STATISTIC_SPECIALQUEST] = "흥부박퀘스트";
	m_pColumnNames[ADDMONEY_STATISTIC_QUEST] = "퀘스트";
	m_pColumnNames[ADDMONEY_STATISTIC_SELLNPC] = "아이템판매";
	m_pColumnNames[ADDMONEY_STATISTIC_SYSTEMREWARD] = "보상NPC";
	m_pColumnNames[ADDMONEY_STATISTIC_MONTHMONEY] = "행수월급";
	m_pColumnNames[ADDMONEY_STATISTIC_NPCBUYITEM] = "윌리스판매";

	m_pColumnNames[ADDMONEY_STATISTIC_DIVIDEND] = "배당금지급[계좌]";
	m_pColumnNames[ADDMONEY_STATISTIC_GUILDDUNGEON] = "길드던젼[계좌]";

	//===========================================================

	for( SI32 i=0; i<ADDMONEY_STATISTIC_TOTAL; i++ )
	{
		if( m_pColumnNames[i].IsEmpty() )
		{
#ifdef _DEBUG
			MessageBox(NULL, "통계 컬럼명이 설정되지 않았습니다", "ERROR", MB_OK);
#endif
		}
	}

	if( CheckColumn() == -1 )
	{
		CreateFile();
	}
	else if( CheckColumn() == 0 )
	{
		ChangeExistFileName();
		CreateFile();
	}
}

void cltDelMoneyStatistics::Init(SI32 siColumnCount)
{
	if( siColumnCount <= 0 )			return;

	m_pColumnNames = new NTCHARString<32>[siColumnCount];
	m_Row = new SI64[siColumnCount];

	if(m_pColumnNames == NULL)			return;
	if(m_Row == NULL)					return;

	ZeroMemory(m_Row, sizeof(SI64) * siColumnCount);

	m_siColumnCount = siColumnCount;

#ifdef _DEBUG
	m_kUpdateTimer_Action.Init(60 * 1000);
#else
	m_kUpdateTimer_Action.Init(10 * 60 * 1000);
#endif
	m_kUpdateTimer_Action.SetActive(true, ::GetTickCount());

	//===========================================================
	m_kFileName = "DelMoney";
	//===========================================================

	//===========================================================
	// 실제 이름 설정 - 컬럼이 추가되면, 이쪽에 추가한다.

	m_pColumnNames[DELMONEY_STATISTIC_DATE]			= "날짜";
	m_pColumnNames[DELMONEY_SERVICECHARGE]			= "수수료";			
	m_pColumnNames[DELMONEY_TAX]					= "사용료";					
	m_pColumnNames[DELMONEY_DONATE]					= "기부금";					
	m_pColumnNames[DELMONEY_SAVEHOUSE]				= "클라라회수";				
	m_pColumnNames[DELMONEY_DEATHPANELTY]			= "사망패널티";			
	m_pColumnNames[DELMONEY_MARKETSERVICECHARGE]	= "마켓등록비";	
	m_pColumnNames[DELMONEY_WARP]					= "워프비용";		
	m_pColumnNames[DELMONEY_ENTEREVENT]				= "이벤트비용";		
	m_pColumnNames[DELMONEY_BUYNPC]					= "아이템구입(NPC)";				
	m_pColumnNames[DELMONEY_POSTOFFICE]				= "우체국수수료";			
	m_pColumnNames[DELMONEY_STOCK]					= "주식구입비";					
	m_pColumnNames[DELMONEY_GUILDCREATE]			= "길드개설비[계좌]";			
	m_pColumnNames[DELMONEY_BANK_FEE]				= "사용료[계좌]";				
	m_pColumnNames[DELMONEY_BANK_SERVICECHARGE]		= "수수료[계좌]";	
	m_pColumnNames[DELMONEY_BUFF_ROULETTE]			= "버프룰렛 사용시 지불 비용";
	m_pColumnNames[DELMONEY_MARKETCONDITIONS]		= "시세확인 사용시 지불 비용";
	m_pColumnNames[DELMONEY_CHAR_ROULETTE]			= "TransformRoulette";
	m_pColumnNames[DELMONEY_NEWYEAREVENT_ROULETTE]	= "신년이벤트룰렛 지불 비용";
	m_pColumnNames[DELMONEY_BALANCECURRENCY]		= "통화량 흡수 정책";

	//===========================================================

	for( SI32 i=0; i<DELMONEY_STATISTIC_TOTAL; i++ )
	{
		if( m_pColumnNames[i].IsEmpty() )
		{
#ifdef _DEBUG
			MessageBox(NULL, "통계 컬럼명이 설정되지 않았습니다", "ERROR", MB_OK);
#endif
		}
	}

	if( CheckColumn() == -1 )
	{
		CreateFile();
	}
	else if( CheckColumn() == 0 )
	{
		ChangeExistFileName();
		CreateFile();
	}
}
