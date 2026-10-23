//---------------------------------
// 2004/10/6 김태곤
//---------------------------------
#ifndef _STATISTICS_H
#define _STATISTICS_H

#include <Directives.h>

//---------------------------------
// Common
//---------------------------------
#include "..\..\Common\CommonHeader.h"
#include "..\..\Common\Quest\Quest.h"

//-------------------------------
// 게임 서버 통계 
//-------------------------------
class cltStatistics{
public:

	//-------------------------------
	// 로그인 사용자 수 
	//-------------------------------
	SI32 siLoginNumber;

	//-------------------------------
	// 채팅 수 
	//-------------------------------
	SI32 siChatNumber;

	//-------------------------------
	// 수렵장 이용자수. 
	//-------------------------------
	SI32 siHuntUserNumber;

	//-------------------------------
	// 일반 던전 이용자수. 
	//-------------------------------
	SI32 siDungeonUserNumber;
	//-------------------------------
	// 인스턴스 던전 이용자수
	//-------------------------------
	SI32 siInstanceDungeonUserNumber;

	//-------------------------------
	// 장인 정보 이용 횟수. 
	//-------------------------------
	SI32 siMasterInfoUseNumber;

	//-------------------------------
	// 정상 비정상 사용자 통계. 
	//-------------------------------
	SI32 siAbnormalQuitUserNumber;
	SI32 siNormalQuitUserNumber;

	//------------------------------
	// 주식 거래량 통계 
	//------------------------------
	SI32 siStockTradeNumber;

	//------------------------------
	// 클라이언트와의 메시지 전송 통계 
	//----------------------------------
	SI32 siReceiveFromClientMsgNumber;
	SI32 siSendToClientMsgNumber;

	//---------------------------------
	// DB와의 메시지 전송 통계. 
	//--------------------------------
	SI32 siReceiveFromDBMsgNumber;
	SI32 siSendToDBMsgNumber;

	//-------------------------------
	// 제조 통계 
	//-------------------------------
	GMONEY siMakeMoney;		
	//--------------------------------
	// 이벤트 통계 
	//--------------------------------
	// 불가사리 사냥 이벤트 횟수 
	SI32 siHuntingEventNumber;

	// 고구려 이벤트 횟수 
	SI32 siGoguEventNumber;

	// 강화도 이벤트 횟수
	SI32 siKanghwadoEventNumber;

	//-------------------------------
	// 수호천사 
	//-------------------------------
	SI32 siAngelNumber;

	//-------------------------------
	// 레벨 50이상 추천인수. 
	//-------------------------------
	SI32 siRecommenderNumber;

	//-------------------------------
	// 사용자 행동 통계 
	//-------------------------------
	SI32 siFishUserNumber;
	SI32 siFarmUserNumber;
	SI32 siMineUserNumber;
	SI32 siSummonNumber;

	//------------------------------
	// 부적 사용 통계. 
	//------------------------------
	SI32 siScrollSummonNumber;
	// 부적 이용 개조 건수 통계 
	SI32 siScrollTuneNumber;

	//-----------------------------
	// SBS 유저 수 통계 
	//-----------------------------
	SI32 siCurSBSUserNum;				// 현재 SBS동접. 

	//-----------------------------
	// 대박 물품 통계 
	//-----------------------------
	SI32 siBigItemNum;

	//-----------------------------
	// 흥부박 퀘스트 완료  통계 
	//-----------------------------
	SI32 siSpecialQuestDoneNum;

	//------------------------------
	// System이 구입해주는 양 
	//------------------------------
	GMONEY siSystemBuyMoney;
	SI32 siSystemBuyNum;

	//------------------------------
	// 말 구입 양 
	//-----------------------------
	SI32 siSystemBuyHorseNum;
	GMONEY siSystemBuyHorseMoney;

	//-----------------------------
	// 무역상 거래량 
	//-----------------------------
	SI32 siTradeMerchantNum;
	GMONEY siTradeMerchantMoney;
	GMONEY siTradeTax;

	//-----------------------------
	// 거간 거래량 
	//-----------------------------
	SI32 siRealEstateTradeNum;
	GMONEY siRealEstateTradeMoney;


	//-----------------------------
	// 모병관 퀘스트 수행 통계 
	//-----------------------------
	SI32 siDailyQuestAcceptNum;
	SI32 siDailyQuestClearNum;

	//-----------------------------
	// 사복시 통계 
	//-----------------------------
	SI32 siHorseMarketTradeNumber;
	GMONEY siHorseMarketTradeMoney;

	//-----------------------------
	// 장예원 통계 
	//-----------------------------
	SI32 siSummonMarketTradeNumber;
	GMONEY siSummonMarketTradeMoney;


	//-----------------------------
	// 시전통계 
	//----------------------------
	SI32 siTotalHouseNumber;
	SI32 siRentHouseNumber;
	SI32 siRecallHouseNumber;


	//-----------------------------
	// 내자시 통계
	//----------------------------
	SI32 siFeastUseNumber;
	GMONEY siFeastUseMoney;


	//-----------------------------
	// 명성등급 상승 공지 통계 
	//-----------------------------
	SI32 siFameLevel10Num;
	SI32 siFameLevel20Num;
	SI32 siFameLevel30Num;
	SI32 siFameLevel40Num;

	//-----------------------------
	// 퀘스트 통계 
	//-----------------------------
	SI32 siQuestClear[MAX_QUEST_TYPE][2]; // 


	//-----------------------------
	// 명나라 상인 구입정보
	//-----------------------------
	GMONEY siForeignMerchantBuyGameItemMoney;
	GMONEY siForeignMerchantBuyCashItemMoney;

	//-----------------------------
	// 말 풀어준 횟수 
	//-----------------------------
	SI32 siReleaseHorse;

	cltStatistics()
	{
		Init();
	}

	~cltStatistics()
	{

	}

	void Init()
	{
		ZeroMemory(this, sizeof(cltStatistics));
	}

	// 중국어 사용자
	SI32 siChineseUserCount;

	// 자신의 통계 데이터를 파일에 쓴다.
	void Write(TCHAR * pszFileName ,_SYSTEMTIME sTime, cltQuestManager* pclQuestManager);

	// 인자로 받은 클래스의 값을 자신의 값에 더한다.
	void Add( cltStatistics * pclAddStatistics );	

	// 중국어 사용자 통계
	void WriteChineseUser( TCHAR * pszFileName , _SYSTEMTIME sTime );	
};

class cltStatisticsManager
{
	SI32 siLastWriteenDay;
	SI32 siLastWriteenWeek;
	
	bool bIsBackUpDateLoaded;

public:
	cltStatistics	clDailyStatistics;
	cltStatistics	clWeeklyStatistcs;

	// 일정 주기마다 통계 파일을 업데이트한다.
	void WriteAction(_SYSTEMTIME sTime, cltQuestManager* pclQuestManager);

	// 당일 분량의 통계를 파일에 기록한다. 
	void WriteDaily(_SYSTEMTIME sTime, cltQuestManager* pclQuestManager);

	// 매주 일주일 분량의 통계를 파일에 기록한다.
	void WriteWeekly(_SYSTEMTIME sTime, cltQuestManager* pclQuestManager);

	void WriteDailyBackUp(TCHAR * pszFileName ,_SYSTEMTIME sTime);

	void ReadDailyBackUp(TCHAR * pszFileName , _SYSTEMTIME sTime);

	void WriteWeeklyBackUp(TCHAR * pszFileName ,_SYSTEMTIME sTime);

	void ReadWeeklyBackUp(TCHAR * pszFileName , _SYSTEMTIME sTime);

	// [희영] 시세확인 2008-07-18
	void ReadMarketConditions(_SYSTEMTIME sTime);

	cltStatisticsManager()
	{
	}
	~cltStatisticsManager()
	{
	}

	void Init();

};


#endif