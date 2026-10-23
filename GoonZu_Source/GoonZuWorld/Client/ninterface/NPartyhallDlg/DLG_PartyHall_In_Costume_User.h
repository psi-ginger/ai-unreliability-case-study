//------------------------------------------------------------------------------
//
//	파티홀 안에서의 폭죽창.
//
//	2008. 3. 6
//	황진성
//
//------------------------------------------------------------------------------
#ifndef _PARTYHALL_IN_COSTUME_USER_DLG_
#define _PARTYHALL_IN_COSTUME_USER_DLG_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"

class NPartyHall_In_Costume_User_Dlg : public CDialogController
{
public: 


public:
	NPartyHall_In_Costume_User_Dlg();
	~NPartyHall_In_Costume_User_Dlg();

	void Create(  CControllerMgr *pParent  );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	static void CALLBACK StaticNPartyHall_In_Costume_User_DlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
  		   void CALLBACK NPartyHall_In_Costume_User_DlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );
	void SendServerMsg( SI32 _MsgType );										// 서버에 메시지를 보냄.
	void SendServerMsgBufferDelete( SI32 _ClassType, void* _ClassData );	
	void SetCostumeItem( void );												// 옷 리스트뷰에 올리기.

	void ShowEffectInfo(void);

private:
	InterfaceFactory	m_InterfaceFactory;

	SI32				m_SelectItemUnique;
	SI16				m_Index;
};

#endif