//===================================================================================================
// 제목	 : 복불복 미니게임
// 스위치: Switch_BokBulBok
// 내용	 : 복불복 맵을 관리할 메니져.
// 작성일: 2009.08.10
// 작성자: 오지연
//===================================================================================================

#ifndef _BBBEVENT_MAP_H_
#define _BBBEVENT_MAP_H_

#include "../../Common/CommonHeader.h"



#define PLAY_MAP_INDEX			97		// 축제
#define MAX_MONSTER_COUNT		10

#define MONSTER_TYPE_NORMAL		0		// 일반/선물 몹
#define MONSTER_TYPE_ONEKILL	1		// 원킬몹
#define MONSTER_TYPE_KEY		2		// 키몹
#define MONSTER_TYPE_ITEM		3		// 선물몹

#define MAX_ITEMKIND_NUM		4
#define MAX_EVENTMAP_NUM		4
#define MAX_GENMOBKIND_NUM		4
#define MAX_BBBSTAGE_NUM		10


//KIND_KOINOBORYENG
//KIND_KINGPIG
//KIND_RABBITBIG
//KIND_MUSHROOMBOSS


struct _StageInfo
{
	SI16 siOneKillMobCount;
	SI16 siKeyMobCount;
	SI16 siItemMobCount;
};

struct _MapPosition
{
	SI16 posX;
	SI16 posY;
};



class CBBBEventMap
{
public:
	CBBBEventMap(SI16 siMapNumber);
	~CBBBEventMap(void);

	// 스태틱 정보
protected:
	_StageInfo m_aStageInfo[MAX_BBBSTAGE_NUM];			// 스테이지 정보 (스태틱으로 객채에 1개만 생성)
	_MapPosition m_aMapPosition[MAX_EVENTMAP_NUM];		// 맵의 중심 위치
	SI32 m_aMonsterKind[MAX_GENMOBKIND_NUM];			// 몬스터 유니크
	SI32 m_aItemUnique[MAX_ITEMKIND_NUM];				// 드랍 가능 아이템 유니크

public:
	SI32 m_siStageStartDelay;				// 스테이지 시작때 카운트 시간 (카운트 시간 + 스타트1초)
	SI32 m_siStageLoseDelay;				// 스테이지 실패 표시 시간
	SI32 m_siStageWinDelay;					// 스테이지 윈 표시 시간
	SI32 m_siStagePlayDelay;				// 스테이지 플레이 시간
	SI32 m_siTotalStageWinDelay;		    // 최종 스테이지 완료후 윈 표시 시간
	SI32 m_siItemDropRate;					// 선물몹이 선물 드롭할 확률

protected:
	// 필드를 이용중인 사람의 정보
	SI16 m_siMapNumber;
	bool m_bOccupied;						// 이 필드가 사용중인지 체크
	SI32 m_siUserID;						// 유저 아이디
	TCHAR m_szUserName[MAX_PLAYER_NAME];	// 유저 이름
	DWORD m_dwStageStartTime;				// 매 스테이지가 시작된 시간 (15초 제한을 위해)
	bool m_bStageStartFlag;					// 스테이지 시작 플래그
	DWORD m_dwStageEndTime;					// 스테이지가 끝난 시간
	bool m_bStageEndFlag;					// 스테이지 종료
	bool m_bNeedClearMap;					// 맵을 클리어해야 한다고 플래그 설정
	bool m_bNeedCreateMonster;				// 맵에 몹을 만들어야 할 필요가 있을때
	bool m_bTemporaryOccupied;				// 임시로 예약 되어 있나? (예외 처리때문에 맵에 임시 예약 중인 캐릭터 정보가 사라지는걸 막기 위해)
	DWORD m_dwTemporaryOccupyCancelTime;	// 임시 예약 종료 제한시간 (10초)

	// 필드의 상태 정보
	SI16 m_siCurrentStage;							// 현재 진행중인 스테이지

	SI16 m_aStageMobInfo[MAX_MONSTER_COUNT];		// 현재 스테이지에 젠된 몹의 정보 (0: 일반, 1:원킬, 2:키, 3:아이템)
	SI32 m_aGenMosterUnique[MAX_MONSTER_COUNT];		// 현재 스테이지에 젠 된 몹 유니크


public:
	// 상태 체크를 위한 함수들 ( set 함수들이 있지만 내부적으로만 사용하도록 약속)
	bool IsAvailable();
	void SetUserID(SI32 siUserID);
	SI32 GetUserID();
	void SetUserName(TCHAR* pszUserName);
	TCHAR* GetUserName();

	void SetCurrentStage(SI16 siCurrentStage);
	SI16 GetCurrentStage();

	void SetTemporaryReserve(SI32 siUserID);
	void ActiveTemporaryReserve(SI32 siUserID);
	void CancelTemporaryReserve(SI32 siUserID);

	// 가장 주된 작업. 이 함수를 통해서 이 클래스를 조작
	void DoTimeTask();
	void EnterUser(SI32 siUserID, TCHAR* pszUserName);		// 들어오는 사람의 신상명세는 알아야 한다.
	void LeaveUser();										// 나가는 사람의 신상명세는 알필요 없다.
	void GameStart();
	void GameFail();
	void GameClear();

	// 내부 처리 함수들
protected:
	void _WarpUserToMap(SI16 siMapNumber);
	void _CreateMonster(SI16 siStageNumber);
	void _StartCount();
	void _StartStage(SI16 siStageNumber);
	bool _EndStage();
	void _ClearThisMap();
	void _ClearMonster();

	void _SetupCurrentStageMonsterInfo();
	void _SetKeyMob();
	void _SetOneKillMob();
	void _SetItemMob();
	void _NextStage();
	void _ClearReward();

	void _Load_DropItemKind();				// LFF : Load From File - 파일에서 선물몹이 드랍할 아이템 종류를 로드
	void _Load_EventMapCenterPosition();	// 파일에서 이벤트 맵의 중앙 포지션을 로드
	void _Load_GenMobKind();				// 파일에서 젠될 몹의 종류를 로드
	void _Load_StageMobInfo();				// 파일에서 스테이지별 몹 구성(원킬몹 수, 키몹 수, 선물몹 수)을 로드
	void _Load_BBBGameInfo();				// 복불복 게임의 기본적인 속성 로드 (플레이 시간, 각종 이미지 표시 시간)

	void _Notice_8_StageUser();
};

#endif
