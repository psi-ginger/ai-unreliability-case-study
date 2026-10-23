#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Static.h"

class CControllerMgr;

class CButton : public CStatic
{
public:
	CButton( CControllerMgr *pParent );
	~CButton();

	void Create( SI16 ControllerID, TCHAR *Text, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight );

	bool HandleKeyboard( UINT uMsg, WPARAM &wParam, LPARAM &lParam );
	bool HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam );
	void OnHotkey();

	void DrawController( SI16 BaseXPos, SI16 BaseYPos, int LeftTime = 0 );
	
	void Enable(bool EnableOption );

	void SetLPressed( bool Pressed )
	{
		m_bLPressed = Pressed;
	}

	bool IsLPressed()
	{
		return m_bLPressed;
	}

	void SetUseToolTip( TCHAR *ToolTipMsg );
	void SetUseToolTip( bool bSet )		
	{
		m_bUseToolTip = bSet;
	}

	/*
	// 급조 그냥 필요해서
	SI32 GetPositionX()
	{
		return m_siX;
	}

	SI32 GetPositionY()
	{
		return m_siY;
	}
	*/

	void SetAniType( bool bSet )
	{
		m_bAniType = bSet;
	}
	
protected:
	bool m_bAniType;
	UI16 m_uiAniCounter;
	UI16 m_uiPreState;

	bool m_bLPressed;
	bool m_bRPressed;

	bool m_bUseToolTip;

	TCHAR *m_strToolTipMsg;
};

#endif
