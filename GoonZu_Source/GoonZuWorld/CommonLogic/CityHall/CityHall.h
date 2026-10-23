//---------------------------------
// 2003/6/9 김태곤
//---------------------------------

#ifndef _CITYHALL_H
#define _CITYHALL_H

#include <Stdio.h>
#include "..\StructureClass\StructureClass.h"
#include "..\..\Common\CommonHeader.h"

#include "GeneralMeeting.h"

#define MAX_CITYHALL_TAB_NUMBER			10		// 탭 숫자 
#define MAX_CITYHALL_CHIEF_TAB_NUMBER	5		// 대행수 메뉴 탭 숫자
#define MAX_RESIDENT_APPLYER			100		// 한 마을 당 최대 주민등록 신청인 숫자
#define MAX_RESIDENT_NUMBER				100

#define MAX_VILLAGEMARK_NUMBER			107

//#if defined(_CITYHALL_BANKRUPTCY)
#define MAX_BANKRUPT_STOCKHOLDER_NUMBER_TO_BE_NOTIFIED	500		// 파산 통지를 받을 최대 주주수, 내림차순 1~최대500등까지
//#define BANKRUPT_STOCK_PERCENT	3	// 파산율 : 3%
//#endif
class cltTime;

class cltCandidate;
class cltCityHallCandidateManager;

class cltTaxPaymentListDlg;
class cltDividendListDlg;

class cltRankCandidate{
public:
	SI32 siRankType;			// 신분코드 
	TCHAR szStructureName[64];	// 시설 이름. 
	TCHAR szRankName[64];		// 신분이름  
	SI32 siNeedLevel;			// 되기 위해 필요한 레벨 
	TCHAR szExplain[256];		// 설명. 
};

// 마을 레벨 상세 정보
class cltVillageLevelDetailInfo
{
public:
	cltVillageLevelDetailInfo() {
		ZeroMemory( this, sizeof( cltVillageLevelDetailInfo ) );
	}

	~cltVillageLevelDetailInfo() {};

	void		Set( cltVillageLevelDetailInfo *pInfo ) 
	{
		siSlaveVillage = pInfo->siSlaveVillage;
		siResident = pInfo->siResident;
		siVisitorNum = pInfo->siVisitorNum;
		siVillageTotalScore = pInfo->siVillageTotalScore;

	}


	SI32		GetVillageLevel()
	{
		return siSlaveVillage + siResident + siVisitorNum;
	}

	SI16		siSlaveVillage;						// 하위 마을 개수에 따른 점수 
	SI16		siResident;							// 주민 숫자에 따른 점수 
	SI16		siVisitorNum;						// 마을 방문자 숫자에 따른 점수
	SI32		siVillageTotalScore;				// 마을 총 점수

};

#define CITYHALL_EVENT_HUNT					1
#define CITYHALL_EVENT_MOFUMOFU				2
#define CITYHALL_EVENT_MONSTER_ATTACK		3	// [진성] 마을 이벤트 - 몬스터 습격. 2008-3-26

// 마을 이벤트 
class cltCityhallEventInfo
{
public:
	cltCityhallEventInfo() { ZeroMemory( this, sizeof( cltCityhallEventInfo ) ); };
	~cltCityhallEventInfo() {};


	int			eventid;
	GMONEY		siEventMoney;
	TCHAR		szEventName[ 32 ];
	TCHAR		szEventExplain[ 512 ];
};


class cltCityhallEventManager
{
public:
	cltCityhallEventManager() { ZeroMemory( this, sizeof( cltCityhallEventManager ) ); };
	~cltCityhallEventManager() {};


#ifdef _SAFE_MEMORY
	NSafeTArray<cltCityhallEventInfo, 20>		clEventInfo;
#else
	cltCityhallEventInfo	clEventInfo[ 20 ];
#endif
	SI32					siEventNum;


	void AddEvent( SI32 eventid, GMONEY money, TCHAR *szEventName, TCHAR *szEventExplain ) {
		clEventInfo[ siEventNum ].eventid = eventid;
		clEventInfo[ siEventNum ].siEventMoney = money;
		
		memcpy( clEventInfo[ siEventNum ].szEventName, szEventName, 32 );
		clEventInfo[ siEventNum ].szEventName[ 31 ] = NULL;

		memcpy( clEventInfo[ siEventNum ].szEventExplain, szEventExplain, 512 );
		clEventInfo[ siEventNum ].szEventExplain[ 511 ] = NULL;

		siEventNum++;
	}

	cltCityhallEventInfo *GetEventInfoFromEventID( SI32 id )
	{
		for( int i = 0; i < siEventNum; ++i ) {
			if( clEventInfo[ i ].eventid == id ) {
				return &clEventInfo[ i ];
			}
		}

		return NULL;
	}


};


// 설명
/*
class cltCityHallMoneyExplain
{
public:
	HWND m_hDlg;	
	BOOL m_DlgRectInitSwitch;
	RECT m_DlgRect;
	HBITMAP m_hHitBitmap;

	SI32 m_siExplainType;

	cltCityHallMoneyExplain();
	~cltCityHallMoneyExplain();

	bool CreateDlg(HINSTANCE hinst, HWND hwnd);
	static BOOL CALLBACK CityHallMoneyExplainStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK CityHallMoneyExplainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	SI32 GetCityHallMoneyExplainType(TCHAR* pExplainType);
	void ShowCityHallMoneyExplainHit(HWND hDlg, SI32 siExplainType);

};
*/

// 주민 클래스 
class cltResident
{
public:
	cltSimplePerson		clPerson;			// 신청자
	SI32				siLevel;			// 레벨 
	SI32				siGrade;			// 주민점수
	cltDate				clDate;				// 주민등록일(게임시간)
	cltDate				clLastLoginDate;	// 마지막 접속일(게임시간)

	cltResident()
	{
		Init();
	}

	void Init()
	{
		clPerson.Init();
		siGrade = 0;
	}

	void Set( cltSimplePerson *pclPerson, SI32 grade, cltDate *pclDate, cltDate *pclLastLoginDate )
	{
		clPerson.Set( pclPerson );
		siGrade = grade;
		clDate.Set( pclDate );
		clLastLoginDate.Set( pclLastLoginDate );
	}

	void Set( cltResident *pclResident )
	{
		memcpy( this, pclResident, sizeof( cltResident ) );
	}
};


// 주민 등록 신청자 클래스 
class cltResidentApplyer
{
public:
	cltSimplePerson		clApplyer;			// 신청자
	SI32				siLevel;			// 레벨
	cltDate				clDate;				// 신청일 

	cltResidentApplyer()
	{
		Init();
	};

	void Init()
	{
		clApplyer.Init();
		siLevel = 0;
	}

	void Set( cltSimplePerson *pclPerson, SI32 level, cltDate *pclDate ) {
		clApplyer.Set( pclPerson );
		siLevel = level;
		clDate.Set( pclDate );
	}

	void Set( cltResidentApplyer *pclResidentApplyer )
	{
		memcpy( this, pclResidentApplyer, sizeof( cltResidentApplyer ) );
	}

};


//-------------------------------------
// 행수 정보 
//-------------------------------------
class cltCheifInfo
{
public:
	cltCheifInfo() {
		ranktype = 0;
		szID[ 0 ] = NULL;
	}

	int			ranktype;
	TCHAR		szID[ MAX_PLAYER_NAME ];
};

class cltChatMember
{
public:
	int		charid;
	int		personid;

	cltChatMember()
	{
		charid = 0;
		personid = 0;
	}
};

class cltChatMemberManager
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltChatMember, 200>		clChatMember;
#else
	cltChatMember	clChatMember[ 200 ];
#endif

	void AddChatMember( int charid, int personid );
	void DelChatMember( int personid );
	void SendMsg( BYTE* msg );
};


//-------------------------------------------------
// 개별적인 상단.  
//-------------------------------------------------
class cltCityHall: public cltStructureBasic{
private:
	cltCityHallCandidateManager* pclCandidateManager;

public:
	cltCityHall(SI32 sivillageunique);
	~cltCityHall();

#ifdef _SAFE_MEMORY
	NSafeTArray<cltResidentApplyer, MAX_RESIDENT_APPLYER>	clResidentApplyer;
	NSafeTArray<cltResident, MAX_RESIDENT_NUMBER>			clResident;
#else
	cltResidentApplyer	clResidentApplyer[ MAX_RESIDENT_APPLYER ];			// 주민 등록 신청자 
	cltResident			clResident[ MAX_RESIDENT_NUMBER ];
#endif

	cltVillageLevelDetailInfo	clLevelInfo;
	cltGeneralMeetingInfo		clGeneralMeetingInfo;

	cltChatMemberManager		clChatMemberManager;

	BOOL	bPrepareBankruptcy;

	void SetResidentApplyer( SI32 index, cltResidentApplyer *pclResidentApplyer )
	{
		clResidentApplyer[ index ].Set( pclResidentApplyer );
	}

	cltResidentApplyer* GetResidentApplyer( SI32 index )
	{
		return &clResidentApplyer[ index ];
	}


	void SetResident( SI32 index, cltResident *pclResident )
	{
		clResident[ index ].Set( pclResident );
	}


	// 모든 주민등록신청자를 삭제한다.
	void InitResidentApplyer()
	{
		for( SI32 i = 0; i < MAX_RESIDENT_APPLYER; i++ )
		{
			clResidentApplyer[ i ].Init();
		}

	}

	// 모든 주민을 삭제한다 
	void InitResident()
	{
		for( SI32 i = 0; i < MAX_RESIDENT_NUMBER; i++ )
		{
			clResident[ i ].Init();
		}
	}

	void SetSymbol( SI16 symbolKind, SI32 symbolLife, SI32 symbolMana, SI16 symbolCapa );	// 심볼(수호신) 정보 설정 
	void SetSymbolLife( SI32 symbolLife );													// 심볼(수호신) 체력 설정
	void SetSymbolMana( SI32 symbolMana );													// 심볼(수호신) Mana 설정 
	void SetSymbolCapa( SI16 symbolCapa );													// 심볼(수호신) 성능 설정

	void SetCastle( SI16 castleKind, SI32 castleLife, SI32 castleMana, SI16 castleCapa );	// 캐슬 정보 설정 
	void SetCastleLife( SI32 castleLife );													// 캐슬 체력 설정
	void SetCastleMana( SI32 castleMana );													// 캐슬 Mana 설정 
	void SetCastleCapa( SI16 castleCapa );													// 캐슬 성능 설정	

	BOOL AddApplyer( cltResidentApplyer *pclApplyer );
	BOOL DeleteApplyer( SI32 personid );

	// Chief 후보를 등록한다. 
	BOOL SetCandidate(SI32 index, cltSimpleRank* pclrank, cltSimplePerson* pclcandidate);
	// 후보 등록을 취소한다. 
	BOOL DeleteCandidate(SI32 personid);
	// 특정 신분의 후보 등록을 모두 취소한다. 
	BOOL DeleteCandidate(cltSimpleRank* pclrank);

	cltSimplePerson* GetCandidateInfoFromPersonID(SI32 ranktype, SI32 personid, SI32* pindex);
	// 입후보자의 숫자를 구한다. 
	SI32 GetCandidateNum(SI32 ranktype);

	cltSimplePerson* GetCandidateInfoFromRef(SI32 ranktype, SI32 ref);

};

//--------------------------------------------------
// 상단관리자. 
//--------------------------------------------------
class cltCityHallManager : public cltStructureMngBasic{
public:
	cltCityHallCandidateManager* pclClientCandidateManager;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltResidentApplyer, MAX_RESIDENT_APPLYER>		clResidentApplyer;
	NSafeTArray<cltResident, MAX_RESIDENT_NUMBER>				clResident;
#else
	cltResidentApplyer	clResidentApplyer[ MAX_RESIDENT_APPLYER ];			// 주민 등록 신청자 
	cltResident			clResident[ MAX_RESIDENT_NUMBER ];
#endif
	
	cltResident			clClientResidentInfo;

	cltVillageLevelDetailInfo	clClientLevelInfo;
	cltGeneralMeetingInfo		clGeneralMeetingInfo;

	cltCityhallEventManager		clEventManager;
	SI32						siSelectedIndex;

	BOOL UpdateInOutMoneySwitch;

	BOOL UpdateWarTreeSwitch;

	BOOL UpdateResidentApplyerSwitch;
	BOOL UpdateResidentListSwitch;
	
	BOOL UpdateResidentInfoSwitch;
	BOOL bResidentInfoValid;

	BOOL UpdateInvestSwitch;
	BOOL UpdateMayorCandidateSwitch;
	BOOL UpdateChiefCandidateSwitch;

	BOOL UpdateLevelInfoSwitch;

	cltCityHallManager(cltCharManager* pclCM, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltCityHallManager();

	BOOL bChiefInfo;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltCheifInfo, MAX_RANKTYPE_NUMBER>	clChiefInfo;
#else
	cltCheifInfo clChiefInfo[ MAX_RANKTYPE_NUMBER ];
#endif

	virtual void OnTimer();

	// 최초 환영 인사말표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 charunique);

	// 상단을  신설한다.
	BOOL CreateCityHall(SI32 siunique);

	// 
	void Update();

	void UpdateResidentApplyerList( SI32 personid );

	// 시설 정보 리스트를 초기화한다
	void InitStructList( HWND hDlg );

	// 신분 정보 리스트를 초기화한다 
	void InitRankList( HWND hDlg );

	// 투자자 리스트를 초기화한다 
	void InitInvestList( HWND hDlg );

	// 투자자 리스트를 보여준다. 
	BOOL ShowInvestList(HWND hDlg);
	
	// 대행수을 보여준다. 
	BOOL ShowMayor(HWND hDlg);

	// Chief	후보를 등록한다. 
	BOOL SetCandidate(SI32 index, cltSimpleRank* pclrank, cltSimplePerson* pclcandidate);

	// 후보 등록을 취소한다. (모든 직책에서..) 
	BOOL DeleteCandidate(SI32 villageunique, SI32 personid);

	// 특정 신분의 모든 후보 등록을 취소한다.
	BOOL DeleteCandidate(cltSimpleRank* pclrank);

	void Action(cltTime* pcltime);

	// '장'의 타입을 설정한다. 
	void SetCandidateCombo(HWND hDlg);

	// 선택된 문자열을 근거로 적절한 도움말을 보여준다. 
	void SetRankCandidateHelp(const TCHAR* szSelText);

	// 선택된 RankType이 되기 위해 필요한 레벨을 구한다.
	SI32 GetNeedLevelFromRankType(SI32 ranktype);

	// 선택된 문자열(시설명)을 근거로 적절한 신분코드를 얻어온다.
	SI32 GetRankTypeFromStructureName(const TCHAR* szSelText);

	// 선택된 문자열(신분명)을 근거로 적절한 신분코드를 얻어온다.
	SI32 GetRankTypeFromRankName( const TCHAR* szSelText );

	BOOL IsGeneralMeeting( SI32 villageunique );

	BOOL IsPrepareBankruptcy( SI32 villageunique );

//#if defined(_CITYHALL_BANKRUPTCY)
	// 상단의 파산 상태를 얻음
	UI08 GetCityHallState( SI32 villageunique );
	// 파산한 후 마을의 모든 요소가 초기화 되었는가
    BOOL IsInitialized( SI32 villageunique );

	// 상단 상태 정보를 기록한다. 
	void WriteAllCityHallState();

	// 관청 파산까지 혹은 안정기가 끝나는 DeadLine 정보를 얻는다.
	cltDate GetCityHallBankruptcyDetailInfo( cltCityhallStrInfo* pstrinfo );
//#endif


	//----------------------------------------
	// CityHall-ChiefDlgProc.cpp
	//----------------------------------------
	
	// 시설들을 콤보에 추가한다.
	BOOL ShowStructure(HWND hDlg);

	void ShowVillageMark( HWND hDlg );
	void ShowMyVillMark( HWND hDlg );

	void ShowChiefDate( HWND hDlg );
	BOOL bRecvChiefInfo;

	SI32	m_siVillageMarkIndex;
	SI32	m_siMyVillMarkIndex;
	// 마을 지도 변경 관련
	SI32	m_siVillageMapIndex;

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, 100>		m_siVillageMapUnique;
#else
	SI32	m_siVillageMapUnique[ 100 ];
#endif
	SI32	m_siVillageMapNumber;

	void	ShowVillageMap( HWND hDlg );


	void	InitVoteList( HWND hDlg );
	
	void	ShowMeetingInfo( HWND hDlg );
	void	ShowVoterList( HWND hDlg );

	void	ShowLeftTime( HWND hDlg );
	
	BOOL	bUpdateGeneralMeetingInfo;
	BOOL	bRequestGeneralMeetingInfo;
};

#endif
