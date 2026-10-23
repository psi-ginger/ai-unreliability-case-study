//////////////////////////////////////////////////////////////////////////
//
// 이름 : 라쿤 피하기 이벤트 
// 만든사람 : 손성웅
// 내용 : 라쿤닷지 이벤트를 진행할 매니저
//		 예약 리스트를 만들어서 관리하고, 케릭터들을 워프 시키는등의 일을 한다
//	//Switch_10MonthEvent_RacoonDodge
//	
//
//////////////////////////////////////////////////////////////////////////
#ifndef _RACOONDODGEEVENT_H_
#define _RACOONDODGEEVENT_H_

#include "../common/CommonHeader.h"
#include "../NLib/NUpdateTimer.h"	//5초 딜레이위한 헤더

#define MAX_RESERVER_NUM				40			// 최대 예약가능 한 숫자
#define EVENT_START_NUM					5			// 이벤트시작 가능 숫자 
#define MAX_RESERVER_SHOW_NUM			20			// 최대 보여줄 예약자 숫자
#define MAX_EVENTPLACE					2			// 최대 이벤트 지역
#define MAX_EVENT_MONSTER_CREATE_ROC	10			// 최대 이벤트 몬스터 생성 좌표 
#define MAX_MONSTER_ROC					20			// 생성할 몬스터 좌표 총갯수
#define	MAX_STAGE_NUM					2			// 최대 맵갯수
#define	MAX_MONSTER_NUM					10			// 최대 등록가능한 갯수	10가지
#define	ROUND_TIME_LMIT					30000		// 한라운드 시간 30초
#define	MAKEMOSTER_TERM					3000		// 한라운드 시간 3초

#define MAX_AGGRESSIVE					7
#define MAX_MOVESPEED					20

#define  MAX_PLAY_LEVEL					10			// 플레이 하는 총 라운드

#define MAKE_MONSTER_NUM_1	1
#define MAKE_MONSTER_NUM_10	10

#define PLAY_MAP_INDEX					97
#define PLAY_MAP_INDEX_INDODEVENT		97

#define MAX_MAKE_MONSTER_NUM			600		// 최대 생산할 몬스터 총수
#define REWARDPOINT						20		// 보상에 따른 보상 배율 현 20배
#define MAX_ROOF_NUM					100		// 몬스터 만드는 루프를 한계치

#define CAN_PLAY_RACOONDODGEEVENT_LMIT	5	// 라쿤 이벤트 최소 가능 인원
#define CAN_PLAY_RACOONDODGEEVENT_2STAGE	10	// 라쿤 이벤트 2 스테이지 가능 인원

// 만들 스테이지필드
enum
{
	RACOONDODGEEVENT_STAGE_FIELD_1ST = 0 ,	// 첫번째 스테이지
	RACOONDODGEEVENT_STAGE_FILED_2ND,	
};

enum // 이벤트 단계
{
	EVENTSTEP_WAIT = 0 ,				// 이벤트 대기 시간.
	EVENTSTEP_MAKE_PLAYUSERLIST ,		// 신청자들중에 이벤트에 참가할 사람들 리스트를 만든다
	EVENTSTEP_REQUEST_PLAY_EVENT_NOW ,	// 선정된 신청자들에게 참가 여부를 묻는다
	EVENTSTEP_WAIT_ANSWER ,				// 질문의 대답을 기다리고 워프하는 시간 10초간의 대기 시간.
	EVENTSTEP_GAMESTART ,				// 게임을 시작 한다.
	EVENTSTEP_PLAY,						// 이벤트가 현제 플레이 중인 상태 이다. 내부에서 레벨을 변경 하면서 게임이 진행 되고 있다.
	EVENTSTEP_WAITTIME,					// 이벤트가 현제 플레이 중인 상태 이다. 내부에서 레벨을 변경 하면서 게임이 진행 되고 있다.
	EVENTSTEP_GAMEEND ,					// 정해진 시간에 레벨에 도달해서 종료 되던가, 유저가 없으면 종료 시킨다.
};

enum // 이벤트 레벨 
{
	EVENTLEVEL_WAIT = 0 ,
	EVENTLEVEL_LV1,
	EVENTLEVEL_LV2,
	EVENTLEVEL_LV3,
	EVENTLEVEL_LV4,
	EVENTLEVEL_LV5,
	EVENTLEVEL_LV6,
	EVENTLEVEL_LV7,
	EVENTLEVEL_LV8,
	EVENTLEVEL_LV9,
	EVENTLEVEL_LV10,
	EVENTLEVEL_CLEAR
};

enum
{
	RACOONDODGEEVENT_STAGE_NONE	=	0,	// 스테이지를 돌릴수 없다 인원 부족  
	RACOONDODGEEVENT_STAGE_ONE,			// 1개를 돌린다 
	RACOONDODGEEVENT_STAGE_TWO,			// 2개를 돌린다 

};

// 실패매세지 value
enum
{
	EVENT_RESERVATION_FALE_NOT_EMPTY	=	0,	// 빈자리가 없다.
};

class ReserveData
{
public:
	ReserveData()							;
	~ReserveData()							;
	void init()								;

	SI32 GetCharPersonID( )					;
	TCHAR* GetCharName( )					;

	void SetCharPersonID(SI32 siPersonID )	;
	void SetCharName(TCHAR *charname )		;

	void SetChar(SI32 siPersonID,TCHAR *charname )
	{
		SetCharPersonID(siPersonID)	;
		SetCharName(charname )		;
	};
private:
	SI32	PersonID	;
	TCHAR	m_szCharName[MAX_PLAYER_NAME] ;	// 자식 이름
};


class cltRocData	// 좌표를 기억하고 있을 클래스
{
public:
	cltRocData(){		ZeroMemory(this,sizeof(cltRocData))	;	};
	~cltRocData(){};
	SI32 GetRocX(){		return X	;	}	;
	SI32 GetRocY(){		return Y	;	}	;
	SI32 GetRocStage(){	return Stage;	}	;

	void SetRocX( SI32 siX ){	X	=	siX;	}	;
	void SetRocY( SI32 siY ){	Y	=	siY;	}	;
	void SetRocStage( SI32 siStage ){	Stage	=	siStage;	}	;
	void SetRoc( SI32 siX , SI32 siY,SI32 siStage )
	{	
		X = siX ; 
		Y = siY; 
		Stage = siStage;
	};
private:
	SI32 Stage	;
	SI32 X	;
	SI32 Y	;
};

class cltRocDataArray
{
public:
	cltRocDataArray()
	{
		ZeroMemory(this, sizeof(cltRocDataArray));
	};
	~cltRocDataArray(){};
	void SetRoc( SI32 siStageIndex, SI32 siX , SI32 siY)
	{
		for ( SI32 i = 0; i < MAX_MONSTER_ROC; i++) 
		{
			if (pclRocData!=NULL &&  0 == pclRocData[i].GetRocStage() )
			{

				pclRocData[i].SetRoc( siX, siY,siStageIndex)	;
				break;
			}
		}
	};
	//void SetRocX( SI32 siIndex, SI32 siX)
	//{
	//	if(siIndex < MAX_MONSTER_ROC)
	//	{
	//		pclRocData[siIndex].SetRocX( siX)	;
	//	}
	//};
	//void SetRocY( SI32 siIndex, SI32 siY)
	//{
	//	if(siIndex < MAX_MONSTER_ROC)
	//	{
	//		pclRocData[siIndex].SetRocY(siY)	;
	//	}
	//};
	//void SetStage( SI32 siIndex, SI32 siStage)
	//{
	//	if(siIndex < MAX_MONSTER_ROC)
	//	{
	//		pclRocData[siIndex].SetRocStage(siStage)	;
	//	}
	//};
	SI32 GetRocX( SI32 siIndex)
	{
		if(siIndex >= MAX_MONSTER_ROC)	return -1	;
		return pclRocData[siIndex].GetRocX()	;

	};
	SI32 GetRocY( SI32 siIndex)
	{
		if(siIndex >= MAX_MONSTER_ROC)	return -1	;
		return pclRocData[siIndex].GetRocY()	;
	};
	SI32 GetStage( SI32 siIndex)
	{
		if(siIndex >= MAX_MONSTER_ROC)	return -1	;
		return pclRocData[siIndex].GetRocStage()	;
	};
	
private:
	cltRocData	pclRocData[MAX_MONSTER_ROC]	;

};
class cltMonsterSetData
{
public:
	cltMonsterSetData(){		ZeroMemory(this,sizeof(cltMonsterSetData))	;	};
	~cltMonsterSetData(){};

	void SetData(SI32 EventLevel, SI32 AGV_Value, SI32 AGV_AddValue, SI32 MoveSpeed_Value, SI32 MoveSpeed_AddValue )
	{
		siEventLevel	=	EventLevel ;			// 이벤트 레벨
		siAGV_Value		=	AGV_Value ;				// 몬스터 어그래시브 min, Max 차이
		siAGV_AddValue	=	AGV_AddValue ;			// 몬스터 어그래시브 추가 증감값		
		siMoveSpeed_Value		=	MoveSpeed_Value ;		// 몬스터 어그래시브 min, Max 차이
		siMoveSpeed_AddValue	=	MoveSpeed_AddValue ;	// 몬스터 어그래시브 추가 증감값

	};
	SI32 GetAGV_Value(){return  siAGV_Value;}	;
	SI32 GetAGV_AddValue(){return  siAGV_AddValue;}	;
	SI32 GetMoveSpeed_Value(){return  siMoveSpeed_Value;}	;
	SI32 GetMoveSpeed_AddValue(){return  siMoveSpeed_AddValue;}	;
	SI32 GetSiGameLevel(){return  siEventLevel;}	;
private:
	SI32 siEventLevel	;	// 이벤트 레벨

	SI32 siAGV_Value	;	// 몬스터 어그래시브 min, Max 차이
	SI32 siAGV_AddValue	;	// 몬스터 어그래시브 추가 증감값

	SI32 siMoveSpeed_Value	;	// 몬스터 어그래시브 min, Max 차이
	SI32 siMoveSpeed_AddValue	;	// 몬스터 어그래시브 추가 증감값
};
class CRacoonDodgeEventMgr
{
public:
	CRacoonDodgeEventMgr()	;	// 생성자
	~CRacoonDodgeEventMgr() ;	// 소멸자

	void init(void)		;									// 변수 초기화
	void Action(void)	;									// 실제 움직일 액션 함수
	void MakeSelectPersonList(void)	;						// 선정된 사람을을 뽑아내서 리스트를 만든다 이 리스트는 예약리스트와는 다르다.
	bool AddReserveList(SI32 siPersonID )	;				// 리스트에 추가 한다.
	void SendMessage_Request_PlayEvent(void)	;			// 만들어진 선발 리스트에서 너 참가 할거나고 메세지를 날린다.
	
	SI32 FindEmptyPosiotion(void)	;						// 리스트에 빈슬롯을 찾는다.
	void Select_Participated_PersonID( SI32 siPersonID);	// 선택한 사람에 대한 처리.
	SI32 FindPersonID_InList( SI32 siInputPersonID) ;		// 리스트에서 해당 사람을 찾아서 인덱스를 리턴 한다.
	bool SetEventChar( SI32 siPersonID)	;					// 이벤트케릭터라고 셋팅 한다.	
	void WarpGameSpace(SI32 siPersonID )	;					// 해당 케릭터를 워프 한다
	void SendReserveList(SI32 siPersonID )	;					// 예약정보를 클라이언트로 날려준다

	SI32 GetStep(  void )	;									// 매니저의 현제 스탭을 가지고 온다
	SI32 GetLevel(  void )	;									// 진행되고 있는 게임의 현제 레벨을가지고 온다
	void ChangeStep( SI32 siStep )	;							// 매니저의 현제 스탭을 변경 한다.
	void ChangeLevel( SI32 siLevel )	;						// 진행되고 있는 게임의 레벨을 변경 한다.

	void SetMonster( SI32 siMake_Monster_Num, SI32 siStageindex)	;	// 몬스터를 해당 마리수 만큼 뿌려준다
	SI32 CheckPlayStageNum()	;										// 2개의 필드에서 모두 게임이 열리는지 아닌지 확인한다 .

	bool CanReserveList(SI32 siPersonID)	;							// 예약할수 있는지 확인
	void SetAggressiveNMoveSpeed( SI32 siCharID )	;					// 만든 엔피씨에 무브스피드와 어그래시브 설정 
	
	void Count5Second(void)	;								// 5초 카운트 하도록 메세지를 보낸다 아직은 이벤트가 시작안했음
	void Count30Second(void)	;							// 이벤트 시작 하면서 30초 카운트 하도록 메세지를 보낸다
	void RoundSet(void)	;									//  라운드표시 한다.

	bool CanPlayEvent(void)	;								// 이벤트가 진행 될수 있는지 확인 한다 .
	void KillEventMonster(void)	;							// 이벤트 몬스터를 다 죽인다.
	//void RememberEventMonster(SI32 siID)	;				// 만든 이벤트 몬스터 아이디를 기억한다 200개 한정 
	void RewardMoney(void)	;								// 레벨 10 달성 돈을 준다 

	void SetRoundTime(SI32 siSecond)	;					// 라운드 시간 셋팅 초단위로 입력받는다.

	void ClearData()	;									// 라운드가 끝날때 매니저 상태를 초기화한다. 그러나 예약리스트는 안지운다. 
	SI32 CountReservePersonNum();

	//---------------------------------------------------------------------------------------------------------------------------
	// 리소스 로드 하는 것들
	//---------------------------------------------------------------------------------------------------------------------------
	void LoadRocFile();				// 좌표를 저장한파일을 읽어 들인다.
	void LoadMonsterKindFile();		// 몬스터 카인드를 저장한파일을 읽어 들인다.
	void LoadMonsterInfoSetFile();	// 몬스터의 레벨당 셋팅 해줄 어그래시브 값과 무브스피드를가지고 온다.

	//---------------------------------------------------------------------------------------------------------------------------


private:
	SI32 siGameStep			;		// 현제 게임 스탭
	SI32 siGameLevel		;		// 현제 게임 레벨 

	SI32 siInEventGroundNum	;		// 이벤트 그라운드에 있는 사람 5가 넘어가면 2번 그라운드로 날린다.
	bool bLeadData			;

	SI32 siCheckFieldNum	;
	SI32 siMonsterKindNum	;
	bool bCheck_1stMakeMonster	;	// 처음 10마리 몬스터를 만들었느냐


	DWORD dwCount_3Second	;		// 각 단계별로 10초당 한번 몬스터를 찍어 내기 위한 시간 변수
	DWORD dwCount_30Second	;		// 다음 단계로 넘어 가기 위해 30초 시간 변수
	DWORD dwRoundTime		;		// 한라운드 시간 
	DWORD siWaitTime5Second	;

	SI32 siMakeMonster_Num	;		// 만든 몬스터 숫자
	
#ifdef _SAFE_MEMORY
	NSafeTArray<ReserveData, MAX_RESERVER_NUM> pclReserveList;						// 예약리스트
	NSafeTArray<ReserveData, EVENT_START_NUM*MAX_EVENTPLACE> pclSelectPersonList;	// 선발 리스트
	NSafeTArray<cltRocDataArray, MAX_STAGE_NUM > pclRocList;						// 맵좌표 리스트 맵 1개고 그안에서 2개의 스테이지가 있다	
	NSafeTArray<cltMonsterSetData, MAX_PLAY_LEVEL > pclMonsterInfo_Level;			// 각 라운드 레벨별 몬스트 어그래시브 무브스피드 값 

	NSafeTArray<SI32, MAX_MONSTER_NUM > pclMonsterUnique;							// 사용하는 몬스터 유니크 종류
	NSafeTArray<SI32, MAX_MAKE_MONSTER_NUM > MakeMonsterArray;						// 나중에 몬스터를 삭제 하기 위해 몬스터 id를저장해둔다 최대 200개 까지 저장한다.
	
#else
	ReserveData			pclReserveList[MAX_RESERVER_NUM]	;				// 예약리스트
	ReserveData			pclSelectPersonList[EVENT_START_NUM*2]	;			// 선택된 예약리스트
	cltRocDataArray		pclRocList[MAX_STAGE_NUM]	;						// 예약리스트	
	cltMonsterSetData	pclMonsterInfo_Level[MAX_PLAY_LEVEL]	;			// 라운드 레벨당 몬스터 셋할 정보들 aggressive , movespeed
	
	SI32				pclMonsterUnique[MAX_MONSTER_NUM]	;				// 사용하는 몬스터 유니크 종류 
	SI32				MakeMonsterArray[MAX_MAKE_MONSTER_NUM]	;			// 나중에 몬스터를 삭제 하기 위해 몬스터 id를저장해둔다 최대 200개 까지 저장한다.
#endif

};
#endif
