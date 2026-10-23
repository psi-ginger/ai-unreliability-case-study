#ifndef _PROMPTMGR_H_
#define _PROMPTMGR_H_

#include "PromptBaseDefinition.h"

#include <ddraw.h>
#include <Graphic.h>
#include <Graphic_Capsule.h>

class	CPrompt;
class CPromptMgr
{
public:
	CPromptMgr();
	~CPromptMgr();

	void		Initialize();
	void		Destroy();

	void		Create();


	bool		Set( TCHAR*	strPrompt, TCHAR* strURL, SI32	si32RepetitionNum, COLORREF TextColor );
	void		PushPrompt( CPrompt *pPrompt );
	void		MoveToBack( CPrompt *pPrompt );

	void		DelAllPrompt();
	void		DelPrompt( CPrompt *pPrompt );

	void		Action();
	void		Draw( SI32 si32DrawXPos, SI32 si32DrawYPos );

	void		SetUnderLineText( bool bSet );

	SI32		GetPromptNum()
	{
		return	m_siPromptNum;
	}
	RECT						m_Rect;

	TCHAR*		GetURL()	
	{
		return m_szCurrentURL;
	}
	bool		HaveUrl()
	{
		return m_bHaveUrl;
	}

private:
	CPrompt						*m_pFirst;
	CPrompt						*m_pLast;

	SI32						m_siPromptNum;

	LPDIRECTDRAWSURFACE7		m_lpSurface;
	RGB16						*m_ImgEncBuffer;

	bool						m_bSetUnderLineText;

	TCHAR						m_szCurrentURL[MAX_PATH];
	bool						m_bHaveUrl;

	HFONT						m_hNormalFont;
	HFONT						m_hUnderlineFont;

	bool						m_bEncodeBuffer;
};

#endif