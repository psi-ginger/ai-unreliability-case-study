#include "ndate.h"
#include "stdio.h"

int NDate::s_uiDaysOfMonth[13];

NDate::NDate()
{
	int days[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	
	for( int i = 0; i < 13; ++i ) 
	{
		s_uiDaysOfMonth[ i ] = days[ i ];
	}

}

NDate::~NDate()
{
}

void NDate::SetDate( UI16 year, UI08 month, UI08 day )
{
	m_uiYear = year;
	m_uiMonth = month;
	m_uiDay = day;
}

void NDate::SetDate( int gday )
{
	UI16 year1, year2;
	
	year1 = ( gday / (365 * 400 + 97) ) * 400;
	gday = gday % (365 * 400 + 97);

	year2 = gday / 365;
	gday = gday % 365;

	gday -= ( year2 / 4 ) - ( year2 / 100 ) + ( year2 / 400 );

	if( gday <= 0 ) {
		gday += 365 + ((year2 % 4) == 0 ) - ((year2 % 100) == 0 ) + ((year2 % 400) == 0 );
		year2--;
	}

	m_uiYear = year1 + year2 + 1;

	s_uiDaysOfMonth[ 2 ] += ((m_uiYear % 4) == 0 ) - ((m_uiYear % 100) == 0 ) + ((m_uiYear % 400) == 0 );

	for( int i = 1; i < 12; ++i ) {
		
		if( gday <= s_uiDaysOfMonth[ i ] ) break;

		gday -= s_uiDaysOfMonth[ i ];
	}

	m_uiMonth = (UI08)i;
	m_uiDay = (UI08)gday;

	s_uiDaysOfMonth[ 2 ] = 28;
}

SI32 NDate::GetDateVary()
{
	UI16 year = m_uiYear - 1;
	int gday = year * 365;

	s_uiDaysOfMonth[ 2 ] += ((m_uiYear % 4) == 0 ) - ((m_uiYear % 100) == 0 ) + ((m_uiYear % 400) == 0 );

	gday += ( year / 4 ) - ( year / 100 ) + ( year / 400 );

	for( int i = 1; i < m_uiMonth; ++i ) 
	{
		gday += s_uiDaysOfMonth[ i ];
	}

	gday += m_uiDay;

	s_uiDaysOfMonth[ 2 ] = 28;

	return gday;
}

void NDate::AddMonths( int months )
{
	months += m_uiMonth;

	if( months > 0 ) {

		if( months > 12 ) {
			m_uiYear += (UI16)( ( months - 1 ) / 12 );
		}

		m_uiMonth = (UI08)( (( months - 1 ) % 12 ) + 1 );

	} else {

		m_uiYear += (UI16)( ( months - 12 ) / 12 );

		m_uiMonth = (UI08)( 12 + ((months - 12 ) % 12 ) );
	}

}

void NDate::AddDays( int days )
{
	SetDate( GetDateVary() + days );
}


void Sub_SystemTime( SYSTEMTIME time1, SYSTEMTIME time2, SYSTEMTIME *timeResult )
{
	ZeroMemory( timeResult, sizeof( *timeResult ) );

	NDate date1, date2, dateResult;

	date1.SetDate( time1.wYear, time1.wMonth, time1.wDay );
	date2.SetDate( time2.wYear, time2.wMonth, time2.wDay );

	int datevary1 = date1.GetDateVary();
	int datevary2 = date2.GetDateVary();
	int datevaryResult;

	timeResult->wMilliseconds = 0;

	// 초단위 계산
	if( time1.wSecond < time2.wSecond ) {
		
		time2.wMinute++; time1.wSecond += 60;
	}	
	timeResult->wSecond = time1.wSecond - time2.wSecond;

	// 분단위 계산
	if( time1.wMinute < time2.wMinute ) {
		time2.wHour++; time1.wMinute += 60;
	}
	timeResult->wMinute = time1.wMinute - time2.wMinute;

	// 시단위 계산
	if( time1.wHour < time2.wHour ) {
		datevary1--; time1.wHour += 24;
	}
	timeResult->wHour = time1.wHour - time2.wHour;

	datevaryResult = datevary1 - datevary2;

	timeResult->wDay = datevaryResult;
}


void Add_SystemTime( SYSTEMTIME time1, SYSTEMTIME time2, SYSTEMTIME *timeResult )
{
	ZeroMemory( timeResult, sizeof( *timeResult ) );

	NDate date1, date2, dateResult;

	date1.SetDate( time1.wYear, time1.wMonth, time1.wDay );
	date2.SetDate( time2.wYear, time2.wMonth, time2.wDay );

	int datevary1 = date1.GetDateVary();
	int datevary2 = date2.GetDateVary();
	int datevaryResult = 0;

	int temp;

	timeResult->wMilliseconds = 0;

	// 초단위 계산
	timeResult->wSecond = time1.wSecond + time2.wSecond;

	if( timeResult->wSecond >= 60 ) {
		
		temp = timeResult->wSecond / 60;

		timeResult->wSecond -= ( 60 * temp );
		timeResult->wMinute += temp;
	}	
	

	// 분단위 계산
	timeResult->wMinute += time1.wMinute + time2.wMinute;

	if( timeResult->wMinute >= 60 ) {

		temp = timeResult->wMinute / 60;

		timeResult->wMinute -= ( 60 * temp );
		timeResult->wHour += temp;
	}
	
	// 시단위 계산
	timeResult->wHour += time1.wHour + time2.wHour;
	if( timeResult->wHour >= 24 ) {

		temp = timeResult->wHour / 24;

		timeResult->wHour -= ( 24 * temp );
		datevaryResult += temp;
	}

	datevaryResult += time1.wDay + time2.wDay;

	timeResult->wDay = datevaryResult;
}

void MakeTimeText( SYSTEMTIME *time, char *szText )
{
	sprintf( szText, "%d:%02d:%02d", time->wDay * 24 + time->wHour, time->wMinute, time->wSecond );
}

void MakeDateText( SYSTEMTIME *time, char *szText )
{
	sprintf( szText, "%04d-%02d-%02d %02d:%02d:%02d", 
		time->wYear, time->wMonth, time->wDay,
		time->wHour, time->wMinute, time->wSecond );
}