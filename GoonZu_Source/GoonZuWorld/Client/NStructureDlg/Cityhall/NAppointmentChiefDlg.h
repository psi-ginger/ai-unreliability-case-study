/* ==========================================================================
	클래스 :		NAppointmentChiefDlg

	작성일 :		05/05/02
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#pragma once

#define _NAPPOINTMENTCHIEF_DLG

//#if defined(_NAPPOINTMENTCHIEF_DLG)

//#include "../StrBaseDlg/StrBaseDlg.h"

#include "CommonHeader.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class cltCityHallManager;

class CButton;
class CList;

#define GET_LISTBOX_ITEMDATA( mc_pList, in_siParam ) \
	{ \
		stListBoxItemData *pListBoxItemData = NULL; \
		SI16 siRow = -1, siCol = -1; \
		mc_pList->GetSelectedIndex( &siRow, &siCol ); \
		if( siRow == -1 || siCol == -1 ) return; \
		pListBoxItemData = mc_pList->GetItemData( siRow, siCol ); \
		if(pListBoxItemData == NULL) return; \
		in_siParam = pListBoxItemData->siParam[0]; \
	}

#define GET_LISTBOX_SELECTED_ROWTEXT( mc_pList, in_pText, const_COLUMN, const_MAX_TEXT_LENGTH  ) \
	{ \
		SI16 siRow = -1, siCol = -1; \
		mc_pList->GetSelectedIndex( &siRow, &siCol ); \
		if( siRow == -1 || siCol == -1 ) return; \
		TCHAR* pTmpChar = NULL; \
		pTmpChar = mc_pList->GetText( siRow, const_COLUMN ); \
		if(pTmpChar == NULL ) return; \
		_tcsncpy( in_pText, pTmpChar, const_MAX_TEXT_LENGTH ); \
	}

class NAppointmentChiefDlg : public CDialogController
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

	NAppointmentChiefDlg();
	~NAppointmentChiefDlg();

	static	void CALLBACK NAppointmentChiefStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NAppointmentChiefProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void Show();
	void Hide();
	bool IsShow();
	void Action();

	bool m_bShow;

	void					ShowChiefCandidate();

	static cltCityHallManager*	m_pclCityHallManager;
	
	static TCHAR					m_szStructure[ 32 ];

	CList	*m_pList_candidate;
	CButton *m_pButton_appointment;
	CButton *m_pButton_delall;

};

//#endif
