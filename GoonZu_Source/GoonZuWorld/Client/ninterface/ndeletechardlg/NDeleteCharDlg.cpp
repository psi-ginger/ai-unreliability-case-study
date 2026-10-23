#include "../Client/NInterface/NDeleteCharDlg/NDeleteCharDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../Client/NInterface/NSelectChar/NSelectChar.h"
extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;

CNDeleteCharDlg::CNDeleteCharDlg() : CDialogController()
{
	Init();

	m_pStatic[0]  = NULL;
	m_pStatic[1]  = NULL;
	m_pStatic[2]  = NULL;
	m_pStatic[3]  = NULL;
	m_pStatic[4]  = NULL;

	m_pEdit = NULL;

	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
}

CNDeleteCharDlg::~CNDeleteCharDlg()
{
	Init();

	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pStatic[3]);
	SAFE_DELETE(m_pStatic[4]);

	SAFE_DELETE(m_pEdit);

	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);

}

void CNDeleteCharDlg::Create()
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NDeleteCharDlg/DLG_NDeleteChar.ddf"));
	file.CreatePopupDialog( this, NDELETECHAR_DLG, TEXT("dialog_NDeleteChar"), StaticCallBackDialogNDeleteCharDlg );

	if(gsiResolution == 1) // 800 X 600
		MovePos(325, 200);
	else if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		MovePos(405, 150);

	m_pStatic[0]  = new CStatic( this );
	m_pStatic[1]  = new CStatic( this );
	m_pStatic[2]  = new CStatic( this );
	m_pStatic[3]  = new CStatic( this );
	m_pStatic[4]  = new CStatic( this );

	m_pEdit = new CEdit( this );

	m_pButton[0] = new CButton( this );
	m_pButton[1] = new CButton( this );

	file.CreateControl( m_pStatic[0], NDELETECHAR_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NDELETECHAR_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
	file.CreateControl( m_pStatic[2], NDELETECHAR_DIALOG_STATIC3, TEXT("statictext_NONAME3") );
	file.CreateControl( m_pStatic[3], NDELETECHAR_DIALOG_STATIC4, TEXT("statictext_NONAME4") );
	file.CreateControl( m_pStatic[4], NDELETECHAR_DIALOG_STATIC5, TEXT("statictext_NONAME5") );

	file.CreateControl( m_pEdit, NDELETECHAR_DIALOG_EDIT, TEXT("editbox_NONAME1") );
	m_pEdit->SetPasswordMode(true);
	
	if ( pclClient->IsWhereServiceArea(ConstServiceArea_English | ConstServiceArea_NHNChina | ConstServiceArea_EUROPE) )
	{
		m_pEdit->Show(false);

	}

	file.CreateControl( m_pButton[0], NDELETECHAR_DIALOG_BUTTON1, TEXT("button_NONAME1") );
	file.CreateControl( m_pButton[1], NDELETECHAR_DIALOG_BUTTON2, TEXT("button_NONAME2") );

	//_LEON_ENGLISH
	m_pStatic[1]->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_WORDBREAK );
	m_pStatic[1]->Refresh();

	Hide();
}


void CALLBACK CNDeleteCharDlg::StaticCallBackDialogNDeleteCharDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNDeleteCharDlg *pThis = (CNDeleteCharDlg *)pControllerMgr;
	pThis->CallBackDialogNDeleteCharDlg( nEvent, nControlID, pControl );

}

void CALLBACK CNDeleteCharDlg::CallBackDialogNDeleteCharDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case NDELETECHAR_DIALOG_BUTTON1:	// Ȯ��
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
				//	TCHAR buffer[8];
				//	GetWindowText(m_hResidenceEdit,buffer,8);
				
					
					StringCchCopy( m_szResidenceNum ,12,  m_pEdit->GetText() );
					m_bCheck = TRUE ;
					m_pEdit->SetText(TEXT(""));
					Hide();

					if ( GetCheck() &&  pclClient->IsCountrySwitch(Switch_PreDeleteChar))
					{
						if(pclclient->clCharListInfo[pclclient->CurrentCharInfoList].DelCharTime.wYear > 0)return;
						//�ɸ��͸� ���� ���� �ʰ��� ���·� �Ѵ�.
						cltGameMsgRequest_PreDelUserChar clinfo(m_siCurrentCharInfoList, GetResidenceNum(),1);
						cltMsg clMsg(GAMEMSG_REQUEST_PREDELUSERCHAR, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					
						pclclient->CurrentCharInfoList = -1;
						((CNSelectCharDlg*)pclclient->m_pDialog[ NSELECTCHAR_DLG ])->Show();
					}
					else if ( GetCheck() )
					{
						// JAYCEE DEL CHAR 
						cltGameMsgRequest_DelUserChar clinfo(m_siCurrentCharInfoList, GetResidenceNum());
						cltMsg clMsg(GAMEMSG_REQUEST_DELUSERCHAR, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}

					if(pclclient->m_pDialog[ NSELECTCHAR_DLG ] &&  ((CNSelectCharDlg*)pclclient->m_pDialog[ NSELECTCHAR_DLG ])->IsShow() == false )
						((CNSelectCharDlg*)pclclient->m_pDialog[ NSELECTCHAR_DLG ])->Show();
				}
				break;
			}
		}
		break;

	case NDELETECHAR_DIALOG_BUTTON2:	// ����
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_pEdit->SetText(TEXT(""));
					Init();
					Hide();

					if(pclclient->m_pDialog[ NSELECTCHAR_DLG ] && ((CNSelectCharDlg*)pclclient->m_pDialog[ NSELECTCHAR_DLG ])->IsShow() == false )
						((CNSelectCharDlg*)pclclient->m_pDialog[ NSELECTCHAR_DLG ])->Show();
				}
				break;
			}
		}
		break;

	}
}

void CNDeleteCharDlg::Action()
{

}

void CNDeleteCharDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{

}

void CNDeleteCharDlg::Show( )
{
	Init();

	CControllerMgr::Show( true );

	return;
}

void CNDeleteCharDlg::Hide()
{
	CControllerMgr::Show( false );

	return;
}

void CNDeleteCharDlg::Init()
{
		m_bCheck = FALSE ;
		ZeroMemory( m_szResidenceNum, 12 );
		m_siCurrentCharInfoList = 0 ;
}

void CNDeleteCharDlg::set(TCHAR * szName,TCHAR * szLevel,SI32 currentchar)
{
	Init();
	Show();

	TCHAR buffer[256] = TEXT("") ;

	TCHAR* pText = GetTxtFromMgr(1889);
	Nsprintf(buffer,pText,TEXT("char"),  szName  ,TEXT("level"), szLevel   ,NULL);
	//sprintf(buffer,pText,szName,szLevel);

	m_pStatic[0]->SetText(buffer, DT_LEFT | DT_VCENTER | DT_WORDBREAK);

	m_siCurrentCharInfoList = currentchar ;
}