#ifndef _RESIDENTLOGINDRAW_H_
#define _RESIDENTLOGINDRAW_H_

#include <Graphic.h>
#include <Graphic_Capsule.h>

#define MAX_RESIDENTLOGIN_WIDTH			200
#define MAX_RESIDENTLOGIN_HEIGHT		100



class CResidentLoginDraw
{
public:
	CResidentLoginDraw();
	~CResidentLoginDraw();

	void Initialize();	

	void Set( TCHAR *CharName );
	void Action();
	void Draw( SI32 XMovePos, SI32 YMovePos );

private:

	RGB16					*m_pImageBuffer;

	
	LPDIRECTDRAWSURFACE7	m_lpSurface;
	TSpr					*m_pLoginSpr;

	bool					m_bShow;
	SI32					m_siFrameCount;
	
	SI32					m_siContinueCount;
	
	SI32					m_siCurrentImageYPos;

};

#endif