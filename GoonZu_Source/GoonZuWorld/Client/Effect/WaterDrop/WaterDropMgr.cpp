#include "WaterDropMgr.h"
#include <Graphic.h>
#include "Graphic_Capsule.h"
#include "CommonLogic.h"

CWaterDropMgr::CWaterDropMgr( TSpr *pXSPR, SI32 siWidth, SI32 siHeight, SI32 siMaxWaterDropNum )
{
	srand( time( NULL ) );
	m_pWaterDropXSPR = pXSPR;
	m_siWidth = siWidth;
	m_siHeight = siHeight;

	m_siWaterDropNum = 0;

	m_pWaterDropObjList = new stWaterDropObj[ siMaxWaterDropNum ];
	m_siMaxWaterDropNum = siMaxWaterDropNum;

	for ( int i = 0; i < m_siMaxWaterDropNum; ++i )
	{
		m_pWaterDropObjList[ i ].siImageIndex = rand() % pXSPR->GetImageNum();
		m_pWaterDropObjList[ i ].siIndex = -1;
		m_pWaterDropObjList[ i ].siXPos = -1000;
		m_pWaterDropObjList[ i ].siYPos = -1000;
	}

	m_bActive = false;
	
	SetMapArea( siWidth, siHeight );

	/*
	while ( m_siWaterDropNum < m_siMaxWaterDropNum / 2 )
	{
		CreateWaterDrop( rand()% m_siWidth , rand() % m_siHeight );
	}
	*/

	m_delayClock = pclClient->CurrentClock;
}

CWaterDropMgr::~CWaterDropMgr()
{
	if ( m_pWaterDropObjList )
	{
		delete [] m_pWaterDropObjList;
		m_pWaterDropObjList = NULL;
	}
}

void CWaterDropMgr::Initialize()
{
	return;
}

void CWaterDropMgr::Destroy()
{
	return;
}
			
void CWaterDropMgr::SetMapArea( SI32 siWidth, SI32 siHeight )
{
	m_siWidth = siWidth;
	m_siHeight = siHeight;

	return;
}
	
bool CWaterDropMgr::IsInArea( SI32 xPos, SI32 yPos )
{
	if ( xPos > m_siWidth || xPos + m_pWaterDropXSPR->GetXSize() < 0 )
	{
		return false;
	}

	if ( yPos > m_siHeight || yPos + m_pWaterDropXSPR->GetYSize() < 0 )
	{
		return false;
	}

	return true;
}

bool CWaterDropMgr::IsInScreenArea( SI32 WaterDropxPos, SI32 WaterDropyPos, SI32 siScreenXPos, SI32 siScreenYPos, SI32 siScreenWidth, SI32 siScreenHeight )
{
	if ( WaterDropxPos + m_pWaterDropXSPR->GetXSize() > siScreenXPos && WaterDropxPos < siScreenWidth +siScreenXPos )
	{
		if ( WaterDropyPos + m_pWaterDropXSPR->GetYSize() > siScreenYPos && WaterDropyPos < siScreenHeight + siScreenYPos )
		{
			return true;
		}
	}

	return false;
}

bool CWaterDropMgr::CreateWaterDrop( SI32 xPos, SI32 yPos )
{
	if ( m_siWaterDropNum > m_siMaxWaterDropNum )
	{
		return false;
	}

	if ( IsRepetition( xPos, yPos ) )
	{
		return false;
	}
	
	SI32 siCount = 0;

	while ( m_pWaterDropObjList[ siCount ].siIndex != -1 )
	{
		if ( m_siMaxWaterDropNum == ++siCount )
		{
			return false;
		}
	}

	m_pWaterDropObjList[ siCount ].siXPos = xPos;
	m_pWaterDropObjList[ siCount ].siYPos = yPos;

	m_pWaterDropObjList[ siCount ].siIndex = siCount;
	m_pWaterDropObjList[ siCount ].siImageIndex = rand() % 3;
	
	++m_siWaterDropNum;

	return true;
}

void CWaterDropMgr::DeleteWaterDrop( SI32 siIndex )
{
	m_pWaterDropObjList[ siIndex ].siImageIndex = -1;
	m_pWaterDropObjList[ siIndex ].siIndex = -1;
	m_pWaterDropObjList[ siIndex ].siXPos = -1000;
	m_pWaterDropObjList[ siIndex ].siYPos = -1000;
	
	--m_siWaterDropNum;

	return;
}

void CWaterDropMgr::Action()
{
	if ( m_bActive == false )
	{
		return;
	}

	if (TABS(pclClient->CurrentClock - m_delayClock) > 50)
	{
		m_delayClock = pclClient->CurrentClock;


		// 무방울은 아래에서 위로 올라와야 하므로 이렇게 했음.
		CreateWaterDrop( rand() % m_siWidth, m_siHeight - CREATE_Y_INDENT_POS );
		
		if ( m_siWaterDropNum < 1 ) 
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

		for ( SI32 i = 0; i < m_siMaxWaterDropNum; ++i )
		{
			if ( m_pWaterDropObjList[ i ].siXPos == -1000 )//&& m_pWaterDropObjList[ i ].siYPos == -1000 )
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

			m_pWaterDropObjList[ i ].siXPos += siXMove;
			m_pWaterDropObjList[ i ].siYPos += siYMove;

			if ( ! IsInArea( m_pWaterDropObjList[ i ].siXPos, m_pWaterDropObjList[ i ].siYPos ) )
			{
				DeleteWaterDrop( i );
			}
		}
	}

	return;
}

bool CWaterDropMgr::IsRepetition( SI32 siXPos, SI32 siYPos )
{
	for ( SI32 i = 0; i < m_siMaxWaterDropNum; ++i )
	{

		if ( m_pWaterDropObjList[ i ].siXPos == -1000  )//&& m_pWaterDropObjList[ i ].siYPos != -1000 )
		{
			continue;
		}

		if ( m_pWaterDropObjList[ i ].siXPos > siXPos + m_pWaterDropXSPR->GetXSize() || m_pWaterDropObjList[ i ].siXPos + m_pWaterDropXSPR->GetXSize() < siXPos )
		{
			continue;
		}
		else
		{
			if ( m_pWaterDropObjList[ i ].siYPos > siYPos + m_pWaterDropXSPR->GetYSize() || m_pWaterDropObjList[ i ].siYPos + m_pWaterDropXSPR->GetYSize() < siYPos )
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

void CWaterDropMgr::Draw( SI32 xPos, SI32 yPos, SI32 siScreenWidth, SI32 siScreenHeight )
{
	if ( m_siWaterDropNum < 1 || m_bActive == false ) 
	{
		return;
	}

	for ( SI32 i = 0; i < m_siMaxWaterDropNum; ++i )
	{
		if ( m_pWaterDropObjList[ i ].siXPos == -1000  )//&& m_pWaterDropObjList[ i ].siYPos != -1000 )
		{
			continue;
		}

		if ( IsInScreenArea( m_pWaterDropObjList[ i ].siXPos, m_pWaterDropObjList[ i ].siYPos, xPos, yPos, siScreenWidth, siScreenHeight ) )
		{
			GP.PutSprLight1(m_pWaterDropXSPR, m_pWaterDropObjList[ i ].siXPos - xPos, 
										m_pWaterDropObjList[ i ].siYPos - yPos,
										m_pWaterDropObjList[ i ].siImageIndex);
			
			//m_pWaterDropXSPR->PutSprShadowT(m_pWaterDropObjList[ i ].siXPos - xPos, 
			//							m_pWaterDropObjList[ i ].siYPos - yPos,
			//							m_pWaterDropObjList[ i ].siImageIndex);
		}
	}

	return;
}

void CWaterDropMgr::SetActive( bool bActive )
{
	m_bActive = bActive;

	if ( m_bActive )
	{
		while ( m_siWaterDropNum < m_siMaxWaterDropNum / 2 )
		{
			CreateWaterDrop( rand()% m_siWidth , rand() % m_siHeight );
		}
	}

	return;
}
