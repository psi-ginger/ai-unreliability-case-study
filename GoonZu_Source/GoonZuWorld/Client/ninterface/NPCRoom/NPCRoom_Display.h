
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../../NLib/NUpdateTimer.h"

/*
,PCROOM_DISPLAY_DLG_BUTTON_OPEN
*/

class CNPCRoom_Display : public CDialogController
{

// -- Member Variable Area
private:
	InterfaceFactory	m_InterfaceFactory;
	NUpdateTimer		m_UpdateTimer_Hide;
protected:

public:

// -- Member Function Area
private:

protected:

public:
	CNPCRoom_Display( void );
	~CNPCRoom_Display( void );

	void Init();
	void Create();
	void Action();

	static void CALLBACK StaticCallBackDialogNPCRoom_Display( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNPCRoom_Display( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show( void );
	void Hide( void );

	void ShowDisplayPCRoom( void );
};