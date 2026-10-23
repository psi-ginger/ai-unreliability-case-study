//Switch_10MonthEvent_RacoonDodge
#ifndef _MSGTYPERACOONDODGEEVENT_H_
#define _MSGTYPERACOONDODGEEVENT_H_

#include "..\..\Common\CommonHeader.h"
#include "../../../DBManager/GameDBManager_World/dbMsg.h"

// 라쿤 이벤트 예약 
class cltGameMsgRequest_ReservationEvent	
{
public:
	SI32 siPersonID	;

	cltGameMsgRequest_ReservationEvent( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};
// 예약후 현제 예약된 리스트를 보여준다.
class cltGameMsgResponse_ReservationEvent
{
public:
	SI32 siPersonID	;

	cltGameMsgResponse_ReservationEvent( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};
// 라쿤 이벤트를 할 건지 물어 본다 
class cltGameMsgReQuest_Request_PlayEvent
{
public:
	SI32 siPersonID	;

	cltGameMsgReQuest_Request_PlayEvent( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};
// 라쿤 이벤트를 할 건지 물어 본다 
class cltGameMsgResponse_Request_PlayEvent
{
public:
	SI32 siPersonID	;

	cltGameMsgResponse_Request_PlayEvent( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};
// 빈자리가없습니다예약을 할수 없습니다.
class cltGameMsgRequest_Cannot_Reservation_NotEmptyList
{
public:
	SI32 siPersonID	;

	cltGameMsgRequest_Cannot_Reservation_NotEmptyList( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};

class cltGameMsgResponse_Cannot_Reservation_NotEmptyList
{
public:
	SI32 siPersonID	;

	cltGameMsgResponse_Cannot_Reservation_NotEmptyList( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};
// 5초이후 시작 메세지를 보낸다
class cltGameMsgResponse_Remain5Seceond
{
public:
	SI32 siPersonID	;

	cltGameMsgResponse_Remain5Seceond( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};
// 5초 대기 종료 시작
class cltGameMsgRequest_RacoonDodgeEveent_StartGame
{
public:
	SI32 siPersonID	;

	cltGameMsgRequest_RacoonDodgeEveent_StartGame( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};

class cltGameMsgResponse_RacoonDodgeEveent_StartGame
{
public:
	SI32 siPersonID	;

	cltGameMsgResponse_RacoonDodgeEveent_StartGame( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};
// 예약리스트를 요청한다
class cltGameMsgRequest_ReserveList
{
public:
	SI32 siPersonID	;

	cltGameMsgRequest_ReserveList( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};
// 예약리스트를 보낸다.
class cltGameMsgResponse_ReserveList
{
public:
	ReserveData	clInfo[MAX_RESERVER_SHOW_NUM];

	cltGameMsgResponse_ReserveList( ReserveData* Listinfo )
	{
		memcpy( clInfo, Listinfo, sizeof(clInfo))	;
	}
};
// 30초 타임 메세지를 보낸다
class cltGameMsgResponse_30Seceond
{
public:
	SI32 siPersonID	;
	SI32 siSecond	;

	cltGameMsgResponse_30Seceond( SI32 PersonID , SI32 Second)
	{
		siPersonID = PersonID;
		siSecond = Second;
	}
};
// 30초 타임 을 중지 한다 
class cltGameMsgResponse_30Seceond_Clear
{
public:
	SI32 siPersonID	;

	cltGameMsgResponse_30Seceond_Clear( SI32 PersonID)
	{
		siPersonID = PersonID;
	}
};
// 라운드 표시를 한다.
class cltGameMsgResponse_RoundCount
{
public:
	SI32 siPersonID	;
	SI32 siRound	;

	cltGameMsgResponse_RoundCount( SI32 PersonID , SI32 Round)
	{
		siPersonID = PersonID;
		siRound = Round;
	}
};
// 라운드 표시를 중지 한다 
class cltGameMsgResponse_Round_Clear
{
public:
	SI32 siPersonID	;

	cltGameMsgResponse_Round_Clear( SI32 PersonID)
	{
		siPersonID = PersonID;
	}
};
#endif
