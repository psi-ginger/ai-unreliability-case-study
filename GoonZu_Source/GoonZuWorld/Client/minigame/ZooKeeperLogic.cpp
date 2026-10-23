#include "../Client/Client.h"
#include "../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-Item.h"
#include "../../CommonLogic/MsgType.h"
#include "../Client\MiniGame/ZooKeeperLogic.h"

extern cltCommonLogic* pclClient;

CZooKeeper::CZooKeeper()
{
	Init();
}

CZooKeeper::~CZooKeeper()
{
}

void CZooKeeper::Init()
{
	m_BlockDownMoveValue = BLOCK_DOWNSPEED;
	m_BlockSwapMoveValue = BLOCK_SWAPSPEED;;
	m_BlockWidth = BLOCK_WIDTH;
	m_BlockHeight = BLOCK_HEIGHT;
	
	m_Mouse.x = 0;
	m_Mouse.y = 0;

	m_FirstClickBlockValue = 0;
	m_SecondClickBlockValue = 0;
	m_FirstClickBlockX = 0;
	m_FirstClickBlockY = 0;
	m_SecondClickBlockX = 0;
	m_SecondClickBlockY = 0;
	m_bClickState = FALSE;
	m_GameRunState =  FALSE;

	m_MoveY = 0;
	m_GameTime = GAME_TIME;
	m_bXCheckState = FALSE;
	m_bYCheckState = FALSE;
	m_GameScore = 0;
	m_TrueGameScore = 0;
	m_Combo = 1;
	m_DeleteBlockCount = 0;
	m_bSpecialBlockLive = FALSE;
}

void CZooKeeper::SetBlockTable()
{
	UI32 x, y;

	for(y=0;y<9;++y)
	{
		for(x=0;x<9;++x)
		{
			if( y == 8 || x == 8)
			{
				m_BlockTableXY[y][x].m_X = x * m_BlockWidth;
				m_BlockTableXY[y][x].m_Y = y * m_BlockHeight;

				m_BlockTableX[y][x].m_X = m_BlockTableXY[y][x].m_X;
				m_BlockTableX[y][x].m_Y = m_BlockTableXY[y][x].m_Y;

				m_BlockTableY[y][x].m_X = m_BlockTableXY[y][x].m_X;
				m_BlockTableY[y][x].m_Y = m_BlockTableXY[y][x].m_Y;

				m_BlockTableXY[y][x].m_Value = 100;
				m_BlockTableX[y][x].m_Value = 100;
				m_BlockTableY[y][x].m_Value = 100;

				
			}
			else
			{
				m_BlockTableXY[y][x].m_X = x * m_BlockWidth;
				m_BlockTableXY[y][x].m_Y = y * m_BlockHeight;

				m_BlockTableX[y][x].m_X = m_BlockTableXY[y][x].m_X;
				m_BlockTableX[y][x].m_Y = m_BlockTableXY[y][x].m_Y;

				m_BlockTableY[y][x].m_X = m_BlockTableXY[y][x].m_X;
				m_BlockTableY[y][x].m_Y = m_BlockTableXY[y][x].m_Y;

				m_BlockTableXY[y][x].m_destX = m_BlockTableXY[y][x].m_X;
				m_BlockTableXY[y][x].m_destY = m_BlockTableXY[y][x].m_Y;

				m_BlockTableX[y][x].m_destX = m_BlockTableXY[y][x].m_X;
				m_BlockTableX[y][x].m_destY = m_BlockTableXY[y][x].m_Y;

				m_BlockTableY[y][x].m_destX = m_BlockTableXY[y][x].m_X;
				m_BlockTableY[y][x].m_destY = m_BlockTableXY[y][x].m_Y;

				if(FALSE == m_bSpecialBlockLive)
					m_BlockTableXY[y][x].m_Value = rand()%8;
				else
					m_BlockTableXY[y][x].m_Value = rand()%7;

				if(SPECIAL_BLOCK_VALUE == m_BlockTableXY[y][x].m_Value)
					m_bSpecialBlockLive = TRUE;

				m_BlockTableX[y][x].m_Value = m_BlockTableXY[y][x].m_Value;
				m_BlockTableY[y][x].m_Value = m_BlockTableXY[y][x].m_Value;
				
				m_BlockTableXY[y][x].m_movtype = 0;
				m_BlockTableX[y][x].m_movtype = 0;
				m_BlockTableY[y][x].m_movtype = 0;

				m_BlockTableXY[y][x].m_bSelect = FALSE;
				m_BlockTableX[y][x].m_bSelect = FALSE;
				m_BlockTableX[y][x].m_bSelect = FALSE;

				m_BlockTableXY[y][x].m_bSelectImage = FALSE;
				m_BlockTableX[y][x].m_bSelectImage = FALSE;
				m_BlockTableX[y][x].m_bSelectImage = FALSE;
			}
		}
	}

}

void CZooKeeper::SetScore(SI32 score, SI32 combo )
{
	m_GameScore += score * combo;
	m_TrueGameScore += (score / 10 + (combo-1)) << 2;

	cltGameMsgRequest_MonsterCorp_ComboInfo clinfo(combo, score);
	cltMsg clMsg(GAMEMSG_REQUEST_MONSTERCORP_COMBOINFO,sizeof(clinfo),(BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CZooKeeper::SetTime(SI32 time)
{
	m_GameTime += TIMEPLUS;
	if(m_GameTime > GAME_TIME)
		m_GameTime = GAME_TIME;

}

BOOL CZooKeeper::HorizonalCheck()
{
	SI32 x = 0, y = 0, z = 0, k = 0;
	SI32 BlockValue = 0;
	SI32 Count = 0;
	SI32 Num;

	BOOL bFlag=FALSE;
	// 세로 방향으로...
	for(y=0 ; y<8 ; ++y)
	{
		Num = 1;
		BlockValue = m_BlockTableX[y][0].m_Value;

		// 가로 방향으로...
		for(x=1 ; x<9 ; ++x)
		{
			// 같은 블록이 나오면 Num을 증가
			if( m_BlockTableX[y][x].m_Value == BlockValue ) Num++;
			else
			{
				// 같은 블록이 3개 이상 나왔으면... 
				if((Num>=3) && (BlockValue > EMPTY_TABLE) )
				{
					m_DeleteBlockCount++;
				//	m_Combo++;
					SetTime(TIMEPLUS);
												
					bFlag = TRUE;
					// 지워질 블록으로 기억해 둠
					for(k=Num ; k>0 ; k--)
					{
						SetScore(SCORE, m_Combo);
						m_BlockTableX[y][x-k].m_Value = (BlockValue*-1) - 2;
					}
				}

				// 새로운 블록으로 검색을 다시 시작
				Num = 1;
				BlockValue = m_BlockTableX[y][x].m_Value;
			}
		}
	}
	return bFlag;
}

BOOL CZooKeeper::VerticalCheck()
{
	SI32 x = 0, y = 0, z = 0, k = 0;
	SI32 BlockValue = 0;
	SI32 Count = 0;
	SI32 Num;

	BOOL bFlag=FALSE;

	for(x=0 ; x<8 ; ++x)
	{
		Num = 1;
		BlockValue = m_BlockTableY[0][x].m_Value;

		// 가로 방향으로...
		for(y=1 ; y<9 ; ++y)
		{
			// 같은 블록이 나오면 Num을 증가
			if( m_BlockTableY[y][x].m_Value == BlockValue ) Num++;
			else
			{
				// 같은 블록이 3개 이상 나왔으면... 
				if((Num>=3) && (BlockValue > EMPTY_TABLE) )
				{
					m_DeleteBlockCount++;
				//	m_Combo++;
					SetTime(TIMEPLUS);
					
					bFlag = TRUE;
					// 지워질 블록으로 기억해 둠
					for(k=Num ; k>0 ; k--)
					{
						SetScore(SCORE, m_Combo);
						m_BlockTableY[y-k][x].m_Value = (BlockValue*-1) - 2;
					}
				}

				// 새로운 블록으로 검색을 다시 시작
				Num = 1;
				BlockValue = m_BlockTableY[y][x].m_Value;
			}
		}
	}
	return bFlag;
}

void CZooKeeper::SpecialCheck()
{
	SI32 x = 0, y = 0;
	SI32 TempValue =0;
	SI32 Count = 0;

	m_bSpecialBlockLive = FALSE;

	TempValue = rand()%7;
	m_Combo = 1;
	for(y=0 ; y<8 ; ++y)
	{
		for(x=0; x<8;++x)
		{
			if(TempValue == m_BlockTableY[y][x].m_Value)
			{
				m_BlockTableXY[y][x].m_Value = (TempValue*-1) - 2;
				Count++;
			}
			if(SPECIAL_BLOCK_VALUE == m_BlockTableY[y][x].m_Value)
			{
				m_BlockTableXY[y][x].m_Value = (TempValue*-1) - 2;
				Count++;
			}
		}
	}
	
	SetTime(TIMEPLUS*5);
	SetScore(SCORE, Count);
}

void CZooKeeper::ResetTable()
{
	SI32 x = 0, y = 0;
	SI32 BlockValue = 0;

	for(y=0;y<8;++y)
	{
		for(x=0;x<8;++x)
		{

			m_BlockTableXY[y][x].m_bSelect = FALSE;
			m_BlockTableXY[y][x].m_bSelectImage = FALSE;
			if(EMPTY_TABLE > m_BlockTableX[y][x].m_Value)
				m_BlockTableXY[y][x].m_Value = m_BlockTableX[y][x].m_Value;
			if(EMPTY_TABLE > m_BlockTableY[y][x].m_Value)
				m_BlockTableXY[y][x].m_Value = m_BlockTableY[y][x].m_Value;
		}
	}
}

void CZooKeeper::ResetXYTable()
{
	SI32 x = 0, y = 0;

	for(y=0;y<8;++y)
	{
		for(x=0;x<8;++x)
		{
		//	m_BlockTableXY[y][x].m_Y = y * m_BlockHeight;
			m_BlockTableX[y][x].m_Value = m_BlockTableXY[y][x].m_Value;
			m_BlockTableY[y][x].m_Value = m_BlockTableXY[y][x].m_Value;

		}
	}
}

void CZooKeeper::BlockDown()
{
	SI32 x = 0, y = 0, k = 0, l = 0;
	SI32 BlockValue = 0;
	SI32 Count =0;

	for(x=0; x<8; ++x)	
	{
		for(y=7; y>-1; y--)
		{
			if(m_BlockTableXY[y][x].m_Value < EMPTY_TABLE)
			{
				for(k=y-1; k>-1; k--)
				{
					if(m_BlockTableXY[k][x].m_Value > EMPTY_TABLE)
						break;
				}
				if(k>-1)
				{
					SI32 TempBlockValue = 0;
					for(l=k; l>-1;l--)
					{
						TempBlockValue = m_BlockTableXY[y-(k-l)][x].m_Value;
						m_BlockTableXY[y-(k-l)][x].m_Value = m_BlockTableXY[l][x].m_Value;
						m_BlockTableXY[y-(k-l)][x].m_Y = m_BlockTableXY[l][x].m_Y;
						m_BlockTableXY[l][x].m_Value = TempBlockValue;
					}
				}
				else 
					break;

			}
		}
	}

	SI32 Temp = 0;
	for(x=0;x<8;++x)
	{
		for(y=7;y>-1;--y)
		{
			if(m_BlockTableXY[y][x].m_Value < EMPTY_TABLE)
				Count++;
		}
		for(y=7;y>-1;--y)
		{
			if(m_BlockTableXY[y][x].m_Value < EMPTY_TABLE)
			{
				Temp = rand() % 10000;

				if(Temp > SPECIAL_RAND)
					m_BlockTableXY[y][x].m_Value = rand()%7;
				else
				{
					if(m_bSpecialBlockLive == FALSE)
					{
						m_BlockTableXY[y][x].m_Value = rand()%8;
						if(SPECIAL_BLOCK_VALUE == m_BlockTableXY[y][x].m_Value)
							m_bSpecialBlockLive = TRUE;
					}
					else
						m_BlockTableXY[y][x].m_Value = rand()%7;
				}
				m_BlockTableXY[y][x].m_Y = m_BlockTableY[y][x].m_Y - Count* m_BlockHeight;
				m_BlockTableXY[y][x].m_movtype = MOVDOWN;
			}
			if(m_BlockTableXY[y][x].m_Y < m_BlockTableY[y][x].m_Y)
				m_BlockTableXY[y][x].m_movtype = MOVDOWN;
		}
		Count = 0;	
	}
}

void CZooKeeper::SetBlockPositionInit()
{
	SI32 x,y;

	for(y=0;y<8;++y)
	{
		for(x=0;x<8;++x)
		{
			m_BlockTableXY[y][x].m_X = x * m_BlockWidth;
			m_BlockTableXY[y][x].m_Y = y * m_BlockHeight;
	
		}
	}
}

BOOL CZooKeeper::SetMouseXY(SI32 x, SI32 y)
{
	m_Mouse.x = x;
	m_Mouse.y = y;

	if(m_Mouse.x < TABLE_X  || m_Mouse.x > (TABLE_X  + m_BlockWidth*8))
	{
		m_SecondClickBlockValue = 0;
		return FALSE;
	}
	if(m_Mouse.y < TABLE_Y  || m_Mouse.y > (TABLE_Y   + m_BlockHeight*8))
	{
		m_SecondClickBlockValue = 0;
		return FALSE;
	}

	m_FirstClickBlockX = (m_Mouse.x - TABLE_X ) / m_BlockWidth;
	m_FirstClickBlockY = (m_Mouse.y - TABLE_Y ) / m_BlockHeight;

	m_FirstClickBlockValue = m_BlockTableXY[m_FirstClickBlockY ][m_FirstClickBlockX].m_Value;

	if(SPECIAL_BLOCK_VALUE == m_FirstClickBlockValue)
	{
		SpecialCheck();
		return TRUE;
	}
	else
	{
		m_BlockTableXY[m_FirstClickBlockY ][m_FirstClickBlockX].m_bSelect = TRUE;
		m_BlockTableXY[m_FirstClickBlockY ][m_FirstClickBlockX].m_bSelectImage = TRUE;

		m_bClickState = TRUE;

		return FALSE;
	}

}

void CZooKeeper::SwapBlockValue(SI32* FirstValue, SI32* SecondValue)
{
	if((m_SecondClickBlockX == m_FirstClickBlockX) && (m_SecondClickBlockY == m_FirstClickBlockY))
		return;

	else if(((m_SecondClickBlockX == m_FirstClickBlockX -1)  || ( m_SecondClickBlockX == m_FirstClickBlockX +1)) && (m_SecondClickBlockY == m_FirstClickBlockY))
	{
		SI32 Temp=0;

		Temp = *FirstValue;
		*FirstValue = *SecondValue;
		*SecondValue = Temp;

		m_FirstClickBlockValue = *FirstValue;
		m_SecondClickBlockValue = *SecondValue;

		m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_Value = m_FirstClickBlockValue;
		m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_Value = m_SecondClickBlockValue;
		m_BlockTableX[m_FirstClickBlockY][m_FirstClickBlockX].m_Value = m_FirstClickBlockValue;
		m_BlockTableY[m_FirstClickBlockY][m_FirstClickBlockX].m_Value = m_FirstClickBlockValue;
		m_BlockTableX[m_SecondClickBlockY][m_SecondClickBlockX].m_Value = m_SecondClickBlockValue;
		m_BlockTableY[m_SecondClickBlockY][m_SecondClickBlockX].m_Value = m_SecondClickBlockValue;
		m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_bSelectImage = FALSE;
		
	}
	else if(((m_SecondClickBlockY == m_FirstClickBlockY -1)  || ( m_SecondClickBlockY == m_FirstClickBlockY +1)) && (m_SecondClickBlockX == m_FirstClickBlockX))
	{
		SI32 Temp=0;

		Temp = *FirstValue;
		*FirstValue = *SecondValue;
		*SecondValue = Temp;

		m_FirstClickBlockValue = *FirstValue;
		m_SecondClickBlockValue = *SecondValue;

		m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_Value = m_FirstClickBlockValue;
		m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_Value = m_SecondClickBlockValue;
		m_BlockTableX[m_FirstClickBlockY][m_FirstClickBlockX].m_Value = m_FirstClickBlockValue;
		m_BlockTableY[m_FirstClickBlockY][m_FirstClickBlockX].m_Value = m_FirstClickBlockValue;
		m_BlockTableX[m_SecondClickBlockY][m_SecondClickBlockX].m_Value = m_SecondClickBlockValue;
		m_BlockTableY[m_SecondClickBlockY][m_SecondClickBlockX].m_Value = m_SecondClickBlockValue;
		m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_bSelectImage = FALSE;
	}
	else
		return;
}

BOOL CZooKeeper::SwapBlockCheck(SI32 x, SI32 y)
{
	m_Mouse.x = x;
	m_Mouse.y = y;

	if(m_Mouse.x < TABLE_X  || m_Mouse.x > (TABLE_X + m_BlockWidth*8))
	{
		m_SecondClickBlockValue = 0;
		return FALSE;
	}
	if(m_Mouse.y < TABLE_Y   || m_Mouse.y > (TABLE_Y + m_BlockHeight*8))
	{
		m_SecondClickBlockValue = 0;
		return FALSE;
	}
	
	m_SecondClickBlockX = (m_Mouse.x - TABLE_X) / m_BlockWidth;
	m_SecondClickBlockY = (m_Mouse.y - TABLE_Y) / m_BlockHeight;

	m_SecondClickBlockValue = m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_Value;

	if((m_SecondClickBlockX == m_FirstClickBlockX) && (m_SecondClickBlockY == m_FirstClickBlockY))
	{
		m_BlockTableXY[m_FirstClickBlockY ][m_FirstClickBlockX].m_bSelect = FALSE;
		m_BlockTableXY[m_FirstClickBlockY ][m_FirstClickBlockX].m_bSelectImage = FALSE;
		PenaltyData();
		m_bClickState = FALSE;
		return FALSE;
	}

	else if(((m_SecondClickBlockX == m_FirstClickBlockX -1)  || ( m_SecondClickBlockX == m_FirstClickBlockX +1)) && (m_SecondClickBlockY == m_FirstClickBlockY))
	{
		m_bClickState = FALSE;
		m_BlockTableXY[m_FirstClickBlockY ][m_FirstClickBlockX].m_bSelect = FALSE;
		InputMoveBlock();
		return TRUE;
	}
	else if(((m_SecondClickBlockY == m_FirstClickBlockY -1)  || ( m_SecondClickBlockY == m_FirstClickBlockY +1)) && (m_SecondClickBlockX == m_FirstClickBlockX))
	{
		m_bClickState = FALSE;
		m_BlockTableXY[m_FirstClickBlockY ][m_FirstClickBlockX].m_bSelect = FALSE;
		InputMoveBlock();
		return TRUE;
	}
	else 
	{
		PenaltyData();
		return FALSE;
	}
}

void CZooKeeper::InputMoveBlock()
{
	if(m_FirstClickBlockY > m_SecondClickBlockY)
	{
		m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_movtype = 3;
		m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_movtype = 4;

		m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_destY = m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_Y;//m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_Y;
		m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_destY = m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_Y;//m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_Y;
	}
	else if(m_FirstClickBlockY < m_SecondClickBlockY )
	{
		m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_movtype = 4;
		m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_movtype = 3;

		m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_destY = m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_Y;//m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_Y;
		m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_destY = m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_Y; //m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_Y;
	}
	else if(m_FirstClickBlockX > m_SecondClickBlockX)
	{
		m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_movtype = 1;
		m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_movtype = 2;

		m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_destX = m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_X;
		m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_destX = m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_X;

	}
	else if(m_FirstClickBlockX < m_SecondClickBlockX)
	{
		m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_movtype = 2;
		m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_movtype = 1;

		m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_destX = m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_X;
		m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_destX = m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_X;
	}
}

void CZooKeeper::InputMoveBlock2()
{
	if((m_SecondClickBlockX == m_FirstClickBlockX) && (m_SecondClickBlockY == m_FirstClickBlockY))
		return;

	else if(((m_SecondClickBlockX == m_FirstClickBlockX -1)  || ( m_SecondClickBlockX == m_FirstClickBlockX +1)) && (m_SecondClickBlockY == m_FirstClickBlockY))
	{
		if(m_FirstClickBlockX > m_SecondClickBlockX)
		{
			m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_movtype = 1;
			m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_movtype = 2;

			m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_destX = m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_X;
			m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_destX = m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_X;

		}
		else if(m_FirstClickBlockX < m_SecondClickBlockX)
		{
			m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_movtype = 2;
			m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_movtype = 1;

			m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_destX = m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_X;
			m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_destX = m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_X;
		}
		
	}
	else if(((m_SecondClickBlockY == m_FirstClickBlockY -1)  || ( m_SecondClickBlockY == m_FirstClickBlockY +1)) && (m_SecondClickBlockX == m_FirstClickBlockX))
	{
		if(m_FirstClickBlockY > m_SecondClickBlockY)
		{
			m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_movtype = 3;
			m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_movtype = 4;

			m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_destY = m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_Y;//m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_Y;
			m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_destY = m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_Y;//m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_Y;
		}
		else if(m_FirstClickBlockY < m_SecondClickBlockY )
		{
			m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_movtype = 4;
			m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_movtype = 3;

			m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_destY = m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_Y;//m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_Y;
			m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_destY = m_BlockTableXY[m_SecondClickBlockY][m_SecondClickBlockX].m_Y;//m_BlockTableXY[m_FirstClickBlockY][m_FirstClickBlockX].m_Y;
		}
	}
	else return;
}

void CZooKeeper::MovBlocks()
{
	SI32 i, j;

	for( i = 0; i < 8; ++i ) 
	{
		for( j = 0; j < 8; ++j )
		{
			switch( m_BlockTableXY[i][j].m_movtype ) 
			{
				case MOVLEFT:
					{
						m_BlockTableXY[i][j].m_X -= 
							( m_BlockWidth / m_BlockDownMoveValue );

						if( m_BlockTableXY[i][j].m_X <=	m_BlockTableXY[i][j].m_destX )
						{
							m_BlockTableXY[i][j].m_movtype = MOVSTOP;
							m_BlockTableXY[i][j].m_X = m_BlockTableXY[i][j].m_destX;
						}
					}
					break;

				case MOVRIGHT:
					{
						m_BlockTableXY[i][j].m_X += 
							( m_BlockWidth / m_BlockDownMoveValue );

						if( m_BlockTableXY[i][j].m_X >=	m_BlockTableXY[i][j].m_destX ) 
						{
							m_BlockTableXY[i][j].m_movtype = MOVSTOP;
							m_BlockTableXY[i][j].m_X = m_BlockTableXY[i][j].m_destX;
						}
					}
					break;

				case MOVUP:
					{
						m_BlockTableXY[i][j].m_Y -= ( m_BlockHeight / m_BlockDownMoveValue );

						if( m_BlockTableXY[i][j].m_Y <=	m_BlockTableXY[i][j].m_destY ) 
						{
							m_BlockTableXY[i][j].m_movtype = MOVSTOP;
							m_BlockTableXY[i][j].m_Y = m_BlockTableX[i][j].m_destY;
						}

					}
					break;

				case MOVDOWN:
					{
						m_BlockTableXY[i][j].m_Y += ( m_BlockHeight / m_BlockDownMoveValue );

						if( m_BlockTableXY[i][j].m_Y >=	m_BlockTableXY[i][j].m_destY ) 
						{
							m_BlockTableXY[i][j].m_movtype = MOVSTOP;
							m_BlockTableXY[i][j].m_Y = m_BlockTableX[i][j].m_destY;
						}

					}
					break;
			}
		}
	}
}

void CZooKeeper::MovBlocks2()
{
	SI32 i, j;

	for( i = 0; i < 8; ++i ) 
	{
		for( j = 0; j < 8; ++j )
		{
			switch( m_BlockTableXY[i][j].m_movtype ) 
			{
			case MOVLEFT:
				{
					m_BlockTableXY[i][j].m_X -= 
						( m_BlockWidth / m_BlockSwapMoveValue );

					if( m_BlockTableXY[i][j].m_X <=	m_BlockTableXY[i][j].m_destX )
					{
						m_BlockTableXY[i][j].m_movtype = MOVSTOP;
						m_BlockTableXY[i][j].m_X = m_BlockTableX[i][j].m_X;
					}
				}
				break;

			case MOVRIGHT:
				{
					m_BlockTableXY[i][j].m_X += 
						( m_BlockWidth / m_BlockSwapMoveValue );

					if( m_BlockTableXY[i][j].m_X >=	m_BlockTableXY[i][j].m_destX ) 
					{
						m_BlockTableXY[i][j].m_movtype = MOVSTOP;
						m_BlockTableXY[i][j].m_X = m_BlockTableX[i][j].m_X;
					}
				}
				break;

			case MOVUP:
				{
					m_BlockTableXY[i][j].m_Y -= ( m_BlockHeight / m_BlockSwapMoveValue );

					if( m_BlockTableXY[i][j].m_Y <=	m_BlockTableXY[i][j].m_destY -m_BlockHeight ) 
					{
						m_BlockTableXY[i][j].m_movtype = MOVSTOP;
						m_BlockTableXY[i][j].m_Y = m_BlockTableXY[i][j].m_destY;
					}

				}
				break;

			case MOVDOWN:
				{
					m_BlockTableXY[i][j].m_Y += ( m_BlockHeight / m_BlockSwapMoveValue );

					if( m_BlockTableXY[i][j].m_Y >=	m_BlockTableXY[i][j].m_destY +m_BlockHeight) 
					{
						m_BlockTableXY[i][j].m_movtype = MOVSTOP;
						m_BlockTableXY[i][j].m_Y = m_BlockTableXY[i][j].m_destY;
					}

				}
				break;
			}
		}
	}
}

BOOL CZooKeeper::IsMovStopAllBlock()
{
	int i, j;
	for( i = 0; i < 8; ++i )
	{
		for( j = 0; j < 8; ++j ) 
		{
			if( m_BlockTableXY[i][j].m_movtype != MOVSTOP ) return FALSE;
		}
	}

	return TRUE;	
}

void CZooKeeper::ClickMouseDataInit()
{
	m_FirstClickBlockX = 0;
    m_SecondClickBlockX = 0;

	m_FirstClickBlockY = 0;
	m_SecondClickBlockY = 0;

	m_FirstClickBlockValue = 0;
	m_SecondClickBlockValue = 0;
}

void CZooKeeper::PenaltyData()
{
	m_GameScore -= PENALTYSCORE;
	m_TrueGameScore -= (PENALTYSCORE / 10) << 2; 
	if(m_GameScore < 0)
		m_GameScore = 0;
	if(m_TrueGameScore < 0)
		m_TrueGameScore = 0;
	m_GameTime  -= PENALTYTIME; 

	cltGameMsgRequest_MonsterCorp_ComboInfo clinfo(-1, PENALTYSCORE);
	cltMsg clMsg(GAMEMSG_REQUEST_MONSTERCORP_COMBOINFO,sizeof(clinfo),(BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

BOOL CZooKeeper::CheckScore()
{
	m_TrueGameScore = (m_TrueGameScore >> 2)*SCORE;

	if(m_GameScore == m_TrueGameScore)
		return TRUE;
	else
		return FALSE;
}