#include "SoundOnOffDlg.h"

CSoundOnOffDlg g_SoundOnOffDlg;

#include "../../Lib/Sound/SoundMgr.h"
#include "../../Client/Music/music.h"

#include "../../Client/client.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/Msg/MsgType-Fishing.h"

#include "../ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Prompt/PromptMgr.h"

#include "Char\CharManager\CharManager.h"
#include "../common/Main.h"

extern cltCommonLogic* pclClient;

extern SI32		gsiResolution;		// 게임의 해상도. 
extern BOOL		g_bFullScreen;		// 풀스크린 여부 
extern bool		SetGameModeInFile(SI32 gamemode, SI32 res, BOOL bFullScreen, SI32 runmode );
extern bool		SetFullScreenInFile( BOOL bFullScreen );
extern bool		GetFullScreenInFile( BOOL* bFullScreen );

CSoundOnOffDlg::CSoundOnOffDlg()
{
	m_hDlg = NULL;
	m_bShow = false;
	
	m_bChatFixed = false;
	m_bEffectSound = true;
	m_bBackGroudnSound = true;
	m_bBigDisplayMode = true;
	m_bMiniMapShow = true;
	
	m_bFullScreenMode = true ;		// default : 새인터페이스 사용
	m_bTmpFullScreenMode = true;	// default : 새인터페이스 사용
	m_bMaxWindowMode = false;				// 최대창 크기 모드 사용
	m_bTmpMaxWindowMode = false;
	
	//m_bFriendRegisterReject = false;
	/*
	leon add - 2005-03-10-10-00
	* m_pNSoundOnOffDlg와 체크 상태 공유를 위해서.. 추가함
	*/
	m_bPrivateTradeReject = false;
	m_bWhisperChatReject = false;
	m_bJewelItemNotifyReject = false;
	m_bSummonEvolveNotifyReject = false;
	m_bPartyInviteNotifyReject = false;
	m_bFriendAddNotifyReject = false;
	m_bNotifyHeadChat = false;
	m_bNotifyMinigame = false;
	m_bRightMouseMentAllow = true;
	m_siSelectCaptionKindIdex = -1;
	m_bShowCharName	= false;
	m_bAloneDraw = false;

	FILE *fp = _tfopen( TEXT("./Config/SoundConfig.dat"), TEXT("rt") );
	if ( fp != NULL )
	{
		SI32 EffectSound = 1;
		SI32 BackGroundSound = 1;
		SI32 DisplayMode = 0;
		SI32 MiniMapShow = 1;
		SI32 FullScreenMode = 0;
		SI32 MaxWindowMode = 0;				// 최대창 크기 모드 사용
		
		TCHAR Buffer[256] = TEXT("");
		_fgetts( Buffer, 255, fp );


		SI32 RightMouseMentAllow = 0;		// 사용자 메뉴 보여주기    
		                                                              
		SI32 WhisperChatReject = 0;			// 모든 귓속말 거부
		SI32 PrivateTradeReject = 0;		// 모든 거래 거부  
		
		SI32 JewelItemNotifyReject = 0;		// 명품 알림 거부          
		                                                              
		SI32 PartyInviteNotifyReject = 0;	// 파트 초대 거부          
		SI32 FriendAddNotifyReject = 0;		// 친구 추가 거부          
		                                                              
		SI32 SummonEvolveNotifyReject = 0;	// 소환수진화 알림 거부    
		                                                              
		SI32 NotifyHeadChat = 0;			// 모든 광고창 거부
		SI32 NotifyMinigame = 0;			// 미니게임 거부   

	                                                              
		SI32 siSelectCaptionKindIdex = 0;	// 자막 정보 인덱스        

		SI32 ChatFixed = 0;					// 채팅창 고정 ( non-floating )

		SI32 siSoundVolume = 0;
		SI32 siBGMVolume = 0;

		SI32 ShowCharName = 0;

		SI32 aloneDraw = 0;

		SI16 siSuccess = _stscanf( Buffer, TEXT("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"), &EffectSound, &BackGroundSound,
			&MiniMapShow, &FullScreenMode,
			&RightMouseMentAllow ,		&WhisperChatReject,
			&PrivateTradeReject,		
			&JewelItemNotifyReject,		&PartyInviteNotifyReject ,
			&FriendAddNotifyReject,		&SummonEvolveNotifyReject ,
			&NotifyHeadChat ,
			&NotifyMinigame,			&siSelectCaptionKindIdex,
			&MaxWindowMode,				&ChatFixed,
			&siSoundVolume,			&siBGMVolume,
			&ShowCharName,
			&aloneDraw
			);

		if( siSuccess <= 5)
			RightMouseMentAllow = 1;

		// 이전 버젼과의 호환성을 위해...
		if( siSuccess <= 16 )	// SoundVolume, BGMVolume 값이 '없다'
		{
			m_siSelectedBGMButton = 3;
			m_siSelectedSoundButton = 3;
		}
		else
		{
			if( siSoundVolume >= 0 && siSoundVolume <5 )	// 정상적인 사운드 크기 값이 들어가있따.
			{
				m_siSelectedSoundButton = siSoundVolume;
			}
			else
			{
				m_siSelectedSoundButton = 3;
			}


			if( siBGMVolume >= 0  && siBGMVolume < 5 )
			{
				m_siSelectedBGMButton = siBGMVolume;
			}
			else
			{
				m_siSelectedBGMButton = 3;
			}
		}	

		if(ChatFixed > 0 )
			m_bChatFixed = true;
		else
			m_bChatFixed = false;

		if( RightMouseMentAllow > 0 )	
			m_bRightMouseMentAllow  = true;
		else 
			m_bRightMouseMentAllow  = false;
		//if( WhisperChatReject > 0 )	
		//	m_bWhisperChatReject  = true; 
		//else 
		//	m_bWhisperChatReject  = false;
		m_bWhisperChatReject  = false;	// 시작시 귓속말 항상 허용!!

		if( PrivateTradeReject > 0 )
			m_bPrivateTradeReject  = true;
		else 
			m_bPrivateTradeReject  = false;
		if( JewelItemNotifyReject > 0 )	
			m_bJewelItemNotifyReject  = true; 
		else 
			m_bJewelItemNotifyReject  = false;
		//if( PartyInviteNotifyReject > 0 )	
		//	m_bPartyInviteNotifyReject  = true;
		//else 
		//	m_bPartyInviteNotifyReject  = false;
		m_bPartyInviteNotifyReject = false;	// 시작시 파티 초대 항상 허용!!

		if( ShowCharName > 0)
			m_bShowCharName = true;
		else
			m_bShowCharName = false;

		if( FriendAddNotifyReject > 0 )
			m_bFriendAddNotifyReject  = true;
		else 
			m_bFriendAddNotifyReject  = false;
		if( SummonEvolveNotifyReject > 0 )
			m_bSummonEvolveNotifyReject  = true; 
		else 
			m_bSummonEvolveNotifyReject  = false;
		if( NotifyHeadChat > 0 )	
			m_bNotifyHeadChat  = true; 
		else 
			m_bNotifyHeadChat  = false;
		if( NotifyMinigame > 0 )
			m_bNotifyMinigame  = true; 
		else 
			m_bNotifyMinigame  = false;
		if( siSelectCaptionKindIdex >= 0 &&  siSelectCaptionKindIdex < MAX_CAPTIONKIND_NUM )	
			m_siSelectCaptionKindIdex  = siSelectCaptionKindIdex; 
		else 
			m_siSelectCaptionKindIdex  = -1;


		SI32 defaultservicearea = 0;
		GetDefaultServiceArea( &defaultservicearea );	// 강제로 바꾸자

		if( defaultservicearea != ConstServiceArea_Korea )
		{
			if( FullScreenMode == 0 )
				FullScreenMode = 1;
		}

		if ( EffectSound > 0 )
		{
			m_bEffectSound = true;			
		}
		else
		{
			m_bEffectSound = false;			
		}
		
		if( BackGroundSound > 0 )
		{
			m_bBackGroudnSound = true;			
		}
		else
		{
			m_bBackGroudnSound = false;			
		}		

		if ( MiniMapShow > 0 )
		{
			m_bMiniMapShow = true;
		}
		else
		{
			m_bMiniMapShow = false;
		}

		if ( FullScreenMode > 0 )
		{
			m_bFullScreenMode = true;
			m_bTmpFullScreenMode = true;
		}
		else
		{
			m_bFullScreenMode = false;
			m_bTmpFullScreenMode = false;
		}

		fclose( fp );

		// FullScreen 여부를 얻어온다.
		GetFullScreenInFile( &g_bFullScreen );

		//if( MaxWindowMode > 0 ) // 최대창 크기 모드 사용
		if( g_bFullScreen )
		{
			m_bMaxWindowMode = true;
			m_bTmpMaxWindowMode = true;

			if( m_bFullScreenMode == false )
			{
				m_bFullScreenMode = true;
				m_bTmpFullScreenMode = true;
			}
		}
		else
		{
			m_bMaxWindowMode = false;
			m_bTmpMaxWindowMode = false;
		}

		if( aloneDraw )
			m_bAloneDraw = true;
		else
			m_bAloneDraw = false;
	}
	
}

CSoundOnOffDlg::~CSoundOnOffDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CSoundOnOffDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_SOUND_ONOFF), hWnd, StaticSoundOnOffDlgProc );
	Hide();

	return;
}

BOOL CALLBACK CSoundOnOffDlg::StaticSoundOnOffDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_SoundOnOffDlg.SoundOnOffDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CSoundOnOffDlg::SoundOnOffDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;
			TCHAR* pText = GetTxtFromMgr(2387);
			SendMessage( GetDlgItem(hDlg, IDC_COMBO_SELECTCAPTIONKIND), CB_ADDSTRING, 0, (LPARAM)pText );
			pText = GetTxtFromMgr(2388);
			SendMessage( GetDlgItem(hDlg, IDC_COMBO_SELECTCAPTIONKIND), CB_ADDSTRING, 0, (LPARAM)pText );

			LoadSoundConfig();
		}
		return TRUE;

	case WM_TIMER:
		{
		}
		break;
	case WM_COMMAND:
		{
			cltClient *pclclient = (cltClient *)pclClient;

			switch ( LOWORD(wParam) )
			{
			case IDC_CHECK_CHATFIXED:
				if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_CHATFIXED ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
				{
					m_bChatFixed = false;
				}
				else
				{
					m_bChatFixed = true;
				}

				WriteSoundConfig();

				break;
			case IDC_CHECK_NOTIFY_MIINIGAME:
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_NOTIFY_MIINIGAME ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						m_bNotifyMinigame = false;
					}
					else
					{
						m_bNotifyMinigame = true;
					}


					WriteSoundConfig();

				}
				break;
			case IDC_CHECK_NOTIFY_HEADCHAT:
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_NOTIFY_HEADCHAT ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						m_bNotifyHeadChat = false;
					}
					else
					{
						m_bNotifyHeadChat = true;
					}

					WriteSoundConfig();

				}
				break;
			case IDCANCEL:
				{
					Hide();
				}
				break;

			case IDC_CHECK_EFFECT:
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_EFFECT ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						m_bEffectSound = false;
						pclclient->pclMusic->pclSoundManager->EffectSoundOn(false);
						SendMessage( GetDlgItem( hDlg, IDC_CHECK_EFFECT ), BM_SETCHECK, BST_UNCHECKED, 0 );
					}
					else
					{
						m_bEffectSound = true;
						pclclient->pclMusic->pclSoundManager->EffectSoundOn(true);
						SendMessage( GetDlgItem( hDlg, IDC_CHECK_EFFECT ), BM_SETCHECK, BST_CHECKED, 0 );
					}

					WriteSoundConfig();
				}
				break;
			case IDC_CHECK_BACKGROUND:
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_BACKGROUND ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						m_bBackGroudnSound = false;
						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							pclclient->pclMusic->pclSoundManager->BackGroundSoundOn(false);
						}
						SendMessage( GetDlgItem( hDlg, IDC_CHECK_BACKGROUND ), BM_SETCHECK, BST_UNCHECKED, 0 );
					}
					else
					{
						m_bBackGroudnSound = true;
						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							pclclient->pclMusic->pclSoundManager->BackGroundSoundOn(true);
							pclclient->PlayBackgroundMusic( pclClient->pclCM->CR[1]->GetMapIndex() );
						}
						SendMessage( GetDlgItem( hDlg, IDC_CHECK_BACKGROUND ), BM_SETCHECK, BST_CHECKED, 0 );
						
					}

					WriteSoundConfig();
				}
				break;
			case IDC_CHECK_MINIMAP_REJECT:
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_MINIMAP_REJECT ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						m_bMiniMapShow = false;
						SendMessage( GetDlgItem( hDlg, IDC_CHECK_MINIMAP_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );
					}
					else
					{
						m_bMiniMapShow = true;
						SendMessage( GetDlgItem( hDlg, IDC_CHECK_MINIMAP_REJECT ), BM_SETCHECK, BST_CHECKED, 0 );
					}

					WriteSoundConfig();
				}
				break;
			case IDC_CHECK_WHISPERCHAT_REJECT:
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_WHISPERCHAT_REJECT ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							cltGameMsgRequest_WhisperChat_Reject clinfo( false );
							cltMsg clMsg(GAMEMSG_REQUEST_WHISPERCHAT_REJECT, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
						}
						
						m_bWhisperChatReject = false;
					}
					else
					{
						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							cltGameMsgRequest_WhisperChat_Reject clinfo( true );
							cltMsg clMsg(GAMEMSG_REQUEST_WHISPERCHAT_REJECT, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
						}

						m_bWhisperChatReject = true;
					}

					WriteSoundConfig();

				}
				break;
			case IDC_CHECK_PRIVATETRADE_REJECT:
				if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_PRIVATETRADE_REJECT ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
				{
					if ( pclClient->GetGameStatus() == GS_GAME )
					{
						cltGameMsgRequest_PrivateTrade_Reject clinfo( false );
						cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE_REJECT, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
					}

					m_bPrivateTradeReject = false;
				}
				else
				{
					if ( pclClient->GetGameStatus() == GS_GAME )
					{
						cltGameMsgRequest_PrivateTrade_Reject clinfo( true );
						cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE_REJECT, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
					}

					m_bPrivateTradeReject = true;

				}

				WriteSoundConfig();

				break;
			case IDC_CHECK_NOTIFY_JEWELITEM_REJECT:
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_NOTIFY_JEWELITEM_REJECT ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						m_bJewelItemNotifyReject = false;
					}
					else
					{
						m_bJewelItemNotifyReject = true;
					}
				}
				WriteSoundConfig();

				break;
			case IDC_CHECK_NOTIFY_SUMMONEVOLVE_REJECT:
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_NOTIFY_SUMMONEVOLVE_REJECT ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						m_bSummonEvolveNotifyReject = false;
					}
					else
					{
						m_bSummonEvolveNotifyReject = true;
					}
				}
				WriteSoundConfig();

				break;
			case IDC_CHECK_NOTIFY_PARTYINVITE_REJECT:
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_NOTIFY_PARTYINVITE_REJECT ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						m_bPartyInviteNotifyReject = false;
						
						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							cltGameMsgRequest_PartyInvite_Reject clinfo( false );
							cltMsg clMsg(GAMEMSG_REQUEST_PARTYINVITE_REJECT, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}
					else
					{
						m_bPartyInviteNotifyReject = true;

						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							cltGameMsgRequest_PartyInvite_Reject clinfo( true );
							cltMsg clMsg(GAMEMSG_REQUEST_PARTYINVITE_REJECT, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}
				}

				WriteSoundConfig();

				break;
			case IDC_CHECK_NOTIFY_FRIENDADD_REJECT:
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_NOTIFY_FRIENDADD_REJECT ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						m_bFriendAddNotifyReject = false;

						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							cltGameMsgRequest_FriendAdd_Reject clinfo( false );
							cltMsg clMsg(GAMEMSG_REQUEST_FRIENDADD_REJECT, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}
					else
					{
						m_bFriendAddNotifyReject = true;

						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							cltGameMsgRequest_FriendAdd_Reject clinfo( true );
							cltMsg clMsg(GAMEMSG_REQUEST_FRIENDADD_REJECT, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}
				}

				WriteSoundConfig();

				break;			
			case IDC_CHECK_RIGHT_MOUSE_MENU_ALLOW:
				if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_RIGHT_MOUSE_MENU_ALLOW ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
				{
					m_bRightMouseMentAllow = false;
				}
				else
				{
					m_bRightMouseMentAllow = true;
				}

				WriteSoundConfig();

				break;
			case IDC_CHECK_WINDOWMODE:	// 윈도우 창 모드
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_WINDOWMODE ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						m_bTmpFullScreenMode = true;
					}
					else
					{
						m_bTmpFullScreenMode = false;
					}

					TCHAR* pTitle = GetTxtFromMgr(5162);
					TCHAR* pText = GetTxtFromMgr(2392);
					g_ErrorMsg.Set( false, pTitle, pText);

					WriteSoundConfig();
				}
				break;
			case IDC_CHECK_MAXWINDOWMODE:
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_MAXWINDOWMODE ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						m_bTmpMaxWindowMode = false;

						EnableWindow( GetDlgItem( hDlg, IDC_CHECK_WINDOWMODE ), TRUE );
					}
					else
					{
						m_bTmpMaxWindowMode = true;

						if( m_bTmpFullScreenMode == false )
						{
                            SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_WINDOWMODE ), BM_SETCHECK, BST_UNCHECKED, 0 );
                            m_bTmpFullScreenMode = true;
						}

						EnableWindow( GetDlgItem( hDlg, IDC_CHECK_WINDOWMODE), FALSE );
					}
					TCHAR* pTitle = GetTxtFromMgr(5162);
					TCHAR* pText = GetTxtFromMgr(2392);
					g_ErrorMsg.Set( false, pTitle, pText);

					WriteSoundConfig();
					SetGameModeInFile( pclClient->GameMode, gsiResolution, m_bTmpMaxWindowMode, pclClient->siRunMode );
					SetFullScreenInFile(m_bTmpMaxWindowMode);	// FullScreen.txt에 저장
				}
				break;
			case IDC_COMBO_SELECTCAPTIONKIND:
				{
					switch( HIWORD( wParam ))
					{
					case CBN_SELCHANGE:
						{
							SI32 SelectedIndex = SendMessage( GetDlgItem( hDlg, IDC_COMBO_SELECTCAPTIONKIND ), CB_GETCURSEL, 0, 0 );

							if ( SelectedIndex == CB_ERR )
							{
								return FALSE;
							}

							m_siSelectCaptionKindIdex = SelectedIndex;
							
							g_PromptMgr.DelAllPrompt();

							if ( pclClient->GetGameStatus() == GS_GAME )
							{
								cltGameMsgRequest_CaptionKindChange clinfo( SelectedIndex );
								cltMsg clMsg(GAMEMSG_REQUEST_CAPTIONKINDCHANGE, sizeof(clinfo), (BYTE*)&clinfo);
								pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
							}


							WriteSoundConfig();

						}
						break;
					}
				}
				break;
			}
		}
		break;

	case WM_CLOSE:
		{
			Hide();
		}
		break;

	case WM_DESTROY:
		{
			m_hDlg = NULL;
		}
		break;
	}

	return FALSE;
}

bool CSoundOnOffDlg::IsShow()
{
	return m_bShow;
}

void CSoundOnOffDlg::Show()
{
	 //NSoundOnOffDlg와 연동함!!
	if(m_bBackGroudnSound)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_BACKGROUND ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_BACKGROUND ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if(m_bEffectSound)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_EFFECT ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_EFFECT ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if(/*m_bFullScreenMode*/m_bTmpFullScreenMode)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_WINDOWMODE ), BM_SETCHECK, BST_UNCHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_WINDOWMODE ), BM_SETCHECK, BST_CHECKED, 0 );

	if(/*m_bMaxWindowMode*/m_bTmpMaxWindowMode)	// 최대창 크기 모드 사용
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_MAXWINDOWMODE ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_MAXWINDOWMODE ), BM_SETCHECK, BST_UNCHECKED, 0 );
	
	if(m_bMiniMapShow)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_MINIMAP_REJECT ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_MINIMAP_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if(m_bFriendAddNotifyReject)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_FRIENDADD_REJECT ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_FRIENDADD_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if(m_bNotifyHeadChat)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_HEADCHAT ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_HEADCHAT ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if(m_bJewelItemNotifyReject)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_JEWELITEM_REJECT ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_JEWELITEM_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if(m_bNotifyMinigame)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_MIINIGAME ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_MIINIGAME ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if(m_bSummonEvolveNotifyReject)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_SUMMONEVOLVE_REJECT ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_SUMMONEVOLVE_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if(m_bPartyInviteNotifyReject)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_PARTYINVITE_REJECT ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_PARTYINVITE_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if(m_bPrivateTradeReject)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_PRIVATETRADE_REJECT ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_PRIVATETRADE_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if(m_bRightMouseMentAllow)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_RIGHT_MOUSE_MENU_ALLOW ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_RIGHT_MOUSE_MENU_ALLOW ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if(m_bWhisperChatReject)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_WHISPERCHAT_REJECT ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_WHISPERCHAT_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if(m_bChatFixed)
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_CHATFIXED ), BM_SETCHECK, BST_CHECKED, 0 );
	else
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_CHATFIXED ), BM_SETCHECK, BST_UNCHECKED, 0 );
	

	// ComboBox 셋팅!!
	SendMessage( GetDlgItem(m_hDlg, IDC_COMBO_SELECTCAPTIONKIND ), CB_SETCURSEL, m_siSelectCaptionKindIdex, 0 );

	if ( m_bShow )
	{
		m_bShow = false;
		ShowWindow( m_hDlg, SW_HIDE );
		return;
	}

	//------------------------------------
	// 게임서버로 플레이 시간을 요청한다. 
	//------------------------------------
	SI32 id  = 1;
	SI32 charunique = pclClient->pclCM->CR[id]->GetCharUnique();
	if(charunique)
	{
		cltGameMsgRequest_PlayTime clinfo(charunique);
		cltMsg clMsg(GAMEMSG_REQUEST_PLAYTIME, sizeof(clinfo), (BYTE*)&clinfo);

		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CSoundOnOffDlg::Hide()
{
	if ( ! m_bShow )
	{
		m_bShow = false;
		ShowWindow( m_hDlg, SW_HIDE );

		return;
	}
	
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CSoundOnOffDlg::SetTotalSpendTime( SI32 TotalSpendTime )
{
	SI32 Month = TotalSpendTime / ( 31 * 24 * 60 * 60 );
	SI32 Day = ( TotalSpendTime / ( 24 * 60 * 60 ) ) % 31;
	SI32 Hour = ( TotalSpendTime / ( 60 * 60 ) ) % 24;
	SI32 Min = ( TotalSpendTime / 60 ) % 60;
	SI32 Sec = TotalSpendTime % 60;

	TCHAR Buffer[ 256 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr(2204);
	StringCchPrintf( Buffer, 256, pText, 
		Month, Day, Hour, Min, Sec );

	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_TOTAL_SPEND_TIME ), Buffer );
}

void CSoundOnOffDlg::LoadSoundConfig()
{
	/*
	cltClient *pclclient = (cltClient *)pclClient;

	FILE *fp = _tfopen( TEXT("./Config/SoundConfig.dat"), TEXT("rt") );
	if ( fp == NULL )
	{
		m_bEffectSound = true;
		m_bBackGroudnSound = true;

		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_EFFECT ), BM_SETCHECK, BST_CHECKED, 0 );
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_BACKGROUND ), BM_SETCHECK, BST_CHECKED, 0 );
		CheckRadioButton( m_hDlg, IDC_RADIO_DISPLAYMODE1, IDC_RADIO_DISPLAYMODE2, IDC_RADIO_DISPLAYMODE1 );
	
		return;
	}

	

	SI32 EffectSound = 0;
	SI32 BackGroundSound = 0;
	SI32 DisplayMode = 1;
	SI32 MiniMapShow = 1;
	SI32 MaxWindowMode = 0;				// 최대창 크기 모드 사용
	SI32 FullScreenMode = 1;			// default : 새인터페이스 사용

	TCHAR Buffer[256] = TEXT("");
	_fgetts( Buffer, 255, fp );


	SI32 RightMouseMentAllow = 0;		// 사용자 메뉴 보여주기    
		                                                            
	SI32 WhisperChatReject = 0;			// 모든 귓속말 거부
	SI32 PrivateTradeReject = 0;		// 모든 거래 거부  
	
	SI32 JewelItemNotifyReject = 0;		// 명품 알림 거부          
		                                                            
	SI32 PartyInviteNotifyReject = 0;	// 파트 초대 거부          
	SI32 FriendAddNotifyReject = 0;		// 친구 추가 거부          
		                                                            
	SI32 SummonEvolveNotifyReject = 0;	// 소환수진화 알림 거부
		                                                            
	SI32 NotifyHeadChat = 0;			// 모든 광고창 거부
	SI32 NotifyMinigame = 0;			// 미니게임 거부   
		      
	SI32 ChatFixed = 0;					// 채팅창 고정 ( non-floating )

	SI32 siSelectCaptionKindIdex = 0;	// 자막 정보 인덱스        

	SI32 siSoundVolume = 0;
	SI32 siBGMVolume = 0;

	SI32 ShowCharName = 0;


	SI32 aloneDraw = 0;
											
	SI16 siSuccess = _stscanf( Buffer, TEXT("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"), &EffectSound, &BackGroundSound, &DisplayMode, &MiniMapShow, &FullScreenMode,
		&RightMouseMentAllow ,		&WhisperChatReject,
		&PrivateTradeReject,
		&JewelItemNotifyReject,		&PartyInviteNotifyReject ,
		&FriendAddNotifyReject,		&SummonEvolveNotifyReject ,
		&NotifyHeadChat ,
		&NotifyMinigame,			&siSelectCaptionKindIdex,
		&MaxWindowMode,				&ChatFixed,
		&siSoundVolume,			&siBGMVolume,
		&ShowCharName ,
		&aloneDraw
		);

	if( siSuccess <= 5)
		RightMouseMentAllow = 1;

	// 이전 버젼과의 호환성을 위해...
	if( siSuccess <= 16 )	// SoundVolume, BGMVolume 값이 '없다'
	{
		m_siSelectedBGMButton = 3;
		m_siSelectedSoundButton = 3;
	}
	else
	{
		if( siSoundVolume >= 0 && siSoundVolume <5 )	// 정상적인 사운드 크기 값이 들어가있따.
		{
			m_siSelectedSoundButton = siSoundVolume;
		}
		else
		{
			m_siSelectedSoundButton = 3;
		}


		if( siBGMVolume >= 0  && siBGMVolume < 5 )
		{
			m_siSelectedBGMButton = siBGMVolume;
		}
		else
		{
			m_siSelectedBGMButton = 3;
		}
	}	

	if( ChatFixed > 0 )
		m_bChatFixed = true;
	else
		m_bChatFixed = false;

	if( RightMouseMentAllow > 0 )	
	{
		m_bRightMouseMentAllow  = true;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_RIGHT_MOUSE_MENU_ALLOW ), BM_SETCHECK, BST_CHECKED, 0 );
	}
	else 
	{
		m_bRightMouseMentAllow  = false;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_RIGHT_MOUSE_MENU_ALLOW ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}
	//if( WhisperChatReject > 0 )	
	//{
	//	m_bWhisperChatReject  = true; 
	//	SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_WHISPERCHAT_REJECT), BM_SETCHECK, BST_CHECKED, 0 );
	//}
	//else 
	//{
	//	m_bWhisperChatReject  = false;
	//	SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_WHISPERCHAT_REJECT), BM_SETCHECK, BST_UNCHECKED, 0 );
	//}
	// 시작시 귓속말 항상 허용!
	m_bWhisperChatReject  = false;
	SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_WHISPERCHAT_REJECT), BM_SETCHECK, BST_UNCHECKED, 0 );

	if( PrivateTradeReject > 0 )
	{
		m_bPrivateTradeReject  = true;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_PRIVATETRADE_REJECT), BM_SETCHECK, BST_CHECKED, 0 );
	}
	else 
	{
		m_bPrivateTradeReject  = false;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_PRIVATETRADE_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}
	if( JewelItemNotifyReject > 0 )	
	{
		m_bJewelItemNotifyReject  = true; 
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_JEWELITEM_REJECT), BM_SETCHECK, BST_CHECKED, 0 );
	}
	else 
	{
		m_bJewelItemNotifyReject  = false;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_JEWELITEM_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}
	//if( PartyInviteNotifyReject > 0 )	
	//{
	//	m_bPartyInviteNotifyReject  = true;
	//	SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_PARTYINVITE_REJECT), BM_SETCHECK, BST_CHECKED, 0 );
	//}
	//else 
	//{
	//	m_bPartyInviteNotifyReject  = false;
	//	SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_PARTYINVITE_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );
	//}
	// 시작시 파티 초대 항상 허용!!
	m_bPartyInviteNotifyReject  = false;
	SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_PARTYINVITE_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );

	if( FriendAddNotifyReject > 0 )
	{
		m_bFriendAddNotifyReject  = true;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_FRIENDADD_REJECT), BM_SETCHECK, BST_CHECKED, 0 );
	}
	else 
	{
		m_bFriendAddNotifyReject  = false;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_FRIENDADD_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}
	if( SummonEvolveNotifyReject > 0 )
	{
		m_bSummonEvolveNotifyReject  = true; 
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_SUMMONEVOLVE_REJECT), BM_SETCHECK, BST_CHECKED, 0 );
	}
	else 
	{
		m_bSummonEvolveNotifyReject  = false;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_SUMMONEVOLVE_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}	
	if( NotifyHeadChat > 0 )	
	{
		m_bNotifyHeadChat  = true; 
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_HEADCHAT), BM_SETCHECK, BST_CHECKED, 0 );
	}
	else 
	{
		m_bNotifyHeadChat  = false;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_HEADCHAT ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}
	if( NotifyMinigame > 0 )
	{
		m_bNotifyMinigame  = true; 
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_MIINIGAME), BM_SETCHECK, BST_CHECKED, 0 );
	}
	else 
	{
		m_bNotifyMinigame  = false;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_NOTIFY_MIINIGAME ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}
	if( siSelectCaptionKindIdex >= 0 &&  siSelectCaptionKindIdex < MAX_CAPTIONKIND_NUM )	
	{
		m_siSelectCaptionKindIdex  = siSelectCaptionKindIdex; 
		SendMessage( GetDlgItem(m_hDlg, IDC_COMBO_SELECTCAPTIONKIND ), CB_SETCURSEL, m_siSelectCaptionKindIdex, 0 );
	}
	else 
	{
		m_siSelectCaptionKindIdex  = -1;
		SendMessage( GetDlgItem(m_hDlg, IDC_COMBO_SELECTCAPTIONKIND ), CB_SETCURSEL, m_siSelectCaptionKindIdex, 0 );
	}


	if( pclClient->siServiceArea != ConstServiceArea_Korea )
	{
		if( FullScreenMode == 0 )
			FullScreenMode = 1;
	}

	if ( EffectSound > 0 )
	{
		m_bEffectSound = true;
		pclclient->pclMusic->pclSoundManager->EffectSoundOn(true);
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_EFFECT ), BM_SETCHECK, BST_CHECKED, 0 );
	}
	else
	{
		m_bEffectSound = false;
		pclclient->pclMusic->pclSoundManager->EffectSoundOn(false);
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_EFFECT ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}
	
	if( BackGroundSound > 0 )
	{
		m_bBackGroudnSound = true;
		pclclient->pclMusic->pclSoundManager->BackGroundSoundOn(true);
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_BACKGROUND ), BM_SETCHECK, BST_CHECKED, 0 );
	}
	else
	{
		m_bBackGroudnSound = false;
		pclclient->pclMusic->pclSoundManager->BackGroundSoundOn(false);
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_BACKGROUND ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}

	if ( MiniMapShow > 0 )
	{
		m_bMiniMapShow = true;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_MINIMAP_REJECT ), BM_SETCHECK, BST_CHECKED, 0 );
	}
	else
	{
		m_bMiniMapShow = false;		
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_MINIMAP_REJECT ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}

	if ( FullScreenMode > 0 )
	{
		m_bFullScreenMode = true;
		m_bTmpFullScreenMode = true;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_WINDOWMODE ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}
	else
	{
		m_bFullScreenMode = false;	
		m_bTmpFullScreenMode = false;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_WINDOWMODE ), BM_SETCHECK, BST_CHECKED, 0 );
	}

	SI32 bFullScreen = 0;

	GetFullScreenInFile( &bFullScreen );

	if(bFullScreen)
		MaxWindowMode = 1;
	else
		MaxWindowMode = 0;

	if( MaxWindowMode > 0 )
	{
		m_bMaxWindowMode = true;
		m_bTmpMaxWindowMode = true;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_MAXWINDOWMODE ), BM_SETCHECK, BST_CHECKED, 0 );

		if( m_bFullScreenMode == false )
		{
			m_bFullScreenMode = true;
			m_bTmpFullScreenMode = true;
			SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_WINDOWMODE ), BM_SETCHECK, BST_UNCHECKED, 0 );
		}

		EnableWindow( GetDlgItem( m_hDlg, IDC_CHECK_WINDOWMODE ), FALSE );
	}
	else
	{
		m_bMaxWindowMode = false;
		m_bTmpMaxWindowMode = false;
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_MAXWINDOWMODE ), BM_SETCHECK, BST_UNCHECKED, 0 );

		EnableWindow( GetDlgItem( m_hDlg, IDC_CHECK_WINDOWMODE ), TRUE );
	}

	fclose( fp );

	return;*/
}

void CSoundOnOffDlg::WriteSoundConfig()
{
	/*
	FILE *fp = _tfopen( TEXT("./Config/SoundConfig.dat"), TEXT("wt") );
	if ( fp == NULL )
	{
		return;
	}

	if ( m_bEffectSound )
	{
		_ftprintf( fp, TEXT("%d "), 1 );
	}
	else
	{
		_ftprintf( fp, TEXT("%d "), 0 );
	}

	if ( m_bBackGroudnSound )
	{
		_ftprintf( fp, TEXT("%d "), 1 );
	}
	else
	{
		_ftprintf( fp, TEXT("%d "), 0 );
	}

	if ( m_bMiniMapShow )
	{
		_ftprintf( fp, TEXT("%d "), 1 );
	}
	else
	{
		_ftprintf( fp, TEXT("%d "), 0 );
	}

	if( m_bTmpFullScreenMode )
	{
		_ftprintf( fp, TEXT("%d "), 1 );
	}
	else
	{
		_ftprintf( fp, TEXT("%d "), 0 );
	}


	if( m_bRightMouseMentAllow   ) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bWhisperChatReject   ) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bPrivateTradeReject   ) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bJewelItemNotifyReject   ) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bPartyInviteNotifyReject   ) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bFriendAddNotifyReject   ) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bSummonEvolveNotifyReject   ) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bNotifyHeadChat   ) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bNotifyMinigame   ) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_siSelectCaptionKindIdex >= 0 &&  m_siSelectCaptionKindIdex < MAX_CAPTIONKIND_NUM ) 	_ftprintf( fp, TEXT("%d "), m_siSelectCaptionKindIdex );	 else 	_ftprintf( fp, TEXT("%d "), -1 );

	if( m_bTmpMaxWindowMode ) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bChatFixed )	_ftprintf( fp, TEXT("%d"), 1 );	else _ftprintf( fp, TEXT("%d"), 0 );


	fclose( fp );
*/
	return;
}

void CSoundOnOffDlg::SendCurrentSettingInfo()
{
	if( pclClient )
	{
		cltClient *pclclient = (cltClient *)pclClient;

		if ( pclClient->GetGameStatus() == GS_GAME )
		{
			// Background Music On/Off
			if( pclclient->pclMusic && pclclient->pclMusic->pclSoundManager )
			{
				pclclient->pclMusic->pclSoundManager->BackGroundSoundOn(m_bBackGroudnSound);
				if(m_bBackGroudnSound) 
					pclclient->PlayBackgroundMusic( pclClient->pclCM->CR[1]->GetMapIndex() );
			}

			//  Send Msg to Server
			if( pclClient->pclCM && pclClient->pclCM->CR[1] )
			{
				// WhisperChat Reject On/Off
				cltGameMsgRequest_WhisperChat_Reject clinfo1( m_bWhisperChatReject );
				cltMsg clMsg1(GAMEMSG_REQUEST_WHISPERCHAT_REJECT, sizeof(clinfo1), (BYTE*)&clinfo1);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg1);

				// Private Trade Reject On/Off
				cltGameMsgRequest_PrivateTrade_Reject clinfo2( m_bPrivateTradeReject );
				cltMsg clMsg2(GAMEMSG_REQUEST_PRIVATETRADE_REJECT, sizeof(clinfo2), (BYTE*)&clinfo2);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg2);

				// PartyInvite Reject On/Off
				cltGameMsgRequest_PartyInvite_Reject clinfo3( m_bPartyInviteNotifyReject );
				cltMsg clMsg3(GAMEMSG_REQUEST_PARTYINVITE_REJECT, sizeof(clinfo3), (BYTE*)&clinfo3);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg3);

                // Friend Add Reject On/Off
				cltGameMsgRequest_FriendAdd_Reject clinfo4( m_bFriendAddNotifyReject );
				cltMsg clMsg4(GAMEMSG_REQUEST_FRIENDADD_REJECT, sizeof(clinfo4), (BYTE*)&clinfo4);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg4);
			} // if( pclClient->pclCM && pclClient->pclCM->CR[1] )
		} // if ( pclClient->GetGameStatus() == GS_GAME )
	} // if( pclClient )
}
