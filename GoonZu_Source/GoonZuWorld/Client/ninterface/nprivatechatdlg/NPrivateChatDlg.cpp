#include "NPrivateChatDlg.h"
#include "NPrivateChatDlgMgr.h"
#include "Client.h" //[영진]

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ChatEdit.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../ninterface/npartydlg/NPartyDlg.h"
#include "../../ninterface/nletterdlg/NLetterDlg.h"

#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/MsgType-system.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../AbuseFilter/AbuseFilter.h"
#include "../../Music/music.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../../Client.h"

#include "../NHelpButton/NHelpButtonDlg.h"
#include "../NMarriageLoveLevelDlg/NMarriageLoveLevelDlg.h"
#include "../NFamilyCookieDlg/NFamilyCookieDlg.h"
#include "../NFamilyInfoDlg/NFamilyInfoDlg.h"
#include "../NDormancyAccount/NDormancyPointDlg.h"

extern cltCommonLogic* pclClient;

CNPrivateChatDlg::CNPrivateChatDlg() : CDialogController()
{
	m_pChatEdit = NULL;
	m_pInputEdit = NULL;

	m_pSendChatButton = NULL;
	m_pRejectButton = NULL;
	m_pRejectListButton = NULL;
	m_pImageMinimizedButton = NULL;

	m_pImageStatic = NULL;
	
	m_bMini = FALSE;
	
	m_strTitle[0] = NULL;

	m_siCharUnique = 0;
	
	m_siOldPosX = 0;
	m_siOldPosY = 0;

	m_siOldXSize = 0;
	m_siOldYSize = 0;


	m_strUserName = new TCHAR[ MAX_PLAYER_NAME ];
	memset( m_strUserName, 0, MAX_PLAYER_NAME );
	m_bAccept = false;

//	m_AcceptDlgBox = NULL;
}

CNPrivateChatDlg::~CNPrivateChatDlg()
{
	SAFE_DELETE( m_pMinimizeButton);

	SAFE_DELETE( m_pChatEdit );
	SAFE_DELETE( m_pInputEdit );

	SAFE_DELETE( m_pSendChatButton );
	SAFE_DELETE( m_pRejectButton );
	SAFE_DELETE( m_pRejectListButton );
	SAFE_DELETE( m_pImageMinimizedButton);

	SAFE_DELETE( m_pImageStatic );
	
	//SAFE_DELETE( m_pImageMinimized );
	SAFE_DELETE( m_pSaveLogCheckbox)

	SAFE_DELETE_A( m_strUserName );
	
}

void CNPrivateChatDlg::Create( SI16 PrivateChatDlgID )
{
	if( IsCreate() )
	{
		DeleteDialog();
	}
	else
	{
		m_pChatEdit = new CChatEdit( this );
		m_pInputEdit = new CEdit( this );

		m_pSendChatButton = new CButton( this );
		m_pRejectButton = new CButton( this );
		m_pRejectListButton = new CButton( this );
		m_pImageMinimizedButton = new CButton ( this );

		m_pImageStatic = new CStatic( this );
		
		m_pMinimizeButton = new CButton( this );
		
		//m_pImageMinimized = new CImageStatic( this );

		m_pSaveLogCheckbox = new CCheckBox( this );


		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NPrivateChatDlg/DLG_NPrivateChat.ddf"));
		file.CreatePopupDialog( this, PrivateChatDlgID, TEXT("dialog_PrivateChat"), StaticCallBackDialogNPrivateChat,false );

		//m_pChatEdit->Create( NPRIVATECHAT_DIALOG_CHAEDIT, NULL, TEXT("NInterface/Common/COMMON_24x24_00_109.SPR"), 0, true, 9, 8, 98, 336, 120, 336, 120 );
		file.CreateControl( m_pChatEdit,			NPRIVATECHAT_DIALOG_CHAEDIT,			TEXT("chatedit_PrivateChat") );
		file.CreateControl( m_pInputEdit,			NPRIVATECHAT_DIALOG_EDIT_INPUT,			TEXT("editbox_PrivateChatInputEdit") );
		file.CreateControl( m_pSendChatButton,		NPRIVATECHAT_DIALOG_BUTTON_SENDCHAT,	TEXT("button_SendChat") );
		file.CreateControl( m_pRejectButton,		NPRIVATECHAT_DIALOG_BUTTON_REJECT,		TEXT("button_Reject") );
		file.CreateControl( m_pRejectListButton,	NPRIVATECHAT_DIALOG_BUTTON_REJECTLIST,	TEXT("button_RejectList") );
		file.CreateControl( m_pImageStatic,			NPRIVATECHAT_DIALOG_STATIC_IMAGE,		TEXT("statictext_NONAME1") );
		file.CreateControl( m_pMinimizeButton,		NPRIVATECHAT_DIALOG_BUTTON_MINIMIZE,	TEXT("button_minimize") );
		file.CreateControl( m_pImageMinimizedButton,		NPRIVATECHAT_DIALOG_BUTTON_IMAGEMINIMIZE,	TEXT("button_imageminimized") );

		//file.CreateControl( m_pImageMinimized,			NPRIVATECHAT_DIALOG_STATIC_IMAGENOTE,	TEXT("imagestatic_imagenote") );
	
		file.CreateControl( m_pSaveLogCheckbox,		NPRIVATECHAT_DIALOG_CHECKBOX_SAVELOG,	TEXT("checkbox_SaveLog") );

		//m_pImageMinimized->Show(false);
		
		//m_pMinimizeButton->Show(false);

		m_pImageMinimizedButton->Show(false);
		m_pInputEdit->SetAutoScroll( true );
		
		pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_WHISPERCHAT")), 0, 0 );

		TCHAR* pText = GetTxtFromMgr(5955);
		m_pSaveLogCheckbox->SetText(pText,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		m_pCloseButton->Move(0,7);

		// 화면 중앙에 위치 시킨다.
		cltClient* pclclient = (cltClient*)pclClient;
		SI16 xPos = ( pclclient->GetScreenXSize() - GetWidth() ) / 2 + 40;
		SI16 yPos = ( pclclient->GetScreenYSize() - GetHeight() ) / 2;
		MovePos( xPos, yPos );

		SetActionTimer(1000);

	}
}

void CALLBACK CNPrivateChatDlg::StaticCallBackDialogNPrivateChat( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNPrivateChatDlg *pThis = (CNPrivateChatDlg*)pControllerMgr;
	pThis->CallBackDialogNPrivateChat( nEvent, nControlID, pControl );
	return;
}

void CALLBACK CNPrivateChatDlg::CallBackDialogNPrivateChat( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*)pclClient;

switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:		DeleteDialog();		break;
	case NPRIVATECHAT_DIALOG_BUTTON_IMAGEMINIMIZE:
		{
			switch( nEvent )
			{		
			case EVENT_BUTTON_CLICKED:
				{
					Minimi(FALSE);
					pclclient->m_pNPrivateChatDlgMgr->SetMinimizeAllDialog(FALSE);
				}
				break;
			}
		}
		break;

	case NPRIVATECHAT_DIALOG_BUTTON_MINIMIZE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					Minimi(TRUE);
					//pclclient->m_pNPrivateChatDlgMgr->SetMinimizeAllDialog(TRUE);
				}
			}
		}
		break;

	case NPRIVATECHAT_DIALOG_EDIT_INPUT:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{	
					cltCharClient* pclCharClient = pclclient->pclCM->GetCharClient(1)	;	

					if ( pclCharClient != NULL )
					{
						if(pclCharClient->m_stMuteTime.wMinute > 0 )	// 채금 걸리면 채팅을 날릴수 없도록 귓속말 보내기 전에채금인지 확인 한다.
						{
							return	;
						}
						else
						{
							SendChat();
						}
					}
				}
				break;
			}
		}
		break;
	case NPRIVATECHAT_DIALOG_BUTTON_SENDCHAT:
		SendChat();
		break;
	case NPRIVATECHAT_DIALOG_BUTTON_REJECT:
		{
			cltClient *pclclient = (cltClient *)pclClient;
			if ( pclclient->m_pNPrivateChatDlgMgr )
			{
				pclclient->m_pNPrivateChatDlgMgr->AddRejectUser( m_strUserName );
			}
		}
		break;
	case NPRIVATECHAT_DIALOG_BUTTON_REJECTLIST:
		{
			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->CreateInterface( NPRIVATECHATREJECT_DLG );
		}
		break;
	case NPRIVATECHAT_DIALOG_CHECKBOX_SAVELOG:
		{
			break;
		}

	}
}

void CNPrivateChatDlg::Action()
{
	cltClient * pclclient = (cltClient*)pclClient;

    if(isMini())
	{
	 	if(  pclclient->m_pNPrivateChatDlgMgr->m_bNewMessage )
		{
			//// 0 과 1이 반복됨
			//SI32 imgfont = m_pImageMinimized->GetFontIndex() + 1;
			//if(imgfont >= 4 ) imgfont = 2; 
			//m_pImageMinimized->SetFontIndex( imgfont );

            //m_pImageMinimized->SetFontIndex( m_pImageMinimized->GetFontIndex() + 7);
		}
	}
}

void CNPrivateChatDlg::RecvChat( TCHAR *PlayerName, TCHAR *ChatText, BOOL bGameMaster )
{
	stChatEditData ChatEditData;
	
	TCHAR* pText1 = GetTxtFromMgr(244);
	TCHAR* pText2 = GetTxtFromMgr(599);

	if ( TRUE == bGameMaster )
	{
		ChatEditData.siChatDataColorIndex	= CHATEDIT_COLOR_VIOLET;
		ChatEditData.siNameDataColorIndex	= CHATEDIT_COLOR_VIOLET;
	}
	else
	{
		if ( !_tcscmp(PlayerName,pText2) || !_tcscmp(PlayerName,pText1) )
		{
			ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_BLUE;
		}
		else
		{
			ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_BLACK;
		}
		ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_BLUE;
	}

	MStrCpy( ChatEditData.strNameData, PlayerName, MAX_PLAYER_NAME );
	MStrCpy( ChatEditData.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	//ChatEditData.siChatKind = 0;
	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	m_pChatEdit->Insert( &ChatEditData );

	//if(m_pSaveLogCheckbox->IsCheck())
	//{
	//	SaveChatLog(PlayerName, ChatText);
	//}
	return;
}

void CNPrivateChatDlg::SendChat()
{
	cltClient*		pclclient	= (cltClient *)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient*	pclChatChar	= pclclient->pclCM->GetCharClient(1);
	if ( NULL == pclChatChar )
	{
		return;
	}

	TCHAR *ChatText = m_pInputEdit->GetText();

	if ( ChatText == NULL || _tcslen( ChatText ) < 1 )
	{
		return;
	}	
	// 보내는 메세지는 가능하게.. tys

	//if( g_SoundOnOffDlg.m_bWhisperChatReject )
	//{
	//	TCHAR* pTitle = GetTxtFromMgr(2395);
	//	TCHAR* pText = GetTxtFromMgr(2396);
	//	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
	//	return;
	//}
#ifdef _IAF_EXPORT
	if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
	{
		TCHAR ResultBuf[1024] = "";
		if( 1 == IAF::Filtering( ChatText, ResultBuf, sizeof(ResultBuf) ) )
		{
			TCHAR* pTitle = GetTxtFromMgr(1853);
			TCHAR* pText = GetTxtFromMgr(1854);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			m_pInputEdit->SetText(TEXT(""));
			return;
		}
	}
#else
	if ( pclclient->m_pCAbuseFilter->CheckAbuse( ChatText ) == false )
	{
		TCHAR* pTitle = GetTxtFromMgr(1853);
		TCHAR* pText = GetTxtFromMgr(1854);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		m_pInputEdit->SetText(TEXT(""));
		return;
	}
#endif


	BOOL bGameMaster = FALSE;

	if ( (BASIC_GAMEMASTER_LEVEL <= pclChatChar->GetGameMasterLevel()) && (GMMODE_ATB_GMSHOW == pclChatChar->pclCI->clBI.uiGMMode) )
	{
		bGameMaster = TRUE;
	}

	RecvChat( (TCHAR*)pclClient->pclCM->CR[1]->GetName(), ChatText, bGameMaster );
	SaveChatLog((TCHAR*)pclClient->pclCM->CR[1]->GetName(), m_strUserName, ChatText);

	if ( pclclient->m_pNPrivateChatDlgMgr )
	{
		stWhisperChatDataSend WhisperSendData;
		WhisperSendData.siToID = m_siCharUnique;
		StringCchCopy( WhisperSendData.szToCharName, MAX_PLAYER_NAME, m_strUserName );
		StringCchCopy( WhisperSendData.strChatData, MAX_CHAT_DATA_LENGTH, ChatText );
		pclclient->m_pNPrivateChatDlgMgr->SendChat( &WhisperSendData );
	}

	m_pInputEdit->SetText(TEXT(""));
	return;
}

void CNPrivateChatDlg::Set( SI32 siCharUnique, TCHAR *strUserName ,SI32 level)
{	
	m_siCharUnique = siCharUnique;
	m_pChatEdit->Clear();

	if ( strUserName != NULL )
	{
		TCHAR buffer[256] = TEXT("") ;
		MStrCpy ( m_strUserName, strUserName, MAX_PLAYER_NAME );
		StringCchPrintf(buffer, 256, TEXT("         %s ( Lv.%d )"),strUserName,level);
		StringCchCopy(m_strTitle,256,buffer);

#ifndef _NEW_INTERFACE_
		SetTitle( buffer);
#else
		SetTitle( buffer, CONTROLLER_TITLE_COLOR,BASE_FONT_HEIGHT,0,7 );
#endif
	}

	return;
}

bool CNPrivateChatDlg::IsExist( SI32 siCharUnique, TCHAR *strUserName )
{
	// fail을 위한 용
	if ( (m_siCharUnique == siCharUnique) && (NULL == strUserName) )
	{
		return true;
	}

	// 체크하기 위한 이름
	if ( strUserName == NULL )
	{
		return false;
	}

	if ( _tcslen( strUserName ) < 1 )
	{
		return false;
	}

	// 원래 저장되어있던 이름
	if ( m_strUserName == NULL )
	{
		return false;
	}

	if ( _tcslen( m_strUserName ) < 1 )
	{
		return false;
	}

	// 케릭터 Unique와 이름이 같다면
	if ( (m_siCharUnique == siCharUnique) && (_tcscmp( strUserName, m_strUserName ) == 0) )
	{
		m_siCharUnique = siCharUnique;
		return true;
	}

	return false;
}

void CNPrivateChatDlg::Reject()
{	
	cltClient *pclclient = (cltClient *)pclClient;

	if ( pclclient->m_pNPrivateChatDlgMgr )
	{
		pclclient->m_pNPrivateChatDlgMgr->AddRejectUser( m_strUserName );
	}

	return;
}


void CNPrivateChatDlg::Minimi(BOOL bMinimize)
{
	if(m_bMini == bMinimize)	return;
	cltClient *pclclient = (cltClient*)pclClient;
	


	m_bMini = bMinimize;

    if(m_bMini == TRUE)
	{	
		m_siOldPosX = GetX();
		m_siOldPosY	= GetY();

		m_siOldXSize = GetWidth();
		m_siOldYSize = GetHeight();

		SetMoveable(FALSE);
		SetFileName(NULL);
		SetTitle(NULL);


		SI16 PartyMinimizedSlot = 0, LetterMinimizedSlot = 0, HelpButtonMiniMizedSlot = 0, LoveLevelMiniMizedSlot = 0 , FamilyCookieEffect = 0 , PrivateChatMinimizedSlot = 0, FamilyMini = 0, siDormancyMiniMizedSlot = 0;

		if( pclclient->m_pDialog[NPARTY_DLG])
			PartyMinimizedSlot = ((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->GetMinimizedSlot();

		if( pclclient->m_pDialog[NLETTER_DLG])
			LetterMinimizedSlot = ((CNLetterDlg*)pclclient->m_pDialog[NLETTER_DLG])->GetMinimizedSlot();

		if(pclClient->IsCountrySwitch(Switch_FreshManHelpDialog))
		{
			if( pclclient->m_pDialog[NHELPBUTTON_DLG])
				HelpButtonMiniMizedSlot = ((CNHelpButtonDlg*)pclclient->m_pDialog[NHELPBUTTON_DLG])->GetMinimizedSlot();
		}

		if ( pclclient->m_pDialog[NMARRIAGELOVELEVEL_DLG] )
			LoveLevelMiniMizedSlot = ((CNMarriageLoveLevelDlg*)pclclient->m_pDialog[NMARRIAGELOVELEVEL_DLG])->GetMinimizedSlot();

		if ( pclclient->m_pDialog[NFAMILYCOOKIELEVEL_DLG] )
			FamilyCookieEffect = ((CNFamilyCookieDlg*)pclclient->m_pDialog[NFAMILYCOOKIELEVEL_DLG])->GetMinimizedSlot();
		if( pclclient->m_pNPrivateChatDlgMgr)
			PrivateChatMinimizedSlot = pclclient->m_pNPrivateChatDlgMgr->GetMinimizedSlot();

		if ( pclclient->m_pDialog[NFAMILYINFO_DLG] )
			FamilyMini = ((CNFamilyInfoDlg*)pclclient->m_pDialog[NFAMILYINFO_DLG])->GetMinimizedSlot();

		if ( pclclient->m_pDialog[NDORMANCYPOINT_DLG] )
			siDormancyMiniMizedSlot = ((CNDormancyPointDlg*)pclclient->m_pDialog[NDORMANCYPOINT_DLG])->GetMinimizedSlot();


		// 빈 공간을 찾는다.
		for(SI16 i = 1; i <= MAX_MINIMIZE_SLOT; i++)
		{
			if( i == PartyMinimizedSlot || i == LetterMinimizedSlot || i == HelpButtonMiniMizedSlot || i == LoveLevelMiniMizedSlot || i == FamilyCookieEffect  || i == FamilyMini || i == siDormancyMiniMizedSlot)
				continue;

			pclclient->m_pNPrivateChatDlgMgr->SetMinimizedSlot(i);
			break;
		}

		// 슬롯에 따른 x 좌표로 움직여 준다.
 		SetDialogPos( pclclient->GetScreenXSize() - 40 - (pclclient->m_pNPrivateChatDlgMgr->GetMinimizedSlot() -1) * 45 , 215, 34, 34 );

//		m_pImageMinimized->Show(true);

		m_pChatEdit->Show(false);
		m_pInputEdit->Show(false);

		m_pSendChatButton->Show(false);
		m_pRejectButton->Show(false);
		m_pRejectListButton->Show(false);
		m_pImageMinimizedButton->Show(true);

		m_pMinimizeButton->Show(false);
		m_pImageStatic->Show(false);
		m_pSaveLogCheckbox->Show(false);
	}
	else
	{
		SetMoveable(TRUE);
		SetDialogPos( m_siOldPosX, m_siOldPosY, m_siOldXSize, m_siOldYSize );
		SetFileName("ninterface/interface/interface_44x44_00_109.spr");
		
		if ( m_strTitle[0] != NULL )
		{
			SetTitle( m_strTitle, CONTROLLER_TITLE_COLOR,BASE_FONT_HEIGHT,0,7 );
		}
		//m_pImageMinimized->Show(false);
		m_pChatEdit->Show(true);
		m_pInputEdit->Show(true);

		m_pSendChatButton->Show(true);
		m_pRejectButton->Show(true);
		m_pRejectListButton->Show(true);
		m_pImageMinimizedButton->Show(false);

		m_pMinimizeButton->Show(true);
		m_pImageStatic->Show(true);
		m_pSaveLogCheckbox->Show(true);
	}	
}

void CNPrivateChatDlg::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen)  //[영진]
{
	if ( RetValue )
	{
		m_bAccept = true;
		// 귓속말 보여주기
		cltClient *pclclient = (cltClient *)pclClient;
		Show(TRUE);

		cltGameMsgRequest_AcceptSecretChatChannel clinfo( true, m_siCharUnique, m_strUserName );
		cltMsg clMsg(GAMEMSG_REQUEST_ACCEPTSECRETCHATCHANNEL, sizeof(clinfo), (BYTE*)&clinfo);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
	else
	{
		m_bAccept = false;

		cltClient *pclclient = (cltClient *)pclClient;
		//귓속말 받기 거부 메시지 서버로

		cltGameMsgRequest_AcceptSecretChatChannel clinfo( false, m_siCharUnique, m_strUserName );
		cltMsg clMsg(GAMEMSG_REQUEST_ACCEPTSECRETCHATCHANNEL, sizeof(clinfo), (BYTE*)&clinfo);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
		
		DeleteDialog(); //[영진]
	}
}

void CNPrivateChatDlg::SaveChatLog(TCHAR* SenderName, TCHAR* RecieverName, TCHAR* ChatText)
{
	// 대화 내용 파일로 저장 - tys
	if(m_pSaveLogCheckbox->IsCheck())
	{
		DWORD dwAttr;

		if((dwAttr = GetFileAttributes(TEXT("ChatLog//"))) == (DWORD)-1)
		{// 디렉토리나 파일이 존재하지 않음. 따라서 디렉토리를 생성함
			CreateDirectory(TEXT("ChatLog\\"),NULL);
		}

		TCHAR buffer[16];
		TCHAR fileName[MAX_PATH];

		time_t ltime;
		struct tm *today;
		time( &ltime );
		today = localtime( &ltime );
		struct _timeb tstruct;
		_ftime( &tstruct );

		StringCchPrintf(fileName, MAX_PATH, TEXT("ChatLog/%s_%d%02d%02d.txt"), 
							(TCHAR*)pclClient->pclCM->CR[1]->GetName(), 
							today->tm_year+1900, 
							today->tm_mon+1, 
							today->tm_mday);

		StringCchPrintf(buffer, 16, TEXT("[%02d:%02d:%02d]"), 
							today->tm_hour, 
							today->tm_min, 
							today->tm_sec);

		FILE *fp = _tfopen( fileName, TEXT("a") );
		_ftprintf( fp, TEXT("%s\t%s\t%s\t%s\n"), buffer, SenderName, RecieverName, ChatText);

		fclose( fp );
	}

}

void CNPrivateChatDlg::ReturnFalseSend()
{
	stChatEditData ChatEditData;

		if(m_strUserName != NULL && m_strUserName[0] != '\0')
		{
			TCHAR *pText = GetTxtFromMgr( 6088 );
			ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_BLUE;

			StringCchPrintf( ChatEditData.strChatData,MAX_CHAT_DATA_LENGTH , pText , m_strUserName );
			ChatEditData.siChatEditDataSize = sizeof( stChatEditData );
		}
		else
		{
			TCHAR *pText = GetTxtFromMgr( 6089 );
			ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_BLUE;

			StringCchPrintf( ChatEditData.strChatData,MAX_CHAT_DATA_LENGTH , pText , m_strUserName );
			ChatEditData.siChatEditDataSize = sizeof( stChatEditData );
		}
		

	m_pChatEdit->Insert( &ChatEditData );
}

void CNPrivateChatDlg::SetHelper( bool _Helper )
{
	if( false ==  IsCreate() ) return;

	if( _Helper )
	{
		m_pMinimizeButton->Show(false);
		m_pImageMinimizedButton->Show(false);
	}
}