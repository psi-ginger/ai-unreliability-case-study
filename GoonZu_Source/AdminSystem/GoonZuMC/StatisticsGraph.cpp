#include "StatisticsGraph.h"
#include "StatisticsData.h"
#include "GoonZuMC.h"
#include "../../Nlib/ndate.h"

CStatisticsGraph::CStatisticsGraph()
{
	m_viewWidth = 0;
	m_viewHeight = 0;

	m_bCaptured = FALSE;

	m_bInit = FALSE;
	
}

CStatisticsGraph::~CStatisticsGraph()
{
}

void CStatisticsGraph::Init( int width, int height )
{

	m_font.CreateFont( 11, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "굴림" );

	COLORREF col[] = { 
		RGB(  70,  70, 225 ),
		RGB( 225,  70,  70 ), 
		RGB( 100,  60,  40 ),
		RGB(  60, 200,  60 ) 
		
	};

	for( int i = 0; i < MAX_LAYER_NUMBER; ++i )
	{
		m_layerColor[ i ] = col[ i ];
	}


	CDC dc;
	dc.CreateDC( "DISPLAY", NULL, NULL, NULL );

	// mem dc
	m_dc.CreateCompatibleDC( &dc );

	m_bitmap.CreateCompatibleBitmap( &dc, width, height );

	m_dc.SelectObject( &m_bitmap );

	// mem dc2
	m_dc2.CreateCompatibleDC( &dc );

	m_bitmap2.CreateCompatibleBitmap( &dc, width, height );

	m_dc2.SelectObject( &m_bitmap2 );

	m_viewWidth = width;
	m_viewHeight = height;

	m_bInit = TRUE;	

	Clear( RGB( 255, 255, 255 ) );

}

void CStatisticsGraph::SetGraphProperty( int orgX, int orgY, int width, int height )
{
	m_orgX = orgX;
	m_orgY = orgY;

	m_graphWidth = width;
	m_graphHeight = height;

	m_maxValue = DEFAULT_MAXVALUE;
}

void CStatisticsGraph::Clear( COLORREF col )
{
	if( m_bInit == FALSE ) return;

	m_dc.FillSolidRect( 0, 0, m_viewWidth, m_viewHeight, col );
}

void CStatisticsGraph::SetMaxValue( int maxValue )
{
	m_maxValue = (int)( (float)( maxValue + 600 ) / 500. ) * 500;

	if( m_maxValue < DEFAULT_MAXVALUE ) m_maxValue = DEFAULT_MAXVALUE;
}

void CStatisticsGraph::DrawGrid( int valueUnit, int impactUnit, int timeUnit )
{
	if( m_bInit == FALSE ) return;

	int i, x, y;

	CPen *oldpen, pen1, pen2;
	pen1.CreatePen( PS_SOLID, 1, RGB( 220, 220, 220 ) );
	pen2.CreatePen( PS_SOLID, 1, RGB( 170, 170, 170 ) );

	oldpen = m_dc.SelectObject( &pen1 );

	for( i = 0; i <= MAX_DATA_NUMBER; i += timeUnit )
	{

		if( i == 10 * 12 || i == 22 * 12 ) m_dc.SelectObject( &pen2 );

		x = m_orgX + (int)( (i / 288.f) * m_graphWidth );
		m_dc.MoveTo( x, m_orgY );
		m_dc.LineTo( x, m_orgY - m_graphHeight );

		m_dc.SelectObject( &pen1 );
	}

	for( i = 0; i <= m_maxValue; i += valueUnit )
	{
		if( i % impactUnit == 0 ) m_dc.SelectObject( &pen2 );

		y = m_orgY - (int)( ((float)i / m_maxValue) * m_graphHeight );
		m_dc.MoveTo( m_orgX, y );
		m_dc.LineTo( m_orgX + m_graphWidth, y );

		m_dc.SelectObject( &pen1 );
	}

	m_dc.SelectObject( oldpen );
}

void CStatisticsGraph::DrawValueUnitText( int interval )
{
	if( m_bInit == FALSE ) return;

	int i, y;
	char buf[ 32 ];
	CFont *pFont;
	CPen *oldpen, pen;

	pen.CreatePen( PS_SOLID, 1, RGB( 50, 90, 90 ) );

	oldpen = m_dc.SelectObject( &pen );

	pFont = m_dc.SelectObject( &m_font );

	for( i = interval; i <= m_maxValue; i+= interval ) {
		
		y = m_orgY - (int)( ((float)i / m_maxValue) * m_graphHeight );

		m_dc.MoveTo( m_orgX - 3, y );
		m_dc.LineTo( m_orgX + 2, y );

		sprintf( buf, "%5d", i );
	
		m_dc.TextOut( m_orgX - 44, y - 4, buf );

	}

	m_dc.SelectObject( pFont );

	m_dc.SelectObject( oldpen );

}

void CStatisticsGraph::DrawTimeUnitText( int interval )
{
	if( m_bInit == FALSE ) return;

	int i, j, x;
	char buf[ 32 ];
	CFont *pFont;
	CPen *oldpen, pen;

	pen.CreatePen( PS_SOLID, 1, RGB( 50, 90, 90 ) );

	oldpen = m_dc.SelectObject( &pen );

	pFont = m_dc.SelectObject( &m_font );

	for( i = 0; i <= MAX_DATA_NUMBER; i += interval ) {
		x = m_orgX + (int)( ( i / 288.f ) * m_graphWidth );

		m_dc.MoveTo( x, m_orgY - 1 );
		m_dc.LineTo( x, m_orgY + 4 );

		sprintf( buf, "%2d", ( i * 5 ) / 60 );
	
		m_dc.TextOut( x - 10, m_orgY + 6, buf );
	}

	m_dc.SelectObject( pFont );

	m_dc.SelectObject( oldpen );
}

void CStatisticsGraph::DrawGraph( int layer )
{
	if( m_bInit == FALSE ) return;

	CStatisticsData *pData = NULL;

	pData = ((CGoonZuMCApp *)AfxGetApp())->m_pStatisticsData;

	if( pData == NULL ) return;
	if( pData->m_bVisible[ layer ] == FALSE ) return;

	CPen *oldpen, pen;

	pen.CreatePen( PS_SOLID, 1, m_layerColor[ layer ] );

	oldpen = m_dc.SelectObject( &pen );

	int value, oldvalue, x, y ;

	oldvalue = 0;

	pData->CriticalEnter();

	for ( int i = 0 ; i < MAX_DATA_NUMBER; i ++ )
	{
		value = pData->m_siUserNum[ layer ][ i ];
		
		x = m_orgX + (int)( ( i / 288.f ) * m_graphWidth );
		y = m_orgY - (int)( ((float)value / m_maxValue) * m_graphHeight );

		if( i == 0 || value < oldvalue / 2 ) {
		
			m_dc.MoveTo( x, y );
		
		}else {

			m_dc.LineTo( x, y);
		}

		oldvalue = value;
	}

	pData->CritcalLeave();

	m_dc.SelectObject( oldpen );
}

void CStatisticsGraph::DrawAvarage( int layer )
{
	char *szDay[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

	if( m_bInit == FALSE ) return;

	CStatisticsData *pData = NULL;

	pData = ((CGoonZuMCApp *)AfxGetApp())->m_pStatisticsData;

	if( pData == NULL ) return;
	if( pData->m_bVisible[ layer ] == FALSE ) return;

	CPen *oldpen, pen;
	CFont *pFont;

	pen.CreatePen( PS_SOLID, 1, m_layerColor[ layer ] );
	pFont = m_dc.SelectObject( &m_font );

	oldpen = m_dc.SelectObject( &pen );

	int value, oldvalue, x, y ;

	oldvalue = 0;

	pData->CriticalEnter();

	int validcount = 0;
	int total = 0;
	float ava = 0;

	for( int i = 0 ; i < MAX_DATA_NUMBER; i ++ )
	{
		value = pData->m_siUserNum[ layer ][ i ];

		if( value > 0 ) validcount = i + 1;
		total += value;
	}

	ava = (float)total / (float)validcount;

	x = m_orgX + (int)( ( validcount / 288.f ) * m_graphWidth );
	y = m_orgY - (int)( ( ava / (float)m_maxValue) * m_graphHeight );

	m_dc.MoveTo( m_orgX, y );
	m_dc.LineTo( x, y );

	char buffer[ 256 ];
	sprintf( buffer, "%d", (int)ava );

	COLORREF col = m_dc.SetTextColor( m_layerColor[ layer ] );

	m_dc.TextOut( m_orgX + (layer * 33), y - 12, buffer );
	

	// 날짜 표시 
	int year, month, day;

	memcpy( buffer, pData->m_szDate[ layer ], 4 );
	buffer[ 4 ] = NULL;
	year = atoi( buffer );

	memcpy( buffer, &pData->m_szDate[ layer ][ 5 ], 2 );
	buffer[ 2 ] = NULL;
	month = atoi( buffer );

	memcpy( buffer, &pData->m_szDate[ layer ][ 8 ], 2 );
	buffer[ 8 ] = NULL;
	day = atoi( buffer );
	
	NDate date;
	date.SetDate( year, month, day );

	sprintf( buffer, "■ %s [%s]", pData->m_szDate[ layer ], szDay[ date.GetDateVary() % 7 ] );
	m_dc.TextOut( m_orgX + m_graphWidth - 100, m_orgY - 14 - ( (3 - layer) * 13 ), buffer );
	
	m_dc.SetTextColor( col );

	pData->CritcalLeave();
	
	m_dc.SelectObject( pFont );
	m_dc.SelectObject( oldpen );
}

void CStatisticsGraph::Draw()
{
	if( m_bInit == FALSE ) return;

	int i;

	CStatisticsData *pData = NULL;

	pData = ((CGoonZuMCApp *)AfxGetApp())->m_pStatisticsData;

	if( pData == NULL ) return;

	Clear( RGB( 255, 255, 255 ) );

	// 최대값 구하기
	int maxvalue = 0;
	for( i = 0; i < MAX_LAYER_NUMBER; ++i ) {

		if( pData->m_bVisible[ i ] ) {

			if( maxvalue < pData->m_siMaxUserNum[ i ] ) maxvalue = pData->m_siMaxUserNum[ i ];
		}

	}

	// 최대값 설정
	SetMaxValue( maxvalue );


	int valueunit = 25;
	int textinterval = 50;

	if( m_maxValue > 500 ) { valueunit = 50; textinterval = 100; }
	if( m_maxValue > 1000 ) { valueunit = 50; textinterval = 150; }
	if( m_maxValue > 2000 ) { valueunit = 100; textinterval = 500; }
	if( m_maxValue > 5000 ) { valueunit = 250; textinterval = 500; };
	if( m_maxValue > 10000 ) { valueunit = 500; textinterval = 1000; }
	if( m_maxValue > 20000 ) { valueunit = 1000; textinterval = 2000; }

	DrawGrid( valueunit, textinterval, 6 );

 	DrawValueUnitText( textinterval );
	DrawTimeUnitText( 24 );

	for( i = MAX_LAYER_NUMBER - 1; i >= 0; --i ) {
		DrawGraph( i );
		DrawAvarage( i );
	}
}

void CStatisticsGraph::PutEx( int x, int y, CDC *pDC, int pointx, int pointy )
{
	if( m_bInit == FALSE ) return;

	int i;

	int gx = pointx - m_orgX;
	int gy = m_orgY - pointy;

	if( gx < 0 || gx > m_graphWidth ||
		gy < 0 || gy > m_graphHeight ) {

		if( m_bCaptured ) {
			ShowCursor( TRUE );
			ReleaseCapture();
			m_bCaptured = FALSE;
		}
		
		return;

	}

	if( m_bCaptured == FALSE ) {
		SetCapture( NULL );
		ShowCursor( FALSE );
		m_bCaptured = TRUE;
	}	

	m_dc2.BitBlt( 0, 0, m_viewWidth, m_viewHeight, &m_dc, 0, 0, SRCCOPY );


	CFont *pFont;

	pFont = m_dc.SelectObject( &m_font );

	CPen *oldpen, pen;

	pen.CreatePen( PS_SOLID, 1, RGB( 30, 30, 30 ) );

	oldpen = m_dc2.SelectObject( &pen );

	m_dc2.MoveTo( pointx, m_orgY );
	m_dc2.LineTo( pointx, m_orgY - m_graphHeight );

	m_dc2.MoveTo( m_orgX, pointy );
	m_dc2.LineTo( m_orgX + m_graphWidth, pointy );


	int bx, by;
	char buf[ 32 ];

	// Draw Value

	if( gx < m_graphWidth / 2 ) bx = m_orgX + m_graphWidth - 40;
	else bx = m_orgX + 2;

	by = pointy - 14;

	int curvalue = (int)(( (float)( gy ) / m_graphHeight ) * m_maxValue );
	sprintf( buf, "%5d", curvalue );

	m_dc2.TextOut( bx, by, buf );


	// Draw Text

	
	if( gx < m_graphWidth / 2 ) bx = pointx + 5;
	else bx = pointx - 105;

	CStatisticsData *pData = NULL;

	pData = ((CGoonZuMCApp *)AfxGetApp())->m_pStatisticsData;
	
	if( pData == NULL ) {

		m_dc2.SelectObject( oldpen );
		m_dc2.SelectObject( pFont );
		return;
	}


	int dataindex = (int)(( (float)(gx + 1 ) / m_graphWidth ) * MAX_DATA_NUMBER);

	pData->CriticalEnter();

	by = pointy - 14;

	sprintf( buf, "Time %2d:%2d", ( dataindex * 5 ) / 60, ( dataindex * 5 ) % 60 );
	m_dc2.TextOut( bx, by, buf );

	by += 20;

	for( i = 0; i < MAX_LAYER_NUMBER; ++i ) {

		if( pData->m_bVisible[ i ] ) {

			sprintf( buf, "%s [%d]", 
				pData->m_szDate[ i ], 
				pData->m_siUserNum[ i ][ dataindex ] );
			
			m_dc2.TextOut( bx, by, buf );

			by += 15;
		}
	}

	if( pData->m_bVisible[ 0 ] && pData->m_bVisible[ 1 ] ) {
		by += 5;
		sprintf( buf, "Diff(1,2) [%d]", 
			pData->m_siUserNum[ 0 ][ dataindex ] - pData->m_siUserNum[ 1 ][ dataindex ] );
		m_dc2.TextOut( bx, by, buf );
	}

	pData->CritcalLeave();

	pDC->BitBlt( x, y, m_viewWidth, m_viewHeight, &m_dc2, 0, 0, SRCCOPY );	

	m_dc2.SelectObject( oldpen );
	m_dc2.SelectObject( pFont );

}

void CStatisticsGraph::Put( int x, int y, CDC *pDC )
{
	if( m_bInit == FALSE ) return;

	pDC->BitBlt( x, y, m_viewWidth, m_viewHeight, &m_dc, 0, 0, SRCCOPY );
}