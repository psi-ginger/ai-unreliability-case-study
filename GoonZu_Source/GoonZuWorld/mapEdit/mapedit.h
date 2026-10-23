//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _MAPEDIT_H
#define _MAPEDIT_H

#include "..\Client\Client.h"

#define MAPEDIT_MODE_TERRAIN            0
#define MAPEDIT_MODE_FIELDOBJET         1
#define MAPEDIT_MODE_FIELDOBJETBRUSH    2
#define MAPEDIT_MODE_GEOINFO			3
#define MAPEDIT_MODE_CUSTOMCOLL			4

#define FOSURFACE_XSIZE					800
#define FOSURFACE_YSIZE					600

#define FIELDOBJECT_COLL_MAP_XSIZE		16
#define FIELDOBJECT_COLL_MAP_YSIZE		16

#define MAX_COLLISION_BRUSH_NUM			10

#include "..\Common\Map\Map.h"


class cltFieldObjectTypeInfo;

class cltMapEdit : public cltClient {
public:

	SI32 CurrentMode ;							
	SI32 CurrentTerrainUnique;				// 현재 선택된 Terrain Unique
	SI32 CurrentFieldObjectUnique;			// 현재 선택된 Field Object Unique
	SI32 CurrentFieldObjectBrush;			// 현재 선택된 Field Object Brush
	SI32 SelectedGeoInfoSlot;				// 선택된 지형 정보 

	SI32 CurrentCustomCollision;			// 현재 선택된 임의충돌 설정 
	SI32 SelectedFieldObjectUnique;			// 현재 선택된 Field Object Unique

	UI08 uiCollisionBrush;					// 충돌 지역 설정시 사용하는 브러쉬 크기 uiCollisionBrush x uiCollisionBrush

	OPENFILENAME MapFileOpen;				// 지도 화일용 

	TCHAR MapFileName[256];
	TCHAR MapFileShortName[128];

	SHORT PutSeaMode;
	SHORT PutWaterMode;

	HWND					hMapEditDlg;
	HWND					hGeoInfoDlg;
	HWND					hDeleteUniqueDlg;

	//------------------------------------
	// 경로 저장변수 
	//------------------------------------
	TCHAR					CurrentDirectory[MAX_PATH];

	//-------------------------------------
	// FieldObject 표시용 Surface
	//--------------------------------------
	LPDIRECTDRAWSURFACE7 pFOSurface;
	LPDIRECTDRAWSURFACE7 pFOSurface2;

	SI32 FieldObjectCollMap[FIELDOBJECT_COLL_MAP_XSIZE][FIELDOBJECT_COLL_MAP_YSIZE];

	TSpr FloorSpr;
	TSpr m_pNewMapPCXSpr;

	SI32 FoTileSize;


	cltMapEdit( TCHAR* appname, HWND hwnd , HINSTANCE hinst, SI32 gamemode, SI32 defaultservicearea, SI32 runmode );
	~cltMapEdit();
	
	void WinSockAction(){};
	
	void MapEditAction();

	void Create();

	//-----------------------------------------
	// 출력 관련 
	//-----------------------------------------

	void PopMapFileInitializeMap(HWND hwnd);
	void PopMapFileInitializeSpr(HWND hwnd);
	BOOL PopMapFileOpenDlg(HWND hwnd, TCHAR* pstrFileName, TCHAR* pstrTitleName);
	BOOL PopMapFileSaveDlg(HWND hwnd, TCHAR* pstrFileName, TCHAR* pstrTitleName);


	//-------------------------------------------
	//	트리 구조 관련 
	//-------------------------------------------
	void InsertTree_Terrain(HWND hDlg, HTREEITEM pNode, cltTileInfo* pclParentTileInfo);
	void InsertTree_FieldObject(HWND hDlg, HTREEITEM pNode, cltFieldObjectTypeInfo* pclParentInfo);

	// 트리구조에  추가한다.
	HTREEITEM TreeViewInsert(HWND hwnd, TCHAR* name, SI32 para, HTREEITEM node);


	//-----------------------------------------
	// 리스트 박스 
	//-----------------------------------------
	void InsertFieldObjectBrush(HWND hDlg);


	//-------------------------------------------
	// 맵 에디터 각종 인터페이스 생성 관련 
	//-------------------------------------------
	void CreateNewMapTool();
	static BOOL CALLBACK StaticNewMapDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK NewMapDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	//------------------------------------------
	// 지형 안내 설정 툴 
	//------------------------------------------
	void CreateGeoInfoTool();
	static BOOL CALLBACK StaticGeoInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK GeoInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	// 지형 정보 리스트 박스에 값을 설정한다. 
	void SetGeoInfoListBox(HWND hDlg);

	//------------------------------------------
	// 해당 유니크 삭제 툴
	//------------------------------------------
	void DeleteUniqueTool();
	static BOOL CALLBACK StaticDeleteUniqueDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK DeleteUniqueDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);




	//------------------------------------------
	// Terrain Select Tool
	//------------------------------------------
	void OpenMapEditTerrainTool();
	static BOOL CALLBACK StaticSelectTerrainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK SelectTerrainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	
	// FOShape를 조절하기 위한 컨트롤을 설정한다. 
	void InitFOShape(HWND hDlg);

	// Field Object 표시기의 정보를 설정한다. 
	void SetFOShape(HWND hDlg, SI32 founique);

	// Field Object를 FOSurface에 옮겨그린다. 
	void DrawFOShape(HWND hDlg, SI32 founique);

	//Field Object 정보를 저장한다.
	bool SaveFieldObjectInfo(SI32 founique);

	void DrawFloorTile(SI32 startx, SI32 starty, SI32 xsize, SI32 ysize, UI08 color);


	void OpenMapEditSaveTool();
	void OpenMapEditLoadTool();


	//----------------------------------
	// MapEdit-HeaderManager.cpp
	//----------------------------------
	// 맵 헤더 수정용 대화 상화를 보여준다.
	BOOL OpenMapEditHeaderManager();
	static BOOL CALLBACK StaticHeaderManagerDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK HeaderManagerDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


	//----------------------------------
	// MapEdit-CustomCollision.cpp
	//----------------------------------
	// 임의 충돌체크 설정대화상자 
	BOOL OpenCustomCollision();
	static BOOL CALLBACK StaticCustomCollisionDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK CustomCollisionDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	//-----------------------------------
	// 모다리스 대화 상자 처리 관련 
	//-----------------------------------
	BOOL ModalessDlgProc(MSG* pmsg);

	//-----------------------------------
	// WM_COMMAND메시지 처리 함수. 
	//-----------------------------------
	bool DoMsg_WM_COMMAND(WPARAM wParam);

	
};

#endif