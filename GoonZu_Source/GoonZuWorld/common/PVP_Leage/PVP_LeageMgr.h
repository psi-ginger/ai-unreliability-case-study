//////////////////////////////////////////////////////////////////////////
//
// PVP_League
//
//	이곳은 PVP_League 의 매니저로 사용
//	할일은 시간이 되면 저장된 리스트에서 매칭할 사람들을 뽑아 와서
//	매칭한 사람들을 경기장에 넣고
//	남겨진 사람들은 다음으로 보낼 대기자 리스트로 합류 시킨다.
//
//////////////////////////////////////////////////////////////////////////
#ifndef _PVP_LEAGUEMGR_H_
#define _PVP_LEAGUEMGR_H_

#include "../common/CommonHeader.h"
#include "PVP_LeageObj.h"

// [영훈] QA를 위해 DEBUG시 16명으로 변경함 QA가 다 끝나면 지울 수 있도록 함(보신분이 지워주세요)
#ifdef _DEBUG
#define MAX_RESERVE_PERSON			16	//최대 예약 인원수
#else
#define MAX_RESERVE_PERSON			500	//최대 예약 인원수
#endif

#define MAX_MATCHING_PERSON			5	//매칭된 최대의 인원
#define MAX_PVP_LEAGUE_GRADE_GAB	3	// 매칭시 등급 최대 등급 허용범위
#define MAX_PVP_LEAGUE_GRADE_UP_DOWN_POINT	5	// 매칭시 등급 최대 등급 허용범위
#define MAX_GRADECOUNT				20	// 최대 등급 갯수

enum PVP_LEAGUE_RESERVE_STATUS
{
	PVP_LEAGUE_RESERVE_STATUS_NONE		= 0,
	PVP_LEAGUE_RESERVE_STATUS_POSSIBLE,			// 메칭리스트에 포함할 수 있음
	PVP_LEAGUE_RESERVE_STATUS_READYFORRESPON,	// 다시 리스트에 있을건지 응답을 기다리는 상태
};
enum PVP_LEAGUE_END_RESULT	// 승패 이유
{
	PVP_LEAGUE_END_RESULT_DIE		= 0,	//	죽었다
	PVP_LEAGUE_END_RESULT_WARP,				//	워프했다
	PVP_LEAGUE_END_RESULT_LOGOUT,			//	로그아웃했다.
};
enum PVP_LEAGUE_END_GOAL	// 승패 이유
{
	PVP_LEAGUE_END_RESULT_100WIN		= 0,	//	100연승
	PVP_LEAGUE_END_RESULT_1000WIN,				//	1000연승
	PVP_LEAGUE_END_RESULT_100DRAW,			//	100연무
	PVP_LEAGUE_END_RESULT_1000DRAW,			//	1000연무
};
enum PVP_LEAGUE_END_GRADE_UPDOWN	// 등급 업 다운
{
	PVP_LEAGUE_END_GRADEUP		= 1,	//	등급 업
	PVP_LEAGUE_END_GRADEDOWN			//	등급 다운
};

enum PVP_LEAGUE_PROGRESS_STATUS	//현제 PVP리그 진행 상태. 
{
	PVP_LEAGUE_PROGRESS_READY		= 0,	
	PVP_LEAGUE_PROGRESS_BEFORE_1_M,			//	시작 1분전 
	PVP_LEAGUE_PROGRESS_BEFORE_10_S,		//	시작 10초전
	PVP_LEAGUE_PROGRESS_ONTIME,				//	시작을 위한 5분마다	순간이동
	PVP_LEAGUE_PROGRESS_AFTER_30_S,			//	경기장에 들어온지 30분 지난 순간
	PVP_LEAGUE_PROGRESS_END_BATTLE,			//	경기장으로 워프 들어온지 3분후 경기 종료 시간.
	PVP_LEAGUE_PROGRESS_END_ESCAPEGROUND	//	경기가 끝난지 30초 이후 밖으로 워프 
};

class CPVP_Leage_List
{
private:
#ifdef _SAFE_MEMORY
	NSafeTArray<CPVP_Leage_ListData, MAX_RESERVE_PERSON>			m_clPVP_LeageReserveList	;	//	PVP_League	리스트
#else
	CPVP_Leage_ListData				m_clPVP_LeageReserveList[MAX_RESERVE_PERSON]	;	//	PVP_League	리스트
#endif

public:
	CPVP_Leage_List();
	~CPVP_Leage_List();

	void Init();

	SI32 Add( SI32 siPersonID, SI16 siGrade );	// 예약리스트에 한사람을 추가한다
	void Delete( SI32 siPersonID );				// 예약리스트에서 특정 사람을 뺀다
	SI32 GetPersonIndex( SI32 siPersonID );		// 위치를 찾는다.

	SI32 GetPersonID(SI32 siIndex);				// 배열의 인덱스로 PersonID를 얻어온다.

	SI32 GetCount();							// 전체 등록 개수를 얻어온다.
	void ArrangeList();							// 리스트의 빈공간을 정리한다.
	void DeleteNoPossiblePerson();				// 매칭을 원하지 않는 유저를 삭제한다.
	void SetStatus(SI32 siPersonID, SI32 siSatus);	// 개인의 상태를 변경한다.

	SI32 FindMatchPerson(SI32 siPersonID);		// 나의 적당한 상대를 찾는다.

	void SendToAgainReserveQuestion();			// 리스트에서 매칭이 되지 않은 유저들에게 메시지를 전송한다.
};

class CPVP_Leage_MatchingList
{
private:
#ifdef _SAFE_MEMORY
	NSafeTArray<CPVP_Leage_MatchingListData, MAX_MATCHING_PERSON>	m_clPVP_LeageMatchingList	;	//	PVP_League	매칭된 리스트
#else
	CPVP_Leage_MatchingListData		m_clPVP_LeageMatchingList[MAX_MATCHING_PERSON]	;	//	PVP_League	매칭된 리스트
#endif

public:
	CPVP_Leage_MatchingList();
	~CPVP_Leage_MatchingList();

	void Init();

	bool Add( SI32 siSourcePersonID, SI32 siDestPersonID, SI32 siTotalScore );
	bool Delete( SI32 siSourcePersonID, SI32 siDestPersonID );

	SI32 GetIndex( SI32 siSourcePersonID, SI32 siDestPersonID );

	SI32 GetBattlePersonIDHome( SI32 siIndex );
	SI32 GetBattlePersonIDAway( SI32 siIndex );

	SI32 FindPersonID( SI32 siPersonID );
	SI32 FindEnemyPersonID( SI32 siPersonID );

	SI32 GetCount();
	bool IsFull();

	void SortByTotalScore();

	void NoticeSpecialNews();
	void NoticeStartPVP();

	void PVP_League_MatchingLog( void );
};

class CPVP_Leage_EndGameList
{
private:
#ifdef _SAFE_MEMORY
	NSafeTArray<CPVP_Leage_EndGameListData, MAX_MATCHING_PERSON>	m_clPVP_Leage_EndGameList	;	// 종료한 게임들을 저장한다.
#else
	CPVP_Leage_EndGameListData		m_clPVP_Leage_EndGameList[MAX_MATCHING_PERSON]	;	// 종료한 게임들을 저장한다.
#endif

public:
	CPVP_Leage_EndGameList();
	~CPVP_Leage_EndGameList();

	void Init();

	void Add( SI32 WinnerID, SI32 LoserID );
	SI32 FindPersonID( SI32 siPersonID );
	void PVP_LeagueEndWarp();
};


struct cltMapInfo
{
	SI16			m_slot	;
	SI16			m_index	;
	SI16			m_HomeX	;
	SI16			m_HomeY	;
	SI16			m_AwayX	;
	SI16			m_AwayY	;
};

struct cltGradeInfo
{
	SI16			m_Grade	;
	SI16			m_GradeValue	;
	SI16			m_UpWin	;	// 다음 등급에 올라갈수 있는 연속 승수
	SI16			m_DownLose	;	// 하위 등급으로 떨어질수 있는 연속 패수
};

class CPVP_LeagueMgr
{
private:
	CPVP_Leage_List					m_clPVP_LeageReserveList;		   //	PVP_League	리스트
	CPVP_Leage_MatchingList			m_clPVP_LeageMatchingList;		   //	PVP_League	매칭된 리스트
	CPVP_Leage_EndGameList			m_clPVP_Leage_EndGameList;		   // 종료한 게임들을 저장한다.

#ifdef _SAFE_MEMORY
	NSafeTArray<cltMapInfo, MAX_MATCHING_PERSON>					pclmapinfo;		;	//	PVP_League 맵 정보 
	NSafeTArray<cltGradeInfo, MAX_GRADECOUNT>					pclGradeInfo;				;	//	PVP_League 등급 정보 
#else
	cltMapInfo						pclmapinfo[MAX_MATCHING_PERSON]					;	//	PVP_League 맵 정보 
	cltGradeInfo						pclGradeInfo[MAX_GRADECOUNT]					;	//	PVP_League 등급 정보
#endif

	SI32							m_siBottomPosition;					// 현재 리스트의 어디까지 저장되어 있는가?

	SYSTEMTIME						stEndTime						;	//종료 시간
	SI32							m_Battle_Status					;	// 현제 PVP리그 상태변수.
	SI32							m_siLastGradeIndex				;
	bool							bEndNotice						;
public:
	CPVP_LeagueMgr();
	~CPVP_LeagueMgr();

	void	Init( void );										// 전체 초기화

	SI32 AddReserveList( SI32 siPersonID, SI16 siGrade );	// 예약리스트에 한사람을 추가한다
	void SubReserveList( SI32 siPersonID );					// 예약리스트에서 특정 사람을 뺀다
	void AgainReserveList( SI32 siPersonID, bool bAnswer );	// 다시 예약리스트에 들어갈것인지 설정한다
	void AddEndGameList( SI32 WinnerID, SI32 LoserID );		// 종료게임 리스트에 추가 한다.

	void MakeMatchingList( void );							// 매칭 리스트를 만든다

	SI32 GetRerserveListCount();

	void Action()	;

	void sendResultDB( SI32 WinnerID, SI32 LoserID )	;
	void sendResultDrawDB( SI32 siFirstPersonID, SI32 siSecondPersonID )	;		
	void End_PVP_Battle_End( SI32 siWinnerPersonID, SI32 siLoserPersonID )	;
	void End_PVP_Battle_Draw( SI32 siFirstPerson, SI32 siSecondPerson )	;
	void Send_PVP_Battle_Result( SI32 siFirstPerson, SI32 siSecondPerson, SI32 siFlag )	;
	//-- 대결
	void PVP_LeagueReady();
	void PVP_LeagueStart();
	void PVP_League_Warp_Stage( SI32 siHomePersonID, SI32 siAwayPersonID , SI32 siMapIndex, SI32 siSlot );
	//---기록에 대한 처리
	void SetAfterPVP_League_Record_Win( SI32 siID )	;	//charID
	void SetAfterPVP_League_Record_Lose( SI32 siID )	;	//charID
	void SetAfterPVP_League_Record_Draw( SI32 siID )	;// charID
	//---보상 
	void AfterPVP_League_Record_Win_Prize( SI32 siID , SI32 siStrightWinCount	)	;
	void AfterPVP_League_Record_Lose_Prize( SI32 siID , SI32 siStrightWinCount	)	;
	void AfterPVP_League_Record_Draw_Prize( SI32 siID , SI32 siStrightWinCount	)	;
	// 등급 상승 하강
	void PVP_League_LevelUp( SI32 siID ,SI32 siStrightWinCountGrade )	;
	void PVP_League_LevelDown( SI32 siID , SI32 siStrightLoseCountGrade )	;

	void PVP_LeagueMapInfo()	;
	void PVP_LeagueGradeInfo()	;

	void Set_PVP_League_Status(	SI32 siBattle_Status	)	{ m_Battle_Status = siBattle_Status ;};	// PVP 리그 상태를 변경한다.
	SI32 Get_PVP_League_Status(){	return m_Battle_Status	;	}	;	// PVP 리그 상태를 가지고 온다.
	void Init_PVP_League_Status(){ m_Battle_Status = PVP_LEAGUE_PROGRESS_READY	; }	;	// PVP 리그 상태를 초기화 한다

	SYSTEMTIME GetEndTiem(){	return stEndTime	;	}	;
	////==위프
	void PVP_LeagueEndWarp()	;
	void PVP_Battle_HaveItemMessage( SI32 siPersonID, SI32 siItemUnique )	;
	void PVP_Battle_Step_Draw( void )		;
	void PVP_Battle_Step_Game( void );
	void PVP_League_Before1m();
	void PVP_League_GiveItem( SI32 siID, SI32 siItemUnique,bool bSpecial );
	void PVP_League_MatchingLog()	;	// 매칭리스트의 로그를 적는다.
	void LogWinLose( SI32 siWinnerPeronID, SI32 siLoserPersonID )	;		// 승패로그를 적는다
	void LogDraw( SI32 siDrawPersonId1 , SI32 siDrawPersonId )	;			// 비긴 로그를 적는다.
	void LogGoal( SI32 siPersonID , SI32 siGoalFlag )	;					// 목표달성 로그를 적는다.
	void LogReserve( SI32 siPersonID )					;					// 예약리스트에넣을떄 로그 적는다.
	void LogGradeUpDown( SI32 siPersonID,SI32 siBeforeGrade, SI32 siAfterGrade, SI32 UpDownFlag )	;
	//== 매칭 리스트에 있는지 확인 하고 
	bool InMatchingList( SI32 siPersonID)	;			// 매칭리스트에 해당 퍼슨이 있는지 확인
	bool InEndGameList( SI32 siPersonID)	;			// EndgameList에 있는지 확인 한다.
	// 상대방을 찾아 낸다
	SI32 GetEnemyPersonID( SI32 siPersonID)	;			// 매칭리스트에서 나의 적이 누구인지 확인
	// 해당 케릭터가 PVP리그 맵에 있는지 확인.
	bool Is_MatchingListPeron_TruePosition( SI32 siPersonID )	; // 해당 케릭터가 매칭 리스트에 존재할때 정상 포지션에 있는 지를 확인.
	SI32 GetMyGradeFromGradeValue(SI32 siGradeValue )	;
	SI32 ChangeGrade( SI32 GradeNow, SI32 WinLoseCount, SI32 siUpDown );

private:
	// 예약리스트에서 매칭되지 않는 사람들에게 다시 예약을 할것인지 물어보는 패킷을 보내주는 함수
	void	SendToAgainReserveQuestion( void );

	// 특설경기장에 경기하는 사람을 자막광고로 알려준다
	void	NoticeSpecialNews( void );	

	// 잠시 뒤에 결기가 시작된다고 알림.
	void	NoticeStartPVP( void );

	// 맨처음 무한대전 경기가 몇시에 시작된다고 알림
	void	NoticeStartTime( void );
	// 맨처음 무한대전 경기가 끝나는 시간.
	void	NoticeEndTime( void );

public:
	// 매칭 특설링 찾기시에 사용될 점수를 얻어내는 함수
	static SI32	GetTotalScore( SI32 siSourcePersonID, SI32 siDestPersonID );
	// 매칭시 내 등급과 유효한 범위내에 있는 등급인지 검사한다
	static bool	CheckTheGrade( SI16 siSourceGrade, SI16 siDestGrade, SI32* psiGradeGab );
	// 나와 매칭이 될 수 있는 상대인지 검사한다
	static bool	IsValidPerson( SI32 siSourcePersonID, SI32 siDestPersonID );
};
#endif
