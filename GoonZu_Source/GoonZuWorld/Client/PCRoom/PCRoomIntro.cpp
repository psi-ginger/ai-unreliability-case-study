#include "PCRoomIntro.h"

#include "..\client.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

extern cltCommonLogic* pclClient;

cltPCRoomIntro::cltPCRoomIntro()
{
	m_bDraw = false;

	m_si32CurrentFrame	= 0;

	m_siFadeMode		= 0;
	m_siFadeStep		= 1;
}

cltPCRoomIntro::~cltPCRoomIntro()
{
	if( spr.pImage ) GP.FreeSpr( spr );
}

void cltPCRoomIntro::Start()
{
	if( m_bDraw ) return;

	m_si32CurrentFrame = 0;

	m_siFadeMode = 1;
	m_siFadeStep = 0;
		
	m_bDraw = true;
}

void cltPCRoomIntro::Stop()
{
	m_bDraw = false;

	if( spr.pImage ) GP.FreeSpr( spr );
}


void cltPCRoomIntro::Draw()
{
	if ( !m_bDraw ) return;

	if( spr.pImage == NULL ) {
		GP.LoadSprFromFile( TEXT("GImg/PCRoomIntro.spr"), &spr );
	}

	++m_si32CurrentFrame;

	if ( m_si32CurrentFrame > 200 )
	{
		if( m_siFadeMode != 2 ) {
			m_siFadeMode = 2;
			m_siFadeStep = 0;
		}
	}

	int alpha = 13;

	// Ã³À½ ¿­¸± ¶§
	if( m_siFadeMode == 1 ) {
		m_siFadeStep++;

		alpha = (int)(m_siFadeStep * 1.4);

		if( alpha > 13 ) {
			alpha = 13;
			m_siFadeMode = 0;
		}


	} 
	// ´ÝÈú ¶§ 
	else if ( m_siFadeMode == 2 ) {
		m_siFadeStep++;

		alpha = (int)(13 - ( m_siFadeStep * 0.9 ));

		if( alpha < 0 ) {
			alpha = 0;
			m_siFadeMode = 0;
			Stop();
			return;
		}
	}

	int x = ((cltClient*)pclClient)->GetScreenRX() + 
		(((cltClient*)pclClient)->siClientScreenXsize - spr.GetXSize() ) / 2;

	int y = ((cltClient*)pclClient)->GetScreenRY() + 100;

	GP.PutSprAlpha(&spr, x, y, alpha, 0 );

}