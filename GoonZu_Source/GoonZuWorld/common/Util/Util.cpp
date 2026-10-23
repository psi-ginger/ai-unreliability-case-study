//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include "..\Client\Client.h"

#include "..\MapEdit\mapedit.h"
#include "..\Map\Map.h"
#include "Util.h"

extern inline BOOL IsDBCS( char ch1 );

extern cltCommonLogic* pclClient;

extern	UI08		SqrtVary[];							// 루트 값 
extern	DWORD				SineDegree[];						// 사인값 

static SI08 cTable[15][15] = { {-1, -1, -1, -1, -1,  7,  7, 7,  7,  7, -1, -1, -1, -1, -1},
							{-1, -1, -1,  7,  7,  6,  6, 6,  6,  6,  7,  7, -1, -1, -1},
							{-1, -1,  7,  6,  6,  5,  5, 5,  5,  5,  6,  6,  7, -1, -1},
							{-1,  7,  6,  6,  5,  5,  4, 4,  4,  5,  5,  6,  6,  7, -1},
							{-1,  7,  6,  5,  5,  4,  3, 3,  3,  4,  5,  5,  6,  7, -1},
							{ 7,  6,  5,  5,  4,  3,  2, 2,  2,  3,  4,  5,  5,  6,  7},
							{ 7,  6,  5,  4,  3,  2,  2, 1,  2,  2,  3,  4,  5,  6,  7},
							{ 7,  6,  5,  4,  3,  2,  1, 0,  1,  2,  3,  4,  5,  6,  7},
							{ 7,  6,  5,  4,  3,  2,  2, 1,  2,  2,  3,  4,  5,  6,  7},
							{ 7,  6,  5,  5,  4,  3,  2, 2,  2,  3,  4,  5,  5,  6,  7},
							{-1,  7,  6,  5,  5,  4,  3, 3,  3,  4,  5,  5,  6,  7, -1},
							{-1,  7,  6,  6,  5,  5,  4, 4,  4,  5,  5,  6,  6,  7, -1},
							{-1, -1,  7,  6,  6,  5,  5, 5,  5,  5,  6,  6,  7, -1, -1},
							{-1, -1, -1,  7,  7,  6,  6, 6,  6,  6,  7,  7, -1, -1, -1},
							{-1, -1, -1, -1, -1,  7,  7, 7,  7,  7, -1, -1, -1, -1, -1} };


// 평균 0을 기준으로 확률을 구한다. (basicrate은 백분율) 
//==================================================
// PARAMETER
//==================================================
// basicrate	: 기본 Quality Rate
// brareswitch	: 레어아이템을 제작할 수 있는가?
// addlevel		: 손재주차이(1000자리),	스킬차이(1자리)
//==================================================
SI32 GetAverage(SI32 basicrate, bool brareswitch, SI32 addlevel, bool bCreateHighGrade /* = true */ )
{
	SI32 extraskilllevel = addlevel % 1000;		// 스킬 수치
	// 손재주는 5당 영향을 주게 만든다!!
	SI32 extrahand = (addlevel / 1000) / 5;		// 손재주 수치

	SI32 randx;
	if(pclClient->IsCountrySwitch(Switch_ManufactureRevitalize))
	{
		randx = max(4000, 10000 - ( (100*extraskilllevel) + (100*extrahand) ));		
	}
	else
	{
		randx = max(4000, 10000 - ( (100*extraskilllevel) + extrahand ));
	}
	// -randx ~ +randx : 랜덤수치를 구함
	SI32 x = randx - (rand() % ((randx*2)+1) );

	//	SI32 templimit = randx/4;
	SI32 templimit = 0;
	SI32 y = 0;
	SI32 high = 0;

#ifdef _CHINA
	high = 0;
#else
	high = 350;
#endif

	if( 0 == extraskilllevel )
	{
		// 고급품
		if( TABS(x) >= randx - 2 - high && TABS(x) < randx - 2)
		{
			if(x < 0)
				y = 25 + rand()%11;
			else
				y = 36 + rand()%11;
		}
		// 호품
		else if( TABS(x) >= randx - 2 && TABS(x) < randx )
		{
			if(x < 0)
				y = 75 + rand()%25;
			else
				y = 100 + rand()%26;
		}
		else if( TABS(x) == randx )
		{
			if(x < 0)
				y = 135 + rand()%10;
			else
				y = 145 + rand()%11;
		}
		else 
			y = 0;
	}
	else if( 1 == extraskilllevel )
	{
		templimit = (randx - high)/(extraskilllevel + 1);

		if( TABS(x) >= 0 && TABS(x) < templimit )
		{
			y = -(rand()%4);
		}
		else if( TABS(x) >= templimit && TABS(x) < randx - 2 - high )
		{
			y = rand()%3;
		}

		else if( TABS(x) >= randx - 2 - high && TABS(x) < randx - 2)
		{
			if(x < 0)
				y = 25 + rand()%11;
			else
				y = 36 + rand()%11;
		}
		else if( TABS(x) >= randx - 2 && TABS(x) < randx )
		{
			if(x < 0)
				y = 75 + rand()%25;
			else
				y = 100 + rand()%26;
		}
		else if( TABS(x) == randx )
		{
			if(x < 0)
				y = 135 + rand()%10;
			else
				y = 145 + rand()%11;
		}
		else 
			y = 0;
	}
	else if( 2 == extraskilllevel )
	{
		templimit = (randx - high)/(extraskilllevel + 1);

		if( TABS(x) >= 0 && TABS(x) < templimit )
		{
			y = -(3 + (rand()%4));
		}
		else if( TABS(x) >= templimit && TABS(x) < templimit*2 )
		{
			y = -(rand()%4);
		}
		else if( TABS(x) >= templimit*2 && TABS(x) < randx - 2 - high )
		{
			y = rand()%3;
		}
		else if( TABS(x) >= randx - 2 - high && TABS(x) < randx - 2)
		{
			if(x < 0)
				y = 25 + rand()%11;
			else
				y = 36 + rand()%11;
		}
		else if( TABS(x) >= randx - 2 && TABS(x) < randx )
		{
			if(x < 0)
				y = 75 + rand()%25;
			else
				y = 100 + rand()%26;
		}
		else if( TABS(x) == randx )
		{
			if(x < 0)
				y = 135 + rand()%10;
			else
				y = 145 + rand()%11;
		}
		else 
			y = 0;
	}
	else if( 3 == extraskilllevel )
	{
		templimit = (randx - high)/(extraskilllevel + 1);

		if( TABS(x) >= 0 && TABS(x) < templimit )
		{
			y = -(6 + (rand()%4));
		}
		else if( TABS(x) >= templimit && TABS(x) < templimit*2 )
		{
			y = -(3 + (rand()%4));
		}
		else if( TABS(x) >= templimit*2 && TABS(x) < templimit*3 )
		{
			y = -(rand()%4);
		}
		else if( TABS(x) >= templimit*3 && TABS(x) < randx - 2 - high )
		{
			y = rand()%3;
		}
		else if( TABS(x) >= randx - 2 - high && TABS(x) < randx - 2)
		{
			if(x < 0)
				y = 25 + rand()%11;
			else
				y = 36 + rand()%11;
		}
		else if( TABS(x) >= randx - 2 && TABS(x) < randx )
		{
			if(x < 0)
				y = 75 + rand()%25;
			else
				y = 100 + rand()%26;
		}
		else if( TABS(x) == randx )
		{
			if(x < 0)
				y = 135 + rand()%10;
			else
				y = 145 + rand()%11;
		}
		else 
			y = 0;
	}
	else if( 4 == extraskilllevel )
	{
		templimit = (randx - high)/(extraskilllevel + 1);

		if( TABS(x) >= 0 && TABS(x) < templimit )
		{
			y = -(9 + (rand()%4));
		}
		else if( TABS(x) >= templimit && TABS(x) < templimit*2 )
		{
			y = -(6 + (rand()%4));
		}
		else if( TABS(x) >= templimit*2 && TABS(x) < templimit*3 )
		{
			y = -(3 + (rand()%4));
		}
		else if( TABS(x) >= templimit*3 && TABS(x) < templimit*4 )
		{
			y = -(rand()%4);
		}
		else if( TABS(x) >= templimit*4 && TABS(x) < randx - 2 - high )
		{
			y = rand()%3;
		}
		else if( TABS(x) >= randx - 2 - high && TABS(x) < randx - 2)
		{
			if(x < 0)
				y = 25 + rand()%11;
			else
				y = 36 + rand()%11;
		}
		else if( TABS(x) >= randx - 2 && TABS(x) < randx )
		{
			if(x < 0)
				y = 75 + rand()%25;
			else
				y = 100 + rand()%26;
		}
		else if( TABS(x) == randx )
		{
			if(x < 0)
				y = 135 + rand()%10;
			else
				y = 145 + rand()%11;
		}
		else 
			y = 0;
	}
	else if( 5 == extraskilllevel )
	{
		templimit = (randx - high)/(extraskilllevel + 1);

		if( TABS(x) >= 0 && TABS(x) < templimit )
		{
			y = -(12 + (rand()%4));
		}
		else if( TABS(x) >= templimit && TABS(x) < templimit*2 )
		{
			y = -(9 + (rand()%4));
		}
		else if( TABS(x) >= templimit*2 && TABS(x) < templimit*3 )
		{
			y = -(6 + (rand()%4));
		}
		else if( TABS(x) >= templimit*3 && TABS(x) < templimit*4 )
		{
			y = -(3 + (rand()%4));
		}
		else if( TABS(x) >= templimit*4 && TABS(x) < templimit*5 )
		{
			y = -(rand()%4);
		}
		else if( TABS(x) >= templimit*5 && TABS(x) < randx - 2 - high )
		{ 
			y = rand()%3;
		}
		else if( TABS(x) >= randx - 2 - high && TABS(x) < randx - 2)
		{
			if(x < 0)
				y = 25 + rand()%11;
			else
				y = 36 + rand()%11;
		}
		else if( TABS(x) >= randx - 2 && TABS(x) < randx )
		{
			if(x < 0)
				y = 75 + rand()%25;
			else
				y = 100 + rand()%26;
		}
		else if( TABS(x) == randx )
		{
			if(x < 0)
				y = 135 + rand()%10;
			else
				y = 145 + rand()%11;
		}
		else 
			y = 0;
	}
	else if( 6 == extraskilllevel )
	{
		templimit = (randx - high)/(extraskilllevel + 1);

		if( TABS(x) >= 0 && TABS(x) < templimit )
		{
			y = -(15 + (rand()%4));
		}
		else if( TABS(x) >= templimit && TABS(x) < templimit*2 )
		{
			y = -(12 + (rand()%4));
		}
		else if( TABS(x) >= templimit*2 && TABS(x) < templimit*3 )
		{
			y = -(9 + (rand()%4));
		}
		else if( TABS(x) >= templimit*3 && TABS(x) < templimit*4 )
		{
			y = -(6 + (rand()%4));
		}
		else if( TABS(x) >= templimit*4 && TABS(x) < templimit*5 )
		{
			y = -(3 + (rand()%4));
		}
		else if( TABS(x) >= templimit*5 && TABS(x) < templimit*6 )
		{
			y = -(rand()%4);
		}
		else if( TABS(x) >= templimit*6 && TABS(x) < randx - 2 - high )
		{
			y = rand()%3;
		}
		else if( TABS(x) >= randx - 2 - high && TABS(x) < randx - 2)
		{
			if(x < 0)
				y = 25 + rand()%11;
			else
				y = 36 + rand()%11;
		}
		else if( TABS(x) >= randx - 2 && TABS(x) < randx )
		{
			if(x < 0)
				y = 75 + rand()%25;
			else
				y = 100 + rand()%26;
		}
		else if( TABS(x) == randx )
		{
			if(x < 0)
				y = 135 + rand()%10;
			else
				y = 145 + rand()%11;
		}
		else 
			y = 0;
	}
	else if( 7 == extraskilllevel )
	{
		templimit = (randx - high)/(extraskilllevel + 1);

		if( TABS(x) >= 0 && TABS(x) < templimit )
		{
			y = -(18 + (rand()%4));
		}
		else if( TABS(x) >= templimit && TABS(x) < templimit*2 )
		{
			y = -(15 + (rand()%4));
		}
		else if( TABS(x) >= templimit*2 && TABS(x) < templimit*3 )
		{
			y = -(12 + (rand()%4));
		}
		else if( TABS(x) >= templimit*3 && TABS(x) < templimit*4 )
		{
			y = -(9 + (rand()%4));
		}
		else if( TABS(x) >= templimit*4 && TABS(x) < templimit*5 )
		{
			y = -(6 + (rand()%4));
		}
		else if( TABS(x) >= templimit*5 && TABS(x) < templimit*6 )
		{
			y = -(3 + (rand()%4));
		}
		else if( TABS(x) >= templimit*6 && TABS(x) < templimit*7 )
		{
			y = -(rand()%4);
		}
		else if( TABS(x) >= templimit*7 && TABS(x) < randx - 2 - high )
		{
			y = rand()%3;
		}
		else if( TABS(x) >= randx - 2 - high && TABS(x) < randx - 2)
		{
			if(x < 0)
				y = 25 + rand()%11;
			else
				y = 36 + rand()%11;
		}
		else if( TABS(x) >= randx - 2 && TABS(x) < randx )
		{
			if(x < 0)
				y = 75 + rand()%25;
			else
				y = 100 + rand()%26;
		}
		else if( TABS(x) == randx )
		{
			if(x < 0)
				y = 135 + rand()%10;
			else
				y = 145 + rand()%11;
		}
		else 
			y = 0;
	}
	else if( 8 == extraskilllevel )
	{
		templimit = (randx - high)/(extraskilllevel + 1);

		if( TABS(x) >= 0 && TABS(x) < templimit )
		{
			y = -(21 + (rand()%4));
		}
		else if( TABS(x) >= templimit && TABS(x) < templimit*2 )
		{
			y = -(18 + (rand()%4));
		}
		else if( TABS(x) >= templimit*2 && TABS(x) < templimit*3 )
		{
			y = -(15 + (rand()%4));
		}
		else if( TABS(x) >= templimit*3 && TABS(x) < templimit*4 )
		{
			y = -(12 + (rand()%4));
		}
		else if( TABS(x) >= templimit*4 && TABS(x) < templimit*5 )
		{
			y = -(9 + (rand()%4));
		}
		else if( TABS(x) >= templimit*5 && TABS(x) < templimit*6 )
		{
			y = -(6 + (rand()%4));
		}
		else if( TABS(x) >= templimit*6 && TABS(x) < templimit*7 )
		{
			y = -(3 + (rand()%4));
		}
		else if( TABS(x) >= templimit*7 && TABS(x) < templimit*8 )
		{
			y = -(rand()%4);
		}
		else if( TABS(x) >= templimit*8 && TABS(x) < randx - 2 - high )
		{
			y = rand()%3;
		}
		else if( TABS(x) >= randx - 2 - high && TABS(x) < randx - 2)
		{
			if(x < 0)
				y = 25 + rand()%11;
			else
				y = 36 + rand()%11;
		}
		else if( TABS(x) >= randx - 2 && TABS(x) < randx )
		{
			if(x < 0)
				y = 75 + rand()%25;
			else
				y = 100 + rand()%26;
		}
		else if( TABS(x) == randx )
		{
			if(x < 0)
				y = 135 + rand()%10;
			else
				y = 145 + rand()%11;
		}
		else 
			y = 0;
	}
	else if( 8 < extraskilllevel )
	{
		templimit = (randx - high)/9;

		SI32 tempextraskilllevel = extraskilllevel - 8;

		// 차이나는 스킬은 30으로 제한한다!!
		if( 30 < tempextraskilllevel )
			tempextraskilllevel = 30;

		SI32 tempvalue = (SI32)(templimit * ((float)tempextraskilllevel/30.0f));

		if( TABS(x) >= 0 && TABS(x) < templimit - (tempvalue*4/10))
		{
			y = -(21 + (rand()%4));
		}
		else if( TABS(x) >= templimit - (tempvalue*4/10) && TABS(x) < templimit*2 - ( (tempvalue*3/10) + (tempvalue*4/10) ) )
		{
			y = -(18 + (rand()%4));
		}
		else if( TABS(x) >= templimit*2 - ( (tempvalue*3/10) + (tempvalue*4/10) ) && TABS(x) < templimit*3 - ( (tempvalue*2/10) + (tempvalue*3/10) + (tempvalue*4/10) ) )
		{
			y = -(15 + (rand()%4));
		}
		else if( TABS(x) >= templimit*3 - ( (tempvalue*2/10) + (tempvalue*3/10) + (tempvalue*4/10)) && TABS(x) < templimit*4 - ( (tempvalue/10) + (tempvalue*2/10) + (tempvalue*3/10) + (tempvalue*4/10) ) )
		{
			y = -(12 + (rand()%4));
		}
		else if( TABS(x) >= templimit*4 - ( (tempvalue/10) + (tempvalue*2/10) + (tempvalue*3/10) + (tempvalue*4/10)) && TABS(x) < templimit*5 - ( (tempvalue*4/10) + (tempvalue*3/10) + (tempvalue*2/10) + (tempvalue/10) ) )
		{
			y = -(9 + (rand()%4));
		}
		else if( TABS(x) >= templimit*5 - ( (tempvalue*4/10) + (tempvalue*3/10) + (tempvalue*2/10) + (tempvalue/10) ) && TABS(x) < templimit*6 - ( (tempvalue*4/10) + (tempvalue*3/10) + (tempvalue*2/10) ) )
		{
			y = -(6 + (rand()%4));
		}
		else if( TABS(x) >= templimit*6 - ( (tempvalue*4/10) + (tempvalue*3/10) + (tempvalue*2/10) ) && TABS(x) < templimit*7 - ( (tempvalue*4/10) + (tempvalue*3/10) ) )
		{
			y = -(3 + (rand()%4));
		}
		else if( TABS(x) >= templimit*7 - ( (tempvalue*4/10) + (tempvalue*3/10) ) && TABS(x) < templimit*8 - (tempvalue*4/10) )
		{
			y = -(rand()%4);
		}
		else if( TABS(x) >= templimit*8 - (tempvalue*4/10) && TABS(x) < randx - 2 - high )
		{
			y = rand()%3;
		}
		else if( TABS(x) >= randx - 2 - high && TABS(x) < randx - 2)
		{
			if(x < 0)
				y = 25 + rand()%11;
			else
				y = 36 + rand()%11;
		}
		else if( TABS(x) >= randx - 2 && TABS(x) < randx )
		{
			if(x < 0)
				y = 75 + rand()%25;
			else
				y = 100 + rand()%26;
		}
		else if( TABS(x) == randx )
		{
			if(x < 0)
				y = 135 + rand()%10;
			else
				y = 145 + rand()%11;
		}
		else 
			y = 0;
	}

	// 명품이 나올 수 있는 모드라면, 
	if(brareswitch)
	{
		return min(280, max(10, basicrate + y));
	}
	// 명품이 나올 수 없는 모드라면, 
	else
	{
		return min(250, max(10, basicrate + y));
	}
}
/*
// 평균 0을 기준으로 확률을 구한다. (basicrate은 백분율) 
SI32 GetAverage(SI32 basicrate, bool brareswitch )
{
	//return 350; //무조건 명품을 만들어 보세 

	SI32 x = 10000 - (rand() % 20000);
	SI32 vary;

	if(TABS(x) <= 6000)	 vary = 500;
	else if(TABS(x) <= 8500)vary = 450;
	else if(TABS(x) <= 9000)vary = 400;
	else if(TABS(x) <= 9400)vary = 350;
	else if(TABS(x) <= 9600)vary = 300;
	else if(TABS(x) <= 9800)vary = 250;
	else if(TABS(x) <= 9900)vary = 200;
	else if(TABS(x) <= 9950)vary = 150;
	else if(TABS(x) <= 9985)vary = 100;
	else if(TABS(x) <= 9995)vary = 50;
	else vary = 30;

	SI32 y = x / vary;

	// 명품이 나올 수 있는 모드라면, 
	if(brareswitch)
	{
		return min(400, max(10, basicrate + y));
	}
	// 명품이 나올 수 없는 모드라면, 
	else
	{
		return min(250, max(10, basicrate + y));
	}


}
*/

// 평균 0을 기준으로 확률을 구한다. (basicrate은 백분율) 
SI32 GetAverageWithWeenyOldMan()
{
	SI32 x = 10000 - (rand() % 20000);
	SI32 vary;

	if (TABS(x) <= 100)			vary = 200;
	else if(TABS(x) <= 200)		vary = 160;
	else if(TABS(x) <= 410)		vary = 150;
	else if(TABS(x) <= 730)		vary = 140;
	else if(TABS(x) <= 1160)	vary = 130;
	else if(TABS(x) <= 1700)	vary = 120;
	else if(TABS(x) <= 2350)	vary = 110;
	else if(TABS(x) <= 3110)	vary = 100;
	else if(TABS(x) <= 3980)	vary = 90;
	else if(TABS(x) <= 4960)	vary = 80;
	else if(TABS(x) <= 6050)	vary = 70;
	else if(TABS(x) <= 7140)	vary = 60;
	else if(TABS(x) <= 8140)	vary = 50;
	else if(TABS(x) <= 9080)	vary = 40;
	else						vary = 30;

	return vary;

	//	// 이전공식	
	//	if(TABS(x) <= 6000)	 vary = 550;
	//	else if(TABS(x) <= 8500)vary = 500;
	//	else if(TABS(x) <= 9000)vary = 450;
	//	else if(TABS(x) <= 9400)vary = 400;
	//	else if(TABS(x) <= 9600)vary = 350;
	//	else if(TABS(x) <= 9800)vary = 300;
	//	else if(TABS(x) <= 9900)vary = 250;
	//	else if(TABS(x) <= 9950)vary = 200;
	//	else if(TABS(x) <= 9985)vary = 150;
	//	else if(TABS(x) <= 9995)vary = 100;
	//	else vary = 50;

	//	SI32 y = x / vary;

	//	y = y - 20;
	//	if (y < 0)
	//		y = 0;

	//	return y;

	//}
}

SI32 GetUniqueWithRange(const cltUniqueRange* pUniquePercent, SI32 siCount, SI32 siMaxRange)
{
	SI32 siRand = rand() % siMaxRange;

	SI32 siStart = 0;
	for(SI32 i=0; i<siCount; i++)
	{
		if(siStart <= siRand && siRand < siStart + pUniquePercent[i].siPercent)
			return pUniquePercent[i].siUnique;

		siStart += pUniquePercent[i].siPercent;
	}

	return 0;
}

//const cltValuesRange  BigItemBox_1_Grade[] = 
//{
//	// 퍼센트,		유니크,			개수,			기간.
//	{   5,		5350,				1,				0 }	
//  ...
// ,{   5,		5350,				1,				0 }	
//}
// 와 같은 형식의 구조체를 선언한 경우에 사용하는 함수
cltValuesRange *GetValuesWithRange(const cltValuesRange* pValuesPercent, SI32 siCount, SI32 siMaxRange)
{
	SI32 siRand = rand() % siMaxRange;
	SI32 siStart = 0;
	for(SI32 i=0; i<siCount; i++)
	{
		if(siStart <= siRand && siRand < siStart + pValuesPercent[i].siPercent)
			return (cltValuesRange*)&pValuesPercent[i];

		siStart += pValuesPercent[i].siPercent;
	}

	return NULL;
}

//const cltValuesRange  BigItemBox_1_Grade[] =
//{
//	// 퍼센트,		유니크,			개수,			기간.
//	{   5,		5350,				1,				0 }	
//  ...
// ,{   5,		5350,				1,				0 }	
// .{	0,		   0,				0,				0 }		// 끝처리
//}
// 와 같은 형식의 구조체를 선언한 경우에 사용하는 함수
cltValuesRange *GetValues( const cltValuesRange* pValuesPercent )
{
	if( pValuesPercent == NULL )		return NULL;

	SI32 siMaxRange = 0;
	SI32 siCount = 0;
	bool bUnlimited = true;

	for(SI32 i=0; bUnlimited; i++)
	{
		if( pValuesPercent[i].siPercent == 0 )		break;
		siCount++;
		siMaxRange += pValuesPercent[i].siPercent;
	}

	SI32 siRand = rand() % siMaxRange;
	SI32 siStart = 0;
	for(SI32 i=0; i<siCount; i++)
	{
		if(siStart <= siRand && siRand < siStart + pValuesPercent[i].siPercent)
			return (cltValuesRange*)&pValuesPercent[i];

		siStart += pValuesPercent[i].siPercent;
	}
	
	return NULL;
}

SI32 GetUniqueWithRandom(const SI32* psiUniques, SI32 siCount)
{
	SI32 siRand = rand() % siCount;
	return psiUniques[siRand];
}

SI32 MakeRealItemUnqiues(const SI32* psiUniques, SI32 siCount, SI32* psiOutUniques, SI32 siOutCount)
{
	SI32 siRetCount = 0;
	for( SI32 i=0; i<siCount; i++ )
	{
		SI32 unique = psiUniques[i];
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(unique);
		if( ref <= 0 )			continue;

		psiOutUniques[siRetCount] = psiUniques[i];
		siRetCount++;

		if( siOutCount <= siRetCount )		break;
	}

	return siRetCount;
}

SI32 DecideDirection8(const cltPos* pclpos1, const cltPos* pclpos2)
{
	SI32 sx = pclpos1->siX;
	SI32 sy	= pclpos1->siY;
	SI32 dx	= pclpos2->siX;
	SI32 dy	= pclpos2->siY;

	return DecideDirection8(sx, sy, dx, dy);
}

// sx, sy와 dx,dy를 가지고 방향을 찾는다. (8방향)
SI32 DecideDirection8(SI32 sx, SI32 sy, SI32 dx, SI32 dy)
{
	 int direction=0;
	 int x_len=TABS(sx-dx);
	 int y_len=TABS(sy-dy);


	 if(sx==dx && sy==dy)
	 {
		return SOUTH;
	 }

	 if(x_len>y_len)
	 {
		if(dx>sx)direction|=EAST;
		else if(dx<sx)direction|=WEST;

		 if(y_len*3>x_len)
		 {
		   if(dy>sy)direction|=SOUTH;
		   else if(dy<sy)direction|=NORTH;
		 }

	 }
	 else if(x_len==y_len)
	 {

		if(dx>sx)direction|=EAST;
		else if(dx<sx)direction|=WEST;

		if(dy>sy)direction|=SOUTH;
		else if(dy<sy)direction|=NORTH;

	 }
	 else
	 {
		if(dy>sy)direction|=SOUTH;
		else if(dy<sy)direction|=NORTH;

		if(x_len*3>y_len)
		{
		   if(dx>sx)direction|=EAST;
		   else if(dx<sx)direction|=WEST;
		}
	 }

	return direction;

}


SHORT DecideDirection16(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{
	int delta_x, delta_y;
	int direction = 0;
	int temp;

	if(x2>x1)delta_x=x2-x1;
	else delta_x=x1-x2;
	if(y2>y1)delta_y=y2-y1;
	else delta_y=y1-y2;

	if(delta_x*6<delta_y){temp=0;}  // x=0 graph
	else if(delta_x*2<delta_y){temp=1;}
	else if(delta_x<delta_y*2){temp=2;}
	else if(delta_x<delta_y*6){temp=3;}
	else {temp=4;}

	if(x1<x2){
		 if(y1>y2){
			if(temp==0)direction=8;
			else if(temp==1)direction=9;
			else if(temp==2)direction=10;
			else if(temp==3)direction=11;
			else if(temp==4)direction=12;
		 }
		 else{
			if(temp==0)direction=0;
			else if(temp==1)direction=15;
			else if(temp==2)direction=14;
			else if(temp==3)direction=13;
			else if(temp==4)direction=12;
		 }
	}
	else {
		if(y1>y2)
		{
			if(temp==0)direction=8;
			else if(temp==1)direction=7;
			else if(temp==2)direction=6;
			else if(temp==3)direction=5;
			else if(temp==4)direction=4;
		 }
		 else{
			if(temp==0)direction=0;
			else if(temp==1)direction=1;
			else if(temp==2)direction=2;
			else if(temp==3)direction=3;
			else if(temp==4)direction=4;
		 }
	}
	return direction;
}

SHORT DecideDirectionChar16(SHORT x1, SHORT y1, SHORT x2, SHORT y2)
{

	int delta_x, delta_y;
	int direction = 0;
	int temp;

	if(x2>x1)delta_x=x2-x1;
	else delta_x=x1-x2;
	if(y2>y1)delta_y=y2-y1;
	else delta_y=y1-y2;

	if(delta_x*6<delta_y){temp=0;}  // x=0 graph
	else if(delta_x*2<delta_y){temp=1;}
	else if(delta_x<delta_y*2){temp=2;}
	else if(delta_x<delta_y*6){temp=3;}
	else {temp=4;}

	if(x1<x2){
		 if(y1>y2){
			if(temp==0)direction=NORTH|WEST;
			else if(temp==1)direction=NNW;
			else if(temp==2)direction=NORTH;
			else if(temp==3)direction=NNE;
			else if(temp==4)direction=NORTH|EAST;
		 }
		 else{
			if(temp==0)direction=SOUTH|EAST;
			else if(temp==1)direction=EES;
			else if(temp==2)direction=EAST;
			else if(temp==3)direction=EEN;
			else if(temp==4)direction=NORTH|EAST;
		 }
	}
	else {
		if(y1>y2)
		{
			if(temp==0)direction=NORTH|WEST;
			else if(temp==1)direction=WWN;
			else if(temp==2)direction=WEST;
			else if(temp==3)direction=WWS;
			else if(temp==4)direction=SOUTH|WEST;
		 }
		 else{
			if(temp==0)direction=SOUTH|EAST;
			else if(temp==1)direction=SSE;
			else if(temp==2)direction=SOUTH;
			else if(temp==3)direction=SSW;
			else if(temp==4)direction=SOUTH|WEST;
		 }
	}
	return direction;
}




// 직선 구하는 함수. 
void FindStraightLine(SHORT sx, SHORT sy, SHORT ex, SHORT ey, SHORT &number, SHORT* lparray, SI32 siMaxNumber)
{
	int		t, distance;
	int		xerr=0, yerr=0, delta_x, delta_y;
	int		incx, incy;
	int		i;

	number=0;

	delta_x=ex-sx;
	delta_y=ey-sy;

	if(delta_x > 0)			incx = 1;
	else if(delta_x == 0)	incx = 0;
	else					incx =-1;

	if(delta_y > 0)			incy = 1;
	else if(delta_y == 0)	incy = 0;
	else					incy =-1;

	delta_x = TABS(delta_x);
	delta_y = TABS(delta_y);

	if(delta_x > delta_y) distance = delta_x;
	else distance=delta_y;

	for(t=0, i=0; t < distance+1; t++, i+=2)
	{

		xerr+=delta_x;
		yerr+=delta_y;

		if(xerr > distance)
		{			
			xerr-=distance;
			sx+=incx;
		}

		if(yerr > distance)
		{
			yerr-=distance;
			sy+=incy;
		}

		lparray[i]=sx;
		lparray[i+1]=sy;

		number++;

		if(siMaxNumber == number)
			break;
	}	
}


BOOL Circle(int nStep, int x, int y, int *StepCnt, POINT *pt)
{
	int index = 0;
	if(nStep >= 8) return FALSE;

	for(int j=0 ; j<15; j++)
		for(int i=0 ; i<15; i++)
		{
			if(nStep == cTable[i][j])
			{				
				pt[index].x = x + i - 7;
				pt[index].y = y + j - 7;

				index++;
			}
		}

	*StepCnt = index;

	return TRUE;
}



int WindEffect(int SDirect, int WDirect)
{
	int nReturnV = 0;

	switch(SDirect)
	{
	case NORTH:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = 2;
			break;
		case NORTH|EAST:
			nReturnV = 1;
			break;
		case EAST:
			nReturnV = 0;
			break;
		case EAST|SOUTH:
			nReturnV = -1;
			break;
		case SOUTH:
			nReturnV = -2;
			break;
		case SOUTH|WEST:
			nReturnV = -1;
			break;
		case WEST:
			nReturnV = 0;
			break;
		case WEST|NORTH:
			nReturnV = 1;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case NORTH|EAST:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = 1;
			break;
		case NORTH|EAST:
			nReturnV = 2;
			break;
		case EAST:
			nReturnV = 1;
			break;
		case EAST|SOUTH:
			nReturnV = 0;
			break;
		case SOUTH:
			nReturnV = -1;
			break;
		case SOUTH|WEST:
			nReturnV = -2;
			break;
		case WEST:
			nReturnV = -1;
			break;
		case WEST|NORTH:
			nReturnV = 0;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case EAST:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = 0;
			break;
		case NORTH|EAST:
			nReturnV = 1;
			break;
		case EAST:
			nReturnV = 2;
			break;
		case EAST|SOUTH:
			nReturnV = 1;
			break;
		case SOUTH:
			nReturnV = 0;
			break;
		case SOUTH|WEST:
			nReturnV = -1;
			break;
		case WEST:
			nReturnV = -2;
			break;
		case WEST|NORTH:
			nReturnV = -1;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case EAST|SOUTH:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = -1;
			break;
		case NORTH|EAST:
			nReturnV = 0;
			break;
		case EAST:
			nReturnV = 1;
			break;
		case EAST|SOUTH:
			nReturnV = 2;
			break;
		case SOUTH:
			nReturnV = 1;
			break;
		case SOUTH|WEST:
			nReturnV = 0;
			break;
		case WEST:
			nReturnV = -1;
			break;
		case WEST|NORTH:
			nReturnV = -2;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case SOUTH:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = -2;
			break;
		case NORTH|EAST:
			nReturnV = -1;
			break;
		case EAST:
			nReturnV = 0;
			break;
		case EAST|SOUTH:
			nReturnV = 1;
			break;
		case SOUTH:
			nReturnV = 2;
			break;
		case SOUTH|WEST:
			nReturnV = 1;
			break;
		case WEST:
			nReturnV = 0;
			break;
		case WEST|NORTH:
			nReturnV = -1;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case SOUTH|WEST:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = -1;
			break;
		case NORTH|EAST:
			nReturnV = -2;
			break;
		case EAST:
			nReturnV = -1;
			break;
		case EAST|SOUTH:
			nReturnV = 0;
			break;
		case SOUTH:
			nReturnV = 1;
			break;
		case SOUTH|WEST:
			nReturnV = 2;
			break;
		case WEST:
			nReturnV = 1;
			break;
		case WEST|NORTH:
			nReturnV = 0;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case WEST:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = 0;
			break;
		case NORTH|EAST:
			nReturnV = -1;
			break;
		case EAST:
			nReturnV = -2;
			break;
		case EAST|SOUTH:
			nReturnV = -1;
			break;
		case SOUTH:
			nReturnV = 0;
			break;
		case SOUTH|WEST:
			nReturnV = 1;
			break;
		case WEST:
			nReturnV = 2;
			break;
		case WEST|NORTH:
			nReturnV = 1;
			break;
		default:
			nReturnV = 0;
		}
		break;
	case WEST|NORTH:
		switch(WDirect)
		{
		case NORTH:
			nReturnV = 1;
			break;
		case NORTH|EAST:
			nReturnV = 0;
			break;
		case EAST:
			nReturnV = -1;
			break;
		case EAST|SOUTH:
			nReturnV = -2;
			break;
		case SOUTH:
			nReturnV = -1;
			break;
		case SOUTH|WEST:
			nReturnV = 0;
			break;
		case WEST:
			nReturnV = 1;
			break;
		case WEST|NORTH:
			nReturnV = 2;
			break;
		default:
			nReturnV = 0;
		}
		break;

	default:
		nReturnV = 0;
	}
	return nReturnV;
}


HANDLE FileMapHandle;

BOOL CheckDupGame(TCHAR *name, TCHAR* title)
{
	FileMapHandle = CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, 1024, name);
	if(FileMapHandle != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return TRUE;
	}

	return FALSE;
}

void CloseDupGameHandle()
{
	if(FileMapHandle)
	   CloseHandle(FileMapHandle);
}


/////////////////////////
// return 1 if MOVE
//        0 if WAIT
/////////////////////////

SHORT FindPathStraight(SHORT realx, SHORT realy, SHORT realdx, SHORT realdy, SHORT direction, SHORT range, SHORT gomode, SHORT& realnx, SHORT &realny)
{

	SHORT x1=realx,  y1=realy;
	SHORT x2=realdx, y2=realdy;
	SHORT x, y;
	SHORT delta_x, delta_y, half, error=0;
	SHORT step=0;
	SHORT index=0;

	

	if(gomode==FALSE)
	{	
		switch(direction)
		{
		case SOUTH:
			x2=x1-range;
			y2=y1+range;			
			break;
		case SSW:
			x2=x1-(range*2);
			y2=y1+range;
			break;
		case SOUTH|WEST:
			x2=x1-range;
			y2=y1;
			break;		
		case WWS:
			x2=x1-(range*2);
			y2=y1-range;
			break;
		case WEST:
			x2=x1-range;
			y2=y1-range;
			break;
		case WWN:
			x2=x1-range;
			y2=y1-(range*2);
			break;			
		case NORTH|WEST:
			x2=x1;
			y2=y1-range;
			break;
		case NNW:
			x2=x1+range;
			y2=y1-(range*2);
			break;			
		case NORTH:
			y2=y1-range;
			x2=x1+range;
			break;
		case NNE:
			x2=x1+(range*2);
			y2=y1-range;
			break;			
		case NORTH|EAST:			
			x2=x1+range;
			y2=y1;
			break;		
		case EEN:
			x2=x1+(range*2);
			y2=y1+range;
			break;
		case EAST:
			x2=x1+range;
			y2=y1+range;			
			break;
		case EES:
			x2=x1+range;
			y2=y1+(range*2);
			break;
		case SOUTH|EAST:
			x2=x1;
			y2=y1+range;
			break;
		case SSE:
			x2=x1-range;
			y2=y1+(range*2);		
			break;
		}		
	}

	if(x1>x2)delta_x=x1-x2;
	else     delta_x=x2-x1;
	if(y1>y2)delta_y=y1-y2;
	else	 delta_y=y2-y1;

	realnx=x1;
	realny=y1;

    step++;
	if(step>=range)return 1;

	if(delta_x>delta_y)
	{
	   y=y1;
	   half=(delta_x/2);

	   if(x1<x2)
	   {
		   for(x=x1+1;x<=x2;x++)
		   {
			  error+=delta_y;
			  if(error>half)
			  {
				   if(y1<y2)y++;
				   else y--;
				   error-=delta_x;
			  }
			  realnx=x;
			  realny=y;

			  step++;
	          if(step>=range)return 1;
		  }
	   }
	   else
	   {

		   for(x=x1-1;x>=x2;x--)
		   {
			  error+=delta_y;
			  if(error>half)
			  {
				   if(y1<y2)y++;
				   else y--;
				   error-=delta_x;
			  }
			  realnx=x;
			  realny=y;

			  step++;
	          if(step>=range)return 1;
		  }
	   }
	}
	else
	{
	   x=x1;
	   half=(delta_y/2);

	   if(y1<y2)
	   {
		  for(y=y1+1;y<=y2;y++)
		  {
			  error+=delta_x;
			  if(error>half)
			  {
				 if(x1<x2)x++;
				 else x--;
				 error-=delta_y;
			  }
				realnx=x;
				realny=y;
				step++;
	            if(step>=range)return 1;
		  }
	   }
	   else
	   {
		  for(y=y1-1;y>=y2;y--)
		  {
			  error+=delta_x;
			  if(error>half)
			  {
				 if(x1<x2)x++;
				 else x--;
				 error-=delta_y;
			  }
				realnx=x;
				realny=y;
				step++;
	            if(step>=range)return 1;
		  }
	   }
	}

	return 1;
}

void ConvertSpaceToUnderLine( TCHAR* pText )
{
	size_t len;
	StringCchLength( pText, 1024, &len );

	for( size_t i=0 ; i< len; )
	{
		if( IsDBCS( pText[ i ] ) )	{	i += 2;		continue;		}

		if( pText[ i ] == TEXT(' ') )
			pText[ i ] = TEXT('_');

		i++;
	}
}

void ConvertUnderLineToSpace(TCHAR* pText, size_t nLen)
{
	for(size_t i = 0 ; i< nLen; )
	{		
		if(IsDBCS( pText[i]))
		{	
			i += 2;
			continue;		
		}

		if(pText[i] == TEXT('_'))
			pText[i] = TEXT(' ');

		i++;
	}
}
