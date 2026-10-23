#ifndef _NHUNT_MAP_INFO_H_
#define _NHUNT_MAP_INFO_H_

#include "../Client/InterfaceMgr/DialogController.h"

#include <Directives.h>
#include <stdio.h>

#include "../../Resource.h"

#include "../Common/Map/MapManager/MapManager.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>
#include "TSpr.h"
#define LEAST_COMMON_MULTILPLE	30

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

const SI32 MAX_MAP_NPCNUMBER = 25;

class CNHuntMapInfoDlg : public CDialogController
{
public:

	CNHuntMapInfoDlg();
	~CNHuntMapInfoDlg();

	void Create(CControllerMgr * pParent);

	static void CALLBACK StaticCallBackDialogNHuntMapInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHuntMapInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

	void Action();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );

	void AddCombo(TCHAR * szMapType,TCHAR * maptypename,SI32 parentmap);
	void ShowListLow(SI32 index,TCHAR * szMapName);

	void ShowMapInfo(TCHAR * szMapName);
	void ShowListMonster(SI32 siKind[],SI32 siLevel[]);

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void AddHuntmaps();
	void ShowMapInfoFromOtherInfo(TCHAR * szMapName);

	void FindHuntMap(TCHAR * szMapName);
	// 검색 기능을 위해
	void ItemFind(TCHAR* pStrname);	// 선택된 아이템 정보를 표시
	void Find(TCHAR* pItemname);
	void AddListItem(TCHAR* findname);
	void Update();

private:

	SI32	m_siCombo;
	SI32	m_siParentMap[MAX_MAP_NUMBER];
	TCHAR	m_szMapTypeName[MAX_MAP_NUMBER][MAX_MAP_NAME_SIZE];
	SI32	m_siComboParentMap;

	SI32	   m_siSprMode; // 0 : 게이트 1: 맵
	SI32	   m_siSelectGateIndex;	// 게이트 순서
	SI32	   m_siSelectMapIndex;	// 리스트에서 순서
	SI32	   m_siSelectMonsterIndex;
	SI32	   m_siMapIndex;
	
	CStatic*   m_pStatic[2];
	CComboBox* m_pComboBox;
	CList*	   m_pList[2];	 // 0: 몬스터 정보 1: 사냥터 하위 메뉴
	CButton*   m_pButton; // 0: 사냥터로 이동 1: 닫기 버튼
	CEdit*     m_pEdit;

	TSpr	   m_SprGate;
	TSpr	   m_SprMap;
	// 검색기능
	SI32			m_timerCount;
	CList			*m_pListBox_AutoMake;
	CEdit			*m_pEdit_Name;
	CButton			*m_pButton_Accept;
	CStatic			*m_pStatic_Back;//백그라운드

private:
	NTCHARString64		m_LastInput;
};

#endif _NHUNT_MAP_INFO_H_
