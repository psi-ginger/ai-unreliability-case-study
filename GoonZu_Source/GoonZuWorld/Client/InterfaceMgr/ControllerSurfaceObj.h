#ifndef _CONTROLLERSURFACE_H_
#define _CONTROLLERSURFACE_H_

#include <ddraw.h>

#include "ControllerBaseDefinition.h"

class CControllerSurfaceObj
{
public:
	CControllerSurfaceObj();
	~CControllerSurfaceObj();

	LPDIRECTDRAWSURFACE7 GetSurface()
	{
		return m_lpSurface;
	}

	void Create( SI16 SurfaceWidth, SI16 SurfaceHeight );


private:
	LPDIRECTDRAWSURFACE7 m_lpSurface;

	SI16 m_siWidth;
	SI16 m_siHeight;

	SI16 m_siFontHeight;


};

#endif