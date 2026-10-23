#include "Graphics.h"
#include "Graphic.h"

#include "Graphics_XPR16.h"
#include "TSpr.h"
#include "Graphic_Capsule.h"

#include "NUtil.h"

#include <memory.h>
#include <wchar.h>

// ������
XPR16::XPR16() : m_ppImage( NULL ), m_siFontNum( 0 ), m_siXSize( 0 ), m_siYSize( 0 ), m_colorkey( 0 )
{
}

// �Ҹ���
XPR16::~XPR16()
{
	Destroy();
}

// �����Լ� 
void XPR16::Destroy()
{
	m_siFontNum = 0;
	m_siXSize = 0;
	m_siYSize = 0;
	m_colorkey = 0;

	for( int i = 0; i < m_siFontNum; ++i ) 
	{
		NDelete_Array( m_ppImage[ i ] );
	}

	NDelete_Array( m_ppImage );
}

// ���Ϸε� 
bool XPR16::LoadFromSpr16( TCHAR *szFilename )
{
	Destroy();

	// �۾��� ����
	RGB16	*pSrcImage = NULL;
	RGB16	*pEncodeImage = NULL;
	
	int		size;

	TSpr	spr;

	if( GP.LoadSprFromFile( szFilename, &spr ) == FALSE ) return false;

	// ��Ʈ ����, x size, y size ���� 
	m_siFontNum = spr.GetImageNum();
	m_siXSize = spr.GetXSize();
	m_siYSize = spr.GetYSize();


	// ������ ����
	m_colorkey = spr.pal[ COLOR_TRANS ];

	// ��Ʈ ���� ��ŭ �̹��� ������ ����
	m_ppImage = new RGB16*[ m_siFontNum ];
	if(m_ppImage == NULL)		return false;

	// pSrcImage ����
	pSrcImage = new RGB16[ m_siXSize * m_siYSize ];
	if(pSrcImage == NULL)		return false;

	// pEncodeImage ����
	pEncodeImage = new RGB16[ m_siXSize * m_siYSize * 2 ];
	if(pEncodeImage == NULL)		return false;

	// pSrcImage�ʱ�ȭ ( 2����Ʈ ���� memset )
	wmemset( (wchar_t*)pSrcImage, m_colorkey, m_siXSize * m_siYSize );
	
	// g_graphics�� pitch �� image��ġ ����
	long pitch = g_graphics.m_lPitch;
	void* pImage = g_graphics.m_pImage;

	g_graphics.m_lPitch = m_siXSize << 1;
	g_graphics.m_pImage = pSrcImage;

	// ��Ʈ ���� ��ŭ �ݺ�
	for( int i = 0; i < m_siFontNum; ++i )
	{
		// pSrcImage�� ���
		if( spr.header.iEncodeType == 0 ) {
			GP.PutSpr(&spr, 0, 0, i );
		} else {
			g_graphics.PutMpr16T( 0, 0, &spr.pImage[ spr.header.ImageInfo[ i ].iStartPos ], 
				spr.GetXSize(), spr.GetYSize() );
		}

		// pSrcImage -> pEncodeImage XPR ���ڵ�
		g_graphics.EncodeXpr( pEncodeImage, m_siXSize, m_siYSize, m_colorkey, &size );

		// �̹��� ���� �� ����
	
		m_ppImage[ i ] = new RGB16[ size >> 1 ];
		if(m_ppImage[ i ])		memcpy( m_ppImage[ i ], pEncodeImage, size );
	}

	// g_graphics�� pitch �� image��ġ ����
	g_graphics.m_lPitch = pitch;
	g_graphics.m_pImage = pImage;

	// �۾��� ���� ����
	NDelete_Array( pSrcImage );
	NDelete_Array( pEncodeImage );

	return true;
}

void XPR16::PutXpr16( int x, int y, int font )
{
	if( m_siFontNum <= font)
		return;

	if(m_ppImage == NULL)				return;
	if(m_ppImage[ font ] == NULL)		return;

	if( clGrp.IsInClipArea( x, y, x + m_siXSize - 1, y + m_siYSize - 1 ) == TRUE ) {

		g_graphics.PutXpr16T( x, y, m_siXSize, m_siYSize, m_ppImage[ font ], m_colorkey );

	} else {

		g_graphics.PutXpr16CT( x, y, m_siXSize, m_siYSize, m_ppImage[ font ], m_colorkey );
	}
}

void XPR16::PutXpr16A( int x, int y, int font, int alpha )
{
	if( m_siFontNum <= font)
		return;

	if(m_ppImage == NULL)				return;
	if(m_ppImage[ font ] == NULL)		return;

	if( clGrp.IsInClipArea( x, y, x + m_siXSize - 1, y + m_siYSize - 1 ) == TRUE ) {

		g_graphics.PutXpr16AT( x, y, m_siXSize, m_siYSize, m_ppImage[ font ], m_colorkey, alpha );

	} else {

		g_graphics.PutXpr16CAT( x, y, m_siXSize, m_siYSize, m_ppImage[ font ], m_colorkey, alpha );
	}
}
