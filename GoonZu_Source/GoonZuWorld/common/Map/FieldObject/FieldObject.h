//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _FIELDOBJECT_H
#define _FIELDOBJECT_H

#include <directives.h>
#include <windowsx.h>

#include "TSpr.h"
#include "../../../../NLib/NArray.h"

class HashStringInt;
class cltFieldObjectManager;

#define MAX_FIELDOBJECTSET_NUMBER			3000
#define MAX_FIELDOBJECTTYPE_NUMBER			200
#define MAX_FIELDOBJECT_PER_BRUSH			20	// 한개 브러시당 가질 수 있는 Field Object 갯수. 
#define MAX_FIELDOBJECTBRUSH_NUMBER			20	// 가질 수 있는 최대 브러시 숫자. 
#define	MAX_FIELD_OBJECT_PER_STRUCTURE_SET	15  // 한개의 StructureSet이 가질 수 있는 FieldObect 숫자. 
#define MAX_VILLAGE_STRUCTURE_NUMBER		20	// 가질 수 있는 최대 Structure의 숫자. (건물 종류의 수.) 
#define MAX_STRUCTURE_NAME_LENGTH			20	// 건물의 이름 길이. 
#define MAX_FOUNIQUE						10000	// FieldObject 의 최대 Unique
#ifdef _CHINA
	#define MAX_FOSPR_NUMBER				500	// FieldObject를 한번에 로딩할 수 있는 최대 개수. 
#else
	#define MAX_FOSPR_NUMBER				100
#endif

//--------------------------------------------
// 필드 오브젝트의 타입. 
//--------------------------------------------
#define FOTYPE_VILLAGE						0
#define FOTYPE_GATE							12			// 문 
/*
#define FOTYPE_TREE							1			// 나무. 			
#define FOTYPE_BAMBOO						2			// 대나무 . 			
#define FOTYPE_FLOWER						3			// Flower
#define FOTYPE_SUNFLOWER					4			// 해바라기
#define FOTYPE_FLOWERTREE					5			// 꽃나무
#define FOTYPE_PINETREE						6			// 소나무
#define FOTYPE_MT							7			// 산 
#define FOTYPE_MTGREEN						8			// 녹색산. 
#define FOTYPE_MTFALL						9			// 가을산 
#define FOTYPE_MTSNOW						10			// 겨울산 
#define FOTYPE_VOLCANO						11			// 화산 

#define FOTYPE_WATERFALL					13			// 폭포 
#define FOTYPE_STATUE						14			// 조각 
#define FOTYPE_BUILD						15			// 건물 
#define FOTYPE_VILLAGE1OBJ					16			// 마을1 구조물 
#define FOTYPE_SEAPALACE					17			// 용궁 1층 구조물 
#define FOTYPE_GOBLINHOUSE1					18			// 깨비집 1층. 
#define FOTYPE_NPC							19			// NPC
#define FOTYPE_POND							20			// Pond
#define FOTYPE_BRIDGE						21			// Bridge
#define FOTYPE_AROUNDBEAR					22			// 웅녀동굴 주변부. 
#define FOTYPE_AROUNDWOOD					23			// 나무소굴 주변부. 
#define FOTYPE_AROUNDSMALLFOREST			24			// 작은풀숲 주변부. 
#define FOTYPE_AROUNDCASTLE					25			// 남한산성 주변부. 
#define FOTYPE_AROUNDCASTLESUB				26			// 남한산성 주변부. (바닥) 
#define FOTYPE_AROUNDGOBLIN					27			// 깨비집 주변부. 
#define FOTYPE_ARSANTA						28			// 산타 작업장 주변부.
#define FOTYPE_AROUNDMIHWANG				29			// 미황사 주변. 
#define FOTYPE_PRISON						30			// 감옥 
#define FOTYPE_PRISONBOTTOM					31
#define FOTYPE_PRISONWALL					32
#define FOTYPE_WOOD2PART					33			// 화룡성부분. 
#define FOTYPE_AROUNDGUMHA					34			// 금하굴주변부 
#define FOTYPE_PALACE						35			// 경복궁 
#define FOTYPE_PALACEFLOOR					36
#define FOTYPE_FLOOR						37			// 땅에 붙고 캐릭터가 위로 나다니는 오브젝트들
#define FOTYPE_WALL							38			// 벽 구조물
#define FOTYPE_FLAG							39			// 깃발들
#define FOTYPE_FRONTSCREEN					40			// 스크린 모드로 찍는 오브젝트들 - 캐릭터가 뒤로 가면 앞으로 나옴 ex: 연기등
#define FOTYPE_FIELDTILE					41			// 필드오브젝트를 타일처럼 찍는 것 - 못 올라감
#define FOTYPE_FIELDTILESTATUE				42			// 필드오브젝트를 타일처럼 찍는 것 - 구조물 종류
#define FOTYPE_FIELDTILEBOTTOM				43			// 필드오브젝트를 타일처럼 찍는 것 - 무조건 올라감
#define FOTYPE_GYUNGJOO						44			// 필드오브젝트를 타일처럼 찍는 것 - 경주지역에 찍히는 것
#define FOTYPE_SHELLFISH_FLOOR				45			// 태안지역갯벌 조개 - floor
#define FOTYPE_SHELLFISH_STATUE				46			// 태안지역갯벌 조개 - statue
#define FOTYPE_FENCE						47			// 어망들
#define FOTYPE_MTBAECKDOO					48			// 백두산
#define FOTYPE_BEGINNER1					49			// 초보자 사냥터
// 고구려 유적지
#define FOTYPE_GAURI_BUILD					50			// 건물
#define FOTYPE_GAURI_STATUE					51			// 구조물
#define FOTYPE_GAURI_FLOOR					52			// 바닥에 붙어서 사람 올라가는 것
#define FOTYPE_SEASIDE_FLOOR				53			// 파도,고기등등
#define FOTYPE_AOZI_STATUE					54			// 아오지 탄광
#define FOTYPE_PORTHOLE						55			// 경상우수영
#define FOTYPE_TOPOGRAPHY					56			// 광산,과수원등등
#define FOTYPE_MTSPECIAL					57			// 특정한 산들
#define FOTYPE_TREESHADOW					58			// 나무 그림자
// 생성 던전용
#define FOTYPE_DUNGEON_TILE					59			// 던전 수동타일
#define FOTYPE_DUNGEON_STATUE				60			// 던전 오브젝트들
#define FOTYPE_DUNGEON_WALL					61			// 던전벽들

#define FOTYPE_GUMHA_WALL					62			// 금하
#define FOTYPE_AOZI_WALL					63			// 아오지
#define FOTYPE_HAHOI_WALL					64			// 하회
#define FOTYPE_JEJU_WALL					65			// 제주

#define FOTYPE_HANGWA_WALL					66			// 한과
#define FOTYPE_HANGWA_STATUE				67

#define FOTYPE_PRISON_WALL					68			// 지하감옥
#define FOTYPE_PRISON_STATUE				69

#define FOTYPE_JEJUVILLAGE_WALL				70
#define FOTYPE_JEJUVILLAGE_STATUE			71

#define FOTYPE_ICEFORTRESS_WALL				72			// 얼음성
#define FOTYPE_ICEFORTRESS_STATUE			73

#define	FOTYPE_WOOD1_STATUE					74			// 나무 소굴 1층 타일폴던엔 wood3으로 되어 있음
#define	FOTYPE_WOOD2_STATUE					75			// 나무 소굴 2층 타일폴던엔 wood1으로 되어 있음

#define FOTYPE_BEARCAVE03_WALL				76
#define FOTYPE_BEARCAVE03_STATUE			77

#define FOTYPE_NAMHANMFW_WALL				78
#define FOTYPE_NAMHANMFW_STATUE				79

#define FOTYPE_OSAKA_WALL					81
#define FOTYPE_OSAKA_STATUE					82
#define FOTYPE_OSAKA_FLOOR					83

#define FOTYPE_KANGHWADO_WALL				84
#define FOTYPE_NEWHEAVENS01_WALL			85				// 천상계 1층
#define FOTYPE_NEWHEAVENS01_STATUE			86
#define FOTYPE_DOSUNPALACE_STATUE			87				// 도선궁
#define FOTYPE_PIRATESHIP01_STATUE			88				// 해적선 1층

#define FOTYPE_CHINA_PALACE					89				// 자금성
#define FOTYPE_CHINA_BEGINNER_STATUE		90				// 중국 초보자 벽 및 기둥
#define FOTYPE_CHINA_BEGINNER_FLOOR			91				// 중국 초보자 바닥
#define FOTYPE_CHINA_PALACESHADOW			92				// 자금성 그림자

#define FOTYPE_CHINA_GOVERMENTOFFICE		93				// 관청
#define FOTYPE_CHINA_GUILD					94				// 상단 관리소
#define FOTYPE_CHINA_VILLAGE				95				// 마을 관련 오브젝트
*/

// MAX_FIELDOBJECTTYPE_NUMBER				100

//---------------------------------------------
// 필드 오브젝트 속성 
//---------------------------------------------
// 마을 
// 상위 15비트 : Field Object Unique 
// 상위 1비트  : 공갈비트 
// 하위 16비트 : Village Unique
// 나무 (Tree, Bamboo등이 모두 나무 속성을 갖는다. )
// 상위 15비트 : Field Object Unique 
// 상위 1비트  : 공갈비트 

#define FOATB_VILLAGE					((DWORD)1)
#define FOATB_TREE						((DWORD)1 << 1)
#define FOATB_MOUNTINE					((DWORD)1 << 2)
#define FOATB_GATE						((DWORD)1 << 3)
#define FOATB_STATUE					((DWORD)1 << 4)
#define FOATB_BUILD						((DWORD)1 << 5)
#define FOATB_FLOOR						((DWORD)1 << 6)  // 바닥에 붙어있는 오브젝트. 
#define FOATB_BRIDGE					((DWORD)1 << 7)

// Field Object Type에 대한 정보 
class cltFieldObjectTypeInfo
{
public:
	cltFieldObjectTypeInfo(DWORD dwtype, UI08 color, TCHAR* name, DWORD atb, cltFieldObjectTypeInfo* pclinfo);
	~cltFieldObjectTypeInfo();

	cltFieldObjectTypeInfo* pParent;		// 부모 TypeInfo의 포인터.  

	DWORD		dwType;
	DWORD		dwAtb;						// type의 속성. FOATB_VILLAGE|FOATB_TREE 등.. 
	TCHAR*		pszName;					// Type의 이름. 
	SHORT		Unique;						// 각 오브텍트 마다의 고유 번호. 
	UI08		uiColor;
};

class cltFieldObjectInfo
{
public:
	cltFieldObjectInfo( SHORT unique, DWORD dwtype, DWORD dwimagefilenumber, /*cltFieldObjectInfo* pparent, */
						TCHAR* szname, SI32 transmode,SI32 alphavalue,SI32 shadow,TCHAR* zipname);
	~cltFieldObjectInfo();

	void		Init();

	bool		GetInfoFileName(TCHAR* szfilename);
	
	bool		ReadSetArrayInfo(TCHAR* szfilename);								// 배열 정보를 읽어온다. 
	bool		WriteSetArrayInfo(TCHAR* szfilename);								// 배열 정보를 저장한다. 

	bool		ReadSetArrayInfoFromMemory(BYTE* pMemory);						// 배열 정보를 읽어온다. 

	TSpr*		GetSpr(cltFieldObjectManager* pFOManager, DWORD dwframe);			// 이미지를 담고 있는 공간의 포인터를 리턴한다. 	
	void		SetLastUseFrame(cltFieldObjectManager* pFOManager, DWORD frame);	// 마지막으로 사용된 프레임을 설정한다. 
	void		LoadDetailInfo();

	//BOOL AddChild(cltFieldObjectInfo* pparent);									// 자식 세트를 추가한다. 

public:
	SHORT		Unique;					// 각 오브텍트 마다의 고유 번호. 
	DWORD		dwType;					// 속성의 종류.
	TCHAR*		pszName;				// 세트의 이름. 
	SI32		siTransMode;			// 찍어주는 모드 
	SI32		siAlphaValue;			// 알파값
	SI32		siTreeShadow;			// 나무 그림자들
	SI16		siFOSprRef;				// 이미지가 저장되어 있는 공간에 대한 ref
	SI32		TileXsize;
	SI32		TileYsize;
	SI32		TileCenterX;
	SI32		TileCenterY;
	SI32		IndentX;
	SI32		IndentY;
	SI32		AniFrame;
	SI32		AniDelay;
	bool		bIsMainMap;

	TCHAR*		pszZipName;				// 압축파일 이름. 

};



class cltFieldObjectBrush
{
public:
	cltFieldObjectBrush(TCHAR* pname);
	~cltFieldObjectBrush();

	BOOL		Add(cltFieldObjectInfo* pfieldobjectinfo);		// 브러쉬에 Field Object를 추가한다. 
	SI32		GetRandUnique();								// 가지고 있는 cltFieldObjectInfo중에서 한개를 랜덤하게 고른 후 이의 unique값을 리턴한다. 


public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltFieldObjectInfo*, MAX_FIELDOBJECT_PER_BRUSH>		pFieldObjectInfo;
#else
	cltFieldObjectInfo* pFieldObjectInfo[MAX_FIELDOBJECT_PER_BRUSH];	// 브러쉬가 갖고 있는 필드 오브젝트의 포인터. 
#endif

	SI32		FielObjectNumber;								// Field Object의 개수. 
	TCHAR		szName[MAX_PATH];
};

class cltFieldObjectManager
{
public:
	cltFieldObjectManager(SI32 gamemode);
	~cltFieldObjectManager();


	//--------------------------------------------------------
	// Field Object Type 관련 
	//--------------------------------------------------------

	bool		InitFieldObjectManager();
	bool		InitFieldObjectManager_China();

	
	BOOL		InitTypeSet();									// Field Object Type를 초기화한다. 
	BOOL		InitTypeSet_China();

	BOOL		InitBrush();									// 브러쉬를 설정한다. 
	BOOL		InitSet();
	BOOL		LoadSet(DWORD frame, cltFieldObjectInfo* pclinfo, bool bmainmap = false );		// 현재 설정에 맞게 세트를 읽어온다. 
	SI32		FindRefFromUniqueMinus(SI32 siunique);			// 유니크 값을 근거로 레퍼런스를 찾는다. 

	SI32		AnalyzeAtbString(TCHAR* string);					// 세트의 속성 정보가 담긴 string문자열을 분석하여 문자열로 되어 있는 텍스트를 게임내에서 의미있는 숫자로 변경하여 리턴한다. 
	SI32		GetAtb(SI32 founique);							// FieldObject의 Atb를 찾는다.
	void		DeleteIfNotUse(DWORD curframe);					// 사용되지 않은 Field Object를 메모리에서 삭제한다. 
	SI32		FindUniqueFromName(TCHAR* name);					// 이름을 근거로 Unique를 찾는다. 
	SI32		FindEmptySpr();									// Spr이미지를 저장할 빈 공간을 찾는다. 
	cltFieldObjectInfo* GetInfoPointer(TCHAR* szname);			// 식별자를 사용하여 인포의 포인터를 구한다. 


public:
	SI32					siLoadedSize;						// 메모리에 불려온 데이터의 총 바이트 수. 

#ifdef _SAFE_MEMORY
	NSafeTArray<cltFieldObjectInfo*, MAX_FIELDOBJECTSET_NUMBER>			pclInfo;
	NSafeTArray<cltFieldObjectTypeInfo*, MAX_FIELDOBJECTTYPE_NUMBER>	pclTypeInfo;
	NSafeTArray<cltFieldObjectBrush*, MAX_FIELDOBJECTBRUSH_NUMBER>		pclFieldObjectBrush;

	NSafeTArray<SI16, MAX_FOUNIQUE>			siFORef;

	NSafeTArray<DWORD, MAX_FOSPR_NUMBER>	dwLastUseFrame;	// 마지막으로 사용된 프레임. 
	NSafeTArray<SI16, MAX_FOSPR_NUMBER>		FOInfoUnique;
	NSafeTArray<TSpr, MAX_FOSPR_NUMBER>		FOSpr;			// 이미지에 대한 포인터. 
#else
	cltFieldObjectInfo*		pclInfo[MAX_FIELDOBJECTSET_NUMBER];
	cltFieldObjectTypeInfo* pclTypeInfo[MAX_FIELDOBJECTTYPE_NUMBER];
	cltFieldObjectBrush*	pclFieldObjectBrush[MAX_FIELDOBJECTBRUSH_NUMBER];

	SI16					siFORef[MAX_FOUNIQUE];

	DWORD					dwLastUseFrame[MAX_FOSPR_NUMBER];	// 마지막으로 사용된 프레임. 
	SI16					FOInfoUnique[MAX_FOSPR_NUMBER];
	TSpr					FOSpr[MAX_FOSPR_NUMBER];			// 이미지에 대한 포인터. 
#endif

	HashStringInt*			pclHashTable;

};

//------------------------------------
// 마을에 들어가는 VillageStructure 정보. 
//------------------------------------
class cltVillageStructureUnitInfo
{
public:
	SI32		siX, siY;
	SI32		siFieldObjectArray;
	bool		bKeySwitch;

	cltVillageStructureUnitInfo()
	{
		siX					= 0;
		siY					= 0;
		siFieldObjectArray	= 0;
		bKeySwitch			= false;
	}
};

class cltVillageStructureInfo
{
public:
	cltVillageStructureInfo(SI32 unique, TCHAR* szname, SI32 ranktype);
	~cltVillageStructureInfo();

	bool		IsKeyStructure(SI32 sicenterx, SI32 sicentery);									//sicenterx, sicentery를 중심으로 삼고 있는 건물이 건물 세트의 키가 되는 건물인지 확인한다. 
	BOOL		Add(SI32 six, SI32 siy, SI32 sifieldobjectunique, bool bkeyswitch = false);		// FieldObject를 StructureInfo에 추가한다. 	
	BOOL		AddUP(SI32 six, SI32 siy, SI32 sifieldobjectunique, bool bkeyswitch = false);	// UPFieldObject를 StructureInfo에 추가한다. 
	void		SetConstructionInfo( SI32 needVillageLevel, GMONEY needMoney, BOOL destroy, SI32 siNeedVillageScore = 0 );
	SI32		GetNeedVillagelevel();															// 건설에 필요한 마을 레벨을 구한다. 

	SI32		siUnique;
	SI32		siRankType;
	SI32		siNeedVillageLevelRate;			// 짓기 위해서 필요로하는 마을의 레벨 .
	GMONEY		siNeedMoney;					// 건설에 필요한 자금. 
	BOOL		bDestroyable;					// 해체 가능 여부
	SI32		siNeedVillageScore;				// 필요한 마을 점수.

	TCHAR		szName[MAX_STRUCTURE_NAME_LENGTH];

	SI32		siFieldObjectNumber;		// 가지고 있는 FieldObject의 숫자. 
#ifdef _SAFE_MEMORY
	NSafeTArray<cltVillageStructureUnitInfo, MAX_FIELD_OBJECT_PER_STRUCTURE_SET>	clUnit;
#else
	cltVillageStructureUnitInfo clUnit[MAX_FIELD_OBJECT_PER_STRUCTURE_SET];
#endif

	SI32		siUPFieldObjectNumber;		// 가지고 있는 UPFieldObject의 숫자
#ifdef _SAFE_MEMORY
	NSafeTArray<cltVillageStructureUnitInfo, MAX_FIELD_OBJECT_PER_STRUCTURE_SET>	clUPUnit;
#else
	cltVillageStructureUnitInfo clUPUnit[MAX_FIELD_OBJECT_PER_STRUCTURE_SET];
#endif



};

//-----------------------------------
// VillageStructure 정보의 관리자. 
//-----------------------------------
class cltVillageStructureSet
{
public:
	cltVillageStructureSet();
	~cltVillageStructureSet();

	SI32		FindRefFromUnique(SI32 unique);

	//sicenterx, sicentery를 중심으로 삼고 있는 건물이 strunique건물 세트의 키가 되는 건물인지 확인한다. 
	bool		IsKeyStructure(SI32 sicenterx, SI32 sicentery, SI32 strunique);

	// ranktype을 근거로 건물 이름을 얻어낸다. 
	bool		GetStructureNameFromRankType(SI32 ranktype, TCHAR* pname, SI16 txtSize);

	// 건물 이름을 근거로 ranktype을 얻어앤다.
	SI32		GetRankTypeFromStructureName(TCHAR* pname);
	SI32		GetRankTypeFromStrUnique( SI32 strunique );
	SI32		GetStrUniqueFromRankType( SI32 ranktype );
	SI32		FindRefFromRankType( SI32 ranktype );


public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltVillageStructureInfo*, MAX_VILLAGE_STRUCTURE_NUMBER>		pclVillageStructureInfo;
#else
	cltVillageStructureInfo* pclVillageStructureInfo[MAX_VILLAGE_STRUCTURE_NUMBER];
#endif
};

#endif
