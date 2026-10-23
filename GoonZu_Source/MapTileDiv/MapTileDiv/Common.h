#pragma once

#ifndef _COMMON_H
#define _COMMON_H

#include "stdafx.h"

#include "directives.h"

//-------------------------------------------------
// Spr 종류 
//-------------------------------------------------
#define SPRTYPE_SPR				0	// 일반 스프라이트 (0번 압축. ) 
#define SPRTYPE_TLE				1	// 타일용 스프라이트 (0번 압축.) 

//-------------------------------------------------
// 출력 속성. 
//-------------------------------------------------
#define SPRATB_DOT				1

//-------------------------------------------------
// cltTSprImgInfo
//-------------------------------------------------
class cltTSprImgInfo{
public:
	SI32 siCollInfo;
	SI32 siStartPos;
	SI32 siLength;

	cltTSprImgInfo()
	{
		siCollInfo	= 0;
		siStartPos	= 0;
		siLength	= 0;
	}

};

//-------------------------------------------------
// cltTSprHeader
//-------------------------------------------------
#define TSPR_VERSION			10
#define NSPR_VERSION			400

#define MAX_IMG_PER_TSPR		256	// Spr 파일 하나당 이미지의 개수.

class cltTSprHeader
{
public:
   SI32 siVersion;
   SI32	siSprType;		// 어떤 타입의 Spr인가 ?
   SI32 siXsize;
   SI32 siYsize;
   SI32 siHRes;
   SI32 siVRes;
   SI32 siTotalLength;

   SI32 Reserved[8];
   
   SI32 siFontNum;
   cltTSprImgInfo clImgInfo[MAX_IMG_PER_TSPR];
   
   cltTSprHeader()
   {
	   //siVersion		= TSPR_VERSION;
	   siVersion		= NSPR_VERSION;
	   siSprType		= SPRTYPE_SPR;
	   siXsize			= 0;
	   siYsize			= 0;	
	   siHRes			= 0;
	   siVRes			= 0;
	   
	   siTotalLength	= 0;
	   siFontNum		= 0;
   }
   
};



//-----------------------------------------------
// cltTSpr
//-----------------------------------------------
class	cltTSpr
{     
public:
	cltTSprHeader	clHeader;
	UI08			*Image;

	UI16			pal[ 256 ];

	cltTSpr();
	~cltTSpr();

	BOOL			LoadSpr( char *filename );
};

#define RGB16_565_RMASK		0xF800	
#define RGB16_565_GMASK		0x07E0
#define RGB16_565_BMASK		0x001F

#define RGB16_555_RMASK		0x7C00
#define RGB16_555_GMASK		0x03E0
#define RGB16_555_BMASK		0x001F



// Standard PCX header
struct PCXHEADER {
	char	id;
	char	version;
	char	encoding;
	char	bitPerPixel;
	short	x1;
	short	y1;
	short	x2;
	short	y2;
	short	hRes;
	short	vRes;
	char	clrMap[16*3];
	char	reserved1;
	char	numPlanes;
	short	BPL;
	short	pal_type;
	char	filler[58];
};

struct XSPRFileHeader
{
	char	id[ 4 ];					// "XPR"
	int		filesize;					// 파일전체크기
	char	version;					// 1
	char	encodetype;					// 1: spr  2: mpr  3: zpr
	char	pixelformat;				// 0: 8bit indexed  1: 16bit 565  2: 16bit 555
	char	pal_type;					// 0: no palette  16: (565, 555)  24:(888)
	short	canvasWidth;				// 캔버스 너비
	short	canvasHeight;				// 캔버스 높이 
	short	xnum;						// 가로 개수
	short	ynum;						// 세로 개수

	char	reserved1[ 20 ];			// 예약1

	// body info 
	int		bodysize;					// 이미지 태그 + 이미지 데이터 크기
	int		imagenum;					// 이미지 개수 

	char	reserved2[ 16 ];			// 예약2
};

struct XSPRImgTag
{
	int				offset;
	int				size;
	short			x;
	short			y;
	short			width;
	short			height;
};

struct XSPRFilePalette
{
	unsigned short pal565[ 256 ];
	unsigned short pal555[ 256 ];
};

class XSpr
{
public:
	XSpr();
	~XSpr();

	BOOL LoadXSpr( char *filename );

public:
	XSPRFileHeader		m_header;
	
	unsigned char*		m_pBody;

	XSPRImgTag*			m_pImageTag;
	unsigned char*		m_pImage;
	unsigned short*		m_pPal;
};

struct PCXPAL {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

#endif