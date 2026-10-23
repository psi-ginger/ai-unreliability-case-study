// NSiegeWinDlg.cpp: implementation of the CNSiegeWinDlg class.

#include "NSiegeWinDlg.h"
#include "Resource.h"
#include "../../Client/client.h"
#include "../../InterfaceMgr/Interface/Static.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"



extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNSiegeWinDlg::CNSiegeWinDlg()
{
	m_pSiegeWinStatic= NULL ;
}

CNSiegeWinDlg::~CNSiegeWinDlg()
{	
	if(m_pSiegeWinStatic)
	{
		delete m_pSiegeWinStatic;
		m_pSiegeWinStatic = NULL;
	}
}

void CNSiegeWinDlg::Create(  )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NSiegeWin/DLG_NSiegeWin.ddf"));
	file.CreatePopupDialog( this, NSEIGEWIN_DLG, TEXT("dialog_SiegeWin"), StatiCNSiegeWinProc);
	m_pSiegeWinStatic =  new CStatic( this );
	file.CreateControl(m_pSiegeWinStatic, NSEIGEWIN_STATIC , TEXT("statictext_SiegeWin"));	

}

void CALLBACK CNSiegeWinDlg::StatiCNSiegeWinProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNSiegeWinDlg *pThis = (CNSiegeWinDlg*) pControllerMgr;
	pThis->NSiegeWinProc( nEvent, nControlID, pControl );
}

void CALLBACK CNSiegeWinDlg::NSiegeWinProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:	
			{
				DeleteDialog();
			}
			break;
	}
	
	

	return;
}

void CNSiegeWinDlg::Set(TCHAR* ptext)
{
	m_pSiegeWinStatic->SetBorder(true);
	m_pSiegeWinStatic->SetBKColorUse(true);
	m_pSiegeWinStatic->SetText(ptext,DT_WORDBREAK);

}


