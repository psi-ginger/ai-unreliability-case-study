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

#define EXPLAIN_BREED_TYPE		7
char* HorseBreed[]=
{
		"말은 무엇입니까?",				"horsewhat.bmp",
		"말을 키우면 무엇이 좋습니까?",	"horsewhatgood.bmp",	
		"말이 수명을 다하면..?",		"horseage.bmp",	
		"말은 어떻게 얻습니까?",		"horsehow.bmp",	
		"말 풀어주기?",					"horsefree.bmp",
		"말을 잘 키우는 방법은?",		"horsehowbreed.bmp",
		"말에게 아이템 옮기기",			"horseitem.bmp",
};

cltHorseBreedExplain::cltHorseBreedExplain()
{
	m_hDlg	= NULL;
	m_DlgRectInitSwitch = FALSE;
	
	m_hHitBitmap = NULL;
}

cltHorseBreedExplain::~cltHorseBreedExplain()
{
	m_hDlg	= NULL;
	m_DlgRectInitSwitch = FALSE;

	m_hHitBitmap = NULL;
}


bool cltHorseBreedExplain::CreateDlg(HINSTANCE hinst, HWND hwnd)
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{
		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_STATUS_HORSE_EXPLAIN), hwnd, HorseBreedExplainStaticProc);
		
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
BOOL CALLBACK cltHorseBreedExplain::HorseBreedExplainStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;
	return pclclient->pclCharStatus->pclHorseBreedExplain->HorseBreedExplainProc(hDlg, iMsg, wParam, lParam);
}


BOOL CALLBACK cltHorseBreedExplain::HorseBreedExplainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 i;
	
	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			// 타이머를 설정한다. 
			SetTimer(hDlg, 1, 1000, NULL);
			
			for(i = 0; i < EXPLAIN_BREED_TYPE; ++i)
			{
				ComboAddString(hDlg, IDC_COMBO_BREED_TYPE, (LPARAM)HorseBreed[i*2]);
			}
			SendDlgItemMessage( hDlg, IDC_COMBO_BREED_TYPE, CB_SETCURSEL, 0, 0 );

			m_siBreedType= 0;
			ShowBreedHit(hDlg, m_siBreedType);
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
			case IDC_COMBO_BREED_TYPE:
				{
					switch(HIWORD(wParam))
					{
					case CBN_SELCHANGE:
						{
							char szWeapon[128] = "NULL";
							ComboGetString(hDlg, IDC_COMBO_BREED_TYPE, (LPARAM)szWeapon);
							
							m_siBreedType = GetBreedType(szWeapon);
							
							ShowBreedHit(hDlg, m_siBreedType);
						}
						break;
					}
					
				}
				return TRUE;

			case IDCANCEL:
				DestroyWindow(m_hDlg);
				m_hDlg = NULL;
				return TRUE;
		}
		return TRUE;
	}
		
	return FALSE;
}


SI32 cltHorseBreedExplain::GetBreedType(char* pBreedType)
{
	SI32 i;
	
	for(i = 0; i < EXPLAIN_BREED_TYPE; ++i)
	{
		if(strcmp(HorseBreed[i*2], pBreedType) == 0)
		{
			return i;
		}
	}
	
	return 0;
}


void cltHorseBreedExplain::ShowBreedHit(HWND hDlg, SI32 siBreedType)
{
	char filename[MAX_PATH];
	sprintf(filename, "StatusHint\\%s", HorseBreed[siBreedType*2+1]);

	if(m_hHitBitmap != NULL)
	{
		DeleteObject(m_hHitBitmap);
		m_hHitBitmap = NULL;
	}

	m_hHitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
		
	if ( m_hHitBitmap != NULL )
	{
		SendDlgItemMessage( hDlg, IDC_STATIC_STATUS_HORSE_EXPLAIN, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHitBitmap);
	}		
		
}



*/