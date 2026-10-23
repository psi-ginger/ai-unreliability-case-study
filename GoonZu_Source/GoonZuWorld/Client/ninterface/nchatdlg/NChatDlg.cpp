#include "NChatDlg.h"
#include "../../Client.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ChatEdit.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/CommonLogic.h"
#include "../../../CommonLogic/MsgType-System.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../../Common/Char/CharManager/CharManager.h"
#include "../../../Common/Skill/Skill-Manager.h"
#include "../../../Common/Skill/Skill.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../../CommonLogic/MsgType-Person.h"

#include "../../StatusBar/ChatStatusBar/ChatStatusBarBuffer.h"

#include "../../Interface/ChatDlg/ChatDlg.h"
//#include "../../Interface/ResidentsChatDlg/ResidentsChatDlg.h"
#include "../../Interface/RichEditChatMgrDlg/RichEditChatMgrDlg.h"

#include "../../Interface/ResidentsViewDlg/ResidentsViewDlg.h"
#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../NResidentsDlg/NResidentsDlg.h"
//#include "../../Interface/RichEditChatMgrDlg/RichEditChatMgrDlg.h"

#include "../../../../NLib/NUtil.h"

extern cltCommonLogic* pclClient;

extern SI32		gsiResolution;

// NUtil.cpp에 선언된 변수
extern UI64	uiSec; // 시간 계산을 위한 기준 변수

CNChatDlg::CNChatDlg() : CDialogController()
{
	m_pChatEdit		=	NULL;
	m_pInputEdit	=	NULL;
	m_pComboBox		=	NULL;
	m_pOpenButton	=	NULL;

	m_pChatStatusBarBuffer = new CChatStatusBarBuffer();
	m_strChatData = new TCHAR[ MAX_CHAT_DATA_LENGTH ];

	m_bOpened		=	false;

}

CNChatDlg::~CNChatDlg()
{
	SAFE_DELETE( m_pChatEdit );
	SAFE_DELETE( m_pInputEdit );
	SAFE_DELETE( m_pComboBox );
	SAFE_DELETE( m_pOpenButton );

	SAFE_DELETE( m_pChatStatusBarBuffer );
	SAFE_DELETE_A( m_strChatData );
}

void CNChatDlg::Create()
{

	//SetTopMostWindow();

	return;
}

void CALLBACK CNChatDlg::StaticCallBackDialogNChat( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNChatDlg *pThis = (CNChatDlg*)pControllerMgr;
	pThis->CallBackDialogNChat( nEvent, nControlID, pControl );
	return;
}

void CALLBACK CNChatDlg::CallBackDialogNChat( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			SetOpenedChatDlg( false );
			Show( false );
		}
		break;
	case NCHAT_DIALOG_EDIT_INPUT:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					TCHAR *EditBoxText = m_pInputEdit->GetText();
					if ( EditBoxText && _tcslen( EditBoxText ) > 0 )
					{
						memcpy( m_strChatData, EditBoxText, MAX_CHAT_DATA_LENGTH );
						m_strChatData[ MAX_CHAT_DATA_LENGTH - 1 ] = NULL;
						ChatDataSend();
					}

					m_pInputEdit->ClearText();
				}
				break;
			}
		}
		break;
	case NCHAT_DIALOG_BUTTON_OPEN:
		{
			SetOpenedChatDlg( ! m_bOpened );
		}
		break;
	}

	return;
}

void CNChatDlg::CharacterLogin()
{
	m_pComboBox->Clear();

	TCHAR* pText = GetTxtFromMgr(1214);

	stComboBoxItemData ItemData;
	ItemData.Set( pText );
	m_pComboBox->AddItem( &ItemData );

	// 마을
	pText = GetTxtFromMgr(1770);
	ItemData.Set( pText );
	m_pComboBox->AddItem( &ItemData );
	// 상단
	pText = GetTxtFromMgr(5560);
	ItemData.Set( pText );
	m_pComboBox->AddItem( &ItemData );
	// 파티
	pText = GetTxtFromMgr(1221);
	ItemData.Set( pText );
	m_pComboBox->AddItem( &ItemData );
	// 광고
	pText = GetTxtFromMgr(1215);
	ItemData.Set( pText );
	m_pComboBox->AddItem( &ItemData );
	// 자막광고
	pText = GetTxtFromMgr(1856);
	ItemData.Set( pText );
	m_pComboBox->AddItem( &ItemData );
	// 대행수라면 마을공지, 주민공지
	if ( pclClient->pclCM->CR[1]->pclCI->clCharRank.siType == RANKTYPE_CITYHALL )
	{
		pText = GetTxtFromMgr(1217);
		ItemData.Set( pText );
		m_pComboBox->AddItem( &ItemData );

		pText = GetTxtFromMgr(1218);
		ItemData.Set( pText );
		m_pComboBox->AddItem( &ItemData );
	}
	// 군주라면 군주공지
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_KING )
	{
		pText = GetTxtFromMgr(1219);
		ItemData.Set( pText );
		m_pComboBox->AddItem( &ItemData );
	}

	//_LEON_GUILD_CHAT
	//#if defined(_DEBUG)
	//if ( pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siType == cltGuildRankManager::GUILDRANK_CHIEF )
	//{
	//	pText = GetTxtFromMgr(5561);
	//	ItemData.Set( pText );
	//	m_pComboBox->AddItem( &ItemData );
	//}
	//#endif
	m_pComboBox->SetCursel( 0 );

	return;
}

void CNChatDlg::ChatDataSend()
{
	SI32 SelectedIndex = m_pComboBox->GetSelectedIndex();
	if ( SelectedIndex < 0 )
	{
		return;
	}

	TCHAR* pText[9];

	// 일반
	pText[0]= GetTxtFromMgr(1214);
	// 마을
	pText[1]= GetTxtFromMgr(1770);
	// 상단
	pText[2]= GetTxtFromMgr(5560);
	// 파티
	pText[3]= GetTxtFromMgr(1221);
	// 광고
	pText[4]= GetTxtFromMgr(1215);
	// 자막광고
	pText[5]= GetTxtFromMgr(1856);
	// 마을공지
	pText[6]= GetTxtFromMgr(1217);
	// 주민공지
	pText[7] = GetTxtFromMgr(1218);
	// 군주공지
	pText[8] = GetTxtFromMgr(1219);

	TCHAR *SelectText;
	SelectText = m_pComboBox->GetText( SelectedIndex );

	// 일반
	if ( _tcscmp( SelectText, pText[0] ) == 0 )				
	{
		SI32 ChatMode = CompareKey(m_strChatData, SelectedIndex);

		if( ChatMode == TAB_MODE_DEAULT )
			Push( CHAT_DATA_SEND, m_strChatData );
		else if( ChatMode == TAB_MODE_RESIDENTSCHAT )
			Push( RESIDENTS_CHAT_DATA_SEND, m_strChatData );
		else if( ChatMode == TAB_MODE_GUILDCHAT )
			Push( GUILD_CHAT_DATA_SEND, m_strChatData );
		else if( ChatMode == TAB_MODE_PARTYCHAT )
			Push( PARTY_CHAT_DATA_SEND, m_strChatData );
	}
	// 주민채팅
	else if ( _tcscmp( SelectText, pText[1] ) == 0 )			
	{
		SI32 ChatMode = CompareKey(m_strChatData, SelectedIndex);

		if( ChatMode == TAB_MODE_DEAULT )
			Push( CHAT_DATA_SEND, m_strChatData );
		else if( ChatMode == TAB_MODE_RESIDENTSCHAT )
			Push( RESIDENTS_CHAT_DATA_SEND, m_strChatData );
		else if( ChatMode == TAB_MODE_GUILDCHAT )
			Push( GUILD_CHAT_DATA_SEND, m_strChatData );
		else if( ChatMode == TAB_MODE_PARTYCHAT )
			Push( PARTY_CHAT_DATA_SEND, m_strChatData );
	}
	// 상단채팅
	else if ( _tcscmp( SelectText, pText[2] ) == 0 )			
	{
		SI32 ChatMode = CompareKey(m_strChatData, SelectedIndex);

		if( ChatMode == TAB_MODE_DEAULT )
			Push( CHAT_DATA_SEND, m_strChatData );
		else if( ChatMode == TAB_MODE_RESIDENTSCHAT )
			Push( RESIDENTS_CHAT_DATA_SEND, m_strChatData );
		else if( ChatMode == TAB_MODE_GUILDCHAT )
			Push( GUILD_CHAT_DATA_SEND, m_strChatData );
		else if( ChatMode == TAB_MODE_PARTYCHAT )
			Push( PARTY_CHAT_DATA_SEND, m_strChatData );
	}
	// 파티채팅
	else if ( _tcscmp( SelectText, pText[3] ) == 0 )
	{
		SI32 ChatMode = CompareKey(m_strChatData, SelectedIndex);

		if( ChatMode == TAB_MODE_DEAULT )
			Push( CHAT_DATA_SEND, m_strChatData );
		else if( ChatMode == TAB_MODE_RESIDENTSCHAT )
			Push( RESIDENTS_CHAT_DATA_SEND, m_strChatData );
		else if( ChatMode == TAB_MODE_GUILDCHAT )
			Push( GUILD_CHAT_DATA_SEND, m_strChatData );
		else if( ChatMode == TAB_MODE_PARTYCHAT )
			Push( PARTY_CHAT_DATA_SEND, m_strChatData );
	}
	// 광고
	else if ( _tcscmp( SelectText, pText[4] ) == 0 )
	{
		Push( HEAD_CHAT_DATA_SEND, m_strChatData );
	}
	// 자막광고
	else if ( _tcscmp( SelectText, pText[5] ) == 0 )
	{
		Push( NEWS_CHAT_DATA_SEND, m_strChatData );		
	}
	// 마을공지
	else if ( _tcscmp( SelectText, pText[6] ) == 0 )
	{
		Push( TOWN_CHAT_DATA_SEND, m_strChatData );		
	}
	// 주민공지
	else if ( _tcscmp( SelectText, pText[7] ) == 0 )
	{
		Push( RESIDENTSNOTIFY_CHAT_DATA_SEND, m_strChatData );	
	}
	// 군주공지
	else if ( _tcscmp( SelectText, pText[8] ) == 0 )
	{
		Push( KINGNOTIFY_CHAT_DATA_SEND, m_strChatData );	
	}

	return;
}

void CNChatDlg::Push( SI16 ChatDataKind, TCHAR *ChatText )
{
	m_pChatStatusBarBuffer->Push( ChatDataKind, ChatText );
	return;
}

bool CNChatDlg::Pop( stChatStatusBarData * pChatStatusBarData )
{
	memset( pChatStatusBarData, 0, sizeof( stChatStatusBarData ) );

	if ( m_pChatStatusBarBuffer->Pop( pChatStatusBarData ) )
	{
		return true;
	}

	return false;
}

void CNChatDlg::SetInputEditBoxFocus()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( pclclient->m_pInterfaceMgr )
	{
		pclclient->m_pInterfaceMgr->SetFocus( this, m_pInputEdit );
	}

	return;
}

void CNChatDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  )
{
	cltClient *pclclient = ( cltClient *) pclClient;
	if ( m_bOpened )
	{
		TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( TEXT("NInterface/Interface/INTERFACE_10x27_00_103.SPR") );

		if ( pSpr )
		{
			if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
			{
				DrawTileImg( pSpr, SPR_PUTSPRT, 0, 0, ScreenRX + GetX(), ScreenRY + GetY() + 153, 430, 27, 3 );
			}
			else
			{
				DrawTileImg( pSpr, SPR_PUTSPRT, 0, 0, ScreenRX + GetX(), ScreenRY + GetY() + 153, 328, 27, 3 );
			}
		}

		/*
		pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg(  TEXT("NInterface/Image/IMAGE_44x24_00_000.SPR") );
		if ( pSpr )
		{
		pSpr->PutSprT( ScreenRX + GetX(), ScreenRY + GetY(), 0 );
		}
		*/
	}
	return;
}

void CNChatDlg::SmallShow()
{
	return;
}

void CNChatDlg::BigShow()
{
	return;
}

bool CNChatDlg::CheckData( TCHAR *PlayerName, TCHAR *ChatText )
{
	if ( PlayerName == NULL || ChatText == NULL )
	{
		return false;
	}

	if ( _tcslen( PlayerName ) > CHATEDIT_NAMEDATA_LENGTH - 1 || _tcslen( ChatText ) > MAX_CHAT_DATA_LENGTH - 1 )
	{
		return false;
	}

	return true;
}

void CNChatDlg::ChatRecv( TCHAR *PlayerName, TCHAR *ChatText, SI32 Kind )
{	
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}

	stChatEditData ChatEditData;
	SI32 ChatKind = 0;

	switch(Kind)
	{
		// 일반
	case 0:
		{
			ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
			ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_WHITE;
			ChatKind = MSG_CHAT_RECV;
		}
		break;
		// 마을
	case 1:
		{
			ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
			ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_GREEN;
			ChatKind = MSG_RESIDENTSCHAT_RECV;
		}
		break;
		// 상단
	case 2:
		{
			//		TCHAR buf[256]=TEXT(" ");
			//		sprintf(buf, TEXT("상단:%s"), PlayerName);
			//		strcpy(PlayerName,buf);

			ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
			ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_PINK;
			ChatKind = MSG_GUILDCHAT_RECV;
		}
		break;
		// 파티
	case 3:
		{
			//	TCHAR buf[256]=TEXT(" ");
			//	sprintf(buf, TEXT("파티:%s"), PlayerName);
			//	strcpy(PlayerName,buf);

			ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
			ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_SKYBLUE;
			ChatKind = MSG_PARTYCHAT_RECV;
		}
		break;

	default:
		{
			ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
			ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_YELLOW;
			ChatKind = MSG_CHAT_RECV;
		}
		break;
	}

	MStrCpy( ChatEditData.strNameData, PlayerName, MAX_PLAYER_NAME );
	MStrCpy( ChatEditData.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );

	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	m_pChatEdit->Insert(&ChatEditData);

	return;
}

void CNChatDlg::MasterNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}

	stChatEditData ChatEditData;
	ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_YELLOW;
	ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
	MStrCpy( ChatEditData.strNameData, PlayerName, CHATEDIT_NAMEDATA_LENGTH );
	MStrCpy( ChatEditData.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	ChatEditData.siChatKind = MSG_MASTERNOTIFYCHAT_RECV;
	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	m_pChatEdit->Insert(&ChatEditData);

	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->m_pDialog[ NRESIDENTS_DLG ] )
	{
		((CNResidentsDlg*)pclclient->m_pDialog[ NRESIDENTS_DLG ])->InsertChatEditData( &ChatEditData );
	}

	return;
}

void CNChatDlg::VillageNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}

	stChatEditData ChatEditData;
	ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_YELLOW;
	ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
	MStrCpy( ChatEditData.strNameData, PlayerName, CHATEDIT_NAMEDATA_LENGTH );
	MStrCpy( ChatEditData.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	ChatEditData.siChatKind = MSG_VILLAGENOTIFYCHAT_RECV;
	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	m_pChatEdit->Insert(&ChatEditData);

	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->m_pDialog[ NRESIDENTS_DLG ] )
	{
		((CNResidentsDlg*)pclclient->m_pDialog[ NRESIDENTS_DLG ])->InsertChatEditData( &ChatEditData );
	}

	return;
}

void CNChatDlg::ResidentsNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}

	stChatEditData ChatEditData;
	ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_YELLOW;
	ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
	MStrCpy( ChatEditData.strNameData, PlayerName, CHATEDIT_NAMEDATA_LENGTH );
	MStrCpy( ChatEditData.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	ChatEditData.siChatKind = MSG_RESIDENTSNOTIFYCHAT_RECV;
	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	m_pChatEdit->Insert(&ChatEditData);

	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->m_pDialog[ NRESIDENTS_DLG ] && ( ! pclclient->m_pDialog[ NRESIDENTS_DLG ]->IsShow() ) )
	{	
		TCHAR szVillageChatMsg[ MAX_CHAT_DATA_LENGTH + MAX_PLAYER_NAME + 20 ] = TEXT("");	// 20 은 기타 표시용 [] :  뭐 요런거랑 공백
		StringCchPrintf( szVillageChatMsg, MAX_CHAT_DATA_LENGTH + MAX_PLAYER_NAME + 20, TEXT("[%s]:%s"), PlayerName, ChatText  );
		pclclient->pclMessage->SetVillageChatMsg( szVillageChatMsg );
	}

	if ( pclclient->m_pDialog[ NRESIDENTS_DLG ] )
	{
		((CNResidentsDlg*)pclclient->m_pDialog[ NRESIDENTS_DLG ])->InsertChatEditData( &ChatEditData );
	}

	return;
}

void CNChatDlg::KingNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}

	stChatEditData ChatEditData;
	ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_YELLOW;
	ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
	MStrCpy( ChatEditData.strNameData, PlayerName, CHATEDIT_NAMEDATA_LENGTH );
	MStrCpy( ChatEditData.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	ChatEditData.siChatKind = MSG_KINGNOTIFYCHAT_RECV;
	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	m_pChatEdit->Insert(&ChatEditData);

	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->m_pDialog[ NRESIDENTS_DLG ] )
	{
		((CNResidentsDlg*)pclclient->m_pDialog[ NRESIDENTS_DLG ])->InsertChatEditData( &ChatEditData );
	}

	return;
}

void CNChatDlg::GoodsProductionNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
{
	if ( CheckData( PlayerName, ChatText ) == false )
	{
		return;
	}

	stChatEditData ChatEditData;
	ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_YELLOW;
	ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
	MStrCpy( ChatEditData.strNameData, PlayerName, CHATEDIT_NAMEDATA_LENGTH );
	MStrCpy( ChatEditData.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	ChatEditData.siChatKind = MSG_GOODSPRODUCTIONNOTIFYCHAT_RECV;
	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	m_pChatEdit->Insert(&ChatEditData);

	return;
}

void CNChatDlg::SetOpenedChatDlg( bool Open )
{
	cltClient *pclclient = (cltClient *)pclClient;
	m_bOpened = Open;
#ifdef _DEBUG
	if( Open )
	{
		SetFileName( TEXT("NInterface/Interface/Interface_30x30_00_109.SPR") );
		SetFontIndex( 0 );
		SetBKTileImgNum( 9 );

		//SetMoveable( true );
		//SetHaveCaption( true );

		m_pChatEdit->Show( true );

		TCHAR* pText = GetTxtFromMgr(5095);
		m_pOpenButton->SetUseToolTip( pText );

		// 1024 X 768
		if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		{
			SetDialogPos( GetX(), pclclient->GetScreenYSize() - 49 - 153, 430, 180 );
		}
		// 800 X 600
		else
		{
			SetDialogPos( GetX(), 591 - 153, 328, 180 );
		}

		// 1024 X 768
		if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		{
			m_pComboBox->MoveControl( 10, 157 );
			m_pInputEdit->MoveControl( 85, 157 );
			m_pOpenButton->MoveControl( 390, 157 );
		}
		// 800 X 600
		else
		{
			m_pComboBox->MoveControl( 10, 157 );
			m_pInputEdit->MoveControl( 85, 157 );
			m_pOpenButton->MoveControl( 287, 157 );
		}

		SetTopMostWindow( false );
	}
	else
	{
		SetFileName( TEXT("NInterface/Interface/INTERFACE_10x27_00_103.SPR") );
		SetFontIndex( 0 );
		SetBKTileImgNum( 3 );

		SetHaveCaption( false );
		SetMoveable( false );

		TCHAR* pText = GetTxtFromMgr(3414);
		m_pOpenButton->SetUseToolTip( pText );

		// 1024 X 768
		if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		{
			SetDialogPos( 100, pclclient->GetScreenYSize() - 49, 430, 27 );

			m_pComboBox->MoveControl( 10, 4 );
			m_pInputEdit->MoveControl( 85, 4 );
			m_pOpenButton->MoveControl( 390, 4 );
		}
		// 800 X 600
		else
		{
			SetDialogPos( 100, 591, 328, 27 );

			m_pComboBox->MoveControl( 10, 4 );
			m_pInputEdit->MoveControl( 85, 4 );
			m_pOpenButton->MoveControl( 287, 4 );
		}

		m_pChatEdit->Show( false );

		SetTopMostWindow( true );
	}
#else
	if( Open )
	{
		SetFileName(  TEXT("NInterface/Interface/Interface_30x30_00_109.SPR") );
		SetFontIndex( 0 );
		SetBKTileImgNum( 9 );

		//SetMoveable( true );
		//SetHaveCaption( true );

		m_pChatEdit->Show( true );

		TCHAR* pText = GetTxtFromMgr(5095);
		m_pOpenButton->SetUseToolTip( pText );

		// 1024 X 768
		if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		{
			SetDialogPos( GetX(), pclclient->GetScreenYSize() - 49 - 153, 430, 180 );
		}
		// 800 X 600
		else
		{
			SetDialogPos( GetX(), 591 - 153, 328, 180 );
		}

		// 1024 X 768
		if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		{
			m_pComboBox->MoveControl( 10, 157 );
			m_pInputEdit->MoveControl( 85, 157 );
			m_pOpenButton->MoveControl( 390, 157 );
		}
		// 800 X 600
		else
		{
			m_pComboBox->MoveControl( 10, 157 );
			m_pInputEdit->MoveControl( 85, 157 );
			m_pOpenButton->MoveControl( 287, 157 );
		}

		SetTopMostWindow( false );
	}
	else
	{
		SetFileName(  TEXT("NInterface/Interface/INTERFACE_10x27_00_103.SPR") );
		SetFontIndex( 0 );
		SetBKTileImgNum( 3 );

		SetHaveCaption( false );
		SetMoveable( false );

		TCHAR* pText = GetTxtFromMgr(3414);
		m_pOpenButton->SetUseToolTip( pText );

		// 1024 X 768
		if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		{
			SetDialogPos( 100, pclclient->GetScreenYSize() - 49, 430, 27 );

			m_pComboBox->MoveControl( 10, 4 );
			m_pInputEdit->MoveControl( 85, 4 );
			m_pOpenButton->MoveControl( 390, 4 );
		}
		// 800 X 600
		else
		{
			SetDialogPos( 100, 591, 328, 27 );

			m_pComboBox->MoveControl( 10, 4 );
			m_pInputEdit->MoveControl( 85, 4 );
			m_pOpenButton->MoveControl( 287, 4 );
		}

		m_pChatEdit->Show( false );

		SetTopMostWindow( true );
	}
#endif
	/*
	if( Open )
	{
	SetFileName(  TEXT("NInterface/Interface/Interface_30x30_00_109.SPR") );
	SetFontIndex( 0 );
	SetBKTileImgNum( 9 );

	//SetMoveable( true );
	//SetHaveCaption( true );

	m_pChatEdit->Show( true );

	TCHAR* pText = GetTxtFromMgr(5095);
	m_pOpenButton->SetUseToolTip( pText );

	// 1024 X 768
	if ( gsiResolution == 2 )
	{
	SetDialogPos( GetX(), pclclient->GetScreenYSize() - 49 - 153, 430, 180 );
	}
	// 800 X 600
	else
	{
	SetDialogPos( GetX(), 591 - 153, 328, 180 );
	}

	// 1024 X 768
	if ( gsiResolution == 2 )
	{
	m_pComboBox->MoveControl( 10, 157 );
	m_pInputEdit->MoveControl( 85, 157 );
	m_pOpenButton->MoveControl( 390, 157 );
	}
	// 800 X 600
	else
	{
	m_pComboBox->MoveControl( 10, 157 );
	m_pInputEdit->MoveControl( 85, 157 );
	m_pOpenButton->MoveControl( 287, 157 );
	}

	SetTopMostWindow( false );
	}
	else
	{
	SetFileName(  TEXT("NInterface/Interface/INTERFACE_10x27_00_103.SPR") );
	SetFontIndex( 0 );
	SetBKTileImgNum( 3 );

	SetHaveCaption( false );
	SetMoveable( false );

	TCHAR* pText = GetTxtFromMgr(3414);
	m_pOpenButton->SetUseToolTip( pText );

	// 1024 X 768
	if ( gsiResolution == 2 )
	{
	SetDialogPos( 100, pclclient->GetScreenYSize() - 49, 430, 27 );

	m_pComboBox->MoveControl( 10, 4 );
	m_pInputEdit->MoveControl( 85, 4 );
	m_pOpenButton->MoveControl( 390, 4 );
	}
	// 800 X 600
	else
	{
	SetDialogPos( 100, 591, 328, 27 );

	m_pComboBox->MoveControl( 10, 4 );
	m_pInputEdit->MoveControl( 85, 4 );
	m_pOpenButton->MoveControl( 287, 4 );
	}

	m_pChatEdit->Show( false );

	SetTopMostWindow( true );
	}
	*/
	return;
} 




void CNChatDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	switch( MsgIndex )
	{
	case 0:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{

				SI16* chatmode= (SI16*)pData;
				SI32* systemtxt= (SI32*)&pData[2];
				TCHAR* pmsg =(TCHAR*)&pData[6];			

				// 채팅 블록
				if( pclClient->pclCM->GetCharClient(1)->m_stMuteTime.wYear > 0 )
				{
					NTCHARString256		kText(GetTxtFromMgr(9626));
					NTCHARString256		kTitle(GetTxtFromMgr(5614));

					SI64 siTimeDiff = TimeOperation( &pclClient->pclCM->GetCharClient(1)->m_stMuteTime, &pclclient->sTime );
					if( siTimeDiff > 0 )
					{
						kText.Replace("#minute#", SI32ToString(siTimeDiff / (uiSec * 60)));
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( kTitle, kText);
					}
				}
				else
				{
					cltGameMsgRequest_Chat clinfo(*chatmode,*systemtxt,pmsg);
					cltMsg clMsg(GAMEMSG_REQUEST_CHAT, clinfo.siDataSize, (BYTE*)&clinfo);
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);				
				}
			}


		}
		break;
	}
	return;
}

void CNChatDlg::ChatMessage(SI16 chatmode, SI32 systemtxt, TCHAR* pmsg)
{
	cltClient *pclclient = ( cltClient * )pclClient;

	TCHAR TempBuffer[ 256 ] = TEXT("");
	TCHAR Buffer[ 128 ] = TEXT("");
	TCHAR* pTitle = GetTxtFromMgr(1856);
	TCHAR* pText = GetTxtFromMgr(1857);	
	StringCchCopy(Buffer, 128, pmsg);


	stMsgBox MsgBox;
	MsgBox.Set( this,  pTitle, pText, MSGBOX_TYPE_YESNO, 0 );

	SI16 Size = sizeof( chatmode ) + sizeof(systemtxt )+ sizeof(Buffer);
	memcpy( TempBuffer, &chatmode, sizeof(chatmode ) );
	memcpy( &TempBuffer[ sizeof( chatmode ) ], &systemtxt, sizeof( systemtxt) );
	memcpy( &TempBuffer[ sizeof( chatmode )+ sizeof(systemtxt )], &Buffer, sizeof( Buffer) );

	pclclient->SetMsgBox( &MsgBox, (BYTE*)TempBuffer, Size );
}

SI32 CNChatDlg::CompareKey(TCHAR *data, SI32 type)
{
	switch(type)
	{
	case 0:
		{
			if(_tcsncmp(data,TEXT("@"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_RESIDENTSCHAT;
			}
			else if(_tcsncmp(data,TEXT("#"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_GUILDCHAT;
			}
			else if(_tcsncmp(data,TEXT("$"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_PARTYCHAT;
			}
			else
				return TAB_MODE_DEAULT;
		}
		break;

	case 1:
		{
			if(_tcsncmp(data,TEXT("!"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_DEAULT;
			}
			else if(_tcsncmp(data,TEXT("#"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_GUILDCHAT;
			}
			else if(_tcsncmp(data,TEXT("$"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_PARTYCHAT;
			}
			else
				return TAB_MODE_RESIDENTSCHAT;
		}
		break;

	case 2:
		{
			if(_tcsncmp(data,TEXT("!"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_DEAULT;
			}
			else if(_tcsncmp(data,TEXT("@"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_RESIDENTSCHAT;
			}
			else if(_tcsncmp(data,TEXT("$"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_PARTYCHAT;
			}
			else
				return TAB_MODE_GUILDCHAT;
		}
		break;
	case 3:
		{
			if(_tcsncmp(data,TEXT("!"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_DEAULT;
			}
			else if(_tcsncmp(data,TEXT("@"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_RESIDENTSCHAT;
			}
			else if(_tcsncmp(data,TEXT("#"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_GUILDCHAT;
			}
			else
				return TAB_MODE_PARTYCHAT;
		}
		break;
	default:
		return TAB_MODE_DEAULT;
		break;
	}
}

