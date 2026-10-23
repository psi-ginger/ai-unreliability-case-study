#pragma once

#include "../Client/InterfaceMgr/DialogController.h"

class CImageStatic;
class CButton;
class CEdit;

class CRouletteBase;

class CRouletteGameDlg : public CDialogController
{
public:
	CRouletteGameDlg();
	~CRouletteGameDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogCRouletteGame( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogCRouletteGame( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	CImageStatic* m_pImageStatic;
	CButton*	  m_pButton;
	CEdit*		  m_pEdit;
	//-------------------------------
	// 미니 게임 박대박
	//-------------------------------
	CRouletteBase*	m_pMiniGameManager;

};

