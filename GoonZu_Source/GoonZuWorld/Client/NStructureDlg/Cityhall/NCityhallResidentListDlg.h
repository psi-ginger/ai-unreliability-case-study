/* ==========================================================================
	클래스 :		NResidentListDlg

	작성일 :		05/04/28
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
#define _NCITYHALLRESIDENTLIST_

#pragma once

#include "../StrBaseDlg/StrBaseDlg.h"

#include "CommonHeader.h"
#include "StructureClass/StructureClass.h"

class		CStatic;
class		CButton;
class		CEdit;
class		CList;
class		CComboBox;
class		COutLine;

class cltStructureMngBasic;

class NResidentListDlg : public CDialogController
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

	NResidentListDlg();
	~NResidentListDlg();

	static	void CALLBACK NResidentListStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NResidentListProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void Show();
	void Hide();
	bool IsShow();
	void Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	bool m_bShow;

	static cltStructureMngBasic*m_pclStructureManager;

	CStatic			*m_pStatic_resident_search;
	CStatic			*m_pStatic_id;
	CStatic			*m_pStatic_resident_info;
	CStatic			*m_pStatic_score;
	CButton			*m_pButton_residentlist;
	CButton			*m_pButton_residentinfo;
	CButton			*m_pButton_openminihome;
	CButton			*m_pButton_expulsion_resident;
	CButton			*m_pButton_setscore;
	CEdit			*m_pEdit_name;
	CEdit			*m_pEdit_residentinfo;
	CEdit			*m_pEdit_score;
	CList			*m_pList_resident;
	CComboBox			*m_pComboBox_sort;
	COutLine			*m_pOutline_NONAME1;
	COutLine			*m_pOutline_NONAME2;
	COutLine			*m_pOutline_NONAME3;
	
};

