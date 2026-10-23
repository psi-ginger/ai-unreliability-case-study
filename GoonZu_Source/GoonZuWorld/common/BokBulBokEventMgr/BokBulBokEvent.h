//===================================================================================================
// 제목: 복불복 미니게임
// 스위치: Switch_BokBulBok
// 내용: 복불복 이벤트를 관리할 메니져. (NPC생성, 예약리스트관리, 캐릭터워프, 게임스테이지관리..)
// 작성일: 2009.08.06
// 작성자: 오지연
//===================================================================================================

#ifndef _BOKBULBOK_EVENT_H_
#define _BOKBULBOK_EVENT_H_



#include "../common/CommonHeader.h"
#include "../NLib/NUpdateTimer.h"	//5초 딜레이위한 헤더

#include "../../CommonLogic/Msg/MsgType-BokBulBok.h"

#include "BBBWaitListManager.h"
#include "BBBEventMapManager.h"
//#include "../common/BokBulBokEventMgr/BBBStage.h"



// 복불복 게임은 총 10개의 스테이지로 구성되어있다.
enum BokBulBok_Game_Stage
{
	BOKBULBOK_0_WAIT = 0,
	BOKBULBOK_1_STAGE,
	BOKBULBOK_2_STAGE,
	BOKBULBOK_3_STAGE,
	BOKBULBOK_4_STAGE,
	BOKBULBOK_5_STAGE,
	BOKBULBOK_6_STAGE,
	BOKBULBOK_7_STAGE,
	BOKBULBOK_8_STAGE,
	BOKBULBOK_9_STAGE,
	BOKBULBOK_10_STAGE,	
};

// 게임 진행중 상태 체크
enum BokBulBok_Game_Status
{
	BOKBULBOK_STATUS_WAIT = 0,			// 이벤트 대기 시간
	BOKBULBOK_STATUS_PLAYER_LIST,		// 게임참여 신청한 유저 리스트
	BOKBULBOK_STATUS_SEND_MSG,			// 리스트에 등록된 신청자에게 (순차적으로) 참가 여부를 묻는다.
	BOKBULBOK_STATUS_WAITING_TIME_MSG,	// 참가여부 메세지 전달 후 10초간 기다려줌(응답 없을 경우 자동 취소됨.)
	BOKBULBOK_STATUS_GAME_STATR,		// 게임 시작
	BOKBULBOK_STATUS_GAME_PLAYING,		// 게임 플레이 상태
	BOKBULBOK_STATUS_GAME_OVER,			// 게임 할 수 없는 상태	(스테이지를 클리어 하던지, 접속이 끊기던지 등등..)
};

enum NPC_Position
{
	BOKBULBOK_X = 22,
	BOKBULBOK_Y = 58,
};

enum BokBulBok_Failure_Msg
{
	BOKBULBOK_RESERVATION_FAIL = 0,
};



class CServer_BBBMgr
{
public:
	CServer_BBBMgr(void);
	~CServer_BBBMgr(void);

	//----------------------------------------------------
	// Server
	//----------------------------------------------------
private:
	SI32 m_siBokBulBok_NPC_ID;		// 복불복 NPC의 캐릭터 ID
	//SI32 m_siTotalReservePlayer;	// 예약한 총 인원수
	SI32 m_siTotalBalance;			// 지금까지 지불된 총 입장 금액(모든유저)
	SI32 m_siGameFee;				// 스테이지별 지불해야할 참가금액
	SI32 m_siMyTotalGameFee;		// 게임에 참여중인 유저가 지불하고 있는 총 금액.
	SI32 m_siRewardPrice;			// 보상으로 지급받을 금액.

public:
	GMONEY m_siTotalMoney;			// 현재 누적금 총액 (서버가 가져가는 30% 제외 안한 액수)
	GMONEY m_siTotalRewardPrice;	// 현재 보상금 총액 (유저에게 실제로 돌아갈 금액)

public:
	CBBBWaitListManager m_BBBWaitListMgr;				// 대기자 리스트 관리자
	CBBBEventMapManager m_BBBEventMapMgr;				// 이벤트 맵 관리자

public:
	// 복불복 공지
	DWORD dwNotice_Msg;			// 출력 간격 (기본 60분)
	DWORD dwNextmNoticetime;	// 다음 공지 출력될 시간

public: 
	void init(void);									// 초기화
	void Action();
	//bool CreateBokBulBokNPC();						// 복불복 NPC 생성.

	void AddBBBList(SI32 siPersonID);					// 요청이 들어온 유저의 아이디를 복불복 매니저에 전달
	void StageFail(SI32 siPersonID);					// 스테이지 클리어 실패
	void StageClear(SI32 siPersonID);					// 스테이지 클리어 성공
	void LeaveMap(SI32 siPersonID);						// 맵에 들어가서 겜중이던 유저를 빼낸다.

	bool Insert_List(SI32 siPersonID);									// 대기리스트 등록
	void Remove_List(SI32 siPersonID);									// 대기 취소
	void SendWaitListToClient(SI32 siPersonID, bool bResult);			// 예약한 유저 리스트를 클라로 보내준다.

	bool AddReserveList(SI32 siPersonID);				// 게임 예약한 유저를 대기자 리스트에 추가한다.

	void DeleteplayerfromWaitingList(SI32 siPersonID);	// 대기자 리스트에서 해당 플레이어를 삭제 시킨다.

	//void Warp_GameStart(SI32 siPerson);					// 게임할 맵으로 이동 시킨다.

	void ActiveTemporaryReserve(SI32 siPersonID);
	void CancelTemporaryReserve(SI32 siPersonID);

	GMONEY GetRewardMoney();
	void SetEntranceFee(GMONEY siEntranceFee);

	void SetPlayTime(SI32 siPlayTime);		// 초단위 플레이 시간(밀리초 단위 아님)
	void ShowMonsterType(SI32 siID, SI32 siShowMonsterType);
	void Set20mNoticeTime(SI32 siNoticeTime);

	void Check_AlreadyReserved(SI32 siID);

	// 내부에서 쓰일 멤버와 멤버 함수들
protected:
	SI16 _siAvailableMapNumber;

protected:
	bool _Check_Avail_Map();
	bool _Check_Avail_List();

	void _Check_StartBBB(SI32 siPersonID);
	void _SendMsg_WaitList(SI32 siPersonID);										// 대기리스트에 등록
	void _SendMsg_EnterFail(SI32 siPersonID);

	void _Notice_20_Minute_TotalRewardMoney();
};

#endif