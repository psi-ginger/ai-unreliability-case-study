#include "DrawChatSurface.h"
#include "EtcTool\EtcTool.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

CDrawChatSurface::CDrawChatSurface()
{
	m_lpSurface = NULL;


	if ( g_graphics.CreateSurface16( &m_lpSurface, MAX_CHAT_SURFACE_WIDTH + 10, MAX_CHAT_SURFACE_HEIGHT + 10 ) == FALSE )
	{
		MsgBox(TEXT("CreateOffScreen Error, CDrawChatSurface"), TEXT("fdsf"));		
	}


	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/DrawChat.Spr"), &m_DrawChatSpr ) )
	{
		MsgBox(TEXT("./Interface/DrawChat/DrawChat.Spr Error, Load Error"), TEXT("fdsf"));
	}

	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/Tail.Spr"), &m_DrawChatTailSpr ) )
	{
		MsgBox(TEXT("./Interface/DrawChat/Tail.Spr Error, Load Error"), TEXT("fdsf"));
	}

	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/DrawChat1.Spr"), &m_DrawChatSpr1 ) )
	{
		MsgBox(TEXT("./Interface/DrawChat/DrawChat1.Spr Error, Load Error"), TEXT("fdsf"));
	}

	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/Tail1.Spr"), &m_DrawChatTailSpr1 ) )
	{
		MsgBox(TEXT("./Interface/DrawChat/Tail1.Spr Error, Load Error"), TEXT("fdsf"));
	}

	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/DrawChat2.Spr"), &m_DrawChatSpr2 ) )
	{
		MsgBox(TEXT("./Interface/DrawChat/DrawChat2.Spr Error, Load Error"), TEXT("fdsf"));
	}

	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/Tail2.Spr"), &m_DrawChatTailSpr2 ) )
	{
		MsgBox(TEXT("./Interface/DrawChat/Tail2.Spr Error, Load Error"), TEXT("fdsf"));
	}

	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/DrawChat3.Spr"), &m_DrawChatSpr3 ) )
	{
		MsgBox(TEXT("./Interface/DrawChat/DrawChat3.Spr Error, Load Error"), TEXT("fdsf"));
	}

	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/Tail3.Spr"), &m_DrawChatTailSpr3 ) )
	{
		MsgBox(TEXT("./Interface/DrawChat/Tail3.Spr Error, Load Error"), TEXT("fdsf"));
	}

	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/DrawChat4.Spr"), &m_DrawChatSpr4 ) )
	{
		MsgBox(TEXT("./Interface/DrawChat/DrawChat4.Spr Error, Load Error"), TEXT("24dsf4"));
	}

	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/Tail4.Spr"), &m_DrawChatTailSpr4 ) )
	{
		MsgBox(TEXT("./Interface/DrawChat/Tail4.Spr Error, Load Error"), TEXT("fdsf"));
	}
	
	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/DrawChat5.Spr"), &m_DrawChatSpr5 ) )
	{
		MsgBox(TEXT("./Interface/DrawChat/DrawChat5.Spr Error, Load Error"), TEXT("b332df"));
	}

	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/Tail5.Spr"), &m_DrawChatTailSpr5 ) )
	{
		MsgBox(TEXT("./Interface/DrawChat/Tail5.Spr Error, Load Error"), TEXT("bw3r"));
	}

	if ( FALSE == GP.LoadSprFromFile( TEXT("./Interface/DrawChat/MAL.Spr"), &m_DrawShopTitleImage) )
	{
		MsgBox(TEXT("./Interface/DrawChat/MAL.Spr Error, Load Error"), TEXT("75gh4"));
	}

	Initialize();
}

CDrawChatSurface::~CDrawChatSurface()
{
	Destroy();
}

void CDrawChatSurface::Initialize()
{
	return;
}

void CDrawChatSurface::Destroy()
{
	if ( m_DrawChatSpr.pImage )
	{
		GP.FreeSpr( m_DrawChatSpr );
	}	

	if ( m_DrawChatTailSpr.pImage )
	{
		GP.FreeSpr( m_DrawChatTailSpr );
	}

	if ( m_DrawChatSpr1.pImage )
	{
		GP.FreeSpr( m_DrawChatSpr1 );
	}	

	if ( m_DrawChatTailSpr1.pImage )
	{
		GP.FreeSpr( m_DrawChatTailSpr1 );
	}

	if ( m_DrawChatSpr2.pImage )
	{
		GP.FreeSpr( m_DrawChatSpr2 );
	}	

	if ( m_DrawChatTailSpr2.pImage )
	{
		GP.FreeSpr( m_DrawChatTailSpr2 );
	}

	if ( m_DrawChatSpr3.pImage )
	{
		GP.FreeSpr( m_DrawChatSpr3 );
	}	

	if ( m_DrawChatTailSpr3.pImage )
	{
		GP.FreeSpr( m_DrawChatTailSpr3 );
	}

	if ( m_DrawChatSpr4.pImage )
	{
		GP.FreeSpr( m_DrawChatSpr4 );
	}	

	if ( m_DrawChatTailSpr4.pImage )
	{
		GP.FreeSpr( m_DrawChatTailSpr4 );
	}

	if ( m_DrawChatSpr5.pImage )
	{
		GP.FreeSpr( m_DrawChatSpr5 );
	}	

	if ( m_DrawChatTailSpr5.pImage )
	{
		GP.FreeSpr( m_DrawChatTailSpr5 );
	}

	if( m_DrawShopTitleImage.pImage )
	{
		GP.FreeSpr( m_DrawShopTitleImage);
	}

	if( m_lpSurface )
	{
		m_lpSurface->Release();
		m_lpSurface = NULL;
	}

	return;
}

LPDIRECTDRAWSURFACE7 CDrawChatSurface::GetChatSurface()
{
	return m_lpSurface;
}

TSpr *CDrawChatSurface::GetDrawChatImage()
{
	return &m_DrawChatSpr;
}

TSpr	*CDrawChatSurface::GetDrawChatTailImage()
{
	return &m_DrawChatTailSpr;
}

TSpr *CDrawChatSurface::GetDrawChatImage1()
{
	return &m_DrawChatSpr1;
}

TSpr	*CDrawChatSurface::GetDrawChatTailImage1()
{
	return &m_DrawChatTailSpr1;
}

TSpr *CDrawChatSurface::GetDrawChatImage2()
{
	return &m_DrawChatSpr2;
}

TSpr	*CDrawChatSurface::GetDrawChatTailImage2()
{
	return &m_DrawChatTailSpr2;
}

TSpr *CDrawChatSurface::GetDrawChatImage3()
{
	return &m_DrawChatSpr3;
}

TSpr	*CDrawChatSurface::GetDrawChatTailImage3()
{
	return &m_DrawChatTailSpr3;
}

TSpr *CDrawChatSurface::GetDrawChatImage4()
{
	return &m_DrawChatSpr4;
}

TSpr	*CDrawChatSurface::GetDrawChatTailImage4()
{
	return &m_DrawChatTailSpr4;
}


TSpr *CDrawChatSurface::GetDrawChatImage5()
{
	return &m_DrawChatSpr5;
}

TSpr	*CDrawChatSurface::GetDrawChatTailImage5()
{
	return &m_DrawChatTailSpr5;
}

TSpr	*CDrawChatSurface::GetDrawShopTitleImage()
{
	return &m_DrawShopTitleImage;
}