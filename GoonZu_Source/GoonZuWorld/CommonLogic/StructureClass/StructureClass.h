//---------------------------------
// 2003/6/2 김태곤
//---------------------------------

#ifndef _STRUCTURECLASS_H
#define _STRUCTURECLASS_H

#include <Directives.h>

//
#include "..\..\Common\CommonHeader.h"
#include "..\..\Common\Item\ItemCommon\ItemUnit.h"

#include "..\..\Lib\JYLibFunction.h"


extern CJYLibFunction g_JYLibFunction;

class NStrBaseDlg;

class cltCharManager;
class cltAccountNum;
class cltBankAccount;
class cltFee;
class cltBankVary;
class cltRentContract;
class cltHouseUnit;
class cltHouseUnitStorageInfo;
class cltRentApplyer;
class cltStructureRepairDlg;
class cltTime;

#define	CITYHALL_BEING_NORMAL		0			// 마을 정상 상태 중
#define	CITYHALL_BEING_BANKRUPT		1			// 마을 파산 중	
#define	CITYHALL_BEING_INITIALIZED	2			// 마을 초기화 된 기간 중
#define	CITYHALL_BEING_STABLE		3			// 마을 안정기 중
#define	CITYHALL_BEING_PROCESSING	4			// 현재 상태가 처리 중이다..(lock 상태임)
#define CITYHALL_UNDEFINED_STATE	5			// 알수 없는 상태 임

#define CITYHALL_BANKRUPTCY_NOTIFY_AFTER_MONTHS			6		// 대행수 해임 후 마을 파산 위험을 공지할 기간
#define	CITYHALL_BANKRUPTCY_NOTIFY_STOCK_AMOUNT_RATE	0.03f	// 마을 파산 경고를 알릴 주주의 주식 비율
#define CITYHALL_BANKRUPTCY_AFTER_YEARS					1		// 대행수 해임 후 마을 파산이 될 기간

#define CITYHALL_STABLE_YEARS_AFTER_CHIEF_ELECTION		1		// 초기화 후 대행수 취임 후 마을 안정기 년수
#define CITYHALL_STABLE_DAYS_AFTER_CHIEF_ELECTION		360		// 초기화 후 대행수 취임 후 마을 안정기 날짜수

#define MAX_STRUCTURE_TAB_NUMBER	10
#define MAX_DIVIDEND_RECORD			24			// 배당 내역 저장 개수 

//KHY - 0730 - 배당 받을 유저의 은행 총 금액을 업데이트 시켜준다. 인원이 늘어서.
#define MAX_DIVIDEND_PERSON			300			// 최대 배당 주주 숫자 

#define MAX_TAX_RECORD_YEAR			10			// 세금 납부 내역 저장 개수 
#define MAX_INOUTMONEY_RECORD		30			// 상단 입출금 내역 저장 개수

#define MAX_STR_DATA_SIZE			5120		// 각 건물 고유의 최대 데이터 사이즈. 
#define MAX_HIRED_NPC_NUM			4			// 각 마을에서 최대 고용할 수 있는 주민 숫자
#define MAX_HIRED_EVENTNPC_NUM		4			// 각 마을에서 최대 고용할 수 있는 이벤트NPC 숫자

#define MAX_STR_NOTICE_SIZE			256			// 건물 안내문 최대길이 NULL포함


//---------------------------------------
// 수수료의 종류.
//---------------------------------------
#define FEETYPE_STOCK_SELL			200
#define FEETYPE_STOCK_BUY			201

#define FEETYPE_MARKET_SELL			300
#define FEETYPE_NEWMARKET_SELL		301

//---------------------------------------
// 상단 입출금 내역 코드 
//---------------------------------------
#define INOUTCODE_DIVIDEND			1		// 배당 
#define INOUTCODE_TAX				2		// 세금
#define INOUTCODE_STOCK				4		// 증자
#define INOUTCODE_GODSTAT			5		// 성황신 스탯 조정
#define INOUTCODE_CASTLESTAT		6		// 성문 스탯 조정 
#define INOUTCODE_TOPARENT			7		// 부모 마을 조공 (보냄)
#define INOUTCODE_FROMCHILD			8		// 자식 마을 조공 (받음)
#define INOUTCODE_NPCHIRE			9		// NPC고용
#define INOUTCODE_NPCPAY			10		// NPC급여
#define INOUTCODE_RESIDENTSHARE		11		// 주민 배당 
#define INOUTCODE_BUILDSTRUCTURE	12		// 건물 건설
#define INOUTCODE_EVENTPAY			13		// 마을 이벤트 실시
#define INOUTCODE_VILLAGEMARK		14		// 마을 마크 변경 
#define INOUTCODE_VILLAGEMAP		15		// 마을 맵 변경
#define INOUTCODE_INSTANCEPROFIT	16		// 대마도 이익금
#define INOUTCODE_DESTROYSTR		17		// 건물 내구도 소모로 변경
#define INOUTCODE_NATIONTAX			18		// 왕실 자금으로 들어간돈
#define INOUTCODE_SETBASEMONEY		19		// 마을 건물 자본금으로 설정한 돈
#define INOUTCODE_VILLAGEWARMONEY	20		// 전쟁 승,패로 인해 가,감된 돈
#define INOUTCODE_DONATEMONEY		21		// 기부금으로 증가된 돈
#define INOUTCODE_CREATEEMPIRE		22		// 제국을 만드는데 사용한 돈
#define INOUTCODE_SETEMPIREMONEY	23		// 제국자본금 넣는데 사용했다
#define INOUTCODE_BLACK_WAR			24		// 흑의 군단으로 얻은 자금.


//---------------------------------------
// 성황신 
//---------------------------------------
#define SYMBOLSTAT_KIND				1		
#define SYMBOLSTAT_LIFE				2		
#define SYMBOLSTAT_MANA				3		
#define SYMBOLSTAT_CAPA				4		

//---------------------------------------
// 상단 입출금 내역 정보 
//---------------------------------------
class cltCityHallInOutMoneyUnitInfo
{
public:
	SI32							siStrType;								// 입출금 건물
	SI32							siInOutCode;							// 입출금 코드
	cltDate							clDate;									// 입출금일 
	cltMoney						clInOutMoney;							// 입출금액 
	cltMoney						clCityhallMoney;						// 상단잔고
};

//---------------------------------------
// 상단 입출금 내역 정보 
//---------------------------------------
class cltCityHallInOutMoneyInfo
{
public:
	SI32							siCount;
	cltCityHallInOutMoneyUnitInfo	clInOutMoneyUnitInfo[ MAX_INOUTMONEY_RECORD ];

	void Init()
	{
		siCount = 0;
	}

	SI32 GetCount() { return siCount; };	
	void AddInOutMoneyInfo( SI32 strType, SI32 inoutcode, cltDate *pclDate, cltMoney *pclInOutMoney, cltMoney *pclCityhallMoney );
};

//---------------------------------------
// 고용 주민(NPC)정보
//---------------------------------------
class cltHiredNPC
{
public:
	SI32		siNPCKind;					// NPC Kind
	GMONEY		siHirePay;					// 고용비용
	GMONEY		siMonthlyPay;				// 월급
	cltDate		clHireDate;					// 고용일

	void Set(cltHiredNPC* pclHiredNPC)
	{
		if(pclHiredNPC == NULL)			return;

		siNPCKind	= pclHiredNPC->siNPCKind;
		siHirePay	= pclHiredNPC->siHirePay;
		siMonthlyPay = pclHiredNPC->siMonthlyPay;
		clHireDate.Set(&pclHiredNPC->clHireDate);
	}
};

//---------------------------------------
// 배당 주주 정보
//---------------------------------------
class cltDividendPerson
{
public:
	SI32		siPersonID;					// personid
	cltMoney	clMoney;					// 배당금 
	//KHY - 0730 - 배당 받을 유저의 은행 총 금액을 업데이트 시켜준다.
	cltMoney	clAllMoney;					// 배당 받은 person이 가지고  있는 총금액.


	cltDividendPerson()
	{
		siPersonID = 0;
		clMoney.Init();
	}
};

//---------------------------------------
// 배당 정보
//---------------------------------------
class cltDividendUnitInfo
{
public:
	cltDate		clDate;						// 배당일
	
	UI16		usYear;						// 배당년도 
	SI32		siTotalStock;				// 총 배당 주식 수
	cltMoney	clDividendPrice;			// 한 주당 배당금  

	void Init()
	{
		siTotalStock = 0;
		clDividendPrice.Init();
	}
};

//---------------------------------------
// 배당 정보
//---------------------------------------
class cltDividendInfo
{
public:
	SI32					siCount;

	cltDividendUnitInfo		clDividendUnitInfo[ MAX_DIVIDEND_RECORD ];

	void Init()
	{
		siCount = 0;
	}

	void		SetCount( SI32 count ) { siCount = count; };
	SI32		GetCount() { return siCount; };

	SI32		GetTotalSize() { 
		return sizeof( cltDividendInfo ); 
	};

	void		AddDividendInfo( cltDate *pclDate, SI32 siTotalStock, cltMoney *pclMoney );
};


// 상단 정보 
class cltCityhallStrInfo {
public:

	cltCityHallInOutMoneyInfo	clInOutMoneyInfo;								// 상단 자금 입출금 내역 정보 
	cltDividendInfo				clDividendInfo;									// 배당 내역 정보

	cltHiredNPC		clHiredNPC[ MAX_HIRED_NPC_NUM ];							// 주민 고용 정보
	cltHiredNPC		clHiredEventNPC[ MAX_HIRED_EVENTNPC_NUM ];					// 이벤트용 NPC 고용 정보

	SI32			siResidentNum;												// 주민수
	SI32			siTotalStockAmount;											// 총 발행 주식 수
	GMONEY			siStockPrice;												// 현재 주식 가격 
	GMONEY			siBeginningPrice;											// 입찰 시작 주식 가격 
	SI32			siVillageLevel;												// 마을 레벨 

	SI32			siMasterVillage;											// 최고 부모 마을
	SI32			siParentVillage;											// 부모마을
	BOOL			bDeclareWarVillage[ MAX_VILLAGE_NUMBER ];					// 선전포고 마을 	

	UI16		uiDeclareWarDateYear;				// 선전포고 일자
	UI08		uiDeclareWarDateMonth;
	UI08		uiDeclareWarDateDay;
	
	UI16		uiLastWarLoseDateYear;				// 최종 공성전 패배일(1주일간 보호받기 위해 쓰이는 변수)
	UI08		uiLastWarLoseDateMonth;
	UI08		uiLastWarLoseDateDay;
 
	SI16		siVillaeWarRecordWin;				// 마을 공성전 성적 승리수
	SI16		siVillaeWarRecordLose;				// 마을 공성전 성적 패배수

	SI16			siSymbolKind;												// 심볼(수호신) 종류 
	SI32			siSymbolLife;												// 심볼(수호신) 체력
	SI32			siSymbolMana;												// 심볼(수호신) Mana
	SI16			siSymbolCapa;												// 심볼(수호신) 성능
	SI32			siSymbolMaxLife;											// 심볼(수호신) 최대 체력 
	cltDate			clLastSymbolUpdate;											// 최근 심볼 스탯 수정일 
	
	SI16			siCastleKind;												// 캐슬 종류 
	SI32			siCastleLife;												// 캐슬 체력
	SI32			siCastleMana;												// 캐슬 Mana
	SI16			siCastleCapa;												// 캐슬 성능
	SI32			siCastleMaxLife;											// 캐슬 최대 체력 
	cltDate			clLastCastleUpdate;											// 최근 캐슬 스탯 수정일 
	
	SI32			siLastDestroyStrDate;										// 마지막으로 건물을 해체한 날의 DateVary

	cltMoney		clProfitMoney;												// 수익금
	cltDate			clChiefDate;												// 대행수 취임일자

	SI32			siVisitLevel;												// 방문자 레벨

//#if	defined(_CITYHALL_BANKRUPTCY)
	cltDate			clResignDate;												// 대행수 해임일
	UI08			usCurStatus;												// 현재 마을의 상태 CITYHALL_BEING_BANKRUPT : 파산, CITYHALL_BEING_INITIALIZED : 초기화, 
																				// CITYHALL_BEING_STABLE : 안정기, CITYHALL_BEING_NORMAL : 정상

	SI32			siTotalVillageScore;
	
	void Set(cltCityhallStrInfo* pclinfo)
	{
		SI32 i = 0;

		clInOutMoneyInfo = pclinfo->clInOutMoneyInfo;
		clDividendInfo = pclinfo->clDividendInfo;
		memcpy( &clHiredNPC, &pclinfo->clHiredNPC, sizeof( cltHiredNPC ) * MAX_HIRED_NPC_NUM );
		memcpy( &clHiredEventNPC, &pclinfo->clHiredEventNPC, sizeof( cltHiredNPC ) * MAX_HIRED_EVENTNPC_NUM );
		siResidentNum = pclinfo->siResidentNum;
		siTotalStockAmount = pclinfo->siTotalStockAmount;
		siStockPrice = pclinfo->siStockPrice;
		siBeginningPrice = pclinfo->siBeginningPrice;
		siVillageLevel = pclinfo->siVillageLevel;
	
		// 공성 관련
		siMasterVillage = pclinfo->siMasterVillage;
		siParentVillage = pclinfo->siParentVillage;
		memcpy( bDeclareWarVillage, pclinfo->bDeclareWarVillage, sizeof( BOOL ) * MAX_VILLAGE_NUMBER );
//		clDeclareWarDate.Set( &pclinfo->clDeclareWarDate );
//		clLastWarLoseDate.Set( &pclinfo->clLastWarLoseDate );


		uiDeclareWarDateYear = pclinfo->uiDeclareWarDateYear;
		uiDeclareWarDateMonth = pclinfo->uiDeclareWarDateMonth;
		uiDeclareWarDateDay = pclinfo->uiDeclareWarDateDay;

		uiLastWarLoseDateYear = pclinfo->uiLastWarLoseDateYear;
		uiLastWarLoseDateMonth = pclinfo->uiLastWarLoseDateMonth;
		uiLastWarLoseDateDay = pclinfo->uiLastWarLoseDateDay;

		siVillaeWarRecordWin	= pclinfo->siVillaeWarRecordWin;
		siVillaeWarRecordLose	= pclinfo->siVillaeWarRecordLose;

		// 심볼 
		siSymbolKind = pclinfo->siSymbolKind;
		siSymbolLife = pclinfo->siSymbolLife;
		siSymbolMana = pclinfo->siSymbolMana;
		siSymbolCapa = pclinfo->siSymbolCapa;
		siSymbolMaxLife = pclinfo->siSymbolMaxLife;
		clLastSymbolUpdate.Set( &pclinfo->clLastSymbolUpdate );		

		// 캐슬
		siCastleKind = pclinfo->siCastleKind;
		siCastleLife = pclinfo->siCastleLife;
		siCastleMana = pclinfo->siCastleMana;
		siCastleCapa = pclinfo->siCastleCapa;
		siCastleMaxLife = pclinfo->siCastleMaxLife;
		clLastCastleUpdate.Set( &pclinfo->clLastCastleUpdate );		

		siLastDestroyStrDate	= pclinfo->siLastDestroyStrDate;

		clProfitMoney.Set( &pclinfo->clProfitMoney );
		clChiefDate.Set( &pclinfo->clChiefDate );

		siVisitLevel = pclinfo->siVillageLevel;

//#if	defined(_CITYHALL_BANKRUPTCY)
		clResignDate.Set( &pclinfo->clResignDate );					// 대행수 해임일
		usCurStatus = pclinfo->usCurStatus;							// 현재 마을의 상태 CITYHALL_BEING_BANKRUPT : 파산, CITYHALL_BEING_INITIALIZED : 초기화, 
																	// CITYHALL_BEING_STABLE : 안정기, CITYHALL_BEING_NORMAL : 정상

		siTotalVillageScore = pclinfo->siTotalVillageScore;
//#endif
	}

	GMONEY GetTotalDividendMoney( int year ) {

		GMONEY ret = 0;

		for( int i = 0; i < MAX_DIVIDEND_RECORD; ++i ) {
			
			if( (int)(clDividendInfo.clDividendUnitInfo[ i ].clDate.uiYear) == year ) {
				ret += (clDividendInfo.clDividendUnitInfo[ i ].clDividendPrice.itMoney);
			}
		}

		return ret;
	}
	//현제 슬롯에 같은 엔피씨가 있는지 확인.
	BOOL IHaveEventNpc( SI32 kind){
		for (int i=0; i<MAX_HIRED_EVENTNPC_NUM; i++) {
			if (clHiredEventNPC[i].siNPCKind == kind ) {
				return TRUE;
			}
		}
		return FALSE;
	}

};


// 전장 정보
class cltBankStrInfo{
public:
	cltFee	 clFee;					// 수수료 변수
	cltMoney clMaxMoney;			// 계좌당 최대 입금 가능액
	cltMoney clTotalInputMoney;		// 총입금액

	void Set(cltBankStrInfo* pclinfo)
	{
		clFee.Set(&pclinfo->clFee);
		clMaxMoney.Set(&pclinfo->clMaxMoney);
		clTotalInputMoney.Set(&pclinfo->clTotalInputMoney);
	}
};

// 여각 정보
class cltStockStrInfo{
public:
	cltFee clFee;

	void Set(cltStockStrInfo* pclinfo)
	{
		clFee.Set(&pclinfo->clFee);
	}

};


// 객주 정보 
class cltMarketStrInfo
{
public:
	cltFee clFee;

	void Set(cltMarketStrInfo* pclinfo)
	{
		clFee.Set(&pclinfo->clFee);
	}
};

class cltNewMarketStrInfo
{
public:
	cltFee clFee;

	void Set(cltNewMarketStrInfo* pclinfo)
	{
		clFee.Set( &pclinfo->clFee );
	}
};


// 시전 정보 
class cltHouseStrInfo
{
public:
	cltRentContract clRentContract;
	cltDate			clLastChangeDate;	// 계약 정보 최종 변경일 

	void Set(cltHouseStrInfo* pclinfo)
	{
		clRentContract.Set( &pclinfo->clRentContract );
		clLastChangeDate.Set( &pclinfo->clLastChangeDate );
	}
};

// 수렵장 정보
#define MAX_HUNT_STG_NUMBER						10			// 몬스터 창고 용량. 
#define MAX_HUNT_LINEUP_NUMBER					10			// 전선에 배치될 몬스터 종류 수
#define MAX_HUNT_MONSTER_NUM					60			// 전선에 배치될 Max 몬스터수
#define MAX_HUNT_LINEUP_EACH_MONSTER_NUM		30			// 몬스터 한 종류당 배치 가능한 수

#define MAX_HUNT_MAP_NUMBER						100			// 수렵장 맵 개수
#define HUNT_MAP_START							201			// 수렵장 시작 인덱스



enum { HUNT_ENTER_ITEMINSUFFICIENY, HUNT_ENTER_ENABLESTATUS };

class cltHuntStrInfo{
public:
	GMONEY siEnterFee;			// 입장료. 

	SI16 siHuntStatus;			// 수렵장 상태

	//----------------------------
	// 저장 되지 말아야 할 정보 
	//----------------------------
	SI32 siChiefExp;			// 행수가 획득하게 되는 경험치. 
	GMONEY siTempMoney;			// 수렵장 자금으로 들어갈 돈의 임시 보관소. 
	SI32 siUserNum;				// 수렵장 이용자 수. 

	cltHuntStg clHuntStg[MAX_HUNT_STG_NUMBER];			// 몬스터 소환 아이템을 넣을 공간. 
	cltHuntLineUp clHuntLineUp[MAX_HUNT_LINEUP_NUMBER];	// 전선에 배치된 몬스터 정보. 

	void Set(cltHuntStrInfo* pclinfo)
	{
		SI32 i;
		
		siEnterFee			= pclinfo->siEnterFee;
		siHuntStatus		= pclinfo->siHuntStatus;
		
		siChiefExp			= pclinfo->siChiefExp;
		siTempMoney			= pclinfo->siTempMoney;
		siUserNum			= pclinfo->siUserNum;


		for(i = 0;i < MAX_HUNT_STG_NUMBER;i++)
		{
			clHuntStg[i].Set(&pclinfo->clHuntStg[i]);
		}

		for(i = 0;i < MAX_HUNT_LINEUP_NUMBER;i++)
		{
			clHuntLineUp[i].Set(&pclinfo->clHuntLineUp[i]);
		}
	}
};



// 우체국 정보
class cltPostOfficeStrInfo {
public:
	cltFee clMoneyFee;											// 송금 수수료 
	cltFee clItemFee;											// 물품 전달 수수료

	GMONEY   siMsgFee;											// 메시지 수수료 (건당)
	GMONEY   siRecvFee;											// 수신함 열람 수수료 (회당)
	
	void Set(cltPostOfficeStrInfo* pclinfo)
	{
		clMoneyFee.Set( &pclinfo->clMoneyFee );
		clItemFee.Set( &pclinfo->clItemFee );

		siMsgFee = pclinfo->siMsgFee;
		siRecvFee = pclinfo->siRecvFee;
	}
};

// 사복시 정보
class cltHorseMarketStrInfo {
public:
	cltFee clFee;

	void Set(cltHorseMarketStrInfo* pclinfo)
	{
		clFee.Set(&pclinfo->clFee);
	}
};


// 거간 정보
class cltRealEstateMarketStrInfo {
public:
	cltFee clFee;

	void Set(cltRealEstateMarketStrInfo* pclinfo)
	{
		clFee.Set(&pclinfo->clFee);
	}
};

// 소환수거래소 정보
class cltSummonMarketStrInfo {
public:
	cltFee clFee;

	void Set(cltSummonMarketStrInfo* pclinfo )
	{
		clFee.Set(&pclinfo->clFee);
	}
};


// 농장정보 
class cltLandStrInfo{
public:
	cltRentContract	clRentContract;
	cltDate			clLastChangeDate;			// 계약 정보 최종 변경일 

	SI32			siLandType;					// 농장 종류

	void Set(cltLandStrInfo* pclinfo)
	{
		clRentContract.Set( &pclinfo->clRentContract );
		clLastChangeDate.Set( &pclinfo->clLastChangeDate );

		siLandType = pclinfo->siLandType;
	}

};

// 파티홀 예약 타입.
#define		TYPE_ENTRANCE_PUBLIC			1	// 공개.
#define		TYPE_ENTRANCE_VILLAGE			2	// 마을.
#define		TYPE_ENTRANCE_GUILD				3	// 길드.
#define		TYPE_ENTRANCE_PRIVATE			4	// 비공개.
#define		RESERVATIO_PRESENT_ITEM			3	// 예약자..그니까 주최자가 선물로 등록 할 수 있는 아이템 최대 수.
#define		ENTRANCE_APPLICATION_LIST		20	// 입장 신청 예약자 20명 까지.


//[진성] 파티장에서 나눠줄 아이템을 등록할 구조체
class cltPartyHallPresentItemInfo
{
public:
	SI32 m_siItemUnique;
	SI32 m_siItemNum;
	
	cltPartyHallPresentItemInfo()
	{
		ZeroMemory(this, sizeof(cltPartyHallPresentItemInfo));
	}

	void Set(cltPartyHallPresentItemInfo* pclPartyHallPresentItemInfo)
	{
		if(pclPartyHallPresentItemInfo == NULL)			return;

		m_siItemUnique	= pclPartyHallPresentItemInfo->m_siItemUnique;
		m_siItemNum		= pclPartyHallPresentItemInfo->m_siItemNum;
	}
};

//[진성] 파티장에서 나눠줄 아이템을 등록할 구조체
class cltPartyHallEntranceApplication
{
public:
	SI32 m_siPersonID;
	SI32 m_siLevel;
	
	cltPartyHallEntranceApplication()
	{
		ZeroMemory(this, sizeof(cltPartyHallEntranceApplication));
	}

	void Set(cltPartyHallEntranceApplication* pclPartyHallEntranceApplication)
	{
		if(pclPartyHallEntranceApplication == NULL)			return;

		m_siPersonID	= pclPartyHallEntranceApplication->m_siPersonID;
		m_siLevel		= pclPartyHallEntranceApplication->m_siLevel;
	}
};

//[진성] 파티홀. 예약 리스트 구조체.
class cltPatyHallResvInfo
{
public:
	SI32			m_siPersonID;									// 신청자 person id
	TCHAR			m_szPersonName[ MAX_PLAYER_NAME ];				// 신청자 name 
	SYSTEMTIME		m_SystemTime;
	TCHAR			m_szReason[ 128 ];								// 연회 사유 

	SI16			m_siEntranceType;								//[진성] 1: 공개,	 2: 마을,    3: 길드,  4: 비공개 => 2008. 3. 10
	SI16			m_siDecorationKind;								//[진성] 장식물 종류. 
	SI16			m_siValueType;									//[진성] 마을번호, 길드번호.

#ifdef _SAFE_MEMORY
	NSafeTArray<cltPartyHallPresentItemInfo, RESERVATIO_PRESENT_ITEM>		m_PresentItem;				//[진성] 파티홀. 예약자가 줄 선물이 등록. => 2008. 3. 13
	NSafeTArray<cltPartyHallEntranceApplication, ENTRANCE_APPLICATION_LIST>	m_clEntranceApplication;	//[진성] 파티홀. 입장 신청 예약자 리스트. => 2008. 3. 14
#else
	cltPartyHallPresentItemInfo			m_PresentItem[RESERVATIO_PRESENT_ITEM];				//[진성] 파티홀. 예약자가 줄 선물이 등록. => 2008. 3. 13
	cltPartyHallEntranceApplication		m_clEntranceApplication[ENTRANCE_APPLICATION_LIST]; //[진성] 파티홀. 입장 신청 예약자 리스트. => 2008. 3. 14
#endif


	void Set( cltPatyHallResvInfo *pclinfo ) 
	{
		ZeroMemory( this, sizeof( cltPatyHallResvInfo ) );
	
		m_siPersonID = pclinfo->m_siPersonID;
		MStrCpy(m_szPersonName, pclinfo->m_szPersonName, sizeof(m_szPersonName));
		m_szPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;
		m_SystemTime.wYear   = pclinfo->m_SystemTime.wYear;  
		m_SystemTime.wMonth  = pclinfo->m_SystemTime.wMonth; 
		m_SystemTime.wDay	 = pclinfo->m_SystemTime.wDay;	
		m_SystemTime.wHour	 = pclinfo->m_SystemTime.wHour;
		m_SystemTime.wMinute = pclinfo->m_SystemTime.wMinute;
		MStrCpy( m_szReason, pclinfo->m_szReason, sizeof(m_szReason) );
		m_szReason[ 127 ] = NULL;

		m_siEntranceType	= pclinfo->m_siEntranceType;
		m_siDecorationKind  = pclinfo->m_siDecorationKind;  
		m_siValueType		= pclinfo->m_siValueType;

		SI32 i = 0;
#ifdef _SAFE_MEMORY
		for(i=0; i<RESERVATIO_PRESENT_ITEM; i++)		m_PresentItem[i].Set(&pclinfo->m_PresentItem[i]);
		for(i=0; i<ENTRANCE_APPLICATION_LIST; i++)		m_clEntranceApplication[i].Set(&pclinfo->m_clEntranceApplication[i]);
#else
		memcpy( m_PresentItem, pclinfo->m_PresentItem, sizeof(m_PresentItem) );
		memcpy( m_clEntranceApplication, pclinfo->m_clEntranceApplication, sizeof(m_clEntranceApplication) );
#endif
	}
};

// 내자시 예약 정보 
class cltFeastResvInfo
{
public:
	SI32			siPersonID;										// 신청자 person id
	TCHAR			szPersonName[ MAX_PLAYER_NAME ];				// 신청자 name 
	SI16			siYear;											// 연회 시간(년)
	SI16			siMonth;										// 연회 시간(월)
	SI16			siDay;											// 연회 시간(일)
	SI16			siHour;											// 연회 시간(시)
	SI16			siMinute;										// 연회 시간(분)
	TCHAR			szReason[ 128 ];								// 연회 사유 

	void			Set( cltFeastResvInfo *pclinfo ) {

		siPersonID = pclinfo->siPersonID;
		memcpy( szPersonName, pclinfo->szPersonName, MAX_PLAYER_NAME );
		szPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;
		siYear = pclinfo->siYear;
		siMonth = pclinfo->siMonth;
		siDay = pclinfo->siDay;
		siHour = pclinfo->siHour;
		siMinute = pclinfo->siMinute;
		memcpy( szReason, pclinfo->szReason, 128 );
		szReason[ 127 ] = NULL;
	}
};



// 내자시 기본 정보 
class cltFeastInfo
{
public:
	GMONEY			siCost;										// 연회 비용
	
	SI16			siItemUnique[ MAX_FEAST_PRESENT_NUMBER ];	// 선물 item unique
	SI16			siItemNum[ MAX_FEAST_PRESENT_NUMBER ];		// 선물 item num
	SI16			siItemGive[ MAX_FEAST_PRESENT_NUMBER ];		// 선물 지급 여부 1:지급됨 0: 지급안됨

	SI32			siStatus;									// 1: 연회중,  2: 예약됨,  0: 연회중 아님 

	void Set( cltFeastInfo *pclinfo ) 
	{

		siCost = pclinfo->siCost;
		memcpy( siItemUnique, pclinfo->siItemUnique, sizeof( siItemUnique ) );
		memcpy( siItemNum, pclinfo->siItemNum, sizeof( siItemNum ) );
		memcpy( siItemGive, pclinfo->siItemGive, sizeof( siItemGive ) );
		siStatus	   = pclinfo->siStatus;
	}


	SI32 GetEmptyPresentIndex() {

		for( int i = 0; i < MAX_FEAST_PRESENT_NUMBER; ++i ) {
			
			if( siItemUnique[ i ] == 0 ) return i;
		}

		return -1;
	}

	void SetPresentItem( SI32 index, SI16 itemunique, SI16 itemnum )
	{
		siItemUnique[ index ] = itemunique;
		siItemNum[ index ] = itemnum;
	}

};

class PartyHall_Decoration
{
public:
	SI16	m_DacorationKind;
	SI16	m_DacorationNum;

	void Set(PartyHall_Decoration* pclPartyHall_Decoration)
	{
		if(pclPartyHall_Decoration == NULL)			return;

		m_DacorationKind = pclPartyHall_Decoration->m_DacorationKind;
		m_DacorationNum = pclPartyHall_Decoration->m_DacorationNum;
	}
};

// 내자시 정보
class cltFeastStrInfo{
public:
	cltFeastInfo		clFeastInfo;							// 내자시 기본정보 
	cltFeastStg			clFeastStg[ MAX_FEAST_STG_NUMBER ];		// 내자시 창고정보
	cltFeastResvInfo	clFeastResvInfo;						// 내자시 예약정보

	PartyHall_Decoration	clDecoration[MAX_PARTYHALL_DECORATION_ITEM];	// 장식물.

	void Set( cltFeastStrInfo* pclinfo )
	{
		clFeastInfo.Set( &pclinfo->clFeastInfo );
		memcpy( clFeastStg, pclinfo->clFeastStg, sizeof( cltFeastStg ) * MAX_FEAST_STG_NUMBER );
		clFeastResvInfo.Set( &pclinfo->clFeastResvInfo );
		memcpy( clDecoration, pclinfo->clDecoration, sizeof( clDecoration ));
	}


	BOOL CanResv() 
	{
		int i, j;

		BOOL bRet = FALSE;

		for( i = 0; i < MAX_FEAST_PRESENT_NUMBER; ++i ) {

			if( clFeastInfo.siItemUnique[ i ] == 0 ) continue;

			bRet = TRUE;

			for( j = 0; j < MAX_FEAST_STG_NUMBER; ++j ) {

				if( clFeastInfo.siItemUnique[ i ] == clFeastStg[ j ].siItemUnique ) {
						
						if( clFeastInfo.siItemNum[ i ] > clFeastStg[ j ].siItemNum ) return FALSE;

						break;
					}
			}

			if( j == MAX_FEAST_STG_NUMBER ) return FALSE;
		}

		return bRet;
	}

};

// 비변사 정보
class cltSummonHeroMarketStrInfo {
public:
	cltFee clFee;

	void Set(cltSummonHeroMarketStrInfo* pclinfo )
	{
		clFee.Set(&pclinfo->clFee);
	}
};

// 광산정보 
class cltMineStrInfo{
public:
	cltRentContract	clRentContract;
	cltDate			clLastChangeDate;			// 계약 정보 최종 변경일 

	SI32			siMineType;					// 광산 종류

	void Set(cltMineStrInfo* pclinfo)
	{
		clRentContract.Set( &pclinfo->clRentContract );
		clLastChangeDate.Set( &pclinfo->clLastChangeDate );

		siMineType = pclinfo->siMineType;
	}

};


// 상단관리소 정보
class cltGuildStrInfo {
public:
	//SI32			siRentMonthPeriod;	// 임대기간	- 월 단위

	GMONEY			siRentFee;			// 월 임대료. 
	//cltDate			clDate;				// 계약일시. 

	cltFee			clFee;				// 수수료
	cltDate			clLastChangeDate;	// 계약 정보 최종 변경일 

	void Set(cltGuildStrInfo* pclinfo )
	{
		siRentFee = pclinfo->siRentFee;
		//siRentMonthPeriod = pclinfo->siRentMonthPeriod;
		//clDate.Set( &pclinfo->clDate );

		clLastChangeDate.Set( &pclinfo->clLastChangeDate );
		clFee.Set( &pclinfo->clFee );
	}
};



//---------------------------------------
// 세금 정보
//---------------------------------------
class cltTaxUnitInfo
{
public:
	cltTaxUnitInfo() {
		usYear = 0;
		clDueTax.Init();
		clPaidTax.Init();
	}

	UI16		usYear;
	cltMoney	clDueTax;
	cltMoney	clPaidTax;
};

//---------------------------------------
// 세금 정보
//---------------------------------------
class cltTaxInfo
{
public:
	cltTaxInfo() {
		siCount = 0;
	}
	SI32			siCount;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltTaxUnitInfo, MAX_TAX_RECORD_YEAR>		clTaxUnitInfo;
#else
	cltTaxUnitInfo	clTaxUnitInfo[ MAX_TAX_RECORD_YEAR ];
#endif

	void			SetCount( SI32 count ) { siCount = count; };
	SI32			GetCount() { return siCount; };

	SI32			GetTotalSize() { return sizeof( cltTaxInfo ); };

	void			SetTax( UI16 year, cltMoney *pclTax );
	void			SetPaidTax( UI16 year, cltMoney *pclTax );
};


//---------------------------------------
// 건물 기본 정보 클래스 
//---------------------------------------
//class cltStrCommonInfo{
//public:
//	SI32		siStrType;				// 건물의 종류.
//	cltMoney	clMoney;
//
//	cltTaxInfo	clTaxInfo;
//	TCHAR		strNotice[ MAX_STR_NOTICE_SIZE ];
//
//	SI32		siStrDura;				// 건물 내구도
//	
//	SI32		siNeedItemUnique;		// 수리하기 위해 필요한 아이템 유니크 
//	SI16		siNeedItemNum;			// 수리하기 위해 필요한 아이템 개수 
//	SI16		siUseItemNum;			// 수리하기 위해 채워 넣은 아이템 개수
//
//	cltStrCommonInfo()
//	{
//		siStrType	= 0;
//		clMoney.Init();
//		strNotice[ 0 ] = NULL;
//
//		siStrDura = 0;
//		siNeedItemUnique = 0;
//		siNeedItemNum = 0;
//		siUseItemNum = 0;
//	}
//
//	void Set(cltStrCommonInfo* pclinfo)
//	{
//		siStrType	= pclinfo->siStrType;
//		clTaxInfo = pclinfo->clTaxInfo;
//		
//		clMoney.Set(&pclinfo->clMoney);
//
//		memcpy( strNotice, pclinfo->strNotice, MAX_STR_NOTICE_SIZE );
//
//		siStrDura = pclinfo->siStrDura;
//		siNeedItemUnique = pclinfo->siNeedItemUnique;
//		siNeedItemNum = pclinfo->siNeedItemNum;
//		siUseItemNum = pclinfo->siUseItemNum;
//	}
//	
//};

class cltStrCommonInfo{
public:
	SI32		siStrType;				// 건물의 종류.
	cltMoney	clMoney;

	cltTaxInfo	clTaxInfo;
	char		strNotice[ MAX_STR_NOTICE_SIZE ];

	SI32		siStrDura;				// 건물 내구도

	SI32		siNeedItemUnique;		// 수리하기 위해 필요한 아이템 유니크 
	SI16		siNeedItemNum;			// 수리하기 위해 필요한 아이템 개수 
	SI16		siUseItemNum;			// 수리하기 위해 채워 넣은 아이템 개수

	SI32		siRepairStrDura;		// 수리할려는 내구도량(0~1000)

	cltMoney	clBaseMoney;			// 건물의 각 자본금( 수리시 사용된다.)
	
	cltStrCommonInfo()
	{
		siStrType	= 0;
		clMoney.Init();
		strNotice[ 0 ] = NULL;

		siStrDura = 0;
		siNeedItemUnique = 0;
		siNeedItemNum = 0;
		siUseItemNum = 0;

		siRepairStrDura = 0;

		clBaseMoney.Init();
	}

	void Set(cltStrCommonInfo* pclinfo)
	{
		siStrType	= pclinfo->siStrType;
		clTaxInfo = pclinfo->clTaxInfo;

		clMoney.Set(&pclinfo->clMoney);

		memcpy( strNotice, pclinfo->strNotice, MAX_STR_NOTICE_SIZE );

		siStrDura = pclinfo->siStrDura;
		siNeedItemUnique = pclinfo->siNeedItemUnique;
		siNeedItemNum = pclinfo->siNeedItemNum;
		siUseItemNum = pclinfo->siUseItemNum;

		siRepairStrDura = pclinfo->siRepairStrDura;

		clBaseMoney.Set(&pclinfo->clBaseMoney);
	}

};



//---------------------------------------
// 건물의 정보. 
//---------------------------------------
class cltStrInfo{
public:
	cltStrCommonInfo clCommonInfo;

	union{
		UI08 cData[MAX_STR_DATA_SIZE];
		cltBankStrInfo				clBankStrInfo;
		cltStockStrInfo				clStockStrInfo;
		cltMarketStrInfo			clMarketStrInfo;
		cltHouseStrInfo				clHouseStrInfo;
		cltCityhallStrInfo			clCityhallStrInfo;
		cltHuntStrInfo				clHuntStrInfo;
		cltPostOfficeStrInfo		clPostOfficeStrInfo;
		cltHorseMarketStrInfo		clHorseMarketStrInfo;
		cltRealEstateMarketStrInfo	clRealEstateMarketStrInfo;
		cltSummonMarketStrInfo		clSummonMarketStrInfo;
		cltLandStrInfo				clLandStrInfo;
		cltFeastStrInfo				clFeastStrInfo;
		cltSummonHeroMarketStrInfo	clSummonHeroMarketStrInfo;
		cltMineStrInfo				clMineStrInfo;
		cltGuildStrInfo				clGuildStrInfo;
		//cltNewMarketStrInfo		clNewMarketStrInfo;			// [진성] 통합마켓 2009-1-5 
		cltNewMarketStrInfo		clNewMarketStrInfo;				// [진성] 통합마켓 2009-1-5 
	};


	cltStrInfo()
	{
		ZeroMemory(cData, MAX_STR_DATA_SIZE);
	}

	void Set(cltStrInfo* pclinfo)
	{
		clCommonInfo.Set(&pclinfo->clCommonInfo);

		switch(clCommonInfo.siStrType)
		{
		case RANKTYPE_BANK:				clBankStrInfo.Set(&pclinfo->clBankStrInfo);							break;
		case RANKTYPE_STOCK:			clStockStrInfo.Set(&pclinfo->clStockStrInfo);						break;
		case RANKTYPE_MARKET:			clMarketStrInfo.Set(&pclinfo->clMarketStrInfo);						break;
		case RANKTYPE_HUNT:				clHuntStrInfo.Set(&pclinfo->clHuntStrInfo);							break;
		case RANKTYPE_HOUSE:			clHouseStrInfo.Set(&pclinfo->clHouseStrInfo);						break;
		case RANKTYPE_CITYHALL:			clCityhallStrInfo.Set(&pclinfo->clCityhallStrInfo);					break;
		case RANKTYPE_POSTOFFICE:		clPostOfficeStrInfo.Set(&pclinfo->clPostOfficeStrInfo);				break;
		case RANKTYPE_HORSEMARKET:		clHorseMarketStrInfo.Set(&pclinfo->clHorseMarketStrInfo);			break;
		case RANKTYPE_REALESTATEMARKET: clRealEstateMarketStrInfo.Set(&pclinfo->clRealEstateMarketStrInfo); break;
		case RANKTYPE_SUMMONMARKET:		clSummonMarketStrInfo.Set(&pclinfo->clSummonMarketStrInfo);			break;
		case RANKTYPE_LAND:				clLandStrInfo.Set( &pclinfo->clLandStrInfo );						break;
		case RANKTYPE_FEAST:			clFeastStrInfo.Set( &pclinfo->clFeastStrInfo );						break;
		case RANKTYPE_SUMMONHEROMARKET: clSummonHeroMarketStrInfo.Set(&pclinfo->clSummonHeroMarketStrInfo); break;
		case RANKTYPE_MINE:				clMineStrInfo.Set( &pclinfo->clMineStrInfo );						break;
		case RANKTYPE_GUILD:			clGuildStrInfo.Set( &pclinfo->clGuildStrInfo );						break;		
		case RANKTYPE_NEWMARKET:		clNewMarketStrInfo.Set( &pclinfo->clNewMarketStrInfo);			break;		
			
		default:
			TCHAR buffer[128];
			StringCchPrintf(buffer, 128, TEXT("cltStrInfo"), TEXT("Strtype:%d"), clCommonInfo.siStrType);
			MessageBox(NULL, buffer, TEXT("Err"), MB_OK|MB_TOPMOST);
		}
	}

	SI32 GetTotalSize()
	{
		SI32 size = sizeof(clCommonInfo);

		switch(clCommonInfo.siStrType)
		{
		case RANKTYPE_BANK:				size += sizeof(cltBankStrInfo);				break;
		case RANKTYPE_STOCK:			size += sizeof(cltStockStrInfo);			break;
		case RANKTYPE_MARKET:			size += sizeof(cltMarketStrInfo);			break;
		case RANKTYPE_HUNT:				size += sizeof(cltHuntStrInfo);				break;
		case RANKTYPE_HOUSE:			size += sizeof(cltHouseStrInfo);			break;
		case RANKTYPE_CITYHALL:			size += sizeof(cltCityhallStrInfo);			break;
		case RANKTYPE_POSTOFFICE:		size += sizeof(cltPostOfficeStrInfo);		break;
		case RANKTYPE_HORSEMARKET:		size += sizeof(cltHorseMarketStrInfo);		break;
		case RANKTYPE_REALESTATEMARKET:	size += sizeof(cltRealEstateMarketStrInfo); break;
		case RANKTYPE_SUMMONMARKET:		size += sizeof(cltSummonMarketStrInfo);		break;
		case RANKTYPE_LAND :			size += sizeof(cltLandStrInfo);				break;		
		case RANKTYPE_FEAST :			size += sizeof(cltFeastStrInfo);			break;
		case RANKTYPE_SUMMONHEROMARKET: size += sizeof(cltSummonHeroMarketStrInfo); break;
		case RANKTYPE_MINE :			size += sizeof(cltMineStrInfo );			break;
		case RANKTYPE_GUILD:			size += sizeof(cltGuildStrInfo );			break;
		case RANKTYPE_NEWMARKET:		size += sizeof(cltNewMarketStrInfo);		break;

		default:				size += MAX_STR_DATA_SIZE; break;
		}
		return size;
	}
	
	void IncreaseMoney(cltMoney clmoney)
	{
		clCommonInfo.clMoney.Increase(&clmoney);
	}
};



class cltStructureBasic{
public:
	SI32 siVillageUnique;	// 전장이 위치한 마을의 유니크값. 
	cltStrInfo clStrInfo;


	cltStructureBasic(SI32 sivillageunique);
	virtual ~cltStructureBasic();
	

	//----------------------
	// 전장 
	//----------------------
	virtual BOOL GetAcountInfo(cltAccountNum* pclaccountnum, cltBankAccount* pclAcountInfo){return FALSE;}
	virtual BOOL GetAcountInfo(SI32 personid, cltBankAccount* pclaccount){return FALSE;}
	virtual BOOL SetFee(cltFee* pclfee){return FALSE;}
	virtual GMONEY CalcFee(GMONEY money){return 0;}
	virtual bool SetBankVary(cltBankVary* pclvary){return false;}

	//----------------------
	// 상단 
	//----------------------
	virtual BOOL SetCandidate(SI32 index, cltSimpleRank* pclrank, cltSimplePerson* pclcandidate){return FALSE;}
	virtual BOOL DeleteCandidate(SI32 personid){return FALSE;}
	virtual BOOL DeleteCandidate(cltSimpleRank* pclrank){return FALSE;}
	virtual cltSimplePerson* GetCandidateInfoFromPersonID(SI32 ranktype, SI32 personid, SI32* pindex){return NULL;};
	virtual SI32 GetCandidateNum(SI32 ranktype){return 0;};
	virtual cltSimplePerson* GetCandidateInfoFromRef(SI32 ranktype, SI32 ref){return NULL;}
	virtual BOOL DeleteRank(SI32 ranktype){return FALSE;}


	//----------------------------
	// 시전. 
	//----------------------------
	virtual BOOL SetRentContract(const cltRentContract* pclcontract){return FALSE;}
	virtual cltHouseUnit* GetHouseUnit(SI32 index){return NULL;}
	virtual void SetHouseUnit(SI32 index, cltHouseUnit* pclunit){}
	virtual void SetHouseApplyer(SI32 index, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract){}
	virtual cltRentApplyer* GetHouseApplyer(SI32 index){return NULL;}
	virtual	BOOL IsSameContract(const cltRentContract* pclcontract){return FALSE;};
	virtual BOOL AddApplyer(const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract ){return FALSE;}
	virtual BOOL DeleteApplyer(SI32 personid){return FALSE;}
	virtual BOOL IsEmpty(SI32 slot){return FALSE;};
	virtual cltRentApplyer* GetHouseApplyerFromPersonID(SI32 personid){return NULL;}
	virtual BOOL SetRent(SI32 slot, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract){return FALSE;}
	virtual bool IsHouseHost(SI32 personid, SI32 houseunitslot){return false;}
	virtual bool CanAddStg(SI32 houseunitslot, cltItemManagerCommon* pclItemManager, cltItem* pclitem, SI32* pitempos){return false;};
	virtual void SetItemStg(SI32 houseunitslot, SI32 stgpos, cltItem* pclitem){};
	virtual cltItem* GetStgItemPointer(SI32 houseunitslot, SI32 stgpos){return NULL;};
	virtual cltHouseUnitStorageInfo* GetStgPointer(SI32 houseunitslot){return NULL;}
	virtual void InitHouseApplyer(){}


	cltStrInfo* GetStrInfo();
	void SetStrInfo(cltStrInfo* pclinfo);


};

class cltStructureMngBasic{
public:
	cltStructureMngBasic(SI32 sitabctrlnum, cltCharManager* pclcm, 	SI32 ranktype, TCHAR *strFileName );	
	virtual ~cltStructureMngBasic();
	
	SI32 siTabCtrlNum;												// 탭 컨트롤이 몇개 있는가 ?

	SI32 siRankType;												// 어떤 종류의 건물인가. 
	
#ifdef _SAFE_MEMORY
	NSafeTArray<cltStructureBasic*, MAX_VILLAGE_NUMBER>		pclStruct;
#else
	cltStructureBasic* pclStruct[MAX_VILLAGE_NUMBER];				// 모든 마을에서의 각 건물들의 객체 
#endif

	// 클라이언트
	cltStrInfo	clClientStrInfo;									// 클라이언트가 알고 있어야 할 건물 정보. 

	cltCharManager* pclCM;											// 캐릭터 매니저 포인터
	
	SI32 CharUnique;												// 캐릭터 유니크
	SI32 siVillageUnique;											// 해당 마을 유니크 

	BOOL DlgRectInitSwitch;
	RECT DlgRect;

	TCHAR szTitle[128];												// 타이틀 

	SI32 siServerResponseValue;										// 서버가 보내온 결과값

	TCHAR m_strFileName[ _MAX_PATH ];								// 초상화 파일명( siPortaitID 가 0 이라면 strFileName 으로 대체하게 했음. )

	bool UpdateChiefSwitch;
	bool bDrawPortraitSwitch;

	TCHAR szMainExplain[ 1024 ];
	DWORD dwLastExplainTick;

	cltStructureRepairDlg* pclStructureRepairDlg;

	BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	// 서버로부터 받은 건물 정보를 가지고 클라이언트의 건물을  설정한다. 
	void SetClientStrInfo(cltStrInfo* pclinfo);

	// 초상화를 그려준다. 
	void DrawPortrait(TCHAR *paccountid);

	// 메인 설명창을 설정한다. 
	BOOL SetMainExplain(HWND hdlg, TCHAR* pstr);

	// 서버 리턴값 처리 함수. 
	virtual void ServerRtnValOperate();
	
	virtual void OnTimer() {};

	void DeleteTabDlg();
 
	// 장의 정보를 보여준다. 
	BOOL ShowChief(HWND hDlg, SI32 dlgitem, cltSimplePerson* pclperson, TCHAR *paccountid);

	void GetTitleText(SI32 villageunique);

	// 최초 환영 인사말표시 . 
	virtual void GreetingMsg(HWND hDlg)		= 0;
	virtual void Do_WM_INITDIALOG(SI32 charunique);

	cltStrInfo* GetStrInfo(SI32 villageunique);
	void SetStrInfo(SI32 villageunique, cltStrInfo* pclinfo);

	bool GetHighestLevelVillage(SI32* villageunique,SI32* level);

	// 특정 건물의 보유 자금을 설정한다
	void SetMoney(SI32 villageunique, cltMoney* pclmoney);

	// 특정 건물의 보유 자본금을 설정한다
	void SetBaseMoney(SI32 villageunique, cltMoney* pclmoney);

	// 건물의 특정 년도의 세금을 설정한다 
	void SetTax( SI32 villageunique, UI16 year, cltMoney* pclTax );

	// 건물의 특정 년도의 납부된 세금을 설정한다
	void SetPaidTax( SI32 villageunique, UI16 year, cltMoney* pclPaidTax );
	
	void RequestStrInfo( SI32 strType );

	// 건물 수리 다이얼로그를 생성한다 
	void CreateStructureRepairDlg( HINSTANCE hInst, HWND hParent );

	void CommonAction(cltTime* pcltime);

	DWORD tickTemp;
	BOOL  bSetNeedItem;

	NStrBaseDlg		*pStrBaseDlg;			// 풀스크린 모드일 경우 다이얼로그 포인터, 창모드일경우는 NULL
};

#endif
 
