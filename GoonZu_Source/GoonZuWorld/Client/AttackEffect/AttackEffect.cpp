#include "AttackEffect.h"

#include <CommonLogic.h>

#include "..\client.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

extern cltCommonLogic* pclClient;


//LPDIRECTDRAWSURFACE7 CAttackEffect::m_lpSurface = NULL;


CAttackEffect::CAttackEffect()
{
	m_lpSurface = NULL;

	Initialize();

	if( m_lpSurface == NULL ) {

		if ( g_graphics.CreateSurface16( &m_lpSurface, ATTACK_EFFECT_WIDTH, ATTACK_EFFECT_HEIGHT, TRUE, RGB16_TRANS ) == FALSE )
		{			
			 //MsgBox(TEXT("CreateOffScreen Error, CAttackEffect "), TEXT("fwf"));
		}
	}
}

CAttackEffect::~CAttackEffect()
{
	m_pPre = NULL;
	m_pNext = NULL;

	Destroy();

}

void CAttackEffect::Initialize()
{
	m_pPre = NULL;
	m_pNext = NULL;

	m_si32CenterXPos = 0;
	m_si32CenterYPos = 0;
	m_si32HitDamage  = 0;
	m_bSpecialAttack = false;

	m_siEffectCount = 0;
	m_si32MoveToYPos = 0;

	m_bMissHit = false;
	m_uiType = 0;

	for ( SI32 i=0; i<10; ++i )
	{
		m_HitSpr[i] = NULL;
	}

	m_MissSpr = NULL;
	m_CriticalSpr = NULL;

	m_siCurFont = 0;
	m_delayClock = 0;

	return;
}

void CAttackEffect::Destroy()
{
	m_pPre = NULL;
	m_pNext = NULL;

	if( m_lpSurface )
	{
		m_lpSurface->Release();
		m_lpSurface = NULL;
	}

	return;
}

CAttackEffect* CAttackEffect::GetPre()
{
	return m_pPre;
}

void CAttackEffect::SetPre( CAttackEffect *pPre )
{
	m_pPre = pPre;
	return;
}

CAttackEffect* CAttackEffect::GetNext()
{
	return m_pNext;
}

void CAttackEffect::SetNext( CAttackEffect *pNext )
{
	m_pNext = pNext;
	return;
}

void CAttackEffect::SetData( SI32 si32CenterXPos, SI32 si32CenterYPos, SI32 si32HitDamage, bool bSpecialAttack, bool bMissHit, bool bDodgeHit, UI08 uiType )
{
	m_si32CenterXPos = si32CenterXPos;
	m_si32CenterYPos = si32CenterYPos;
	m_si32HitDamage  = si32HitDamage;
	m_bSpecialAttack = bSpecialAttack;
	m_bMissHit = bMissHit;
	m_bDodgeHit = bDodgeHit;
	m_si32MoveToYPos = 0;
	m_uiType = uiType;

	if ( pclClient->IsCountrySwitch( Switch_NewHitEffect ) )
	{
		for ( SI32 i=0; i<10; ++i )
		{
			m_HitSpr[i] = uiType ? pclClient->GetGlobalSpr( GIMG_DAMAGE_PC_0 + i ) : pclClient->GetGlobalSpr( GIMG_DAMAGE_NPC_0 + i );
		}
		m_MissSpr = uiType ? pclClient->GetGlobalSpr( GIMG_DAMAGE_PC_MISS ) : pclClient->GetGlobalSpr( GIMG_DAMAGE_NPC_MISS ) ;
		m_DodgeSpr = uiType ? pclClient->GetGlobalSpr( GIMG_DAMAGE_PC_DODGE ) : pclClient->GetGlobalSpr( GIMG_DAMAGE_NPC_DODGE ) ;
		m_CriticalSpr = pclClient->GetGlobalSpr( GIMG_DAMAGE_CRITICAL );

		m_delayClock = pclClient->CurrentClock;

		TCHAR strNum[ 10 ]= TEXT("");
		_itot( si32HitDamage, strNum, 10 );
		m_strLen = _tcslen( strNum );

		// 전체 길이같은건 미리 다계산 해서 넣어놓자...
		switch( m_strLen ) 
		{
		case 1:
			m_strWidth = 12;
			m_siDigitIndex[0] = m_si32HitDamage;
			break;
		case 2:
			m_strWidth = 24;
			m_siDigitIndex[0] = m_si32HitDamage / 10;
			m_siDigitIndex[1] = m_si32HitDamage % 10;
			break;
		case 3:
			m_strWidth = 36;
			m_siDigitIndex[0] = m_si32HitDamage / 100;
			m_siDigitIndex[1] = m_si32HitDamage % 100 / 10;
			m_siDigitIndex[2] = m_si32HitDamage % 10;
			break;
		case 4:
			m_strWidth = 48;
			m_siDigitIndex[0] = m_si32HitDamage / 1000;
			m_siDigitIndex[1] = m_si32HitDamage % 1000 / 100;
			m_siDigitIndex[2] = m_si32HitDamage % 100 / 10;
			m_siDigitIndex[3] = m_si32HitDamage % 10;
			break;
		case 5:
			m_strWidth = 60;
			m_siDigitIndex[0] = m_si32HitDamage / 10000;
			m_siDigitIndex[1] = m_si32HitDamage % 10000 / 1000;
			m_siDigitIndex[2] = m_si32HitDamage % 1000 / 100;
			m_siDigitIndex[3] = m_si32HitDamage % 100 / 10;
			m_siDigitIndex[4] = m_si32HitDamage % 10;
			break;
		case 6:
			m_strWidth = 72;
			m_siDigitIndex[0] = m_si32HitDamage / 100000;
			m_siDigitIndex[1] = m_si32HitDamage % 100000 / 10000;
			m_siDigitIndex[2] = m_si32HitDamage % 10000 / 1000;
			m_siDigitIndex[3] = m_si32HitDamage % 1000 / 100;
			m_siDigitIndex[4] = m_si32HitDamage % 100 / 10;
			m_siDigitIndex[5] = m_si32HitDamage % 10;
			break;
		}
	}

	return;
}

CAttackEffect* CAttackEffect::Action()
{
	if ( pclClient->IsCountrySwitch( Switch_NewHitEffect ) )
	{
		if ( m_siCurFont >= ATTACK_EFFECT_FRAME_NUM - 1 )
		{
			return this;
		}

		if(TABS(pclClient->CurrentClock - m_delayClock) > 100 )  // 초당 10프레임
		{
			m_delayClock = pclClient->CurrentClock;
			++m_siCurFont;
		}

		return NULL;
	}
	else
	{
		SI32 siFrameDelta = 1;
		if(pclClient->GetFrameDelay() == 25)	siFrameDelta = 2;
		else									siFrameDelta = 1;

		if ( m_siEffectCount > ATTACK_EFFECT_FRAME_NUM * siFrameDelta )
		{
			m_siEffectCount = 0;
			m_si32MoveToYPos = 0;
			return this;
		}
		else
		{

			if(m_bSpecialAttack == true)	{	m_siEffectCount +=2;	m_si32MoveToYPos += 3; }
			else							{	++m_siEffectCount;		m_si32MoveToYPos += 2; }

		}
	}
	
	return NULL;
}

void CAttackEffect::Draw( LPDIRECTDRAWSURFACE7 lpsurface, SI32 siAbsoluteX, SI32 siAbsoluteY, SI32 siRelativeX, SI32 siRelativeY )
{

	if( m_lpSurface == NULL ) return;

	if ( pclClient->IsCountrySwitch( Switch_NewHitEffect ) )
	{
		if ( lpsurface == NULL  ) return;


		SI32 siScreenPrintX = siRelativeX - ( siAbsoluteX - m_si32CenterXPos );
		SI32 siScreenPrintY = siRelativeY - ( siAbsoluteY - m_si32CenterYPos );


		//테스트
		if ( GP.LockSurface( lpsurface ) == TRUE )	
		{
			if ( m_bMissHit )
			{
				if ( m_MissSpr )
				{
					GP.PutSpr( m_MissSpr, siScreenPrintX - (m_MissSpr->GetXSize() / 2), siScreenPrintY - m_MissSpr->GetYSize(), m_siCurFont );
				}
			}
			else if ( m_bDodgeHit )
			{
				if ( m_DodgeSpr )
				{
					GP.PutSpr( m_DodgeSpr, siScreenPrintX - (m_DodgeSpr->GetXSize() / 2), siScreenPrintY - m_DodgeSpr->GetYSize(), m_siCurFont );
				}
			}
			else
			{
				if ( m_bSpecialAttack )
				{
					if ( m_siCurFont >= 1 && m_siCurFont <=4 && m_CriticalSpr )
					{
						GP.PutSpr( m_CriticalSpr, siScreenPrintX - (m_CriticalSpr->GetXSize()/2), siScreenPrintY - m_CriticalSpr->GetYSize(), m_siCurFont - 1 );
					}
				}


				siScreenPrintX -= m_strWidth + 24;
				if ( m_HitSpr[0] )
				{
					siScreenPrintY -= m_HitSpr[0]->GetYSize();
				}

				SI32 i=0;
				while ( i < 6 && i < (SI32)m_strLen )
				{
					if ( m_HitSpr[m_siDigitIndex[i]] )
					{
						GP.PutSpr( m_HitSpr[m_siDigitIndex[i]], siScreenPrintX, siScreenPrintY, m_siCurFont );
						siScreenPrintX += 24;
						++i;
					}
				}
			}

			GP.UnlockSurface( lpsurface );
		}
	}
	else
	{
		RECT	srcRect;
		RECT	destRect;

		g_graphics.ClearSurface( m_lpSurface, RGB16_TRANS );

		if ( m_bMissHit || m_bDodgeHit )
		{
			TSpr *pXSPR = pclClient->GetGlobalSpr( GIMG_MISSHIT );

			if ( GP.LockSurface( m_lpSurface ) == TRUE )	
			{
				GP.PutSpr( pXSPR, 0, 0,  m_uiType );
				SetRect( &srcRect, 0, 0, pXSPR->GetXSize(), pXSPR->GetYSize() );

				GP.UnlockSurface( m_lpSurface );
			}

			SI32 siScreenPrintX = siRelativeX - ( siAbsoluteX - m_si32CenterXPos );
			SI32 siScreenPrintY = siRelativeY - ( siAbsoluteY - m_si32CenterYPos );

			SetRect( &destRect,	siScreenPrintX - ( pXSPR->GetXSize() ) / 2, siScreenPrintY - ( pXSPR->GetYSize() ) - m_si32MoveToYPos, 
				siScreenPrintX + ( pXSPR->GetXSize() ) / 2, siScreenPrintY + ( pXSPR->GetYSize() / 2 ) - m_si32MoveToYPos );
		}
		else
		{
			TSpr *pXSPR = pclClient->GetGlobalSpr( GIMG_DMGCOUNTER );

			TCHAR strNum[ 10 ]= TEXT("");
			_itot( m_si32HitDamage, strNum, 10 );

			SI32 si32StrLength = _tcslen( strNum );
			SI32 addFont = ( m_uiType * 10);

			if ( GP.LockSurface( m_lpSurface ) == TRUE )	
			{

				switch( si32StrLength )
				{
				case 1:
					{
						GP.PutSpr(pXSPR, 0, 0,  m_si32HitDamage + addFont );
						SetRect( &srcRect, 0, 0, pXSPR->GetXSize(), pXSPR->GetYSize() );
					}
					break;

				case 2:
					{
						GP.PutSpr( pXSPR,0, 0,  m_si32HitDamage/10 + addFont );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() - 2, 0, m_si32HitDamage%10 + addFont );
						SetRect( &srcRect, 0, 0, pXSPR->GetXSize() * 2 - 2, pXSPR->GetYSize() );
					}
					break;

				case 3:
					{
						GP.PutSpr( pXSPR,0, 0,  m_si32HitDamage/100 + addFont );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() - 2 , 0,  (m_si32HitDamage/10)%10 + addFont );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() * 2 - 4, 0,  m_si32HitDamage%10 + addFont );
						SetRect( &srcRect, 0, 0, pXSPR->GetXSize() * 3 - 4, pXSPR->GetYSize() );
					}
					break;

				case 4:
					{
						GP.PutSpr( pXSPR,0, 0,  m_si32HitDamage/1000 + addFont  );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() - 2, 0,  (m_si32HitDamage/100)%10 + addFont );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() * 2 - 4, 0,  (m_si32HitDamage%100)/10 + addFont );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() * 3 - 6, 0, m_si32HitDamage%10 + addFont );
						SetRect( &srcRect, 0, 0, pXSPR->GetXSize() * 4 - 6, pXSPR->GetYSize() );
					}
					break;
				case 5:
					{
						GP.PutSpr( pXSPR,0, 0,  m_si32HitDamage/10000 + addFont  );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() - 2, 0,  (m_si32HitDamage/1000)%10 + addFont );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() * 2 - 4, 0,  (m_si32HitDamage%1000)/100 + addFont );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() * 3 - 6, 0, (m_si32HitDamage%100)/10 + addFont );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() * 4 - 8, 0, m_si32HitDamage%10 + addFont );
						SetRect( &srcRect, 0, 0, pXSPR->GetXSize() * 5 - 8, pXSPR->GetYSize() );

					}
					break;
				case 6:
					{
						GP.PutSpr( pXSPR,0, 0,  m_si32HitDamage/100000 + addFont );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() - 2, 0,  (m_si32HitDamage/10000)%10 + addFont );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() * 2 - 4, 0,  (m_si32HitDamage%10000)/1000 + addFont );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() * 3 - 6, 0, (m_si32HitDamage%1000)/100 + addFont  );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() * 4 - 8, 0, (m_si32HitDamage%100)/10 + addFont );
						GP.PutSpr( pXSPR,pXSPR->GetXSize() * 5 - 10, 0, m_si32HitDamage%10 + addFont );
						SetRect( &srcRect, 0, 0, pXSPR->GetXSize() * 6 - 10, pXSPR->GetYSize() );

					}
					break;
				}

				GP.UnlockSurface( m_lpSurface );
			}


			SI32 siScreenPrintX = siRelativeX - ( siAbsoluteX - m_si32CenterXPos );
			SI32 siScreenPrintY = siRelativeY - ( siAbsoluteY - m_si32CenterYPos );

			// CriticalHit 경우 
			if(m_bSpecialAttack == true)
			{
				// left, top, right, bottom
				SI32 siMultiplier = (100+m_siEffectCount*2);
				SetRect( &destRect, 
					siScreenPrintX - (( pXSPR->GetXSize() * si32StrLength + 2 * ( si32StrLength - 1 ) ) / 2) * siMultiplier /100, 
					siScreenPrintY - (( pXSPR->GetYSize() / 2 ) + 1 * m_si32MoveToYPos )					   * siMultiplier /100,  
					siScreenPrintX + (( pXSPR->GetXSize() * si32StrLength  + 2 * ( si32StrLength - 1 ) ) / 2)* siMultiplier /100, 
					siScreenPrintY + (( pXSPR->GetYSize() / 2 ) - 1 * m_si32MoveToYPos )					   * siMultiplier /100);
			}	
			else
			{
				SetRect( &destRect, 
					siScreenPrintX - ( pXSPR->GetXSize() * si32StrLength - 2 * ( si32StrLength - 1 ) ) / 2, 
					siScreenPrintY - ( pXSPR->GetYSize() / 2 ) - m_si32MoveToYPos, 
					siScreenPrintX + ( pXSPR->GetXSize() * si32StrLength - 2 * ( si32StrLength - 1 ) ) / 2, 
					siScreenPrintY + ( pXSPR->GetYSize() / 2 ) - m_si32MoveToYPos							);
			}
		}

		cltClient* pclclient = (cltClient*)pclClient;

		if ( destRect.left < 0 || destRect.right >  pclclient->siClientScreenXsize - 1 
			|| destRect.top < 0 || destRect.bottom > pclclient->siClientScreenYsize - 1 )
		{
			return;
		}

		lpsurface->Blt( &destRect, m_lpSurface, &srcRect, DDBLT_WAIT | DDBLT_KEYSRC, NULL );

		return;
	}
}