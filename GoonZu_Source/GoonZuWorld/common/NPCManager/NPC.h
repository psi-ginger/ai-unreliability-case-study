//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _NPC_H
#define _NPC_H

#include <directives.h>

#include "Char\CharCommon\Char-Common.h"
#include "../../../NLib/NArray.h"

#define MAX_NPC_KIND_PER_GROUP	5	// KKM 2007 / 08 / 22 NPC설정 파일에서 최대 5개	
#define MAX_NPC_PER_GROUP		1000
#define MAX_NPC_GROUP_NUMBER	1000

class cltNPCGroupUnit{
public:
	SI32 siKind;			// 배치할 NPC 종류.		
	SI32 siKindMaxNum;		// 배치할 NPC의 최대 숫자.
	SI32 siKindNum;			// 종류별 NPC의 존재 숫자. 
	SI32 siKindLevel;

	cltNPCGroupUnit()
	{
		Init();
	}

	void Init()
	{
		siKind				= 0;
		siKindMaxNum		= 0;
		siKindNum			= 0;
		siKindLevel			= 0;
	}

	void Set(SI32 kind, SI32 maxnum, SI32 level)
	{
		siKind			= kind;
		siKindMaxNum	= maxnum;
		siKindLevel		= level;
	}
};

class cltNPCGroup
{
public:

	SI32 ID;				// 그룹고유 값. 
	SI32 MaxNPCNumber;	// 그룹내 NPC 합계 
	SI32 siMapIndex;
	SI32 siX, siY;			// NPC 배치의 중심. 
	SI32 siRadius;			// NPC 배치의 반경 
	DWORD dwLastCheckClock;	// 배치를 검사한 마지막 시간. 
	DWORD dwSetIntervalClock;	// 배치 검사 간격. 	
	DWORD dwLastNoticeClock;	// 출현 정보를 클라이언트에 통보한 마지막 시간.

	bool bBossGroup ;		// 보스 그룹이냐 아니냐

	SI32 NPCKindNumber;		// 배치할 NPC종류의 숫자. 

	SI32 CurrentNPCNumber;	// 현재의 NPC 숫자.		

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_NPC_PER_GROUP>					NPCList;
	NSafeTArray<SI32, MAX_NPC_PER_GROUP>					NPCKindList;

	NSafeTArray<cltNPCGroupUnit, MAX_NPC_KIND_PER_GROUP>	clUnit;
#else
	SI32 NPCList[MAX_NPC_PER_GROUP];
	SI32 NPCKindList[MAX_NPC_PER_GROUP];

	cltNPCGroupUnit clUnit[MAX_NPC_KIND_PER_GROUP];
#endif

	cltNPCGroup(SI32 id, SI32 maxnpcnumber, SI32 mapindex, SI32 x, SI32 y, SI32 radius, DWORD interval, bool bossGroup );
	~cltNPCGroup();

	// 그룹에 속할 NPC의 종류와 그 수를 추가한다.
	BOOL AddNPCKind(SI32 kind, SI32 num, SI32 level);
	bool AddChar(SI32 id, SI32 kindindex);
	bool SubChar(SI32 idindex);

	// NPCGroup범위안에 존재하는가 ?
	bool IsInNPCGroup(SI32 mapindex, SI32 x, SI32 y);

	// 특정 kind의 캐릭터가 이 그룹에 존재하는가 ?
	bool IsInKind(SI32 kind);


};

class cltGetNPC
{
protected:
	SI32 m_siRadius;
	SI32 m_siX;
	SI32 m_siY;
	SI32 m_siLevel;
	SI32 m_siKind;
	SI32 m_siNumber;

public:
	cltGetNPC() : m_siRadius(0), m_siX(0), m_siY(0), m_siLevel(0), m_siKind(0), m_siNumber(0) {};
	~cltGetNPC() {};
	
public:
	void SetRadious( SI32 siRadius )	{ m_siRadius	= siRadius; };
	void SetX( SI32 siX )				{ m_siX			= siX; };
	void SetY( SI32 siY )				{ m_siY			= siY; };
	void SetLevel( SI32 siLevel )		{ m_siLevel		= siLevel; };
	void SetKind( SI32 siKind )			{ m_siKind		= siKind; };
	void SetNumber( SI32 siNumber )		{ m_siNumber	= siNumber; };
	

	void SetAll( SI32 siRadius, SI32 siX, SI32 siY, SI32 siLevel, SI32 siKind, SI32 siNumber )
	{
		m_siRadius		= siRadius;
		m_siX			= siX;
		m_siY			= siY;
		m_siLevel		= siLevel;
		m_siKind		= siKind;
		m_siNumber		= siNumber;
	}

	SI32 GetRadius( void )				{ return m_siRadius; };
	SI32 GetX( void )					{ return m_siX; };
	SI32 GetY( void )					{ return m_siY; };
	SI32 GetLevel( void )				{ return m_siLevel; };
	SI32 GetKind( void )				{ return m_siKind; };
	SI32 GetNumber( void )				{ return m_siNumber; };

};
//-------------------------------------------
// cltNPCManagerCommon
//-------------------------------------------
class cltNPCManagerCommon{
public:
	cltCharManager* pclCM;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltNPCGroup*, MAX_NPC_GROUP_NUMBER>		pclNPCGroup;
#else
	cltNPCGroup* pclNPCGroup[MAX_NPC_GROUP_NUMBER];
#endif

	cltNPCManagerCommon(cltCharManager* pclCM);
	virtual ~cltNPCManagerCommon();

	virtual BOOL SetChar(){return FALSE;}

	enum NPCMC
	{
		NPCMC_SEARCH_COUNT = 20
	};


	BOOL SetNPCGroup(SI32& group, TCHAR* npcgroupname, bool clonemap = false, SI32 clonemapindex = 0 );
	BOOL SetNPCGroup();

	// NPCGroup범위안에 존재하는가 ?
	bool IsInNPCGroup(SI32 npcgroup, SI32 mapindex, SI32 x, SI32 y);

	bool MakeNPCInstance(SI32 mapindex,SI32 party,bool boss = false );
	bool DeleteNPCInstance(SI32 mapindex);
	bool CheckTimeOut(SI32 mapindex); // 시간 초과 계산. true가 오면 시간 지나버린 것임.

	void SendInstanceMapInfo(SI32 mapindex);
	void SetInstanceChar(SI32 mapindex);
	void SendGift(SI32 mapindex,SI32 myid,SI32 enemyid);
	
	// [영훈] 20404 - 젠 몬스터 추가 
	void SetGuildNPCPoint( SI32 siKind, cltIdentity *pclIdentity );	// 길드 인던 몬스터 점수
	void MakeGuildRegenerationNPC( SI32 siMapIndex );				// 길드 인던 몬스터 리젠하기
	bool GetGuildNPCInformation( IN SI32 siMapIndex, OUT SI32 *psiKind, OUT SI32 *psiNumber );		// 길드 인던 안에 있는 몬스터 종류/수 알아오기
	bool GetGroupNPCInformation( IN SI32 siMapIndex, OUT cltGetNPC *pclGetNPC );		// NPC 그룹에 있는 몬스터 종류/수 알아오기

	// npc 그룹에서 특정 kind의 레벨을 얻는다.
	SI32 GetKindLevel(SI32 kind);

	// npc 그룹에서 특정 kind의 최고레벨을 얻는다.
	SI32 GetKindMaxLevel(SI32 kind);
};


//-------------------------------------------
// REGEN 정보를 얻어오기위한 구조체 // JAYCEE
//-------------------------------------------
class cltRegenPos {
public:
	TCHAR szMapName[50];
	TCHAR szMapPos[50];
	TCHAR szNearMapInfo[50];

	cltRegenPos()
	{
		memset(szMapName,		0, sizeof(szMapName));
		memset(szMapPos,		0, sizeof(szMapPos));
		memset(szNearMapInfo,	0, sizeof(szNearMapInfo));
	}	
};

//-------------------------------------------
// cltNPCManagerClient
//-------------------------------------------
class cltNPCManagerClient : public cltNPCManagerCommon{
public:

	cltNPCManagerClient(cltCharManager* pclCM);
	~cltNPCManagerClient();

	// npc 그룹에서 특정 kind의 위치 정보 text를 얻어온다.
	bool GetKindPos(SI32 kind, TCHAR* ptext, SI16 txtSize);
	bool GetKindPos(SI32 kind, cltRegenPos *regenpos, SI32 *siCount);
	// npc 그룹에서 특정 kind의 레벨을 얻는다.
//	SI32 GetKindLevel(SI32 kind);
//	bool GetDirectionStr(SI32 sx, SI32 sy, SI32 dx, SI32 dy, TCHAR *szText);

	// npc 그룹에서 특정 맵(index)에 있는 몬스터들의 kind를 가져온다.
	bool GetKindMapInx(SI32 mapindex, SI32 siKind[], SI32 siKindLevel[], SI32 siMaxSize);
	// 찾아낸 데이타에서 이미 있는지 없는지 검색한다 있으면 true 없으면 false
	bool FindKindData(SI32 siKind[], SI32 siCount, SI32 npckind);

};

//-------------------------------------------
// cltNPCManagerServer
//-------------------------------------------
class cltNPCManagerServer : public cltNPCManagerCommon{
public:

	cltNPCManagerServer(cltCharManager* pclCM);
	~cltNPCManagerServer();

	// NPC 그룹 정보에 따라 캐릭터를 추가한다.
	BOOL SetChar();

	// npc 그룹에서 특정 레벨범위에 있는 몬스터들의 kind를 가져온다.
	bool GetKindLevel(SI32 sLevel, SI32 eLevel, SI32 siKind[], SI32 maxsize);

	// 찾아낸 데이타에서 이미 있는지 없는지 검색한다 있으면 true 없으면 false
	bool FindKindData(SI32 siKind[], SI32 siCount, SI32 npckind);

	//[추가 : 황진성 2007. 10. 31 도마돈 생성시 이름 생성된 지도내 몬스터 이름을 탖기위해 추가.]
	// npc 그룹에서 특정 맵(index)에 있는 몬스터들의 kind를 가져온다.
	bool GetKindMapInx(SI32 mapindex, SI32 siKind[], SI32 siKindLevel[], SI32 siMaxSize);
};

#endif