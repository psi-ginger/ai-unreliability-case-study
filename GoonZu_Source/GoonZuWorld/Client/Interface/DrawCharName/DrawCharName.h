#ifndef _DRAWCHARNAME_H_
#define _DRAWCHARNAME_H_

#include "DrawCharNameBaseInfo.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

class CDrawCharNameSurface;

class CDrawCharName
{
public:
	CDrawCharName();
	~CDrawCharName();

	void	Initialize();
	void	Destroy();

	void	Set( CDrawCharNameSurface *pDrawChatSurface, TCHAR *text, TSpr* pSchoolSprImageList, SI16 SchoolSprImageListIndex, SI16 sisex, UI08 gmmode ,SI32 guildicon,TSpr*GuildImageList, SI32 siEmblemIndex, TSpr* psprEmblem );
	void	Draw( SI32 si32CenterXPos, SI32 si32CenterYPos );

	bool	IsSet()			{		return m_bSet;		}

	void	SetForceDraw( const bool bForceDraw );
	bool	GetForceDraw( void ) const;

private:
	bool	m_bSet;
	bool	m_bSchool;
	bool	m_bForceDraw;	// 어떠한 조건에도 상관없이 다시 그린다


	RGB16*	m_pTextBuffer;

	SI32	m_siWidth;
	SI32	m_siHeight;

	SI32	m_siSchoolImageWidth;	
	
	HFONT	m_hNameFont;

	TCHAR	m_szOldMakeCharName[128];

	SI32 	m_siOldEmblem;

};

#endif
