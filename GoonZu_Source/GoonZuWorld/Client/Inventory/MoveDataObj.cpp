//#include "stdafx.h"
#include "MoveDataObj.h"

CMoveDataObj::CMoveDataObj( stMoveData *pMoveData )
{
	memcpy( &m_stMoveData, pMoveData, sizeof( stMoveData ) );
	m_pPre = NULL;
	m_pNext = NULL;

	m_siSize = 0;
}

CMoveDataObj::CMoveDataObj( BYTE *pData, SI32 siSize )
{
	SetData( pData, siSize );
	m_pPre = NULL;
	m_pNext = NULL;
}


CMoveDataObj::~CMoveDataObj()
{
	m_pPre = NULL;
	m_pNext = NULL;
}

CMoveDataObj* CMoveDataObj::GetPre()
{
	return m_pPre;
}

void CMoveDataObj::SetPre( CMoveDataObj *pObj )
{
	m_pPre = pObj;
	return;
}

CMoveDataObj* CMoveDataObj::GetNext()
{
	return m_pNext;
}

void CMoveDataObj::SetNext( CMoveDataObj *pObj )
{
	m_pNext = pObj;
	return;
}

stMoveData* CMoveDataObj::GetMoveData()
{
	return &m_stMoveData;
}

void CMoveDataObj::GetData( BYTE *pData )
{
	memcpy( pData, m_pData, m_siSize );
	return;
}

void CMoveDataObj::SetData( BYTE *pData, SI32 siSize )
{
	memcpy( m_pData, pData, siSize );
	m_siSize = siSize;

	return;
}