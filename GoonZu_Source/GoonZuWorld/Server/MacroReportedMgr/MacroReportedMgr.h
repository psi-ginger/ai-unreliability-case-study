#pragma once

#include <windows.h>
#include <directives.h>

#define	MACROREPORTEDMGR_MAX_MANAGED_USER_NUM		100
#define	MACROREPORTEDMGR_REPORTEDNUM_PERDAY			3				// 하루에 한 캐릭터가 신고되어질 수 있는 회수

struct	strMacroReportedUser
{
	SI32	siPersondID;			// 신고되어진 캐릭터의 PersonID
	SI32	siReportedNum;			// 신고되어진 회수
};

class cltMacroReportedMgr
{
private:

	strMacroReportedUser	m_stMacroReportedUser[ MACROREPORTEDMGR_MAX_MANAGED_USER_NUM ];	

public:
	cltMacroReportedMgr();
	~cltMacroReportedMgr();

	VOID	Initialize();
	VOID	Free();	
	
	SI32	GetReportedNum( SI32 siPersonID );
		
	/*
	CanUserReport 리턴값 정의
	1	:	성공
	0	:	일반 적인 실패
	-1	:	해당 캐릭터의 당일 신고 회수 초과로 인한 실패
	*/
	SI32	CanUserReport( SI32 siPersonID );
	VOID	OnUserReported( SI32 siPersonID );

	VOID	OnDayChanged();
};
