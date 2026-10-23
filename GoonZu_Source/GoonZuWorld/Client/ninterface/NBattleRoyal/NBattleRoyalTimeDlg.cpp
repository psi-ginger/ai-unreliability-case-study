#include "NBattleRoyalTimeDlg.h"

#include "../../CommonLogic/MsgType-System.h"
#include "../../StatusBar/ChatStatusBar/ChatStatusBarBaseInfo.h"

#include "Char\CharManager\CharManager.h"

#include "../../InterfaceMgr/Interface/Static.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../GlobalImgFile/GlobalImgFile.h"

#include "../Client/MarkManager/MarkManager.h"

extern cltCommonLogic* pclClient;

CNBattleRoyalTimeDlg::CNBattleRoyalTimeDlg()
{
	
	m_pNTimeStatic = NULL;  
}

CNBattleRoyalTimeDlg::~CNBattleRoyalTimeDlg()
{
	SAFE_DELETE(m_pNTimeStatic);
	return;
}


void CNBattleRoyalTimeDlg::Create()
{
	if ( ! IsCreate() )
	{
		//CDialogController::Create( NRAREITEM_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCNRareItemDlgProc, TEXT("¸íÇ° "), TEXT("NInterface/Interface/Interface_20X20_00_109.Spr"), 9, true, 80, 100, 300, 240, true, false, true);

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NBattleRoyal/DLG_NBattleRoyalTimeDlg.ddf"));
		file.CreatePopupDialog( this, NBATTLEROYALTIME_DLG, TEXT("dialog_BattleRoyalTime"),StatiCNBattleRoyalTimeDlgProc);	
		
		m_pNTimeStatic= new CStatic( this );

		file.CreateControl( m_pNTimeStatic, NBATTLEROYALTIME_STATIC , TEXT("statictext_Text") );
	}

	return;
}

void CALLBACK CNBattleRoyalTimeDlg::StatiCNBattleRoyalTimeDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl)
{
	CNBattleRoyalTimeDlg *pThis = (CNBattleRoyalTimeDlg*) pControllerMgr;
	pThis->NBattleRoyalTimeDlgProc( nEvent, nControlID, pControl);
}

void CALLBACK CNBattleRoyalTimeDlg::NBattleRoyalTimeDlgProc(  UINT nEvent, int nControlID, CControllerObj* pControl  )
{
	
	cltClient *pclclient = (cltClient*)pclClient;

	switch(nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;	
	}


	return;
}

void CNBattleRoyalTimeDlg::SetDescription(TCHAR* szDescription)
{
	m_pNTimeStatic->SetBorder(true);
	m_pNTimeStatic->SetBKColorUse(true);
	m_pNTimeStatic->SetText(szDescription, DT_WORDBREAK );
}
