
#ifndef _ITEMMALLDATA_H
#define _ITEMMALLDATA_H

#include <Directives.h>

#define MAX_ITEMMALL_TAB	2

#define ITEMMALL_WEBPAGE_WIDTH		420
#define ITEMMALL_WEBPAGE_HEIGHT		490

#define ITEMMALL_BUTTON_WIDTH		74
#define ITEMMALL_BUTTON_HEIGHT		74

#define SCROLLBAR_SIZE				16+2

//#define MAX_ITEMMALL_NUM			18

#define ITEMALL_CHARACTER_MENU			10  // 캐릭터 치장 메뉴
#define ITEMALL_CHARACTER_SUBJECT1		11	// 세트
#define ITEMALL_CHARACTER_SUBJECT2		12	// 망토
#define ITEMALL_CHARACTER_SUBJECT3		13	// 의복
#define ITEMALL_CHARACTER_SUBJECT4		14	// 모자
#define ITEMALL_CHARACTER_SUBJECT5		15	// 악세사리

#define ITEMALL_POWER_MENU				20	// 파워 메뉴
#define ITEMALL_POWER_SUBJECT1			21	// 프리미엄
#define ITEMALL_POWER_SUBJECT2			22	// 초기화
#define ITEMALL_POWER_SUBJECT3			23	// 이동
#define ITEMALL_POWER_SUBJECT4			24	// 이용권

#define ITEMALL_SUMMONHORSE_MENU		30	//				소환/말
#define ITEMALL_SUMMONHORSE_SUBJECT1	31	//				소환영웅  -> 동물
#define ITEMALL_SUMMONHORSE_SUBJECT2	32	//				소환영웅육성  -> 동물육성
#define ITEMALL_SUMMONHORSE_SUBJECT3	33	//				동물 X
#define ITEMALL_SUMMONHORSE_SUBJECT4	34	//				동물육성 X

#define ITEMALL_MATERIALFOOD_MENU		40  // 재료/음식
#define ITEMALL_MATERIALFOOD_SUBJECT1	41	// 약
#define ITEMALL_MATERIALFOOD_SUBJECT2	42	// 음식
#define ITEMALL_MATERIALFOOD_SUBJECT3	43	// 재료
#define ITEMALL_MATERIALFOOD_SUBJECT4	44	// 책


#define ITEMALL_BEGINNER_MENU			50	// 초보자
#define ITEMALL_BEGINNER_SUBJECT1		51	// 세트
#define ITEMALL_BEGINNER_SUBJECT2		52	// 무기사용술
#define ITEMALL_BEGINNER_SUBJECT3		53	// 이용권

#define ITEMALL_SILVERCOIN_MENU			60	// 은전
#define ITEMALL_SILVERCOIN_SUBJECT1		61	// 은전 메뉴 부른다.

#include "../common/Item/ItemCommon/ItemUnit.h"
#include "../DBManager/GameDBManager_world/DBMsg-ItemMall.h"

class cltItemMallData{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_ITEMMALL_SET_NUM>		clItem;
#else
	cltItem clItem[MAX_ITEMMALL_SET_NUM]; // 이 상품에 들어가는 아이템의 정보
#endif

	SI32 siPrice;				// 이 상품의 가격
	SI32 siEventPrice;			// 이 상품의 Event 가격

	SI32 siProductKey;			// Product Key
	SI32 siGroupKey;			// Group Key

	SI32 siImageFont;

	TCHAR szName[128] ;
	TCHAR szExplain[256] ;
	TCHAR szExplain2[256] ;		// [종호] 상품에 대한 설명2

	bool bGift;					// true면 선물가능

	bool bOnlyBuySilverCoin;	// 은전으로만 구매가 가능하다
	bool bAgeLimit;				// 거래제한. KOR: 나이

#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_ITEMMALL_SET_NUM>		siLimitLevel;
#else
	SI16 siLimitLevel[MAX_ITEMMALL_SET_NUM];		// 아이템 별 거래제한. For English
#endif

	void Init()
	{
		for ( SI32 i = 0 ; i < MAX_ITEMMALL_SET_NUM ; i ++ )
		{
			clItem[i].Init();
			siLimitLevel[i] = 0;
		}

		siPrice				= 0;
		siEventPrice		= 0;

		siProductKey		= 0 ;
		siGroupKey			= 0 ;
		
		siImageFont			= 0 ;

		szName[0]			= '\0';
		szExplain[0]		= '\0';
		szExplain2[0]		= '\0';

		bGift				= true ;

		bOnlyBuySilverCoin	= false;
		bAgeLimit			= false;	
	}

	void Set(cltItemMallData* pclinfo)
	{
		for ( SI32 i = 0 ; i < MAX_ITEMMALL_SET_NUM ; i ++ )
		{
			clItem[i].Set(&pclinfo->clItem[i]);
			siLimitLevel[i] = pclinfo->siLimitLevel[i];
		}

		siPrice			= pclinfo->siPrice ;
		siEventPrice	= pclinfo->siEventPrice ;

		siProductKey	= pclinfo->siProductKey ;
		siGroupKey		= pclinfo->siGroupKey ;

		siImageFont		= pclinfo->siImageFont ;

		MStrCpy(szName,		pclinfo->szName,	128);
		MStrCpy(szExplain,	pclinfo->szExplain,	256);
		MStrCpy(szExplain2,	pclinfo->szExplain2,256);

		bGift				= pclinfo->bGift ;

		bOnlyBuySilverCoin = pclinfo->bOnlyBuySilverCoin;
		bAgeLimit		   = pclinfo->bAgeLimit;
	}

};

#endif // _ITEMMALLDATA_H