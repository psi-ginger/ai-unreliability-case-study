
#pragma once

#include <Directives.h>

#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"

class CButton;

class  CNBattleRoyalMoveDlg : public CDialogController
{
	

public:
	 CNBattleRoyalMoveDlg();
	~ CNBattleRoyalMoveDlg();

	void Create();
	void Action();
	
	static void CALLBACK StaticNBattleRoyalMoveProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NBattleRoyalMoveProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void SetFlash(bool bFlash) { m_bFlash = bFlash;  } ; // ±ô¹ÚÀÌ°Ô ¸¸µé°Å³ª ¸ØÃá´Ù

private:

	SI32	m_siDrawX;
	SI32	m_siDrawY;
	CButton *m_pButtonMove;

	SI32    m_siBitmapAnimationIndex  ;

	bool	m_bFlash;
	
	UI32	m_uiFlashCount;
};
