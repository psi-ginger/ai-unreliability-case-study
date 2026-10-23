//---------------------------------
// 2003/9/27 김태곤
//---------------------------------

#ifndef _LETTER_H
#define _LETTER_H

#define MAX_LETTER_SIZE		64
#define MAX_LETTER_NUMBER	10

#define MAX_ACCOUNTLETTER_SIZE	512

#include <Directives.h>

#include <CommonHeader.h>
#include "..\..\Server\Time\MyTime.h"
#include "../../CommonLogic/Guild/Guild.h"
#include "../SystemReward/SystemReward.h"


#define LETTERTYPE_FIRE								1	// 해임 통지. 
#define LETTERTYPE_BERANK							2	// 임명 통지. 
#define LETTERTYPE_RECALLHOUSE						3	// 시전 회수. 
#define LETTERTYPE_PAYRENTFEE						4	// 시전 임대료 지불
#define LETTERTYPE_STOCKTRADE						5	// 주식 거래 통지. 
#define LETTERTYPE_MARKETTRADE						6	// 객주에서 물건 팔림 통보. 
#define LETTERTYPE_MARKETPRICEDOWN					7	// 객주에 올려 놓은 물건 값이 인하됨
#define LETTERTYPE_HOUSERECALLWARINING				8	// 시전 회수 직전 경고. 
#define LETTERTYPE_RENTHOUSE						9	// 시전 임차가 허락되었따. 
#define LETTERTYPE_GETDIVIDEND						10	// 주식 배당을 받았다. 
#define LETTERTYPE_FATHER							11	// 스승님 설정/해제 
#define LETTERTYPE_CHILDLEVELUP						12	// 제자 레벨 업으로 포상 지급 
#define LETTERTYPE_GETSALARY						13	// 세금/배당 납부로 행수/대행수 급여 지급 받음. 
#define LETTERTYPE_MAIL								14	// 메일이 도착했습니다.  
#define LETTERTYPE_RECOMMENDER						15	// 추천인 등록 완료
#define LETTERTYPE_RECOMMENDERLEVELUP				16	// 추천인 레벨업
#define LETTERTYPE_STOCKLOSS						17  // 전쟁 패배로 주식을 소각 당했다
#define LETTERTYPE_KINGPAY							18	// 마을 배당으로 군주(왕)가 배당금의 일부를 받음
#define LETTERTYPE_RESIDENTSHARE					19	// 마을 주민 배당금
#define LETTERTYPE_HORSEMARKETTRADE					20  // 사복시에서 말 팔림 통보
#define LETTERTYPE_HORSEMARKETPRICEDOWN				21  // 사복시에 올려 놓은 말의 가격이 인하됨 
#define LETTERTYPE_STRUCTUREREPAIRITEM				25	// 건물 수리에 필요한 아이템을 행수에게 알림
#define LETTERTYPE_REALESTATEMARKETTRADE			26	// 거간에서 부동산 팔림 통보 
#define LETTERTYPE_REALESTATEMARKETPRICEDOWN		27	// 거간의 부동산 주문의 가격이 인하됨
#define LETTERTYPE_SUMMONMARKETTRADE				28	// 장예원에서 소환수 팔림 통보 
#define LETTERTYPE_SUMMONMARKETPRICEDOWN			29	// 장예원에서 소환수 주문의 가격이 인하됨
#define LETTERTYPE_RESIDENTOUTDATE					30  // 주민 자동 탈퇴 (1년이상 접속 하지 않은 주민)
#define LETTERTYPE_RESULTBIDSTOCK					31  // 증자 종료 (결과)
#define LETTERTYPE_RENTLAND							34  // 농장 임차가 허락되었다 
#define LETTERTYPE_PAYLANDRENTFEE					35	// 농장 임대료 지불 
#define LETTERTYPE_LANDRECALL						36  // 농장 회수
#define LETTERTYPE_LANDRECALLWARNING				37  // 농장 회수 경고
#define LETTERTYPE_HOZOANGEL						38	// 수호천사 관련
#define LETTERTYPE_CHIEFOUTWARNING					39	// 대행수 임기 만료전 알림 
#define LETTERTYPE_CHIEFOUTDATE						40	// 대행수 임기 만료
#define LETTERTYPE_GENERALMEETING					41	// 내가 보유중인 주식의 마을에서 주주총회 진행중.
#define LETTERTYPE_SUMMONHEROMARKETTRADE			42	// 비변사에서 소환영웅 팔림 통보 
#define LETTERTYPE_SUMMONHEROMARKETPRICEDOWN		43	// 비변사에서 소환영웅 주문의 가격이 인하됨
#define LETTERTYPE_POSTOFFICE_SENDOK				44	// 역참에서 보낸 물품이나 현금이 상대방에게 도착했을때 통보

#define LETTERTYPE_RENTMINE							45  // 광산 임차가 허락되었다 
#define LETTERTYPE_PAYMINERENTFEE					46	// 광산 임대료 지불 
#define LETTERTYPE_MINERECALL						47  // 광산 회수
#define LETTERTYPE_MINERECALLWARNING				48  // 광산 회수 경고

#define LETTERTYPE_VILLAGENAME_AND_DATE				49	// 해당 마을 이름 및 일시 (예, 마을 파산 경고 및 파산함에 쓰임)

#define LETTERTYPE_HYUNGZOPOLICE					51	// 형조 포교 관련
#define LETTERTYPE_EXISTENT_SAVEUSERITEM			52	// 시전 연장 못해서 DB에 아이템이 있다는 메세지
#define LETTERTYPE_STOCKOFFERING_RESULT				53	// 주식 공모 신청 추첨 결과 통보

#define ACCOUNT_LETTERTYPE_PRESENT					54	// 선물 받음 메시지

#define ACCOUNT_LETTERTYPE_PRESENTANSWER			55	// 선물 감사 메시지
#define LETTERTYPE_WORLDMONEYTRADERESULT			56	// 월드 머니 거래 결과
#define LETTERTYPE_TRADESERVER_SELL					57	// 무역 거래

#define LETTERTYPE_CHANGERECOMMENDPOINT				58
#define LETTERTYPE_LEVELUPRECOMMANDREWARD			59

#define LETTERTYPE_WORLDMONEY_REDUCE_PRICE			60	// 무역

#define LETTERTYPE_LEVELUPRECOMMANDREWARD_FOR_RECOMMENDER			61

#define LETTERTYPE_GUILDJOIN							62			// 길드 가입에 대한 것
#define LETTERTYPE_GUILD_CONFIRMCREATEGUILD				63			// 길드 생성에 대한 것
#define LETTERTYPE_GUILD_SECONDMASTER					64			// 길드 오피서 임명에 대한 것
#define LETTERTYPE_GUILD_CHANGEMASTER					65			// 갈드 마스터 변경
#define LETTERTYPE_GUILD_CONFIRMCHANGEVILLAGE			66			// 길드 마을 변경
#define LETTERTYPE_GUILD_SUCCESSRESERVE					67			// 길드전 성립!!
#define	LETTERTYPE_GUILD_DESTROY						68			// 길드가 없어졌다
#define LETTERTYPE_GUILDDUNGEON_APPLY					69			// 사냥터 소유권 도전신청
#define LETTERTYPE_GUILDDUNGEON_CHANGEGPMONEY			70			// 사냥터 소유권에 의한 GP, MONEY 획득
#define LETTERTYPE_GUILDDUNGEON_GETOWNERSHIP			71			// 사냥터 소유권 획득
#define LETTERTYPE_RENOUNCEPUPIL						72			// 스승이 제자를 짜른다.
#define LETTERTYPE_ACCEPTFRIEND_RESULT					73			// 친구등록하기의 결과
#define LETTERTYPE_TEACHERREWARD_PUPILREWARD			74			// 스승제자 보상 받을때 메세지 보내기
#define LETTERTYPE_PVPRESULT							75			// PVP결과
#define LETTERTYPE_GUILD_ALERTDESTROY					76			// 길드 해체 경고
#define LETTERTYPE_SYSTEMREWARD_GIVE					77			// 보상NPC 에게 보상 보냄.
#define LETTERTYPE_GUILD_CHANGEMASTER_NOTCONNECT_MONTH	78			// 갈드 마스터 변경
#define LETTERTYPE_INVITE_APPLYRESIDENT					79			// 스승캐릭이 제자캐릭을 마을에 초대하였음을 영주에게 보냄. 
#define	LETTERTYPE_CREATECHARREWARD						80			// 케릭터 생성에 대한 아이템 보상 메시지
#define LETTERTYPE_GUILDDUNGEON_APPLY_NEW				81			// 새로운 사냥터 소유권 도전신청
#define LETTERTYPE_WEDDINGCEREMONY_CANCEL				82			// 예식이 취소됨
#define LETTERTYPE_FAMILY_DEL_FAMILY					83			// 가족에서 제외 되셨습니다.
#define LETTERTYPE_FAMILY_LEAVE_FAMILY					84			// 가족에서 나갔다..
#define LETTERTYPE_FAMILY_ADD_FAMILY					85			// 가족에 자식을 등록 하였다._기존 가족
#define LETTERTYPE_FAMILY_ADD_FAMILY_TO_ME				86			// 가족에 자식을 등록 하였다.-새 가족 
#define LETTERTYPE_DORMANCY_WARING_EXPIRATION			87			// 휴면 계정 종료 경고
#define LETTERTYPE_SKILLBOOK_MAGIC_DEL					88			// 스킬북으로 획득한 마법의 삭제를 알림.

class cltLetterHeader{
public:
	SI16 siType;
	SI16 siSize;

	cltLetterHeader()
	{
		siType	= 0;
		siSize	= 0;
	}
};

//#if	defined(_CITYHALL_BANKRUPTCY)

	//-------------------------------------------
	// 마을명/시간/타입 Letter
	// - 마을 파산 경보 및 파산 알림 ( 포맷 : 마을이름, 시간 )
	//-------------------------------------------
	class cltLetterMsg_VillageNameAndDate{
	public:
		enum {
			LETTERTYPE_CITYHALLBANKRUPTCY_ALERT = 0,
			LETTERTYPE_CITYHALLBANKRUPTED = 1,
			LETTERTYPE_RETURNSTOCKBIDVILLAGE = 2,

			LETTERTYPE_END,
		};

		cltLetterHeader clHeader;

		SI32			siVillageUnique;			//  대상 마을 이름 (예, 파산 위험 마을 )
		cltDate			clDate;						//  대상 시간 (예, 파산 예정 시간 혹은 파산일)
		UI08			usType;						// 

		cltLetterMsg_VillageNameAndDate( SI32 villageunique, cltDate* pcldate, UI08 ustype)
		{
			clHeader.siType = LETTERTYPE_VILLAGENAME_AND_DATE;
			clHeader.siSize	= sizeof(cltLetterMsg_VillageNameAndDate);

			siVillageUnique = villageunique;
			clDate.Set( pcldate );
			usType = ustype;
		}
	};

//#endif

//-------------------------------------------
// 추천한 사람이 레벨업 해서 추천된 사람이 돈이랑 경험치 받는것
//-------------------------------------------
class cltLetterMsg_RecommenderLevelUp
{
public:
	cltLetterHeader clHeader;

	SI16 siIncLevel;						// 추천된 사람의 레벨 
	TCHAR szName[MAX_PLAYER_NAME];
	SI16 siItemUnique;						// 포상 물품 유니크. 
	SI16 siItemNum;							// 포상 물품 개수. 
	SI16 siFameExp;

	cltLetterMsg_RecommenderLevelUp( SI16 IncLevel, TCHAR* pname, SI16 itemunique, SI16 itemnum, SI16 fameexp)
	{
		clHeader.siType = LETTERTYPE_RECOMMENDERLEVELUP;
		clHeader.siSize	= sizeof(cltLetterMsg_RecommenderLevelUp);

		siIncLevel = IncLevel;
		StringCchCopy(szName, MAX_PLAYER_NAME, pname);
		siItemUnique	= itemunique;
		siItemNum		= itemnum;
		siFameExp		= fameexp;
	}
};




//-------------------------------------------
// 추천인 등록
//-------------------------------------------
class cltLetterMsg_Recommender{
public:
	cltLetterHeader clHeader;

	TCHAR szRecommenderName[MAX_PLAYER_NAME];

	cltLetterMsg_Recommender(TCHAR* pname)
	{
		clHeader.siType = LETTERTYPE_RECOMMENDER;
		clHeader.siSize	= sizeof(cltLetterMsg_Recommender);

		StringCchCopy(szRecommenderName, MAX_PLAYER_NAME, pname);
	}
};

//-------------------------------------------
// 메일이 도착
//-------------------------------------------
class cltLetterMsg_Mail{
public:
	cltLetterHeader clHeader;

	TCHAR szFromPersonName[MAX_PLAYER_NAME];

	cltLetterMsg_Mail(TCHAR* pname)
	{
		clHeader.siType = LETTERTYPE_MAIL;
		clHeader.siSize	= sizeof(cltLetterMsg_Mail);

		StringCchCopy(szFromPersonName, MAX_PLAYER_NAME, pname);
	}
};

//-------------------------------------------
// 세금/배당에 의한 봉급 수령. 
//-------------------------------------------
class cltLetterMsg_GetSalary{
public:
	cltLetterHeader clHeader;

	cltSimpleRank	clRank;					// 수령자. 
	GMONEY			siMoney;				// 수령액.

	cltLetterMsg_GetSalary(cltSimpleRank* pclRank, GMONEY money)
	{
		clHeader.siType = LETTERTYPE_GETSALARY;
		clHeader.siSize	= sizeof(cltLetterMsg_GetSalary);

		clRank.Set(pclRank);
		siMoney	= money;
	}
};

//-------------------------------------------
// 제자 레벨업으로 인해 포상금 지급 
//-------------------------------------------
class cltLetterMsg_ChildLevelUp{
public:
	cltLetterHeader clHeader;

	enum { FATHERREWARD = 0, 
		   GRANDFATHERREWARD, 
		   PUPILREWARD };

	SI16 siIncLevel;						// 제자의 증가된 레벨 
	TCHAR szChildName[MAX_PLAYER_NAME];
	GMONEY siMoney;							// 포상금. 
	SI32 siFameExp;							// 명성 경험치. 
	UI08 uiType;


	cltLetterMsg_ChildLevelUp( SI16 inclevel, TCHAR* pname, GMONEY money, SI32 fameexp, UI08 Type = FATHERREWARD )
	{
		clHeader.siType = LETTERTYPE_CHILDLEVELUP;
		clHeader.siSize	= sizeof(cltLetterMsg_ChildLevelUp);

		siIncLevel	= inclevel;

		StringCchCopy(szChildName, MAX_PLAYER_NAME, pname);

		siMoney		= money;
		siFameExp	= fameexp;
		uiType = Type;
	}
};

//-------------------------------------------
// 스승 설정 해제 
//-------------------------------------------
class cltLetterMsg_Father{
public:
	cltLetterHeader clHeader;

	bool bFatherSwitch;			// 스승 임명/해제 
	TCHAR szChildName[MAX_PLAYER_NAME];


	cltLetterMsg_Father(bool bfatherswitch, TCHAR* pname)
	{
		clHeader.siType = LETTERTYPE_FATHER;
		clHeader.siSize	= sizeof(cltLetterMsg_Father);

		bFatherSwitch	= bfatherswitch;
		StringCchCopy(szChildName, MAX_PLAYER_NAME, pname);
	}
};

//-------------------------------------------
// 주식 배당을 받음. 
//-------------------------------------------
class cltLetterMsg_GetDividend{
public:
	cltLetterHeader clHeader;

	SI08 siType;					// 0 : 주주 1: 주민
	SI16 siVillageUnique;			// 어느 마을 주식인가. 
	GMONEY siMoney;					// 배당 받은 금액. 

	cltLetterMsg_GetDividend(SI16 villageunique, GMONEY money, SI08 type = 0)
	{
		clHeader.siType = LETTERTYPE_GETDIVIDEND;
		clHeader.siSize	= sizeof(cltLetterMsg_GetDividend);

		siVillageUnique	= villageunique;
		siMoney			= money;
		siType			= type;
	}
};


//-------------------------------------------
// 객주에 올려 놓은 물건값 인하 통보. 
//-------------------------------------------
class cltLetterMsg_MarketPriceDown{
public:
	enum
	{
		TYPE_MARKET = 0,
		TYPE_NEWMARKET
	};

	cltLetterHeader clHeader;

	SI16 siItemUnique;			// 대상 물품. 
	SI16 siItemNum;				// 올려 놓은 개수 

	GMONEY siPrice;				// 인하된 금액. 
	UI08 uiType;

	cltLetterMsg_MarketPriceDown(SI16 itemunique, SI16 itemnum, GMONEY price, UI08 type = TYPE_MARKET)
	{
		clHeader.siType = LETTERTYPE_MARKETPRICEDOWN;
		clHeader.siSize	= sizeof(cltLetterMsg_MarketPriceDown);

		siItemUnique	= itemunique;
		siItemNum		= itemnum;
		siPrice			= price;
		uiType			= type;
	}
};

//-------------------------------------------
// 객주에서 물건 팔림 통보. 
//-------------------------------------------
class cltLetterMsg_MarketTrade{
public:
	cltLetterHeader clHeader;

	enum
	{
		TYPE_MARKET = 0,
		TYPE_NEWMARKET
	};

	SI16	siItemUnique;			// 대상 물품. 
	SI16	siItemNum;				// 거래개수. 

	GMONEY	siTradeMoney;			// 거래 대금. 	
	GMONEY	siFee;					// 지급 수수료. 
	UI08	uiType;

	cltLetterMsg_MarketTrade(SI16 itemunique, SI16 itemnum, GMONEY trademoney, GMONEY fee, UI08 type = cltLetterMsg_MarketTrade::TYPE_MARKET)
	{
		clHeader.siType = LETTERTYPE_MARKETTRADE;
		clHeader.siSize	= sizeof(cltLetterMsg_MarketTrade);

		siItemUnique	= itemunique;
		siItemNum		= itemnum;
		siTradeMoney	= trademoney;
		siFee			= fee;
		uiType			= type;
	}
};

//-------------------------------------------
// 사복시에서 말 팔림 통보. 
//-------------------------------------------
class cltLetterMsg_HorseMarketTrade{
public:
	cltLetterHeader clHeader;

	GMONEY siTradeMoney;			// 거래 대금. 	
	GMONEY siFee;					// 지급 수수료. 

	cltLetterMsg_HorseMarketTrade( GMONEY trademoney, GMONEY fee )
	{
		clHeader.siType = LETTERTYPE_HORSEMARKETTRADE;
		clHeader.siSize	= sizeof(cltLetterMsg_HorseMarketTrade);

		siTradeMoney	= trademoney;
		siFee			= fee;
	}
};


//-------------------------------------------
// 사복시에 올려 놓은 말 가격 인하 통보. 
//-------------------------------------------
class cltLetterMsg_HorseMarketPriceDown{
public:
	cltLetterHeader clHeader;

	TCHAR	szHorseName[ MAX_HORSE_NAME ];		
	GMONEY	siPrice;								// 인하된 금액. 

	cltLetterMsg_HorseMarketPriceDown( TCHAR *szhorsename, GMONEY price)
	{
		clHeader.siType = LETTERTYPE_HORSEMARKETPRICEDOWN;
		clHeader.siSize	= sizeof( cltLetterMsg_HorseMarketPriceDown );

		StringCchCopy( szHorseName, MAX_HORSE_NAME, szhorsename );
		siPrice = price;
	}
};


//-------------------------------------------
// 주식 처분 통지.쪽지. 
//-------------------------------------------
class cltLetterMsg_StockTrade{
public:
	cltLetterHeader clHeader;

	SI16 siVillageUnique;			// 대상 주식. 
	SI16 siOrderMode;

	SI32 siTradeAmount;				// 처분 수. 
	SI32 siLeftAmount;				// 남은 주식 수. 
	GMONEY siTradeMoney;				// 주식 처분으로 인해 총 획득 금액. 
	GMONEY siFee;						// 지불된 수수료. 

	cltLetterMsg_StockTrade(SI16 villageunique, SI16 ordermode, SI32 tradeamount, SI32 leftamount, GMONEY trademoney, GMONEY fee)
	{
		clHeader.siType = LETTERTYPE_STOCKTRADE;
		clHeader.siSize	= sizeof(cltLetterMsg_StockTrade);

		siVillageUnique = villageunique;
		siOrderMode		= ordermode;

		siTradeAmount	= tradeamount;
		siLeftAmount	= leftamount;
		siTradeMoney	= trademoney;
		siFee			= fee;
	}
};


//-------------------------------------------
// 시전임대료 지불  통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_PayRentFee{
public:
	cltLetterHeader clHeader;

	SI32 siVillageUnique;			// 시전이 있는 마을의 유니크. 
	SI32 siHouseIndex;				// 단위 시전의 주소. 
	GMONEY siRentFee;					// 임대료. 
	SI32 siDelayNum;				// 연체 횟수.
	GMONEY siDelayMoney;				// 연체금. 

	cltLetterMsg_PayRentFee(SI32 villageunique, SI32 houseindex, GMONEY rentfee, SI32 delaynum, GMONEY delaymoney)
	{
		clHeader.siType = LETTERTYPE_PAYRENTFEE;
		clHeader.siSize	= sizeof(cltLetterMsg_PayRentFee);

		siVillageUnique = villageunique;
		siHouseIndex	= houseindex;
		siRentFee		= rentfee;
		siDelayNum		= delaynum;
		siDelayMoney	= delaymoney;
	}
};

//-------------------------------------------
// 시전회수  통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_RecallHouse{
public:
	cltLetterHeader clHeader;

	SI32			siRecallReason;
	SI32			siVillageUnique;
	SI32			siHouseIndex;
	TCHAR			szOwnerName[MAX_PLAYER_NAME];

	cltLetterMsg_RecallHouse(SI32 reason, SI32 villageunique, SI32 houseindex, TCHAR* OwnerName = NULL)
	{
		clHeader.siType = LETTERTYPE_RECALLHOUSE;
		clHeader.siSize	= sizeof(cltLetterMsg_RecallHouse);

		siRecallReason	= reason;
		siVillageUnique	= villageunique;
		siHouseIndex	= houseindex;
		MStrCpy(szOwnerName,OwnerName,MAX_PLAYER_NAME);
	}
};

//-------------------------------------------
// 시전회수  임박 통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_HouseRecallWarning{
public:
	cltLetterHeader clHeader;

	SI16			siVillageUnique;
	SI16			siHouseIndex;
	SI16			siLeftMonth;		// 계약 종료시까지 남은 시간. (월)


	cltLetterMsg_HouseRecallWarning(SI32 villageunique, SI32 houseindex, SI32 leftmonth)
	{
		clHeader.siType = LETTERTYPE_HOUSERECALLWARINING;
		clHeader.siSize	= sizeof(cltLetterMsg_HouseRecallWarning);

		siVillageUnique	= villageunique;
		siHouseIndex	= houseindex;
		siLeftMonth		= leftmonth;
	}
};

//-------------------------------------------
// 시전 임차 허락 통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_RentHouse{
public:
	cltLetterHeader clHeader;

	SI16			siVillageUnique;
	SI16			siHouseIndex;

	cltLetterMsg_RentHouse(SI32 villageunique, SI32 houseindex)
	{
		clHeader.siType = LETTERTYPE_RENTHOUSE;
		clHeader.siSize	= sizeof(cltLetterMsg_RentHouse);

		siVillageUnique	= villageunique;
		siHouseIndex	= houseindex;
	}
};


//-------------------------------------------
// 임명 통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_BeRank{
public:
	cltLetterHeader clHeader;

	cltSimpleRank clRank;					// 임명된 신분. 
	cltDate clDate;							// 임명된 시간 

	UI08 uiGrank ;							// 군주나 6조판서 임명시 사용

	cltLetterMsg_BeRank( cltSimpleRank* pclrank ,cltDate* pcldate, UI08 grank = 0 )
	{
		clHeader.siType = LETTERTYPE_BERANK;
		clHeader.siSize	= sizeof(cltLetterMsg_BeRank);

		clRank.Set(pclrank);
		clDate.Set(pcldate);

		uiGrank = grank ;
	}
};

//-------------------------------------------
// 해고 통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_Fire{
public:
	cltLetterHeader clHeader;

	cltSimpleRank clRank;					// 해고당한 신분. 
	cltDate clDate;							// 해고당한 시간 

	UI08 uiGrank;							// 군주나 6조판서에서 해임될 시 사용

	cltLetterMsg_Fire(cltSimpleRank* pclrank ,cltDate* pcldate, UI08 grank = 0 )
	{
		clHeader.siType = LETTERTYPE_FIRE;
		clHeader.siSize	= sizeof(cltLetterMsg_Fire);

		clRank.Set(pclrank);
		clDate.Set(pcldate);

		uiGrank = grank ;
	}
};


//-------------------------------------------
// 주식 소각 통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_StockLoss{
public:
	cltLetterHeader clHeader;

	SI32		siVillageUnique;			// 마을 유니크
	SI32		siWinVillage;				// 승리 마을
	SI32		siLossPersent;				// 주식 소각 비율 
	SI32		siStockAmount;				// 주식 수량 

	cltLetterMsg_StockLoss( SI32 villageUnique, SI32 winVillage, SI32 lossPersent, SI32 stockAmount )
	{
		clHeader.siType = LETTERTYPE_STOCKLOSS;
		clHeader.siSize = sizeof( cltLetterMsg_StockLoss );

		siVillageUnique = villageUnique;
		siWinVillage = winVillage;
		siLossPersent = lossPersent;
		siStockAmount = stockAmount;
	}

};

//------------------------------------------
// 마을 배당시에 왕이 받게 되는 급여 통지 쪽지
//------------------------------------------
class cltLetterMsg_GetKingPay
{
public:
	cltLetterHeader clHeader;
	
	SI32			siVillageUnique;			// 배당 실시 마을 
	GMONEY			siKingPay;					// 왕이 배당으로 받은 돈 
	GMONEY			siPayPersent;				// 왕의 배당 수익 비율 
	
	cltLetterMsg_GetKingPay( SI32 villageUnique, GMONEY kingPay, GMONEY payPersent )
	{
		clHeader.siType = LETTERTYPE_KINGPAY;
		clHeader.siSize = sizeof( cltLetterMsg_GetKingPay );

		siVillageUnique = villageUnique;
		siKingPay = kingPay;
		siPayPersent = payPersent;
	}
};

//------------------------------------------
// 마을 주민 배당 통보 메시지
//------------------------------------------
class cltLetterMsg_ResidentShare
{
public:
	cltLetterHeader clHeader;

	SI32			siVillageUnique;			// 배당 실시 마을 
	SI32			siShareType;				// 배당 조건
	GMONEY			siShareMoney;				// 배당금 

	cltLetterMsg_ResidentShare( SI32 villageUnique, SI32 shareType, GMONEY shareMoney )
	{
		clHeader.siType = LETTERTYPE_RESIDENTSHARE;
		clHeader.siSize = sizeof( cltLetterMsg_ResidentShare );

		siVillageUnique = villageUnique;
		siShareType = shareType;
		siShareMoney = shareMoney;
	}

};

//------------------------------------------
// 건물 수리에 필요한 아이템을 행수에게 알린다
//------------------------------------------
class cltLetterMsg_StructureRepairItem
{
public:
	cltLetterHeader clHeader;

	SI32			siVillageUnique;		// 마을 
	SI32			siStrType;				// 건물 
	SI16			siItemUnique;			// 아이템 유니크
	SI16			siItemNum;				// 아이템 개수 

	cltLetterMsg_StructureRepairItem( SI32 villageUnique, SI32 strType, SI16 itemunique, SI16 itemnum )
	{
		clHeader.siType = LETTERTYPE_STRUCTUREREPAIRITEM;
		clHeader.siSize = sizeof( cltLetterMsg_StructureRepairItem );

		siVillageUnique = villageUnique;
		siStrType = strType;
		siItemUnique = itemunique;
		siItemNum = itemnum;
	}

};

//-------------------------------------------
// 거간에 올려 놓은 부동산 팔림
//-------------------------------------------
class cltLetterMsg_RealEstateMarketTrade
{
public:
	cltLetterHeader clHeader;

	SI32 siHouseVillage;		// 마을 
	SI32 siHouseIndex;			// 시전 번호 

	GMONEY siTradeMoney;			// 거래 대금. 	
	GMONEY siFee;					// 지급 수수료. 

	cltLetterMsg_RealEstateMarketTrade( SI32 housevillage, SI32 houseindex, GMONEY trademoney, GMONEY fee )
	{
		clHeader.siType = LETTERTYPE_REALESTATEMARKETTRADE;
		clHeader.siSize	= sizeof(cltLetterMsg_RealEstateMarketTrade);

		siHouseVillage	= housevillage;
		siHouseIndex	= houseindex;

		siTradeMoney	= trademoney;
		siFee			= fee;
	}
};

//-------------------------------------------
// 거간에 올려 놓은 부동산 주문 가격 인하 통보. 
//-------------------------------------------
class cltLetterMsg_RealEstateMarketPriceDown
{
public:
	cltLetterHeader clHeader;

	SI32	siHouseVillage;		// 마을 
	SI32	siHouseIndex;		// 시전 번호 

	GMONEY	siPrice;			// 인하된 금액. 

	cltLetterMsg_RealEstateMarketPriceDown( SI32 housevillage, SI32 houseindex, GMONEY price )
	{
		clHeader.siType = LETTERTYPE_REALESTATEMARKETPRICEDOWN;
		clHeader.siSize	= sizeof( cltLetterMsg_RealEstateMarketPriceDown );

		siHouseVillage = housevillage;
		siHouseIndex = houseindex;

		siPrice = price;
	}
};


//-------------------------------------------
// 장예원에 올려 놓은 소환수 팔림
//-------------------------------------------
class cltLetterMsg_SummonMarketTrade
{
public:
	cltLetterHeader clHeader;

	SI32 siKind;
	TCHAR szName[ MAX_SUMMON_NAME ];
	
	GMONEY siTradeMoney;			// 거래 대금. 	
	GMONEY siFee;					// 지급 수수료. 

	cltLetterMsg_SummonMarketTrade( SI32 kind, TCHAR *name, GMONEY trademoney, GMONEY fee )
	{
		clHeader.siType = LETTERTYPE_SUMMONMARKETTRADE;
		clHeader.siSize	= sizeof(cltLetterMsg_SummonMarketTrade);

		siKind = kind;
		if( name ) {
			StringCchCopy( szName, MAX_SUMMON_NAME, name );
		} else {
			szName[ 0 ] = NULL;
		}

		siTradeMoney	= trademoney;
		siFee			= fee;
	}
};

//-------------------------------------------
// 장예원에 올려 놓은 소환수 가격 인하 통보. 
//-------------------------------------------
class cltLetterMsg_SummonMarketPriceDown
{
public:
	cltLetterHeader clHeader;

	SI32	siKind;
	TCHAR	szName[ MAX_SUMMON_NAME ];

	GMONEY	siPrice;			// 인하된 금액. 

	cltLetterMsg_SummonMarketPriceDown( SI32 kind, TCHAR *name, GMONEY price )
	{
		clHeader.siType = LETTERTYPE_SUMMONMARKETPRICEDOWN;
		clHeader.siSize	= sizeof( cltLetterMsg_SummonMarketPriceDown );

		siKind = kind;

		if( name ) {
			StringCchCopy( szName, MAX_SUMMON_NAME, name );
		} else {
			szName[ 0 ] = NULL;
		}

		siPrice = price;
	}
};

class cltLetterMsg_ResidentOutDate
{
public:
	cltLetterHeader clHeader;

	SI32	siVillageUnique;

	cltLetterMsg_ResidentOutDate( SI32 villageunique )
	{
		clHeader.siType = LETTERTYPE_RESIDENTOUTDATE;
		clHeader.siSize	= sizeof( cltLetterMsg_ResidentOutDate );

		siVillageUnique = villageunique;
	}
};

class cltLetterMsg_ResultBidStock
{
public:
	cltLetterHeader clHeader;

	BOOL		bSuccessfulBid;				// 낙찰 되었는가 

	SI32		siVillageUnique;			// 증자 주식 마을 
	SI32		siStockAmount;				// 총 증자 주식 수 

	cltMoney	clBidMoney;					// 총 입찰 금액 
	BOOL		bExceedLimit;				// 통장 한도가 초과하였는가 


	cltLetterMsg_ResultBidStock( BOOL bsuccessfulbid, SI32 villageunique, SI32 stockamount, cltMoney *pclMoney, BOOL bexceedlimit )
	{
		clHeader.siType = LETTERTYPE_RESULTBIDSTOCK;
		clHeader.siSize = sizeof( cltLetterMsg_ResultBidStock );

		bSuccessfulBid = bsuccessfulbid;
		
		siVillageUnique = villageunique;
		siStockAmount = stockamount;

		clBidMoney.Set( pclMoney );

		bExceedLimit = bexceedlimit;
	};

};

//-------------------------------------------
// 농장 임차 허락 통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_RentLand{
public:
	cltLetterHeader clHeader;

	SI16			siVillageUnique;
	SI16			siLandIndex;

	cltLetterMsg_RentLand(SI32 villageunique, SI32 landindex)
	{
		clHeader.siType = LETTERTYPE_RENTLAND;
		clHeader.siSize	= sizeof(cltLetterMsg_RentLand);

		siVillageUnique	= villageunique;
		siLandIndex		= landindex;
	}
};


//-------------------------------------------
// 농장 임대료 지불  통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_PayLandRentFee{
public:
	cltLetterHeader clHeader;

	SI32 siVillageUnique;			// 시전이 있는 마을의 유니크. 
	SI32 siLandIndex;				// 단위 시전의 주소. 
	GMONEY siRentFee;					// 임대료. 
	SI32 siDelayNum;				// 연체 횟수.
	GMONEY siDelayMoney;				// 연체금. 

	cltLetterMsg_PayLandRentFee(SI32 villageunique, SI32 landindex, GMONEY rentfee, SI32 delaynum, GMONEY delaymoney)
	{
		clHeader.siType = LETTERTYPE_PAYLANDRENTFEE;
		clHeader.siSize	= sizeof(cltLetterMsg_PayLandRentFee);

		siVillageUnique = villageunique;
		siLandIndex		= landindex;
		siRentFee		= rentfee;
		siDelayNum		= delaynum;
		siDelayMoney	= delaymoney;
	}
};

//-------------------------------------------
// 농장회수  통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_LandRecall{
public:
	cltLetterHeader clHeader;

	SI32			siRecallReason;
	SI32			siVillageUnique;
	SI32			siLandIndex;

	cltLetterMsg_LandRecall(SI32 reason, SI32 villageunique, SI32 landindex)
	{
		clHeader.siType = LETTERTYPE_LANDRECALL;
		clHeader.siSize	= sizeof(cltLetterMsg_LandRecall);

		siRecallReason	= reason;
		siVillageUnique	= villageunique;
		siLandIndex	= landindex;
	}
};

//-------------------------------------------
// 농장회수  임박 통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_LandRecallWarning{
public:
	cltLetterHeader clHeader;

	SI16			siVillageUnique;
	SI16			siLandIndex;
	SI16			siLeftMonth;		// 계약 종료시까지 남은 시간. (월)


	cltLetterMsg_LandRecallWarning(SI32 villageunique, SI32 landindex, SI32 leftmonth)
	{
		clHeader.siType = LETTERTYPE_LANDRECALLWARNING;
		clHeader.siSize	= sizeof(cltLetterMsg_LandRecallWarning);

		siVillageUnique	= villageunique;
		siLandIndex	= landindex;
		siLeftMonth		= leftmonth;
	}
};

//--------------------------------------------
// 수호천사
//--------------------------------------------
class cltLetterMsg_HozoAngel
{
public:
	cltLetterHeader clHeader;

	SI32		siType ;

	cltDate		clDate;

	cltLetterMsg_HozoAngel(SI32 type,cltDate* pcldate)
	{
		clHeader.siType = LETTERTYPE_HOZOANGEL;
		clHeader.siSize	= sizeof(cltLetterMsg_HozoAngel);

		siType = type ;
		clDate.Set(pcldate);
	}
};

//--------------------------------------------
// 포교
//--------------------------------------------
class cltLetterMsg_HyungzoPolice
{
public:
	cltLetterHeader clHeader;

	SI32		siType ;

	cltDate		clDate;

	cltLetterMsg_HyungzoPolice(SI32 type,cltDate* pcldate)
	{
		clHeader.siType = LETTERTYPE_HYUNGZOPOLICE;
		clHeader.siSize	= sizeof(cltLetterMsg_HyungzoPolice);

		siType = type ;
		clDate.Set(pcldate);
	}
};

//-------------------------------------------
// 월드머니 거래 결과
//-------------------------------------------
class cltLetterMsg_WorldMoneyTradeResult{
public:
	cltLetterHeader clHeader;

	SI64 siWorldMoney;			// 거래된 아큐수
	SI64 siWorldMoneyPrice;		// 거래된 금액


	cltLetterMsg_WorldMoneyTradeResult(SI64 WorldMoney, SI64 WorldMoneyPrice)
	{
		clHeader.siType = LETTERTYPE_WORLDMONEYTRADERESULT;
		clHeader.siSize	= sizeof(cltLetterMsg_WorldMoneyTradeResult);

		siWorldMoney = WorldMoney;
		siWorldMoneyPrice = WorldMoneyPrice;
	}
};

// 무역 거래
class cltLetterMsg_TradeServerSell
{
public:
	cltLetterHeader clHeader;

	SI16 siItemUnique;			// 대상 물품. 
	SI16 siItemNum;				// 거래개수. 

	SI64 siTradeMoney;			// 거래 대금. 	

	TCHAR				szBuyerServer[20] ; // 산 사람이 있는 서버
	TCHAR				szBuyerName[20] ; // 산 사람의 이름

	cltLetterMsg_TradeServerSell(SI16 itemunique, SI16 itemnum, SI64 trademoney,TCHAR*buyserver,TCHAR*buyname)
	{
		clHeader.siType = LETTERTYPE_TRADESERVER_SELL;
		clHeader.siSize	= sizeof(cltLetterMsg_TradeServerSell);

		siItemUnique	= itemunique;
		siItemNum		= itemnum;
		siTradeMoney	= trademoney;

		MStrCpy(szBuyerServer,buyserver,20);
		MStrCpy(szBuyerName,buyname,20);
	}

};

class cltLetterMsg_WorldMoneyReducePrice
{
public:
	cltLetterHeader clHeader;

	SI64 siPrice;			
	SI64 siWorldMoney;		
	SI32 siDate;


	cltLetterMsg_WorldMoneyReducePrice( SI64 Price, SI64 WorldMoney, SI32 Date )
	{
		clHeader.siType = LETTERTYPE_WORLDMONEY_REDUCE_PRICE;
		clHeader.siSize	= sizeof(cltLetterMsg_WorldMoneyReducePrice);

		siPrice				= Price;
		siWorldMoney		= WorldMoney;
		siDate				= Date;
	}

};

//--------------------------------------------
// 내가 보유중인 주식의 마을에서 주총 진행중. 
//--------------------------------------------
class cltLetterMsg_GeneralMeeting
{
public:
	cltLetterHeader clHeader;

	SI16		siVillageUnique;

	cltLetterMsg_GeneralMeeting(SI16 villageunique)
	{
		clHeader.siType = LETTERTYPE_GENERALMEETING;
		clHeader.siSize	= sizeof(cltLetterMsg_GeneralMeeting);

		siVillageUnique = villageunique;
	}
};

//-------------------------------------------
// 비변사에 올려 놓은 소환영웅 팔림
//-------------------------------------------
class cltLetterMsg_SummonHeroMarketTrade
{
public:
	cltLetterHeader clHeader;

	SI32 siKind;
	TCHAR szName[ MAX_SUMMON_NAME ];
	
	GMONEY siTradeMoney;			// 거래 대금. 	
	GMONEY siFee;					// 지급 수수료. 

	cltLetterMsg_SummonHeroMarketTrade( SI32 kind, TCHAR *name, GMONEY trademoney, GMONEY fee )
	{
		clHeader.siType = LETTERTYPE_SUMMONHEROMARKETTRADE;
		clHeader.siSize	= sizeof(cltLetterMsg_SummonHeroMarketTrade);

		siKind = kind;
		if( name ) {
			StringCchCopy( szName, MAX_SUMMON_NAME, name );
		} else {
			szName[ 0 ] = NULL;
		}

		siTradeMoney	= trademoney;
		siFee			= fee;
	}
};

//-------------------------------------------
// 비변사에 올려 놓은 소환영웅 가격 인하 통보. 
//-------------------------------------------
class cltLetterMsg_SummonHeroMarketPriceDown
{
public:
	cltLetterHeader clHeader;

	SI32	siKind;
	TCHAR	szName[ MAX_SUMMON_NAME ];

	GMONEY	siPrice;			// 인하된 금액. 

	cltLetterMsg_SummonHeroMarketPriceDown( SI32 kind, TCHAR *name, GMONEY price )
	{
		clHeader.siType = LETTERTYPE_SUMMONHEROMARKETPRICEDOWN;
		clHeader.siSize	= sizeof( cltLetterMsg_SummonHeroMarketPriceDown );

		siKind = kind;

		if( name ) {
			StringCchCopy( szName, MAX_SUMMON_NAME, name );
		} else {
			szName[ 0 ] = NULL;
		}

		siPrice = price;
	}
};

//-------------------------------------------
// 역참에 보낸 물품이 도착했을 때 통보
//-------------------------------------------
class cltLetterMsg_PostOfficeSendOk
{
public:
	cltLetterHeader clHeader;
	
	SI16	siType;

	TCHAR	szRecvPersonName[ MAX_PLAYER_NAME ];
	SI16	siItemUnique;
	SI16	siItemNum;
	GMONEY	siMoney;

	cltLetterMsg_PostOfficeSendOk( SI16 type, TCHAR *szName, SI16 itemunique, SI16 itemnum, GMONEY money )
	{
		clHeader.siType = LETTERTYPE_POSTOFFICE_SENDOK;
		clHeader.siSize = sizeof( cltLetterMsg_PostOfficeSendOk );

		siType = type;
		MStrCpy( szRecvPersonName, szName, MAX_PLAYER_NAME );
		siItemUnique = itemunique;
		siItemNum = itemnum;
		siMoney = money;
	}
};

//-------------------------------------------
// 광산 임차 허락 통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_RentMine{
public:
	cltLetterHeader clHeader;

	SI16			siVillageUnique;
	SI16			siMineIndex;

	cltLetterMsg_RentMine(SI32 villageunique, SI32 landindex)
	{
		clHeader.siType = LETTERTYPE_RENTMINE;
		clHeader.siSize	= sizeof(cltLetterMsg_RentMine);

		siVillageUnique	= villageunique;
		siMineIndex		= landindex;
	}
};


//-------------------------------------------
// 광산 임대료 지불  통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_PayMineRentFee{
public:
	cltLetterHeader clHeader;

	SI32 siVillageUnique;			// 광산이 있는 마을의 유니크. 
	SI32 siMineIndex;				// 단위 광산의 주소. 
	GMONEY siRentFee;					// 임대료. 
	SI32 siDelayNum;				// 연체 횟수.
	GMONEY siDelayMoney;				// 연체금. 

	cltLetterMsg_PayMineRentFee(SI32 villageunique, SI32 landindex, GMONEY rentfee, SI32 delaynum, GMONEY delaymoney)
	{
		clHeader.siType = LETTERTYPE_PAYMINERENTFEE;
		clHeader.siSize	= sizeof(cltLetterMsg_PayMineRentFee);

		siVillageUnique = villageunique;
		siMineIndex		= landindex;
		siRentFee		= rentfee;
		siDelayNum		= delaynum;
		siDelayMoney	= delaymoney;
	}
};

//-------------------------------------------
// 광산회수  통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_MineRecall{
public:
	cltLetterHeader clHeader;

	SI32			siRecallReason;
	SI32			siVillageUnique;
	SI32			siMineIndex;

	cltLetterMsg_MineRecall(SI32 reason, SI32 villageunique, SI32 landindex)
	{
		clHeader.siType = LETTERTYPE_MINERECALL;
		clHeader.siSize	= sizeof(cltLetterMsg_MineRecall);

		siRecallReason	= reason;
		siVillageUnique	= villageunique;
		siMineIndex	= landindex;
	}
};

//-------------------------------------------
// 광산회수  임박 통지 쪽지. 
//-------------------------------------------
class cltLetterMsg_MineRecallWarning{
public:
	cltLetterHeader clHeader;

	SI16			siVillageUnique;
	SI16			siMineIndex;
	SI16			siLeftMonth;		// 계약 종료시까지 남은 시간. (월)


	cltLetterMsg_MineRecallWarning(SI32 villageunique, SI32 landindex, SI32 leftmonth)
	{
		clHeader.siType = LETTERTYPE_MINERECALLWARNING;
		clHeader.siSize	= sizeof(cltLetterMsg_MineRecallWarning);

		siVillageUnique	= villageunique;
		siMineIndex	= landindex;
		siLeftMonth		= leftmonth;
	}
};

//-------------------------------------------
// 시전 연장하지 못해서 시스템에서 아이템을 보관하고 있다는 메세지 
//-------------------------------------------
class cltLetterMsg_Existent_SaveUserItem{
public:
	cltLetterHeader clHeader;

	cltLetterMsg_Existent_SaveUserItem()
	{
		clHeader.siType = LETTERTYPE_EXISTENT_SAVEUSERITEM;
		clHeader.siSize	= sizeof(cltLetterMsg_Existent_SaveUserItem);
	}
};


//-------------------------------------------
// 주식 공모 추첨 결과 통보
//-------------------------------------------
class cltLetterMsg_StockOfferingResult
{
public:
	cltLetterHeader clHeader;

	SI32			siType;
	SI32			siVillageUnique;
	SI32			siStockAmount;
	GMONEY			siPrice;


	cltLetterMsg_StockOfferingResult( SI32 type, SI32 villageunique, SI32 stockamount, GMONEY price )
	{
		clHeader.siType = LETTERTYPE_STOCKOFFERING_RESULT;
		clHeader.siSize	= sizeof(cltLetterMsg_StockOfferingResult);

		siType			= type;
		siVillageUnique	= villageunique;
		siStockAmount	= stockamount;
		siPrice			= price;
	}
};


//-------------------------------------------
// 추천 점수 상승 알림 통보.
//-------------------------------------------
class cltLetterMsg_ChangeRecommendPoint{
public:
	cltLetterHeader clHeader;

	TCHAR			szCharName[MAX_PLAYER_NAME];
	SI32			siLevel;
	SI32			siChangedRecommendPoint;	// 추가된 추천 점수
	SI32			siRecommentPoint;	// 최종 추천 점수
	
	cltLetterMsg_ChangeRecommendPoint( TCHAR * CharName, SI32 Level ,SI32	ChangedRecommendPoint, SI32	RecommentPoint)
	{
		clHeader.siType = LETTERTYPE_CHANGERECOMMENDPOINT;
		clHeader.siSize	= sizeof(cltLetterMsg_ChangeRecommendPoint);
		
		StringCchCopy( szCharName, MAX_PLAYER_NAME, CharName);
		siLevel = Level;	
		siChangedRecommendPoint = ChangedRecommendPoint;
		siRecommentPoint	= RecommentPoint;
	}
};

//-------------------------------------------
// 추천 포상 아이템 지급 알림 통보.
//-------------------------------------------
class cltLetterMsg_LevelUpREcommandRewardForRecommender{
public:
	cltLetterHeader clHeader;

	TCHAR			szCharName[MAX_PLAYER_NAME];
	SI32			siLevel;
	SI32			siRewardItemUnique;	// 추가된 추천 점수
	SI32			siRewardItemNum;	// 최종 추천 점수

	cltLetterMsg_LevelUpREcommandRewardForRecommender( TCHAR * CharName, SI32 Level ,SI32 RewardItemUnique, SI32 RewardItemNum)
	{
		clHeader.siType = LETTERTYPE_LEVELUPRECOMMANDREWARD_FOR_RECOMMENDER;
		clHeader.siSize	= sizeof(cltLetterMsg_LevelUpREcommandRewardForRecommender);

		StringCchCopy( szCharName, MAX_PLAYER_NAME, CharName);
		siLevel = Level;	
		siRewardItemUnique = RewardItemUnique;
		siRewardItemNum	= RewardItemNum;
	}
};



//-------------------------------------------
// 추천인 등록시 레벨업 보상
//-------------------------------------------
class cltLetterMsg_LevelUpREcommandReward{
public:
	cltLetterHeader clHeader;

	SI32			siLevel;
	SI32			siRewardItemUnique;	// 추가된 추천 점수
	SI32			siRewardItemNum;	// 최종 추천 점수

	cltLetterMsg_LevelUpREcommandReward( SI32 Level , SI32	RewardItemUnique, SI32	RewardItemNum)
	{
		clHeader.siType = LETTERTYPE_LEVELUPRECOMMANDREWARD;
		clHeader.siSize	= sizeof(cltLetterMsg_LevelUpREcommandReward);

		siLevel = Level;	
		siRewardItemUnique = RewardItemUnique;
		siRewardItemNum	= RewardItemNum;
	}
};


//-------------------------------------------
// 길드 가입 쪽지
//-------------------------------------------
class cltLetterMsg_GuildJoin{
public:
	cltLetterHeader clHeader;

	BOOL m_bJoin;

	cltLetterMsg_GuildJoin( BOOL bJoin)
	{
		clHeader.siType = LETTERTYPE_GUILDJOIN;
		clHeader.siSize	= sizeof(cltLetterMsg_GuildJoin);

		m_bJoin = bJoin;
	}
};

//-----------------------------------------------
// 길드 생성에 대한 것
//----------------------------------------------
class cltLetterMsg_Guild_ConfirmCreateGuild
{
public:
	cltLetterHeader clHeader;

	BOOL bConfirm;
	GMONEY siMoney;

	cltLetterMsg_Guild_ConfirmCreateGuild(BOOL confirm,GMONEY money)
	{
		clHeader.siType = LETTERTYPE_GUILD_CONFIRMCREATEGUILD;
		clHeader.siSize = sizeof(cltLetterMsg_Guild_ConfirmCreateGuild);

		bConfirm = confirm ;
		siMoney = money ;
	}
};

class cltLetterMsg_Guild_ConfirmChangeVillage
{
public:
	cltLetterHeader clHeader;

	BOOL bConfirm;

	cltLetterMsg_Guild_ConfirmChangeVillage(BOOL confirm)
	{
		clHeader.siType = LETTERTYPE_GUILD_CONFIRMCREATEGUILD;
		clHeader.siSize = sizeof(cltLetterMsg_Guild_ConfirmCreateGuild);

		bConfirm = confirm ;
	}
};

//-----------------------------------------------
// 길드 오피서 임명에 대한 것
//----------------------------------------------
class cltLetterMsg_Guild_SecondMaster
{
public:
	cltLetterHeader clHeader;

	BOOL bConfirm;

	cltLetterMsg_Guild_SecondMaster(BOOL confirm)
	{
		clHeader.siType = LETTERTYPE_GUILD_SECONDMASTER;
		clHeader.siSize = sizeof(cltLetterMsg_Guild_SecondMaster);

		bConfirm = confirm ;
	}
};
//-----------------------------------------------
// 길드 마스터 변경
//----------------------------------------------
class cltLetterMsg_Guild_ChangeMaster
{
public:
	cltLetterHeader clHeader;

	cltLetterMsg_Guild_ChangeMaster()
	{
		clHeader.siType = LETTERTYPE_GUILD_CHANGEMASTER;
		clHeader.siSize = sizeof(cltLetterMsg_Guild_ChangeMaster);
	}

};
//-----------------------------------------------
// 길드 마스터 변경
//----------------------------------------------
class cltLetterMsg_Guild_ChangeMaster_Notconnect_Month
{
public:
	cltLetterHeader clHeader;

	TCHAR			szCharName[MAX_PLAYER_NAME];

	cltLetterMsg_Guild_ChangeMaster_Notconnect_Month( TCHAR* pszCharName )
	{
		clHeader.siType = LETTERTYPE_GUILD_CHANGEMASTER_NOTCONNECT_MONTH;
		clHeader.siSize = sizeof(cltLetterMsg_Guild_ChangeMaster_Notconnect_Month);

		if ( pszCharName )
		{
			StringCchCopy( szCharName, MAX_PLAYER_NAME, pszCharName);
		}
		else
		{
			StringCchCopy( szCharName, MAX_PLAYER_NAME, TEXT("") );
		}
	}

};

//------------------------------------------------
// 길드가 파괴되었음
//------------------------------------------------
class cltLetterMsg_Guild_Destroy
{
public:
	cltLetterHeader clHeader;
	SI08			siReason;

	cltLetterMsg_Guild_Destroy(SI08 reason)
	{
		clHeader.siType = LETTERTYPE_GUILD_DESTROY;
		clHeader.siSize = sizeof(cltLetterMsg_Guild_Destroy);

		siReason = reason;
	}

};


//------------------------------------------------
// 길드전이 예약 되었음
//------------------------------------------------
class cltLetterMsg_GuildWar_SuccessReserve
{
public:
	cltLetterHeader clHeader;

	SI32	siFieldIndex;
	SI32	siRoomIndex;
	TCHAR	szHomeGuild[MAX_GUILD_NAME];
	TCHAR	szAwayGuild[MAX_GUILD_NAME];

	cltLetterMsg_GuildWar_SuccessReserve( SI32 fieldindex, SI32 roomindex, TCHAR* homeguild, TCHAR* awayguild)
	{
		clHeader.siType = LETTERTYPE_GUILD_SUCCESSRESERVE;
		clHeader.siSize = sizeof(cltLetterMsg_GuildWar_SuccessReserve);

		siFieldIndex = fieldindex;
		siRoomIndex = roomindex;
		StringCchCopy( szHomeGuild, MAX_GUILD_NAME, homeguild );
		StringCchCopy( szAwayGuild, MAX_GUILD_NAME, awayguild );
	}
};
//------------------------------------------------
// 새로운 길드 사냥터 소유 방식 길드전 신청 했음
//------------------------------------------------
class cltLetterMsg_GuildDungeon_Apply_New
{
public:
	cltLetterHeader	clHeader;

	SI32	siMapIndex;

	cltLetterMsg_GuildDungeon_Apply_New( SI32 mapindex )
	{
		clHeader.siType = LETTERTYPE_GUILDDUNGEON_APPLY_NEW;
		clHeader.siSize = sizeof(cltLetterMsg_GuildDungeon_Apply_New);

		siMapIndex = mapindex;
	}
};
//------------------------------------------------
// 길드 사냥터 소유권 도전 신청했음
//------------------------------------------------
class cltLetterMsg_GuildDungeon_Apply
{
public:
	cltLetterHeader	clHeader;

	SI32	siMapIndex;

	cltLetterMsg_GuildDungeon_Apply( SI32 mapindex )
	{
		clHeader.siType = LETTERTYPE_GUILDDUNGEON_APPLY;
		clHeader.siSize = sizeof(cltLetterMsg_GuildDungeon_Apply);

		siMapIndex = mapindex;
	}
};
//-------------------------------------------------
// 사냥터 소유권에 의한 GP, Money 획득
//-------------------------------------------------
class cltLetterMsg_GuildDungeon_ChangeGPMoney
{
public:
	cltLetterHeader clHeader;

	SI32	siAddGP;
	GMONEY	siAddMoney;
	SI32	siItemUnique;
	SI32	siItemNum;

	cltLetterMsg_GuildDungeon_ChangeGPMoney( SI32 gp, GMONEY money, SI32 itemunique, SI32 itemnum )
	{
		clHeader.siType = LETTERTYPE_GUILDDUNGEON_CHANGEGPMONEY;
		clHeader.siSize = sizeof(cltLetterMsg_GuildDungeon_ChangeGPMoney);

		siAddGP = gp;
		siAddMoney = money;

		siItemUnique = itemunique;
		siItemNum = itemnum;
	}
};
//-------------------------------------------------
// 사냥터 소유권획득
//-------------------------------------------------
class cltLetterMsg_GuildDungeon_GetOwnerShip
{
public:
	cltLetterHeader clHeader;
	SI32			siMapIndex;

	cltLetterMsg_GuildDungeon_GetOwnerShip( SI32 mapindex )
	{
		clHeader.siType = LETTERTYPE_GUILDDUNGEON_GETOWNERSHIP;
		clHeader.siSize = sizeof(cltLetterMsg_GuildDungeon_GetOwnerShip);

		siMapIndex = mapindex;
	}
};

class cltLetterMsg_RenouncePupil
{
public:
	cltLetterHeader clHeader;
	TCHAR	szTeacherName[MAX_PLAYER_NAME];
    
	cltLetterMsg_RenouncePupil( TCHAR* pTeacherName )
	{
		clHeader.siType = LETTERTYPE_RENOUNCEPUPIL;
		clHeader.siSize = sizeof(cltLetterMsg_RenouncePupil);

		MStrCpy( szTeacherName, pTeacherName, MAX_PLAYER_NAME );
	}
};

class cltLetterMsg_AcceptFriend_Result
{
public:
	cltLetterHeader clHeader;
	enum { ACCEPTFRIEND = 0, REJECTFRIEND };
	UI08	uiType;
	TCHAR	szPersonName[MAX_PLAYER_NAME];

	cltLetterMsg_AcceptFriend_Result( UI08 Type, TCHAR* pPersonName )
	{
		clHeader.siType = LETTERTYPE_ACCEPTFRIEND_RESULT;
		clHeader.siSize = sizeof(cltLetterMsg_AcceptFriend_Result);

		uiType = Type;
		MStrCpy( szPersonName, pPersonName, MAX_PLAYER_NAME );
	}
};

class cltLetterMsg_TeacherReward_PupilReward
{
public:
	cltLetterHeader clHeader;
	enum { TEACHER = 0, PUPIL };
	UI08	uiType;
	UI08	uiTiredState;
	TCHAR   szPersonName[MAX_PLAYER_NAME];
	SI32	siLevel;
	SI32	siItemUnique;
	SI32	siItemNum;
	
	cltLetterMsg_TeacherReward_PupilReward( UI08 Type, UI08 TiredState, TCHAR* pPersonName, SI32 Level, SI32	ItemUnique, SI32 ItemNum )
	{
		clHeader.siType = LETTERTYPE_TEACHERREWARD_PUPILREWARD;
		clHeader.siSize = sizeof(cltLetterMsg_TeacherReward_PupilReward);

		uiType = Type;
		uiTiredState = TiredState;
		MStrCpy( szPersonName, pPersonName, MAX_PLAYER_NAME );
		siLevel = Level;
		siItemUnique = ItemUnique;
        siItemNum = ItemNum;
	}
};

class cltLetterMsg_PVPResult
{
public:
	cltLetterHeader clHeader;

	enum { WIN = 0, LOSE, VILLAGE, GUILD };
	bool	bWin;
	UI08	uiType;
	TCHAR	szWinUserName[MAX_PLAYER_NAME];
	TCHAR   szLoseUserName[MAX_PLAYER_NAME];
	// 빼앗긴 아이템 이름
	bool	bDestroyItem;
	SI16	siItemUnique;
	
	cltLetterMsg_PVPResult( bool Win, UI08 Type, TCHAR* WinUserName, TCHAR* LoseUserName, bool	DestroyItem, SI16 ItemUnique )
	{
		clHeader.siType = LETTERTYPE_PVPRESULT;
		clHeader.siSize = sizeof(cltLetterMsg_PVPResult);

		bWin = Win;
		uiType = Type;
		MStrCpy( szWinUserName, WinUserName, MAX_PLAYER_NAME );
		MStrCpy( szLoseUserName, LoseUserName, MAX_PLAYER_NAME );
		bDestroyItem = DestroyItem;
		siItemUnique = ItemUnique;
	}
};

class cltLetterMsg_Guild_AlertDestroy
{
public:
	cltLetterHeader clHeader;

	cltLetterMsg_Guild_AlertDestroy()
	{
		clHeader.siType = LETTERTYPE_GUILD_ALERTDESTROY;
		clHeader.siSize = sizeof(cltLetterMsg_Guild_AlertDestroy);
	}
};


class cltLetterMsg_GiveSystemReward
{
public:

	cltLetterHeader clHeader;

	cltSystemRewardInfo clInfo;

	cltLetterMsg_GiveSystemReward( cltSystemRewardInfo* pclinfo )
	{
		clHeader.siType = LETTERTYPE_SYSTEMREWARD_GIVE;
		clHeader.siSize = sizeof(cltLetterMsg_GiveSystemReward);

		clInfo.Set( pclinfo );
	}
};


class cltLetterUnit{
public:

	union{
		BYTE cData[MAX_LETTER_SIZE];
		SI32 siData[MAX_LETTER_SIZE/4];
	};
	


	cltLetterUnit()
	{
		ZeroMemory(cData, MAX_LETTER_SIZE);
	}

	cltLetterUnit(cltLetterHeader* pdata)
	{
		if(pdata->siSize > MAX_LETTER_SIZE)
		{
			TCHAR buffer[256];
			StringCchPrintf(buffer, 256, TEXT("Exceed: size:%d type:%d"), pdata->siSize, pdata->siType);
			MessageBox(NULL, TEXT("cltLetterUnit"), buffer, MB_OK|MB_TOPMOST);
		}
		else
		{
			memcpy(cData, (BYTE*)pdata, pdata->siSize);										//ok
		}

	}

	void Set(cltLetterUnit* pclletter)
	{
		memcpy(cData, pclletter->cData, MAX_LETTER_SIZE);
	}
	
	bool GetText(TCHAR* buffer, SI16 txtSize );
	
};





class cltLetter{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltLetterUnit, MAX_LETTER_NUMBER>		clLetterUnit;
#else
	cltLetterUnit clLetterUnit[MAX_LETTER_NUMBER];
#endif

	cltLetter(){}
	~cltLetter(){}
};

//////////////////////////////////////////////////////////////////////////
// 계정을 대상으로한 편지
//////////////////////////////////////////////////////////////////////////




class cltAccountLetterUnit{
public:

	union{
		BYTE cData[MAX_ACCOUNTLETTER_SIZE];
		SI32 siData[MAX_ACCOUNTLETTER_SIZE/4];
	};



	cltAccountLetterUnit()
	{
		ZeroMemory(cData, MAX_ACCOUNTLETTER_SIZE);
	}

	cltAccountLetterUnit(cltLetterHeader* pdata)
	{
		if(pdata->siSize > MAX_ACCOUNTLETTER_SIZE)
		{


#ifdef _DEBUG
			TCHAR buffer[256];
			StringCchPrintf(buffer, 256, TEXT("Exceed: size:%d type:%d"), pdata->siSize, pdata->siType);
			MessageBox(NULL, TEXT("cltLetterUnit"), buffer, MB_OK|MB_TOPMOST);
#endif
		}
		else


		{
			memcpy(cData, (BYTE*)pdata, pdata->siSize);										//ok
		}

	}

	void Set(cltAccountLetterUnit* pclletter)
	{
		memcpy(cData, pclletter->cData, MAX_ACCOUNTLETTER_SIZE);
	}

	bool GetText(TCHAR* buffer, SI16 txtSize );

};





class cltAccountLetter{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltAccountLetterUnit, MAX_LETTER_NUMBER>		clLetterUnit;
#else
	cltAccountLetterUnit clLetterUnit[MAX_LETTER_NUMBER];
#endif

	cltAccountLetter(){}
	~cltAccountLetter(){}
};


//-------------------------------------------
// 선물을 받았다고 알려주는 메시지
//-------------------------------------------

class cltLetterMsg_Present
{
public:

	cltLetterHeader clHeader;

	SI16 siFromServerUnique;
    TCHAR szFromPlayerName[MAX_PLAYER_NAME];
	
	TCHAR szComment[200];
	TCHAR szItemName[50];
	SI32  siItemNum;
	
	SI32 siItemImageFont;
	
	TCHAR szFromPlayerAccount[MAX_PLAYER_NAME];

	cltLetterMsg_Present( SI16 FromServerUnique,TCHAR * FromPlayerName, TCHAR * Comment, 
		TCHAR * ItemName, SI32 ItemNum , SI32 ItemImageFont , TCHAR * FromPlayerAccount )
	{
		clHeader.siType = ACCOUNT_LETTERTYPE_PRESENT;
		clHeader.siSize	= sizeof(cltLetterMsg_Present);	

		siFromServerUnique = FromServerUnique;
		MStrCpy(szFromPlayerName,FromPlayerName,MAX_PLAYER_NAME);
		MStrCpy(szComment,Comment,200);
		MStrCpy(szItemName,ItemName,50);
		siItemNum = ItemNum;
		siItemImageFont = ItemImageFont; 
		MStrCpy(szFromPlayerAccount, FromPlayerAccount, MAX_PLAYER_NAME );
	}
};


class cltLetterMsg_PresentAnswer
{
public:

	cltLetterHeader clHeader;

	SI16 siFromServerUnique;
	TCHAR szFromPlayerName[MAX_PLAYER_NAME];
	TCHAR szComment[200];

	cltLetterMsg_PresentAnswer( SI16 FromServerUnique, TCHAR * FromPlayerName, TCHAR * Comment)
	{
		clHeader.siType = ACCOUNT_LETTERTYPE_PRESENTANSWER;
		clHeader.siSize	= sizeof(cltLetterMsg_PresentAnswer);	

		siFromServerUnique = FromServerUnique;
		MStrCpy(szFromPlayerName,FromPlayerName,MAX_PLAYER_NAME);
		MStrCpy(szComment,Comment,200);
	}
};

class cltLetterMsg_Invite_Applyresident
{
public:
	cltLetterHeader clHeader;

	TCHAR			szMentorName[MAX_PLAYER_NAME];
	TCHAR			szDiscipleName[MAX_PLAYER_NAME];

	cltLetterMsg_Invite_Applyresident( TCHAR* pMentorName, TCHAR* pDiscipleName)
	{
		clHeader.siType = LETTERTYPE_INVITE_APPLYRESIDENT;
		clHeader.siSize	= sizeof(cltLetterMsg_Invite_Applyresident);	
	
		memcpy(szMentorName,   pMentorName,   sizeof(szMentorName));
		memcpy(szDiscipleName, pDiscipleName, sizeof(szDiscipleName));
	}
};

//-------------------------------------------
// 캐릭터 생성으로 보상아이템 받음
//-------------------------------------------
class cltLetterMsg_CreateCharReward
{
public:
	cltLetterHeader clHeader;

	cltLetterMsg_CreateCharReward( void )
	{
		clHeader.siType = LETTERTYPE_CREATECHARREWARD;
		clHeader.siSize	= sizeof(cltLetterMsg_CreateCharReward);
	}
};

//-------------------------------------------
// 캐릭터 생성으로 보상아이템 받음
//-------------------------------------------
class cltLetterMsg_WeddingCeremony_Cancel
{
public:
	cltLetterHeader	clHeader;

	cltLetterMsg_WeddingCeremony_Cancel( void )
	{
		clHeader.siType = LETTERTYPE_WEDDINGCEREMONY_CANCEL;
		clHeader.siSize	= sizeof(cltLetterMsg_WeddingCeremony_Cancel);
	}
};

//-------------------------------------------
// 가족에서 탈퇴 당했다.
//-------------------------------------------
class cltLetterMsg_DelLetter
{
public:
	cltLetterHeader	clHeader;

	cltLetterMsg_DelLetter( void )
	{
		clHeader.siType = LETTERTYPE_FAMILY_DEL_FAMILY;
		clHeader.siSize	= sizeof(cltLetterMsg_DelLetter);
	}
};
//-------------------------------------------
// 가족을 나갔다.
//-------------------------------------------
class cltLetterMsg_LeaveFamilyLetter
{
public:
	cltLetterHeader	clHeader;

	TCHAR			szName[MAX_PLAYER_NAME];

	cltLetterMsg_LeaveFamilyLetter( TCHAR* pName )
	{
		clHeader.siType = LETTERTYPE_FAMILY_LEAVE_FAMILY;
		clHeader.siSize	= sizeof(cltLetterMsg_LeaveFamilyLetter);
		memcpy(szName,   pName,   sizeof(szName));	// 나간 사람 이름
	}
};
//-------------------------------------------
// 해당 케릭터가 가족에 등록 되었다.-기존 가족들에게 날린다
//-------------------------------------------
class cltLetterMsg_ADDFamilyLetter_ToFamily
{
public:
	cltLetterHeader	clHeader;

	TCHAR			szParentName_1[MAX_PLAYER_NAME];
	TCHAR			szParentName_2[MAX_PLAYER_NAME];
	TCHAR			szAddCharName[MAX_PLAYER_NAME];

	cltLetterMsg_ADDFamilyLetter_ToFamily( TCHAR* pParentName_1,TCHAR* pParentName_2,TCHAR* pAddCharName )
	{
		clHeader.siType = LETTERTYPE_FAMILY_ADD_FAMILY;
		clHeader.siSize	= sizeof(cltLetterMsg_ADDFamilyLetter_ToFamily);
		memcpy(szParentName_1,   pParentName_1,   sizeof(szParentName_1));	// 나간 사람 이름
		memcpy(szParentName_2,   pParentName_2,   sizeof(szParentName_2));	// 나간 사람 이름
		memcpy(szAddCharName,   pAddCharName,   sizeof(szAddCharName));	// 나간 사람 이름
	}
};
//-------------------------------------------
// 해당 케릭터가 가족에 등록 되었다.-자기 자신에게 날린다
//-------------------------------------------
class cltLetterMsg_ADDFamilyLetter_ToFamily_To_Me
{
public:
	cltLetterHeader	clHeader;

	TCHAR			szParentName_1[MAX_PLAYER_NAME];
	TCHAR			szParentName_2[MAX_PLAYER_NAME];
	TCHAR			szAddCharName[MAX_PLAYER_NAME];

	cltLetterMsg_ADDFamilyLetter_ToFamily_To_Me( TCHAR* pParentName_1,TCHAR* pParentName_2,TCHAR* pAddCharName )
	{
		clHeader.siType = LETTERTYPE_FAMILY_ADD_FAMILY_TO_ME;
		clHeader.siSize	= sizeof(cltLetterMsg_ADDFamilyLetter_ToFamily_To_Me);
		memcpy(szParentName_1,   pParentName_1,   sizeof(szParentName_1));	// 나간 사람 이름
		memcpy(szParentName_2,   pParentName_2,   sizeof(szParentName_2));	// 나간 사람 이름
		memcpy(szAddCharName,   pAddCharName,   sizeof(szAddCharName));	// 나간 사람 이름
	}
};
//-------------------------------------------
// 휴면 계정 종료 경고
//-------------------------------------------
class cltLetterMsg_DormancyWaringExpiration
{
public:
	cltLetterHeader clHeader;

	SI16 m_siDate;			// 남은 기간(단위 : 일)

	cltLetterMsg_DormancyWaringExpiration( SI16 siDate)
	{
		clHeader.siType = LETTERTYPE_DORMANCY_WARING_EXPIRATION;
		clHeader.siSize	= sizeof(cltLetterMsg_DormancyWaringExpiration);

		m_siDate = siDate;
	}
};

// 스킬북 관련된 마법 삭제 알림.
class cltLetterMsg_SkillBookMagicDel
{
public:
	cltLetterHeader clHeader;

	SI32			m_siMagicKind;

	cltLetterMsg_SkillBookMagicDel( SI32 _siMagicKind )
	{
		clHeader.siType = LETTERTYPE_SKILLBOOK_MAGIC_DEL;
		clHeader.siSize	= sizeof(cltLetterMsg_SkillBookMagicDel);

		m_siMagicKind = _siMagicKind;
	}
};


#endif