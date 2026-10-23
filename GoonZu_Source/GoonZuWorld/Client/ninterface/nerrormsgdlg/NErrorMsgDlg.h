/* ==========================================================================
	클래스 :		CNErrorMsgPrint, CNErrorMsgPrint1

	작성일 :		05/05/03
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#include <Directives.h>
#include <stdio.h>

#include "../../DBManager/GameDBManager_World/DBGameLogMsg.h"

#include "../../InterfaceMgr/DialogController.h"

class CEdit;

// small message
class CNErrorMsgPrint : public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_WIDTH = 0,	// DDF 파일에서 읽어서 처리
		DLG_HEIGHT = 0,	// DDF 파일에서 읽어서 처리
		DLG_BIG_POS_X = 1024 - DLG_WIDTH + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 65,
		DLG_SMALL_POS_X = 800 - DLG_WIDTH + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 65,
	};

	CNErrorMsgPrint();
	~CNErrorMsgPrint();

	static	void CALLBACK NErrorMsgPrintStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NErrorMsgPrintProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void Show();
	void Hide();
	bool IsShow();

	void SetEditText( TCHAR* in_pTmp );
	void SetTitleCaption( TCHAR* in_pTmp );

	bool m_bShow;

	CEdit	*m_pEdit_error_print;

};

// big message
class CNErrorMsgPrint1 : public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_WIDTH = 0,	// DDF 파일에서 읽어서 처리
		DLG_HEIGHT = 0,	// DDF 파일에서 읽어서 처리
		DLG_BIG_POS_X = 1024 - DLG_WIDTH + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 65,
		DLG_SMALL_POS_X = 800 - DLG_WIDTH + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 65,
	};

	CNErrorMsgPrint1();
	~CNErrorMsgPrint1();

	static	void CALLBACK NErrorMsgPrint1StaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NErrorMsgPrint1Proc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void Show();
	void Hide();
	bool IsShow();

	void SetEditText( TCHAR* in_pTmp );
	void SetTitleCaption( TCHAR* in_pTmp );

	bool m_bShow;

	CEdit	*m_pEdit_error_print1;

};
