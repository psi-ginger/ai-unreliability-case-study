#ifndef _LIST_H_
#define _LIST_H_

#include "../ControllerObj.h"
#include "Scroll.h"

#define MAX_LISTBOX_COLUMN_NUM					6		// 한 행에는 최대 5개의 열이 존재가능하다.

#define MAX_LISTBOXITEM_TEXT_LEN				128

#define MAX_LISTBOX_FOCUSIMAGE_FILENAME_LEN		128

#define LISTBOX_FOCUSIMAGE_ALPHA_VALUE			8

#define LISTBOX_COLOR_DEFAULT					0
#define LISTBOX_COLOR_GREEN						6
#define LISTBOX_COLOR_RED						12
#define LISTBOX_LINECOLOR_GREEN					( RGB(117, 172, 117) )
#define	LISTBOX_LINECOLOR_RED					( RGB(172, 117, 117) )
//#define LISTBOX_COLOR_VIOLET						#EE82EE


struct stListBoxColumnInfo
{
	SI16 siColumnWidth;
	TCHAR strColumnName[ MAX_LISTBOXITEM_TEXT_LEN ];

	stListBoxColumnInfo()
	{
		Init();
	}

	void Init()
	{
		memset( this, 0, sizeof( stListBoxColumnInfo ) );
	}

	void Set( SI16 ColumnWidth, TCHAR *ColumnName )
	{
		siColumnWidth = ColumnWidth;

		memset( strColumnName, 0, MAX_LISTBOXITEM_TEXT_LEN );

		if ( ColumnName )
		{
			StringCchCopy( strColumnName, MAX_LISTBOXITEM_TEXT_LEN, ColumnName );
		}
	}
};

struct stListBoxItemData
{	
	TCHAR		strText[ MAX_LISTBOX_COLUMN_NUM ][ MAX_LISTBOXITEM_TEXT_LEN ];

	SI16		siSmallIconFileIndex[ MAX_LISTBOX_COLUMN_NUM ];
	SI16		siSmallIconGImg[ MAX_LISTBOX_COLUMN_NUM ];
	SI16		siSmallIconFontIndex[ MAX_LISTBOX_COLUMN_NUM ];

	SI32		siParam[ MAX_LISTBOX_COLUMN_NUM ];
	TCHAR		strToolTipMsg[ MAX_LISTBOX_COLUMN_NUM ][ MAX_TOOLTIP_MSG_SIZE ];				// 툴팁 Msg

	COLORREF	ColumnColor[ MAX_LISTBOX_COLUMN_NUM ];

	stListBoxItemData()
	{
		Init();
	}

	stListBoxItemData( stListBoxItemData *pListBoxItemData )
	{
		memcpy( this, pListBoxItemData, sizeof( stListBoxItemData ) );
	}

	void Init()
	{
		memset( this, 0, sizeof( stListBoxItemData ) );

		for ( SI16 i = 0; i < MAX_LISTBOX_COLUMN_NUM; ++i )
		{
			siSmallIconFileIndex[ i ] = -1;
			siSmallIconFontIndex[ i ] = -1;
			siSmallIconGImg[ i ] = -1;

			ColumnColor[ i ] = RGB( 0, 0, 0 );
		}

		return;
	}

	void Set( SI16 Column, TCHAR *Text )
	{
#ifdef _DEBUG
		if( Column < 0 || Column >= MAX_LISTBOX_COLUMN_NUM )
		{
			OutputDebugString( TEXT("Column Index 초과") );
		}
#endif
		if ( Text )
		{	
			_tcsncpy( strText[ Column ], Text, MAX_LISTBOXITEM_TEXT_LEN - 1 );
			strText[ Column ][ MAX_LISTBOXITEM_TEXT_LEN - 1 ] = NULL;

		}
		else
		{
			memset( strText[ Column ], 0, MAX_LISTBOXITEM_TEXT_LEN );
		}
	}

	void Set( SI16 Column, TCHAR *Text, SI32 Param )
	{
#ifdef _DEBUG
		if( Column < 0 || Column >= MAX_LISTBOX_COLUMN_NUM )
		{
			OutputDebugString( TEXT("Column Index 초과") );
		}
#endif
		if ( Text )
		{	
			_tcsncpy( strText[ Column ], Text, MAX_LISTBOXITEM_TEXT_LEN - 1 );
			strText[ Column ][ MAX_LISTBOXITEM_TEXT_LEN - 1 ] = NULL;
		}
		else
		{
			memset( strText[ Column ], 0, MAX_LISTBOXITEM_TEXT_LEN );
		}

		siParam[ Column ] = Param;
	}

	void Set( SI16 Column, TCHAR *Text, COLORREF fontColor )
	{
#ifdef _DEBUG
		if( Column < 0 || Column >= MAX_LISTBOX_COLUMN_NUM )
		{
			OutputDebugString( TEXT("Column Index 초과") );
		}
#endif

		ColumnColor[Column] = fontColor;
		if ( Text )
		{	
			_tcsncpy( strText[ Column ], Text, MAX_LISTBOXITEM_TEXT_LEN - 1 );
			strText[ Column ][ MAX_LISTBOXITEM_TEXT_LEN - 1 ] = NULL;
		}
		else
		{
			memset( strText[ Column ], 0, MAX_LISTBOXITEM_TEXT_LEN );
		}		
	}


	void SetToolTipMsg( SI16 Column, TCHAR *ToolTipMsg )
	{
#ifdef _DEBUG
		if( Column < 0 || Column >= MAX_LISTBOX_COLUMN_NUM )
		{
			OutputDebugString( TEXT("Column Index 초과") );
		}
#endif
		if ( ToolTipMsg == NULL )
		{
			return;
		}

		StringCchCopy( strToolTipMsg[ Column ], MAX_TOOLTIP_MSG_SIZE, ToolTipMsg );
	}
	
	void SetSmallIconGImg( SI16 Column, SI16 siSmallIcon, SI16 _siSmallIconFontIndex)
	{
#ifdef _DEBUG
		if( Column < 0 || Column >= MAX_LISTBOX_COLUMN_NUM )
		{
			OutputDebugString( TEXT("Column Index 초과") );
		}
#endif

		siSmallIconGImg[Column]		 = siSmallIcon;
		siSmallIconFontIndex[Column] = _siSmallIconFontIndex;
	}
};

class CList : public CControllerObj
{
public:
	CList( CControllerMgr *pParent );
	~CList();

	void Create( SI16 ControllerID, 
		TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum,
		SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, 
		SI16 SurfaceWidth, SI16 SurfaceHeight,
		bool MultiColumn,
		bool MultiSelection,
		bool ShowHeader = false, bool DrawLine = true,
		SI16 LineHeight = LISTBOX_LINE_HEIGHT, 
		bool UserToolTip = false,
		SI16 OffsetHeight = 0
		);

	bool HandleKeyboard( UINT uMsg, WPARAM &wParam, LPARAM &lParam );
	bool HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam );

	void Clear();
	SI16 AddItem( stListBoxItemData *pListBoxItemData );
	SI16 ChangeItem( stListBoxItemData *pListBoxItemData, SI16 FindKey);	// 입력한 param 값으로 찾는다 리턴값은 인덱스
	void Refresh();
	SI16 DelItem( SI16 FindKey );
	SI16 DelItemIndex( SI16 ItemIndex );									//[진성] 해당 인덱스 삭제. 2008-8-5

	void MakeListImgEncBuffer();
	void DrawBox( HDC hDC, RECT *pRect );

	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime = 0 );

	bool SetColumn( SI16 ColumnIndex, SI16 ColumnWidth, TCHAR *ColumnName )
	{
		if ( ColumnIndex < 0 || ColumnIndex >= MAX_LISTBOX_COLUMN_NUM )
		{
			return false;
		}

		if ( m_pListBoxColumnInfo[ ColumnIndex ] == NULL )
		{
			m_pListBoxColumnInfo[ ColumnIndex ] = new stListBoxColumnInfo();
			++m_siColumnNum;
		}

		m_pListBoxColumnInfo[ ColumnIndex ]->Set( ColumnWidth, ColumnName );

		return true;
	}

	void SetShowHeader( bool ShowHeader )
	{
		m_bShowHeader = ShowHeader;
	}

	TCHAR *GetText( SI16 RowIndex , SI16 ColumnIndex );
	SI32 GetParam( SI32 RowIndex, SI16 ColumnIndex );

	void SetShowMouseOverImage( BOOL bShowMouseOverImage);

	bool SetSelectIndex( SI16 RowIndex );
	bool ClearSelectIndex();

	SI16 GetSelectedIndex()
	{
		return m_siSelectedRowIndex;
	}

	void GetSelectedIndex( SI16 *psiRowIndex, SI16 *psiColumnIndex );

	SI16 GetMouseOverIndex()
	{
		return m_siMouseOverIndex;
	}

	SI16 GetListNum()	{ return m_siCurrentEmptyListBoxIndex; }; 
	stListBoxItemData *GetItemData( SI16 RowIndex, SI16 ColumnIndex	= 0 );

	void SetDrawLine( bool DrawLine )
	{
		m_bDrawLine = DrawLine;
	}

	void SetDrawLineColor( COLORREF DrawLineColor )
	{
		m_crDrawLine = DrawLineColor;
	}

	void DrawLine( HDC hDC, SI16 Width, SI16 Height );

	void SetLineHeight( SI16 LineHeight )
	{
		m_siLineHeight = LineHeight;
	}

	SI16 GetLineHeight()
	{
		return m_siLineHeight;
	}

	void SetUseToolTip( bool UseToolTip )
	{
		m_bUseToolTip = UseToolTip;
	}

	void SetOffsetHeight( SI16 OffsetHeight )
	{
		m_siOffsetHeight = OffsetHeight;
	}

	void SetFocusImageFileName( TCHAR *FocusImageFileName )
	{
		if ( FocusImageFileName == NULL )
		{
			return;
		}

		StringCchCopy( m_strFocusImageFileName, MAX_LISTBOX_FOCUSIMAGE_FILENAME_LEN, FocusImageFileName );
	}

	void SetFocusImageFontIndex( SI16 FocusImageFontIndex )
	{
		m_siFocusImageFontIndex = FocusImageFontIndex;
	}

	void SetFocusImageAlphaValue( SI16 FocusImageAlphaValue )
	{
		m_siFocusImageAlphaValue = FocusImageAlphaValue;
	}

	void SetFocusImageTileImg( bool FocusImageTileImg )
	{
		m_bFocusImageTileImg = FocusImageTileImg;
	}

	void SetFocusImageTileNum( SI16 FocusImageTileNum )
	{
		m_siFocusImageTileNum = FocusImageTileNum;
	}



	void SetMouseOverImageFileName( TCHAR *MouseOverImageFileName )
	{
		if ( MouseOverImageFileName == NULL )
		{
			return;
		}

		StringCchCopy( m_strMouseOverImageFileName, MAX_LISTBOX_FOCUSIMAGE_FILENAME_LEN, MouseOverImageFileName );
	}

	void SetMouseOverImageFontIndex( SI16 MouseOverImageFontIndex )
	{
		m_siMouseOverImageFontIndex = MouseOverImageFontIndex;
	}

	void SetMouseOverImageAlphaValue( SI16 MouseOverImageAlphaValue )
	{
		m_siMouseOverImageAlphaValue = MouseOverImageAlphaValue;
	}

	void SetMouseOverImageTileImg( bool MouseOverImageTileImg )
	{
		m_bMouseOverImageTileImg = MouseOverImageTileImg;
	}

	void SetMouseOverImageTileNum( SI16 MouseOverImageTileNum )
	{
		m_siMouseOverImageTileNum = MouseOverImageTileNum;
	}

	void SetEachLineOffset( SI16 OffsetX, SI16 OffsetY )
	{
		m_siEachLineOffsetX = OffsetX;
		m_siEachLineOffsetY = OffsetY;
	}

	void SetListBoxColor( SI08 Color )
	{
		m_siListBoxColor = Color;
	}

	void SetMultyLineNumber( SI08 MultyLineNumber);

	SI32 GetScrollTrackPos();
	void SetScrollTrackPos(SI32 TrackPos);

	// 리스트 컨트롤의 크기를 리스트 아이템의 숫자에 맞게 고정한다..
	void AdjustHeight( SI32 MaxItemNum , bool ForcedFix = false);

	void SetSelectedIndex(SI32 Index)
	{
		m_siSelectedRowIndex = Index;
	}

	void SetListAlphaValue( SI16 ListAlphaValue )
	{
		m_siAlphaValue = ListAlphaValue;
	}

	CScroll* GetScroll()		{		return m_pScroll;		}

private:
	CScroll *m_pScroll;

	TCHAR	m_strFocusImageFileName[ MAX_LISTBOX_FOCUSIMAGE_FILENAME_LEN ];
	SI16	m_siFocusImageFontIndex;
	SI16	m_siFocusImageAlphaValue;
	bool	m_bFocusImageTileImg;
	SI16	m_siFocusImageTileNum;

	TCHAR	m_strMouseOverImageFileName[ MAX_LISTBOX_FOCUSIMAGE_FILENAME_LEN ];
	SI16	m_siMouseOverImageFontIndex;
	SI16	m_siMouseOverImageAlphaValue;
	bool	m_bMouseOverImageTileImg;
	SI16	m_siMouseOverImageTileNum;

	bool	m_bMultiColumn;			
	bool	m_bMultiSelection;		
	bool	m_bShowHeader;	
	bool	m_bDrawLine;
	COLORREF	m_crDrawLine;

	bool	m_bUseToolTip;

	SI16	m_siLineHeight;
	SI16	m_siOffsetHeight;

	SI16	m_siEachLineOffsetX;
	SI16	m_siEachLineOffsetY;


	SI16	m_siSelectedRowIndex;
	SI16	m_siSelectedColumnIndex;
	SI16	m_siFocusedIndex;
	SI16	m_siMouseOverIndex;

	int		m_siSelStart;			
	bool	m_bDrag;				


	RECT	m_rcText;

	SI16	m_siColumnNum;
	stListBoxColumnInfo	*m_pListBoxColumnInfo[ MAX_LISTBOX_COLUMN_NUM ];


	SI16	m_siCurrentEmptyListBoxIndex;
	stListBoxItemData	*m_pListBoxItemDatas[ MAX_LISTBOX_ITEM_NUM ];


	RGB16*	m_ImgEncBuffer;

	SI16	m_siIconXSize[ MAX_LISTBOX_COLUMN_NUM ];
	SI08	m_siListBoxColor;

	SI08	m_siMultyLineNumber;

	bool	m_bShowMouseOverImage;

	SI16	m_siAlphaValue;
};

#endif