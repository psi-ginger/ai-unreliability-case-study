#ifndef _NMAKECHAR_H_
#define _NMAKECHAR_H_

#include "../../InterfaceMgr/DialogController.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>


class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

#define SELECT_CHAR1 0	// 일본남
#define SELECT_CHAR2 1  // 일본여
#define SELECT_CHAR3 2  // 서양남
#define SELECT_CHAR4 3  // 서양여

class CMakeCharInfo
{
public :
	SI32 siUnique;
	SI32 siKind;
	SI32 siStartIndex;
	SI32 siIndexNum;
};

class CNMakeCharDlg : public CDialogController
{
public:

	CNMakeCharDlg();
	~CNMakeCharDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNMakeCharDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNMakeCharDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );

	void ShowCharMakeWindow(bool bShow, SI16 fontindex = 0 );

	void SelectSlot(SI32 Slot);

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	void Action();

private:
	SI32            m_siChar_flag; // 캐릭터 선택을 위한 플레그
	SI32			m_siSelectedSlot;

	DWORD			m_dwStratClock;
	DWORD			m_dwLastClock;

	bool			m_bShowCharMakeWindow;
		
	CEdit*			m_pEditName;
	CButton*		m_pButton[3]; // 0 : 만들기 1 : 취소 2 : 닫기
	CImageStatic*	m_pStatic[5]; // 0 : 큰 그림 1 : 작은 그림 2 : 견이 3 : 연이 4 : 신이
	
	CStatic*		m_pStaticCharname;

	CButton*		m_pButton_Char[4];
};

#endif

