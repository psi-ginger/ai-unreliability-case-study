#include "NotifyMsgDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"
#include "../CommonLogic/MsgType-System.h"
#include "../CommonLogic/MsgRval-Define.h"
#include "../CommonLogic/MsgType-Person.h"
#include "../CommonLogic/Msg/MsgType-Wedding.h"
#include "../CommonLogic/Marriage/MarriageManager.h"

#include "../../../CommonLogic/CommonLogic.h"
#include "../../../Common/Order/Order.h"


extern cltCommonLogic* pclClient;

CNotifyMsgDlg::CNotifyMsgDlg() : CDialogController()
{
	m_pTextStatic = NULL;
}

CNotifyMsgDlg::~CNotifyMsgDlg()
{


	if ( m_pTextStatic )
	{
		delete m_pTextStatic;
		m_pTextStatic = NULL;
	}
}

void CNotifyMsgDlg::Create()
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NNotifyMsgDlg/DLG_NotifyMsg.ddf"));
	file.CreatePopupDialog( this, NOTIFYMSG_DLG, TEXT("dialog_NotifyMsg"), StaticCallBackDialogNotifyMsg );
	
	NEWCEDIT(m_pTextStatic);

	file.CreateControl( m_pTextStatic, NOTIFYMSG_DLG_TEXT_STATIC, TEXT("editbox_NONAME1") );
	m_pTextStatic->SetText(TEXT("   "));
	m_pTextStatic->SetBKColorUse(true);
	m_pTextStatic->SetBorder(true);

	m_siOriginalWidth	= GetWidth();
	m_siOriginalHeight	= GetHeight();

	Hide();
}

void CALLBACK CNotifyMsgDlg::StaticCallBackDialogNotifyMsg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNotifyMsgDlg *pThis = (CNotifyMsgDlg *)pControllerMgr;
	pThis->CallBackDialogNotifyMsg( nEvent, nControlID, pControl );
}

void CALLBACK CNotifyMsgDlg::CallBackDialogNotifyMsg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					Hide();
				}
				break;
			default:
				break;
			}
		}
		break;
	}
}

void CNotifyMsgDlg::Hide()
{
	CControllerMgr::Show( false );
}

void CNotifyMsgDlg::Show()
{
	CControllerMgr::Show( true );
}

void CNotifyMsgDlg::SetText( TCHAR *Title, TCHAR *strPrintData, ... )
{
	//  [7/13/2009 종호] 예외처리 코드 추가.
	if(Title == NULL || strPrintData == NULL)
		return;
    
	cltClient* pclclient = (cltClient*)pclClient;

	SetWidth(m_siOriginalWidth);
	SetHeight(m_siOriginalHeight);

	CControllerMgr::Show( true );
	
	SetTitle( Title );

	va_list ap;
	TCHAR PrintMsg[2048] = TEXT("");
	memset( PrintMsg, 0, sizeof(PrintMsg) );	
	va_start( ap, strPrintData ); 
	StringCchVPrintf( PrintMsg, sizeof(PrintMsg), strPrintData, ap );
  	va_end( ap );  

  	m_pTextStatic->SetText( PrintMsg );
	
	// 화면 중심에 출력
	SI32 xPos = ( pclclient->GetScreenXSize() - GetWidth() ) / 2 + 40;
	SI32 yPos = ( pclclient->GetScreenYSize() - GetHeight() ) / 2;

	MovePos( xPos, yPos );

//	MovePos( 445 + (g_SoundOnOffDlg.m_bMaxWindowMode? 30:0) , 100);


	/*
	if ( WINVER >= 0x0400 )
	{
		m_pTextStatic->SetText( PrintMsg, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_EDITCONTROL );
	}
	else
	{
		m_pTextStatic->SetText( PrintMsg, DT_LEFT | DT_TOP | DT_WORDBREAK );
	}
	*/
}

void CNotifyMsgDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{

	case 0:
		{
			if (RetValue)
			{
				SI32 selectOrder = *((SI32*)pData);
				SI32 x = *((SI32*)(pData + sizeof(SI32)));
				SI32 y = *((SI32*)(pData + sizeof(SI32) + sizeof(SI32)));
				SI32 id = *((SI32*)(pData + sizeof(SI32) + sizeof(SI32) + sizeof(SI32)));


				cltClient* pclclient = (cltClient*)pclClient;
				//---------------------------------------
				// 명령을 만든다. 
				//---------------------------------------

				pclclient->pclOrderManager->clOrder.MakeOrder(
					selectOrder, 
					x, y, 
					id,
					0, 0);


				//---------------------------------
				// 선택한 명령과 오브젝트를 초기화한다. 
				//---------------------------------
				pclclient->siSelectedOrder			= 0;
				pclclient->siSelectedOrderPara1	= 0;
				pclclient->siSelectedOrderPara2	= 0;

				// 선택된 내용을 초기화한다. 
				pclclient->clMouseObject.Init();
			}
		}
		break;

	case SRVAL_BLACKARMY_MAPPOSITION:	
		{
			if (RetValue)
			{
				SI32 *gateunique = (SI32*)pData;

				cltGameMsgRequest_BlackArmy_WarpToGate clGateMsg(*gateunique);
				cltMsg clMsg(GAMEMSG_REQUEST_BLACKARMY_WARPTOGATE, sizeof(clGateMsg), (BYTE*)&clGateMsg);

				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

			}

		}
		break;

	case SRVAL_NOTICE_WARPTOPOLL_MAP:
		{
			if ( RetValue )
			{
				cltMsg clMsg( GAMEMSG_REQUEST_WARPTOPOLLMAP, 0, NULL );
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
			}
		}
		break;

	case SRVAL_PVP_LEAGUE_AGAIN_RESERVE:
		{
			cltGameMsgRequest_PVP_League_Again_Reserve clAnswer( RetValue );
			cltMsg clMsg( GAMEMSG_REQUEST_PVP_LEAGUE_AGAIN_RESERVE, sizeof(clAnswer), (BYTE*)&clAnswer );
			pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
		}
		break;

	case SRVAL_WEDDING_MATE1_QUESTION:
		{
			cltGameMsgRequest_WeddingQuestionAnswer clinfo( MARRIAGE_WEDDING_STATUS_MATE1_QUESTION_WAIT );
			cltMsg clMsg( GAMEMSG_REQUEST_WEDDING_QUESTION_ANSWER, sizeof(clinfo), (BYTE*)&clinfo );
			pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
		}
		break;

	case SRVAL_WEDDING_MATE2_QUESTION:
		{
			cltGameMsgRequest_WeddingQuestionAnswer clinfo( MARRIAGE_WEDDING_STATUS_MATE2_QUESTION_WAIT );
			cltMsg clMsg( GAMEMSG_REQUEST_WEDDING_QUESTION_ANSWER, sizeof(clinfo), (BYTE*)&clinfo );
			pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
		}
		break;

	case SRVAL_WEDDING_GIVEITEM_QUESTION:
		{
			cltGameMsgRequest_WeddingQuestionAnswer clinfo( MARRIAGE_WEDDING_STATUS_GIVEITEM_WAIT );
			cltMsg clMsg( GAMEMSG_REQUEST_WEDDING_QUESTION_ANSWER, sizeof(clinfo), (BYTE*)&clinfo );
			pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
		}
		break;

	// PK 가능맵에 들어가길 원하는지 물어보는 창의 반환값 - by LEEKH 2007.10.11
	case 10:
		{
			// 아무런 처리를 하지 않는다.
		}
		break;
	//KHY - 1113 - 크리스마스 이벤트 -  사냥터 입장여부를 물어본다.
	case 20:
		{
			if (RetValue)
			{
				cltMsg clMsg( GAMEMSG_REQUEST_CHRISTMASEVET_ENTERFIELD, 0, NULL );
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );			
			}
		}
		break;
	}
}

