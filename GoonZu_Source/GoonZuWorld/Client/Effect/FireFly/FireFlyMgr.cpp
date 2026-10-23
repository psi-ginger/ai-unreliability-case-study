#include "FireFlyMgr.h"
#include <Graphic.h>
#include "Graphic_Capsule.h"
#include "CommonLogic.h"

CFireFlyMgr::CFireFlyMgr( TSpr *pXSPR, SI32 siWidth, SI32 siHeight, SI32 siMaxFireFlyNum )
{
	srand( time( NULL ) );

	m_bAreaCheck = false;

	m_pFireFlyXSPR = pXSPR;
	m_siWidth = siWidth;
	m_siHeight = siHeight;

	m_siFireFlyNum = 0;

	m_pFireFlyObjList = new stFireFlyObj[ siMaxFireFlyNum ];
	m_siMaxFireFlyNum = siMaxFireFlyNum;

	for ( int i = 0; i < m_siMaxFireFlyNum; ++i )
	{
		m_pFireFlyObjList[ i ].siImageIndex = -1;
		m_pFireFlyObjList[ i ].siIndex = -1;
		m_pFireFlyObjList[ i ].siXPos = -1000;
		m_pFireFlyObjList[ i ].siYPos = -1000;
		m_pFireFlyObjList[ i ].siDelayFlame = 0;
	}

	m_bActive = false;
	
	SetMapArea( siWidth, siHeight );

	m_delayClock = pclClient->CurrentClock;

	/*
	while ( m_siFireFlyNum < m_siMaxFireFlyNum / 2 )
	{
		CreateFireFly( rand()% m_siWidth , rand() % m_siHeight );
	}
	*/
}

CFireFlyMgr::~CFireFlyMgr()
{
	if ( m_pFireFlyObjList )
	{
		delete [] m_pFireFlyObjList;
		m_pFireFlyObjList = NULL;
	}
}

void CFireFlyMgr::Initialize()
{
	return;
}

void CFireFlyMgr::Destroy()
{
	return;
}
			
void CFireFlyMgr::SetMapArea( SI32 siWidth, SI32 siHeight )
{
	m_siWidth = siWidth;
	m_siHeight = siHeight;

	return;
}
	
bool CFireFlyMgr::IsInArea( SI32 xPos, SI32 yPos )
{
	if ( xPos > m_siWidth || xPos + m_pFireFlyXSPR->GetXSize() < 0 )
	{
		return false;
	}

	if ( yPos > m_siHeight || yPos + m_pFireFlyXSPR->GetYSize() < 0 )
	{
		return false;
	}

	return true;
}

bool CFireFlyMgr::IsInScreenArea( SI32 FireFlyxPos, SI32 FireFlyyPos, SI32 siScreenXPos, SI32 siScreenYPos, SI32 siScreenWidth, SI32 siScreenHeight )
{
	if ( FireFlyxPos + m_pFireFlyXSPR->GetXSize() > siScreenXPos && FireFlyxPos < siScreenWidth +siScreenXPos )
	{
		if ( FireFlyyPos + m_pFireFlyXSPR->GetYSize() > siScreenYPos && FireFlyyPos < siScreenHeight + siScreenYPos )
		{
			return true;
		}
	}

	return false;
}

bool CFireFlyMgr::CreateFireFly( SI32 xPos, SI32 yPos )
{
	if ( m_siFireFlyNum > m_siMaxFireFlyNum )
	{
		return false;
	}

	if ( IsRepetition( xPos, yPos ) )
	{
		return false;
	}
	
	SI32 siCount = 0;

	while ( m_pFireFlyObjList[ siCount ].siIndex != -1 )
	{
		if ( m_siMaxFireFlyNum == ++siCount )
		{
			return false;
		}
	}

	m_pFireFlyObjList[ siCount ].siXPos = xPos;
	m_pFireFlyObjList[ siCount ].siYPos = yPos;

	m_pFireFlyObjList[ siCount ].siIndex = siCount;
	m_pFireFlyObjList[ siCount ].siImageIndex = -1;
	m_pFireFlyObjList[ siCount ].siDelayFlame = 0;
	
	++m_siFireFlyNum;

	return true;
}

void CFireFlyMgr::DeleteFireFly( SI32 siIndex )
{
	m_pFireFlyObjList[ siIndex ].siImageIndex = -1;
	m_pFireFlyObjList[ siIndex ].siIndex = -1;
	m_pFireFlyObjList[ siIndex ].siXPos = -1000;
	m_pFireFlyObjList[ siIndex ].siYPos = -1000;
	m_pFireFlyObjList[ siIndex ].siDelayFlame = 0;
	
	--m_siFireFlyNum;

	return;
}

void CFireFlyMgr::Action()
{
	if ( m_bActive == false )
	{
		return;
	}

	if (TABS(pclClient->CurrentClock - m_delayClock) > 50)
	{
		m_delayClock = pclClient->CurrentClock;

	
		if ( m_bAreaCheck )
		{
			CreateFireFly( m_siXLeft + ( rand() % ( m_siXRight - m_siXLeft ) ), m_siYTop + ( rand() % ( m_siYBottom - m_siYTop ) ) );
		}
		else
		{
			CreateFireFly( rand() % m_siWidth, rand() % m_siHeight );
		}
		
		if ( m_siFireFlyNum < 1 ) 
		{
			return;
		}
		
		int siXMove = 0;
		int siYMove = -1;

		for ( SI32 i = 0; i < m_siMaxFireFlyNum; ++i )
		{
			if ( m_pFireFlyObjList[ i ].siXPos == -1000 )//&& m_pFireFlyObjList[ i ].siYPos == -1000 )
			{
				continue;
			}

			if ( rand() % 2 == 0 )
			{
				siXMove = rand() % FIREFLY_MOVE_X_POS;
			}
			else
			{
				siXMove = ( rand() % FIREFLY_MOVE_X_POS ) * -1;
			}


			if ( rand() % 2 == 0 )
			{
				siYMove = rand() % FIREFLY_MOVE_Y_POS;
			}
			else
			{
				siYMove = ( rand() % FIREFLY_MOVE_Y_POS ) * -1;
			}

			m_pFireFlyObjList[ i ].siXPos += siXMove;
			m_pFireFlyObjList[ i ].siYPos += siYMove;

			if ( ! IsInArea( m_pFireFlyObjList[ i ].siXPos, m_pFireFlyObjList[ i ].siYPos ) )
			{
				DeleteFireFly( i );
			}

			if ( ++m_pFireFlyObjList[ i ].siDelayFlame == FIREFLY_DELAY_FLAME )
			{
				m_pFireFlyObjList[ i ].siDelayFlame = 0;

				if ( ++m_pFireFlyObjList[ i ].siImageIndex == m_pFireFlyXSPR->GetImageNum() )
				{
					DeleteFireFly( i );
				}
			}
		}
	}

	return;
}

bool CFireFlyMgr::IsRepetition( SI32 siXPos, SI32 siYPos )
{
	for ( SI32 i = 0; i < m_siMaxFireFlyNum; ++i )
	{

		if ( m_pFireFlyObjList[ i ].siXPos == -1000  )//&& m_pFireFlyObjList[ i ].siYPos != -1000 )
		{
			continue;
		}

		if ( m_pFireFlyObjList[ i ].siXPos > siXPos + m_pFireFlyXSPR->GetXSize() || m_pFireFlyObjList[ i ].siXPos + m_pFireFlyXSPR->GetXSize() < siXPos )
		{
			continue;
		}
		else
		{
			if ( m_pFireFlyObjList[ i ].siYPos > siYPos + m_pFireFlyXSPR->GetYSize() || m_pFireFlyObjList[ i ].siYPos + m_pFireFlyXSPR->GetYSize() < siYPos )
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

void CFireFlyMgr::Draw( SI32 xPos, SI32 yPos, SI32 siScreenWidth, SI32 siScreenHeight )
{
	if ( m_siFireFlyNum < 1 || m_bActive == false ) 
	{
		return;
	}

	for ( SI32 i = 0; i < m_siMaxFireFlyNum; ++i )
	{
		if ( m_pFireFlyObjList[ i ].siXPos == -1000  )//&& m_pFireFlyObjList[ i ].siYPos != -1000 )
		{
			continue;
		}

		if ( IsInScreenArea( m_pFireFlyObjList[ i ].siXPos, m_pFireFlyObjList[ i ].siYPos, xPos, yPos, siScreenWidth, siScreenHeight ) )
		{
			GP.PutSprLight1(m_pFireFlyXSPR, m_pFireFlyObjList[ i ].siXPos - xPos, 
										m_pFireFlyObjList[ i ].siYPos - yPos,
										m_pFireFlyObjList[ i ].siImageIndex);
			
			//m_pFireFlyXSPR->PutSprShadowT(m_pFireFlyObjList[ i ].siXPos - xPos, 
			//							m_pFireFlyObjList[ i ].siYPos - yPos,
			//							m_pFireFlyObjList[ i ].siImageIndex);
		}
	}

	return;
}

void CFireFlyMgr::SetActive( bool bActive )
{
	m_bActive = bActive;

	m_bAreaCheck = false;

	if ( m_bActive )
	{
		while ( m_siFireFlyNum < m_siMaxFireFlyNum / 2 )
		{
			CreateFireFly( rand()% m_siWidth , rand() % m_siHeight );
		}
	}
	else
	{
		for ( int i = 0; i < m_siFireFlyNum; ++i )
		{
			DeleteFireFly( i );
		}
	}

	return;
}

void CFireFlyMgr::SetActive( bool bActive, int XLeft, int YTop, int XRight, int YBottom, int siMaxFireFlyNum )
{
	m_bActive = bActive;

	m_bAreaCheck = true;

	m_siXLeft = XLeft;
	m_siXRight = XRight;

	m_siYTop = YTop;
	m_siYBottom = YBottom;

	m_siMaxFireFlyNum = siMaxFireFlyNum;

	if ( m_bActive )
	{
		while ( m_siFireFlyNum < m_siMaxFireFlyNum / 2 )
		{
			CreateFireFly( XLeft + ( rand() % ( XRight - XLeft ) ), YTop + ( rand() % ( YBottom - YTop ) ) );
		}
	}
}
