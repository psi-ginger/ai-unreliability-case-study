#ifndef _SCROLL_H_
#define _SCROLL_H_

#include "../ControllerObj.h"

#define SCROLLBAR_MINTHUMBSIZE			12

enum ARROWSTATE { CLEAR, CLICKED_UP, CLICKED_DOWN, HELD_UP, HELD_DOWN };

class CScroll : public CControllerObj
{
public:
	CScroll( CControllerMgr *pParent );
	~CScroll();

	void Create( SI16 ControllerID, CControllerObj* pScrollParent,
		TCHAR *FileName, SI16 FontIndex,
		SI16 XPos, SI16 YPos, SI16 Width, SI16 Height,
		SI16 SurfaceWidth, SI16 SurfaceHeight,
		SI16 LineHeight, bool HorizontalScroll = false
		);

	void SetScrollPos( SI16 XPos, SI16 YPos, SI16 Width, SI16 Height );

	bool HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam );

	void UpdateThumbRect( bool CallRefresh = true );
	void Cap();  
	void SetTrackRange( int nStart, int nEnd );
	int GetTrackPos()
	{
		return m_siPosition;
	}
	int GetTrackReversePos()
	{
		return m_siEnd - m_siPosition;
	}
	void SetTrackPos( int nPosition )
	{
		m_siPosition = nPosition;
		Cap();
		UpdateThumbRect(); 
	}
	int GetPageSize()
	{
		return m_siPageSize;
	}
	void SetPageSize( int nPageSize )
	{
		m_siPageSize = nPageSize;
		Cap();
		UpdateThumbRect();
	}

	void SetLineHeight( int nLineHeight)
	{
		m_siLineHeight = nLineHeight;
		m_siPageSize = GetHeight() / m_siLineHeight;
	}

	void ScrollMove( int nPos );
	void Scroll( int nDelta );    
	void ShowItem( int nIndex );  

	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime = 0 );

	void Show( bool ShowOption );

	CControllerObj *GetScrollParent()
	{
		return m_pScrollParent;
	}

	void SetScrollBKFile( TCHAR *FileName, SI16 FontIndex )
	{
		if ( FileName && FontIndex > -1 )
		{
			StringCchCopy( m_strScrollBKFileName, MAX_PATH - 1, FileName );
			m_siScrollBKFontIndex = FontIndex;
		}
		else
		{
			m_strScrollBKFileName[ 0 ] = NULL;
		}
	}

	void SetScrollBKFileFontIndex( SI16 FontIndex )
	{
		if ( FontIndex > -1 )
		{
			m_siScrollBKFontIndex = FontIndex;
		}
	}

	void SetScrollThumbFile( TCHAR *FileName, SI16 FontIndex )
	{
		if ( FileName && FontIndex > -1 )
		{
			StringCchCopy( m_strScrollThumbFileName, MAX_PATH - 1, FileName );
			m_siScrollThumbFontIndex = FontIndex;
		}
		else
		{
			m_strScrollThumbFileName[ 0 ] = NULL;
		}
	}

	void SetScrollThumbFileFontIndex( SI16 FontIndex )
	{
		if ( FontIndex > -1 )
		{
			m_siScrollThumbFontIndex = FontIndex;
		}
	}

	void SetScrollUpButtonFile( TCHAR *FileName, SI16 FontIndex )
	{
		if ( FileName && FontIndex > -1 )
		{
			StringCchCopy( m_strScrollUpButtonFileName, MAX_PATH - 1, FileName );
			m_siScrollUpButtonFontIndex = FontIndex;
		}
		else
		{
			m_strScrollUpButtonFileName[ 0 ] = NULL;
		}
	}

	void SetScrollUpButtonFileFontIndex( SI16 FontIndex )
	{
		if ( FontIndex > -1 )
		{
			m_siScrollUpButtonFontIndex = FontIndex;
		}
	}

	void SetScrollDownButtonFile( TCHAR *FileName, SI16 FontIndex )
	{
		if ( FileName && FontIndex > -1 )
		{

			StringCchCopy( m_strScrollDownButtonFileName, MAX_PATH - 1, FileName );
			m_strScrollDownButtonFileName[ MAX_PATH - 1 ] = NULL;

			m_siScrollDownButtonFontIndex = FontIndex;
		}
		else
		{
			m_strScrollDownButtonFileName[ 0 ] = NULL;
		}
	}

	void SetScrollDownButtonFileFontIndex( SI16 FontIndex )
	{
		if ( FontIndex > -1 )
		{
			m_siScrollDownButtonFontIndex = FontIndex;
		}
	}

	void SetScrollButtonSize( UI16 ScrollButtonWidth, UI16 ScrollButtonHeight )
	{
		m_uiScrollButtonWidth = ScrollButtonWidth;
		m_uiScrollButtonHeight = ScrollButtonHeight;
		return;
	}

private:
	bool m_bShowThumb;
	RECT m_rcUpButton;
	RECT m_rcDownButton;
	RECT m_rcTrack;
	RECT m_rcThumb;

	bool m_bHorizontalScroll;

	LONG m_siPosition;
	SI16 m_siPageSize;			
	SI16 m_siStart;				
	SI16 m_siEnd;				
	SI16 m_siLineHeight;		

	POINT m_LastMouse;
	ARROWSTATE m_Arrow;

	CControllerObj *m_pScrollParent;

	TCHAR m_strScrollBKFileName[ MAX_PATH ];
	SI16 m_siScrollBKFontIndex;

	TCHAR m_strScrollThumbFileName[ MAX_PATH ];
	SI16 m_siScrollThumbFontIndex;

	TCHAR m_strScrollUpButtonFileName[ MAX_PATH ];
	SI16 m_siScrollUpButtonFontIndex;

	TCHAR m_strScrollDownButtonFileName[ MAX_PATH ];
	SI16 m_siScrollDownButtonFontIndex;

	UI16 m_uiScrollButtonWidth;
	UI16 m_uiScrollButtonHeight;
};

#endif