#include "AttackEffectMgr.h"
#include "AttackEffect.h"

CAttackEffectMgr	g_AttackEffectMgr;

CAttackEffectMgr::CAttackEffectMgr()
{
	m_pFirst = NULL;
	m_pLast = NULL;

	m_siAttackEffectCount = 0;

	Initialize();
}

CAttackEffectMgr::~CAttackEffectMgr()
{
	Destroy();
}


void CAttackEffectMgr::Initialize()
{
	return;
}

void CAttackEffectMgr::Destroy()
{
	CAttackEffect *pAttackEffect = m_pFirst;

	while( pAttackEffect )
	{
		m_pFirst = m_pFirst->GetNext();
		pAttackEffect->Destroy();
		delete pAttackEffect;
		pAttackEffect = NULL;
		pAttackEffect = m_pFirst;
	}

	m_pFirst = NULL;
	m_pLast = NULL;

	return;
}

void CAttackEffectMgr::SetDamageData( SI32 si32XCenterPos, SI32 si32YCenterPos, SI32 si32HitDamage, bool bSpecialAttack, bool bMissHit, bool bDodgeHit, UI08 uiType )
{
	CAttackEffect *pAttackEffect = new CAttackEffect();
	pAttackEffect->SetData( si32XCenterPos, si32YCenterPos, si32HitDamage, bSpecialAttack, bMissHit, bDodgeHit, uiType );
	PushAttackEffect( pAttackEffect );
	
	return;
}

void CAttackEffectMgr::Action()
{
	CAttackEffect *pAttackEffect = m_pFirst;
	CAttackEffect *pTemp = NULL;

	while ( pAttackEffect )
	{
		if ( pTemp = pAttackEffect->Action() )
		{	
			pAttackEffect = pAttackEffect->GetNext();
			DelAttackEffect( pTemp );
		}
		else
		{
			pAttackEffect = pAttackEffect->GetNext();
		}
	}

	return;
}


void CAttackEffectMgr::Draw( LPDIRECTDRAWSURFACE7 lpsurface, SI32 siAbsoluteX, SI32 siAbsoluteY, SI32 siRelativeX, SI32 siRelativeY )
{
	CAttackEffect *pAttackEffect = m_pFirst;

	while ( pAttackEffect )
	{
		if( lpsurface )pAttackEffect->Draw( lpsurface, siAbsoluteX, siAbsoluteY, siRelativeX, siRelativeY );
		pAttackEffect = pAttackEffect->GetNext();
	}

	return;
}

void CAttackEffectMgr::PushAttackEffect( CAttackEffect *pAttackEffect )
{
	if ( m_pFirst )
	{
		m_pLast->SetNext( pAttackEffect );
		pAttackEffect->SetPre( m_pLast );
		m_pLast = pAttackEffect;
	}
	else
	{
		m_pFirst = pAttackEffect;
		m_pLast = pAttackEffect;
	}

	++m_siAttackEffectCount;

	return;
}

void CAttackEffectMgr::DelAttackEffect( CAttackEffect *pAttackEffect )
{
	if ( pAttackEffect == m_pFirst )
	{
		if ( m_pFirst->GetNext() )
		{
			m_pFirst->GetNext()->SetPre( NULL );
			m_pFirst = m_pFirst->GetNext();
			delete pAttackEffect;
			pAttackEffect = NULL;
		}
		else
		{
			delete pAttackEffect;
			pAttackEffect = NULL;
			m_pFirst = NULL;
			m_pLast = NULL;
		}
	}
	else if ( pAttackEffect == m_pLast )
	{
		m_pLast->GetPre()->SetNext( NULL );
		m_pLast = m_pLast->GetPre();
		delete pAttackEffect;
		pAttackEffect = NULL;
	}
	else
	{
		pAttackEffect->GetNext()->SetPre( pAttackEffect->GetPre() );
		pAttackEffect->GetPre()->SetNext( pAttackEffect->GetNext() );
		delete pAttackEffect;
		pAttackEffect = NULL;
	}

	return;
}

void CAttackEffectMgr::DelFirstAttackEffect()
{
	CAttackEffect *pAttackEffect = m_pFirst;

	if ( pAttackEffect )
	{
		if ( pAttackEffect->GetNext() )
		{
			m_pFirst = pAttackEffect->GetNext();
			delete pAttackEffect;
			pAttackEffect = NULL;
		}
		else
		{
			m_pFirst = NULL;
			m_pLast = NULL;
			delete pAttackEffect;
			pAttackEffect = NULL;
		}
	}
	
	--m_siAttackEffectCount;

	return;
}