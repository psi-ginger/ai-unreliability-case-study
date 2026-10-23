#ifndef _STATISTICSDATA_H
#define _STATISTICSDATA_H

#include "stdafx.h"

#define MAX_LAYER_NUMBER	4
#define MAX_DATA_NUMBER		288

class CStatisticsData
{
public:
	CStatisticsData();
	~CStatisticsData();

	void				CriticalEnter() { EnterCriticalSection( &m_cs ); };
	void				CritcalLeave() { LeaveCriticalSection( &m_cs ); };

	void				ClearAll();
	void				Clear( int layer );

	void				SetMaxUser( int layer );
	int					GetMaxUser( int layer ) { return m_siMaxUserNum[ layer ]; };


public:
	int					m_siUserNum[ MAX_LAYER_NUMBER ][ MAX_DATA_NUMBER ];
	BOOL				m_bVisible[ MAX_LAYER_NUMBER ];

	char				m_szDate[ MAX_LAYER_NUMBER ][ 32 ];

	int					m_siMaxUserNum[ MAX_LAYER_NUMBER ];

	CRITICAL_SECTION	m_cs;
};


#endif
