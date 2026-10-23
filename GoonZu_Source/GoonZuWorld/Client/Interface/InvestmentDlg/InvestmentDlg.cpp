// InvestmentDlg.cpp: implementation of the CInvestmentDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "../../Client/client.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"

#include "InvestmentDlg.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInvestmentDlg::CInvestmentDlg()
{
	m_hInst = NULL ;
	m_hWnd = NULL ;

	m_bRectSwitch = FALSE ;
	SetRect(&m_rtDlg,0,0,0,0);

	m_hDlg = NULL ;

	m_hInvestmentList = NULL ;

	m_hPortraitBitmap = NULL ;

	m_hEditBrush = NULL ;
	m_hDlgBrush = NULL ;

	m_siNPCCharUnique = 0 ;

	for ( int i = 0 ; i < MAX_INVESTMENT_ITEM_NUMBER ; i ++ )
		m_siItemUnique[i] = 0 ;
}

CInvestmentDlg::~CInvestmentDlg()
{
}

void CInvestmentDlg::Create(HINSTANCE hInst, HWND hParent )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;
}

BOOL CALLBACK CInvestmentDlg::StatiCInvestmentDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;

	return pclclient->m_pInvestmentDlg->InvestmentDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CInvestmentDlg::InvestmentDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_YELLOW );
			m_hDlgBrush  = CreateSolidBrush( COLOR_DIALOG_LIGHTBLUE);

			m_hInvestmentList = GetDlgItem( hDlg, IDC_LIST_INVESTMENT );

			if(m_hInvestmentList)
			{
				ListView_SetExtendedListViewStyle( m_hInvestmentList, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
				ListView_SetHoverTime( m_hInvestmentList, 10 );

				char* pText = GetTxtFromMgr(1641);
				ListViewInsertColumn( m_hInvestmentList, 40, pText,			0, 1);
				pText = GetTxtFromMgr(1831);
				ListViewInsertColumn( m_hInvestmentList, 160, pText,			1, 1);
				pText = GetTxtFromMgr(2303);
				ListViewInsertColumn( m_hInvestmentList, 100, pText,			2, 1);
				pText = GetTxtFromMgr(2304);
				ListViewInsertColumn( m_hInvestmentList, 100, pText,			3, 1);
				pText = GetTxtFromMgr(2305);
				ListViewInsertColumn( m_hInvestmentList, 100, pText,			4, 1);

				ListView_DeleteAllItems(m_hInvestmentList);
			}

			if( m_hPortraitBitmap == NULL )
			{
				m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/KIND_INVESTMENT.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

				if ( m_hPortraitBitmap != NULL )
				{
					SendDlgItemMessage( hDlg, IDC_STATIC_PORTRAIT_INVESTMENT, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);
				}
			}

			char buffer[1024] = "" ;
			char* pText = GetTxtFromMgr(2306);
			sprintf(buffer, pText);

			SetWindowText( GetDlgItem(hDlg,IDC_EDIT_INVESTMENT), buffer );
		}
		break;
/*	case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC)wParam;
			SetTextColor( hdc, COLOR_DIALOG_BLACK);
			SetBkColor( hdc, COLOR_DIALOG_YELLOW );
							
			return (BOOL)m_hEditBrush;
		}
		break;*/
/*	case WM_CTLCOLORDLG:
		{
			HDC hdc = (HDC)wParam;
			SetTextColor( hdc, COLOR_DIALOG_BLACK);
			SetBkColor( hdc, COLOR_DIALOG_YELLOW );
								
			return (BOOL)m_hDlgBrush;
		}
		break;*/
	case WM_NOTIFY:
		{
			//m_hItemListCtrl
			LPNMHDR						hdr;
			LPNMITEMACTIVATE			nia;
			LPNMLVGETINFOTIP			ngt;
			LPNMLISTVIEW				nlv;
			hdr = ( LPNMHDR )			lParam;
			ngt = ( LPNMLVGETINFOTIP )	lParam;
			nlv = ( LPNMLISTVIEW )		lParam;
			nia = (LPNMITEMACTIVATE)	lParam;

			if ( hdr->hwndFrom == m_hInvestmentList )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						SI32 index = nia->iItem ;
						if ( index < 0 ) break;

						char buffer[1024] = "" ;

						SI32 ref = pclclient->pclItemManager->FindItemRefFromUnique(m_siItemUnique[index]);
						cltItemInfo * pclinfo = pclclient->pclItemManager->pclItemInfo[ref] ;
						if ( pclinfo == NULL ) break ;

						for ( int i = 0 ; i < MAX_MATERIAL_NUMBER ; i ++ )
						{
							SI32 materialunique = pclinfo->clProductionInfo.clMaterial[i].siUnique ;
							if ( materialunique <= 0 ) continue;

							const char * name = pclclient->pclItemManager->GetName(materialunique);

							SI32 neednum = pclinfo->clProductionInfo.clMaterial[i].siNeedNum;

							char buf[256] = "" ;

							if ( i == 0 )
							{
								char* pText = GetTxtFromMgr(2307);
								sprintf( buf, pText,name,neednum);
							}
							else
							{
								char* pText = GetTxtFromMgr(2308);
								sprintf( buf, pText,name,neednum);
							}

							strcat(buffer,buf);
						}

						SetWindowText(GetDlgItem(hDlg,IDC_EDIT_MATERIALS),buffer);
					}
					break;
				}
			}
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				char buffer[256] = "" ;
				char* pText = GetTxtFromMgr(2309);
				wsprintf(buffer, pText,
					INVESTMENT_GETINFO_MONEY);
				char* pTitle = GetTxtFromMgr(2310);
				if (  MessageBox(NULL, buffer,
					pTitle, MB_ICONQUESTION | MB_YESNO) == IDNO )
					break;

				cltGameMsgRequest_InvestmentGetInfo clinfo(m_siNPCCharUnique);
				cltMsg clMsg( GAMEMSG_REQUEST_INVESTMENTGETINFO, sizeof( clinfo ), (char*)&clinfo );
				pclClient->SendMsgToServer((char*)&clMsg);
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
			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}
			if ( m_hDlgBrush )
			{
				DeleteObject( m_hDlgBrush );
				m_hDlgBrush = NULL;
			}
			if ( m_hPortraitBitmap )
			{
				DeleteObject( m_hPortraitBitmap );
				m_hPortraitBitmap = NULL;
			}

			for ( int i = 0 ; i < MAX_INVESTMENT_ITEM_NUMBER ; i ++ )
				m_siItemUnique[i] = 0 ;

			GetWindowRect(m_hDlg, &m_rtDlg);
			m_bRectSwitch = TRUE ;

			m_hInvestmentList = NULL ;

			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

void CInvestmentDlg::Set(SI32 npcunique)
{
	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_INVESTMENT ), m_hWnd, StatiCInvestmentDlgProc );

		if ( m_bRectSwitch )
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}

		m_siNPCCharUnique = npcunique ;

		Show();
	}
	else
	{
		DestroyWindow(m_hDlg);
	}
}

void CInvestmentDlg::Show()
{
	cltClient *pclclient = (cltClient*)pclClient;

	ShowWindow( m_hDlg, SW_SHOW );
}

void CInvestmentDlg::ShowListData(InvestmentLines* investmentlines)
{
	cltClient *pclclient = (cltClient*)pclClient;

	ListView_DeleteAllItems(m_hInvestmentList);

	SI32 siIndex = 0 ;
	for ( int i = 0 ; i < MAX_INVESTMENT_ITEM_NUMBER ; i ++ )
	{
		if ( investmentlines[i].siInvestmentItemUnique <= 0 ) continue ;

		SI32 ref = pclclient->pclItemManager->FindItemRefFromUnique(investmentlines[i].siInvestmentItemUnique);
		char buffer[256] = "" ;
		sprintf(buffer,"%d",siIndex+1);
		ListViewInsertItem(m_hInvestmentList, -1, siIndex, 0, buffer, -1);

		sprintf(buffer,"%s",pclClient->pclItemManager->GetName(investmentlines[i].siInvestmentItemUnique)); //최대내구상승부적 (물품내용)
		ListViewInsertItem(m_hInvestmentList, -1, siIndex, 1, buffer, -1);
		m_siItemUnique[siIndex] = investmentlines[i].siInvestmentItemUnique ;

		SI32 materialprice = 0 ;
		for ( int j = 0 ; j < MAX_MATERIAL_NUMBER ; j ++ )
		{
			if ( investmentlines[i].siMaterialItemPrice[j] <= 0 ) continue;

			SI32 neednum = pclclient->pclItemManager->pclItemInfo[ref]->clProductionInfo.clMaterial[j].siNeedNum;
			SI32 price = investmentlines[i].siMaterialItemPrice[j] * neednum ;

			materialprice += price ;  //417885
		}

		char* pText = GetTxtFromMgr(611);  //냥
		g_JYLibFunction.SetNumUnit(materialprice,buffer,pText);
		ListViewInsertItem(m_hInvestmentList, -1, siIndex, 2, buffer, -1);

		g_JYLibFunction.SetNumUnit(investmentlines[i].siInvestmentItemPrice,buffer,pText);
		ListViewInsertItem(m_hInvestmentList, -1, siIndex, 3, buffer, -1);

		g_JYLibFunction.SetNumUnit(investmentlines[i].siMarginalProfit,buffer,pText);
		ListViewInsertItem(m_hInvestmentList, -1, siIndex, 4, buffer, -1);
		siIndex ++ ;
	}
}