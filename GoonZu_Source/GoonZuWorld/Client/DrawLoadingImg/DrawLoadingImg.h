// DrawLoadingImg.h: interface for the CDrawLoadingImg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWLOADINGIMG_H__A6075B3F_7B9E_4218_8585_94B385339C81__INCLUDED_)
#define AFX_DRAWLOADINGIMG_H__A6075B3F_7B9E_4218_8585_94B385339C81__INCLUDED_

#include <Directives.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDrawLoadingImg  
{
public:
	CDrawLoadingImg(HWND hWnd,SI32 sx,SI32 sy);
	virtual ~CDrawLoadingImg();

	void Draw( LPDIRECTDRAWSURFACE7 pSurface,SI32 scX, SI32 scY );

private:
	TSpr	m_pLoadingImg;

	SI32 m_siX;
	SI32 m_siY;

};

extern		cltGrp clGrp;

#endif // !defined(AFX_DRAWLOADINGIMG_H__A6075B3F_7B9E_4218_8585_94B385339C81__INCLUDED_)
