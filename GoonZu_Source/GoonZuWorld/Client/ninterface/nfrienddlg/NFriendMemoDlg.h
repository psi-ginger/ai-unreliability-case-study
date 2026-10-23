#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNFriendMemoDlg : public CDialogController
{
public:
	CNFriendMemoDlg();
	~CNFriendMemoDlg();

	void Initialize();
	void Destroy();

	void Create();

	static void CALLBACK StaticFriendMemoDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NFriendMemoDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	void SetFriendMemo( TCHAR* memo );
	void SetTitle( TCHAR* charName );
	void SetFriendPersonID( SI32 personID );

private:
	SI32 m_siFriendPersonID;
};