//---------------------------------
// 2003/8/26 김태곤
//---------------------------------

#ifndef _HORSE_H
#define _HORSE_H

#include <Directives.h>

class cltServer;


// 동물의 성별. 
#define ANIMAL_MALE				0
#define ANIMAL_FEMALE			1

#define MAX_HORSE_TYPE_INFO		40


// 말의 유니크는 horse.txt 데이터와 일치되어야 한다

#define HORSEUNIQUE_BROWN		1		// 갈색마
#define HORSEUNIQUE_BLACK		2		// 흑마
#define HORSEUNIQUE_WHITE		3		// 백마
#define HORSEUNIQUE_RED			4		// 적토마
#define HORSEUNIQUE_RUDOLF		5		// 루돌프
#define HORSEUNIQUE_TIGER		6		// 호랑이   // 호랑이	//KHY - 0911 - 호랑이 추가.
#define HORSEUNIQUE_THOUSAND	7		// 천리마
#define HORSEUNIQUE_STEEL		8		// 철기마
#define HORSEUNIQUE_WOLF		9		// 늑대		// 고기류만 먹도록 설정되어있다
#define HORSEUNIQUE_ZEBRA		10		// 얼룩말
#define HORSEUNIQUE_BEAST		11		// 구미호
#define HORSEUNIQUE_FOAL		12		// 망아지	//KHY - 0622 - 군주S 초보자용  망아지 추가.
#define HORSEUNIQUE_CAMEL		13		// 낙타
#define HORSEUNIQUE_TIGER2		14		// 군주S용 이벤트용 호랑이 //PCK : 08.04.01 추가	
#define HORSEUNIQUE_ELEPHANT	15		// 코끼리 : PCK - 08.06.12
#define HORSEUNIQUE_WHITETIGER	16		// 백호추가 : PCK - 08.08.14
#define HORSEUNIQUE_MOTORCYCLE	17		// 오토바이추가 : PCK - 08.09.02
#define HORSEUNIQUE_FIREHATCH	18		// 불해치추가 : PCK - 08.12.10
#define HORSEUNIQUE_WHITE_TEST	19		// 체험용 백마 : 손성웅-2009.03.17
#define HORSEUNIQUE_BAPHOMET	20		// 바포메트 추가 : PCK - 2009.07.30
#define HORSEUNIQUE_GRAYWOLF	21		// 회색 늑대 추가 : 기형 - 2009.08.30
#define HORSEUNIQUE_SASURI		22		// 사수리 추가 : 기형 - 2009.12.31

#define MAX_BROWN_SPEED		1500
#define MAX_BROWN_STR		3000
#define MAX_BROWN_DEX		3000
#define MAX_BROWN_MAG		3000

#define MAX_HORSE_SPEED		2000	
#define MAX_HORSE_STR		4000	
#define MAX_HORSE_DEX		4000	
#define MAX_HORSE_MAG		4000	

#define MAX_BEAST_SPEED		2000
#define MAX_BEAST_STR		5000
#define MAX_BEAST_DEX		4000
#define MAX_BEAST_MAG		5000

#define MAX_RUDOLF_STR		3500 
#define MAX_RUDOLF_DEX		3500 
#define MAX_RUDOLF_MAG		5000 

#define MAX_REDHORSE_DEX	5000

#define MAX_STEELHORSE_STR	5000

#define MAX_WOLF_SPEED		2000
#define MAX_WOLF_DEX		5500
#define MAX_WOLF_MAG		4500
#define MAX_WOLF_STR		4000

//KHY - 0622 - 군주S 초보자용  망아지 추가.
#define MAX_FOAL_SPEED		1400
#define MAX_FOAL_STR		500
#define MAX_FOAL_DEX		500
#define MAX_FOAL_MAG		500

//KHY - 0911 - 호랑이 추가.
#define MAX_TIGER_SPEED		2000
#define MAX_TIGER_STR		5500	
#define MAX_TIGER_DEX		5000
#define MAX_TIGER_MAG		4500

//PCK - 08/02/18 - 낙타 추가.
#define MAX_CAMEL_SPEED		2000
#define MAX_CAMEL_STR		4000
#define MAX_CAMEL_DEX		4000
#define MAX_CAMEL_MAG		8000

//PCK - 08.04.01 - 군주S용 이벤트호랑이 추가.
#define MAX_TIGER2_SPEED	2000
#define MAX_TIGER2_STR		5500
#define MAX_TIGER2_DEX		5000
#define MAX_TIGER2_MAG		4500

//PCK - 08.06.12 - 코끼리 추가
#define MAX_ELEPHANT_SPEED	2000
#define MAX_ELEPHANT_STR	8000
#define MAX_ELEPHANT_DEX	4000
#define MAX_ELEPHANT_MAG	4000

//PCK - 08.08.14 - 백호 추가
#define MAX_WHITETIGER_SPEED	2000
#define MAX_WHITETIGER_STR		7500
#define MAX_WHITETIGER_DEX		8000
#define MAX_WHITETIGER_MAG		7500

//PCK - 08.09.02 - 오토바이 추가 
#define MAX_MOTORCYCLE_SPEED	2000
#define MAX_MOTORCYCLE_STR		7000
#define MAX_MOTORCYCLE_DEX		7000
#define MAX_MOTORCYCLE_MAG		6000

//PCK - 08.12.10 - 불해치 추가
#define MAX_FIREHATCH_SPEED		2000
#define MAX_FIREHATCH_STR		6500
#define MAX_FIREHATCH_DEX		7000
#define MAX_FIREHATCH_MAG		6500

//PCK - 09.07.30 - 바포메트 추가
#define MAX_BAPHOMET_SPEED		2000
#define MAX_BAPHOMET_STR		7500
#define MAX_BAPHOMET_DEX		5000
#define MAX_BAPHOMET_MAG		7500

//기형 - 09.07.30 - 회색늑대 추가
#define MAX_GRAYWOLF_SPEED		2000
#define MAX_GRAYWOLF_DEX		5500
#define MAX_GRAYWOLF_MAG		4000
#define MAX_GRAYWOLF_STR		4000

//기형 - 09.07.30 - 사수리 추가
#define MAX_SASURI_SPEED		2000
#define MAX_SASURI_DEX			6000
#define MAX_SASURI_MAG			9000
#define MAX_SASURI_STR			6000

//-------------------------------------
// 말의 행위 결과. 
//-------------------------------------
#define HORSEACTION_NONE	0
#define HORSEACTION_UPDATE	1
#define HORSEACTION_DIE		200  //KHY - 1001 - 기승동물 5마리 보유 수정. - 몇번째 말이 죽는지 알기위해서// 200 + 죽은말의 인덱스값으로 반환된다.
#define HORSEATTION_TICKET	3 // 말능력향상이용권때문
#define HORSEATTION_PREMIUMPARTS	4 // 말 프리미엄 파츠 떄문에.

//----------------------------------------
// Person이 보유하는 말의 정보. 
//---------------------------------------
//KHY - 1001 - 기승동물 5마리 보유 수정. - 말 수량.
#define MAX_HORSE_PER_PERSON				5

// 말의 추가 스탯
#define MAX_HORSE_ADDSTATUS					10
// 말의 프리미엄파츠 추가 스탯 
#define MAX_HORSE_PREMIUMPARTS				30
// 말의 추가 스탯 적용 일자
#define MAX_HORSE_ADDSTATUS_LIMITDATE		360 // 30이 실제 시간 약 하루

//-------------------------------------------
// 탈 것에 의해 이펙트가 수정되야 하는 높이 값
//-------------------------------------------
#define HORSE_EFFECTHIGHT_NORMAL_SASURI		70		
#define HORSE_EFFECTHIGHT_PVP_SASURI		100

// 말이 먹고 싶다고 하는 채식 아이템의 유니크
const SI32 HorseFood_Vegetable_ItemUnique[] = 
{
	ITEMUNIQUE(3600), // 쌀
	ITEMUNIQUE(3605), // 무
	ITEMUNIQUE(3610), // 미나리. 
	ITEMUNIQUE(3615), //표고버섯.
	ITEMUNIQUE(3620), //녹두 
	ITEMUNIQUE(3625), // 감자
	ITEMUNIQUE(3630), //고구마
	ITEMUNIQUE(3635), //양파 
	ITEMUNIQUE(3640), //오이
	ITEMUNIQUE(3645), //당근
	ITEMUNIQUE(3650), // 마늘 
	ITEMUNIQUE(3655), // 콩
	ITEMUNIQUE(3660), //참깨 
	ITEMUNIQUE(3665), //밀 
	ITEMUNIQUE(16000), //소금 
	ITEMUNIQUE(3685)  //꿀
};

// 말이 먹고 싶다고 하는 육식 먹이 아이템의 유니크
const SI32 HorseFood_Meat_ItemUnique[] = 
{
	ITEMUNIQUE(3670), // 닭고기
	ITEMUNIQUE(3675), // 돼지고기
	ITEMUNIQUE(3680), // 쇠고기
	ITEMUNIQUE(16100), // 붕어
	ITEMUNIQUE(16110), // 메기
	ITEMUNIQUE(16120), // 잉어
	ITEMUNIQUE(16150), // 장어
	ITEMUNIQUE(16160), // 고등어
};

// 오토바이가 먹고 싶다고 하는 광물 먹이 아이템의 유니크
const SI32 HorseFood_Mineral_ItemUnique[] = 
{
/*	
	ITEMUNIQUE(5000), // 석탄
	ITEMUNIQUE(5010), // 철광석
	ITEMUNIQUE(5020), // 구리광석
	ITEMUNIQUE(5040), // 금광석
	ITEMUNIQUE(5050), // 은광석
	ITEMUNIQUE(5065), // 점토
	ITEMUNIQUE(5070), // 유황
	ITEMUNIQUE(5025), // 놋쇠
*/
	ITEMUNIQUE(3922), // 
	ITEMUNIQUE(3923), // 
	ITEMUNIQUE(3924), // 
	ITEMUNIQUE(3925)
};

class cltPersonHorseInfo{
public:

	bool	bAutoFoodSwitch;				// 자동으로 말먹이를 먹는가. 
	//SI16	siCurrentHorseIndex;			// 현재 타고 있는 말의 인덱스. 

	bool	siCurrentHorseRIDEHORSE;			// 현재 타고 있는지. True = 타고있다. False = 내렸다.
	SI16	siCurrentShowHorse;				// 현재 나와 있는 말의 인덱스. 

protected:
//public:
	//cltHorse clHorse;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltHorse, MAX_HORSE_PER_PERSON>		clHorse;
#else
	cltHorse clHorse[MAX_HORSE_PER_PERSON];
#endif

public:
	cltPersonHorseInfo()
	{
		Init();
	}
	void Init()
	{
		bAutoFoodSwitch		= true;
//		siCurrentHorseIndex = -1;
		siCurrentHorseRIDEHORSE = false;
		siCurrentShowHorse	= 0;


		//clHorse.Init();
		for(SI32 i = 0;i < MAX_HORSE_PER_PERSON;i++)
		{
			clHorse[i].Init();
		}
	}

	void Set(cltPersonHorseInfo* pclinfo)
	{
		bAutoFoodSwitch			= pclinfo->bAutoFoodSwitch;
		siCurrentHorseRIDEHORSE = pclinfo->siCurrentHorseRIDEHORSE;
		siCurrentShowHorse		= pclinfo->siCurrentShowHorse;

		//clHorse.Set( &pclinfo->clHorse);
		for(SI32 i = 0;i < MAX_HORSE_PER_PERSON;i++)
		{
			clHorse[i].Set( &pclinfo->clHorse[i]);
		}		
	}

	cltHorse* GetCurrentShowHorse()
	{
		//return &clHorse;
		if(siCurrentShowHorse >= 0 && siCurrentShowHorse < MAX_HORSE_PER_PERSON)
			return &clHorse[ siCurrentShowHorse ];
		else
			return NULL;
	}

	SI32 GetCurrentRideHorseUnique()
	{
		if( siCurrentHorseRIDEHORSE )
		{
			if(siCurrentShowHorse >= 0 && siCurrentShowHorse < MAX_HORSE_PER_PERSON)
				return clHorse[ siCurrentShowHorse ].siHorseUnique;
			else
				return 0;
		}
		return 0;
	}

	void SetCurrentShowHorseIndex(SI16 index)
	{
		if(index < 0)						return;
		if(index >= MAX_HORSE_PER_PERSON)	return;

		siCurrentShowHorse = index;
	}

	SI16 GetCurrentShowHorseIndex()
	{
		return siCurrentShowHorse;
	}

	void SetCurrentHorseRIDEHORSE(bool ridehorse)
	{
		siCurrentHorseRIDEHORSE = ridehorse;
	}

	bool GetCurrentHorseRIDEHORSE()
	{
		return siCurrentHorseRIDEHORSE;
	}

	cltHorse* GetHorse(SI16 index)
	{
		if(index>=0 && index<MAX_HORSE_PER_PERSON)	return &clHorse[index];
		return NULL;
	}


	bool Add(cltHorse* pclhorse, SI32* pindex);
	
	// 말의 잔여 요구조건을 구한다. 
	bool GetNeed(cltHorse* pclhorse, SI32* pneeditemunique, SI32* pneeditemnum);

	// index의 말을 탈 수 있는가?
	bool CanRide(SI32 index, cltDate* pcldate);

	// 말을 넣을 수 있는 인덱스를 찾는다. 
	SI32 FindEmptyHorseIndex();

	// 말을 한마리라도 갖고 있는가. 
	bool HaveHorse()
	{
		//if(clHorse.siHorseUnique)return true;
		for(SI32 i = 0;i < MAX_HORSE_PER_PERSON;i++)
		{
			if(clHorse[i].siHorseUnique)return true;
		}

		return false;
	}

	void SetHorse(SI32 index, cltHorse* pclhorse)
	{
		//clHorse.Set(pclhorse);
		if(index < 0)						return;
		if(index >= MAX_HORSE_PER_PERSON)	return;
		if(pclhorse == NULL)				return;

		clHorse[index].Set(pclhorse);
	}

	void Init(SI32 index)
	{
		if(index < 0)						return;
		if(index >= MAX_HORSE_PER_PERSON)	return;

		//clHorse.Init();
		clHorse[index].Init();
	}

};

class cltHorseTypeInfo{
public:
	SI32 siUnique;
	TCHAR szCode[64];
	TCHAR szSpecies[64];
	SI32 siKind;			// 캐릭터로서의 kind

	SI32 siServiceArea;
	bool bNotTrade;

	cltHorseTypeInfo(SI32 unique, TCHAR* pcode, TCHAR* pspecies, SI32 kind, SI32 ServiceArea, bool NotTrade);
	~cltHorseTypeInfo();

};

class cltHorseManager{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltHorseTypeInfo*, MAX_HORSE_TYPE_INFO>		pclHorseTypeInfo;
#else
	cltHorseTypeInfo* pclHorseTypeInfo[MAX_HORSE_TYPE_INFO];
#endif

	cltHorseManager();
	~cltHorseManager();

	
	// 말의 타입을 추가한다. 
	bool AddHorseType(SI32 unique, TCHAR* pcode, TCHAR* pspecies, SI32 kind, SI32 ServiceArea, bool NotTrade);

	// 말에 대한 설명을 텍스트에 담아 리턴한다. 
	bool GetText(cltHorse* pclhorse, TCHAR* ptext, SI16 txtSize);

	// 말 품종 문자열로 유니크 값을 얻는다
	SI32 GetHorseUniqueFromString( TCHAR *szSpecies );
	
	// 말의 유니크를 근거로 말의 이름을 구한다. 
	TCHAR* GetHorseName(SI32 horseunique);
	
	SI32 GetFullStr(SI32 unique);
	SI32 GetFullDex(SI32 unique);
	SI32 GetFullMag(SI32 unique);
	SI32 GetFullSpeed(SI32 unique);
	SI32 GetFullLife(SI32 unique);

	// 남은 수명(일)을 구한다. 
	SI32 GetLife(cltHorse* pclhorse, SI32 curday);
	void SetLife(cltHorse* pclhorse, SI32 curday, SI32 life);


	SI32 Action(cltServer* pclserver, cltHorse* pclhorse, SI32 skilllevel, SI32 datevary, bool bselfswitch, SI32* prewardamount);

	// 요구조건을 충족시켰는지 확인한다.
	SI32 CheckNeed(cltHorse* pclhorse);

	// 말에게 보상한다. 
	bool Reward(cltHorse* pclhorse, SI32 rate, bool bselfswitch, SI32* prewardamount);

	// 말의 요구조건을 만든다. 
	bool MakeHorseNeed(SI32 npcpricerate, SI32 skilllevel, cltHorse* pclhorse, SI32 duetime, cltDate* pcldate);

	// 말이 먹기 원하는 음식의 유니크와 개수를 얻어온다. 
	bool GetHorseFoodNeed(cltHorse* pclhorse, SI32* pitemunique, SI32* pitemnum);

	// 말에게 먹이를 준다.
	bool GiveFood(cltHorse* pclhorse, cltItem* pclitem);

	// 말의 요구 조건을 텍스트로 얻어온다. 
	bool GetNeedText(cltHorse* pclhorse, TCHAR* ptext, SI16 txtSize );

	// 말의 추가 스탯의 제한날자를 텍스트로 얻어온다.
	void GetAddStatusLimitDateText(cltHorse* pclhorse,TCHAR* ptext, SI16 txtSize);
	void GetAddStatusLimitDateText(cltHorse* pclhorse,TCHAR* ptext, SI16 txtSize, SI32 TextMgrIndex);

	// 프리미엄 말파츠슬롯1의 제한날자를 텍스트로 얻어 온다.
	void GetPremiumPartsDateText_Slot1(cltHorse* pclhorse,TCHAR* ptext, SI16 txtSize);
	// 프리미엄 말파츠슬롯2의 제한날자를 텍스트로 얻어 온다.
	void GetPremiumPartsDateText_Slot2(cltHorse* pclhorse,TCHAR* ptext, SI16 txtSize);
	// 프리미엄 말파츠슬롯3의 제한날자를 텍스트로 얻어 온다.
	void GetPremiumPartsDateText_Slot3(cltHorse* pclhorse,TCHAR* ptext, SI16 txtSize);

	// 말의 추가 스탯의 제한 날짜를 구해온다.
	SI32 GetAddStatusLimitDateVary(cltHorse* pclhorse);
	// 말 프리미엄 파츠의 제한날짜를 구해온다
	SI32 GetPremiumPArtsDateVary_Slot1(cltHorse* pclhorse);
	SI32 GetPremiumPArtsDateVary_Slot2(cltHorse* pclhorse);
	SI32 GetPremiumPArtsDateVary_Slot3(cltHorse* pclhorse);
};

#endif
