#include "SeedDropMgr.h"

#include "..\..\Common\CommonHeader.h"
#include "..\Client\Client.h"

#include <Graphic.h>
#include "Graphic_Capsule.h"

CSeedDropMgr::CSeedDropMgr( SI32 GimgIndex, SI32 SeedTypeNum, SI32 siWidth, SI32 siHeight, SI32 siMaxSeedDropNum )
{
	srand( time( NULL ) );

	m_siSeedTypeNum = SeedTypeNum;
	
	for (SI32 i = 0; i < SeedTypeNum; i++)
	{
		m_pSeedDropXSPR[i] = pclClient->GetGlobalSpr(GIMG_SEEDDROP1 + i);
	}

	m_siWidth = siWidth;
	m_siHeight = siHeight;

	m_siSeedDropNum = 0;

	m_pSeedDropObjList = new stSeedDropObj[ siMaxSeedDropNum ];
	m_siMaxSeedDropNum = siMaxSeedDropNum;

	for ( int i = 0; i < m_siMaxSeedDropNum; ++i )
	{
		m_pSeedDropObjList[ i ].siImageIndex = -1;
		m_pSeedDropObjList[ i ].siIndex = -1;
		m_pSeedDropObjList[ i ].siXPos = -1000;
		m_pSeedDropObjList[ i ].siYPos = -1000;
		m_pSeedDropObjList[ i ].siSeedTypeIndex = -1;
	}

	m_bActive = false;

	SetMapArea( siWidth, siHeight );

	m_delayClock = pclClient->CurrentClock;

	/*
	while ( m_siSeedDropNum < m_siMaxSeedDropNum / 2 )
	{
	CreateSeedDrop( rand()% m_siWidth , rand() % m_siHeight );
	}
	*/
}

CSeedDropMgr::~CSeedDropMgr()
{
	if (m_pSeedDropObjList)
	{
		delete [] (m_pSeedDropObjList);
		m_pSeedDropObjList = NULL;
	}

}

void CSeedDropMgr::Initialize()
{
	return;
}

void CSeedDropMgr::Destroy()
{
	return;
}

void CSeedDropMgr::SetMapArea( SI32 siWidth, SI32 siHeight )
{
	m_siWidth = siWidth;
	m_siHeight = siHeight;

	return;
}

bool CSeedDropMgr::IsInArea( SI32 type, SI32 xPos, SI32 yPos )
{
	if (type >= 0 && type < m_siSeedTypeNum)
	{
		if ( xPos > m_siWidth || xPos + m_pSeedDropXSPR[type]->GetXSize() < 0 )
		{
			return false;
		}

		if ( yPos > m_siHeight || yPos + m_pSeedDropXSPR[type]->GetYSize() < 0 )
		{
			return false;
		}

		return true;
	}
	else
		return false;

}

bool CSeedDropMgr::IsInScreenArea( SI32 type, SI32 SeedDropxPos, SI32 SeedDropyPos, SI32 siScreenXPos, SI32 siScreenYPos, SI32 siScreenWidth, SI32 siScreenHeight )
{
	if ( type >= 0 && type < m_siSeedTypeNum )
	{
		if ( SeedDropxPos + m_pSeedDropXSPR[type]->GetXSize() > siScreenXPos && SeedDropxPos < siScreenWidth +siScreenXPos )
		{
			if ( SeedDropyPos + m_pSeedDropXSPR[type]->GetYSize() > siScreenYPos && SeedDropyPos < siScreenHeight + siScreenYPos )
			{
				return true;
			}
		}
	}

	return false;
}

bool CSeedDropMgr::CreateSeedDrop( SI32 xPos, SI32 yPos )
{
	if ( m_siSeedDropNum > m_siMaxSeedDropNum )
	{
		return false;
	}

	if ( IsRepetition( xPos, yPos ) )
	{
		return false;
	}

	SI32 siCount = 0;

	while ( m_pSeedDropObjList[ siCount ].siIndex != -1 )
	{
		if ( m_siMaxSeedDropNum == ++siCount )
		{
			return false;
		}
	}

	m_pSeedDropObjList[ siCount ].siXPos = xPos;
	m_pSeedDropObjList[ siCount ].siYPos = yPos;

	m_pSeedDropObjList[ siCount ].siIndex = siCount;
	m_pSeedDropObjList[ siCount ].siImageIndex = 0;
	m_pSeedDropObjList[ siCount ].siSeedTypeIndex = rand() % m_siSeedTypeNum;

	++m_siSeedDropNum;

	return true;
}

void CSeedDropMgr::DeleteSeedDrop( SI32 siIndex )
{
	m_pSeedDropObjList[ siIndex ].siImageIndex = -1;
	m_pSeedDropObjList[ siIndex ].siIndex = -1;
	m_pSeedDropObjList[ siIndex ].siXPos = -1000;
	m_pSeedDropObjList[ siIndex ].siYPos = -1000;
	m_pSeedDropObjList[ siIndex ].siSeedTypeIndex = -1;

	--m_siSeedDropNum;

	return;
}

void CSeedDropMgr::Action()
{
	if ( m_bActive == false )
	{
		return;
	}

	if (TABS(pclClient->CurrentClock - m_delayClock) > 50)
	{
		m_delayClock = pclClient->CurrentClock;


		// 무방울은 아래에서 위로 올라와야 하므로 이렇게 했음.
		CreateSeedDrop( rand() % m_siWidth, m_siHeight - CREATE_Y_INDENT_POS );

		if ( m_siSeedDropNum < 1 ) 
		{
			return;
		}

		int siXMove = 0;
		int siYMove = -1;

		/*
		switch ( siWindDirection )
		{
		case CLD_SOUTH_EAST:
		siXMove = 1;
		siYMove = 1;
		break;
		case CLD_SOUTH:
		siXMove = 0;
		siYMove = 1;
		break;
		case CLD_SOUTH_WEST:
		siXMove = -1;
		siYMove = 1;
		break;
		case CLD_WEST:
		siXMove = -1;
		siYMove = 0;
		break;
		case CLD_NORTH_WEST:
		siXMove = -1;
		siYMove = -1;
		break;
		case CLD_NORTH:
		siXMove = 0;
		siYMove = -1;
		break;
		case CLD_NORTH_EAST:
		siXMove = 1;
		siYMove = -1;
		break;
		case CLD_EAST:
		siXMove = 1;
		siYMove = 0;
		break;
		}
		*/

		for ( SI32 i = 0; i < m_siMaxSeedDropNum; ++i )
		{
			if ( m_pSeedDropObjList[ i ].siXPos == -1000 )//&& m_pSeedDropObjList[ i ].siYPos == -1000 )
			{
				continue;
			}


			if ( rand() % 2 == 0 )
			{
				siXMove = rand() % MOVE_X_POS;
			}
			else
			{
				siXMove = ( rand() % MOVE_X_POS ) * -1;
			}


			siYMove = -1 * MOVE_Y_POS;
			/*
			siYMove = -1 * ( rand() % MOVE_Y_POS );

			if ( siYMove == 0 )
			{
			siYMove = -1;
			}
			*/

			m_pSeedDropObjList[ i ].siXPos += siXMove;
			m_pSeedDropObjList[ i ].siYPos += siYMove;

			if (m_pSeedDropObjList[ i ].siSeedTypeIndex >= 0 && m_pSeedDropObjList[ i ].siSeedTypeIndex < m_siSeedTypeNum)
			{
				if (m_pSeedDropObjList[ i ].siImageIndex >= m_pSeedDropXSPR[m_pSeedDropObjList[ i ].siSeedTypeIndex]->GetImageNum())
					m_pSeedDropObjList[ i ].siImageIndex = 0;
				else
					m_pSeedDropObjList[ i ].siImageIndex++;

				if ( ! IsInArea( m_pSeedDropObjList[ i ].siSeedTypeIndex, m_pSeedDropObjList[ i ].siXPos, m_pSeedDropObjList[ i ].siYPos ) )
				{
					DeleteSeedDrop( i );
				}

			}
		}

	}

	return;
}

bool CSeedDropMgr::IsRepetition( SI32 siXPos, SI32 siYPos )
{
	for ( SI32 i = 0; i < m_siMaxSeedDropNum; ++i )
	{

		if ( m_pSeedDropObjList[ i ].siXPos == -1000  )//&& m_pSeedDropObjList[ i ].siYPos != -1000 )
		{
			continue;
		}

		if ( m_pSeedDropObjList[ i ].siSeedTypeIndex <= 0 || m_pSeedDropObjList[ i ].siSeedTypeIndex >= m_siSeedTypeNum )
		{
			continue;
		}

		if ( m_pSeedDropObjList[ i ].siXPos > siXPos + m_pSeedDropXSPR[m_pSeedDropObjList[ i ].siSeedTypeIndex]->GetXSize() || m_pSeedDropObjList[ i ].siXPos + m_pSeedDropXSPR[m_pSeedDropObjList[ i ].siSeedTypeIndex]->GetXSize() < siXPos )
		{
			continue;
		}
		else
		{
			if ( m_pSeedDropObjList[ i ].siYPos > siYPos + m_pSeedDropXSPR[m_pSeedDropObjList[ i ].siSeedTypeIndex]->GetYSize() || m_pSeedDropObjList[ i ].siYPos + m_pSeedDropXSPR[m_pSeedDropObjList[ i ].siSeedTypeIndex]->GetYSize() < siYPos )
			{
				continue;
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

void CSeedDropMgr::Draw( SI32 xPos, SI32 yPos, SI32 siScreenWidth, SI32 siScreenHeight )
{
	if ( m_siSeedDropNum < 1 || m_bActive == false ) 
	{
		return;
	}

	for ( SI32 i = 0; i < m_siMaxSeedDropNum; ++i )
	{
		if ( m_pSeedDropObjList[ i ].siXPos == -1000  )//&& m_pSeedDropObjList[ i ].siYPos != -1000 )
		{
			continue;
		}

		if ( IsInScreenArea( m_pSeedDropObjList[ i ].siSeedTypeIndex, m_pSeedDropObjList[ i ].siXPos, m_pSeedDropObjList[ i ].siYPos, xPos, yPos, siScreenWidth, siScreenHeight ) )
		{
			if (m_pSeedDropObjList[ i ].siSeedTypeIndex >= 0 && m_pSeedDropObjList[ i ].siSeedTypeIndex < m_siSeedTypeNum)
			{
				GP.PutSprScreen(m_pSeedDropXSPR[m_pSeedDropObjList[ i ].siSeedTypeIndex], m_pSeedDropObjList[ i ].siXPos - xPos, 
					m_pSeedDropObjList[ i ].siYPos - yPos,
					m_pSeedDropObjList[ i ].siImageIndex);

			}

		}
	}

	return;
}

void CSeedDropMgr::SetActive( bool bActive )
{
	m_bActive = bActive;

	if ( m_bActive )
	{
		while ( m_siSeedDropNum < m_siMaxSeedDropNum / 2 )
		{
			CreateSeedDrop( rand()% m_siWidth , rand() % m_siHeight );
		}
	}

	return;
}
