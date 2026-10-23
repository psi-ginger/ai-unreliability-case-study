#include "VillageNotice.h"

#include "../../Client/client.h"
extern cltCommonLogic* pclClient;


#define VILLAGENOTICE_WIDTH				250
#define VILLAGENOTICE_HEIGHT			130

CVillageNotice::CVillageNotice()
{
	m_siVillageUnique = 0;
	m_bSetVillageNotice = false;
	memset( m_strVillageNotice, 0, MAX_VILLAGENOTICE_LENGTH );
}

CVillageNotice::~CVillageNotice()
{
	if ( m_VillageNoticeSpr.pImage )
	{
		GP.FreeSpr( m_VillageNoticeSpr );
	}
}

bool CVillageNotice::Set( SI32 VillageUnique, SI32 XPos, SI32 YPos )
{
	m_siXPos = XPos;
	m_siYPos = YPos;

	if ( VillageUnique == m_siVillageUnique )
	{
		return false;
	}

	m_siVillageUnique = VillageUnique;	

	if ( m_siVillageUnique == 0 )
	{
		m_bSetVillageNotice = false;
	}

	return true;
}

void CVillageNotice::Set( TCHAR *szVillageNotice, bool SetVillageNotice )
{
	if ( m_siVillageUnique == 0 )
	{
		return;
	}

	m_bSetVillageNotice = SetVillageNotice;
	
	if ( m_bSetVillageNotice )
	{
		MStrCpy( m_strVillageNotice, szVillageNotice, MAX_VILLAGENOTICE_LENGTH );
	}
	else
	{
		memset( m_strVillageNotice, 0, MAX_VILLAGENOTICE_LENGTH );
	}
	
	return;
}

void CVillageNotice::Draw( LPDIRECTDRAWSURFACE7 lpsurface, SI32 ScreenXPos, SI32 ScreenYPos )
{
	if ( m_siVillageUnique == 0 )
	{
		return;
	}

	if ( ! m_bSetVillageNotice )
	{
		return;
	}

	TSpr* pspr = pclClient->GetGlobalSpr(GIMG_VILLAGENOTICEBACKGROUND);

	if( GP.LockSurface( lpsurface ) == TRUE )
	{


		GP.PutSprAlpha(pspr, m_siXPos + ScreenXPos - pspr->GetXSize() / 2, 
							m_siYPos + ScreenYPos - pspr->GetYSize(), 12, 0 );

		GP.UnlockSurface( lpsurface );
	}
	
	HDC hdc;
	RECT rect;
	rect.left = 10 + m_siXPos + ScreenXPos - pspr->GetXSize() / 2;
	rect.right = rect.left + 230;
	rect.top = 10 + m_siYPos + ScreenYPos  - pspr->GetYSize();
	rect.bottom = rect.top + 110;

	HFONT hFont, hOldFont;
	if ( pclClient->siServiceArea == ConstServiceArea_English	||
		pclClient->siServiceArea == ConstServiceArea_NHNChina	||
		pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
	{
		hFont = g_FontManager.SetFont( 13 , TEXT("Tahoma") );
	}
	else
	{
		hFont = g_FontManager.SetFont( 13 );
	}

	if( lpsurface->GetDC(&hdc) == DD_OK)
	{				
		hOldFont = (HFONT)SelectObject( hdc, hFont );
		SetBkMode( hdc, TRANSPARENT );


		DrawText( hdc, m_strVillageNotice, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK );
	
		SelectObject( hdc, hOldFont );

		lpsurface->ReleaseDC(hdc);
	}
	
	return;
}