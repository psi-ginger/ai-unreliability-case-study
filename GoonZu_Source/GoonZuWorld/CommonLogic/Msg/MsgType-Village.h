#ifndef _MSGTYPEVILLAGE_H
#define _MSGTYPEVILLAGE_H

#include "./CityHall/CityHall.h"
#include "NDate.h"

// [추가 : 황진성 2008. 2. 21 => 마을 주주총회 가능 여부 알려주기위한 구조체.]
class cltGameMsgResponse_VillageGeneralMeeting{
public:
	bool	m_siVillageGeneralMeeting[MAX_VILLAGE_NUMBER];

	cltGameMsgResponse_VillageGeneralMeeting( void )
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_VillageGeneralMeeting ) );
	}
}; 



//-------------------------------------------
// 뉴스 정보 통보. 
//-------------------------------------------
class cltGameMsgRequest_VillageDetailInfo{
public:

	SI16	siVillageDetailInfoType;

	cltGameMsgRequest_VillageDetailInfo(SI32 detailtype)
	{
		siVillageDetailInfoType = detailtype;
	}
};

class cltGameMsgResponse_VillageDetailInfo{
public:

	SI16	siVillageDetailInfoType;

	union{
		SI32	siWarKillNum[MAX_VILLAGE_NUMBER];		// 전쟁전과 통보. 
		UI32	uiPeople[MAX_VILLAGE_NUMBER];			// 주민수. 
		SI32	siUserNum[MAX_VILLAGE_NUMBER];			// 마을 사용자수. 
		GMONEY	siBidMoney[MAX_VILLAGE_NUMBER];			// 배당액. 
		GMONEY	siStrData[MAX_VILLAGE_NUMBER];			// 건물 데이터. 
		//SI32	siVillageLevel[MAX_VILLAGE_NUMBER];		// 마을 레벨 제한. 
		SI16	siVillageWarRecordWinAndLose[2][MAX_VILLAGE_NUMBER];	// 마을 공성전 전적. 배열의 앞의 반은 승리, 뒤의 반은 패배 데이터
		//SI16	siVillageWarRecordLose[MAX_VILLAGE_NUMBER];
		UI08	uiCityHallState[MAX_VILLAGE_NUMBER];	// 관청상태정보(정상, 파산, 안정 등 )
		SI32	siTotalVillageScore[MAX_VILLAGE_NUMBER];// 마을 총 점수

	};

	cltGameMsgResponse_VillageDetailInfo()
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_VillageDetailInfo));
	}
};

class cltGameMsgResponse_DeclareWar{
public:
	SI32		siVillageUnique;
	SI32		siTargetVillage;

	NDate		clDate;

	cltGameMsgResponse_DeclareWar()
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_DeclareWar) );
	}

};

class cltGameMsgResponse_CancelDeclareWar{
public:
	SI32		siVillageUnique;
	SI32		siTargetVillage;

	cltGameMsgResponse_CancelDeclareWar()
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_CancelDeclareWar) );
	}
};

class cltGameMsgResponse_VillageWarResult
{
public:
	SI32		siWinVillage;
	SI32		siLoseVillage;

	cltGameMsgResponse_VillageWarResult()
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_VillageWarResult) );
	}
};


class cltGameMsgResponse_VillageWarResultToVillageMember
{
public:
	SI32		siWinVillage;
	SI32		siLoseVillage;

	SI32		siWinnerGetVillageWarScore;
	GMONEY		siWinnerGetVillageMoney;

	cltGameMsgResponse_VillageWarResultToVillageMember(SI32 WinVillage, SI32 LoseVillage, SI32 WinnerGetVillageWarScore, GMONEY WinnerGetVillageMoney  )
	{
		siWinVillage = WinVillage;
		siLoseVillage = LoseVillage;
		siWinnerGetVillageWarScore = WinnerGetVillageWarScore;
		siWinnerGetVillageMoney = WinnerGetVillageMoney;		
	}
};


class cltGameMsgRequest_SetSymbolStatus
{
public:
	SI16		siStatKind;
	SI32		siStatValue;

	cltGameMsgRequest_SetSymbolStatus( SI16 statKind, SI32 statValue )
	{
		siStatKind = statKind;
		siStatValue = statValue;
	}

};

class cltGameMsgResponse_SetSymbolStatus
{
public:
	SI32		siVillageUnique;

	SI16		siStatKind;
	SI32		siStatValue;

	GMONEY		siCost;

	cltGameMsgResponse_SetSymbolStatus()
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_SetSymbolStatus ) );
	}

};

class cltGameMsgRequest_SetCastleStatus
{
public:
	SI16		siStatKind;
	SI32		siStatValue;

	cltGameMsgRequest_SetCastleStatus( SI16 statKind, SI32 statValue )
	{
		siStatKind = statKind;
		siStatValue = statValue;
	}

};

class cltGameMsgResponse_SetCastleStatus
{
public:
	SI32		siVillageUnique;

	SI16		siStatKind;
	SI32		siStatValue;

	GMONEY		siCost;

	cltGameMsgResponse_SetCastleStatus()
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_SetCastleStatus ) );
	}

};

class cltGameMsgRequest_ResidentsList
{
public:
	SI32		siVillageUnique;

	cltGameMsgRequest_ResidentsList( SI32 VillageUnique )
	{
		siVillageUnique = VillageUnique;
	}
};

class cltVillageResidentsList
{
public:
	cltSimplePerson	clPerson;
	SI32			siLevel;
	SI32			siMilitaryMerit;

	cltSimpleRank	clSimpleRank;
		
	TCHAR			strAccountID[ MAX_PLAYER_NAME ];

	void Set( cltVillageResidentsList *pclVillageResidentsObj )
	{
		memcpy( this, pclVillageResidentsObj, sizeof( cltVillageResidentsList ) );
	}

	void Set( cltSimplePerson *pSimplePerson, SI32 Level, SI32 MilitaryMerit, cltSimpleRank *pclSimpleRank, TCHAR *AccountID )
	{
		clPerson.Set( pSimplePerson );
		siLevel = Level;
		siMilitaryMerit = MilitaryMerit;
		
		clSimpleRank.Set( pclSimpleRank );

		memcpy( strAccountID, AccountID, MAX_PLAYER_NAME - 1 );
		strAccountID[ MAX_PLAYER_NAME - 1 ] = '\0';
	}
};

class cltGameMsgResponse_ResidentsList
{
public:
	UI32					uiCount;
	SI32					siVillageUnique;
		
	cltVillageResidentsList	clResident[ MAX_RESIDENT_NUMBER ];

	cltGameMsgResponse_ResidentsList()
	{
		memset( this, 0, sizeof( cltGameMsgResponse_ResidentsList ) );
	}

	void Set( SI32 VillageUnique )
	{
		siVillageUnique = VillageUnique;
	}

	void Add( cltVillageResidentsList *pclVillageResidentsObj )
	{
		if ( uiCount >= MAX_RESIDENT_NUMBER )
		{
			return;
		}

		clResident[ uiCount ++ ].Set( pclVillageResidentsObj );
	}
	
	void Add( cltSimplePerson *pSimplePerson, SI32 Level, SI32 MilitaryMerit, cltSimpleRank *pclSimpleRank, TCHAR *AccountID )
	{
		if ( uiCount >= MAX_RESIDENT_NUMBER )
		{
			return;
		}

		clResident[ uiCount ++ ].Set( pSimplePerson, Level, MilitaryMerit, pclSimpleRank, AccountID );
	}

	SI32 GetTotalSize()
	{
		return sizeof( UI32 ) + sizeof( SI32 ) + sizeof( cltVillageResidentsList ) * ( uiCount );
	}	
};

class cltGameMsgResponse_ResidentLogin
{
public:	
	SI32					siVillageUnique;
	TCHAR					strCharName[ MAX_PLAYER_NAME ];
	
	cltGameMsgResponse_ResidentLogin( SI32 VillageUnique, TCHAR *CharName )
	{
		siVillageUnique = VillageUnique;
		memcpy ( strCharName, CharName, MAX_PLAYER_NAME );
		strCharName[ MAX_PLAYER_NAME - 1 ] = NULL;
	}
};

class cltGameMsgRequest_SetVillageMark
{
public:
	SI32		siVillageMark;

	cltGameMsgRequest_SetVillageMark( SI32 villagemark )
	{
		siVillageMark = villagemark;
	}

};

class cltGameMsgResponse_SetVillageMark
{
public:

	SI32		siVillageUnique;
	SI32		siVillageMark;

	cltGameMsgResponse_SetVillageMark( SI32 villageunique, SI32 villagemark )
	{
		siVillageUnique = villageunique;
		siVillageMark = villagemark;
	}
};


class cltGameMsgRequest_SetVillageMap
{
public:
	enum {
		MAPTYPE_CITYHALL = 	1			, // 대행수
		MAPTYPE_BANK					, // 전장행수 
		MAPTYPE_STOCK					, // 여각행수 
		MAPTYPE_MARKET					, // 객주행수
		MAPTYPE_HOUSE					, // 시전행수
		MAPTYPE_HUNT					, // 수렵장행수 
		MAPTYPE_POSTOFFICE				, // 우체국행수
		MAPTYPE_PORT					, // 포구행수
		MAPTYPE_HORSEMARKET				, // 사복시행수
		MAPTYPE_REALESTATEMARKET		, // 거간행수
		MAPTYPE_SUMMONMARKET			, // 장예원행수
		MAPTYPE_LAND					, // 농장행수  
		MAPTYPE_FEAST					, // 내자시행수
		MAPTYPE_SUMMONHEROMARKET		, // 비변사행수
		MAPTYPE_MINE					, // 광산행수
		MAPTYPE_GUILD					, // 상단행수 	
	};

	SI32		siVillageMapUnique;
	SI08		siMapType;

	cltGameMsgRequest_SetVillageMap( SI32 mapunique, SI08 maptype)
	{
		siVillageMapUnique = mapunique;
		siMapType = maptype;
	}

};

class cltGameMsgResponse_SetVillageMap
{
public:

	SI32		siVillageUnique;
	SI32		siVillageMapUnique;

	cltGameMsgResponse_SetVillageMap( SI32 villageunique, SI32 mapunique )
	{
		siVillageUnique = villageunique;
		siVillageMapUnique = mapunique;
	}
};

class cltGameMsgResponse_SetVillageUpgrade
{
public:
	SI32		siVillageUnique[10] ;

	cltGameMsgResponse_SetVillageUpgrade(SI32 * villageunique)
	{
		for ( SI32 i = 0 ; i < 10 ; i ++ )
		{
			siVillageUnique[i] = villageunique[i];
		}
	}
};


//[진성] 마을에 점수를 변경하는 패킷. C->S => 2008-4-3
class cltGameMsgRequest_AddVillageScore
{
public:
	SI32		m_siAddVillageScore;

	cltGameMsgRequest_AddVillageScore(SI32 _siAddVillageScore)
	{
		m_siAddVillageScore = _siAddVillageScore;
	}

};

//[진성] 마을에 점수를 변경하는 패킷. S->C => 2008-4-3
class cltGameMsgResponse_AddVillageScore
{
public:

	SI32		m_siVillageUnique;
	SI32		m_siAddVillageScore;

	cltGameMsgResponse_AddVillageScore( SI32 _siVillageUnique, SI32 _siAddVillageScore )
	{
		m_siVillageUnique   = _siVillageUnique;
		m_siAddVillageScore = _siAddVillageScore;
	}
};

//[지연] 마을 수렵제 이벤트(몬스터 어택/라티우스(불가사리/키메라))의 보상내역처리
class cltGameMsgResponse_Event_MonsterAttack_Reward
{
public:
	SI32		m_siCharExp;
	SI32		m_siFameExp;
	SI32		m_siRewardItemUnique;
	SI32		m_siRewardItemNum;

	cltGameMsgResponse_Event_MonsterAttack_Reward(SI32 charexp, SI32 fameexp, SI32 rewarditemunique, SI32 rewarditemnum)
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Event_MonsterAttack_Reward) );

		m_siCharExp = charexp;
		m_siFameExp = fameexp;
		m_siRewardItemUnique = rewarditemunique;
		m_siRewardItemNum = rewarditemnum;
	}
};


#endif