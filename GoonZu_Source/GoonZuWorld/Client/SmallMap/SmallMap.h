//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _SMALLMAP_H
#define _SMALLMAP_H

#include "..\..\Common\Map\Map.h"

#define SMALLMAPMODE_MAPEDIT		1
#define SMALLMAPMODE_GAME			2


#define SMAP_INFOMODE_VILLAGE		0
#define SMAP_INFO_COLOR				40

class cltSmallMap{

public:
	HWND m_hDlg;								// 전체 축소 지도 다이얼로그 박스의 HWND

	cltMapClient* pclMap;
	SI32 SmallMapMode;

	SI32 CurrentInfoMode;
	LPDIRECTDRAWSURFACE7  pSmallMapSurface;		 // 축소지도의 원본이 담길 공간.
	LPDIRECTDRAWSURFACE7  pSmallMapWorkSurface;  // 축소지도의 변형된 형태(축소등.)가 담길 공간. 
	LPDIRECTDRAWSURFACE7  pSmallMapWorkSurface2; // 화면으로 옮겨질 최종 이미지가 들어 갈 공간. 

	SI32 MapXsize, MapYsize;					// 지도의 가로 세로 타일 크기. 

	SI32 SmallMapXsize, SmallMapYsize;			// 축소 지도 원본의 가로 세로 크기. 
	SI32 CurrentMapXsize, CurrentMapYsize;		// 현재 축소 비율에서의 가로 세로 지도 크기 


	SI32 ParentMapX, ParentMapY;				// 이변수에 설정된 값을 근거로 실제 지도를 움직이게 된다. 

	SI32 DrawStartPosX, DrawStartPosY;			// 화면에 ㅉ찍어주는 시작위치. 
	RECT DrawAreaRect;

	SI32 StandardX, StandardY;					// 축소지도를 표시하는 기준이 되는 지점.
	SI32 OldStandardX,OldStandardY;
	
	SI32 UserPosX, UserPosY;					// 사용자의 타일 좌표 위치. 

	SI32 MouseX, MouseY;
	SI32 DragStartX, DragStartY;

	BOOL LeftPressSwitch;
	BOOL LeftPressDragSwitch;

	BOOL ConnectToMapSwitch;					// 지도와 축소지도를 연동시키는가 ?(이동시 같이 움직임. ) 
	
	SI32 SmallMapRate;							// 축소 비율 (5~10, 10이면 원본과 같은 사이즈다. ) 
	SI32 siOrgSmallMapRate;						// 작은 지도가 만들어진 비율. 

	char SmallMapFileName[MAX_PATH];

	cltSmallMap(SI32 smallmapmode, char* SmallMapFileName,  cltMapClient* pclmap);
	~cltSmallMap();

	
	BOOL CALLBACK SMapDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	BOOL DrawSmallMap(HWND hDlg);

	BOOL IncreaseDrawStartX(SI32 inc);
	BOOL IncreaseDrawStartY(SI32 inc);

	// 축소지도 상에서 x, y도트에 해당하는 타일 좌표를 구해준다. 
	BOOL GetSmallMapTileXYFromCord(SI32 dotx, SI32 doty, SI32* ptilex, SI32* ptiley);

	// 축소지도 상에서 타일 x, y 좌표에 해당하는 좌표를 구해준다. 
	BOOL GetSmallMapCordFromTileXY(SI32 tilex, SI32 tiley, SI32* pdotx, SI32* pdoty);

	BOOL SetScrollPara(HWND hDlg);

	// 지도와의 연동을 계산한다. 
	void CalcMapConnection(HWND hDlg);

	// 원본 맵을 비율에 맞게 축소해서 작업용 서피스에 옮겨 그린다. 
	void ReadyRateMap();

	void UpdateSmallMap();
	void UpdateSmallMap(SI32 x, SI32 y);

	void SetUserPos(SI32 x, SI32 y)
	{
		UserPosX = x;
		UserPosY = y;
	}

	// 텍스트를 출력한다. 
	void DrawText(HDC hdc, SI32 dotx, SI32 doty, char* text, COLORREF rgb);

	// 마크를 출력한다. 
	void DrawMark(SI32 dotx, SI32 doty);


	// 작은 지도상의 정보를 표시한다.
	void DrawSMapInfo(SI32 x, SI32 y, char* text, COLORREF rgb  = RGB(250, 250, 250) );

	DWORD		m_lastTickCount;

};

#endif