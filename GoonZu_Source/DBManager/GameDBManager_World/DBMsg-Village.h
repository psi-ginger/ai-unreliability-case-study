#ifndef _DBMSGVILLAGE_H
#define _DBMSGVILLAGE_H

#include "DBMsg.h"

//***************************************************************************************************
//
// Class Name 		: sDBRequest_VillageEventSuccess
//
// Last Modified 	: 2005 / 12 / 12 
// Created		 	: 김광명
//
// Function			: 마을 이벤트가 성공했을시 DB에 저장된 마을 이벤트 횟수를 증가 시킨다
//
//*************************************************************************************************** 
class sDBRequest_VillageEventSuccess
{
public:
	sPacketHeader	packetHeader;

	SI32			siVillageUnique;
	

	sDBRequest_VillageEventSuccess( SI32 VillageUnique  )
	{
		packetHeader.usSize = sizeof( sDBRequest_VillageEventSuccess );
		packetHeader.usCmd = DBMSG_REQUEST_VILLAGEEVENTSUCCESS;

		siVillageUnique = VillageUnique;
		
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_VillageWarKillNumber
//
// Last Modified 	: 2005 / 12 / 13 
// Created		 	: 김광명
//
// Function			: 전쟁이 끝난후 마을 전공수를 DB로 보낸다
//
//*************************************************************************************************** 
class sDBRequest_VillageWarKillNumber
{
public:
	sPacketHeader	packetHeader;

	SI32			siVillageUnique[MAX_VILLAGE_NUMBER];
	SI32			siVillageWarKillNumber[MAX_VILLAGE_NUMBER];

	sDBRequest_VillageWarKillNumber( SI32* VillageUnique, SI32* VillageWarKillNumber )
	{
		packetHeader.usSize = sizeof( sDBRequest_VillageWarKillNumber );
		packetHeader.usCmd = DBMSG_REQUEST_VILLAGEWARKILLNUMBER;

		SI32 i;
		for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if( VillageUnique[i] > 0 )
			{
				siVillageUnique[i]			= VillageUnique[i];
				siVillageWarKillNumber[i]	= VillageWarKillNumber[i];
			}
			else
			{
				siVillageUnique[i] = 0;
				siVillageWarKillNumber[i] = 0;
			}
		}
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_VillageResidentsVisitorsInfo
//
// Last Modified 	: 2005 / 12 / 13 
// Created		 	: 김광명
//
// Function			: [요청]전체 주민수와 마을 방문자수를 DB로 보낸다
//
//*************************************************************************************************** 
class sDBRequest_VillageResidentsVisitorsInfo
{
public:
	sPacketHeader	packetHeader;

	sDBRequest_VillageResidentsVisitorsInfo()
	{
		packetHeader.usSize = sizeof( sDBRequest_VillageResidentsVisitorsInfo );
		packetHeader.usCmd = DBMSG_REQUEST_VILLAGERESIDENTSVISITORSINFO;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_VillageResidentsVisitorsInfo
//
// Last Modified 	: 2006 / 03 / 09 
// Created		 	: 김광명
//
// Function			: [응답]전체 주민수와 마을 방문자수를 DB로 보낸다
//
//*************************************************************************************************** 
class sDBResponse_VillageResidentsVisitorsInfo
{
public:
	sPacketHeader	packetHeader;

	SI32 siResult;

	sDBResponse_VillageResidentsVisitorsInfo()
	{
		ZeroMemory( this, sizeof( sDBResponse_VillageResidentsVisitorsInfo ) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRquest_VillageJoinPlayTime
//
// Last Modified 	: 2005 / 12 / 14
// Created		 	: 김광명
//
// Function			: 유저의 마을 가입 후 플레이 시간을 DB로 보낸다!!
//
//***************************************************************************************************
class sDBRequest_VillageJoinPlayTime
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;					
	SI32				siVillageJoinPlayTime;
	
	// PCK : 가입된 마을 유니크 (로그용)
	SI32				siVillageUnique;

	sDBRequest_VillageJoinPlayTime( SI32 PersonId, SI32 VillageJoinPlayTime, SI32 VillageUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_VillageJoinPlayTime );
		packetHeader.usCmd = DBMSG_REQUEST_VILLAGEJOINPLAYTIME;

		siPersonID = PersonId;
		siVillageJoinPlayTime = VillageJoinPlayTime;
		siVillageUnique = VillageUnique;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRquest_VillageJoinLevelUp
//
// Last Modified 	: 2005 / 12 / 14
// Created		 	: 김광명
//
// Function			: 유저의 마을 가입 후 레벨업 횟수
//
//***************************************************************************************************
class sDBRequest_VillageJoinLevelUp
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;					
	
	// PCK : 레벨업 횟수, 마을 유니크 추가
	SI32				siLevelCount;
	SI32				siVillageUnique;

	sDBRequest_VillageJoinLevelUp( SI32 PersonId, SI32 LevelCount, SI32 VillageUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_VillageJoinLevelUp );
		packetHeader.usCmd = DBMSG_REQUEST_VILLAGEJOINLEVELUP;

		siPersonID = PersonId;
		siLevelCount = LevelCount;
		siVillageUnique = VillageUnique;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRquest_VillageJoinWarKillNumber
//
// Last Modified 	: 2005 / 12 / 14
// Created		 	: 김광명
//
// Function			: 마을 가입 후에 전공수를 보낸다
//
//***************************************************************************************************
class sDBRequest_VillageJoinWarKillNumber
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;					
	SI32				siVillageJoinWarKillNumber;

	sDBRequest_VillageJoinWarKillNumber( SI32 PersonId, SI32 VillageJoinWarKillNumber )
	{
		packetHeader.usSize = sizeof( sDBRequest_VillageJoinWarKillNumber );
		packetHeader.usCmd = DBMSG_REQUEST_VILLAGEJOINWARKILLNUMBER;

		siPersonID = PersonId;
		siVillageJoinWarKillNumber = VillageJoinWarKillNumber;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRquest_VillageJoinMakeRareItem
//
// Last Modified 	: 2005 / 12 / 14
// Created		 	: 김광명
//
// Function			: 마을 가입 후에 명품횟수를 계산한다
//
//***************************************************************************************************
class sDBRequest_VillageJoinMakeRareItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;					
	
	sDBRequest_VillageJoinMakeRareItem( SI32 PersonId )
	{
		packetHeader.usSize = sizeof( sDBRequest_VillageJoinMakeRareItem );
		packetHeader.usCmd = DBMSG_REQUEST_VILLAGEJOINMAKERAREITEM;

		siPersonID = PersonId;
	}
};


//***************************************************************************************************
//
// Class Name 		: sDBRequest_VillageJoinMakeItem
//
// Last Modified 	: 2006 / 03 / 06
// Created		 	: 김광명
//
// Function			: 마을 가입 후에 아이템 제조 경험치
//
//***************************************************************************************************
class sDBRequest_VillageJoinMakeItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;		
	SI32				siItemSkillExp;

	// PCK : 마을 유니크 추가
	SI32				siVillageUnique;

	sDBRequest_VillageJoinMakeItem( SI32 PersonId, SI32 ItemSkillExp, SI32 VillageUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_VillageJoinMakeItem );
		packetHeader.usCmd = DBMSG_REQUEST_VILLAGEJOINMAKEITEM;

		siPersonID = PersonId;
		siItemSkillExp = ItemSkillExp;
		siVillageUnique = VillageUnique;
	}
};

//[진성] 마을에 점수를 변경하는 패킷. S->DB => 2008-4-3
class sDBRequest_Add_Village_Score
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;	

	SI32				m_siVillageUnique;		// 마을 유니크.
	SI32				m_siAddVillageScore;    // 추가될 마을 점수.

	sDBRequest_Add_Village_Score(SI32 _siVillageUnique, SI32 _siAddVillageScore)
	{
		packetHeader.usSize = sizeof( sDBRequest_Add_Village_Score );
		packetHeader.usCmd  = DBMSG_REQUEST_ADD_VILLAGE_SCORE;
		
		m_siVillageUnique   = _siVillageUnique;
		m_siAddVillageScore = _siAddVillageScore;
	}

};

//[진성] 마을에 점수를 변경하는 패킷. DB->S => 2008-4-3
class sDBResponse_Add_Village_Score
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;
	SI32			m_siVillageUnique;		// 마을 유니크.
	SI32			m_siVillageScore;		// 현재 마을 점수.
	SI32			m_siAddVillageScore;	// 추가될 마을 점수. (+,-)

	sDBResponse_Add_Village_Score(void)
	{
		ZeroMemory(this, sizeof(sDBResponse_Add_Village_Score));
	}

	sDBResponse_Add_Village_Score(SI32 _siVillageUnique, SI32 _siVillageScore)
	{
		m_siVillageUnique   = _siVillageUnique;
		m_siVillageScore	= _siVillageScore;
	}
};

// PCK : 유저의 마을 가입 후 플레이 시간 (로그용 패킷)
class sDBReponse_VillageJoinPlayTime
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;					
	SI32				siVillageJoinPlayTime;
	SI32				siVillageUnique;

	sDBReponse_VillageJoinPlayTime()
	{
		ZeroMemory(this, sizeof(sDBReponse_VillageJoinPlayTime));
	}
};

// PCK : 유저의 마을 가입 후 레벨업 횟수 (로그용 패킷)
class sDBResponse_VillageJoinLevelUp
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;					
	SI32				siLevelCount;
	SI32				siVillageUnique;

	sDBResponse_VillageJoinLevelUp()
	{
		ZeroMemory(this, sizeof(sDBResponse_VillageJoinLevelUp));
	}
};

// PCK : 마을 가입후에 아이템 제조 경험치 (로그용 패킷)
class sDBResponse_VillageJoinMakeItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;		
	SI32				siItemSkillExp;
	SI32				siVillageUnique;

	sDBResponse_VillageJoinMakeItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_VillageJoinMakeItem));
	}
};
#endif