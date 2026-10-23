#include "../common/Item/ItemCommon/ItemUnit.h"
#include "../CommonLogic/CommonLogic.h"

//-----------------------------
// Common
//-----------------------------
#include "../Client/Client.h"

#include "../common/PersonInfo.h"
#include "../common/Char/CharManager/CharManager.h"

#include "../Client/MiniGame/Roulette/RouletteLogic.h"

#include "../CommonLogic/MsgType-Item.h"
#include "../CommonLogic/MsgType.h"


extern cltCommonLogic* pclClient;

CRouletteLogic::CRouletteLogic()
{
	Init();
}

CRouletteLogic::~CRouletteLogic()
{
}

void CRouletteLogic::Init()
{
	SI32 x = 0, y = 0;
	for( y = 0 ; y< 5; ++y )
	{
		m_siRandomValue[y] = 0;

		for( x = 0; x < 5; ++x )
		{
			m_stRouletteTable[y][x].m_bUserSelect = false;
			m_stRouletteTable[y][x].m_bCpuSelect  = false;
			m_stRouletteTable[y][x].m_bResult     = false;

			m_stRouletteTable[y][x].m_siX = x * ROULETTE_WIDTH  + ROULETTE_X;
			m_stRouletteTable[y][x].m_siY = y * ROULETTE_HEIGHT + ROULETTE_Y;
			m_stRouletteTable[y][x].m_siValue = y*5 + (x + 1);
		}
	}

	m_siBettingCount = 0;
	m_bStart = false;
}

void CRouletteLogic::StartRoulette()
{
	if( m_siBettingCount <= 0)
		return;

	m_bStart = true;

	cltGameMsgRequest_RouletteGame_Info clinfo(m_stRouletteTable);
	cltMsg clMsg(GAMEMSG_REQUEST_ROULETTEGAME_INFO,sizeof(clinfo),(BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CRouletteLogic::CheckRoulette()
{
	SI32 x = 0, y = 0, z=0;
	for( y = 0 ; y< 5; ++y )
	{
		for( x = 0; x < 5; ++x )
		{
			if( m_stRouletteTable[y][x].m_bUserSelect == true)
			{
				for( z = 0; z<5; ++z)
				{
					if( m_stRouletteTable[y][x].m_siValue == m_siRandomValue[z])
						m_stRouletteTable[y][x].m_bResult = true;
				}
			}
		}
	}
}

bool CRouletteLogic::SetMouseXY(SI32 x, SI32 y)
{
	m_Mouse.x = x;
	m_Mouse.y = y;

	if( m_Mouse.x > (ROULETTE_WIDTH + 6)*5 )
	{
		return false;
	}
	if( m_Mouse.y > (ROULETTE_HEIGHT + 6)*5 )
	{
		return false;
	}

	SI32 SelectRoulettX = (m_Mouse.x ) / (ROULETTE_WIDTH + 6);
	SI32 SelectRoulettY = (m_Mouse.y ) / (ROULETTE_HEIGHT + 6);

	m_stRouletteTable[SelectRoulettY][SelectRoulettX].m_bUserSelect = !m_stRouletteTable[SelectRoulettY][SelectRoulettX].m_bUserSelect;
	
	if(m_stRouletteTable[SelectRoulettY][SelectRoulettX].m_bUserSelect == true)
	{
		if(m_siBettingCount >= GetGourdNumber())
		{
			m_stRouletteTable[SelectRoulettY][SelectRoulettX].m_bUserSelect = false;
			return false;
		}
		else
		{
			m_siBettingCount++;
			// 抗寇贸府
			if(m_siBettingCount > 25)
				m_siBettingCount = 25;
		}
	}
	else
	{
		m_siBettingCount--;
		// 抗寇贸府
		if(m_siBettingCount < 0)
			m_siBettingCount = 0;
	}

	return true;
}

void CRouletteLogic::SetRandomValue(SI16 randomvalue[])
{
	m_siRandomValue[0] = randomvalue[0];
	m_siRandomValue[1] = randomvalue[1];
	m_siRandomValue[2] = randomvalue[2];
	m_siRandomValue[3] = randomvalue[3];
	m_siRandomValue[4] = randomvalue[4];
}

SI32 CRouletteLogic::GetGourdNumber()
{
	SI32 Number = 0;

	Number = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(8200);
	
	if(Number < 0)
		Number = 0;
	return Number;
}

void CRouletteLogic::StopRoulette()
{
	cltMsg clMsg(GAMEMSG_REQUEST_ROULETTEGAME_STOP, 0, NULL);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}