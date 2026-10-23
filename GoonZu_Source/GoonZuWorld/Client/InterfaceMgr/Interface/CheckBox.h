#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include "Button.h"

class CControllerMgr;

class CCheckBox : public CButton
{
public:
	CCheckBox( CControllerMgr *pParent );
	~CCheckBox();

	void Create( SI16 ControllerID, TCHAR *Text, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight );
	bool HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam );

	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime );
	
	void SetCheck( bool Check, SI16 ControllerKind = CONTROLLER_KIND_CHECKBOX, bool bSendEvent = true );

	bool IsCheck()
	{
		return m_bCheck;
	}

protected:
	bool m_bCheck;
};

#endif