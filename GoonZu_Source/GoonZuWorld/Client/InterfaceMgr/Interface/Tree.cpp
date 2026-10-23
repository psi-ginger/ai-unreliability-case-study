#include "Tree.h"

#include "../ControllerMgr.h"
#include "../ControllerSurfaceObj.h"

#include "../InterfaceMgr.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"

#include "Scroll.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

#include <zmouse.h>

#include <Client.h>
extern cltCommonLogic *pclClient;


CTree::CTree( CControllerMgr *pParent ) : CControllerObj( pParent )
{
	m_siLineHeight = TREE_LINE_HEIGHT;
	m_siLineSize = m_siLineHeight/2;
	m_ImgEncBuffer = NULL;

	m_siOffsetX = 0;
	m_siOffsetY = 0;

	m_siSelectedIndex = -1;

	m_pVScroll = new CScroll( pParent );
	m_pHScroll = new CScroll( pParent );

	m_siMaxWidth = 0;

	m_siAvailabeUseWidth = 0;
	m_siAvailabeUseHeight = 0;
}

CTree::~CTree()
{
	m_TreeItemVector.clear();

	SAFE_DELETE_A( m_ImgEncBuffer );

	SAFE_DELETE( m_pVScroll );
	SAFE_DELETE( m_pHScroll );
}

void CTree::Create( SI16 ControllerID,
				   TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum,
				   SI16 XPos, SI16 YPos, SI16 Width, SI16 Height,
				   SI16 SurfaceWidth, SI16 SurfaceHeight,
				   SI16 LineHeight, SI16 OffsetX, SI16 OffsetY
				   )
{	
	m_siLineHeight = LineHeight;
	m_siLineSize = m_siLineHeight/2;

	m_siOffsetX = OffsetX;
	m_siOffsetY = OffsetY;

	m_siLineHeight = LineHeight;

	m_siAvailabeUseWidth = Width - OffsetX - SCROLL_WIDTH;
	m_siAvailabeUseHeight = Height - OffsetY - SCROLL_HEIGHT;

	CControllerObj::Create( ControllerID, CONTROLLER_KIND_TREE, FileName, FontIndex, BKTileImg, BKTileImgNum, XPos, YPos, Width, Height, SurfaceWidth, SurfaceHeight );

	m_pVScroll->Create( COMMON_SCROLL, this, NULL, 0, XPos + Width - SCROLL_WIDTH - 1, YPos, SCROLL_WIDTH, Height - SCROLL_HEIGHT, SCROLL_WIDTH, Height - SCROLL_HEIGHT, LineHeight, false );
	m_pHScroll->Create( COMMON_HSCROLL, this, NULL, 0, XPos, YPos + Height - SCROLL_HEIGHT - 1, Width - SCROLL_WIDTH, SCROLL_HEIGHT, Width - SCROLL_WIDTH, SCROLL_HEIGHT, LineHeight, true );

	return;
}

bool CTree::HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam )
{
	if ( ! IsAvailable() )
		return false;

	if( m_pVScroll->HandleMouse( uMsg, pt, wParam, lParam ) )
	{
		if( ! m_pVScroll->IsFocus() )
		{
			GetParent()->RequestFocus( m_pVScroll );
		}

		return true;
	}

	if( m_pHScroll->HandleMouse( uMsg, pt, wParam, lParam ) )
	{
		if( ! m_pHScroll->IsFocus() )
		{
			GetParent()->RequestFocus( m_pHScroll );
		}

		return true;
	}

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			if ( IsInArea( pt.x, pt.y ) )
			{
				m_siSelectedIndex = GetItemIndexFromPosition( pt.x, pt.y );
				GetParent()->SendEvent( EVENT_TREE_SELECTION, true, this );

				return true;
			}
		}
		break;
		/*
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		{
		if ( IsInArea( pt.x, pt.y ) )
		{
		return true;
		}
		}
		break;
		*/
	case WM_MOUSEWHEEL:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				int zDelta = (short) HIWORD(wParam) / WHEEL_DELTA;
				m_pVScroll->Scroll( -zDelta );

				return true;
			}
		}
		break;
	}

	return false;
}

void CTree::Clear()
{
	m_TreeItemVector.clear();
	m_siSelectedIndex = -1;

	m_siMaxWidth = 0;

	m_pVScroll->SetTrackRange(0, -1);
	m_pVScroll->Show( false );

	m_pHScroll->SetTrackRange(0, -1);
	m_pHScroll->Show( false );

	return;
}

void CTree::InsertItem( stTreeItem *pTreeItem )
{
	if ( pTreeItem == NULL )
	{
		return;
	}

	CControllerSurfaceObj *pControllerSurfaceObj =  GetControllerSurfaceObj();

	if ( pControllerSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();

		if ( lpSurface )
		{
			cltClient *pclclient = ( cltClient * )pclClient;

			SI16 CurrentDrawXPos = m_siOffsetX;
			SI16 CurrentDrawYPos = m_siOffsetY;

			SI16 SurfaceWidth = GetSurfaceWidth();
			SI16 SurfaceHeight = GetSurfaceHeight();

			SAFE_DELETE_A(m_ImgEncBuffer);

			m_ImgEncBuffer = new RGB16[ SurfaceWidth * SurfaceHeight * 2 ];

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			HDC hDC;
			HFONT hFont;
			if( lpSurface->GetDC( &hDC ) == DD_OK)
			{				
				DrawBorder( hDC, 0, 0, SurfaceWidth, SurfaceHeight );

				if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, GetFontKind() );
				}
				else {
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, GetFontKind() );
				}
				

				HFONT hOldFont = NULL;

				hOldFont = (HFONT)SelectObject( hDC, hFont );

				SetTextColor( hDC, GetFontColor() );

				SetBkMode( hDC, TRANSPARENT );

				if ( pTreeItem->strText )
				{
					pTreeItem->siTextPixelLen = GetTextExtent( hDC, pTreeItem->strText, _tcslen( pTreeItem->strText ) );
				}

				SelectObject( hDC, hOldFont );

				if ( hFont )
				{
					DeleteObject( hFont );
					hFont = NULL;
				}

				lpSurface->ReleaseDC( hDC );
			}
		}
	}

	m_TreeItemVector.push_back( *pTreeItem );

	return;
}

void CTree::UpdateItem( stTreeItem *pUpdateTreeItem )
{
	if( pUpdateTreeItem == NULL || pUpdateTreeItem->siIndex < 1 )
	{
		return;	
	}

	stTreeItem *pTreeItem = GetTreeItemFromIndex( pUpdateTreeItem->siIndex );

	if ( pTreeItem == NULL )
	{
		return;
	}

	memcpy( pTreeItem, pUpdateTreeItem, sizeof( stTreeItem ) );

	Refresh();

	return;
}

void CTree::MakeTreeImgEncBuffer()
{
	if ( ! IsAvailable() )
	{
		return;
	}

	if ( m_ImgEncBuffer )
	{
		delete [] m_ImgEncBuffer;
		m_ImgEncBuffer = NULL;
	}
	/*
	// 클리핑 영역을 설정한다. 
	SI32 x1, y1, x2, y2;
	GP.GetClipArea( x1, y1, x2, y2 );

	GP.SetClipArea( 0, 0, Spr.clHeader.siXsize-1, Spr.clHeader.siYsize-1 );

	if(GP.LockSurface( m_lpSmallMapSurface ) == TRUE )
	{
	Spr.PutImgT( 0, 0, 0 );
	GP.UnlockSurface( m_lpSmallMapSurface );
	}
	// 클리핑 영역을 복구한다.
	GP.SetClipArea( x1, y1, x2, y2 );
	*/

	cltClient *pclclient = ( cltClient * )pclClient;

	m_siMaxWidth = 0;

	SI32 x1, y1, x2, y2;
	GP.GetClipArea( x1, y1, x2, y2 );

	//GP.SetClipArea( m_siOffsetX + m_pHScroll->GetTrackPos(), m_siOffsetY + m_pVScroll->GetTrackPos(), m_siOffsetX + m_pHScroll->GetTrackPos() + SurfaceWidth, m_siOffsetY + m_pVScroll->GetTrackPos() + SurfaceHeight );

	SI16 SurfaceWidth = GetSurfaceWidth();
	SI16 SurfaceHeight = GetSurfaceHeight();

	GP.SetClipArea( 0, 0, SurfaceWidth - SCROLL_WIDTH, SurfaceHeight - SCROLL_HEIGHT );

	CControllerSurfaceObj *pControllerSurfaceObj =  GetControllerSurfaceObj();

	if ( pControllerSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();

		if ( lpSurface )
		{
			SI16 CurrentDrawXPos = m_siOffsetX - m_pHScroll->GetTrackPos();
			SI16 CurrentDrawYPos = m_siOffsetY - m_pVScroll->GetTrackPos() * m_siLineHeight;

			m_ImgEncBuffer = new RGB16[ SurfaceWidth * SurfaceHeight * 2 ];

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			HDC hDC;

			if( lpSurface->GetDC( &hDC ) == DD_OK)
			{
				HFONT hFont;
				DrawBorder( hDC, 0, 0, SurfaceWidth, SurfaceHeight );

				if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, GetFontKind() );
				}
				else {
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, GetFontKind() );
				}
				
				HPEN hPen = CreatePen( PS_SOLID, 1, RGB( 231, 189, 140 ) );

				HFONT hOldFont = NULL;
				HPEN hOldPen = NULL;

				hOldFont = (HFONT)SelectObject( hDC, hFont );
				hOldPen = (HPEN)SelectObject( hDC, hPen );

				SetTextColor( hDC, GetFontColor() );

				SetBkMode( hDC, TRANSPARENT );

				DrawInfoInit();

				vector< stTreeItem >::iterator itr;
				itr = m_TreeItemVector.begin();

				stTreeItem *pTreeItem = NULL;
				while ( itr != m_TreeItemVector.end() )
				{
					pTreeItem = &(*itr);
					if ( pTreeItem )
					{
						if ( pTreeItem->siParentIndex == 0 )
						{
							CurrentDrawXPos = m_siOffsetX  - m_pHScroll->GetTrackPos();

							if( ! DrawTree( hDC, pTreeItem, CurrentDrawXPos, CurrentDrawYPos ) )
							{
								break;
							}
						}
					}

					++itr;
				}

				SelectObject( hDC, hOldFont );
				SelectObject( hDC, hOldPen );

				if ( hPen )
				{
					DeleteObject( hPen );
					hPen = NULL;
				}

				if ( hFont )
				{
					DeleteObject( hFont );
					hFont = NULL;
				}

				lpSurface->ReleaseDC( hDC );
			}

			if ( GP.LockSurface( lpSurface ) )
			{				
				vector< stTreeItem >::iterator itr;
				itr = m_TreeItemVector.begin();

				TSpr *pSpr = NULL;
				stTreeItem *pTreeItem = NULL;
				while ( itr != m_TreeItemVector.end() )
				{
					pTreeItem = &(*itr);
					if ( pTreeItem )
					{
						if ( pTreeItem->sTreeItemDrawInfo.siDrawXPos > 0 && pTreeItem->sTreeItemDrawInfo.siDrawYPos > 0 &&
							pTreeItem->siFileIndex > -1 && pTreeItem->siImageIndex > -1
							)
						{	
							switch( pTreeItem->siFileIndex )
							{
							case 0:
								pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( SMALL_ICON_IMG_FILE_0 );
								break;
							case 1:
								pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( SMALL_ICON_IMG_FILE_1 );
								break;
							}
							GP.PutSpr(pSpr, pTreeItem->sTreeItemDrawInfo.siDrawXPos, pTreeItem->sTreeItemDrawInfo.siDrawYPos, pTreeItem->siImageIndex );
						}
					}

					++itr;
				}

				if(m_ImgEncBuffer)
				{
					int size;
					g_graphics.EncodeXpr( m_ImgEncBuffer, SurfaceWidth, SurfaceHeight, RGB16_TRANS, &size );
				}

				GP.UnlockSurface( lpSurface );
			}
		}
	}

	m_pHScroll->SetTrackRange( 0, m_siMaxWidth );

	if( m_siMaxWidth > GetSurfaceWidth() )
	{
		m_pHScroll->Show( true );
	}


	vector< stTreeItem >::size_type LineInfoListNum;
	LineInfoListNum = m_TreeItemVector.size();

	m_pVScroll->SetTrackRange( 0, LineInfoListNum );//* m_siLineHeight );

	if ( (SI16)LineInfoListNum * m_siLineHeight > GetSurfaceHeight() )
	{
		m_pVScroll->Show( true );
	}

	GP.SetClipArea( x1, y1, x2, y2 );

	return;
}

void CTree::Refresh()
{
	MakeTreeImgEncBuffer();
	return;
}

SI16 CTree::GetItemIndexFromPosition( SI16 XPos, SI16 YPos )
{
	if ( !IsInArea( XPos, YPos ) )
	{
		return 0;
	}

	XPos -= GetX();
	YPos -= GetY();

	vector< stTreeItem >::iterator itr;
	itr = m_TreeItemVector.begin();

	TSpr *pSpr = NULL;
	stTreeItem *pTreeItem = NULL;
	while ( itr != m_TreeItemVector.end() )
	{
		pTreeItem = &(*itr);
		if ( pTreeItem )
		{
			if ( pTreeItem->sTreeItemDrawInfo.siDrawXPos < XPos && pTreeItem->sTreeItemDrawInfo.siDrawXPos + pTreeItem->siTextPixelLen > XPos &&
				pTreeItem->sTreeItemDrawInfo.siDrawYPos < YPos && pTreeItem->sTreeItemDrawInfo.siDrawYPos + m_siLineHeight > YPos
				)
			{
				return pTreeItem->siIndex;
			}
		}
		++itr;
	}

	return 0;
}

stTreeItem *CTree::GetTreeItemFromIndex( SI16 Index )
{
	if ( Index < 1 )
	{
		return NULL;
	}

	vector< stTreeItem >::iterator itr;

	itr = m_TreeItemVector.begin();

	while ( itr != m_TreeItemVector.end() )
	{
		stTreeItem *pTreeItem = &(*itr);
		if( pTreeItem->siIndex == Index )
		{
			return pTreeItem;
		}

		++itr;
	}

	return NULL;
}

stTreeItem *CTree::GetParentTreeItemFromIndex( SI16 ParentTreeItemIndex )
{
	if ( ParentTreeItemIndex < 1 )
	{
		return NULL;
	}

	vector< stTreeItem >::iterator itr;

	itr = m_TreeItemVector.begin();

	while ( itr != m_TreeItemVector.end() )
	{
		stTreeItem *pTreeItem = &(*itr);
		if( pTreeItem->siIndex == ParentTreeItemIndex )
		{
			return pTreeItem;
		}

		++itr;
	}

	return NULL;
}

stTreeItem *CTree::GetPreTreeItemFromIndex( SI16 ParentIndex, SI16 MyIndex )
{
	if ( ParentIndex < 1 )
	{
		return NULL;
	}

	vector< stTreeItem >::iterator itr;

	itr = m_TreeItemVector.begin();

	stTreeItem *RetTreeItem = NULL;

	while ( itr != m_TreeItemVector.end() )
	{
		stTreeItem *pTreeItem = &(*itr);
		if( pTreeItem->siParentIndex == ParentIndex )
		{
			if ( RetTreeItem == NULL )
			{
				if ( pTreeItem->siIndex == MyIndex )
				{
					return NULL;
				}
				else
				{
					RetTreeItem = pTreeItem;
				}
			}
			else
			{
				if ( pTreeItem->siIndex == MyIndex )
				{
					return RetTreeItem;
				}
				else
				{
					RetTreeItem = pTreeItem;
				}
			}
		}

		++itr;
	}

	return NULL;
}

bool CTree::IsHaveParentItem( stTreeItem *pTreeItem )
{
	if ( pTreeItem == NULL )
	{
		return false;
	}

	if ( pTreeItem->siParentIndex > 0 )
	{
		return true;
	}

	return false;
}


void CTree::DrawInfoInit()
{
	vector< stTreeItem >::iterator itr;
	itr = m_TreeItemVector.begin();

	stTreeItem *pTreeItem = NULL;
	while ( itr != m_TreeItemVector.begin() )
	{
		pTreeItem = &(*itr);
		if ( pTreeItem )
		{
			pTreeItem->sTreeItemDrawInfo.Init();
		}
		++itr;
	}

	return;
}

bool CTree::DrawTree( HDC hDC, stTreeItem *pTreeItem, SI16 DrawXPos, SI16 &DrawYPos )
{
	if ( pTreeItem == NULL )
	{
		return false;
	}

	/*
	if ( DrawYPos + m_siLineHeight >= GetSurfaceHeight() )
	{
	return false;
	}
	*/

	stTreeItem *pTempTreeItem = NULL;
	if ( pTreeItem )
	{
		if ( ! DrawTreeItem( hDC, pTreeItem, DrawXPos, DrawYPos ) )
		{
			return false;
		}

		DrawYPos += m_siLineHeight;

		vector< stTreeItem >::iterator itr;
		itr = m_TreeItemVector.begin();

		while ( itr != m_TreeItemVector.end() )
		{
			pTempTreeItem = &(*itr);

			if ( pTempTreeItem && pTempTreeItem->siParentIndex == pTreeItem->siIndex )
			{
				if ( pTreeItem->siParentIndex == 0 )
				{
					if ( ! DrawTree( hDC, pTempTreeItem, DrawXPos + m_siLineSize, DrawYPos ) )
					{
						return false;
					}
				}
				else
				{
					if ( ! DrawTree( hDC, pTempTreeItem, DrawXPos + m_siLineSize * 2, DrawYPos ) )
					{
						return false;
					}
				}
			}

			++itr;
		}
	}

	return true;
}

bool CTree::DrawTreeItem( HDC hDC, stTreeItem *pTreeItem, SI16 DrawXPos, SI16 DrawYPos )
{
	if ( pTreeItem == NULL )
	{
		return false;
	}

	/*
	if ( DrawYPos + m_siLineHeight >= GetSurfaceHeight() )
	{
	return false;
	}
	*/

	if ( pTreeItem->siParentIndex > 0 )		// Line 그려 줘야징...
	{
		stTreeItem *pPreTreeItem = GetPreTreeItemFromIndex( pTreeItem->siParentIndex, pTreeItem->siIndex );
		if ( pPreTreeItem )
		{
			DrawLinkLine( hDC, pPreTreeItem, pTreeItem, DrawXPos, DrawYPos );
		}

		DrawConnectLine( hDC, pTreeItem, DrawXPos, DrawYPos );
		DrawXPos += m_siLineSize;
	}

	pTreeItem->sTreeItemDrawInfo.siDrawXPos = DrawXPos;
	pTreeItem->sTreeItemDrawInfo.siDrawYPos = DrawYPos;

	if ( pTreeItem->siFileIndex != -1 && pTreeItem->siImageIndex != -1 )
	{	
		DrawXPos += TREEITEM_ICON_SIZE;
	}	

	DrawTreeItemText( hDC, pTreeItem, DrawXPos, DrawYPos );

	return true;
}

void CTree::DrawLinkLine( HDC hDC, stTreeItem *pPreTreeItem, stTreeItem *pTreeItem, SI16 DrawXPos, SI16 DrawYPos )
{
	if ( pPreTreeItem == NULL || pTreeItem == NULL )
	{	
		return;
	}

	/*
	if ( DrawYPos + m_siLineHeight >= GetSurfaceHeight() )
	{
	return;
	}

	if ( DrawXPos >= GetSurfaceWidth() )
	{
	return;
	}
	*/

	if ( DrawXPos < m_siOffsetX || DrawXPos > m_siAvailabeUseWidth )
	{
		return;
	}

	SI16 TempDrawYPos = DrawYPos;

	while( TempDrawYPos > pPreTreeItem->sTreeItemDrawInfo.siDrawYPos )
	{
		if( TempDrawYPos < m_siOffsetY || TempDrawYPos > m_siAvailabeUseHeight )
		{
			TempDrawYPos -= m_siLineHeight;
			continue;
		}

		MoveToEx( hDC, DrawXPos, TempDrawYPos - m_siLineHeight, NULL );
		LineTo( hDC, DrawXPos, TempDrawYPos );

		TempDrawYPos -= m_siLineHeight;
	}

	return;
}

void CTree::DrawConnectLine( HDC hDC, stTreeItem *pTreeItem, SI16 DrawXPos, SI16 DrawYPos )
{
	if ( pTreeItem == NULL )
	{	
		return;
	}

	if ( DrawYPos + m_siLineHeight >= GetSurfaceHeight() )
	{
		return;
	}

	if ( DrawXPos >= GetSurfaceWidth() )
	{
		return;
	}

	MoveToEx( hDC, DrawXPos, DrawYPos, NULL );
	LineTo( hDC, DrawXPos, DrawYPos + m_siLineSize );

	MoveToEx( hDC, DrawXPos, DrawYPos + m_siLineSize, NULL );
	LineTo( hDC, DrawXPos + m_siLineSize, DrawYPos + m_siLineSize );

	return;
}

void CTree::DrawTreeItemText( HDC hDC, stTreeItem *pTreeItem, SI16 DrawXPos, SI16 DrawYPos )
{
	RECT Rect;
	Rect.left = DrawXPos;
	Rect.top = DrawYPos;
	Rect.right = DrawXPos + pTreeItem->siTextPixelLen;
	Rect.bottom = DrawYPos + m_siLineHeight;

	if ( m_siMaxWidth < Rect.right )
	{
		m_siMaxWidth = Rect.right;
	}

	if ( DrawYPos + m_siLineHeight >= GetSurfaceHeight() )
	{
		return;
	}

	if ( DrawXPos + pTreeItem->siTextPixelLen >= GetSurfaceWidth() - m_siOffsetX )
	{
		Rect.right = GetSurfaceWidth() - m_siOffsetX;
		//return;
	}

	if ( pTreeItem && pTreeItem->strText )
	{
		DrawText( hDC, pTreeItem->strText, -1, &Rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	}

	return;
}


void CTree::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	cltClient *pclclient = (cltClient *)pclClient;

	TSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );

	if ( pBkImg )
	{
		if ( IsBKTileImg() )
		{
			DrawTileImg( pBkImg, GetDrawOption(), GetFontIndex(), GetAlphaValue(), BaseXPos + GetX(), BaseYPos + GetY(), GetSurfaceWidth(), GetSurfaceHeight(), 9 );
		}
		else
		{
			switch( GetDrawOption() )
			{
			case SPR_PUTSPRT:		GP.PutSpr(pBkImg, BaseXPos + GetX(), BaseYPos + GetY(), GetFontIndex() );						break;
			case SPR_PUTSPRALPHAT:	GP.PutSprAlpha(pBkImg, BaseXPos + GetX(), BaseYPos + GetY(), GetAlphaValue(), GetFontIndex() );	break;
			}
			
		}
	}

	if ( m_ImgEncBuffer )
	{
		switch( GetDrawOption() )
		{
		case SPR_PUTSPRT:		
			{
				GP.PutXpr16( BaseXPos + GetX(), BaseYPos + GetY(), 
					GetSurfaceWidth(), GetSurfaceHeight(),
					m_ImgEncBuffer,
					RGB16_TRANS );
			}
			break;
		case SPR_PUTSPRALPHAT:
			{
				GP.PutXpr16A( BaseXPos + GetX(), BaseYPos + GetY(), 
					GetSurfaceWidth(), GetSurfaceHeight(),
					m_ImgEncBuffer,
                    GetAlphaValue(),
					RGB16_TRANS );
			}
			break;
		}
	}

	if ( m_siSelectedIndex >= 0 )
	{	
		TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg(  TEXT("NInterface/Common/Common_4X14_00_103.Spr") );
		if ( pSpr )
		{
			stTreeItem *pTreeItem = GetTreeItemFromIndex( m_siSelectedIndex );
			if ( pTreeItem )
			{
				/*
				if (
				pTreeItem->sTreeItemDrawInfo.siDrawXPos > 0 &&
				pTreeItem->sTreeItemDrawInfo.siDrawXPos < GetSurfaceWidth() &&
				pTreeItem->sTreeItemDrawInfo.siDrawYPos > 0 && 
				pTreeItem->sTreeItemDrawInfo.siDrawYPos < GetSurfaceHeight()
				)
				*/
				{
					if ( pTreeItem->sTreeItemDrawInfo.siDrawXPos < m_siOffsetX )
					{
						DrawTileImg(
							pSpr, SPR_PUTSPRALPHAT, 3, 8,
							BaseXPos + GetX() + m_siOffsetX,
							BaseYPos + GetY() + pTreeItem->sTreeItemDrawInfo.siDrawYPos,
							m_siOffsetX + pTreeItem->sTreeItemDrawInfo.siDrawXPos + pTreeItem->siTextPixelLen + TREEITEM_ICON_SIZE > GetSurfaceWidth() - m_siOffsetX ? GetSurfaceWidth() - m_siOffsetX - m_siOffsetX : pTreeItem->sTreeItemDrawInfo.siDrawXPos + pTreeItem->siTextPixelLen + TREEITEM_ICON_SIZE,
							pSpr->GetYSize(),
							3
							);
					}
					else
					{
						DrawTileImg(
							pSpr, SPR_PUTSPRALPHAT, 3, 8,
							BaseXPos + GetX() + pTreeItem->sTreeItemDrawInfo.siDrawXPos,
							BaseYPos + GetY() + pTreeItem->sTreeItemDrawInfo.siDrawYPos,
							pTreeItem->sTreeItemDrawInfo.siDrawXPos + pTreeItem->siTextPixelLen + TREEITEM_ICON_SIZE > GetSurfaceWidth() - m_siOffsetX ? GetSurfaceWidth() - m_siOffsetX - pTreeItem->sTreeItemDrawInfo.siDrawXPos : pTreeItem->siTextPixelLen + TREEITEM_ICON_SIZE,
							pSpr->GetYSize(),
							3
							);
					}
				}
			}			
		}
	}

	if ( m_pVScroll->IsShow() )
	{
		m_pVScroll->DrawController( BaseXPos, BaseYPos, LeftTime );
	}

	if ( m_pHScroll->IsShow() )
	{
		m_pHScroll->DrawController( BaseXPos, BaseYPos, LeftTime );
	}

	return;
}