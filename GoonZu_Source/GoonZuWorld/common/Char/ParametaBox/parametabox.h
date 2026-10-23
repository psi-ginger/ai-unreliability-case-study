//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _PARAMETABOX_H
#define _PARAMETABOX_H

#include "../../../Lib/Directives.h"
#include "..\Health\Health.h"

#define VIT_LIFE_RATE						4			// Life = Vit * VIT_LIFE_RATE
#define INT_MANA_RATE						4			// Mana	= Int * INT_MANA_RATE

#define MAX_LEVELUP_REWARD_ITEM				2

class cltPersonInfo;
class cltKindInfo;
class cltItemManagerCommon;
class cltHorse;
class cltCharCommon;
class cltPersonHorseInfo;
class cltPItemInfo;
class cltPersonHorseInfo;
class cltPSkillInfo;
class cltSummonScroll;
class cltIdentity;
class cltItem;

class cltBasicAbility;
class cltGMCommandKindInfo;

//---------------------------------
// LevelInfo
//---------------------------------
class cltLevelInfo{
public:
	SI64		siExp;// sI32-> Si64 로 변경하면서 강제로 형변경 차후 지워야함
	SI16		siLevel;
	SI16		siBonus;
	SI16		siSkillBonus;
	
	SI32		siFameExp;		// 명성경험치. 
	SI16		siFameLevel;


	cltLevelInfo()
	{
		Init();
	}

	cltLevelInfo(SI64 exp, SI16 level, SI16 bonus, SI16 skillbonus, SI32 fameexp, SI16 famelevel)// sI32-> Si64 로 변경하면서 강제로 형변경 차후 지워야함
	{
		Set(exp, level, bonus, skillbonus, fameexp, famelevel);
	}
	
	void Init()
	{
		siExp			= 0;
		siLevel			= 0;	
		siBonus			= 0;
		siSkillBonus	= 0;
		siFameExp		= 0;
		siFameLevel		= 0;
	}

	void Set(SI64 exp, SI16 level, SI16 bonus, SI16 skillbonus, SI32 fameexp, SI32 famelevel)// sI32-> Si64 로 변경하면서 강제로 형변경 차후 지워야함
	{
		siExp			= exp;
		siLevel			= level;	
		siBonus			= bonus;
		siSkillBonus	= skillbonus;
		siFameExp		= fameexp;
		siFameLevel		= famelevel;
	}

	bool IsSame(cltLevelInfo* pclinfo)
	{
		if(pclinfo == NULL)return false;

		if(siExp		!= pclinfo->siExp)return false;
		if(siLevel		!= pclinfo->siLevel)return false;
		if(siBonus		!= pclinfo->siBonus)return false;
		if(siSkillBonus != pclinfo->siSkillBonus)return false;
		if(siFameExp	!= pclinfo->siFameExp)	return false;
		if(siFameLevel	!= pclinfo->siFameLevel)	return false;

		return true;
	}
};


//--------------------------------
// 전투시 전달되어야 할 필수 파라메타. 
//--------------------------------
class cltPIPInfoCommon : public cltBasicAbility{
public:
	// 기본 능력치.(사용자가 보너스를 가지고 올리는 능력치.) 
/*	SI32	siStr;				// 근력	
	SI32	siDex;				// 순발력
	SI32	siVit;				// 생명력 
	SI32	siMag;				// 도력
	SI32	siLuk;				// 행운 (2005-12-21추가 by cyj)
	SI32	siWis;				//KHY - 0325 - 지혜 추가.
*/

	// 레벨 정보. 
	SI16	siLevel;
	SI16	siBonus;
	SI32	Life;				// 체력 
	SI32	Mana;				// 마법
	SI64	siExp;				// SI32->SI64 경험치 오버플로우때문에 수정
	
	cltPIPInfoCommon()
	{
		Init();
	}

	void Set(cltPIPInfoCommon* pip)
	{
		siStr	= pip->siStr;				// 근력	
		siDex	= pip->siDex;				// 순발력
		siVit	= pip->siVit;				// 생명력 
		siMag	= pip->siMag;				// 도력 
		siLuk	= pip->siLuk;
		siWis	= pip->siWis;
		siHnd	= pip->siHnd;
		
		siExp	= pip->siExp;
		siLevel	= pip->siLevel;
		siBonus	= pip->siBonus;
		
		Life	= pip->Life;				// 체력 
		Mana	= pip->Mana;				// 마법력 
	}

	void Init()
	{
		siStr	= 5;
		siDex	= 5;
		siVit	= 5;
		siMag	= 5;
		siLuk	= 5;

		Life	= 5 * VIT_LIFE_RATE;
		Mana	= 5 * INT_MANA_RATE;

		// 레벨은 기본적으로 1에서부터 시작한다.
		siExp		= 0;
		siLevel		= 1;
		siBonus		= 5;
	}

	SI64 GetCurExp()const				{	return siExp;						}	// SI32->SI64 경험치 오버플로우때문에 수정
	void SetCurExp(SI64 exp)			{	siExp = max(0, exp);				}	// SI32->SI64 경험치 오버플로우때문에 수정

	SI32 GetBonus()const				{	return siBonus;						}
	void SetBonus(SI32 dat)				{	siBonus = dat;						}
	SI32 IncreaseBonus(SI32 amount)		{	siBonus+=amount; return siBonus;	}
	SI32 DecreaseBonus(SI32 amount)
	{ 
		if( siBonus > amount ) 	siBonus -= amount; 
		else					siBonus  = 0;
		
		return siBonus;
	}

	SI32 GetLevel()const				{	return siLevel;						}
	void SetLevel(SI32 dat)				{	siLevel = dat;						}
	SI32 IncreaseLevel(SI32 amount)
	{
		siLevel += amount;
		if( siLevel > MAX_CHARACTER_LEVEL )	siLevel = MAX_CHARACTER_LEVEL;

		return siLevel;
	}


	SI32 GetLife()const					{	return Life;						}
	void SetLife(SI32 life)				{	Life = life;						}
	SI32 DecreaseLife(SI32 mount)
	{
		if(Life > mount)	Life-= mount;
		else				Life = 0;

		return Life;
	}
	SI32 IncreaseLife(SI32 amount)
	{
		Life += amount;
		return Life;
	}

	SI32 GetMana()const{return Mana;}
	void SetMana(SI32 mana) 
	{ 
		Mana = mana; 
		Mana = Mana < 0 ? 0 : Mana; 
	} // [지연] 마나 음수값 나올수 없도록.
	  // 처음엔 안바뀔수도 있음 DataCopy 하게 되면 SetMana  함수를 사용하지 않을 것이므로. 나중에 마나에 액션을 주는 식으로..
	SI32 DecreaseMana(SI32 mount)
	{
		if(Mana > mount)	Mana-= mount;
		else				Mana = 0;

		return Mana;
	}
	SI32 IncreaseMana(SI32 amount)
	{
		Mana += amount;
		return Mana;
	}

	// 경험치를 올려준다. 
	BOOL IncreaseExp(SI64 exp);// SI32->SI64 경험치 오버플로우때문에 수정	
	

	// 경험치를 낮춰준다. 
	void DecreaseExp(SI32 exp)
	{
		if(siExp >= exp)siExp -= exp;
		else siExp = 0;
	}

	void SetBasicAbility(cltBasicAbility* pclba)
	{
		siStr	= pclba->siStr;
		siDex	= pclba->siDex;
		siVit	= pclba->siVit;
		siMag	= pclba->siMag;
		siLuk	= pclba->siLuk;
		siWis	= pclba->siWis;
		siHnd	= pclba->siHnd;
	}

	SI32 CalcLevelDamage();
	
	// 레벨에 따른 보상 아이템 정보를 얻는다.
	BOOL cltPIPInfoCommon::GetLevelUpRewardItem(SI32 level, SI32* pItemUnique, SI32* pItemCount, SI16* psiNotice);

	void GetGMSectionData(GMSectionData* pkGMSectionData);
};

class cltPIPInfo : public cltPIPInfoCommon{
public:
//	SI32		siHnd;				// 손재주. 
	SI16		siFameLevel;
	SI16		siSkillBonus;
	SI32		siFameExp;			// 명성경험치. 
	GMONEY		siMoney;			// 소지금.

public:

	cltPIPInfo()
	{
		Init();
	};

	~cltPIPInfo(){}

	void Init()
	{
		cltPIPInfoCommon::Init();

		siSkillBonus	= 1;
		siFameExp	= 0;
		siFameLevel	= 0;

		siMoney			= 0;

		siHnd	= 5;

	}

	void Set(cltPIPInfo* pip);
	void Set(cltPIPInfo ip);

	void Set(cltPIPInfoCommon* pip);

/*
	SI32 GetHnd()const{return siHnd;	}
	void SetHnd(SI32 dat){ siHnd = dat;}
	SI32 IncreaseHnd(SI32 amount){ siHnd += amount ; return siHnd;}
	SI32 DecreaseHnd(SI32 amount){ siHnd -= amount; return siHnd;}
*/
	SI32 GetSkillBonus()const{ return siSkillBonus;	}
	void SetSkillBonus(SI32 dat){siSkillBonus = dat;}
	SI32 IncreaseSkillBonus(SI32 amount){ siSkillBonus+=amount; return siSkillBonus;}
	SI32 DecreaseSkillBonus(SI32 amount)
	{ 
		if(siSkillBonus > amount) siSkillBonus-=amount; 
		else siSkillBonus = 0;
		return siSkillBonus;
	}


	void SetMoney(GMONEY money){siMoney	= money;}
	GMONEY GetMoney()const{return siMoney;}
	GMONEY IncreaseMoney(GMONEY money){siMoney += money; return siMoney;}
	GMONEY DecreaseMoney(GMONEY money){siMoney -= money; return siMoney;}

	// 명성 경험치를 올려준다. 
	bool IncreaseFameExp(SI32 exp);

	void SetBasicAbility(cltBasicAbility* pclba)
	{
		cltPIPInfoCommon::SetBasicAbility(pclba);
		//siHnd	= pclba->siHnd;
	}

	void GetGMSectionData(GMSectionData* pkGMSectionData);
};


//------------------------------------------
// KindInfo에서 얻어올 정보. 
//------------------------------------------
class cltBasicParametaBox{

public:
	SI16	siWeightConstant;	// 물건을 지닐 수 있는 힘을 결정하는 상수. 

	SI16	siDamageResist;	// 물리저항력 
	SI16	siMagicResist;	// 마법저항력.

	SI32	siAttackE;		//공격 속성
	SI32    siAttackR;		//공격 저항
	SI32	siDefenE;		//방어 속성
	SI32    siDefenR;		//방어 저항

	cltBasicParametaBox()
	{
		ZeroMemory(this, sizeof(cltBasicParametaBox));
	}

	void Set(SI32 weightconstant, SI32 damageresist, SI32 magicresist,SI32 AttackE, SI32 AttackR, SI32 DefenE, SI32 DefenR )
	{
		siWeightConstant	= weightconstant;
		siDamageResist		= damageresist;
		siMagicResist		= magicresist;
		siAttackE			= AttackE;
		siAttackR			= AttackR;	
		siDefenE			= DefenE;
		siDefenR			= DefenR;	

	}
	void Set(cltBasicParametaBox* pclbpb)
	{
		siWeightConstant	= pclbpb->siWeightConstant;
		siDamageResist		= pclbpb->siDamageResist;
		siMagicResist		= pclbpb->siMagicResist;
	}

	SI32 GetAttackElementType(){ return siAttackE;}
	SI32 GetAttackElementPower(){ return siAttackR;}
	SI32 GetDefenElementType(){ return siDefenE;}
	SI32 GetDefenElementPower(){ return siDefenR;}


	SI32 GetWeightConstant()const{return siWeightConstant;}

	SI32 GetDamageResist()const{return siDamageResist;}

	SI32 GetMagicResist()const{return siMagicResist;}
};

//-------------------------------------------------------
// cltParametaBox
//-------------------------------------------------------
class cltParametaBox{
private:
	UI32 uiParaActionFrame;				// 마지막으로 Action을 행한 프레임. 

	UI32 uiLastGetCriticalRateFrame;	// 마지막으로 크리티컬 공격 확률을 구한 프레임. 
	SI32 siOldLevel;					// 마지막으로 체크한 레벨. 

public:

	cltPIPInfo*				pclIP;

	cltBasicParametaBox*	pclBPB;		// KindInfo에서 값을 얻어온다. 

	cltKindInfo*			pclKI;

	cltPersonInfo*			pclCI;
	cltItemManagerCommon*	pclItemManager;
	

	SI32					siHealthStrRate;	// 건강에 의한 근력 향상률. 
	SI32					siHealthDexRate;	// 건강에 의한 순발력 향상률. 
	SI32					siHealthMagRate;	// 건강에 의한 도력 향상률. 

	SI32					siHealthHndRate;	// 건강에 의한 손재주 향상률. 
	SI32					siHealthLukRate;	// 건강에 의한 행운 향상률. 
	SI32					siHealthWisRate;	// 건강에 의한 지혜 향상률. 

	SI32					siSkillVitRate;		// 전투향상술에 의한 생명력 향상률
	SI32					siSkillMagRate;		// 전투향상술에 의한 도력 향상률


/*	SI32					siGuildStr;			// 길드 힘
	SI32					siGuildDex;			// 길드 순발
	SI32					siGuildMag;			// 길드 도력
	SI32					siGuildVit;			// 길드 체력
	SI32					siGuildHnd;			// 길드 손재주
	SI32					siGuildLuck;		// 길드 운
	SI32					siGuildWis;			// 길드 지혜*/

	SI32					siHorseStr;
	SI32					siHorseDex;
	SI32					siHorseMag;

	cltBasicAbility			clBasicBA;	// 기본 능력
	cltBasicAbility			clEquipBA;	// 장비에 의한 능력치 향상. 
	cltBasicAbility			clTotalBA;	
	cltBasicAbility			clHorseBA;
	cltBasicAbility			clGuildBA;
	cltBasicAbility			clHealthAdd;
	cltBasicAbility			clSetItemAdd;
	cltBasicAbility			clSkillAdd;
	cltBasicAbility			clbufAdd;		//[진성] 버프에 의한 스텟 증가. => 2008-6-18
	cltBasicAbility			clElixirAdd;	//[진성] 엘릭서 스텟 증가.		=> 2008-6-18
	cltBasicAbility			clHinamatsuriAdd;	// PCK : 히나마쯔리 이벤트기간에 받는 스텟 증가 (08.12.17)
	cltBasicAbility			clValentineAdd;	//[기형] 발렌타인 데이 버프 스텟 증가
	//cltBasicAbility			clHorsePartsAdd;

	cltBasicAbility			clSoulGuardAdd;	// 수호정령에의한 추가 능력치
	cltBasicAbility			clWorldChristmasTreeAdd;		// [춘기] 크리스마스 이벤트에 의한 추가 능력치

	bool					bSetTotalBA;	// TotalBA를 한번이라도 계산하면, true로 설정된다.

	//KHY - 1220 - 변신시스템.
	cltBasicAbility			clTransFormBA;	// 변신에 의한 능력치 향상. 
	SI32					siTransFormSyc; // 변신 캐릭터의  싱크로 %.

	SI32					siReservedLife;		// 예약된 체력. 

	cltHealth				clHealth;	// 건강 .

	cltGMCommandKindInfo*	pclGMCommandKindInfo;	// CMT명령어로 생성한 몬스터의 정보

//--------------------------------------------------------------------------------------
//KHY - 0326 - 새로운 공식에 따른 변수들. - 아예 분리 시켰다. - 변수 타입에 주의.
// 나중에 이것만 유지 시켜주면 된다.
//--------------------------------------------------------------------------------------
protected:

	//물리 공격력 - 이곳에서 결정되는 값이 최종 물리 데미지 값이다.
	SI16 siTotalPHysicalMinAttackDmg;		// 최종 최소 물리 데미지.
	SI16 siTotalPHysicalMaxAttackDmg;		// 최종 최대 물리 데미지.
	SI16 siEquipPHysicalMinAttackDmg;		// 장비에 따른 최소 물리 데미지. 
	SI16 siEquipPHysicalMaxAttackDmg;		// 장비에 따른 최대 물리 데미지. 
	SI16 siStrPHysicalAttackDmg;			// 근력에 따른 물리 데미지.

	//마법 공격력 - 이곳에서 결정된 값에, 각 마법에 대한 뎀쥐가 합산되어 최종 마법 뎀쥐가 나온다.
	SI16 siTotalMagicalMinAttackDmg;		// 최종 최소 마법 데미지.
	SI16 siTotalMagicalMaxAttackDmg;		// 최종 최대 마법 데미지.
	SI16 siEquipMagicalMinAttackDmg;		// 장비에 따른 최소 마법 데미지. - %
	SI16 siEquipMagicalMaxAttackDmg;		// 장비에 따른 최대 마법 데미지. - %
	//REAL32 siMagMagicalAttackDmg;			// 지력 에 따른 마법 데미지. - 필요없음.

	//물리 명중률 - Hit 시에 최종 계산 된다.
	SI32 siTotalPHysicalHitRate;			// 최종 물리 명중률.
	SI32 siPHysicalHitRateRank;				// 물리 명중등급 - Hnd 포함.
	SI32 siPHysicalHitRateLevel;			// 물리 명중 레벨 보정.
	SI32 siEquipPHysicalHitRate;			// 장비에 따른 물리 명중률. - %
	SI16 siAddPHysicalHitRate;				// 추가 물리 명중률. - NPC등

	//마법 명중률 - Hit 시에 최종 계산 된다.
	SI32 siTotalMagicalHitRate;			// 최종 마법 명중률.
	SI32 siMagicalHitRateRank;				// 마법 명중등급 - Wis 포함.
	SI32 siMagicalHitRateLevel;				// 마법 명중 레벨 보정.
	SI32 siEquipMagicalHitRate;				// 장비에 따른 마법 명중률. - %
	SI16 siAddMagicalHitRate;				// 추가 물리 명중률. - NPC등

	// 물리 방어력 
	SI32 siTotalPHysicalDefensiveRate;			// 최종 물리 방어률. - %
	SI32 siPHysicalDefensive;					// 물리 방어력.
	SI16 siEquipPHysicalDefensive;				// 장비에 따른 물리 방어력. 
	SI32 siHndPHysicalDefensive;				// 손재주에 따른 물리 방어력.
	
	// 마법 방어력 
	SI32 siTotalMagicalDefensiveRate;			// 최종 마법 방어률. - %
	SI32 siMagicalDefensive;						// 마법 방어력.
	SI16 siEquipMagicalDefensive;				// 장비에 따른 마법 방어력. - %
	SI32 siWisMagicalDefensive;				// 지혜에 따른 마법 방어력.

	//물리 회피률 - Hit 시에 최종 계산 된다.
	SI32 siTotalPHysicalDodgeRate;		// 최종 물리 회피률.
	SI32 siPHysicalDodgeRateRank;		// 물리 회피등급
	SI32 siPHysicalDodgeRateLevel;		// 물리 회피 레벨 보정.
	SI16 siEquipPHysicalDodgeRate;		// 장비에 따른 물리 회피률. - %

	//마법 회피률 - Hit 시에 최종 계산 된다.
	SI32 siTotalMagicalDodgeRate;		// 최종 마법 회피률.
	SI32 siMagicalDodgeRateRank;		// 마법 회피등급
	SI32 siMagicalDodgeRateLevel;		// 마법 회피 레벨 보정.
	SI16 siEquipMagicalDodgeRate;		// 장비에 따른 마법 회피률. -%

	//체력.(생명력)
	SI32 siTotalHitpoint;		// 최종 체력.
	SI16 siEquipHitpoint;		// 장비에 따른 체력. - %
	SI32 siVitHitpoint;		// 생명력에 따른 체력.

	//마법력.(마나)
	SI32 siTotalManapoint;		// 최종 마나.
	SI16 siEquipManapoint;		// 장비에 따른 마나. - %
	SI32 siMagManapoint;		// 지력에 따른 마나.

	//무게량 (소지품)
	SI32 siTotalWeigt;		// 소지품 무게량.
	SI16 siEquipWeigt;		// 장비에 따른 무게량. - %
	SI32 siStrWeigt;			// 힘에 따른 무게량

//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// 구 스테이터스 - 옜날 공식에만 사용된다.
//--------------------------------------------------------------------------------------

public:

	//--------------------------------
	// 방어력. 
	//--------------------------------
	SI32	siLevelAC;			// 레벨에 의한 방어력. 
	SI32	siEquipAC;			// 장비에 의한 방어력.
	SI32	siDefaultAC;
	
	SI32	siGeneralAC;		// 장수의 지휘효과에 의한 방어력. 
	DWORD	dwGeneralACClock;	// 지휘효과 설정 시간. 

	SI32	siEvolveAC;			// 진화에 의한 방어력. 
	SI32	siBufAC;			// 버프에 의한 방어력(%)
	SI32	siNamingAC;			// 명칭에 의한 방어력(%)
	//KHY - 0124 - 가챠레어- 닌자 변신 아이템.
	SI32	siTransFormAC;			// 변신에 의한 방어력(%)
	

	SI32	TotalAC;			// 전체 방어력. 

	SI32 TotalDamageResist;

	SI32 TotalMagicResist;
	


	SI32 siAttackRange, siItemAttackRange;                 // 공격 사정거리 

//---------------------
//  서브 스테이터스 
//---------------------
	SI32 MaxLife;
	SI32 MaxMana;
	SI32 MaxStamina;

	//----------------------
	// 크리티컬 공격 확률 
	//----------------------
	SI32 siCriticalRate;

	//----------------------
	// 데미지 
	//----------------------
	SI16 siStrDamage;			// 근력에 의한 데미지. 
	SI16 siStaffSkillDamage;	// 지팡이술에 의한 데미지. 
	SI16 siLevelDamage;			// 레벨에 의한 데미지 
	SI16 siTorpedoDamage;		// 어뢰에 의한 공격력 상승 효과. 

	SI32 TotalEquipMinDamage;	// 무기에 의한 데미지. 
	SI32 TotalEquipMaxDamage;

	SI32	siGeneralDamage;		// 장수의 지휘효과에 의한 데미지.
	DWORD	dwGeneralDamageClock;	// 지휘효과 설정 시간. 

	SI32 siEvolveDamage;			// 진화에 의한 데미지. 

	SI32 siBufDamage;				// 버프에 의한 데미지(%).
	SI32 siNamingDamage;			// 명칭에 의한 데미지(%).
	
	SI32 TotalMinDamage;
	SI32 TotalMaxDamage;
	
	//---------------------
	// 운
	//---------------------
	SI32 siTotalLuck;
	SI32 siEquipLuck;


	//----------------------
	// MagicDelay 줄이기
	//-----------------------
	SI32 siMagicDecDelay;


	//--------------------
	// 이동속도. 
	//--------------------
	SI32 BasicMoveSpeed;
	SI32 ShoesMoveSpeed;
	SI32 TotalMoveSpeed;
	SI32 BufMoveSpeed;
	
	SI32 GivenMoveSpeed;	// 서버로부터 주어진 이동 속도. 


	SI32 siAttackType;								// 공격 방식. 	
	DWORD dwAttackInterval;							// 공격 주기.   = 공격 속도.

	//--------------------------
	// 명중률 
	//--------------------------
	SI32 AR;
	SI32 DR;
	SI32 HitRate;
	SI32 siEquipAssistHitRate;
	SI32 siWeaponDefaultHitRate;

	//--------------------------
	// 장비에 의한 데미지 감소. 
	//--------------------------
	SI32 siEquipAssistDamageReduceRate;

	//--------------------------
	// 장비 관련 
	//--------------------------
	SI32 siArmWeaponUnique;						// 현재 장비하고 있는 무기의 유니크..

	cltShapeEquipInfo clShapeEquip;

protected:

	// 레벨에 의한 방어력 상승 효과를 구한다. 
	SI32 CalcLevelAC(SI32 level);
	
	//----------------------------------------------------------
	// 스텍(스테이터스) 계산 함수
	//----------------------------------------------------------
	void SetEquipBasicAbility(cltCharCommon* pclchar, cltItem* pclitemlist, SI32 activeweaponpos);
	void SetTransformBasicAbility(cltCharCommon* pclchar, cltItem* pclitemlist, SI32 activeweaponpos);
	void SetHorseBasicAbility(cltPersonHorseInfo* pclhorseinfo, cltCharCommon* pclchar);
	void SetGuildBasicAbility(cltCharCommon* pclchar);
	void SetHealthAddAbility(cltHealthInfo* pclhealthinfo, cltBasicAbility BasicBA);
	void SetSetItemAddAbility(cltCharCommon* pclchar, cltItem* pclitemlist);
	void SetSkillAddAbility(cltItem* pclitemlist, SI32 activeweaponpos, cltPSkillInfo* pclskillinfo, cltBasicAbility BasicBA);
	void SetElixirAddAbility(cltCharCommon* pclchar );
	void SetSoulGuardAddAbility(cltCharCommon* pclchar );

	//[진성] 버프로 인해 스탯에 적용되는 능력치. => 2008-6-18
	void SetBufStatusUp(cltCharCommon* pclchar);

	//[기형] 발렌타인 이벤트 - 버프 보상
	void SetValentineEventAbility(cltCharCommon* pclchar);

	// PCK : 히나마쯔리 이벤트 기간동안에 받는 스탯 능력치 (08.12.17)
	void SetHinamatsuriAbility(cltCharCommon* pclchar);
	//// 성웅 프리미엄 파츠를 장착하면 스태이터스 상승
	void SetPremiumPartsstatusUp(  SI32 PremiumItemUnique );
	// [춘기] 크리스마스 이벤트 - 엔펠리스 앞에 크리스마스 트리가 건설되면 1차: +5, 2차: +10의 스테이터스 상승 효과를 준다. (09.10.20)
	void SetWorldChristmasTreeAbility(cltCharCommon* pclchar);

	//-----------------------------------------------------------
	// 능력치 산출 함수. - 공격력, 방어력, 명중률,회피율
	//-----------------------------------------------------------
	void SetPhysicalAttackDemage(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc ,SI32 gamemode ,cltPSkillInfo* pclskillinfo,cltIdentity* pclidentityinfo ,bool *bSwitch, SI16 siAttackWearStoneNum); // 물리 최소, 최대 공격력 설정.
	void SetMagicalAttackDemage(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc ,SI32 gamemode ,cltPSkillInfo* pclskillinfo,cltIdentity* pclidentityinfo ,bool *bSwitch);  // 마법 최소, 최대 공격력 설정.

	void SetPhysicalHitRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc,cltIdentity* pclidentityinfo, cltPSkillInfo* pclskillinfo); // 물리 명중률 설정.
	void SetMagicalHitRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc,cltIdentity* pclidentityinfo, cltPSkillInfo* pclskillinfo);  // 마법 명중률 설정.

	void SetPhysicalDefensiveRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc ,SI32 gamemode ,cltPSkillInfo* pclskillinfo,cltIdentity* pclidentityinfo, bool *bSwitch, SI16 siDefenseWearStoneNum);// 물리 방어율 설정.
	void SetMagicalDefensiveRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc, cltPSkillInfo* pclskillinfo);  // 마법 방어율설정.

	void SetPhysicalDodgeRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc, cltPSkillInfo* pclskillinfo); // 물리 회피율 설정.
	void SetMagicalDodgeRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc, cltPSkillInfo* pclskillinfo);  // 마법 회피율 설정.

	void SetHitPoint(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos, bool bnpc, cltPSkillInfo* pclskillinfo); 	// 생명력 설정.
	void SetManaPoint(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc, cltPSkillInfo* pclskillinfo);// 마나량 설정.
	void SetWeight(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc); 	// 무게량 설정.

	void SetCritcalRate(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc,cltPSkillInfo*	pclskillinfo,cltSummonScroll*	pclsummonscroll);
	void SetEtcSetting(cltCharCommon* pclchar, cltItem* pclitemlist, SI16 activeweaponpos,bool bnpc,cltPSkillInfo*	pclskillinfo,cltSummonScroll*	pclsummonscroll, cltPersonHorseInfo* pclhorseinfo);

	//-----------------------------------------------------------

	SI16 etcPhysicalDefensiveRate(cltItem* pclitemlist, SI16 activeweaponpos,cltCharCommon* pclchar,  SI32 * DefensivePer , SI32 gamemode ,cltPSkillInfo* pclskillinfo , bool *bSwitch);
	//-----------------------------------------------------------

public:

	cltParametaBox();
	void Init(cltItemManagerCommon* pclitemmanager, cltPIPInfo* pclip, cltHealthInfo* pclhealthinfo, cltKindInfo* pclki, cltGMCommandKindInfo* pclGMki) ;


	BOOL Action(cltCharCommon* pclchar, SI32 personid, SI32 activeweaponpos, 
		cltItem*			pclitemlist,	// 장비품 아이템 리스트 . 				
		//cltPItemInfo* pclcharitem, 
		cltPersonHorseInfo* pclhorseinfo, 
		cltHealthInfo*		pclhealthinfo, 
		cltPSkillInfo*		pclskillinfo, 
		cltIdentity*		pclidentityinfo, 
		//cltSummonScroll*	pclsummonscroll,
		SI16				siSummonAttackWearStoneNum,
		SI16				siSummonDefenseWearStoneNum,
		SI32 gamemode, bool bnpc, bool bmonsterhero = false );

	
	SI32 GetTotalAC()			{return TotalAC;}
	SI32 GetTotalDamageResist()	{return TotalDamageResist;}
	SI32 GetTotalMagicResist()	{return TotalMagicResist;}
	SI32 GetTotalEquipMinDamage(){return TotalEquipMinDamage;}
	SI32 GetTotalEquipMaxDamage(){return TotalEquipMaxDamage;}
	SI32 GetTotalMoveSpeed()	{return TotalMoveSpeed;}
	SI32 GetAR()				{return AR;}
	SI32 GetDR()				{return DR;}
	SI32 GetHitRate(SI32 enemydr, bool MagicFlag = false);			
	//-----------------------------------------------------------------------------
	//KHY - 0327
	SI32 GetHitRate(cltCharCommon* pclchar, cltCharCommon* pclAttackChar, bool MagicFlag = false); // 명중률을 구한다.			
	SI32 GetDodgeRate(cltCharCommon* Enemypclchar, cltCharCommon* pclAttackChar, bool MagicFlag= false); // 회피율을 구한다.
	SI32 GetDefensiveRate(cltCharCommon* Enemypclchar,bool MagicFlag = false); // 방어률을 구한다.	

	SI32 GetClientHitRate( bool MagicFlag ); // 명중률을 구한다. - 클라이언트용이다. - 적을 자기 레벨로 설정해서 계산한다.			
	SI32 GetClientDodgeRate(bool MagicFlag ); // 회피율을 구한다.- 클라이언트용이다. - 적을 자기 레벨로 설정해서 계산한다.
	SI32 GetClientDefensiveRate(bool MagicFlag ); // 방어률을 구한다.	- 클라이언트용이다. - 적을 자기 레벨로 설정해서 계산한다.
	//-----------------------------------------------------------------------------

	SI16 GetTotalPHysicalMinAttackDmg( ){return siTotalPHysicalMinAttackDmg;}  // 최종 최소 물리 데미지.
	SI16 GetTotalPHysicalMaxAttackDmg( ){return siTotalPHysicalMaxAttackDmg;} // 최종 최대 물리 데미지.
	
	SI16 GetTotalMagicalMinAttackDmg( ){return siTotalMagicalMinAttackDmg;}  // 최종 최소 마법 데미지.
	SI16 GetTotalMagicalMaxAttackDmg( ){return siTotalMagicalMaxAttackDmg;} // 최종 최대 마법 데미지.

	//SI16 GetTotalPHysicalHitRate( ){return siTotalPHysicalHitRate;} 				 // 최종 물리 명중률.
	SI16 GetPHysicalHitRate( ){return siPHysicalHitRateRank;} 			 // 물리 명중등급 - Hnd 포함.
	SI16 GetEquipPHysicalHitRate( ){return siEquipPHysicalHitRate;}  		//  장비에 따른 물리 명중률. - %

	//SI16 GetTotalMagicalHitRate( ){return siTotalMagicalHitRate;} 			// 최종 마법 명중률.
	SI16 GetMagicalHitRate( ){return siMagicalHitRateRank;} 			// 최종 마법 명중률.
	SI16 GetEquipMagicalHitRate( ){return siEquipMagicalHitRate;} 	// 장비에 따른 마법 명중률. - %

	//SI16 GetTotalPHysicalDefensiveRate( ){return siTotalPHysicalDefensiveRate;}  // 최종 물리 방어률.
	SI16 GetPHysicalDefensive( ){return siPHysicalDefensive;}  			// 물리 방어력.
	SI16 GetEquipPHysicalDefensive( ){return siEquipPHysicalDefensive;} // 장비에 따른 물리 방어력. 

	//SI16 GetTotalMagicalDefensiveRate( ){return siTotalMagicalDefensiveRate;} 	// 최종 마법 방어률.
	SI16 GetMagicalDefensive( ){return siMagicalDefensive;} 				// 마법 방어력.
	SI16 GetEquipMagicalDefensive( ){return siEquipMagicalDefensive;} 	// 장비에 따른 마법 방어력. - %

	//SI16 GetTotalPHysicalDodgeRate( ){return siTotalPHysicalDodgeRate;} 			// 최종 물리 회피률.
	SI16 GetPHysicalDodgeRateRank( ){return siPHysicalDodgeRateRank;} 	// 물리 회피등급
	SI16 GetEquipPHysicalDodgeRate( ){return siEquipPHysicalDodgeRate;} 	// 장비에 따른 물리 회피률. - %

	//SI16 GetTotalMagicalDodgeRate( ){return siTotalMagicalDodgeRate;} 				// 최종 마법 회피률.
	SI16 GetMagicalDodgeRateRank( ){return siMagicalDodgeRateRank;} 		// 마법 회피등급
	SI16 GetsiEquipMagicalDodgeRate( ){return siEquipMagicalDodgeRate;} 	// 장비에 따른 마법 회피률. -%

	SI32 GetTotalHitpoint( ){return siTotalHitpoint;} // 최종 체력.
	SI32 GetTotalManapoint( ){return siTotalManapoint;} 	// 최종 마나.
	SI32 GetTotalWeight( ){return siTotalWeigt;} 	// 최종 마나.
	
	//-----------------------------------------------------------------------------

	SI32 GetMaxLife();	
	void SetMaxLife(SI32 life)	{MaxLife = life;}
	SI32 GetMaxMana()	;
	void SetMaxMana(SI32 mana)	{MaxMana = mana;}
	void SetMaxStamina(SI32 stamina)	{MaxStamina = stamina;}

	SI32 GetTotalMinDamage(){return TotalMinDamage;}
	SI32 GetTotalMaxDamage(){return TotalMaxDamage;}
	//SI32 GetDpower(){return Dpower;}

	//--------------------------------
	// 사거리 관련 함수 
	//--------------------------------
	SI32 GetItemAttackRange(){return siItemAttackRange;}
	void SetItemAttackRange(SI32 range){siItemAttackRange = range;}

	SI32 GetAttackRange()const{ return siAttackRange;}

	//-------------------------------
	// 공격방식 관련 함수. 
	//-------------------------------
	SI32 GetAttackType()const{return siAttackType;}
	void SetAttackType(SI32 type){siAttackType = type;}

	// 현재의 정보를 근거로 공격 속성을 파악한다. 
	void CalculateAttackType(cltCharCommon* pclchar, SI16 activeweapon, cltItem* pclitemlist);

	//----------------------------------
	// 크리티컬 공격 확률 구하기. 
	//----------------------------------
	SI32 GetCriticalRate()const{return siCriticalRate;}

	//----------------------------------------
	// 인챈트 관련 by tae-yang
	//----------------------------------------
	SI32 GetEnchantDefDamage(SI32 EnchantType, cltItem *pclItemList);
	SI32 GetEnchantAttDamage(SI32 EnchantType, cltItem *pclItemList);


	// 최소공격력, 최대 공격력을 참고하여 실질 공격력을 얻어온다. 
	SI32 CalcApower();

	// 다음 레벨로 올라가는데 필요한 경험치. 
	SI64 GetNextExp();// SI32->SI64 경험치 오버플로우때문에 수정
	// 이전 레벨에서 지금 레벨로 올라오는데 필요했던 경험치. 
	SI64 GetPrevExp();// SI32->SI64 경험치 오버플로우때문에 수정

	//------------------------
	// 마법 데미지 추가분 
	//------------------------
	SI32 GetAddMagicDamage()
	{
		return clTotalBA.GetMag()/4;
	}

	// 스테이터스가 변화되게 될경우의 데미지를 미리 계산해보는 함수
	// 유저인 경우는 2.5당 1씩 증가
	SI32 GetStrDamageIfChange(SI32 ChangeStr, bool Flag)
	{
		if( Flag )
		{
			return  (clTotalBA.GetStr() + ChangeStr) * 2 / 5;
		}
		else
			return  (clTotalBA.GetStr() + ChangeStr) / 5;
	}

	SI32 GetMagDamageIfChange(SI32 ChangeMag)
	{
		return  (clTotalBA.GetMag() + ChangeMag)/4;
	}

	// 전투 능력을 수치화해서 얻어온다. 
	SI32 GetBattlePower();
	// 전투 능력을 레벨로 변환해서 얻어온다.
	SI32 GetBattleLevel();
	SI32 GetBossBattleLevel();

	//------------------------
	// 무게 
	//------------------------
	SI32 GetMaxWeight();
	
};


//---------------------------------
// LevelInfo
//---------------------------------
class cltLevelUpReward{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_LEVELUP_REWARD_ITEM>		ItemUnique;
	NSafeTArray<SI32, MAX_LEVELUP_REWARD_ITEM>		ItemCount;
	NSafeTArray<SI16, MAX_LEVELUP_REWARD_ITEM>		siNotice;
#else
	SI32	ItemUnique[MAX_LEVELUP_REWARD_ITEM];
	SI32	ItemCount[MAX_LEVELUP_REWARD_ITEM];
	SI16	siNotice[MAX_LEVELUP_REWARD_ITEM];
#endif

	cltLevelUpReward()
	{
		Init();
	}

	void Init()
	{
		ZeroMemory( this, sizeof(cltLevelUpReward) );
	}

	bool IsSetItem(SI32 index)
	{
		if(index < 0 || index > MAX_LEVELUP_REWARD_ITEM-1)	return true;

		if(ItemUnique[index] > 0)	return true;
		if(ItemCount[index] > 0)	return true;

		return false;
	}

	bool SetItem(SI32 index, SI32 Unique, SI32 Count, SI16 Notice)
	{
		if(index < 0 || index > MAX_LEVELUP_REWARD_ITEM-1)	return false;
		
		ItemUnique[index]	= Unique;
		ItemCount[index]	= Count;
		siNotice[index]		= Notice;

		return true;
	}
};

enum EXPFORLEVEL_TYPE
{
	EXPFORLEVEL_TYPE_CHARACTOR,
	EXPFORLEVEL_TYPE_GUILD,
};

BOOL LoadExpData();

// 레벨업에 따른 보상 로딩 - 2007.02.06 아이템 - by LEEKH
BOOL LoadLevelUpReward();

// level에 필요한 경험치 값. (1부터 시작한다.)
SI64 GetRequiredExpForLevel(SI32 level, SI32 siType = EXPFORLEVEL_TYPE_CHARACTOR);// SI32->SI64 경험치 오버플로우때문에 수정

// level에 필요한 경험치 값. (1부터 시작한다.)
SI32 GetRequiredFameExpForLevel(SI32 level);

// 명성 경험치를 근거로 명성등급을 구한다. 
SI32 GetFameLevelFromExp(SI32 exp);

// 경험치를 근거로 이 경험치의 레벨을 구한다.
SI32 GetLevelFromExp(SI32 exp, SI32 siType = EXPFORLEVEL_TYPE_CHARACTOR);

// 레벨업에 따른 보상 아이템 - 2007.02.06 아이템 - by LEEKH
BOOL GetLevelUpRewardItem(SI32* pItemUnique, SI32* pItemCount, SI16* psiNotice);

#endif
