#ifndef _GUILD_H
#define _GUILD_H

#include <Directives.h>

#include "../../Common/CommonHeader.h"
#include "../StructureClass/StructureClass.h"



#define MAX_GUILD_NUM			500			// 서버 내의 전체 길드 제한수
// 디비 에서 한번에 보낼 리스트 제한 수를 패킷사이즈가 오버 될수 있어서 줄입니다.
// 차후 디비 정보를 늘릴때 패킷사이즈를 확인 해야 합니다. 25 -> 10
#define MAX_GUILD_PER_DBLIST	10			// DB에서 가져올 때 한번에 가져올 수 있는 리스트에서의 제한수	

#define MAX_GUILD_NAME			13			// 길드 이름 제한. 13바이트이지만 실질적으로 12바이트 제한

#define MAX_GUILD_MEMBER_NUM	35			// 길드 최대 인원수
#define MIN_GUILD_MEMBER_NUM	8			// 길드 최소 인원수

#define MAX_GUILD_PER_VILLAGE	50			// 하나의 마을에 소속되는 길드의 최대수

#define MIN_GUILD_MASTER_LEVEL	40			// 길드 마스터 최소 레벨

#define MIN_GUILD_CREATE_FEE	1000000		// 길드 개설비용

#define MAX_GUILDHUNTWAR_REQUEST_NUM 100	// 최대 길드맵소유권변경 전투 신정 길드<-현제 사용 안함.
#define MAX_GUILDHUNTWAR_RANK_NUM 10		// 길드맵 소유권변경랭크

#define MAX_HUNTMAP_GUILDWAE_NUM	180			// 길드 최대 인원수


class cltGuildMember // 길드원
{
public:
	SI32			siPersonID;
	TCHAR			szCharName[MAX_PLAYER_NAME];
	SI08			siType;
	GMONEY			siMoney;
	SI32			siGP;	// 개인 GP
	SI16			m_siLevel;	//[추가 : 황진성 2008. 2. 13 길드원 레벨 정보 추가.]

	cltGuildMember()
	{
		siPersonID = 0 ;
		ZeroMemory(szCharName,sizeof(TCHAR)* MAX_PLAYER_NAME);
		siType = 0 ;
		siMoney = 0 ;
		m_siLevel = 0;
	}
};
// 하루동안 싸운 길드 유니크를 기록한다.- 변역된 길드워로 싸운 적길드
class cltHuntMapGuildWarEnemy
{
public:
	SI32 siEnemyGuildUnique;
	cltHuntMapGuildWarEnemy()
	{
		siEnemyGuildUnique = 0;
	}
	void init()
	{
		siEnemyGuildUnique	=	0;
	}
	void SetEnemy(SI32 EnemyGuild)
	{
		siEnemyGuildUnique	=	EnemyGuild	;
	}
	SI32 GetEnemy()
	{
		return siEnemyGuildUnique;
	}
};

class cltGuildStandingMemeber // 길드원 대기자
{
public:
	SI32			siPersonID;
	TCHAR			szCharName[MAX_PLAYER_NAME];

	cltGuildStandingMemeber()
	{
		siPersonID = 0 ;
		ZeroMemory(szCharName,sizeof(TCHAR)* MAX_PLAYER_NAME);
	}
};

// 길드 GP 및 돈 관련
class cltGuildPoint{
public:
	SI32			siLevel; // Guild Level
	SI32			siPoint; // GP(Guild Point)
	GMONEY			siMoney; // Guild Money

	cltDate			clDate; // 수수료 납부한 마지막 날자
	SI32			siFaultNum; // 수수료 미납 횟수 - 3번 미납하면 길드 해체

	SI32			siMarkIndex; // 마크 인덱스

	SI32			siApplyMapIndex;	// 신청한 던전의 맵인덱스
	SI32			siBattleGP;				// 길드전으로 모인 GP

	cltGuildPoint()
	{
		siLevel = 0 ;
		siPoint = 0 ;
		siMoney = 0 ;

		clDate.Set(0,0,0,0,0);
		siFaultNum = 0 ;

		siMarkIndex = 0 ;

		siApplyMapIndex = 0;
		siBattleGP = 0;
	}
};

// 길드 능력치
class cltGuildStatus : public cltBasicAbility{
public:
/*
	SI32		siStr;		// 힘
	SI32		siDex;		// 민첩
	SI32		siMag;		// 도력
	SI32		siVit;		// 체력
	SI32		siHnd;		// 손재주
	SI32		siLuck;		// 운
*/
	SI32		siBonus;

	cltGuildStatus()
	{
		siBonus = 0 ;
	}

	SI32	GetTotalStatus()
	{
		return siStr+siDex+siMag+siVit+siHnd+siLuk+siBonus;
	}
};

#define GUILDSTATUS_STANDING			0	// 현재 대기중
#define GUILDSTATUS_NORMAL				1	// 정상 활동중
#define GUILDSTATUS_CHANGEVILLAGE		2	// 본거지 이동중
#define MAX_GUUILDENEMYINFO_TODAY			72	// 하루동안 최대로 만날수 있는 길드전 상대길드의 정보 
// 길드의 기본정보
class cltGuildInfo{
public:
	SI32					siUnique; // 길드 유니크
	TCHAR					szName[MAX_GUILD_NAME]; // 길드 이름

	SI32					siVillageUnique;			// 길드가 있는 건물 소속 국가
	SI32					siMoveVillageUnique;		// 거주지 이동할 국가

	SI32					siMasterPersonID; // 길드 마스터 personid

	cltGuildPoint			clGPoint;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltGuildMember, MAX_GUILD_MEMBER_NUM>				clMember;
	NSafeTArray<cltGuildStandingMemeber, MAX_GUILD_MEMBER_NUM>		clStandingMember;
	NSafeTArray<cltHuntMapGuildWarEnemy, MAX_GUUILDENEMYINFO_TODAY>				clHuntguildEnemy;// 길드맵소유방식변경길드전을싸운상대방
#else
	cltGuildMember			clMember[MAX_GUILD_MEMBER_NUM]; // 길드 회원 정보
	cltGuildStandingMemeber	clStandingMember[MAX_GUILD_MEMBER_NUM]; // 길드 가입 요청자 정보
	cltHuntMapGuildWarEnemy	clHuntguildEnemy[MAX_GUUILDENEMYINFO_TODAY]; // 길드맵소유방식변경길드전을싸운상대방길드
#endif

	cltDate					clDate; // 길드 개설일
	GMONEY					siFee; // 임대료
	UI08					uiConfirm; // 승인 여부. true면 승인. false면 비승인(비활성)

	cltGuildStatus			clStatus;	// 길드 능력치

	cltGuildInfo()
	{
		Init();
	}

	void Init()
	{
		siUnique = 0 ;
		ZeroMemory(szName,sizeof(TCHAR)*MAX_GUILD_NAME);

		siVillageUnique = 0 ;
		siMoveVillageUnique = 0;

		siMasterPersonID = 0 ;

		ZeroMemory(&clGPoint,sizeof(cltGuildPoint));

		for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
		{
			ZeroMemory(&clMember[i],sizeof(cltGuildMember));
			ZeroMemory(&clStandingMember[i],sizeof(cltGuildMember));
		}
		/*for ( SI32 i = 0 ; i < MAX_HUNTMAP_GUILDWAE_NUM ; i ++ )
		{
			ZeroMemory(&clHuntguildEnemy[i],sizeof(cltHuntMapGuildWarEnemy));
		}*/

		clDate.Set(0,0,0,0,0);
		siFee = 0 ;
		uiConfirm = GUILDSTATUS_STANDING ;

		ZeroMemory(&clStatus,sizeof(cltGuildStatus));
		ZeroMemory(&clStatus,sizeof(cltGuildStatus));
	}

	void Set(cltGuildInfo* pclinfo)
	{
		memcpy(this,pclinfo,sizeof(cltGuildInfo));
	}

	SI32 GetMemberNum();
	SI32 GetLimitMemberNum();
	bool IsSecondMaster();
	TCHAR* GetName();
	
	// 길드맵 소유방식변경 길드전 상대방 기록
	void setHuntWarEnemy(SI32 enemyGuildunique);
	SI32 findEmptySlot();
	bool FindBeforeEnemyGuild(SI32 siGuildUnique);// 이전에 싸운 길드
};

//-------------------------------------------------
// 개별적인 사복시
//-------------------------------------------------
class cltGuild : public cltStructureBasic{
private:

public:
	cltGuild(SI32 sivillageunique);
	~cltGuild();

};

//--------------------------------------
// 길드상단 관리자. 
//--------------------------------------
class cltGuildManager : public cltStructureMngBasic{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltGuildInfo, MAX_GUILD_NUM>		clUnit;
#else
	cltGuildInfo clUnit[MAX_GUILD_NUM]; // 길드의 정보
#endif
	cltGuildInfo clCommonUnit;			// 불러진 길드 정보(Client에서만 사용)

	HIMAGELIST	hImageList;

	// 인스턴스던젼 탑 길드 정보
	SI32			siTopClearPoint;				//획득 점수	- 현재 년도
	SI32			siBeforeYearTopGuildUnique;		//길드 유니크 - 전 년도
	// 하루에 3번까지 할수 있다.
	cltGuildManager( cltCharManager* pclcm, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltGuildManager();

	cltGuildInfo* GetGuildInfo(SI32 guildunique);
	cltGuildInfo* GetGuildInfo(TCHAR* guildname );
	TCHAR*	GetGuildName(SI32 guildunique);

	// 최초 환영 인사말표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 charunique);

	BOOL CreateGuild(SI32 siunique);

	bool AddGuildInfo(cltGuildInfo* pclinfo);
	bool DeleteGuildInfo(SI32 guildunique);
	
	void Action(bool bdaychangedswitch);
};

#endif