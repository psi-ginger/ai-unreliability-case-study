#include "IceFlashMgr.h"
#include <Graphic.h>
#include "Graphic_Capsule.h"
#include "CommonLogic.h"

CIceFlashMgr::CIceFlashMgr( TSpr *pXSPR, SI32 siWidth, SI32 siHeight, SI32 siMaxIceFlashNum )
{
	srand( time( NULL ) );

	m_bAreaCheck = false;

	m_pIceFlashXSPR = pXSPR;
	m_siWidth = siWidth;
	m_siHeight = siHeight;

	m_siIceFlashNum = 0;

	m_pIceFlashObjList = new stIceFlashObj[ siMaxIceFlashNum ];
	m_siMaxIceFlashNum = siMaxIceFlashNum;

	for ( int i = 0; i < m_siMaxIceFlashNum; ++i )
	{
		m_pIceFlashObjList[ i ].siImageIndex = -1;
		m_pIceFlashObjList[ i ].siIndex = -1;
		m_pIceFlashObjList[ i ].siXPos = -1000;
		m_pIceFlashObjList[ i ].siYPos = -1000;
		m_pIceFlashObjList[ i ].siDelayFlame = 0;
	}

	m_bActive = false;
	
	SetMapArea( siWidth, siHeight );

	m_delayClock = pclClient->CurrentClock;

	/*
	while ( m_siIceFlashNum < m_siMaxIceFlashNum / 2 )
	{
		CreateIceFlash( rand()% m_siWidth , rand() % m_siHeight );
	}
	*/
}

CIceFlashMgr::~CIceFlashMgr()
{
	if ( m_pIceFlashObjList )
	{
		delete [] m_pIceFlashObjList;
		m_pIceFlashObjList = NULL;
	}
}

void CIceFlashMgr::Initialize()
{
	return;
}

void CIceFlashMgr::Destroy()
{
	return;
}
			
void CIceFlashMgr::SetMapArea( SI32 siWidth, SI32 siHeight )
{
	m_siWidth = siWidth;
	m_siHeight = siHeight;

	return;
}
	
bool CIceFlashMgr::IsInArea( SI32 xPos, SI32 yPos )
{
	if ( xPos > m_siWidth || xPos + m_pIceFlashXSPR->GetXSize() < 0 )
	{
		return false;
	}

	if ( yPos > m_siHeight || yPos + m_pIceFlashXSPR->GetYSize() < 0 )
	{
		return false;
	}

	return true;
}

bool CIceFlashMgr::IsInScreenArea( SI32 IceFlashxPos, SI32 IceFlashyPos, SI32 siScreenXPos, SI32 siScreenYPos, SI32 siScreenWidth, SI32 siScreenHeight )
{
	if ( IceFlashxPos + m_pIceFlashXSPR->GetXSize() > siScreenXPos && IceFlashxPos < siScreenWidth +siScreenXPos )
	{
		if ( IceFlashyPos + m_pIceFlashXSPR->GetYSize() > siScreenYPos && IceFlashyPos < siScreenHeight + siScreenYPos )
		{
			return true;
		}
	}

	return false;
}

bool CIceFlashMgr::CreateIceFlash( SI32 xPos, SI32 yPos )
{
	if ( m_siIceFlashNum > m_siMaxIceFlashNum )
	{
		return false;
	}

	if ( IsRepetition( xPos, yPos ) )
	{
		return false;
	}
	
	SI32 siCount = 0;

	while ( m_pIceFlashObjList[ siCount ].siIndex != -1 )
	{
		if ( m_siMaxIceFlashNum == ++siCount )
		{
			return false;
		}
	}

	m_pIceFlashObjList[ siCount ].siXPos = xPos;
	m_pIceFlashObjList[ siCount ].siYPos = yPos;

	m_pIceFlashObjList[ siCount ].siIndex = siCount;
	m_pIceFlashObjList[ siCount ].siImageIndex = -1;
	m_pIceFlashObjList[ siCount ].siDelayFlame = 0;
	
	++m_siIceFlashNum;

	return true;
}

void CIceFlashMgr::DeleteIceFlash( SI32 siIndex )
{
	m_pIceFlashObjList[ siIndex ].siImageIndex = -1;
	m_pIceFlashObjList[ siIndex ].siIndex = -1;
	m_pIceFlashObjList[ siIndex ].siXPos = -1000;
	m_pIceFlashObjList[ siIndex ].siYPos = -1000;
	m_pIceFlashObjList[ siIndex ].siDelayFlame = 0;
	
	--m_siIceFlashNum;

	return;
}

void CIceFlashMgr::Action()
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
			CreateIceFlash( m_siXLeft + ( rand() % ( m_siXRight - m_siXLeft ) ), m_siYTop + ( rand() % ( m_siYBottom - m_siYTop ) ) );
		}
		else
		{
			CreateIceFlash( rand() % m_siWidth, rand() % m_siHeight );
		}
		
		if ( m_siIceFlashNum < 1 ) 
		{
			return;
		}
		
		//int siXMove = 0;
		//int siYMove = -1;

		for ( SI32 i = 0; i < m_siMaxIceFlashNum; ++i )
		{
			if ( m_pIceFlashObjList[ i ].siXPos == -1000 )//&& m_pIceFlashObjList[ i ].siYPos == -1000 )
			{
				continue;
			}

	/*		if ( rand() % 2 == 0 )
			{
				siXMove = rand() % ICEFLASH_MOVE_X_POS;
			}
			else
			{
				siXMove = ( rand() % ICEFLASH_MOVE_X_POS ) * -1;
			}


			if ( rand() % 2 == 0 )
			{
				siYMove = rand() % ICEFLASH_MOVE_Y_POS;
			}
			else
			{
				siYMove = ( rand() % ICEFLASH_MOVE_Y_POS ) * -1;
			}

			m_pIceFlashObjList[ i ].siXPos += siXMove;
			m_pIceFlashObjList[ i ].siYPos += siYMove;*/

			if ( ! IsInArea( m_pIceFlashObjList[ i ].siXPos, m_pIceFlashObjList[ i ].siYPos ) )
			{
				DeleteIceFlash( i );
			}

			if ( ++m_pIceFlashObjList[ i ].siDelayFlame == ICEFLASH_DELAY_FLAME )
			{
				m_pIceFlashObjList[ i ].siDelayFlame = 0;

				if ( ++m_pIceFlashObjList[ i ].siImageIndex == m_pIceFlashXSPR->GetImageNum() )
				{
					DeleteIceFlash( i );
				}
			}
		}
	}

	return;
}

bool CIceFlashMgr::IsRepetition( SI32 siXPos, SI32 siYPos )
{
	for ( SI32 i = 0; i < m_siMaxIceFlashNum; ++i )
	{

		if ( m_pIceFlashObjList[ i ].siXPos == -1000  )//&& m_pIceFlashObjList[ i ].siYPos != -1000 )
		{
			continue;
		}

		if ( m_pIceFlashObjList[ i ].siXPos > siXPos + m_pIceFlashXSPR->GetXSize() || m_pIceFlashObjList[ i ].siXPos + m_pIceFlashXSPR->GetXSize() < siXPos )
		{
			continue;
		}
		else
		{
			if ( m_pIceFlashObjList[ i ].siYPos > siYPos + m_pIceFlashXSPR->GetYSize() || m_pIceFlashObjList[ i ].siYPos + m_pIceFlashXSPR->GetYSize() < siYPos )
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

void CIceFlashMgr::Draw( SI32 xPos, SI32 yPos, SI32 siScreenWidth, SI32 siScreenHeight )
{
	if ( m_siIceFlashNum < 1 || m_bActive == false ) 
	{
		return;
	}

	for ( SI32 i = 0; i < m_siMaxIceFlashNum; ++i )
	{
		if ( m_pIceFlashObjList[ i ].siXPos == -1000  )//&& m_pIceFlashObjList[ i ].siYPos != -1000 )
		{
			continue;
		}

		if ( IsInScreenArea( m_pIceFlashObjList[ i ].siXPos, m_pIceFlashObjList[ i ].siYPos, xPos, yPos, siScreenWidth, siScreenHeight ) )
		{
			GP.PutSprScreen(m_pIceFlashXSPR, m_pIceFlashObjList[ i ].siXPos - xPos, 
										m_pIceFlashObjList[ i ].siYPos - yPos,
										m_pIceFlashObjList[ i ].siImageIndex);
			
			//m_pIceFlashXSPR->PutSprShadowT(m_pIceFlashObjList[ i ].siXPos - xPos, 
			//							m_pIceFlashObjList[ i ].siYPos - yPos,
			//							m_pIceFlashObjList[ i ].siImageIndex);
		}
	}

	return;
}

void CIceFlashMgr::SetActive( bool bActive )
{
	m_bActive = bActive;

	m_bAreaCheck = false;

	if ( m_bActive )
	{
		while ( m_siIceFlashNum < m_siMaxIceFlashNum / 2 )
		{
			CreateIceFlash( rand()% m_siWidth , rand() % m_siHeight );
		}
	}
	else
	{
		for ( int i = 0; i < m_siIceFlashNum; ++i )
		{
			DeleteIceFlash( i );
		}
	}

	return;
}

void CIceFlashMgr::SetActive( bool bActive, int XLeft, int YTop, int XRight, int YBottom, int siMaxIceFlashNum )
{
	m_bActive = bActive;

	m_bAreaCheck = true;

	m_siXLeft = XLeft;
	m_siXRight = XRight;

	m_siYTop = YTop;
	m_siYBottom = YBottom;

	m_siMaxIceFlashNum = siMaxIceFlashNum;

	if ( m_bActive )
	{
		while ( m_siIceFlashNum < m_siMaxIceFlashNum / 2 )
		{
			CreateIceFlash( XLeft + ( rand() % ( XRight - XLeft ) ), YTop + ( rand() % ( YBottom - YTop ) ) );
		}
	}
}
