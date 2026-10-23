#ifndef _STATISTICSGRAPH_H
#define _STATISTICSGRAPH_H

#define DEFAULT_MAXVALUE			500

#include "stdafx.h"
#include "StatisticsData.h"

class CStatisticsGraph
{
public:
	CStatisticsGraph();
	~CStatisticsGraph();

	void		Init( int width, int height );
	void		Put( int x, int y, CDC *pDC );
	void		PutEx( int x, int y, CDC *pDC, int pointx, int pointy );
	
	void		SetGraphProperty( int orgX, int orgY, int width, int height );
	void		SetMaxValue( int maxValue );
	
	void		Clear( COLORREF col );
	void		DrawGrid( int valueUnit, int impactUnit, int timeUnit );
	
	void		DrawValueUnitText( int interval );
	void		DrawTimeUnitText( int interval );

	void		DrawGraph( int layer );
	void		DrawAvarage( int layer );
	
	void		Draw();

private:
	CDC			m_dc;
	CBitmap		m_bitmap;

	CDC			m_dc2;
	CBitmap		m_bitmap2;

	int			m_viewWidth;
	int			m_viewHeight;

	int			m_orgX;
	int			m_orgY;

	int			m_graphWidth;
	int			m_graphHeight;

	int			m_maxValue;
	
	CFont		m_font;
	COLORREF	m_layerColor[ MAX_LAYER_NUMBER ];

	BOOL		m_bCaptured;

	BOOL		m_bInit;
};

#endif