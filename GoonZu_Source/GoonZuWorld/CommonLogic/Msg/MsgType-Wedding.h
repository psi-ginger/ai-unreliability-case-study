#ifndef _MSGTYPEWEDDING_H
#define _MSGTYPEWEDDING_H

#include "../Marriage/WeddingManager.h"

// 결혼식 예약하기
class cltGameMsgRequest_WeddingReserve
{
public:
	SI16	m_siYear;
	SI16	m_siMonth;
	SI16	m_siDay;
	SI16	m_siHour;

	cltGameMsgRequest_WeddingReserve( SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour )
	{
		m_siYear	= siYear;
		m_siMonth	= siMonth;
		m_siDay		= siDay;
		m_siHour	= siHour;
	}
};	// GAMEMSG_REQUEST_WEDDING_RESERVE

class cltGameMsgResponse_WeddingReserve
{
public:
	SI32	m_siResult;

	SI16	m_siYear;
	SI16	m_siMonth;
	SI16	m_siDay;
	SI16	m_siHour;

	GMONEY	m_gmLeftBankMoney;

	cltGameMsgResponse_WeddingReserve( SI32 siResult, SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour, GMONEY gmLeftBankMoney )
	{
		m_siResult			= siResult;

		m_siYear			= siYear;
		m_siMonth			= siMonth;
		m_siDay				= siDay;
		m_siHour			= siHour;

		m_gmLeftBankMoney	= gmLeftBankMoney;
	}
};	// GAMEMSG_RESPONSE_WEDDING_RESERVE

// 결혼 간단 예약 리스트 받기
// 요청 패킷은 없음
// GAMEMSG_REQUEST_WEDDING_SIMPLELIST_GET

class cltGameMsgResponse_WeddingSimpleListGet
{
public:
	cltWeddingReserveSimpleUnit	m_clSimpleUnit[WEDDING_RESERVE_LIST_NUM];

	cltGameMsgResponse_WeddingSimpleListGet( void )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_WeddingSimpleListGet) );
	}
};	// GAMEMSG_RESPONSE_WEDDING_SIMPLELIST_GET

// 결혼 관련 질문에 대한 대답
class cltGameMsgRequest_WeddingQuestionAnswer
{
public:
	SI32 m_siAnswerKind;

	cltGameMsgRequest_WeddingQuestionAnswer( SI32 siAnswerKind )
	{
		m_siAnswerKind = siAnswerKind;
	}
};	// GAMEMSG_REQUEST_WEDDING_QUESTION_ANSWER

// 결혼리스트 보여주기
class cltGameMsgRequest_WeddingListGet
{
public:
	SI32 m_siRequestPage;

	cltGameMsgRequest_WeddingListGet( SI32 siRequestPage )
	{
		m_siRequestPage = siRequestPage;
	}
};	// GAMEMSG_REQUEST_WEDDING_LIST_GET

class cltGameMsgResponse_WeddingListGet
{
public:
	cltWeddingReserveUnit m_clUnit[WEDDING_RESERVE_LIST_PAGE_PER_NUM];

	cltGameMsgResponse_WeddingListGet(void)
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_WeddingListGet) );
	}
};	// GAMEMSG_RESPONSE_WEDDING_LIST_GET

// 결혼 리스트 삭제
class cltGameMsgRequest_WeddingReserveCancel
{
public:
	SI16 m_siYear;
	SI16 m_siMonth;
	SI16 m_siDay;
	SI16 m_siHour;

	cltGameMsgRequest_WeddingReserveCancel( SI16 siYear, SI16 siMonth, SI16 siDay, SI16 siHour )
	{
		m_siYear	= siYear;
		m_siMonth	= siMonth;
		m_siDay		= siDay;
		m_siHour	= siHour;
	}
};	// GAMEMSG_REQUEST_WEDDING_RESERVE_CANCEL

class cltGameMsgResponse_WeddingReserveCancel
{
public:
	SI32 m_siResult;

	cltGameMsgResponse_WeddingReserveCancel( SI32 siResult )
	{
		m_siResult = siResult;
	}

};	// GAMEMSG_RESPONSE_WEDDING_RESERVE_CANCEL

#endif