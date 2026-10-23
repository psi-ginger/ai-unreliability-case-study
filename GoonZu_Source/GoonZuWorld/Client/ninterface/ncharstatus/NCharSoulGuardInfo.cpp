#include "NCharSoulGuard.h"
#include "NCharSoulGuardInfo.h"
#include "NCharStatus.h"

#include "Char\CharManager\CharManager.h"

#include "..\Client\Client.h"

#include "../CommonLogic/MsgType-Person.h"

#include "../../Client/InterfaceMgr/Interface/Button.h"
#include "../../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../../Client/InterfaceMgr/Interface/Static.h"
#include "../../Client/InterfaceMgr/Interface/Edit.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"


//-----------------------------
// Common
//-----------------------------

extern cltCommonLogic* pclClient;

CNCharSoulGuardInfo::CNCharSoulGuardInfo()
{
	m_pclEdit_Info		= NULL;
	m_pclStatic_Noname1	= NULL;
	m_pclButton_Close	= NULL;
}

CNCharSoulGuardInfo::~CNCharSoulGuardInfo()
{
	SAFE_DELETE( m_pclEdit_Info );
	SAFE_DELETE( m_pclStatic_Noname1 );
	SAFE_DELETE( m_pclButton_Close );
}

void CNCharSoulGuardInfo::Create()
{
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NChangeGreetingDlg/DLG_NCharStatus_NSoulGuardInfo.ddf"));
	file.CreatePopupDialog( this, NCHARSOULGUARDINFO_DLG, TEXT("dialog_soulguardinfo"), NCharSoulGuardInfoStaticProc );

	NEWCEDIT( m_pclEdit_Info );			file.CreateControl( m_pclEdit_Info,			NCHARSOULGUARDINFO_DLG_EDITBOX_INFO,	TEXT("editbox_info") );
	NEWCSTATIC( m_pclStatic_Noname1 );	file.CreateControl( m_pclStatic_Noname1,	NCHARSOULGUARDINFO_DLG_STATIC_NONAME1,	TEXT("statictext_NONAME1") );
	NEWCBUTTON( m_pclButton_Close );	file.CreateControl( m_pclButton_Close,		NCHARSOULGUARDINFO_DLG_BUTTON_CLOSE,	TEXT("button_close") );
	
}

void CALLBACK CNCharSoulGuardInfo::NCharSoulGuardInfoStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNCharSoulGuardInfo* pThis = (CNCharSoulGuardInfo*)pControllerMgr;
	if ( pThis )
	{
		pThis->NCharSoulGuardInfoProc( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNCharSoulGuardInfo::NCharSoulGuardInfoProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
	case NCHARSOULGUARDINFO_DLG_BUTTON_CLOSE:
		{
			DeleteDialog();
		}
		break;

	} // switch( nControlID ) Á¾·á
}
