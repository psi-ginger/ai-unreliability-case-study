#include "Marriage.h"

SI16 CMarriage::CalcNowLoveLevel( SI32 siNowHourVary )
{
	// 이건 값이 잘못 들어왔음
	if ( 0 >= siNowHourVary )
	{
		return m_siLoveLevel;
	}

	if ( 0 >= m_siLimitHourVary )
	{
		return m_siLoveLevel;
	}

	// 제한 시간에서 하루(24시간을 빼준다)
	SI32 siDiffHour = siNowHourVary - (m_siLimitHourVary - 24);
	if ( 0 >= siDiffHour )
	{
		return m_siLoveLevel;
	}

	// 하루를 기준으로 나눈다
	SI32 siDivHour = (SI32)(siDiffHour / 24);
	if ( 0 >= siDivHour )
	{
		return m_siLoveLevel;
	}

	// 게임시간 하루에 2단계씩 떨어진다
	SI16 siCalcLoveLevel = m_siLoveLevel - ( 2 * siDivHour );
	if ( 0 >= siCalcLoveLevel )
	{
		// 결혼했을때는 1로
		if ( TRUE == IsMarried() )
		{
			siCalcLoveLevel = MARRIAGE_LOVE_LEVEL_01;
		}
		else
		{
			siCalcLoveLevel = 0;
		}
	}

	return siCalcLoveLevel;
}

BOOL CMarriage::IsWeddingAnniversary( SI32 siNowDateVary )
{
	if ( 0 >= siNowDateVary )
	{
		return FALSE;
	}

	if ( 0 >= m_siWeddingDateVary )
	{
		return FALSE;
	}

	cltDate clNowDate;
	clNowDate.MakeFullDateFromDateVary( siNowDateVary );

	cltDate clWeddingDate;
	clWeddingDate.MakeFullDateFromDateVary( m_siWeddingDateVary );

	// 우선 결혼한 다음 년도 부터 결혼 기념일을 계산할수 있다
	if ( (clWeddingDate.uiYear < clNowDate.uiYear) 
		&& (clWeddingDate.uiMonth == clNowDate.uiMonth) 
		&& (clWeddingDate.uiDay == clNowDate.uiDay) )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CMarriage::IsWeddingItemGet( SI32 siNowDateVary )
{
	if ( 0 >= siNowDateVary )
	{
		return FALSE;
	}

	if ( 0 >= m_siItemGetDateVary )
	{
		return FALSE;
	}

	cltDate clNowDate;
	clNowDate.MakeFullDateFromDateVary( siNowDateVary );

	cltDate clItemGetDate;
	clItemGetDate.MakeFullDateFromDateVary( m_siItemGetDateVary );

	// 년도,달,일 모두 같다면 이미 받은 것이다
	if ( (clNowDate.uiYear == clItemGetDate.uiYear)
		&& (clNowDate.uiMonth == clItemGetDate.uiMonth)
		&& (clNowDate.uiDay == clItemGetDate.uiDay) )
	{
		return TRUE;
	}

	return FALSE;
}

SI32 CMarriage::GetRemainHourVary( SI32 siNowHourVary )
{
	if ( 0 >= siNowHourVary )
	{
		return 0;
	}

	if ( 0 >= m_siLimitHourVary )
	{
		return 0;
	}

	SI32 siRemainHourVary = m_siLimitHourVary - siNowHourVary;
	if ( 0 > siRemainHourVary )
	{
		siRemainHourVary = 0;
	}

	return siRemainHourVary;

}