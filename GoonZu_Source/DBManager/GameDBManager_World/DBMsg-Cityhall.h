#ifndef _DBMSGCITYHALL_H
#define _DBMSGCITYHALL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DBMsg.h"
#include "..\..\GoonZuWorld\CommonLogic\Cityhall\GeneralMeeting.h"
#include "..\..\GoonZuWorld\CommonLogic\Cityhall\Cityhall.h"
#include "..\..\GoonZuWorld\Server\Candidate\Candidate.h"


//-----------------------------------------
// 시청에 입후보할 것을 요청한다. 
//-----------------------------------------
class sDBRequest_Candidate
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;				// 입후보자. 
	cltSimpleRank		clRank;					// 입후보 대상 신분. 

	sDBRequest_Candidate()
	{
		ZeroMemory(this, sizeof(sDBRequest_Candidate));
	}

	sDBRequest_Candidate(SI32 id, SI32 personid, cltSimpleRank* pclrank)
	{
		packetHeader.usSize = sizeof( sDBRequest_Candidate );
		packetHeader.usCmd = DBMSG_REQUEST_CANDIDATE;
		usCharID			= id;

		siPersonID			= personid;
		clRank.Set(pclrank);
	}
};


class sDBResponse_Candidate
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siIndex;			// 입후보자 저장 공간의 인덱스. 
	cltSimplePerson		clPerson;			// 입후보자의 정보. 
	cltSimpleRank		clRank;				// 입후보한 신분 

	sDBResponse_Candidate()
	{
		ZeroMemory(this, sizeof(sDBResponse_Candidate));
	}
};

//-----------------------------------------
// 시청에 입후보한 것을 취소할 것을 요청한다. 
//-----------------------------------------
class sDBRequest_CancelCandidate
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;				// 입후보자. 

	sDBRequest_CancelCandidate()
	{
		ZeroMemory(this, sizeof(sDBRequest_CancelCandidate));
	}

	sDBRequest_CancelCandidate(SI32 id, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_CancelCandidate );
		packetHeader.usCmd = DBMSG_REQUEST_CANCELCANDIDATE;
		usCharID			= id;

		siPersonID			= personid;
	}
};


class sDBResponse_CancelCandidate
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	SI32				siIndex;			// 입후보자 저장 공간의 인덱스. 
	cltSimplePerson		clPerson;			// 입후보자의 정보. 
	cltSimpleRank		clRank;				// 입후보한 신분 

	sDBResponse_CancelCandidate()
	{
		ZeroMemory(this, sizeof(sDBResponse_CancelCandidate));
	}
};


//-----------------------------------------
// Person을 특정 Rank로 만들어 줄 것을 요청한다. 
//-----------------------------------------
class sDBRequest_BeRank
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 대상이되는 Person
	SI32				siCandidateIndex;	// 후보자 인덱스. 
	cltSimpleRank		clRank;				// Rank정보.

	sDBRequest_BeRank()
	{
		ZeroMemory(this, sizeof(sDBRequest_BeRank));
	}

	sDBRequest_BeRank(SI32 id, SI32 personid, SI32 candidateindex, cltSimpleRank* pclrank)
	{
		packetHeader.usSize = sizeof( sDBRequest_BeRank );
		packetHeader.usCmd = DBMSG_REQUEST_BERANK;
		usCharID			= id;

		siPersonID			= personid;
		siCandidateIndex	= candidateindex;
		clRank.Set(pclrank);
	}
};


class sDBResponse_BeRank
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	cltRank				clRank;				// 결과 신분이 어떻게 되었는가 ?

	SI32				siCandidateIndex;	// 후보자 인덱스. 
	cltSimplePerson		clCandidatePerson;	// 후보자의 최신 정보. 

	sDBResponse_BeRank()
	{
		ZeroMemory(this, sizeof(sDBResponse_BeRank));
	}
};


//-----------------------------------------
// 복수개의 후보자 정보를 얻어올 것을 요청한다. 
//-----------------------------------------
class sDBRequest_ManyCandidateInfo
{
public:
	sPacketHeader		packetHeader;

	cltSimpleRank		clRank;

	sDBRequest_ManyCandidateInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_ManyCandidateInfo));
	}

	sDBRequest_ManyCandidateInfo(cltSimpleRank* pclrank)
	{
		packetHeader.usSize = sizeof( sDBRequest_ManyCandidateInfo );
		packetHeader.usCmd = DBMSG_REQUEST_MANYCANDIDATEINFO;

		clRank.Set(pclrank);
	}
};


class sDBResponse_ManyCandidateInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	cltSimpleRank		clRank;							// 입후보한 신분
	cltSimplePerson		clPerson[MAX_CANDIDATE_NUMBER];	// 후보자 명단. (인덱스가 의미 있음)ㅒ

	sDBResponse_ManyCandidateInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_ManyCandidateInfo));
	}
};


//-----------------------------------------
// 등록된 후보들을 모두 삭제한다. 
//-----------------------------------------
class sDBRequest_DelCandidate
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	cltSimpleRank		clRank;		// 삭제할 신분. (이 신분에 입후보한 사람들의 명단은 모두 삭제한다.) 

	sDBRequest_DelCandidate()
	{
		ZeroMemory(this, sizeof(sDBRequest_DelCandidate));
	}

	sDBRequest_DelCandidate(SI32 id, cltSimpleRank* pclrank)
	{
		packetHeader.usSize = sizeof( sDBRequest_DelCandidate );
		packetHeader.usCmd = DBMSG_REQUEST_DELCANDIDATE;

		usCharID	= id;

		clRank.Set(pclrank);
	}
};


class sDBResponse_DelCandidate
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	cltSimpleRank		clRank;				// 입후보한 신분

	sDBResponse_DelCandidate()
	{
		ZeroMemory(this, sizeof(sDBResponse_DelCandidate));
	}
};

//-----------------------------------------
// 등록된 신분직을 사임한다. 
//-----------------------------------------
class sDBRequest_Resign
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;	// 대상이 되는 Person
	cltSimpleRank		clRank;		// 삭제할 신분. 

	sDBRequest_Resign()
	{
		ZeroMemory(this, sizeof(sDBRequest_Resign));
	}

	sDBRequest_Resign(SI32 id, SI32 personid, cltSimpleRank* pclrank)
	{
		packetHeader.usSize = sizeof( sDBRequest_Resign );
		packetHeader.usCmd	= DBMSG_REQUEST_RESIGN;

		usCharID	= id;
		siPersonID	= personid;

		clRank.Set(pclrank);
	}
};


class sDBResponse_Resign
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	cltSimpleRank		clRank;				// 사임한 신분
	cltSimplePerson		clPerson;			// 사임한 신분에 결과적으로 들어가는 Person정보. 

//#if defined(_CITYHALL_BANKRUPTCY)
	cltDate				clDate;				// 사임한 일자
	SI32				siVillageUnique;	// 사임한 마을 정보 => 사임일자 갱신에 사용
//#endif

	sDBResponse_Resign()
	{
		ZeroMemory(this, sizeof(sDBResponse_Resign));
	}
};



//-----------------------------------------
// 마을에 NPC고용을 요청한다 
//-----------------------------------------
enum{HIRENPC_HIRENPC, HIRENPC_HIREEVENTNPC}; //npc 고용 모드 
class sDBRequest_HireNPC
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;			// VillageUnique

	UI16				usSlot;						// slot
	cltHiredNPC			clHireNPCInfo;				// NPC Info
	UI08				uiMode;						// NPC 고용 이냐 Tree 고용이냐

	sDBRequest_HireNPC( UI16 id, SI32 villageunique, UI16 slot, SI32 npckind, GMONEY hirepay, GMONEY monthlypay, cltDate *hireDate , UI08 mode )
	{

		packetHeader.usSize = sizeof( sDBRequest_HireNPC );
		packetHeader.usCmd = DBMSG_REQUEST_HIRENPC;

		usCharID = id;

		siVillageUnique	= villageunique;

		usSlot = slot;

		clHireNPCInfo.siNPCKind = npckind;
		clHireNPCInfo.siHirePay = hirepay;
		clHireNPCInfo.siMonthlyPay = monthlypay;
		clHireNPCInfo.clHireDate.Set( hireDate );
		
		uiMode = mode;

	}

};

class sDBResponse_HireNPC
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 결과값.

	SI32				siVillageUnique;			// VillageUnique

	UI16				usSlot;						// slot
	cltHiredNPC			clHireNPCInfo;				// NPCInfo


	cltMoney			clCityhallMoney;			// 상단 보유자금 
	
	UI08				uiMode;						// NPC 고용이냐 트리 고용이냐

	sDBResponse_HireNPC()
	{
		ZeroMemory( this, sizeof( sDBResponse_HireNPC ) );
	}

};



//---------------------------------------
// 주민 등록을 신청한다 
//---------------------------------------
class sDBRequest_ApplyResident
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;					// person ID
	SI32				siVillageUnique;			// 마을의 VillageUnique
	cltDate				clDate;						// 신청일

	bool				bSendMsg;					// 마을을 통한 신청이 아닌 다른 캐릭의 초대로 신청이 된거라 결과를 받아 별도의 메시지 처리를 위해. 


	sDBRequest_ApplyResident( SI32 id, SI32 personid, SI32 villageUnique, cltDate *pclDate, bool _bSendMsg = false )
	{

		packetHeader.usSize = sizeof( sDBRequest_ApplyResident );
		packetHeader.usCmd = DBMSG_REQUEST_APPLYRESIDENT;

		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageUnique;

		clDate.Set( pclDate );
		
		bSendMsg = _bSendMsg;
	}

};

//---------------------------------------
// 주민 등록 신청에 응답한다
//---------------------------------------
class sDBResponse_ApplyResident
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 결과: 1성공 0실패 -1이미 신청되어있음 -2이미 주민임 -3신청자초과

    cltResidentApplyer	clApplyer;					// 신청자 정보
	SI32				siVillageUnique;			// 마을의 VillageUnique (-1이거나 -2일 경우 해당마을의 코드를 반환)

	bool				bSendMsg;					// 마을을 통한 신청이 아닌 다른 캐릭의 초대로 신청이 된거라 결과를 받아 별도의 메시지 처리를 위해. 

	sDBResponse_ApplyResident()
	{
		ZeroMemory( this, sizeof( sDBResponse_ApplyResident ) );
	}

};

//---------------------------------------
// 주민 등록 신청자 리스트를 요청한다 
//---------------------------------------
class sDBRequest_GetResidentApplyerList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;			// 마을의 VillageUnique

	sDBRequest_GetResidentApplyerList( UI16 id, SI32 villageUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_GetResidentApplyerList );
		packetHeader.usCmd = DBMSG_REQUEST_GETRESIDENTAPPLYERLIST;

		usCharID		= id;

		siVillageUnique = villageUnique;
	}

};


//---------------------------------------
// 주민 등록 신청자 리스트를 반환한다
//---------------------------------------
class sDBResponse_GetResidentApplyerList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 결과: 1성공 0실패

	SI32				siVillageUnique;
	
	cltResidentApplyer	clApplyer[ MAX_RESIDENT_APPLYER ];

	sDBResponse_GetResidentApplyerList()
	{
		ZeroMemory( this, sizeof( sDBResponse_GetResidentApplyerList ) );
	}

};


//---------------------------------------
// 주민 리스트를 요청한다 
//---------------------------------------
class sDBRequest_GetResidentList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;			// 마을의 VillageUnique
	SI32				siSort;

	sDBRequest_GetResidentList( UI16 id, SI32 villageUnique, SI32 sort )
	{
		packetHeader.usSize = sizeof( sDBRequest_GetResidentList );
		packetHeader.usCmd = DBMSG_REQUEST_GETRESIDENTLIST;

		usCharID		= id;

		siVillageUnique = villageUnique;
		siSort = sort;
	}

};


//---------------------------------------
// 주민 리스트를 반환한다
//---------------------------------------
class sDBResponse_GetResidentList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 결과: 1성공 0실패

	SI32				siVillageUnique;
	
	cltResident			clResident[ MAX_RESIDENT_NUMBER ];

	sDBResponse_GetResidentList()
	{
		ZeroMemory( this, sizeof( sDBResponse_GetResidentList ) );
	}

};


//---------------------------------------
// 주민 등록 신청을 허가한다 
//---------------------------------------
class sDBRequest_BeResident
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI32				siVillageUnique;

	cltDate				clDate;						// 등록일
	
	bool				bAutoResident;				// 이값이 true 이면 마을신청리스트에 없어도 가입된다


	sDBRequest_BeResident( UI16 id, SI32 personid, SI32 villageUnique, cltDate *pclDate, bool autoResident ) 
	{
		packetHeader.usSize = sizeof( sDBRequest_BeResident );
		packetHeader.usCmd = DBMSG_REQUEST_BERESIDENT;

		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique = villageUnique;

		clDate.Set( pclDate );

		bAutoResident	= autoResident;

	}

};


//---------------------------------------
// 주민 등록 신청 허가에대한 응답
//---------------------------------------
class sDBResponse_BeResident
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 1:성공	0:실패

	SI32				siVillageUnique;

	cltResident			clResident;

	sDBResponse_BeResident()
	{
		ZeroMemory( this, sizeof( sDBResponse_BeResident ) );
	}

};


//---------------------------------------
// 주민 등록 신청을 취소한다 or 주민등록을 해지한다 
//---------------------------------------
class sDBRequest_CancelResident
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI32				siVillageUnique;

	bool				bAutoCancel;

	sDBRequest_CancelResident( UI16 id, SI32 personid, SI32 villageUnique, bool autoCancel )
	{
		packetHeader.usSize = sizeof( sDBRequest_CancelResident );
		packetHeader.usCmd = DBMSG_REQUEST_CANCELRESIDENT;

		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique = villageUnique;

		bAutoCancel		= autoCancel;
	}

};



//---------------------------------------
// 주민 등록 신청 취소 및 해지 요청에 대한 응답 
//---------------------------------------
class sDBResponse_CancelResident
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 1:성공	0:실패

	SI32				siPersonID;					// personid
	SI32				siVillageUnique;			// 마을 유니크 

	SI32				siResidentNum;				// 마을 주민 숫자 
	
	sDBResponse_CancelResident()
	{
		ZeroMemory( this, sizeof( sDBResponse_CancelResident ) );
	}

};

//---------------------------------------
// 주민 상세 정보를 요청한다 
//---------------------------------------
class sDBRequest_GetResidentInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	TCHAR				szPersonName[MAX_PLAYER_NAME];

	sDBRequest_GetResidentInfo( UI16 id, SI32 villageUnique, TCHAR *personName )
	{
		packetHeader.usCmd = DBMSG_REQUEST_GETRESIDENTINFO;
		packetHeader.usSize = sizeof( sDBRequest_GetResidentInfo );

		usCharID = id;

		siVillageUnique = villageUnique;

		memcpy( szPersonName, personName, MAX_PLAYER_NAME );
		szPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;
	}
};


//---------------------------------------
// 주민 상세 정보 요청에 대한 응답
//---------------------------------------
class sDBResponse_GetResidentInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;							// 1:성공, 0:실패

	SI32				siVillageUnique;

	TCHAR				szPersonName[MAX_PLAYER_NAME];
	SI32				siPersonID;

	SI32				siScore;

	cltDate				clRegDate;


	sDBResponse_GetResidentInfo()
	{
		ZeroMemory( this, sizeof( sDBResponse_GetResidentInfo ) );
	}
};

class sDBRequest_SetResidentScore
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	
	SI32				siVillageUnique;
	SI32				siPersonID;
	SI32				siScore;

	sDBRequest_SetResidentScore( SI32 villageUnique, SI32 personid, SI32 score )
	{
		packetHeader.usCmd = DBMSG_REQUEST_SETRESIDENTSCORE;
		packetHeader.usSize = sizeof( sDBRequest_SetResidentScore );

		siVillageUnique = villageUnique;
		siPersonID = personid;
		siScore = score;
	}

};

class sDBResponse_SetResidentScore
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;


};

class sDBRequest_CityHallSetInOutMoney
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;						// 마을 
	SI32				siStrType;								// 입출금 건물
	SI32				siInOutCode;							// 입출금 코드
	cltDate				clDate;									// 입출금일 
	cltMoney			clInOutMoney;							// 입출금액 
	cltMoney			clCityHallMoney;						// 상단보유자금

	sDBRequest_CityHallSetInOutMoney( SI32 villageunique, SI32 strtype, SI32 inoutcode, cltDate *pclDate, cltMoney *pclMoney, cltMoney *pclCityhallMoney )
	{
		packetHeader.usCmd = DBMSG_REQUEST_CITYHALL_SETINOUTMONEY;
		packetHeader.usSize = sizeof( sDBRequest_CityHallSetInOutMoney );

		siVillageUnique = villageunique;
		siStrType = strtype;
		siInOutCode = inoutcode;
		clDate.Set( pclDate );
		clInOutMoney.Set( pclMoney );
		clCityHallMoney.Set( pclCityhallMoney );
	}
};

// PCK : 로그용 제작
class sDBResponse_CityHallSetInOutMoney
{
public:
	sPacketHeader	packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;						// 마을 
	SI32				siStrType;								// 입출금 건물
	SI32				siInOutCode;							// 입출금 코드
	cltDate				clDate;									// 입출금일 
	cltMoney			clInOutMoney;							// 입출금액 
	cltMoney			clCityHallMoney;						// 상단보유자금

	sDBResponse_CityHallSetInOutMoney()
	{
		ZeroMemory( this, sizeof( sDBResponse_CityHallSetInOutMoney ) );
	}
};
class sDBRequest_SetVillageSymbol
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;						// 마을
	
	SI16				siSymbolKind;							// 심볼(수호신) 종류
	SI32				siSymbolLife;							// 심볼(수호신) 체력
	SI32				siSymbolMana;							// 심볼(수호신) Mana
	SI16				siSymbolCapa;							// 심볼(수호신) 성능 

	sDBRequest_SetVillageSymbol( SI32 villageunique, SI16 symbolKind, SI32 symbolLife, SI32 symbolMana, SI16 symbolCapa )
	{
		packetHeader.usCmd = DBMSG_REQUEST_SETVILLAGESYMBOL;
		packetHeader.usSize = sizeof( sDBRequest_SetVillageSymbol );

		siVillageUnique = villageunique;
		
		siSymbolKind = symbolKind;
		siSymbolLife = symbolLife;
		siSymbolMana = symbolMana;
		siSymbolCapa = symbolCapa;
	}

};


class sDBRequest_SetSymbolStatus
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;						// 마을

	SI16				siStatKind;								// 스탯 종류: 1:종류  2: 체력  3:Mana  4:성능
	SI32				siStatValue;							// 스탯 수치

	GMONEY				siCost;									// 지불 비용
	cltDate				clDate;

	sDBRequest_SetSymbolStatus( SI32 id, SI32 villageunique, SI16 statKind, SI32 statValue, GMONEY cost, cltDate *pclDate )
	{
		packetHeader.usCmd = DBMSG_REQUEST_SETSYMBOLSTATUS;
		packetHeader.usSize = sizeof( sDBRequest_SetSymbolStatus );

		usCharID = id;
		siVillageUnique = villageunique;
		siStatKind = statKind;
		siStatValue = statValue;
		siCost = cost;
		clDate.Set( pclDate );
	}
};

class sDBResponse_SetSymbolStatus
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;								// 결과: 1:성공 0:실패 2:상단보유자금부족

	SI32				siVillageUnique;						// 마을

	SI16				siStatKind;								// 스탯 종류 
	SI32				siStatValue;							// 스탯 수치

	GMONEY				siCost;									// 지불 비용
	cltDate				clDate;									// 시간

	cltMoney			clCityhallMoney;						// 상단 자금 

	sDBResponse_SetSymbolStatus()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetSymbolStatus ) );
	}
};


class sDBRequest_SetVillageCastle
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;						// 마을
	
	SI16				siCastleKind;							// 캐슬 종류
	SI32				siCastleLife;							// 캐슬 체력
	SI32				siCastleMana;							// 캐슬 Mana
	SI16				siCastleCapa;							// 캐슬 성능 

	sDBRequest_SetVillageCastle( SI32 villageunique, SI16 castleKind, SI32 castleLife, SI32 castleMana, SI16 castleCapa )
	{
		packetHeader.usCmd = DBMSG_REQUEST_SETVILLAGECASTLE;
		packetHeader.usSize = sizeof( sDBRequest_SetVillageCastle );

		siVillageUnique = villageunique;
		
		siCastleKind = castleKind;
		siCastleLife = castleLife;
		siCastleMana = castleMana;
		siCastleCapa = castleCapa;
	}

};


class sDBRequest_SetCastleStatus
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;						// 마을

	SI16				siStatKind;								// 스탯 종류: 1:종류  2: 체력  3:Mana  4:성능
	SI32				siStatValue;							// 스탯 수치

	GMONEY				siCost;									// 지불 비용
	cltDate				clDate;

	sDBRequest_SetCastleStatus( SI32 id, SI32 villageunique, SI16 statKind, SI32 statValue, GMONEY cost, cltDate *pclDate )
	{
		packetHeader.usCmd = DBMSG_REQUEST_SETCASTLESTATUS;
		packetHeader.usSize = sizeof( sDBRequest_SetCastleStatus );

		usCharID = id;
		siVillageUnique = villageunique;
		siStatKind = statKind;
		siStatValue = statValue;
		siCost = cost;
		clDate.Set( pclDate );
	}
};

class sDBResponse_SetCastleStatus
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;								// 결과: 1:성공 0:실패 2:상단보유자금부족

	SI32				siVillageUnique;						// 마을

	SI16				siStatKind;								// 스탯 종류 
	SI32				siStatValue;							// 스탯 수치

	GMONEY				siCost;									// 지불 비용
	cltDate				clDate;									// 시간

	cltMoney			clCityhallMoney;						// 상단 자금 

	sDBResponse_SetCastleStatus()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetCastleStatus ) );
	}
};

class sDBRequest_ResidentShare
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;


	SI32				siVillageUnique;
	cltMoney 			clTotalShareMoney;				// 총 배당금

	SI32				siCount;						// 배당자 숫자 

	SI32				siCharIndex[ 100 ];
	SI32				siPersonID[ 100 ];				// PersonID
	GMONEY				siShareMoney[ 100 ];			// 배당금 

	sDBRequest_ResidentShare()
	{
		packetHeader.usCmd = DBMSG_REQUEST_RESIDENTSHARE;
		packetHeader.usSize = sizeof( sDBRequest_ResidentShare );
	}
};

class sDBResponse_ResidentShare
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;
	
	cltMoney			clTotalShareMoney;				// 총 배당금 
	cltMoney			clCityhallMoney;

	SI32				siCount;						// 배당자 숫자

	SI32				siCharIndex[ 100 ];
	SI32				siPersonID[ 100 ];				// PersonID
	GMONEY				siShareMoney[ 100 ];
	GMONEY				siMoney[ 100 ];					// 현재 소지금 

	sDBResponse_ResidentShare()
	{
		ZeroMemory( this, sizeof( sDBResponse_ResidentShare ) );
	}
};

class sDBRequest_SetVillageLevel
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	SI32				siLevel;

	SI32				siSlaveVillageNum;
	SI32				siResidentLevel;
	SI32				siVisitLevel;

	sDBRequest_SetVillageLevel( SI32 villageunique, SI32 level, SI32 slave, SI32 resident, SI32 visit )
	{
		packetHeader.usCmd = DBMSG_REQUEST_SETVILLAGELEVEL;
		packetHeader.usSize = sizeof( sDBRequest_SetVillageLevel );

		siVillageUnique = villageunique;
		siLevel = level;

		siSlaveVillageNum = slave;
		siResidentLevel = resident;
		siVisitLevel = visit;
	}

};

class sDBResponse_SetVillageLevel
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;
	SI32				siLevel;

	SI32				siSlaveVillageNum;
	SI32				siResidentLevel;
	SI32				siVisitLevel;

	sDBResponse_SetVillageLevel()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetVillageLevel ) );
	}


};


// 이익금 -> 자본금으로 전환
class sDBRequest_ProfitToCapitalFund
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;			// villageunique
	GMONEY				siMoney;					// 전환할 금액

	sDBRequest_ProfitToCapitalFund( UI16 id, SI32 villageunique, GMONEY money )
	{
		packetHeader.usCmd = DBMSG_REQUEST_PROFITTOCAPITALFUND;
		packetHeader.usSize = sizeof( sDBRequest_ProfitToCapitalFund );
		
		usCharID = id;
		siVillageUnique = villageunique;
		siMoney = money;
	}

};

class sDBResponse_ProfitToCapitalFund
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 1:성공 0:실패 2:이익금 부족
	
	SI32				siVillageUnique;

	cltMoney			clCapitalFund;				// 자본금
	cltMoney			clProfitMoney;				// 이익금

	sDBResponse_ProfitToCapitalFund()
	{
		ZeroMemory( this, sizeof( sDBResponse_ProfitToCapitalFund ) );
	}

};


class sDBRequest_SetVillageMap
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	SI32				siVillageMapIndex;
	GMONEY				siNeedMoney;
	SI08				siMapType;

	sDBRequest_SetVillageMap( UI16 id, SI32 villageunique, SI32 villagemap, GMONEY needmoney, SI08 maptype )
	{
		packetHeader.usCmd = DBMSG_REQUEST_SETVILLAGEMAP;
		packetHeader.usSize = sizeof( sDBRequest_SetVillageMap );

		usCharID = id;

		siVillageUnique = villageunique;
		siVillageMapIndex = villagemap;
		siNeedMoney = needmoney;
		siMapType = maptype;
	}


};

class sDBResponse_SetVillageMap
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	
	SI32				siResult;

	SI32				siVillageUnique;
	SI32				siVillageMapIndex;
	GMONEY				siNeedMoney;
	SI08				siMapType;

	sDBResponse_SetVillageMap()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetVillageMap ) );
	}

};

class sDBRequest_ManyResidentOut
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	cltDate				clDate;

	sDBRequest_ManyResidentOut( SI32 villageunique, cltDate *pclDate )
	{
		packetHeader.usCmd = DBMSG_REQUEST_MANYRESIDENTOUT;
		packetHeader.usSize = sizeof( sDBRequest_ManyResidentOut );

		siVillageUnique = villageunique;
		clDate.Set( pclDate );

	}
};

class sDBResponse_ManyResidentOut
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;
	cltDate				clDate;

	UI16				usCount;
	SI32				siPersonID[ 200 ];

	sDBResponse_ManyResidentOut()
	{
		ZeroMemory( this, sizeof( sDBResponse_ManyResidentOut ) );
	}
};


// 주주 총회 정보를 요청한다 
class sDBRequest_GeneralMeetingInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;

	sDBRequest_GeneralMeetingInfo( SI32 villageunique )
	{
		packetHeader.usCmd = DBMSG_REQUEST_GENERALMEETING_INFO;
		packetHeader.usSize = sizeof(sDBRequest_GeneralMeetingInfo);

		siVillageUnique = villageunique;
	}

};

class sDBResponse_GeneralMeetingInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;			
	SI32				siMatterType;						// 안건 내용 
	SI32				siPersonID;							// 발의자 personid
	cltDate				clStartDate;						// 주총 마지막 시작일자
	cltDate				clCloseDate;						// 종료 일자
	SI32				siTotalStock;						// 마을 총 주식수 
	SI32				siCandidatePersonID;				// 대행수 후보자 person id
	TCHAR				szCandidateName[ MAX_PLAYER_NAME ];	// 대행수 후보자 이름 
	SI32				siAcceptCandidate;					// 대행수 후보자 동의 여부 

	cltGeneralMeetingVoter	voter[ 100 ];					// 투표권자 정보

	sDBResponse_GeneralMeetingInfo()
	{
		ZeroMemory( this, sizeof( sDBResponse_GeneralMeetingInfo ) );
	}

};

// 주주총회에 대행수 임명건을 발의한다 
class sDBRequest_GeneralMeeting_SuggestChief
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;					// 마을 
	SI32				siPersonID;							// 발의자 personid

	TCHAR				szCandidateName[ MAX_PLAYER_NAME ];	// 후보자 name
	SI32				siStartDate;
	cltDate				clCloseDate;

	sDBRequest_GeneralMeeting_SuggestChief( UI16 id, SI32 villageunique, SI32 personid, TCHAR *candidate, SI32 StartDate, cltDate *pclDate )
	{
		packetHeader.usCmd = DBMSG_REQUEST_GENERALMEETING_SUGGEST_CHIEF;
		packetHeader.usSize = sizeof(sDBRequest_GeneralMeeting_SuggestChief);

		usCharID = id;

		siVillageUnique = villageunique;
		siPersonID = personid;

		memcpy( szCandidateName, candidate, MAX_PLAYER_NAME );
		szCandidateName[ MAX_PLAYER_NAME - 1 ] = NULL;
		siStartDate = StartDate;
		clCloseDate.Set( pclDate );
	}

};

class sDBResponse_GeneralMeeting_SuggestChief
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;					// 마을 
	SI32				siTotalStock;						// 마을 총 주식 수
	SI32				siPersonID;							// 발의자 personid
	SI32				siStockAmount;						// 발의자의 해당 마을 보유주식수 

	cltDate				clStartDate;						// 주총 시작 날
	cltDate				clCloseDate;

	SI32				siCandidatePersonID;				// 후보자 personid
	TCHAR				szCandidateName[ MAX_PLAYER_NAME ];	// 후보자 name

	cltGeneralMeetingVoter	clVoter[ 100 ];					// 투표권자 정보

	sDBResponse_GeneralMeeting_SuggestChief()
	{
		ZeroMemory( this, sizeof( sDBResponse_GeneralMeeting_SuggestChief ) );
	}

};

//[추가 : 황진성 2008. 2. 20 => 마을 주주총회 가능 여부를 운영자가 제어하게 될 패킷.]
class sDBRequest_GeneralMeeting_Suggest_On_Off
{
public:
	sPacketHeader		m_packetHeader;
	
	SI32				m_siVillageUnique;					// 마을 
	
    bool				m_bGeneralMeeting_Suggest_On_Off;
	
	sDBRequest_GeneralMeeting_Suggest_On_Off( SI32 _villageunique, bool _On_Off )
	{
		m_packetHeader.usCmd  = DBMSG_REQUEST_GENERALMEETING_SUGGEST_ON_OFF;
		m_packetHeader.usSize = sizeof(sDBRequest_GeneralMeeting_Suggest_On_Off);

		m_siVillageUnique				 = _villageunique;
		m_bGeneralMeeting_Suggest_On_Off = _On_Off;
	}
};

class sDBResponse_GeneralMeeting_Suggest_On_Off
{
public:
	sPacketHeader		m_packetHeader;
	
	SI32				m_siResult;

	SI32				m_siVillageUnique;					// 마을 

	bool				m_bGeneralMeeting_Suggest_On_Off;

	sDBResponse_GeneralMeeting_Suggest_On_Off()
	{
		ZeroMemory( this, sizeof( sDBResponse_GeneralMeeting_Suggest_On_Off ) );

		m_packetHeader.usCmd  = DBMSG_RESPONSE_GENERALMEETING_SUGGEST_ON_OFF;
		m_packetHeader.usSize = sizeof(sDBResponse_GeneralMeeting_Suggest_On_Off);
	}

};


//[추가 : 황진성 2008. 2. 20 => 마을 주주총회 가능 여부를 운영자가 제어하게 될 패킷.]
class sDBRequest_GeneralMeeting_Suggest_On_Off_List
{
public:
	sPacketHeader		m_packetHeader;


	sDBRequest_GeneralMeeting_Suggest_On_Off_List( SI32 _villageunique, bool _On_Off )
	{
		m_packetHeader.usCmd  = DBMSG_REQUEST_GENERALMEETING_SUGGEST_ON_OFF_LIST;
		m_packetHeader.usSize = sizeof(sDBRequest_GeneralMeeting_Suggest_On_Off_List);
	}
};

class sDBResponse_GeneralMeeting_Suggest_On_Off_List
{
public:
	sPacketHeader		m_packetHeader;

	bool				m_bGeneralMeeting_Suggest_On_Off_List[MAX_VILLAGE_NUMBER];

	sDBResponse_GeneralMeeting_Suggest_On_Off_List()
	{
		ZeroMemory( this, sizeof( sDBResponse_GeneralMeeting_Suggest_On_Off_List ) );

		m_packetHeader.usCmd  = DBMSG_RESPONSE_GENERALMEETING_SUGGEST_ON_OFF_LIST;
		m_packetHeader.usSize = sizeof(sDBResponse_GeneralMeeting_Suggest_On_Off_List);
	}

};





//---------------







// 투표한다 
class sDBRequest_GeneralMeeting_Vote
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	SI32				siVoterPersonID;

	SI32				siVoteType;

	sDBRequest_GeneralMeeting_Vote( UI16 id, SI32 villageunique, SI32 personid, SI32 votetype )
	{
		packetHeader.usCmd = DBMSG_REQUEST_GENERALMEETING_VOTE;
		packetHeader.usSize = sizeof(sDBRequest_GeneralMeeting_Vote);

		usCharID = id;

		siVillageUnique = villageunique;
		siVoterPersonID = personid;
		siVoteType = votetype;
	}

};

class sDBResponse_GeneralMeeting_Vote
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;
	SI32				siVoterPersonID;
	TCHAR				szVoterName[ MAX_PLAYER_NAME ];

	SI32				siVoteType;

	sDBResponse_GeneralMeeting_Vote()
	{
		ZeroMemory( this, sizeof( sDBResponse_GeneralMeeting_Vote ) );
	}

};

// 대행수 후보자가 본인 동의를 한다 
class sDBRequest_GeneralMeeting_AcceptCandidate
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	SI32				siPersonID;

	//KHY - 1115 - 주주총회 발동 시점 변경. - 	대행수 후보자가 본인 동의를 하는 순간 2달을 투표기간으로 설정한다.
	SI32				siStartDate;
	cltDate				clCloseDate;

	sDBRequest_GeneralMeeting_AcceptCandidate( UI16 id, SI32 villageunique, SI32 personid ,SI32 StartDate, cltDate *pclDate)
	{
		packetHeader.usCmd = DBMSG_REQUEST_GENERALMEETING_ACCEPTCANDIDATE;
		packetHeader.usSize = sizeof(sDBRequest_GeneralMeeting_AcceptCandidate);

		usCharID = id;

		siVillageUnique = villageunique;
		siPersonID = personid;

		siStartDate = StartDate;
		clCloseDate.Set( pclDate );
	}

};

class sDBResponse_GeneralMeeting_AcceptCandidate
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;
	SI32				siPersonID;

	//KHY - 1115 - 주주총회 발동 시점 변경. - 	대행수 후보자가 본인 동의를 하는 순간 2달을 투표기간으로 설정한다.
	SI32				siStartDate;
	cltDate				clCloseDate;

	sDBResponse_GeneralMeeting_AcceptCandidate()
	{
		ZeroMemory( this, sizeof( sDBResponse_GeneralMeeting_AcceptCandidate ) );
	}

};

// 대행수 임명건 주주총회를 종료한다
class sDBRequest_GeneralMeeting_CloseChief
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	
	cltDate				clDate;

	SI32				siVillageUnique;
	SI32				siVoteResult;

	sDBRequest_GeneralMeeting_CloseChief( SI32 villageunique, SI32 result, cltDate *pclDate )
	{
		packetHeader.usCmd = DBMSG_REQUEST_GENERALMEETING_CLOSE_CHIEF;
		packetHeader.usSize = sizeof(sDBRequest_GeneralMeeting_CloseChief);

		siVillageUnique = villageunique;
		siVoteResult = result;

		clDate.Set( pclDate );
	}

};

class sDBResponse_GeneralMeeting_CloseChief
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	cltDate				clDate;

	SI32				siVillageUnique;
	SI32				siVoteResult;

	SI32				siChiefPersonID;
	TCHAR				szChiefName[ MAX_PLAYER_NAME ];

	sDBResponse_GeneralMeeting_CloseChief()
	{
		ZeroMemory( this, sizeof( sDBResponse_GeneralMeeting_CloseChief ) );
	}

};

//  투표자의 주주총회 참여 여부를 설정한다
class sDBRequest_GeneralMeeting_SetVoterState
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	SI32				siPersonID;
	SI32				siParticipate;

	sDBRequest_GeneralMeeting_SetVoterState( SI32 villageunique, SI32 personid, SI32 participate )
	{
		packetHeader.usCmd = DBMSG_REQUEST_GENERALMEETING_SETVOTERSTATE;
		packetHeader.usSize = sizeof(sDBRequest_GeneralMeeting_SetVoterState);

		siVillageUnique = villageunique;
		siPersonID = personid;
		siParticipate = participate;
	}
};


class sDBResponse_GeneralMeeting_SetVoterState
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;
	SI32				siPersonID;
	SI32				siParticipate;

	sDBResponse_GeneralMeeting_SetVoterState()
	{
		ZeroMemory( this, sizeof( sDBResponse_GeneralMeeting_SetVoterState ) );
	}

};

class sDBRequest_SetVillageWarResult
{
public:
	sPacketHeader		packetHeader;
		
	// 전쟁이 끝났을때 승리마을, 패배 마을의 유니크
	SI32				m_siwinVillageUnique;
	SI32				m_siloseVillageUnique;
	// 승리한 마을에게 + 하고 패배한 마을엑 - 해야할 마을점수
	SI32				m_siwinnerGetVillageWarScore;
	// 승리한 마을에게 + 하고 패배한 마을엑 - 해야할 관청 자본금
	GMONEY				m_siwinnerGetVillageMoney;

	// 패배한 마을의 관청정보에 기록될 최종 패배일
	NDate				m_clDateLoser;

	sDBRequest_SetVillageWarResult( SI32 winVillageUnique, SI32 loseVillageUnique, SI32 winnerGetVillageWarScore, GMONEY winnerGetVillageMoney, NDate* Dateloser )
	{
		packetHeader.usCmd = DBMSG_REQUEST_SETVILLAGEWAR_RESULT;
		packetHeader.usSize = sizeof(sDBRequest_SetVillageWarResult);
		m_siwinVillageUnique = winVillageUnique;
		m_siloseVillageUnique = loseVillageUnique;
		m_siwinnerGetVillageWarScore = winnerGetVillageWarScore;
		m_siwinnerGetVillageMoney	= winnerGetVillageMoney;		
		m_clDateLoser.Set(Dateloser);
	}
};


class sDBResponse_SetVillageWarResult
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				m_siwinVillageUnique;
	SI32				m_siloseVillageUnique;	
	GMONEY				m_siwinnerVillageMoney; // 가감, 차감 후 승자의 남은 관청 자금
	GMONEY				m_siloserVillageMoney; // 가감, 차감 후 승자의 남은 관청 자금
	// 승리한 마을에게 + 하고 패배한 마을엑 - 해야할 관청 자본금
	GMONEY				m_siwinnerGetVillageMoney;
	NDate				m_clDateLoser;

	sDBResponse_SetVillageWarResult()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetVillageWarResult ) );
	}
};



//#if defined(_CITYHALL_BANKRUPTCY)
//---------------------------------------
// [요청] 상단 상태(파산기, 초기화기, 안정기, 정상기)가 변경되었다.
//---------------------------------------
class sDBRequest_CityHall_State_Changed
{
public:
	sPacketHeader		packetHeader;
	UI08				usPrevStatus;
	UI08				usCurStatus;

	SI32				siVillageUnique;


	sDBRequest_CityHall_State_Changed( SI32 villageunique, UI08 usprevstatus, UI08 uscurstatus )
	{
		packetHeader.usCmd = DBMSG_REQUEST_CITYHALL_STATE_CHANGED;
		packetHeader.usSize = sizeof(sDBRequest_CityHall_State_Changed);

		siVillageUnique = villageunique;
		usPrevStatus = usprevstatus;
		usCurStatus = uscurstatus;
	}

};

//---------------------------------------
// [응답] 상단 상태(파산기, 초기화기, 안정기, 정상기)가 변경되었다.
//---------------------------------------
class sDBReponse_CityHall_State_Changed
{
public:
	sPacketHeader		packetHeader;
	UI08				usPrevStatus;
	UI08				usCurStatus;

	SI32				siVillageUnique;

	SI32				siResult;

	sDBReponse_CityHall_State_Changed( )
	{
		ZeroMemory( this, sizeof( sDBReponse_CityHall_State_Changed ) );
	}

};




//---------------------------------------
// [요청] 상단 상태가 파산되었음을 LETTER로 알린다.
//---------------------------------------
class sDBRequest_Notify_Cityhall_Bankruptcy
{
public:
	enum {
		MAX_STOCKHOLDER_NUMBER = MAX_BANKRUPT_STOCKHOLDER_NUMBER_TO_BE_NOTIFIED, // 파산을 알릴 최대 주주수는 인자로 입력받지 않고 고정된다.
		// 파산된 마을의 주주 중 주식수로 1~500등까지만
	};

	sPacketHeader		packetHeader;

	SI32				siVillageUnique;		// 파산된 마을 고유 번호
	UI08				usType;					// 파산 인지, 파산 예정인지 구분

	sDBRequest_Notify_Cityhall_Bankruptcy( SI32 villageunique, UI08 ustype)
	{
		packetHeader.usCmd = DBMSG_REQUEST_NOTIFY_CITYHALL_BANKRUPTCY;
		packetHeader.usSize = sizeof(sDBRequest_Notify_Cityhall_Bankruptcy);

		siVillageUnique = villageunique;
		usType = ustype;
	}

};

//---------------------------------------
// [응답] 상단 상태가 파산되었음을 LETTER로 알린다.
//---------------------------------------
class sDBReponse_Notify_Cityhall_Bankruptcy
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	SI32				siVillageUnique;
	SI32				siPersonID[MAX_BANKRUPT_STOCKHOLDER_NUMBER_TO_BE_NOTIFIED];	// 위에 요청할때 최대수와 동일함
	UI08				usType;					// 파산 인지, 파산 예정인지 구분
    
	sDBReponse_Notify_Cityhall_Bankruptcy( )
	{
		ZeroMemory( this, sizeof( sDBReponse_Notify_Cityhall_Bankruptcy ) );
	}

};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_DonateVillageMoney
//
// Last Modified 	: 2007 / 08 / 10
// Created		 	: 김광명
//
// Function			: 마을에 돈 기부하기
//
//***************************************************************************************************
// 최하 10만원으로 변경 - by LEEKH 2007.12.06
const SI32 MIN_DONATE_MONEY = 100000;

class sDBRequest_DonateVillageMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	/// 기부하는 사람 PersonID
	SI32				siPersonID;		
	/// 기부받는 마을
	SI32				siVillageUnique;
	/// 기부날짜
	SI32				siDonateDate;
	/// 기부금액
	SI64				siDonateMoney;


	sDBRequest_DonateVillageMoney( SI32 CharID, SI32 PersonID, SI32 VillageUnique, SI32 DonateDate, SI64 DonateMoney )
	{
		packetHeader.usSize = sizeof( sDBRequest_DonateVillageMoney );
		packetHeader.usCmd = DBMSG_REQUEST_DONATEVILLAGEMONEY;

		siCharID = CharID;
		siPersonID = PersonID;
		siVillageUnique = VillageUnique;
		siDonateDate = DonateDate;
		siDonateMoney = DonateMoney;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_DonateVillageMoney
//
// Last Modified 	: 2007 / 08 / 10
// Created		 	: 김광명
//
// Function			: 마을에 돈 기부하기
//
//***************************************************************************************************
class sDBResponse_DonateVillageMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	/// 기부하는 사람 PersonID
	SI32				siPersonID;		
	/// 기부받는 마을
	SI32				siVillageUnique;
	/// 기부날짜
	SI32				siDonateDate;
	/// 기부금액
	SI64				siDonateMoney;
	/// 기부한 사람 이름
	TCHAR				szDonateName[MAX_PLAYER_NAME];
	/// 변경된 마을 이익금
	SI64				siChangedVillageProfitMoney;
	/// 기부후에 변화된 소지금
	SI64				siChangedMyMoney;

	sDBResponse_DonateVillageMoney()
	{
		ZeroMemory(this, sizeof(sDBResponse_DonateVillageMoney));
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_DonateMemberList
//
// Last Modified 	: 2007 / 08 / 10
// Created		 	: 김광명
//
// Function			: 기부자 리스트 불러오기
//
//***************************************************************************************************
struct DonateMemberList
{
	/// 기부한 사람 이름
	TCHAR				szDonateName[MAX_PLAYER_NAME];
	/// 기부한 시간
	SI32				siDonateDate;
	/// 기부한 금액
	SI64				siDonateMoney;
};

const SI32 MAX_DONATEMEMBERLIST_PER_PAGE = 10;

class sDBRequest_DonateMemberList
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siVillageUnique;
	SI32				siRequestPage;

	sDBRequest_DonateMemberList( SI32 CharID, SI32 VillageUnique, SI32 RequestPage )
	{
		packetHeader.usSize = sizeof( sDBRequest_DonateMemberList );
		packetHeader.usCmd = DBMSG_REQUEST_DONATEMEMBERLIST;

		siCharID = CharID;
		siVillageUnique = VillageUnique;
		siRequestPage = RequestPage;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_DonateMemberList
//
// Last Modified 	: 2007 / 08 / 10
// Created		 	: 김광명
//
// Function			: 기부자 리스트 불러오기
//
//***************************************************************************************************
class sDBResponse_DonateMemberList
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	SI32				siCharID;
	SI32				siVillageUnique;
	// 요청한 페이지
	SI32				siRequestPage;
	/// 목록 불러올 마을유니크
	DonateMemberList    sDonateMemberList[MAX_DONATEMEMBERLIST_PER_PAGE];
	SI32				siMaxPage;

	sDBResponse_DonateMemberList()
	{
		ZeroMemory(this, sizeof(sDBResponse_DonateMemberList));
	}
};
// 상단 파산에 따른 마을을 초기화 한다.
// 파산 후 마을 여러 정보 초기화는 서버 점검시 수작업으로 한다.
//class sDBRequest_Village_Initialization
//{
//public:
//	sPacketHeader		packetHeader;
//	UI08				usPrevStatus;
//	UI08				usCurStatus;
//
//	SI32				siVillageUnique;
//
//
//	sDBRequest_Village_Initialization( SI32 villageunique, UI08 usprevstatus, UI08 uscurstatus )
//	{
//		packetHeader.usCmd = DBMSG_REQUEST_VILLAGE_INITIALIZATION;
//		packetHeader.usSize = sizeof(sDBRequest_Village_Initialization);
//
//		siVillageUnique = villageunique;
//		usPrevStatus = usprevstatus;
//		usCurStatus = uscurstatus;
//	}
//
//};
//
//class sDBReponse_Village_Initialization
//{
//public:
//	sPacketHeader		packetHeader;
//	UI08				usPrevStatus;
//	UI08				usCurStatus;
//
//	SI32				siVillageUnique;
//
//	SI32				siResult;
//
//	sDBReponse_Village_Initialization( )
//	{
//		ZeroMemory( this, sizeof( sDBReponse_Village_Initialization ) );
//	}
//
//};
//#endif




#endif