//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _VILLAGE_H
#define _VILLAGE_H

#define VILLAGEWAR_DECLARE		1
#define VILLAGEWAR_DECLARED		2

#define CUR_VILLAGE_LIMIT	88		// 마을의 최종 VillageUnique 몇개나 추가하는가? - 현재 서귀포까지 

#define CUR_GLOBAL_VILLAGE_LIMIT	42		// 마을의 최종 VillageUnique 몇개나 추가하는가? - 현재 서귀포까지 

#ifdef _JAPAN
	#define NEWMARKET_REPAIR_RANKING	20		// [진성] 통합마켓 건물 수리가능한 마을 순위.
#else
	#define NEWMARKET_REPAIR_RANKING	5		// [진성] 통합마켓 건물 수리가능한 마을 순위.
#endif

#include <windowsx.h>

#include <Directives.h>
#include "..\..\Server\Rank\Rank.h"
#include "..\..\Common\Construction\Construction.h"

#include "NDate.h"

#include <algorithm>
using std::sort;

class cltStockAccount;
class cltBankManager;
class cltStockManager;
class cltCityHallManager;
class cltMarketManager;
class cltHouseManager;
class cltHuntManager;
class cltPostOfficeManager;
class cltHorseMarketManager;
class cltRealEstateMarketManager;
class cltSummonMarketManager;
class cltLandManager;
class cltFeastManager;
class cltSummonHeroMarketManager;
class cltMineManager;
class cltGuildManager;
//class cltNewMarketManager;
class cltNewMarketManager;

class cltVillageManager;
class cltMapManager;

//[추가 : 황진성 2008. 1. 22 => 흑의 군단 마을 점수 관련 클레스 추가.]
class BlackWarVillageScore
{
private:
	SI32	m_siVillageUnique;
	SI32	m_siVillageScore;

public:
	BlackWarVillageScore(){ }
	~BlackWarVillageScore(){ }
	BlackWarVillageScore(SI32 _VillageUnique, SI32 _VillageScore) : m_siVillageUnique(_VillageUnique),
																	m_siVillageScore(_VillageScore)
	{ }

	void SetVillageUnique(SI32 _VillageUnique)	 { m_siVillageUnique = _VillageUnique;	}
	SI32 GetVillageUnique(void)					 { return m_siVillageUnique;			}

	void SetVillageScore(SI32 _VillageScore)	 { m_siVillageScore = _VillageScore;	}
	SI32 GetVillageScore(void)					 { return m_siVillageScore;				}

	void AddVillageScore(SI32 _VillageAddScore) { m_siVillageScore += _VillageAddScore;	}
};

// 흑의 군단 마을 점수로 정열시 비교할때 사용.
struct compare_Village_Score
{
	bool operator() ( BlackWarVillageScore& _Value1, BlackWarVillageScore& _Value2)
	{
		return _Value1.GetVillageScore() > _Value2.GetVillageScore();
	}					
};

//------------------------------------
// 저장되어야 할 마을 정보. 
//------------------------------------
class cltVillageStockPrice{
public:
	SI32 siVillageUnique;	
	GMONEY siCurrentStockPrice;		// 현재의 마을 주가 
	GMONEY siOldStockPrice;			// 이전의 마을 주가. 

	cltVillageStockPrice()
	{
		siVillageUnique		= 0;
		siCurrentStockPrice = 0;
		siOldStockPrice		= 0;
	}

	void Set(cltVillageStockPrice* pprice)
	{
		memcpy(this, pprice, sizeof(cltVillageStockPrice));
	}

};


//--------------------------------------------
// 마을의 핵심 정보 
// 이 정보들이 DB에 저장되게 된다. 
//-------------------------------------------
class cltVillageVary{
public:

#ifdef _SAFE_MEMORY
	NSafeTArray<cltSimplePerson, MAX_RANKTYPE_NUMBER>		clRankPerson;
#else
	cltSimplePerson			clRankPerson[MAX_RANKTYPE_NUMBER];
#endif
	cltVillageStockPrice	clVillageStockPrice;						// 마을 주가 정보. 
	cltVillageInvestInfo	clInvestInfo;								// 마을 주주 정보 

	CConstruction			clStructureInfo;							// 건물 정보. 

	SI32					siMasterVillage;							// 최고부모 마을 
	SI32					siParentVillage;							// 부모 마을 

	SI32					siVillageMark;								// 마을 마크 코드 
	
	SI32					siResidentsScore;							// 마을 주민 점수
	SI32					siVillageScore;								// 마을 점수
	SI32					siTotalVillageScore;						// 마을 총 점수 ( siResidentsScore + siVillageScore + siVillageWarScore);
	SI32					siVillageWarKillNumber;						// 마을 전쟁 전공수
	SI32					siVillageEventSuccessNumber;				// 마을 이벤트 성공횟수
	SI32					siNewYearEventScore;						// 구리새댁 이벤트
	//SI32					siVillageWarScore;							// 마을 공성전으로 얻게 되는 점수

	bool					m_bGeneralMeetingSuggest;					//[추가 : 황진성 2008. 2. 20 => 주주총회 발동 플레그, 운영자 명령어로 변경 가능.]

	cltVillageVary()
	{
		siMasterVillage = 0;
		siParentVillage = 0;
		siVillageMark = 0;

		siResidentsScore = 0;
		siVillageScore = 0;
		siTotalVillageScore = 0;		
		siVillageWarKillNumber = 0;
		siVillageEventSuccessNumber = 0;
		siNewYearEventScore =0;

		//siVillageWarScore = 0;
		m_bGeneralMeetingSuggest = 0;
	}
};

//------------------------------------------
// 개별 마을의 정보 
//------------------------------------------
class cltVillageInfo{
public:
	cltVillageManager* pclVillageManager;

	SI32 siVillageUnique;	
	DWORD dwAtb;									// 속성의 특성. 
	TCHAR szName[MAX_PATH];

	SI32 TitleImageXsize, TitleImageYsize;

	UI08* pTitleBuffer;								// 마을 이름이 쓰여질 공간. 
	UI08* pTitleEncBuffer;

	bool bEnemyCaptureSwitch;						// 적군이 이마을을 점령했는가. 
	SI16 siEnemyNum;								// 마을에 침입한 적의 숫자를 기록한다. 
	SI16 siPersonalShopNum;							// 마을안에 있는 개인 상점의 개수. 

	SI32 PosX, PosY;								// 이 마을이 메인 지도의 어느 좌표에 있는가 

	SI16 siWarKillNumber;							// 전쟁에서 이 마을 주민들이 처치한 적의 수. 
	SI16 siUserNum;									// 마을안의 사용자 수
	

	DWORD dwLastVillageGodClock;
	DWORD dwLastCastleClock;
	DWORD dwLastAttackedVillageClock;

	SI16 siVillageGodCharUnique;					// 마을의 성황신 캐릭터 Unique
	SI16 siCastleCharUnique;						// 마을 성문 캐릭터 유니크. 
	SI16 siKingMonumentUnique;							// 마을 수호상 유니크

	BOOL bAttackedVillage;

	bool bOsakaWarSwitch;							// 마을전공이 1000명이상이면 true

private:	
	SI32 siVisitorNumber;							// 이 마을의 방문객 수. 
	SI32 siVisitLevel;								// 방문자수에 의한 레벨. 

public:
	cltVillageVary clVillageVary;					// 저장 되어야할 마을 정보. 

	TCHAR	RankAccountID[MAX_RANKTYPE_NUMBER][MAX_PLAYER_NAME];	// 마을의 각 행수의 AccountID


	SI32 siVillageType;								// 마을의 타입. 
	SI32 siSeason;									// 계절 
	bool bFarmSwitch;								// 농장 가능한지  여부. 
	bool bIronSwitch;								// 철광산 가능한지  여부. 
	bool bUpgradeSwitch;							// 레벨에 따른 업그레이드된 마을인지
	
	cltVillageInfo(cltVillageManager* pclvillageset, SI32 villagemode, SI32 siunique, DWORD atb, TCHAR* name, SI32 villageposx, SI32 villageposy, SI32 mapindex, SI32 season, bool bfarm, bool biron );
	~cltVillageInfo();

	BOOL  LoadTitle();

	BOOL Action();

	//==========================
	// 수호상
	//==========================
	BOOL CreateVillageKingMonument();

	//==========================
	// 성황신 
	//==========================
	// 현재 성황신의 상태를 DB에 저장하도록 한다.
	BOOL SetVillageGodtoDB();
	// 성황신을 새로 생성한다.
	BOOL CreateVillageGod();

	//==========================
	// 성문 
	//==========================
	// 성문의 상태를 DB에 저장하도록 한다.
	BOOL SetCastletoDB();
	// 성문을 새로 생성한다.
	BOOL CreateCastle();
	//==========================
	// 특정 시설이 마을에 있는지 여부를 확인한다. 
	BOOL HasStructure(SI32 structureunique);

	// 마을 주가를 설정한다. 
	void SetCurrentStockPrice(GMONEY price)
	{
		clVillageVary.clVillageStockPrice.siCurrentStockPrice = price;
	}

	// 마을 주가를 얻어온다. 
	GMONEY GetCurrentStockPrice()const
	{
		return clVillageVary.clVillageStockPrice.siCurrentStockPrice;
	}

	// PersonID가 최대 주주인지 확인한다. 
	BOOL IsMostBeholder(SI32 personid);

	// 마을을 출력한다.
	void Draw(SI32 dotx, SI32 doty);

public:
	//--------------------------------------------
	// Village-DB.cpp
	//--------------------------------------------
	// DB에서 전장 정보를 읽어와서 업데이트 한다. 
	BOOL LoadBankFromDB();
	// DB에서 여각 정보를 읽어와서 업데이트 한다. 
	BOOL LoadStockFromDB();
	// DB에서 상단 정보를 읽어와서 업데이트 한다. 
	BOOL LoadCityHallFromDB();
	// DB에서 객주 정보를 읽어와서 업데이트 한다. 
	BOOL LoadMarketFromDB();
	// DB에서 시전 정보를 읽어와서 업데이트 한다. 
	BOOL LoadHouseFromDB();
	// DB에서 수렵장 정보를 읽어와서 업데이트 한다. 
	BOOL LoadHuntFromDB();
	// DB에서 역참 정보를 읽어와서 업데이트 한다. 
	BOOL LoadPostOfficeFromDB();
	// DB에서 사복시 정보를 읽어와서 업데이트 한다.
	BOOL LoadHorseMarketFromDB();
	// 거간 
	BOOL LoadRealEstateMarketFromDB();
	// 장예원
	BOOL LoadSummonMarketFromDB();
	// 농장 
	BOOL LoadLandFromDB();
	// 내자시 
	BOOL LoadFeastFromDB();
	// 비변사 
	BOOL LoadSummonHeroMarketFromDB();
	// 광산
	BOOL LoadMineFromDB();
	// 길드
	BOOL LoadGuildFromDB();

	//[진성] 통합마켓.
	BOOL LoadNewMarketFromDB();	

	void IncVisitNum(SI32 inc)
	{
		siVisitorNumber += inc;
	}
	
	SI32 GetVisitNum()
	{
		return siVisitorNumber;
	}
	void SetVisitLevel(SI32 level)
	{
		siVisitLevel = level;
	}
	SI32 GetVisitLevel()
	{
		return siVisitLevel;
	}

};


class cltVillageManager 
{
public:
	cltBankManager*								pclBankManager;
	cltStockManager*							pclStockManager;
	cltCityHallManager*							pclCityHallManager;
	cltMarketManager*							pclMarketManager;
	cltHouseManager*							pclHouseManager;
	cltHuntManager*								pclHuntManager;
	cltPostOfficeManager*						pclPostOfficeManager;
	cltHorseMarketManager*						pclHorseMarketManager;
	cltRealEstateMarketManager* 				pclRealEstateMarketManager;
	cltSummonMarketManager*						pclSummonMarketManager;
	cltLandManager*								pclLandManager;
	cltFeastManager*							pclFeastManager;
	cltSummonHeroMarketManager*					pclSummonHeroMarketManager;
	cltMineManager*								pclMineManager;
	cltGuildManager*							pclGuildManager;
	//cltNewMarketManager*						pclNewMarketManager;
	cltNewMarketManager*						pclNewMarketManager;

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_VILLAGE_NUMBER>		siDeclareWarVillage;						// 공성전을 선포한 예약된 정보.  
	NSafeTArray<SI32, MAX_VILLAGE_NUMBER>		siNowWarVillage;							// 현재 공성전 중인 정보. 현재 진행중 

	NSafeTArray<NDate, MAX_VILLAGE_NUMBER>		clDeclareWarDate;							// 공성전을 선포한 시간,시각
#else
	SI32										siDeclareWarVillage[ MAX_VILLAGE_NUMBER ];	// 공성전을 선포한 예약된 정보.  
	SI32										siNowWarVillage[ MAX_VILLAGE_NUMBER ];		// 현재 공성전 중인 정보. 현재 진행중 

	NDate										clDeclareWarDate[ MAX_VILLAGE_NUMBER ];		// 공성전을 선포한 시간,시각
//	cltDate										clLastWarLoseDate[ MAX_VILLAGE_NUMBER ];	// 공성전에서 마지막으로 패배한 시간. 일주일간 공성전에서 보호하는 용도로 쓰임.
#endif

	SI32 siVillageMode;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltVillageInfo*, MAX_VILLAGE_NUMBER>	pclVillageInfo;
#else
	cltVillageInfo* pclVillageInfo[MAX_VILLAGE_NUMBER];
#endif

	DWORD		dwLastEnemyNumCountClock;			// 각 마을의 적군 수를 파악한 마지막 시간. 


	bool		bCheckVisitLevelSwitch;				// 마을 방문자 수가 유효한가 ?

	SI32		siInstanceProfitVillageunique;		// 인스턴스 던전 이익금 가질 마을
	SI64		siTempInstanceProfit;				// 인스턴스 던전 이익금
	DWORD		dwLastInstanceProfit;				// 마지막으로 인스턴스 이익금 준 시간

#ifdef _SAFE_MEMORY
	NSafeTArray<BlackWarVillageScore, MAX_VILLAGE_NUMBER>		cBlackWarVillageScore;
#else
	BlackWarVillageScore	cBlackWarVillageScore[MAX_VILLAGE_NUMBER];			//[추가 : 황진성 2008. 1. 22 => 흑의 군단 발동후 마을점수 저장. 마을 주민의 점수가 더해저 만들어짐.]
#endif

	cltVillageManager(SI32 villagemode);

	~cltVillageManager();

	// unique값의 마을을 추가한다. 
	BOOL AddVillage(SI32 siunique, TCHAR * pname, SI32 villageposx, SI32 villageposy, SI32 mapindex, SI32 season, bool bfarm, bool biron );

	// 마을 정보를 삭제한다.
	BOOL DeleteVillage();

	BOOL IsAggressiveVillage( SI32 village1, SI32 village2 );

	SI32 FindVillageUniqueFromName(TCHAR* name);

	BOOL Action();

	// 마을 주가를 설정한다. 
	void SetCurrentStockPrice(SI32 villageunique, GMONEY price);
	
	// 마을의 이름을 얻어온다. 
	TCHAR* GetName(SI32 villageunique);
	
	// 마을의 이름을 근거로 맵인덱스를 찾는다. 
	SI32 GetMapIndexFromName(TCHAR* name);

	// 임의의 마을을 찾는다. 
	SI32 FindRandVillage();

	//-----------------------------------------
	// 성황신 
	//-----------------------------------------
	// 각 마을의 성황신의 최대 Life를 얻어온다.
	SI32 GetMaxLifeOfGodInVillage(SI32 villageunique);
	// 각 마을의 성황신의 Life를 설정한다.
	BOOL SetLifeOfGodInVillage(SI32 villageunique, SI32 life);
	// 각 마을의 성황신의 Mana를 설정한다.
	BOOL SetManaOfGodInVillage(SI32 villageunique, SI32 mana);
	// 각 마을의 성황신의 Capa를 설정한다.
	BOOL SetCapaOfGodInVillage(SI32 villageunique, SI32 capa);

	// 각 마을의 성황신의 Capa를 변경한다.
	BOOL AdjustCapaOfGodInVillage(SI32 villageunique, SI32 capa);

	// 마을의 성황신을 바꾼다. (villageunique 에서 villagegodkind 가 가능함을 이미 검사했어야 한다.)
	BOOL ChangeVillageGod(SI32 villageunique, SI32 villagegodkind);
	// 모든 마을의 성황신의 상태를 저장한다.
	void SetAllVillageGodtoDB();

	// 해당 마을에 VillageGod이 존재하는지 
	BOOL IsThereVillageGod(SI32 villageunique,SI32 *siGodCharUnique);

	//-----------------------------------------
	// 성문 
	//-----------------------------------------
	// 각 마을의 성문의 최대 Life를 얻어온다.
	SI32 GetMaxLifeOfCastleInVillage(SI32 villageunique);
	// 각 마을의 성문의 Life를 설정한다.
	BOOL SetLifeOfCastleInVillage(SI32 villageunique, SI32 life);
	// 각 마을의 성문의 Mana를 설정한다.
	BOOL SetManaOfCastleInVillage(SI32 villageunique, SI32 mana);
	// 각 마을의 성문의 Capa를 설정한다.
	BOOL SetCapaOfCastleInVillage(SI32 villageunique, SI32 capa);

	// 각 마을의 성문의 Capa를 변경한다.
	BOOL AdjustCapaOfCastleInVillage(SI32 villageunique, SI32 adjustcapa);

	// 마을의 성문  바꾼다. (villageunique 에서  castlekind 가 가능함을 이미 검사했어야 한다.)
	BOOL ChangeCastle(SI32 villageunique, SI32 castlekind);

	// 모든 마을의 성문 상태를 저장한다.
	void SetAllCastletoDB();
	
	// 해당 마을에 Castle이 존재하는지 
	BOOL IsThereCastle(SI32 villageunique, SI32 *siCastleUnique);
	//-----------------------------------------

public:
	//-----------------------------------------
	// 클래스 외부에서 사용하는 멤버 함수 
	//-----------------------------------------

	//Person이 villageunique의 최대 주주인지 확인한다. 
	BOOL IsMostBeholder(SI32 villageunique, SI32 personid);

	// 주어진 좌표로부터 가까운 마을을 찾는다. 
	SI32 FindNearVillage(SI32 x, SI32 y);
	
	// 마을 전공을 초기화한다. 
	void InitWarKillNumber();

	// 마을 전공을 소트한다. 
	void WarKillNumberSort(SI16* psiVillageUnique,  SI16* psiVillageWarKillNumber);
	void WarOsakaWarpSwitch();

	// 현재 마을에 있는 사용자를 구한다.
	void GetCurrentVillageUserNum();

	// 마을간 전쟁 결과를 설정한다 
	void SetWarResult( SI32 siWinVillage, SI32 siLoseVillage );

	// 
	BOOL GetVillageRelation( SI32 village, SI32 slaveVillage );

	// 모든 마을의 선전포고 상태를 초기화한다
	void ResetDeclareWar();

	
	// 마을의 타입을 구한다. 
	SI32 GetVillateType(SI32 villageunique)
	{
		if(pclVillageInfo[villageunique] == NULL)return false;

		return pclVillageInfo[villageunique]->siVillageType;
	}
	
	// 유효한 마을인가?
	bool IsValidVillage(SI32 villageunique)
	{
		if(villageunique <= 0)return false;
		if(villageunique >= MAX_VILLAGE_NUMBER)return false;

		if(pclVillageInfo[villageunique] == NULL)return false;

		return true;
	}
	
	BOOL IsThereStructure( SI32 villageunique, SI32 ranktype );
	

	// 유효한 마을 가운데 주식이 전혀 없는 마을을 찾아서 리턴한다. 
	SI32 FindNoStockVillage();

	
	// 마을 방문자수에 의한 레벨을 구한다. 
	void CalcVisitLevel();
	// 마을 방문자수에 의한 레벨을 구한다. 
	bool GetVisitLevel(SI32 villageunique, SI32* plevel);

	// 마을 안의 개인 상점 개수를 파악한다. 
	SI32 GetPersonalShopNumber(SI32 villageunique);

	// 모든 마을을 삭제한다. 
	void DelAllVillage();

	BOOL LoadVillage(SI32 gamemode, SI32 servicearea);
	
	// 해당 마을이 전쟁중인지 체크
	BOOL IsVillageAtWar(SI32 villageunique);
	
	// 해당마을의 마을점수 순위를 구해옴
	SI32 GetVillageRank( SI32 villageunique );

	BOOL AdjustAllStrDurInVillage( SI32 villageunique, SI32 adjustDur, SI32 lowest);

	bool GetPosFromStrUnique(SI32 villageUnique, SI32 strunqiue, OUT SI32* xpos, OUT SI32* ypos );

	void FindLowTotalVillageScore( SI32* VillageUnique );

	//[추가 : 황진성 2008. 1. 22 => 흑의 군단 모든 정보 초기화.]
	void BlackWarRecordInit(void)
	{
#ifdef _SAFE_MEMORY
		cBlackWarVillageScore.ZeroMem();
#else
		ZeroMemory( cBlackWarVillageScore, sizeof( cBlackWarVillageScore ) );
#endif
		for(int Count = 0; Count < MAX_VILLAGE_NUMBER; ++Count)
		{
			cBlackWarVillageScore[Count].SetVillageUnique( Count + 1 );
		}
	}
	//[추가 : 황진성 2008. 1. 22 => 흑의 군단 진행중 마을 점수 증가.]
	void BlackWarScoreAdd(SI32 _VillageUnique, SI32 _AddScore)
	{
		if( _VillageUnique <= 0 || _VillageUnique >= MAX_VILLAGE_NUMBER)
			return;
	
		for(int Count = 0; Count < MAX_VILLAGE_NUMBER; ++Count)
		{
			if( cBlackWarVillageScore[Count].GetVillageUnique() == _VillageUnique )
			{
				cBlackWarVillageScore[Count].AddVillageScore(_AddScore);
				break;
			}
		}  
		
		sort( &cBlackWarVillageScore[0], &cBlackWarVillageScore[MAX_VILLAGE_NUMBER-1], compare_Village_Score() );
	}
	//[추가 : 황진성 2008. 1. 23 => 흑의 군단 으로 누적된 마을 점수 얻기.]
	SI16 GetBlackWarVillageScore(SI32 _VillageUnique)
	{
		if( _VillageUnique <= 0 || _VillageUnique >= MAX_VILLAGE_NUMBER)
			return 0;

		for(int Count = 0; Count < MAX_VILLAGE_NUMBER; ++Count)
		{
			if( cBlackWarVillageScore[Count].GetVillageUnique() == _VillageUnique )
			{
				return 	cBlackWarVillageScore[Count].GetVillageScore();			
			}
		}

		return 0;
	}
};

#endif
