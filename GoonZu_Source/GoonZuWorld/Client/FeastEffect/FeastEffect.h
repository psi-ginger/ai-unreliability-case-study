#ifndef _FEASTEFFECT_H
#define _FEASTEFFECT_H

#include <Directives.h>
#include "../lib/ngraphics/graphics.h"
#include "TSpr.h"

// 하늘에서 떨어지는것
#define EFFECT_KIND_0	TEXT("FeastEffect/FeastEffect_Flower.SPR")
#define EFFECT_KIND_1	TEXT("FeastEffect/FeastEffect_Petal.SPR")
#define EFFECT_KIND_2	TEXT("FeastEffect/FeastEffect_Ribbon.SPR")

// 그냥 혼자서 뿌려지는것
#define EFFECT_KIND_3	TEXT("FeastEffect/FeastEffect_Lovefire.SPR") 
#define EFFECT_KIND_4	TEXT("FeastEffect/FeastEffect_Clap.SPR")
#define EFFECT_KIND_5	TEXT("FeastEffect/FeastEffect_Spark.SPR")
#define EFFECT_KIND_6	TEXT("FeastEffect/FeastEffect_UpperSpark.SPR")
#define EFFECT_KIND_7	TEXT("FeastEffect/FeastEffect_Lovefire2.SPR")

// 2개가 하나 되는것
#define EFFECT_KIND_8	TEXT("FeastEffect/FeastEffect_Gourd.SPR")
#define EFFECT_KIND_9	TEXT("FeastEffect/FeastEffect_Box.SPR")
#define EFFECT_KIND_10	TEXT("FeastEffect/FeastEffect_firecracker.SPR")
#define EFFECT_KIND_11	TEXT("FeastEffect/FeastEffect_GourdSpark.SPR")
#define EFFECT_KIND_12	TEXT("FeastEffect/FeastEffect_BoxFire.SPR")
#define EFFECT_KIND_13	TEXT("FeastEffect/FeastEffect_firecrackerSpark.SPR")

#define NORMAL_DRAW	0
#define EFFECT_DRAW 1

class CFeastEffect
{
private:

	TSpr*	m_pSprFeastEffect;

    SI32		m_siPositionX;			// X위치값
	SI32		m_siPositionY;			// Y위치값
	SI32		m_siDrawType;			// 0: 그냥 찍기 1: 효과로 찍기
	SI32		m_siCurrentFrame;
	bool		m_bEffectLive;

public:

	CFeastEffect();
	virtual ~CFeastEffect();

//	CFeastEffect(const CFeastEffect& aFeastEffect);
//	CFeastEffect& operator=(const CFeastEffect& aFeastEffect);

	void	Init();
	void	DeleteEffect();

	void	SetPositionX(SI32 siX)			{ m_siPositionX = siX;		 }
	void	SetPositionY(SI32 siY)			{ m_siPositionY = siY;		 }
	void	SetDrawType(SI32 siType)		{ m_siDrawType  = siType;    }
	void	SetCurrentFrame(SI32 siCount)   { m_siCurrentFrame = siCount;}
	void	SetEffectLive(bool bLive)		{ m_bEffectLive = bLive;	 }

	TSpr* GetEffectSpr()					{ return m_pSprFeastEffect; }

	SI32	GetPositionX()					{ return m_siPositionX;		}
	SI32	GetPositionY()					{ return m_siPositionY;		}
	SI32	GetDrawType()					{ return m_siDrawType;		}
	SI32	GetCurrentFrame()				{ return m_siCurrentFrame;  }
	bool	GetEffectLive()					{ return m_bEffectLive;		}
	
	virtual void	CreateEffect(SI32 siKind) = 0;
	virtual void	Action() = 0;
	virtual void	Draw() = 0;
};

class CFeastEffectOne : public CFeastEffect
{
private:
	SI32	siStepX;
	SI32	siStepY;
	SI32	siCount;

public:

	CFeastEffectOne(SI32 siKind);
	~CFeastEffectOne();

	virtual void CreateEffect(SI32 siKind);
	virtual void Action();
	virtual void Draw();
};

class CFeastEffectTwo : public CFeastEffect
{
public:

	CFeastEffectTwo(SI32 siKind);
	~CFeastEffectTwo();

	virtual void CreateEffect(SI32 siKind);
	virtual void Action();
	virtual void Draw();
};

class CFeastEffectThree : public CFeastEffect
{

public:

	CFeastEffectThree(SI32 siKind);
	~CFeastEffectThree();

	virtual void CreateEffect(SI32 siKind);
	virtual void Action();
	virtual void Draw();
};
#endif 