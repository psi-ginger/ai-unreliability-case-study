#ifndef _RADIOBUTTON_H_
#define _RADIOBUTTON_H_

#include "CheckBox.h"

class CControllerMgr;

class CRadioButton : public CCheckBox
{
public:
	CRadioButton( CControllerMgr *pParent );
	~CRadioButton();

	void Create( SI16 ControllerID, TCHAR *Text, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight, SI16 GroupID );
	bool HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam );

	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime );

	SI16 GetGroupID()
	{
		return m_siGroupID;
	}
	
		// 자신의 버튼체크만 변경한다.
	void SetNonEventOneButtonCheck(bool bCheck);
	void SetNonEventCheck();	// SendEvent 없이 버튼 외관만 변경할때 사용하는 메소드

private:

	SI16	m_siGroupID;
};

#endif