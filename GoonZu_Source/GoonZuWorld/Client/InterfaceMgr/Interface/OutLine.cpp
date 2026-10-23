#include "OutLine.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"
#include "../InterfaceMgr.h"

#include <Client.h>

extern cltCommonLogic *pclClient;


COutLine::COutLine( CControllerMgr *pParent ) : CControllerObj( pParent )
{
}

COutLine::~COutLine()
{
}

void COutLine::Create( SI16 ControllerID, TCHAR *Text,
			TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, 
			SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight
			)
{
	SetBorder( true );
	CControllerObj::Create( ControllerID, CONTROLLER_KIND_OUTLINE, FileName, FontIndex, BKTileImg, BKTileImgNum, XPos, YPos, Width, Height, SurfaceWidth, SurfaceHeight );
}

void COutLine::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	/*
	COLORREF BorderColor = GetBorderColor();
	RGB16 Color_16bit = g_graphics.MakeRGB16( GetRValue( BorderColor ), GetGValue( BorderColor ), GetBValue( BorderColor ) );

	g_graphics.Box16C( BaseXPos + GetX(), BaseYPos + GetY(), BaseXPos + GetX() + GetWidth() - 1, BaseYPos + GetY() + GetHeight() - 1, Color_16bit );
	*/

	
	//RGB16	fillColor = g_graphics.MakeRGB16( 248, 224, 168 );
	cltClient *pclclient = (cltClient *)pclClient;

	if ( IsBKTileImg() )
	{
		TSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );
		if ( pBkImg )
		{
			DrawTileImg( pBkImg, GetDrawOption(), GetFontIndex(), GetAlphaValue(), BaseXPos + GetX(), BaseYPos + GetY(), GetWidth(), GetHeight(), GetBKTileImgNum() );
		}
	}
	else
	{
		switch( GetDrawOption() )
		{
		case SPR_PUTSPRT:
			{
				RGB16	lineColor1 = g_graphics.MakeRGB16( CONTROLLER_OUTLINE_COLOR );
				RGB16	lineColor2 = g_graphics.MakeRGB16( 248, 240, 224 );

				int x1 = BaseXPos + GetX();
				int y1 = BaseYPos + GetY();
				int x2 = x1 + GetWidth() - 1;
				int y2 = y1 + GetHeight() - 1;

				g_graphics.Box16C( x1, y1, x2, y2, lineColor1 );

				g_graphics.Line16( x1 + 1, y1 + 1, x2 - 1, y1 + 1, lineColor2 );
				g_graphics.Line16( x1 + 1, y1 + 1, x1 + 1, y2 - 1, lineColor2 );
				g_graphics.Line16( x1 + 1, y2 + 1, x2 + 1, y2 + 1, lineColor2 );
				g_graphics.Line16( x2 + 1, y1 + 1, x2 + 1, y2 + 1, lineColor2 );
			}
			break;
		}
	}
	

	return;
}