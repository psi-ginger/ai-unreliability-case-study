#pragma once

#include "../MagicBaseInfo.h"
#include "../MagicBaseObj.h"

class cltMapCommon;

class	CMagicSkillBookObj : public CMagicBaseObj
{
public:
	CMagicSkillBookObj( CMagicMgr *pMagicMgr );
	~CMagicSkillBookObj();

	void	Initialize();
	void	Destroy();

	bool	SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique );	// Set
	bool	Action();											// Action
	void	Draw();												// Draw
	void	SetDamage( SI32 si32Damage );

	
	bool	Push( BYTE *pData, SI32 si32Size );					// Push
	


	//--------------------- 메시지 별 처리------------------
	void	SetDamage( SI32 si32Damage, SI32 si32CurrentRadius );


private:

	stMagicInfo				*m_pMagicInfo;

	SI32					m_si32DrawXPos;
	SI32					m_si32DrawYPos;

	//SI32					m_si32CurrentFrame;
	//SI32					m_si32MaxFrameNum;
	SI32					m_si32AddDamage;
	SI32					m_si32TotalDamage;
	
	cltPos					m_clDotHitPos;
	cltMapCommon*			m_pclMap;
};


