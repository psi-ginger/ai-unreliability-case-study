//-----------------------------------------------------------------------------
// 2005/09/27 이제완
//-----------------------------------------------------------------------------

#ifndef _TILESET_H
#define _TILESET_H

#include "ndatatypes.h"
#include "TSpr.h"
#include "../../../../NLib/NArray.h"

#define MAX_TILESET_NUMBER			255
#define MAX_CHILD_TILE_SET_NUMBER	20				// 가질 수 있는 최대 자식 타일의 수. 
#define MAX_TILE_PER_TILESET		100				// 한 타일 세트 마다 가질 수 있는 최대의 타일 이미지파일 수.	
#define MAX_TILE_UNIQUE_NUMBER		1000			// 타일 유니크는 이수 범위를 넘어갈 수 없다. 


#define TILEATB_BASE			((DWORD)1)			// 기본 타일. 다른 타일과의 접속 영역이 파일 안에 들어있지 않다. 
#define TILEATB_SUB				((DWORD)1 << 1)		// 서브 타일. 부모 타일과의 접속 영역이 파일 안에 들었있다.

#define TILEATB_NODIR			((DWORD)1 << 2)
#define TILEATB_8DIR			((DWORD)1 << 4)

#define TILEATB_LAYER1			((DWORD)1 << 5)		// 바닥에 찍는 타일 (스프라이트가 아님) 
#define TILEATB_LAYER2			((DWORD)1 << 6)		// 바닥 위에 찍는 타일 (스프라이트)

#define TILEATB_1X				((DWORD)1 << 7)		// 1배수 타일
#define TILEATB_2X				((DWORD)1 << 8)		// 2배수 타일
#define TILEATB_4X				((DWORD)1 << 9)		// 4배수 타일

#define TILEATB_BLOCK			((DWORD)1 << 10)	// 충돌지역. 
#define TILEATB_WATER			((DWORD)1 << 11)	// 물지역. 
#define TILEATB_DEEPWATER		((DWORD)1 << 12)	// 심해지역. 

//#define TILEATB_XSPR			((DWORD)1 << 13)	// xspr파일 형식의 타일인가

// 타일용 방위 
#define MAPTILE_SOUTH			((DWORD)1)
#define MAPTILE_WEST			((DWORD)1 << 1)
#define MAPTILE_NORTH			((DWORD)1 << 2)
#define MAPTILE_EAST			((DWORD)1 << 3)

class XSpr;

// 타일세트의 정보를 처리하는 클래스. 
class cltTileInfo
{
public:
	cltTileInfo(SHORT unique, DWORD dwatb, UI08 color, DWORD dwimagefilenumber, cltTileInfo* pparent, TCHAR* szname);
	~cltTileInfo();

	SI32			FreeAllSpr();								// 보유한 모든 타일 세트를 메모리 해제한다
	BOOL			AddChild(cltTileInfo* pparent);				// 자식 타일 세트를 추가한다
	BOOL			ReadTileSetArrayInfo(TCHAR* szfilename);		// 타일 배열 정보를 읽어온다


public:
	SHORT			Unique;								// 각 타일 세트 마다의 고유 번호. 
	DWORD			dwAtb;								// 타일 속성의 특성. 
	TCHAR			szName[MAX_PATH];					// 타일 세트의 이름. 
	cltTileInfo*	pParent;							// 부모 타일 세트의 포인터. (부모 타일은 한 가지 밖에 없다.) 
#ifdef _SAFE_MEMORY
	NSafeTArray<cltTileInfo*, MAX_CHILD_TILE_SET_NUMBER>	pChild;
#else
	cltTileInfo*	pChild[MAX_CHILD_TILE_SET_NUMBER];	// 자식 타일 세트의 포인터들. ( 자식 타일은 여러개가 될 수 있다.) 	
#endif
	DWORD			dwChildNumber;						// 자식 타일의 종류 갯수. 
	UI08			uiColor;							// 맵 에디터에서 사용될 대표색상. 

	DWORD			TileImageFileNumber ;				// 타일 이미지 파일의 갯수. 
#ifdef _SAFE_MEMORY
	NSafeTArray<TSpr*, MAX_TILE_PER_TILESET>		pSpr;
#else
	TSpr*			pSpr[MAX_TILE_PER_TILESET];			// 타일 이미지에 대한 포인터. 
#endif
	BYTE*	pTileSetArrayInfo;					// 타일이 배열되어 있는 규칙성을 정의한 포인터

};


class cltTileManager
{
public:
	cltTileManager(const TCHAR* szsavepath);
	~cltTileManager();


public:
	BOOL			InitTileSet();
	BOOL			DeleteTileSet();	
	bool			FreeTileSet();								// 모든 타일 세트를 메모리에서 해제한다. 

	//------------------------------------
	// 타일 세트 불러오기 
	//------------------------------------
	BOOL			LoadTileSet(SI32 sibasetilesetunique);		// 현재 설정에 맞게 타일 세트를 읽어온다. 	
	bool			LoadTileSet(SI32 tileset1, SI32 tileset2);	// 지도에 필요한 타일세트를 전부 로딩한다. 

	SI32			FindRefFromUnique_TileSet(SI32 siunique);	// 유니크 값을 근거로 레퍼런스를 찾는다. 
	cltTileInfo*	GetTileInfoPointer(TCHAR* szname);			// 식별자를 사용하여 타일인포의 포인터를 구한다. 

public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_TILE_UNIQUE_NUMBER>		siTileRef;
#else
	SI16			siTileRef[MAX_TILE_UNIQUE_NUMBER];
#endif
	SI32			siLoadedTileSetImageSize;					// 타일 세트로 불려온 타일의 바이트 수
																// 현재 얼마나 많은 메모리가 타일세트로 할당되어 있는지 알 수 있다.

	TCHAR			szSavePath[MAX_PATH];						// 파일을 읽어들일 경로를 저장한다. 
#ifdef _SAFE_MEMORY
	NSafeTArray<cltTileInfo*, MAX_TILESET_NUMBER>		pclTileInfo;
#else
	cltTileInfo*	pclTileInfo[MAX_TILESET_NUMBER];
#endif
};


#endif
