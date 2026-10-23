#ifndef _MAGICDRAGONCANNONOBJ_H_
#define _MAGICDRAGONCANNONOBJ_H_

#include "../MagicBaseInfo.h"
#include "../MagicBaseObj.h"

class cltMapCommon;

#define DRAGONCANNON_BULLET_NUM	13

class CMagicDragonCannonObj : public CMagicBaseObj
{
public:
	CMagicDragonCannonObj( CMagicMgr *pMagicMgr );
	~CMagicDragonCannonObj();

	void	Initialize();
	void	Destroy();

	bool	SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique );	// Set
	bool	Action();										// Action
	void	Draw();											// Draw
	void	DrawDragonCannon(SI32 siRealX, SI32 siRealY, SI32 siDrawFrame);

	bool	Push( BYTE *pData, SI32 si32Size );				// Push
	//--------------------- 메시지 별 처리------------------
	void	SetDamage( SI32 si32Damage );

private:

	stMagicInfo			*m_pMagicInfo;

	SI32				m_si32DrawXPos;
	SI32				m_si32DrawYPos;

	SI32				m_si32AddDamage;
	SI32				m_si32TotalDamage;

	cltPos				m_clDotHitPos;
	cltMapCommon*		m_pclMap;

#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, DRAGONCANNON_BULLET_NUM>		m_siDrawFrame;
	NSafeTArray<SI32, DRAGONCANNON_BULLET_NUM>		m_siCannonPointX;
	NSafeTArray<SI16, DRAGONCANNON_BULLET_NUM>		m_siCannonPointY;
#else
	SI16				m_siDrawFrame[DRAGONCANNON_BULLET_NUM];
	SI32				m_siCannonPointX[DRAGONCANNON_BULLET_NUM];
	SI32				m_siCannonPointY[DRAGONCANNON_BULLET_NUM]; 
#endif

};


#endif