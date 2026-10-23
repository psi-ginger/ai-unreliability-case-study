#pragma once

#include "../CommonLogic/CommonLogic.h"

class CValentineStore
{
public:
	CValentineStore();
	~CValentineStore();
	void Create(SI32 siCharUnique,SI32 siNameTextUnique);
	BOOL IsCreate();
	void Destroy();
	BOOL Open();
	void Close();
	BOOL SellChocolate(SI32 siCount);
	BOOL SellCandy(SI32 siCount);
	BOOL HaveChocolate();
	BOOL HaveCandy();
	BOOL IsOpened();
	SI32 GetCharUnique();
	SI32 GetNameTextUnique();
	BOOL MakeItem(SI32 siChocolateCountThatNPCBuy, SI32 siCandyCountThatNPCBuy);
	SI32 GetRemainedChocolateCount();
	SI32 GetRemainedCandyCount();
	static void AdvertiseOpening();
protected:
	static const SI32 MAXCHOCOLATECOUNT = 250;
	static const SI32 MAXCANDYCOUNT = 250;

	SI32 m_siCharUnique;

	SI32 m_siRemainedChocolateCount;
	SI32 m_siRemainedCandyCount;

	bool m_bOpened;
	
	SI32 m_siNameTextUnique;

	void Init();
private:

};