#ifndef _ATTACKEFFECT_H_
#define _ATTACKEFFECT_H_

#include <Directives.h>
#include <ddraw.h>
#include "..\GlobalImgFile\GlobalImgFile.h"

#define	ATTACK_EFFECT_FRAME_NUM					10

#define	ATTACK_EFFECT_WIDTH						200					
#define	ATTACK_EFFECT_HEIGHT					200	

class CAttackEffect
{
public:
	CAttackEffect();
	~CAttackEffect();

	void			Initialize();
	void			Destroy();


	bool			IsAttackStatus();

	CAttackEffect	*GetPre();
	void			SetPre( CAttackEffect *pPre );

	CAttackEffect	*GetNext();
	void			SetNext( CAttackEffect *pNext );

	void			SetData( SI32 si32CenterXPos, SI32 si32CenterYPos, SI32 si32HitDamage, bool bSpecialAttack, bool bMissHit, bool bDodgeHit, UI08 uiType );
	

	CAttackEffect*	Action();
	void			Draw( LPDIRECTDRAWSURFACE7 lpsurface, SI32 siAbsoluteX, SI32 siAbsoluteY, SI32 siRelativeX, SI32 siRelativeY );

private:
	CAttackEffect				*m_pPre;
	CAttackEffect				*m_pNext;

	SI32						m_si32CenterXPos;
	SI32						m_si32CenterYPos;
	SI32						m_si32HitDamage;
	bool						m_bSpecialAttack;
	bool						m_bMissHit;
	bool						m_bDodgeHit;
	UI08						m_uiType;

	SI32						m_siEffectCount;
	SI32						m_si32MoveToYPos;

	// 새로운 이펙트 출력을 위한 변수들
	TSpr*						m_HitSpr[10];
	TSpr*						m_MissSpr;
	TSpr*						m_DodgeSpr;
	TSpr*						m_CriticalSpr;
	SI32						m_siCurFont;
	clock_t						m_delayClock;
	size_t						m_strLen;
	size_t						m_strWidth;
	SI32						m_siDigitIndex[6];
	// 


	LPDIRECTDRAWSURFACE7		m_lpSurface;


};

#endif