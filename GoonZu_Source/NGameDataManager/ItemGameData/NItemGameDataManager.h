//=============================================================================
//
//=============================================================================
#ifndef _NITEMGAMEDATAMANAGER_H_
#define _NITEMGAMEDATAMANAGER_H_

#include "../../NLib/NArray.h"
#include "NItemGameData.h"

#define MAX_ITEM_COUNT		50000

struct NDataTypeInfo;

class NItemGameDataManager
{
protected:
	NItemGameData*		m_pkData[MAX_ITEM_COUNT];

public:
	NItemGameDataManager();
	~NItemGameDataManager();

	BOOL		Init(SI32 siServiceArea);
	BOOL		LoadItemGameData(SI32 siServiceArea);

	//===============================================================
	// INLINES
	//===============================================================
	inline NItemGameData*	GetData(SI32 siUnique)
	{
		if(siUnique < 0 || siUnique >= MAX_ITEM_COUNT)	return NULL;
		return m_pkData[siUnique];
	}

	inline TCHAR* GetItemName(SI32 siUnique)
	{
		if(siUnique < 0 || siUnique >= MAX_ITEM_COUNT)	return NULL;
		if(m_pkData[siUnique] != NULL)		return m_pkData[siUnique]->m_szItemName;

		return NULL;
	}

	SI32 GetItemUnique(TCHAR* pszItemName);

	inline SI32				GetCount()
	{
		return MAX_ITEM_COUNT;
	}

	inline SI32				GetMaxCount()
	{
		return MAX_ITEM_COUNT;
	}

protected:
	/*BOOL AddItemData(NDataTypeInfo* pDataTypeInfo, TCHAR* pszFileName);
	BOOL AddItemData_UnCrypt(NDataTypeInfo* pDataTypeInfo, TCHAR* pszFileName);*/

private:
	SI32	m_siItemUnique;
	TCHAR	m_szItemName[64];

};

#endif
