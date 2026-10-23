#ifndef _CONTROLLEROBJ_H_
#define _CONTROLLEROBJ_H_

#include "ControllerBaseDefinition.h"

#include "../../Common/SMLib/LibList/Obj.h"
#include "../../Client/GlobalImgFile/GlobalImgFile.h"

class CControllerSurfaceObj;
class CControllerMgr;

class CControllerObj : public CObj
{
public:
	CControllerObj( CControllerMgr *pParent = NULL );
	CControllerObj( CControllerMgr *pParent,SI32 siTmpBufferSize );
	virtual ~CControllerObj();

	void Init();

	CControllerMgr* GetParent()
	{
		return m_pParent;
	}

	void SetControllerID(  SI16 ControllerID )
	{
		m_siControllerID = ControllerID;
	}

	SI16 GetControllerID()
	{
		return m_siControllerID;
	}

	SI16 GetControllerKind()
	{
		return m_siControllerKind;
	}

	void SetHotkey( UI32 Hotkey ) { m_uiHotkey = Hotkey; }
	UINT GetHotkey() { return m_uiHotkey; }

	bool IsChanged()
	{
		return m_bChanged;
	}

	void SetChange( bool Change )
	{
		m_bChanged = Change;
		return;
	}

	void Create( SI16 ControllerID, SI16 ControllerKind,
		TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum,
		SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, 
		SI16 MaxSurfaceWidth, SI16 MaxSurfaceHeight );

	void Draw( SI16 BaseXPos, SI16 BaseYPos );
	virtual void DrawController( SI16 BaseXPos, SI16 BaseYPos, int LeftTime = 0 ) {};

	virtual bool IsInArea( SI32 XPos, SI32 YPos );
	void Move( SI16 OffsetX, SI16 OffsetY );

	void Enable( bool EnableOption )	{ m_bEnable = EnableOption; }
	bool IsEnable()
	{
		return m_bEnable;
	}

	bool IsAvailable()
	{
		if ( m_bShow && m_bEnable )
		{
			return true;
		}
		return false;
	}

	bool IsShow() { return m_bShow; }
	void Show( bool ShowOption );

	bool IsFocus() { return m_bHasFocus; }
	bool CanHaveFocus()
	{
		if ( m_bEnable && m_bShow )
		{
			return true;
		}
		return false;
	}
	virtual void SetFocus( bool HasFocus ) { m_bHasFocus = HasFocus; }

	virtual bool MsgProc( UINT uMsg, WPARAM &wParam, LPARAM &lParam ) { return false; }

	virtual bool HandleKeyboard( UINT uMsg, WPARAM &wParam, LPARAM &lParam ) { return false; }
	virtual bool HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam ) { return false; }


	void OnMouseEnter() { m_bMouseOver = true; }
	void OnMouseLeave() { m_bMouseOver = false; }

	bool IsMouseOver()
	{
		return m_bMouseOver;
	}

	virtual void OnHotkey() {}

	bool LButtonDown( SI16 XPos, SI16 YPos, SI16 *pControllerID, SI16 *pKind );
	virtual void Mouse_LButtonDown() {};
	bool LButtonUp( SI16 XPos, SI16 YPos, SI16 *pControllerID, SI16 *pKind );
	virtual void Mouse_LButtonUp() {};

	bool RButtonDown( SI16 XPos, SI16 YPos, SI16 *pControllerID, SI16 *pKind );
	virtual void Mouse_RButtonDown() {};
	bool RButtonUp( SI16 XPos, SI16 YPos, SI16 *pControllerID, SI16 *pKind );
	virtual void Mouse_RButtonUp() {};

	bool MouseMove( SI16 XPos, SI16 YPos, SI16 PreXPos, SI16 PreYPos, SI16 *pControllerID, SI16 *pKind );
	virtual void Mouse_MouseMove() {};

	//void SetBKImg( TCHAR *FileName );
	//bool LoadBKImg();

	void SetBKTileImgNum( SI16 _siBKTileImgNum )
	{
		m_siBKTileImgNum = _siBKTileImgNum;
	}
	SI16 GetBKTileImgNum()
	{
		return m_siBKTileImgNum;
	}

	void SetDrawOption( SI16 DrawOption )
	{
		m_siDrawOption = DrawOption;
	}

	SI16 GetDrawOption()
	{
		return m_siDrawOption;
	}

	void SetAlphaValue( SI16 AlphaValue )
	{
		m_siAlphaValue = AlphaValue;
	}

	SI16 GetAlphaValue()
	{
		return m_siAlphaValue;
	}

	//bool LoadCaretImg( TCHAR *FileName = "CARET.SPR" );

	TCHAR *GetFileName()
	{
		return m_strFileName;
	}

	void SetFileName( TCHAR *FileName )
	{
		
		StringCchCopy( m_strFileName, MAX_PATH, FileName );
		return;
	}

	void SetItemSprType( SI32 type )
	{
		TCHAR buf[64] = TEXT("");
		StringCchPrintf( buf, 64, TEXT("GImg/ItemImg%02d.spr"), type - GIMG_ITEMIMG01+1 );
		SetFileName( buf );
		return;
	}

	void SetFontIndex( SI16 FontIndex )
	{
		m_siFontIndex = FontIndex;
		return;
	}

	SI16 GetFontIndex()
	{
		return m_siFontIndex;
	}

	SI16 GetX()
	{
		return m_siXPos;
	}

	void SetX( SI16 XPos )
	{
		m_siXPos = XPos;

		Refresh();

		return;
	}

	SI16 GetY()
	{
		return m_siYPos;
	}

	void SetY( SI16 YPos )
	{
		m_siYPos = YPos;
		
		Refresh();

		return;
	}

	void SetControllerPos( SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight );
		
	void MoveController( SI16 XPos, SI16 YPos )
	{
		m_siXPos = XPos;
		m_siYPos = YPos;
	}

	SI16 GetWidth()
	{
		return m_siWidth;
	}

	void SetWidth( SI16 Width )
	{
		m_siWidth = Width;

		Refresh();

		return;
	}

	SI16 GetHeight()
	{
		return m_siHeight;
	}

	void SetHeight( SI16 Height )
	{
		m_siHeight = Height;

		Refresh();

		return;
	}

	SI16 GetSurfaceWidth()
	{
		return m_siMaxSurfaceWidth;
	}

	void SetSurfaceWidth( SI16 siSurfaceWidht );

	SI16 GetSurfaceHeight()
	{
		return m_siMaxSurfaceHeight;
	}

	void SetSurfaceHeight( SI16 siSurfaceHeight );

	bool IsBKTileImg()
	{
		return m_bBKTileImg;
	}

	CControllerSurfaceObj *GetControllerSurfaceObj()
	{
		return m_pControllerSurfaceObj;
	}

	void MoveControl( SI16 XPos, SI16 YPos );

	void SetFontKind( TCHAR *FontKind )
	{
		if ( FontKind == NULL )
		{
			StringCchCopy( m_strFontKind, MAX_PATH, DEFAULT_FONT_KIND );
		}
		else
		{
			StringCchCopy( m_strFontKind, MAX_PATH, FontKind );
		}
	}

	TCHAR *GetFontKind()
	{
		if ( m_strFontKind && m_strFontKind[ 0 ] )
		{
			return m_strFontKind;
		}

		return DEFAULT_FONT_KIND;
	}

	SI16 GetFontHeight()
	{
		return m_siFontHeight;
	}

	SI16 GetFontWidth()
	{
		return m_siFontWidth;
	}

	SI16 GetFontWeight()
	{
		return m_siFontWeight;
	}

	COLORREF GetFontColor()
	{
		return m_FontColor;
	}

	void SetFontWeight( SI16 Weight )
	{
		m_siFontWeight = Weight;
		return;
	}

	void SetFontWidth( SI16 FontWidth )
	{
		m_siFontWidth = FontWidth;
		return;
	}

	void SetFontHeight( SI16 FontHeight )
	{
		m_siFontHeight = FontHeight;
		return;
	}

	void SetFontSize( SI16 FontHeight, SI16 FontWidth = 0 )
	{
		m_siFontWidth = FontWidth;
		m_siFontHeight = FontHeight;
		return;
	}

	void SetFontColor( COLORREF FontColor )
	{
		m_FontColor = FontColor;
		return;
	}

	virtual void Refresh(){};

	void SetTextPrintOption( SI32 TextPrintOption )
	{
		m_siTextPrintOption = TextPrintOption;
	}

	SI32 GetTextPrintOption()
	{
		return m_siTextPrintOption;
	}

	bool IsBorder()
	{
		return m_bBorder;
	}

	virtual void SetBorder( bool Border )
	{
		m_bBorder = Border;
	}

	void SetBorderColor( COLORREF BorderColor )
	{
		m_crBorder = BorderColor;
	}

	COLORREF GetBorderColor()
	{
		return m_crBorder;
	}

	bool IsBKColorUse()
	{
		return m_bBKColorUse;
	}

	virtual void SetBKColorUse( bool BKColorUse )
	{
		m_bBKColorUse = BKColorUse;
	}

	void SetBKColor( COLORREF BKColor )
	{
		m_crBKColor = BKColor;
	}

	COLORREF GetBKColor()
	{
		return m_crBKColor;
	}

	void DrawBorder( HDC hDC, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height );

	void BackGroundDrawOption( SI16 BackGroundDrawOption, SI16 Param1 = 0, SI16 Param2 = 0 );
	
	void SetSurfaceObjSize(SI16 SurfaceWidth, SI16 SurfaceHeight);

	virtual TCHAR* GetText( SI16 Index ){ return NULL;}
	virtual TCHAR* GetText( void ) { return NULL; }
	virtual void   SetText( TCHAR *Text){ ;}

	void SetImageArea(bool bArea)	{		m_bImageArea = bArea;		}

private:
	CControllerMgr *m_pParent;

	SI16 m_siControllerID;
	SI16 m_siControllerKind;

	SI16 m_siBackGroundDrawOption;
	SI16 m_siParam1;
	SI16 m_siParam2;

	UI32 m_uiHotkey;

	bool m_bEnable;
	bool m_bShow;

	bool m_bHasFocus;
	bool m_bMouseOver;

	TCHAR m_strFileName[ MAX_PATH ];
	SI16 m_siFontIndex;

	// cltTSpr	m_BKImg;
	bool m_bBKTileImg;
	SI16 m_siBKTileImgNum;
	SI16 m_siDrawOption;
	SI16 m_siAlphaValue;

	SI16 m_siXPos;
	SI16 m_siYPos;
	SI16 m_siWidth;
	SI16 m_siHeight;
	bool m_bChanged;


	SI16 m_siMaxSurfaceWidth;
	SI16 m_siMaxSurfaceHeight;

	CControllerSurfaceObj	*m_pControllerSurfaceObj;

	TCHAR	m_strFontKind[ MAX_PATH ];
	SI16	m_siFontWeight;		// ¹®ÀÚ ±½±â ( FW_BOLD )
	SI16	m_siFontHeight;
	SI16	m_siFontWidth;
	COLORREF m_FontColor;

	SI32	m_siTextPrintOption;

	bool		m_bBorder;
	COLORREF	m_crBorder;

	bool		m_bBKColorUse;
	COLORREF	m_crBKColor;

	bool		m_bImageArea;
};

#endif