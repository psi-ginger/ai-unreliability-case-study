#include "../Client/NInterface/NRecommendPersonDlg/NRecommendPersonDlg.h"

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

#include "../../CommonLogic/CommonLogic.h"
#include "../../Common/Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-Friend.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/MsgType-Person.h"


extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;

CNRecommendPersonDlg::CNRecommendPersonDlg() : CDialogController()
{
	m_pEdit = NULL;

	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[2] = NULL;

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;
}

CNRecommendPersonDlg::~CNRecommendPersonDlg()
{
	SAFE_DELETE(m_pButton[2]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[0]);

	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[0]);

	SAFE_DELETE(m_pEdit);
}

void CNRecommendPersonDlg::Create()
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NRecommendPersonDlg/DLG_RecommendPerson.ddf"));
	file.CreatePopupDialog( this, NRECOMMENDPERSON_DLG, TEXT("dialog_RecommendPerson"), StaticCallBackDialogNRecommendPersonDlg );

	// ȭ�� �߾ӿ� ��ġ ��Ų��.
	cltClient* pclclient = (cltClient*)pclClient;
	SI16 xPos = ( pclclient->GetScreenXSize() - GetWidth() ) / 2 + 40;
	SI16 yPos = ( pclclient->GetScreenYSize() - GetHeight() ) / 2;
	MovePos( xPos, yPos );


	m_pEdit = new CEdit( this );
	m_pButton[0] = new CButton( this );
	m_pButton[1] = new CButton( this );
	m_pButton[2] = new CButton( this );

	m_pStatic[0] = new CStatic( this );
	m_pStatic[1] = new CStatic( this );
	m_pStatic[2] = new CStatic( this );

	file.CreateControl( m_pEdit,	  NRECOMMENDPERSON_DIALOG_EDIT,    TEXT("editbox_NONAME1") );
	file.CreateControl( m_pButton[0], NRECOMMENDPERSON_DIALOG_BUTTON1, TEXT("button_NONAME1") );
	file.CreateControl( m_pButton[1], NRECOMMENDPERSON_DIALOG_BUTTON2, TEXT("button_NONAME2") );
	file.CreateControl( m_pButton[2], NRECOMMENDPERSON_DIALOG_BUTTON3, TEXT("button_NONAME3") );
	
	file.CreateControl( m_pStatic[0], NRECOMMENDPERSON_DIALOG_STATIC1 ,	TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NRECOMMENDPERSON_DIALOG_STATIC2 ,	TEXT("statictext_NONAME2") );
	file.CreateControl( m_pStatic[2], NRECOMMENDPERSON_DIALOG_STATIC3 ,	TEXT("statictext_NONAME3") );

	
	
	Hide();
}


void CALLBACK CNRecommendPersonDlg::StaticCallBackDialogNRecommendPersonDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNRecommendPersonDlg *pThis = (CNRecommendPersonDlg *)pControllerMgr;
	pThis->CallBackDialogNRecommendPersonDlg( nEvent, nControlID, pControl );

}

void CALLBACK CNRecommendPersonDlg::CallBackDialogNRecommendPersonDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	
	case NRECOMMENDPERSON_DIALOG_BUTTON3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{                    
					
				}
				break;
			}
		}
		break;

	case NRECOMMENDPERSON_DIALOG_BUTTON1: // Ȯ��
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR Buffer[ MAX_PLAYER_NAME ] = TEXT("");
				//	GetDlgItemText( m_hDlg, IDC_EDIT_PERSONNAME, Buffer, MAX_PLAYER_NAME - 1 );
					StringCchCopy(Buffer, MAX_PLAYER_NAME, m_pEdit->GetText());

					SI32 index = pclclient->GetCurrentCharInfoList() ;

					cltGameMsgRequest_RecommendPerson clRecommendPerson( Buffer,index );
					cltMsg clMsg( GAMEMSG_REQUEST_RECOMMENDPERSON, sizeof(clRecommendPerson), (BYTE*)&clRecommendPerson );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					Hide();
    			}
				break;
			}
		}
		break;

	case NRECOMMENDPERSON_DIALOG_BUTTON2: // ���
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR Buffer[ MAX_PLAYER_NAME ] = TEXT("");
					//	GetDlgItemText( m_hDlg, IDC_EDIT_PERSONNAME, Buffer, MAX_PLAYER_NAME - 1 );
					//StringCchCopy(Buffer, MAX_PLAYER_NAME, TEXT(""));

					SI32 index = pclclient->GetCurrentCharInfoList() ;

					cltGameMsgRequest_RecommendPerson clRecommendPerson( Buffer,index );
					cltMsg clMsg( GAMEMSG_REQUEST_RECOMMENDPERSON, sizeof(clRecommendPerson), (BYTE*)&clRecommendPerson );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					Hide();
				}
				break;
			}
		}
		break;
	}
}

void CNRecommendPersonDlg::Hide()
{
	CControllerMgr::Show( false );
	//SetModalDialog(false);
}

void CNRecommendPersonDlg::Show()
{ 
	
	 /*if(pclClient->siServiceArea == ConstServiceArea_Japan) //�����翡�� ��õ�ε�� ���Ѵ�.
	{
        CControllerMgr::Show( false );
	}
	else
	{*/
		CControllerMgr::Show( true );
		//SetModalDialog(true);
	//}
}