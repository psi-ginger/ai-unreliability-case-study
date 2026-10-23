#ifndef _OUTLINE_H_
#define _OUTLINE_H_

#include "../ControllerObj.h"

class CControllerMgr;

class COutLine : public CControllerObj
{
public:
	COutLine( CControllerMgr *pParent );
	~COutLine();

	void Create( SI16 ControllerID, TCHAR *Text,
		TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, 
		SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight
		);
	
	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime = 0 );

private:
};

#endif