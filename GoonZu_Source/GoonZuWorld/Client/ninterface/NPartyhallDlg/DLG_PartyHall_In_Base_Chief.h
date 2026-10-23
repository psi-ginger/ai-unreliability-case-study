//------------------------------------------------------------------------------
//
//	파티홀 안에서의 기본창.
//
//	2008. 3. 6
//	황진성
//
//------------------------------------------------------------------------------
#ifndef _PARTYHALL_IN_BASE_DLG_
#define _PARTYHALL_IN_BASE_DLG_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"

class NPartyHall_In_BaseDlg : public CDialogController
{
public: 


public:
	NPartyHall_In_BaseDlg();
	~NPartyHall_In_BaseDlg();

	void Create( void );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	static void CALLBACK StaticNPartyHall_In_BaseDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NPartyHall_In_BaseDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );
	void SendServerMsg( SI32 _MsgType );										// 서버에 메시지를 보냄.
	void SendServerMsgBufferDelete( SI32 _ClassType, void* _ClassData );	

private:
	InterfaceFactory	m_InterfaceFactory;
};

#endif