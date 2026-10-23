//#ifndef _GACHAMANAGER_H_
//#define _GACHAMANAGER_H_
//
//#include <Directives.h>
//#include "CommonLogic.h"
//
//#define MAX_SHOW_GACHA_PRODUCT 7
//#define	MAX_GACHA_PRODUCT 9
//
//#define GACHA_USE_GOLDKEY	1
//#define GACHA_USE_SILVERKEY	2
//#define GACHA_USE_BRONZEKEY	3
//#define GACHA_USE_KEYPIECE	4
//
//#define GACHA_MAX_KEYTYPE	3
//
//#define GACHA_MAX_RARELIST	100
//
//const SI32 GACHA_GOLDKEY_ITEMUNIQUE =  ITEMUNIQUE(7872);
//const SI32 GACHA_SILVERKEY_ITEMUNIQUE =  ITEMUNIQUE(7870);
//const SI32 GACHA_BRONZEKEY_ITEMUNIQUE =  ITEMUNIQUE(7868);
//const SI32 GACHA_KEYPIECE_ITEMUNIQUE =  ITEMUNIQUE(7875);
//const SI32 GACHA_KEYPIECE_NEED_NUM =  10;	// 1회에 소모되는 가차열쇠조각의 갯수
//
//
//
//struct stGachaProduct
//{
//	SI32 ItemUnique;
//	SI32 ItemNum;
//	SI32 ItemUseDate;
//	SI32 GachaRate;
//
//	stGachaProduct()
//	{
//		Init();
//	}
//
//	void Init()
//	{
//		memset(this, 0, sizeof(stGachaProduct));
//	}
//
//	void Set( stGachaProduct * src )
//	{
//		memcpy(this,src, sizeof(stGachaProduct));
//	}
//	bool IsSame( stGachaProduct * src )
//	{
//		if( ItemUnique == src->ItemUnique && ItemNum == src->ItemNum 
//			&& ItemUseDate == src->ItemUseDate && GachaRate == src->GachaRate)
//		{
//			return true;
//		}
//
//		return false;
//	}
//
//};
//
//class CGachaManager
//{
//public:
//
//	CGachaManager();
//	~CGachaManager();
//
//	void Init();	
//
//	// 현제 가차 시스템의 보상 목록을 읽어온다.
//	bool LoadGachaProductDataFromTxt();
//
//	stGachaProduct	m_GachaProduct[GACHA_MAX_KEYTYPE][MAX_GACHA_PRODUCT];
//
//	// 가차 시스템에서 얻을수있었거나 얻을수있는 아이템들의 목록을 읽어온다.
//	// 등록된 아이템들은 아이템을 만들때, 분해할때 특별한 동작을 취한다.
//	bool LoadGachaRareListDataFromTxt();
//	SI32 m_GachaRareList[GACHA_MAX_RARELIST];
//
//	bool IsGachaRareItem( SI32 ItemUnique );
//
//private:	
//
//};
//
//#endif
