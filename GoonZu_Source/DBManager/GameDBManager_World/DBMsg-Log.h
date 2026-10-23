#ifndef _DBMSGLOG_H
#define _DBMSGLOG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

//----------------------------------------------
// Common
//----------------------------------------------
#include "../../GoonZuWorld/Common/CommonHeader.h"
#include "../../GoonZuWorld/Common/PersonInfo.h"

#include "../../GoonZuWorld/Common/Char/CharCommon/Char-Common.h"
#include "../../GoonZuWorld/Common/Char/ParametaBox/ParametaBox.h"
#include "../../GoonZuWorld/Common/Item/ItemCommon/ItemUnit.h"
#include "../../GoonZuWorld/Server/Rank/Rank.h"
#include "../../GoonZuWorld/Common/Skill/Skill.h"
#include "../../GoonZuWorld/Common/Quest/Quest.h"
#include "../../GoonZuWorld/Common/Horse/Horse.h"
#include "../../GoonZuWorld/Common/Char/CharCommon/Char-EconomyInfo.h"
#include "../../GoonZuWorld/Common/SystemReward/SystemReward.h"


#include "../../GoonZuWorld/CommonLogic/Bank/Bank.h"
#include "../../GoonZuWorld/CommonLogic/Guild/Guild.h"

//--------------------------------------------------------------------------------------
//Person의 BasicInfo 정보를 요청하는 부분
//--------------------------------------------------------------------------------------
class sDBRequest_BasicInfoByName
{
public:
	sPacketHeader		packetHeader;
	SI32				siClientID;

	TCHAR				szCharname[MAX_PLAYER_NAME];
	SI32				siPAGENum;	

	sDBRequest_BasicInfoByName()
	{
		ZeroMemory(this, sizeof(sDBRequest_BasicInfoByName));
	}

	sDBRequest_BasicInfoByName(SI32 clientid, TCHAR* charname, SI32 pagenum)
	{
		packetHeader.usSize = sizeof( sDBRequest_BasicInfoByName );
		packetHeader.usCmd = DBMSG_REQUEST_GMTOOL_CHARBASICINFO;

		siClientID	= clientid;
		szCharname[0] = '\0';

		if(charname)		strcpy(szCharname, charname);
		siPAGENum = pagenum;

	}
};

class sDBResponse_BasicInfoByName
{
public:
	sPacketHeader		packetHeader;
	SI32				siClientID;

	TCHAR				Charname[MAX_PLAYER_NAME];
	SI32				siPAGENum;
	//케릭터 기본 정보.
	cltPBasicInfo		BasicInfo;			// Person의 기본 정보. 
	cltPIPInfo			clIp;				//케릭터의 각종 정보
	cltHealthInfo		clHealthInfo;		//건강정보
	cltPStockInfo		clStockInfo;
	cltBankAccount		clBankaccount;
    
	sDBResponse_BasicInfoByName()
	{
		ZeroMemory(this, sizeof(sDBResponse_BasicInfoByName));
	}
};


//--------------------------------------------------------------------------------------
//Person의 스킬정보를 요청하는 부분
//--------------------------------------------------------------------------------------
class sDBRequest_SkillByName
{
public:
	sPacketHeader		packetHeader;
	SI32				siClientID;

	TCHAR				szCharname[MAX_PLAYER_NAME];
	SI32				siPAGENum;	

	sDBRequest_SkillByName()
	{
		ZeroMemory(this, sizeof(sDBRequest_SkillByName));
	}

	sDBRequest_SkillByName(SI32 clientid, TCHAR* charname, SI32 pagenum)
	{
		packetHeader.usSize = sizeof( sDBRequest_SkillByName );
		packetHeader.usCmd = DBMSG_REQUEST_GMTOOL_SKILLINFO;
		siClientID	= clientid;
		szCharname[0] = '\0';
		if(charname)		strcpy(szCharname, charname);
		siPAGENum= pagenum;
	}
};

class sDBResponse_SkillByName
{
public:
	sPacketHeader		packetHeader;
	SI32				siPersonID;
	SI32				siClientID;
	TCHAR				szCharname[MAX_PLAYER_NAME];
	SI32				siPAGENum;

	cltPSkillInfo			SkillInfo;			// Person의 기술 정보.
	cltPItemInfo			clItemInfo;			// Person의 아이템 정보.
	

	sDBResponse_SkillByName()
	{
		ZeroMemory(this, sizeof(sDBResponse_SkillByName));
	}
};

//--------------------------------------------------------------------------------------
//Person의 동물 정보를 요청하는 부분
//--------------------------------------------------------------------------------------
class sDBRequest_AnimalByName
{
public:
	sPacketHeader		packetHeader;
	SI32				siClientID;
	TCHAR				szCharname[MAX_PLAYER_NAME];
	SI32				siPAGENum;	

	sDBRequest_AnimalByName()
	{
		ZeroMemory(this, sizeof(sDBRequest_AnimalByName));
	}

	sDBRequest_AnimalByName(SI32 clientid, TCHAR* charname, SI32 pagenum)
	{
		packetHeader.usSize = sizeof( sDBRequest_AnimalByName );
		packetHeader.usCmd = DBMSG_REQUEST_GMTOOL_ANIMALLINFO;
		siClientID	= clientid;
		szCharname[0] = '\0';
		if(charname)		strcpy(szCharname, charname);
		siPAGENum= pagenum;

	}
};

class sDBResponse_AnimalByName
{
public:
	sPacketHeader		packetHeader;
	SI32				siPersonID;
	SI32				siClientID;
	TCHAR				Charname[MAX_PLAYER_NAME];
	SI32				siPAGENum;

	cltPersonHorseInfo		clHorseInfo;		// 말 정보.

	sDBResponse_AnimalByName()
	{
		ZeroMemory(this, sizeof(sDBResponse_AnimalByName));
	}
};

//--------------------------------------------------------------------------------------
//Person의 인벤토리 정보를 요청하는 부분
//--------------------------------------------------------------------------------------
class sDBRequest_InventoryByName
{
public:
	sPacketHeader		packetHeader;
	SI32				siClientID;
	TCHAR				szCharname[MAX_PLAYER_NAME];
	SI32				siPAGENum;	

	sDBRequest_InventoryByName()
	{
		ZeroMemory(this, sizeof(sDBRequest_InventoryByName));
	}

	sDBRequest_InventoryByName(SI32 clientid,TCHAR* charname, SI32 pagenum)
	{
		packetHeader.usSize = sizeof( sDBRequest_InventoryByName );
		packetHeader.usCmd = DBMSG_REQUEST_GMTOOL_INVENTORYINFO;
		siClientID	= clientid;
		szCharname[0] = '\0';
		if(charname)		strcpy(szCharname, charname);

		siPAGENum= pagenum;

	}
};

class sDBResponse_InventoryByName
{
public:
	sPacketHeader		packetHeader;
	SI32				siPersonID;
	SI32				siClientID;
	TCHAR				Charname[MAX_PLAYER_NAME];
	SI32				siPAGENum;

	cltPItemInfo			clItemInfo;			// Person의 아이템 정보.

	sDBResponse_InventoryByName()
	{
		ZeroMemory(this, sizeof(sDBResponse_InventoryByName));
	}
};

//--------------------------------------------------------------------------------------
//Person의 부동산 정보를 요청하는 부분
//--------------------------------------------------------------------------------------
class sDBRequest_RealestateByNAme
{
public:
	sPacketHeader		packetHeader;
	TCHAR				szCharname[MAX_PLAYER_NAME];
	SI32				siClientID;
	SI32				siPAGENum;	

	sDBRequest_RealestateByNAme()
	{
		ZeroMemory(this, sizeof(sDBRequest_RealestateByNAme));
	}

	sDBRequest_RealestateByNAme(SI32 clientid,TCHAR* charname, SI32 pagenum)
	{
		packetHeader.usSize = sizeof( sDBRequest_RealestateByNAme );
		packetHeader.usCmd = DBMSG_REQUEST_GMTOOL_REALESTATEINFO;
		siClientID	= clientid;
		szCharname[0] = '\0';
		if(charname)		strcpy(szCharname, charname);
		siPAGENum= pagenum;

	}
};

class sDBResponse_RealestateByName
{
public:
	sPacketHeader		packetHeader;
	SI32				siPersonID;
	TCHAR				Charname[MAX_PLAYER_NAME];
	SI32				siClientID;
	SI32				siPAGENum;

	cltCharRealEstate		clRealEstateInfo;	// 부동산 정보(주택...) 

	sDBResponse_RealestateByName()
	{
		ZeroMemory(this, sizeof(sDBResponse_RealestateByName));
	}
};

//--------------------------------------------------------------------------------------
//Person의 소환수 정보를 요청하는 부분
//--------------------------------------------------------------------------------------
class sDBRequest_SummonByName
{
public:
	sPacketHeader		packetHeader;
	TCHAR				szCharname[MAX_PLAYER_NAME];
	SI32				siClientID;
	SI32				siPAGENum;	

	sDBRequest_SummonByName()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonByName));
	}

	sDBRequest_SummonByName(SI32 clientid,TCHAR* charname, SI32 pagenum)
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonByName );
		packetHeader.usCmd = DBMSG_REQUEST_GMTOOL_SUMMONINFO;
		siClientID	= clientid;
		szCharname[0] = '\0';
		if(charname)		strcpy(szCharname, charname);
		siPAGENum= pagenum;

	}
};

class sDBResponse_SummonByName
{
public:
	sPacketHeader		packetHeader;
	SI32				siPersonID;
	TCHAR				Charname[MAX_PLAYER_NAME];
	SI32				siClientID;
	SI32				siPAGENum;

	cltSummonInfo			clSummonInfo;		// 소환수 정보 

	sDBResponse_SummonByName()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonByName));
	}
};

////--------------------------------------------------------------------------------------
////Person의 거래 정보를 요청하는 부분
////--------------------------------------------------------------------------------------
//class sDBRequest_TradingByIDNum
//{
//public:
//	sPacketHeader		packetHeader;
//	UI16				usCharID;
//	SI32				siIDNum;	
//
//	sDBRequest_TradingByIDNum()
//	{
//		ZeroMemory(this, sizeof(sDBRequest_TradingByIDNum));
//	}
//
//	sDBRequest_TradingByIDNum(SI32 id, SI32 idnum)
//	{
//		packetHeader.usSize = sizeof( sDBRequest_TradingByIDNum );
//		packetHeader.usCmd = DBMSG_REQUEST_GMTOOL_TRADINGINFO;
//		usCharID			= id;
//		siIDNum	= idnum;
//
//	}
//};
//
//class sDBResponse_TradingByIDNum
//{
//public:
//	sPacketHeader		packetHeader;
//	UI16				usCharID;
//	SI32				siPersonID;
//	//케릭터 기본 정보.
//	cltPBasicInfo		BasicInfo;			// Person의 기본 정보. 
//	cltInvestInfo		InvestInfo;			// Person의 주식 정보.
//
//	sDBResponse_TradingByIDNum()
//	{
//		ZeroMemory(this, sizeof(sDBResponse_TradingByIDNum));
//	}
//};
#endif