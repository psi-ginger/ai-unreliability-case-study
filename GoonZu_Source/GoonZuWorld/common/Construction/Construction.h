#ifndef _CONSTRUCTION_H_
#define _CONSTRUCTION_H_

#include "../CommonHeader.h"

#include "../../NLib/NArray.h"

class CConstruction
{
public:
	CConstruction();
	~CConstruction();

	void	Initialize();
	void	Destroy();

	void	ReadStructureInfoData();

	void	Set( SI16 *psiUniqueConstructionID );					// 완전히 전체에 데이터를 삽입( 초기화 시에 DB 에서 데이터 받아올때만 사용 함 ), 아래의 함수를 사용해도 됨
	bool	Set( SI16 siUniqueConstructionID, SI16 siIndex );		// 건물 추가시에 사용
	
	bool	Del( SI16 siIndex );
	
	SI16*	Get();
	SI16	Get( SI16 siIndex );

	SI32	GetSlotFromStrUnique( SI32 strunique );
	
	bool	IsExist( SI16 siIndex );
	bool	IsDuplicate( SI16 siUniqueConstructionID );

	BOOL	IsThereStructure( SI16 ranktype ) { return m_bStructure[ ranktype ]; }

	bool	GetPosFromStrUnique( SI32 strunique, SI32* xpos, SI32 *ypos );
	
private:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, MAX_CONSTRUCTION_NUM>		m_siUniqueConstructionID;
	NSafeTArray<BOOL, MAX_RANKTYPE_NUMBER>		m_bStructure;
#else
	SI16	m_siUniqueConstructionID[ MAX_CONSTRUCTION_NUM ];			// 건물 Unique ID들 집합
	BOOL	m_bStructure[ MAX_RANKTYPE_NUMBER ];						// RankType별 건물 존재여부
#endif

};

#endif