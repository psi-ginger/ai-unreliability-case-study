#ifndef _DRAWCHARNAMESURFACE_H_
#define _DRAWCHARNAMESURFACE_H_

#include "DrawCharNameBaseInfo.h"



class CDrawCharNameSurface
{
public:
	CDrawCharNameSurface();
	~CDrawCharNameSurface();

	void						Initialize();
	void						Destroy();
	
	LPDIRECTDRAWSURFACE7		GetCharNameSurface();
	
private:
	LPDIRECTDRAWSURFACE7		m_lpSurface;
};

#endif