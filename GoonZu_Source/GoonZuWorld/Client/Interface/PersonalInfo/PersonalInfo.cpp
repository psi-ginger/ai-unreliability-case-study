#include "../../Common/CommonHeader.h"
#include "..\..\Client\client.h"


#include "PersonalInfo.h"
#include "../../../lib/WebHTML/WebHTML.h"

#include "../../Lib/JYLibFunction.h"

CPersoanlInfoDlg		g_PersoanlInfoMsg;
extern CJYLibFunction g_JYLibFunction;
extern cltCommonLogic* pclClient;

CPersoanlInfoDlg::CPersoanlInfoDlg()
{
	m_hDlgBrush				= NULL;
	m_hPersoanlInfoMsgDlg	= NULL ;
	memset(m_szAccountID,	0, sizeof(m_szAccountID));
	
}

CPersoanlInfoDlg::~CPersoanlInfoDlg()
{
	if( m_hPersoanlInfoMsgDlg )
	{
		DestroyWindow( m_hPersoanlInfoMsgDlg );
	}
}

void CPersoanlInfoDlg::Create( HINSTANCE hInst, HWND hParent )
{
	m_hPersoanlInfoMsgDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_PERSONAL ), hParent, StatiCPersoanlInfoDlgProc );
	
	Hide();
}

BOOL CALLBACK CPersoanlInfoDlg::StatiCPersoanlInfoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return g_PersoanlInfoMsg.PersoanlInfoMsgDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CPersoanlInfoDlg::PersoanlInfoMsgDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlgBrush = CreateSolidBrush( COLOR_DIALOG_YELLOW );

			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================			
			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_GOMZI_70);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
			hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
			hwnd = GetDlgItem(hDlg, IDC_STATIC_INTRO);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}

			MoveWindow( GetDlgItem( hDlg, IDC_STATIC_PROFILE ), 7, 170, 73, 73, TRUE );
		
			//==================================
		}
		break;
	case WM_SIZE:
		{
			//============================
			// HTML Resize 를 한다.
			//============================
			//==================================
			HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_GOMZI_70);
			if( hwnd )
			{
				ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			}
			hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE);
			if( hwnd )
			{
				ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			}
			hwnd = GetDlgItem(hDlg, IDC_STATIC_INTRO);
			if( hwnd )
			{
				ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			}
						
			return FALSE;
		}
		break;

	case WM_CTLCOLORDLG:
		{
			HDC hdc = (HDC)wParam;			
			return (BOOL)m_hDlgBrush;
		}
		break;
	case WM_COMMAND:
		{
			switch( wParam )
			{
			case IDCANCEL:
				{
					Hide();
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
			if ( m_hDlgBrush )
			{
				DeleteObject( m_hDlgBrush );
				m_hDlgBrush = NULL;
			}

			m_hPersoanlInfoMsgDlg = NULL;
			//---------------------------------
			// HTML 을 해제한다.
			//---------------------------------
			//==================================
			HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_GOMZI_70 );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}			
			hwnd = GetDlgItem( hDlg, IDC_STATIC_PROFILE );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}			
			hwnd = GetDlgItem( hDlg, IDC_STATIC_INTRO );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}			
		}
		break;
	}


	return FALSE;
}

void CPersoanlInfoDlg::set(char * szPersonalName,char * szFatherName , SI32 ChangeNum ,
						   SI16 Level,SI16 FameLevel ,/*char * szSchoolName,*/ char * szAccountID,
						   SI32 wealthrank,cltMoney money, char* guildname, char* guildranktype,
						   SI32 battlewin,SI32 totalkill,SI32 timekill,SI32 winrank,SI32 killrank)
{
	char strTitle[256] = "";
	HWND hWnd;
	char strBuf[256] = "";
	char buf[256] = "";

	memset( strTitle, 0, 32 );
	char* pText = GetTxtFromMgr(3881);
	sprintf(strTitle,pText,szPersonalName);
	SetWindowText( m_hPersoanlInfoMsgDlg, strTitle );

	memset(strBuf,0,256);
	sprintf(strBuf,"%s ( Lv. %d )",szPersonalName,Level);
	hWnd = GetDlgItem( m_hPersoanlInfoMsgDlg, IDC_EDIT_PERSONALNAME );
	SetWindowText( hWnd, strBuf );

	memset(strBuf,0,256);
	pText = GetTxtFromMgr(3880);
	wsprintf(strBuf,pText,szFatherName,ChangeNum);
	hWnd = GetDlgItem( m_hPersoanlInfoMsgDlg, IDC_EDIT_PERSONALTEACHER );
	SetWindowText( hWnd, strBuf );

	memset(strBuf,0,256);
	wsprintf(strBuf,"%d",FameLevel);
	hWnd = GetDlgItem( m_hPersoanlInfoMsgDlg, IDC_STATIC_PERSONALFAMELEVEL );
	SetWindowText( hWnd, strBuf );

//	memset(strBuf,0,256);
//	strcpy(strBuf,szSchoolName);
//	hWnd = GetDlgItem( m_hPersoanlInfoMsgDlg, IDC_STATIC_PERSONALSCHOOL );
//	SetWindowText( hWnd, strBuf );

	memset(strBuf,0,256);
	if ( wealthrank < 0 ) wealthrank = 0 ;
	if ( money.itMoney < 0 ) money.itMoney = 0 ;
	g_JYLibFunction.SetNumUnit(money.itMoney,buf,"냥");
	wsprintf(strBuf,"%d위(%s)",wealthrank,buf);
	hWnd = GetDlgItem( m_hPersoanlInfoMsgDlg,IDC_STATIC_PERSONALRANK);
	SetWindowText(hWnd,strBuf);

	memset(strBuf,0,256);
	if( guildname == NULL || strcmp(guildname, "" ) == 0 )
		pText = GetTxtFromMgr(1287);
	else
		pText = guildname;
	wsprintf(strBuf,"%s", pText);
	hWnd = GetDlgItem( m_hPersoanlInfoMsgDlg, IDC_STATIC_GUILDNAME );
	SetWindowText( hWnd, strBuf );

	memset(strBuf,0,256);
	if( guildranktype == NULL || strcmp(guildranktype, "" ) == 0 )
		pText = GetTxtFromMgr(1287);
	else
		pText = guildranktype;
	wsprintf(strBuf,"%s", pText);
	hWnd = GetDlgItem( m_hPersoanlInfoMsgDlg, IDC_STATIC_RANKNAME);
	SetWindowText( hWnd, strBuf );

	// ACCOUNT 를 설정한다.
	if(strlen(m_szAccountID)>0 && strcmp(m_szAccountID, szAccountID) == 0)
	{
/*		// 같은 ID를 다시 보여주게되면 혹시나모르니 Refresh 한다.
		hWnd = GetDlgItem(m_hPersoanlInfoMsgDlg, IDC_STATIC_GOMZI_70);
		if(hWnd)	DoPageAction(hWnd, WEBPAGE_REFRESH);
		
		hWnd = GetDlgItem(m_hPersoanlInfoMsgDlg, IDC_STATIC_PROFILE);
		if(hWnd)	DoPageAction(hWnd, WEBPAGE_REFRESH);*/

		hWnd = GetDlgItem(m_hPersoanlInfoMsgDlg, IDC_STATIC_INTRO);
		if(hWnd)	DoPageAction(hWnd, WEBPAGE_REFRESH);

	}
	else if(strlen(szAccountID)>=2)
	{
		// ID를 복사한다.
		strcpy(m_szAccountID, szAccountID);

		// GOMZI 70%
		char GomziURL[MAX_PATH];

		if( szAccountID[ 1 ] == '_' && ( szAccountID[ 0 ] == 's' || szAccountID[ 0 ] == 'S' ) ) {
			// sbs

			sprintf(GomziURL,"http://goonzucz.sbs.co.kr/mh/image/darak/s_mydk/%c/%c/%s/%s_70.html", szAccountID[2], szAccountID[3], &szAccountID[2], &szAccountID[2] );

		} else {
			// goonzu

			sprintf(GomziURL,"http://ccomzi.ndoors.com/mh/image/darak/mydk/%c/%c/%s/%s_70.html", szAccountID[0], szAccountID[1], szAccountID, szAccountID);
		}
		
/*		hWnd = GetDlgItem(m_hPersoanlInfoMsgDlg, IDC_STATIC_GOMZI_70);
		if(hWnd)	DisplayHTMLPage(hWnd, GomziURL);

		// PROFILE
		pclClient->GetCZProfileUrl( GomziURL, szAccountID );
		
		hWnd = GetDlgItem(m_hPersoanlInfoMsgDlg, IDC_STATIC_PROFILE);
		if(hWnd)	DisplayHTMLPage(hWnd, GomziURL);*/

		// INTRO
		pclClient->GetCZIntroUrl( GomziURL, szAccountID );
		
		hWnd = GetDlgItem(m_hPersoanlInfoMsgDlg, IDC_STATIC_INTRO);
		if(hWnd)	DisplayHTMLPage(hWnd, GomziURL);
	}

	memset(strBuf,0,256);
	pText = GetTxtFromMgr(5498);
	wsprintf(strBuf,pText,battlewin,winrank);
	hWnd = GetDlgItem( m_hPersoanlInfoMsgDlg, IDC_STATIC_BATTLEZONEWIN );
	SetWindowText( hWnd, strBuf );

	memset(strBuf,0,256);
	pText = GetTxtFromMgr(5499);
	wsprintf(strBuf,pText,totalkill,killrank);
	hWnd = GetDlgItem( m_hPersoanlInfoMsgDlg, IDC_STATIC_BATTLEZONETOTALKILL );
	SetWindowText( hWnd, strBuf );

	memset(strBuf,0,256);
	wsprintf(strBuf,"%d",timekill);
	hWnd = GetDlgItem( m_hPersoanlInfoMsgDlg, IDC_STATIC_BATTLEZONETIMEKILL );
	SetWindowText( hWnd, strBuf );

	
	Show();
	return;
}

void CPersoanlInfoDlg::Show()
{
	ShowWindow( m_hPersoanlInfoMsgDlg, SW_SHOW );
}

void CPersoanlInfoDlg::Hide()
{
	ShowWindow( m_hPersoanlInfoMsgDlg, SW_HIDE );
}