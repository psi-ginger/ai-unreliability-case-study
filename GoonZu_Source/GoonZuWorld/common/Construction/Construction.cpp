#include "Construction.h"

#include "..\CommonLogic\CommonLogic.h"

#include "Map\FieldObject\FieldObject.h"

extern cltCommonLogic *pclClient;

CConstruction::CConstruction()
{
	Initialize();
}

CConstruction::~CConstruction()
{
	Destroy();
}

void CConstruction::Initialize()
{
#ifdef _SAFE_MEMORY
	m_siUniqueConstructionID.ZeroMem();
#else
	memset( m_siUniqueConstructionID, 0, sizeof( m_siUniqueConstructionID ) );
#endif
	
	for( int i = 0; i < MAX_RANKTYPE_NUMBER; ++i )
	{
		m_bStructure[ i ] = FALSE;
	}

//	ReadStructureInfoData();

	return;
}

void CConstruction::Destroy()
{
	return;
}

void CConstruction::Set( SI16 *psiUniqueConstructionID )
{
#ifdef _SAFE_MEMORY
	for(SI32 i=0; i<MAX_CONSTRUCTION_NUM; i++)		m_siUniqueConstructionID[i] = psiUniqueConstructionID[i];
	m_bStructure.ZeroMem();
#else
	memcpy( m_siUniqueConstructionID, psiUniqueConstructionID, sizeof( SI16 ) * MAX_CONSTRUCTION_NUM );
	ZeroMemory( m_bStructure, sizeof( BOOL ) * MAX_RANKTYPE_NUMBER );
#endif


	int ranktype;
	for( int i = 0; i < MAX_CONSTRUCTION_NUM; ++i ) {

		if( m_siUniqueConstructionID[ i ] != 0 ) {
			ranktype = pclClient->pclVillageStructureSet->GetRankTypeFromStrUnique( m_siUniqueConstructionID[ i ] );

			m_bStructure[ ranktype ] = TRUE;
		}
	}

	return;
}

// siIndex : 건물 위치 
// UniqueConstructionID : 건물 고유번호 (ranktype아님)

bool CConstruction::Set( SI16 siUniqueConstructionID, SI16 siIndex )
{
	if( siIndex < 0 || siIndex >= MAX_CONSTRUCTION_NUM )		return false;

	if ( IsExist( siIndex ) )
	{
		return false;
	}

	if ( IsDuplicate( siUniqueConstructionID ) )
	{
		return false;
	}

	m_siUniqueConstructionID[ siIndex ] = siUniqueConstructionID;

	int ranktype = pclClient->pclVillageStructureSet->GetRankTypeFromStrUnique( siUniqueConstructionID );

	if(ranktype >= 0 && ranktype < MAX_RANKTYPE_NUMBER)
		m_bStructure[ ranktype ] = TRUE;

	return true;
}

bool CConstruction::Del( SI16 siIndex )
{
	if ( IsExist( siIndex ) )
	{

		int ranktype = pclClient->pclVillageStructureSet->GetRankTypeFromStrUnique( m_siUniqueConstructionID[ siIndex ] );

		m_bStructure[ ranktype ] = FALSE;		

		m_siUniqueConstructionID[ siIndex ] = 0;
		return true;
	}

	return false;
}


SI16* CConstruction::Get()
{
	return &m_siUniqueConstructionID[0];
}


SI16 CConstruction::Get( SI16 siIndex )
{
	return m_siUniqueConstructionID[ siIndex ];
}

bool CConstruction::IsExist( SI16 siIndex )
{
	if ( m_siUniqueConstructionID[ siIndex ] == 0 )
	{
		return false;
	}

	return true;
}

bool CConstruction::IsDuplicate( SI16 siUniqueConstructionID )
{
	for ( SI16 i = 0; i < MAX_CONSTRUCTION_NUM; ++i )
	{
		if ( m_siUniqueConstructionID[ i ] == siUniqueConstructionID )
		{
			return true;
		}
	}

	return false;
}


SI32 CConstruction::GetSlotFromStrUnique( SI32 strunique )
{
	for( int i = 0; i < MAX_CONSTRUCTION_NUM; ++i )
	{
		if ( m_siUniqueConstructionID[ i ] == strunique )
		{
			return i;
		}
	}

	return -1;
}

bool CConstruction::GetPosFromStrUnique( SI32 strunique, SI32* xpos, SI32* ypos )
{
	for( int i = 0 ; i < MAX_CONSTRUCTION_NUM; ++i )
	{
		if ( m_siUniqueConstructionID[i] == strunique )
		{
			switch(i)
			{
			case 0:
				*xpos = MAPTYPE_STRUCTURE_POINT_X_SLOT0;
				*ypos = MAPTYPE_STRUCTURE_POINT_Y_SLOT0;
				return true;
			case 1:
				*xpos = MAPTYPE_STRUCTURE_POINT_X_SLOT1;
				*ypos = MAPTYPE_STRUCTURE_POINT_Y_SLOT1;
				return true;
			case 2:
				*xpos = MAPTYPE_STRUCTURE_POINT_X_SLOT2;
				*ypos = MAPTYPE_STRUCTURE_POINT_Y_SLOT2;
				return true;
			case 3:
				*xpos = MAPTYPE_STRUCTURE_POINT_X_SLOT3;
				*ypos = MAPTYPE_STRUCTURE_POINT_Y_SLOT3;
				return true;
			case 4:
				*xpos = MAPTYPE_STRUCTURE_POINT_X_SLOT4;
				*ypos = MAPTYPE_STRUCTURE_POINT_Y_SLOT4;
				return true;
			case 5:
				*xpos = MAPTYPE_STRUCTURE_POINT_X_SLOT5;
				*ypos = MAPTYPE_STRUCTURE_POINT_Y_SLOT5;
				return true;
			case 6:
				*xpos = MAPTYPE_STRUCTURE_POINT_X_SLOT6;
				*ypos = MAPTYPE_STRUCTURE_POINT_Y_SLOT6;
				return true;
			case 7:
				*xpos = MAPTYPE_STRUCTURE_POINT_X_SLOT7;
				*ypos = MAPTYPE_STRUCTURE_POINT_Y_SLOT7;
				return true;
			case 8:
				*xpos = MAPTYPE_STRUCTURE_POINT_X_SLOT8;
				*ypos = MAPTYPE_STRUCTURE_POINT_Y_SLOT8;
				return true;
			case 9:
				*xpos = MAPTYPE_STRUCTURE_POINT_X_SLOT9;
				*ypos = MAPTYPE_STRUCTURE_POINT_Y_SLOT9;
			}
		}
	}

	return false;
}