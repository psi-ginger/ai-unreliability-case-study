#include "ITZCell.h"
#include "ITZWorld.h"
#include "ITZList.h"
#include "ITZPerson.h"
#include "ITZSendBuffer.h"

ITZCell::ITZCell()
{
	m_pWorld = NULL;
}

ITZCell::~ITZCell()
{

}

void ITZCell::CreateCell( ITZWorld *pWorld )
{

	m_pWorld = pWorld;

	m_personPtrList.CreateList();

	for( int i = 0; i < 9; ++i ) {

		m_pNearCells[ i ] = NULL;
	}

}

void ITZCell::AddPerson( ITZPerson *pPerson )
{
	ITZObjectPtr *pObjPtr = m_personPtrList.push_from( &m_pWorld->m_personPtrList );

	if( pObjPtr == NULL ) return;

	pObjPtr->SetObject( (ITZObject *)pPerson );
}

void ITZCell::DelPerson( ITZPerson *pPerson )
{

}

void ITZCell::SendPacketNearAll( sPacketHeader *pPacket )
{



}

void ITZCell::SendPacket( sPacketHeader *pPacket )
{
	ITZObjectPtr *pObjPtr;
	
	LIST_LOOP( &m_personPtrList, pObjPtr )	
	{
		((ITZPerson *)(pObjPtr->GetObject()))->GetSendBuffer()->Write( (char *)pPacket );
	}
}





