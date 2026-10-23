#include "HouseUnitBuffer.h"
#include "HouseUnitDataObj.h"

CHouseUnitBuffer::CHouseUnitBuffer()
{	
	Initialize();
}

CHouseUnitBuffer::~CHouseUnitBuffer()
{
	Destroy();	
}

void CHouseUnitBuffer::Initialize()
{
	m_siHouseUnitDataObjCount = 0;
	m_pFirst = NULL;
	m_pLast = NULL;

	return;
}

void CHouseUnitBuffer::Destroy()
{
	CHouseUnitDataObj *pTemp = m_pFirst;
	CHouseUnitDataObj *pTemp1 = NULL;
	
	while ( pTemp )
	{
		pTemp1 = pTemp;
		pTemp = pTemp->GetNext();
		delete pTemp1;
		pTemp1 = NULL;
		--m_siHouseUnitDataObjCount;
	}

	m_pFirst = NULL;
	m_pLast = NULL;

	m_siHouseUnitDataObjCount = 0;

	return;
}

void CHouseUnitBuffer::Push( SI16 Msg, SI32 lParam,  SI32 si32VillageIndex, SI32 si32HouseIndex, cltItem clItem )
{
	stHouseUnitData temp;
	
	temp.si16Msg = Msg;
	temp.si32lParam1 = lParam;
	temp.si32VillageUnique = si32VillageIndex;
	temp.si32HouseUnitSlot = si32HouseIndex;
	temp.clItem = clItem;
	

	CHouseUnitDataObj *pTemp = new CHouseUnitDataObj( &temp );
	
	if ( m_pFirst )
	{
		m_pLast->SetNext( pTemp );
		pTemp->SetPre( m_pLast );
		m_pLast = pTemp;
	}
	else
	{
		m_pFirst = pTemp;
		m_pLast = pTemp;
	}

	++m_siHouseUnitDataObjCount;

	return;
}


void CHouseUnitBuffer::Push( SI16 Msg, SI32 lParam, SI32 lParam2, SI32 si32VillageIndex, SI32 si32HouseIndex, cltItem *pclItem )
{
	stHouseUnitData temp;
	
	temp.si16Msg = Msg;
	temp.si32lParam1 = lParam;
	temp.si32lParam2 = lParam2;
	temp.si32VillageUnique = si32VillageIndex;
	temp.si32HouseUnitSlot = si32HouseIndex;
	if ( pclItem == NULL )
	{
	}
	else
	{
		memcpy( &temp.clItem, pclItem, sizeof( cltItem ) );
	}
	

	CHouseUnitDataObj *pTemp = new CHouseUnitDataObj( &temp );
	
	if ( m_pFirst )
	{
		m_pLast->SetNext( pTemp );
		pTemp->SetPre( m_pLast );
		m_pLast = pTemp;
	}
	else
	{
		m_pFirst = pTemp;
		m_pLast = pTemp;
	}

	++m_siHouseUnitDataObjCount;

	return;
}

void CHouseUnitBuffer::Push( SI16 Msg, cltRentContract clRentContract, SI32 si32VillageIndex, SI32 si32HouseIndex, cltItem *pclItem )
{

	stHouseUnitData temp;
	
	temp.si16Msg = Msg;
	temp.clContract = clRentContract;	
	temp.si32VillageUnique = si32VillageIndex;
	temp.si32HouseUnitSlot = si32HouseIndex;
	
	CHouseUnitDataObj *pTemp = new CHouseUnitDataObj( &temp );
	
	if ( m_pFirst )
	{
		m_pLast->SetNext( pTemp );
		pTemp->SetPre( m_pLast );
		m_pLast = pTemp;
	}
	else
	{
		m_pFirst = pTemp;
		m_pLast = pTemp;
	}

	++m_siHouseUnitDataObjCount;


	return;
}


bool CHouseUnitBuffer::Pop( stHouseUnitData *pHouseUnitData )
{
	if ( m_siHouseUnitDataObjCount == 0 )
	{
		return false;
	}	

	CHouseUnitDataObj *pTemp = m_pFirst;
	memcpy( pHouseUnitData, pTemp->GetHouseUnitData(), sizeof( stHouseUnitData ) );
	
	if ( m_siHouseUnitDataObjCount > 1 )
	{
		m_pFirst->GetNext()->SetPre( NULL );
		m_pFirst = m_pFirst->GetNext();
	}
	else
	{
		m_pFirst = NULL;
		m_pLast = NULL;
	}

	--m_siHouseUnitDataObjCount;
	
	delete pTemp;
	pTemp = NULL;

	return true;
}