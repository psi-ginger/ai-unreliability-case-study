#include "../Client/Client.h"
#include "../CommonLogic/CommonLogic.h"
#include "../Client/MiniGame\MiniGameManager.h"
#include "../Client/MiniGame/Roulette/RouletteLogic.h"
#include "../Client/MiniGame/Roulette/RouletteDraw.h"
#include "../Client/Music/music.h"

extern cltCommonLogic* pclClient;


void CRouletteBase::Draw(SI32 x, SI32 y)
{
	DrawRoulette(x, y);
}

void CRouletteBase::DrawRoulette(SI32 dlgx, SI32 dlgy)
{
	SI32 x = 0, y = 0;

	if( START_ROULETTE ==  GAME_STATE || CHECK_ROULETTE == GAME_STATE)
	{
		for(x=0;x<5;++x)
		{
			if( true == m_pRouletteLogic->GetResult(m_siRandomY[x], m_siRandomX[x]) )
			{
			//	if(pclClient->GetFrame() % 20 == 0)
					m_pRouletteDraw->DrawCpuSelect(dlgx - 5 + 2*x + m_pRouletteLogic->GetXPosition(0,x), dlgy - 90 + m_pRouletteLogic->GetYPosition(0,x) , 5);
			}
			if( true == m_bSelect[x])
				DrawValue(dlgx + 10 + 2*x + m_pRouletteLogic->GetXPosition(0,x) , dlgy - 75 + m_pRouletteLogic->GetYPosition(0,x), m_pRouletteLogic->GetRandom(x));
			else
				DrawRoll(dlgx - 4 + 3*x + m_pRouletteLogic->GetXPosition(0,x) , dlgy - 87 + m_pRouletteLogic->GetYPosition(0,x), m_siDelay);
		}

		DrawCpuSelect(dlgx, dlgy);
	}

	DrawValue2(dlgx + 502 , dlgy + 347, m_siNumber);
	DrawValue2(dlgx + 502 , dlgy + 418, m_pRouletteLogic->GetBettingCount());

	for(y=0;y<5;++y)
	{
		for(x=0;x<5;++x)
		{
			DrawValue(dlgx + 10 + 2*x + m_pRouletteLogic->GetXPosition(y,x) , dlgy + 5 + 3*y + m_pRouletteLogic->GetYPosition(y,x),m_pRouletteLogic->GetValue(y,x));

			if( true == m_pRouletteLogic->GetUserSelect(y,x) )
				DrawUserSelect(dlgx + 6 + 3*x +m_pRouletteLogic->GetXPosition(y,x), dlgy + 2*y + m_pRouletteLogic->GetYPosition(y,x));
			
		}
	}

	if( END_ROULETTE == GAME_STATE )
		DrawBird();
}

void CRouletteBase::DrawValue(SI32 x, SI32 y, SI32 value)
{
	SI32 Temp = 0;
	for(SI32 i  = 10; i>=1 ; i /=10)
	{
		Temp = (value / i) % 10;
		m_pRouletteDraw->DrawNumber(x, y, Temp);
		x += 17;
	}
}

void CRouletteBase::DrawValue2(SI32 x, SI32 y, SI32 value)
{
	SI32 Temp = 0;
	for(SI32 i  = 1000; i>=1 ; i /=10)
	{
		Temp = (value / i) % 10;
		m_pRouletteDraw->DrawNumber2(x, y, Temp);
		x += 15;
	}
}

void CRouletteBase::Printf(int x, int y, int Color, TCHAR* szStr, ...)
{
	TCHAR Buf[256];
	va_list VList;
	va_start(VList, szStr);
	StringCchVPrintf(Buf, 256, szStr, VList);
	va_end(VList);

	HDC hDC;
	if(g_graphics.m_pddsBackBuffer->GetDC(&hDC) == DD_OK)
	{
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, Color);
		TextOut(hDC, x, y, Buf, lstrlen(Buf));
		g_graphics.m_pddsBackBuffer->ReleaseDC(hDC);
	}
}

void CRouletteBase::DrawUserSelect(SI32 x, SI32 y)
{
	m_pRouletteDraw->DrawUserSelect(x, y);
}

void CRouletteBase::DrawCpuSelect(SI32 dlgx, SI32 dlgy)
{
	SI32 y = 0;
	for(y=0;y<5;++y)
	{
		if( true == m_pRouletteLogic->GetResult(m_siRandomY[y], m_siRandomX[y]) )
		{
			m_pRouletteDraw->DrawCpuSelect(dlgx + m_pRouletteLogic->GetXPosition(m_siRandomY[y],m_siRandomX[y]) - 6 + 3*m_siRandomX[y], dlgy + m_pRouletteLogic->GetYPosition(m_siRandomY[y], m_siRandomX[y]) - 14 + 3*m_siRandomY[y] , 5);
		}
		else
		{
			m_pRouletteDraw->DrawCpuSelect(dlgx + m_pRouletteLogic->GetXPosition(m_siRandomY[y],m_siRandomX[y]) - 6 + 3*m_siRandomX[y], dlgy + m_pRouletteLogic->GetYPosition(m_siRandomY[y], m_siRandomX[y]) - 14 + 3*m_siRandomY[y] , 4);

		}

/*		if( true == m_pRouletteLogic->GetResult(m_siRandomY[y], m_siRandomX[y]) )
		{
			if(pclClient->GetFrame() % 20 == 0)
				m_pRouletteDraw->DrawCpuSelect(dlgx + m_pRouletteLogic->GetXPosition(m_siRandomY[y],m_siRandomX[y]) - 6 + 3*m_siRandomX[y], dlgy + m_pRouletteLogic->GetYPosition(m_siRandomY[y], m_siRandomX[y]) - 14 + 3*m_siRandomY[y] , 5);
		}
		else
		{
			m_pRouletteDraw->DrawCpuSelect(dlgx + m_pRouletteLogic->GetXPosition(m_siRandomY[y],m_siRandomX[y]) - 6 + 3*m_siRandomX[y], dlgy + m_pRouletteLogic->GetYPosition(m_siRandomY[y], m_siRandomX[y]) - 14 + 3*m_siRandomY[y] , 4);

		}*/
	}
}

void CRouletteBase::DrawResult(SI32 x, SI32 y)
{
	m_pRouletteDraw->DrawCpuSelect(x, y, 5);
}

void CRouletteBase::DrawBird()
{
	PlaySound(pclClient, pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SWALLOW")));
	m_pRouletteDraw->DrawBird(m_siBirdX, 300);

	if(m_siBirdX > 300)
		DrawUserSelect(m_siBirdX + 120, 300 + 30);
}

void CRouletteBase::DrawRoll(SI32 x, SI32 y, SI32 delay)
{
	m_pRouletteDraw->DrawRoll(x, y, delay);
}
