#include "NPCTradeData.h"

CNPCTradeData::CNPCTradeData( stNPCTradeData *pNPCTradeData )
{
	memcpy( &m_stNPCTradeData, pNPCTradeData, sizeof( m_stNPCTradeData ) );
	m_pPre = NULL;
	m_pNext = NULL;
}

CNPCTradeData::~CNPCTradeData()
{
}

CNPCTradeData* CNPCTradeData::GetPre()
{
	return m_pPre;
}

void CNPCTradeData::SetPre( CNPCTradeData * pTradeData )
{
	m_pPre = pTradeData;
	return;
}
	
CNPCTradeData* CNPCTradeData::GetNext()
{
	return m_pNext;
}

void CNPCTradeData::SetNext( CNPCTradeData * pTradeData )
{
	m_pNext = pTradeData;
	return;
}

stNPCTradeData*	CNPCTradeData::GetTradeData()
{
	return &m_stNPCTradeData;
}