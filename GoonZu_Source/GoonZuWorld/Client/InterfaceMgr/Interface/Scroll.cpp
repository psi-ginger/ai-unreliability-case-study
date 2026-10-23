#include "Scroll.h"

#include "../InterfaceMgr.h"
#include "../ControllerMgr.h"

#include "../ControllerSurfaceObj.h"

#include "../InterfaceImgMgr/InterfaceImgMgr.h"

#include "ComboBox.h"
#include "List.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

#include <zmouse.h>

#include <Client.h>
extern cltCommonLogic *pclClient;


CScroll::CScroll( CControllerMgr *pParent ) : CControllerObj( pParent )
{
	m_bShowThumb = false;
	SetRect( &m_rcUpButton, 0, 0, 0, 0 );
	SetRect( &m_rcDownButton, 0, 0, 0, 0 );
	SetRect( &m_rcTrack, 0, 0, 0, 0 );
	SetRect( &m_rcThumb, 0, 0, 0, 0 );
	
	m_bHorizontalScroll = false;

	m_siPosition = 0;
	m_siPageSize = 1;
	m_siStart = 0;
	m_siEnd = 1;
	m_Arrow = CLEAR;

	SetScrollBKFile( SCROLL_BACKGROUND_FILENAME, SCROLL_BACKGROUND_FONTINDEX );
	SetScrollThumbFile( SCROLL_THUMB_FILENAME, SCROLL_THUMB_FONTINDEX );
	SetScrollUpButtonFile( SCROLL_UPBUTTON_FILENAME, SCROLL_UPBUTTON_FONTINDEX );
	SetScrollDownButtonFile( SCROLL_DOWNBUTTON_FILENAME, SCROLL_DOWNBUTTON_FONTINDEX );
	
	m_pScrollParent = NULL;

	m_uiScrollButtonWidth = SCROLL_BUTTON_WIDTH;
	m_uiScrollButtonHeight = SCROLL_BUTTON_HEIGHT;
}

CScroll::~CScroll()
{

}

void CScroll::Create( SI16 ControllerID, CControllerObj* pScrollParent,
					 TCHAR *FileName, SI16 FontIndex,
					 SI16 XPos, SI16 YPos, SI16 Width, SI16 Height,
					 SI16 SurfaceWidth, SI16 SurfaceHeight,
					 SI16 LineHeight, bool HorizontalScroll
					 )
{
	CControllerObj::Create( ControllerID, CONTROLLER_KIND_SCROLL,
		FileName, FontIndex, false, 0,
		XPos, YPos, Width, Height, 
		SurfaceWidth, SurfaceHeight );

	m_pScrollParent = pScrollParent;

	m_siLineHeight = LineHeight;

	m_bHorizontalScroll = HorizontalScroll;

	if ( HorizontalScroll )
	{
		m_siPageSize = Width / LineHeight;

		SetScrollBKFile( HSCROLL_BACKGROUND_FILENAME, HSCROLL_BACKGROUND_FONTINDEX );
		SetScrollThumbFile( HSCROLL_THUMB_FILENAME, HSCROLL_THUMB_FONTINDEX );
		SetScrollUpButtonFile( HSCROLL_LEFTBUTTON_FILENAME, HSCROLL_LEFTBUTTON_FONTINDEX );
		SetScrollDownButtonFile( HSCROLL_RIGHTBUTTON_FILENAME, HSCROLL_RIGHTBUTTON_FONTINDEX );
	}
	else
	{
		m_siPageSize = Height / LineHeight;
	}

	SetScrollPos( XPos, YPos, Width, Height );

	Show( false );

	return;
}

void CScroll::SetScrollPos( SI16 XPos, SI16 YPos, SI16 Width, SI16 Height )
{
	SetControllerPos( XPos, YPos, Width, Height, Width, Height );

	if ( m_bHorizontalScroll )
	{
		// +1 또는 -2 가 있는 이유는 스크롤 외에 스크롤을 둘러싼 테두리가 있기 때문.
		m_rcUpButton.left = XPos + 1;
		m_rcUpButton.right = m_rcUpButton.left + m_uiScrollButtonWidth - 2;
		m_rcUpButton.top = YPos + 1;
		m_rcUpButton.bottom = m_rcUpButton.top + m_uiScrollButtonHeight - 2;

		m_rcDownButton.left = XPos + Width - m_uiScrollButtonWidth - 1;
		m_rcDownButton.right = m_rcDownButton.left + m_uiScrollButtonWidth - 2;
		m_rcDownButton.top = m_rcUpButton.top;
		m_rcDownButton.bottom = m_rcUpButton.bottom;

		m_rcTrack.left = m_rcUpButton.right + 1;
		m_rcTrack.right = m_rcDownButton.left - 1;
		m_rcTrack.top = m_rcUpButton.top;
		m_rcTrack.bottom = m_rcUpButton.bottom;

		m_rcThumb.top = m_rcUpButton.top;
		m_rcThumb.bottom = m_rcUpButton.bottom;

		m_siPageSize = Width / m_siLineHeight;
	}
	else
	{
		// +1 또는 -2 가 있는 이유는 스크롤 외에 스크롤을 둘러싼 테두리가 있기 때문.
		m_rcUpButton.left = XPos + 1;
		m_rcUpButton.right = m_rcUpButton.left + Width - 2;
		m_rcUpButton.top = YPos + 1;
		m_rcUpButton.bottom = m_rcUpButton.top + m_uiScrollButtonHeight - 2;

		m_rcDownButton.left = m_rcUpButton.left;	// XPos + 1;	<-- 따져보면 같음.. 연산 줄여야지... 아래도 그렇고...
		m_rcDownButton.right = m_rcUpButton.right;	// m_rcDownButton.left + Width - 2;
		m_rcDownButton.top = YPos + Height - m_uiScrollButtonHeight - 1;
		m_rcDownButton.bottom = m_rcDownButton.top + m_uiScrollButtonHeight - 2;

		m_rcTrack.left = m_rcUpButton.left;
		m_rcTrack.right = m_rcUpButton.right;
		m_rcTrack.top = m_rcUpButton.bottom + 1;
		m_rcTrack.bottom = m_rcDownButton.top - 1;

		m_rcThumb.left = XPos + 1;
		m_rcThumb.right = m_rcThumb.left + Width - 2;

		m_siPageSize = Height / m_siLineHeight;
	}

	return;
}

bool CScroll::HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam )
{
	if ( ! IsShow() )
	{
		return false;
	}

	static int ThumbOffset;	
	static bool bDrag;

	m_LastMouse = pt;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{	
			if( PtInRect( &m_rcUpButton, pt ) )
			{
				if( m_siPosition > m_siStart )
					--m_siPosition;
				UpdateThumbRect();
				m_Arrow = CLICKED_UP;
				GetParent()->SendEvent( EVENT_SCROLL_UPBUTTONCLICKED, true, this );
				return true;
			}
			
			if( PtInRect( &m_rcDownButton, pt ) )
			{
				if( m_siPosition + m_siPageSize < m_siEnd )
					++m_siPosition;
				UpdateThumbRect();
				m_Arrow = CLICKED_DOWN;
				GetParent()->SendEvent( EVENT_SCROLL_DOWNBUTTONCLICKED, true, this );
				return true;
			}
			
			if( PtInRect( &m_rcThumb, pt ) )
			{	
				SetCapture( pclClient->GetHwnd() );

				bDrag = true;

				if ( m_bHorizontalScroll )
				{
					ThumbOffset = pt.x - m_rcThumb.left;
				}
				else
				{
					ThumbOffset = pt.y - m_rcThumb.top;
				}

				return true;
			}
			
			if ( m_bHorizontalScroll )
			{
				if( m_rcThumb.top <= pt.y && m_rcThumb.bottom > pt.y )
				{
					if( m_rcThumb.left > pt.x &&	m_rcTrack.left <= pt.x )
					{
						Scroll( - ( m_siPageSize - 1 ) );
						return true;
					}
					else if( m_rcThumb.right <= pt.x && m_rcTrack.right > pt.x )
					{
						Scroll( m_siPageSize - 1 );
						return true;
					}
				}
			}
			else
			{
				if( m_rcThumb.left <= pt.x && m_rcThumb.right > pt.x )
				{
					if( m_rcThumb.top > pt.y &&	m_rcTrack.top <= pt.y )
					{
						Scroll( -( m_siPageSize - 1 ) );
						return true;
					}
					else if( m_rcThumb.bottom <= pt.y && m_rcTrack.bottom > pt.y )
					{
						Scroll( m_siPageSize - 1 );
						return true;
					}
				}
			}

			break;
		}

	case WM_LBUTTONUP:
		{
			if ( bDrag )
			{
				ReleaseCapture();
				bDrag = false;
				UpdateThumbRect();
				m_Arrow = CLEAR;

				return true;
			}			

			if ( IsInArea( pt.x, pt.y ) )
			{
				return true;
			}
		}
		break;

	case WM_MOUSEMOVE:
		{
			if( bDrag && m_bShowThumb )
			{
				if ( m_bHorizontalScroll )
				{
					m_rcThumb.right += pt.x - ThumbOffset - m_rcThumb.left;
					m_rcThumb.left = pt.x - ThumbOffset;
					if( m_rcThumb.left < m_rcTrack.left )
					{
						OffsetRect( &m_rcThumb, m_rcTrack.left - m_rcThumb.left, 0 );
					}
					else if( m_rcThumb.right > m_rcTrack.right )
					{
						OffsetRect( &m_rcThumb, m_rcTrack.right - m_rcThumb.right, 0 );
					}

					int nMaxFirstItem = m_siEnd - m_siStart - m_siPageSize;  
					int nMaxThumb = RectWidth( m_rcTrack ) - RectWidth( m_rcThumb );  

					m_siPosition = m_siStart +
						( m_rcThumb.left - m_rcTrack.left +
						nMaxThumb / ( nMaxFirstItem * 2 ) ) * 
						nMaxFirstItem  / nMaxThumb;
				}
				else
				{
					m_rcThumb.bottom += pt.y - ThumbOffset - m_rcThumb.top;
					m_rcThumb.top = pt.y - ThumbOffset;
					if( m_rcThumb.top < m_rcTrack.top )
					{
						OffsetRect( &m_rcThumb, 0, m_rcTrack.top - m_rcThumb.top );
					}
					else if( m_rcThumb.bottom > m_rcTrack.bottom )
					{
						OffsetRect( &m_rcThumb, 0, m_rcTrack.bottom - m_rcThumb.bottom );
					}

					int nMaxFirstItem = m_siEnd - m_siStart - m_siPageSize;  
					int nMaxThumb = RectHeight( m_rcTrack ) - RectHeight( m_rcThumb );  
					
					if(nMaxThumb <= 0) return false;

					m_siPosition = m_siStart +
						( m_rcThumb.top - m_rcTrack.top +
						nMaxThumb / ( nMaxFirstItem * 2 ) ) * 
						nMaxFirstItem  / nMaxThumb;
				}

				UpdateThumbRect();
				GetParent()->SendEvent( EVENT_SCROLL_MOUSEMOVE, true, this );
				return true;
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				int zDelta = (short) HIWORD(wParam) / WHEEL_DELTA;
				Scroll( -zDelta );

				GetParent()->SendEvent( EVENT_SCROLL_MOUSEMOVE, true, this );
				return true;
			}
		}
		break;
	}

	return false;
}

void CScroll::UpdateThumbRect( bool CallRefresh )
{
	if( m_siEnd - m_siStart > m_siPageSize )
	{
		if ( m_bHorizontalScroll )
		{
			int nThumbWidth = max( RectWidth( m_rcTrack ) * m_siPageSize / ( m_siEnd - m_siStart ), MIN_SCROLL_THUMB_SIZE );
			int nMaxPosition = m_siEnd - m_siStart - m_siPageSize;
			m_rcThumb.left = m_rcTrack.left +
				( m_siPosition - m_siStart ) * ( RectWidth( m_rcTrack ) - nThumbWidth )	/ nMaxPosition;
			m_rcThumb.right = m_rcThumb.left + nThumbWidth;
		}
		else
		{
			int nThumbHeight = max( RectHeight( m_rcTrack ) * m_siPageSize / ( m_siEnd - m_siStart ), MIN_SCROLL_THUMB_SIZE );
			int nMaxPosition = m_siEnd - m_siStart - m_siPageSize;
			m_rcThumb.top = m_rcTrack.top +
				( m_siPosition - m_siStart ) * ( RectHeight( m_rcTrack ) - nThumbHeight )	/ nMaxPosition;
			m_rcThumb.bottom = m_rcThumb.top + nThumbHeight;
		}

		m_bShowThumb = true;
	} 
	else
	{	
		if ( m_bHorizontalScroll )
		{
			m_rcThumb.right = m_rcThumb.left;
		}
		else
		{
			m_rcThumb.bottom = m_rcThumb.top;
		}
		
		m_bShowThumb = false;
	}

	if ( m_pScrollParent && CallRefresh )
	{
		m_pScrollParent->Refresh();
	}
}

void CScroll::Cap()  
{
	if( m_siPosition < m_siStart ||
		m_siEnd - m_siStart <= m_siPageSize )
	{
		m_siPosition = m_siStart;
	}
	else if( m_siPosition + m_siPageSize > m_siEnd )
		m_siPosition = m_siEnd - m_siPageSize;

	return;
}

void CScroll::SetTrackRange( int nStart, int nEnd )
{
	m_siStart = nStart;
	m_siEnd = nEnd;

	Cap();
	//UpdateThumbRect();

	return;
}

void CScroll::ScrollMove( int nPos )
{
	if ( nPos < m_siStart )
	{
		nPos = m_siStart;
	}
	else if ( nPos > m_siEnd )
	{
		nPos = m_siEnd;
	}

	m_siPosition = nPos;

	Cap();

	UpdateThumbRect();
}

void CScroll::Scroll( int nDelta )    
{	
	m_siPosition += nDelta;

	Cap();

	UpdateThumbRect();
}

void CScroll::ShowItem( int nIndex )  
{
	
	if( nIndex < 0 )
		nIndex = 0;

	if( nIndex >= m_siEnd )
		nIndex = m_siEnd - 1;

	
	if( m_siPosition > nIndex )
		m_siPosition = nIndex;
	else
		if( m_siPosition + m_siPageSize <= nIndex )
			m_siPosition = nIndex - m_siPageSize + 1;

	UpdateThumbRect();
}

void CScroll::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	if ( m_pScrollParent == NULL )
	{
		return;
	}

	if ( !IsShow() )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	SI16 DrawXPos = BaseXPos + GetX();
	SI16 DrawYPos = BaseYPos + GetY();

	if ( m_bHorizontalScroll )
	{
		TSpr *pBackGround = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strScrollBKFileName );
		if ( pBackGround )
		{
			DrawTileImg( pBackGround, GetDrawOption(), m_siScrollBKFontIndex, GetAlphaValue(), DrawXPos, DrawYPos, GetWidth(), GetHeight(), 3, true );
		}

		TSpr *pUpButton = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strScrollUpButtonFileName );
		if ( pUpButton )
		{
			switch( GetDrawOption() )
			{
			case SPR_PUTSPRT:		GP.PutSpr(pUpButton,  DrawXPos + 1, DrawYPos + 1, m_siScrollUpButtonFontIndex );						break;
			case SPR_PUTSPRALPHAT:	GP.PutSprAlpha(pUpButton,  DrawXPos + 1, DrawYPos + 1, GetAlphaValue(), m_siScrollUpButtonFontIndex );	break;
			}
			
		}

		TSpr *pDownButton = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strScrollDownButtonFileName );
		if ( pDownButton )
		{	
			switch( GetDrawOption() )
			{
			case SPR_PUTSPRT:		GP.PutSpr(pDownButton, DrawXPos + GetWidth() - pDownButton->GetXSize() - 1, DrawYPos + 1, m_siScrollDownButtonFontIndex );							break;
			case SPR_PUTSPRALPHAT:	GP.PutSprAlpha(pDownButton, DrawXPos + GetWidth() - pDownButton->GetXSize() - 1, DrawYPos + 1, GetAlphaValue(), m_siScrollDownButtonFontIndex );	break;
			}
		}

		if ( m_bShowThumb )
		{	
			TSpr *pThumbSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strScrollThumbFileName );

			if ( pThumbSpr )
			{	
				DrawTileImg( pThumbSpr, GetDrawOption(), m_siScrollThumbFontIndex, GetAlphaValue(), DrawXPos - GetX() + m_rcThumb.left, DrawYPos - GetY() + m_rcThumb.top, m_rcThumb.right - m_rcThumb.left, m_rcThumb.bottom - m_rcThumb.top + 1, 3, true );
			}
		}
	}
	else
	{
		TSpr *pBackGround = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strScrollBKFileName );
		if ( pBackGround )
		{
			DrawTileImg( pBackGround, GetDrawOption(), m_siScrollBKFontIndex, GetAlphaValue(), DrawXPos, DrawYPos, GetWidth(), GetHeight(), 3, false );
		}

		TSpr *pUpButton = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strScrollUpButtonFileName );
		if ( pUpButton )
		{	
			switch( GetDrawOption() )
			{
			case SPR_PUTSPRT:		GP.PutSpr(pUpButton,  DrawXPos + 1, DrawYPos + 1, m_siScrollUpButtonFontIndex );							break;
			case SPR_PUTSPRALPHAT:	GP.PutSprAlpha(pUpButton,  DrawXPos + 1, DrawYPos + 1, GetAlphaValue(), m_siScrollUpButtonFontIndex );		break;
			}
		}

		TSpr *pDownButton = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strScrollDownButtonFileName );
		if ( pDownButton )
		{	
			switch( GetDrawOption() )
			{
			case SPR_PUTSPRT:		GP.PutSpr(pDownButton, DrawXPos + 1, DrawYPos + GetHeight() - pDownButton->GetYSize() - 1 , m_siScrollDownButtonFontIndex );						break;
			case SPR_PUTSPRALPHAT:	GP.PutSprAlpha(pDownButton, DrawXPos + 1, DrawYPos + GetHeight() - pDownButton->GetYSize() - 1 , GetAlphaValue(), m_siScrollDownButtonFontIndex );	break;
			}
		}

		// Thumb Draw
		if ( m_bShowThumb )
		{	
			TSpr *pThumbSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strScrollThumbFileName );

			if ( pThumbSpr )
			{	
				DrawTileImg( pThumbSpr, GetDrawOption(), m_siScrollThumbFontIndex, GetAlphaValue(), DrawXPos - GetX() + m_rcThumb.left, DrawYPos - GetY() + m_rcThumb.top, m_rcThumb.right - m_rcThumb.left, m_rcThumb.bottom - m_rcThumb.top + 1, 3, false );
			}
		}
	}

	return;
}

void CScroll::Show( bool ShowOption )
{
	CControllerObj::Show( ShowOption );

	if ( ShowOption )
	{
		UpdateThumbRect( false );
	}
	return;
}