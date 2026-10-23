//=======================================================================================================================
// 제목: 야외지역 전체맵
// 스위치: x
// 내용: 전체맵을 한눈에 볼 수 있도록 하고, 원하는 마을, 사냥터, 엔피시, 기타 등의 위치를 말풍선 및 색깔로 표시해준다.
// 작성일: 2009.09.28
// 작성자: 오지연
//========================================================================================================================
#pragma once

#include "../Client/Client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "MsgType-System.h"


// 마을, 사냥터, NPC, ETC 최대 100으로 설정함.
#define MAINMAP_OBJECT_TOWN_MAX			100	
#define MAINMAP_OBJECT_HUNTINGZONE_MAX	100
#define MAINMAP_OBJECT_NPC_MAX			100
#define MAINMAP_OBJECT_ETC_MAX			100

#define MAINMAP_OBEJCTTYPE_TOWN			0
#define MAINMAP_OBEJCTTYPE_HUNTINGZONE	1
#define MAINMAP_OBEJCTTYPE_NPC			2
#define MAINMAP_OBEJCTTYPE_ETC			3

#define DEFAULT_MAP_X_SIZE				100
#define DEFAULT_MAP_Y_SIZE				100

#define MAX_MAINMAP_OBJECTTYPE_NUM		4


class CMainMapDlg: public CDialogController
{
public:
	struct _MapObjectInfo
	{
		SI32 siMapObjectType;
		SI32 siPosX;
		SI32 siPosY;
		SI32 siTextUnique;
	};

public:
	_MapObjectInfo		m_arTown[MAINMAP_OBJECT_TOWN_MAX];
	_MapObjectInfo		m_arHuntingZone[MAINMAP_OBJECT_HUNTINGZONE_MAX];
	_MapObjectInfo		m_arNPC[MAINMAP_OBJECT_NPC_MAX];
	_MapObjectInfo		m_arETC[MAINMAP_OBJECT_ETC_MAX];

	SI32				m_siCurrentTownPos;
	SI32				m_siCurrentHuntingZonePos;
	SI32				m_siCurrentNPCPos;
	SI32				m_siCurrentETCPos;

	bool				m_siCheckIndex[MAX_MAINMAP_OBJECTTYPE_NUM];			// [종호] 체크박스들의 체크상태

public:
	CMainMapDlg();
	~CMainMapDlg();

	void init();

	void Create();

	void UpdateState();

	void LoadMapInfo_FromFile();


	static void CALLBACK StaticCallBackDialogCMainMap(CControllerMgr *pControllerMgr, UINT nEvent, int nControllID, CControllerObj *pControl );
		   void CALLBACK CallBackDialogCMainMap( UINT nEvent, int nControlID, CControllerObj* pControl );

public:
	void Process_ComboBox_Village();
	void Process_ComboBox_HuntingZone();
	void Process_ComboBox_NPC();
	void Process_ComboBox_ETC();

	void Process_Dlg_MouseMove();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );
	void DrawBalloon(CStatic* pBalloon, TCHAR* pText, POINT& point);

	bool IsInRect(RECT& rect, POINT& point);

	POINT ConvertMinimapPos(const POINT& rResourcePos);

	bool IsChecked(SI32 siIndex);


	// 축소지도 상에서 타일 x, y 좌표에 해당하는 좌표를 구해준다.

public:
	InterfaceFactory	m_InterfaceFactory;

private:
	TSpr				m_sprImageList;

	CImageStatic*		m_pMapImage;

	CStatic*			m_pTownBalloon;
	CStatic*			m_pHuntBalloon;
	CStatic*			m_pNPCBalloon;
	CStatic*			m_pEtcBalloon;
	CStatic*			m_pMouseBalloon;

	// 미니맵 좌표/스케일 관련
private:
	SI32				m_siRealMapXSize;
	SI32				m_siRealMapYSize;
	SI32				m_siCurrentMapWidth;
	SI32				m_siCurrentMapHeight;
};

// NCarryOverGachaDlg.cpp
// NShowGetItemDlg.cpp
