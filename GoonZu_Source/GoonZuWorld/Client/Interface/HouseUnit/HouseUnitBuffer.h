#ifndef _HOUSEUNITBUFFER_H_
#define _HOUSEUNITBUFFER_H_

#include "HouseUnitBaseInfo.h"

class CHouseUnitDataObj;

class cltRentContract;

class CHouseUnitBuffer
{
public:
	CHouseUnitBuffer();
	~CHouseUnitBuffer();

	void	Initialize();
	void	Destroy();

	void	Push( SI16 Msg, SI32 lParam, SI32 si32VillageIndex, SI32 si32HouseIndex, cltItem clItem );	
	void	Push( SI16 Msg, SI32 lParam1, SI32 lParam2, SI32 si32VillageIndex, SI32 si32HouseIndex, cltItem *pclItem );
	void	Push( SI16 Msg, cltRentContract clRentContract, SI32 si32VillageIndex, SI32 si32HouseIndex, cltItem *pclItem );
	bool	Pop( stHouseUnitData *pHouseUnitData );

private:

	CHouseUnitDataObj	*m_pFirst;
	CHouseUnitDataObj	*m_pLast;

	SI32				m_siHouseUnitDataObjCount;	

};


#endif