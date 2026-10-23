#include "../../Client/client.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/Msg/MsgType-Village.h"
#include "../../CommonLogic/Msg/MsgType-TugOfWar.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"

#include "../../Client/Effect/BattleZoneCounter/BattleZoneCounter.h"

#include "../../Music/Music.h"
#include "../../StatusBar/ChatStatusBar/ChatStatusBar.h"
#include "../../StatusBar/ChatStatusBar/ChatStatusBarBuffer.h"
#include "../SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "../../NInterface/NChatDlg/NChatDlg.h"

#include "../QuizMgrDlg/QuizMgrDlg.h"

#include "../../../lib/WebHTML/WebHTML.h"

#include "OnGameNetEnviromentDlg.h"

extern cltCommonLogic* pclClient;

class cltOnGameItemInfo
{
public:
	char szItemName[128] ;
	SI32 siItemNum;
};

cltOnGameItemInfo clItemInfo[] =
{
	"금괴",10,
/*	"붕어매운탕",50,
	"백마망아지",1,
	"말능력향상이용권",1,
	"순간이동이용권",30,
	"기술부분초기화이용권",1,
	"명중부적",50,
	"개인상점이용권",10,
	"가미대보탕",50,*/
	NULL ,0
};

COnGameNetEnviromentDlg::COnGameNetEnviromentDlg()
{
	m_hDlg = NULL;
	m_bShow = false;

	m_siSelectVillageIndex = 0;
	m_siSelectMinuteIndex = 0;
	m_siSelectLevelIndex = 0;
	m_siSelectNumIndex = 0;

	for(SI32 i=0; i<MAX_ONGAMENETENVIROMENT_PAGE_NUM; i++)
		TabDlg[i] = NULL;

	m_CurPage = 0;

	m_CurTugMode = cltTugOfWar::GAMEMODE_ONEGOAL;
}

COnGameNetEnviromentDlg::~COnGameNetEnviromentDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

BOOL CALLBACK COnGameNetEnviromentDlg::StaticOnGameNetMainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pOnGameNetEnviromentDlg->OnGameNetMainDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK COnGameNetEnviromentDlg::OnGameNetMainDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 i;
	HWND hwnd;

	cltClient* pclclient = (cltClient*)pclClient;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();

			m_hDlg = hDlg;

			hwnd = GetDlgItem(hDlg, IDC_TAB_ONGAMENET);

			TC_ITEM item;
			item.mask = TCIF_TEXT;
			item.iImage = -1;
			item.pszText = GetTxtFromMgr(1249);	// 결투장
			TabCtrl_InsertItem(hwnd, 0, &item);

			item.pszText = GetTxtFromMgr(5459);	// 줄다리기
			TabCtrl_InsertItem(hwnd, 1, &item);

			m_CurPage = 0;

			switch(m_CurPage)
			{
			case 0:
				CreateTabDlg(hDlg, m_CurPage, IDD_DIALOG_ONGAMENETENVIROMENT, StaticOnGameNetEnviromentDlgProc);
				break;
			case 1:
				CreateTabDlg(hDlg, m_CurPage, IDD_DIALOG_ONGAMENETENVIROMENT, StaticOnGameNetEnviromentDlgProc);
				break;
			default :
				m_CurPage = 0;
				CreateTabDlg(hDlg, m_CurPage, IDD_DIALOG_ONGAMENETENVIROMENT, StaticOnGameNetEnviromentDlgProc);
				break;
			}
		}
		return TRUE;
	case WM_CTLCOLORDLG:
		{
			HDC hdc = (HDC)wParam;
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, COLOR_DIALOG_BLACK);
			SetBkColor(hdc, COLOR_DIALOG_STATIC_BLUE);
		}
		return TRUE;
	case WM_NOTIFY:
		{
			NMHDR FAR *tem = (NMHDR FAR*)lParam;

			if(tem->code == TCN_SELCHANGE)
			{
				hwnd = GetDlgItem(hDlg, IDC_TAB_ONGAMENET);

				if(TabDlg[m_CurPage])
				{
					ShowWindow(TabDlg[m_CurPage], SW_HIDE);
				}

				m_CurPage = TabCtrl_GetCurSel(tem->hwndFrom);

				switch(m_CurPage)
				{
				case 0:
					CreateTabDlg(hDlg, m_CurPage, IDD_DIALOG_ONGAMENETENVIROMENT, StaticOnGameNetEnviromentDlgProc);
					break;
				case 1:
					CreateTabDlg(hDlg, m_CurPage, IDD_DIALOG_ONGAMENET_TUG, StaticOnGameNetTugOfWarDlgProc);
					break;
				default :
					m_CurPage = 0;
					CreateTabDlg(hDlg, m_CurPage, IDD_DIALOG_ONGAMENETENVIROMENT, StaticOnGameNetEnviromentDlgProc);
					break;
				}
			}

		}
		break;
	case WM_CHAR:
		return TRUE;
	case WM_DESTROY:
		{
            for(i=0; i<MAX_ONGAMENETENVIROMENT_PAGE_NUM; i++)
			{
				if(TabDlg[i])
				{
					DestroyWindow(TabDlg[i]);
					TabDlg[i] = NULL;
				}
			}

			m_hDlg = NULL;
		}
		return TRUE;
	case WM_COMMAND:
		{
			switch(wParam)
			{
			case IDOK:
				Hide();//DestroyWindow(m_hDlg);
				return TRUE;
			case IDCANCEL:
				Hide();//DestroyWindow(m_hDlg);
				return TRUE;
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK COnGameNetEnviromentDlg::StaticOnGameNetTugOfWarDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;

	return pclclient->m_pOnGameNetEnviromentDlg->OnGameNetTugOfWarDlgProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK COnGameNetEnviromentDlg::OnGameNetTugOfWarDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 i;
	HWND hwnd;

	cltClient* pclclient = (cltClient*)pclClient;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();
		}
		return TRUE;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BUTTON_PREVENTENTER:		// 축구 모드 설정
				{
					cltGameMsgRequest_TugOfWarCommand clinfo(cltGameMsgRequest_TugOfWarCommand::MODE_SOCCER);
					cltMsg	clMsg(GAMEMSG_REQUEST_SETTUGOFWARMODE, sizeof(clinfo), (char*)&clinfo );
					pclclient->SendMsgToServer((char*)&clMsg);
				}
				break;
			case IDC_BUTTON_FORCE_OUT:	// 모두 내보내기
				{	
					cltGameMsgRequest_TugOfWarCommand clinfo(cltGameMsgRequest_TugOfWarCommand::FORCE_OUT);
					cltMsg	clMsg(GAMEMSG_REQUEST_FORCE_OUT, sizeof(clinfo), (char*)&clinfo );
					pclclient->SendMsgToServer((char*)&clMsg);
				}
				break;
			case IDC_BUTTON_TUGOFWAR_MC_IN:	// MC 들여보내기
				{
					cltGameMsgRequest_TugOfWarCommand clinfo(cltGameMsgRequest_TugOfWarCommand::MC_IN);
					cltMsg	clMsg(GAMEMSG_REQUEST_MC_IN, sizeof(clinfo), (char*)&clinfo );
					pclclient->SendMsgToServer((char*)&clMsg);
				}
				break;
			case IDC_BUTTON_TUGOFWAR_PRESENT:	// 선물 주기
				{
					//cltGameMsgRequest_TugOfWarCommand clinfo(cltGameMsgRequest_TugOfWarCommand::GIVE_ITEM);
					//cltMsg	clMsg(GAMEMSG_REQUEST_PRESENT, sizeof(clinfo), (char*)&clinfo );
					//pclclient->SendMsgToServer((char*)&clMsg);
				}
				break;
			case IDC_BUTTON_GOMZITEM:		// 아이템 있는 배낭곰쥐 생성
				{
					cltGameMsgRequest_TugOfWarCommand clinfo(cltGameMsgRequest_TugOfWarCommand::CREATE_GOMZITEM);
					cltMsg	clMsg(GAMEMSG_REQUEST_GOMZITEM, sizeof(clinfo), (char*)&clinfo );
					pclclient->SendMsgToServer((char*)&clMsg);
				}
				break;
			case IDC_BUTTON_GOMZ:			// 아이템 없는 배낭곰쥐 생성
				{
					cltGameMsgRequest_TugOfWarCommand clinfo(cltGameMsgRequest_TugOfWarCommand::CREATE_GOMZ);
					cltMsg	clMsg(GAMEMSG_REQUEST_GOMZ, sizeof(clinfo), (char*)&clinfo );
					pclclient->SendMsgToServer((char*)&clMsg);
				}
				break;
			case IDC_BUTTON_TUG_WATCH:
				{
					PushData( CHAT_DATA_SEND, "/watch" );
				}
				break;
			case IDC_BUTTON_TUG_OP:
				{
					PushData( CHAT_DATA_SEND, "/moveobs 2" );
				}
				break;
			case IDC_BUTTON_TUG_LEFT:
				{
					PushData( CHAT_DATA_SEND, "/move 0 좌왕" );
				}
				break;
			case IDC_BUTTON_TUG_RIGHT:
				{
					PushData( CHAT_DATA_SEND, "/move 0 우왕" );
				}
				break;
			}
		}
		return TRUE;
	}

	return FALSE;
}

void COnGameNetEnviromentDlg::Create( HINSTANCE hInst, HWND hParent )
{
	//m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_ONGAMENETENVIROMENT ), hParent, StaticOnGameNetEnviromentDlgProc );

	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_ONGAMENETBASE ), hParent, StaticOnGameNetMainDlgProc );
	
	Hide();
}

//_LEON_TUGOFWAR
BOOL  COnGameNetEnviromentDlg::CreateTabDlg(HWND hDlg, SI32 page, SI32 dlg, DLGPROC proc)
{
	RECT rect;
	SI32 tabindex = 0;

	tabindex = page;

	if(tabindex >= MAX_ONGAMENETENVIROMENT_PAGE_NUM )
		tabindex = 0;

	if(TabDlg[tabindex])
		ShowWindow(TabDlg[tabindex], SW_SHOW);
	else
	{
		HWND hwnd = GetDlgItem(hDlg, IDC_TAB_ONGAMENET);

		TabDlg[tabindex] = CreateDialog(pclClient->GetInstance(), MAKEINTRESOURCE(dlg), hwnd, proc );

		GetClientRect(hwnd, &rect);
		TabCtrl_AdjustRect(hwnd, FALSE, &rect);

		MoveWindow(TabDlg[tabindex], rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE );

		ShowWindow(TabDlg[tabindex], SW_SHOW);
	}

	return TRUE;

}
	
BOOL CALLBACK COnGameNetEnviromentDlg::StaticOnGameNetEnviromentDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pOnGameNetEnviromentDlg->OnGameNetEnviromentDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK COnGameNetEnviromentDlg::OnGameNetEnviromentDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
		case WM_INITDIALOG:
			{
				InitCommonControls();
				//m_hDlg = hDlg; => Base Dialog로 이동함

				SI32 index = 0;
				while(clItemInfo[index].siItemNum != 0 )
				{
					char buffer[256] = "" ;
					sprintf(buffer,"%s %d",clItemInfo[index].szItemName,clItemInfo[index].siItemNum) ;

					ComboAddString(hDlg,IDC_COMBO_SELECTITEM,(LPARAM)buffer);
					index++;
				}
				
				SI32 i;
				for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
				{
					if(pclClient->pclVillageManager->GetName(i))
					{
						char buffer[256] = "";
						sprintf(buffer, "%s", pclClient->pclVillageManager->GetName(i));
						ComboAddString(hDlg,IDC_COMBO_VILLAGENAME,(LPARAM)buffer);
					}
				}
				
				for(i = 0;i < 60;i++)
				{
					char buffer[256] = "";
					m_arrayMinute[i] = i+1;
					sprintf(buffer, "%d", m_arrayMinute[i]);
					ComboAddString(hDlg,IDC_COMBO_MINUTE,(LPARAM)buffer);
				}

				for(i = 0;i < 100;i++)
				{
					char buffer[256] = "";
					m_arrayLevel[i] = i +1;
					sprintf(buffer, "%d", m_arrayLevel[i]);
					ComboAddString(hDlg,IDC_COMBO_LEVEL,(LPARAM)buffer);
				}

				for(i = 0;i <= 40;i++)
				{
					char buffer[256] = "";
					m_arrayNum[i] = 100 + i*10;
					sprintf(buffer, "%d", m_arrayNum[i]);
					ComboAddString(hDlg,IDC_COMBO_NUM,(LPARAM)buffer);
				}


				m_siSelectIndex = 0 ;
				m_siSelectVillageIndex = 0;
				m_siSelectMinuteIndex = 0;
				m_siSelectLevelIndex = 0;
				m_siSelectNumIndex = 0;
			}
			return TRUE;	

		case WM_COMMAND:
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_BUTTON_SELECT_MC:
					{
						PushData(CHAT_DATA_SEND, "/btmcs mc" );
					}
					break;
				case IDC_BUTTON_SELECTITEM:
					{
						char buffer[256] = "" ;
						sprintf(buffer,"/bi 0 %s %d",clItemInfo[m_siSelectIndex].szItemName,clItemInfo[m_siSelectIndex].siItemNum) ;
						PushData( CHAT_DATA_SEND, buffer );
					}
					break ;
				case IDC_COMBO_SELECTITEM:
					{
						switch(HIWORD(wParam))
						{
						case CBN_SELCHANGE:
							{
								SI32 index = SendDlgItemMessage(hDlg, IDC_COMBO_SELECTITEM, CB_GETCURSEL, 0, 0);

								m_siSelectIndex =index ;
							}
							break;
						}
					}
					break;
				case IDOK:
				case IDCANCEL:
					Hide();
					break;
				case IDC_BUTTON_FOR_OBSERVER:
					//PushData( CHAT_DATA_SEND, "/event 3" );
					PushData( CHAT_DATA_SEND, "/event1char 0" );
					PushData( CHAT_DATA_SEND, "/event2char 0" );
					PushData( CHAT_DATA_SEND, "/event3char 0" );
					
					break;
				case IDC_BUTTON_WATCH:
					PushData( CHAT_DATA_SEND, "/watch" );
					break;
				case IDC_BUTTON_OXQUIZREADY:
					PushData( CHAT_DATA_SEND, "/oxquizready" );
					break;
				case IDC_BUTTON_MOVEMC:
					PushData( CHAT_DATA_SEND, "/movemc" );
					break;
				case IDC_BUTTON_OBSERVERMOVETOFIGHT:
					PushData( CHAT_DATA_SEND, "/moveobs 0" );
					break;
				case IDC_BUTTON_OBSERVERMOVETOPALACE:
					PushData( CHAT_DATA_SEND, "/moveobs 1" );
					break;
				case IDC_BUTTON_STOPOXQUIZ:
					PushData( CHAT_DATA_SEND, "/stopoxquiz" );
					break;
				case IDC_BUTTON_STARTOXQUIZ:
					PushData(CHAT_DATA_SEND, "/startoxquiz" );
					break;
				case IDC_BUTTON_BATTLEZONE:
					PushData(CHAT_DATA_SEND, "/battlezone" );
					break;
				case IDC_BUTTON_FORCE_OUT:
					PushData(CHAT_DATA_SEND, "/btleaderout" );
					break;
				case IDC_BUTTON_BATTLEZONE_MC_IN:
					PushData(CHAT_DATA_SEND, "/btleaderin" );
					break;
				case IDC_BUTTON_BATTLEZONE_PRESENT:
					PushData(CHAT_DATA_SEND, "/bi 0 금괴 5" );
					break;
				case IDC_BUTTON_LEFTUP:
					if(pclclient->bWatchModeSwitch == true)
					{
						pclclient->siWatchModeDetailX--;
					}
					break;
				case IDC_BUTTON_UP:
					if(pclclient->bWatchModeSwitch == true)
					{
						pclclient->siWatchModeDetailX--;
						pclclient->siWatchModeDetailY--;
					}
					break;
				case IDC_BUTTON_RIGHTUP:
					if(pclclient->bWatchModeSwitch == true)
					{
						pclclient->siWatchModeDetailY--;
					}
					break;
				case IDC_BUTTON_LEFT:
					if(pclclient->bWatchModeSwitch == true)
					{
						pclclient->siWatchModeDetailX--;
						pclclient->siWatchModeDetailY++;
					}
					break;
				case IDC_BUTTON_RIGHT:
					if(pclclient->bWatchModeSwitch == true)
					{
						pclclient->siWatchModeDetailX++;
						pclclient->siWatchModeDetailY--;
					}
					break;
				case IDC_BUTTON_LEFTDOWN:
					if(pclclient->bWatchModeSwitch == true)
					{
						pclclient->siWatchModeDetailY++;
					}
					break;
				case IDC_BUTTON_DOWN:
					if(pclclient->bWatchModeSwitch == true)
					{
						pclclient->siWatchModeDetailX++;
						pclclient->siWatchModeDetailY++;
					}
					break;
				case IDC_BUTTON_RIGHTDOWN:
					if(pclclient->bWatchModeSwitch == true)
					{
						pclclient->siWatchModeDetailX++;
					}
					break;
				case IDC_BUTTON_INIT:
					if(pclclient->bWatchModeSwitch == true)
					{
						pclclient->siWatchModeDetailX = 0;
						pclclient->siWatchModeDetailY = 0;
					}
					break;
				case IDC_BUTTON_ATEAM_CHANCE:
					{
						char data = BATTLEZONE_SIDE_A ;
						cltMsg clMsg(GAMEMSG_REQUEST_TEAM_CHANCE, 1, &data);
						pclclient->SendMsgToServer((char*)&clMsg);
					}
					break;
				case IDC_BUTTON_BTEAM_CHANCE:
					{
						char data = BATTLEZONE_SIDE_B ;
						cltMsg clMsg(GAMEMSG_REQUEST_TEAM_CHANCE, 1, &data);
						pclclient->SendMsgToServer((char*)&clMsg);
					}
					break;
				case IDC_BUTTON_CHANCE_MC:
					{
						PushData(CHAT_DATA_SEND, "/btchancemc" );
						if ( pclclient->pclBattleZoneCounter )
							pclclient->pclBattleZoneCounter->SetChanceTeam(0,0);
					}
					break;

				case IDC_COMBO_VILLAGENAME:
					{
						switch(HIWORD(wParam))
						{
						case CBN_SELCHANGE:
							{
								SI32 index = SendDlgItemMessage(hDlg, IDC_COMBO_VILLAGENAME, CB_GETCURSEL, 0, 0);

								m_siSelectVillageIndex =index +1;
							}
							break;
						}
					}
					break;

				case IDC_BUTTON_VILLAGEWAR:
					{
						char buffer[256] = "" ;
						sprintf(buffer,"/kangwar 0 %s", pclClient->pclVillageManager->GetName(m_siSelectVillageIndex)) ;
						PushData( CHAT_DATA_SEND, buffer );
					}
					break;

				case IDC_BUTTON_WORLDWAR:
					{
						PushData( CHAT_DATA_SEND, "/kangwar 1" );
					}
					break;

				case IDC_COMBO_MINUTE:
					{
						switch(HIWORD(wParam))
						{
						case CBN_SELCHANGE:
							{
								SI32 index = SendDlgItemMessage(hDlg, IDC_COMBO_MINUTE, CB_GETCURSEL, 0, 0);

								m_siSelectMinuteIndex =index ;
							}
							break;
						}
					}
					break;

				case IDC_BUTTON_MINUTE_CONTROL:
					{
						char buffer[256] = "" ;
						sprintf(buffer,"/kangtime %d", m_arrayMinute[m_siSelectMinuteIndex]) ;
						PushData( CHAT_DATA_SEND, buffer );
					}
					break;

				case IDC_COMBO_LEVEL:
					{
						switch(HIWORD(wParam))
						{
						case CBN_SELCHANGE:
							{
								SI32 index = SendDlgItemMessage(hDlg, IDC_COMBO_LEVEL, CB_GETCURSEL, 0, 0);

								m_siSelectLevelIndex =index ;
							}
							break;
						}
					}
					break;

				case IDC_BUTTON_LEVEL_CONTROL:
					{
						char buffer[256] = "" ;
						sprintf(buffer,"/kangmonlevel %d", m_arrayLevel[m_siSelectLevelIndex]) ;
						PushData( CHAT_DATA_SEND, buffer );
					}
					break;

				case IDC_COMBO_NUM:
					{
						switch(HIWORD(wParam))
						{
						case CBN_SELCHANGE:
							{
								SI32 index = SendDlgItemMessage(hDlg, IDC_COMBO_NUM, CB_GETCURSEL, 0, 0);

								m_siSelectNumIndex =index ;
							}
							break;
						}
					}
					break;

				case IDC_BUTTON_NUM_CONTROL:
					{
						char buffer[256] = "" ;
						sprintf(buffer,"/kangmonnum %d", m_arrayNum[m_siSelectNumIndex]) ;
						PushData( CHAT_DATA_SEND, buffer );
					}
					break;

				case IDC_BUTTON_KANGWAREND:
					{
						PushData( CHAT_DATA_SEND, "/kangwarend");
					}
//_LEON_TUGOFWAR
//#if defined(_DEBUG)
				//case IDC_BUTTON_CHANCE_MC_TUGOFWAR:
				//	{
				//		PushData(CHAT_DATA_SEND, "/btchancemctugofwar" );
				//		if ( pclclient->pclTugOfWarCounter )
				//			pclclient->pclTugOfWarCounter->SetChanceTeam(0,0);
				//	}
				//	break;
//#endif
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

bool COnGameNetEnviromentDlg::IsShow()
{
	return m_bShow;
}

void COnGameNetEnviromentDlg::Show()
{
	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );
	
	return;
}

void COnGameNetEnviromentDlg::Hide()
{
	ShowWindow( m_hDlg, SW_HIDE );
	m_bShow = false;

	return;
}

void COnGameNetEnviromentDlg::PushData( SI16 Kind, char *Text )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		if ( pclclient->m_pDialog[ NCHAT_DLG ] )
		{
			((CNChatDlg *)pclclient->m_pDialog[ NCHAT_DLG ])->Push( Kind, Text );
		}
		else
		{
			if ( pclclient->g_ChatStatusBar )
			{
				pclclient->g_ChatStatusBar->m_pChatStatusBarBuffer->Push( Kind, Text );
			}
		}
	}
	else
	{
		if ( pclclient->g_ChatStatusBar )
		{
			pclclient->g_ChatStatusBar->m_pChatStatusBarBuffer->Push( Kind, Text );
		}
	}

	return;
}