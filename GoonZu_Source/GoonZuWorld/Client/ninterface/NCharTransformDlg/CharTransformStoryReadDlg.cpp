
#include "../Client/ninterface/NCharTransformDlg/CharTransformStoryReadDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "Char\CharManager\CharManager.h"


extern cltCommonLogic* pclClient;



CNChartransformStroyReadDlg::CNChartransformStroyReadDlg()
{	 

}

CNChartransformStroyReadDlg::~CNChartransformStroyReadDlg()
{
	Destroy();
}



void CNChartransformStroyReadDlg::Init()
{

}

void CNChartransformStroyReadDlg::Destroy()
{

}

void CNChartransformStroyReadDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/CharTransformDlg/TransformCharStoryReadDlg.ddf"));

	file.CreatePopupDialog( this, CHARTRANSFORMINFO_STORY_READ_DLG, TEXT("dialog_CharTransformStoryRead"), StaticChartransformStroyReadDlgProc);

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHARTRANSFORMSTORYREAD_BTN_CLOSE,	  this ), NCHARTRANSFORMSTORYREAD_BTN_CLOSE,		"button_close");

	//MovePos();
}

void CALLBACK CNChartransformStroyReadDlg::StaticChartransformStroyReadDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNChartransformStroyReadDlg *pThis = (CNChartransformStroyReadDlg*) pControllerMgr;
	pThis->NChartransformStroyReadDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNChartransformStroyReadDlg::NChartransformStroyReadDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;	

	case NCHARTRANSFORMSTORYREAD_BTN_CLOSE:
		{
			DeleteDialog();
		}
		break;

	default:
		{

		}
		break;
	}
}


void CNChartransformStroyReadDlg::Action()
{
	if(IsShow())
	{		


	}
}