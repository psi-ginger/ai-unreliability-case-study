#include "../Client/Interface/NExitDlg/NExitDlg.h"


#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"
#include "../Client/DLGPositionSave/DLGPositionSave.h"

#include "NInterface/NCommonMsgDlg/NCommonMsgDlg.h"
#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"

extern cltCommonLogic* pclClient;
extern DWORD dwQuitTimer;
extern SI32 gsiResolution;

CNExitConfirmDlg::CNExitConfirmDlg() : CDialogController()
{

	for(int i=0; i < 2; ++i)
		m_pBtn[i] = NULL;

	m_pStatic = NULL;
}

CNExitConfirmDlg::~CNExitConfirmDlg()
{
	for(int i=0; i< 2; ++i)
	{
		if(m_pBtn[i])
		{
			delete m_pBtn[i];
			m_pBtn[i] = NULL;
		}
	}

	if(m_pStatic)
	{
		delete m_pStatic;
		m_pStatic = NULL;
	}
}

void CNExitConfirmDlg::Create()
{
	//	TCHAR* pText = GetTxtFromMgr(3168);

	if ( ! IsCreate() ) 
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NExitDlg/DLG_ExitConfirm.ddf"));
		file.CreatePopupDialog( this, NEXITCONFIRM_DLG, TEXT("dialog_ExitConfirm"), StaticCallBackDialogNExitConfirmDlg );

		if(gsiResolution == 1) // 800 X 600
			MovePos(370, 300);
		else if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
			MovePos(450, 360);

		NEWCBUTTON(m_pBtn[0]);
		NEWCBUTTON(m_pBtn[1]);
		NEWCSTATIC(m_pStatic);

		file.CreateControl( m_pBtn[0], NEXITCONFIRM_DIALOG_EXIT_BUTTON, TEXT("button_NONAME1")); 
		file.CreateControl( m_pBtn[1], NEXITCONFIRM_DIALOG_CANCEL_BUTTON, TEXT("button_NONAME2")); 
		file.CreateControl( m_pStatic, NEXITCONFIRM_DIALOG_STATIC, TEXT("statictext_NONAME1")); 

	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNExitConfirmDlg::StaticCallBackDialogNExitConfirmDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNExitConfirmDlg *pThis = (CNExitConfirmDlg *)pControllerMgr;
	pThis->CallBackDialogNExitConfirmDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNExitConfirmDlg::CallBackDialogNExitConfirmDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NEXITCONFIRM_DIALOG_EXIT_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					//cyj ���� ���϶��� �α׾ƿ� 20�ʰ� ī��Ʈ �ϵ��� ����
					if (pclclient->GetGameStatus() >= GS_GAME && pclclient->pGameSession->GetState() != SESSION_STATE_NONE
							&& pclclient->pGameSession->GetState() != SESSION_STATE_DISCONNECTED)
					{					
						if(pclclient->m_pDialog[ NCOMMONMSG_DLG ]==NULL)
						{
							pclclient->CreateInterface( NCOMMONMSG_DLG );
						}
						((CNCommonMsgDlg*)pclclient->m_pDialog[ NCOMMONMSG_DLG ])->Set(COMMONDLG_EXITCOUNT);					
										
#ifdef USE_HANAUTH
						TCHAR FullURL[1024] = {'\0', };						
						if (pclclient->IsWhereServiceArea(Switch_AuctionEvent)
							&&  pclclient->m_siSucceedEventType > 0) //�����̺�Ʈ��� ����� �̺�Ʈ���������� [2007.09.11 �ռ���]
						{
							if(pclclient->m_siHanAuth_ServiceTypeCode == SERVICE_ALPHA)
							{
								StringCchPrintf(FullURL,1024,
									"http://alpha-gzs.hangame.com/event/auction.nhn");
								ShellExecute(NULL,NULL,TEXT("IEXPLORE.EXE"),FullURL,NULL,SW_SHOWNORMAL);
							}
							else
							{
								StringCchPrintf(FullURL,1024,
									"http://gzs.hangame.com/event/auction.nhn");
								ShellExecute(NULL,NULL,TEXT("IEXPLORE.EXE"),FullURL,NULL,SW_SHOWNORMAL);
							}
						}

						if (pclclient->IsWhereServiceArea(Switch_NowComEvent)
							&&  pclclient->m_siSucceedEventType > 0) //KHY - 1025 - ������ �̺�Ʈ
						{
							if(pclclient->m_siHanAuth_ServiceTypeCode == SERVICE_ALPHA)
							{
								StringCchPrintf(FullURL,1024,
									"http://alpha-gzs.hangame.com/event/nowevent.nhn");
								ShellExecute(NULL,NULL,TEXT("IEXPLORE.EXE"),FullURL,NULL,SW_SHOWMAXIMIZED);
							}
							else
							{
								StringCchPrintf(FullURL,1024,
									"http://gzs.hangame.com/event/nowevent.nhn");
								ShellExecute(NULL,NULL,TEXT("IEXPLORE.EXE"),FullURL,NULL,SW_SHOWMAXIMIZED);
							}
						}

#endif // USE_HANAUTH
					}
					else
					{
						if( pclClient->GetFrame() <= 0 ||
							( pclclient->dwLastReceiveFromServerClock && TABS(pclclient->CurrentClock - pclclient->dwLastReceiveFromServerClock) > 5000))
						{
							dwQuitTimer = 1;
						}
						else if (pclclient->pGameSession->GetState() == SESSION_STATE_NONE || pclclient->pGameSession->GetState() == SESSION_STATE_DISCONNECTED)
						{
							dwQuitTimer = 1;
						}
						else
						{
							cltMsg clMsg(GAMEMSG_REQUEST_HOPETOQUIT, 0, NULL);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						}
					}

					//g_DlgPositionSave.SavePosition();
					DeleteDialog();
				}
				break;
			}
		}
		break;

	case NEXITCONFIRM_DIALOG_CANCEL_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;
	}
}
