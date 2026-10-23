//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _UTIL_H
#define _UTIL_H   1

class cltPos;

//-----------------------------------------------
// 8방향 방위 찾기 
//-----------------------------------------------
SI32 DecideDirection8(const cltPos* pclpos1, const cltPos* pclpos2);
SI32 DecideDirection8(SI32 sx, SI32 sy, SI32 dx, SI32 dy);

//-----------------------------------------------
// 16방향 방위 찾기 
//-----------------------------------------------
SHORT DecideDirection16(SHORT x1, SHORT y1, SHORT x2, SHORT y2);
SHORT DecideDirectionChar16(SHORT x1, SHORT y1, SHORT x2, SHORT y2);

void FindStraightLine(SHORT sx, SHORT sy, SHORT ex, SHORT ey, SHORT &number, SHORT* lparray, SI32 siMaxNumber);

BOOL Circle(int nStep, int x, int y, int *StepCnt, POINT *pt);
int WindEffect(int SDirect, int WDirect);

BOOL CheckDupGame(TCHAR *name, TCHAR* title);
void CloseDupGameHandle();


//----------------------------
SHORT FindPathStraight(SHORT x, SHORT y, SHORT dx, SHORT dy, SHORT direction, SHORT range, SHORT gomode, SHORT& nx, SHORT &ny);;

// 평균 0을 기준으로 확률을 구한다. (basicrate은 백분율) 
SI32 GetAverage(SI32 basicrate, bool bCanMakeRareItemSwitch, SI32 addleve, bool bCreateHighGrade = true );

SI32 GetAverageWithWeenyOldMan();

struct cltUniqueRange
{
public:
	SI32	siUnique;
	SI32	siPercent;
	void Init(){
		 siUnique = 0 ;
		 siPercent = 0 ;
	}
};

class cltValuesRange
{
public:
	SI32	siPercent;
	SI32	siPara1;
	SI32	siPara2;
	SI32	siPara3;

	void Set(SI32 percent, SI32 para1, SI32 para2, SI32 para3)
	{
		siPercent	= percent;
		siPara1		= para1;
		siPara2		= para2;
		siPara3		= para3;
	}
};

SI32 GetUniqueWithRange(const cltUniqueRange* pUniquePercent, SI32 siCount, SI32 siMaxRange);
cltValuesRange *GetValuesWithRange(const cltValuesRange* pValuesPercent, SI32 siCount, SI32 siMaxRange);
SI32 GetUniqueWithRandom(const SI32* psiUniques, SI32 siCount);
SI32 MakeRealItemUnqiues(const SI32* psiUniques, SI32 siCount, SI32* psiOutUniques, SI32 siOutCount);

cltValuesRange *GetValues( const cltValuesRange* pValuesPercent );

void ConvertSpaceToUnderLine( TCHAR* pText);
void ConvertUnderLineToSpace(TCHAR* pText, size_t nLen);


#endif
