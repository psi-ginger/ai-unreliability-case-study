//------------------------------------------------------------------------------
//
//	파티홀 안에서의 초대창.
//
//	2008. 3. 6
//	황진성
//
//------------------------------------------------------------------------------
#ifndef _PARTYHALL_IN_INVITE_CHIEF_DLG_
#define _PARTYHALL_IN_INVITE_CHIEF_DLG_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/Msg/MsgType-Feast.h"

class NPartyHall_In_Invite_Chief_Dlg : public CDialogController
{
public: 


public:
	NPartyHall_In_Invite_Chief_Dlg();
	~NPartyHall_In_Invite_Chief_Dlg();

	void Create( void );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	static void CALLBACK StaticNPartyHall_In_Invite_Chief_DlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NPartyHall_In_Invite_Chief_DlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );
	void SendServerMsg( SI32 _MsgType );										// 서버에 메시지를 보냄.
	void SendServerMsgBufferDelete( SI32 _ClassType, void* _ClassData );	
	void SetEntranceApplicationList( Entrance_Application_Info* Entrance_Application_List );

private:
	InterfaceFactory	m_InterfaceFactory;

	SI32				m_ResvPersonID[ENTRANCE_APPLICATION_LIST];
};

#endif