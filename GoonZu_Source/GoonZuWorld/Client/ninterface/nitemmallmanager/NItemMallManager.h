
#pragma once

#include <Directives.h>

#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"




class CButton;

class  CNItemMallButtonManager : public CDialogController
{
	

public:
	 CNItemMallButtonManager();
	~ CNItemMallButtonManager();

	void Create();
	void Action();
	//void ShowItemMallButton();
	
	static void CALLBACK StaticNItemMallButtonMsgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NItemMallButtonMsgProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void SetFlash(bool bFlash) { m_bFlash = bFlash;  } ; // ±ô¹ÚÀÌ°Ô ¸¸µé°Å³ª ¸ØÃá´Ù

private:

	SI32	m_siDrawX;
	SI32	m_siDrawY;
	CButton *m_pNItemMallButton;

	SI32    m_siBitmapAnimationIndex  ;

	bool	m_bFlash;
	
};
