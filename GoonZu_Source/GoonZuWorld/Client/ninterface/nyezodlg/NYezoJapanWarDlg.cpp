#include "../Client/NInterface/NYezoDlg/NYezoJapanWarDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

extern cltCommonLogic* pclClient;

CNYezoJapanWarDlg::CNYezoJapanWarDlg() : CDialogController()
{
	m_siSword = 0;
	m_siGun = 0;
	m_siCommand = 0;
	m_siPorter = 0;
	m_siVillageUnique = 0;
	m_siSelectComboIndex =-1;

	SI32 i;
	for(i=0;i<12;++i)
		m_pStatic[i] = NULL;
	for(i=0;i<5;++i)
		m_pEdit[i] = NULL;
	m_pCombo = NULL;
	m_pOutLine = NULL;

}

CNYezoJapanWarDlg::~CNYezoJapanWarDlg()
{
	SI32 i;
	for(i=0;i<12;++i)
		SAFE_DELETE(m_pStatic[i]);
	for(i=0;i<5;++i)
		SAFE_DELETE(m_pEdit[i]); 
	SAFE_DELETE(m_pCombo);
	SAFE_DELETE(m_pOutLine);
}

void CNYezoJapanWarDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NYezoDlg/DLG_Yezo_JapanWar.ddf"));
	file.CreateChildDialog( this, NYEZOJAPANWAR_DLG, TEXT("dialog_Yezo_JapanWar"), StaticCallBackDialogNYezoJapanWarDlg, pParent );

	SI32 i;
	for(i=0;i<12;++i)
		NEWCSTATIC(m_pStatic[i]);
	for(i=0;i<5;++i)
		NEWCEDIT(m_pEdit[i]); 
	NEWCCOMBOBOX(m_pCombo);
	NEWCOUTLINE(m_pOutLine);

	TCHAR buffer[20] = TEXT("");
	for(i=0;i<12;++i)
	{
		StringCchPrintf(buffer, 20, TEXT("statictext_NONAME%d"), i+1);
		file.CreateControl( m_pStatic[i], NYEZOJAPANWAR_DIALOG_STATIC, buffer);
		m_pStatic[i]->SetTextPrintOption(DT_LEFT);
		m_pStatic[i]->Refresh();
	}

	for(i=0;i<5;++i)
	{
		StringCchPrintf(buffer, 20, TEXT("editbox_NONAME%d"), i+1);
		file.CreateControl( m_pEdit[i], NYEZOJAPANWAR_DIALOG_EDIT1+i, buffer);
	}
	
	file.CreateControl( m_pCombo, NYEZOJAPANWAR_DIALOG_COMBOBOX, TEXT("combobox_NONAME1"));
	file.CreateControl( m_pOutLine, NYEZOJAPANWAR_DIALOG_OUTLINE, TEXT("outline_NONAME1"));
	
	Hide();
}

void CNYezoJapanWarDlg::Action()
{

}

void CNYezoJapanWarDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNYezoJapanWarDlg::Show()
{
	CControllerMgr::Show( true );

}

void CALLBACK CNYezoJapanWarDlg::StaticCallBackDialogNYezoJapanWarDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNYezoJapanWarDlg *pThis = (CNYezoJapanWarDlg *)pControllerMgr;
	pThis->CallBackDialogNYezoJapanWarDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNYezoJapanWarDlg::CallBackDialogNYezoJapanWarDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case NYEZOJAPANWAR_DIALOG_COMBOBOX:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32 SelectInedx = m_pCombo->GetSelectedIndex();

					if(SelectInedx < 0)
						return;
					m_siSelectComboIndex = SelectInedx;
				}
				break;
			}
		}
		break;
	}
}

void CNYezoJapanWarDlg::SetJapanWar(SI32 Sword, SI32 Gun, SI32 Command, SI32 Porter, SI32 VillageUnique)
{
	m_siSword = Sword;
	m_siGun = Gun;
	m_siCommand = Command;
	m_siPorter = Porter;

	TCHAR buffer[256] = TEXT("");
	SI32 healer = 0 ;
//	HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT1 );
	StringCchPrintf(buffer, 256, TEXT("%d"),m_siSword);
//	SetWindowText(hWnd,buffer);
//	m_pEdit[0]->SetTextPrintOption(DT_RIGHT | DT_VCENTER| DT_SINGLELINE);
	m_pEdit[0]->SetText(buffer);
//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT2 );
	StringCchPrintf(buffer, 256, TEXT("%d"),m_siGun);
//	SetWindowText(hWnd,buffer);
//	m_pEdit[1]->SetTextPrintOption(DT_RIGHT | DT_VCENTER| DT_SINGLELINE);
	m_pEdit[1]->SetText(buffer);
//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT3 );
	StringCchPrintf(buffer, 256, TEXT("%d"),m_siCommand);
//	SetWindowText(hWnd,buffer);
//	m_pEdit[2]->SetTextPrintOption(DT_RIGHT | DT_VCENTER| DT_SINGLELINE);
	m_pEdit[2]->SetText(buffer);
	StringCchPrintf(buffer, 256, TEXT("%d"), m_siPorter);
	//	SetWindowText(hWnd,buffer);
//	m_pEdit[3]->SetTextPrintOption(DT_RIGHT | DT_VCENTER| DT_SINGLELINE);
	m_pEdit[3]->SetText(buffer);
//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT4 );
	healer = ( m_siSword + m_siGun + m_siCommand ) / 10 ;
	StringCchPrintf(buffer, 256, TEXT("%d"),healer);
//	SetWindowText(hWnd,buffer);
//	m_pEdit[4]->SetTextPrintOption(DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	m_pEdit[4]->SetText(buffer);
//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT5 );
	

	SI32 siVillageUnique = VillageUnique ;

	if ( siVillageUnique <= 0 || siVillageUnique > CUR_VILLAGE_LIMIT )
		siVillageUnique = 0 ;

	SetVillageCombo();

	m_siSelectComboIndex = m_pCombo->FindIndexFromText(pclClient->pclVillageManager->GetName(siVillageUnique));
	m_pCombo->SetCursel(m_siSelectComboIndex);
}

void CNYezoJapanWarDlg::GetJapanWar()
{
	TCHAR buffer[256] = TEXT("") ;
//	HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT1 );
//	GetWindowText( hWnd, buffer, 8 );
	MStrCpy(buffer, m_pEdit[0]->GetText(), 8);
	m_siSword = _tstoi(buffer);
//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT2 );
//	GetWindowText( hWnd, buffer, 8 );
	MStrCpy(buffer, m_pEdit[1]->GetText(), 8);
	m_siGun = _tstoi(buffer);
//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT3 );
//	GetWindowText( hWnd, buffer, 8 );
	MStrCpy(buffer, m_pEdit[2]->GetText(), 8);
	m_siCommand = _tstoi(buffer);
//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT5 );
//	GetWindowText( hWnd, buffer, 8 );
	MStrCpy(buffer, m_pEdit[3]->GetText(), 8);
	m_siPorter = _tstoi(buffer);

	SI32 Healer = ( m_siSword + m_siGun + m_siCommand ) / 10 ;
	StringCchPrintf(buffer, 256, TEXT("%d"),Healer);
//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT4 );
//	SetWindowText(hWnd,buffer);
	m_pEdit[4]->SetText(buffer);
}

void CNYezoJapanWarDlg::JapanWarCheck()
{
	cltClient *pclclient = (cltClient*)pclClient;

	TCHAR buffer[8] = TEXT("") ;

	//	HWND hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT1 );
	//	GetWindowText( hWnd, buffer, 8 );
	MStrCpy(buffer, m_pEdit[0]->GetText(), 8);
	m_siSword = _tstoi(buffer);
	if ( m_siSword < 10 || m_siSword > 15000 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2019);
		TCHAR* pText = GetTxtFromMgr(2093);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return;
	}
	//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT2 );
	//	GetWindowText( hWnd, buffer, 8 );
	MStrCpy(buffer, m_pEdit[1]->GetText(), 8);
	m_siGun = _tstoi(buffer);
	if ( m_siGun < 10 || m_siGun > 15000 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2019);
		TCHAR* pText = GetTxtFromMgr(2094);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return;
	}
	//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT3 );
	//	GetWindowText( hWnd, buffer, 8 );
	MStrCpy(buffer, m_pEdit[2]->GetText(), 8);
	m_siCommand = _tstoi(buffer);
	if ( m_siCommand < 0 || m_siCommand > 1000 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2019);
		TCHAR* pText = GetTxtFromMgr(2095);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(  pTitle, pText);
		return;
	}
	//	hWnd = GetDlgItem( m_hTabDlg[0], IDC_EDIT5 );
	//	GetWindowText( hWnd, buffer, 8 );
	MStrCpy(buffer, m_pEdit[3]->GetText(), 8);
	m_siPorter = _tstoi(buffer);
	if ( m_siPorter < 0 || m_siPorter > 3000 )
	{
		TCHAR* pTitle = GetTxtFromMgr(2019);
		TCHAR* pText = GetTxtFromMgr(2096);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(  pTitle, pText);
		return;
	}

	TCHAR szVillage[MAX_PATH] = TEXT("") ;
	
	MStrCpy(szVillage, m_pCombo->GetText(m_siSelectComboIndex), MAX_PATH);

	if ( _tcscmp(szVillage,TEXT("")) )
		m_siVillageUnique= pclclient->pclVillageManager->FindVillageUniqueFromName(szVillage);

	if ( m_siVillageUnique < 0 || m_siVillageUnique > CUR_VILLAGE_LIMIT )
	{
		TCHAR* pTitle = GetTxtFromMgr(1898);
		TCHAR* pText = GetTxtFromMgr(2097);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(  pTitle, pText);
		return;
	}

}
void CNYezoJapanWarDlg::SetVillageCombo()
{
	cltClient *pclclient = (cltClient*)pclClient;

	TCHAR buffer[256] = TEXT("");

//	ComboDelete(m_hTabDlg[0],IDC_COMBO_WARVILLAGE);

	for ( SI32 i = 0 ; i <= CUR_VILLAGE_LIMIT ; i++ )
	{
		StringCchPrintf(buffer, 256, TEXT("%s"),pclclient->pclVillageManager->GetName(i));

		if ( !_tcscmp(buffer,TEXT("")) ) continue;

		stComboBoxItemData Data;
		Data.Init();
		Data.Set(buffer);

		m_pCombo->AddItem(&Data);
	//	ComboAddString(m_hTabDlg[0],IDC_COMBO_WARVILLAGE,(LPARAM)buffer);
	}
}

void CNYezoJapanWarDlg::EnableEditBox()
{
	m_pEdit[0]->SetReadOnly(FALSE);
	m_pEdit[1]->SetReadOnly(FALSE);
	m_pEdit[2]->SetReadOnly(FALSE);
	m_pEdit[3]->SetReadOnly(FALSE);
}

void CNYezoJapanWarDlg::SetPrintEditBox(BOOL flag)
{
	if(flag)
	{
		m_pEdit[0]->SetTextPrintOption(DT_LEFT | DT_VCENTER| DT_SINGLELINE);
		m_pEdit[1]->SetTextPrintOption(DT_LEFT | DT_VCENTER| DT_SINGLELINE);
		m_pEdit[2]->SetTextPrintOption(DT_LEFT | DT_VCENTER| DT_SINGLELINE);
		m_pEdit[3]->SetTextPrintOption(DT_LEFT | DT_VCENTER| DT_SINGLELINE);
		m_pEdit[4]->SetTextPrintOption(DT_LEFT | DT_VCENTER| DT_SINGLELINE);
	}
	else
	{
		m_pEdit[0]->SetTextPrintOption(DT_RIGHT | DT_VCENTER| DT_SINGLELINE);
		m_pEdit[1]->SetTextPrintOption(DT_RIGHT | DT_VCENTER| DT_SINGLELINE);
		m_pEdit[2]->SetTextPrintOption(DT_RIGHT | DT_VCENTER| DT_SINGLELINE);
		m_pEdit[3]->SetTextPrintOption(DT_RIGHT | DT_VCENTER| DT_SINGLELINE);
		m_pEdit[4]->SetTextPrintOption(DT_RIGHT | DT_VCENTER| DT_SINGLELINE);
		m_pCombo->Enable(FALSE);
	}
}