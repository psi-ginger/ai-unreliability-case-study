#ifndef _DBMSGEMPIRE_H
#define _DBMSGEMPIRE_H

#include "DBMsg.h"
#include "../../GoonZuWorld/Server/Empire/EmpireManager.h"

//***************************************************************************************************
//
// Class Name 		: sDBRequest_CreateEmpire
//
// Last Modified 	: 2007 / 10 / 15 
// Created		 	: 김광명
//
// Function			: [요청]제국을 만든다.
//
//*************************************************************************************************** 
class sDBRequest_CreateEmpire
{
public:

	sPacketHeader	packetHeader;
	
	/// 제국을 만드는 사람
	SI32			siCharID;
	/// 제국에서 맹주가 될 마을
	SI32			siVillageUnique;
	/// 제국 만들때 필요한 금액
	SI64			siNeedMoney;

	sDBRequest_CreateEmpire( SI32 CharID, SI32 VillageUnique, SI64 NeedMoney )
	{
		packetHeader.usSize = sizeof( sDBRequest_CreateEmpire );
		packetHeader.usCmd = DBMSG_REQUEST_CREATEEMPIRE;

		siCharID = CharID;
		siVillageUnique = VillageUnique;
		siNeedMoney = NeedMoney;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_CreateEmpire
//
// Last Modified 	: 2007 / 10 / 15 
// Created		 	: 김광명
//
// Function			: [응답]제국을 만든다.
//
//*************************************************************************************************** 
class sDBResponse_CreateEmpire
{
public:
	sPacketHeader	packetHeader;

	SI32 siResult;
	SI32 siCharID;
	/// 만들어진 제국 유니크
	SI32 siEmpireUnique;
	/// 제국이 만들어진 후에 맹주가 된 마을
	SI32			siVillageUnique;
	/// 제국을 만들고 남은 마을의 이익금
	SI64 siLeftProfitMoney;			

	sDBResponse_CreateEmpire()
	{
		ZeroMemory( this, sizeof( sDBResponse_CreateEmpire ) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_DestroyEmpire
//
// Last Modified 	: 2007 / 10 / 15 
// Created		 	: 김광명
//
// Function			: [요청]제국이 해체된다( 시스템적으로 )
//
//*************************************************************************************************** 
class sDBRequest_DestroyEmpire
{
public:

	sPacketHeader	packetHeader;

	/// 해체될 제국 유니크
	SI32			siEmpireUnique;

	sDBRequest_DestroyEmpire( SI32 EmpireUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_DestroyEmpire );
		packetHeader.usCmd = DBMSG_REQUEST_DESTROYEMPIRE;

		siEmpireUnique = EmpireUnique;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_DestroyEmpire
//
// Last Modified 	: 2007 / 10 / 15 
// Created		 	: 김광명
//
// Function			: [응답]제국이 해체된다( 시스템적으로 )
//
//*************************************************************************************************** 
class sDBResponse_DestroyEmpire
{
public:
	sPacketHeader	packetHeader;

	SI32 siResult;
	/// 해체된 제국에 소속되어 있던 마을 유니크
	SI32 siVillageUnique[MAX_EMPIRE_IN_VILLAGE];
	/// 해체된 제국에 소속되어 있던 길드 유니크
	SI32 siGuildUnique[MAX_EMPIRE_IN_GUILD];			

	sDBResponse_DestroyEmpire()
	{
		ZeroMemory( this, sizeof( sDBResponse_DestroyEmpire ) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_JoinEmpire
//
// Last Modified 	: 2007 / 10 / 16 
// Created		 	: 김광명
//
// Function			: [요청]제국에 가입 신청을 한다.
//
//*************************************************************************************************** 
class sDBRequest_JoinEmpire
{
public:

	sPacketHeader	packetHeader;

	enum { VILLAGE = 0, GUILD };
	/// 제국 가입을 요청하는 사람
	SI32			siCharID;
	/// 제국 가입을 요청받는 사람
	SI32			siOtherCharID;
	/// 마을로 제국 가입하는지, 길드로 제국 가입하는지
	SI32			siJoinType;
	/// 가입할 제국의 유니크
	SI32			siEmpireUnique;
	/// 제국에 가입할 마을의 유니크
	SI32			siVillageUnique;
	/// 제국에 가입할 길드의 유니크
	SI32			siGuildUnique;

	sDBRequest_JoinEmpire( SI32 CharID, SI32 OtherCharID, SI32 JoinType, 
		                   SI32 EmpireUnique, SI32 VillageUnique, SI32 GuildUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_JoinEmpire );
		packetHeader.usCmd = DBMSG_REQUEST_JOINEMPIRE;

		siCharID = CharID;
		siOtherCharID = OtherCharID;
		siJoinType = JoinType;
		siEmpireUnique = EmpireUnique;
		siVillageUnique = VillageUnique;
		siGuildUnique = GuildUnique;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_JoinEmpire
//
// Last Modified 	: 2007 / 10 / 16 
// Created		 	: 김광명
//
// Function			: [응답]제국 맹주한테서 가입 신청을 받는다
//
//*************************************************************************************************** 
class sDBResponse_JoinEmpire
{
public:
	sPacketHeader	packetHeader;

	SI32 siResult;
	/// 제국 가입을 요청하는 사람
	SI32			siCharID;
	/// 제국 가입을 요청받는 사람
	SI32			siOtherCharID;
	/// 가입된 제국의 유니크
	SI32			siEmpireUnique;
	/// 마을로 제국 가입하는지, 길드로 제국 가입하는지
	SI32			siJoinType;
	/// 제국에 가입할 마을의 유니크
	SI32			siVillageUnique;
	/// 제국에 가입할 길드의 유니크
	SI32			siGuildUnique;
	
	sDBResponse_JoinEmpire()
	{
		ZeroMemory( this, sizeof( sDBResponse_JoinEmpire ) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_SecedeEmpire
//
// Last Modified 	: 2007 / 10 / 16 
// Created		 	: 김광명
//
// Function			: [요청]제국에서 탈퇴한다
//
//*************************************************************************************************** 
class sDBRequest_SecedeEmpire
{
public:

	sPacketHeader	packetHeader;

	enum { VILLAGE = 0, GUILD };
	/// 제국 탈퇴를 요청하는 사람
	SI32			siCharID;
	/// 탈퇴를 맹주가 하는지(true), 아니면 제국에 소속된 대행수나 길드마스터가 하는지(false)
	bool			bSecedeEmpireByEmperor; 
	/// 마을로 제국 탈퇴하는지, 길드로 제국 탈퇴하는지
	SI32			siSecedeType;
	/// 탈퇴할 제국의 유니크
	SI32			siEmpireUnique;
	/// 제국에서 탈퇴할 마을의 유니크
	SI32			siVillageUnique;
	/// 제국에서 탈퇴할 길드의 유니크
	SI32			siGuildUnique;

	sDBRequest_SecedeEmpire( SI32 CharID, bool SecedeEmpireByEmperor, SI32 SecedeType, 
		SI32 EmpireUnique, SI32 VillageUnique, SI32 GuildUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_SecedeEmpire );
		packetHeader.usCmd = DBMSG_REQUEST_SECEDEEMPIRE;

		siCharID = CharID;
		bSecedeEmpireByEmperor = SecedeEmpireByEmperor;
		siSecedeType = SecedeType;
		siEmpireUnique = EmpireUnique;
		siVillageUnique = VillageUnique;
		siGuildUnique = GuildUnique;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_SecedeEmpire
//
// Last Modified 	: 2007 / 10 / 16 
// Created		 	: 김광명
//
// Function			: [응답]제국에서 탈퇴한다
//
//*************************************************************************************************** 
class sDBResponse_SecedeEmpire
{
public:
	sPacketHeader	packetHeader;

	SI32 siResult;
	/// 제국 탈퇴를 요청하는 사람
	SI32			siCharID;
	/// 탈퇴를 맹주가 하는지(true), 아니면 제국에 소속된 대행수나 길드마스터가 하는지(false)
	bool			bSecedeEmpireByEmperor; 
	/// 마을로 제국 탈퇴하는지, 길드로 제국 탈퇴하는지
	SI32			siSecedeType;
	/// 탈퇴하는 제국 유니크
	SI32			siEmpireUnique;
	/// 제국에서 탈퇴한 마을의 유니크
	SI32			siVillageUnique;
	/// 제국에서 탈퇴한 길드의 유니크
	SI32			siGuildUnique;

	sDBResponse_SecedeEmpire()
	{
		ZeroMemory( this, sizeof( sDBResponse_SecedeEmpire ) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_AllEmpireInfo
//
// Last Modified 	: 2007 / 10 / 15 
// Created		 	: 김광명
//
// Function			: [요청] DB에 저장된 제국 정보를 요청한다.
//
//*************************************************************************************************** 
class sDBRequest_AllEmpireInfo
{
public:

	sPacketHeader	packetHeader;

	sDBRequest_AllEmpireInfo()
	{
		packetHeader.usSize = sizeof( sDBRequest_AllEmpireInfo );
		packetHeader.usCmd = DBMSG_REQUEST_ALLEMPIREINFO;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_AllEmpireInfo
//
// Last Modified 	: 2007 / 10 / 15 
// Created		 	: 김광명
//
// Function			: [응답] DB에 저장된 제국 정보를 요청한다.
//
//*************************************************************************************************** 
class sDBResponse_AllEmpireInfo
{
public:
	sPacketHeader	packetHeader;

	CEmpireUnitInfo clEmpireInfo[MAX_EMPIRENUMBER];

	sDBResponse_AllEmpireInfo()
	{
		ZeroMemory( this, sizeof( sDBResponse_AllEmpireInfo ) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_SetEmpireMoney
//
// Last Modified 	: 2007 / 10 / 26 
// Created		 	: 김광명
//
// Function			: [요청] 제국 자본금을 설정한다.( 더하거나 뺀다 )
//
//*************************************************************************************************** 
class sDBRequest_SetEmpireMoney
{
public:

	sPacketHeader	packetHeader;

	enum { USER = 0, SYSTEM };
	/// 주체가 유저인지, 시스템인지
	SI16 siType;
	/// 제국 자본금을 넣는 사람
	SI32 siCharID;
	/// 제국 자본금이 변화될 제국 유니크
	SI32 siEmpireUnique;
	/// 어디 마을에서 제국 자본금을 넣는지
	SI32 siVillageUnique;
	/// 더하거나 뺄 금액( 양수 또는 음수 )
	SI64 siAddMoney;

	sDBRequest_SetEmpireMoney( SI16 Type, SI32 CharID, SI32 EmpireUnique, SI32 VillageUnique, SI64 AddMoney )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetEmpireMoney );
		packetHeader.usCmd = DBMSG_REQUEST_SETEMPIREMONEY;

		siType = Type; 
		siCharID = CharID; 
		siEmpireUnique = EmpireUnique; 
		siVillageUnique = VillageUnique; 
		siAddMoney = AddMoney; 
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_SetEmpireMoney
//
// Last Modified 	: 2007 / 10 / 26
// Created		 	: 김광명
//
// Function			: [응답] 제국 자본금을 설정한다.( 더하거나 뺀다 )
//
//*************************************************************************************************** 
class sDBResponse_SetEmpireMoney
{
public:
	sPacketHeader	packetHeader;

	/// 리턴값
	SI32	siResult;
	/// 제국 자본금을 넣는 사람
	SI32	siCharID;
	/// 제국 자본금이 변경된 제국 유니크
	SI32	siEmpireUnique;
	/// 제국 자본금을 넣은 마을
	SI32	siVillageUnique;
	/// 바뀐 제국 자본금
	SI64	siLeftEmpireMoney;
	/// 바뀐 마을 이익금
	SI64	siLeftVillageProfitMoney;
	/// 제국 자본금을 넣은 마을에서 이제까지 제국에 납부한 금액
	SI64	siInputTotalEmpireMoney;
	///	제국 자본금 넣은 돈
	SI64	siAddMoney;

	sDBResponse_SetEmpireMoney()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetEmpireMoney ) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_ChangeEmperor
//
// Last Modified 	: 2007 / 10 / 26 
// Created		 	: 김광명
//
// Function			: [요청] 제국 맹주가 바뀐다( 맹주, 마을이 바뀐다 )
//
//*************************************************************************************************** 
class sDBRequest_ChangeEmperor
{
public:

	sPacketHeader	packetHeader;

	/// 제국 맹주가 될 사람
	SI32	siCharID;
	/// 새로운 맹주 PersonID
	SI32	siNewEmperorPersonID;
	/// 맹주가 바뀔 제국 유니크
	SI32	siEmpireUnique;
	/// 이전 맹주 마을 유니크
	SI32	siOldEmperorVillageUnique;
	/// 새로운 맹주 마을 유니크
	SI32	siNewEmperorVillageUnique;
	
	sDBRequest_ChangeEmperor( SI32 CharID, SI32 NewEmperorPersonID, 
		                      SI32 EmpireUnique, SI32 NewEmperorVillageUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_ChangeEmperor );
		packetHeader.usCmd = DBMSG_REQUEST_CHANGEEMPEROR;

		siCharID = CharID; 
		siNewEmperorPersonID = NewEmperorPersonID; 
		siEmpireUnique = EmpireUnique; 
		siNewEmperorVillageUnique = NewEmperorVillageUnique; 
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_ChangeEmperor
//
// Last Modified 	: 2007 / 10 / 26
// Created		 	: 김광명
//
// Function			: [응답] 제국 맹주가 바뀐다( 맹주, 마을이 바뀐다 )
//
//*************************************************************************************************** 
class sDBResponse_ChangeEmperor
{
public:
	sPacketHeader	packetHeader;

	/// 리턴값
	SI32	siResult;
	/// 제국 맹주가 될 사람
	SI32	siCharID;
	/// 이전 맹주 PersonID
	SI32	siOldEmperorPersonID;
	/// 새로운 맹주 PersonID
	SI32	siNewEmperorPersonID;
	/// 맹주가 바뀔 제국 유니크
	SI32	siEmpireUnique;
	/// 이전 맹주 마을 유니크
	SI32	siOldEmperorVillageUnique;
	/// 새로운 맹주 마을 유니크
	SI32	siNewEmperorVillageUnique;
	/// 남은 제국 자본금
	SI64	siLeftEmpireMoney;
	
	sDBResponse_ChangeEmperor()
	{
		ZeroMemory( this, sizeof( sDBResponse_ChangeEmperor ) );
	}
};

#endif