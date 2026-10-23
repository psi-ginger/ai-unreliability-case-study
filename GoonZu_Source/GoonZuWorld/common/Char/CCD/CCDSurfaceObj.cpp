#include "CCDSurfaceObj.h"

#include "CommonLogic.h"
extern cltCommonLogic* pclClient;

CCCDSurfaceObj::CCCDSurfaceObj() : CObj()
{
	m_siIndex = -1;

	m_lpSurface = NULL;

	m_uiFrame = 0;

	Initialize();	
}

CCCDSurfaceObj::~CCCDSurfaceObj()
{
	Destroy();

	if( m_lpSurface )
	{
		m_lpSurface->Release();
		m_lpSurface = NULL;
	}
}

void CCCDSurfaceObj::Initialize()
{
	m_siIndex = -1;

	return;
}

void CCCDSurfaceObj::Destroy()
{
	return;
}

bool CCCDSurfaceObj::IsExist( SI32 siIndex, UI32 *puiFrame )
{
	*puiFrame = m_uiFrame;

	if ( m_siIndex == siIndex )
	{
		MsgBox("siIndex = -1", "fwf");
		return true;
	}

	return false;
}

SI32 CCCDSurfaceObj::GetIndex()
{
	return m_siIndex;
}

void CCCDSurfaceObj::Set( SI32 siIndex )
{
	if ( GP.CreateOffScreen8( m_lpSurface, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT ) == FALSE )
	{
#ifdef _DEBUG
		MsgBox("CreateOffScreen Error, CCharacterDrawing", "fwf");
#endif
	}

#ifdef _DEBUG
	if ( siIndex == -1 )
	{
		MsgBox( "siIndex 값이 반드시 -1이 아니어야 하는디..", "%d", siIndex );
	}
#endif

	m_siIndex = siIndex;

	return;
}

LPDIRECTDRAWSURFACE7 CCCDSurfaceObj::GetSurface( UI32 uiFrame )
{
	m_uiFrame = uiFrame;

	return m_lpSurface;
}