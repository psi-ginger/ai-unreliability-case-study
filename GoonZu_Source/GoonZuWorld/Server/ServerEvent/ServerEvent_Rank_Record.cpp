#include "ServerEvent_Rank_Record.h"

#include "../CommonLogic/MsgType-System.h"

CNServerEvent_Rank_Record::CNServerEvent_Rank_Record()
{
	Clear();	
}

CNServerEvent_Rank_Record::~CNServerEvent_Rank_Record()
{

}

void CNServerEvent_Rank_Record::Clear(void)
{
	ZeroMemory(this, sizeof(CNServerEvent_Rank_Record));
}

void CNServerEvent_Rank_Record::Destroy(void)
{
	
}

CNServerRankInfo* CNServerEvent_Rank_Record::GetServerRankInfo(SI32 _Index)	
{
	if(false == SAFE_ARRAY_CHECK(_Index, m_clServerRankInfo))
		return NULL;

	return &m_clServerRankInfo[_Index];
}

CNPersonalRankInfo* CNServerEvent_Rank_Record::GetPersonalRankInfo(SI32 _Index)
{
	if(false == SAFE_ARRAY_CHECK(_Index, m_clPersonalRankInfo))
		return NULL;

	return &m_clPersonalRankInfo[_Index];
}

CNServerDailyRankInfo* CNServerEvent_Rank_Record::GetServerDailyRankInfo(SI32 _Index)
{
	if(false == SAFE_ARRAY_CHECK(_Index, m_clServerDailyRankInfo))
		return NULL;

	return &m_clServerDailyRankInfo[_Index];
}

void CNServerEvent_Rank_Record::SetCNServerEvent_Quest(void* _ServerEvent_Quest)
{
	m_pServerEvent_Quest = _ServerEvent_Quest;
}

CNServerEvent_Quest* CNServerEvent_Rank_Record::GetCNServerEvent_Quest(void)
{
	return (CNServerEvent_Quest*)m_pServerEvent_Quest;
}



