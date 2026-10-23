#ifndef _CDSHOW_H_ 
#define _CDSHOW_H_ 

//#include "../lib/NGraphics/Graphics.h"
#include <mmstream.h>
#include <amstream.h> 
#include <ddstream.h> 

#pragma comment(lib,"quartz.lib")
#pragma comment(lib,"strmiids.lib")

class CDrawLogo 
{ 

public : 
	CDrawLogo(); 
	~CDrawLogo(); // call ShutDown(); 

	void PlayMovie(TCHAR* Name);//, NGraphics pDD);
	BOOL OpenMMStream(TCHAR * pszFileName, IMultiMediaStream **ppMMStream);// NGraphics DDraw); 
	BOOL RenderStreamToSurface(IMultiMediaStream *pMMStream); 
	void Release();
}; 

#endif 