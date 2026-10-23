#ifndef _DRAWCHAT_H_
#define _DRAWCHAT_H_

#include "DrawChatBaseInfo.h"

class	CDrawChatSurface;

struct stEmoticonDataDraw
{
	SI16	siXpos;
	SI16	siYpos;
	SI32	siFontIndex;

	void Set( SI16 x, SI16 y, SI32 index )
	{
		siXpos = x;
		siYpos = y;
		siFontIndex = index;
	}
};

class	CDrawChat
{
public:
	CDrawChat();
	~CDrawChat();

	void	Initialize(SI32 siCharUnique);
	void	Destroy();

	void	Set( CDrawChatSurface *pDrawChatSurface, TCHAR	*text, bool bContinueDraw = false, SI32 siSelectChatKind = 0 , SI32 siShopMode = 0 ,TCHAR *shopTitle = NULL);

	void	Action();

	void	Draw( SI32 si32CenterXPos, SI32 si32CenterYPos, RECT *pScreenRect );

	void	DivideChatData();

	void	StopDraw();
	
	bool	IsDrawStatus()	{ return m_bDrawStatus;	}

	SI16	GetChatMode();


private:

	SIZE			m_Size;
	SI32			m_si32CurrentFrame;
	TCHAR			m_strChatBuffer[ MAX_CHAT_DATA_LENGTH ];


	RGB16			*m_pImageBuffer;
	RGB16			*m_pTextBuffer;

	RGB16			*m_pTitleBackBuffer;
	RGB16			*m_pTitleBuffer;

	bool			m_bDrawStatus;
	SI32			m_si32Count;

	SI32			m_si32Width;
	SI32			m_si32Height;

	SI32			m_siTitleWidth;
	SI32			m_siTitleHeight;

	bool			m_bDrawTitle;

	bool			m_bContinueDraw;
	SI32			m_siDrawChatKind;
	
	SI16			m_siChatMode;

	SI16			m_siFadeMode;
	SI16			m_siFadeStep;

	SI32			m_siCharUnique;				// 2009-07- 08 쓸데없는 패킷을 서버로 날려서 추가.
};

#endif