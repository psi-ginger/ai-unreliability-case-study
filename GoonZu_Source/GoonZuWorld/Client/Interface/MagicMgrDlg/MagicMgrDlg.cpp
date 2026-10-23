#include "MagicMgrDlg.h"

CMagicMgrDlg g_MagicMgrDlg;

#include "..\..\..\CommonLogic\MagicMgr\MagicMgr.h"
#include "..\..\..\CommonLogic\MagicMgr\MagicAttribute\MagicAttribute.h"

#include "..\..\..\Common\Skill\Skill-Manager.h"
#include "Char\CharManager\CharManager.h"

#include "../../Lib/Sound/SoundMgr.h"
#include "../../Client/Music/music.h"

#include "../../Client/client.h"
#include "../../CommonLogic/MsgType-Person.h"


extern cltCommonLogic* pclClient;

CMagicMgrDlg::CMagicMgrDlg()
{
	m_hDlg = NULL;
	m_bShow = false;
	m_CurrentTabIndex = 0;
	m_hButton = NULL ;
	m_hBitMap = NULL ;
	m_hEditBrush = NULL;
	m_dwLastChangedMagic = 0;
	
	memset(m_szBitmapStr, 0, sizeof(m_szBitmapStr));
	memset(m_szOldBitmapStr, 0, sizeof(m_szOldBitmapStr));
	

	GP.LoadSpr("./Interface/MagicMgrDlg/MagicIcon.SPR", &m_MagicIcon );
}

CMagicMgrDlg::~CMagicMgrDlg()
{
	if ( m_MagicIcon.Image )
	{
		GP.FreeSpr( m_MagicIcon );
	}

	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

// 화면에 현재 선정된 MAGIC ICON을 그려준다.
void CMagicMgrDlg::DrawMagicIcon(LPDIRECTDRAWSURFACE7 lpsurface,SI32 x , SI32 y )
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 siScreenDrawX = m_siDrawX + x ;
	SI32 siScreenDrawY = pclclient->siScreenMode800X600YSize + pclclient->siScreenModeYOffSet - m_siDrawY - m_MagicIcon.clHeader.siYsize + y ;

	//===============================================================
	// 현재 선정된 마법이 적절한 그림을 표시될수 있는지를 검사한다.
	//===============================================================
	SI32 CurrentMagic = pclClient->pclCM->CR[1]->GetCurrentMagic();
	if ( CurrentMagic < MAGIC_INDEX_START ) return ;
	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	SI32 Group = pMagicAttributePtr->GetMagicGroup(CurrentMagic);
	if ( Group <= 0 ) return ;
	if ( Group >= m_MagicIcon.clHeader.siFontNum ) return ;
	
	//===============================================================
	// 현재 마법을 사용할수 있는 상태인지를 검사한다. 
	// 지금은  CLIENT에서는 알수가 없다.
	//===============================================================
	SI32 returnval = 0;
	if(pclClient->pclCM->CR[1]->CanMagicManaDelay(pclClient->pclCM->CR[1]->GetCurrentMagic(), &returnval) == true)
	{
		if( GP.LockSurface(lpsurface) == TRUE )
		{
			m_MagicIcon.PutSprScreenT( siScreenDrawX, siScreenDrawY, Group );
			GP.UnlockSurface(lpsurface);
		}
	}
	else
	{
		//===============================================================
		// 사용할수 없는 이유에 따라 화면에 표시를 다르게 한다. (현재는 같음)
		//===============================================================
		switch(returnval)
		{
		case CANUSEMAGIC_ERROR_NEEDMANA:
		case CANUSEMAGIC_ERROR_NEEDTIMEDELAY:
		case CANUSEMAGIC_ERROR_NOTENOUGHSKILL:
			{
				if( GP.LockSurface(lpsurface) == TRUE )
				{
					m_MagicIcon.PutSprMultiplyT( siScreenDrawX, siScreenDrawY, Group );
					GP.UnlockSurface(lpsurface);
				}
			}
			break;

		}
	}
}

void CMagicMgrDlg::Create( HINSTANCE hInst, HWND hWnd , SI32 x , SI32 y )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_MAGICMGR), hWnd, StaticMagicMgrDlgProc );
	m_siDrawX = x;
	m_siDrawY = y ;
	Hide();

	return;
}

BOOL CALLBACK CMagicMgrDlg::StaticMagicMgrDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_MagicMgrDlg.MagicMgrDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CMagicMgrDlg::MagicMgrDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hDlg = hDlg;
			// Tab Control의 Handle
			m_hTabCtrl = GetDlgItem( hDlg, IDC_TAB_MAGICMGR );
			//---------------------------------------------------
			// 텝 컨트롤의 탭 부분 기본 설정
			TC_ITEM item;
			// to use an image set this to TCIF_IMAGE  
			item.mask= TCIF_TEXT; 
			// index into tabs image list, set to -1 when not in use
			item.iImage = -1;

			char* pText = GetTxtFromMgr(2366);
			item.lParam = 0;	item.pszText = pText;
			TabCtrl_InsertItem( m_hTabCtrl, 0, &item );
			pText = GetTxtFromMgr(2368);
			item.lParam = 1;	item.pszText = pText;
			TabCtrl_InsertItem( m_hTabCtrl, 1, &item );
			pText = GetTxtFromMgr(2370);
			item.lParam = 2;	item.pszText = pText;
			TabCtrl_InsertItem( m_hTabCtrl, 2, &item );
			pText = GetTxtFromMgr(2376);
			item.lParam = 3;	item.pszText = pText;
			TabCtrl_InsertItem( m_hTabCtrl, 3, &item );
			pText = GetTxtFromMgr(2372);
			item.lParam = 4;	item.pszText = pText;
			TabCtrl_InsertItem( m_hTabCtrl, 4, &item );
			pText = GetTxtFromMgr(2374);
			item.lParam = 5;	item.pszText = pText;
			TabCtrl_InsertItem( m_hTabCtrl, 5, &item );
			//--------------------------------------------------
			// 맨 끝에 탭 안에 내용물 다이얼로그 생성
			m_hMagicTabDlg = CreateDialog( pclClient->GetInstance(), MAKEINTRESOURCE(IDD_DIALOG_MAGICTABDLG), hDlg, StaticMagicTabDlgProc );

			// Tab Control의 크기안에 DialogBox 크기를 맞춘다.
			RECT rect;
			GetClientRect( m_hTabCtrl, &rect);
			TabCtrl_AdjustRect( m_hTabCtrl, FALSE, &rect);
			
			MoveWindow( m_hMagicTabDlg,
				rect.left, rect.top, rect.right-rect.left,
				rect.bottom-rect.top, TRUE);
						
			ShowWindow( m_hMagicTabDlg, SW_SHOW );

			m_hButton = GetDlgItem(hDlg,IDOK);
			EnableWindow(m_hButton,FALSE);
			
			m_CurrentTabIndex = 0;

			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE );

/*			m_hBitmapBinWind = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/MagicMgrDlg/BigWind.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hBitmapFireBall = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/MagicMgrDlg/FireBall.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hBitmapFlame = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/MagicMgrDlg/Flame.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hBitmapThunder = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/MagicMgrDlg/Thunder.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hBitmapIce = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/MagicMgrDlg/Ice.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hBitmapPoison = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/MagicMgrDlg/Poison.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			m_hBitmapWhirlpool = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/MagicMgrDlg/Whirlpool.BMP", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );*/
		}
		return TRUE;

	case WM_TIMER:
		{
			SI16 MagicKind= GetMagicKind();

			if ( MagicKind < MAGIC_INDEX_START )
			{
				if ( IsWindowEnabled(m_hButton) )
					EnableWindow(m_hButton,FALSE);
				return FALSE;
			}

			SI32 siSkillUnique = 0;
			switch(m_CurrentTabIndex)
			{
			case MAGIC_TYPE_SWORD:	siSkillUnique = SKILL_USESWORD1	;	break;// 검술
			case MAGIC_TYPE_SPEAR:	siSkillUnique = SKILL_USESPEAR1	;	break;// 창술
			case MAGIC_TYPE_BOW:	siSkillUnique = SKILL_USEBOW1	;	break;// 궁술
			case MAGIC_TYPE_AXE:	siSkillUnique = SKILL_USEAXE1	;	break;// 부술(도끼술)
			case MAGIC_TYPE_GUN:	siSkillUnique = SKILL_USEGUN1	;	break;// 총술
			case MAGIC_TYPE_STAFF:	siSkillUnique = SKILL_USESTAFF1	;	break;// 지팡이술
			case MAGIC_TYPE_MON:	siSkillUnique = 0;					break;
			default :				return FALSE;
			}

			BOOL bWeaponTypeOK = FALSE;
			// 현재 장착된 무기의 Type에 따른 설정가능성을 타진한다.
			switch(pclClient->pclCM->CR[1]->GetAttackType())
			{
			case ATTACKTYPE_SWORD:		if(m_CurrentTabIndex == MAGIC_TYPE_SWORD) bWeaponTypeOK = TRUE;	break;
			case ATTACKTYPE_SPEAR:		if(m_CurrentTabIndex == MAGIC_TYPE_SPEAR) bWeaponTypeOK = TRUE;	break;
			case ATTACKTYPE_AXE:		if(m_CurrentTabIndex == MAGIC_TYPE_AXE)	  bWeaponTypeOK = TRUE;	break;
			case ATTACKTYPE_STAFF:		if(m_CurrentTabIndex == MAGIC_TYPE_STAFF) bWeaponTypeOK = TRUE;	break;
			case ATTACKTYPE_STAFF1:		if(m_CurrentTabIndex == MAGIC_TYPE_STAFF) bWeaponTypeOK = TRUE;	break;
			case ATTACKTYPE_STAFF2:		if(m_CurrentTabIndex == MAGIC_TYPE_STAFF) bWeaponTypeOK = TRUE;	break;
			case ATTACKTYPE_NEW_STAFF2:		if(m_CurrentTabIndex == MAGIC_TYPE_STAFF) bWeaponTypeOK = TRUE;	break;
			case ATTACKTYPE_BOW:		if(m_CurrentTabIndex == MAGIC_TYPE_BOW)	  bWeaponTypeOK = TRUE;	break;
			case ATTACKTYPE_BOW1:		if(m_CurrentTabIndex == MAGIC_TYPE_BOW)	  bWeaponTypeOK = TRUE;	break;
			case ATTACKTYPE_NEW_BOW1:		if(m_CurrentTabIndex == MAGIC_TYPE_BOW)	  bWeaponTypeOK = TRUE;	break;
			case ATTACKTYPE_GUN:		if(m_CurrentTabIndex == MAGIC_TYPE_GUN)	  bWeaponTypeOK = TRUE;	break;
			case ATTACKTYPE_GUN1:		if(m_CurrentTabIndex == MAGIC_TYPE_GUN)	  bWeaponTypeOK = TRUE;	break;
			case ATTACKTYPE_NEW_GUN1:		if(m_CurrentTabIndex == MAGIC_TYPE_GUN)	  bWeaponTypeOK = TRUE;	break;
			default:				bWeaponTypeOK = FALSE;	break;
			}

			// 현재 장착된 무기의 Type에 따른 설정가능성을 타진한다.
			if( bWeaponTypeOK )
			{
				SI32 SkillLevel = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0]);

				if ( SkillLevel > 0 )
				{
					stMagicAttributeData *pMagicAttributeData = pclClient->pclMagicManager->GetMagicAttribute( MagicKind );
					
					if ( SkillLevel >= pMagicAttributeData->si16MagicSkillLevel )
						EnableWindow(m_hButton,TRUE);
					else EnableWindow(m_hButton,FALSE);
				}
				else EnableWindow(m_hButton,FALSE);
			}
			else EnableWindow(m_hButton,FALSE);
			/*
			if ( IsChanged() )
			{
			}
			*/
		}
		break;

	case WM_NOTIFY:
		{
			LPNMHDR lpNmhdr =(LPNMHDR)lParam;

			switch ( lpNmhdr->idFrom )
			{
			case IDC_TAB_MAGICMGR:
				{
					if ( lpNmhdr->code == TCN_SELCHANGE )
					{
						int index = TabCtrl_GetCurSel( GetDlgItem ( hDlg, IDC_TAB_MAGICMGR ) );

						if ( index < MAGIC_TYPE_START || index > MAGIC_TYPE_LAST )
						{
							return FALSE;
						}


						m_CurrentTabIndex = index;
						ShowTabDialog(m_CurrentTabIndex);
					}
				}
				break;
			}
		}
		break;
	case WM_COMMAND:
		{
			cltClient *pclclient = (cltClient *)pclClient;

			switch ( LOWORD(wParam) )
			{			
			// 마법 적용
			case IDOK:
				{
					return SetNewMagic(-1);
				}
				break;
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
			if ( m_hBitMap )
			{
				DeleteObject(m_hBitMap);
				m_hBitMap = NULL ;
			}
			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}

			m_hButton = NULL;
			m_hDlg = NULL;
		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK CMagicMgrDlg::StaticMagicTabDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_MagicMgrDlg.MagicTabDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CMagicMgrDlg::MagicTabDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hMagicList		= GetDlgItem( hDlg, IDC_LIST_MAGIC );
			if(m_hMagicList)
			{
				ListView_SetExtendedListViewStyle( m_hMagicList , LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
				char* pText = GetTxtFromMgr(2411);
				ListViewInsertColumn( m_hMagicList, 130, pText, 0, 0);		
				pText = GetTxtFromMgr(2412);
				ListViewInsertColumn( m_hMagicList,  60, pText, 1, 1);		
			}

			m_hMagicExplainEdit = GetDlgItem( hDlg, IDC_EDIT_MAGIC_EXPLAIN );
			m_hMagicExplainList = GetDlgItem( hDlg, IDC_LIST_MAGIC_EXPLAIN );
			if(m_hMagicExplainList)
			{
				ListView_SetExtendedListViewStyle( m_hMagicExplainList , LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
				char* pText = GetTxtFromMgr(2413);
				ListViewInsertColumn( m_hMagicExplainList, 170, pText, 0, 0);		
				pText = GetTxtFromMgr(1226);
				ListViewInsertColumn( m_hMagicExplainList, 120, pText, 1, 1);		
			}

			ShowTabDialog(m_CurrentTabIndex);
			
			// 타이머를 설정한다. 
			SetTimer(hDlg, 1, 10000, NULL);
		}
		return TRUE;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_MAGIC_EXPLAIN ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE );
				
				return (BOOL)m_hEditBrush;
			}
		}
		break;	
		
	case WM_TIMER:
		{
			SetExplain(); 
			SetMagicBitmap(hDlg);
		}
		break;
	case WM_NOTIFY:
		{
			LPNMHDR						hdr;
			LPNMITEMACTIVATE			nia;
			LPNMLVGETINFOTIP			ngt;
			LPNMLISTVIEW				nlv;
			hdr = ( LPNMHDR )			lParam;
			ngt = ( LPNMLVGETINFOTIP )	lParam;
			nlv = ( LPNMLISTVIEW )		lParam;
			nia = (LPNMITEMACTIVATE)	lParam;
			

			if ( hdr->hwndFrom == GetDlgItem( hDlg, IDC_LIST_MAGIC) )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if(nlv->uChanged == LVIF_STATE && nlv->uNewState == (LVIS_SELECTED | LVIS_FOCUSED))
						{
							SetExplain(); 
							SetMagicBitmap(hDlg);
						}
					}
					break;
				}
			}
		}
		break;
	case WM_DESTROY:
		{
			// 타이머를 해제한다. 
			KillTimer(hDlg, 1);
		}
		break;

	}

	return FALSE;
}

void CMagicMgrDlg::ShowTabDialog(int index)
{			
	// ListView를 초기화한다.
	ListView_DeleteAllItems( m_hMagicList );
	
	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	
	SI32 Start, End;
	BOOL bSuccess = pMagicAttributePtr->GetMagicStartEnd(index, Start, End);
	if(bSuccess == FALSE) return;

	SI32 siCount = 0;
	for (SI32 i = Start; i<= End; ++i )
	{
		stMagicAttributeData *pMagicAttributeData = pMagicAttributePtr->GetMagicAttribute(i);
		
		// 유저가 사용 가능한 마법을 보여줌
		if ( pMagicAttributeData->si16MagicUserUsing == 1)
		{
			AddMagicList( pMagicAttributeData->strMagicName, siCount );
			siCount++;
		}		
	}	
}

BOOL CMagicMgrDlg::SetNewMagic(int index)
{
	cltClient *pclclient = (cltClient *)pclClient;

	// 적어도 0.25 초 이하로 바꾸지 않도록 한다.
	if(TABS(pclclient->CurrentClock - m_dwLastChangedMagic) < 250) return FALSE;
	m_dwLastChangedMagic = pclclient->CurrentClock;
	
	SI16 MagicKind    = 0;

	if(index<0)		{	MagicKind = GetMagicKind();		}
	else			{	MagicKind = GetMagicKind(index);}
	
	SI32 CurrentMagic = pclClient->pclCM->CR[1]->GetCurrentMagic();
				
	if ( MagicKind == CurrentMagic )	return FALSE ;
	if ( MagicKind < MAGIC_INDEX_START )
	{
		if (m_hButton &&  IsWindowEnabled(m_hButton) )	EnableWindow(m_hButton,FALSE);
		return FALSE;
	}
				
	char Explain[ 128 ] ="";
	CMagicAttribute *pMagicAttributePtr			= pclClient->pclMagicManager->GetMagicAttributePtr();
	stMagicAttributeData *pMagicAttributeData	= pclClient->pclMagicManager->GetMagicAttribute( MagicKind );
	SI32 SkillLevel = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( pMagicAttributePtr->GetMagicSkillType(MagicKind), 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0]);
	if ( SkillLevel > 0 )
	{
		if ( SkillLevel >= pMagicAttributeData->si16MagicSkillLevel )
		{
			if(pMagicAttributeData->b16MagicActive)
			{
				char* pText = GetTxtFromMgr(2414);
				sprintf( Explain, pText, pMagicAttributeData->strMagicName );
			}
			else
			{
				char* pText = GetTxtFromMgr(2415);
				sprintf( Explain, pText, pMagicAttributeData->strMagicName );
			}
			if(m_hMagicExplainEdit)	SetWindowText( m_hMagicExplainEdit, Explain );
			//========================================
			// JAYCEE - 서버에서 받아야한다.
			//pclClient->pclCM->CR[1]->SetCurrentMagic( MagicKind, pMagicAttributeData->b16MagicActive );
			//========================================
			// 서버로 현재의 MagicKind를 날려줘야함 
			cltMsg clMsg(GAMEMSG_REQUEST_CHANGEMAGIC, MagicKind);
			pclclient->SendMsgToServer((char*)&clMsg);

		}
	}
	else
	{
		char* pText = GetTxtFromMgr(2416);
		sprintf( Explain, pText, pMagicAttributeData->strMagicName );
		if(m_hMagicExplainEdit)	SetWindowText( m_hMagicExplainEdit, Explain );
	}

	return TRUE;
}

bool CMagicMgrDlg::IsShow()
{
	return m_bShow;
}


// TOOL BAR에서 버튼을 눌려 화면에 띄울때 사용
void CMagicMgrDlg::Show()
{
	if ( m_bShow )
	{
		Hide();
		return;
	}
	
	SetTimer( m_hDlg, 0, MAGICMGRDLG_SETTIMER_TIME, NULL );
	
	// 마지막까지 설정되어져 있던 컨트롤을 보여준다.
	ShowTabDialog(m_CurrentTabIndex);
	
	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );
	
	return;
}

void CMagicMgrDlg::Hide()
{
	KillTimer( m_hDlg, 0 );

	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

// 마법 기술레벨이 변경 되었는지 안 되었는지만 체크하면 될듯.
bool CMagicMgrDlg::IsChanged()
{
	return false;
}

void CMagicMgrDlg::Clear()
{
	// ListView를 초기화한다.
	ListView_DeleteAllItems( m_hMagicList );
	return;
}

void CMagicMgrDlg::AddMagicList( char *MagicName, SI32 index )
{
	if ( MagicName == NULL || strlen( MagicName ) < 1 )
	{
		ListViewInsertItem(m_hMagicList, -1 , 0, 0, "", -1);
		return;
	}
	else
	{
		ListViewInsertItem(m_hMagicList, -1 , index, 0, MagicName, -1);
		char buffer[256]="";
		sprintf(buffer, "CTRL+%d", index+1);
		ListViewInsertItem(m_hMagicList, -1 , index, 1, buffer, -1);
	}
	return;
}

void CMagicMgrDlg::SetExplain()
{
	//=======================================================
	// MagicDialog의 설정된 마법이 Valid한지 검사한다.
	//=======================================================
	SI16 SelectedIndex = ListView_GetNextItem(m_hMagicList, -1, LVNI_ALL | LVNI_SELECTED);
	if ( SelectedIndex < 0 ){	return;	}

	char Buffer[256 ] = "";
	ListView_GetItemText(m_hMagicList, SelectedIndex, 0, Buffer, 64);

	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	SI16 MagicKind = pMagicAttributePtr->FindRefFromName(Buffer, m_CurrentTabIndex);

	// ListView, BMP 설정을 한다.
	if(m_hMagicExplainList== NULL) return;
	strcpy(m_szBitmapStr,"");
	strcpy(m_szBitmapStr,pMagicAttributePtr->GetMagicKindString(MagicKind));
	
	if(MagicKind < 0) return;
	stMagicAttributeData *pMagicAttributeData = pMagicAttributePtr->GetMagicAttribute(MagicKind);

	SI32 siSkillUnique = 0;
	switch(m_CurrentTabIndex)
	{
	case MAGIC_TYPE_SWORD:	siSkillUnique = SKILL_USESWORD1	;	break;// 검술
	case MAGIC_TYPE_SPEAR:	siSkillUnique = SKILL_USESPEAR1	;	break;// 창술
	case MAGIC_TYPE_BOW:	siSkillUnique = SKILL_USEBOW1	;	break;// 궁술
	case MAGIC_TYPE_AXE:	siSkillUnique = SKILL_USEAXE1	;	break;// 부술(도끼술)
	case MAGIC_TYPE_GUN:	siSkillUnique = SKILL_USEGUN1	;	break;// 총술
	case MAGIC_TYPE_STAFF:	siSkillUnique = SKILL_USESTAFF1	;	break;// 지팡이술
	case MAGIC_TYPE_MON:	siSkillUnique = 0;					break;
	default :				return;
	}


	// ListView를 초기화한다.
	ListView_DeleteAllItems( m_hMagicExplainList );

	SI32 SkillLevel = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0]);
	SI32 AddDamage  = pclClient->pclCM->CR[1]->clPB.GetAddMagicDamage();
	SI32 siOrganUnique = pclClient->pclCM->CR[1]->pclCI->clHealthInfo.GetOrg(pMagicAttributeData->si16MagicOrganKind);
	SI32 CharLevel  = pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel();

	char explain[1024] ="";
	char skillname[128];
	
	
	if(pclClient->pclSkillManager->GetSkillName(siSkillUnique, skillname) == true)
	{
		if ( SkillLevel >= pMagicAttributeData->si16MagicSkillLevel )
		{
			// Skill Level에 의한 영향력을 주고 싶으면 영향력을 넣어 마나소비량을 계산한다.
			SI32 mana = 0, Min = 0, Max = 0;
			SI32 SkillMin= 0, SkillMax = 0;
			SI32 HealthMin= 0, HealthMax= 0;
			mana = pclClient->pclMagicManager->GetSpendMana(MagicKind, SkillLevel);
			stMagicInfo clMagicInfo(MagicKind, 0, 0, 0);
			pclClient->pclMagicManager->GetMinMaxDamage(&clMagicInfo, AddDamage, SkillLevel, Min, Max, siOrganUnique);
			pclClient->pclMagicManager->GetMinMaxSkillDamage(MagicKind, SkillLevel, SkillMin, SkillMax);
			HealthMin= Min-SkillMin-AddDamage-pMagicAttributeData->si32MagicMinDamage;
			HealthMax= Max-SkillMax-AddDamage-pMagicAttributeData->si32MagicMaxDamage;


			if (pMagicAttributeData->b16MagicActive )
			{

				SI32 siIndex = 0;
				char* pText = GetTxtFromMgr(2417);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				pText = GetTxtFromMgr(2418);
				sprintf(explain, pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
				ListViewInsertItem(m_hMagicExplainList, -1,  siIndex, 1, explain,	-1);
				siIndex++;

				pText = GetTxtFromMgr(2419);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				pText = GetTxtFromMgr(2420);
				sprintf(explain, pText, pMagicAttributeData->si32MagicDelay/20.0f); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;

				pText = GetTxtFromMgr(2421);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				pText = GetTxtFromMgr(2422);
				sprintf(explain, pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;
				
				pText = GetTxtFromMgr(2423);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				pText = GetTxtFromMgr(2424);
				sprintf(explain, pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;

				pText = GetTxtFromMgr(2425);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				sprintf(explain, "%d ~ %d", Min, Max); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;

				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, "", -1);
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, "", -1);
				siIndex++;

				pText = GetTxtFromMgr(2426);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, "", -1);
				siIndex++;

				pText = GetTxtFromMgr(2427);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				sprintf(explain, "%d ~ %d", pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;

				pText = GetTxtFromMgr(2428);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				sprintf(explain, "%d", AddDamage); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;
				
				pText = GetTxtFromMgr(2429);
				sprintf(explain,pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				sprintf(explain, "%d ~ %d", SkillMin, SkillMax); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;

				pText = GetTxtFromMgr(2430);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				sprintf(explain, "%d ~ %d", HealthMin, HealthMax); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;

			}
			else
			{
				if ( pMagicAttributePtr->GetMagicGroup(MagicKind) == MAGIC_GROUP_DRAIN )
				{

					SI32 siIndex = 0;
					char* pText = GetTxtFromMgr(2417);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2418);
					sprintf(explain, pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
					ListViewInsertItem(m_hMagicExplainList, -1,  siIndex, 1, explain,	-1);
					siIndex++;
					
					pText = GetTxtFromMgr(2431);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					//sprintf(explain, "%2.1f초", pMagicAttributeData->si32MagicDelay/20.0f); 
					//ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;

					pText = GetTxtFromMgr(2421);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2422);
					sprintf(explain, pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2423);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2424);
					sprintf(explain, pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2432);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d%%~%d%%", min(30, Min), min(30, Max)); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, "", -1);
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, "", -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2433);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, "", -1);
					siIndex++;

					pText = GetTxtFromMgr(2434);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d ~ %d", pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2435);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d", AddDamage); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2436);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d ~ %d", SkillMin, SkillMax); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2437);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d ~ %d", HealthMin, HealthMax); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
				}
				else if ( pMagicAttributePtr->GetMagicGroup(MagicKind) == MAGIC_GROUP_CURSECRITICAL)
				{

					SI32 siIndex = 0;
					char* pText = GetTxtFromMgr(2417);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2418);
					sprintf(explain, pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
					ListViewInsertItem(m_hMagicExplainList, -1,  siIndex, 1, explain,	-1);
					siIndex++;
					
					pText = GetTxtFromMgr(2431);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					//sprintf(explain, "%2.1f초", pMagicAttributeData->si32MagicDelay/20.0f); 
					//ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;

					pText = GetTxtFromMgr(2421);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2422);
					sprintf(explain, pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2423);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2424);
					sprintf(explain, pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2438);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d%%~%d%%", min(80, Min), min(80, Max)); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, "", -1);
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, "", -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2439);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, "", -1);
					siIndex++;

					pText = GetTxtFromMgr(2440);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d ~ %d", pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2441);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d", AddDamage); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2442);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d ~ %d", SkillMin, SkillMax); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2443);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d ~ %d", HealthMin, HealthMax); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
				}

			}
		}
		else
		{
			SI32 mana = 0, Min = 0, Max = 0;
			SI32 SkillMin= 0, SkillMax = 0;
			SI32 HealthMin = 0,HealthMax = 0;
			
			if ( pMagicAttributeData->b16MagicActive )
			{
				SI32 siIndex = 0;
				char* pText = GetTxtFromMgr(2417);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				pText = GetTxtFromMgr(2418);
				sprintf(explain, pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
				ListViewInsertItem(m_hMagicExplainList, -1,  siIndex, 1, explain,	-1);
				siIndex++;

				pText = GetTxtFromMgr(2419);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				pText = GetTxtFromMgr(2420);
				sprintf(explain, pText, pMagicAttributeData->si32MagicDelay/20.0f); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;

				pText = GetTxtFromMgr(2421);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				pText = GetTxtFromMgr(2422);
				sprintf(explain, pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;
				
				pText = GetTxtFromMgr(2423);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				pText = GetTxtFromMgr(2424);
				sprintf(explain, pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;

				pText = GetTxtFromMgr(2425);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				sprintf(explain, "%d ~ %d", Min, Max); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;

				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, "", -1);
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, "", -1);
				siIndex++;
				
				pText = GetTxtFromMgr(2426);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				pText = GetTxtFromMgr(2444);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;

				pText = GetTxtFromMgr(2427);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				sprintf(explain, "%d ~ %d", pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;
				
				pText = GetTxtFromMgr(2428);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				sprintf(explain, "%d", AddDamage); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;
				
				pText = GetTxtFromMgr(2429);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				sprintf(explain, "%d ~ %d", SkillMin, SkillMax); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;
				
				pText = GetTxtFromMgr(2430);
				sprintf(explain, pText); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
				sprintf(explain, "%d ~ %d", HealthMin, HealthMax); 
				ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
				siIndex++;				

			}
			else
			{
				if ( pMagicAttributePtr->GetMagicGroup(MagicKind) == MAGIC_GROUP_DRAIN )
				{
					SI32 siIndex = 0;
					char* pText = GetTxtFromMgr(2417);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2418);
					sprintf(explain, pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
					ListViewInsertItem(m_hMagicExplainList, -1,  siIndex, 1, explain,	-1);
					siIndex++;
					
					pText = GetTxtFromMgr(2431);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					//sprintf(explain, "%2.1f초", pMagicAttributeData->si32MagicDelay/20.0f); 
					//ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2421);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2422);
					sprintf(explain, pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2423);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2424);
					sprintf(explain, pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2432);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d%%~%d%%", min(30, Min), min(30, Max)); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, "", -1);
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, "", -1);
					siIndex++;

					pText = GetTxtFromMgr(2433);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2444);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;

					pText = GetTxtFromMgr(2434);
					sprintf(explain,pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d ~ %d", pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2435);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d", AddDamage); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2436);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d ~ %d", SkillMin, SkillMax); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2437);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d ~ %d", HealthMin, HealthMax); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
				}
				else if ( pMagicAttributePtr->GetMagicGroup(MagicKind) == MAGIC_GROUP_CURSECRITICAL)
				{

					SI32 siIndex = 0;
					char* pText = GetTxtFromMgr(2417);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2418);
					sprintf(explain, pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
					ListViewInsertItem(m_hMagicExplainList, -1,  siIndex, 1, explain,	-1);
					siIndex++;
					
					pText = GetTxtFromMgr(2431);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					//sprintf(explain, "%2.1f초", pMagicAttributeData->si32MagicDelay/20.0f); 
					//ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;

					pText = GetTxtFromMgr(2421);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2422);
					sprintf(explain, pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2423);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2424);
					sprintf(explain, pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2438);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d%%~%d%%", min(80, Min), min(80, Max)); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, "", -1);
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, "", -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2439);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					pText = GetTxtFromMgr(2444);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;

					pText = GetTxtFromMgr(2440);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d ~ %d", pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2441);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d", AddDamage); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2442);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d ~ %d", SkillMin, SkillMax); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
					
					pText = GetTxtFromMgr(2443);
					sprintf(explain, pText); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 0, explain, -1);
					sprintf(explain, "%d ~ %d", HealthMin, HealthMax); 
					ListViewInsertItem(m_hMagicExplainList, -1 , siIndex, 1, explain, -1);
					siIndex++;
				}
				
			}
		}
	}

	
	//SetWindowText( m_hMagicExplainEdit, "" );
	
	return;
}

SI16 CMagicMgrDlg::GetMagicKind()
{
	SI16 SelectedIndex = ListView_GetNextItem(m_hMagicList, -1, LVNI_ALL | LVNI_SELECTED);
	if ( SelectedIndex < 0 )
	{
		return -1;
	}

	char Buffer[256 ] = "";
	ListView_GetItemText(m_hMagicList, SelectedIndex, 0, Buffer, 64);

	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();

	SI32 MagicKind = pMagicAttributePtr->FindRefFromName(Buffer, m_CurrentTabIndex);
	if(MagicKind > 0)
	{
		return MagicKind;
	}
	return -1;
}

SI16 CMagicMgrDlg::GetMagicKind( SI32 key)
{
	
	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	
	SI32 index = -1;
	// 현재 장착된 무기의 Type에 따른 설정가능성을 타진한다.
	switch(pclClient->pclCM->CR[1]->GetAttackType())
	{
		case ATTACKTYPE_SWORD:	index = MAGIC_TYPE_SWORD;	break;
		case ATTACKTYPE_SPEAR:	index = MAGIC_TYPE_SPEAR;	break;
		case ATTACKTYPE_AXE:	index = MAGIC_TYPE_AXE;		break;
		case ATTACKTYPE_STAFF:	index = MAGIC_TYPE_STAFF;	break;
		case ATTACKTYPE_STAFF1:	index = MAGIC_TYPE_STAFF;	break;
		case ATTACKTYPE_STAFF2:	index = MAGIC_TYPE_STAFF;	break;
		case ATTACKTYPE_NEW_STAFF2:	index = MAGIC_TYPE_STAFF;	break;
		case ATTACKTYPE_BOW:	index = MAGIC_TYPE_BOW;		break;
		case ATTACKTYPE_BOW1:	index = MAGIC_TYPE_BOW;		break;
		case ATTACKTYPE_NEW_BOW1:	index = MAGIC_TYPE_BOW;		break;
		case ATTACKTYPE_GUN:	index = MAGIC_TYPE_GUN;		break;
		case ATTACKTYPE_GUN1:	index = MAGIC_TYPE_GUN;		break;
		case ATTACKTYPE_NEW_GUN1:	index = MAGIC_TYPE_GUN;		break;
		default:				return -1;					break;
	}

	SI32 Start, End;
	BOOL bSuccess = pMagicAttributePtr->GetMagicStartEnd(index, Start, End);
	if(bSuccess == FALSE) return -1;

	SI32 siCount = 1;
	for (SI32 i = Start; i<= End; ++i )
	{
		stMagicAttributeData *pMagicAttributeData = pMagicAttributePtr->GetMagicAttribute(i);
		// 유저가 사용 가능한 마법주에서 선택한다.
		if ( pMagicAttributeData->si16MagicUserUsing == 1)
		{
			if(key == siCount)	return i;
			siCount++;
		}		
	}	

	return -1;
}

void CMagicMgrDlg::SetMagicBitmap(HWND hDlg)
{
	if(strcmp(m_szOldBitmapStr, m_szBitmapStr) ==0) return;

	// Old 값으로 변경한다.  같은 그림이라면 갱신하지 않는다.
	strcpy(m_szOldBitmapStr, m_szBitmapStr);

	char buffer[256] = "" ;
	strtok(m_szBitmapStr,"_");
	strtok(NULL,"_");
	const char * data = strtok(NULL,"_") ;
	wsprintf(buffer,"Interface/MagicMgrDlg/%s.bmp",data);

	if ( m_hBitMap )
	{
		DeleteObject(m_hBitMap);
		m_hBitMap = NULL ;
	}

	m_hBitMap = (HBITMAP)LoadImage( pclClient->GetInstance(), buffer, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	SendDlgItemMessage( hDlg, IDC_STATIC_MAGIC_IMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hBitMap);
}

