/*
#include "LetterDlg.h"
#include "LetterMgr.h"
#include "..\..\..\Common\SMLib\LibList\LibList.h"
#include "..\..\..\Common\SMLib\LibList\Obj.h"

#include "../../Client/client.h"
extern cltCommonLogic* pclClient;

CLetterDlg g_LetterDlg;

CLetterDlg::CLetterDlg()
{
	m_hDlg = NULL;
	m_pCurrentLetter =  NULL;

	m_pLetterMgr = new CLetterMgr();

	Initialize();
}

CLetterDlg::~CLetterDlg()
{
	if ( m_pLetterMgr )
	{
		delete m_pLetterMgr;
		m_pLetterMgr = NULL;
	}

	Destroy();

	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CLetterDlg::Initialize()
{
	m_hAnswerEditBrush = NULL ;

	return;
}

void CLetterDlg::Destroy()
{
	if ( m_hAnswerEditBrush)
	{
		DeleteObject( m_hAnswerEditBrush);
		m_hAnswerEditBrush = NULL;
	}

	return;
}

void CLetterDlg::Create( HWND hWnd, HINSTANCE hInst )
{
	m_hDlg = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_LETTER), hWnd, StaticLetterDlgProc );
	Hide();
	m_bShow = false;

	return;
}

BOOL CALLBACK CLetterDlg::StaticLetterDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_LetterDlg.LetterDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CLetterDlg::LetterDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;
			m_hAnswerEditBrush = CreateSolidBrush(COLOR_DIALOG_YELLOW);
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_LETTER_DATA ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK);
				SetBkColor( hdc, COLOR_DIALOG_YELLOW);

				return (BOOL)m_hAnswerEditBrush;			}
		}
		break;
	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDC_BUTTON_PRE_LETTER:
				{
					if ( m_pCurrentLetter )
					{
						if ( m_pCurrentLetter->GetPre() )
						{
							stLetterInsert LetterInsert;
							UI32 uiDataSize = 0;

							m_pCurrentLetter = m_pCurrentLetter->GetPre();
							m_pCurrentLetter->GetData( (TCHAR*)&LetterInsert, &uiDataSize );
							
							SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_LETTER_DATA ), LetterInsert.strLetterMsg );

							CObj* pTemp = m_pLetterMgr->GetExistList()->GetLast();

							SI32 siTotalCount = m_pLetterMgr->GetExistList()->GetObjNum();
							SI32 siCount = 0;

							while ( pTemp )
							{
								if ( pTemp == m_pCurrentLetter )
								{
									TCHAR Buffer[256];
									TCHAR* pText = GetTxtFromMgr(2313);
									sprintf( Buffer, pText, siTotalCount - siCount, siTotalCount );
									SetWindowText( m_hDlg, Buffer );
									break;
								}
								pTemp = pTemp->GetPre();
								++siCount;
							}
							
						}
					}
				}
				break;
			case IDC_BUTTON_NEXT_LETTER:
				{
					if ( m_pCurrentLetter )
					{
						if ( m_pCurrentLetter->GetNext() )
						{
							stLetterInsert LetterInsert;
							UI32 uiDataSize = 0;

							m_pCurrentLetter = m_pCurrentLetter->GetNext();
							m_pCurrentLetter->GetData( (TCHAR*)&LetterInsert, &uiDataSize );
							
							SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_LETTER_DATA ), LetterInsert.strLetterMsg );

							CObj* pTemp = m_pLetterMgr->GetExistList()->GetLast();

							SI32 siTotalCount = m_pLetterMgr->GetExistList()->GetObjNum();
							SI32 siCount = 0;

							while ( pTemp )
							{
								if ( pTemp == m_pCurrentLetter )
								{
									TCHAR Buffer[256];
									TCHAR* pText = GetTxtFromMgr(2313);
									sprintf( Buffer, pText, siTotalCount - siCount, siTotalCount );
									SetWindowText( m_hDlg, Buffer );
									break;
								}
								pTemp = pTemp->GetPre();
								++siCount;
							}

						}
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

/*
void CLetterDlg::DeleteLetter()
{
	m_pLetterMgr->PushDeleteIndex( m_siCurrentLetter );
	
	SI32 ExistLetterNum = m_pLetterMgr->GetExistList()->GetObjNum();
	
	TCHAR Buffer[256];
	sprintf( Buffer, TEXT("받은 쪽지, 총 %d개"), ExistLetterNum );
	SetWindowText( m_hDlg, Buffer );
	
	CObj *pTemp = m_pLetterMgr->GetExistList()->GetFirst();

	stLetterInsert *pLetterInsert = NULL;
	UI32 uiDataSize;	
	pTemp->GetDataPtr( (TCHAR*)pLetterInsert, &uiDataSize );

	m_siCurrentLetter = pLetterInsert->siLetterIndex;
	
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_LETTER_DATA ), pLetterInsert->strLetterMsg );
	
	return;
}
*/
/*
void CLetterDlg::Show()
{
	if ( m_bShow )
	{
		return;
	}

	m_bShow = true;	
	ShowWindow ( m_hDlg, SW_SHOWNORMAL );

	return;
}

void CLetterDlg::Hide()
{
	if ( ! m_bShow )
	{
		return;
	}

	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	m_pLetterMgr->Clear();

	return;
}

bool CLetterDlg::IsShow()
{
	return m_bShow;
}

bool CLetterDlg::Push( TCHAR *strLetterMsg )
{
	if ( m_pLetterMgr->Push( strLetterMsg ) )
	{
		SI32 ExistLetterNum = m_pLetterMgr->GetExistList()->GetObjNum();
	
		TCHAR Buffer[256];
		TCHAR* pText = GetTxtFromMgr(2313);
		sprintf( Buffer, pText, ExistLetterNum, ExistLetterNum );
		SetWindowText( m_hDlg, Buffer );		
		
		m_pCurrentLetter = m_pLetterMgr->GetExistList()->GetLast();
		
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_LETTER_DATA ), strLetterMsg );
		
		Show();
		return true;
	}

	return false;
}
*/