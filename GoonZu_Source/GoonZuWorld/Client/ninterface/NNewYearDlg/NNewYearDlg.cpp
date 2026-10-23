
#include ".\NNewYeardlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/Msg/MsgType-Quest.h"

SI32	m_siButtonAniFrame;

extern cltCommonLogic* pclClient;


SYSTEMTIME g_sEndTime;

CNNewYearDlg::CNNewYearDlg()
{	 
	m_siButtonAniFrame = 0;
}

CNNewYearDlg::~CNNewYearDlg()
{
	Destroy();
}



void CNNewYearDlg::Init()
{

}

void CNNewYearDlg::Destroy()
{
                     
}

void CNNewYearDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/NNewYear/DLG_NNewYear.ddf"));
	
	//[수정 : 황진성 2007. 12. 17 신년 버튼 창에 x표시 제거.]
	SI32 controlindex = file.GetControlIndex( TEXT("dialog_newyear") );
	file.m_control[ controlindex ].dialog.bClosable = false;
	
	file.CreatePopupDialog( this, NNEWYEAR_DLG, TEXT("dialog_newyear"), StaticNewYearDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NNEWYEAR_BUTTON_CHECK, this ), NNEWYEAR_BUTTON_CHECK,   "button_check");
	
	m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->SetDrawOption( SPR_PUTSPRALPHAT );
	m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->SetAlphaValue( 0 );

	SetShowHideMethod( false );

	SetFileName(TEXT(""));
	SetDialogPos( pclclient->GetScreenXSize()/2-100, 120, 300, 54 );
//	Show( true );
	
	SetActionTimer( 100 );
	SetTimerOnOff( true );
		
	TimeAddMinute(&pclclient->sTime, 5, &g_sEndTime);    
}

void CALLBACK CNNewYearDlg::StaticNewYearDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNNewYearDlg *pThis = (CNNewYearDlg*) pControllerMgr;
	pThis->NNewYearDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNNewYearDlg::NNewYearDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
//				DeleteDialog();
			}
			break;	
		case NNEWYEAR_BUTTON_CHECK:
			{
				cltMsg clMsg( GAMEMSG_REQUEST_NEWYEAR_CLICK, 0 );
				pclclient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );	
				DeleteDialog();
			}
			break;
	}
}


void CNNewYearDlg::Action()
{
	if(IsShow())
	{		
		cltClient* pclclient = (cltClient*)pclClient;
		if(pclclient->sTime.wMinute == g_sEndTime.wMinute)
		{
			DeleteDialog();
		}

		SI32 siAlphaValue = m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->GetAlphaValue();
		if( siAlphaValue < MAX_ALPHA_LEVEL - 1 )
		{
			siAlphaValue++;
			m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->SetAlphaValue( siAlphaValue );
		}
		else
		{
			m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->SetDrawOption( SPR_PUTSPRT);
			m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->SetAlphaValue( MAX_ALPHA_LEVEL - 1 );

			if(m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->IsMouseOver() == false)
			{
				switch(m_siButtonAniFrame)
				{
					case 0:		m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->Show( false );	break;
					case 1:		m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->Show( true );	break;
					case 2:		m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->Show( false );	break;
					case 3:		m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->Show( true );	break;
					case 4:		m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->Show( false );	break;
					case 5:		m_InterfaceFactory.GetImageStatic( NNEWYEAR_BUTTON_CHECK )->Show( true );	break;
				}
			}
			m_siButtonAniFrame++;
			m_siButtonAniFrame %= 30;
		}
	}
}