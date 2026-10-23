#include "./NaesiPortraitDraw.h"

#define NAESIPORTRAITDRAW_XPOS		300
#define NAESIPORTRAITDRAW_YPOS		395

#include "../../Client/client.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNaesiPortraitDraw::CNaesiPortraitDraw()
{
	m_bShow = false;	
}

CNaesiPortraitDraw::~CNaesiPortraitDraw()
{
}

bool CNaesiPortraitDraw::IsShow()
{
	return m_bShow;
}

void CNaesiPortraitDraw::Set( bool bShow )
{
	m_bShow = bShow;

	return;
}

void CNaesiPortraitDraw::Draw( LPDIRECTDRAWSURFACE7 lpsurface, SI32 ScreenX, SI32 ScreenY )
{
	if( pclClient->pclCM->CR[1]->pclKI->siDefaultCountry == ConstServiceArea_China)
		return;

    
	cltClient *pclclient= ( cltClient* ) pclClient;
	TSpr* pspr	= pclclient->GetGlobalSpr(GIMG_NAESI);
	
	if ( m_bShow )
	{
		if( GP.LockSurface(lpsurface) == TRUE )
		{
			GP.PutSpr(pspr, ScreenX + 100, ScreenY + pclclient->GetScreenYSize() - 250, 0 );
			GP.UnlockSurface(lpsurface);
		}
		
	}

	return;
}