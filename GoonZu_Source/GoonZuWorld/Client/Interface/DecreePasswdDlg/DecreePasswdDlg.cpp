// DecreePasswdDlg.cpp: implementation of the CDecreePasswdDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "../../Client/client.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"

#include "../../../lib/WebHTML/WebHTML.h"

#include "DecreePasswdDlg.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDecreePasswdDlg::CDecreePasswdDlg()
{
	m_hInst = NULL ;
	m_hWnd = NULL ;

	m_hDlg = NULL ;
	
	strcpy(m_szAccountID,"");

	m_siXsize = 0 ;
	m_siYsize = 0 ;
}

CDecreePasswdDlg::~CDecreePasswdDlg()
{
}

void CDecreePasswdDlg::Create(HINSTANCE hInst, HWND hParent,SI32 xsize,SI32 ysize  )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;

	m_siXsize = xsize ;
	m_siYsize = ysize ;
}

BOOL CALLBACK CDecreePasswdDlg::StatiCDecreePasswdProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pDecreePassWdDlg->DecreePasswdProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CDecreePasswdDlg::DecreePasswdProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			char buffer[1024] = "" ;
			HWND hwnd = GetDlgItem(hDlg,IDC_STATIC_DECREE);

			if ( hwnd )
			{
/*				sprintf(buffer,"최근 계정 도용 사건이 빈번히 일어나고 있습니다.\r\n"
							   "귀하가 사용하고 계신 비밀번호는 보안 수준을 높일 필요가 있습니다.\r\n"
							   "홈페이지에 로그인 하셔서 사용하고 계시는 비밀번호를 "
							   "'영문+숫자 4글자 이상'으로 변경 하시기를 권고합니다.\r\n");

				SetWindowText(hwnd,buffer);*/

				SI32 x = 320 ;
				SI32 y = 320 ;
				MoveWindow(hwnd,0,0,x,y,TRUE);
				EmbedBrowserObject(hwnd);

				SI32 buttonx = ( x - 190 ) / 2 ;
				SI32 buttony = ( y + 5 ) ;

				MoveWindow(GetDlgItem(hDlg,IDOK),buttonx,buttony,175,20,TRUE);
				MoveWindow(GetDlgItem(hDlg,IDCANCEL),buttonx,buttony+25,175,20,TRUE);

				MoveWindow(hDlg,0,0,x-15,y+80,TRUE);

				char buf[256] = "" ;
				char path[512] = "" ;
				GetCurrentDirectory( 256, path );
				int BufLen = strlen( path );
				if( path [ BufLen - 1 ] == '\\' )
				{
					strcpy(buf, "Interface\\EventNotice\\decree.htm");
					strcat(path,buf);
				}
				else
				{				
					strcpy(buf, "\\Interface\\EventNotice\\decree.htm");
					strcat(path,buf);
				}

				DisplayHTMLPage(hwnd,path);
			}
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				char url[ MAX_PATH ];
				pclclient->GetCPasswdUrl( url, m_szAccountID );

				HWND hwnd = GetDlgItem(hDlg,IDC_STATIC_DECREE);

				if ( hwnd )
				{
					UnEmbedBrowserObject(hwnd);

					MoveWindow(hwnd,0,0,m_siXsize,m_siYsize,TRUE);
					EmbedBrowserObject(hwnd);

					MoveWindow(hDlg,0,0,m_siXsize+5,m_siYsize,TRUE);

					DisplayHTMLPage(hwnd,url);
				}

				hwnd = GetDlgItem(hDlg,IDOK);
				ShowWindow(hwnd,SW_HIDE);

				hwnd = GetDlgItem(hDlg,IDCANCEL);
				ShowWindow(hwnd,SW_HIDE);

				//ShellExecute( NULL, NULL, "IEXPLORE.EXE", url, NULL, SW_SHOWNORMAL );
			}
			break;
		case IDCANCEL:
			{
				DestroyWindow(m_hDlg);
			}
			break;
		}
		break;
	case WM_CLOSE:
		{
			DestroyWindow(m_hDlg);
		}
		break;
	case WM_DESTROY:
		{
			HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_DECREE );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}

			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

BOOL CDecreePasswdDlg::Set(char * accountID , char * passwd)
{
	if ( CorrectPasswdRule(accountID,passwd) == true ) return FALSE ;

	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_DECREE_PASSWD ), m_hWnd, StatiCDecreePasswdProc );

		ShowWindow( m_hDlg, SW_SHOW );
		return TRUE ;
	}

	return FALSE ;
}

bool CDecreePasswdDlg::CorrectPasswdRule(char * accountID , char * passwd)
{
	char accountid[FILE_NAME_SIZE] = "" ;
	char password[FILE_NAME_SIZE] = "" ;

	strcpy(accountid,accountID);
	strcpy(m_szAccountID,accountid);
	strlwr(accountid);

	strcpy(password,passwd);
	strlwr(password);

	if ( !stricmp(accountid,password) ) return false ;
	if ( strstr(accountid,password) ) return false ;
	if ( strlen(password) < 4 ) return false ;

	bool bNumber = false ;
	bool bString = false ;
	for ( SI32 i = 0 ; i < FILE_NAME_SIZE ; i++ )
	{
		if ( password[i] == '\0' || password[i] == NULL )
			continue ;

		if ( password[i] >= '0' && password[i] <= '9' && !bNumber )
			bNumber = true ;

		if ( password[i] >= 'a' && password[i] <= 'z' && !bString )
			bString = true ;

		if ( bNumber && bString )
			return true ;
	}

	return false ;
}