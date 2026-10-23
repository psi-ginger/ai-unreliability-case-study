#include "NNewNotifyDlg.h"

#include "Char\CharManager\CharManager.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

extern cltCommonLogic* pclClient;

#define TIMER_CLOSE	10000		// 1/1000초


CNNewNotifyDlg::CNNewNotifyDlg()
{	
	Initialize();
}

CNNewNotifyDlg::~CNNewNotifyDlg()
{
	Destroy();
}


void CNNewNotifyDlg::Init()
{
	dwUpdateTime = 0;
}

void CNNewNotifyDlg::Destroy()
{

}

void CNNewNotifyDlg::Create()
{
	// 국가 스위치 추가 - By LEEKH 2007.09.19
	//if( !pclClient->IsCountrySwitch(Switch_LevelupUpText) )//
	//		return;

	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NNewNotify/DLG_NNewNotify.ddf"));
	file.CreatePopupDialog( this, NNEWNOTIFY_DLG , TEXT("dialog_newnotify"), StaticNewNotifyDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NNEWNOTIFY_EDIT_NOTIFY , this ), NNEWNOTIFY_EDIT_NOTIFY,   "editbox_notify");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NNEWNOTIFY_IMAGE_PORTRAIT , this ), NNEWNOTIFY_IMAGE_PORTRAIT,   "imagestatic_portrait");
	

	CImageStatic* pImage = m_InterfaceFactory.GetImageStatic( NNEWNOTIFY_IMAGE_PORTRAIT );
	if ( pImage )
	{
		pImage->SetFontIndex( pclClient->GetUniqueFromHash( TEXT("KIND_WAITINGMAN") )-1);
	}
	CEdit* pEdit = m_InterfaceFactory.GetEdit( NNEWNOTIFY_EDIT_NOTIFY );
	if ( pEdit )
	{
		pEdit->SetChangeTextDrawMode( true );
		pEdit->SetTextPrintOption( DT_LEFT | DT_WORDBREAK );
	}

	MovePos( 350, 40 );
	Show( false );
	SetActionTimer( false );
}

void CALLBACK CNNewNotifyDlg::StaticNewNotifyDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNNewNotifyDlg *pThis = (CNNewNotifyDlg*) pControllerMgr;
	pThis->NNewNotifyDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNNewNotifyDlg::NNewNotifyDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Show( false );
			SetTimerOnOff( false );
		}
		break;
	}
}

void CNNewNotifyDlg::SetText(TCHAR *Title, TCHAR *strPrintData, ... )
{
	CControllerMgr::Show( true );

	SetTitle( Title );

	va_list ap;
	TCHAR PrintMsg[ MAX_PATH ] = TEXT("");
	memset( PrintMsg, 0, MAX_PATH );	
	va_start( ap, strPrintData ); 
	StringCchVPrintf( PrintMsg, MAX_PATH, strPrintData, ap );
	va_end( ap ); 

	m_InterfaceFactory.GetEdit(NNEWNOTIFY_EDIT_NOTIFY)->SetText( PrintMsg );

	dwUpdateTime = GetTickCount();
	SetTimerOnOff( true );
	SetActionTimer( 100 );

}

void CNNewNotifyDlg::Action()
{
	if ( GetTickCount() > dwUpdateTime + TIMER_CLOSE )
	{
        Show( false );
		SetTimerOnOff( false );
	}
}

void CNNewNotifyDlg::SetImage(SI32 siNPCKind)
{
	TCHAR szPortrait[256];
	SI32 siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(siNPCKind, szPortrait, sizeof(szPortrait), &siPortraitFont);
	CImageStatic* pImage = m_InterfaceFactory.GetImageStatic( NNEWNOTIFY_IMAGE_PORTRAIT );
	if(pImage)
	{
		pImage->SetFileName( szPortrait );
		pImage->SetFontIndex( siPortraitFont );
	}
}