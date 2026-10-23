#ifndef _HOUSEUNITDATAOBJ_H_
#define _HOUSEUNITDATAOBJ_H_

#include "HouseUnitBaseInfo.h"

class CHouseUnitDataObj
{
public:
	CHouseUnitDataObj( stHouseUnitData *pHouseUnitData );
	~CHouseUnitDataObj();

	CHouseUnitDataObj*	GetPre();
	void				SetPre( CHouseUnitDataObj *pHouseUnitData );
	
	CHouseUnitDataObj*	GetNext();
	void				SetNext( CHouseUnitDataObj *pHouseUnitData );

	stHouseUnitData*	GetHouseUnitData();

private:
	stHouseUnitData		m_stHouseUnitData;

	CHouseUnitDataObj	*m_pPre;
	CHouseUnitDataObj	*m_pNext;

};


#endif