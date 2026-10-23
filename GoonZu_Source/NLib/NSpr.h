//-----------------------------------------------------------------------------
// 2005/09/20 이제완
//-----------------------------------------------------------------------------

#ifndef _NSPR_H
#define _NSPR_H

#include "NDraw.h"

#define NSPRLOAD_PAL_RGB565		1
#define NSPRLOAD_PAL_RGB555		2
#define NSPRLOAD_PAL_BOTH		3

#define NSPR_ENCODETYPE_NONE	0
#define NSPR_ENCODETYPE_NPR1	1

#define NSPR_VERSION			100

struct NSprHeader
{
	char	id[ 4 ];			// "NPR"	
	
	short	version;			// 버전
	short	encodeType;			// 0:none	1:npr1

	int		fileSize;			// 파일 크기

	short	canvasWidth;		// 캔버스 너비
	short	canvasHeight;		// 캔버스 높이

	short	rowNum;				// 행 개수
	short	colNum;				// 열 개수

	short	imageNum;			// 이미지 개수
	short	colorNum;			// 팔레트 색상 개수

	int		offsetImageInfo;	// 이미지 정보의 오프셋
	int		offsetImageData;	// 이미지 데이터의 오프셋

	int		offsetPal565;		// 16비트 565팔레트의 오프셋
	int		offsetPal555;		// 16비트 555팔레트의 오프셋

	char	reserved[ 24 ];		// 예약

};

struct NSprImageInfo
{
	int		offset;				// 이미지 데이터 시작 위치로 부터의 오프셋
	int		size;				// 이미지 크기
	short	x;					// 캔버스에서 x 위치
	short	y;					// 캔버스에서 y 위치
	short	width;				// 이미지 너비
	short	height;				// 이미지 높이
};

class NSpr
{
public:
	NSpr();
	~NSpr();

	void				CreateSpr( int width, int height, int imageNum );
	void				LoadSprFromFile( char *filename );


public:
	NSprHeader*			pHeader;

	NSprImageInfo*		pImageInfo;
	unsigned char*		pImageData;

	RGB16*				pPal565;
	RGB16*				pPal555;

	unsigned char*		m_pBuffer;

};

#endif