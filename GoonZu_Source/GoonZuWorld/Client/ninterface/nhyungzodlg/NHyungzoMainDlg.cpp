#include "../Client/NInterface/NHyungzoDlg/NHyungzoMainDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/RadioButton.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

extern cltCommonLogic* pclClient;

CNHyungzoMainDlg::CNHyungzoMainDlg() : CDialogController()
{
	m_siStealNum = 0; // 악의적 스틸 제한 횟수
	m_siSuspectTime = 0; // 매크로 프로그램 체크 시간
	m_siPrisonTime = 0; // 매크로 사용자 수감 시간
	m_siPrisonerPenalty = 0; // 매크로 사용자 형벌 정도
	
	m_pOutLine = NULL;

	SI32 i;
	for(i=0;i<9;++i)
		m_pStatic[i] = NULL;
	for(i=0;i<3;++i)
		m_pEdit[i] = NULL;
	for(i=0;i<2;++i)
		m_pButton[i] = NULL;
	for(i=0;i<5;++i)
		m_pRadio[i] = NULL;
}

CNHyungzoMainDlg::~CNHyungzoMainDlg()
{
	SAFE_DELETE(m_pOutLine);
	SI32 i;
	for(i=0;i<9;++i)
		SAFE_DELETE(m_pStatic[i]);
	for(i=0;i<3;++i)
		SAFE_DELETE(m_pEdit[i]);
	for(i=0;i<2;++i)
		SAFE_DELETE(m_pButton[i]);
	for(i=0;i<5;++i)
		SAFE_DELETE(m_pRadio[i]);
}

void CNHyungzoMainDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

 	file.LoadFile(TEXT("NInterface/Data/NHyungzoDlg/DLG_Hyungzo_Main.ddf"));
	file.CreateChildDialog( this, NHYUNGZOMAIN_DLG, TEXT("dialog_Hyungzo_Main"), StaticCallBackDialogNHyungzoMainDlg, pParent );

	SI32 i;
	for(i=0;i<9;++i)
		NEWCSTATIC(m_pStatic[i]);
	for(i=0;i<3;++i)
		NEWCEDIT(m_pEdit[i]);
	for(i=0;i<2;++i)
		NEWCBUTTON(m_pButton[i]);
	for(i=0;i<5;++i)
		NEWCRADIOBUTTON(m_pRadio[i]);
	
	m_pOutLine = new COutLine(this);

	TCHAR buffer[20] = TEXT("");
	for(i=0;i<9;++i)
	{	
		StringCchPrintf(buffer, 20, TEXT("statictext_NONAME%d"), i+1);
		file.CreateControl( m_pStatic[i], NHYUNGZOMAIN_DIALOG_STATIC, buffer );
	}
	for(i=0;i<3;++i)
	{	
		StringCchPrintf(buffer, 20, TEXT("editbox_NONAME%d"), i+1);
		file.CreateControl( m_pEdit[i], NHYUNGZOMAIN_DIALOG_EDIT1 + i, buffer );
	}
	for(i=0;i<2;++i)
	{	
		StringCchPrintf(buffer, 20, TEXT("button_NONAME%d"), i+1);
		file.CreateControl( m_pButton[i], NHYUNGZOMAIN_DIALOG_BUTTON1 + i, buffer );
	}
	for(i=0;i<5;++i)
	{	
		StringCchPrintf(buffer, 20, TEXT("radiobutton_NONAME%d"), i+1);
		file.CreateControl( m_pRadio[i], NHYUNGZOMAIN_DIALOG_RADIOBUTTON1 + i, buffer );
	}

	file.CreateControl(m_pOutLine, NHYUNGZOMAIN_DIALOG_OUTLINE , "outline_NONAME1");

	//_LEON_ENGLISH 
	m_pStatic[3]->SetTextPrintOption(TA_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pStatic[8]->SetTextPrintOption(TA_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pStatic[3]->Refresh();
	m_pStatic[8]->Refresh();
	
	Hide();
}

void CNHyungzoMainDlg::Action()
{

}

void CNHyungzoMainDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHyungzoMainDlg::Show()
{
	CControllerMgr::Show( true );

}

void CALLBACK CNHyungzoMainDlg::StaticCallBackDialogNHyungzoMainDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHyungzoMainDlg *pThis = (CNHyungzoMainDlg *)pControllerMgr;
	pThis->CallBackDialogNHyungzoMainDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHyungzoMainDlg::CallBackDialogNHyungzoMainDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case NHYUNGZOMAIN_DIALOG_BUTTON1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buffer[512] = TEXT("") ;
					TCHAR* pText = GetTxtFromMgr(2025);
					StringCchPrintf(buffer, 512, pText);
					TCHAR* pTitle = GetTxtFromMgr(2026);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);
				}
				break;
			}
		}
		break;

	case NHYUNGZOMAIN_DIALOG_BUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buffer[512] = TEXT("") ;
					TCHAR* pText = GetTxtFromMgr(2027);
					StringCchPrintf(buffer, 512, pText);
					TCHAR* pTitle = GetTxtFromMgr(2028);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
				}
				break;
			}
		}
		break;

	case NHYUNGZOMAIN_DIALOG_RADIOBUTTON1:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					BOOL Check = m_pRadio[0]->IsCheck();
					if(Check)
						m_siPrisonerPenalty = 20 ;
				}
				break;
			}
		}
		break;

	case NHYUNGZOMAIN_DIALOG_RADIOBUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					BOOL Check = m_pRadio[1]->IsCheck();
					if(Check)
						m_siPrisonerPenalty = 40 ;
				}
				break;
			}
		}
		break;

	case NHYUNGZOMAIN_DIALOG_RADIOBUTTON3:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					BOOL Check = m_pRadio[2]->IsCheck();
					if(Check)
						m_siPrisonerPenalty = 60 ;
				}
				break;
			}
		}
		break;

	case NHYUNGZOMAIN_DIALOG_RADIOBUTTON4:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					BOOL Check = m_pRadio[3]->IsCheck();
					if(Check)
						m_siPrisonerPenalty = 80 ;
				}
				break;
			}
		}
		break;

	case NHYUNGZOMAIN_DIALOG_RADIOBUTTON5:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					BOOL Check = m_pRadio[4]->IsCheck();
					if(Check)
						m_siPrisonerPenalty = 100 ;
				}
				break;
			}
		}
		break;
	}
}

void CNHyungzoMainDlg::SetHyungzoMain(SI16 stealnum,SI16 suspecttime,SI16 prisontime,	SI16 prisonerpenalty)
{
	m_siStealNum = stealnum ;

	TCHAR buffer[256] = TEXT("") ;

	m_siStealNum = stealnum;
	m_siSuspectTime = suspecttime ;
	m_siPrisonTime = prisontime ;
	m_siPrisonerPenalty = prisonerpenalty ;

//	HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_LIMIT_STEAL );
	StringCchPrintf(buffer, 256, TEXT("%d"),m_siStealNum);
//	SetWindowText(hWnd,buffer);
	m_pEdit[0]->SetText(buffer);

	m_siSuspectTime = suspecttime ;

//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_LIMIT_SUSPECTTIME );
	StringCchPrintf(buffer, 256, TEXT("%d"),m_siSuspectTime);
//	SetWindowText(hWnd,buffer);
	m_pEdit[1]->SetText(buffer);

//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_LIMIT_SUSPECT_PRISONTIME );
	StringCchPrintf(buffer, 256, TEXT("%d"),m_siPrisonTime);
//	SetWindowText(hWnd,buffer);
	m_pEdit[2]->SetText(buffer);
	
	
	switch(m_siPrisonerPenalty)
	{
	case 20 : m_pRadio[0]->SetCheck(TRUE, CONTROLLER_KIND_RADIOBUTTON);
			  break;	// CheckRadioButton(m_hTabDlg[0],IDC_RADIO_ONE,IDC_RADIO_FIVE,IDC_RADIO_ONE); break;
	case 40 : m_pRadio[1]->SetCheck(TRUE, CONTROLLER_KIND_RADIOBUTTON);
			  break;	// CheckRadioButton(m_hTabDlg[0],IDC_RADIO_ONE,IDC_RADIO_FIVE,IDC_RADIO_TWO); break;
	case 60 : m_pRadio[2]->SetCheck(TRUE, CONTROLLER_KIND_RADIOBUTTON);
			  break;// CheckRadioButton(m_hTabDlg[0],IDC_RADIO_ONE,IDC_RADIO_FIVE,IDC_RADIO_THREE); break;
	case 80 : m_pRadio[3]->SetCheck(TRUE, CONTROLLER_KIND_RADIOBUTTON);// CheckRadioButton(m_hTabDlg[0],IDC_RADIO_ONE,IDC_RADIO_FIVE,IDC_RADIO_FOUR); break;
			  break;
	case 100 : m_pRadio[4]->SetCheck(TRUE, CONTROLLER_KIND_RADIOBUTTON);// CheckRadioButton(m_hTabDlg[0],IDC_RADIO_ONE,IDC_RADIO_FIVE,IDC_RADIO_FIVE); break;
				break;
	}
	
}

void CNHyungzoMainDlg::SetEnableButton(BOOL flag)
{
	if(flag)
	{
		m_pEdit[0]->SetReadOnly(FALSE);
		m_pEdit[1]->SetReadOnly(FALSE);
		m_pEdit[2]->SetReadOnly(FALSE);

		m_pEdit[0]->SetTextPrintOption(DT_LEFT | DT_VCENTER);
		m_pEdit[1]->SetTextPrintOption(DT_LEFT | DT_VCENTER);
		m_pEdit[2]->SetTextPrintOption(DT_LEFT | DT_VCENTER);

		m_pRadio[0]->Enable(TRUE);
		m_pRadio[1]->Enable(TRUE);
		m_pRadio[2]->Enable(TRUE);
		m_pRadio[3]->Enable(TRUE);
		m_pRadio[4]->Enable(TRUE);
	}
	else
	{
		m_pEdit[0]->SetTextPrintOption(DT_RIGHT | DT_VCENTER);
		m_pEdit[1]->SetTextPrintOption(DT_RIGHT | DT_VCENTER);
		m_pEdit[2]->SetTextPrintOption(DT_RIGHT | DT_VCENTER);

		m_pRadio[0]->Enable(FALSE);
		m_pRadio[1]->Enable(FALSE);
		m_pRadio[2]->Enable(FALSE);
		m_pRadio[3]->Enable(FALSE);
		m_pRadio[4]->Enable(FALSE);
	}
}

BOOL CNHyungzoMainDlg::HyungzoMainCheck()
{
	cltClient *pclclient = (cltClient*)pclClient;

	TCHAR buffer[8] = TEXT("") ;

//	HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_LIMIT_STEAL );
//	GetWindowText( hWnd, buffer, 8 );
	MStrCpy(buffer, m_pEdit[0]->GetText(), 8);
	m_siStealNum = _tstoi(buffer);
	if ( m_siStealNum < 3 || m_siStealNum > 10 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2019);
		TCHAR* pText = GetTxtFromMgr(2020);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return FALSE;
	}

//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_LIMIT_SUSPECTTIME );
//	GetWindowText( hWnd, buffer, 8 );
	MStrCpy(buffer, m_pEdit[1]->GetText(), 8);
	m_siSuspectTime = _tstoi(buffer);
	if ( m_siSuspectTime < 20 || m_siSuspectTime > 300 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2019);
		TCHAR* pText = GetTxtFromMgr(2021);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return FALSE;
	}

//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT_LIMIT_SUSPECT_PRISONTIME );
//	GetWindowText( hWnd, buffer, 8 );
	MStrCpy(buffer, m_pEdit[2]->GetText(), 8);
	m_siPrisonTime = _tstoi(buffer);
	if ( m_siPrisonTime < 5 || m_siPrisonTime > 60 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2019);
		TCHAR* pText = GetTxtFromMgr(2022);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return FALSE;
	}

	if ( m_siPrisonerPenalty < 20 || m_siPrisonerPenalty > 100 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2019);
		TCHAR* pText = GetTxtFromMgr(2023);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return FALSE;
	}

	return TRUE;
}