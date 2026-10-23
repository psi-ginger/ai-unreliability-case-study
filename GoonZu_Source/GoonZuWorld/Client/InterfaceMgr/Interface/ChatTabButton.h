#ifndef _IMAGEBUTTON_H_
#define _IMAGEBUTTON_H_

#include "Static.h"
#include "Button.h"

class CControllerMgr;


// [영진] 2006.01.12 날씨 맑음. 채팅바 다이얼로그를 위해 급조한 인터페이스. 재사용시 보완필요.
class CChatTabButton : public CButton
{
public:
	CChatTabButton( CControllerMgr *pParent );
	~CChatTabButton();

	void Create( SI16 ControllerID, TCHAR *Text, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight );
	void DrawController( SI16 BaseXPos, SI16 BaseYPos, int LeftTime = 0 );
	//void SetInImageFontIndex( SI16 InImageFontIndex );
	void SetSelect(BOOL select){	m_bSelected = select;  };
	BOOL IsSelected(){	return m_bSelected;	};

protected:
	
	BOOL m_bSelected;
};

#endif
