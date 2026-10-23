#include "MiningMgr.h"

#include "CommonLogic.h"
extern cltCommonLogic* pclClient;

CMiningMgr::CMiningMgr()
{
	for ( SI16 i = 0; i < MAX_MININGDATA_NUM; ++i )
	{
		m_pstMiningData[ i ]=NULL;
	}
	
	dwMiningStartClock = 0;
	ReadData();
}

CMiningMgr::~CMiningMgr()
{
	for ( SI16 i = 0; i < MAX_MININGDATA_NUM; ++i )
	{
		delete m_pstMiningData[ i ];
		m_pstMiningData[ i ] = NULL;
	}
}

void CMiningMgr::ReadData()
{
	FILE *fp = _tfopen( TEXT("Agriculture/Mine/Mine.txt"), TEXT("rt") );
	if ( fp == NULL )
	{
		return;
	}

	TCHAR buffer[ 2048 ] = TEXT("");

	for ( SI16 k = 0; k < 6; ++k )
	{
		_fgetts(buffer, 2048, fp);
	}

	SI32 VillageUnique;
	SI16 MineItemUnique[MAX_MINEITEM_NUM];
	SI32 FieldObjectUnique;

	SI32 siReadBuf[MAX_MINEITEM_NUM];

	stMiningData MiningData;

	ZeroMemory( MineItemUnique, sizeof(MineItemUnique) );
	ZeroMemory( siReadBuf, sizeof(siReadBuf) );

	while( _fgetts(buffer, 2048, fp) != NULL)
	{
		_stscanf(buffer, TEXT("%d %d %d %d %d %d %d %d %d %d %d"), &VillageUnique, 
			&siReadBuf[0], &siReadBuf[1], &siReadBuf[2], &siReadBuf[3], &siReadBuf[4], 
			&siReadBuf[5], &siReadBuf[6], &siReadBuf[7], &siReadBuf[8], 
			&FieldObjectUnique );

		for ( SI32 siIndex=0;siIndex<MAX_MINEITEM_NUM; siIndex++ )
		{
			if ( siReadBuf[ siIndex ] <= 0 )
			{
				break;
			}

			MineItemUnique[siIndex] = (SI16)siReadBuf[siIndex];
		}
		
		MiningData.Set( VillageUnique, MineItemUnique, FieldObjectUnique );
		Add( &MiningData );
		
	}

	fclose( fp );

	return;
}

void CMiningMgr::Add( stMiningData *pMiningData )
{
	for ( SI16 i = 0; i < MAX_MININGDATA_NUM; ++i )
	{
		if ( m_pstMiningData[ i ] == NULL )
		{
			m_pstMiningData[ i ] = new stMiningData( pMiningData );
			break;
		}
	}

	return;
}

bool CMiningMgr::IsAvailableMining( SI32 FieldObjectUnique )
{
	for ( SI16 i = 0; i < MAX_MININGDATA_NUM; ++i )
	{
		if ( m_pstMiningData[ i ] )
		{
			if ( m_pstMiningData[ i ]->siFieldObjectUnique == FieldObjectUnique )
			{
				return true;
			}
		}
	}

	return false;
}

bool CMiningMgr::IsAvailableMiningItem( SI16 ItemUnique )
{
	for ( SI16 i = 0; i < MAX_MININGDATA_NUM; ++i )
	{
		if ( m_pstMiningData[ i ] )
		{
			for(SI32 j=0; j<MAX_MINEITEM_NUM; j++)
			{
				if ( m_pstMiningData[ i ]->siMineItemUnique[j] == ItemUnique )
				{
					return true;
				}
			}
		}		
	}

	return false;
}

SI16 CMiningMgr::GetMineItemUniqueFromVillageUnique( SI16 VillageUnique )
{
	if ( VillageUnique < 1 )			return 0;

	for ( SI16 i = 0; i < MAX_MININGDATA_NUM; ++i )
	{
		if ( m_pstMiningData[ i ] )
		{
			if ( m_pstMiningData[ i ]->siVillageUnique == VillageUnique )
			{
				// PCK : 채광개편이 적용되지 않은국가는 맨처음의 광산아이템유니크만 반환한다.(한종류만 채광 가능)
				return m_pstMiningData[ i ]->siMineItemUnique[0];
			}	
		}
	}

	return 0;
}

SI16 CMiningMgr::GetMineItemUniqueRandom( SI16 VillageUnique )
{
	// 랜덤으로 9개의 광물중에 하나가 나오도록 한다.

	SI32 siSlotUnique	=	0	;
	for ( SI16 i = 0; i < MAX_MININGDATA_NUM; ++i )
	{
		if(m_pstMiningData[ i ]	==	NULL)	return	0;
		if ( m_pstMiningData[ i ]->siVillageUnique == VillageUnique )
		{
			siSlotUnique = i	;
			break;
		}
	}
	SI32 randdata = rand() % 100;
	for ( SI16 i = 0; i < MAX_MINEITEM_NUM; ++i )
	{
		if(randdata < 11)
			{
				return m_pstMiningData[ siSlotUnique ]->siMineItemUnique[0];
			}
			else if (randdata < 22) 
			{
				return m_pstMiningData[ siSlotUnique ]->siMineItemUnique[1];
			}
			else if (randdata < 33) 
			{
				return m_pstMiningData[ siSlotUnique ]->siMineItemUnique[2];
			}
			else if (randdata < 44) 
			{
				return m_pstMiningData[ siSlotUnique ]->siMineItemUnique[3];
			}
			else if (randdata < 55) 
			{
				return m_pstMiningData[ siSlotUnique ]->siMineItemUnique[4];
			}
			else if (randdata < 66) 
			{
				return m_pstMiningData[ siSlotUnique ]->siMineItemUnique[5];
			}
			else if (randdata < 77) 
			{
				return m_pstMiningData[ siSlotUnique ]->siMineItemUnique[6];
			}
			else if (randdata < 88) 
			{
				return m_pstMiningData[ siSlotUnique ]->siMineItemUnique[7];
			}
			else if (randdata <= 100) 
			{
				return m_pstMiningData[ siSlotUnique ]->siMineItemUnique[8];
			}
	}
	

	//for ( SI16 i = 0; i < MAX_MININGDATA_NUM; ++i )
	//{
	//	if ( m_pstMiningData[ i ] )
	//	{
	//		if(randdata < 11)
	//		{
	//			return m_pstMiningData[ i ]->siMineItemUnique[0];
	//		}
	//		else if (randdata < 22) 
	//		{
	//			return m_pstMiningData[ i ]->siMineItemUnique[1];
	//		}
	//		else if (randdata < 33) 
	//		{
	//			return m_pstMiningData[ i ]->siMineItemUnique[2];
	//		}
	//		else if (randdata < 44) 
	//		{
	//			return m_pstMiningData[ i ]->siMineItemUnique[3];
	//		}
	//		else if (randdata < 55) 
	//		{
	//			return m_pstMiningData[ i ]->siMineItemUnique[4];
	//		}
	//		else if (randdata < 66) 
	//		{
	//			return m_pstMiningData[ i ]->siMineItemUnique[5];
	//		}
	//		else if (randdata < 77) 
	//		{
	//			return m_pstMiningData[ i ]->siMineItemUnique[6];
	//		}
	//		else if (randdata < 88) 
	//		{
	//			return m_pstMiningData[ i ]->siMineItemUnique[7];
	//		}
	//		else if (randdata <= 100) 
	//		{
	//			return m_pstMiningData[ i ]->siMineItemUnique[8];
	//		}
	//	}
	//}

	return 0;
}

SI16 CMiningMgr::GetMineItemUnique(SI16 *MiningKindList)
{
	//(*MiningKindList) = new SI16[ MAX_MININGDATA_NUM ];

	memset( MiningKindList, 0, sizeof( SI16 ) * MAX_MININGDATA_NUM );

	SI16 Counter = 0;
	for ( SI16 i = 0; i < MAX_MININGDATA_NUM; ++i )
	{
		if ( m_pstMiningData[ i ] != NULL )
		{
			for(SI16 j = 0; j < 9 ; j++)
			{
				MiningKindList[ Counter ] = m_pstMiningData[i]->siMineItemUnique[j];
				++Counter;
			}
		}
	}

	return Counter;
}