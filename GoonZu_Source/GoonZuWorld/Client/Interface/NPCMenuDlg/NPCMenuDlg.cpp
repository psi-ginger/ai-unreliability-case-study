// NPCMenuDlg.cpp: implementation of the CNPCMenuDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "../../../Client/client.h"
#include "Resource.h"

#include "Char/CharManager/CharManager.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/CommonLogic.h"
#include "../../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../../Client/Interface/NPC1/NPCTradeDlg.h"
#include "../../../Client/Interface/JangGilSan/JangGilSanDlg.h"
#include "../../../Client/Interface/InvestmentDlg/InvestmentDlg.h"
#include "../../../Client/Interface/TradeMerchantDlg/TradeMerchantDlg.h"
#include "NInterface/NInvestmentDlg/NInvestmentDlg.h"  //봉이김선달 
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "NInterface/NNPC1/NNPCTradeDlg.h"
#include "NInterface/NTradeMerchant/NTradeMerchant.h"

extern cltCommonLogic* pclClient;

#include "NPCMenuDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNPCMenuDlg::CNPCMenuDlg()
{
	m_hInst = NULL ;
	m_hWnd = NULL ;

	m_hPortraitBitmap = NULL ;

	m_hDlg = NULL ;

	SetRect(&m_rtDlg,0,0,0,0);

	m_bRect = FALSE ;

	m_siCharKind = 0 ;
}

CNPCMenuDlg::~CNPCMenuDlg()
{
	m_hInst = NULL ;
	m_hWnd = NULL ;
}

void CNPCMenuDlg::Create(HINSTANCE hinst, HWND hwnd )
{
	m_hInst = hinst ;
	m_hWnd = hwnd ;
}

void CNPCMenuDlg::CreateDlg(SI32 charkind,SI32 charunique,char * txt1 , char * txt2 ,
							bool button1,bool button2,char * txt )
{
	if ( IsWindow(m_hDlg) && m_siCharKind == charkind )
		DestroyWindow(m_hDlg);

	if ( txt1 == NULL ) return ;
	if ( txt2 == NULL ) return ;
	if ( txt == NULL ) return ;
	if ( charkind <= 0 || charkind > MAX_KIND_NUMBER ) return ;
	if ( charunique <= 0 ) return ;
	cltKindInfo* clKind = pclClient->pclKindInfoSet->pclKI[charkind];
	if ( clKind == NULL ) return ;

	m_siCharKind = charkind ;
	m_siCharUnique = charunique ;

	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_NPCMENU ), m_hWnd, StatiCNPCMenuProc );

		if ( m_bRect )
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}
	}

	if ( m_hPortraitBitmap )
	{
		DeleteObject(m_hPortraitBitmap);
		m_hPortraitBitmap = NULL ;
	}
	char buffer[1024] = "" ;
	sprintf(buffer,"Interface/Portrait/%s.bmp", clKind->szCode);

	if ( m_hPortraitBitmap )
	{
		DeleteObject(m_hPortraitBitmap);
		m_hPortraitBitmap = NULL ;
	}

	m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), buffer, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

	if ( m_hPortraitBitmap != NULL )
	{
		SendDlgItemMessage( m_hDlg, IDC_STATIC_PORTRAIT, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);
	}

	HWND hwnd = GetDlgItem(m_hDlg,IDC_BUTTON_1);
	strcpy(buffer,txt1);
	SetWindowText(hwnd,buffer);
	EnableWindow(hwnd,button1);

	hwnd = GetDlgItem(m_hDlg,IDC_BUTTON_2);
	strcpy(buffer,txt2);
	SetWindowText(hwnd,buffer);
	EnableWindow(hwnd,button2);

	if( KIND_INVESTMENT == m_siCharKind)
	{
		hwnd = GetDlgItem(m_hDlg,IDC_BUTTON_3);
		strcpy(buffer,"물품 회수");
		SetWindowText(hwnd,buffer);
		EnableWindow(hwnd, true);

		hwnd = GetDlgItem(m_hDlg,IDC_BUTTON_4);
		strcpy(buffer,"시세확인");
		SetWindowText(hwnd,buffer);
		EnableWindow(hwnd, true);
	}
	else
	{
		hwnd = GetDlgItem(m_hDlg,IDC_BUTTON_3);
	//	strcpy(buffer,"물품 회수");
	//	SetWindowText(hwnd,buffer);
		ShowWindow(hwnd, SW_HIDE);
	}

	hwnd = GetDlgItem(m_hDlg,IDC_EDIT_NPC);
	strcpy(buffer,txt);
	SetWindowText(hwnd,buffer);

	strcpy(buffer,clKind->GetName());
	SetWindowText(m_hDlg,buffer);

	ShowWindow(m_hDlg,SW_SHOW);
}

BOOL CALLBACK CNPCMenuDlg::StatiCNPCMenuProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pNPCMenuDlg->NPCMenuProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CNPCMenuDlg::NPCMenuProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
		}
		break;
	case WM_TIMER:
		{
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_1:
			{
				switch(m_siCharKind)
				{
//				case KIND_JANGGILSAN:
//					{
//						pclClient->CreateQuestDlg(KIND_JANGGILSAN);
//					}
//					break;
				case KIND_INVESTMENT:
					{
						
		



						if ( g_SoundOnOffDlg.m_bFullScreenMode )
						{
							if(( pclclient->m_pDialog[ NNPCTRADE1_DLG ])==NULL)
								{
								pclclient->CreateInterface( NNPCTRADE1_DLG );
								}
							//(pclclient->(CNNPCTradeDlg*)m_pDialog[ NNPCTRADE1_DLG  ])->Set( &pclCM->CR[1]->pclCI->clCharItem,  pclNPCItemInfo);
							((CNNPCTradeDlg*)pclclient->m_pDialog[ NNPCTRADE1_DLG  ])->Show(m_siCharUnique);
						}
						else
						{
							pclclient->m_pNPCTradeDlg->Show(m_siCharUnique);
						}


						
						
					}
					break;
				case KIND_RUSSIATRADER:
					{

						


						



						if ( g_SoundOnOffDlg.m_bFullScreenMode )
						{
							if((  pclclient->m_pDialog[ NNPCTRADE1_DLG ])==NULL)
								{
								pclclient->CreateInterface( NNPCTRADE1_DLG );
								}
							((CNNPCTradeDlg*)pclclient->m_pDialog[ NNPCTRADE1_DLG  ])->Show(pclclient->pclCM->CR[m_siCharUnique]->GetCharUnique());
						}
						else
						{
							pclclient->m_pNPCTradeDlg->Show(pclclient->pclCM->CR[m_siCharUnique]->GetCharUnique());
						}


						

					}
					break;
				}
			}
			break;
		case IDC_BUTTON_2:
			{
				switch(m_siCharKind)
				{
//				case KIND_JANGGILSAN:
//					{
//						pclclient->m_pJangGilSanDlg->ShowSBS(pclclient->pclCM->CR[1]->ClearQuest(30090));
//					}
//					break;
				case KIND_INVESTMENT:
					{
						if ( g_SoundOnOffDlg.m_bFullScreenMode )
						{
							if(pclclient->m_pDialog[NINVESTMENT_DLG]==NULL)
							{
								pclclient->CreateInterface(NINVESTMENT_DLG);
								((CNInvestmentDlg*)pclclient->m_pDialog[NINVESTMENT_DLG])->Set(m_siCharUnique);
							}
							else
							{
								(pclclient->m_pDialog[NINVESTMENT_DLG])->DeleteDialog();
							}
						}
						else
						{
							pclclient->m_pInvestmentDlg->Set(m_siCharUnique);
						}
					}
					break;
				case KIND_RUSSIATRADER:
					{
						if(g_SoundOnOffDlg.m_bFullScreenMode)
						{
							pclclient->CreateInterface( NTRADEMERCHANT_DLG );
							if( ((CNTradeMerchantDlg*)pclclient->m_pDialog[NTRADEMERCHANT_DLG]) )
								((CNTradeMerchantDlg*)pclclient->m_pDialog[ NTRADEMERCHANT_DLG ])->SetTradeMerChant(m_siCharUnique);
						}
						else
							pclclient->m_pTradeMerchantDlg->Show( m_siCharUnique );
					}
					break;
				}
			}
			break;

		case IDC_BUTTON_3:
			{
				if(m_siCharKind ==KIND_INVESTMENT ) //봉이김선달이면 ..
				{                  

					if(( pclclient->m_pDialog[ NSAVEITEMLIST_DLG ]) == NULL)
					{
						pclclient->CreateInterface( NSAVEITEMLIST_DLG );

						cltMsg clMsg(GAMEMSG_REQUEST_GET_SAVEUSERITEMLIST, 0, NULL);
						pclclient->SendMsgToServer((char*)&clMsg);
					}
					else
					{
						(pclclient->m_pDialog[ NSAVEITEMLIST_DLG  ])->DeleteDialog();
					}

				}
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
			if ( m_hPortraitBitmap )
			{
				DeleteObject(m_hPortraitBitmap);
				m_hPortraitBitmap = NULL ;
			}

			GetWindowRect(m_hDlg, &m_rtDlg);
			m_bRect = TRUE ;

			m_siCharKind = 0 ;

			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

void CNPCMenuDlg::Close()
{
	if ( IsWindow(m_hDlg) )
	{
		DestroyWindow(m_hDlg);
	}
}
