#include "FeastEffect.h"
#include "../Lib/Graphic/Graphic_Capsule.h"

#include <math.h>
#include "..\client.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

extern cltCommonLogic* pclClient;

CFeastEffect::CFeastEffect()
{
	Init();
}
/*
CFeastEffect::CFeastEffect(const CFeastEffect& aFeastEffect)
{
	m_pSprFeastEffect = aFeastEffect.m_pSprFeastEffect;

	m_siPositionX = aFeastEffect.m_siPositionX;			// X위치값
	m_siPositionY = aFeastEffect.m_siPositionY;			// Y위치값
	m_siDrawType = aFeastEffect.m_siDrawType;			// 0: 그냥 찍기 1: 효과로 찍기
	m_siCurrentFrame = aFeastEffect.m_siCurrentFrame;
	m_bEffectLive = aFeastEffect.m_bEffectLive;
}

CFeastEffect& CFeastEffect::operator=(const CFeastEffect& aFeastEffect)
{
	if( this == &aFeastEffect)
		return *this;

	if(m_pSprFeastEffect)
	{
		if(m_pSprFeastEffect->Image)
		{
			GP.FreeSpr( *m_pSprFeastEffect );
		}

		delete m_pSprFeastEffect;
		m_pSprFeastEffect = NULL;
	}

	m_pSprFeastEffect = new cltTSpr;

	m_siPositionX = aFeastEffect.m_siPositionX;			// X위치값
	m_siPositionY = aFeastEffect.m_siPositionY;			// Y위치값
	m_siDrawType = aFeastEffect.m_siDrawType;			// 0: 그냥 찍기 1: 효과로 찍기
	m_siCurrentFrame = aFeastEffect.m_siCurrentFrame;
	m_bEffectLive = aFeastEffect.m_bEffectLive;

	return *this;
}
*/
CFeastEffect::~CFeastEffect()
{
	DeleteEffect();
}

void CFeastEffect::Init()
{
	m_pSprFeastEffect = new TSpr;

	m_siPositionX = 0;	
	m_siPositionY = 0;	
	m_siDrawType = NORMAL_DRAW;
	m_siCurrentFrame = -1;

	m_bEffectLive = FALSE;
}

void CFeastEffect::CreateEffect(SI32 siKind)
{
}

void CFeastEffect::DeleteEffect()
{
	if(m_pSprFeastEffect)
	{
		if(m_pSprFeastEffect->pImage)
		{
			GP.FreeSpr( *m_pSprFeastEffect );
		}

		delete m_pSprFeastEffect;
		m_pSprFeastEffect = NULL;
	}
}

void CFeastEffect::Action()
{
}

void CFeastEffect::Draw()
{
}

CFeastEffectOne::CFeastEffectOne(SI32 siKind) : CFeastEffect()
{
	siStepX = 0;
	siStepY = 0;
	
	CreateEffect(siKind);
	srand( (unsigned)time( NULL ) );
}

CFeastEffectOne::~CFeastEffectOne()
{
}

void CFeastEffectOne::CreateEffect(SI32 siKind)
{
	if(siKind < 0 || siKind > 2)
		return;

	switch(siKind)
	{
	case 0:
		{
			GP.LoadSprFromFile(EFFECT_KIND_0, GetEffectSpr());

			SetDrawType( NORMAL_DRAW );
			SetEffectLive( TRUE );
			
		}
		break;
	case 1:
		{
			GP.LoadSprFromFile(EFFECT_KIND_1, GetEffectSpr());

			SetDrawType( NORMAL_DRAW );
			SetEffectLive( TRUE );
		}
		break;
	case 2:
		{
			GP.LoadSprFromFile(EFFECT_KIND_2, GetEffectSpr());

			SetDrawType( NORMAL_DRAW );
			SetEffectLive( TRUE );

		}
		break;
	}
	static int i=0;
	siCount = i++;
}

void CFeastEffectOne::Action()
{
	if( GetEffectLive() )
	{
		SetCurrentFrame(GetCurrentFrame() + 1);

		if( GetCurrentFrame() == GetEffectSpr()->GetImageNum() )
		{
			SetCurrentFrame( 0 );
		}
		
		SI32 Length = 20;
		SI32 TempX = GetPositionX(); 
	
		SetPositionX(GetPositionX() - (rand()%10 + 5));

		TempX = TempX - GetPositionX();

		SI32 StepY = (SI32)(sqrt((float)Length*(float)Length - (float)TempX * (float)TempX));

		SetPositionY(GetPositionY() + StepY);


		if( GetPositionX() < 0 || GetPositionY() > 800)
		{
			SetEffectLive( FALSE );
		}
	}
}

void CFeastEffectOne::Draw()
{
	if( GetEffectLive() )
	{
		if( GetEffectSpr() )
		{
			int x = ((cltClient*)pclClient)->GetScreenRX() + GetPositionX(); 


			int y = ((cltClient*)pclClient)->GetScreenRY() + GetPositionY(); 

			if( GetDrawType() == NORMAL_DRAW )
				GP.PutSpr(GetEffectSpr(),x, y, GetCurrentFrame());
			else if( GetDrawType() == EFFECT_DRAW )
				GP.PutSprAdd(GetEffectSpr(),x, y, GetCurrentFrame());
			else
				return;
		}
	}
}

CFeastEffectTwo::CFeastEffectTwo(SI32 siKind) : CFeastEffect()
{
	CreateEffect(siKind);
}

CFeastEffectTwo::~CFeastEffectTwo()
{
}

void CFeastEffectTwo::CreateEffect(SI32 siKind)
{
	if(siKind < 0 || siKind > 4)
		return;

	switch(siKind)
	{
	case 0:
		{
			GP.LoadSprFromFile(EFFECT_KIND_3, GetEffectSpr());

			SetDrawType( EFFECT_DRAW );
			SetEffectLive( TRUE );

		}
		break;
	case 1:
		{
			GP.LoadSprFromFile(EFFECT_KIND_4, GetEffectSpr());

			SetDrawType( NORMAL_DRAW );
			SetEffectLive( TRUE );
		}
		break;
	case 2:
		{
			GP.LoadSprFromFile(EFFECT_KIND_5, GetEffectSpr());

			SetDrawType( EFFECT_DRAW );
			SetEffectLive( TRUE );

		}
		break;
	case 3:
		{
			GP.LoadSprFromFile(EFFECT_KIND_6, GetEffectSpr());

			SetDrawType( EFFECT_DRAW );
			SetEffectLive( TRUE );

		}
		break;
	case 4:
		{
			GP.LoadSprFromFile( EFFECT_KIND_7, GetEffectSpr() );

			SetDrawType( EFFECT_DRAW );
			SetEffectLive( TRUE );

		}
		break;
	}
}

void CFeastEffectTwo::Action()
{
	if( GetEffectLive() )
	{
		SetCurrentFrame(GetCurrentFrame() + 1);

		if( GetCurrentFrame() == GetEffectSpr()->GetImageNum() )
		{
			SetEffectLive( FALSE );
		}
	}
}

void CFeastEffectTwo::Draw()
{
	if( GetEffectLive() )
	{
		if( GetEffectSpr() )
		{
			int x = ((cltClient*)pclClient)->GetScreenRX() + GetPositionX(); 


			int y = ((cltClient*)pclClient)->GetScreenRY() + GetPositionY(); 

			if( GetDrawType() == NORMAL_DRAW )
				GP.PutSpr(GetEffectSpr(),x, y, GetCurrentFrame());
			else if( GetDrawType() == EFFECT_DRAW )
				GP.PutSprAdd(GetEffectSpr(), x, y, GetCurrentFrame());
			else
				return;
		}
	}
}

CFeastEffectThree::CFeastEffectThree(SI32 siKind) : CFeastEffect()
{
	CreateEffect(siKind);
}

CFeastEffectThree::~CFeastEffectThree()
{
}

void CFeastEffectThree::CreateEffect(SI32 siKind)
{
	if(siKind < 0 || siKind > 5)
		return;

	switch(siKind)
	{
	case 0:
		{
			GP.LoadSprFromFile( EFFECT_KIND_8, GetEffectSpr() );

			SetDrawType( NORMAL_DRAW );
			SetEffectLive( TRUE );

		}
		break;
	case 1:
		{
			GP.LoadSprFromFile( EFFECT_KIND_9, GetEffectSpr() );

			SetDrawType( NORMAL_DRAW );
			SetEffectLive( TRUE );
		}
		break;
	case 2:
		{
			GP.LoadSprFromFile( EFFECT_KIND_10, GetEffectSpr() );

			SetDrawType( NORMAL_DRAW );
			SetEffectLive( TRUE );

		}
		break;
	case 3:
		{
			GP.LoadSprFromFile( EFFECT_KIND_11, GetEffectSpr() );

			SetDrawType( EFFECT_DRAW );
			SetEffectLive( TRUE );

		}
		break;
	case 4:
		{
			GP.LoadSprFromFile( EFFECT_KIND_12, GetEffectSpr() );

			SetDrawType( EFFECT_DRAW );
			SetEffectLive( TRUE );

		}
		break;
	case 5:
		{
			GP.LoadSprFromFile( EFFECT_KIND_13, GetEffectSpr() );

			SetDrawType( EFFECT_DRAW );
			SetEffectLive( TRUE );

		}
		break;
	}
}

void CFeastEffectThree::Action()
{
	if( GetEffectLive() )
	{
		SetCurrentFrame(GetCurrentFrame() + 1);
		
		if( GetCurrentFrame() == GetEffectSpr()->GetImageNum() )
		{
			SetEffectLive( FALSE );
		}
	}
}

void CFeastEffectThree::Draw()
{
	if( GetEffectLive() )
	{
		if( GetEffectSpr() )
		{
			int x = ((cltClient*)pclClient)->GetScreenRX() + GetPositionX(); 


			int y = ((cltClient*)pclClient)->GetScreenRY() + GetPositionY(); 

			if( GetDrawType() == NORMAL_DRAW )
				GP.PutSpr(GetEffectSpr(), x, y, GetCurrentFrame());
			else if( GetDrawType() == EFFECT_DRAW )
				GP.PutSprAdd(GetEffectSpr(), x, y, GetCurrentFrame());
			else
				return;
		}
	}
}