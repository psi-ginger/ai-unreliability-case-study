#include "SystemReward.h"

cltSystemRewardInfo::cltSystemRewardInfo()
{
	Init();
}

cltSystemRewardInfo::cltSystemRewardInfo( cltSystemRewardInfo* pclInfo )
{
	Set( pclInfo );
}

cltSystemRewardInfo::~cltSystemRewardInfo()
{

}

void cltSystemRewardInfo::Init()
{
	ZeroMemory( this, sizeof(cltSystemRewardInfo) );
}

void cltSystemRewardInfo::Set( cltSystemRewardInfo* pclInfo )
{
	m_siIndex = pclInfo->m_siIndex;
	m_sGiveTime = pclInfo->m_sGiveTime;
	m_sExpireTime = pclInfo->m_sExpireTime;

	m_siMoney = pclInfo->m_siMoney;
	m_clItem.Set( &pclInfo->m_clItem );
	StringCchCopy( m_szReason, 256, pclInfo->m_szReason );
}