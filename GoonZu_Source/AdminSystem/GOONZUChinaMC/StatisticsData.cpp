#include "StatisticsData.h"

CStatisticsData::CStatisticsData()
{
	InitializeCriticalSection( &m_cs );

	ClearAll();
}

CStatisticsData::~CStatisticsData()
{
	DeleteCriticalSection( &m_cs );
}

void CStatisticsData::ClearAll()
{
	for( int i = 0; i < MAX_LAYER_NUMBER; ++i ) 
	{
		Clear( i );
	}
}

void CStatisticsData::Clear( int layer )
{
	if( layer < 0 || layer >= MAX_LAYER_NUMBER ) return;

	m_bVisible[ layer ] = FALSE;
	ZeroMemory( m_siUserNum[ layer ], sizeof( int ) * MAX_DATA_NUMBER );
	
	m_szDate[ layer ][ 0 ] = NULL;

	m_siMaxUserNum[ layer ] = 0;
	
}

void CStatisticsData::SetMaxUser( int layer )
{
	int maxUser = 0;
	for( int i = 0; i < MAX_DATA_NUMBER; ++i )
	{
		if( m_siUserNum[ layer ][ i ] > maxUser ) maxUser = m_siUserNum[ layer ][ i ];
	}

	m_siMaxUserNum[ layer ] = maxUser;
}
