#pragma once
#include "Directives.h"
#include "../../../../NLib/NArray.h"
//#include "../../../common/Item/ItemCommon/cltSetItem.h"
#include "../../../common/Item/ItemCommon/cltItem.h"

//#include "../../../Common/Char/CharCommon/Char-Common.h"

class cltCharCommon;
class cltItemManagerCommon;

//***************************************************************************************************
//
// Class Name 		: CSetItemInfo
//
// Last Modified 	: 2006 / 12 / 05
// Created		 	: 김광명
//
// Function			: 세트 아이템 정보를 저장한다.
//
//***************************************************************************************************


const UI08 MAX_SETITEM_UNIQUE_NUMBER = 13;

class CSetItemInfo
{
private:

	// 세트 아이템의 종류
	SI16 m_siSetItemType;
	// 세트 아이템을 구성하는 아이템 유니크들
	// 반지는 둘중에 하나만 있어도 되고, 배열에서 8번 10번은 그냥 무조건 0으로 간다.
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_SETITEM_UNIQUE_NUMBER>		m_siSetItemUnique;
#else
	SI16 m_siSetItemUnique[MAX_SETITEM_UNIQUE_NUMBER];
#endif
	
public:

	CSetItemInfo()
	{
		Init();
	}

	~CSetItemInfo()
	{
	}

	void Init();
	void SetSetItemInfo( SI16 SetItemType, SI32 SetItemUnique[] );
	void SetSetItemInfo( CSetItemInfo& SetItemInfo );

	SI16 GetSetItemType()
	{
		return m_siSetItemType;
	}

	SI16 GetSetItemUnique( SI16 Index )
	{
		if( Index < 0 || Index >= MAX_SETITEM_UNIQUE_NUMBER )
			return 0;

		return m_siSetItemUnique[Index];
	}
};

class cltItem;
//***************************************************************************************************
//
// Class Name 		: CSetItemManager
//
// Last Modified 	: 2006 / 12 / 05
// Created		 	: 김광명
//
// Function			: 세트 아이템을 관리한다.
//
//***************************************************************************************************
class CSetItemManager 
{
//private:
public:

	SI16		  m_siMaxSetItemNumber;
	CSetItemInfo* m_pclSetItemInfo;

public:

	CSetItemManager() 
	{
		m_siMaxSetItemNumber = 0;
		m_pclSetItemInfo = NULL;
	}

	~CSetItemManager()
	{
		if( m_pclSetItemInfo )
		{
			delete[] m_pclSetItemInfo;
			m_pclSetItemInfo = NULL;
		}
	}

	void LoadSetItemInfo();
	bool CompareSetItemUnique( SI16 Index, cltItem* pclItemList );
	SI16 FindSetItemType( cltItem* pclItemList );
	
	bool IsSetItemStep( SI16 _siType );	// 단계별 효과가 있는 세트 아이템인가.
	SI16 FindSetItemType( cltCharCommon* _pclChar, SI16* _siStep, SI32 _UseSetItem );
	bool CompareSetItemUniqueStep( SI16 Index, cltCharCommon* _pclchar, SI16* _siStep, SI32 _UseSetItem );
};

