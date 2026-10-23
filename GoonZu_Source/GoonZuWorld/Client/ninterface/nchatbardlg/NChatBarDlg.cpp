#include "NChatBarDlg.h"

#include "../../../Client/ItemInfo/ItemInfo.h"

#include "../../Inventory/ItemInventory.h"

#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../../Interface/NInventory/NInventory.h"
//#include "../../Interface/MonsterInfoDlg/MonsterInfoDlg.h"

#include "../../Client.h"

#include "../../Prompt/PromptMgr.h"

#include "../../InterfaceMgr/Interface/ChatEdit.h"
#include "../../InterfaceMgr/Interface/ChatTabButton.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../StatusBar/ChatStatusBar/ChatStatusBarBuffer.h"
//#include "../NChatDlg/NChatDlg.h"

#include "../../NInterface/NMonsterInfoDlg/NMonsterInfoDlg.h"

#include "../../Interface/ResidentsViewDlg/ResidentsViewDlg.h"
#include "../../Interface/FriendDlg/FriendDlg.h"
#include "../../NInterface/NFriendDlg/NFriendDlg.h"

#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../../NInterface/NItemInfoDlg/ItemInfoDlg.h"
#include "../../NInterface/NHomePageDlg/HomePageDlg.h"
#include "../../NInterface/NHuntMapInfoDlg/HuntMapInfoDlg.h"
#include "../../NInterface/NResidentsInfoDlg/ResidentsInfoDlg.h"
#include "../../NInterface/NMagicInfoDlg/MagicInfoDlg.h"
#include "../../NInterface/NTeacherDlg/TeacherDlg.h"
#include "../../NInterface/NTeacherDlg/FatherDlg.h"
#include "../../NInterface/NTeacherDlg/ChildDlg.h"
#include "../../NInterface/NItemMallManager/NItemMallManager1.h"
#include "../../ninterface/ncharmenubardlg/NCharMenuBarDlg.h"
#include "../../ninterface/npartydlg/NPartyDlg.h"

#include "../../NInterface/NCommonMsgDlg/NCommonMsgDlg.h"
#include "../../ninterface/NEmoticonDlg/NEmoticonDlg.h"
#include "../../ninterface/NSetChatDlg/NSetChatDlg.h"

#include "../../../Common/Father/Father.h"
#include "../../../common/HuntMapInfo/HuntMapInfo.h"
#include "../../../common/Master/Master.h"

#include "../../../Common/Skill/Skill-Manager.h"
#include "../../../Common/Skill/Skill.h"

#include "../../NInterface/NMaster/NMaster.h"

#include "../../../common/HintDlg/HintDlg.h"

#include "../../../lib/ScreenShot/CaptureJPG.h"
#include "../../../NLib/NUtil.h"

// 마을 상세 정보 
#include "NInterface/NVillageDetailInfo/NVillageDetailInfo.h"
#include "NInterface/NResidentsDlg/NResidentsDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/Static.h"

#include "Msg/MsgType-PersonalShop.h"
#include "Msg/MsgType-ItemMall.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "NInterface/NCharStatus/NCharStatus.h"
#include "NInterface/NBossMarket/NBossMarketDlg.h"
#include "../Client/DLGPositionSave/DLGPositionSave.h"

#include "../../../CommonLogic/Guild/Guild.h"

#include "../../Client.h"

//KHY - 0110 - 채팅 모드 고정
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
extern CSoundOnOffDlg g_SoundOnOffDlg;

extern cltCommonLogic* pclClient;

extern SI32		gsiResolution;

// NUtil.cpp에 선언된 변수
extern UI64	uiSec; // 시간 계산을 위한 기준 변수

CNChatBarDlg::CNChatBarDlg() : CDialogController()
{
	m_pChatEdit = NULL;
	m_pInputEdit = NULL;
	m_pOpenButton = NULL;
	m_pEmoticonButton = NULL;
	m_pIMEButton = NULL;
	m_siSelectedButton = 0;
	m_OpendsiSize = CHATEDIT_OPEN_5LINES;
	m_siRepeatCnt = 0;

	for( SI16 i = 0; i < MAX_TABBUTTON; i++)
	{	
		m_pTabButton[i] = NULL;
	}

	m_pChatBarBuffer = new CChatStatusBarBuffer();
	m_strChatData = new TCHAR[ MAX_CHAT_DATA_LENGTH ];
	
	m_bSetGoonzu = false;

	m_bFirstCreate = false;

	/*m_bitChatstatus.ClearAll();*/
	m_pChatSetButton = NULL;
}

CNChatBarDlg::~CNChatBarDlg()
{

	//cltClient *pclclient = (cltClient *)pclClient;
	//g_DlgPositionSave.SetPosition( this->GetDialogControllerID(), this->GetX(), this->GetY() );


	SAFE_DELETE(m_pChatBarBuffer);
	SAFE_DELETE(m_strChatData);

	for( SI16 i = 0; i < MAX_TABBUTTON; i++)
	{	
		SAFE_DELETE(m_pTabButton[i]);
	}
	
	SAFE_DELETE(m_pOpenButton);
	SAFE_DELETE(m_pInputEdit);
	SAFE_DELETE(m_pChatEdit);
	SAFE_DELETE(m_pEmoticonButton);
	SAFE_DELETE(m_pIMEButton);
	SAFE_DELETE(m_pChatSetButton);
	
}

void CNChatBarDlg::Create()
{

	cltClient *pclclient = (cltClient *)pclClient;
	
	SI16 xPos  = MAP_TILE_XSIZE + (g_SoundOnOffDlg.m_bMaxWindowMode ? 32 : 0);
	SI16 yPos = pclclient->GetScreenYSize() - 55;
	SI16 Height = 26;

	// 1024x600
	if(gsiResolution == 4)
	{
		//Height = 16;
	}

	if(	pclClient->IsCountrySwitch( Switch_Char_Option ) )
	{
		CDialogController::Create( NCHATBAR_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar,
		NULL, TEXT("NInterface/Interface/INTERFACE_344X26_00_000.SPR"), 0, true, 3, 
		xPos,yPos,
		344	, Height, TRUE, false, false,true ); // 풀스크린이라면 32픽셀 오른쪽에 생성
	}
	else
	{
		CDialogController::Create( NCHATBAR_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar,
			NULL, TEXT("NInterface/Interface/INTERFACE_344x42_CHAT.SPR"), 0, true, 3, 
			xPos,yPos,
			344	, Height, TRUE, false, false,true ); // 풀스크린이라면 32픽셀 오른쪽에 생성
	}
	
	

	// 채팅 입력창
	m_pInputEdit = new CEdit( this );
	if(	pclClient->IsCountrySwitch( Switch_Char_Option ) )
	{
		m_pInputEdit->Create( NCHAT_DIALOG_EDIT_INPUT, NULL, NULL, 0, true, 0, 27, 6, 240, 17, 240, 17, false, false );
	}
	else
	{
		m_pInputEdit->Create( NCHAT_DIALOG_EDIT_INPUT, NULL, NULL, 0, true, 0, 27, 6, 260, 17, 260, 17, false, false );
	}
	m_pInputEdit->SetAutoScroll( true );
	m_pInputEdit->SetMaxEditTextLength( MAX_CHAT_DATA_LENGTH - 2 );

	//KHY - 1114 - 이모티콘 일본 추가.
//	if ( pclclient->siServiceArea != ConstServiceArea_Japan )
	{
		// 이모티콘 버튼
		m_pEmoticonButton = new CButton( this );
		if(	pclClient->IsCountrySwitch( Switch_Char_Option ) )
		{
			m_pEmoticonButton->Create( NCHAT_DIALOG_BUTTON_EMOTICON, NULL, TEXT("NInterface/Button/Button_22x18_00_000.SPR"), 0, false, 0, 275, 5, 22, 18, 22, 18 );
		}
		else{
			m_pEmoticonButton->Create( NCHAT_DIALOG_BUTTON_EMOTICON, NULL, TEXT("NInterface/Button/Button_22x18_00_000.SPR"), 0, false, 0, 294, 5, 22, 18, 22, 18 );
		}
		if(	pclClient->IsCountrySwitch( Switch_Char_Option ) )
		{
			// 채팅창 셋 버튼
			m_pChatSetButton = new CButton( this );
			m_pChatSetButton->Create( NCHAT_DIALOG_BUTTON_CHATSET, NULL, TEXT("NInterface/Button/Button_19x18_00_001.SPR"), 0, false, 0, 297, 5, 19, 18, 19, 18 );
		}

		// 채팅창 열기
		m_pOpenButton = new CButton( this );
		m_pOpenButton->Create( NCHAT_DIALOG_BUTTON_OPEN, NULL, TEXT("NInterface/Button/Button_16x18_00_000.SPR"), 0, false, 0, 318, 5, 16, 18, 16, 18 );

		// IME 버튼
		m_pIMEButton = new CButton ( this );
		m_pIMEButton->Create( NCHAT_DIALOG_BUTTON_IME, NULL, TEXT("NInterface/Button/BUTTON_18x18_00_000.SPR"), 0, false, 0, 10, 5, 18, 18, 18, 18 );
	}
/*
	else
	{
		// 채팅창 열기
		m_pOpenButton = new CButton( this );
		m_pOpenButton->Create( NCHAT_DIALOG_BUTTON_OPEN, NULL, TEXT("NInterface/Button/InterfaceButton_38x18_CHATOPEN.SPR"), 0, false, 0, 294, 21, 40, 17, 40, 17 );
	}
*/

	// 채팅 탭 버튼들
	for(SI32 i = 0; i < MAX_TABBUTTON; i++)
	{		
		m_pTabButton[i] = new CChatTabButton(this);
		m_pTabButton[i]->Create( NCHAT_DIALOG_BUTTON_TAB_0 + i, NULL, TEXT("NInterface/Button/BUTTON_48X18_CHATBARTAB.SPR"),i*4,false, 0 , i*50 + 27, 0, 48, 18, 48, 18 );
	
	}
	
	//m_pTabButton[BUTTON_ID_GOONZU]->MoveControl(  27  , m_pTabButton[BUTTON_ID_GOONZU]->GetY() +   m_pTabButton[BUTTON_ID_GOONZU]->GetHeight() );
	m_pTabButton[BUTTON_ID_GOONZU]->Show(false);

	m_bSetGoonzu = false;

	m_pTabButton[0]->SetSelect(TRUE);
	m_siSelectedButton = 0;

	//KHY - 0110 - 채팅 모드 고정
	SetFirstCreate(true); //처음 생성되엇다.
//	m_pTabButton[2]->SetSelect(TRUE);
//	m_siSelectedButton = 2;
//	SetSelectButton(BUTTON_ID_GUILD);
	

	//NCHAT_DIALOG_CHATEDIT
	m_pChatEdit = new CChatEdit( this );
	m_pChatEdit->Create( NCHAT_DIALOG_CHATEDIT, NULL, TEXT("NInterface/Interface/INTERFACE_344X160_CHATEDIT.SPR"), 0, false, 0, 0, -160, 344, 150, 344, 150, CHATEDIT_LINEHEIGHT, true, 5, 5 );

	//m_pChatEdit->Create( NCHAT_DIALOG_CHATEDIT, NULL, TEXT("NInterface/Interface/INTERFACE_344X90_CHATEDIT.SPR"), 0, false, 0, 0, -74, 344, 90, 344, 90, CHATEDIT_LINEHEIGHT, true, 5, 5 );
	m_pChatEdit->SetDrawOption( SPR_PUTSPRALPHAT );

	m_pChatEdit->SetScrollThumbFileFontIndex( 6 );
	m_pChatEdit->SetScrollBKFile( NULL, -1 );
	m_pChatEdit->SetScrollUpButtonFileFontIndex( 28 );
	m_pChatEdit->SetScrollDownButtonFileFontIndex( 32 );

	SetOpenedChatDlg( CHATEDIT_OPEN_5LINES );

	SetTopMostWindow( false );

	SetActionTimer( 100 );
	SetTimerOnOff( true );

	m_ChatHistoryDequeItr = m_ChatHistoryDeque.end();
	return;
}


void CNChatBarDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient *pclclient = ( cltClient * )pclClient;

	if(pclclient->m_pInterfaceMgr->GetToggleState() == IMEUI_STATE_ON )    
	{
		m_pIMEButton->SetFontIndex(0);
	}
	else
	{
		m_pIMEButton->SetFontIndex(4);
	}

	return;
}

void CALLBACK CNChatBarDlg::StaticCallBackDialogBar( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNChatBarDlg *pThis = (CNChatBarDlg *)pControllerMgr;
	pThis->CallBackDialogBar( nEvent, nControlID, pControl );
	return;
}

void CALLBACK CNChatBarDlg::CallBackDialogBar( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = ( cltClient * ) pclClient;

	switch ( nControlID )
	{
	case NCHAT_DIALOG_BUTTON_TAB_0 + BUTTON_ID_NORMAL:	// 일반
	case NCHAT_DIALOG_BUTTON_TAB_0 + BUTTON_ID_VILLAGE: //마을
	case NCHAT_DIALOG_BUTTON_TAB_0 + BUTTON_ID_GUILD: //길드
	case NCHAT_DIALOG_BUTTON_TAB_0 + BUTTON_ID_PARTY: //파티
	case NCHAT_DIALOG_BUTTON_TAB_0 + BUTTON_ID_HEAD: //광고
	case NCHAT_DIALOG_BUTTON_TAB_0 + BUTTON_ID_NEWSITEM: //자막광고
	case NCHAT_DIALOG_BUTTON_TAB_0 + BUTTON_ID_GOONZU:	// 군주채팅
		{					
			SI16 buttonindex = nControlID - NCHAT_DIALOG_BUTTON_TAB_0;			
			SetSelectButton(buttonindex);

			if( pclClient->IsCountrySwitch(Switch_ChatEditFilter))
			{
				switch(buttonindex)
				{
				case 0:		m_pChatEdit->SetShowChatDataFilter(0);							break;
				case 1:		m_pChatEdit->SetShowChatDataFilter(MSG_RESIDENTSCHAT_RECV);		break;
				case 2:		m_pChatEdit->SetShowChatDataFilter(MSG_GUILDCHAT_RECV);			break;
				case 3:		m_pChatEdit->SetShowChatDataFilter(MSG_PARTYCHAT_RECV);			break;

				default:	m_pChatEdit->SetShowChatDataFilter(0);		break;				
				}

				m_pChatEdit->Refresh();			
			}

			//KHY - 0110 - 채팅 모드 고정
			if((g_SoundOnOffDlg.m_bChatTabFixed == true) &&(GetFirstCreate()==false)) // 채팅모드 고정이 설정되어있고, 처음들어온것이 아니다.
			{
				g_SoundOnOffDlg.m_siChatTabNowIndex = buttonindex;
				(( CNSoundOnOffDlg *)pclclient->m_pDialog[ NSOUNDONOFF_DLG ] )->m_siChatTabNowIndex = buttonindex;
				
				(( CNSoundOnOffDlg *)pclclient->m_pDialog[ NSOUNDONOFF_DLG ] )->WriteSoundConfig();
			}
		}
		break;
	case NCHAT_DIALOG_BUTTON_OPEN:
		{
			m_OpendsiSize++;
			if(m_OpendsiSize > CHATEDIT_OPEN_MAX )
				m_OpendsiSize = CHATEDIT_CLOSE;
			//if(m_OpendsiSize == CHATEDIT_OPEN_MAX )
			//	m_OpendsiSize = CHATEDIT_CLOSE;
			
			//else
			//	m_OpendsiSize = CHATEDIT_OPEN_MAX;

			SetOpenedChatDlg( m_OpendsiSize );
		}
		break;
	case NCHAT_DIALOG_BUTTON_CHATSET:
		{
			pclclient->CreateInterface(NSETCHAT_DLG);
		}
		break;
	case NCHAT_DIALOG_BUTTON_EMOTICON:
		{
			((CNEmoticonDlg*)pclclient->m_pDialog[NEMOTICON_DLG])->Show( true );
		}
		break;
	case NCHAT_DIALOG_BUTTON_IME:
		{
			//if(pclclient->m_pInterfaceMgr->GetToggleState() == IMEUI_STATE_ON )    
			//{
			//	m_pIMEButton->SetFontIndex(0);
			//	pclclient->m_pInterfaceMgr->CheckToggleState();
			//}
			//else
			//{
			//	m_pIMEButton->SetFontIndex(4);
			//	pclclient->m_pInterfaceMgr->CheckToggleState();
			//}
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
						
						InsertChatHistory(m_strChatData);
						
						// 같은 말 반복횟수를 체크 하여 일정 회수 이상일경우 채팅을 막는다.
						// [영훈] GM일경우 반복횟수 체크를 안하도록 한다
						if( (m_siRepeatCnt > MAX_REPEAT_LIMIT_NUM) && (pclclient->pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL) )
						{
   							TCHAR * pText = GetTxtFromMgr(5943);
                            pclclient->pclMessage->SetMsg(pText,0,RGB(230,70,70),RGB(0,0,0)); 
						}
						else
						{
							ChatDataSend();
						}						
					}
					m_pInputEdit->ClearText();
				}
				break;		
			case EVENT_EDITBOX_ARROW_UP:
				{
                    GetChatHistoryPre();
				}
				break;
			case EVENT_EDITBOX_ARROW_DOWN:
				{
					GetChatHistoryNext();
				}
				break;
			case EVENT_EDITBOX_PAGEUP:
				{
					if(m_OpendsiSize == CHATEDIT_OPEN_MAX )
						break;
					
					m_OpendsiSize++;

					SetOpenedChatDlg( m_OpendsiSize );
				}
				break;
			case EVENT_EDITBOX_PAGEDOWN:
				{
					if(m_OpendsiSize == CHATEDIT_CLOSE )
						break;
					
					m_OpendsiSize--;
					SetOpenedChatDlg( m_OpendsiSize );
				}
				break;
			case EVENT_EDITBOX_TAB:
				{
					if(m_OpendsiSize == CHATEDIT_CLOSE )
						break;

					m_OpendsiSize--;
					SetOpenedChatDlg( m_OpendsiSize );
				}
				break;
			}
		}				
		break;
	}						// switch ( nControlID )
}

#define POW(X) {X * X}

void CNChatBarDlg::Action()
{
 	cltClient *pclclient = (cltClient *)pclClient;
	
	//if( IsFocus  )
	{
		// NSysMenu_Dlg , NCharMenu_Dlg , STATUSBAR_DLG, 외곽선 과의 스냅 체크

		SI16 distanceX, distanceY; //대상과의 거리
		SI16 destX, destY; // 대상의 좌표
		//SI32 snapPointY = MAP_TILE_YSIZE + pclclient->m_pDialog[STATUSBAR_DLG]->GetY() - GetHeight();		
		destY = pclclient->GetScreenYSize() - 24 - GetHeight();  // 
        distanceY =  GetY() - destY ;
		destX = MAP_TILE_XSIZE + (g_SoundOnOffDlg.m_bMaxWindowMode ? 32 : 0);
		distanceX = GetX() - destX;

		if(abs(distanceY) < SNAPRANGE )
		{
			MovePos(GetX(),destY);
			if(abs(distanceX) < SNAPRANGE )
			{
				MovePos(destX,GetY());
			}
		}	
	}
	

	// 버튼의 활성화 상태변화 필요 체크
	//TCHAR * pText = NULL;

	//마을채팅 활성화 체크
	if( pclclient->pclCM->CR[1]->pclCI->clBI.siHomeVillage &&  m_pTabButton[BUTTON_ID_VILLAGE]->IsEnable() == FALSE  )
	{ 
		m_pTabButton[BUTTON_ID_VILLAGE]->Enable(true);
		//m_pTabButton[BUTTON_ID_VILLAGE]->SetUseToolTip(false);
	}
	else if( !pclclient->pclCM->CR[1]->pclCI->clBI.siHomeVillage &&  m_pTabButton[BUTTON_ID_VILLAGE]->IsEnable() == TRUE )
	{
		m_pTabButton[BUTTON_ID_VILLAGE]->Enable(false);
		//KHY - 0110 - 채팅 모드 고정
		SetFirstCreate(true);
		SendEvent( EVENT_BUTTON_CLICKED, true, m_pTabButton[BUTTON_ID_NORMAL] );
		//pText = GetTxtFromMgr(1042);
		//m_pTabButton[BUTTON_ID_PARTY]->SetUseToolTip(pText);
	}

	//길드채팅
	if( pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siType 
		&&  pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siType != GUILDRANKTYPE_STANDING 
		&&  pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.uiConfirm != GUILDSTATUS_STANDING
		&& m_pTabButton[BUTTON_ID_GUILD]->IsEnable() == FALSE)
	{
		m_pTabButton[BUTTON_ID_GUILD]->Enable(true);	
		//m_pTabButton[BUTTON_ID_GUILD]->SetUseToolTip(false);
	}
	else if( !pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siType && m_pTabButton[BUTTON_ID_GUILD]->IsEnable() == TRUE)
	{
		m_pTabButton[BUTTON_ID_GUILD]->Enable(false);
		//KHY - 0110 - 채팅 모드 고정
		SetFirstCreate(true);
		SendEvent( EVENT_BUTTON_CLICKED, true, m_pTabButton[BUTTON_ID_NORMAL] );
		//pText = GetTxtFromMgr(5557);
		//m_pTabButton[BUTTON_ID_PARTY]->SetUseToolTip(pText);
	}

	//파티채팅
	if( ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->IsPartyStatus() && m_pTabButton[BUTTON_ID_PARTY]->IsEnable() == FALSE ) // 파티에 가입되어 있지 않다면 파티버튼 디스에이블
	{
		m_pTabButton[BUTTON_ID_PARTY]->Enable(true);
		//m_pTabButton[BUTTON_ID_PARTY]->SetUseToolTip(false);
	}
	else if(  !(((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->IsPartyStatus()) && m_pTabButton[BUTTON_ID_PARTY]->IsEnable() == TRUE )
	{
		m_pTabButton[BUTTON_ID_PARTY]->Enable(false);
		//KHY - 0110 - 채팅 모드 고정
		SetFirstCreate(true);
		SendEvent( EVENT_BUTTON_CLICKED, true, m_pTabButton[BUTTON_ID_NORMAL] );
		
		//pText = GetTxtFromMgr(5585);
		//m_pTabButton[BUTTON_ID_PARTY]->SetUseToolTip(pText);
	}
		
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_KING 
		&& 	(pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique == ITEMUNIQUE(23500) ||

			pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique == ITEMUNIQUE(23505))
		&& m_bSetGoonzu == false)  // 군주 이면 군주 채팅 Show
	{
		m_pTabButton[BUTTON_ID_GOONZU]->Show(true);
		
		//탭들의 위치 수정
		for(SI32 i = 0 ; i < MAX_TABBUTTON ; i++)
		{
			m_pTabButton[i]->MoveControl(3 + m_pTabButton[i]->GetWidth() * i , m_pTabButton[i]->GetY());
		}

		m_bSetGoonzu = true;
	}
	else if( 
		(pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique != ITEMUNIQUE(23500) &&
		pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique != ITEMUNIQUE(23505))
		&&	m_bSetGoonzu == true )
	{
		m_pTabButton[BUTTON_ID_GOONZU]->Show(false);
		
		for(SI32 i = 0 ; i < MAX_TABBUTTON ; i++)
		{
			m_pTabButton[i]->MoveControl( i*50 + 27 , m_pTabButton[i]->GetY());
		}
		//KHY - 0110 - 채팅 모드 고정
		SetFirstCreate(true);
		SendEvent( EVENT_BUTTON_CLICKED, true, m_pTabButton[BUTTON_ID_NORMAL] );
		m_bSetGoonzu = false;
	}

	//KHY - 0110 - 채팅 모드 고정
	if((g_SoundOnOffDlg.m_bChatTabFixed == true) &&(GetFirstCreate()==true))	
	{
		if(g_SoundOnOffDlg.m_siChatTabNowIndex  >= 0)
		{
			SetFirstCreate(false);
			SendEvent( EVENT_BUTTON_CLICKED, true, m_pTabButton[g_SoundOnOffDlg.m_siChatTabNowIndex] );
		}
	}

	SetFirstCreate(false);

/*
	// 레벨이 안되면, 거부설정을 해두면, 상점을 열어두면 광고를 못함
	
	if(pclclient->pclCM->CR[1]->pclCI->clIP.GetLevel() >= FATHER_LEVEL 
		&& pclclient->pclCM->CR[1]->bPersonalShopSwitch == false
		&& g_SoundOnOffDlg.m_bNotifyHeadChat == false ) // 광고창을 띄울수 있는 조건이라면
	{
		m_pTabButton[BUTTON_ID_HEAD]->Enable(true);
		m_pTabButton[BUTTON_ID_HEAD]->SetUseToolTip(false);		
	}
	else
	{
		if(pclclient->pclCM->CR[1]->pclCI->clIP.GetLevel() < FATHER_LEVEL)		
		{
			TCHAR szBuffer[256];
			pText = GetTxtFromMgr(1048);	// %d렙보다 낮아서 안됩니다
			StringCchPrintf(szBuffer,256,pText,FATHER_LEVEL);
			m_pTabButton[BUTTON_ID_HEAD]->SetUseToolTip(szBuffer);
		}
		else if( pclclient->pclCM->CR[1]->bPersonalShopSwitch == true )
		{	
			pText = GetTxtFromMgr(1046);	// 상점개설중 안됩니다
			m_pTabButton[BUTTON_ID_HEAD]->SetUseToolTip(pText);
		}
		else if( g_SoundOnOffDlg.m_bNotifyHeadChat == true )
		{
			pText = GetTxtFromMgr(1047);	// 거부해놓고 하려면 안되요~
			m_pTabButton[BUTTON_ID_HEAD]->SetUseToolTip(pText);
		}	
		SendEvent( EVENT_BUTTON_CLICKED, true, m_pTabButton[BUTTON_ID_NORMAL] );
		m_pTabButton[BUTTON_ID_HEAD]->Enable(false);
	}
*/
	return;
}



void CNChatBarDlg::ChatDataSend()
{	
		// 일반
	if(m_siSelectedButton == BUTTON_ID_NORMAL )
	{
		SI32 ChatMode = CompareKey(m_strChatData, BUTTON_ID_NORMAL);

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
	else if (m_siSelectedButton == BUTTON_ID_VILLAGE )			
	{
		SI32 ChatMode = CompareKey(m_strChatData, BUTTON_ID_VILLAGE);

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
	else if (m_siSelectedButton == BUTTON_ID_GUILD )	
	{
		SI32 ChatMode = CompareKey(m_strChatData, BUTTON_ID_GUILD);

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
	else if (m_siSelectedButton == BUTTON_ID_PARTY )
	{
		SI32 ChatMode = CompareKey(m_strChatData, BUTTON_ID_PARTY);

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
	else if (m_siSelectedButton == BUTTON_ID_HEAD )
	{
		Push( HEAD_CHAT_DATA_SEND, m_strChatData );
	}
	// 자막광고
	else if (m_siSelectedButton == BUTTON_ID_NEWSITEM )
	{
		Push( NEWS_CHAT_DATA_SEND, m_strChatData );		
	}
	else if( m_siSelectedButton == BUTTON_ID_GOONZU  )
	{
		Push( KINGNOTIFY_CHAT_DATA_SEND, m_strChatData );
	}


	return;
}

void CNChatBarDlg::Push( SI16 ChatDataKind, TCHAR *ChatText )
{
	m_pChatBarBuffer->Push( ChatDataKind, ChatText );
	return;
}

bool CNChatBarDlg::Pop( stChatStatusBarData * pChatStatusBarData )
{
	memset( pChatStatusBarData, 0, sizeof( stChatStatusBarData ) );

	if ( m_pChatBarBuffer->Pop( pChatStatusBarData ) )
	{
		return true;
	}

	return false;
}

void CNChatBarDlg::SetInputEditBoxFocus()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( pclclient->m_pInterfaceMgr )
	{
		pclclient->m_pInterfaceMgr->SetFocus( this, m_pInputEdit );
	}

	return;
}
bool CNChatBarDlg::CheckData( TCHAR *PlayerName, TCHAR *ChatText )
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

void CNChatBarDlg::ChatRecv( TCHAR *PlayerName, TCHAR *ChatText, bool bGameMaster, SI32 Kind )
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

	if (bGameMaster)
		ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_YELLOW;

	MStrCpy( ChatEditData.strNameData, PlayerName, MAX_PLAYER_NAME );
	MStrCpy( ChatEditData.strChatData, ChatText, MAX_CHAT_DATA_LENGTH );
	
	if (CheckChatstatus( ChatKind ) == false) return;// 셋팅을 체크 해서 보여줄지 생각 한다.

	ChatEditData.siChatKind = ChatKind;	// 해당 대화의 카인드 값을 체크 한다.
	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	m_pChatEdit->Insert(&ChatEditData);

	return;
}

void CNChatBarDlg::MasterNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
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
	if (CheckChatstatus( ChatEditData.siChatKind ) == false) return;// 셋팅을 체크 해서 보여줄지 생각 한다.
	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	m_pChatEdit->Insert(&ChatEditData);

	cltClient *pclclient = (cltClient *)pclClient;
	if ( pclclient->m_pDialog[ NRESIDENTS_DLG ] )
	{
		((CNResidentsDlg*)pclclient->m_pDialog[ NRESIDENTS_DLG ])->InsertChatEditData( &ChatEditData );
	}

	return;
}

void CNChatBarDlg::VillageNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
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

void CNChatBarDlg::ResidentsNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
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

void CNChatBarDlg::KingNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
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

void CNChatBarDlg::GoodsProductionNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
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
void CNChatBarDlg::ItemGetChatRecv( TCHAR* itemname, SI16 itemCnt, GMONEY price )
{
	TCHAR buffer[ 96 ] = TEXT("");
	TCHAR moneybuf[ 32 ] = TEXT("");

	bool bPrice = DATA_TO_BOOL(price);
	//// 채팅창에 시스템메세지출력 안함으로 설정한다면 아이템 획득여부 표시 안함
	// 현제는 적용 안함 추가 요청시 추가 (2008-06-24 ) 
	//cltCharServer* pclcharoption = (cltCharServer*)pclClient->pclCM->CR[1];
	//if ( pclcharoption == NULL ) return ;
	//if (pclcharoption->pclCI->clCharOption.bChatSetSystem == true) return;

	if(bPrice)
	{
		TCHAR* pText = GetTxtFromMgr(611);
		g_JYLibFunction.SetNumUnit(price, moneybuf,32, pText );
		pText  = GetTxtFromMgr(2393);
		StringCchPrintf(buffer, 96, pText, itemname,itemCnt,moneybuf);
	}
	else
	{
		TCHAR* pText  = GetTxtFromMgr(2394);
		StringCchPrintf(buffer, 96, pText, itemname,itemCnt);
	}

	stChatEditData ChatEditData;
	ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_GREEN;
	ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_GREEN;
	MStrCpy( ChatEditData.strNameData, TEXT(""), CHATEDIT_NAMEDATA_LENGTH );
	MStrCpy( ChatEditData.strChatData, buffer, MAX_CHAT_DATA_LENGTH );
	ChatEditData.siChatKind = MSG_CHAT_RECV;
	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	m_pChatEdit->Insert(&ChatEditData);

	return;
}

//KHY - 1205 - 유저레벨업 공지
void CNChatBarDlg::LevelUpNoticeChatRecv( TCHAR* charname, SI32 level, SI32 skilllevel,SI32 skilltype ,SI16 messagetype)
{
	TCHAR buffer[ 1024 ] = TEXT("");
	stChatEditData ChatEditData;

	if(messagetype ==1) // 길드.
	{
		if ( CLevelupskillupNoticeChatstatus( messagetype ) == false)	return ;// 현제 메세지타입으로 채팅옵션 확인하여 보여줄것인지 선택
		TCHAR* pMessageTypeText = GetTxtFromMgr(5561); //길드 공지.

		if(level >20) // 전투 레벨업. 20보다 커야 한다.
		{
			TCHAR* pText  = GetTxtFromMgr(7226);
			
			StringCchPrintf(buffer, 1024, pText, pMessageTypeText,charname,level);		
		}
		else if(skilllevel > 30) //스킬 레벨업. 30부터
		{
			TCHAR* pText  = GetTxtFromMgr(7227);
			TCHAR SkillName[ 64 ] = TEXT("");
			if(pclClient->pclSkillManager->GetSkillName( skilltype, SkillName ) ==  false)return;
			
			StringCchPrintf(buffer, 1024, pText, pMessageTypeText,charname,SkillName,skilllevel);			
		}
		else
		{
			return;
		}

		ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
		ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_PINK;
		ChatEditData.siChatKind = MSG_GUILDCHAT_RECV;
	}
	else if(messagetype ==2) // 마을.
	{
		if ( CLevelupskillupNoticeChatstatus( messagetype ) == false)	return ;// 현제 메세지타입으로 채팅옵션 확인하여 보여줄것인지 선택
		TCHAR* pMessageTypeText = GetTxtFromMgr(1217); //마을 공지.

		if(level >20) // 전투 레벨업. 20보다 커야 한다.
		{
			TCHAR* pText  = GetTxtFromMgr(7226);
			
			StringCchPrintf(buffer, 1024, pText, pMessageTypeText,charname,level);		
		}
		else if(skilllevel > 30) //스킬 레벨업. 30부터
		{
			TCHAR* pText  = GetTxtFromMgr(7227);
			TCHAR SkillName[ 64 ] = TEXT("");
			if(pclClient->pclSkillManager->GetSkillName( skilltype, SkillName ) ==  false)return;
			
			StringCchPrintf(buffer, 1024, pText, pMessageTypeText,charname,SkillName,skilllevel);			
		}
		else
		{			
			return;
		}

		ChatEditData.siNameDataColorIndex = CHATEDIT_COLOR_YELLOW;
		ChatEditData.siChatDataColorIndex = CHATEDIT_COLOR_GREEN;
		ChatEditData.siChatKind = MSG_RESIDENTSCHAT_RECV;		
	}

	MStrCpy( ChatEditData.strNameData, TEXT(""), CHATEDIT_NAMEDATA_LENGTH );
	MStrCpy( ChatEditData.strChatData, buffer, MAX_CHAT_DATA_LENGTH );
	
	ChatEditData.siChatEditDataSize = sizeof( stChatEditData );

	m_pChatEdit->Insert(&ChatEditData);

	return;
}

void CNChatBarDlg::SetOpenedChatDlg( SI16 OpenSize )
{
 	cltClient *pclclient = (cltClient *)pclClient;
	m_OpendsiSize = OpenSize;

	if( OpenSize == CHATEDIT_OPEN_MAX)
	{
 		m_pChatEdit->Show( true );
		m_pChatEdit->SetFileName(TEXT("NInterface/Interface/INTERFACE_344X510_CHATEDIT.SPR"));
		m_pChatEdit->SetControllerPos(0, -510, 344, 500, 344, 500);

		m_pChatEdit->Refresh();
		m_pChatEdit->RefreshScroll();
		TCHAR* pText = GetTxtFromMgr(5095);
		m_pOpenButton->SetUseToolTip( pText );

		for(SI16 i = 0; i < MAX_TABBUTTON; i++)
		{
			m_pTabButton[i]->MoveControl( m_pTabButton[i]->GetX(), -15 );
		}

	}
	else if( OpenSize == CHATEDIT_OPEN_10LINES)
	{
		m_pChatEdit->Show( true );

		m_pChatEdit->SetFileName(TEXT("NInterface/Interface/INTERFACE_344X160_CHATEDIT.SPR"));
		m_pChatEdit->SetControllerPos(0, -160, 344, 150, 344, 150);

		m_pChatEdit->Refresh();
		m_pChatEdit->RefreshScroll();
		TCHAR* pText = GetTxtFromMgr(3414);
		m_pOpenButton->SetUseToolTip( pText );

		for(SI16 i = 0; i < MAX_TABBUTTON; i++)
		{
			m_pTabButton[i]->MoveControl( m_pTabButton[i]->GetX(), -15 );
		}
	}
	else if(OpenSize == CHATEDIT_OPEN_5LINES)
	{
		m_pChatEdit->SetFileName(TEXT("NInterface/Interface/INTERFACE_344X90_CHATEDIT.SPR"));
		m_pChatEdit->SetControllerPos(0, -90, 344, 80, 344, 80);

		m_pChatEdit->Refresh();
		m_pChatEdit->RefreshScroll();

		m_pChatEdit->Show( true );

		TCHAR* pText = GetTxtFromMgr(3414);
		m_pOpenButton->SetUseToolTip( pText );

		for(SI16 i = 0; i < MAX_TABBUTTON; i++){
			m_pTabButton[i]->MoveControl( m_pTabButton[i]->GetX(), -15 );
		}
	}
	else if( OpenSize == CHATEDIT_CLOSE ) 
	{
		TCHAR* pText = GetTxtFromMgr(3414);
		m_pOpenButton->SetUseToolTip( pText );
		
		m_pChatEdit->Show( false );		

		for(SI16 i = 0; i < MAX_TABBUTTON; i++){
			m_pTabButton[i]->MoveControl( m_pTabButton[i]->GetX(), -15 );
		}
	}

	return;
} 

void CNChatBarDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
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
			if ( RetValue ) //자막광고 보내기 승낙하면 오는부분?
			{
				SI16* chatmode= (SI16*)pData;
				SI32* systemtxt= (SI32*)&pData[2];
				TCHAR* pmsg =(TCHAR*)&pData[6];

				// 채팅 블록
				if( pclClient->pclCM->GetCharClient(1)->m_stMuteTime.wYear > 0 )
				{
					NTCHARString256		kText(GetTxtFromMgr(9626));
					NTCHARString256		kTitle(GetTxtFromMgr(5614));

					SI64 siTimeDiff = TimeOperation(&pclClient->pclCM->GetCharClient(1)->m_stMuteTime, &pclclient->sTime);
					if( siTimeDiff > 0 )
					{
						kText.Replace("#minute#", SI32ToString(siTimeDiff / (uiSec * 60)) );
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

void CNChatBarDlg::ChatMessage(SI16 chatmode, SI32 systemtxt, TCHAR* pmsg)
{
	cltClient *pclclient = ( cltClient * )pclClient;

	BYTE TempBuffer[ 256 ];
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



	pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
}

SI32 CNChatBarDlg::CompareKey(TCHAR *data, SI32 type)
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
		/*
	case 4:
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
	case 5:
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
		*/
	}
	return TAB_MODE_PARTYCHAT;

}


BOOL CNChatBarDlg::IsInputFocus()
{
	if(m_pInputEdit == NULL) return FALSE;

	return m_pInputEdit->IsFocus();
}

void CNChatBarDlg::SetSelectButton(SI16 TabButtonIndex)
{
	// 선택된 탭버튼 설정
	for( SI16 i = 0; i < MAX_TABBUTTON; i++)
	{						
		m_pTabButton[i]->SetSelect(FALSE);
	}					
	m_pTabButton[TabButtonIndex]->SetSelect(TRUE);
	m_siSelectedButton = TabButtonIndex;

}
void CNChatBarDlg::MoveTabButtonToRight()
{
	SI16 ButtonIndex = m_siSelectedButton;
	
	// 오른쪽에 남은 탭들수만큼 반복하며 선택 가능한 탭을 찾는다. 바로 탭 설정 바꾸어 주고 끝냄
	for( SI16 i = ButtonIndex + 1 ; i < 6 ; i++ )
	{
		if( m_pTabButton[ i ]->IsEnable() ) 
		{
			SetSelectButton( i );
			return;
		}

	}

	// 제일 오른쪽 끝 탭이 선택되어 있던 상태였다거나 Enable된 버튼이 없었다거나 한다면 제일 왼쪽 기본탭을 선택해준다.
	
	if(m_siSelectedButton == BUTTON_ID_NORMAL )	// 이미 기본탭이 선택되어 있으므로 바로 끝냄
		return;

	SetSelectButton(BUTTON_ID_NORMAL);

}

void CNChatBarDlg::MoveTabButtonToLeft()
{
	SI16 ButtonIndex = m_siSelectedButton;

	ButtonIndex -= 1;

	// 모든 탭들수만큼 반복하며 선택 가능한 탭을 찾는다. 찾았다면 바로 탭 설정 바꾸어 주고 끝냄
	for( SI16 i = 0 ; i < 6 ; i++ )
	{
		if(ButtonIndex < 0) ButtonIndex = 6-1;

		if( m_pTabButton[ ButtonIndex ]->IsEnable() ) 
		{
			SetSelectButton( ButtonIndex );
			return;
		}
		ButtonIndex--;
	}	
	// 못찾았다면 기본 탭을 선택해주고 끝냄.
	SetSelectButton(BUTTON_ID_NORMAL);
}

void CNChatBarDlg::InsertChatHistory(TCHAR *ChatData)
{
	if( ChatData == NULL)
		return;
		
	deque<stChatHistoryData>::size_type	ChatHistoryDataNum;
 	ChatHistoryDataNum = m_ChatHistoryDeque.size();
	
	//마지막 히스토리와 새로온 데이터가 같은 문자열이라면 히스토리에 넣지 않는다.
	if(ChatHistoryDataNum > 0)
	{
		m_ChatHistoryDequeItr = m_ChatHistoryDeque.end();
		m_ChatHistoryDequeItr--;

		stChatHistoryData * pChatHistoryData = NULL;
		
		pChatHistoryData = &(*m_ChatHistoryDequeItr);

		if(pChatHistoryData != NULL)
		{
			if(_tcscmp(pChatHistoryData->strChatData,ChatData) == 0)
			{
				if(MAX_REPEAT_LIMIT_NUM + 1 >= m_siRepeatCnt) // m_siRepeatCnt 의 값이 무한정 커질것을 막는다.
					m_siRepeatCnt++;
				
				m_ChatHistoryDequeItr = m_ChatHistoryDeque.end();
				return;
			}
		}		
	}

	//데크의 채팅 히스토리가 MAX_CHAT_HISTORY_NUM 을 넘어가면 오래된 히스토리를 팝해서 지워준다.
	if( ChatHistoryDataNum > MAX_CHAT_HISTORY_NUM)
	{
		m_ChatHistoryDeque.pop_front();
	}

	stChatHistoryData ChathistoryData(ChatData);

	m_ChatHistoryDeque.push_back(&ChathistoryData);
	
	m_ChatHistoryDequeItr = m_ChatHistoryDeque.end();

	m_siRepeatCnt = 0;	// 같은 말을 반복했던 횟수를 0으로 초기화 해준다.      
}

void CNChatBarDlg::GetChatHistoryPre()
{
	deque<stChatHistoryData>::size_type ChatHistoryDataNum;

	ChatHistoryDataNum = m_ChatHistoryDeque.size();

	if( ChatHistoryDataNum <= 0 )
		return;
	
 	stChatHistoryData * pChatHistoryData = NULL;
	
	//m_ChatHistoryDequeItr = m_ChatHistoryDeque.begin();

	if( m_ChatHistoryDequeItr != m_ChatHistoryDeque.begin() )
		m_ChatHistoryDequeItr--;	// 데크의 시작(가장오래된 히스토리)이 아니라면 이전 히스토리를 가르킨다.

	pChatHistoryData = &(*m_ChatHistoryDequeItr);	// 값을 가져온다.

	if(pChatHistoryData == NULL ) return;

	m_pInputEdit->SetText(pChatHistoryData->strChatData);
		
}

void CNChatBarDlg::GetChatHistoryNext()
{
	deque<stChatHistoryData>::size_type ChatHistoryDataNum;

	ChatHistoryDataNum = m_ChatHistoryDeque.size();

	if( ChatHistoryDataNum <= 0 )
		return;

	// 가장 마지막 히스토리 였다면 입력 칸 초기화 
	deque<stChatHistoryData>::iterator ChatHistoryDequeItr;
	ChatHistoryDequeItr = m_ChatHistoryDequeItr;
 	ChatHistoryDequeItr++;

	if(ChatHistoryDequeItr == m_ChatHistoryDeque.end() )
	{	
		m_pInputEdit->Clear();

		m_ChatHistoryDequeItr = m_ChatHistoryDeque.end();
		// 그리고 바로 종료
		return;		

	}
	// 히스토리를 선택해둔게 아니었다면 초기화 하지 않는다.
   	if( m_ChatHistoryDequeItr == m_ChatHistoryDeque.end() ) 
	{
		//m_pInputEdit->Clear();
		
		m_ChatHistoryDequeItr = m_ChatHistoryDeque.end();
		// 그리고 바로 종료
		return;		
	}
	
	stChatHistoryData * pChatHistoryData = NULL;

	m_ChatHistoryDequeItr++;

	pChatHistoryData = &(*m_ChatHistoryDequeItr);

	if(pChatHistoryData == NULL ) return;

	m_pInputEdit->SetText(pChatHistoryData->strChatData);
		
}

//KHY - 0110 - 채팅 모드 고정
SI16 CNChatBarDlg::GetChatTabIndex()
{
	return m_siSelectedButton;
}

bool CNChatBarDlg::GetFirstCreate()
{
	return m_bFirstCreate;
}

void CNChatBarDlg::SetFirstCreate(bool bFirst)
{
	m_bFirstCreate = bFirst;
}
// 보여줄 챗 메세지 종류 셋팅.
void CNChatBarDlg::setChatstatus( UI32 Index)
{
	if( Index < 0 || Index > CHTFILLTER_END )		return;

	//m_bitChatstatus.SetBit( Index );
	CSaveChatstatus();// 옵션기록

}
void CNChatBarDlg::DelChatstatus( UI32 Index)
{
	if( Index < 0 || Index > CHTFILLTER_END )		return;

	//m_bitChatstatus.ClearBit( Index );
	CSaveChatstatus();// 옵션기록

}
BOOL CNChatBarDlg::CheckChatstatus( SI32 ChatKind  )
{
	switch( ChatKind ) 
	{
	case MSG_CHAT_RECV : // 일반
		{
			cltCharServer* pclcharoption = (cltCharServer*)pclClient->pclCM->CR[1];
			if ( pclcharoption == NULL ) return false;
			if (pclcharoption->pclCI->clCharOption.bChatSetNormal == true) return false;
			/*if ( m_bitChatstatus.GetBit( 1 ) ) return false;*/
		}		
		break;
	case MSG_RESIDENTSCHAT_RECV ://마을
		{
			cltCharServer* pclcharoption = (cltCharServer*)pclClient->pclCM->CR[1] ;
			if ( pclcharoption == NULL ) return false;
			if ( pclcharoption->pclCI->clCharOption.bChatSetVillage == true ) return false;
			//if ( m_bitChatstatus.GetBit( 2 ) ) return false;
		}		
		break;
	case MSG_GUILDCHAT_RECV : //길드
		{
			cltCharServer* pclcharoption = (cltCharServer*)pclClient->pclCM->CR[1] ;
			if ( pclcharoption == NULL ) return false;
			if ( pclcharoption->pclCI->clCharOption.bChatSetGuild == true ) return false;
			//if ( m_bitChatstatus.GetBit( 3 ) ) return false;
		}		
		break;
	case MSG_PARTYCHAT_RECV : // 파티
		{
			cltCharServer* pclcharoption = (cltCharServer*)pclClient->pclCM->CR[1] ;
			if ( pclcharoption == NULL ) return false;
			if ( pclcharoption->pclCI->clCharOption.bChatSetParty == true ) return false;
			//if ( m_bitChatstatus.GetBit( 4 ) ) return false;
		}		
		break;
	case MSG_MASTERNOTIFYCHAT_RECV : // 공지창
		{
			cltCharServer* pclcharoption = (cltCharServer*)pclClient->pclCM->CR[1] ;
			if ( pclcharoption == NULL ) return false;
			if ( pclcharoption->pclCI->clCharOption.bChatSetSystem == true ) return false;
			//if ( m_bitChatstatus.GetBit( 5 ) ) return false;
		}		
		break;
	}
	return true;
}
void CNChatBarDlg::CSaveChatstatus( )// 현제 채팅 셋팅 저장
{

}
BOOL CNChatBarDlg::CLevelupskillupNoticeChatstatus(SI16 messagetype ) // 레벨업 공지에서 채팅창 옵션 확인.
{
	if( messagetype ==1 ) // 길드
	{
		// 채팅창 옵션 선택, 길드 채팅 옵션을  안보도록 설정 하면 레벨업 공지도 보이지 않도록 한다,.
		cltCharServer* pclcharoption = (cltCharServer*)pclClient->pclCM->CR[1];
		if ( pclcharoption == NULL )									return FALSE;
		if (pclcharoption->pclCI->clCharOption.bChatSetGuild == false)	return TRUE;
	}
	else if( messagetype == 2 ) // 마을
	{
		// 채팅창 옵션 선택, 마을챗팅 옵션을 안보도록 설정 하면 레벨업 공지도 보이지 않도록 한다,.
		cltCharServer* pclcharoption = (cltCharServer*)pclClient->pclCM->CR[1];
		if ( pclcharoption == NULL )										return FALSE;
		if (pclcharoption->pclCI->clCharOption.bChatSetVillage == false)	return TRUE;
	}
	else
	{
		return TRUE;
	}

	return FALSE;
}
