#ifndef _MiniGameDlg_H_
#define _MiniGameDlg_H_

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CImageStatic;

class CMiniGameFactory;
class CMiniGameManager;

class CMiniGameDlg : public CDialogController
{
public:
	CMiniGameDlg();
	~CMiniGameDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogZooKeeper( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogZooKeeper( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Hide();
	void Show();

	void DrawDialog( SI32 ScreenRX, SI32 ScreenRY );

private:
	CButton *m_pCloseButton;
	CImageStatic *m_pImageStatic;

	//-------------------------------
	// 미니 게임 주키퍼
	//-------------------------------
	CMiniGameFactory*	m_pMiniGame;
	CMiniGameManager*	m_pMiniGameManager;

};

#endif