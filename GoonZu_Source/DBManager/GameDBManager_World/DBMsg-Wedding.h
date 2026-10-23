#ifndef _DBMSG_WEDDING_H_
#define _DBMSG_WEDDING_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "../../../GoonZuWorld/CommonLogic/Marriage/WeddingManager.h"


// [영훈] 결혼시스템 : 예약 리스트 받기
class sDBRequest_WeddingReserveListGet
{
public:
	sPacketHeader	m_packetHeader;

	SYSTEMTIME		m_stCurrentTime;	// 이 간부터 24시간 이후까지의 예약 정보를 받아온다

public:
	sDBRequest_WeddingReserveListGet( SYSTEMTIME& stCurrentTime )
	{
		m_packetHeader.usCmd	= DBMSG_REQUEST_WEDDING_RESERVE_LIST_GET;
		m_packetHeader.usSize	= sizeof(sDBRequest_WeddingReserveListGet);

		memcpy( &m_stCurrentTime, &stCurrentTime, sizeof(m_stCurrentTime) );
	}
};


class sDBResponse_WeddingReserveListGet
{
public:
	sPacketHeader			m_packetHeader;

	cltWeddingReserveUnit	m_clReserveList[WEDDING_RESERVE_LIST_NUM];

public:
	sDBResponse_WeddingReserveListGet()
	{
		ZeroMemory( this, sizeof(sDBResponse_WeddingReserveListGet) );
	}

};	// DBMSG_RESPONSE_WEDDING_RESERVE_LIST_GET

// [영훈] 결혼 시스템 : 예식 예약 하기
class sDBRequest_WeddingReserve
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;		// 예약자

	GMONEY			m_gmCost;			// 예약 비용

	SYSTEMTIME		m_stReserveTime;	// 예약하려는 시간

public:
	sDBRequest_WeddingReserve( SI32 siPersonID, GMONEY gmCost, SYSTEMTIME& stReserveTime )
	{
		m_packetHeader.usCmd	= DBMSG_REQUEST_WEDDING_RESERVE;
		m_packetHeader.usSize	= sizeof(sDBRequest_WeddingReserve);

		m_siPersonID			= siPersonID;

		m_gmCost				= gmCost;

		memcpy( &m_stReserveTime, &stReserveTime, sizeof(m_stReserveTime) );

	}
};

class sDBResponse_WeddingReserve
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siResult;			// 결과값

	SI32			m_siPersonID;		// 예약자
	TCHAR			m_szPersonName[MAX_PLAYER_NAME];

	SI32			m_siMatePersonID;	// 예약자의 배우자
	TCHAR			m_szMateName[MAX_PLAYER_NAME];

	SI32			m_siReserveNum;		// 예약번호

	GMONEY			m_gmLeftBankMoney;	// 예약자의 남은 은행돈
	SYSTEMTIME		m_stReserveTime;	// 예약시간


	GMONEY			m_gmReserveMoney;	// 예약자금액 : [추가] 2009-07-09 손성웅

public:
	sDBResponse_WeddingReserve()
	{
		ZeroMemory( this, sizeof(sDBResponse_WeddingReserve) );
	}
};	// DBMSG_RESPONSE_WEDDING_RESERVE

// [영훈] 결혼시스템 : 예약 취소하기
class sDBRequest_WeddingReserveCancel
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;	// 취소하는 사람

	SI32			m_siReserveNum;	// 취소하는 예약 번호

public:
	sDBRequest_WeddingReserveCancel( SI32 siPersonID, SI32 siReserveNum )
	{
		m_packetHeader.usCmd	= DBMSG_REQUEST_WEDDING_RESERVE_CANCEL;
		m_packetHeader.usSize	= sizeof(sDBRequest_WeddingReserveCancel);

		m_siPersonID			= siPersonID;
		m_siReserveNum			= siReserveNum;
	}
};

class sDBResponse_WeddingReserveCancel
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siResult;		// 결과값

	SI32			m_siPersonID;	// 취소하는 사람

	SI32			m_siReserveNum;	// 취소하는 예약 번호

public:
	sDBResponse_WeddingReserveCancel( )
	{
		ZeroMemory( this, sizeof(sDBResponse_WeddingReserveCancel) );
	}
};	// DBMSG_RESPONSE_WEDDING_RESERVE_CANCEL

#endif
