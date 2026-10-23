#include "HouseUnitDataObj.h"

CHouseUnitDataObj::CHouseUnitDataObj( stHouseUnitData *pHouseUnitData )
{
	memcpy( &m_stHouseUnitData, pHouseUnitData, sizeof( m_stHouseUnitData ) );
	m_pPre = NULL;
	m_pNext = NULL;
}

CHouseUnitDataObj::~CHouseUnitDataObj()
{
}

CHouseUnitDataObj* CHouseUnitDataObj::GetPre()
{
	return m_pPre;
}

void CHouseUnitDataObj::SetPre( CHouseUnitDataObj * pTradeData )
{
	m_pPre = pTradeData;
	return;
}
	
CHouseUnitDataObj* CHouseUnitDataObj::GetNext()
{
	return m_pNext;
}

void CHouseUnitDataObj::SetNext( CHouseUnitDataObj * pTradeData )
{
	m_pNext = pTradeData;
	return;
}

stHouseUnitData* CHouseUnitDataObj::GetHouseUnitData()
{
	return &m_stHouseUnitData;
}