#ifndef _DBMSGSTRUCTURE_H
#define _DBMSGSTRUCTURE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\GoonZuWorld\CommonLogic\Cityhall\Cityhall.h"
#include "..\..\GoonZuWorld\CommonLogic\House\House.h"
#include "..\..\GoonZuWorld\CommonLogic\Bank\Bank.h"
#include "DBMsg.h"

#include "../../GoonZuWorld/Server/Event/ContributionEvent_Common/ContributionEvent_Common.h"


#define CHANGESTRMONEY_NORMAL					0
#define CHANGESTRMONEY_CITYHALL_PROFIT			1
#define CHANGESTRMONEY_NPCPAY					2	// NPC 에 급여를 지급한다.
#define CHANGESTRMONEY_EVENTPAY					3	// 마을 이벤트 비용
#define CHANGESTRMONEY_HUNT						4	// 수렵장 이용에 따른 요금 지불. 
#define CHANGESTRMONEY_VILLAGEMARK				5   // VillageMark변경
#define CHANGESTRMONEY_VILLAGEMAP				6	// VillageMap변경
#define CHANGESTRMONEY_INSTANCEPROFIT			7	// 인스턴스 던전 이익금
#define CHANGESTRMONEY_DESTORYSTR				8	// 건물 내구도 소모로 감소
#define CHANGESTRMONEY_VILLAGEWAR				9	// 마을 공성전 승리 혹은 패배로 인한 변동
#define CHANGESTRMONEY_DONATEMONEY				10	// 기부금으로 마을 이익금이 변한다

//---------------------------------------------
// 건물의 보유 자금을 증가 시키거나 감소 시킨다
//---------------------------------------------
class sDBRequest_ChangeStrMoney
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siReason;					// DB매니저에서는 이값을 받아서 Response에 그대로 대입해주면 된다 
	LPARAM				lParam;						// DB매니저에서는 이값을 받아서 Response에 그대로 대입해주면 된다 

	SI32				siVillageUnique;			// 마을 

	SI32				siStrType;					// 건물 종류
	GMONEY				siMoney;					// 건물 보유자금에서 더 하거나 뺄 금액 

	sDBRequest_ChangeStrMoney()
	{
		Init();
	}

	sDBRequest_ChangeStrMoney( SI32 reason, LPARAM lparam, SI32 villageUnique, SI32 strType, GMONEY money ) {

		Set( reason, lparam, villageUnique, strType, money );
	}

	void				Init()
	{
		ZeroMemory( this, sizeof( sDBRequest_ChangeStrMoney ) );
	}

	void				Set( SI32 reason, LPARAM lparam, SI32 villageUnique, SI32 strType, GMONEY money ) 
	{
		packetHeader.usSize = sizeof( sDBRequest_ChangeStrMoney );
		packetHeader.usCmd = DBMSG_REQUEST_CHANGESTRMONEY;

		siReason = reason;
		lParam = lparam;
		siVillageUnique = villageUnique;
		siStrType = strType;
		siMoney = money;
	}

};

class sDBResponse_ChangeStrMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;					// 성공:1, 실패:0, 보유자금부족:2

	SI32				siReason;
	LPARAM				lParam;

	SI32				siVillageUnique;

	SI32				siStrType;					// 건물 종류
	cltMoney			clMoney;					// 건물 보유자금 
	
	GMONEY				siChangeMoney;				// 증감 금액

	sDBResponse_ChangeStrMoney() {
		ZeroMemory( this, sizeof( sDBResponse_ChangeStrMoney ) );
	}
};

//---------------------------------------------
// 상단 건물의 이익금을 증가 시키거나 감소 시킨다
//---------------------------------------------
class sDBRequest_ChangeCityhallProfitMoney
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siReason;					// DB매니저에서는 이값을 받아서 Response에 그대로 대입해주면 된다 
	LPARAM				lParam;						// DB매니저에서는 이값을 받아서 Response에 그대로 대입해주면 된다 

	SI32				siVillageUnique;			// 마을 
	GMONEY				siMoney;					// 상단 이익금에서 더 하거나 뺄 금액 

	sDBRequest_ChangeCityhallProfitMoney()
	{
		ZeroMemory( this, sizeof( sDBRequest_ChangeCityhallProfitMoney ) );
	}

	sDBRequest_ChangeCityhallProfitMoney( SI32 reason, LPARAM lparam, SI32 villageunique, GMONEY money )
	{
		packetHeader.usSize = sizeof( sDBRequest_ChangeCityhallProfitMoney );
		packetHeader.usCmd = DBMSG_REQUEST_CHANGECITYHALLPROFITMONEY;

		siReason = reason;
		lParam = lparam;
		siVillageUnique = villageunique;
		siMoney = money;
	}

};

class sDBResponse_ChangeCityhallProfitMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;					// 성공:1, 실패:0, 보유자금부족:2

	SI32				siReason;
	LPARAM				lParam;

	SI32				siVillageUnique;
	cltMoney			clMoney;					// 상단 이익금
	
	GMONEY				siChangeMoney;				// 증감 금액

	sDBResponse_ChangeCityhallProfitMoney() {
		ZeroMemory( this, sizeof( sDBResponse_ChangeCityhallProfitMoney ) );
	}
};


//------------------------------------------
// 건물 정보를 설정할 것을 요청
//------------------------------------------
class sDBRequest_SetStrInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;	// 건물이 위치한 곳의 VillageUnique
	SI32				siStrType;			// 건물의 종류.

	//----------------------------
	// 맨 마지막에 놓여야 한다. 사이즈가 가변적이므로. 
	//----------------------------
	cltStrInfo			clStrInfo;			// 설정해야 할 건물의 정보. 

	sDBRequest_SetStrInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetStrInfo));
	}

	sDBRequest_SetStrInfo(SI32 id, SI32 villageunique, SI32 strtype, cltStrInfo* pclinfo)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetStrInfo );
		packetHeader.usCmd = DBMSG_REQUEST_SETSTRINFO;
		usCharID		= id;

		siVillageUnique	= villageunique;
		siStrType		= strtype;

		clStrInfo.Set(pclinfo);
	}
};


class sDBResponse_SetStrInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siVillageUnique;	// 건물이 위치한 마을의 VillageUnique

	//----------------------------
	// 맨 마지막에 놓여야 한다. 사이즈가 가변적이므로. 
	//----------------------------
	cltStrInfo			clStrInfo;			// 설정된 할 건물의 정보. 

	sDBResponse_SetStrInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetStrInfo));
	}
	// 이 메시지의 유효한 사이즈를 구한다. 
	SI32 GetTotalSize()
	{
		return sizeof(sDBResponse_SetStrInfo) - sizeof(cltStrInfo) + clStrInfo.GetTotalSize();
	}
	
};


//------------------------------------------
// 건물 정보를 보내줄 것을 요청
//------------------------------------------
class sDBRequest_GetStrInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;			// 건물이 위치한 곳의 VillageUnique
	SI32				siStrType;					// 건물의 타입. 

	sDBRequest_GetStrInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetStrInfo));
	}

	sDBRequest_GetStrInfo(SI32 id, SI32 villageunique, SI32 strtype)
	{
		packetHeader.usSize = sizeof( sDBRequest_GetStrInfo );
		packetHeader.usCmd = DBMSG_REQUEST_GETSTRINFO;
		usCharID		= id;

		siVillageUnique	= villageunique;
		siStrType		= strtype;
	}
};


class sDBResponse_GetStrInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siVillageUnique;	// 건물이 위치한 마을의 VillageUnique

	//----------------------------
	// 맨 마지막에 놓여야 한다. 사이즈가 가변적이므로. 
	//----------------------------
	cltStrInfo			clStrInfo;			// 건물의 정보. 

	sDBResponse_GetStrInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetStrInfo));
	}
	// 이 메시지의 유효한 사이즈를 구한다. 
	SI32 GetTotalSize()
	{
		return sizeof(sDBResponse_GetStrInfo) - sizeof(cltStrInfo) + clStrInfo.GetTotalSize();
	}
};

//------------------------------------------
// 세금 설정 요청 
//------------------------------------------
class sDBRequest_SetTax
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;			// 건물이 위치한 곳의 VillageUnique
	SI32				siStrType;					// 건물의 타입. 

	UI16				usYear;
	cltMoney			clDueTax;

	sDBRequest_SetTax()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetTax));
	}

	sDBRequest_SetTax(SI32 id, SI32 villageunique, SI32 strtype, UI16 year, cltMoney *pclMoney )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetTax );
		packetHeader.usCmd = DBMSG_REQUEST_SETTAX;
		usCharID		= id;

		siVillageUnique	= villageunique;
		siStrType		= strtype;

		usYear = year;
		clDueTax.Set( pclMoney );
	}
};

//------------------------------------------
// 세금 설정 응답
//------------------------------------------
class sDBResponse_SetTax
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;
	
	SI32				siVillageUnique;			// 건물이 위치한 곳의 VillageUnique
	SI32				siStrType;					// 건물의 타입. 

	UI16				usYear;
	cltMoney			clDueTax;
	
	sDBResponse_SetTax()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetTax));
	}

};

//------------------------------------------
// 세금 납부 요청 
//------------------------------------------
class sDBRequest_PayTax
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;			// 건물이 위치한 곳의 VillageUnique
	SI32				siStrType;					// 건물의 타입. 

	cltMoney			clPayTax;					// 납부 세금액
	GMONEY				siChiefPay;					// 행수 급여

	UI16				usYear;						// 세금 납부 년도
	cltDate				clDate;						// 최근 세금 납부일

	sDBRequest_PayTax()
	{
		ZeroMemory(this, sizeof(sDBRequest_PayTax));
	}

	sDBRequest_PayTax( SI32 id, SI32 villageunique, SI32 strtype, cltMoney *pclMoney, GMONEY sichiefpay, UI16 year, cltDate *pclDate )
	{
		packetHeader.usSize = sizeof( sDBRequest_PayTax );
		packetHeader.usCmd = DBMSG_REQUEST_PAYTAX;
		usCharID		= id;

		siVillageUnique	= villageunique;
		siStrType		= strtype;

		clPayTax.Set( pclMoney );		
		siChiefPay = sichiefpay;

		usYear			= year;
		clDate.Set( pclDate );
	}

};

//------------------------------------------
// 세금 납부 응답
//------------------------------------------
class sDBResponse_PayTax
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;			// 건물이 위치한 곳의 VillageUnique
	SI32				siStrType;					// 건물의 타입. 

	cltMoney			clPaidTax;					// 납부 세금액
	GMONEY				siChiefPay;					// 행수 급여 

	UI16				usYear;						// 납부 년도
	cltDate				clDate;						// 최근 세금 납부일
	
	cltMoney			clStructureMoney;			// 납부 후 건물의 보유자금
	cltMoney			clCityhallMoney;			// 납부 후 상단의 보유자금(수익금)
	GMONEY				siPersonMoney;				// 납부 후 행수 소지금

	sDBResponse_PayTax()
	{
		ZeroMemory(this, sizeof(sDBResponse_PayTax));
	}
	
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_AutoPayDividend
//
// Last Modified 	: 2006 / 02 / 16
// Created		 	: 김광명
//
// Function			: [요청]자동배당하기
//
//***************************************************************************************************
class sDBRequest_AutoPayDividend
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;	

	SI32				siVillageUnique;			// VillageUnique

	cltMoney			clStockDividend;			// 주주들에게 배당되는 금액
	cltMoney			clResidentDividend;			// 주민들에게 배당되는 금액
	cltMoney			clTaxlDividend;				// 세금으로 왕실 자금으로 들어가는 금액
	GMONEY				siChiefPay;					// 대행수 급여 
	GMONEY				siKingPay;					// 군주(왕) 급여
	SI32				siKingPayPersent;			// 군주(왕) 급여 비율 

	UI16				usYear;						// 배당년도
	cltDate				clDate;						// 최근 배당일

	sDBRequest_AutoPayDividend( SI32 id, SI32 villageUnique, cltMoney *pStockDividend, cltMoney *pResidentDividend,
		cltMoney *pTaxlDividend, GMONEY sichiefpay, GMONEY kingPay, SI32 kingPayPersent,
		UI16 year, cltDate *pclDate )
	{
		packetHeader.usSize = sizeof( sDBRequest_AutoPayDividend );
		packetHeader.usCmd = DBMSG_REQUEST_AUTOPAYDIVIDEND;
		usCharID		= id;

		siVillageUnique	= villageUnique;

		clStockDividend.Set( pStockDividend );
		clResidentDividend.Set( pResidentDividend );
		clTaxlDividend.Set( pTaxlDividend );
		siChiefPay = sichiefpay;
		siKingPay = kingPay;
		siKingPayPersent = kingPayPersent;

		usYear = year;
		clDate.Set( pclDate );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_AutoPayDividend
//
// Last Modified 	: 2006 / 02 / 16
// Created		 	: 김광명
//
// Function			: [응답]자동배당하기
//
//***************************************************************************************************
class sDBResponse_AutoPayDividend
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;									// 결과: 1성공 0실패  -1보유자금 부족 

	SI32				siVillageUnique;							// 배당 마을

	SI32				siTotalStock;								// 총 배당 주식수 
	cltMoney			clStockDividendPrice;						// 한주당 배당금 
	cltMoney			clResidentDividendPrice;					// 주민 한명당 받은돈 금액 
	cltMoney			clTaxDividendPrice;							// 왕실 세금으로 납부한 금액

	GMONEY				siChiefPay;									// 대행수 급여

	SI32				siKingPersonID;								// 군주(왕) Person ID
	GMONEY				siKingPay;									// 군주(왕) 급여 
	SI32				siKingPayPersent;							// 군주(왕) 급여 비율

	UI16				usYear;										// 배당년도 
	cltDate				clDate;										// 최근 배당일 

	cltMoney			clCityhallMoney;							// 배당후 상단보유 이익금 
	GMONEY				siPersonMoney;								// 배당후 대행수 소지금

	UI16				usStockPersonCount;										// 주식 배당받는 유저 수
	cltDividendPerson	clStockDividendPerson[ MAX_DIVIDEND_PERSON ];		// 주식 배당받는 유저 정보 

	UI16				usResidentPersonCount;										// 주민 배당받는 유저 수
	cltDividendPerson	clResidentDividendPerson[ MAX_DIVIDEND_PERSON ];	// 주민 배당받는 유저 정보 

	//KHY - 0730 - 배당 받을 유저의 은행 총 금액을 업데이트 시켜준다.
	GMONEY			clKingAllMoney;					// 월급을 포함한 군주의 총 은행금액.


	sDBResponse_AutoPayDividend()
	{
		usCharID = 0;

		siResult = 0;
		siVillageUnique = 0;

		siTotalStock = 0;
		clStockDividendPrice.Init();
		clResidentDividendPrice.Init();
		clTaxDividendPrice.Init();

		siChiefPay = 0;

		siKingPersonID = 0;
		siKingPay = 0;
		siKingPayPersent = 0;

		usYear = 0;

		clCityhallMoney.Init();
		siPersonMoney = 0;

		usStockPersonCount = 0;
		usResidentPersonCount = 0;

	}

};


//------------------------------------------
// 배당을 요청한다 
//------------------------------------------
class sDBRequest_PayDividend
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;	

	SI32				siVillageUnique;			// VillageUnique
	
	cltMoney			clTotalDividend;			// 총 배당금액
	GMONEY				siChiefPay;					// 대행수 급여 

	UI16				usYear;						// 배당년도
	cltDate				clDate;						// 최근 배당일
	
	SI32				siParentVillage;			// 부모마을
	GMONEY				siParentPay;				// 부모마을에 납부 조공 
	
	GMONEY				siKingPay;					// 군주(왕) 급여
	SI32				siKingPayPersent;			// 군주(왕) 급여 비율 

	sDBRequest_PayDividend( SI32 id, SI32 villageUnique, cltMoney *pclMoney, GMONEY sichiefpay, UI16 year, cltDate *pclDate,
		SI32 parentVillage, GMONEY parentPay, GMONEY kingPay, SI32 kingPayPersent )
	{

		packetHeader.usSize = sizeof( sDBRequest_PayDividend );
		packetHeader.usCmd = DBMSG_REQUEST_PAYDIVIDEND;
		usCharID		= id;

		siVillageUnique	= villageUnique;

		clTotalDividend.Set( pclMoney );
		siChiefPay = sichiefpay;

		usYear = year;
		clDate.Set( pclDate );

		siParentVillage = parentVillage;
		siParentPay = parentPay;

		siKingPay = kingPay;
		siKingPayPersent = kingPayPersent;
	}

};


//------------------------------------------
// 배당 요청에 응답한다
//------------------------------------------
class sDBResponse_PayDividend
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;									// 결과: 1성공 0실패  -1보유자금 부족 

	SI32				siVillageUnique;							// 배당 마을

	SI32				siTotalStock;								// 총 배당 주식수 
	cltMoney			clDividendPrice;							// 한주당 배당금 

	GMONEY				siChiefPay;									// 대행수 급여
	
	SI32				siKingPersonID;								// 군주(왕) Person ID
	GMONEY				siKingPay;									// 군주(왕) 급여 
	SI32				siKingPayPersent;							// 군주(왕) 급여 비율

	SI32				siParentVillage;							// 부모 마을
	GMONEY				siParentPay;								// 부모 마을로의 납부 조공

	cltMoney			clParentCityhallMoney;						// 부모마을 상단 보유자금


	UI16				usYear;										// 배당년도 
	cltDate				clDate;										// 최근 배당일 
	
	cltMoney			clCityhallMoney;							// 배당후 상단보유자금 
	GMONEY				siPersonMoney;								// 배당후 대행수 소지금
	
	UI16				usPersonCount;								// 배당받는 유저 수
	cltDividendPerson	clDividendPerson[ MAX_DIVIDEND_PERSON ];	// 배당받는 유저 정보 

	sDBResponse_PayDividend()
	{
		ZeroMemory(this, sizeof(sDBResponse_PayDividend));
	}

};

class sDBRequest_SetStructureMessage
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	SI32				siStrType;

	TCHAR				szMessage[ 256 ];

	sDBRequest_SetStructureMessage( SI32 villageUnique, SI32 strType, TCHAR *msg )
	{

		packetHeader.usSize = sizeof( sDBRequest_SetStructureMessage );
		packetHeader.usCmd = DBMSG_REQUEST_SETSTRUCTUREMESSAGE;

		siVillageUnique = villageUnique;
		siStrType = strType;

		memcpy( szMessage, msg, 256 );
		szMessage[ 255 ] = NULL;
	}

};

#define BUILD_BY_PERSON 0
#define BUILD_BY_MASTER 1

class sDBRequest_BuildStructure
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;					// 마을 
	SI32				siSlot;								// 슬롯
	SI32				siStructure;						// 건설 건물 unique
	GMONEY				siCost;								// 건설 비용
	SI32				siType;
	SI32				siNeedVillageScore;					// 필요한 마을 점수.

	sDBRequest_BuildStructure( UI16 id, SI32 villageUnique, SI32 slot, SI32 structure, GMONEY cost, SI32 Type, SI32		NeedVillageScore )
	{
		packetHeader.usCmd = DBMSG_REQUEST_BUILDSTRUCTURE;
		packetHeader.usSize = sizeof( sDBRequest_BuildStructure );

		usCharID = id;
		siVillageUnique = villageUnique;
		siSlot = slot;
		siStructure = structure;
		siCost = cost;
		siType = Type;
		siNeedVillageScore = NeedVillageScore;
	}

};

class sDBResponse_BuildStructure
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;
	SI32				siSlot;
	SI32				siStructure;
	GMONEY				siCost;

	cltMoney			clCityhallMoney;
	SI32				siType;
	SI32				siNeedVillageScore;					// 필요한 마을 점수.


	sDBResponse_BuildStructure()
	{
		ZeroMemory( this, sizeof( sDBResponse_BuildStructure ) );
	}

};

class sDBRequest_DestroyStructure
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;					// 마을 
	SI32				siSlot;								// 슬롯
	SI32				siStructure;						// 건물 unique
	SI32				siRankType;							// 행수 타입
	
	SI32				siLastDestroyDateVary;				// 건물을 해체한 마지막 시간. 
	

	sDBRequest_DestroyStructure( UI16 id, SI32 villageUnique, SI32 slot, SI32 structure, SI32 ranktype, SI32 destroydate )
	{
		packetHeader.usCmd = DBMSG_REQUEST_DESTROYSTRUCTURE;
		packetHeader.usSize = sizeof( sDBRequest_DestroyStructure );

		usCharID = id;
		siVillageUnique = villageUnique;
		siSlot = slot;
		siStructure = structure;
		siRankType = ranktype;

		siLastDestroyDateVary	= destroydate;
	}


};

class sDBResponse_DestroyStructure
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	
	SI32				siResult;

	SI32				siVillageUnique;					// 마을
	SI32				siSlot;								// 슬롯
	SI32				siStructure;						// 건물 unique
	SI32				siRankType;							// 행수 타입

	SI32				siChiefPersonID;					// 건물 행수 person id

	SI32				siLastDestroyDateVary;				// 건물을 해체한 마지막 시간. 

	sDBResponse_DestroyStructure()
	{
		ZeroMemory( this, sizeof( sDBResponse_DestroyStructure ) );
	}

};


//-----------------------------------------
// 
//-----------------------------------------
class sDBRequest_ChiefInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 건물 행수 person id

	cltSimpleRank		SimpleRank;

	sDBRequest_ChiefInfo( UI16 id, SI32 personid, cltSimpleRank* pRank )
	{
		packetHeader.usCmd = DBMSG_REQUEST_CHIEFINFO;
		packetHeader.usSize = sizeof( sDBRequest_ChiefInfo );

		usCharID = id;
		siPersonID = personid;

		SimpleRank.Set(pRank);
	}


};

class sDBResponse_ChiefInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;
	
	SI32				siPersonID;			// 건물 행수 person id
	cltRank				Rank;
	
	TCHAR				AccountID[MAX_PLAYER_NAME];

	sDBResponse_ChiefInfo()
	{
		ZeroMemory( this, sizeof( sDBResponse_ChiefInfo ) );
	}


};

/*
class sDBRequest_RepairStructure
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;										// villageunique
	SI32				siRankType;												// 건물 

	SI32				siPersonID;												// personid

	SI16				siItemInventoryIndex;									// item index
	SI32				siItemUnique;											// item unique
	SI16				siItemNum;												// item num

	sDBRequest_RepairStructure( UI16 id, SI32 villageUnique, SI32 ranktype, SI32 personid,
		SI16 itemInventoryIndex, SI32 itemUnique, SI16 itemNum )
	{
		packetHeader.usCmd = DBMSG_REQUEST_REPAIRSTRUCTURE;
		packetHeader.usSize = sizeof( sDBRequest_RepairStructure );
		usCharID = id;

		siVillageUnique = villageUnique;
		siRankType = ranktype;

		siPersonID = personid;

		siItemInventoryIndex = itemInventoryIndex;
		siItemUnique = itemUnique;
		siItemNum = itemNum;
	}

};

class sDBResponse_RepairStructure
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;										// villageunique
	SI32				siRankType;												// 건물 

	SI16				siUseItemNum;											// 사용한 아이템 개수 

	SI32				siPersonID;

	SI16				siItemInventoryIndex;									// item index
	SI32				siItemUnique;											// item unique
	SI16				siItemNum;												// item num


	sDBResponse_RepairStructure()
	{
		ZeroMemory( this, sizeof( sDBResponse_RepairStructure ) );
	}

};
*/

class sDBRequest_SetStrRepairItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;										// villageunique
	SI32				siRankType;												// 건물 

	SI32				siItemUnique;
	SI16				siItemNum;

	sDBRequest_SetStrRepairItem( SI32 villageunique, SI32 ranktype, SI32 itemUnique, SI16 itemNum )
	{
		packetHeader.usCmd = DBMSG_REQUEST_SETSTRREPAIRITEM;
		packetHeader.usSize = sizeof( sDBRequest_SetStrRepairItem );

		siVillageUnique = villageunique;
		siRankType = ranktype;
		siItemUnique = itemUnique;
		siItemNum = itemNum;
	}


};

class sDBResponse_SetStrRepairItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;										// villageunique
	SI32				siRankType;												// 건물 

	SI32				siItemUnique;
	SI16				siItemNum;

	sDBResponse_SetStrRepairItem()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetStrRepairItem ) );
	}

};

class sDBRequest_SetStrDura
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;										// villageunique
	SI32				siRankType;												// 건물 

	SI32				siStrDura;

	sDBRequest_SetStrDura( SI32 villageunique, SI32 ranktype, SI32 strdura )
	{
		packetHeader.usCmd = DBMSG_REQUEST_SETSTRDURA;
		packetHeader.usSize = sizeof( sDBRequest_SetStrDura );

		siVillageUnique = villageunique;
		siRankType = ranktype;
		siStrDura = strdura;
	}

};

class sDBResponse_SetStrDura
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;										// villageunique
	SI32				siRankType;												// 건물 

	SI32				siStrDura;

	sDBResponse_SetStrDura()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetStrDura ) );
	}

};

class sDBRequest_ChangeStrDura
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;										// villageunique
	SI32				siRankType;												// 건물 

	SI32				siChangeDura;

	sDBRequest_ChangeStrDura( SI32 villageunique, SI32 ranktype, SI32 changedura )
	{
		packetHeader.usCmd = DBMSG_REQUEST_CHANGESTRDURA;
		packetHeader.usSize = sizeof( sDBRequest_ChangeStrDura );

		siVillageUnique = villageunique;
		siRankType = ranktype;
		siChangeDura = changedura;
	}

};

class sDBResponse_ChangeStrDura
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;										// villageunique
	SI32				siRankType;												// 건물 

	SI32				siStrDura;

	sDBResponse_ChangeStrDura()
	{
		ZeroMemory( this, sizeof( sDBResponse_ChangeStrDura ) );
	}

};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_ChangeStrBaseMoney
//
// Last Modified 	: 2006 / 02 / 21
// Created		 	: 김광명
//
// Function			: [요청] 건물의 자본금(수리시 필요한 돈)을 변화한다.
//
//***************************************************************************************************
class sDBRequest_ChangeStrBaseMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siVillageUnique;			// 마을 

	SI32				siStrType;					// 건물 종류
	GMONEY				siMoney;					// 건물 보유자금에서 더 하거나 뺄 금액 

	sDBRequest_ChangeStrBaseMoney()
	{
		Init();
	}

	sDBRequest_ChangeStrBaseMoney( SI32 CharId, SI32 VillageUnique, SI32 StrType, GMONEY Money )
	{
		Set( CharId, VillageUnique, StrType, Money );
	}

	void Init()
	{
		ZeroMemory( this, sizeof( sDBRequest_ChangeStrBaseMoney ) );
	}

	void Set( SI32 CharId, SI32 villageUnique, SI32 strType, GMONEY money ) 
	{
		packetHeader.usSize = sizeof( sDBRequest_ChangeStrBaseMoney );
		packetHeader.usCmd = DBMSG_REQUEST_CHANGESTRBASEMONEY;

		siCharID = CharId;
		siVillageUnique = villageUnique;
		siStrType = strType;
		siMoney = money;
	}

};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_ChangeStrBaseMoney
//
// Last Modified 	: 2006 / 02 / 21
// Created		 	: 김광명
//
// Function			: [응답] 건물의 자본금(수리시 필요한 돈)을 변화한다.
//
//***************************************************************************************************
class sDBResponse_ChangeStrBaseMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;					// 성공:1, 실패:0, 보유자금부족:2
	SI32				siCharID;					// 요청한 대행수 게임내 아이디값
	SI32				siVillageUnique;
	SI32				siStrType;					// 건물 종류
	cltMoney			clBaseMoney;				// 건물 보유자금 

	GMONEY				siChangeMoney;				// 증감 금액

	cltMoney			clCityhallMoney;			// 증감되고 난후에 관청 자본금

	sDBResponse_ChangeStrBaseMoney()
	{
		ZeroMemory( this, sizeof( sDBResponse_ChangeStrBaseMoney ) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_SetStrRepairAmount
//
// Last Modified 	: 2006 / 02 / 23
// Created		 	: 김광명
//
// Function			: [요청] 건물의 수리량을 요청한다(수리 초기화)
//
//***************************************************************************************************
class sDBRequest_SetStrRepairAmount
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;										// villageunique
	SI32				siRankType;												// 건물 

	sDBRequest_SetStrRepairAmount( SI32 VillageUnique, SI32 RankType )
	{
		packetHeader.usCmd = DBMSG_REQUEST_SETSTRREPAIRAMOUNT;
		packetHeader.usSize = sizeof( sDBRequest_SetStrRepairItem );

		siVillageUnique = VillageUnique;
		siRankType = RankType;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_SetStrRepairAmount
//
// Last Modified 	: 2006 / 02 / 23
// Created		 	: 김광명
//
// Function			: [응답] 건물의 수리량을 요청한다(수리 초기화)
//
//***************************************************************************************************
class sDBResponse_SetStrRepairAmount
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;										// villageunique
	SI32				siRankType;												// 건물 

	sDBResponse_SetStrRepairAmount()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetStrRepairAmount ) );
	}

};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_RepairStructure
//
// Last Modified 	: 2006 / 02 / 23
// Created		 	: 김광명
//
// Function			: [요청] 건물을 수리한다
//
//***************************************************************************************************
class sDBRequest_RepairStructure
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;		// villageunique
	SI32				siRankType;				// 건물 

	SI32				siPersonID;				// personid

	SI32				siStrRepairAmount;		// 수리할려교 요청하는 양
	GMONEY				siRepairMoney;

	//KHY - 1102 - 마을 수리비 재조정.
	SI16 			siItemUnique; //수리 셋트 유니크 - 필요겟수는  siStrRepairAmount와 같다.

	sDBRequest_RepairStructure( UI16 Id, SI32 VillageUnique, SI32 RankType, SI32 PersonId, 
		SI32 StrRepairAmount, GMONEY RepairMoney ,SI16 itemunique )
	{
		packetHeader.usCmd = DBMSG_REQUEST_REPAIRSTRUCTURE;
		packetHeader.usSize = sizeof( sDBRequest_RepairStructure );
		usCharID = Id;

		siVillageUnique = VillageUnique;
		siRankType = RankType;

		siPersonID = PersonId;
		siStrRepairAmount = StrRepairAmount;
		siRepairMoney = RepairMoney;
		siItemUnique = itemunique;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_RepairStructure
//
// Last Modified 	: 2006 / 02 / 23
// Created		 	: 김광명
//
// Function			: [응답] 건물을 수리한다
//
//***************************************************************************************************
class sDBResponse_RepairStructure
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;			// villageunique
	SI32				siRankType;					// 건물 

	SI32				siPersonID;

	SI32				siStrRepairAmount;			// 행수가 수리요청한 양
	GMONEY				siLeftBaseMoney;			// 수리하고 난 뒤에 건물 자본금
	SI32				siUpdateStrRepairAmount;	// 수리학고 난 뒤에 수리량(누적값 최고 1000)

	SI16 				siItemUnique;				// 수리하는데 사용한 아이템

	SI16				siItemPos;					// 수리하는데 사용한 아이템 위치
	cltItem				clItem;						// 수리하는데 사용하고 남은 아이템 정보

	sDBResponse_RepairStructure()
	{
		ZeroMemory( this, sizeof( sDBResponse_RepairStructure ) );
	}

};

class sDBRequest_Contribution_Item_List_Input
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;
	SI32				m_siPersonID;

	SI32 				m_siVillageUnique;
	SI32 				m_siNPCKind;
	SI32 				m_siInputItemUnique[CONTRIBUTION_ITEM_NUM];
	SI16 				m_siInputItemNum[CONTRIBUTION_ITEM_NUM];
	SI32				m_siInputItemIvenPos[CONTRIBUTION_ITEM_NUM];
	
	sDBRequest_Contribution_Item_List_Input( UI16 _usID, SI32 _siPersonID, SI32 _siVillageUnique, SI32 _siNPCKind, SI32* _psiInputItemUnique,  SI16* _psiInputItemNum, SI32* _psiInputItemIvenPos )
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usSize = sizeof( sDBRequest_Contribution_Item_List_Input );
		m_packetHeader.usCmd = DBMSG_REQUEST_CONTRIBUTION_ITEM_LIST_INPUT;
		m_usCharID			= _usID;
		m_siPersonID		= _siPersonID;

		m_siVillageUnique	= _siVillageUnique;
		m_siNPCKind			= _siNPCKind;
		memcpy( m_siInputItemUnique,	_psiInputItemUnique,	sizeof(m_siInputItemUnique) );
		memcpy( m_siInputItemNum,		_psiInputItemNum,		sizeof(m_siInputItemNum) );
		memcpy( m_siInputItemIvenPos,	_psiInputItemIvenPos,	sizeof(m_siInputItemIvenPos) );
	}
};

class sDBResponse_Contribution_Item_List_Input
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siResult;
	UI16				m_usCharID;
	SI32				m_siPersonID;

	SI32 				m_siVillageUnique;
	SI32 				m_siNPCKind;
	SI32 				m_siItemUnique[CONTRIBUTION_ITEM_NUM];
	SI16 				m_siItemNum[CONTRIBUTION_ITEM_NUM];
	
	SI16 				m_siInputItemNum[CONTRIBUTION_ITEM_NUM];
	SI32				m_siInputItemIvenPos[CONTRIBUTION_ITEM_NUM];

	sDBResponse_Contribution_Item_List_Input( void )
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

class sDBRequest_Contribution_Make_NPC
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;
	SI32				m_siPersonID;

	SI32 				m_siVillageUnique;
	SI32 				m_siNPCKind;
	SI32 				m_siItemUnique[CONTRIBUTION_ITEM_NUM];
	SI16 				m_siItemNum[CONTRIBUTION_ITEM_NUM];

	sDBRequest_Contribution_Make_NPC( UI16 _usID, SI32 _siPersonID, SI32 _siVillageUnique, SI32 _siNPCKind, SI32* _psiItemUnique,  SI16* _psiItemNum )
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usSize = sizeof( sDBRequest_Contribution_Make_NPC );
		m_packetHeader.usCmd = DBMSG_REQUEST_CONTRIBUTION_MAKE_NPC;
		m_usCharID			= _usID;
		m_siPersonID		= _siPersonID;

		m_siVillageUnique	= _siVillageUnique;
		m_siNPCKind			= _siNPCKind;
		memcpy( m_siItemUnique,		_psiItemUnique,		sizeof(m_siItemUnique) );
		memcpy( m_siItemNum,		_psiItemNum,		sizeof(m_siItemNum) );
	}

};

class sDBResponse_Contribution_Make_NPC
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siResult;
	UI16				m_usCharID;
	SI32				m_siPersonID;

	SI32 				m_siVillageUnique;
	SI32 				m_siNPCKind;

	sDBResponse_Contribution_Make_NPC( void )
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

class sDBRequest_Contribution_Item_List_Delete
{
public:
	sPacketHeader		m_packetHeader;

	SI32 				m_siVillageUnique;
	SI32 				m_siNPCKind;

	sDBRequest_Contribution_Item_List_Delete( SI32 _siVillageUnique, SI32 _siNPCKind )
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usSize = sizeof( sDBRequest_Contribution_Item_List_Delete );
		m_packetHeader.usCmd = DBMSG_REQUEST_CONTRIBUTION_ITEM_LIST_DELETE;

		m_siVillageUnique	= _siVillageUnique;
		m_siNPCKind			= _siNPCKind;
	}
};

class sDBRsponse_Contribution_Item_List_Delete
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siResult;

	SI32 				m_siVillageUnique;
	SI32 				m_siNPCKind;

	sDBRsponse_Contribution_Item_List_Delete( void )
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

#endif
