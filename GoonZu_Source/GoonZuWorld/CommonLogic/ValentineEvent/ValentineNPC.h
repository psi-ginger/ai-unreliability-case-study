#pragma once

#include "../CommonLogic/CommonLogic.h"

class CValentineNPC
{
public:
	CValentineNPC()
	{
		m_siCharID = 0;
		m_siReceivedItemCount = 0;
		m_siThxTextIndex = 0;
		m_siCharKind = 0;
		m_siBuffType = 0;
		m_siBuffValue = 0;
		m_bSexMale = true;
		Init();
	}
	~CValentineNPC(){}

	static const SI32 MAX_ITEMCOUNT_NOTBUYITEM = 20;

	void Init();

	void SetCharID(SI32 siCharID) { m_siCharID = siCharID;}
	void SetReceivedItemCount(SI32 siReceivedItemCount){ m_siReceivedItemCount = siReceivedItemCount;}
	void SetThxTextIndex(SI32 siThxTextIndex){ m_siThxTextIndex = siThxTextIndex;}
	void SetCharKind(SI32 siCharKind){ m_siCharKind = siCharKind;}
	void SetBuffType(SI32 siBuffType){ m_siBuffType = siBuffType;}
	void SetBuffValue(SI32 siBuffValue){ m_siBuffValue = siBuffValue;}
	void SetIsSexMale(bool bSexMale){ m_bSexMale = bSexMale;}

	SI32 GetCharID() { return m_siCharID;}
	SI32 GetReceivedItemCount() { return m_siReceivedItemCount;}
	SI32 GetThxTextIndex() { return m_siThxTextIndex;}
	SI32 GetCharKind() { return m_siCharKind;}
	SI32 GetBuffType() { return m_siBuffType;}
	SI32 GetBuffValue() { return m_siBuffValue;}
	SI32 IsSexMale() { return m_bSexMale;}

	bool ReceiveItem(SI32 siVillageUnique, SI32 siGivedItemCount);
	
	bool SetPresentItemByVillageUser(SI32 siVillageUnique, SI32 siPresentItemCount);

	SI32 GetUniqueOfBestVillage();

	void ShowImpression(SI32 siVillageUnique);
private:
	SI32 m_siCharKind;
	SI32 m_siCharID;
	SI32 m_siReceivedItemCount;
	SI32 m_siThxTextIndex;
	SI32 m_siBuffType;
	SI32 m_siBuffValue;
	bool m_bSexMale;

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_VILLAGE_NUMBER>		m_TheNumberOfItemEachVillageList;
#else
	SI32 m_TheNumberOfItemEachVillageList[MAX_VILLAGE_NUMBER];
#endif
};
