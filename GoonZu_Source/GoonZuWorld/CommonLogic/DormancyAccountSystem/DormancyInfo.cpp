#include ".\dormancyInfo.h"

// 持失切
CDormancyInfo::CDormancyInfo()
{
	ZeroMemory(this, sizeof(CDormancyInfo));
}
// 社瑚切
CDormancyInfo::~CDormancyInfo(void)
{

}


VOID	CDormancyInfo::Set( CDormancyInfo	*pclDormancyInfo )
{
	if ( NULL == pclDormancyInfo )
	{
		return;
	}
	memcpy( this, pclDormancyInfo, sizeof(CDormancyInfo) );
	
	this->m_StartedDate.wHour = 0;
	this->m_StartedDate.wMinute = 0;
	this->m_StartedDate.wSecond = 0;
}


VOID	CDormancyInfo::SetDormancyPoint(SI32 siDormancyPoint)
{
	m_siDormancyPoint = siDormancyPoint;
}
VOID	CDormancyInfo::SetRemainedDormancyPlayTime(SI32 siRemainedDormancyPlayTime)
{
	m_siRemainedDormancyPlayTime = siRemainedDormancyPlayTime;
}
VOID	CDormancyInfo::SetDormancyState(BOOL bDormancyState)
{
	m_bDormancyState = bDormancyState;
}
VOID	CDormancyInfo::SetGiveDormancyEquipItem(BOOL bGiveDormancyEquipItem)
{
	m_bGiveDormancyEquipItem = bGiveDormancyEquipItem;
}

SI32	CDormancyInfo::GetDormancyPoint()
{
	return m_siDormancyPoint;
}
SI32	CDormancyInfo::GetRemainedDormancyPlayTime()
{
	return m_siRemainedDormancyPlayTime;
}
BOOL	CDormancyInfo::GetDormancyState()
{
	return m_bDormancyState;
}

BOOL	CDormancyInfo::GetGiveDormancyEquipItem()
{
	return m_bGiveDormancyEquipItem;
}

VOID	CDormancyInfo::IncreasePoint()
{
	m_siDormancyPoint++;
}
VOID	CDormancyInfo::CalcPoint(SI32 siPoint)
{
	m_siDormancyPoint = m_siDormancyPoint + siPoint;
}
VOID	CDormancyInfo::IncreasePlayTime()
{
	m_siRemainedDormancyPlayTime++;
}

SI32*	CDormancyInfo::GetDormancyPointPointer()
{
	return &m_siDormancyPoint;
}
SI32*	CDormancyInfo::GetRemainedDormancyPlayTimePointer()
{
	return &m_siRemainedDormancyPlayTime;
}
BOOL*	CDormancyInfo::GetDormancyStatePointer()
{
	return &m_bDormancyState;
}
BOOL*	CDormancyInfo::GetGiveDormancyEquipItemPointer()
{
	return &m_bGiveDormancyEquipItem;
}
_SYSTEMTIME*	CDormancyInfo::GetStartedDate()
{
	return &m_StartedDate;
}