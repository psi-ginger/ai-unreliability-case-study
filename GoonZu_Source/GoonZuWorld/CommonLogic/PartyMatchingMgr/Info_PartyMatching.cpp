// 파티매칭
#include "Info_PartyMatching.h"








CInfo_PartyMatching::CInfo_PartyMatching()
{
	m_siLeaderID = -1;
	m_siParentQuest = -1;
	m_siChildQuest = -1;
}

CInfo_PartyMatching::~CInfo_PartyMatching()
{

}

bool CInfo_PartyMatching::CheckCondition()
{
	if ( 0 > m_siLeaderID )						return false;
	if ( 0 > m_siParentQuest )					return false;
	if ( 0 > m_siChildQuest )					return false;
	if ( true == m_strLeaderName.IsEmpty() )	return false;
	if ( true == m_strPartyName.IsEmpty() )		return false;
	// 인사말은 없어도 된다. 체크 안함.

	return true;
}

void CInfo_PartyMatching::SetLeaderID(SI32 siLeaderID)
{
	if ( 0 >= siLeaderID ) return;

	m_siLeaderID = siLeaderID;
}

void CInfo_PartyMatching::SetLeaderName(TCHAR* pszLeaderName)
{
	if ( NULL == pszLeaderName ) return;

	m_strLeaderName = pszLeaderName;
}

void CInfo_PartyMatching::SetPartyName(TCHAR* pszPartyName)
{
	if ( NULL == pszPartyName ) return;

	m_strPartyName = pszPartyName;
}

void CInfo_PartyMatching::SetHelloMessage(TCHAR* pszHelloMessage)
{
	if ( NULL == pszHelloMessage ) return;

	m_strHelloMessage = pszHelloMessage;
}

SI32 CInfo_PartyMatching::GetLeaderID()
{
	return m_siLeaderID;
}

TCHAR* CInfo_PartyMatching::GetLeaderName()
{
	return m_strLeaderName;
}

TCHAR* CInfo_PartyMatching::GetPartyName()
{
	return m_strPartyName;
}

TCHAR* CInfo_PartyMatching::GetHelloMessage()
{
	return m_strHelloMessage;
}

void CInfo_PartyMatching::AddMemberID(SI32 siMemberID)
{
	// 파티원을 추가한다. 
}

void CInfo_PartyMatching::RemoveMemberID(SI32 siMemberID)
{
	// 파티원을 제거한다.
}

void CInfo_PartyMatching::SetParentQuest(SI32 siParentQuest)
{
	if ( 0 > siParentQuest ) return;

	m_siParentQuest = siParentQuest;
}

void CInfo_PartyMatching::SetChildQuest(SI32 siChildQuest)
{
	if ( 0 > siChildQuest ) return;

	m_siChildQuest = siChildQuest;
}