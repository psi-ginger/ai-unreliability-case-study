#ifndef _MININGMGR_H_
#define _MININGMGR_H_

#include "../../CommonHeader.h"
#include "../Agriculture.h"

#define MAX_MININGDATA_NUM			128
#define	MAX_MINEITEM_NUM			9

struct stMiningData
{
	stMiningData()
	{
		Init();
	}

	stMiningData( stMiningData *pMiningData )
	{
		memcpy( this, pMiningData, sizeof( stMiningData ) );
	}

	void Init()
	{
		memset( this, 0, sizeof( stMiningData ) );
	}

	void Set( SI16 VillageUnique, SI16 *pMineItemUnique, SI16 FieldObjectUnique )
	{
		if ( pMineItemUnique == NULL )
		{
			return;
		}

		siVillageUnique = VillageUnique;
		SI32 i=0;
#ifdef _SAFE_MEMORY
		for(i=0; i<MAX_MINEITEM_NUM; i++)		siMineItemUnique[i] = pMineItemUnique[i];
#else
		memcpy(siMineItemUnique, pMineItemUnique, sizeof( siMineItemUnique ) );
#endif
		siFieldObjectUnique = FieldObjectUnique;
	}

	SI16 siVillageUnique;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_MINEITEM_NUM>		siMineItemUnique;
#else
	SI16 siMineItemUnique[MAX_MINEITEM_NUM];
#endif
	SI16 siFieldObjectUnique;
};

class CMiningMgr
{
public:
	CMiningMgr();
	~CMiningMgr();

	void ReadData();

	void Add( stMiningData *pMiningData );

	bool IsAvailableMining( SI32 FieldObjectUnique );
	bool IsAvailableMiningItem( SI16 ItemUnique );

	SI16 GetMineItemUniqueFromVillageUnique( SI16 VillageUnique );
	SI16 GetMineItemUniqueRandom( SI16 siVillageUnique );
	SI16 GetMineItemUnique(SI16 *MiningKindList);

	DWORD dwMiningStartClock;

private:
#ifdef _SAFE_MEMORY
	NSafeTArray<stMiningData*, MAX_MININGDATA_NUM>		m_pstMiningData;
#else
	stMiningData *m_pstMiningData[ MAX_MININGDATA_NUM ];
#endif
};

#endif