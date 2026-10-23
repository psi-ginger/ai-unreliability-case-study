
#include "TotalChatDlg.h"

#include "..\..\Client\Client.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;



CTotalChatDlg::CTotalChatDlg()
{
	m_hTotalChatDlg = NULL ;
	CurrentPage = 0;
	for(SI32 i = 0; i< MAX_TOTALCHAT_PAGE_NUMBER;i++)
	{
		TabDlg[i]	= NULL;
	}
	//m_pCTotalChatTabDlg1 = new CTotalChatTabDlg1();
	//m_pCTotalChatTabDlg2 = new CTotalChatTabDlg2();
	//m_pCTotalChatTabDlg3 = new CTotalChatTabDlg3();
	//m_pCTotalChatTabDlg4 = new CTotalChatTabDlg4();

}

CTotalChatDlg::~CTotalChatDlg()
{
	if( m_hTotalChatDlg )
	{
		DestroyWindow( m_hTotalChatDlg );
	}

}

void CTotalChatDlg::Create(HINSTANCE hInst, HWND hParent )
{

	m_hInst = hInst ;
	m_hWnd = hParent ;	
	
//	if(m_hTotalChatDlg== NULL)                
//		m_hTotalChatDlg = CreateDialog( pclClient->GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_TOTALCHAT), m_hWnd, StatiCTotalChatDlgProc );

	


}


BOOL CALLBACK CTotalChatDlg::StatiCTotalChatDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pTotalChatDlg->TotalChatDlgProc( hDlg, iMsg, wParam, lParam );
}


BOOL CALLBACK CTotalChatDlg::TotalChatDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			//m_hDlg = CreateDialog()
			m_hDlg = hDlg;
			// Tab Control의 Handle
			m_hTabCtrl = GetDlgItem( hDlg,  IDC_TOTALCHAT_TAB );
			//---------------------------------------------------
			// 텝 컨트롤의 탭 부분 기본 설정
			TC_ITEM subject;
			// to use an image set this to TCIF_IMAGE  
			subject.mask= TCIF_TEXT; 
			// index into tabs image list, set to -1 when not in use
			subject.iImage = -1;

			char* pText = GetTxtFromMgr(5552);			//전체 
			subject.lParam = 0;	subject.pszText = pText;
			TabCtrl_InsertItem( m_hTabCtrl, 0, &subject );
			pText = GetTxtFromMgr(5553);				//마을
			subject.lParam = 1;	subject.pszText = pText;
			TabCtrl_InsertItem( m_hTabCtrl, 1, &subject );
			pText = GetTxtFromMgr(5554);				//상단
			subject.lParam = 2;	subject.pszText = pText;
			TabCtrl_InsertItem( m_hTabCtrl, 2, &subject );
			pText = GetTxtFromMgr(5555);				 //파티
			subject.lParam = 3;	subject.pszText = pText;
			TabCtrl_InsertItem( m_hTabCtrl, 3, &subject );			
			//--------------------------------------------------
			// 맨 끝에 탭 안에 내용물 다이얼로그 생성
			//if(m_hTotalChatDlg== NULL)                
			CurrentPage++;
			//CreateTabDlg(hDlg, CurrentPage, IDD_DIALOG_TOTALCHAT_TAB1, StatiCTotalChatTabDlg1Proc);
			
			 
			ComboSetting(hDlg);
			
			//m_hTotalChatTabDlg1 = CreateDialog( pclClient->GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_TOTALCHAT_TAB1), m_hTotalChatDlg, StatiCTotalChatTabDlg1Proc );
			 /*RECT rect;
			 GetClientRect( m_hTotalChatTabDlg1, &rect);
			 TabCtrl_AdjustRect( m_hTotalChatTabDlg1, FALSE, &rect);

			 MoveWindow( m_hTotalChatDlg,
				 rect.left, rect.top, rect.right-rect.left,
				 rect.bottom-rect.top, TRUE);

			 ShowWindow( m_hTotalChatDlg, SW_SHOW );
			

			// Tab Control의 크기안에 DialogBox 크기를 맞춘다.
			RECT rect;
			GetClientRect( m_hTabCtrl, &rect);
			TabCtrl_AdjustRect( m_hTabCtrl, FALSE, &rect);

			MoveWindow( m_hTotalChatTabDlg,
				rect.left, rect.top, rect.right-rect.left,
				rect.bottom-rect.top, TRUE);

			ShowWindow( m_hTotalChatTabDlg, SW_SHOW );
		*/	

			//m_hButton = GetDlgItem(hDlg,IDOK);
			//EnableWindow(m_hButton,FALSE);

			//m_CurrentTabIndex = 0;

			//m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE );
			
		}
		break;

	case WM_TIMER:
		{
		

		}
		break;
	case WM_NOTIFY:
		{
			if(TabDlg[m_CurrentTabIndex])
			{
				ShowWindow(TabDlg[m_CurrentTabIndex], SW_HIDE) ;
			}

			/*
			LPNMHDR lpNmhdr =(LPNMHDR)lParam;

			switch ( lpNmhdr->idFrom )
			{
			case IDC_TOTALCHAT_TAB:
				{
					if ( lpNmhdr->code == TCN_SELCHANGE )
					{
						int index = TabCtrl_GetCurSel( GetDlgItem ( hDlg, IDC_TOTALCHAT_TAB ) );

						if ( index < 0 || index > 4 )
						{
							return FALSE;
						}


						m_CurrentTabIndex = index;
						ShowTabDialog(m_CurrentTabIndex);
					}
				}
				break;
			}
			*/
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
			for(SI32 i = 0;i < MAX_TOTALCHAT_PAGE_NUMBER;i++)
			{
				if(TabDlg[i])
				{
					DestroyWindow( TabDlg[i] );
					TabDlg[i] = NULL;
				}
			}
			
		}
		break;
	}

	return FALSE;
}


void CTotalChatDlg::ComboSetting(HWND hDlg)
{
	ComboDelete(hDlg, IDC_TOTALCHAT_COMBO);

	char* pText = GetTxtFromMgr(1216);
	SendDlgItemMessage(hDlg,IDC_TOTALCHAT_COMBO,CB_ADDSTRING, 0,(LPARAM)pText);

	pText = GetTxtFromMgr(1215);
	SendDlgItemMessage(hDlg,IDC_TOTALCHAT_COMBO,CB_ADDSTRING, 1,(LPARAM)pText);

	pText = GetTxtFromMgr(5482);
	SendDlgItemMessage(hDlg,IDC_TOTALCHAT_COMBO,CB_ADDSTRING, 2,(LPARAM)pText);

	SendDlgItemMessage( hDlg, IDC_TOTALCHAT_COMBO, CB_SETCURSEL, 0, 0 );

}

void CTotalChatDlg::Show()
{

	ShowWindow( m_hTotalChatDlg, SW_SHOW );
}

void CTotalChatDlg::Hide()
{
	
	ShowWindow( m_hTotalChatDlg, SW_HIDE );
}
