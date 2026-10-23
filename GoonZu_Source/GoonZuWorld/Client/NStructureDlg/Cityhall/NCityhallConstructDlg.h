/* ==========================================================================
	클래스 :		NConstructDlg

	작성일 :		05/04/29
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#define _NCITYHALLCONSTRUCTOR_

#pragma once

#include "../StrBaseDlg/StrBaseDlg.h"

#include "CommonHeader.h"
#include "StructureClass/StructureClass.h"

class		CStatic;
class		CButton;
class		CEdit;
class		CList;
class		COutLine;
class		CImageButton;

class cltStructureMngBasic;

class NConstructDlg	: public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_WIDTH = 0,	// DDF 파일에서 읽어서 처리
		DLG_HEIGHT = 0,	// DDF 파일에서 읽어서 처리
		DLG_BIG_POS_X = 1024 - DLG_WIDTH + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 100,
		DLG_SMALL_POS_X = 800 - DLG_WIDTH + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 100,
	};

	NConstructDlg();
	~NConstructDlg(); 

	static	void CALLBACK NConstructStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NConstructProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void Show();
	void Hide();
	bool IsShow();
	void Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	void SetStructureManager( cltStructureMngBasic *pclStructureManager );
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );
	void NConstructDlg::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY );

	bool m_bShow;

	static cltStructureMngBasic*	m_pclStructureManager;

	static BOOL						m_bUpdateSwitch;
	
	/*static */void						SetExplain( TCHAR *szText );
	static DWORD					m_dwLastTick;

	static SI32						m_currentIndex;
	static SI32						m_currentUnique;
	static SI32						m_buildUnique;

	CStatic		*m_pStatic_able_structure;
	CButton		*m_pButton_destruction;
	CButton		*m_pButton_construction;
	CEdit		*m_pEdit_explain;	// IDC_EDIT2
	CList		*m_pList_able_structure;
	CStatic		*m_pStatic_Background;


	CImageButton	*m_pButton_SelectPostion[MAX_CONSTRUCTION_NUM];
	SI32			m_siStructureUnique[MAX_CONSTRUCTION_NUM];		// 해당 버튼에 건설되어있는 건물 유니크

	//CList		*m_pList_structure;
	//CStatic		*m_pStatic_structure;
	//CStatic		*m_pStatic_destruction;
	//CStatic		*m_pStatic_construction;
	//CStatic		*m_pStatic_NONAME1;
	//CEdit		*m_pEdit_destruction;
	//CEdit		*m_pEdit_position;	// IDC_EDIT1
	//CEdit		*m_pEdit_construction;	// IDC_EDIT3
	//COutLine		*m_pOutLine_NONAME2;
	//COutLine		*m_pOutLine_NONAME3;

};

