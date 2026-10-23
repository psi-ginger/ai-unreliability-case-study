#include "..\..\Common\CommonHeader.h"
#include "..\Server.h"
#include "MinisterBase.h"

CMinisterBase::CMinisterBase()
{
	memset(m_szAccountID,	0 , sizeof(m_szAccountID));		// 계정
	memset(m_szPersonName,	0 , sizeof(m_szPersonName));	// PERSON NAME
	memset(m_szGreeting,	0 , sizeof(m_szGreeting));		// 인사말 
}



void CMinisterBase::Set(CMinisterBase *pMinister)
{

	StringCchCopy(m_szAccountID, MAX_PLAYER_NAME, 	pMinister->m_szAccountID);
	StringCchCopy(m_szPersonName,	MAX_PLAYER_NAME, pMinister->m_szPersonName);
	StringCchCopy(m_szGreeting,	256, pMinister->m_szGreeting);
}

