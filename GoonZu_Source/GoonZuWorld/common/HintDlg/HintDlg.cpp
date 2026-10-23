/*
//---------------------------------
// 2004/9/18 성준엽
//---------------------------------

#include "CommonLogic.h"
#include "../../Client/Client.h"
#include "../../Resource.h"

#include "HintDlg.h"

#include "../../lib/WebHTML/WebHTML.h"

extern cltCommonLogic* pclClient;

cltHintDlgExplain::cltHintDlgExplain()
{
	m_hDlg	= NULL;

	m_DlgRectInitSwitch = FALSE;
	
	m_hHitBitmap = NULL;

	m_hInst = NULL ;
	m_hWnd = NULL ;
}

cltHintDlgExplain::~cltHintDlgExplain()
{
}

void cltHintDlgExplain::Create(HINSTANCE hinst, HWND hwnd)
{
	m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_HINT), hwnd, HintDlgExplainStaticProc);

	ReadListFile();

	ShowWindow(m_hDlg, SW_HIDE);
}

void cltHintDlgExplain::Show(TCHAR * szFileName)
{
	if(!IsWindow(m_hDlg))
	{
		if(m_DlgRectInitSwitch == TRUE)
		{
			MoveWindow(m_hDlg, 
				m_DlgRect.left,	 m_DlgRect.top, 
				m_DlgRect.right - m_DlgRect.left, 
				m_DlgRect.bottom - m_DlgRect.top, FALSE);
		}
	}

	TCHAR buffer[256] = TEXT("") ;
	_tcscpy(buffer,szFileName);

	ShowList(buffer);
	Set(buffer);

	ShowWindow(m_hDlg, SW_SHOW);
}

BOOL CALLBACK cltHintDlgExplain::HintDlgExplainStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;
	return pclclient->m_pHintDlg->HintDlgExplainProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltHintDlgExplain::HintDlgExplainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			for ( SI32 i = 0 ; i < MAX_HINTCOMBO_TYPE ; i ++ )
			{
				_tcscpy(m_szComboType[i], TEXT(""));
				_tcscpy(m_szBmpName[i], TEXT(""));
			}
		}
		break;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_LIST_HINT_SUBJECT:
				{
					switch(HIWORD(wParam))
					{
					case LBN_SELCHANGE:
						{
							SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_HINT_SUBJECT);
							if ( index < 0 ) break ;

							SI32 selectinx = SendDlgItemMessage(hDlg, IDC_LIST_HINT_SUBJECT, LB_GETITEMDATA, index, 0);

							SetWindowText(m_hDlg,m_szSubject[selectinx]);
							Set(m_szListType[selectinx]);
						}
						break;
					}
				}
				break;
			case IDC_COMBO_HINT_TYPE:
				{
					switch(HIWORD(wParam))
					{
					case CBN_SELCHANGE:
						{
							TCHAR buffer[256] = TEXT("");
							ComboGetString(hDlg, IDC_COMBO_HINT_TYPE, (LPARAM)buffer);

							SI32 type = GetExplainType(buffer);
							if ( type < 0 ) break ;

							ShowExplainHint(hDlg, type);
						}
						break;
					}
				}
				break;
			case IDCANCEL:
				{
					ShowWindow(m_hDlg, SW_HIDE);
				}
				break;
			}
			break;
		}
		break;
	case WM_CLOSE:
		{
			ShowWindow(m_hDlg, SW_HIDE);
		}
		break;
	case WM_DESTROY:
		{
			if ( m_hHitBitmap )
			{
				DeleteObject(m_hHitBitmap);
				m_hHitBitmap = NULL ;
			}

			GetWindowRect(m_hDlg, &m_DlgRect);
			m_DlgRectInitSwitch = TRUE ;

			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE ;
}

SI32 cltHintDlgExplain::GetExplainType(TCHAR* pExplainType)
{
	if ( pExplainType == NULL || !_tcscmp(pExplainType, TEXT("")) ) return -1 ;

	for(SI32 i = 0; i < MAX_HINTCOMBO_TYPE; ++i)
	{
		if ( !_tcscmp(m_szComboType[i], TEXT("")) ) continue ;
		if ( !_tcscmp(m_szComboType[i], pExplainType) )
		{
			return i;
		}
	}

	return -1 ;
}

void cltHintDlgExplain::ShowExplainHint(HWND hDlg, SI32 siHintType)
{
	TCHAR filename[MAX_PATH] = TEXT("") ;
	_tcscpy( filename, m_szBmpName[siHintType] );

	if(m_hHitBitmap != NULL)
	{
		DeleteObject(m_hHitBitmap);
		m_hHitBitmap = NULL;
	}

	m_hHitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

	if ( m_hHitBitmap != NULL )
	{
		SendDlgItemMessage( hDlg, IDC_STATIC_EXPLAIN_BMP, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHitBitmap);
	}		
}

void cltHintDlgExplain::Set(TCHAR * szFileName )
{
	if ( !_tcscmp(szFileName, TEXT("NULL")) )
		return ;

	TCHAR buffer[1024] = TEXT("") ;
	TCHAR path[256] = TEXT("") ;

	sprintf(path,TEXT("hintdlg\\%s.txt"),szFileName);

	FILE* fp = _tfopen(path, TEXT("rt"));
	if(fp == NULL)
	{
		MsgBox(TEXT("fileError"), TEXT("not HintDlg"));
		return ;
	}


	// 첫번째 줄에서 주제를 얻어온다.
	_fgetts(buffer, 1024, fp);
	SI32 len = _tcslen(buffer);
	buffer[len-1] = '\0';
	SetWindowText(GetDlgItem(m_hDlg,IDC_STATIC_SUBJECT),buffer);

	ComboDelete(m_hDlg,IDC_COMBO_HINT_TYPE);

	// 5줄은 무시한다.
	for(SI32 i = 0;i < 5;i++)
		_fgetts(buffer, 1024, fp);

	for ( i = 0 ; i < MAX_HINTCOMBO_TYPE ; i ++ )
	{
		_tcscpy(m_szComboType[i], TEXT(""));
		_tcscpy(m_szBmpName[i], TEXT(""));
	}

	i = 0 ;
	while( _fgetts(buffer, 1024, fp) != NULL)
	{
		if ( i >= MAX_HINTCOMBO_TYPE ) break ;

		TCHAR * szcode;
		TCHAR * szname;

		szcode = _tcstok(buffer, TEXT("\t"));
		szname = _tcstok(NULL, TEXT("\r\n"));

		if ( !szcode || !szname ) break ;

		ComboAddString(m_hDlg, IDC_COMBO_HINT_TYPE, (LPARAM)szcode);

		_tcscpy(m_szComboType[i],szcode);
		_tcscpy(m_szBmpName[i],szname);
		i ++ ;
	}
	SendDlgItemMessage( m_hDlg, IDC_COMBO_HINT_TYPE, CB_SETCURSEL, 0, 0 );
	ShowExplainHint(m_hDlg, 0);

	fclose(fp);
}

void cltHintDlgExplain::ReadListFile()
{
	TCHAR buffer[1024] = TEXT("") ;

	FILE* fp = _tfopen(TEXT("hintdlg\\filelist.txt"), TEXT("rt"));
	if(fp == NULL)
	{
		MsgBox(TEXT("fileError"), TEXT("not HintDlgList"));
		return ;
	}

	// 6줄은 무시한다.
	for(SI32 i = 0;i < 6;i++)
		_fgetts(buffer, 1024, fp);

	for ( i = 0 ; i < MAX_HINTCOMBO_TYPE ; i ++ )
	{
		_tcscpy(m_szListType[i], TEXT(""));
	}

	ListDelete(m_hDlg, IDC_LIST_HINT_SUBJECT);
	i = 0 ;
	while( _fgetts(buffer, 1024, fp) != NULL)
	{
		if ( i >= MAX_HINTCOMBO_TYPE ) break ;

		TCHAR * szcode;
		TCHAR * szname;
		TCHAR * szshow;

		szcode = _tcstok(buffer, TEXT("\t"));
		szname = _tcstok(NULL, TEXT("\t"));
		szshow = _tcstok(NULL, TEXT("\t\n"));

		if ( !szcode || !szname || !szshow ) break ;

		bool bshow = DATA_TO_BOOL(_tstoi(szshow)) ;
		if ( bshow == false ) continue ;

		_tcscpy(m_szSubject[i],szcode);
		_tcscpy(m_szListType[i],szname);

		SI32 index = ListAddString(m_hDlg, IDC_LIST_HINT_SUBJECT, (LPARAM)m_szSubject[i]);
		ListSetItemData(m_hDlg, IDC_LIST_HINT_SUBJECT, index, i);

		i ++ ;
	}

	fclose(fp);
}

void cltHintDlgExplain::ShowList(TCHAR * szFileName)
{
	SendMessage(GetDlgItem(m_hDlg,IDC_LIST_HINT_SUBJECT), LB_SETCURSEL, (WPARAM)-1, 0 );

	if ( !_tcscmp(szFileName, TEXT("NULL")) )
	{
		SetWindowText(m_hDlg, TEXT("게임 도움말"));
		SI32 index = (SI32)SendDlgItemMessage(m_hDlg, IDC_LIST_HINT_SUBJECT, LB_GETITEMDATA, 0, 0);
		_tcscpy(szFileName,m_szListType[index]);
		return ;
	}

	for ( SI32 i = 0 ; i < MAX_HINTLIST_TYPE ; i ++ )
	{
		if ( !_tcscmp(m_szListType[i],szFileName) )
		{
			TCHAR buffer[256] = TEXT("") ;
			for ( SI32 j = 0 ; j < MAX_HINTLIST_TYPE ; j ++ )
			{
				SendDlgItemMessage(m_hDlg, IDC_LIST_HINT_SUBJECT, LB_GETTEXT, j, (LPARAM)buffer);

				if ( !_tcscmp(m_szSubject[i],buffer) )
				{
					SendMessage(GetDlgItem(m_hDlg,IDC_LIST_HINT_SUBJECT), LB_SETCURSEL, j, 0 );
					break;
				}
			}
			SetWindowText(m_hDlg,m_szSubject[i]);
			break;
		}
	}
}
*/