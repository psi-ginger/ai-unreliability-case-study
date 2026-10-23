#pragma once

#include "../../Client/Client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../CommonLogic/Msg/MsgType-PostOffice.h"


class NPostOfficeGroupListDlg : public CDialogController
{
public:
	NPostOfficeGroupListDlg();
	~NPostOfficeGroupListDlg();

public:
	void	Create(void);
	void	Destroy(void);


	static void CALLBACK StaticNPostOfficeGroupListDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NPostOfficeGroupListDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

public:
	void AddRecvUser( cltGameMsgResponse_Char_Add_Name_Check* pUser );
	void AddRecvUserGroup( cltGameMsgResponse_PostOffice_User_List_Group* pListGroup );

	void SendItem( SI16 ItemIndex, SI32 ItemUnique, SI16 ItemNum );
	void SendMessage( TCHAR* pMsg );

	SI32 GetListCountNum( void );

private:
	InterfaceFactory	m_InterfaceFactory;
};