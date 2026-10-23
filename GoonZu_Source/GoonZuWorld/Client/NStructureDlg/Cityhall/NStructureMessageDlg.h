//#if defined(_NSTRUCTUREMESSAGEDLG)

#pragma once

#include "CommonHeader.h"
#include "StructureClass/StructureClass.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

#define MAX_VILLAGENOTICE_LENGTH	255

class cltStructureMngBasic;
class CButton;
class CEdit;

#define GET_EDITBOX_TEXT( mc_pEdit, in_chText, in_siSize ) \
	{ \
		TCHAR *pchTmp = NULL; \
		pchTmp = mc_pEdit->GetText(); \
		if(pchTmp == NULL ) return; \
		if(_tcslen( pchTmp ) >= in_siSize ) return; \
		_tcsncpy( in_chText, pchTmp, in_siSize ); \
	}

class NStructureMessageDlg : public CDialogController
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

	NStructureMessageDlg();
	~NStructureMessageDlg();

	static	void CALLBACK NStructureMessageStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NStructureMessageProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void Show();
	void Hide();
	bool IsShow();

	bool m_bShow;

	static cltStructureMngBasic*	m_pclStructureManager;

	static SI32						m_siStrType;

	CEdit	*m_pEdit_message;
	CButton *m_pButton_set;
	CButton *m_pButton_close;	
};

//#endif
