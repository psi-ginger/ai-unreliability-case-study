#ifndef _FOREIGNMERCHANTNPC_H_
#define _FOREIGNMERCHANTNPC_H_

#include <Directives.h>
#include "CommonLogic.h"
#include "../../../../NLib/NArray.h"

#define MAX_FOREIGNMERCHANTNPC_NUM			30			// 교역 상인은 몇 명인가?
#define MAX_FOREIGNMERCHANTNPC_NAME_LENGTH	64			// 교역 상인 이름 길이
//enum { FOREIGNMERCHANTNPC_KIND_JAPAN = 200, FOREIGNMERCHANTNPC_KIND_CHINESE = 300 };

#define MAX_FOREIGNMERCHANTNPC_BUY_ITEM_TOTAL_PRICE 1000000		// 100만냥 만큼만 아이템을 구입한다.

enum { FOREIGNMERCHANT_HELMET, FOREIGNMERCHANT_ARMOUR, FOREIGNMERCHANT_AXE, FOREIGNMERCHANT_SWORD, FOREIGNMERCHANT_STAFF, FOREIGNMERCHANT_SPEAR, FOREIGNMERCHANT_GUN, FOREIGNMERCHANT_BOW, FOREIGNMERCHANT_SHOES, FOREIGNMERCHANT_BELT, FOREIGNMERCHANT_POTTER, FOREIGNMERCHANT_HARDWARE, FOREIGNMERCHANT_RING, FOREIGNMERCHANT_CLOTHES, FOREIGNMERCHANT_HAT, FOREIGNMERCHANT_NECK, FOREIGNMERCHANT_PAPER, FOREIGNMERCHANT_FISHING, FOREIGNMERCHANT_CANNON, FOREIGNMERCHANT_END };

#define SAVE_TIME_FOREIGNMERCHANTNPC_BUY_INFO_TO_DB		1000	// 1000 프레임당 1번씩 저장

enum { FOREIGNMERCHANT_NOT_EXIST = -1, FOREIGNMERCHANT_STOP_STAUS = 0, FOREIGNMERCHANT_BUY_STATUS };

#define MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM	50

struct stForeignMerchantNPCInfo
{
	SI16	siForeignMerchantKind;			// 현재 NPC 종류
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM>		siBuyItemUniqueList;
	NSafeTArray<GMONEY, MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM>	siBuyItemPriceList;
#else
	SI16	siBuyItemUniqueList[ MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM ];		// 현재 NPC 가 구입중인 아이템의 유니크 값 리스트
	GMONEY	siBuyItemPriceList[ MAX_FOREIGNMERCHANT_BUYITEM_LIST_NUM ];		// 현재 NPC 가 구입하는 아이템은 가격 리스트
#endif
	SI16	siBuyItemType;
	SI16	siBuyItemListCount;
};

class CForeignMerchantNPC
{
public:
	CForeignMerchantNPC();
	~CForeignMerchantNPC();

	void Initialize();
	void Destroy();

	SI16 GetStatus();

	SI16 GetForeignMerchantKind();

	void SetKind( SI16 siForeignMerchantKind, SI16 siItemType );
	void Set( SI32 *psiItemUniqueList, GMONEY *psiItemPriceList, SI32 siListCount );
	void Set( SI16 *psiItemUniqueList, GMONEY *psiItemPriceList, SI32 siListCount );
	//void Set( SI16 siItemUnique, SI32 siItemAveragePrice, SI32 siBuyItemRestNum, cltDate clDate );		// 유니크 값에 따른 시세를 DB 로 부터 받아와 입력함
	stForeignMerchantNPCInfo* GetForeignMerchantNPCInfo();		// 현재 정보를 얻어옮
	void GetForeignMerchantNPCInfo( stForeignMerchantNPCInfo * );	// 현재 정보를 얻어옮

	//bool Buy( UI16 uiItemNum );									// 아이템을 유저로 부터 구입했을 경우

	void GetBuyInfoText( TCHAR *pBuffer );

	bool IsAvailable();

	void RefreshPrice();

	/*
	void DecreaseBuyItemNum( UI16 uiDecreaseBuyItemNum );		// 구입하는 아이템의 개수를 줄인다.

	void SaveDifferent();										// SAVE_TIME_FOREIGNMERCHANTNPC_BUY_INFO_TO_DB 마다 바뀐게 있다면 저장함.

	cltDate *GetDate();

	SI32 GetRestNum();
	*/


private:
	stForeignMerchantNPCInfo	sForeignMerchantNPCInfo;
	
	SI16						m_siForeignMerchantKind;
	SI32						m_siBuyItemTotalNum;				// 현재 NPC 가 구입할려는 아이템의 총 개수
	SI16						m_siStatus;							// 현재 NPC 가 구입중인 상태인지 아닌지에 대한 정보를 가지고 있음.
	bool						m_bAvailable;
	SI32						m_siSaveLastBuyRestNum;				// 최근에 DB로 데이터를 저장하면서, 바뀐값
	SI16						m_siBuyItemType;
	
};


class CForeignMerchantNPCMgr
{
public:
	CForeignMerchantNPCMgr();
	~CForeignMerchantNPCMgr();

	void Initialize();
	void Destroy();

	void Add( SI16 siKind, SI16	siBuyItemType );
	void Set( SI16 siKind, SI32 siBuyItemType, SI32 *psiItemUniqueList, GMONEY *psiItemPriceList, SI32 siListCount );
	void Set( SI16 siKind, SI32 siBuyItemType, SI16 *psiItemUniqueList, GMONEY *psiItemPriceList, SI32 siListCount );
	//void Set( SI16 siKind, SI16 siBuyItemUnique, SI32 siBuyItemAveragePrice, SI32 siBuyItemRestNum, cltDate	clDate );

	void Action( cltTime *pTime );

	SI16 GetStatus( SI16 siKind );	
	CForeignMerchantNPC *GetForeignMerchantNPC( SI16 siKind );
	stForeignMerchantNPCInfo* GetForeignMerchantNPCInfo( SI16 siKind );

	void GetBuyInfoText( SI16 siKind, TCHAR *pBuffer );

private:
#ifdef _SAFE_MEMORY
	NSafeTArray<CForeignMerchantNPC, MAX_FOREIGNMERCHANTNPC_NUM>		m_ForeignMerchant;
#else
	CForeignMerchantNPC m_ForeignMerchant[ MAX_FOREIGNMERCHANTNPC_NUM ];
#endif
};

#endif
