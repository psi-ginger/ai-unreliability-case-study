#include "List.h"
#include "Static.h"
#include "Scroll.h"
#include "ToolTip.h"

#include "../ControllerMgr.h"
#include "../ControllerSurfaceObj.h"

#include "../InterfaceMgr.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

#include <Client.h>
extern cltCommonLogic *pclClient;

#include <zmouse.h>

CList::CList( CControllerMgr *pParent ) : CControllerObj( pParent )
{
	m_bMultiColumn = false;
	m_bMultiSelection = false;
	m_bShowHeader = false;
	m_bDrawLine = true;

	m_siLineHeight = LISTBOX_LINE_HEIGHT;
	m_siOffsetHeight = 0;

	m_siSelectedRowIndex = -1;
	m_siSelectedColumnIndex = -1;
	m_siFocusedIndex = -1;
	m_siMouseOverIndex = -1;

	m_siSelStart = -1;
	m_bDrag = false;
	m_siColumnNum = 0;

	m_siCurrentEmptyListBoxIndex = 0;

	SI16 i = 0;
	for ( i = 0; i < MAX_LISTBOX_ITEM_NUM; ++i )
	{
		m_pListBoxItemDatas[ i ] = NULL;
	}

	m_ImgEncBuffer = NULL;

	for ( i = 0; i < MAX_LISTBOX_COLUMN_NUM; ++i )
	{
		m_pListBoxColumnInfo[ i ] = NULL;
	}

	m_crDrawLine = CONTROLLER_BORDER_COLOR;
	m_bUseToolTip = false;

	StringCchCopy( m_strFocusImageFileName, MAX_LISTBOX_FOCUSIMAGE_FILENAME_LEN, LISTBOX_FOCUSIMAGE_FILENAME );
	m_siFocusImageFontIndex = LISTBOX_FOCUSIMAGE_FONTINDEX;
	m_siFocusImageAlphaValue = LISTBOX_FOCUSIMAGE_ALPHA_VALUE;
	m_bFocusImageTileImg = true;
	m_siFocusImageTileNum = 3;



	m_bShowMouseOverImage = false;

	StringCchCopy( m_strMouseOverImageFileName, MAX_LISTBOX_FOCUSIMAGE_FILENAME_LEN, LISTBOX_FOCUSIMAGE_FILENAME );
	m_siMouseOverImageFontIndex = LISTBOX_FOCUSIMAGE_FONTINDEX;
	m_siMouseOverImageAlphaValue = LISTBOX_FOCUSIMAGE_ALPHA_VALUE;
	m_bMouseOverImageTileImg = true;
	m_siMouseOverImageTileNum = 3;

	m_pScroll = new CScroll( pParent );

	for ( i = 0; i < MAX_LISTBOX_COLUMN_NUM; ++i )
	{
		m_siIconXSize[ i ] = 0;
	}

	m_siEachLineOffsetX = 0;
	m_siEachLineOffsetY = 0;

	SetTextPrintOption( DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	m_siListBoxColor = LISTBOX_COLOR_DEFAULT;

	m_siMultyLineNumber = 1;
}

CList::~CList()
{
	SI16 i = 0;

	for ( i = 0; i < MAX_LISTBOX_COLUMN_NUM; ++i )
	{
		if ( m_pListBoxColumnInfo[ i ] )
		{
			delete m_pListBoxColumnInfo[ i ];
			m_pListBoxColumnInfo[ i ] = NULL;
		}
	}

	//cyj m_pListBoxItemDatas 메모리 해제
	for ( i = 0; i < MAX_LISTBOX_ITEM_NUM; ++i )
	{
		if ( m_pListBoxItemDatas[ i ] )
		{
			delete m_pListBoxItemDatas[ i ];
			m_pListBoxItemDatas[ i ] = NULL;
		}
	}

	if ( m_ImgEncBuffer )
	{
		delete [] m_ImgEncBuffer;
		m_ImgEncBuffer = NULL;
	}

	SAFE_DELETE( m_pScroll );
}

void CList::Create( SI16 ControllerID, 
				   TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum,
				   SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, 
				   SI16 SurfaceWidth, SI16 SurfaceHeight,
				   bool Multicolumn,
				   bool MultiSelection,
				   bool ShowHeader, bool DrawLine,
				   SI16 LineHeight, bool UseToolTip,
				   SI16 OffsetHeight
				   )
{
	m_bMultiColumn = Multicolumn;
	m_bMultiSelection = MultiSelection;
	m_bShowHeader = ShowHeader;
	m_bDrawLine = DrawLine;

	m_siLineHeight = LineHeight;
	m_siOffsetHeight = OffsetHeight;

	SetUseToolTip( UseToolTip );

	// 헤더가 있다면, 라인 높이가 조금 줄어야 겠지만... 우선은 바빠서 구현 먼저 하고 봄
	//m_pScroll->Create( COMMON_SCROLL, this, NULL, 0, Width - SCROLL_WIDTH - 1, 0, SCROLL_WIDTH, Height, SCROLL_WIDTH, Height, m_siLineHeight + m_siOffsetHeight );
    

	CControllerObj::Create( ControllerID, CONTROLLER_KIND_LIST, FileName, FontIndex, BKTileImg, BKTileImgNum, XPos, YPos, Width, Height, SurfaceWidth, SurfaceHeight );

	m_pScroll->Create( COMMON_SCROLL, this, NULL, 0, XPos + Width - SCROLL_WIDTH - 1, YPos, SCROLL_WIDTH, Height, SCROLL_WIDTH, Height, LineHeight + OffsetHeight );
	if ( m_bShowHeader )
	{
		m_pScroll->SetPageSize( (Height - LISTBOX_LINE_HEIGHT) / ( LineHeight + OffsetHeight ) );
	}

	m_siMultyLineNumber = 1;

	return;
}

void CList::Clear()
{
	for ( SI16 i = 0; i < MAX_LISTBOX_ITEM_NUM; ++i )
	{
		if ( m_pListBoxItemDatas[ i ] )
		{
			delete m_pListBoxItemDatas[ i ];
			m_pListBoxItemDatas[ i ] = NULL;
		}
	}

	m_siCurrentEmptyListBoxIndex = 0;

	m_siSelectedRowIndex = -1;
	m_siSelectedColumnIndex = -1;
	m_siFocusedIndex = -1;
	m_siMouseOverIndex = -1;

	m_pScroll->SetTrackRange( 0, 0 );

	return;
}

SI16 CList::AddItem( stListBoxItemData *pListBoxItemData)
{
	if ( pListBoxItemData == NULL )
	{
		return -1;
	}

	if ( m_siCurrentEmptyListBoxIndex >= MAX_LISTBOX_ITEM_NUM )
	{
		return -1;
	}

	SI16 RetVal = m_siCurrentEmptyListBoxIndex;

	m_pListBoxItemDatas[ m_siCurrentEmptyListBoxIndex++ ] = new stListBoxItemData( pListBoxItemData );

	m_pScroll->SetTrackRange( 0, m_siCurrentEmptyListBoxIndex );

	m_siSelectedRowIndex	= -1;
	m_siSelectedColumnIndex = -1;
	m_siFocusedIndex		= -1;
	m_siMouseOverIndex		= -1;

	return RetVal;
}

SI16 CList::ChangeItem( stListBoxItemData *pListBoxItemData, SI16 FindKey )
{
	if ( pListBoxItemData == NULL )
	{
		return -1;
	}

	if ( m_siCurrentEmptyListBoxIndex >= MAX_LISTBOX_ITEM_NUM )
	{
		return -1;
	}

	SI16 FindIndex = -1;
	SI16 i;
	for ( i = 0; i < MAX_LISTBOX_ITEM_NUM; ++i )
	{
		if( m_pListBoxItemDatas[ i ] != NULL )
		{
			if( m_pListBoxItemDatas[ i ]->siParam[0] == FindKey )
			{
				FindIndex = i;
				break;
			}
		}
	}

	if( FindIndex < 0)
		return -1;

	if ( m_pListBoxItemDatas[ FindIndex ] )
	{
		delete m_pListBoxItemDatas[ FindIndex ];
		m_pListBoxItemDatas[ FindIndex ] = NULL;
	}

	m_pListBoxItemDatas[ FindIndex ] = new stListBoxItemData( pListBoxItemData );

	return FindIndex;
}
void CList::AdjustHeight( SI32 MaxLineNum, bool ForcedFix )
{
	SI16 headerSize = 0;
	if ( m_bShowHeader )
	{
		headerSize = LISTBOX_LINE_HEIGHT;
	}

	SI16 linNum = 0;
	if ( ForcedFix )
	{
		linNum = MaxLineNum;
	}
	else
	{
		linNum = MaxLineNum > GetListNum() ? GetListNum() : MaxLineNum ;
	}

	SetControllerPos( GetX(), GetY(), 
					GetWidth(), GetLineHeight() * linNum + headerSize, 
					GetSurfaceWidth(), GetLineHeight() * linNum + headerSize );

	Refresh();
}
void CList::Refresh()
{
	MakeListImgEncBuffer();
	return;
}


SI16 CList::DelItem( SI16 FindKey )
{
	SI16 FindIndex = -1;
	SI16 i;
	for ( i = 0; i < MAX_LISTBOX_ITEM_NUM; ++i )
	{
		if( m_pListBoxItemDatas[ i ] != NULL )
		{
			if( m_pListBoxItemDatas[ i ]->siParam[0] == FindKey )
			{
				FindIndex = i;
				break;
			}
		}
	}

	if( FindIndex < 0)
		return -1;

	if ( m_pListBoxItemDatas[ FindIndex ] )
	{
		delete m_pListBoxItemDatas[ FindIndex ];
		m_pListBoxItemDatas[ FindIndex ] = NULL;
	}

	stListBoxItemData	*tempListBoxItemDatas[ MAX_LISTBOX_ITEM_NUM ];

	for ( i = 0; i < MAX_LISTBOX_ITEM_NUM; ++i )
	{
		tempListBoxItemDatas[ i ] = NULL;
	}


	SI16 tempCount = 0;

	for ( i = 0; i < MAX_LISTBOX_ITEM_NUM; ++i )
	{
		if( m_pListBoxItemDatas[ i ] != NULL )
		{
			tempListBoxItemDatas[ tempCount++ ] = new stListBoxItemData( m_pListBoxItemDatas[ i ] );
		}

	}

	Clear();

	for ( i = 0; i < MAX_LISTBOX_ITEM_NUM; ++i )
	{
		if( tempListBoxItemDatas[ i ] != NULL )
			AddItem( tempListBoxItemDatas[ i ] );
	}

	if ( tempListBoxItemDatas[ FindIndex ] )
	{
		delete tempListBoxItemDatas[ FindIndex ];
		tempListBoxItemDatas[ FindIndex ] = NULL;
	}

	return FindIndex;

}

SI16 CList::DelItemIndex( SI16 ItemIndex )
{
	SI16 FindIndex = -1;
	FindIndex = ItemIndex;

	if( FindIndex < 0 || FindIndex >= MAX_LISTBOX_ITEM_NUM )
		return -1;

	if( GetListNum() <= ItemIndex )
		return -1;

	if ( m_pListBoxItemDatas[ FindIndex ] )
	{
		delete m_pListBoxItemDatas[ FindIndex ];
		m_pListBoxItemDatas[ FindIndex ] = NULL;
	}

	stListBoxItemData	*tempListBoxItemDatas[ MAX_LISTBOX_ITEM_NUM ];

	SI16 i;
	for ( i = 0; i < MAX_LISTBOX_ITEM_NUM; ++i )
	{
		tempListBoxItemDatas[ i ] = NULL;
	}


	SI16 tempCount = 0;

	for ( i = 0; i < MAX_LISTBOX_ITEM_NUM; ++i )
	{
		if( m_pListBoxItemDatas[ i ] != NULL )
		{
			tempListBoxItemDatas[ tempCount++ ] = new stListBoxItemData( m_pListBoxItemDatas[ i ] );
		}

	}

	Clear();

	for ( i = 0; i < MAX_LISTBOX_ITEM_NUM; ++i )
	{
		if( tempListBoxItemDatas[ i ] != NULL )
			AddItem( tempListBoxItemDatas[ i ] );
	}

	if ( tempListBoxItemDatas[ FindIndex ] )
	{
		delete tempListBoxItemDatas[ FindIndex ];
		tempListBoxItemDatas[ FindIndex ] = NULL;
	}

	return FindIndex;
}

/*
bool CList::DelItem( SI16 ItemIndex )
{
if( ItemIndex < 0 || ItemIndex >= (int)m_ListBoxItemDatas.GetSize() )
return false;

stListBoxItemData *pItem = m_ListBoxItemDatas.GetAt( ItemIndex );
SAFE_DELETE( pItem );

bool RetVal = m_ListBoxItemDatas.Remove( ItemIndex );

if ( RetVal )
{
m_pScroll->SetTrackRange( 0, m_ListBoxItemDatas.GetSize() );

if( m_siSelectedRowIndex >= (int)m_ListBoxItemDatas.GetSize() )
{
m_siSelectedRowIndex = m_ListBoxItemDatas.GetSize() - 1;
}

GetParent()->SendEvent( EVENT_LISTBOX_SELECTION, true, this );
}

return RetVal;
}
*/

bool CList::HandleKeyboard( UINT uMsg, WPARAM &wParam, LPARAM &lParam )
{
	if ( ( ! IsAvailable() ) || ( ! IsFocus() ) )
	{
		return false;
	}

	switch( uMsg )
	{
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_UP:
				{
					SetSelectIndex( m_siFocusedIndex - 1 );
					GetParent()->SendEvent( EVENT_LISTBOX_SELECTION, true, this );
				}
				return true;
			case VK_DOWN:
				{
					SetSelectIndex( m_siFocusedIndex + 1 );
					GetParent()->SendEvent( EVENT_LISTBOX_SELECTION, true, this );
				}
				return true;
			}
		}
		break;
	}

	return false;
}

bool CList::HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam )
{
	if ( ! IsAvailable() )
		return false;

	if( WM_LBUTTONDOWN == uMsg )
	{
		if( ! IsFocus() )
			GetParent()->RequestFocus( this );
	}

	/*
	POINT ListBoxScrollPt;
	ListBoxScrollPt.x = pt.x - GetX();
	ListBoxScrollPt.y = pt.y - GetY();

	if( m_pScroll->HandleMouse( uMsg, ListBoxScrollPt, wParam, lParam ) )
		return true;
	*/
	if( m_pScroll->HandleMouse( uMsg, pt, wParam, lParam ) )
	{
		if( ! m_pScroll->IsFocus() )
		{
			GetParent()->RequestFocus( m_pScroll );
		}
		return true;
	}

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		{	
			if( IsInArea( pt.x, pt.y ) )
			{
				return true;
			}
		}
		break;
		
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONUP:
		{
			m_bDrag = false;
			if( IsInArea( pt.x, pt.y ) )
			{
				if ( m_siCurrentEmptyListBoxIndex > 0 )
				{
					int nClicked;
					if( m_siLineHeight )
					{
						if ( m_bShowHeader )
						{
							nClicked = m_pScroll->GetTrackPos() + ( pt.y - GetY() - LISTBOX_LINE_HEIGHT ) / (( m_siLineHeight + m_siOffsetHeight )*m_siMultyLineNumber);
						}
						else
						{
							nClicked = m_pScroll->GetTrackPos() + ( pt.y - GetY() ) / (( m_siLineHeight + m_siOffsetHeight )*m_siMultyLineNumber);
						}
					}
					else
					{
						nClicked = -1;
					}

					if( nClicked >= m_pScroll->GetTrackPos() &&
						nClicked < m_siCurrentEmptyListBoxIndex &&
						nClicked < m_pScroll->GetTrackPos() + m_pScroll->GetPageSize() )
					{	
						m_bDrag = true;

						m_siSelectedRowIndex = nClicked;
						m_siFocusedIndex = m_siSelectedRowIndex;

						m_siSelectedColumnIndex = -1;
						LONG ListBoxXPos = pt.x - GetX();
						SI16 ListBoxColumnWidthSizeCounter = 0;

						if ( ListBoxXPos >= 0 )
						{
							for ( SI16 i = 0; i < m_siColumnNum; ++i )
							{
								if ( m_pListBoxColumnInfo[ i ] )
								{
									ListBoxColumnWidthSizeCounter += m_pListBoxColumnInfo[ i ]->siColumnWidth;
									if ( ListBoxXPos <= ListBoxColumnWidthSizeCounter )
									{
										m_siSelectedColumnIndex = i;
										break;
									}
								}
							}
						}

						if( uMsg == WM_LBUTTONDBLCLK )
						{
							GetParent()->SendEvent( EVENT_LISTBOX_ITEM_DBLCLK, true, this );
							return true;
						}

						GetParent()->SendEvent( EVENT_LISTBOX_SELECTION, true, this );
					}
				}
				return true;
			}
			return false;
		}
		break;

	case WM_MOUSEMOVE:
		{
			if( IsInArea( pt.x, pt.y ) )
			{	
				if ( m_siCurrentEmptyListBoxIndex > 0 )
				{
					if( m_siLineHeight )
					{
 						if ( m_bShowHeader )
						{
							m_siMouseOverIndex = m_pScroll->GetTrackPos() + ( pt.y - GetY() - LISTBOX_LINE_HEIGHT ) / ( m_siLineHeight + m_siOffsetHeight );
						}
						else
						{
							m_siMouseOverIndex = m_pScroll->GetTrackPos() + ( pt.y - GetY() ) / ( m_siLineHeight + m_siOffsetHeight );
						}
					}
					else
					{
						m_siMouseOverIndex = -1;
					}

					if( m_siMouseOverIndex >= m_pScroll->GetTrackPos() &&
						m_siMouseOverIndex < m_siCurrentEmptyListBoxIndex &&
						m_siMouseOverIndex < m_pScroll->GetTrackPos() + m_pScroll->GetPageSize() )
					{
						// 마우스 오버 메시지 보냄

						GetParent()->SendEvent( EVENT_LISTBOX_MOUSEOVER, true, this );


						if ( m_pListBoxItemDatas[ m_siMouseOverIndex ] && m_pListBoxItemDatas[ m_siMouseOverIndex ]->strToolTipMsg )
						{
							cltClient *pclclient = ( cltClient * )pclClient;
							if ( m_bUseToolTip )
							{
								LONG ListBoxXPos = pt.x - GetX();
								SI16 ListBoxColumnWidthSizeCounter = 0;

								if ( ListBoxXPos >= 0 )
								{
									SI16 i = 0;
									for ( i = 0; i < m_siColumnNum; ++i )
									{
										if ( m_pListBoxColumnInfo[ i ] )
										{
											ListBoxColumnWidthSizeCounter += m_pListBoxColumnInfo[ i ]->siColumnWidth;
											if ( ListBoxXPos <= ListBoxColumnWidthSizeCounter )
											{	
												if ( m_pListBoxItemDatas[ m_siMouseOverIndex ]->strToolTipMsg[ i ] && _tcslen( m_pListBoxItemDatas[ m_siMouseOverIndex ]->strToolTipMsg[ i ] ) > 0 )
												{
													pclclient->m_pInterfaceMgr->GetToolTip()->SetToolTip( m_pListBoxItemDatas[ m_siMouseOverIndex ]->strToolTipMsg[ i ] );
												}
												else
												{
													m_siMouseOverIndex = -1;
												}
												break;
											}
										}
									}
									if ( i == m_siColumnNum )
									{
										m_siMouseOverIndex = -1;
									}
								}
							}
						}
					}
					else
					{
						m_siMouseOverIndex = -1;
					}
				}
			}
			else
			{
				m_siMouseOverIndex = -1;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				int zDelta = (short) HIWORD(wParam) / WHEEL_DELTA;
				m_pScroll->Scroll( -zDelta );

				return true;
			}
		}
		break;
	}

	return false;
}

void CList::MakeListImgEncBuffer()
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

	CControllerSurfaceObj *pControllerSurfaceObj =  GetControllerSurfaceObj();

	cltClient *pclclient = ( cltClient * )pclClient;

	if ( pControllerSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();

		if ( lpSurface )
		{
			SI16 i = 0, j = 0;

			SI16 CurrentDrawXPos = m_siEachLineOffsetX;
			SI16 CurrentDrawYPos = m_siEachLineOffsetY;

			SI16 SurfaceWidth = GetSurfaceWidth();
			SI16 SurfaceHeight = GetSurfaceHeight();

			RECT Rect;

			m_ImgEncBuffer = new RGB16[ SurfaceWidth * SurfaceHeight * 2 ];

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			if ( GP.LockSurface( lpSurface ) )
			{
				if ( IsBKTileImg() )
				{
					// cltTSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );
					TSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );
					if ( pBkImg )
					{
						DrawTileImg( pBkImg, SPR_PUTSPRT, GetFontIndex(), 0, 0, 0, GetWidth(), GetHeight(), GetBKTileImgNum() );
					}
				}
				else
				{
					TSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );
					if ( pBkImg )
					{
						GP.PutSpr(pBkImg, 0, 0, 0 );
					}
				}

				GP.UnlockSurface( lpSurface );
			}

			HDC hDC;
			if( lpSurface->GetDC( &hDC ) == DD_OK)
			{
				DrawBorder( hDC, 0, 0, SurfaceWidth, SurfaceHeight );
				lpSurface->ReleaseDC( hDC );
			}

			if ( GP.LockSurface( lpSurface ) )
			{
				if ( m_bShowHeader )
				{
					// 헤더 배경 이미지 그려주는 부분
					for ( j = 0; j < m_siColumnNum; ++j )
					{
						if ( CurrentDrawYPos + m_siLineHeight - 1 >= GetHeight() )
						{
							break;
						}

						if ( CurrentDrawXPos + m_pListBoxColumnInfo[ j ]->siColumnWidth - 1 >= GetWidth() )
						{
							break;
						}

						TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg(  TEXT("NInterface/Common/Common_4x14_00_103.SPR") );
						if ( pSpr )
						{
							if(m_siListBoxColor == LISTBOX_COLOR_DEFAULT)
							{
								DrawTileImg( pSpr, SPR_PUTSPRT, LISTBOX_COLOR_DEFAULT, 0, CurrentDrawXPos, CurrentDrawYPos, m_pListBoxColumnInfo[ j ]->siColumnWidth, pSpr->GetYSize(), 3 );
							}
							else if(m_siListBoxColor == LISTBOX_COLOR_GREEN)
							{
								DrawTileImg( pSpr, SPR_PUTSPRT, LISTBOX_COLOR_GREEN, 0, CurrentDrawXPos, CurrentDrawYPos, m_pListBoxColumnInfo[ j ]->siColumnWidth, pSpr->GetYSize(), 3 );
							}
							else if(m_siListBoxColor == LISTBOX_COLOR_RED)
							{
								DrawTileImg( pSpr, SPR_PUTSPRT, LISTBOX_COLOR_RED, 0, CurrentDrawXPos, CurrentDrawYPos, m_pListBoxColumnInfo[ j ]->siColumnWidth, pSpr->GetYSize(), 3 );
							}
							CurrentDrawXPos += m_pListBoxColumnInfo[ j ]->siColumnWidth;
						}
					}
					CurrentDrawYPos = CurrentDrawYPos + LISTBOX_LINE_HEIGHT + m_siOffsetHeight;
				}

				CurrentDrawXPos = m_siEachLineOffsetX;

				for ( i = m_pScroll->GetTrackPos(); i < m_siCurrentEmptyListBoxIndex; ++i )
				{
					if ( CurrentDrawYPos + m_siLineHeight * m_siMultyLineNumber - 1 >= GetHeight() )
					{
						break;
					}

					if ( m_pListBoxItemDatas[ i ] )
					{
						CurrentDrawXPos = m_siEachLineOffsetX;

						for ( j = 0; j < m_siColumnNum; ++j )
						{	
							// GIMG사용할수 있도록 추가
							if( m_pListBoxItemDatas[ i ]->siSmallIconGImg[ j ] != -1 )
							{
								TSpr *pSpr = pclclient->GetGlobalSpr( m_pListBoxItemDatas[i]->siSmallIconGImg[j] );
								if ( pSpr )
								{
									//[추가 : 황진성 2008. 1. 8 => 리스트박스에 그려지는 이미지의 가로위치를 정 가운데로.]
									SI16 Width    = m_pListBoxColumnInfo[j]->siColumnWidth;
									SI16 DrawXPos = CurrentDrawXPos + (Width-pSpr->GetXSize())/2;
									
									SI16 DrawYPos = CurrentDrawYPos + (GetLineHeight()-pSpr->GetYSize())/2;
                                    GP.PutSpr(pSpr, DrawXPos, DrawYPos , m_pListBoxItemDatas[ i ]->siSmallIconFontIndex[ j ] );
									m_siIconXSize[ j ] = pSpr->GetXSize()+4;
								}
							}
							
							if( m_pListBoxItemDatas[ i ]->siSmallIconFileIndex[ j ] != -1 )
							{
								switch( m_pListBoxItemDatas[ i ]->siSmallIconFileIndex[ j ] )
								{
								case -1:
									break;
								case 0:
									{
										TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( SMALL_ICON_IMG_FILE_0 );
										if ( pSpr )
										{
											GP.PutSpr(pSpr, CurrentDrawXPos, CurrentDrawYPos, m_pListBoxItemDatas[ i ]->siSmallIconFontIndex[ j ] );
											m_siIconXSize[ j ] = pSpr->GetXSize();
										}
										
									}
									break;
								case 1:
									{
										TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( SMALL_ICON_IMG_FILE_1 );
										if ( pSpr )
										{
											GP.PutSpr(pSpr, CurrentDrawXPos, CurrentDrawYPos, m_pListBoxItemDatas[ i ]->siSmallIconFontIndex[ j ] );

											m_siIconXSize[ j ] = pSpr->GetXSize();
										}
									}
									break;
								case 2:
									{
										TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( SMALL_ICON_IMG_FILE_2 );
										if ( pSpr )
										{
											GP.PutSpr(pSpr, CurrentDrawXPos, CurrentDrawYPos, m_pListBoxItemDatas[ i ]->siSmallIconFontIndex[ j ] );

											m_siIconXSize[ j ] = pSpr->GetXSize();
										}
									}
									break;
								case 3:
									{
										TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( SMALL_ICON_IMG_FILE_3 );
										if ( pSpr )
										{
											GP.PutSpr(pSpr, CurrentDrawXPos, CurrentDrawYPos, m_pListBoxItemDatas[ i ]->siSmallIconFontIndex[ j ] );

											m_siIconXSize[ j ] = pSpr->GetXSize();
										}
									}
									break;
								case 4:
									{
										TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( SMALL_ICON_IMG_FILE_4 );
										if ( pSpr )
										{
											GP.PutSpr(pSpr, CurrentDrawXPos, CurrentDrawYPos, m_pListBoxItemDatas[ i ]->siSmallIconFontIndex[ j ] );

											m_siIconXSize[ j ] = pSpr->GetXSize();
										}
									}
									break;

								}
							}

							CurrentDrawXPos += m_pListBoxColumnInfo[ j ]->siColumnWidth;
						}

						CurrentDrawYPos = CurrentDrawYPos + m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight;
					}
				}

				GP.UnlockSurface( lpSurface );
			}

			if( lpSurface->GetDC( &hDC ) == DD_OK)
			{
				HFONT hFont;
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

				CurrentDrawXPos = m_siEachLineOffsetX;
				CurrentDrawYPos = m_siEachLineOffsetY;

				if ( m_bDrawLine )
				{
					DrawLine( hDC, SurfaceWidth, SurfaceHeight );
				}

				// 헤더는 윗 부분에서 그려 줬음으로 그냥 좌표만 더 해줌.
				if ( m_bShowHeader )
				{
					// 헤더 배경 이미지 위에 Text 그려주는 부분
					for ( j = 0; j < m_siColumnNum; ++j )
					{
						if ( CurrentDrawYPos + m_siLineHeight - 1 >= GetHeight() )
						{
							break;
						}

						if ( CurrentDrawXPos + m_pListBoxColumnInfo[ j ]->siColumnWidth - 1 >= GetWidth() )
						{
							break;
						}

						SetRect( &Rect, CurrentDrawXPos, CurrentDrawYPos + 1, CurrentDrawXPos + m_pListBoxColumnInfo[ j ]->siColumnWidth - 1, CurrentDrawYPos + LISTBOX_LINE_HEIGHT );
						DrawText( hDC, m_pListBoxColumnInfo[ j ]->strColumnName, -1, &Rect, GetTextPrintOption() );
						CurrentDrawXPos += m_pListBoxColumnInfo[ j ]->siColumnWidth;
					}

					CurrentDrawYPos = CurrentDrawYPos + LISTBOX_LINE_HEIGHT + m_siOffsetHeight;
				}

				//DrawText( hDC, Text, -1, &rect, TextPrintOption );
				// 12X12
				for ( i = m_pScroll->GetTrackPos(); i < m_siCurrentEmptyListBoxIndex; ++i )
				{
					if ( CurrentDrawYPos + m_siLineHeight * m_siMultyLineNumber - 1 >= GetHeight() )
					{
						break;
					}

					if ( m_pListBoxItemDatas[ i ] )
					{
						CurrentDrawXPos = 0;

						for ( j = 0; j < m_siColumnNum; ++j )
						{	
							if ( CurrentDrawXPos + m_pListBoxColumnInfo[ j ]->siColumnWidth - 1 >= GetWidth() )
							{
								break;
							}

							SetRect( &Rect, CurrentDrawXPos, CurrentDrawYPos + 1, CurrentDrawXPos + m_pListBoxColumnInfo[ j ]->siColumnWidth - 1, CurrentDrawYPos + m_siLineHeight * m_siMultyLineNumber );

							if( m_pListBoxItemDatas[ i ]->siSmallIconFileIndex[ j ] != -1 ||
								m_pListBoxItemDatas[ i ]->siSmallIconGImg[ j ] != -1 )
							{	
								Rect.left += m_siIconXSize[ j ]+2;;
							}

							++Rect.top;
							++Rect.left;
							--Rect.bottom;
							--Rect.right;

							if ( m_pListBoxItemDatas[ i ]->strText[ j ] )
							{	
								SetTextColor( hDC, m_pListBoxItemDatas[ i ]->ColumnColor[ j ] );
								DrawText( hDC, m_pListBoxItemDatas[ i ]->strText[ j ], lstrlen(m_pListBoxItemDatas[ i ]->strText[ j ]), &Rect, GetTextPrintOption() );
							}

							CurrentDrawXPos += m_pListBoxColumnInfo[ j ]->siColumnWidth;
						}

						CurrentDrawYPos = CurrentDrawYPos + m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight;
					}
				}

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
				if(m_ImgEncBuffer)
				{
					int size;
					g_graphics.EncodeXpr( m_ImgEncBuffer, SurfaceWidth, SurfaceHeight, RGB16_TRANS, &size );
				}

				GP.UnlockSurface( lpSurface );
			}
		}
	}

	if ( m_bShowHeader )
	{
		if ( ( m_siCurrentEmptyListBoxIndex ) * ( m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight ) + LISTBOX_LINE_HEIGHT +m_siOffsetHeight  > GetHeight() )
		{
			m_pScroll->Show( true );
		}
		else
		{
			m_pScroll->Show( false );
		}
	}
	else
	{
		if ( m_siCurrentEmptyListBoxIndex * ( m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight ) > GetHeight() )
		{
			m_pScroll->Show( true );
		}
		else
		{
			m_pScroll->Show( false );
		}
	}
}

void CList::DrawBox( HDC hDC, RECT *pRect )
{	
	/*
	MoveToEx( hDC, pRect->right, pRect->top, NULL );
	LineTo( hDC, pRect->right, pRect->bottom + 1 );

	MoveToEx( hDC, pRect->left, pRect->bottom, NULL );
	LineTo( hDC, pRect->right + 1, pRect->bottom );
	*/

	return;
}

void CList::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	if ( ! IsShow() )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	if ( m_ImgEncBuffer ) 
	{
		switch( GetDrawOption() )
		{
		case SPR_PUTSPRT:		GP.PutXpr16( BaseXPos + GetX(), BaseYPos + GetY(), GetSurfaceWidth(), GetSurfaceHeight(), m_ImgEncBuffer, RGB16_TRANS );							break;
		case SPR_PUTSPRALPHAT:	GP.PutXpr16A( BaseXPos + GetX(), BaseYPos + GetY(), GetSurfaceWidth(), GetSurfaceHeight(), m_ImgEncBuffer, m_siAlphaValue, RGB16_TRANS );			break;
		}
	}

	if ( m_siFocusedIndex >= 0 )
	{	
		TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strFocusImageFileName );
		if ( pSpr )
		{
			if ( m_siFocusedIndex < m_pScroll->GetTrackPos() || m_siFocusedIndex >= m_pScroll->GetTrackPos() + m_pScroll->GetPageSize() )
			{
			}
			else
			{
				if ( m_bFocusImageTileImg )
				{
					if ( m_bShowHeader )
					{
						if ( m_pScroll->IsShow() )
						{
							DrawTileImg( pSpr, SPR_PUTSPRALPHAT, m_siFocusImageFontIndex, m_siFocusImageAlphaValue, BaseXPos + GetX(), BaseYPos + GetY() + ( m_siFocusedIndex - m_pScroll->GetTrackPos() ) * ( m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight ) + ( LISTBOX_LINE_HEIGHT + m_siOffsetHeight ), GetWidth() - SCROLL_WIDTH, m_siLineHeight* m_siMultyLineNumber, m_siFocusImageTileNum );
						}
						else
						{
							DrawTileImg( pSpr, SPR_PUTSPRALPHAT, m_siFocusImageFontIndex, m_siFocusImageAlphaValue, BaseXPos + GetX(), BaseYPos + GetY() + ( m_siFocusedIndex - m_pScroll->GetTrackPos() ) * ( m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight ) + ( LISTBOX_LINE_HEIGHT + m_siOffsetHeight ), GetWidth(), m_siLineHeight* m_siMultyLineNumber, m_siFocusImageTileNum );
						}
					}
					else
					{
						if ( m_pScroll->IsShow() )
						{
							DrawTileImg( pSpr, SPR_PUTSPRALPHAT, m_siFocusImageFontIndex, m_siFocusImageAlphaValue, BaseXPos + GetX(), BaseYPos + GetY() + ( m_siFocusedIndex - m_pScroll->GetTrackPos() ) * ( m_siLineHeight* m_siMultyLineNumber + m_siOffsetHeight ), GetWidth() - SCROLL_WIDTH, m_siLineHeight* m_siMultyLineNumber , m_siFocusImageTileNum );
						}
						else
						{
							DrawTileImg( pSpr, SPR_PUTSPRALPHAT, m_siFocusImageFontIndex, m_siFocusImageAlphaValue, BaseXPos + GetX(), BaseYPos + GetY() + ( m_siFocusedIndex - m_pScroll->GetTrackPos() ) * ( m_siLineHeight* m_siMultyLineNumber + m_siOffsetHeight ), GetWidth(), m_siLineHeight* m_siMultyLineNumber, m_siFocusImageTileNum );
						}
					}
				}
				else
				{
					if ( m_bShowHeader )
					{	
						GP.PutSprAlpha(pSpr, BaseXPos + GetX(), BaseYPos + GetY() + ( m_siFocusedIndex - m_pScroll->GetTrackPos()) * ( m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight ) + ( LISTBOX_LINE_HEIGHT + m_siOffsetHeight ), m_siFocusImageAlphaValue, m_siFocusImageFontIndex );
					}
					else
					{
						GP.PutSprAlpha(pSpr, BaseXPos + GetX(), BaseYPos + GetY() + ( m_siFocusedIndex - m_pScroll->GetTrackPos() ) * ( m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight ), m_siFocusImageAlphaValue, m_siFocusImageFontIndex );
					}
				}
			}
		}
	}

	if ( m_siMouseOverIndex >= 0 && m_siFocusedIndex != m_siMouseOverIndex )
	{	
		TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strMouseOverImageFileName );
		if ( pSpr && m_bShowMouseOverImage )
		{
			if ( m_siMouseOverIndex < m_pScroll->GetTrackPos() || m_siMouseOverIndex >= m_pScroll->GetTrackPos() + m_pScroll->GetPageSize() )
			{
			}
			else
			{
				if ( m_bMouseOverImageTileImg)
				{
					if ( m_bShowHeader )
					{
						if ( m_pScroll->IsShow() )
						{
							DrawTileImg( pSpr, SPR_PUTSPRALPHAT, m_siMouseOverImageFontIndex, m_siMouseOverImageAlphaValue, BaseXPos + GetX(), BaseYPos + GetY() + ( m_siMouseOverIndex - m_pScroll->GetTrackPos() ) * ( m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight ) + ( LISTBOX_LINE_HEIGHT + m_siOffsetHeight ), GetWidth() - SCROLL_WIDTH, m_siLineHeight * m_siMultyLineNumber, m_siMouseOverImageTileNum );
						}
						else
						{
							DrawTileImg( pSpr, SPR_PUTSPRALPHAT, m_siMouseOverImageFontIndex, m_siMouseOverImageAlphaValue, BaseXPos + GetX(), BaseYPos + GetY() + ( m_siMouseOverIndex - m_pScroll->GetTrackPos()) * ( m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight ) + ( LISTBOX_LINE_HEIGHT + m_siOffsetHeight ), GetWidth(), m_siLineHeight * m_siMultyLineNumber, m_siMouseOverImageTileNum );
						}
					}
					else
					{
						DrawTileImg( pSpr, SPR_PUTSPRALPHAT, m_siMouseOverImageFontIndex, m_siMouseOverImageAlphaValue, BaseXPos + GetX(), BaseYPos + GetY() + ( m_siMouseOverIndex - m_pScroll->GetTrackPos() ) * ( m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight ), GetWidth(), m_siLineHeight * m_siMultyLineNumber, m_siMouseOverImageTileNum );
					}
				}
				else
				{
					if ( m_bShowHeader )
					{
						GP.PutSprAlpha(pSpr, BaseXPos + GetX(), BaseYPos + GetY() + ( m_siMouseOverIndex - m_pScroll->GetTrackPos()) * ( m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight ) + ( LISTBOX_LINE_HEIGHT + m_siOffsetHeight ), m_siMouseOverImageAlphaValue, m_siMouseOverImageFontIndex );
					}
					else
					{
						GP.PutSprAlpha(pSpr, BaseXPos + GetX(), BaseYPos + GetY() + ( m_siMouseOverIndex - m_pScroll->GetTrackPos() ) * ( m_siLineHeight * m_siMultyLineNumber + m_siOffsetHeight ), m_siMouseOverImageAlphaValue, m_siMouseOverImageFontIndex );
					}
				}
			}
		}
	}

	if ( m_pScroll->IsShow() )
	{
		m_pScroll->DrawController( BaseXPos, BaseYPos, LeftTime );
	}

	if ( m_bUseToolTip && m_siMouseOverIndex >= 0 && IsMouseOver() )
	{
		pclclient->m_pInterfaceMgr->SetDrawToolTip( true );
	}

	return;
}

TCHAR *CList::GetText( SI16 RowIndex, SI16 ColumnIndex )
{
	if ( ColumnIndex < 0 || ColumnIndex >= m_siColumnNum ||
		RowIndex < 0 || RowIndex >= m_siCurrentEmptyListBoxIndex
		)
	{
		return NULL;
	}

	if ( m_pListBoxItemDatas[ RowIndex ] == NULL )
	{
		return NULL;
	}

	if ( m_pListBoxItemDatas[ RowIndex ]->strText[ ColumnIndex ] )
	{
		return m_pListBoxItemDatas[ RowIndex ]->strText[ ColumnIndex ];
	}

	return NULL;
}

SI32 CList::GetParam( SI32 RowIndex, SI16 ColumnIndex )
{
	if ( ColumnIndex < 0 || ColumnIndex >= m_siColumnNum ||
		RowIndex < 0 || RowIndex >= m_siCurrentEmptyListBoxIndex
		)
	{
		return 0;
	}

	if ( m_pListBoxItemDatas[ RowIndex ] == NULL )
	{
		return 0;
	}

	if ( m_pListBoxItemDatas[ RowIndex ]->strText[ ColumnIndex ] )
	{
		return m_pListBoxItemDatas[ RowIndex ]->siParam[ ColumnIndex ];
	}

	return 0;
}

void CList::SetShowMouseOverImage( BOOL bShowMouseOverImage)
{
	m_bShowMouseOverImage = (bShowMouseOverImage == TRUE);
}

bool CList::SetSelectIndex( SI16 RowIndex )
{
	if ( RowIndex < 0 || RowIndex >= m_siCurrentEmptyListBoxIndex )
	{
		return false;
	}

	if ( m_pListBoxItemDatas[ RowIndex ] == NULL )
	{
		return false;
	}

	m_siFocusedIndex = RowIndex;
	m_siSelectedRowIndex = RowIndex;
	if ( m_siColumnNum > 0 )
	{
		m_siSelectedColumnIndex = 0;
	}
	else
	{
		m_siSelectedColumnIndex = -1;
	}

	if ( m_pScroll->GetTrackPos() > RowIndex || m_pScroll->GetTrackPos() + m_pScroll->GetPageSize() <= RowIndex )
	{
		m_pScroll->SetTrackPos( RowIndex );
	}
	return true;
}

bool CList::ClearSelectIndex()
{
	SI16 RowIndex = -1;

	m_siFocusedIndex = RowIndex;
	m_siSelectedRowIndex = RowIndex;
	m_siSelectedColumnIndex = -1;

	return true;
}

void CList::GetSelectedIndex( SI16 *psiRowIndex, SI16 *psiColumnIndex )
{
	*psiRowIndex = m_siSelectedRowIndex;
	*psiColumnIndex = m_siSelectedColumnIndex;

	return;
}

stListBoxItemData *CList::GetItemData( SI16 RowIndex, SI16 ColumnIndex )
{
	if ( ColumnIndex < 0 || ColumnIndex >= m_siColumnNum ||
		RowIndex < 0 || RowIndex >= m_siCurrentEmptyListBoxIndex
		)
	{
		return NULL;
	}

	if ( m_pListBoxItemDatas[ RowIndex ] == NULL )
	{
		return NULL;
	}

	return m_pListBoxItemDatas[ RowIndex ];
}

void CList::DrawLine( HDC hDC, SI16 Width, SI16 Height )
{
	if ( ! m_bDrawLine )
	{
		return;
	}

	HPEN hPen, hOldPen;
	hPen = CreatePen( PS_SOLID, 1, m_crDrawLine );
	hOldPen = (HPEN)SelectObject( hDC, hPen );

	// 세로선 긋기
	SI16 CurrentDrawXPos = 0;
	if ( m_siColumnNum > 1 )
	{
		for ( SI16 i = 0; i < m_siColumnNum - 1; ++i )
		{
			if ( m_pListBoxColumnInfo[ i ] )
			{
				CurrentDrawXPos += m_pListBoxColumnInfo[ i ]->siColumnWidth;
				MoveToEx( hDC, CurrentDrawXPos, 0, NULL );
				LineTo( hDC, CurrentDrawXPos, Height - 1 );
			}
		}
	}

	// 가로선 긋기
	SI16 CurrentDrawYPos = 0;
	if ( m_bShowHeader )
	{
		CurrentDrawYPos += LISTBOX_LINE_HEIGHT;
	}

	while ( CurrentDrawYPos < Height )
	{
		CurrentDrawYPos += m_siLineHeight * m_siMultyLineNumber;

		MoveToEx( hDC, 0, CurrentDrawYPos, NULL );
		LineTo( hDC, Width - 1, CurrentDrawYPos );

		CurrentDrawYPos += m_siOffsetHeight;
	}

	SelectObject( hDC, hOldPen );
	DeleteObject( hPen );

	return;
}

void CList::SetMultyLineNumber( SI08 MultyLineNumber)
{

	if(MultyLineNumber < 1) return;

	m_siMultyLineNumber = MultyLineNumber;

	if(m_siMultyLineNumber == 1)
	{
		SetTextPrintOption( DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	}
	else
	{
		SetTextPrintOption( DT_CENTER|DT_VCENTER);
	}	

	m_pScroll->SetLineHeight( m_siLineHeight * m_siMultyLineNumber );
	
	if ( m_bShowHeader )
	{
		m_pScroll->SetPageSize( (GetHeight() - LISTBOX_LINE_HEIGHT) / (( m_siLineHeight + m_siOffsetHeight ) * m_siMultyLineNumber ));
	}
	else
	{
		m_pScroll->SetPageSize( (GetHeight() ) / (( m_siLineHeight + m_siOffsetHeight ) * m_siMultyLineNumber ));
	}

}

SI32 CList::GetScrollTrackPos()
{
	
	return m_pScroll->GetTrackPos();
}

void CList::SetScrollTrackPos(SI32 TrackPos)
{
	m_pScroll->SetTrackPos(TrackPos);
}