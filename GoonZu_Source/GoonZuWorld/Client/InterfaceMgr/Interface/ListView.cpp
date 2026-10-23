#include "ListView.h"
#include "ToolTip.h"

#include "../ControllerMgr.h"
#include "../ControllerSurfaceObj.h"

#include "../InterfaceMgr.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

#include <zmouse.h>

#include <Client.h>
extern cltCommonLogic *pclClient;

CListView::CListView( CControllerMgr *pParent ) : CControllerObj( pParent )
{	
	Init();
	m_pScroll = new CScroll( pParent );
}

CListView::CListView( CControllerMgr *pParent,SI32 siTmpBufferSize ) : CControllerObj( pParent,siTmpBufferSize )
{
	Init();
	m_pScroll = new CScroll( pParent );
}

void CListView::Init()
{
	m_bLMPressed = false;
	m_bRMPressed = false;

	m_siSelectedIndex = -1;
	m_siLastSelectedIndex = -1;
	m_siMouseOverItemIndex = -1;

	m_siMaxHorizontalItemNum = 0;

	m_bUseToolTip = false;

	m_siListViewOffsetX = 0;
	m_siListViewOffsetY = 0;

	m_siListViewItemOffsetX = 0;
	m_siListViewItemOffsetY = 0;

	m_siListViewItemSizeWidth = 0;
	m_siListViewItemSizeHeight = 0;

	m_bItemExplainShow = false;

	m_bItemNumShow = false;

	m_bAllowEmptyItemPos = false;

	m_siListViewItemCounter = 0;

	for ( SI16 i = 0; i < MAX_LISTVIEW_ITEM_NUM; ++i )
	{
		m_pListViewItemData[ i ] = NULL;
	}

	m_siListViewStartItemPos = 0;

	m_ImgEncBuffer = NULL;

	m_siPreScrollPos = 0;
}

CListView::~CListView()
{
	if ( m_ImgEncBuffer )
	{
		delete [] m_ImgEncBuffer;
		m_ImgEncBuffer = NULL;
	}

	for ( SI16 i = 0; i < MAX_LISTVIEW_ITEM_NUM; ++i )
	{
		if ( m_pListViewItemData[ i ] )
		{
			delete m_pListViewItemData[ i ];
			m_pListViewItemData[ i ] = NULL;
		}
	}

	SAFE_DELETE( m_pScroll );
}

void CListView::Create( SI16 ControllerID, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum,
					   SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, 
					   SI16 SurfaceWidth, SI16 SurfaceHeight, 
					   SI16 ListViewOffsetX, SI16 ListViewOffsetY,
					   SI16 ListViewItemOffsetX, SI16 ListViewItemOffsetY,
					   SI16 ListViewItemSizeWidth, SI16 ListViewItemSizeHeight,
					   bool AllowEmptyItemPos, bool ItemNumShow, bool UseToolTip, SI16 ToolTipWidth, SI16 ToolTipHeight )
{
	m_siListViewOffsetX = ListViewOffsetX;
	m_siListViewOffsetY = ListViewOffsetY;

	m_siListViewItemOffsetX = ListViewItemOffsetX;
	m_siListViewItemOffsetY = ListViewItemOffsetY;

	m_siListViewItemSizeWidth = ListViewItemSizeWidth;
	m_siListViewItemSizeHeight = ListViewItemSizeHeight;

	m_bAllowEmptyItemPos = AllowEmptyItemPos;

	m_bItemNumShow = ItemNumShow;

	m_bUseToolTip = UseToolTip;

	m_siMaxHorizontalItemNum = Width / ( m_siListViewItemSizeWidth + m_siListViewItemOffsetX );
	
	if ( ( Width % ( m_siListViewItemSizeWidth + m_siListViewItemOffsetX ) ) / m_siListViewItemSizeWidth == 1 )
	{
		++m_siMaxHorizontalItemNum;
	}
	else
	{
	
	}

#ifdef _DEBUG
	if ( m_siMaxHorizontalItemNum < 1 )
	{
		OutputDebugString(  TEXT("리스트박스 작다.\r\n") );
	}
#endif

	CControllerObj::Create( ControllerID, CONTROLLER_KIND_LISTVIEW, FileName, FontIndex, BKTileImg, BKTileImgNum, XPos, YPos, Width, Height, SurfaceWidth, SurfaceHeight );

	m_pScroll->Create( COMMON_SCROLL, this, NULL, 0, XPos + Width - SCROLL_WIDTH - 1, YPos, SCROLL_WIDTH, Height, SCROLL_WIDTH, Height, ListViewItemSizeHeight );	
}

bool CListView::InsertItem( stListViewItemData *pItemData )
{
	if ( pItemData->ItemListViewPos < 0 || pItemData->ItemListViewPos >= MAX_LISTVIEW_ITEM_NUM )
	{
		return false;
	}

	if ( m_pListViewItemData[ pItemData->ItemListViewPos ] == NULL )
	{
		m_pListViewItemData[ pItemData->ItemListViewPos ] = new stListViewItemData( pItemData );
		++m_siListViewItemCounter;

		UpdateScrollPos();

		return true;
	}

    return false;
}

bool CListView::UpdateItem( stListViewItemData *pItemData )
{
	if ( pItemData->ItemListViewPos < 0 || pItemData->ItemListViewPos >= MAX_LISTVIEW_ITEM_NUM )
	{
		return false;
	}

	if ( m_pListViewItemData[ pItemData->ItemListViewPos ] == NULL )
	{
		m_pListViewItemData[ pItemData->ItemListViewPos ] = new stListViewItemData( pItemData );

		return true;
	}

	return false;
}

bool CListView::DeleteItem( SI16 ItemListViewPos )
{
	if ( ItemListViewPos < 0 || ItemListViewPos >= MAX_LISTVIEW_ITEM_NUM )
	{
		return false;
	}

	if ( m_pListViewItemData[ ItemListViewPos ] )
	{
		delete m_pListViewItemData[ ItemListViewPos ];
		m_pListViewItemData[ ItemListViewPos ] = NULL;

		--m_siListViewItemCounter;

		UpdateScrollPos();

		if ( ItemListViewPos == m_siSelectedIndex )
		{
			ClearSelectedItem();
		}

		return true;
	}

	return false;
}

void CListView::DeleteAllItems()
{
	m_siPreScrollPos = m_pScroll->GetTrackPos();

	for ( SI16 i = 0; i < MAX_LISTVIEW_ITEM_NUM; ++i )
	{
		if ( m_pListViewItemData[ i ] )
		{
			delete m_pListViewItemData[ i ];
			m_pListViewItemData[ i ] = NULL;
		}
	}

	m_siListViewItemCounter = 0;

	ClearSelectedItem();

	return;
}

bool CListView::ChangeItemNum( SI16 ItemListViewPos, SI16 ItemNum )
{
	if ( ItemListViewPos < 0 || ItemListViewPos >= MAX_LISTVIEW_ITEM_NUM ||
		ItemNum < 1 || ItemNum >= MAX_LISTVIEW_DUPLICATE_ITEM_NUM
		)
	{
		return false;
	}

	if ( m_pListViewItemData[ ItemListViewPos ] )
	{
		m_pListViewItemData[ ItemListViewPos ]->ItemNum = ItemNum;

		return true;
	}

	return false;
}

bool CListView::ChangeItemPos( SI16 PreItemPos, SI16 NextItemPos )
{
	PreItemPos -= m_siListViewStartItemPos;
	NextItemPos -= m_siListViewStartItemPos;

	if ( PreItemPos < 0 || PreItemPos >= MAX_LISTVIEW_ITEM_NUM ||
		NextItemPos < 0 || NextItemPos >= MAX_LISTVIEW_ITEM_NUM )
	{
		return false;
	}

	if ( m_bAllowEmptyItemPos )
	{
		if ( m_pListViewItemData[ PreItemPos ] )
		{
			if ( m_pListViewItemData[ NextItemPos ] )
			{
				stListViewItemData Temp;
				Temp.Set( m_pListViewItemData[ PreItemPos ] );
				m_pListViewItemData[ PreItemPos ]->Set( m_pListViewItemData[ NextItemPos ] );
				m_pListViewItemData[ NextItemPos ]->Set( &Temp );

				m_pListViewItemData[ PreItemPos ]->ItemListViewPos = PreItemPos;
				m_pListViewItemData[ NextItemPos ]->ItemListViewPos = NextItemPos;
			}
			else
			{
				m_pListViewItemData[ NextItemPos ] = new stListViewItemData( m_pListViewItemData[ PreItemPos ] );
				m_pListViewItemData[ NextItemPos ]->ItemListViewPos = NextItemPos;
				delete m_pListViewItemData[ PreItemPos ];
				m_pListViewItemData[ PreItemPos ] = NULL;
			}
		}
		else if ( m_pListViewItemData[ NextItemPos ] )
		{
			m_pListViewItemData[ PreItemPos ] = new stListViewItemData( m_pListViewItemData[ NextItemPos ] );
			m_pListViewItemData[ PreItemPos ]->ItemListViewPos = PreItemPos;
			delete m_pListViewItemData[ NextItemPos ];
			m_pListViewItemData[ NextItemPos ] = NULL;
		}
		else
		{	
			return false;
		}
	}
	else
	{

	}
	
	return true;
}

void CListView::SetFocus( bool HasFocus )
{
	cltClient *pclclient = ( cltClient *)pclClient;
	CControllerObj::SetFocus( HasFocus );

	if ( ! HasFocus )
	{
		ClearSelectedItem();
		pclclient->m_pInterfaceMgr->GetToolTip()->SetToolTip( NULL );
	}

	return;
}

void CListView::ClearSelectedItem()
{
	m_siSelectedIndex = -1;
	m_siLastSelectedIndex = -1;
	m_siMouseOverItemIndex = -1;

	UpdateScrollPos();

	return;
}

bool CListView::HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam )
{
	if ( ! IsAvailable() )
		return false;

	cltClient *pclclient = (cltClient *)pclClient;

	//POINT ListBoxScrollPt;
	//ListBoxScrollPt.x = pt.x - GetX();
	//ListBoxScrollPt.y = pt.y - GetY();

	//if( m_pScroll->HandleMouse( uMsg, ListBoxScrollPt, wParam, lParam ) )
	if( m_pScroll->HandleMouse( uMsg, pt, wParam, lParam ) )
	{
		if( IsInArea( pt.x, pt.y ) )
		{	
			m_bLMPressed = true;

			GetParent()->SendEvent( EVENT_LISTVIEW_LBUTTONDOWN, true, this );
  
			if( !IsFocus() )
			{	
				GetParent()->RequestFocus( this );
			}

			return true;
		}
	}
	
	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		{
			if( IsInArea( pt.x, pt.y ) )
			{	
				if ( pclclient->m_pInterfaceMgr->IsItemDragMoveStatus() )
				{
					int a = 0;
				}

				m_bLMPressed = true;
				m_siSelectedIndex = GetMouseSelectedItemIndex( pt.x, pt.y );

				GetParent()->SendEvent( EVENT_LISTVIEW_LBUTTONDOWN, true, this );
				
				if( ! IsFocus() )
				{	
					GetParent()->RequestFocus( this );
				}

				return true;
			}
			else
			{
				if ( pclclient->m_pInterfaceMgr->IsItemDragMoveStatus() )
				{	
					m_bLMPressed = false;
					m_siSelectedIndex = -1;
					m_siLastSelectedIndex = -1;

					GetParent()->SendEvent( EVENT_LISTVIEW_LBUTTONDOWN, true, this );

					return true;
				}
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if( IsInArea( pt.x, pt.y ) )
			{	
				SI16 MouseSelectedItemIndex = GetMouseSelectedItemIndex( pt.x, pt.y );
				if ( MouseSelectedItemIndex == m_siSelectedIndex )
				{
					m_siSelectedIndex = MouseSelectedItemIndex;
				}
				else
				{
					m_siSelectedIndex = -1;
					m_siLastSelectedIndex = -1;
				}

				cltClickAndDrop* pclClickAndDrop = pclclient->m_pInterfaceMgr->GetClickAndDrop();
				if( pclClickAndDrop )
				{
					pclclient->m_pInterfaceMgr->EndClickAndDrop();
				}

				GetParent()->SendEvent( EVENT_LISTVIEW_DBLCLK, true, this );
			}
			else
			{
				if ( pclclient->m_pInterfaceMgr->IsItemDragMoveStatus() )
				{
					m_bLMPressed = false;
					m_siSelectedIndex = -1;
					m_siLastSelectedIndex = -1;

					return true;
				}
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if( m_bLMPressed )
			{
				m_bLMPressed = false;

				if( IsInArea( pt.x, pt.y ) )
				{	
					SI16 MouseSelectedItemIndex = GetMouseSelectedItemIndex( pt.x, pt.y );

					if(pclClient->IsCountrySwitch(Switch_ClickAndDrop))
					{
						cltClickAndDrop* pclClickAndDrop = pclclient->m_pInterfaceMgr->GetClickAndDrop();
						if( pclClickAndDrop )
						{
							pclClickAndDrop->m_siDstDialogID = GetParent()->GetDialogControllerID();
							pclClickAndDrop->m_siDstControlID = GetControllerID();
							if (GetParent()->EndClickAndDrop(pclClickAndDrop) )
								pclclient->m_pInterfaceMgr->EndClickAndDrop();
						}
						else
						{
							GetParent()->SendEvent( EVENT_LISTVIEW_LBUTTONUP, true, this );
						}
					}
					else
					{
						if ( MouseSelectedItemIndex == m_siSelectedIndex )
						{
							m_siSelectedIndex = MouseSelectedItemIndex;
						}
						else
						{
							m_siSelectedIndex = -1;
							m_siLastSelectedIndex = -1;
						}

						GetParent()->SendEvent( EVENT_LISTVIEW_LBUTTONUP, true, this );
					}
				}
				else
				{
					if ( pclclient->m_pInterfaceMgr->IsItemDragMoveStatus() )
					{
						m_bLMPressed = false;
						m_siSelectedIndex = -1;
						m_siLastSelectedIndex = -1;
						GetParent()->SendEvent( EVENT_LISTVIEW_LBUTTONUP, true, this );
					}
				}

				return true;
			}
		}
		break;
	case WM_RBUTTONDOWN:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				
				m_bRMPressed = true;
				m_siSelectedIndex = GetMouseSelectedItemIndex( pt.x, pt.y );
				GetParent()->SendEvent( EVENT_LISTVIEW_RBUTTONDOWN, true, this );

				if( ! IsFocus() )
				{
					GetParent()->RequestFocus( this );
				}

				return true;
			}
			else
			{
				if ( pclclient->m_pInterfaceMgr->IsItemDragMoveStatus() )
				{	
					m_bRMPressed = false;
					m_siSelectedIndex = -1;
					m_siLastSelectedIndex = -1;

					GetParent()->SendEvent( EVENT_LISTVIEW_RBUTTONDOWN, true, this );

					return true;
				}
			}
		}
		break;
	case WM_RBUTTONUP:
		{
			if( m_bRMPressed )
			{
				m_bRMPressed = false;
				
				if( IsInArea( pt.x, pt.y ) )
				{
					SI16 MouseSelectedItemIndex = GetMouseSelectedItemIndex( pt.x, pt.y );
					if ( MouseSelectedItemIndex == m_siSelectedIndex )
					{
						m_siSelectedIndex = MouseSelectedItemIndex;
					}
					else
					{
						m_siSelectedIndex = -1;
					}

					GetParent()->SendEvent( EVENT_LISTVIEW_RBUTTONUP, true, this );
				}

				return true;
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			if ( m_bLMPressed || m_bRMPressed )
			{
				break;
			}

			if( IsInArea( pt.x, pt.y ) )
			{
				m_siMouseOverItemIndex = GetMouseSelectedItemIndex( pt.x, pt.y );
				if ( m_siMouseOverItemIndex >= 0 && m_pListViewItemData[ m_siMouseOverItemIndex ] )
				{
					pclclient->m_pInterfaceMgr->GetToolTip()->SetToolTip( m_pListViewItemData[ m_siMouseOverItemIndex ]->ToolTipMsg );
				}
			}
			else
			{
				m_siMouseOverItemIndex = -1;
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				int zDelta = (short) HIWORD(wParam) / WHEEL_DELTA;
				m_pScroll->Scroll( -zDelta );

				GetParent()->SendEvent( EVENT_SCROLL_MOUSEMOVE, true, this );

				return true;
			}
		}
		break;
	}

	return false;
}

SI16 CListView::GetSelectedItemIndex()
{
	if ( m_siSelectedIndex != -1 )
	{
		return m_siSelectedIndex + m_siListViewStartItemPos;
	}

	return -1;
}

SI16 CListView::GetLastSelectedItemIndex()
{
	if ( m_siLastSelectedIndex != -1 )
	{
		return m_siLastSelectedIndex + m_siListViewStartItemPos;
	}

	return -1;
}

SI16 CListView::GetMouseSelectedItemIndex( LONG XPos, LONG YPos )
{	
	if ( ! IsInArea( XPos, YPos ) )
	{
		return -1;
	}
	
	SI16 Row = ( YPos - GetY() ) / ( m_siListViewItemSizeHeight + m_siListViewItemOffsetY );
	if ( ( YPos - GetY() ) % ( m_siListViewItemSizeHeight + m_siListViewItemOffsetY ) != 0 )
	{
		++Row;
	}
	--Row;		

	SI16 Column = ( XPos - GetX() ) / ( m_siListViewItemSizeWidth + m_siListViewItemOffsetX );
	if ( ( XPos - GetX() ) % ( m_siListViewItemSizeWidth + m_siListViewItemOffsetX ) != 0 )
	{
		++Column;
	}
	--Column;	
	

	if ( Row < 0 || Column < 0 )
	{
		return -1;
	}

	SI16 CurrentSelectedIndex = m_siMaxHorizontalItemNum * Row + Column;

	if ( m_bAllowEmptyItemPos )
	{
		m_siLastSelectedIndex = CurrentSelectedIndex + m_pScroll->GetTrackPos() * m_siMaxHorizontalItemNum;

		if ( m_pListViewItemData[ m_siLastSelectedIndex ] )
		{
			return m_siLastSelectedIndex;
		}
		else
		{	
			return -1;
		}
	}
	else
	{
		bool FindItem = false;
		SI16 CounterIndex = 0;

		CurrentSelectedIndex += m_pScroll->GetTrackPos() * m_siMaxHorizontalItemNum;

		SI16 i = 0;
		for ( i = 0; i < MAX_LISTVIEW_ITEM_NUM; ++i )
		{
			if ( m_pListViewItemData[ i ] )
			{
				if ( CounterIndex == CurrentSelectedIndex )
				{
					FindItem = true;
					break;
				}
				++CounterIndex;
			}
		}

		if ( FindItem )
		{
			m_siLastSelectedIndex = i;

			if ( m_pListViewItemData[ m_siLastSelectedIndex ] )
			{
				return m_siLastSelectedIndex;
			}
			else
			{	
				return -1;
			}
		}
	}

	return -1;
}

SI16 CListView::GetItemIndexFromPoint( POINT pt, bool AbsolutePoint )
{
	LONG XPos, YPos;

	if ( AbsolutePoint )
	{
		CControllerMgr *pTemp = GetParent();
		while( pTemp )
		{
			pt.x -= pTemp->GetX();
			pt.y -= pTemp->GetY();

			if ( pTemp->GetDialogControllerKind() == CONTROLLER_KIND_POPUPDIALOG )
			{
				break;
			}
			else
			{	
				pTemp = pTemp->GetParentDialog();
			}
		}
	}

	XPos = pt.x;
	YPos = pt.y;

	if ( ! IsInArea( XPos, YPos ) )
	{
		return -1;
	}

	SI16 Row = ( YPos - GetY() ) / ( m_siListViewItemSizeHeight + m_siListViewItemOffsetY );
	if ( ( YPos - GetY() ) % ( m_siListViewItemSizeHeight + m_siListViewItemOffsetY ) != 0 )
	{
		++Row;
	}
	--Row;		

	SI16 Column = ( XPos - GetX() ) / ( m_siListViewItemSizeWidth + m_siListViewItemOffsetX );
	if ( ( XPos - GetX() ) % ( m_siListViewItemSizeWidth + m_siListViewItemOffsetX ) != 0 )
	{
		++Column;
	}
	--Column;	


	if ( Row < 0 || Column < 0 )
	{
		return -1;
	}

	SI16 CurrentSelectedIndex = m_siMaxHorizontalItemNum * Row + Column;

	if ( m_bAllowEmptyItemPos )
	{
		CurrentSelectedIndex = CurrentSelectedIndex + m_pScroll->GetTrackPos() * m_siMaxHorizontalItemNum;

		if ( CurrentSelectedIndex > -1 )
		{
			return m_siListViewStartItemPos + CurrentSelectedIndex;
		}
		else
		{	
			return -1;
		}
	}
	else
	{
		bool FindItem = false;
		SI16 CounterIndex = 0;

		CurrentSelectedIndex += m_pScroll->GetTrackPos() * m_siMaxHorizontalItemNum;

		SI16 i = 0;
		for ( i = 0; i < MAX_LISTVIEW_ITEM_NUM; ++i )
		{
			if ( m_pListViewItemData[ i ] )
			{
				if ( CounterIndex == CurrentSelectedIndex )
				{
					FindItem = true;
					break;
				}
				++CounterIndex;
			}
		}

		if ( FindItem )
		{
			CurrentSelectedIndex = i;

			if ( m_pListViewItemData[ CurrentSelectedIndex ] )
			{
				return m_siListViewStartItemPos + CurrentSelectedIndex;
			}
			else
			{	
				return -1;
			}
		}
	}

	return -1;
}

void CListView::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	
	if( ! IsShow() )
	{
		return;
	}

	if ( IsBorder() || IsBKColorUse() )
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

	TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );
	if ( pSpr )
	{
		switch( GetDrawOption() )
		{
		case SPR_PUTSPRT:		GP.PutSpr(pSpr, BaseXPos + GetX(), BaseYPos + GetY(), GetFontIndex() );							break;
		case SPR_PUTSPRALPHAT:	GP.PutSprAlpha(pSpr, BaseXPos + GetX(), BaseYPos + GetY(), GetAlphaValue(), GetFontIndex() );	break;
		}
	}
	
	SI16 ItemCounter = 0;

	SI16 DrawXPos = 0;
	SI16 DrawYPos = 0;	

	if ( m_bAllowEmptyItemPos )
	{
		for ( SI16 i = m_pScroll->GetTrackPos() * m_siMaxHorizontalItemNum; i < MAX_LISTVIEW_ITEM_NUM; ++i )
		{
			if ( m_pListViewItemData[ i ] )
			{
				DrawXPos = m_siListViewOffsetX + BaseXPos + GetX() + ( ItemCounter % m_siMaxHorizontalItemNum ) * ( m_siListViewItemSizeWidth + m_siListViewItemOffsetX );
				DrawYPos = m_siListViewOffsetY + BaseYPos + GetY() + ( ItemCounter / m_siMaxHorizontalItemNum ) * ( m_siListViewItemSizeHeight + m_siListViewItemOffsetY );

				if ( DrawYPos + m_siListViewItemSizeHeight > BaseYPos + GetY() + GetHeight() )
				{
					break;
				}

				if ( m_siSelectedIndex == i || m_pListViewItemData[ i ]->bDrawAlphaOption )
				{
					pSpr = pclclient->pclGImg->GetGlobalSpr( m_pListViewItemData[ i ]->ImageFileIndex );

					if ( pSpr )
					{
						GP.PutSprEF(pSpr, DrawXPos, DrawYPos, 2, m_pListViewItemData[ i ]->ImageFontIndex );
						
					}

					if ( m_bItemNumShow )
					{
						DrawItemNumber( DrawXPos, DrawYPos + 29, m_pListViewItemData[ i ]->ItemNum );
					}
				}
				else
				{
					pSpr = pclclient->pclGImg->GetGlobalSpr( m_pListViewItemData[ i ]->ImageFileIndex );

					if ( pSpr )
					{
						switch( GetDrawOption() )
						{
						case SPR_PUTSPRT:		GP.PutSpr(pSpr, DrawXPos, DrawYPos, m_pListViewItemData[ i ]->ImageFontIndex );							break;
						case SPR_PUTSPRALPHAT:	GP.PutSprAlpha(pSpr, DrawXPos, DrawYPos, GetAlphaValue(), m_pListViewItemData[ i ]->ImageFontIndex );	break;
						}
					}

					if ( m_bItemNumShow )
					{
						DrawItemNumber( DrawXPos, DrawYPos + 29, m_pListViewItemData[ i ]->ItemNum );
					}
				}

				//cyj 봉인이미지 추가
				/*if (m_pListViewItemData[ i ]->bItemSealed)
				{
					pSpr = pclclient->pclGImg->GetGlobalSpr( GIMG_ITEMSEALED );
					DrawXPos = DrawXPos - 3;

					if ( pSpr )
					{
						//이미지 리소스 분리 작업 [2007.07.18 손성웅.]
						GP.PutSpr(pSpr, DrawXPos, DrawYPos, 0);
					}
				}*/

				if(m_pListViewItemData[ i ]->siItemMask & LISTVIEW_ITEMMASK_SEALED)
				{
					pSpr = pclclient->pclGImg->GetGlobalSpr( GIMG_ITEMSEALED );
					SI32 siTempXPos = DrawXPos - 3;
					switch( GetDrawOption() )
					{
					case SPR_PUTSPRT:		if( pSpr )		GP.PutSpr(pSpr, siTempXPos, DrawYPos, 0);		break;
					}					
				}

				if(m_pListViewItemData[ i ]->siItemMask & LISTVIEW_ITEMMASK_EXPIRED)
				{
					pSpr = pclclient->pclGImg->GetGlobalSpr( GIMG_ITEMEXPIRED );
					SI32 siTempXPos = DrawXPos - 3;
					switch( GetDrawOption() )
					{
					case SPR_PUTSPRT:		if( pSpr )		GP.PutSpr(pSpr, siTempXPos, DrawYPos, 0);		break;
					}
				}

				if(m_pListViewItemData[ i ]->siItemMask & LISTVIEW_ITEMMASK_CANNOTEQIP)
				{
					pSpr = pclclient->pclGImg->GetGlobalSpr( GIMG_ITEMCANNOTEQIP );
					SI32 siTempXPos = DrawXPos;
					switch( GetDrawOption() )
					{
					case SPR_PUTSPRT:		if( pSpr )		GP.PutSprAlpha(pSpr, siTempXPos, DrawYPos, 9);	break;
					}
				}
			}
			
			++ItemCounter;
		}
	}
	else
	{		
		for ( SI16 i = 0; i < MAX_LISTVIEW_ITEM_NUM; ++i )
		{
			if ( m_pListViewItemData[ i ] )
			{
				if ( ItemCounter < m_pScroll->GetTrackPos() * m_siMaxHorizontalItemNum )
				{
					++ItemCounter;
					continue;
				}

				DrawXPos = BaseXPos + GetX() + ( ( ItemCounter - m_pScroll->GetTrackPos() * m_siMaxHorizontalItemNum ) % m_siMaxHorizontalItemNum ) * ( m_siListViewItemSizeWidth + m_siListViewItemOffsetX );
				DrawYPos = BaseYPos + GetY() + ( ( ItemCounter - m_pScroll->GetTrackPos() * m_siMaxHorizontalItemNum ) / m_siMaxHorizontalItemNum ) * ( m_siListViewItemSizeHeight + m_siListViewItemOffsetY );

				if ( DrawYPos + m_siListViewItemSizeHeight > BaseYPos + GetY() + GetHeight() )
				{
					break;
				}

				if ( m_siSelectedIndex == i )
				{
					pSpr = pclclient->pclGImg->GetGlobalSpr( m_pListViewItemData[ i ]->ImageFileIndex );

					if ( pSpr )
					{
						GP.PutSprEF(pSpr, DrawXPos, DrawYPos, 2, m_pListViewItemData[ i ]->ImageFontIndex );

					}

					if ( m_bItemNumShow )
					{
						DrawItemNumber( DrawXPos, DrawYPos + 29, m_pListViewItemData[ i ]->ItemNum );
					}
				}
				else
				{
					pSpr = pclclient->pclGImg->GetGlobalSpr( m_pListViewItemData[ i ]->ImageFileIndex );

					if ( pSpr )
					{
						switch( GetDrawOption() )
						{
						case SPR_PUTSPRT:		GP.PutSpr(pSpr, DrawXPos, DrawYPos, m_pListViewItemData[ i ]->ImageFontIndex );							break;
						case SPR_PUTSPRALPHAT:	GP.PutSprAlpha(pSpr, DrawXPos, DrawYPos, GetAlphaValue(), m_pListViewItemData[ i ]->ImageFontIndex );	break;
						}
					}

					if ( m_bItemNumShow )
					{
						DrawItemNumber( DrawXPos, DrawYPos + 29, m_pListViewItemData[ i ]->ItemNum );
					}
				}

				++ItemCounter;
			}
		}
	}

	if ( m_pScroll->IsShow() )
	{
		m_pScroll->DrawController( BaseXPos, BaseYPos, LeftTime );
	}

	if ( IsMouseOver() && m_bUseToolTip && m_siMouseOverItemIndex >= 0 )
	{
		pclclient->m_pInterfaceMgr->SetDrawToolTip( true );
	}

	return;
}

void CListView::DrawItemNumber( SI16 DrawXPos, SI16 DrawYPos, SI16 ItemNum )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	
	TSpr *pListViewNumberSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( LISTVIEW_ITEMNUMBER_FILENAME );

	SI16 ItemCipher = 1;


	if ( ItemNum < 10 )
	{
		ItemCipher = 1;
	}
	else if ( ItemNum < 100 )
	{
		ItemCipher = 2;
	}
	else if ( ItemNum < 1000 )
	{
		ItemCipher = 3;
	}
	else if ( ItemNum < 10000 )
	{
		ItemCipher = 4;
	}
	else 
	{
		ItemCipher = 5;
	}

	SI32 siAlphaValue = 12;

	switch( GetDrawOption() )
	{
	case SPR_PUTSPRALPHAT:
		{
			siAlphaValue = (12 * GetAlphaValue()) / (MAX_ALPHA_LEVEL - 1 );
		}
		break;
	}

	switch( ItemCipher )
	{
	case 1:
		{
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos, DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ItemNum );
		}
		break;
	case 2:
		{
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos, DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ItemNum / 10 );
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos + pListViewNumberSpr->GetXSize(), DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ItemNum % 10 );
		}
		break;
	case 3:
		{
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos, DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ItemNum / 100 );
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos + pListViewNumberSpr->GetXSize(), DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ( ItemNum % 100 ) / 10);
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos + pListViewNumberSpr->GetXSize() * 2, DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ItemNum % 10 );
		}
		break;
	case 4:
		{
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos, DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ItemNum / 1000 );
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos + pListViewNumberSpr->GetXSize(), DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ( ItemNum % 1000 ) / 100);
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos + pListViewNumberSpr->GetXSize() * 2, DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ( ItemNum % 100 ) / 10 );
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos + pListViewNumberSpr->GetXSize() * 3, DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ItemNum % 10 );
		}
		break;
	case 5:
		{
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos, DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ItemNum / 10000);
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos + pListViewNumberSpr->GetXSize(), DrawYPos, siAlphaValue , LISTVIEW_ITEMNUMBER_FONTINDEX + ( ItemNum % 10000 ) / 1000);
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos + pListViewNumberSpr->GetXSize() * 2, DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ( ItemNum % 1000 ) / 100 );
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos + pListViewNumberSpr->GetXSize() * 3, DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ( ItemNum % 100 ) / 10 );
			GP.PutSprAlpha( pListViewNumberSpr,DrawXPos + pListViewNumberSpr->GetXSize() * 4, DrawYPos, siAlphaValue, LISTVIEW_ITEMNUMBER_FONTINDEX + ItemNum % 10);
		}
		break;
	default:
		break;
	}

	return;
}

void CListView::UpdateScrollPos()
{
	SI16 ScrollLength = 0;

	if ( m_bAllowEmptyItemPos )
	{
		SI16 LastPos = -1;

		for ( SI16 i = MAX_LISTVIEW_ITEM_NUM - 1; i > 0; --i )
		{
			if ( m_pListViewItemData[ i ] )
			{
				LastPos = i;
				break;
			}
		}

		if ( LastPos >= 0 )
		{
			ScrollLength = ( ( LastPos + 1 ) /  m_siMaxHorizontalItemNum ) + ( ( ( LastPos + 1 ) % m_siMaxHorizontalItemNum > 0 )? 1 : 0 );
			m_pScroll->SetTrackRange( 0, ScrollLength );
		}
		else
		{
			ScrollLength = 0;
			m_pScroll->SetTrackRange( 0, ScrollLength );
		}
	}
	else
	{
		ScrollLength = ( m_siListViewItemCounter /  m_siMaxHorizontalItemNum ) + ( ( m_siListViewItemCounter % m_siMaxHorizontalItemNum > 0 )? 1 : 0 );
		m_pScroll->SetTrackRange( 0, ScrollLength );
	}

	if ( ScrollLength > m_pScroll->GetPageSize() )
	{
		if ( m_siPreScrollPos * m_pScroll->GetPageSize() * m_siMaxHorizontalItemNum >= ScrollLength - 1 )
		{
			m_pScroll->SetTrackPos( m_siPreScrollPos );
		}
		m_pScroll->Show( true );
	}
	else
	{
		m_pScroll->Show( false );
	}

	return;
}

void CListView::Refresh()
{
	m_siMouseOverItemIndex = -1;
	return;
}

stListViewItemData *CListView::GetListViewItemData( SI16 Index )
{
	if ( Index < 0 || Index >= MAX_LISTVIEW_ITEM_NUM )
	{
		return NULL;
	}

	if ( m_pListViewItemData[ Index ] == NULL )
	{
		return NULL;
	}

	return m_pListViewItemData[ Index ];
}

void CListView::SetBorder( bool Border )
{
	SetListViewBorderAndBKColorUse( Border, IsBKColorUse() );
	return;
}

void CListView::SetBKColorUse( bool BKColorUse )
{
	SetListViewBorderAndBKColorUse( IsBorder(), BKColorUse );
	return;
}

void CListView::SetItemNumShow( bool ItemNumShow )
{
	m_bItemNumShow = ItemNumShow;
	return;
}


void CListView::SetItemDrawAlphaOption( SI16 ItemPos,  bool DrawAlphaOption/* = true */)
{
	ItemPos -= m_siListViewStartItemPos;
	
	if ( ItemPos < 0 || ItemPos >= MAX_LISTVIEW_ITEM_NUM  )
		return;		
	
	if(m_pListViewItemData[ItemPos] == NULL)
		return;

	m_pListViewItemData[ItemPos]->bDrawAlphaOption = DrawAlphaOption;
	
}

void CListView::SetListViewBorderAndBKColorUse( bool Border, bool BKColorUse )
{
	CControllerObj::SetBorder( Border );
	CControllerObj::SetBKColorUse( Border );

	if ( Border || BKColorUse )
	{
		if ( m_ImgEncBuffer )
		{
			delete [] m_ImgEncBuffer;
			m_ImgEncBuffer = NULL;
		}

		CControllerSurfaceObj *pControllerSurfaceObj =  GetControllerSurfaceObj();
		if ( pControllerSurfaceObj )
		{
			LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();

			if ( lpSurface )
			{
				cltClient *pclclient = ( cltClient * )pclClient;				

				SI16 SurfaceWidth = GetSurfaceWidth();
				SI16 SurfaceHeight = GetSurfaceHeight();

				m_ImgEncBuffer = new RGB16[ SurfaceWidth * SurfaceHeight * 2 ];

				g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

				HDC hDC;
				if( lpSurface->GetDC( &hDC ) == DD_OK)
				{
					DrawBorder( hDC, 0, 0, SurfaceWidth, SurfaceHeight );
					lpSurface->ReleaseDC( hDC );
				}

				if ( GP.LockSurface( lpSurface ) )
				{
					if(m_ImgEncBuffer)
					{
						int size;
						g_graphics.EncodeXpr( m_ImgEncBuffer, SurfaceWidth, SurfaceHeight, RGB16_TRANS, &size );
					}

					GP.UnlockSurface( lpSurface );
				}
			}
		}
	}

	return;
}
