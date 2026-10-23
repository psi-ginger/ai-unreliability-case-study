#include "QuizSubmitDlg.h"

#include "..\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\NotifyMsgDlg\NotifyMsgDlg.h"

#include "..\..\..\CommonLogic\Msg\MsgType-Quiz.h"

#include "..\QuizMgrDlg\QuizMgrDlg.h"

#include "..\..\StatusBar\ChatStatusBar\ChatStatusBarBaseInfo.h"
#include "..\..\StatusBar\ChatStatusBar\ChatStatusBarBuffer.h"
#include "..\..\StatusBar\ChatStatusBar\ChatStatusBar.h"

#include "../../Client/Music/Music.h"

#include "../../Client/client.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CQuizSubmitDlg::CQuizSubmitDlg()
{	
	m_hDlg = NULL;
	m_QuizAnswerList = NULL;

	m_bQuizPauseStatus = true;

	m_siPrizeMoney = 0;

	m_siQuizMaxOXUserNum = 0;

	m_siQuizStartModeSelectIndex = -1;

	if ( m_QuizAnswerList == NULL )
	{
		m_QuizAnswerList = new stAnswer[ MAX_QUIZ_ANSWER_LIST_NUM ];
	}

	Initialize();
}

CQuizSubmitDlg::~CQuizSubmitDlg()
{
	if ( m_QuizAnswerList )
	{
		delete [] m_QuizAnswerList;
		m_QuizAnswerList = NULL;
	}
	
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CQuizSubmitDlg::Initialize()
{
	m_siQuizCreateKind = 0;
	m_bShow = false;
	m_bQuizStatus = false;
	m_siQuizIndex = 0;
	m_siQuizNumber = 0;
	m_siVillageIndex = 0;
	m_bQuizPauseStatus = true;
	m_siAnswerListCount = 0;
	m_siPrizeMoney = 0;
	m_siQuizStartModeSelectIndex = -1;
	m_siQuizMaxOXUserNum = 0;
	m_siQuizCurrentOXUserNum = 0;
	return;
}

void CQuizSubmitDlg::Destroy()
{
	return;
}

void CQuizSubmitDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_QUIZ_SUBMIT), hWnd, StaticQuizSubmitDlgProc );
	

	//Hide();

	return;
}

LRESULT CALLBACK CQuizSubmitDlg::StaticAnswerSubmitEditSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient *)pclClient;
	return pclclient->pclQuizMgrDlg->m_pQuizSubmitDlg->AnswerSubmitEditSubProc( hWnd, iMsg, wParam, lParam );
}

LRESULT CALLBACK CQuizSubmitDlg::AnswerSubmitEditSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch( iMsg ) 
	{
	case WM_CHAR :
		{
			if( wParam == VK_RETURN )
			{
				if ( m_hDlg )
				{
					m_bQuizPauseStatus = true;

					char Buffer[ MAX_QUIZ_ANSWER_LENGTH ] = "";
					
					if ( m_siQuizCreateKind != QUIZ_KIND_OX )
					{
						GetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_ANSWERSUBMIT), Buffer, MAX_QUIZ_ANSWER_LENGTH - 1 );
						strcat( Buffer, "" );
						
						if ( strlen( Buffer ) < 1 )
						{
							break;
						}

						SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_ANSWERSUBMIT), "" );

						// 퀴즈가 생성 된 것 체크				
						if ( m_bQuizStatus && m_siQuizIndex > 0 && m_siQuizNumber > 0 )
						{
							cltGameMsgRequest_Quiz_AnswerSubmit clQuizAnswerSubmit( m_siQuizIndex, m_siQuizNumber, Buffer, -1, -1 );
							cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_ANSWERSUBMIT, sizeof(clQuizAnswerSubmit), (char*)&clQuizAnswerSubmit);
							pclClient->SendMsgToServer((char*)&clMsg);

							pclClient->PushEffect( EFFECT_SUCCESS, 0, 0 );
						}
					}
				}
			}
		}
		break;
	}

	return CallWindowProc( m_OldAnswerSubmitEditProc, hWnd, iMsg, wParam, lParam );
}


BOOL CALLBACK CQuizSubmitDlg::StaticQuizSubmitDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient *)pclClient;
	return pclclient->pclQuizMgrDlg->m_pQuizSubmitDlg->QuizSubmitDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CQuizSubmitDlg::QuizSubmitDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient *)pclClient;
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;

			SendMessage( GetDlgItem( hDlg, IDC_EDIT_QUIZ_SUBMIT ), EM_LIMITTEXT, (WPARAM) MAX_QUIZ_SUBMIT_LENGTH - 1, (LPARAM) 0 );
			SendMessage( GetDlgItem( hDlg, IDC_EDIT_ANSWERSUBMIT ), EM_LIMITTEXT, (WPARAM) MAX_QUIZ_ANSWER_LENGTH - 1, (LPARAM) 0 );

			ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_1 ), SW_HIDE );
			ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_2 ), SW_HIDE );
			ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_3 ), SW_HIDE );
			ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_4 ), SW_HIDE );
			ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_O ), SW_HIDE );
			ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_X ), SW_HIDE );

			m_OldAnswerSubmitEditProc = (WNDPROC)SetWindowLong( GetDlgItem( hDlg, IDC_EDIT_ANSWERSUBMIT ), GWL_WNDPROC, (LONG)StaticAnswerSubmitEditSubProc );
		}
		return TRUE;	
	case WM_COMMAND:
		{
			switch ( LOWORD(wParam) )
			{
			case IDC_COMBO_SELECT_QUIZMODE:
				{
					switch( HIWORD( wParam ))
					{
					case CBN_SELCHANGE:
						{
							SI32 SelectedIndex = SendMessage( GetDlgItem( hDlg, IDC_COMBO_SELECT_QUIZMODE ), CB_GETCURSEL, 0, 0 );
							if ( CB_ERR == SelectedIndex )
							{
								return FALSE;
							}
							
							char Buffer[256] = "";
							SendMessage( GetDlgItem( hDlg, IDC_COMBO_SELECT_QUIZMODE ), CB_GETLBTEXT, SelectedIndex, (LPARAM)Buffer );
							
							char* pText[4];

							pText[0] = GetTxtFromMgr(2480);
							pText[1] = GetTxtFromMgr(2481);
							pText[2] = GetTxtFromMgr(2482);
							pText[3] = GetTxtFromMgr(1381);

							if ( strcmp( Buffer, pText[0] ) == 0 )
							{
								m_siQuizStartModeSelectIndex = QUIZ_KIND_VILLAGE;
							}
							else if ( strcmp( Buffer, pText[1] ) == 0 )
							{
								m_siQuizStartModeSelectIndex = QUIZ_KIND_GOONZU;
							}
							else if ( strcmp( Buffer, pText[2] ) == 0 )
							{
								m_siQuizStartModeSelectIndex = QUIZ_KIND_GAMEMASTER;
							}
							else if ( strcmp( Buffer, pText[3] ) == 0 )
							{
								m_siQuizStartModeSelectIndex = QUIZ_KIND_OX;
							}
							else
							{
								m_siQuizStartModeSelectIndex = -1;
							}
						}
						break;
					}
				}
				break;

			case IDC_BUTTON_QUIZCREATE:
				{
					if ( m_siQuizStartModeSelectIndex < 0 )
					{
						char* pTitle = GetTxtFromMgr(2497);
						char* pText = GetTxtFromMgr(2498);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return FALSE;
					}
					
					if ( m_bQuizStatus )
					{
						char* pTitle = GetTxtFromMgr(2499);
						char* pText = GetTxtFromMgr(2500);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return FALSE;
					}
					
					SetWindowText( GetDlgItem( hDlg, IDC_EDIT_PRIZEMONEY ), "0" );
					SetWindowText( GetDlgItem( hDlg, IDC_EDIT_QUIZOXMAXUSERNUM ), "0" );
										
					m_bQuizPauseStatus = true;
					
					switch( m_siQuizStartModeSelectIndex )
					{
					case QUIZ_KIND_VILLAGE:
						{
							if ( pclClient->pclCM->CR[1]->pclCI->clCharRank.GetRankType() == RANKTYPE_CITYHALL )
							{
								SI32 VillageUnique = pclClient->pclCM->CR[1]->pclCI->clCharRank.GetVillageUnique();
								if ( VillageUnique < 1 )
									return FALSE;
								
								cltGameMsgRequest_Quiz_Create clQuizCreate( m_siQuizStartModeSelectIndex, VillageUnique );
								cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_CREATE, sizeof(clQuizCreate), (char*)&clQuizCreate);
								pclClient->SendMsgToServer((char*)&clMsg);
							}
							else
							{
								char* pTitle = GetTxtFromMgr(2501);
								char* pText = GetTxtFromMgr(2502);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
							}
						}
						break;
					case QUIZ_KIND_GOONZU:
						{
							if ( pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_KING )
							{
								cltGameMsgRequest_Quiz_Create clQuizCreate( m_siQuizStartModeSelectIndex, MASTER_VILLAGE_INDEX );
								cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_CREATE, sizeof(clQuizCreate), (char*)&clQuizCreate);
								pclClient->SendMsgToServer((char*)&clMsg);
							}
						}
						break;
					case QUIZ_KIND_GAMEMASTER:
						{
							if ( pclClient->pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL + 1 )
							{
								cltGameMsgRequest_Quiz_Create clQuizCreate( m_siQuizStartModeSelectIndex, MASTER_VILLAGE_INDEX );
								cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_CREATE, sizeof(clQuizCreate), (char*)&clQuizCreate);
								pclClient->SendMsgToServer((char*)&clMsg);
							}
						}
						break;
					case QUIZ_KIND_OX:
						{
							if ( pclClient->pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL + 1 )
							{
								//////////////////////////////////////////////////////////////////////////
								// OX 퀴즈 이용권이 있는지 확인
								SI32 TicketItemUnique = 14030;			//14030 은 OX 퀴즈 이용권 아이템 Unique 값
								
								// OX퀴즈 이용권이 준비되어 있는지 확인한다. 
								SI32 TicketPos = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemPos( TicketItemUnique );
								
								// 위치 확인
								if( TicketPos < PERSONITEM_INV0 || TicketPos >= PERSONITEM_INV0 + MAX_ITEM_PER_INVENTORY )
								{
									char* pTitle = GetTxtFromMgr(2503);
									char* pText = GetTxtFromMgr(2504);
									pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
									return FALSE;
								}

								char szCreateOXQuiz[256] = "";
								char* pTitle = GetTxtFromMgr(2506);
								char* pText = GetTxtFromMgr(2505);
								sprintf( szCreateOXQuiz, pText, MIN_PRIZEMONEY_OXQUIZ );

								if( MessageBox( NULL, szCreateOXQuiz, pTitle, MB_ICONQUESTION | MB_YESNO) == IDNO ) 
								{
									return FALSE;
								}
								//////////////////////////////////////////////////////////////////////////
							}
							
							cltGameMsgRequest_Quiz_Create clQuizCreate( m_siQuizStartModeSelectIndex, MASTER_VILLAGE_INDEX );
							cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_CREATE, sizeof(clQuizCreate), (char*)&clQuizCreate);
							pclClient->SendMsgToServer((char*)&clMsg);
						}
						break;
					}
				}
				break;
			case IDOK:							// 문제 제출
				{
					if ( m_bQuizStatus == false )
					{
						char* pTitle = GetTxtFromMgr(2507);
						char* pText = GetTxtFromMgr(2508);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						return FALSE;
					}
					  
					if ( m_siQuizCreateKind == QUIZ_KIND_OX )
					{
						if ( pclClient->pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL + 1 )
						{
							if ( m_siPrizeMoney < MIN_PRIZEMONEY_OXQUIZ )
							{
								char* pTitle = GetTxtFromMgr(2509);
								char* pText = GetTxtFromMgr(2510);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, MIN_PRIZEMONEY_OXQUIZ );
								return FALSE;
							}

							if ( m_siPrizeMoney > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
							{
								char* pTitle = GetTxtFromMgr(2511);
								char* pText = GetTxtFromMgr(2512);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
								return FALSE;
							}
						}
					}
					else
					{
						if ( m_siPrizeMoney < 1 )
						{
							char* pTitle = GetTxtFromMgr(2513);
							char* pText = GetTxtFromMgr(2514);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
							return FALSE;
						}
						
						if ( m_siPrizeMoney > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
						{
							char* pTitle = GetTxtFromMgr(2511);
							char* pText = GetTxtFromMgr(2512);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
							return FALSE;
						}
					}

					m_bQuizPauseStatus = false;

					SendMessage( GetDlgItem( m_hDlg, IDC_LIST_ANSWER ), LB_RESETCONTENT, 0, 0 );
					SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_ANSWERSUBMIT ), "" );

					m_siAnswerListCount = 0;

					char Buffer[ MAX_QUIZ_SUBMIT_LENGTH ] = "";
					memset( Buffer, 1, MAX_QUIZ_SUBMIT_LENGTH );
					GetWindowText( GetDlgItem( hDlg, IDC_EDIT_QUIZ_SUBMIT), Buffer, MAX_QUIZ_SUBMIT_LENGTH - 1 );
					strcat( Buffer, "" );

					if ( strlen( Buffer ) < 1 )
					{
						return FALSE;
					}

					for ( int i = 0; i < MAX_QUIZ_ANSWER_LIST_NUM; ++i )
					{
						m_QuizAnswerList[ i ].Init();
						m_siAnswerListCount = 0;
					}

					// 퀴즈가 생성 된 것 체크					
					if ( m_bQuizStatus && m_siQuizIndex > 0 )
					{
						cltGameMsgRequest_Quiz_Submit clQuizSubmit( m_siQuizIndex, Buffer );
						cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_SUBMIT, sizeof( clQuizSubmit ), (char*)&clQuizSubmit);
						pclClient->SendMsgToServer((char*)&clMsg);
						++m_siQuizNumber;
						
						char* pTitle = GetTxtFromMgr(2515);
						char* pText = GetTxtFromMgr(2516);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						
						pclClient->PushEffect( EFFECT_SUCCESS, 0, 0 );
					}
				}
				break;
			case IDC_BUTTON_ANSWERSUBMIT:		// 정답 발표
				{
					if ( m_bQuizStatus == false )
					{
						char* pTitle = GetTxtFromMgr(2517);
						char* pText = GetTxtFromMgr(2518);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return FALSE;
					}

					m_bQuizPauseStatus = true;

					if ( m_siQuizCreateKind == QUIZ_KIND_OX )
					{
						SI16 QuizAnswer1 = -1;
						SI16 QuizAnswer2 = -1;
						if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_1 ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							if ( QuizAnswer1 == -1 )
							{
								QuizAnswer1 = 0;
							}
							else if( QuizAnswer2 == -1 )
							{
								QuizAnswer2 = 0;
							}
							else
							{
								char* pTitle = GetTxtFromMgr(5157);
								char* pText = GetTxtFromMgr(5156);
								MessageBox( pclClient->GetHwnd(), pText, pTitle, MB_OK );
								return FALSE;
							}
						}

						if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_2 ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							if ( QuizAnswer1 == -1 )
							{
								QuizAnswer1 = 1;
							}
							else if( QuizAnswer2 == -1 )
							{
								QuizAnswer2 = 1;
							}
							else
							{
								char* pTitle = GetTxtFromMgr(5157);
								char* pText = GetTxtFromMgr(5156);
								MessageBox( pclClient->GetHwnd(), pText, pTitle, MB_OK );
								return FALSE;
							}
						}

						if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_3 ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							if ( QuizAnswer1 == -1 )
							{
								QuizAnswer1 = 2;
							}
							else if( QuizAnswer2 == -1 )
							{
								QuizAnswer2 = 2;
							}
							else
							{
								char* pTitle = GetTxtFromMgr(5157);
								char* pText = GetTxtFromMgr(5156);
								MessageBox( pclClient->GetHwnd(), pText, pTitle, MB_OK );
								return FALSE;
							}
						}

						if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_4 ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							if ( QuizAnswer1 == -1 )
							{
								QuizAnswer1 = 3;
							}
							else if( QuizAnswer2 == -1 )
							{
								QuizAnswer2 = 3;
							}
							else
							{                              
								char* pTitle = GetTxtFromMgr(5157);
								char* pText = GetTxtFromMgr(5156);
								MessageBox( pclClient->GetHwnd(), pText, pTitle, MB_OK );
								return FALSE;
							}
						}

						if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_O ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							if ( QuizAnswer1 == -1 )
							{
								QuizAnswer1 = 4;
							}
							else if( QuizAnswer2 == -1 )
							{
								QuizAnswer2 = 4;
							}
							else
							{
								char* pTitle = GetTxtFromMgr(5157);
								char* pText = GetTxtFromMgr(5156);
								MessageBox( pclClient->GetHwnd(), pText, pTitle, MB_OK );
								return FALSE;
							}
						}

						if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_X ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							if ( QuizAnswer1 == -1 )
							{
								QuizAnswer1 = 5;
							}
							else if( QuizAnswer2 == -1 )
							{
								QuizAnswer2 = 5;
							}
							else
							{
								char* pTitle = GetTxtFromMgr(5157);
								char* pText = GetTxtFromMgr(5156);
								MessageBox( pclClient->GetHwnd(), pText, pTitle, MB_OK );
								return FALSE;
							}
						}
						
						// 퀴즈가 생성 된 것 체크					
						if ( m_bQuizStatus && m_siQuizIndex > 0 && m_siQuizNumber > 0 )
						{
							char Buffer[ MAX_QUIZ_ANSWER_LENGTH ] = "";
							if ( QuizAnswer1 == -1 && QuizAnswer2 == -1 )
							{
								char* pTitle = GetTxtFromMgr(5158);
								char* pText = GetTxtFromMgr(5159);
								MessageBox( pclClient->GetHwnd(),  pTitle, pText,MB_OK );
								//MessageBox( NULL, "퀴즈 정답 미선택", "퀴즈 정답을 선택하신 후에 정답 발표 버튼을 눌러 주십시오.", MB_OK );
								return FALSE;
							}

							cltGameMsgRequest_Quiz_AnswerSubmit clQuizAnswerSubmit( m_siQuizIndex, m_siQuizNumber, Buffer, QuizAnswer1, QuizAnswer2 );
							cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_ANSWERSUBMIT, sizeof(clQuizAnswerSubmit), (char*)&clQuizAnswerSubmit);
							pclClient->SendMsgToServer((char*)&clMsg);

							char* pTitle = GetTxtFromMgr(2515);
							char* pText = GetTxtFromMgr(2519);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, Buffer );

							pclClient->PushEffect( EFFECT_SUCCESS, 0, 0 );
						}
					}
					else
					{
						char Buffer[ MAX_QUIZ_ANSWER_LENGTH ] = "";

						GetWindowText( GetDlgItem( hDlg, IDC_EDIT_ANSWERSUBMIT), Buffer, MAX_QUIZ_ANSWER_LENGTH - 1 );
						strcat( Buffer, "" );
						
						if ( strlen( Buffer ) < 1 )
						{
							return FALSE;
						}

						SetWindowText( GetDlgItem( hDlg, IDC_EDIT_ANSWERSUBMIT), "" );

						// 퀴즈가 생성 된 것 체크					
						if ( m_bQuizStatus && m_siQuizIndex > 0 && m_siQuizNumber > 0 )
						{
							cltGameMsgRequest_Quiz_AnswerSubmit clQuizAnswerSubmit( m_siQuizIndex, m_siQuizNumber, Buffer, -1, -1 );
							cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_ANSWERSUBMIT, sizeof(clQuizAnswerSubmit), (char*)&clQuizAnswerSubmit);
							pclClient->SendMsgToServer((char*)&clMsg);

							char* pTitle = GetTxtFromMgr(2515);
							char* pText = GetTxtFromMgr(2519);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, Buffer );

							pclClient->PushEffect( EFFECT_SUCCESS, 0, 0 );
						}
					}					
				}
				break;			
			case IDC_BUTTON_RIGHTANSWER:		// 정답자 선정
				{
					// 퀴즈가 생성 된 것 체크
					if ( m_bQuizStatus == false )
					{
						char* pTitle = GetTxtFromMgr(2520);
						char* pText = GetTxtFromMgr(2521);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return FALSE;
					}
					
					if ( m_siQuizIndex > 0 && m_siQuizNumber > 0 )
					{
						int num = SendMessage( GetDlgItem( hDlg, IDC_LIST_ANSWER ), LB_GETSELCOUNT, 0, 0 );
						
						if ( num < 1 )
						{
							return FALSE;
						}

						if ( m_siPrizeMoney * num > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
						{
							char* pTitle = GetTxtFromMgr(2511);
							char* pText = GetTxtFromMgr(2522);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
							return FALSE;
						}

						m_bQuizPauseStatus = true;

						int SelectIndex[ MAX_QUIZ_ANSWER_LIST_NUM ];

						SendMessage( GetDlgItem( hDlg, IDC_LIST_ANSWER ), LB_GETSELITEMS, MAX_QUIZ_ANSWER_LIST_NUM, (LPARAM) SelectIndex );

						for ( int i = 0; i < num; ++i )
						{
							cltGameMsgRequest_Quiz_RightAnswer clQuizRightAnswer( 
								m_QuizAnswerList[ SelectIndex[ i ] ].siPersonID, 
								m_QuizAnswerList[ SelectIndex[ i ] ].siCharUnique, 
								m_siQuizIndex,
								m_QuizAnswerList[ SelectIndex[ i ] ].siQuizNumber, 
								SelectIndex[ i ] );

							cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_RIGHTANSWER, sizeof(clQuizRightAnswer), (char*)&clQuizRightAnswer);
							pclClient->SendMsgToServer((char*)&clMsg);
						}

						char* pTitle = GetTxtFromMgr(2515);
						char* pText = GetTxtFromMgr(2523);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						pclClient->PushEffect( EFFECT_SUCCESS, 0, 0 );
					}					
				}
				break;
			case IDC_BUTTON_WRONGANSWER:		// 오답자 선정
				{
					// 퀴즈가 생성 된 것 체크					
					if ( m_siQuizIndex > 0 && m_siQuizNumber > 0 )
					{
						m_bQuizPauseStatus = true;
						
						int num = SendMessage( GetDlgItem( hDlg, IDC_LIST_ANSWER ), LB_GETSELCOUNT, 0, 0 );
						
						if ( num < 1 )
						{
							return FALSE;
						}
						
						int SelectIndex[ MAX_QUIZ_ANSWER_LIST_NUM ];
						
						SendMessage( GetDlgItem( hDlg, IDC_LIST_ANSWER ), LB_GETSELITEMS, MAX_QUIZ_ANSWER_LIST_NUM, (LPARAM) SelectIndex );
						
						for ( int i = 0; i < num; ++i )
						{
							cltGameMsgRequest_Quiz_WrongAnswer clQuizWrongAnswer( 
								m_QuizAnswerList[ SelectIndex[ i ] ].siPersonID, 
								m_QuizAnswerList[ SelectIndex[ i ] ].siCharUnique, 
								m_siQuizIndex,
								m_QuizAnswerList[ SelectIndex[ i ] ].siQuizNumber, 
								SelectIndex[ i ] );
							
							cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_WRONGANSWER, sizeof(clQuizWrongAnswer), (char*)&clQuizWrongAnswer);
							pclClient->SendMsgToServer((char*)&clMsg);
						}
						char* pTitle = GetTxtFromMgr(2515);
						char* pText = GetTxtFromMgr(2524);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						pclClient->PushEffect( EFFECT_SUCCESS, 0, 0 );
					}
				}
				break;
			case IDCANCEL:
				{
					// 퀴즈가 생성 된 것 체크					
					if ( m_bQuizStatus && m_siQuizIndex > 0 )
					{
						cltGameMsgRequest_Quiz_Stop clQuizStop( m_siQuizIndex );
						cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_STOP, sizeof(clQuizStop), (char*)&clQuizStop);
						pclClient->SendMsgToServer((char*)&clMsg);

						SetWindowText( GetDlgItem( hDlg, IDC_EDIT_PRIZEMONEY ), "0" );
						SetWindowText( GetDlgItem( hDlg, IDC_EDIT_QUIZOXMAXUSERNUM ), "0" );
						
						Initialize();				// 초기화

						cltClient *pclclient = (cltClient *)pclClient;
						pclclient->pclQuizMgrDlg->Hide();
					}
				}
				break;
			case IDC_BUTTON_QUIZCLEAR:			// 문제를 시작하기 위한 clear 작업
				{
					// 퀴즈가 생성 된 것 체크					
					if ( m_bQuizStatus && m_siQuizIndex > 0 && m_siQuizNumber > 0 )
					{
						m_bQuizPauseStatus = true;
						SetWindowText( GetDlgItem( hDlg, IDC_EDIT_QUIZ_SUBMIT ), "" );
						SetWindowText( GetDlgItem( hDlg, IDC_EDIT_ANSWERSUBMIT ), "" );

						cltGameMsgRequest_Quiz_SubmitClear clQuizSubmitClear( m_siQuizIndex );
						cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_SUBMITCLEAR, sizeof(clQuizSubmitClear), (char*)&clQuizSubmitClear);
						pclClient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_QUIZ_MAXOXUSERNUM:
				{
					if ( m_bQuizStatus == false )
					{
						char* pTitle = GetTxtFromMgr(2515);
						char* pText = GetTxtFromMgr(2526);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						return FALSE;
					}

					SI16 OXMaxQuizUserNum = GetDlgItemInt( hDlg, IDC_EDIT_QUIZOXMAXUSERNUM, NULL, FALSE );

					if ( OXMaxQuizUserNum < 0 )
					{
						return FALSE;
					}

					// OX 퀴즈 인지 확인
					if ( m_siQuizCreateKind != QUIZ_KIND_OX )
					{
						char* pTitle = GetTxtFromMgr(2515);
						char* pText = GetTxtFromMgr(2527);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,pText );
						return FALSE;
					}

					if ( OXMaxQuizUserNum > MAX_OXQUIZ_PARTICIPATION_USER_NUM )
					{
						char* pTitle = GetTxtFromMgr(2528);
						char* pText = GetTxtFromMgr(2529);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, MAX_OXQUIZ_PARTICIPATION_USER_NUM );
						return FALSE;
					}

					// 퀴즈가 생성 된 것 다시 한번 더 체크					
					if ( m_bQuizStatus && m_siQuizIndex > 0 )
					{
						cltGameMsgRequst_Quiz_MaxOXUserNum clQuizMaxOXUserNum( m_siQuizIndex, OXMaxQuizUserNum );
						cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_MAXOXUSERNUM, sizeof(clQuizMaxOXUserNum), (char*)&clQuizMaxOXUserNum);
						pclClient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			case IDC_BUTTON_PRIZEMONEY:
				{
					if ( m_bQuizStatus == false )
					{
						char* pTitle = GetTxtFromMgr(2525);
						char* pText = GetTxtFromMgr(2400);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return FALSE;
					}

					SI32 PrizeMoney = GetDlgItemInt( hDlg, IDC_EDIT_PRIZEMONEY, NULL, FALSE );

					if ( PrizeMoney < 1 )
					{
						return FALSE;
					}

					if ( m_siQuizCreateKind == QUIZ_KIND_OX )
					{
						if ( pclClient->pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL + 1 )
						{
							if ( PrizeMoney < MIN_PRIZEMONEY_OXQUIZ )
							{
								char* pTitle = GetTxtFromMgr(2509);
								char* pText = GetTxtFromMgr(2510);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, MIN_PRIZEMONEY_OXQUIZ );
								return FALSE;
							}
						}
					}


					if ( PrizeMoney > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
					{
						char* pTitle = GetTxtFromMgr(2513);
						char* pText = GetTxtFromMgr(2401);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return FALSE;
					}

					if ( PrizeMoney > MAX_QUIZ_PRIZEMONEY )
					{
						char* pTitle = GetTxtFromMgr(2402);
						char* pText = GetTxtFromMgr(2403);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, MAX_QUIZ_PRIZEMONEY );
						return FALSE;
					}

					// 퀴즈가 생성 된 것 체크					
					if ( m_bQuizStatus && m_siQuizIndex > 0 )
					{
						m_siPrizeMoney = PrizeMoney;
						cltGameMsgRequest_Quiz_PrizeMoneySubmit clQuizPrizeMoneySubmit( m_siQuizIndex, PrizeMoney );
						cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_PRIZEMONEYSUBMIT, sizeof(clQuizPrizeMoneySubmit), (char*)&clQuizPrizeMoneySubmit);
						pclClient->SendMsgToServer((char*)&clMsg);
					}
				}
				break;
			}
		}
		break;

	case WM_CLOSE:
		{
			cltGameMsgRequest_Quiz_Stop clQuizStop( m_siQuizIndex );
			cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_STOP, sizeof(clQuizStop), (char*)&clQuizStop);
			pclClient->SendMsgToServer((char*)&clMsg);

			Initialize();				// 초기화
		}
		break;
		
	case WM_DESTROY:
		{
			SetWindowLong( GetDlgItem( hDlg, IDC_EDIT_ANSWERSUBMIT ), GWL_WNDPROC, (LONG) m_OldAnswerSubmitEditProc );

			m_hDlg = NULL;			
		}
		break;
	}

	return FALSE;
}

bool CQuizSubmitDlg::IsShow()
{
	return m_bShow;
}

void CQuizSubmitDlg::Show()
{
	HWND hComboSelectQuizMonde = GetDlgItem( m_hDlg, IDC_COMBO_SELECT_QUIZMODE );
	SendMessage( hComboSelectQuizMonde, CB_RESETCONTENT, 0, 0 );

	if ( pclClient->pclCM->CR[1]->pclCI->clCharRank.GetRankType() == RANKTYPE_CITYHALL )
	{
		char* pText = GetTxtFromMgr(2480);
		SendMessage( hComboSelectQuizMonde, CB_ADDSTRING, 0, (LPARAM)pText );
	}

	if ( pclClient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_KING )
	{
		char* pText = GetTxtFromMgr(2481);
		SendMessage( hComboSelectQuizMonde, CB_ADDSTRING, 0, (LPARAM)pText );
	}

	if ( pclClient->pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL + 1 )
	{
		char* pText = GetTxtFromMgr(2482);
		SendMessage( hComboSelectQuizMonde, CB_ADDSTRING, 0, (LPARAM)pText );
	}

	//if ( pclClient->pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL + 1 )
	//{
		char* pText = GetTxtFromMgr(1381);
		SendMessage( hComboSelectQuizMonde, CB_ADDSTRING, 0, (LPARAM)pText );
	//}

	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );

	return;
}

void CQuizSubmitDlg::Hide()
{
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CQuizSubmitDlg::InterfaceInit()
{
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUIZ_SUBMIT ), "" );
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_ANSWERSUBMIT ), "" );

	SendMessage( GetDlgItem( m_hDlg, IDC_LIST_ANSWER ), LB_RESETCONTENT, 0, 0 );
	
	Initialize();
	return;
}

bool CQuizSubmitDlg::IsQuizStatus()
{
	return m_bQuizStatus;
}

void CQuizSubmitDlg::QuizCreate( SI16 QuizIndex, SI08 QuizCreateKind, SI16 VillageIndex )
{
	cltClient *pclclient = (cltClient *)pclClient;
	InterfaceInit();

	m_bQuizStatus = true;
	m_bQuizPauseStatus = false;
	m_siQuizIndex = QuizIndex;

	m_siQuizCreateKind = QuizCreateKind;
	m_siVillageIndex = VillageIndex;

	// 운영자 또는 왕 퀴즈 또는 OX 퀴즈
	if ( m_siQuizCreateKind == QUIZ_KIND_GOONZU || m_siQuizCreateKind == QUIZ_KIND_GAMEMASTER || m_siQuizCreateKind == QUIZ_KIND_OX )
	{
		pclClient->PushEffect( EFFECT_QUIZSTART, 0, 0 );
	}
	
	if ( m_siQuizCreateKind == QUIZ_KIND_OX )
	{
		ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_1 ), SW_SHOW );
		ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_2 ), SW_SHOW );
		ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_3 ), SW_SHOW );
		ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_4 ), SW_SHOW );
		ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_O ), SW_SHOW );
		ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_X ), SW_SHOW );
		
		ShowWindow( GetDlgItem( m_hDlg, IDC_EDIT_ANSWERSUBMIT ), SW_HIDE );

		char* pTitle = GetTxtFromMgr(2501);
		char* pText = GetTxtFromMgr(2403);
		if ( pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
		{
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, MAX_QUIZ_SUBMIT_ALLOW_TIME/1000/60 );
		}
		
	}
	else
	{
		ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_1 ), SW_HIDE );
		ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_2 ), SW_HIDE );
		ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_3 ), SW_HIDE );
		ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_4 ), SW_HIDE );
		ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_O ), SW_HIDE );
		ShowWindow( GetDlgItem( m_hDlg, IDC_CHECK_X ), SW_HIDE );
		ShowWindow( GetDlgItem( m_hDlg, IDC_EDIT_ANSWERSUBMIT ), SW_SHOW );

		char* pTitle = GetTxtFromMgr(2501);
		char* pText = GetTxtFromMgr(2405);
		if ( pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
		{
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		}
		
	}

	return;
}

void CQuizSubmitDlg::QuizAnswer( SI32 PersonID, SI32 CharUnique, SI16 AnswerListIndex, char *CharName, char *Answer )
{
	// 정답 그만 받기를 눌렀을 경우임
	if ( m_bQuizPauseStatus )
	{
		return;
	}

	HWND hAnswerListBox = GetDlgItem( m_hDlg, IDC_LIST_ANSWER );

	m_QuizAnswerList[ m_siAnswerListCount++ ].Set( PersonID, CharUnique, m_siQuizNumber, Answer );

	char Buffer[ MAX_QUIZ_ANSWER_LENGTH + 30 ] = "";
	sprintf( Buffer, "%s-%s", CharName, Answer );
	SendMessage( hAnswerListBox, LB_ADDSTRING, 0, (LPARAM)Buffer );
	
	return;
}

void CQuizSubmitDlg::QuizStop()
{
	cltGameMsgRequest_Quiz_Stop clQuizStop( m_siQuizIndex );
	cltMsg clMsg(GAMEMSG_REQUEST_QUIZ_STOP, sizeof(clQuizStop), (char*)&clQuizStop);
	pclClient->SendMsgToServer((char*)&clMsg);
	Initialize();
	return;
}

void CQuizSubmitDlg::QuizMaxOXUserNum( SI16 QuizIndex, SI16 QuizMaxOXUserNum, SI16 QuizCurrentOXUserNum )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( m_siQuizIndex != QuizIndex )
	{
		return;
	}

	pclClient->PushEffect( EFFECT_SUCCESS, 0, 0 );
	char* pTitle = GetTxtFromMgr(1381);
	char* pText = GetTxtFromMgr(2406);
	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, QuizMaxOXUserNum );

	m_siQuizMaxOXUserNum = QuizMaxOXUserNum;
	SetDlgItemInt( m_hDlg, IDC_EDIT_QUIZOXMAXUSERNUM, QuizMaxOXUserNum, FALSE );

	m_siQuizCurrentOXUserNum = QuizCurrentOXUserNum;
	char Buffer[256 ] = "";
	pText = GetTxtFromMgr(2407);
	sprintf( Buffer, pText, QuizCurrentOXUserNum );
	SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_QUIZOXCURRENTUSERNUM ), Buffer );

	return;
}

void CQuizSubmitDlg::QuizCurrentOXUserNum( SI16 QuizIndex, SI16 QuizCurrentOXUserNum )
{
	if ( m_siQuizIndex != QuizIndex )
	{
		return;
	}
	
	m_siQuizCurrentOXUserNum = QuizCurrentOXUserNum;
	char Buffer[256 ] = "";
	char* pText = GetTxtFromMgr(2407);
	sprintf( Buffer, pText, QuizCurrentOXUserNum );
	SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_QUIZOXCURRENTUSERNUM ), Buffer );
	return;
}
