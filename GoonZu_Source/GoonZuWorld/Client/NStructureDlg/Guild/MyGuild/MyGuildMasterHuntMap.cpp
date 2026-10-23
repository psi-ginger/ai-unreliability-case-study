

#include "../../../Client.h"
#include "../../../InterfaceMgr/InterfaceFile.h"
#include "MyGuildMasterHuntMap.h"


extern cltCommonLogic* pclClient;

CNMyGuildMasterHuntMap::CNMyGuildMasterHuntMap()
{
	Initialize();
}	

CNMyGuildMasterHuntMap::~CNMyGuildMasterHuntMap()
{
	Destroy();
}

void CNMyGuildMasterHuntMap::Initialize()
{

}

void CNMyGuildMasterHuntMap::Destroy()
{

}

void CNMyGuildMasterHuntMap::Create( CControllerMgr *pParent )
{

	cltClient *pclclient = (cltClient *)pclClient;

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/Guild/DLG_MyGuildMasterHuntMap.ddf"));

		file.CreateChildDialog( this, NMYGUILDMASTERMANAGE_DLG, TEXT("dialog_MyGuildMasterHuntMap"),StaticMyGuildMasterHuntMapDlgProc , pParent);

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST   ,NMYGUILDMASTERSTATUS_LIST1 , this)  , NMYGUILDMASTERSTATUS_LIST1 , TEXT("listbox_NONAME1"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILDMASTERSTATUS_BUTTON1 , this)  , NMYGUILDMASTERSTATUS_BUTTON1 , TEXT("button_select"));

	}

}

void CALLBACK CNMyGuildMasterHuntMap::StaticMyGuildMasterHuntMapDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMyGuildMasterHuntMap *pThis = (CNMyGuildMasterHuntMap*) pControllerMgr;
	pThis->NMyGuildMasterHuntMapDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNMyGuildMasterHuntMap::NMyGuildMasterHuntMapDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{

	}
}


void CNMyGuildMasterHuntMap::Hide()
{
	CControllerMgr::Show(false);

}

void CNMyGuildMasterHuntMap::Show()
{
	CControllerMgr::Show(true);

}
