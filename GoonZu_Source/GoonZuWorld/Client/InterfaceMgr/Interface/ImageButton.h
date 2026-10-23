#ifndef _IMAGEBUTTON_H_
#define _IMAGEBUTTON_H_

#include "Static.h"
#include "Button.h"

class CControllerMgr;

class CImageButton : public CButton
{
public:
	CImageButton( CControllerMgr *pParent );
	~CImageButton();

	void Create( SI16 ControllerID, TCHAR *Text, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight );
	void DrawController( SI16 BaseXPos, SI16 BaseYPos, int LeftTime = 0 );
	void SetInImageFileName( TCHAR *InImageFileName );
	void SetInImageFontIndex( SI16 InImageFontIndex );
	void SetInImagePos( SI32 XPos, SI32 YPos );
	void SetInTextPos( SI32 XPos, SI32 YPos );
	void SetValue(SI32 Value);
	SI32 GetValue();

protected:
	TCHAR m_strInImageFileName[ MAX_PATH ];
	SI16 m_siInImageFontIndex;
	SI32 m_siInImageXPos;
	SI32 m_siInImageYPos;
	SI32 m_siInTextXPos;
	SI32 m_siInTextYPos;

	SI32 m_siValue;

};

#endif
