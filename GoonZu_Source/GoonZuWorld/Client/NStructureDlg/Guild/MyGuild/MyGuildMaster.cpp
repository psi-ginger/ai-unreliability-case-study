

#include "../../../Client.h"
#include "../../../InterfaceMgr/InterfaceFile.h"
#include "MyGuildMaster.h"
#include "MyGuildBase.h"

#include "MsgType-Guild.h"


extern cltCommonLogic* pclClient;

CNMyGuildMaster::CNMyGuildMaster()
{
	Initialize();
}	

CNMyGuildMaster::~CNMyGuildMaster()
{
	Destroy();
}

void CNMyGuildMaster::Initialize()
{

}

void CNMyGuildMaster::Destroy()
{
}

void CNMyGuildMaster::Create( CControllerMgr *pParent )
{

	cltClient *pclclient = (cltClient *)pclClient;

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/Guild/DLG_MyGuildMaster.ddf"));
		file.CreateChildDialog( this, NMYGUILDMASTER_DLG, TEXT("dialog_MyGuildMaster"), StaticMyGuildMasterDlgProc, pParent);

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILDMASTER_BUTTON_1 , this)  , NMYGUILDMASTER_BUTTON_1 , TEXT("button_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILDMASTER_BUTTON_2 , this)  , NMYGUILDMASTER_BUTTON_2 , TEXT("button_NONAME2"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILDMASTER_BUTTON_3 , this)  , NMYGUILDMASTER_BUTTON_3 , TEXT("button_NONAME3"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILDMASTER_BUTTON_5 , this)  , NMYGUILDMASTER_BUTTON_5 , TEXT("button_NONAME5"));
		
	//	m_pManageTab = new CNMyGuildMasterManage;

	//	m_pManageTab->Create(this);

	}

}

void CALLBACK CNMyGuildMaster::StaticMyGuildMasterDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMyGuildMaster *pThis = (CNMyGuildMaster*) pControllerMgr;
	pThis->NMyGuildMasterDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNMyGuildMaster::NMyGuildMasterDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NMYGUILDMASTER_BUTTON_1:
		{		
			((CNMyGuildBase*) pclclient->m_pDialog[NMYGUILDBASE_DLG])->SelectMasterSubTab( NMYGUILDMASTERMANAGE_DLG );
		}
		break;

	case NMYGUILDMASTER_BUTTON_2:
		{		
			((CNMyGuildBase*) pclclient->m_pDialog[NMYGUILDBASE_DLG])->SelectMasterSubTab( NMYGUILDMASTERSTATUS_DLG );
		}
		break;
	case NMYGUILDMASTER_BUTTON_3:
		{		
			((CNMyGuildBase*) pclclient->m_pDialog[NMYGUILDBASE_DLG])->SelectMasterSubTab( NMYGUILDMASTERHUNTMAP_DLG );
		}
		break;
	case NMYGUILDMASTER_BUTTON_5:
		{		
			((CNMyGuildBase*) pclclient->m_pDialog[NMYGUILDBASE_DLG])->SelectMasterSubTab( NMYGUILDMASTERMARK_DLG );
		}
		break;

	}
}


void CNMyGuildMaster::Hide()
{
	CControllerMgr::Show(false);

}

void CNMyGuildMaster::Show()
{
	CControllerMgr::Show(true);

}

