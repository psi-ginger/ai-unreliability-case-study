/*
//---------------------------------
// 2004/3/22 이제형
//---------------------------------

#include "..\Client\Client.h"
#include "CharStatus.h"
#include "..\resource.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "Msg\MsgType-Horse.h"

extern cltCommonLogic* pclClient;

#define EXPLAIN_WEAPON_TYPE		6
char* StatusWeapon[]=
{
		"검",		"sword.bmp",
		"창",		"spear.bmp",	
		"활",		"bow.bmp",
		"도끼",		"axe.bmp",
		"총",		"gun.bmp",
		"지팡이",	"staff.bmp",	
};

cltCharStatusExplain::cltCharStatusExplain()
{
	m_hDlg	= NULL;
	m_DlgRectInitSwitch = FALSE;
	
	m_hHitBitmap = NULL;
}

cltCharStatusExplain::~cltCharStatusExplain()
{
	m_hDlg	= NULL;
	m_DlgRectInitSwitch = FALSE;

	m_hHitBitmap = NULL;
}


bool cltCharStatusExplain::CreateDlg(HINSTANCE hinst, HWND hwnd)
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{
		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_STATUS_EXPLAIN), hwnd, CharStatusExplainStaticProc);
		
		if(m_DlgRectInitSwitch == TRUE)
		{
			MoveWindow(m_hDlg, 
				m_DlgRect.left,	 m_DlgRect.top, 
				m_DlgRect.right - m_DlgRect.left, 
				m_DlgRect.bottom - m_DlgRect.top, FALSE);
		}
		
		ShowWindow(m_hDlg, SW_SHOW);
		
		return TRUE;
	}
	else
	{
		DestroyWindow(m_hDlg);
	}
	
	return FALSE;	
}
BOOL CALLBACK cltCharStatusExplain::CharStatusExplainStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;
	return pclclient->pclCharStatus->pclCharStatusExplain->CharStatusExplainProc(hDlg, iMsg, wParam, lParam);
}


BOOL CALLBACK cltCharStatusExplain::CharStatusExplainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 i;
	
	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			// 타이머를 설정한다. 
			SetTimer(hDlg, 1, 1000, NULL);
			
			for(i = 0; i < EXPLAIN_WEAPON_TYPE; ++i)
			{
				ComboAddString(hDlg, IDC_COMBO_WEAPON_TYPE, (LPARAM)StatusWeapon[i*2]);
			}
			SendDlgItemMessage( hDlg, IDC_COMBO_WEAPON_TYPE, CB_SETCURSEL, 0, 0 );

			m_siWeaponType= 0;
			ShowWeaponHit(hDlg, m_siWeaponType);
		}
		return TRUE;
		
		
	case WM_TIMER:
		return TRUE;
		
	case WM_DESTROY:
		// 초상화용 핸들을 삭제한다.
		if(m_hHitBitmap)
		{
			DeleteObject(m_hHitBitmap);
			m_hHitBitmap = NULL;
		}
		// 타이머를 해제한다. 
		KillTimer(hDlg, 1);
		
		// 윈도우의 위치를 기억한다. 
		GetWindowRect(m_hDlg, &m_DlgRect);
		m_DlgRectInitSwitch = TRUE;
		m_hDlg = NULL;
		return TRUE;
		
	case WM_NOTIFY:
		return TRUE;
		
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDC_COMBO_WEAPON_TYPE:
				{
					switch(HIWORD(wParam))
					{
					case CBN_SELCHANGE:
						{
							char szWeapon[128] = "NULL";
							ComboGetString(hDlg, IDC_COMBO_WEAPON_TYPE, (LPARAM)szWeapon);
							
							m_siWeaponType = GetWeaponType(szWeapon);
							
							ShowWeaponHit(hDlg, m_siWeaponType);
						}
						break;
					}
					
				}
				return TRUE;

			case IDCANCEL:
				DestroyWindow(m_hDlg);
				return TRUE;
		}
		return TRUE;
	}
		
	return FALSE;
}


SI32 cltCharStatusExplain::GetWeaponType(char* pWeaponType)
{
	SI32 i;
	
	for(i = 0; i < EXPLAIN_WEAPON_TYPE; ++i)
	{
		if(strcmp(StatusWeapon[i*2], pWeaponType) == 0)
		{
			return i;
		}
	}
	
	return 0;
}


void cltCharStatusExplain::ShowWeaponHit(HWND hDlg, SI32 siWeaponType)
{
	char filename[MAX_PATH];
	sprintf(filename, "StatusHint\\%s", StatusWeapon[siWeaponType*2+1]);

	if(m_hHitBitmap != NULL)
	{
		DeleteObject(m_hHitBitmap);
		m_hHitBitmap = NULL;
	}

	m_hHitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
		
	if ( m_hHitBitmap != NULL )
	{
		SendDlgItemMessage( hDlg, IDC_STATIC_STATUS_EXPLAIN, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHitBitmap);
	}		
		
}*/



