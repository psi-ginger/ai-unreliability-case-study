#include "CloudMgr.h"
#include <Time.h>
#include <Graphic.h>
#include "Graphic_Capsule.h"

CCloudMgr::CCloudMgr( TSpr *pXSPR0, TSpr *pXSPR1, TSpr *pXSPR2, SI32 siWidth, SI32 siHeight, SI32 siMaxCloudNum )
//CCloudMgr::CCloudMgr( cltTSpr *pXSPR0, cltTSpr *pXSPR1, cltTSpr *pXSPR2, cltTSpr *pXSPR3, SI32 siWidth, SI32 siHeight, SI32 siMaxCloudNum )
{
	srand( time( NULL ) );

	m_pCloudXSPR[ 0 ] = pXSPR0;
	m_pCloudXSPR[ 1 ] = pXSPR1;
	m_pCloudXSPR[ 2 ] = pXSPR2;
	//m_pCloudXSPR[ 3 ] = pXSPR3;
	
	/*
	for ( int i = 0; i < CLOUD_IMAGE_NUM; ++i )
	{
		
	}
	*/

	
	m_siWidth = siWidth;
	m_siHeight = siHeight;

	m_siCloudNum = 0;

	m_pCloudObjList = new stCloudObj[ siMaxCloudNum ];
	m_siMaxCloudNum = siMaxCloudNum;

	for ( int i = 0; i < m_siMaxCloudNum; ++i )
	{
		m_pCloudObjList[ i ].siImageIndex = -1;
		m_pCloudObjList[ i ].siIndex = -1;
		m_pCloudObjList[ i ].siXPos = -1000;
		m_pCloudObjList[ i ].siYPos = -1000;
	}

	m_bActive = false;
	
	SetMapArea( siWidth, siHeight );
	
}

CCloudMgr::~CCloudMgr()
{
	if ( m_pCloudObjList )
	{
		delete [] m_pCloudObjList;
		m_pCloudObjList = NULL;
	}
}

void CCloudMgr::Initialize()
{
	return;
}

void CCloudMgr::Destroy()
{
	return;
}

void CCloudMgr::SetMapArea( SI32 siWidth, SI32 siHeight )
{
	m_siWidth = siWidth;
	m_siHeight = siHeight;

	return;
}

bool CCloudMgr::IsInArea( SI32 xPos, SI32 yPos, SI32 CloudObjListIndex )
{
	if ( xPos > m_siWidth || xPos + m_pCloudXSPR[ m_pCloudObjList[ CloudObjListIndex ].siImageIndex ]->GetXSize() < 0 )
	{
		return false;
	}

	if ( yPos > m_siHeight || yPos + m_pCloudXSPR[ m_pCloudObjList[ CloudObjListIndex ].siImageIndex ]->GetYSize() < 0 )
	{
		return false;
	}

	return true;
}

bool CCloudMgr::IsRepetition( SI32 siXPos, SI32 siYPos )
{
	for ( SI32 i = 0; i < m_siMaxCloudNum; ++i )
	{
		if ( m_pCloudObjList[ i ].siXPos == -1000  )//&& m_pCloudObjList[ i ].siYPos != -1000 )
		{
			continue;
		}

		if ( m_pCloudObjList[ i ].siXPos > siXPos + m_pCloudXSPR[ m_pCloudObjList[ i ].siImageIndex ]->GetXSize() || m_pCloudObjList[ i ].siXPos + m_pCloudXSPR[ m_pCloudObjList[ i ].siImageIndex ]->GetXSize() < siXPos )
		{
			continue;
		}
		else
		{
			if ( m_pCloudObjList[ i ].siYPos > siYPos + m_pCloudXSPR[ m_pCloudObjList[ i ].siImageIndex ]->GetYSize() || m_pCloudObjList[ i ].siYPos + m_pCloudXSPR[ m_pCloudObjList[ i ].siImageIndex ]->GetYSize() < siYPos )
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

bool CCloudMgr::CreateCloud(  SI32 xPos, SI32 yPos )
{
	if ( m_siCloudNum > m_siMaxCloudNum )
	{
		return false;
	}

	if ( IsRepetition( xPos, yPos ) )
	{
		return false;
	}
	
	SI32 siCount = 0;

	while ( m_pCloudObjList[ siCount ].siIndex != -1 )
	{
		if ( m_siMaxCloudNum == ++siCount )
		{
			return false;
		}
	}

	m_pCloudObjList[ siCount ].siXPos = xPos;
	m_pCloudObjList[ siCount ].siYPos = yPos;

	m_pCloudObjList[ siCount ].siIndex = siCount;
	m_pCloudObjList[ siCount ].siImageIndex = rand() % CLOUD_IMAGE_NUM;
	
	++m_siCloudNum;

	return true;
}

void CCloudMgr::DeleteCloud( SI32 siIndex )
{

	m_pCloudObjList[ siIndex ].siImageIndex = -1;
	m_pCloudObjList[ siIndex ].siIndex = -1;
	m_pCloudObjList[ siIndex ].siXPos = -1000;
	m_pCloudObjList[ siIndex ].siYPos = -1000;
	
	--m_siCloudNum;

	return;
}

void CCloudMgr::Action( SI08 siWindDirection )
{
	if ( m_bActive == false )
	{
		return;
	}

	CreateCloud( rand() % m_siWidth, rand() % m_siHeight );

	if ( m_siCloudNum < 1 ) 
	{
		return;
	}

	int siXMove = 0;
	int siYMove = 0;

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

	for ( SI32 i = 0; i < m_siMaxCloudNum; ++i )
	{
		if ( m_pCloudObjList[ i ].siXPos == -1000 )//&& m_pCloudObjList[ i ].siYPos == -1000 )
		{
			continue;
		}

		m_pCloudObjList[ i ].siXPos += siXMove;
		m_pCloudObjList[ i ].siYPos += siYMove;

		if ( ! IsInArea( m_pCloudObjList[ i ].siXPos, m_pCloudObjList[ i ].siYPos, i ) )
		{
			DeleteCloud( i );
		}
	}

	return;
}

void CCloudMgr::Action( stWeatherData *pWeatherData )
{
	Action( pWeatherData->m_si08WindDirection );

	return;
}

bool CCloudMgr::IsInScreenArea( SI32 CloudxPos, SI32 CloudyPos, SI32 siScreenXPos, SI32 siScreenYPos, SI32 siScreenWidth, SI32 siScreenHeight, SI32 CloudObjListIndex )
{
	if ( CloudxPos + m_pCloudXSPR[ m_pCloudObjList[ CloudObjListIndex ].siImageIndex ]->GetXSize() > siScreenXPos && CloudxPos < siScreenWidth +siScreenXPos )
	{
		if ( CloudyPos + m_pCloudXSPR[ m_pCloudObjList[ CloudObjListIndex ].siImageIndex ]->GetYSize() > siScreenYPos && CloudyPos < siScreenHeight + siScreenYPos )
		{
			return true;
		}
	}

	return false;
}

void CCloudMgr::Draw( SI32 xPos, SI32 yPos, SI32 siScreenWidth, SI32 siScreenHeight )
{
	if ( m_siCloudNum < 1 || m_bActive == false ) 
	{
		return;
	}

	for ( SI32 i = 0; i < m_siMaxCloudNum; ++i )
	{
		if ( m_pCloudObjList[ i ].siXPos == -1000  )//&& m_pCloudObjList[ i ].siYPos != -1000 )
		{
			continue;
		}

		if ( IsInScreenArea( m_pCloudObjList[ i ].siXPos, m_pCloudObjList[ i ].siYPos, xPos, yPos, siScreenWidth, siScreenHeight, i ) )
		{
			GP.PutSprShadow(m_pCloudXSPR[ m_pCloudObjList[ i ].siImageIndex ], m_pCloudObjList[ i ].siXPos - xPos, 
										m_pCloudObjList[ i ].siYPos - yPos,
										0 );
		}
	}

	return;
}



//------------------------------------------------------------------
//----------- 던전에 구름이 없어야 하므로...
void CCloudMgr::SetActive( bool bActive, SI32 siWidth, SI32 siHeight, SI32 siMaxCloudNum )
{
	m_bActive = bActive;

	m_siWidth = siWidth;
	m_siHeight = siHeight;
	m_siMaxCloudNum = siMaxCloudNum;

	if ( m_bActive )
	{
		while ( m_siCloudNum < m_siMaxCloudNum / 2 )
		{
			CreateCloud( rand()% m_siWidth , rand() % m_siHeight );
		}	
	}

	return;
}