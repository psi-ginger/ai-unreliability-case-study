#include "../Client/Client.h"
#include "../CommonLogic/CommonLogic.h"
#include "../Client/MiniGame\MiniGameManager.h"
#include "../Client/MiniGame/ZooKeeperLogic.h"
#include "../Client/MiniGame/ZooKeeperDraw.h"

void CZooKeeperBase::Draw(SI32 x, SI32 y)
{
	DrawTable(x,y);
	if(GAME_STATE > INIT_BLOCK)
	{
		static float Time=0.0f, Last=0;

		LARGE_INTEGER m_ticksPerSecond;

		if(!QueryPerformanceFrequency(&m_ticksPerSecond))
		{
			return;
		}

		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);

		Time = ((float)currentTime.QuadPart  / (float)m_ticksPerSecond.QuadPart);

		if(Time - Last >=1)
		{
			Last = ((float)currentTime.QuadPart  / (float)m_ticksPerSecond.QuadPart);

			m_ZooKeeperLogic->m_GameTime--;
			if(m_ZooKeeperLogic->m_GameTime < 0)
				GAME_STATE = GAME_OVER;
		}
	}
}
void CZooKeeperBase::DrawTable(SI32 dlgx, SI32 dlgy)
{
	SI32 BlockValue = 0;
	SI32 x = 0, y = 0;
	SI32 Temp = 0;

	m_ZooDraw->DrawBlockBack(dlgx, dlgy);

	DrawTimeBar(dlgx, dlgy, (SI32)m_ZooKeeperLogic->GetGameTime());

	DrawScore( dlgx + 345, dlgy + 50, m_ZooKeeperLogic->GetGameScore());


//	Printf(dlgx,  dlgy , RGB(255, 0, 0), TEXT("%d"), m_ZooKeeperLogic->GetTrueGameScore());

	for(y=0;y<8;++y)
	{
		for(x=0;x<8;++x)
		{
			BlockValue = m_ZooKeeperLogic->	GetBlockValue(y, x);

			if(EMPTY_TABLE < BlockValue )
			{
				if(SPECIAL_BLOCK_VALUE == BlockValue)
				{
					m_ZooDraw->DrawSpecialBlock(dlgx + TABLE_X +m_ZooKeeperLogic->GetBlockXPosition(y, x),
						dlgy + m_ZooKeeperLogic->m_MoveY + m_ZooKeeperLogic->GetBlockYPosition(y, x));
				}
				else
				{
					if(TRUE == m_ZooKeeperLogic->m_BlockTableXY[y][x].m_bSelectImage)
					{
						m_ZooDraw->DrawSelectBlock(dlgx + TABLE_X + m_ZooKeeperLogic->GetBlockXPosition(y, x),
							dlgy +  m_ZooKeeperLogic->m_MoveY + m_ZooKeeperLogic->GetBlockYPosition(y, x),
							BlockValue);
					}
					else
						m_ZooDraw->DrawBlock(dlgx + TABLE_X + m_ZooKeeperLogic->GetBlockXPosition(y, x),
						dlgy + m_ZooKeeperLogic->m_MoveY + m_ZooKeeperLogic->GetBlockYPosition(y, x),
						BlockValue);
					if(TRUE == m_ZooKeeperLogic->GetBlockIsSelect(y,x) )
					{
						m_ZooDraw->DrawSelect(dlgx + TABLE_X + m_ZooKeeperLogic->GetBlockXPosition(y, x), dlgy + m_ZooKeeperLogic->m_MoveY + m_ZooKeeperLogic->GetBlockYPosition(y,x));
					}
				}
			//	if(FALSE_BLOCK == GAME_STATE)
			//	{
			//		Printf(14 + dlgx + TABLE_X + m_ZooKeeperLogic->GetBlockXXPosition(m_ZooKeeperLogic->GetFirstClickBlockY(), m_ZooKeeperLogic->GetFirstClickBlockX()), 10 + dlgy + m_ZooKeeperLogic->m_MoveY + m_ZooKeeperLogic->GetBlockYYPosition(m_ZooKeeperLogic->GetFirstClickBlockY(), m_ZooKeeperLogic->GetFirstClickBlockX()), RGB(255, 0, 0),TEXT("-%d"), PENALTYSCORE);
			//		Printf(14 + dlgx + TABLE_X + m_ZooKeeperLogic->GetBlockXXPosition(m_ZooKeeperLogic->GetFirstClickBlockY(), m_ZooKeeperLogic->GetFirstClickBlockX()), 24 + dlgy + m_ZooKeeperLogic->m_MoveY + m_ZooKeeperLogic->GetBlockYYPosition(m_ZooKeeperLogic->GetFirstClickBlockY(), m_ZooKeeperLogic->GetFirstClickBlockX()), RGB(255, 0, 0),TEXT("-%d"), PENALTYTIME);
			//	}

			}
			else if(EMPTY_TABLE > BlockValue)
			{
				BlockValue = (BlockValue*-1) - 2;

				switch(GAME_STATE)
				{
				case CHECK_BLOCK:
				case SPECIAL_BLOCK:
					m_ZooDraw->DrawRemoveBlock(dlgx + TABLE_X + m_ZooKeeperLogic->GetBlockXPosition(y, x), dlgy + m_ZooKeeperLogic->m_MoveY + m_ZooKeeperLogic->GetBlockYPosition(y,x),BlockValue );
					break;
				case EFFECT_BLOCK:
					m_ZooDraw->DrawEffectBlock(dlgx + TABLE_X + m_ZooKeeperLogic->GetBlockXPosition(y, x), dlgy + m_ZooKeeperLogic->m_MoveY + m_ZooKeeperLogic->GetBlockYPosition(y,x));
				//	Printf(14 + dlgx + TABLE_X + m_ZooKeeperLogic->GetBlockXPosition(y, x), 16 + dlgy + m_ZooKeeperLogic->m_MoveY + m_ZooKeeperLogic->GetBlockYPosition(y,x), RGB(0, 0, 255),TEXT("+%d"), SCORE * (m_ZooKeeperLogic->m_Combo));
					break;
				}
			}
		}
	}
}

void CZooKeeperBase::DrawTimeBar(SI32 x, SI32 y, SI32 Time)
{
	m_ZooDraw->DrawTime2( x + 260, y + 16);

	SI32 i;
	// 좀 무식하다(나중에 수정이 필요할지도 모른다)
	SI32 drawnum =  min(96, ( Time * 96 * 2 / GAME_TIME ) / 2);

	for(i=drawnum; i >  -1 ;--i)
	{
		m_ZooDraw->DrawTime1(x + 263 + i *2, y + 20);
	}
//	m_ZooDraw->DrawTime3(x, y + 100);

}

void CZooKeeperBase::DrawScore(SI32 x, SI32 y, SI32 Score)
{
	SI32 Temp = 0;
	for(SI32 i  = 1000000; i>=1 ; i /=10)
	{
		Temp = (Score / i) % 10;
		m_ZooDraw->DrawNumber(x, y, Temp);
		x += 17;
	}
}

void CZooKeeperBase::Printf(int x, int y, int Color, TCHAR* szStr, ...)
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