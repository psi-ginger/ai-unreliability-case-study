//-----------------------------------------------------------------------------
// 2005/09/20 이제완
//-----------------------------------------------------------------------------

#ifndef _NDRAW_H
#define _NDRAW_H

#include "NDataTypes.h"

typedef unsigned short		RGB16;

#define NDRAW_TRANSPARENT	254

#define RGB16_MODE_565		1
#define RGB16_MODE_555		2

#define RGB16_565_RMASK		0xF800
#define RGB16_565_GMASK		0x07E0
#define RGB16_565_BMASK		0x001F

#define RGB16_555_RMASK		0x7C00
#define RGB16_555_GMASK		0x03E0
#define RGB16_555_BMASK		0x001F

#define RGB16_PALETTE_SIZE	512

class NSpr;

class NDraw
{
public:
	NDraw();
	~NDraw();

	// 목적 버퍼를 설정 한다
	inline void	SetTargetBuffer( UI08 *pTargetBuffer, int pitch ) {
		m_pTargetBuffer = pTargetBuffer;
		m_iPitch = pitch;
	}

	// 색상 테이블을 설정 한다
	inline void SetColorTable( RGB16 *pColorTable ) { m_pColorTable = pColorTable; }

	void		PutSpr16T( UI08 *pSrcImage, int x, int y, int width, int height );
	void		PutSpr16CT( UI08 *pSrcImage, int x, int y, int width, int height );
	void		PutSpr16RT( UI08 *pSrcImage, int x, int y, int width, int height );
	void		PutSpr16CRT( UI08 *pSrcImage, int x, int y, int width, int height );


	UI08*		m_pTargetBuffer;
	int			m_iPitch;

	RGB16*		m_pColorTable;
};

#endif
