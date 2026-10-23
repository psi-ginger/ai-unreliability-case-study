#ifndef _MAGICWHIRLPOOLOBJ_H_
#define _MAGICWHIRLPOOLOBJ_H_

#include "../MagicBaseInfo.h"
#include "../MagicBaseObj.h"

#define		MOVEMENT				5				// 회오리가 움직이는 크기( X, Y 둘다 이것 사용 )
#define		MOVEMENTDELAYTIME		5				// Action이 10번 호출 될때 마다 회오리 방향이 변경됨. 그렇게 하지 않으면 계속 따라다니면서, 타격을 주게 되므로...

class cltMapCommon;

class	CMagicWhirlPoolObj : public CMagicBaseObj
{
public:
	CMagicWhirlPoolObj( CMagicMgr *pMagicMgr );
	~CMagicWhirlPoolObj();

	void	Initialize();
	void	Destroy();

	bool	SetMagic( stMagicInfo *pstMagicInfo, SI32 si32AddDamage, cltMapCommon *pclMap, SI32 si32SkillLevel, SI32 siOrganUnique );	// Set
	bool	Action();											// Action
	void	Draw();												// Draw

	
	bool	Push( BYTE *pData, SI32 si32Size );					// Push
	


	//--------------------- 메시지 별 처리------------------
	void	SetDamage( SI32 si32Damage );
	bool	GetMinMaxDamage(SI32 &Min, SI32 &Max);

private:

	stMagicInfo				*m_pMagicInfo;

	SI32					m_si32DrawXPos;
	SI32					m_si32DrawYPos;

	//SI32					m_si32CurrentFrame;
	//SI32					m_si32MaxFrameNum;
	SI32					m_si32AddDamage;
	SI32					m_si32TotalDamage;

	SI32					m_si32CurrentMagicActionFrame;

	SI32					m_si32DotStartXPos;
	SI32					m_si32DotStartYPos;

	cltMapCommon			*m_pclMap;

	SI32					m_si32MoveMentDelayTime;

	SI32					m_si32TargetDotXPos;
	SI32					m_si32TargetDotYPos;

	char					m_cBigXPos;			// 회오리가 가던 방향으로 계속 가게 하기 위해서..
	char					m_cBigYPos;
};


#endif