#pragma once 

#include <Directives.h>
#include "..\..\Common\CommonHeader.h"

#define WEDDING_RESERVE_LIST_NUM			24		// 예약 리스트 갯수
#define WEDDING_RESERVE_BASIC_COST			100000	// 예약 기준 가격

#define WEDDING_RESERVE_LIST_PAGE_PER_NUM	5		// 한쪽당 보여줄 리스트 갯수

enum WEDDING_RESERVE_RESULT
{
	WEDDING_RESERVE_RESULT_INVALID = 0,		// 일반적인 오류로 안됨

	WEDDING_RESERVE_RESULT_ALREADY_OTHER,	// 다른사람 예약되어 있음
	WEDDING_RESERVE_RESULT_ALREADY_MY,		// 내가 예약되어 있음


	WEDDING_RESERVE_RESULT_SUCCESS
};

enum WEDDING_RESERVE_STATUS
{
	WEDDING_RESERVE_STATUS_NONE	= 0,	// 예약 없음
	WEDDING_RESERVE_STATUS_RESERVE,		// 예약 되있음
	WEDDING_RESERVE_STATUS_PROGRESS,	// 진행중
};

class cltWeddingReserveUnit
{
public:
	SI32		m_siReserveNum;		// 예약 번호

	SI32		m_siMate1_PersonID;	// 예약자1
	TCHAR		m_szMate1_Name[MAX_PLAYER_NAME];

	SI32		m_siMate2_PersonID;	// 예약자2
	TCHAR		m_szMate2_Name[MAX_PLAYER_NAME];

	SYSTEMTIME	m_stReserveTime;	// 예약 시간

public:
	cltWeddingReserveUnit( void )
	{
		ZeroMemory( this, sizeof(cltWeddingReserveUnit) );
	}

	~cltWeddingReserveUnit( void )
	{
	}

	void	Init( void )
	{
		ZeroMemory( this, sizeof(cltWeddingReserveUnit) );
	}

	void	Set( cltWeddingReserveUnit* pclUnit )
	{
		if ( NULL == pclUnit )
		{
			return;
		}

		memcpy( this, pclUnit, sizeof(cltWeddingReserveUnit) );
	}

	void	Set( SI32 siReserveNum, SI32 siMate1_PersonID, TCHAR* pszMate1_Name, SI32 siMate2_PersonID, TCHAR* pszMate2_Name,  SYSTEMTIME& stReserveTime )
	{
		m_siReserveNum		= siReserveNum;

		m_siMate1_PersonID	= siMate1_PersonID;
		if ( pszMate1_Name )	StringCchCopy( m_szMate1_Name, sizeof(m_szMate1_Name), pszMate1_Name );

		m_siMate2_PersonID	= siMate2_PersonID;
		if ( pszMate2_Name )	StringCchCopy( m_szMate2_Name, sizeof(m_szMate2_Name), pszMate2_Name );

		memcpy( &m_stReserveTime, &stReserveTime, sizeof(m_stReserveTime) );
	}

	void	Set( SI32 siReserveNum, SI32 siMate1_PersonID, TCHAR* pszMate1_Name, SI32 siMate2_PersonID, TCHAR* pszMate2_Name, SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour )
	{
		m_siReserveNum		= siReserveNum;

		m_siMate1_PersonID	= siMate1_PersonID;
		if ( pszMate1_Name )	StringCchCopy( m_szMate1_Name, sizeof(m_szMate1_Name), pszMate1_Name );

		m_siMate2_PersonID	= siMate2_PersonID;
		if ( pszMate2_Name )	StringCchCopy( m_szMate2_Name, sizeof(m_szMate2_Name), pszMate2_Name );


		ZeroMemory( &m_stReserveTime, sizeof(m_stReserveTime) );

		m_stReserveTime.wYear	= siYear;
		m_stReserveTime.wMonth	= siMonth;
		m_stReserveTime.wDay	= siDay;
		m_stReserveTime.wHour	= siHour;
	}

//Utility Function
	BOOL	IsSameTime( SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour )
	{
		if ( m_stReserveTime.wYear != siYear )		return FALSE;
		if ( m_stReserveTime.wMonth != siMonth )	return FALSE;
		if ( m_stReserveTime.wDay != siDay )		return FALSE;
		if ( m_stReserveTime.wHour != siHour )		return FALSE;

		return TRUE;
	}

	BOOL	IsReserved( void )
	{
		if ( 0 < m_siMate1_PersonID )	return TRUE;
		if ( 0 < m_siMate2_PersonID )	return TRUE;

		return FALSE;
	}

	void	InitExecptTime( void )
	{
		SI16 siYear		= m_stReserveTime.wYear;
		SI16 siMonth	= m_stReserveTime.wMonth;
		SI16 siDay		= m_stReserveTime.wDay;
		SI16 siHour		= m_stReserveTime.wHour;

		Init();

		m_stReserveTime.wYear	= siYear;
		m_stReserveTime.wMonth	= siMonth;
		m_stReserveTime.wDay	= siDay;
		m_stReserveTime.wHour	= siHour;
	}
};

// 가장 간단한 예식 목록을 나타낸다(시간 예약 여부)
class cltWeddingReserveSimpleUnit
{
public:
	SYSTEMTIME	m_stReserveTime;

	SI16		m_siReserveStatus;

public:
	cltWeddingReserveSimpleUnit( void )
	{
		ZeroMemory( this, sizeof(cltWeddingReserveSimpleUnit) );
	}

	~cltWeddingReserveSimpleUnit( void )
	{

	}

	void Init( void )
	{
		ZeroMemory( this, sizeof(cltWeddingReserveSimpleUnit) );
	}

	void Set( cltWeddingReserveSimpleUnit* pclSimpleUnit )
	{
		if ( NULL == pclSimpleUnit )
		{
			return;
		}

		memcpy( this, pclSimpleUnit, sizeof(cltWeddingReserveSimpleUnit) );
	}

	void Set( SYSTEMTIME& rstReserveTime, SI16 siReserveStatus )
	{
		memcpy( &m_stReserveTime, &rstReserveTime, sizeof(m_stReserveTime) );

		m_siReserveStatus = siReserveStatus;
	}

};

typedef NSafeTArray<cltWeddingReserveUnit, WEDDING_RESERVE_LIST_NUM> NSafty_WeddingReserveList;

class cltWeddingReserveList
{
private:
#ifdef _SAFE_MEMORY
	NSafty_WeddingReserveList	m_clReserveList;
#else
	cltWeddingReserveUnit		m_clReserveList[WEDDING_RESERVE_LIST_NUM];
#endif

	SI16						m_siRequestHour;

public:
	cltWeddingReserveList();
	~cltWeddingReserveList();

	void Init( void );

	// 현재 시간으로 예약 리스트를 셋팅한다
	void SetReserveListTime( SI16 siAddHour=0 );

	// 요청될 시간이 되었는가
	BOOL IsRequestHour( void );

	// 리스트 요청 프로세스
	void RequestListProcess( bool bFirst=false );

	// 날짜를 기본으로 리스트 인덱스를 가져온다
	SI32 GetReserveListIndex( SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour );

	// 예약할 수 있는가?
	SI32 CanReserve( SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, SI32 siPersonID );

	// 예약 한다
	BOOL Reserve( SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, SI32 siPersonID );

	// DB에서 받아온 예약 정보를 셋팅한다
	void SetReserve( SI32 siListIndex, cltWeddingReserveUnit* pclReserveUnit );

	// 단위 예약 클레크 포인터를 가져온다
	cltWeddingReserveUnit* GetReserveUnitPtr( SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour );

	// 단위 예약 클레크 포인터를 가져온다
	cltWeddingReserveUnit* GetReserveUnitPtr( SI32 siListIndex );

	// 단위 예약 클레크 포인터를 가져온다
	cltWeddingReserveUnit* GetReserveUnitPtrFromNowTime( void );

	// 예약 번호에 해당하는 유닛 하나의 예약정보를 시간을 제외하고 지운다
	BOOL InitOneUnitExecptTime( SI32 siReserveNum );


};