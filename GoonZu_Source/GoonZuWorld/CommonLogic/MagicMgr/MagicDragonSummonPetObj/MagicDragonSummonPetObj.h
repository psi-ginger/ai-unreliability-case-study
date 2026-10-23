#ifndef _MAGICDRAGONSTUN_H_
#define _MAGICDRAGONSTUN_H_

#include "../MagicBaseInfo.h"
#include "../MagicBaseObj.h"

class cltMapCommon;

#define DRAGONSTUN_BULLET_NUM	13

class MagicDragonSummonPetObj : public CMagicBaseObj
{
public:
	MagicDragonSummonPetObj( CMagicMgr *pMagicMgr );
	~MagicDragonSummonPetObj();

	void	Initialize();
	void	Destroy();

	bool	SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique );	// Set
	bool	Action();										// Action
	void	Draw();											// Draw
	
	bool	Push( BYTE *pData, SI32 si32Size );				// Push
	//--------------------- 메시지 별 처리------------------
	void	SetDamage( SI32 si32Damage, SI32 si32CurrentRadius );

private:

	stMagicInfo			*m_pMagicInfo;

	SI32				m_si32DrawXPos;
	SI32				m_si32DrawYPos;

	SI32				m_si32AddDamage;
	SI32				m_si32TotalDamage;

	cltPos				m_clDotHitPos;
	cltMapCommon*		m_pclMap;

#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, DRAGONSTUN_BULLET_NUM>		m_siDrawFrame;
	NSafeTArray<SI32, DRAGONSTUN_BULLET_NUM>		m_siCannonPointX;
	NSafeTArray<SI16, DRAGONSTUN_BULLET_NUM>		m_siCannonPointY;
#else
	SI16				m_siDrawFrame[DRAGONSTUN_BULLET_NUM];
	SI32				m_siCannonPointX[DRAGONSTUN_BULLET_NUM];
	SI32				m_siCannonPointY[DRAGONSTUN_BULLET_NUM]; 
#endif

};


#endif