#include "WeddingManager.h"

#include "../Server/Server.h"
#include "../CommonLogic/CommonLogic.h"
#include "../CommonLogic/MsgRval-Define.h"

#include "../DBManager/GameDBManager_World/DBMsg-Wedding.h"

extern cltCommonLogic* pclClient;

cltWeddingReserveList::cltWeddingReserveList()
{
	Init();
}

cltWeddingReserveList::~cltWeddingReserveList()
{
}

void cltWeddingReserveList::Init()
{
#ifdef _SAFE_MEMORY
	m_clReserveList.ZeroMem();
#else
	ZeroMemory( m_clReserveList, (sizeof(cltWeddingReserveUnit) * WEDDING_RESERVE_LIST_NUM) );
#endif

	m_siRequestHour = 0;

}

BOOL cltWeddingReserveList::IsRequestHour( void )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return FALSE;
	}

	if ( (55 < pclserver->sTime.wMinute) && (m_siRequestHour == pclserver->sTime.wHour) )
	{
		return TRUE;
	}

	return FALSE;
}

void cltWeddingReserveList::SetReserveListTime( SI16 siAddHour/* =0  */)
{
	SYSTEMTIME stCurrentTime;
	GetLocalTime( &stCurrentTime );
	
	SYSTEMTIME stListTime;
	ZeroMemory( &stListTime, sizeof(stListTime) );

	stListTime.wYear	= stCurrentTime.wYear;
	stListTime.wMonth	= stCurrentTime.wMonth;
	stListTime.wDay		= stCurrentTime.wDay;
	stListTime.wHour	= stCurrentTime.wHour;

	// 리스트가 갱신될 시간을 설정
	m_siRequestHour		= stCurrentTime.wHour;

	// 이 값이 0보다 크면 한시간 후로 설정을 바꾼다
	if ( 0 < siAddHour )
	{
		// 1시간을 더한 시간을 리스트로 만든다
		TimeAddHour( &stListTime, 1, &stListTime );

		// 요청시간도 한시간 후로
		m_siRequestHour = (m_siRequestHour + 1) % 24;
	}

	for ( SI32 siIndex=0; siIndex<WEDDING_RESERVE_LIST_NUM; siIndex++ )
	{
		m_clReserveList[siIndex].Init();

		m_clReserveList[siIndex].m_stReserveTime.wYear	= stListTime.wYear;
		m_clReserveList[siIndex].m_stReserveTime.wMonth	= stListTime.wMonth;
		m_clReserveList[siIndex].m_stReserveTime.wDay	= stListTime.wDay;
		m_clReserveList[siIndex].m_stReserveTime.wHour	= stListTime.wHour;

		TimeAddHour( &stListTime, 1, &stListTime );
	}

}

void cltWeddingReserveList::RequestListProcess( bool bFirst/* =false  */)
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return;
	}

	SI16		siAddHour		= 0;
	SYSTEMTIME	stRequestTime;
	ZeroMemory( &stRequestTime, sizeof(stRequestTime) );

	stRequestTime.wYear		= pclserver->sTime.wYear;
	stRequestTime.wMonth	= pclserver->sTime.wMonth;
	stRequestTime.wDay		= pclserver->sTime.wDay;
	stRequestTime.wHour		= pclserver->sTime.wHour;

	// 처음 요청이 아니면 무조건 1시간 후의 정보를 얻어온다
	if ( false == bFirst )
	{
		 siAddHour = 1;
		 TimeAddHour( &stRequestTime, 1, &stRequestTime );
	}

	// 리스트에 시간을 설정해주고
	SetReserveListTime( siAddHour );

	// 예약정보를 DB에서 요청한다
	sDBRequest_WeddingReserveListGet clMsg( stRequestTime );
	pclserver->SendDBMsg(DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
	
}

SI32 cltWeddingReserveList::GetReserveListIndex( SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour )
{
	if ( 1000 > siYear )					return -1;
	if ( (0 > siMonth)	|| (12 < siMonth) )	return -1;
	if ( (0 > siDay)	|| (31 < siDay) )	return -1;
	if ( (0 > siHour)	|| (23 < siHour) )	return -1;

	for ( SI32 siIndex=0; siIndex<WEDDING_RESERVE_LIST_NUM; siIndex++ )
	{
		if ( m_clReserveList[siIndex].IsSameTime(siYear, siMonth, siDay, siHour) )
		{
			return siIndex;
		}
	}

	return -1;
}

SI32 cltWeddingReserveList::CanReserve( SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, SI32 siPersonID )
{
	if ( 1000 > siYear )					return WEDDING_RESERVE_RESULT_INVALID;
	if ( (0 > siMonth)	|| (12 < siMonth) )	return WEDDING_RESERVE_RESULT_INVALID;
	if ( (0 > siDay)	|| (31 < siDay) )	return WEDDING_RESERVE_RESULT_INVALID;
	if ( (0 > siHour)	|| (23 < siHour) )	return WEDDING_RESERVE_RESULT_INVALID;

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return WEDDING_RESERVE_RESULT_INVALID;
	}

	SYSTEMTIME stWantTime;
	ZeroMemory( &stWantTime, sizeof(stWantTime) );

	stWantTime.wYear	= siYear;
	stWantTime.wMonth	= siMonth;
	stWantTime.wDay		= siDay;
	stWantTime.wHour	= siHour;

	// 예약하려는 시간이 현재 시간보다 빠르면 안됨
	if ( 0 < TimeCompare(&pclserver->sTime, &stWantTime) )
	{
		return WEDDING_RESERVE_RESULT_INVALID;
	}

	SI32 siIndex = 0;

	// 해당 시간에 다른 예약자가 있는지 검사한다
	for ( siIndex=0; siIndex<WEDDING_RESERVE_LIST_NUM; siIndex++ )
	{
		if ( TRUE == m_clReserveList[siIndex].IsSameTime(siYear, siMonth, siDay, siHour) )
		{
			if ( (0 < m_clReserveList[siIndex].m_siMate1_PersonID) || (0 < m_clReserveList[siIndex].m_siMate2_PersonID) )
			{
				return WEDDING_RESERVE_RESULT_ALREADY_OTHER;
			}
		}
	}

	// 혹시 다른 시간에 동일 캐릭터가 예약했는지 검사한다
	for ( SI32 siIndex=0; siIndex<WEDDING_RESERVE_LIST_NUM; siIndex++ )
	{
		if ( (siPersonID == m_clReserveList[siIndex].m_siMate1_PersonID) || (siPersonID == m_clReserveList[siIndex].m_siMate2_PersonID) )
		{
			return WEDDING_RESERVE_RESULT_ALREADY_MY;
		}
	}

	return WEDDING_RESERVE_RESULT_SUCCESS;
}

void cltWeddingReserveList::SetReserve( SI32 siListIndex, cltWeddingReserveUnit* pclReserveUnit )
{
	if ( (0 > siListIndex) || (WEDDING_RESERVE_LIST_NUM <= siListIndex) )
	{
		return;
	}

	if ( NULL == pclReserveUnit )
	{
		return;
	}

	m_clReserveList[siListIndex].Set( pclReserveUnit );

}

cltWeddingReserveUnit* cltWeddingReserveList::GetReserveUnitPtr( SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour )
{
	SI32 siGetIndex = GetReserveListIndex( siYear, siMonth, siDay, siHour );
	if ( 0 > siGetIndex )
	{
		return NULL;
	}

	return &m_clReserveList[siGetIndex];
}

cltWeddingReserveUnit* cltWeddingReserveList::GetReserveUnitPtr( SI32 siListIndex )
{
	if ( (0 > siListIndex) || (WEDDING_RESERVE_LIST_NUM <= siListIndex) )
	{
		return NULL;
	}

	return &m_clReserveList[siListIndex];
}

cltWeddingReserveUnit* cltWeddingReserveList::GetReserveUnitPtrFromNowTime( void )
{
	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return NULL;
	}

	SI16 siYear		= pclserver->sTime.wYear;
	SI16 siMonth	= pclserver->sTime.wMonth;
	SI16 siDay		= pclserver->sTime.wDay;
	SI16 siHour		= pclserver->sTime.wHour;

	return GetReserveUnitPtr( siYear, siMonth, siDay, siHour );
}


BOOL cltWeddingReserveList::InitOneUnitExecptTime( SI32 siReserveNum )
{
	if ( 0 >= siReserveNum )
	{
		return FALSE;
	}

	for ( SI32 siIndex=0; siIndex<WEDDING_RESERVE_LIST_NUM; siIndex++ )
	{
		if ( ( siReserveNum == m_clReserveList[siIndex].m_siReserveNum ) )
		{
			m_clReserveList[siIndex].InitExecptTime();

			return TRUE;
		}
	}

	return FALSE;

}

