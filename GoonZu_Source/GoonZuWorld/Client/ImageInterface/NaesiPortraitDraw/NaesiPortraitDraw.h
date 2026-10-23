#ifndef _NAESIPORTRAITDRAW_H_
#define _NAESIPORTRAITDRAW_H_

#include <Graphic.h>
#include <Graphic_Capsule.h>

class CNaesiPortraitDraw
{
public:
	CNaesiPortraitDraw();
	~CNaesiPortraitDraw();
	
	bool IsShow();

	void Set( bool bShow );
	void Draw( LPDIRECTDRAWSURFACE7 lpsurface, SI32 ScreenX, SI32 ScreenY );

private:
	bool	m_bShow;	
	
};
#endif