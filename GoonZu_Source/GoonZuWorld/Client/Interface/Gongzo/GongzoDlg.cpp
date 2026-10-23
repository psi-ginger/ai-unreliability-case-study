// GongzoDlg.cpp: implementation of the CGongzoDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "../../Client/client.h"

#include "../../Client/Music/Music.h"

#include "../../lib/WebHTML/WebHTML.h"

//-----------------------------
// CommonLogic
//-----------------------------
#include "MsgType.h"
#include "CommonLogic.h"
#include "MsgType-System.h"
#include "MsgType-Person.h"

#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "GongzoDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGongzoDlg::CGongzoDlg()
{
	m_siPriceTime = 0 ;
	m_siLeftOverValue = 0 ;
	m_siSetSkill = 0 ;

	for ( int i = 0 ; i < GONGZO_SKILL_TOTAL ; i ++ )
	{
		m_stGongzoMember.siPriceValue[i] = 0 ;
		m_stGongzoMember.siSkillValue[i] = 0 ;
	}

	m_hInst = NULL ;
	m_hWnd = NULL ;

	m_bRectSwitch = FALSE ;
	SetRect(&m_rtDlg,0,0,0,0);

	m_hDlg = NULL ;
	m_hGroup = NULL ;

	m_bSet = FALSE ;
	m_bSetSpin = FALSE ;
	m_bMonthOver = TRUE ;

	m_hEditBrush = NULL ;
	m_hDlgBrush = NULL ;

	strcpy(m_szGongzoName,"");
	strcpy(m_szGongzoProfile,"");
	strcpy(m_szGongzoID,"");
}

CGongzoDlg::~CGongzoDlg()
{
}

void CGongzoDlg::Create(HINSTANCE hInst, HWND hParent )
{
	m_hInst = hInst ;
	m_hWnd = hParent ;
}

BOOL CALLBACK CGongzoDlg::StatiCGongzoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->m_pGongzoDlg->GongzoDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CGongzoDlg::GongzoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_YELLOW );
			m_hDlgBrush  = CreateSolidBrush( COLOR_DIALOG_LIGHTBLUE);
			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER6 )
				SetTimer(hDlg, 1, 1000, NULL);
			else
				SetTimer(hDlg, 2, 60000, NULL);

			//---------------------------------
			// HTML 을 초기화한다.
			//---------------------------------
			//==================================			
			HWND hwnd = NULL ;

			hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
			MoveWindow( GetDlgItem( hDlg, IDC_STATIC_PROFILE ), 9, 122, 73, 73, TRUE );

			hwnd = GetDlgItem(hDlg, IDC_STATIC_GONGZOINTRO);
			if ( hwnd )
			{
				EmbedBrowserObject(hwnd);
			}
		}
		break;
	case WM_TIMER:
		{
			switch(wParam)
			{
			case 1:
				{
					char buffer[256] = "" ;
					GetDlgItemText(hDlg,IDC_EDIT_GONGZO,buffer,256);
					SI32 limit = strlen(buffer);

					char* pText = GetTxtFromMgr(2049);
					wsprintf(buffer, pText,m_szGongzoName,limit);
					SetWindowText(m_hGroup,buffer);

					if ( m_bSetSpin && m_bMonthOver )
					{
						m_bMonthOver = FALSE ;

						SI16 price = LOWORD( SendMessage(GetDlgItem(hDlg, m_siSpinNum), UDM_GETPOS, 0, 0 ) ) * 10 ;

						if ( price == m_stGongzoMember.siPriceValue[m_siSetSkill] &&
							m_stGongzoMember.siPriceValue[m_siSetSkill] != 0 &&
							m_stGongzoMember.siPriceValue[m_siSetSkill] != 50 )
							price -= 10 ;

						sprintf(buffer,"%d",price);
						HWND hwnd = GetDlgItem( hDlg, m_siIDCEditNum );
						SetWindowText( hwnd, buffer);

						if ( !SendGongzoMessage() )
						{
							cltMsg clMsg(GAMEMSG_REQUEST_GETGONGZOINFO,0,NULL);
							pclclient->SendMsgToServer((char*)&clMsg);
						}
					}
				}
				break;
			case 2:
				{
					cltMsg clMsg(GAMEMSG_REQUEST_GETGONGZOINFO,0,NULL);
					pclclient->SendMsgToServer((char*)&clMsg);
				}
				break;
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_GONGZO ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK);
				SetBkColor( hdc, COLOR_DIALOG_YELLOW );
								
				return (BOOL)m_hEditBrush;
			}
		}
		break;
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
			LPNMHDR pnmhdr=(LPNMHDR)lParam;
			if ( m_bSetSpin || !m_bMonthOver ) break ;

			if( pnmhdr->hwndFrom == GetDlgItem(hDlg, IDC_SPIN_GONGZO_SWORD) )
			{
				SetSpinValue(IDC_SPIN_GONGZO_SWORD,GONGZO_SKILL_SWORD,IDC_EDIT_SWORD);
				break;
			}
			if( pnmhdr->hwndFrom == GetDlgItem(hDlg, IDC_SPIN_GONGZO_SPEAR) )
			{
				SetSpinValue(IDC_SPIN_GONGZO_SPEAR,GONGZO_SKILL_SPEAR,IDC_EDIT_SPEAR);
				break;
			}
			if( pnmhdr->hwndFrom == GetDlgItem(hDlg, IDC_SPIN_GONGZO_STAFF) )
			{
				SetSpinValue(IDC_SPIN_GONGZO_STAFF,GONGZO_SKILL_STAFF,IDC_EDIT_STAFF);
				break;
			}
			if( pnmhdr->hwndFrom == GetDlgItem(hDlg, IDC_SPIN_GONGZO_AXE) )
			{
				SetSpinValue(IDC_SPIN_GONGZO_AXE,GONGZO_SKILL_AXE,IDC_EDIT_AXE);
				break;
			}
			if( pnmhdr->hwndFrom == GetDlgItem(hDlg, IDC_SPIN_GONGZO_BOW) )
			{
				SetSpinValue(IDC_SPIN_GONGZO_BOW,GONGZO_SKILL_BOW,IDC_EDIT_BOW);
				break;
			}
			if( pnmhdr->hwndFrom == GetDlgItem(hDlg, IDC_SPIN_GONGZO_GUN) )
			{
				SetSpinValue(IDC_SPIN_GONGZO_GUN,GONGZO_SKILL_GUN,IDC_EDIT_GUN);
				break;
			}
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDGGOM:
			{
				cltGameMsgRequest_Userid clUserid( m_szGongzoName );
				cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
				pclClient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDWHISPER:
			{
				if ( strlen(m_szGongzoName) > 0 && m_szGongzoName != NULL )
					pclclient->RequestSecretChatByCharName(m_szGongzoName);
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

			//---------------------------------
			// HTML 을 해제한다.
			//---------------------------------
			//==================================
			HWND hwnd = NULL ;

			hwnd = GetDlgItem( hDlg, IDC_STATIC_PROFILE );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}
			hwnd = GetDlgItem( hDlg, IDC_STATIC_GONGZOINTRO );
			if( hwnd )
			{
				DoPageAction(hwnd, WEBPAGE_STOP);
				UnEmbedBrowserObject(hwnd);
			}

			GetWindowRect(m_hDlg, &m_rtDlg);
			m_bRectSwitch = TRUE ;

			if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER6 )
				KillTimer(hDlg,1);
			else
				KillTimer(hDlg,2);

			m_bSet = FALSE ;
			m_hDlg = NULL ;
		}
		break;
	}

	return FALSE;
}

void CGongzoDlg::Set()
{
	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog( m_hInst, MAKEINTRESOURCE( IDD_DIALOG_GONGZO ), m_hWnd, StatiCGongzoDlgProc );
		SendDlgItemMessage( m_hDlg, IDC_EDIT_GONGZO, EM_LIMITTEXT, 255, 0 );
		m_hGroup = GetDlgItem(m_hDlg,IDC_STATIC_GONGZOPROFILE);

		if ( m_bRectSwitch )
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}

		HWND hButton = NULL ;
		cltClient *pclclient = (cltClient*)pclClient;
		if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER6 )
		{
			//hButton = GetDlgItem(m_hDlg,IDSETUP);
			//EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDC_EDIT_GONGZO);
			EnableWindow(hButton,FALSE);
		}
		else
		{
			hButton = GetDlgItem(m_hDlg,IDGGOM);
			EnableWindow(hButton,FALSE);

			hButton = GetDlgItem(m_hDlg,IDWHISPER);
			EnableWindow(hButton,FALSE);
		}

		cltMsg clMsg(GAMEMSG_REQUEST_GETGONGZOINFO,0,NULL);
		pclclient->SendMsgToServer((char*)&clMsg);
	}
	else
	{
		DestroyWindow(m_hDlg);
	}
}

void CGongzoDlg::Show(CMinisterBase *pMini,SI32 pricetime,SI16 leftovervalue,GongzoMember *gongzomember)
{
	cltClient *pclclient = (cltClient*)pclClient;

	m_bSetSpin = FALSE ;
	m_bMonthOver = TRUE ;

	if ( m_bSet )
	{
		m_bSet = FALSE ;

		char* pTitle = GetTxtFromMgr(2045);
		char* pText = GetTxtFromMgr(2046);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return ;
	}

	pclClient->PushEffect(EFFECT_GONGZO,0,0);

	char buffer[256]="";

	strcpy(m_szGongzoID,pMini->m_szAccountID);
	strcpy(m_szGongzoName,pMini->m_szPersonName);
	strcpy(m_szGongzoProfile,pMini->m_szGreeting);

	m_siPriceTime = pricetime ;
	m_siLeftOverValue = leftovervalue ;

	char* pText = GetTxtFromMgr(2050);
	wsprintf(buffer, pText,m_szGongzoName);
	SetWindowText( m_hDlg ,buffer);
	SetDlgItemText( m_hDlg,IDC_EDIT_GONGZO, m_szGongzoProfile );
	pText = GetTxtFromMgr(2048);
	strcat(buffer, pText);
	SetWindowText(m_hGroup,buffer);

	for ( int i = GONGZO_SKILL_START ; i < GONGZO_SKILL_TOTAL ; i ++ )
	{
		m_stGongzoMember.siPriceValue[i] = gongzomember->siPriceValue[i] ;
		m_stGongzoMember.siSkillValue[i] = gongzomember->siSkillValue[i] ;
	}

	// 날짜를 구한다.
	cltDate cldate;
	cldate.MakeFullDateFromDateVary(pricetime);
	if(cldate.GetDateText(buffer) == false)
	{
		return ;
	}

	if ( pclclient->clClientDate.GetDateVary() <= m_siPriceTime + 30 ||
		pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank != GRANK_MINISTER6 )
		m_bMonthOver = FALSE ;

	SetSpinDis(m_bMonthOver);

	SetSpinButton();

	SetDlgItemText(m_hDlg,IDC_STATIC_GONGZODAY,buffer);

	HWND hWnd = NULL ;
	char ProfileURL[MAX_PATH];

/*	pclClient->GetCZProfileUrl( ProfileURL, m_szGongzoID );

	hWnd = GetDlgItem( m_hDlg, IDC_STATIC_PROFILE );
	if(hWnd) DisplayHTMLPage(hWnd, ProfileURL);*/

	// INTRO
	pclClient->GetCZIntroUrl( ProfileURL, m_szGongzoID );
	
	hWnd = GetDlgItem(m_hDlg, IDC_STATIC_GONGZOINTRO);
	if(hWnd) DisplayHTMLPage(hWnd, ProfileURL);

	ShowWindow( m_hDlg, SW_SHOW );
}

void CGongzoDlg::SetSpinButton()
{
	SetSpinData(GONGZO_SKILL_SWORD,IDC_SPIN_GONGZO_SWORD,IDC_EDIT_SWORD,IDC_STATIC_SWORD);
	SetSpinData(GONGZO_SKILL_SPEAR,IDC_SPIN_GONGZO_SPEAR,IDC_EDIT_SPEAR,IDC_STATIC_SPEAR);
	SetSpinData(GONGZO_SKILL_STAFF,IDC_SPIN_GONGZO_STAFF,IDC_EDIT_STAFF,IDC_STATIC_STAFF);
	SetSpinData(GONGZO_SKILL_AXE,IDC_SPIN_GONGZO_AXE,IDC_EDIT_AXE,IDC_STATIC_AXE);
	SetSpinData(GONGZO_SKILL_BOW,IDC_SPIN_GONGZO_BOW,IDC_EDIT_BOW,IDC_STATIC_BOW);
	SetSpinData(GONGZO_SKILL_GUN,IDC_SPIN_GONGZO_GUN,IDC_EDIT_GUN,IDC_STATIC_GUN);

	char buffer[16] = "" ;
	char* pText = GetTxtFromMgr(2052);
	sprintf(buffer, pText,m_siLeftOverValue);
	SetDlgItemText(m_hDlg, IDC_STATIC_LEFTOVERVALUE,buffer);
}

BOOL CGongzoDlg::SendGongzoMessage()
{
	cltClient *pclclient = (cltClient *)pclClient;
	char buffer[16] = "" ;

	SetSpinDis(m_bMonthOver);

	HWND hWnd = GetDlgItem( m_hDlg, m_siIDCEditNum );
	GetWindowText( hWnd, buffer, 8 );
	SI16 price = m_siLeftOverValue + m_stGongzoMember.siPriceValue[m_siSetSkill] - atoi(buffer);

	if ( m_siLeftOverValue == price ) return FALSE ;
	else m_siLeftOverValue = price ;

	m_stGongzoMember.siPriceValue[m_siSetSkill] = atoi(buffer);

	if ( m_siLeftOverValue < 0 )
	{
		char* pTitle = GetTxtFromMgr(2053);
		char* pText = GetTxtFromMgr(2054);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return FALSE ;
	}

	char* pText = GetTxtFromMgr(2052);
	sprintf(buffer, pText,m_siLeftOverValue);
	SetDlgItemText(m_hDlg, IDC_STATIC_LEFTOVERVALUE,buffer);

	GetDlgItemText(m_hDlg,IDC_EDIT_GONGZO,m_szGongzoProfile,256);
	SI32 limit = strlen(m_szGongzoProfile);
	if ( limit <= 0 || limit > 255 )
	{
		char* pTitle = GetTxtFromMgr(2019);
		char* pText = GetTxtFromMgr(2024);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		return FALSE ;
	}

	char* pTitle = GetTxtFromMgr(2055);
	pText = GetTxtFromMgr(2056);

	if (  MessageBox(NULL, pTitle, pText, MB_ICONQUESTION | MB_YESNO) == IDYES )
	{
		cltGameMsgRequest_SetGongzoInfo clinfo(m_szGongzoProfile,m_siLeftOverValue,m_stGongzoMember.siPriceValue);
		cltMsg clMsg(GAMEMSG_REQUEST_SETGONGZOINFO, sizeof(clinfo), (char*)&clinfo);
		pclClient->SendMsgToServer((char*)&clMsg);

		m_bSet = TRUE ;
	}
	else return FALSE ;

	return TRUE ;
}

void CGongzoDlg::SetSpinDis(BOOL bView)
{
	HWND hButton = GetDlgItem(m_hDlg,IDC_SPIN_GONGZO_SWORD);
	EnableWindow(hButton,bView);
	hButton = GetDlgItem(m_hDlg,IDC_SPIN_GONGZO_SPEAR);
	EnableWindow(hButton,bView);
	hButton = GetDlgItem(m_hDlg,IDC_SPIN_GONGZO_STAFF);
	EnableWindow(hButton,bView);
	hButton = GetDlgItem(m_hDlg,IDC_SPIN_GONGZO_AXE);
	EnableWindow(hButton,bView);
	hButton = GetDlgItem(m_hDlg,IDC_SPIN_GONGZO_BOW);
	EnableWindow(hButton,bView);
	hButton = GetDlgItem(m_hDlg,IDC_SPIN_GONGZO_GUN);
	EnableWindow(hButton,bView);

	if ( m_bSetSpin ) m_bSetSpin = FALSE ;
}

void CGongzoDlg::SetSpinData(SI32 skillunique,SI32 spinunique,SI32 editunique,SI32 staticunique)
{
	HWND hspin = NULL ;
	HWND hedit = NULL ;
	char buffer[16] = "" ;

	hspin = GetDlgItem(m_hDlg, spinunique);
	hedit = GetDlgItem(m_hDlg, editunique);

	SendMessage(hspin, UDM_SETBUDDY, (WPARAM)hedit, 0);
	if ( m_stGongzoMember.siPriceValue[skillunique] <= 0 )
		SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(m_stGongzoMember.siPriceValue[skillunique]/10+1, 0));
	else if ( m_stGongzoMember.siPriceValue[skillunique] >= 50 )
		SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(5, m_stGongzoMember.siPriceValue[skillunique]/10-1));
	else
		SendMessage(hspin, UDM_SETRANGE, 0, MAKELPARAM(m_stGongzoMember.siPriceValue[skillunique]/10+1, m_stGongzoMember.siPriceValue[skillunique]/10-1));

	wsprintf(buffer, "%d", m_stGongzoMember.siPriceValue[skillunique] );
	SetDlgItemText(m_hDlg, editunique, buffer);
	sprintf(buffer, "%d", m_stGongzoMember.siSkillValue[skillunique] );
	SetDlgItemText(m_hDlg, staticunique, buffer);
}

void CGongzoDlg::SetSpinValue(SI32 spinunique,SI32 skillunique,SI32 editunique)
{
	HWND hwnd = NULL ;
	char buffer[8] = "" ;
	SI16 price = 0 ;

	m_bSetSpin = TRUE ;
	m_siSetSkill = skillunique;
	m_siIDCEditNum = editunique;
	m_siSpinNum = spinunique;
}