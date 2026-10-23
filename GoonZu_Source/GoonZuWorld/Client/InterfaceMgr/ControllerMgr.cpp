#include "ControllerMgr.h"
#include "ControllerObj.h"

#include "../../Common/SMLib/LibList/LibList.h"
#include "../../Common/Char/CharCommon/Char-Common.h"
#include "../../Common/Char/CharManager/CharManager.h"
#include "../../Common/Map/Map.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "InterfaceMgr.h"

#include "ControllerSurfaceObj.h"

#include "Interface/Button.h"
#include "Interface/ComboBox.h"
#include "Interface/RadioButton.h"

#include "InterfaceImgMgr/InterfaceImgMgr.h"
#include "DLGPositionSave/DLGPositionSave.h"

#include <Client.h>
extern cltCommonLogic *pclClient;

#include <zmouse.h>


CControllerMgr::CControllerMgr() : CLibListMgr()
{
	m_pPre = NULL;
	m_pNext = NULL;

	m_siDialogControllerID = 0;
	m_siDialogControllerKind = CONTROLLER_KIND_POPUPDIALOG;

	for ( SI16 i = 0; i < MAX_HAVE_CHILD_DIALOG_NUM; ++i )
	{
		m_pChildDialog[ i ] = NULL;
	}

	m_pParentDialog = NULL;

	m_bHaveCaption = true;
	m_bHaveCloseButton = true;
	//m_bHaveMinimizeButton = false;

	memset( m_strFileName, 0, MAX_PATH );
	m_siFontIndex = 0;
	m_bBKTileImg = true;
	m_siBKTileNum = 9;

	m_siXPos = 0;
	m_siYPos = 0;
	m_siWidth = 0;
	m_siHeight = 0;

	m_bHasFocus = false;

	m_bShow = true;

	m_bMoveable = true;

	m_bKeyboardInput = false;
	m_bMouseInput = true;

	m_bHasTitle = false;
	memset( m_strTitle, 0, MAX_PATH );
	m_TitleEncBuffer = NULL;
	m_pTitleSurfaceObj = NULL;

	m_pFocusObj = NULL;
	m_pMouseOverObj = NULL;

	m_bTimerOnOff = false;
	m_siActionTimer = 1000;

	m_bHaveComboBox = false;

	m_bCreate = false;
	m_bTopMostWindow = false;
	m_bModalDialog = false;
	m_bShowHideMethod = false;

	m_siDrawOption = SPR_PUTSPRT;
	m_siAlphaValue = MAX_ALPHA_LEVEL - 1;

	m_siFadeInTime = 0;
	m_dwShowStartTime = 0;
}

CControllerMgr::~CControllerMgr()
{
	cltClient *pclclient = (cltClient *)pclClient;

	SAFE_DELETE_A( m_TitleEncBuffer );
	SAFE_DELETE( m_pTitleSurfaceObj );

	CControllerObj *pTemp = (CControllerObj *)GetExistList()->GetFirst();
	CControllerObj *pTemp1 = NULL;

#ifdef _DEBUG
	while ( pTemp )
	{
		pTemp1 = pTemp;
		pTemp = (CControllerObj *)pTemp->GetNext();

		MessageBox( NULL, TEXT("리소스 해제 하세용"), TEXT("리소스 해제 안됐음"), MB_OK );
		GetExistList()->Pop( pTemp1 );

		SAFE_DELETE( pTemp1 );
	}
#endif

	if ( GetDialogControllerKind() == CONTROLLER_KIND_POPUPDIALOG )
	{
		pclclient->m_pInterfaceMgr->Del( this );
	}
	else
	{
		m_pParentDialog->DelChildDialog( this );
	}
}

void CControllerMgr::Create( SI16 DialogControllerID, SI16 DialogControllerKind, CControllerMgr *pParentDialog,
							GUICallBackEvent pGUICallBackEvent,
							TCHAR *Title,
							TCHAR *FileName, SI16 FontIndex,
							bool BKTileImg, SI16 BKTileNum,
							SI16 XPos, SI16 YPos,
							SI16 Width, SI16 Height,
							bool Movable,
							bool HaveCaption, bool HaveCloseButton//,bool HaveMinimizeButton
							)
{	
	m_siDialogControllerID = DialogControllerID;
	m_siDialogControllerKind = DialogControllerKind;
	m_pParentDialog = pParentDialog;

	if ( pParentDialog )
	{
		if ( ! pParentDialog->AddChildDialog( this ) )
		{
			MsgBox( TEXT("Child Dialog Insert Error"), TEXT("Child Dialog Num Exceed") );
		}
	}

	m_pGUICallBackEvent = pGUICallBackEvent;

	m_siFontIndex = FontIndex;
	m_bBKTileImg = BKTileImg;
	m_siBKTileNum = BKTileNum;

	m_siXPos = XPos;
	m_siYPos = YPos;

	m_siWidth = Width;
	m_siHeight = Height;

	m_bHaveCaption = HaveCaption;
	m_bHaveCloseButton = HaveCloseButton;
	//m_bHaveMinimizeButton = HaveMinimizeButton;

	m_bMoveable = Movable;

	//_tcslen( FileName ) < 1 == (! FileName[ 0 ])
	if ( FileName == NULL || FileName[ 0 ] == NULL )
	{
		memset( m_strFileName, 0, MAX_PATH );
	}
	else
	{
		StringCchCopy( m_strFileName, MAX_PATH, FileName );
	}

	SetTitle( Title );

	m_bCreate = true;
	m_bTopMostWindow = false;

	m_dwShowStartTime = GetTickCount();

	return;
}

void CControllerMgr::DeleteDialog()
{
	cltClient *pclclient = (cltClient *)pclClient;
	g_DlgPositionSave.SetPosition( this->GetDialogControllerID(), this->GetX(), this->GetY() ); //자신위치를 메니져의 멤버변수에 저장
	pclclient->m_pInterfaceMgr->PushToDeleteDialog( this );
	return;
}

bool CControllerMgr::Add( CControllerObj *pObj )
{
	if ( pObj == NULL )
	{
		return false;
	}

	PushBackToExistList( pObj );

	if ( pObj->GetControllerKind() == CONTROLLER_KIND_COMBOBOX )
	{
		m_bHaveComboBox = true;
	}

	return true;
}
void CControllerMgr::Del( CControllerObj *pObj )
{
	CControllerObj *pTemp = (CControllerObj *)GetExistList()->GetFirst();
	CControllerObj *pTemp1 = NULL;

	while ( pTemp )
	{
		if ( pTemp == pObj )
		{
			GetExistList()->Pop( pObj );
			return;
		}

		pTemp = (CControllerObj *)pTemp->GetNext();
	}

	return;
}

void CControllerMgr::DelAll()
{	
	CControllerObj *pTemp = (CControllerObj *)GetExistList()->GetFirst();
	CControllerObj *pTemp1 = NULL;

	while ( pTemp )
	{
		pTemp1 = pTemp;
		pTemp = (CControllerObj *)pTemp->GetNext();

		GetExistList()->Pop( pTemp1 );
	}
}

bool CControllerMgr::AddChildDialog( CControllerMgr *pChild )
{
	for ( int i = 0; i < MAX_HAVE_CHILD_DIALOG_NUM; ++i )
	{
		if ( m_pChildDialog[ i ] == NULL )
		{
			m_pChildDialog[ i ] = pChild;
			return true;
		}
	}

	return false;
}

void CControllerMgr::DelChildDialog( CControllerMgr *pChild )
{
	if ( pChild )
	{
		return;
	}

	for ( int i = 0; i < MAX_HAVE_CHILD_DIALOG_NUM; ++i )
	{
		if ( m_pChildDialog[ i ] == pChild )
		{
			m_pChildDialog[ i ] = NULL;
			break;
		}
	}

	return;
}

CControllerObj *CControllerMgr::GetControl( SI16 ControllerID )
{
	CControllerObj *pObj = (CControllerObj *)GetExistList()->GetFirst();

	while( pObj )
	{
		if ( ControllerID == pObj->GetControllerID() )
		{
			return pObj;
		}

		pObj = (CControllerObj *)pObj->GetNext();
	}

	return NULL;
}

CControllerObj *CControllerMgr::GetControlAtPoint( POINT pt )
{

	CControllerObj *pObj = (CControllerObj *)GetExistList()->GetFirst();
	while( pObj )
	{
		if ( pObj->IsInArea( pt.x, pt.y ) )
		{
			return pObj;
		}

		pObj = (CControllerObj *)pObj->GetNext();
	}

	return NULL;
}

CControllerObj *CControllerMgr::GetControlAtPointFromInterfaceMgr( POINT pt )
{
	POINT TempPt;
	CControllerObj *pObj = NULL;

	pObj = (CControllerObj *)GetExistList()->GetFirst();
	while( pObj )
	{
		if ( pObj->IsInArea( pt.x, pt.y ) )
		{
			return pObj;
		}

		pObj = (CControllerObj *)pObj->GetNext();
	}

	for( SI16 i = 0; i < MAX_HAVE_CHILD_DIALOG_NUM; ++i )
	{
		if ( m_pChildDialog[ i ] && m_pChildDialog[ i ]->IsShow() )
		{
			TempPt.x = pt.x - m_pChildDialog[ i ]->GetX();
			TempPt.y = pt.y - m_pChildDialog[ i ]->GetY();

			if ( pObj = m_pChildDialog[ i ]->GetControlAtPoint( TempPt ) )
			{
				return pObj;
			}
		}
	}

	//pObj = (CControllerObj *)GetExistList()->GetFirst();
	//while( pObj )
	//{
	//	if ( pObj->IsInArea( pt.x, pt.y ) )
	//	{
	//		return pObj;
	//	}

	//	pObj = (CControllerObj *)pObj->GetNext();
	//}

	return NULL;
}

void CControllerMgr::SetTitle( TCHAR *Title, COLORREF rgb, SI32 size, SI16 OffsetX, SI16 OffsetY )
{	
	cltClient *pclclient = (cltClient *)pclClient;

	m_bHasTitle = false;
	m_siTitleOffsetX = OffsetX;
	m_siTitleOffsetY = OffsetY;

	if ( Title == NULL || !Title[ 0 ] )
	{	
		TMemSet( m_strTitle, 0, MAX_PATH );

		return;
	}

	StringCchCopy( m_strTitle, MAX_PATH, Title );

	static SI16 siOldWidth = m_siWidth;

	if( siOldWidth != m_siWidth)
	{
		siOldWidth = m_siWidth;
        SAFE_DELETE(m_pTitleSurfaceObj);
	}

	if ( m_pTitleSurfaceObj == NULL )
	{
		m_pTitleSurfaceObj = new CControllerSurfaceObj();
		m_pTitleSurfaceObj->Create( m_siWidth, DIALOG_TITLEBAR_SURFACE_HEIGHT );
	}

	if ( m_pTitleSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = m_pTitleSurfaceObj->GetSurface();

		if ( lpSurface )
		{
			if ( m_TitleEncBuffer )
			{
				delete [] m_TitleEncBuffer;
				m_TitleEncBuffer = NULL;
			}

			m_TitleEncBuffer = new RGB16[ m_siWidth * DIALOG_TITLEBAR_SURFACE_HEIGHT * 2 ];

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );
			
			HDC hDC;

			if( lpSurface->GetDC( &hDC ) == DD_OK)
			{
				HFONT hFont;
				if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
					hFont = CreateFont( size, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, DEFAULT_FONT_KIND );
				}
				else {
					hFont = CreateFont( size, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, DEFAULT_FONT_KIND );
				}
								
				HFONT hOldFont = NULL;
				hOldFont = (HFONT)SelectObject( hDC, hFont );
				SetTextColor( hDC, rgb );

				SetBkMode( hDC, TRANSPARENT );

				RECT rect = { 3 , 1, m_siWidth - 1, DIALOG_TITLEBAR_SURFACE_HEIGHT - 1 };

				DrawText( hDC, Title, -1, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE );

				SelectObject( hDC, hOldFont );

				if ( hFont )
				{
					DeleteObject( hFont );
					hFont = NULL;
				}

				lpSurface->ReleaseDC( hDC );
			}

			if ( GP.LockSurface( lpSurface ) )
			{
				if(m_TitleEncBuffer)
				{
					int size;
					g_graphics.EncodeXpr( m_TitleEncBuffer, m_siWidth, DIALOG_TITLEBAR_SURFACE_HEIGHT, RGB16_TRANS, &size );
				}

				GP.UnlockSurface( lpSurface );

				m_bHasTitle = true;
			}
		}
	}

	return;
}

void CControllerMgr::Draw( SI32 ScreenRX, SI32 ScreenRY )
{
	if ( ! IsShow() )
	{
		return;
	}
	
	cltClient *pclclient = (cltClient *)pclClient;

	SI16 i = 0;
	SI16 j = 0;

	if ( GetFileName() != NULL )
	{
		TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );

		if ( pSpr )
		{
			if ( IsBKTileImg() )
			{	
				DrawTileImg( pSpr, GetDrawOption(), GetFontIndex(), GetAlphaValue(), ScreenRX + GetX(), ScreenRY + GetY(), GetWidth(), GetHeight(), m_siBKTileNum );
			}
			else
			{
				if( GetDrawOption() == SPR_PUTSPRT )
					GP.PutSpr(pSpr, ScreenRX + GetX(), ScreenRY + GetY(), GetFontIndex() );
				else if( GetDrawOption() == SPR_PUTSPRALPHAT )
					GP.PutSprAlpha(pSpr, ScreenRX + GetX(), ScreenRY + GetY(), GetAlphaValue(), GetFontIndex() );
			}
		}
	}

	if ( m_bHasTitle )
	{
		DrawTitle( ScreenRX, ScreenRY );
	}

	for ( i = 0; i < MAX_HAVE_CHILD_DIALOG_NUM; ++i )
	{
		if ( m_pChildDialog [ i ] && m_pChildDialog [ i ]->IsShow() )
		{
			m_pChildDialog [ i ]->Draw( ScreenRX + GetX(), ScreenRY + GetY() );
		}
	}

	DrawDialogFirst( ScreenRX, ScreenRY );

	//------------------------------------------------------------------------
	// 컨트롤 그리기 방식 수정 - by LEEKH 2008-08-01
	// 뒤에서 부터 그리기
	//------------------------------------------------------------------------
	CControllerObj *pObj = (CControllerObj *)GetExistList()->GetLast();

	while( pObj )
	{
		pObj->Draw( ScreenRX + GetX(), ScreenRY + GetY() );

		pObj = (CControllerObj *)pObj->GetPre();
	}
	//------------------------------------------------------------------------

/*	CControllerObj *pObj = (CControllerObj *)GetExistList()->GetFirst();

	while( pObj )
	{
		switch( pObj->GetControllerKind() )
		{
		case CONTROLLER_KIND_IMAGESTATIC:
		case CONTROLLER_KIND_STATIC:
		case CONTROLLER_KIND_OUTLINE:
			pObj->Draw( ScreenRX + GetX(), ScreenRY + GetY() );
			break;
		default:
			break;
		}

		pObj = (CControllerObj *)pObj->GetNext();
	}

	pObj = (CControllerObj *)GetExistList()->GetFirst();

	while( pObj )
	{
		switch( pObj->GetControllerKind() )
		{
		case CONTROLLER_KIND_IMAGESTATIC:
		case CONTROLLER_KIND_STATIC:
		case CONTROLLER_KIND_OUTLINE:
		case CONTROLLER_KIND_SCROLL:
			break;
		default:
			pObj->Draw( ScreenRX + GetX(), ScreenRY + GetY() );
			break;
		}
		
		pObj = (CControllerObj *)pObj->GetNext();
	}
*/
	DrawDialogLast( ScreenRX, ScreenRY );
	
	if ( GetDialogControllerKind() == CONTROLLER_KIND_POPUPDIALOG )
	{
		DrawLater( ScreenRX, ScreenRY );
	}

	return;
}

void CControllerMgr::DrawTransparent( SI32 ScreenRX, SI32 ScreenRY, SI32 Percent )
{
	if ( ! IsShow() )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	SI16 i = 0;
	SI16 j = 0;

	if ( GetFileName() != NULL )
	{
		TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );

		if ( pSpr )
		{
			if ( IsBKTileImg() )
			{	
				SI32 siNewAlphaValue = (Percent * GetAlphaValue()) / 100;
				DrawTileImg( pSpr, GetDrawOption(), GetFontIndex(), siNewAlphaValue, ScreenRX + GetX(), ScreenRY + GetY(), GetWidth(), GetHeight(), m_siBKTileNum );
			}
			else
			{
				if( GetDrawOption() == SPR_PUTSPRT )
				{
					SI32 siNewAlphaValue = (Percent * (MAX_ALPHA_LEVEL-1)) / 100;
					GP.PutSprAlpha(pSpr, ScreenRX + GetX(), ScreenRY + GetY(), siNewAlphaValue, GetFontIndex() );
				}
				else if( GetDrawOption() == SPR_PUTSPRALPHAT )
				{
					SI32 siNewAlphaValue = (Percent * m_siAlphaValue) / 100;
					GP.PutSprAlpha(pSpr, ScreenRX + GetX(), ScreenRY + GetY(), siNewAlphaValue, GetFontIndex() );
				}
			}
		}
	}

	if ( m_bHasTitle )
	{
		DrawTitle( ScreenRX, ScreenRY );
	}

	for ( i = 0; i < MAX_HAVE_CHILD_DIALOG_NUM; ++i )
	{
		if ( m_pChildDialog [ i ] && m_pChildDialog [ i ]->IsShow() )
		{
			m_pChildDialog [ i ]->DrawTransparent( ScreenRX + GetX(), ScreenRY + GetY(), Percent );
		}
	}

	//------------------------------------------------------------------------
	// 컨트롤 그리기 방식 수정 - by LEEKH 2008-08-01
	// 뒤에서 부터 그리기
	//------------------------------------------------------------------------
	CControllerObj *pObj = (CControllerObj *)GetExistList()->GetLast();
	SI32		siAlphaValue, siNewAlphaValue;
	while( pObj )
	{
		switch(pObj->GetDrawOption())
		{
		case SPR_PUTSPRT:
			{
				siAlphaValue = MAX_ALPHA_LEVEL-1;
				siNewAlphaValue = (Percent * siAlphaValue) / 100;
				pObj->SetAlphaValue(siNewAlphaValue);
				// DRAW - 잠시 알파값과 알파옵션을 사용하여 그린다.
				pObj->SetDrawOption(SPR_PUTSPRALPHAT);
				pObj->Draw( ScreenRX + GetX(), ScreenRY + GetY() );
				// 컨트롤의 원래 값 되돌림
				pObj->SetAlphaValue(siAlphaValue);
				pObj->SetDrawOption(SPR_PUTSPRT);
			}
			break;
		case SPR_PUTSPRALPHAT:
			{
				siAlphaValue = pObj->GetAlphaValue();
				siNewAlphaValue = (Percent * siAlphaValue) / 100;
				pObj->SetAlphaValue(siNewAlphaValue);
				// DRAW - 알파값을 변경하여 그린다.
				pObj->Draw( ScreenRX + GetX(), ScreenRY + GetY() );
				// 컨트롤의 원래 값 되돌림
				pObj->SetAlphaValue(siAlphaValue);
			}
			break;
		}

		pObj = (CControllerObj *)pObj->GetPre();
	}
	//------------------------------------------------------------------------

	return;
}

void CControllerMgr::DrawLater( SI32 ScreenRX, SI32 ScreenRY )
{
	for ( SI16 i = 0; i < MAX_HAVE_CHILD_DIALOG_NUM; ++i )
	{
		if ( m_pChildDialog [ i ] && m_pChildDialog [ i ]->IsShow() )
		{
			m_pChildDialog [ i ]->DrawLater( ScreenRX + GetX(), ScreenRY + GetY() );
		}
	}

	CControllerObj *pObj = (CControllerObj *)GetExistList()->GetFirst();

	while( pObj )
	{
		if( pObj->GetControllerKind() == CONTROLLER_KIND_COMBOBOX && pObj->IsShow() )
		{
			CComboBox *pComboBox = static_cast< CComboBox * >(pObj);
			pComboBox->DrawOpenedImage( ScreenRX + GetX(), ScreenRY  + GetY());
		}

		pObj = (CControllerObj *)pObj->GetNext();
	}

	/*
	pObj = (CControllerObj *)GetExistList()->GetFirst();

	while( pObj )
	{	
		if( pObj->GetControllerKind() == CONTROLLER_KIND_SCROLL && pObj->IsShow() )
		{
			CScroll *pComboBox = static_cast< CScroll * >(pObj);
			pComboBox->Draw( ScreenRX + GetX(), ScreenRY  + GetY());
		}

		pObj = (CControllerObj *)pObj->GetNext();
	}
	*/

	return;
}

void CControllerMgr::DirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackSurface, SI32 ScreenRX, SI32 ScreenRY )
{
	VDirectDrawToBackScreen( lpBackSurface, ScreenRX, ScreenRY );
}

void CControllerMgr::DrawTitle( SI32 ScreenRX, SI32 ScreenRY )
{
	if ( m_bHasTitle && m_TitleEncBuffer )
	{
		GP.PutXpr16( ScreenRX + GetX() + 3 + m_siTitleOffsetX, ScreenRY + GetY() + m_siTitleOffsetY, 
			m_siWidth, DIALOG_TITLEBAR_SURFACE_HEIGHT,
			m_TitleEncBuffer,
			RGB16_TRANS );
	}

	return;
}

bool CControllerMgr::IsInArea( SI16 XPos, SI16 YPos )
{
	if ( ! IsShow() )
	{
		return false;
	}

	if ( m_siXPos < XPos && m_siYPos < YPos &&
		m_siXPos + m_siWidth > XPos && m_siYPos + m_siHeight > YPos )
	{
		return true;
	}

	return false;
}

// Interface 를 이동
void CControllerMgr::MoveDialog( SI16 OffsetX, SI16 OffsetY )
{
	cltClient *pclclient = ( cltClient * )pclClient;

	SI16 TempXPos = m_siXPos + OffsetX;
	SI16 TempYPos = m_siYPos + OffsetY;

	bool Available = pclclient->m_pInterfaceMgr->IsAvailableMove( TempXPos, TempYPos, GetWidth(), GetHeight() );

	if ( ! Available )
	{
		return;
	}

	pclclient->m_pInterfaceMgr->SetSnapIfAvailable( TempXPos, TempYPos, GetWidth(), GetHeight() );

	m_siXPos = TempXPos;
	m_siYPos = TempYPos;

	return;
}

void CControllerMgr::Show( SI16 ControllerID, bool ShowOption )
{
	CControllerObj *pObj = (CControllerObj *)GetExistList()->GetFirst();

	while( pObj )
	{
		if ( ControllerID == pObj->GetControllerID() )
		{
			pObj->Show( ShowOption );
			break;
		}

		pObj = (CControllerObj *)pObj->GetNext();
	}

	return;
}

void CControllerMgr::Show( bool ShowOption )
{
	m_bShow = ShowOption;

	if ( m_bShow )
	{
		cltClient *pclclient = ( cltClient * )pclClient;
		if ( GetDialogControllerKind() == CONTROLLER_KIND_POPUPDIALOG )
		{
			pclclient->m_pInterfaceMgr->MoveToFirst( this );
		}

		GetDialogControllerID();
		m_dwShowStartTime = GetTickCount();
	}
	else
	{
		cltClient *pclclient = ( cltClient * )pclClient;

		if (this == pclclient->m_pInterfaceMgr->GetFouceMgr())
		{
			pclclient->m_pInterfaceMgr->ClearFocusMgr();
		}
		
		ClearFocus();

		g_DlgPositionSave.SetPosition( this->GetDialogControllerID(), this->GetX(), this->GetY() ); //자신위치를 메니져의 멤버변수에 저장
	}
}

bool CControllerMgr::MsgProc( UINT iMsg, WPARAM &wParam, LPARAM &lParam, bool MouseOverSet )
{
	if ( ! IsShow() )
	{
		return false;
	}

	if ( m_pFocusObj &&	m_pFocusObj->GetParent() == this &&	m_pFocusObj->IsAvailable() )
	{
		if ( m_pFocusObj->MsgProc( iMsg, wParam, lParam ) )
		{
			return true;
		}
	}

	/*
	if ( iMsg == WM_LBUTTONUP && GetDialogControllerID() == 181 )
	{
		int a = 0;
	}
	*/

	SI16 i = 0;
	for ( i = 0; i < MAX_HAVE_CHILD_DIALOG_NUM; ++i )//차일드다이얼로그 메세지를 받아 잡아오는거.
	{
		if ( m_pChildDialog[ i ] && m_pChildDialog[ i ]->IsShow() )
		{
			if ( m_pChildDialog[ i ]->MsgProc( iMsg, wParam, lParam, MouseOverSet ) )
			{
				return true;
			}
		}
	}

	switch( iMsg )
	{
	case WM_ACTIVATEAPP:
		if( m_pFocusObj && 
			m_pFocusObj->GetParent() == this && 
			m_pFocusObj->IsAvailable() )
		{
			if( wParam )
			{
				m_pFocusObj->SetFocus( true );
			}
			else
			{
				ClearFocus();
			}
		}
		break;
	case WM_CHAR:
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
			if( m_pFocusObj && 
				m_pFocusObj->GetParent() == this && 
				m_pFocusObj->IsAvailable() )
			{
				if( m_pFocusObj->HandleKeyboard( iMsg, wParam, lParam ) )
					return true;
			}

			if( iMsg == WM_KEYUP  && ( !m_pFocusObj ||
				( m_pFocusObj->GetControllerKind() != CONTROLLER_KIND_IMEEDIT
				&& m_pFocusObj->GetControllerKind() != CONTROLLER_KIND_EDIT ) ) )
			{
				CControllerObj *pObj = (CControllerObj *)GetExistList()->GetFirst();

				while( pObj )
				{
					if ( pObj->GetHotkey() == wParam )
					{
						pObj->OnHotkey();
						return true;
					}

					pObj = (CControllerObj *)pObj->GetNext();
				}
			}
		}
		break;

	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
		{	

			if( !m_bMouseInput )
				return false;

			cltClient *pclclient = (cltClient *)pclClient;
			//SI32 XPos = GET_X_LPARAM(lParam); 
			//SI32 YPos = GET_Y_LPARAM(lParam); 
			//SI32 XPos = LOWORD(lParam);
			//SI32 YPos = HIWORD(lParam);
		
			
			POINT mousePoint;
			GetCursorPos( &mousePoint );
			ScreenToClient( pclclient->GetHwnd(), &mousePoint );

			pclclient->MouseMoveAction( mousePoint.x, mousePoint.y );
			((cltClient*)pclclient)->SetMousePos( (SI32 *)&mousePoint.x, (SI32 *)&mousePoint.y );

			if ( m_pParentDialog )
			{	
				mousePoint.x = mousePoint.x + MAP_TILE_XSIZE - GetX() - m_pParentDialog->GetX();
				mousePoint.y = mousePoint.y + MAP_TILE_YSIZE - GetY() - m_pParentDialog->GetY();
				//mousePoint.x = XPos + MAP_TILE_XSIZE - GetX() - m_pParentDialog->GetX();
				//mousePoint.y = YPos + MAP_TILE_YSIZE - GetY() - m_pParentDialog->GetY();
			}
			else
			{	
				mousePoint.x = mousePoint.x + MAP_TILE_XSIZE - GetX();
				mousePoint.y = mousePoint.y + MAP_TILE_YSIZE - GetY();
				//mousePoint.x = XPos + MAP_TILE_XSIZE - GetX();
				//mousePoint.y = YPos + MAP_TILE_YSIZE - GetY();
			}
			
			if( m_pFocusObj && 
				m_pFocusObj->GetParent() == this && 
				m_pFocusObj->IsAvailable() &&
				m_pFocusObj->GetControllerKind() != CONTROLLER_KIND_IMAGESTATIC
				)
			{
				if( m_pFocusObj->HandleMouse( iMsg, mousePoint, wParam, lParam ) )
					return true;
			}

			CControllerObj* pControl = GetControlAtPoint( mousePoint );
			if( pControl != NULL && pControl->IsAvailable() )
			{
				if ( pControl->GetControllerKind() == CONTROLLER_KIND_SCROLL )
				{	
					return false;
				}

				//if ( pControl->GetControllerKind() == CONTROLLER_KIND_SCROLL )
				//{
				//	CScroll *pScroll = (CScroll *)( pControl );
				//	CControllerObj *pScrollParent = pScroll->GetScrollParent();
				//	if ( pScrollParent )
				//	{
				//		POINT ScrollPt;
				//		ScrollPt.x = mousePoint.x - pScrollParent->GetX();
				//		ScrollPt.y = mousePoint.y - pScrollParent->GetY();
				//	}
				//}
				//else
				//{
			
				if ( pControl->HandleMouse( iMsg, mousePoint, wParam, lParam ) )
						return true;
				//}
			}
			else
			{	
				if( iMsg == WM_LBUTTONDOWN && 
					m_pFocusObj && 
					m_pFocusObj->GetParent() == this )
				{
					ClearFocus();
				}
			}

			switch( iMsg )
			{
			case WM_MOUSEMOVE:
				{	
					if ( ! MouseOverSet )
					{
						OnMouseMove( mousePoint, MouseOverSet );
					}
					else
					{
						if ( m_pMouseOverObj )
						{
							m_pMouseOverObj->OnMouseLeave();
							m_pMouseOverObj = NULL;
						}
					}
					return false;
				}
				break;
			}
			break;
		}
		break;
	}

	return false;
}

void CControllerMgr::SendEvent( UINT nEvent, bool bTriggeredByUser, CControllerObj* pController )
{
	m_pGUICallBackEvent( this, nEvent, pController->GetControllerID(), pController );
	return;
}

void CControllerMgr::RequestFocus( CControllerObj *pController )	
{
	if( m_pFocusObj == pController )
		return;

	if( !pController->CanHaveFocus() )
		return;

	if( m_pFocusObj )
	{
		// 리스트 선택후 에디트입력 케이스의 경우 포커스 유지(아이템 선택후 판매수량입력)
		if(!(m_pFocusObj->GetControllerKind() == CONTROLLER_KIND_LISTVIEW 
			&& pController->GetControllerKind() == CONTROLLER_KIND_EDIT))
				ClearFocus();
	}

	pController->SetFocus( true );
	m_pFocusObj = pController;

	return;
}

void CControllerMgr::ClearFocus()
{
	SI16 i = 0;
	for ( i = 0; i < MAX_HAVE_CHILD_DIALOG_NUM; ++i )
	{
		if ( m_pChildDialog[ i ] && m_pChildDialog[ i ]->IsShow() )
		{
			m_pChildDialog[ i ]->ClearFocus();
		}
	}

	if( m_pFocusObj )
	{
		m_pFocusObj->SetFocus( false );
		m_pFocusObj = NULL;
	}

	ReleaseCapture();

	return;
}

void CControllerMgr::SetFocus( bool Focus )
{
	m_bHasFocus = Focus;

	if ( ! Focus )
	{
		ClearFocus();
	}

	/*
	if ( ! Focus )
	{
		SI16 i = 0;
		for ( i = 0; i < MAX_HAVE_CHILD_DIALOG_NUM; ++i )
		{
			if ( m_pChildDialog[ i ] && m_pChildDialog[ i ]->IsShow() )
			{
				m_pChildDialog[ i ]->SetFocus( false );
			}
		}

		CControllerObj *pObj = (CControllerObj *)GetExistList()->GetFirst();

		while( pObj )
		{
			pObj->SetFocus( false );
			pObj = (CControllerObj *)pObj->GetNext();
		}
	}
	*/
}

void CControllerMgr::SetFocus( CControllerObj *pControllerObj )
{
	if ( pControllerObj == NULL )
	{
		if ( m_pFocusObj )
		{
			m_pFocusObj->SetFocus( false );
			m_pFocusObj = NULL;
		}
		return;
	}

	if ( m_pFocusObj )
	{
		if ( m_pFocusObj == pControllerObj )
		{
			return;
		}
		else
		{
			m_pFocusObj->SetFocus( false );
			
			m_pFocusObj = pControllerObj;
			m_pFocusObj->SetFocus( true );
		}
	}
	else
	{
		m_pFocusObj = pControllerObj;
		m_pFocusObj->SetFocus( true );
	}

	return;
}

void CControllerMgr::OnMouseMove( POINT pt, bool MouseOverSet )
{
	CControllerObj* pControl = GetControlAtPoint( pt );

	if( pControl && pControl == m_pMouseOverObj )
	{	
		return;
	}

	if( m_pMouseOverObj )
	{
		m_pMouseOverObj->OnMouseLeave();
	}

	m_pMouseOverObj = pControl;

	if( pControl != NULL )
	{
		m_pMouseOverObj->OnMouseEnter();
	}
}

void CControllerMgr::ControllerMgrAction( clock_t CurrentClock )
{
	if ( m_bTimerOnOff && abs( CurrentClock - m_PreClock ) > m_siActionTimer )
	{
		m_PreClock = CurrentClock;
		Action();
		// Child ControllerMgr->Action 호출
		for ( SI16 i = 0; i < MAX_HAVE_CHILD_DIALOG_NUM; ++i )
		{
			if( m_pChildDialog[ i ] != NULL)
			{
				m_pChildDialog[ i ]->Action();
			}
		}

	}

	return;
}

void CControllerMgr::SetRadioButtonCheck( SI16 RadioButtonControllerID, SI16 GroupID )
{
	CControllerObj *pTemp = (CControllerObj *)GetExistList()->GetFirst();

	while ( pTemp )
	{
		if ( pTemp->GetControllerKind() == CONTROLLER_KIND_RADIOBUTTON && pTemp->GetControllerID() != RadioButtonControllerID )
		{
			CRadioButton *pRadioButton = (CRadioButton *)pTemp;
			if ( pRadioButton->GetGroupID() == GroupID )
			{
				pRadioButton->SetCheck( false, pRadioButton->GetControllerKind() );
			}
		}
		pTemp = (CControllerObj *)pTemp->GetNext();
	}

	return;
}

void CControllerMgr::SetNonEventRadioButtonCheck( SI16 RadioButtonControllerID, SI16 GroupID )
{
	CControllerObj *pTemp = (CControllerObj *)GetExistList()->GetFirst();

	while ( pTemp )
	{
		if ( pTemp->GetControllerKind() == CONTROLLER_KIND_RADIOBUTTON && pTemp->GetControllerID() != RadioButtonControllerID )
		{
			CRadioButton *pRadioButton = (CRadioButton *)pTemp;
			if ( pRadioButton->GetGroupID() == GroupID )
			{
				pRadioButton->SetNonEventOneButtonCheck(false);
			}
		}
		pTemp = (CControllerObj *)pTemp->GetNext();
	}

	return;
}

void CControllerMgr::SetTopMostWindow( bool TopMostWindow )
{
	cltClient *pclclient = ( cltClient * )pclClient;

	if ( GetDialogControllerKind() == CONTROLLER_KIND_POPUPDIALOG )
	{
		m_bTopMostWindow = TopMostWindow;
		pclclient->m_pInterfaceMgr->SetTopMostWindow( this, TopMostWindow );
	}

	return;
}

void CControllerMgr::SetModalDialog( bool ModalDialog )
{
	cltClient *pclclient = ( cltClient * )pclClient;

	if ( GetDialogControllerKind() == CONTROLLER_KIND_POPUPDIALOG )
	{
		m_bModalDialog = ModalDialog;
		pclclient->m_pInterfaceMgr->SetModalDialog( this, ModalDialog );
	}

	return;
}

void CControllerMgr::SetDrawOptionAllControl( SI16 DrawOption )
{
	CControllerObj *pObj = (CControllerObj *)GetExistList()->GetFirst();

	while( pObj )
	{
		pObj->SetDrawOption(DrawOption);
		pObj = (CControllerObj*)pObj->GetNext();
	}
}

void CControllerMgr::SetAlphaValueAllControl( SI16 AlphaValue )
{
	SI32 siAlpha = ( (AlphaValue>=MAX_ALPHA_LEVEL) ? (MAX_ALPHA_LEVEL-1) : AlphaValue );

	CControllerObj *pObj = (CControllerObj *)GetExistList()->GetFirst();

	while( pObj )
	{
		pObj->SetAlphaValue(siAlpha);
		pObj = (CControllerObj*)pObj->GetNext();
	}
}
