#ifndef _ATTACKEFFECTMGR_H_
#define _ATTACKEFFECTMGR_H_

#include <Directives.h>
#include <ddraw.h>

class CAttackEffect;

class CAttackEffectMgr
{
public:
	CAttackEffectMgr();
	~CAttackEffectMgr();

	void	Initialize();
	void	Destroy();

	void	SetDamageData( SI32 si32XCenterPos, SI32 si32YCenterPos, SI32 si32HitDamage, bool bSpecialAttack, bool bMissHit, bool bDodgeHit, UI08 uiType );

	void	Action();

	void	Draw( LPDIRECTDRAWSURFACE7 lpsurface, SI32 siAbsoluteX = 0, SI32 siAbsoluteY = 0, SI32 siRelativeX = 0, SI32 siRelativeY = 0 );
	
	void	PushAttackEffect( CAttackEffect *pAttackEffect );
	void	DelFirstAttackEffect();
	void	DelAttackEffect( CAttackEffect *pAttackEffect );


private:
	CAttackEffect	*m_pFirst;
	CAttackEffect	*m_pLast;

	SI32			m_siAttackEffectCount;
};

extern	CAttackEffectMgr	g_AttackEffectMgr;

#endif