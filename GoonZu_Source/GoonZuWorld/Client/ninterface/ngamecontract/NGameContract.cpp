//---------------------------------
// 2004/1/28 김태곤
//---------------------------------


#ifdef __SUN_GAMECONTRACT__


#include "GameContract.h"
#include "..\..\Resource.h"
#include "..\Client.h"

#include "../Client/Interface/LoginClient/LoginClient.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../Client/NInterface/NLogin/NLogin.h"
#include "../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

cltGameContract::cltGameContract()
{
	m_hDlg				= NULL;
	bDlgRectInitSwitch	= false;

}

cltGameContract::~cltGameContract()
{
	
}


BOOL cltGameContract::CreateDlg(HINSTANCE hinst, HWND hwnd)
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{

		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_GAMECONTRACT), hwnd, StaticDlgProc);

		if(bDlgRectInitSwitch == true)
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

BOOL CALLBACK cltGameContract::StaticDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

	cltClient* pclclient = (cltClient*)pclClient;
	return pclclient->pclGameContract->DlgProc(hDlg, iMsg, wParam, lParam);
}


BOOL CALLBACK cltGameContract::DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			SetTimer(hDlg, 1, 1000, NULL);

			//SendMessage( GetDlgItem( hDlg, IDC_EDIT_REGISTRATION_NUMBER ), EM_LIMITTEXT, 7, 0 );

			/*
			if ( g_LoginClient.m_siCurrentSelectedSiteIndex == 2 )
			{
				ShowWindow( GetDlgItem( hDlg, IDC_STATIC_REGISTRATION_NUMBER ), SW_SHOW );
				ShowWindow( GetDlgItem( hDlg, IDC_EDIT_REGISTRATION_NUMBER ), SW_SHOW );
			}
			else
			{
				ShowWindow( GetDlgItem( hDlg, IDC_STATIC_REGISTRATION_NUMBER ), SW_HIDE );
				ShowWindow( GetDlgItem( hDlg, IDC_EDIT_REGISTRATION_NUMBER ), SW_HIDE );
			}
			*/

//			FILE* fp;
//			TCHAR buffer[1024];
//			TCHAR* ptext = NULL;

//			ptext = new TCHAR[1024 * 100];

//			strcpy(ptext , TEXT(""));
			
			TCHAR buffer[256] = TEXT("") ;
			TCHAR GameContractFilePath[ 512 ] = TEXT("");
			GetCurrentDirectory( 256, GameContractFilePath );
			
            StringCchPrintf( buffer, 256, TEXT("\\GameContract\\%dGameContract.htm"), ((CNLoginDlg*)pclclient->m_pDialog[ NLOGIN_DLG ])->m_siCurrentSelectedSiteIndex );
			
//			sprintf( buffer, TEXT("\\GameContract\\%dGameContract.htm"), g_LoginClient.m_siCurrentSelectedSiteIndex );
			StringCchCat(GameContractFilePath, 512, buffer);

			HWND hWnd = GetDlgItem(hDlg,IDC_STATIC_GAMECONTRACT_HTML);
			if ( hWnd )
			{
				EmbedBrowserObject(hWnd);
				DisplayHTMLPage( hWnd, GameContractFilePath );
			}

/*			fp = _tfopen( GameContractFilePath, TEXT("rt") );
			if(fp)
			{
				while(_fgetts(buffer, 1024, fp) != NULL)
				{
					for(SI32 i = 0; i < lstrlen(buffer);i++)
					{
						if(buffer[i] == '\n')
						{
							buffer[i] = ' ';
						}
					}

					strcat(ptext, buffer);
					strcat(ptext, TEXT("\r\n"));
				}

				//------------------------------
				// 에디트 박스에 추가한다. 
				//------------------------------
				SetDlgItemText(hDlg, IDC_EDIT_GAMECONTRACT, ptext);
				
				if(ptext)
				{
					delete[] ptext;
					ptext = NULL;
				}
				fclose(fp);
			}*/

		}
		return TRUE;

	case WM_NOTIFY:
		return TRUE;

	case WM_TIMER:
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				//---------------------------------
				// 프론트 서버에 웹DB로의 이전을  요청한다.
				//----------------------------------
				cltClient* pclclient = (cltClient*)pclClient;
				pclclient->RequestJoinGame();

				

				DestroyWindow(m_hDlg);

			}
			return TRUE;
			
		case IDCANCEL:
			DestroyWindow(m_hDlg);
			return TRUE;
		}
		return TRUE;
		
	case WM_DESTROY:

		// 타이머를 해제한다. 
		KillTimer(hDlg, 1);

		HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_GAMECONTRACT_HTML );
		if( hwnd )
		{
			DoPageAction(hwnd, WEBPAGE_STOP);
			UnEmbedBrowserObject(hwnd);
		}

		// 윈도우의 위치를 기억한다. 
		GetWindowRect(hDlg, &DlgRect);
		bDlgRectInitSwitch = true;

		m_hDlg = NULL;
		return TRUE;

	}

	return FALSE;
}


#endif