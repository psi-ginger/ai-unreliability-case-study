/* ==========================================================================
	클래스 :		NBeResidentDlg

	작성일 :		05/04/30
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
#define _NBERESIDENTDLG

#pragma once

#include "../StrBaseDlg/StrBaseDlg.h"

#include "CommonHeader.h"
#include "StructureClass/StructureClass.h"

class cltStructureMngBasic;

class		CButton;
class		CEdit;
class		CList;

class NBeResidentDlg : public CDialogController
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

	NBeResidentDlg();
	~NBeResidentDlg();

	static	void CALLBACK NBeResidentStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NBeResidentProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void Show();
	void Hide();
	bool IsShow();
	void Action();

	void UpdateResidentApplyerList(SI32 personid);

	bool m_bShow;

	static cltStructureMngBasic*	m_pclStructureManager;

	CButton	*m_pButton_resident	;
	CButton	*m_pButton_delresident	;
	CEdit	*m_pEdit_name	;
	CList	*m_pList_resident	;

};

