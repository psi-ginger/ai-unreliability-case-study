#ifndef _STATIC_H_
#define _STATIC_H_

#include "../ControllerObj.h"

class CControllerMgr;


#define MAX_STATIC_TEXT_LEN		512

class CStatic : public CControllerObj  
{
public:
	CStatic( CControllerMgr *pParent );
	~CStatic();

	void Create( SI16 ControllerID, TCHAR *Text,
		TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, 
		SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight
		);

	TCHAR *GetText();
	void SetText( TCHAR *Text, SI32 TextPrintOption = -1, LPSIZE lpStringSize = NULL );

	void MakeTextImgEncBuffer(LPSIZE lpStringSize = NULL);
	void Refresh();

	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime = 0 );
	void DrawStaticText( SI16 BaseXPos, SI16 BaseYPos );

	void SetDrawRect( RECT Rect );

private:
	bool m_bSetText;
	TCHAR m_strText[ MAX_STATIC_TEXT_LEN ];

	RGB16*	m_ImgEncBuffer;

	bool m_bSetDrawRect;
	RECT m_rcDrawRect;

	SIZE m_StringSize; //[2007.12.20 김영훈 추가 : 현재 스트링 사이즈 크기를 저장]
};

#endif