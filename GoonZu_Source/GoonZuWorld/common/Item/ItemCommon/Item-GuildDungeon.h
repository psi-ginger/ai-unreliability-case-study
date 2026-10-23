#pragma once

#include <Directives.h>
#include "../../../CommonLogic/Guild/Guild-Dungeon.h"

class cltGuildDungeonItemUnit
{
public:
	SI32 m_siMapIndex;
	SI32 m_siItemUnique;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, 5>		m_siCondition;
	NSafeTArray<SI32, 5>		m_siItemNum;
#else
	SI32 m_siCondition[5];
	SI32 m_siItemNum[5];
#endif

	cltGuildDungeonItemUnit( SI32 index, SI32 unique,
							SI32* cond, SI32* num )
	{
		m_siMapIndex = index;
		m_siItemUnique = unique;

		for( SI32 i=0; i < 5 ; i++ )
		{
			m_siCondition[i] = cond[i];
			m_siItemNum[i] = num[i];
		}
	}
};

class cltGuildDungeonItem 
{
public:
	cltGuildDungeonItem();
	~cltGuildDungeonItem();

	void Init();
	void Destroy();

	void LoadFromFile();

	SI32 GetItem( IN SI32 mapindex, IN SI32 gp, OUT SI32 *itemunique );

public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltGuildDungeonItemUnit*, MAX_GUILDDUNGEON>		m_pclItem;
#else
	cltGuildDungeonItemUnit* m_pclItem[MAX_GUILDDUNGEON];
#endif
};