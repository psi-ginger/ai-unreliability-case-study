#ifndef _STRBASEDLG_H
#define _STRBASEDLG_H

#include <Directives.h>

#include "../../Client/client.h"
#include "../../Commonlogic/StructureClass/StructureClass.h"
#include "../../InterfaceMgr/DialogController.h"
#include "nutil.h"

class CEdit;
class CButton;
class CImageStatic;
class CList;
class CComboBox;

class cltCharManager;

enum {
	NSTRBASE_CONTROL		= 10000,
	NSTRBASE_EDIT_EXPLAIN,
	NSTRBASE_IMAGE_PORTRAIT,
	NSTRBASE_BUTTON_CLOSE,
	NSTRBASE_BUTTON_MINIMIZE,
	NSTRBASE_BUTTON_UNMINIMIZE,
	NSTRBASE_BUTTON_CHIEFTOGGLE,
	

	NSTRBASE_USER_TAB_BUTTON		= 11000,
	NSTRBASE_USER_TAB_DIALOG		= 11100,
	
	NSTRBASE_CHIEF_TAB_BUTTON		= 12000, 
	NSTRBASE_CHIEF_TAB_DIALOG		= 12100,

	NSTRBASE_CHILD_CONTROL			= 13000,
};

#define MAX_CHILD_DLG			10
#define MAX_CHILD_CONTROL		64
#define MAX_STRUCTURE_IN_VILLAGE 9

class NStrTabDlg;

// 건물 다이얼로그 베이스 클래스
class NStrBaseDlg : public CDialogController
{
public:
	NStrBaseDlg();
	virtual ~NStrBaseDlg();

	void	CreateDlg( SI16 id, cltStructureMngBasic* pStrManager, int portrait = 0);
	void	InitDlg(BOOL bStateShow = FALSE);

	void	Action();

	void	SetTabInfo( int row, int currentrow );

	void	AddUserTabDialog( int index, TCHAR *text, int width, NStrTabDlg *pChildDlg, TCHAR *filename, TCHAR *szID );
	void	AddChiefTabDialog( int index, TCHAR *text, int width, NStrTabDlg *pChildDlg, TCHAR *filename, TCHAR *szID );

	void	SetChiefMode( BOOL bChief, BOOL bStateShow = FALSE );
	void	SetActiveTab( int index );
	void	HideAllTab();

	void	ShowAllTabButton( bool bShow );

	void	SetMainExplain( TCHAR *text );
		
	void	Minimi(BOOL bMini );	//[영진]
	void	SetUnMinimizeButtonImage(SI32 id);
	SI16	GetEmptyMinimizeSlot();	
	void	SetEmptyMinimizeSlot(SI16 emptySlot);
	
	virtual void GreetingMsg() = 0;

	static	void CALLBACK StaticNDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	virtual void NDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	virtual void DrawDialogFirst( SI32 ScreenRX, SI32 SreenRY );
	

	// [영훈] 텝다이얼로그 활성화/비활성화
	void EnableTabButton( int nIndex, bool bChefMode = false );
	void DisableTabButton( int nIndex, bool bChefMode = false );

	// [영훈] 텝다이얼로그 버튼 활성화
	//void SetTabButtonEnable(  int nIndex, bool bEnable, bool bChefMode = false );
	//bool GetTabButtonEnable(  int nIndex, bool bChefMode = false ) const;

public:

	CEdit					*m_pExplainEdit;
	CImageStatic			*m_pPortraitImage;
	CButton					*m_pCloseBtn;
	CButton					*m_pChiefToggleBtn;
	CButton					*m_pMinimizeButton;
	CButton					*m_pUnMinimizeButton;
	

	CButton					*m_pUserTabButton[ MAX_CHILD_DLG ];
	NStrTabDlg				*m_pUserChildDlg[ MAX_CHILD_DLG ];

	CButton					*m_pChiefTabButton[ MAX_CHILD_DLG ];
	NStrTabDlg				*m_pChiefChildDlg[ MAX_CHILD_DLG ];

	RECT					m_ChildRect;
	
	static	SI32			MinimizeFlagBit;

	int						m_row;
	int						m_currentrow;
	int						m_tabX;
	int						m_tabY;
	int						m_tabHeight;
	BOOL					m_bMini;
	BOOL					m_bChiefMode;

	DWORD					m_dwActionCount;

	int						m_portraitindex;
	
	SI16					m_siOldPosX;
	SI16					m_siOldPosY;
	SI32					m_siOldActiveTab;
	SI16					m_siMinimizedSlot;
	TCHAR					m_szOldTitle[MAX_PATH];
public:
	cltStructureMngBasic	*m_pStrManager;

};


// 탭 다이얼로그 베이스 클래스
class NStrTabDlg : public CDialogController
{
public:
	NStrTabDlg();
	virtual ~NStrTabDlg();

	void CreateTabDlg( int index, NStrBaseDlg *pParent, TCHAR *filename, TCHAR *szID, BOOL bChief );
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
	void				SetEditText( int nControlMapID, TCHAR *text , COLORREF fontColor);
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

	NStrBaseDlg			*m_pBaseDlg;								// 부모 다이얼로그


	cltStructureMngBasic*	m_pStrManager;
	cltCharManager*			pclCM;

};

#define CASE_CONTROL( a )		if( nControlID == m_nControlID[ a ] )

#endif
