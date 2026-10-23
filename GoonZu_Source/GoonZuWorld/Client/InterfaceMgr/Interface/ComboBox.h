#ifndef _COMBOBOX_H_
#define _COMBOBOX_H_

#include "Button.h"
#include "Scroll.h"

struct stComboBoxItemData
{
	TCHAR	strText[ MAX_COMBOBOXITEM_TEXT_LEN ];
	SI32	siParam;

	stComboBoxItemData()
	{
		Init();
	}

	stComboBoxItemData( stComboBoxItemData *pComboBoxItemData )
	{
		Set( pComboBoxItemData );
	}

	void Init()
	{
		memset( this, 0, sizeof( stComboBoxItemData ) );
		return;
	}

	void Set( TCHAR *Text )
	{
		if ( Text )
		{
			_tcsncpy( strText, Text, MAX_COMBOBOXITEM_TEXT_LEN - 1 );
			strText[ MAX_COMBOBOXITEM_TEXT_LEN - 1 ] = NULL;
		}
		else
		{
			memset( strText, 0, MAX_COMBOBOXITEM_TEXT_LEN );
		}
	}

	void Set( SI32 Param, TCHAR *Text )
	{
		siParam = Param;

		if ( Text )
		{
			_tcsncpy( strText, Text, MAX_COMBOBOXITEM_TEXT_LEN - 1 );
			strText[ MAX_COMBOBOXITEM_TEXT_LEN - 1 ] = NULL;
		}
		else
		{
			memset( strText, 0, MAX_COMBOBOXITEM_TEXT_LEN );
		}
	}

	void Set( stComboBoxItemData *pComboBoxItemData )
	{
		memcpy( this, pComboBoxItemData, sizeof( stComboBoxItemData ) );
	}
};

class CComboBox : public CButton
{
public:
	CComboBox( CControllerMgr *pParent );

	~CComboBox();

	void Create( SI16 ControllerID, 
		TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum,
		SI16 XPos, SI16 YPos, SI16 Width, SI16 Height,
		SI16 SurfaceWidth, SI16 SurfaceHeight,
		SI16 ComboBoxHeight, SI16 ComboBoxLineHeight = COMBOBOX_LINE_HEIGHT,
		SI16 OffsetY = 1
		);

	void Clear();
	SI16 AddItem( stComboBoxItemData *pComboBoxItemData );
	//bool DelItem( SI16 ItemIndex );
	SI16 GetItemNum()
	{
		return m_siCurrentEmptyComboBoxIndex;
	}

	//UINT GetNumItems() { return m_ComboBoxItemDatas.GetSize(); }

	void SetFocus( bool HasFocus );
	//bool IsInArea( SI32 XPos, SI32 YPos );
	bool IsInOpenedArea( SI32 XPos, SI32 YPos );

	bool HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam );

	void PrepareDropDownText();
	void Refresh();

	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime = 0 );
	void DrawOpenedImage( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime = 0 );

	bool IsOpened()
	{
		return m_bOpened;
	}

	void SetSelectedIndex(SI16 _siSelectedIndex)
	{
		m_siSelectedIndex = _siSelectedIndex;
	}

	SI16 GetSelectedIndex()
	{
		return m_siSelectedIndex;
	}

	SI16 FindIndexFromText( TCHAR *Text );

	TCHAR *GetText( SI16 Index );
	SI32 GetParam( SI16 Index );
	stComboBoxItemData *GetItemData( SI16 Index );

	bool SetCursel( SI16 Index, bool bSendEvent = true );

	void SetEnableWheel(bool bEnableWheel)
	{
		m_bEnableWheel = bEnableWheel;
	}


	TCHAR *GetComboBoxButtonFileName()
	{
		return m_strComboBoxButtonFileName;
	}

	void SetComboBoxButtonFileName( TCHAR *FileName )
	{
		memset( m_strComboBoxButtonFileName, 0, MAX_PATH );

		if ( FileName == NULL )
		{
			return;
		}

		_tcsncpy( m_strComboBoxButtonFileName, FileName, MAX_PATH - 1 );
		m_strComboBoxButtonFileName[ MAX_PATH - 1 ] = NULL;
		return;
	}

	void SetComboBoxButtonFontIndex( SI16 FontIndex )
	{
		m_siComboBoxButtonFontIndex = FontIndex;
		return;
	}

	SI16 GetComboBoxButtonFontIndex()
	{
		return m_siComboBoxButtonFontIndex;
	}

	TCHAR *GetComboBoxDropDownImageFileName()
	{
		return m_strDropDownImageFileName;
	}

	void SetComboBoxDropDownImageFileName( TCHAR *FileName )
	{
		memset( m_strDropDownImageFileName, 0, MAX_PATH );

		if ( FileName == NULL )
		{
			return;
		}

		_tcsncpy( m_strDropDownImageFileName, FileName, MAX_PATH - 1 );
		m_strDropDownImageFileName[ MAX_PATH - 1 ] = NULL;
		return;
	}

	void SetComboBoxDropDownImageFontIndex( SI16 FontIndex )
	{
		m_siDropDownImageFontIndex = FontIndex;
		return;
	}

	SI16 GetComboBoxDropDownImageFontIndex()
	{
		return m_siDropDownImageFontIndex;
	}





	TCHAR *GetComboBoxFocusImageFileName()
	{
		return m_strFocusImageFileName;
	}

	void SetComboBoxFocusImageFileName( TCHAR *FileName )
	{
		memset( m_strFocusImageFileName, 0, MAX_PATH );

		if ( FileName == NULL )
		{
			return;
		}

		_tcsncpy( m_strFocusImageFileName, FileName, MAX_PATH - 1 );
		m_strFocusImageFileName[ MAX_PATH - 1 ] = NULL;
		return;
	}

	void SetComboBoxFocusImageFontIndex( SI16 FontIndex )
	{
		m_siFocusImageFontIndex = FontIndex;
		return;
	}

	SI16 GetComboBoxFocusImageFontIndex()
	{
		return m_siFocusImageFontIndex;
	}

	void SetComboBoxFocusImageAlphaValue( SI16 FocusImageAlphaValue )
	{
		m_siFocusImageAlphaValue = FocusImageAlphaValue;
	}








	void SetDropDownBKColor( COLORREF DropDownBKColor )
	{
		m_crDropDownBKColor = DropDownBKColor;
		return;
	}

	void SetDropDownLineColor( COLORREF DropDownLineColor )
	{
		m_crDropDownLineColor = DropDownLineColor;
		return;
	}

	void SetComboBoxLineHeight( SI16 ComboBoxLineHeight )
	{
		m_siComboBoxLineHeight = ComboBoxLineHeight;
	}

private:
	CScroll				*m_pScroll;

	SI16				m_siComboBoxHeight;

	SI16				m_siComboBoxLineHeight;

	SI16				m_siOffsetY;


	bool				m_bOpened;
	SI16				m_siSelectedIndex;
	SI16				m_siFocusedIndex;

	SI16				m_siDropDownYPos;
	SI16				m_siDropDownYSize;
	//RECT				m_rcDropDown;

	stComboBoxItemData	*m_pComboBoxItemDatas[ MAX_COMBOBOX_ITEM_NUM ];
	SI16				m_siCurrentEmptyComboBoxIndex;

	TCHAR				*m_strDropDownImageFileName;
	SI16				m_siDropDownImageFontIndex;
	bool				m_bDropDownImageTileImg;
	SI16				m_siDropDownImageTileNum;

	TCHAR				*m_strFocusImageFileName;
	SI16				m_siFocusImageFontIndex;
	bool				m_bFocusImageTileImg;
	SI16				m_siFocusImageTileNum;
	SI16				m_siFocusImageAlphaValue;

	RGB16*				m_ImgEncBuffer;

	bool				m_bReverseDraw;

	TCHAR				*m_strComboBoxButtonFileName;
	SI16				m_siComboBoxButtonFontIndex;

	COLORREF			m_crDropDownBKColor;
	COLORREF			m_crDropDownLineColor;

	bool				m_bEnableWheel;
};

#endif