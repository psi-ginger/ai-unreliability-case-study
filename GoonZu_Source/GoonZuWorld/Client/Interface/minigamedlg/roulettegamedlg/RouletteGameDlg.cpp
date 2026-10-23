#include "../Client/Interface/MinigameDlg/RouletteGameDlg/RouletteGameDlg.h"

#include "../Client/InterfaceMgr/Interface/Button.h"
#include "../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../client/InterfaceMgr/Interface/Edit.h"

#include "../Client/InterfaceMgr/InterfaceFile.h"
#include "../Client/InterfaceMgr/InterfaceMgr.h"
#include "../Client/InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../Client/MiniGame/MiniGameManager.h"
#include "..\CommonLogic\CommonLogic.h"
#include "..\\Lib\\Graphic\\Graphic_Capsule.h"

#include "../CommonLogic/MsgType-Item.h"
#include "../CommonLogic/MsgType.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;


CRouletteGameDlg::CRouletteGameDlg() :CDialogController()
{	
	m_pImageStatic = NULL;
	m_pButton	   = NULL;
	m_pEdit		   = NULL;

	m_pMiniGameManager = NULL;
}

CRouletteGameDlg::~CRouletteGameDlg()
{	
	SetTimerOnOff( false );

	if ( m_pImageStatic )
	{
		delete m_pImageStatic;
		m_pImageStatic = NULL;
	}
	SAFE_DELETE(m_pButton);
	SAFE_DELETE(m_pEdit);
	
	if( m_pMiniGameManager)
	{
		delete m_pMiniGameManager;
		m_pMiniGameManager = NULL;
	}
}

void CRouletteGameDlg::Create()
{

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NRouletteGameDlg/DLG_RouletteGame.ddf"));
		file.CreatePopupDialog( this, ROULETTEGAME_DLG, TEXT("dialog_Roulette"), StaticCallBackDialogCRouletteGame );

		if(gsiResolution == 1) // 800 X 600
			MovePos(120, 40);
		else if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
			MovePos(200, 83);

		NEWCIMAGESTATIC(m_pImageStatic);
		NEWCBUTTON(m_pButton);
		NEWCEDIT(m_pEdit);

		file.CreateControl( m_pImageStatic, ROULETTEGAME_DIALOG_IMAGESTATIC, TEXT("imagestatic_NONAME1") );
		file.CreateControl( m_pButton, ROULETTEGAME_DIALOG_BUTTON, TEXT("button_NONAME1") );
		file.CreateControl( m_pEdit, ROULETTEGAME_DIALOG_EDIT, TEXT("editbox_NONAME1") );

		m_pEdit->SetText(TEXT("1. ���� �÷��� �ڴٴ�   ����� �Ѵ�.\r\n2. ������ ���� ����   �����Ѵ�\r\n3. ���۹�ư�� ������\r\n4. ������ ��ȣ��      ��÷�Ǳ� �⵵�Ѵ�.\r\n�� 5�� �̻��� �ɾ� �� ���� ����Ǹ� 100���� ��ι��� �޽��ϴ�."));
	
		m_pMiniGameManager = new CRouletteBase;

		if(!m_pMiniGameManager->Init())
		{
			MessageBox(NULL, TEXT("MINI_ROULETTE �ʱ�ȭ ����"), TEXT("����"),MB_OK|MB_TOPMOST);
		}

		srand( (unsigned)time( NULL ) );

		SetTimerOnOff( true );
		SetActionTimer( 1 );
	}
	else
	{
		DeleteDialog();
	}

	return;
}

void CALLBACK CRouletteGameDlg::StaticCallBackDialogCRouletteGame( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CRouletteGameDlg *pThis = (CRouletteGameDlg*)pControllerMgr;
	pThis->CallBackDialogCRouletteGame( nEvent, nControlID, pControl );

	return;
}

void CALLBACK CRouletteGameDlg::CallBackDialogCRouletteGame( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR* pTitle = TEXT("�ڴ�� ����");
					TCHAR* pText = TEXT("������ �����Ͻðڽ��ϱ�?");

					cltClient *pclclient = (cltClient *)pclClient;

					stMsgBox MsgBox;
					MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}
				break;
			default:
				break;
			}
		}
		break;
	case ROULETTEGAME_DIALOG_IMAGESTATIC:
		{
			switch( nEvent )
			{

			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					POINT pt;
					m_pImageStatic->GetImageStaticPos( pt );
					m_pMiniGameManager->KeyAction( pt.x, pt.y, 0 );

				}
				break;
			case EVENT_IMAGESTATIC_RBUTTONDOWN:
				{
					POINT pt;
					m_pImageStatic->GetImageStaticPos( pt );
					m_pMiniGameManager->KeyAction( pt.x, pt.y, 0 );

				}
				break;
			}
		}
		break;

	case ROULETTEGAME_DIALOG_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_pMiniGameManager->KeyAction( 0, 0, 1 );
				}
			}
		}
	}

	return;
}

void CRouletteGameDlg::Action()
{
	if( m_pMiniGameManager )
	{
		m_pMiniGameManager->Action();
	}

}

void CRouletteGameDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if( m_pMiniGameManager )
		m_pMiniGameManager->Draw( ScreenRX + GetX(), ScreenRY + GetY());
}

void CRouletteGameDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( RetValue )
			{
				m_pMiniGameManager->m_pRouletteLogic->StopRoulette();
				DeleteDialog();
			}
		}
		break;
	}
}
