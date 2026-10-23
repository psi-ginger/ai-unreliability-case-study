#include "MacroReportedMgr.h"

cltMacroReportedMgr::cltMacroReportedMgr()
{
	ZeroMemory( m_stMacroReportedUser, sizeof( m_stMacroReportedUser ) );

}

cltMacroReportedMgr::~cltMacroReportedMgr()
{
}

VOID	cltMacroReportedMgr::Initialize()
{
	ZeroMemory( m_stMacroReportedUser, sizeof( m_stMacroReportedUser ) );
}	

VOID	cltMacroReportedMgr::Free()
{
	ZeroMemory( m_stMacroReportedUser, sizeof( m_stMacroReportedUser ) );
}

VOID	cltMacroReportedMgr::OnDayChanged()
{
	ZeroMemory( m_stMacroReportedUser, sizeof( m_stMacroReportedUser ) );
}

BOOL	cltMacroReportedMgr::CanUserReport( SI32 siPersonID )
{
	SI32	i;

	for( i = 0; i < MACROREPORTEDMGR_MAX_MANAGED_USER_NUM; i++ )
	{
		if( m_stMacroReportedUser[ i ].siPersondID == 0	 )			break;

		if( m_stMacroReportedUser[ i ].siPersondID != siPersonID )	continue;		

		if( m_stMacroReportedUser[ i ].siReportedNum < MACROREPORTEDMGR_REPORTEDNUM_PERDAY )
		{
            return	1;			
		}
		else
		{
			return	-1;
		}
	}

	if( i == MACROREPORTEDMGR_MAX_MANAGED_USER_NUM )				
	{
		// 오늘은 더이상 새로운 캐릭터를 신고할 수 없다.
		return	0;
	}

	return	1;
}

VOID	cltMacroReportedMgr::OnUserReported( SI32 siPersonID )
{
	SI32	i;
	SI32	siEmptySlot = -1;

	for( i = 0; i < MACROREPORTEDMGR_MAX_MANAGED_USER_NUM; i++ )
	{
		if( m_stMacroReportedUser[ i ].siPersondID == 0	 )			break;

		if( m_stMacroReportedUser[ i ].siPersondID != siPersonID )	continue;		

		m_stMacroReportedUser[ i ].siReportedNum++;

		return;
	}

	if( i == MACROREPORTEDMGR_MAX_MANAGED_USER_NUM )				
	{
		// 오늘은 더이상 새로운 캐릭터를 신고할 수 없다.
		return;
	}

	m_stMacroReportedUser[ i ].siPersondID		=	siPersonID;
	m_stMacroReportedUser[ i ].siReportedNum	=	1;
}


SI32	cltMacroReportedMgr::GetReportedNum( SI32 siPersonID )
{
	SI32	i;

	for( i = 0; i < MACROREPORTEDMGR_MAX_MANAGED_USER_NUM; i++ )
	{
		if( m_stMacroReportedUser[ i ].siPersondID == 0	 )			break;

		if( m_stMacroReportedUser[ i ].siPersondID != siPersonID )	continue;		

		return	m_stMacroReportedUser[ i ].siReportedNum;
	}


	return	0;
}

