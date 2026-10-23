#ifndef _MAP_H
#define _MAP_H     1

#include <windows.h>
#include <ddraw.h>
#include "../Map\TileSet\TileSet.h"
//#include "Map\FieldObject\FieldObject.h"
#include "..\..\Client\Cloud\CloudMgr.h"
#include "..\..\Client\Effect\WaterDrop\WaterDropMgr.h"
#include "..\..\Client\Effect\SeedDrop\SeedDropMgr.h"

// 네트워크용 헤더. 
#include "..\..\NetworkLib\ITZNetLib\ITZNetworkThread.h"
#include "..\..\NetworkLib\ITZNetLib\ITZPerson.h"
#include "..\..\NetworkLib\ITZNetLib\ITZListen.h"

#include "../../CommonLogic/Guild/Guild.h"

#include "MapLayor\Map-Layor.h"

class CAStarListMgr;

class _MoveMark;
class cltCharManager;
class cltServerMap;
class cltMapCommon;
class CFireFlyMgr;
class CIceFlashMgr;
class cltHelpManager;
class cltFieldObjectManager;
class cltClient;

#define MAP_TILE_XSIZE			80
#define MAP_TILE_XSIZE_HALF		40
#define MAP_TILE_YSIZE			40
#define MAP_TILE_YSIZE_HALF		20

#define MAX_MAP_LAYER			2
#define MOUSETILEATB_FARM		1


//--------------------------------------------------
// 충돌 속성 
//--------------------------------------------------
// 0x1000부터 시작된다. 
#define BLOCK_CHAR							((DWORD)1<<12)      // 캐릭터의 속성 10진수로 4096
#define BLOCK_SYSTEMNPC						((DWORD)1<<14)		//

#define BLOCK_NONE							((DWORD)1)
#define BLOCK_DEEPWATER						((DWORD)1<<1)
#define BLOCK_SHALLOWWATER					((DWORD)1<<2)
#define BLOCK_NORMAL						((DWORD)1<<3)
#define BLOCK_LAND							((DWORD)1<<4)
#define BLOCK_CUSTOM						((DWORD)1<<5)		// 임의 지역에 대한 충돌 설정. (위치 변경되면 안됨) 
#define BLOCK_EVENT1						((DWORD)1<<6)		// 이벤트 위한 충돌 설정( 위치 변경되면 안됨)
#define BLOCK_EVENT2						((DWORD)1<<7)		// 이벤트 위한 충돌 설정( 위치 변경되면 안됨)
#define BLOCK_EVENT3						((DWORD)1<<8)		// 이벤트 위한 충돌 설정( 위치 변경되면 안됨)
#define BLOCK_EVENT4						((DWORD)1<<9)
#define BLOCK_EVENT5						((DWORD)1<<10)
#define BLOCK_EVENT6						((DWORD)1<<11)
#define BLOCK_EVENT7						((DWORD)1<<13)
	

//-------------------------------------------------
// 주요지역 MapIndex
//-------------------------------------------------
#define MAPINDEX_MAINFIELD						0			// 메인필드 
#define MAPINDEX_BEGINNER1						108			// 초보자사냥터1
#define MAPINDEX_PALACE							158			// 엔펠리스.

//#define MAPINDEX_BATTLESEA					162			// 문무대왕릉
//#define MAPINDEX_DOCKDO						171			// 독도
#define MAPINDEX_MASTERWORLD					329			// 영자나라
//#define MAPINDEX_DAEMADOGATE					173			// 대마도해적소굴입구
//#define MAPINDEX_DAEMADO						300			// 대마도

//#define MAPINDEX_CHINABORDER					176
//#define MAPINDEX_BEGINNER2					177			// 초보자사냥터2

#define MAPINDEX_TRADINGPORT					179			// 무역항

#define MAPINDEX_GUILDWARFIELD					195			// 길드전 필드 195~199	->	기존 길드전 필드는 유지시키고 새로운 길드전에서 사용한다

#define MAPINDEX_GUILDWARFIELD_OLD				369			// 길드전 필드 369~371 -> 일반 길드전은 3경기로 축소 되면서 길드전 맵을 옮긴다.

#define MAPINDEX_MOFUMOFULAND					103			// 모후모후 던젼 이벤트 맵

#define MAPINDEX_MOFUMOFU_AGIT					320			// 모후모후 아지트 인던

#define MAPINDEX_PIRATESHIP						330			// 해적선 파티용 인던 맵

#define MAPINDEX_PRIVATEMARKETAREA				95			// 개인상점 지역
#define MAPINDEX_FESTIVAL						97			// 축제맵 지역

#define MAX_INSTANCE_MAP_NUMBER					5			// 인스턴스 던전의 수
#define MAX_INSTANCE_BONUS_MONSTER_NUMBER		3			// 인스턴스 던전 내에서 보너스 몬스터 수

#define MAX_INSTACNE_REGEN_MONSTER_NUMBER		2			// 인스턴스 던전 내에서 리젠 몬스터 수

#define PIRATESHIP_START_POSITION_X				12			// 해적선 내부 스타트 위치 X				
#define PIRATESHIP_START_POSITION_Y				180			// 해적선 내부 스타트 위치 Y

#define MAPINDEX_SERVEREVENT_LOBBY				356			// 서버 이벤트 인스턴스던전 1

#define MAPINDEX_SERVEREVENT_DUNGEON1			357			// 서버 이벤트 인스턴스던전 1
#define MAPINDEX_SERVEREVENT_DUNGEON2			358			// 2
#define MAPINDEX_SERVEREVENT_DUNGEON3			359			// 3
#define MAPINDEX_SERVEREVENT_DUNGEON4			360			// 4

#define MAPINDEX_PARTYHALL_1					335			// 파티홀(궁전파티장)
#define MAPINDEX_PARTYHALL_2					336			// 파티홀(황금의 방)
#define MAPINDEX_PARTYHALL_3					337			// 파티홀(무인도 파티)
#define MAPINDEX_PARTYHALL_4					338			// 파티홀(군주의 방)
#define MAPINDEX_PARTYHALL_5					339			// 파티홀(정원파티)

#define MAPINDEX_PVPMAP							380			// PVPMAP

#define MAPINDEX_SANTAFACTORY_1					385			// 아이의 산타 작업장.
#define MAPINDEX_SANTAFACTORY_2					386			// 청년의 산타 작업장.
#define MAPINDEX_SANTAFACTORY_3					387			// 어른의 산타 작업장.
#define MAPINDEX_SANTAFACTORY_1_2				102			// 아이의 산타 작업장.
#define MAPINDEX_SANTAFACTORY_2_2				123			// 청년의 산타 작업장.
#define MAPINDEX_SANTAFACTORY_3_2				124			// 어른의 산타 작업장.

#define MAPINDEX_WEDDINGHALL					372			// 예식장
#define MAPINDEX_BATTLEROYAL_MABRIUM			373			// 배틀로얄 - 마브리움
#define MAPINDEX_BATTLEROYAL_GRANDMABRIUM		374			// 배틀로얄 - 그랑마브리움

#define MAPINDEX_LABYRINTH_1					375			// 안개미궁 1층

//#define MAPINDEX_BOKBULBOK_NPC					95			// 축제 - 복불복NPC	



//-------------------------------------------------
// PVP가 가능한 MapIndex
//-------------------------------------------------
#define PVP_MAPINDEX1							135			

enum INSTANCEMAP_TYPE
{
	INSTANCEMAP_TYPE_NONE,
	INSTANCEMAP_TYPE_GUILD,
	INSTANCEMAP_TYPE_PARTY,
	INSTANCEMAP_TYPE_VILLAGE,
	INSTANCEMAP_TYPE_SERVER,
	

	//---------------------------------------------------------
	//	이 위로 추가해 주세요
	//---------------------------------------------------------
	INSTANCEMAP_TYPE_MAX
};

//-------------------------------------
// FOG(안개)
//----------------------------------
// 포그 속성
#define FOG_BLACK				2		// 완전 어두움
#define FOG_HALF				1		// 반투명
#define FOG_NONE				0		// 없음

// 포그처리용 방향. 
#define FOG_SOUTH		1
#define FOG_SW			2
#define FOG_WEST		4
#define FOG_NW			8
#define FOG_NORTH		16	
#define FOG_NE			32
#define FOG_EAST		64	
#define FOG_SE			128

//---------------------------------------------------
// 지도의 헤더. 
//---------------------------------------------------
class cltMapHeader{
public:
	DWORD Version;                          // 지도의 버전 
	DWORD Unique;                           // 고유 정보 
	TCHAR Text[128];                         // 문자열 정보 
	DWORD Reserved0;	                   // 게임에 참여하는 유저 수 .
	SHORT MapXsize, MapYsize;               // 지도의 가로 세로 사이즈 
	SI32 MapType;                          // 지도 타일의 종류 
	SI32 TileSet1;							// Layor1 용 타일세트

	bool bWeatherSwitch;					// 날씨가 작동하는 지도인가 ?(구름이나 비등..) 

	SI16 TileSet2;							// Layor2 용 타일세트 

	TCHAR Reserved[19];                   

	cltMapHeader();

};

// 일반적인 Field Object의 데이터 분석값. 
class cltFieldObjectBitField_Normal{
private:
	UI32 FieldUnique:		15;
	UI32 ValidSwitch:		1;
	UI32 RandomValue:		4;
	UI32 LengthToBottom:	4;		// 해당 FieldObject 맨 아랫쪽과의 변위 
	UI32 LengthToRight:		4;		// 해당 FieldObject 맨 오른쪽과의 변위 
	UI32 Reserved:			4;

public:
	UI32 GetFieldUnique()const;
	void SetFieldUnique(UI32 unique);
	UI32 GetValidSwitch()const;
	void SetValidSwitch(UI32 valid);
	UI32 GetRandomValue()const;
	void SetRandomValue(UI32 random);
	UI32 GetLengthToBottom()const;
	void SetLengthToBottom(UI32 bottom);
	UI32 GetLengthToRight()const;
	void SetLengthToRight(UI32 right);

};

//-----------------------------------------
// 아이템 지도 
//-----------------------------------------
class cltItemMapInfo{
public:
	UI16 uiItemUnique;		// 아이템의 유니크. 
	UI16 uiItemIndex;		// 서버 아이템리스트 상의 인덱스. 

	cltItemMapInfo();
};


#define MAX_EFFECT_PER_MAP			200
//--------------------------------------
// 이펙트 관련
//----------------------------------------
class cltEffectMapInfo{
public:
	SI32			gimg;
	SI32			x;
	SI32			y;
	SI32			font;
	bool			reverse;

	void Init();
	void Set(cltEffectMapInfo* pclinfo);
};

// Field Object 데이터 값을 분석하기 위한 유니온. 
union uFieldObjectUnion{
	UI32 Data32;

	cltFieldObjectBitField_Normal FieldObject_Normal;
};

//-----------------------------------------------------
// 지도 지형 안내 정보 
//-----------------------------------------------------
#define MAX_GEO_NUMBER	256
#define GEO_TEXT_LENGTH	64
class cltMapGeoInfo{
public:
	cltMapCommon* pclMap;
	SI32		siXsize, siYsize;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_GEO_NUMBER>		siGeoPosX;
	NSafeTArray<SI32, MAX_GEO_NUMBER>		siGeoPosY;
	NSafeTArray<SI32, MAX_GEO_NUMBER>		siGeoPosAccount;
#else
	SI32		siGeoPosX[MAX_GEO_NUMBER];
	SI32		siGeoPosY[MAX_GEO_NUMBER];
	SI32		siGeoPosAccount[MAX_GEO_NUMBER];	
#endif

	TCHAR		szGetText[MAX_GEO_NUMBER][GEO_TEXT_LENGTH];	// 지형정보의 텍스트가 저장될 공간. 
	UI08*		pclGeoMap;									// 지형 정보가 들어갈 공간. 

	cltMapGeoInfo(cltMapCommon* pclmap, SI32 xsize, SI32 ysize);
	~cltMapGeoInfo();

	void SetGeoMap(SI32 x, SI32 y, UI08 data);
	UI08 GetGeoMap(SI32 x, SI32 y);
				
	void InitGeoPosInfo();									// 지형정보의 위치 변수를 초기화한다. 
	void UpdateGeoPos(SI32 x, SI32 y, SI32 geoinfo);		// 지형정보 위치를 업데이트 한다. 

	BOOL Write(FILE* fp);
	BOOL Read(FILE* fp);
};

//----------------------------------------------
// cltMapCommon
//----------------------------------------------
class cltMapCommon{
public:
	// 지도의 기본 정보. 
	cltMapHeader			MH;
	
	SI32					siMapUnique;			// 지도의 유니크.
	SI32					siMapIndex;

protected:
	short					*pIDMap;

	// 충돌 속성을 저장한다. 
	short					*pCollMap;  

	DWORD					*pFieldObjectMap;  
	DWORD					*pFieldObjectMapPara;

	cltItemMapInfo*			pclItemMap;				// 아이템이 땅에 떨어진 정보. 

public:
	cltMapGeoInfo			*pclMapGeoInfo;			// 지형안내 정보. 

	cltTileManager*			pclTileManager;
	cltFieldObjectManager*	pclFieldObjectManager;

	//-----------------------------------
	// 캐릭터 관리자. 
	//-----------------------------------
	cltCharManager*			pclCM;

	//-----------------------------------
	// 마을의 유니크. 
	//------------------------------------
	SI32					siVillageUnique; // 이지도가 마을의 지도라면 이 변수에 마을의 Unique값이 저장된다. 
	
	//------------------------------------------
	// 길찾기 객체 
	//------------------------------------------
	CAStarListMgr*			pclPath;
	SI32					MoveID;

	//---------------------------------------
	// 지도 검색용 Ref
	//---------------------------------------
	SI32*					YOff;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltMapLayer*, MAX_MAP_LAYER>		pclLayer;
#else
	cltMapLayer*			pclLayer[MAX_MAP_LAYER];
#endif

	RECT					ValidRect;		// 지도의 가동 범위 

	//---------------------------------------
	// 인스턴스 던젼용 데이터
	//---------------------------------------
	SI32					m_siInstanceMapData;		// 인스턴스 던젼 소유 정보
	DWORD					m_dwInstanceMapStartTime;	// 인스턴스 던젼 시작 시각
	DWORD					m_dwInstanceMapMaxTime;		// 인스턴스 던젼 시작 시각
	SI32					m_siInstanceMapScore;		// 점수 
	SI16					m_siInstanceMapGateIndex;	// 게이트 세트 번호
	SI32					m_siGateMonID;				// 게이트 생성 몬스터
	SI32					m_siBossID;					// 게이트 생성 몬스터
	
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_INSTANCE_BONUS_MONSTER_NUMBER>		m_siBonusMonID;
#else
	SI32					m_siBonusMonID[MAX_INSTANCE_BONUS_MONSTER_NUMBER];	// 인스턴스 던젼내 보너스 몬스터
#endif
	SI32					m_siMaxMonsterNum;			// 총 생성된 몬스터 수
	SI32					m_siKillCount;				// 죽인 몬스터 수
	SI32					m_siBonusMonCount;			// 보너스 시간계산용 보너스 몬스터를 얼마나 죽였는지
	
	SI32					m_siClearStep;				// 현재 인던을 몇 단계 클리어 했는가?
	SI32					m_siNPCGroupID;				// 현재 소환되어있는 NPC GroupID
    	

	// 길드 버그에 의한 임시 데이터 - by LEEKH 2008.02.13
	TCHAR					m_szGuildName[MAX_GUILD_NAME];

public:
	cltMapCommon();
	cltMapCommon(SI32 mapindex, SI32 mapunique, SHORT xsize, SHORT ysize, SI32 tileset1, SI32 tileset2, cltCharManager* pclCM, cltTileManager* pcltileset, cltFieldObjectManager* pclfieldobjectmanager);
	
	virtual ~cltMapCommon();
	void Init();

	virtual BOOL MakeSmallMap(SI32 smallmapmode, TCHAR* mapfilename){return FALSE;}
	virtual void DeleteSmallMap(){}
	

	//-----------------------------------
	// 기본 정보 설정. 
	//-----------------------------------
	SHORT	GetXsize();
	SHORT	GetYsize();

	//---------------------------------------
	// 지도 할당 및 해제 
	//--------------------------------------
	void AllocMap(SHORT xsize, SHORT ysize);
	void FreeMap();

	//--------------------------------------------------------
	// Field Object 관련 
	//--------------------------------------------------------
	BOOL PutFieldObject(SI32 six, SI32 siy, TCHAR* name, SI32 para);
	BOOL PutFieldObject(SI32 six, SI32 siy, SI32 siunique, SI32 para = 0);
	BOOL PutGateFieldObject(SI32 six, SI32 siy, SI32 siunique, SI32 para);

	BOOL DeleteFieldObject(SI32 six, SI32 siy);						// six, siy좌표에 있는 Field Object를 삭제한다.
	void DeleteFieldObjectUnique(SI32 siUnique);					// 해당 유니크의 Field Object를 모두 지운다
	BOOL PutFieldObjectBrush(SI32 six, SI32 siy, SI32 brush);		// Field Object 브러시를 사용해서 찍어준다. 

	BOOL SetFieldObject_Unique(SI32 six, SI32 siy, SI32 siunique);
	SI32 GetFieldObject_Unique(SI32 six, SI32 siy);

	BOOL SetFieldObjectPara(SI32 six, SI32 siy, SI32 para);
	SI32 GetFieldObjectPara(SI32 six, SI32 siy);

	BOOL GetFieldObjectValidSwitch(SI32 six, SI32 siy);
	void SetFieldObjectValidSwitch(SI32 six, SI32 siy, BOOL bmode);		// 필드 오브젝트를 출력 유효하게 하거나 무효하게 한다. 

	void SetFieldObjectRandomValue(SI32 six, SI32 siy, BOOL value);
	UI32 GetFieldObjectRandomValue(SI32 six, SI32 siy);

	void SetFieldObjectLengthToBottom(SI32 six, SI32 siy, SI32 length);
	UI32 GetFieldObjectLengthToBottom(SI32 six, SI32 siy);

	void SetFieldObjectLengthToRight(SI32 six, SI32 siy, SI32 length);
	UI32 GetFieldObjectLengthToRight(SI32 six, SI32 siy);
	bool GetFieldObjectCenterPos(SI32 six, SI32 siy, SI32* pcenterx, SI32* pcentery);		// six, siy지점에 있는 Field Object의 중심 위치를 구해서 리턴한다. 
	
	void NewFieldObjectSetInMap();		// 지도에 새롭게 Field Object를 배치한다. 

	//------------------------------------------------
	// 영역 확인 
	//------------------------------------------------
	// x, y가 지도 좌표 안에 있는가?
	// x, y 기본좌표 
	bool	IsInMapArea(SI32 x, SI32 y);

	//---------------------------------------------
	// 좌표 변환 
	//---------------------------------------------
	// 지도상의 X, Y좌표의 중심의 도트 좌표를 돌려준다. (절대좌표)
	// 화면상의 좌표가 아니라 절대 좌표이다. 
	BOOL GetXYDotFromXYCord(SI32 x, SI32, SI32 & centerx, SI32 & centery);
	void GetRealTileByRealDot(SHORT realx, SHORT realy, SI32* x, SI32* y);

	//----------------------------------
	// Save & Load
	//----------------------------------
	BOOL WriteIDMap(FILE *fp);
	BOOL ReadIDMap(FILE *fp);

	BOOL WriteCollMap(FILE *fp);
	BOOL ReadCollMap(FILE *fp);

	BOOL WriteFieldObjectMap(FILE *fp);
	BOOL ReadFieldObjectMap(FILE *fp);

	BOOL WriteFieldObjectMapPara(FILE *fp);
	BOOL ReadFieldObjectMapPara(FILE *fp);

    BOOL Save(TCHAR* filename);		// 지도를 저장한다. 

	//---------------------------------------------------------
	// 건물 관련 
	//---------------------------------------------------------
	
	void MakeVillageStructure();				// 마을의 건물들을 만든다. 
	void DeleteVillageStructure(SI32 slot);		// 특정 슬롯에 있는 건물을 지운다. 

	
	
	// 리턴값 : -1 -> 슬롯이 아닌 곳
	// 지도의 특정 좌표가 몇 번 건물용 슬롯에 해당하는지 알려준다. 
	SI32 GetStructureSlot(SI32 villageunique, SI32 mapx, SI32 mapy);	
	
	// 특정 슬롯의 시작 좌표를 알려준다.
	bool GetSlotStartPos(SI32 villageunique, SI32 slot, SI32* pstartx, SI32* pstarty);

//------------------------------------------------------------
//   타일 관련 함수 
//------------------------------------------------------------
	
	void SetUnique(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 siunique);				// 타일의 배수등을 고려하여 Unique를 설정한다.
	void SetUniqueSouth(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 siunique);			// 주어진 타일의 남쪽에 같은 unique를 설정한다.
	void SetUniqueWest(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 siunique);			// 주어진 타일의 서쪽에 같은 unique를 설정한다.
	void SetUniqueNorth(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 siunique);			// 주어진 타일의 북쪽에 같은 unique를 설정한다.
	void SetUniqueEast(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 siunique);			// 주어진 타일의 동쪽에 같은 unique를 설정한다.
	
	SI32 GetSelfUniqueInfo(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 workunique);		// 자기 블럭의 상태를 파악한다. 
	void SetDrawInfo(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 unique, SI32 sifile, SI32 sifont);		// 출력용 정보를 타일에 기록한다. 

	void PutTileXpr(SI32 layor, SI32 six, SI32 siy, SI32 unique);
	void PutTile0(SI32 layor, SI32 six, SI32 siy, SI32 unique);
    BOOL PutTile2(SI32 layer, SI32 six, SI32 sy, SI32 siunique);
	BOOL PutTile4(SI32 layer, SI32 six, SI32 sy, SI32 siunique);
	
	BOOL IsChildTile(SI32 layer, SI32 six, SI32 siy, SI32 rate, SI32 unique, SI32* pchildunique);		// layer, six, siy 지점의 DrawInfo의 출력 타일 세트가 unique타일 세트의 자식 세트인지 확인한다. 
	BOOL IsDiffTileSet(SI32 sisrcunique, SI32 sidstunique);		// sisrcunique와 sidstunique가 서로 이종타일(같지도 않고 부모자식 관계도 아닌 )인지의 여부를 알려준다. 
	BOOL GetProperFileNFont(SI32 unique, SI32 result, SI32& file, SI32& font);


	//-------------------------------------------
	// 지도를 최종적으로 사용가능하게 만드는 함수. 
	//--------------------------------------------
	virtual void MakeLastMap();

	//-----------------------------------------
	// 충돌 관련 
	//----------------------------------------
	SI32 CalcCollInfo(SI32 x, SI32 y);		// 지도 x, y지점의 충돌종류와 여부를 계산한다. 
	
    BOOL	SetCollMap(SHORT x, SHORT y, SHORT block);		// 충돌 속성을 설정한다. 
	SHORT	GetCollMap(SI32 x, SI32 y);
	
    void	DeleteCollMap(SHORT x, SHORT y, SHORT block);	// 특정 비트를 지운다. 
	BOOL	DeleteCharCollMap(SHORT x, SHORT y);

    bool	IsColl(SHORT x, SHORT y, SHORT block);			// 어느지점의 충돌 여부를 알려준다. 

	//----------------------------------------------
	// IDMap 관련 
	//----------------------------------------------
	void	SetIDMap(SHORT id, SI32 x, SI32 y);
    SHORT	GetIDMap(SI32 x, SI32 y);
	void	ClearIDMap(int x, int y);

	//------------------------------------------------
	// 아이템 지도 관련 
	//------------------------------------------------
	 void SetItemMap(SI32 x, SI32 y, UI16 itemunique, UI16 itemlistindex);
	 cltItemMapInfo* GetItemMap(SI32 x, SI32 y);
	
	//-----------------------------------------
	// 마을
	//-----------------------------------------
	void AttachVillage();			// 마을을 지도에 붙인다.
	void DeattachVillage();			// 마을을 지도에서 지운다.

	void DeattachGate();			// 게이트를 지도에서 지운다.

	//---------------------------------
	// 빈공간 찾기 
	//---------------------------------
	// 캐릭터를 위치시키기 위한 orgx, orgy 주변의 빈 공간을 찾는다. 
	bool FindEmptyAreaForCharLocate(SI32 kind,  SI32 orgx, SI32 orgy, SI32 range, SI32 *pdestx, SI32 *pdesty, bool bcharswitch);
    bool	IsEmpty(SI32 x, SI32 y);				// 지상이 비어 있는가? 지상 캐릭터가 있는곳, 물, 일반 장애물
    BOOL	IsEmptyWater(SHORT x, SHORT y);			// 지상이 비어 있는가? 지상 캐릭터가 있는곳, 물, 일반 장애물
    BOOL	IsEmpty(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT idx, SHORT idy); // 지상이 비어 있는가? 지상 캐릭터가 있는곳, 물, 일반 장애물
    BOOL	IsEmptyWater(SHORT x, SHORT y, SHORT xsize, SHORT ysize, SHORT idx, SHORT idy);	// 비어 있는가?

	SI32	GetMouseTileAtb(SI32 x, SI32 y);			// 특정 좌표의 타일 정보를 얻어온다. 
	SI32	GetUserNum(SI32 x, SI32 y);

	DWORD	GetInstanceMapMaxTime();
};


//-----------------------------------------------
// cltMapServer
//-----------------------------------------------

#include "../NetworkLib/ITZNetLib/ITZWorld.h"
class cltMapServer : public cltMapCommon {
public:

	cltServerMap* pclServerMap;

	cltMapServer ( ITZWorld< ITZMap > *pWorld, UI32 mapindex, SI32 mapunique, SHORT xsize, SHORT ysize,  SI32 tileset1, SI32 tileset2, cltCharManager* pclcm, cltTileManager* pcltileset, cltFieldObjectManager* pclfieldobjectmanager);
	~cltMapServer ();

	
};


//-----------------------------------------------
// cltMapClient
//-----------------------------------------------
class cltMapClient : public cltMapCommon {
public:

	//------------------------------------
	// 물 방울
	//------------------------------------
	CWaterDropMgr	*pclWaterDrop;

	//------------------------------------
	// 반딧불
	//------------------------------------
	CFireFlyMgr		*pclFireFly;
	
	//-----------------------------------
	// 얼음성 반짝이
	//-----------------------------------
	CIceFlashMgr	*pclIceFlash;

	//-----------------------------------
	// 작은풀숩 홀씨
	//-----------------------------------
	CSeedDropMgr	*pclSeedDrop;

	//------------------------------------
	// 이동 표시 
	//------------------------------------
	_MoveMark*				pclMoveMark;

	//-----------------------------------
	// 화면의 중심 
	//-----------------------------------
	int						VirtualMapX, VirtualMapY;
	SI32					siOldVirtualMapX, siOldVirtualMapY;

	bool					bMapDrawSwitch;						// 지도를 그리느냐 마느냐

	bool					bCollDrawSwitch;					// 충돌 지역을 표시하는지 여부를 알려주는 변수. 

	// 이펙트 관련
#ifdef _SAFE_MEMORY
	NSafeTArray<cltEffectMapInfo, MAX_EFFECT_PER_MAP>		pclEffectMap;
#else
	cltEffectMapInfo		pclEffectMap[MAX_EFFECT_PER_MAP];
#endif
	SI32					CurrentEffect;

	UI08					* uiFogMap;

public:

	cltMapClient();
	cltMapClient(SI32 mapindex, SI32 mapunique, SHORT xsize, SHORT ysize, SI32 tileset1, SI32 tileset2, cltCharManager* pclCM, cltTileManager* pcltileset, cltFieldObjectManager* pclfieldobjectmanager);
	~cltMapClient();

	//-------------------------------------------
	// 지도를 최종적으로 사용가능하게 만드는 함수. 
	//--------------------------------------------
	void MakeLastMap();

	//------------------------------------
	// 영역 확인 
	//------------------------------------
	// x, y좌표가 화면안에 있는지 확인한다.
    BOOL	IsInScreen(SHORT x, SHORT y);
	// x, y 타일 좌표가 화면안에 있는지 확인한다.
	BOOL IsInScreenTile(SI32 x, SI32 y);


	//------------------------------------
	// 화면 중심 타일 좌표 확인 
	//------------------------------------
	SHORT	GetVirtualMapX();
	SHORT	GetVirtualMapY();


	//------------------------------------
	// 출력 
	//------------------------------------
    void SetMapDrawSwitch(bool mode);

	//----------------------------------------------
	// 포그(안개)
	//----------------------------------------------
	bool DrawFog(LPDIRECTDRAWSURFACE7 lpsurface , cltClient * pclclient);
	// 한개의 FOG_BLACK을 그린다. 
	BOOL DrawFogBlack(cltClient* pclclient, SI32 x, SI32 y, SI32 centerx, SI32 centery);

	UI32 GetFogMap(SI32 x, SI32 y);
	
	void SetFogMap(SI32 x, SI32 y, UI32 data);
	
	// 포그 처리. 
	void FogRecoverAction();
	UI08 GetFogData(SI32 x, SI32 y, SI32 fog);
	// 특정 타일에 포그 관련 작업을 수행한다. 
	void PutFog(SI32 x, SI32 y, SI32 fog);
	// 특정 위치의 포그 정보를 계산한다. 
	bool CalcFogData(SI32 x, SI32 y, SI32* pfont);

	//-----------------------------------
	// 좌표 변환 (화면 관련 ) 
	//-----------------------------------
    // 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (화면좌표)
    BOOL	GetCenterXYByMapCenterXY(SHORT x, SHORT y, SHORT & centerx, SHORT & centery);
	

	// 성곽등을 고려한 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (절대좌표)
	// 기준 좌표 
	// 지도상의 X, Y좌표의 중심의 좌표를 돌려준다. (화면 좌표)
    BOOL	GetCenterXY(SHORT X, SHORT Y, SHORT & centerx, SHORT & centery);
    BOOL	GetRefXY(SHORT X, SHORT Y, SHORT & centerx, SHORT & centery);
	
	
    // 절대 좌표를 화면 좌표로 변환한다.
    void	TransFromRealDotToScreenDot(SHORT realx, SHORT realy, SHORT& screenx, SHORT &screeny);
    // 화면 좌표를 절대 좌표로 변환한다.
    void	TransFromScreenToRealXY(SHORT screenx, SHORT screeny, SHORT& realx, SHORT &realy);


	BOOL	IncreaseVirtualMapX(int mount);
	BOOL	DecreaseVirtualMapX(int mount);
	BOOL	IncreaseVirtualMapY(int mount);
	BOOL	DecreaseVirtualMapY(int mount);


	// mousex, mousey가 클라이언트 영역에 있는가?
    BOOL	IsPtInMap(SHORT mouse_x, SHORT mouse_y);


//-----------------------------
// 그리기 함수 
//-----------------------------

	// 출력해야 할 중심 위치를 구한다. 
	void GetDrawCenterXY(SI32 x, SI32 y, SI32* pcenterx, SI32* pcentery);

	// 캐릭터 이름을 표시한다. 
	void DrawCharName();

	// 지도를 그린다. 
    BOOL DrawMap();
	// 북쪽으로 스크롤. 
	void Scroll_North(cltClient* pclclient);
	// 남쪽으로 스크롤. 
	void Scroll_South(cltClient* pclclient);
	// 동쪽으로 스크롤. 
	void Scroll_East(cltClient* pclclient);
	// 서쪽으로 스크롤. 
	void Scroll_West(cltClient* pclclient);
	// 북동쪽으로 스크롤. 
	void Scroll_NorthEast(cltClient* pclclient);
	// 북서쪽으로 스크롤.
	void Scroll_NorthWest(cltClient* pclclient);
	// 남동쪽으로 스크롤.
	void Scroll_SouthEast(cltClient* pclclient);
	// 님서쪽으로 스크롤.
	void Scroll_SouthWest(cltClient* pclclient);





    BOOL DrawObj();
	// 지도의 충돌 영역을 표시한다. 
	void DrawMapCollInfo();
	// 현재 마우스가 위치한 타일의 위치를 표시한다. 
	void DrawCurrentTilePos(SI32 mapx, SI32 mapy);
	

	// 땅에 떨어진 아이템을 그린다.		
	void DrawLandItem();	
	// 캐릭터를 지도에 그린다. 
	void DrawCharInMap(SI32 x, SI32 y);


	// 지형 정보 데이터를 설정한다. 
	void SetGeoInfo(HDC hdc);

	// 지형 정보 데이터를 보여준다. 
	void DrawGeoInfo(LPDIRECTDRAWSURFACE7 lpsurface, cltHelpManager* pclhelpmanager);


	// Field Object 를 그린다. 
	void DrawFieldObject(SHORT mapx, SHORT mapy, SHORT cx, SHORT cy, SI32 fieldobjectunique, SI32 siForceTransMode = NULL);
	
	// 불을 그려준다. 
	void DrawFire(SHORT mapx, SHORT mapy, SHORT cx, SHORT cy, SI32 file);


	// 지도 요소를 그리는 함수들 
    // 바닥을 그린다. 
    BOOL DrawLayer(SHORT centerx, SHORT centery, SI32 layer, SHORT x, SHORT y);

	// x, y:2배수좌표 
    BOOL DrawMousePos(SHORT centerx, SHORT centery, SHORT x, SHORT y, BYTE * table, SHORT mode);

	void DrawTile(SI32 ref, SI32 file, SI32 font, SI32 x, SI32 y, SI32 startx, SI32 starty);


////////////////////////////////////////////////////////////////////
/// 지도를 저장하거나 불러온다. 
///////////////////////////////////////////////////////////////////
	
	BOOL MapSaveAsPCX(LPDIRECTDRAWSURFACE7 pSurface, int xsize, int ysize);		// 지도를  캡춰한다. 
	BOOL MapSaveAsBMP(LPDIRECTDRAWSURFACE7 pSurface, int xsize, int ysize);
 	
	BOOL IsLand(SHORT x, SHORT y);		// x, y기본 좌표 
	BOOL IsSea(SHORT x, SHORT y);		// x, y기본 좌표 		

	void SetVirtualPos(SI16 siX, SI16 siY);

	//-------------------------------------------------
	// 지도 캡쳐 관련 
	// Map-Capture.cpp
	//-------------------------------------------------
	BOOL MapCapture();
	static BOOL CALLBACK StaticMapCaptureDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK MapCaptureDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	//----------------------------------------
	// 작은 지도 관련 
	//----------------------------------------
	BOOL MakeSmallMap(SI32 smallmapmode, TCHAR* SmallMapFileName);
	
	
	//---------------------------------------
	// 지도 조작 관련 
	//---------------------------------------
	void ScrollOperation();

	
	void PlayBackEffect();		// 배경 효과음을 연주한다. 
	void MakeRareItemSmoke(bool breverseswitch, SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI32 rare,
						   bool horse = false );		// 명품 아이템에게 표시한다.

	
	void PopEffect();			// 이펙트 관련
	void PushEffect(cltEffectMapInfo * pclinfo);

};

// 화일이름을 갖고 지도의 정보를 얻어온다. 
BOOL GetMapInfo(TCHAR* filename, cltMapHeader* header);

bool IsMoveColl(int x, int y, int SrcX, int SrcY, void* thisptr);


// 출력물의 종류. 
#define DRAWOBJ_TYPE_FIELDOBJ	1
#define DRAWOBJ_TYPE_CHAR		2

class cltFieldObjDrawData{
public:
	SI32 x, y;
	SI32 cx, cy;
	SI32 founique;
	SI32 type;

	void Set(cltFieldObjDrawData* pcldata);
};

class cltDrawObjData{
public:
	SI32 siType;			// 어떤 종류의 출력물인가.?

	union {
		SI32				siID;				// DRAWOBJ_TYPE_FIELDOBJ인 경우, 
		cltFieldObjDrawData clFODrawData;		// DRAWOBJ_TYPE_CHAR인 경우, 
	};

	cltDrawObjData();

	cltDrawObjData(SI32 type, SI32 id);
	cltDrawObjData(SI32 type, cltFieldObjDrawData* pcldata);

};


#endif
