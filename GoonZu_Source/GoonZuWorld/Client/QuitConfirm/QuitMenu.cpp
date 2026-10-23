//---------------------------------
// 2004/10/05 성준엽
//---------------------------------
#include "../Client.h"
#include "QuitConfirm.h"

#include "../../Resource.h"

#include "Char\CharManager\CharManager.h"
#include "../common/ItemMallManager/ItemMallManager.h"
#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

extern cltCommonLogic* pclClient;

cltQuitMenu::cltQuitMenu()
{
	m_hDlg = NULL;
	DlgRectInitSwitch = FALSE ;
}

cltQuitMenu::~cltQuitMenu()
{
}

BOOL cltQuitMenu::CreateDlg(HINSTANCE hinst, HWND hwnd)
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{
		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_QUITMENU), hwnd, StaticQuitMenuDlgProc);

		if(DlgRectInitSwitch == TRUE)
		{
			MoveWindow(m_hDlg, 
				DlgRect.left, DlgRect.top, 
				DlgRect.right - DlgRect.left, 
				DlgRect.bottom - DlgRect.top, FALSE);
		}

		ShowWindow(m_hDlg, SW_SHOW);

		return TRUE;
	}
	else
	{
		DestroyWindow(m_hDlg);
	}

	return 0;
}

BOOL CALLBACK cltQuitMenu::StaticQuitMenuDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;

	return pclclient->pclQuitMenu->QuitMenuDlgProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltQuitMenu::QuitMenuDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			BOOL bInServer = TRUE ;

			cltCharClient* pclchar = (cltCharClient*)pclclient->pclCM->CR[1];
			if ( pclchar->pclKI == NULL )
			{
				bInServer = FALSE ;
			}
			else
			{
				bInServer = TRUE ;
			}

			// 운영자인 경우, 
/*			if(pclchar->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL)
			{
				EnableWindow(GetDlgItem( hDlg,	IDC_BUTTON_CHAR),		TRUE);
			}
			*/
		}
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_ITEAM_MALL:
			{
				/*
				if(g_SoundOnOffDlg.m_bFullScreenMode)
				{
					ShowWindow( pclclient->GetHwnd(), SW_HIDE );
					pclClient->pclItemMallManager->CreateItemMallDlg( pclclient->GetHwnd(), pclclient->GetInstance(), 0 );
					
					char url[ MAX_PATH ];			
					strcpy(url,"http://goonzu.ndoors.com/center/mall/mall_center.asp?menu=mall");
					ShellExecute( NULL, NULL, "IEXPLORE.EXE", url, NULL, SW_SHOWNORMAL );
					
				}
				else 
				*/
				{
					if( pclClient->bItemMallUseSwitch == FALSE)
					{	
						char* pText = GetTxtFromMgr(962);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText("ItemMall",pText, pclclient->clSelectedWorldInfo.baseinfo.worldname);
					}
					else
					{
						if( g_bFullScreen )
						{
							ShowWindow( pclclient->GetHwnd(), SW_MINIMIZE );
						}

						pclClient->pclItemMallManager->CreateItemMallDlg( pclclient->GetHwnd(), pclclient->GetInstance(), 0 );
					}
				}
				DestroyWindow(m_hDlg);
			}
			return TRUE;
		case IDC_BUTTON_SERVER:
			{
			}
			return TRUE;
		case IDC_BUTTON_CHAR:
			{
				pclclient->bReceiveMyCharAllInfoSwitch	= false;
				pclclient->bBackUpSwitch				= false;

				pclClient->SetGameStatus(GS_INITCHARLIST);
				DestroyWindow(m_hDlg);
			}
			return TRUE;
		case IDC_BUTTON_GAME:
			{
				DestroyWindow(m_hDlg);
			}
			return TRUE;
		case IDC_BUTTON_QUIT:
			{
				if ( pclclient->pclQuitConfirm )
				{
					pclclient->pclQuitConfirm->CreateDlg(pclClient->GetInstance(), pclClient->GetHwnd());
				}
			}
			return TRUE;
		}
		return TRUE;

	case WM_CLOSE:
		{
			DestroyWindow(m_hDlg);
		}
		return TRUE;

	case WM_DESTROY:
		{
			// 윈도우의 위치를 기억한다. 
			GetWindowRect(hDlg, &DlgRect);
			DlgRectInitSwitch = TRUE;

			m_hDlg = NULL;
		}
		return TRUE;
	}

	return FALSE;
}
