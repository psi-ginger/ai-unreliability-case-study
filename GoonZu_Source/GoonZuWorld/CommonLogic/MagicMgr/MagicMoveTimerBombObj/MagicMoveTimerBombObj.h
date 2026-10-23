#ifndef _MAGICMOVETIMERBOMBOBJ_H_
#define _MAGICMOVETIMERBOMBOBJ_H_

#include "../MagicBaseInfo.h"
#include "../MagicBaseObj.h"

class cltMapCommon;

class CMagicMoveTimerBombObj : public CMagicBaseObj
{
public:
	CMagicMoveTimerBombObj( CMagicMgr *pMagicMgr );
	~CMagicMoveTimerBombObj();

	void	Initialize();
	void	Destroy();

	bool	SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, SI32 si32SkillLevel, SI32 siOrganUnique );	// Set
	bool	Action();										// Action
	void	Draw();											// Draw

	
	bool	Push( char *pData, SI32 si32Size );				// Push
		
	//--------------------- 메시지 별 처리------------------
	void	SetDamage( SI32 si32Damage, SI32 si32CurrentRadius );
	bool	GetMinMaxDamage(SI32 &Min, SI32 &Max);
	

private:

	stMagicInfo			*m_pMagicInfo;

	SI32				m_si32DrawXPos;
	SI32				m_si32DrawYPos;
	

	SI32				m_si32AddDamage;	
	SI32				m_si32TotalDamage;

	cltPos				m_clDotHitPos;
	cltMapCommon*		m_pclMap;


};


#endif