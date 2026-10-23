//-----------------------------------------------------------------------------
// 2005/09/20 ¿Ã¡¶øœ
//-----------------------------------------------------------------------------

#ifndef _NDIB_H
#define _NDIB_H

#include <windows.h>

class NDIB
{
public:
	NDIB();
	~NDIB();

	void CreateDIB( HDC hdc, int width, int height );
	void Put( HDC hdc, int x, int y );


	HBITMAP				m_hBitmap;
	BITMAPINFO*			m_pBMI;

	unsigned short*		m_pImage;

	int					m_width;
	int					m_height;
};



#endif