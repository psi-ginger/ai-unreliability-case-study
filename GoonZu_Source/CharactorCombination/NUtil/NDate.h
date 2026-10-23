#ifndef _NDATE_H
#define _NDATE_H

#include <windows.h>
#include "ndatatypes.h"

class NDate 
{
public:
	NDate();
	~NDate();

	void		SetDate( int gday );
	void		SetDate( UI16 year, UI08 month, UI08 day );
	SI32		GetDateVary();

	void		AddMonths( int months );
	void		AddDays( int days );

public:
	static int	s_uiDaysOfMonth[ 13 ];

	UI16		m_uiYear;
	UI08		m_uiMonth;
	UI08		m_uiDay;
};

void Sub_SystemTime( SYSTEMTIME time1, SYSTEMTIME time2, SYSTEMTIME *timeResult );
void Add_SystemTime( SYSTEMTIME time1, SYSTEMTIME time2, SYSTEMTIME *timeResult );
void MakeTimeText( SYSTEMTIME *time, char *szText );
void MakeDateText( SYSTEMTIME *time, char *szText );

#endif