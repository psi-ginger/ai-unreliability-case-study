#include "ControllerSurfaceObj.h"

#include <client.h>

#include <Graphic.h>
#include <Graphic_Capsule.h>

CControllerSurfaceObj::CControllerSurfaceObj()
{
	m_lpSurface = NULL;
}

CControllerSurfaceObj::~CControllerSurfaceObj()
{
	if( m_lpSurface )
	{
		m_lpSurface->Release();
		m_lpSurface = NULL;
	}
}

void CControllerSurfaceObj::Create( SI16 SurfaceWidth, SI16 SurfaceHeight )
{
	/*
	if ( GP.CreateOffScreen8( m_lpSurface, SurfaceWidth, SurfaceHeight ) == FALSE )
	{
		MsgBox( TEXT("CreateOffScreen Error"), TEXT("CControllerSurfaceObj::Create") );
		return;
	}
	*/

	if( g_graphics.CreateSurface16( &m_lpSurface, SurfaceWidth, SurfaceHeight, TRUE, RGB16_TRANS ) == FALSE )
	{
		MsgBox( TEXT("CreateOffScreen Error"), TEXT("CControllerSurfaceObj::Create") );
		return;
	}


	m_siWidth = SurfaceWidth;
	m_siHeight = SurfaceHeight;
	
	return;
}