//------------------------------------------------------------------------------
//
//	ÆÄÆ¼È¦ ¾È¿¡¼­ÀÇ ÆøÁ×Ã¢.
//
//	2008. 3. 6
//	È²Áø¼º
//
//------------------------------------------------------------------------------
#ifndef _PARTYHALL_IN_FIRECRACKER_CHIEF_DLG_
#define _PARTYHALL_IN_FIRECRACKER_CHIEF_DLG_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/Msg/MsgType-Feast.h"

class NPartyHall_In_Firecracker_Chief_Dlg : public CDialogController
{
public: 
	enum
	{ 
		USE_FIRECRACKER,	// ÆøÁ×»ç¿ë
		GIVE_PRESEMT_ITEM,	// ¼±¹°ÁÖ±â.
	};

public:
	NPartyHall_In_Firecracker_Chief_Dlg();
	~NPartyHall_In_Firecracker_Chief_Dlg();

	void Create(  CControllerMgr *pParent  );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	static void CALLBACK StaticNPartyHall_In_Firecracker_Chief_DlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NPartyHall_In_Firecracker_Chief_DlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );
	void SendServerMsg( SI32 _MsgType );										// ¼­¹ö¿¡ ¸Þ½ÃÁö¸¦ º¸³¿.
	void SendServerMsgBufferDelete( SI32 _ClassType, void* _ClassData );	
	void ShowEffectInfo( void );
	void SetEntranceApplicationList( Entrance_Application_Info* Entrance_Application_List );

private:
	InterfaceFactory	m_InterfaceFactory;
	SI16				m_Index;

	SI32				m_ResvPersonID[ENTRANCE_APPLICATION_LIST];
};

#endif