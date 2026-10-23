#ifndef _STRSUBBASEDLG_H
#define _STRSUBBASEDLG_H

#include <Directives.h>

//#include "../../Client/client.h"
//#include "../../Commonlogic/StructureClass/StructureClass.h"
//#include "../../InterfaceMgr/DialogController.h"
//#include "../../lib/nutil/nutil.h"
#include "StrBaseDlg.h"

class CEdit;
class CButton;
class CImageStatic;
class CList;
class CComboBox;

class cltCharManager;

enum {
	NSTRSUBBASE_CONTROL		= 10100,
	NSTRSUBBASE_EDIT_EXPLAIN,
	NSTRSUBBASE_IMAGE_PORTRAIT,
	NSTRSUBBASE_BUTTON_CLOSE,
	NSTRSUBBASE_BUTTON_CHIEFTOGGLE,

	NSTRSUBBASE_USER_TAB_BUTTON		= 11050,
	NSTRSUBBASE_USER_TAB_DIALOG		= 11150,
	
	NSTRSUBBASE_CHIEF_TAB_BUTTON		= 12050, 
	NSTRSUBBASE_CHIEF_TAB_DIALOG		= 12150,

	NSTRSUBBASE_CHILD_CONTROL			= 13050,

};

//enum {
//	NSTRSUBBASE_CONTROL		= NSTRBASE_CONTROL + 100,
//	NSTRSUBBASE_EDIT_EXPLAIN,
//	NSTRSUBBASE_IMAGE_PORTRAIT,
//	NSTRSUBBASE_BUTTON_CLOSE,
//	NSTRSUBBASE_BUTTON_CHIEFTOGGLE,
//
//	NSTRSUBBASE_USER_TAB_BUTTON		= NSTRBASE_USER_TAB_BUTTON + 50,
//	NSTRSUBBASE_USER_TAB_DIALOG		= NSTRBASE_USER_TAB_DIALOG + 50,
//	
//	NSTRSUBBASE_CHIEF_TAB_BUTTON		= NSTRBASE_CHIEF_TAB_BUTTON + 50, 
//	NSTRSUBBASE_CHIEF_TAB_DIALOG		= NSTRBASE_CHIEF_TAB_DIALOG + 50,
//
//	NSTRSUBBASE_CHILD_CONTROL			= NSTRBASE_CHILD_CONTROL + 50,
//};

//#define MAX_CHILD_DLG			10
//#define MAX_CHILD_CONTROL		64

class NStrSubTabDlg;

// 건물 다이얼로그 베이스 클래스
class NStrSubBaseDlg : public CDialogController
{
public:
	NStrSubBaseDlg();
	virtual ~NStrSubBaseDlg();

	void	CreateDlg( SI16 id, cltStructureMngBasic *pStrManager, int portrait = 0);
	void	InitDlg();

	void	Action();

	void	SetTabInfo( int row, int currentrow );

	void	AddUserTabDialog( int index, TCHAR *text, int width, NStrSubTabDlg *pChildDlg, TCHAR *filename, TCHAR *szID );
	void	AddChiefTabDialog( int index, TCHAR *text, int width, NStrSubTabDlg *pChildDlg, TCHAR *filename, TCHAR *szID );

	void	SetChiefMode( BOOL bChief );
	void	SetActiveTab( int index );
	void	HideAllTab();

	void	ShowAllTabButton( bool bShow );

	void	SetMainExplain( TCHAR *text );

	virtual void GreetingMsg() = 0;

	static	void CALLBACK StaticNDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	virtual void NDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	virtual void DrawDialogFirst( SI32 ScreenRX, SI32 SreenRY );


public:
	CEdit					*m_pExplainEdit;
	CImageStatic			*m_pPortraitImage;
	CButton					*m_pCloseBtn;
	CButton					*m_pChiefToggleBtn;

	CButton					*m_pUserTabButton[ MAX_CHILD_DLG ];
	NStrSubTabDlg				*m_pUserChildDlg[ MAX_CHILD_DLG ];

	CButton					*m_pChiefTabButton[ MAX_CHILD_DLG ];
	NStrSubTabDlg				*m_pChiefChildDlg[ MAX_CHILD_DLG ];

	RECT					m_ChildRect;

	int						m_row;
	int						m_currentrow;
	int						m_tabX;
	int						m_tabY;
	int						m_tabHeight;

	BOOL					m_bChiefMode;

	DWORD					m_dwActionCount;

	int						m_portraitindex;
	SI32					m_siVillageUnique;	// 마을 번호
	UI08					m_uiUnitSlot;		// 길드 슬롯 번호		

public:
	cltStructureMngBasic	*m_pStrManager;

};


// 탭 다이얼로그 베이스 클래스
class NStrSubTabDlg : public CDialogController
{
public:
	NStrSubTabDlg();
	virtual ~NStrSubTabDlg();

	void CreateTabDlg( int index, NStrSubBaseDlg *pParent, TCHAR *filename, TCHAR *szID, BOOL bChief );
	void SetControlMap( int nControlMapID, TCHAR *controlid );

	virtual void InitDlg() {}
	virtual void Show() { CControllerMgr::Show( true ); }
	virtual void Hide() { CControllerMgr::Show( false ); }
	
	static	void CALLBACK StaticNDlgTabProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	virtual	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj* pControl ) = 0;

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	void				SetMainExplain( TCHAR *pText );

	void				SetEditText( int nControlMapID, TCHAR *text );
	TCHAR*				GetEditText( int nControlMapID );
	
	//_LEON_NINTERFACE_CONTROL_HELPERS
	void	NComboBoxSort(CComboBox* in_pCombo );					// Ninterface ComboBox Sort
	void	NListSort(CList* in_pList, SI16 in_SortColumn = 0 );		// Ninterface ListBox Sort
	//void	NListFindDelete(CList* in_pList, SI16 in_SortColumn);	// Ninterface Delete Item after Finding

public:
	CControllerObj		*m_pChildControl[ MAX_CHILD_CONTROL ];		// 컨트롤
	TCHAR				m_controlid[ MAX_CHILD_CONTROL ][ 32 ];		// file에서 문자열 id
	
	CControllerObj*		m_pControlMap[ MAX_CHILD_CONTROL ];			// 컨트롤 포인터 매핑 
	int					m_nControlID[ MAX_CHILD_CONTROL ];			// 컨트롤 id 매핑
	int					m_controlnum;								// 컨트롤 개수

	NStrSubBaseDlg			*m_pBaseDlg;								// 부모 다이얼로그


	cltStructureMngBasic*	m_pStrManager;
	cltCharManager*			pclCM;

};

#endif

