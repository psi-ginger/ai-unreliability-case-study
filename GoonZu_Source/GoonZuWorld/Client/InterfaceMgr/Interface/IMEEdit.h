#ifndef _IMEEDIT_H_
#define _IMEEDIT_H_

#include "Edit.h"

enum { INDICATOR_NON_IME, INDICATOR_CHS, INDICATOR_CHT, INDICATOR_KOREAN, INDICATOR_JAPANESE };
enum IMESTATE { IMEUI_STATE_OFF, IMEUI_STATE_ON, IMEUI_STATE_ENGLISH };

class CIMEEdit : public CEdit
{
public:
	CIMEEdit();
	~CIMEEdit();

	/*
	void Create( SI16 ControllerID, TCHAR *Text, TCHAR *FileName, 
		SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, 
		SI16 SurfaceWidth, SI16 SurfaceHeight, SI32 Option, bool Multiline,
		SI16 FontHeight = BASE_FONT_HEIGHT );

	void SetText( TCHAR *Text, SI32 TextPrintOption );

	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime = 0 );
	void DrawEditText( SI16 BaseXPos, SI16 BaseYPos );
	*/

private:

};


#endif