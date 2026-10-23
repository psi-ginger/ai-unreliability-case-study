//-----------------------------------------------------------------------------
// 2005/09/26 이제완
//-----------------------------------------------------------------------------

#ifndef _MAPTYPEMANAGER_H
#define _MAPTYPEMANAGER_H

#define MAX_MAPTYPE_NUMBER	200										// 지도 타입의 최대 개수. 

#define MAPTYPE_ATB_VILLAGE_SPRING			(DWORD)1				// 봄 마을
#define MAPTYPE_ATB_VILLAGE_SUMMER			((DWORD)1 << 1)			// 여름 마을
#define MAPTYPE_ATB_VILLAGE_AUTUMN			((DWORD)1 << 2)			// 가을 마을
#define MAPTYPE_ATB_VILLAGE_WINTER			((DWORD)1 << 3)			// 겨울 마을
#define MAPTYPE_ATB_VILLAGE_PORT			((DWORD)1 << 4)			// 항구 마을 
#define MAPTYPE_ATB_HUNT					((DWORD)1 << 5)			// 수렵장 맵
 


#define MAPTYPE_STRUCTURE_POINT_X_SLOT0	9
#define MAPTYPE_STRUCTURE_POINT_Y_SLOT0	42
#define MAPTYPE_STRUCTURE_POINT_X_SLOT1	36
#define MAPTYPE_STRUCTURE_POINT_Y_SLOT1	52
#define MAPTYPE_STRUCTURE_POINT_X_SLOT2	46
#define MAPTYPE_STRUCTURE_POINT_Y_SLOT2	52
#define MAPTYPE_STRUCTURE_POINT_X_SLOT3	24
#define MAPTYPE_STRUCTURE_POINT_Y_SLOT3	40
#define MAPTYPE_STRUCTURE_POINT_X_SLOT4	6
#define MAPTYPE_STRUCTURE_POINT_Y_SLOT4	26
#define MAPTYPE_STRUCTURE_POINT_X_SLOT5	34
#define MAPTYPE_STRUCTURE_POINT_Y_SLOT5	40
#define MAPTYPE_STRUCTURE_POINT_X_SLOT6	46
#define MAPTYPE_STRUCTURE_POINT_Y_SLOT6	26
#define MAPTYPE_STRUCTURE_POINT_X_SLOT7	49
#define MAPTYPE_STRUCTURE_POINT_Y_SLOT7	42
#define MAPTYPE_STRUCTURE_POINT_X_SLOT8	22
#define MAPTYPE_STRUCTURE_POINT_Y_SLOT8	52
#define	MAPTYPE_STRUCTURE_POINT_X_SLOT9	9
#define	MAPTYPE_STRUCTURE_POINT_Y_SLOT9 52

//-----------------------------------------------------------------------------
// 맵 타입 정보. 
//-----------------------------------------------------------------------------
class cltMapTypeInfo
{
public:
	cltMapTypeInfo();
	~cltMapTypeInfo();

	void	SetMapType( SI32 mapunique, TCHAR* pname, 
						SI32 centerx, SI32 centery, SI32 gatex, SI32 gatey, 
						POINT* ppoint,
						SI32 atb, SI32 adapt, SI32 level, GMONEY money,SI32 huntadapt );

public:
	SI32	siMapUnique;
	TCHAR	szMapTypeName[ 64 ];

	SI32	siCenterX;
	SI32	siCenterY;

	SI32	siGateX;	
	SI32	siGateY;								// 이 지도안에서의 메인 게이트 위치. 

#ifdef _SAFE_MEMORY
	NSafeTArray<POINT, MAX_CONSTRUCTION_NUM>		StrLocation;
#else
	POINT	StrLocation[ MAX_CONSTRUCTION_NUM ];	// 건물 시작 좌표. 
#endif

	SI32	siAdaptVillageType;						// 이지도가 어느 마을 타입으로 사용될 수 있는가 ?
													// (0이면 마을용으로 사용될 수 없다) 

	SI32	siAdaptHuntType;						// 이 지도가 수렵장 맵으로 사용될 수있는가? 0이면 사용 못한다.

	SI32	siAdaptNeedVillageLevel;				// 마을 지도로 채택하는데 필요한 마을 레벨 
	GMONEY	siAdaptNeedVillageMoney;				// 마을 지도로 채택하는데 필요한 마을 자금. 
	SI32	siAtb;

};

//-----------------------------------------------------------------------------
// 맵 타입 정보 관리자
//-----------------------------------------------------------------------------
class cltMapTypeManager
{
public:
	cltMapTypeManager();
	~cltMapTypeManager();

	void		LoadMapTypeInfo();					// MapTypeInfo를 로딩한다
	void		LoadMapTypeInfo_Village();			// MapTypeInfo_Village를 로딩한다
	void		LoadMapTypeInfo_Hunt();				// MapTypeInfo_Hunt를 로딩한다

	bool		AddMapType( SI32 mapunique, TCHAR* maptypename, 
							SI32 centerx = 0, SI32 centery = 0, 
							SI32 gatex = 0, SI32 gatey = 0, 
							POINT* ppoint = 0, SI32 atb = 0, 
							SI32 adapt = 0, SI32 level = 0, 
							GMONEY money = 0, SI32 huntadapt = 0 );

	bool		SetMapType( SI32 mapunique, TCHAR* maptypename, 
							SI32 centerx = 0, SI32 centery = 0, 
							SI32 gatex = 0, SI32 gatey = 0, 
							POINT* ppoint = 0, SI32 atb = 0, 
							SI32 adapt = 0, SI32 level = 0, 
							GMONEY money = 0, SI32 huntadapt = 0 );

	TCHAR*		GetMapTypeNameFromUnique( SI32 mapunique );

public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltMapTypeInfo*, MAX_MAPTYPE_NUMBER>		pclMapTypeInfo;
#else
	cltMapTypeInfo*		pclMapTypeInfo[ MAX_MAPTYPE_NUMBER ];
#endif

};


#endif