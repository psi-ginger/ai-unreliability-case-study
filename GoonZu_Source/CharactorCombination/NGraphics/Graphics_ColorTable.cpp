#include "graphics.h"

#include "..\\nutil\\nutil.h"




// 색상 테이블을 생성한다 
void NGraphics::CreateColorTable( SI32 siMaxTableNum )
{
	_delete_array( m_pColorTable );

	m_siMaxColorTableNum = 0;

	SI32 tablesize = 0;

	switch( m_dwBPP )
	{
	case 16:	tablesize = siMaxTableNum * 512;
		break;

	case 24:	tablesize = siMaxTableNum * 768;
		break;

	case 32:	tablesize = siMaxTableNum * 1024;
		break;
	}

	if( tablesize ) {
	
		m_pColorTable = new UI08[ tablesize ];

		memset( m_pColorTable, 0, tablesize );

		m_siMaxColorTableNum = siMaxTableNum;

		m_pCurrentColorTablePtr = m_pColorTable;
	}
}


// 컬러 테이블을 설정한다 
void NGraphics::SetColorTable( BYTE *pal, SI32 siTableIndex )
{

	SI32 tablesize = 0;

	switch( m_dwBPP )
	{
	case 16:	tablesize = 512;
		break;

	case 24:	tablesize = 768;
		break;

	case 32:	tablesize = 1024;
		break;
	}

	if( tablesize ) {

		memcpy( &m_pColorTable[ siTableIndex * tablesize ], pal, tablesize );
	}
}


// 파일로 부터 컬러 테이블을 읽어온다 
BOOL NGraphics::LoadColorTableFromFile( TCHAR* filename, SI32 siTableIndex )
{
	if( m_pColorTable == NULL ) return FALSE;

	HANDLE hFile = CreateFile( filename, 
							GENERIC_READ, 
							FILE_SHARE_READ, 
							NULL, 
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	DWORD dwFileSize;

	dwFileSize = GetFileSize( hFile, NULL );

	if( dwFileSize == INVALID_FILE_SIZE ||
		dwFileSize == 0 ) {

		CloseHandle( hFile );
		return FALSE;
	}

	// 사이즈가 맞지 않으면 리턴 
	if( dwFileSize % 768 ) {
		CloseHandle( hFile );
		return FALSE;
	}

	// 컬러 테이블 개수 
	SI32 tablenum = dwFileSize / 768;

	UI08 pal[ 1024 ];
	DWORD dwRead;
	BOOL bResult;

	int i, j;

	switch( m_dwBPP )
	{
	case 16:
		{
			RGB16 r, g, b;
			RGB16 *rgb16;

			for( i = 0; i < tablenum; ++i ) {

				bResult = ReadFile( hFile, pal, 768, &dwRead, NULL );

				if( bResult == FALSE || dwRead != 768 ) break;
			
				for( j = 0; j < 256; ++j ) {

					rgb16 = (RGB16 *)m_pColorTable;
					rgb16 += ( siTableIndex + i ) * 256 + j; 

					if( m_rgbmode == RGB16_565 ) {

						r = RGB16_565_RMASK & ( ( pal[ j * 3 ] >> 1 ) << 11 );
						g = RGB16_565_GMASK & ( ( pal[ j * 3 + 1 ] ) << 5 );
						b = RGB16_565_BMASK & ( pal[ j * 3 + 2 ] >> 1 );
						
						*rgb16 = r | g | b;
						
					} else if( m_rgbmode == RGB16_555 ) {

						r = RGB16_555_RMASK & ( ( pal[ j * 3 ] >> 1 ) << 10 );
						g = RGB16_555_GMASK & ( ( pal[ j * 3 + 1 ] >> 1 ) << 5 );
						b = RGB16_555_BMASK & ( pal[ j * 3 + 2 ] >> 1 );
						
						*rgb16 = r | g | b;

					}

				}
			}
		}
		break;

	case 24:
		{
		}
		break;

	case 32:
		{
		}
		break;
	}

	CloseHandle( hFile );

	return TRUE;

}

void NGraphics::SetCurrentColorTable( SI32 tableindex )
{
	m_pCurrentColorTablePtr = &m_pColorTable[ tableindex * 512 ];
}

void NGraphics::SetCurrentColorTable( RGB16 *pColorTable )
{
	m_pCurrentColorTablePtr = (UI08*)pColorTable;
}

void NGraphics::SetClipFullScreen()
{
	SetRect( &m_rcClip, 0, 0, m_width - 1, m_height - 1 );
}

void NGraphics::SetClipRect( RECT *pRect )
{
	CopyRect( &m_rcClip, pRect );
}

void NGraphics::SetClipRect( int x1, int y1, int x2, int y2 )
{
	SetRect( &m_rcClip, x1, y1, x2, y2 );
}

void NGraphics::CreateAlphaTable()
{
	int i, j;

	RGB16 r, g, b, c;

	if( m_rgbmode == RGB16_565 ) {

		for( i = 0; i < MAX_ALPHA_LEVEL; ++i ) {

			_delete_array( m_pAlphaTable[ i ] );

			m_pAlphaTable[ i ] = new RGB16[ 65536 ];

			for( j = 0; j < 65536; ++j ) {

				c = j;

				r = ( c & RGB16_565_RMASK ) >> 11;
				g = ( c & RGB16_565_GMASK ) >> 5;
				b = ( c & RGB16_565_BMASK );

				r = (RGB16)((float)r * ( i / (float)( MAX_ALPHA_LEVEL - 1) ));
				g = (RGB16)((float)g * ( i / (float)( MAX_ALPHA_LEVEL - 1) ));
				b = (RGB16)((float)b * ( i / (float)( MAX_ALPHA_LEVEL - 1) ));

				r = ( r << 11 ) & RGB16_565_RMASK;
				g = ( g << 5 ) & RGB16_565_GMASK;
				b = b & RGB16_565_BMASK;

				m_pAlphaTable[ i ][ j ] = r | g | b;

			}
		}

	} else if( m_rgbmode == RGB16_555 ) {

		for( i = 0; i < MAX_ALPHA_LEVEL; ++i ) {

			_delete_array( m_pAlphaTable[ i ] );

			m_pAlphaTable[ i ] = new RGB16[ 65536 ];

			for( j = 0; j < 65536; ++j ) {

				c = j;

				r = ( c & RGB16_555_RMASK ) >> 10;
				g = ( c & RGB16_555_GMASK ) >> 5;
				b = ( c & RGB16_555_BMASK );

				r = (RGB16)((float)r * ( i / (float)( MAX_ALPHA_LEVEL - 1) ));
				g = (RGB16)((float)g * ( i / (float)( MAX_ALPHA_LEVEL - 1) ));
				b = (RGB16)((float)b * ( i / (float)( MAX_ALPHA_LEVEL - 1) ));

				r = ( r << 10 ) & RGB16_555_RMASK;
				g = ( g << 5 ) & RGB16_555_GMASK;
				b = b & RGB16_555_BMASK;

				m_pAlphaTable[ i ][ j ] = r | g | b;

			}
		}

	}

}

void NGraphics::CreateEffectTable()
{

	int color;
	int i, j;

	for( i = 0; i < 32; ++i ) {

		for( j = 0; j < 32; ++j ) { 

			color = max( i, j );
			m_pEffectTable[ EFFECT_LIGHTEN ][ ( i << 5 ) + j ] = (RGB16)color;
			
			color = max( i, j ) + ( (32 - max( i, j )) / 32 * min( i, j ) );
			if( color < 0 ) color = 0;
			if( color > 31 ) color = 31;
			m_pEffectTable[ EFFECT_SCREEN ][ ( i << 5 ) + j ] = (RGB16)color;
			
			color = ( i * 32 ) / ( 32 - j );
			if( color < 0 ) color = 0;
			if( color > 31 ) color = 31;
			m_pEffectTable[ EFFECT_COLORDODGE ][ ( i << 5 ) + j ] = (RGB16)color;
				
			color = (i * ( 32 - j )) / 32;
			if( color < 0 ) color = 0;
			if( color > 31 ) color = 31;
			m_pEffectTable[ EFFECT_DODGEBURN ][ ( i << 5 ) + j ] = (RGB16)color;
				
			color = max( i, j ) - min( i, j );
			if( color < 0 ) color = 0;
			if( color > 31 ) color = 31;
			m_pEffectTable[ EFFECT_DIFFERENT ][ ( i << 5 ) + j ] = (RGB16)color;




			color = max( i, j ) + (( 32 - max( i, j )) / 32 * min( i, j ));
			if( color < 0 ) color = 0;
			if( color > 31 ) color = 31;
			m_pEffectTable[ EFFECT_GLIGHT ][ ( i << 5 ) + j ] = (RGB16)color;			

			color = j + i * ( 32 - j ) / 32;
			if( color < 0 ) color = 0;
			if( color > 31 ) color = 31;
			m_pEffectTable[ EFFECT_GSCREEN ][ ( i << 5 ) + j ] = (RGB16)color;

			color = i + j;
			if( color < 0 ) color = 0;
			if( color > 31 ) color = 31;
			m_pEffectTable[ EFFECT_GADD ][ ( i << 5 ) + j ] = (RGB16)color;

			color = i * j / 32;
			if( color < 0 ) color = 0;
			if( color > 31 ) color = 31;
			m_pEffectTable[ EFFECT_GMULTIPLY ][ ( i << 5 ) + j ] = (RGB16)color;
				
		}
	}

}

void NGraphics::CreateEffectFTable()
{
	_delete_array( m_pEffectFTable[ 0 ] );
	_delete_array( m_pEffectFTable[ 1 ] );
	_delete_array( m_pEffectFTable[ 2 ] );

	m_pEffectFTable[ 0 ] = new RGB16[ 65536 ];
	m_pEffectFTable[ 1 ] = new RGB16[ 65536 ];
	m_pEffectFTable[ 2 ] = new RGB16[ 65536 ];

	int i;
	RGB16 r, g, b, c;

	if( m_rgbmode == RGB16_565 ) {

		for( i = 0; i < 65536; ++i ) {

			c = i;

			r = ( c & RGB16_565_RMASK ) >> 11;
			g = ( c & RGB16_565_GMASK ) >> 6;
			b = ( c & RGB16_565_BMASK );

			r = (RGB16)(r * 1.7);
			if( r > 31 ) r = 31;

			g = (RGB16)(g * 1.7);
			if( g > 31 ) g = 31;

			b = (RGB16)(b * 1.7);
			if( b > 31 ) b = 31;

			r = ( r << 11 ) & RGB16_565_RMASK;
			g = ( g << 6 ) & RGB16_565_GMASK;
			b = b & RGB16_565_BMASK;

			m_pEffectFTable[ 0 ][ i ] = r | g | b;



			r = ( c & RGB16_565_RMASK ) >> 11;
			g = ( c & RGB16_565_GMASK ) >> 6;
			b = ( c & RGB16_565_BMASK );

			r = r / 4;
			if( r > 31 ) r = 31;

			g = g / 4;
			if( g > 31 ) g = 31;

			b = b / 4;
			if( b > 31 ) b = 31;

			r = ( r << 11 ) & RGB16_565_RMASK;
			g = ( g << 6 ) & RGB16_565_GMASK;
			b = b & RGB16_565_BMASK;

			m_pEffectFTable[ 1 ][ i ] = r | g | b;




			r = ( c & RGB16_565_RMASK ) >> 11;
			g = ( c & RGB16_565_GMASK ) >> 6;
			b = ( c & RGB16_565_BMASK );

			r = r * 0.5;
			if( r > 31 ) r = 31;

			g = g * 0.5;
			if( g > 31 ) g = 31;

			b = b * 0.9;
			if( b > 31 ) b = 31;

			r = ( r << 11 ) & RGB16_565_RMASK;
			g = ( g << 6 ) & RGB16_565_GMASK;
			b = b & RGB16_565_BMASK;

			m_pEffectFTable[ 2 ][ i ] = r | g | b;


		}

	} else {

		for( i = 0; i < 65536; ++i ) {

			c = i;

			r = ( c & RGB16_555_RMASK ) >> 10;
			g = ( c & RGB16_555_GMASK ) >> 5;
			b = ( c & RGB16_555_BMASK );

			r = (RGB16)(r * 1.7);
			if( r > 31 ) r = 31;

			g = (RGB16)(g * 1.7);
			if( g > 31 ) g = 31;

			b = (RGB16)(b * 1.7);
			if( b > 31 ) b = 31;

			r = ( r << 10 ) & RGB16_555_RMASK;
			g = ( g << 5 ) & RGB16_555_GMASK;
			b = b & RGB16_555_BMASK;

			m_pEffectFTable[ 0 ][ i ] = r | g | b;



			r = ( c & RGB16_555_RMASK ) >> 11;
			g = ( c & RGB16_555_GMASK ) >> 5;
			b = ( c & RGB16_555_BMASK );

			r = r / 4;
			if( r > 31 ) r = 31;

			g = g / 4;
			if( g > 31 ) g = 31;

			b = b / 4;
			if( b > 31 ) b = 31;

			r = ( r << 10 ) & RGB16_555_RMASK;
			g = ( g << 5 ) & RGB16_555_GMASK;
			b = b & RGB16_555_BMASK;

			m_pEffectFTable[ 1 ][ i ] = r | g | b;

			r = ( c & RGB16_555_RMASK ) >> 10;
			g = ( c & RGB16_555_GMASK ) >> 5;
			b = ( c & RGB16_555_BMASK );

			r = r * 0.5;
			if( r > 31 ) r = 31;

			g = g * 0.5;
			if( g > 31 ) g = 31;

			b = b * 0.9;
			if( b > 31 ) b = 31;

			r = ( r << 10 ) & RGB16_555_RMASK;
			g = ( g << 5 ) & RGB16_555_GMASK;
			b = b & RGB16_555_BMASK;

			m_pEffectFTable[ 2 ][ i ] = r | g | b;


		}

	}
}


/*

	int i, j;

	RGB16 r1, g1, b1, r2, g2, b2, c1, c2;

	c2 = 0;

	r2 = ( c2 & RGB16_565_RMASK ) >> 11;
	g2 = ( c2 & RGB16_565_GMASK ) >> 6;
	b2 = ( c2 & RGB16_565_BMASK );

	r2 = 20;
	g2 = 20;
	b2 = 20;


	if( m_rgbmode == RGB16_565 ) {

		_delete_array( m_pEffectTable[ 0 ] );
		_delete_array( m_pEffectTable[ 1 ] );

		m_pEffectTable[ 0 ] = new RGB16[ 65536 ];
		m_pEffectTable[ 1 ] = new RGB16[ 65536 ];

		for( i = 0; i < 65536; ++i ) {

			c1 = i;

			r1 = ( c1 & RGB16_565_RMASK ) >> 11;
			g1 = ( c1 & RGB16_565_GMASK ) >> 6;
			b1 = ( c1 & RGB16_565_BMASK );
		
			r1 = r1 + r1 * (r1 / 31);
			g1 = r1 + g1 * (g1 / 31);
			b1 = r1 + b1 * (b1 / 31);

			if( r1 > 31 ) r1 = 31;
			if( g1 > 31 ) g1 = 31;
			if( b1 > 31 ) b1 = 31;

			r1 = ( r1 << 11 ) & RGB16_565_RMASK;
			g1 = ( g1 << 6 ) & RGB16_565_GMASK;
			b1 = b1 & RGB16_565_BMASK;

			m_pEffectTable[ 0 ][ i ] = r1 | g1 | b1;


			r1 = ( c1 & RGB16_565_RMASK ) >> 11;
			g1 = ( c1 & RGB16_565_GMASK ) >> 6;
			b1 = ( c1 & RGB16_565_BMASK );
		
			r1 = r1 * 10.1;
			g1 = g1 * 10.1;
			b1 = b1 * 10.1;

			if( r1 > 31 ) r1 = 31;
			if( g1 > 31 ) g1 = 31;
			if( b1 > 31 ) b1 = 31;

			r1 = ( r1 << 11 ) & RGB16_565_RMASK;
			g1 = ( g1 << 6 ) & RGB16_565_GMASK;
			b1 = b1 & RGB16_565_BMASK;

			m_pEffectTable[ 1 ][ i ] = r1 | g1 | b1;


		}

		for( i = 0; i < 32; ++i ) {

			for( j = 0; j < 32; ++j ) { 

				m_pEffect[ 0 ][ ( i << 5 ) + j ] = max( i, j );
			}
		}

	} else if( m_rgbmode == RGB16_555 ) {

	}


*/