
#pragma once

#include "../../InterfaceMgr/DialogController.h"



/*
,PCROOM_DLG_BUTTON_GET
,PCROOM_DLG_LISTBOX_REWARD
*/

class CNPCRoom_Reward : public CDialogController
{

// -- Member Variable Area
private:
	InterfaceFactory m_InterfaceFactory;
protected:

public:


// -- Member Function Area
private:

protected:

public:
	CNPCRoom_Reward( void );
	~CNPCRoom_Reward( void );

	void Init();
	void Create();

	static void CALLBACK StaticCallBackDialogNPCRoom_Reward( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNPCRoom_Reward( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetRewardList( void );


};