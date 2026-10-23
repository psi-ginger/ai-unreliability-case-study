#ifndef _MiniGameDlg_H_
#define _MiniGameDlg_H_

#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;

class CMiniGameFactory;
class CMiniGameManager;

class CMiniGameDlg : public CDialogController
{
public:
	CMiniGameDlg();
	~CMiniGameDlg();

	void Create();
	
	static void CALLBACK StaticCallBackDialogMiniGame( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogMiniGame( UINT nEvent, int nControlID, CControllerObj* pControl );

//	void Hide();
//	void Show();

	void Action();
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );
	CMiniGameManager* GetMiniGameManager() { return m_pMiniGameManager; }

private:
	CImageStatic *m_pImageStatic;

	//-------------------------------
	// 미니 게임 주키퍼
	//-------------------------------
	CMiniGameFactory*	m_pMiniGame;
	CMiniGameManager*	m_pMiniGameManager;

};

#endif