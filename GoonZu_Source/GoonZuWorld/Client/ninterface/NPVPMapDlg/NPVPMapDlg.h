#ifndef _NPVPMAP_H_
#define _NPVPMAP_H_

#include "../Client/InterfaceMgr/DialogController.h"

#include <Directives.h>
#include <stdio.h>

#include "../../Resource.h"

#include "../Common/Map/MapManager/MapManager.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>
#include "TSpr.h"
//#define LEAST_COMMON_MULTILPLE	30
//
class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;
//const SI32 MAX_MAP_NPCNUMBER = 25;

#define MAX_PVP_MAPINDEXNUM	100	// 리스트에 최대로 들어가는 맵인덱스
class CNPVPMapDlg : public CDialogController
{
public:

	CNPVPMapDlg();
	~CNPVPMapDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogCNPVPMapDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogCNPVPMapDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )	;	// 선택한 맵정보( 지도를 보여준다 )
	void SetList()	;	// 맵리스트 셋팅
	void LoadMapData()	;	// 맵정보를 읽어 온다.
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )	;
	InterfaceFactory m_InterfaceFactory;

private:
	

	TSpr	m_SprMap;	// 보여줄 맵 그림.
	
	SI32	m_siSprMode;				// 0 : 게이트 1: 맵
	SI32	m_siSelectGateIndex;		// 선택된 게이트 
	SI32	m_siSelectMapIndex;		// 선택된 맵
	SI32	m_siSelectMonsterIndex;
	SI32	m_siMapIndex;


	SI32	m_ListData_MapIndex[MAX_PVP_MAPINDEXNUM]	;	// 리스트 데이터 맵인덱스
};

#endif _NPVPMAP_H_
