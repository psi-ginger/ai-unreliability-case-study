//---------------------------------
// 2003/8/19 ���°�
//---------------------------------

#ifndef _NPCITEM_H
#define _NPCITEM_H

#include "cltItem.h"

//-------------------------------------
// NPC�� ���� �� ������ ����. 
//-------------------------------------
#define MAX_NPC_ITEM_NUMBER		20
#define MAX_NPC_MERCHANT_NUMBER	10
#define MAX_NPC_SELLLIST		64

class cltNPCInteInfoUnit{
public:
	cltItem clItem;
	GMONEY	siPrice;

	cltNPCInteInfoUnit()
	{
		siPrice = 0;
		clItem.Init();
	}

	void Set(cltNPCInteInfoUnit* pclNPCInteInfoUnit)
	{
        if(pclNPCInteInfoUnit == NULL)			return;

		clItem.Set(&pclNPCInteInfoUnit->clItem);
		siPrice= pclNPCInteInfoUnit->siPrice;
	}
};

class cltNPCMerchantInfo
{
public:
	SI32	siNPCKind;
	SI32*	psiNPCItemInfo;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltNPCInteInfoUnit, MAX_NPC_ITEM_NUMBER>	clItemInfo;
#else
	cltNPCInteInfoUnit clItemInfo[MAX_NPC_ITEM_NUMBER];
#endif

	cltNPCMerchantInfo()
	{
		siNPCKind		= 0;
		psiNPCItemInfo	= NULL;
	}
};

class cltNPCItemInfo{
protected:
	SI32 siCurNPCMerchantNumber;
public:

#ifdef _SAFE_MEMORY
	NSafeTArray<cltNPCMerchantInfo, MAX_NPC_MERCHANT_NUMBER>	clNPCMerchantInfo;
#else
	cltNPCMerchantInfo clNPCMerchantInfo[MAX_NPC_MERCHANT_NUMBER];
#endif

	cltItemManagerCommon* pclItemManager;

	cltNPCItemInfo(cltItemManagerCommon* pclitemmanager);
	~cltNPCItemInfo();
	void Create(SI32 npcpricerate);
	void Add(SI32 npckind, SI32* pnpcinfo);
	void SetNPCItemInfo(SI32 npcpricerate);
	

	//npckind�� �ٰŷ� ������ �����͸� ���Ѵ�.
	cltNPCMerchantInfo* GetMerchantInfo(SI32 npckind);

	// �ü��� ����ȵ� �⺻����
	GMONEY GetPrice(SI32 npckind, SI32 unique);

	// �ü��� ����� ��¥����
	GMONEY GetRealPrice(SI32 npckind, SI32 unique);

	SI32*	pclNPCItemInfo_KIND_WAITINGWOMAN;
	SI32*	pclNPCItemInfo_KIND_INVESTMENT;
	SI32*	pclNPCItemInfo_KIND_MAGICALRACCOON_NPC;
	SI32*	pclNPCItemInfo_KIND_ANNIVERSARY;

	// PCK : ���� ���� ���� �߰� (09.08.31)
	SI32*	pclNPCItemInfo_KIND_SNACKBAR1;
	SI32*	pclNPCItemInfo_KIND_SNACKBAR2;
	SI32*	pclNPCItemInfo_KIND_SNACKBAR3;
	SI32*	pclNPCItemInfo_KIND_SNACKBAR4;

	SI32*	m_psiNPCItemInfo_KIND_CHOCOLATE;
	SI32*	m_psiNPCItemInfo_KIND_GIFTRACCOON;
};


#endif