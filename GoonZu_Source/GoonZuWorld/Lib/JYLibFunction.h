#ifndef _JYLIBFUNCTION_H_
#define _JYLIBFUNCTION_H_

#include <Directives.h>
#include <stdio.h>
#include "string.h"

#define MAX_ALLOC_RECTS  100

class CJYLibFunction
{
public:
	CJYLibFunction();
	~CJYLibFunction();

	void SetNumTxt(INT64 num , TCHAR * dest , SI16 destsize,  TCHAR * add = NULL ); // num : 숫자 , dest : 숫자->문자열로 되는 데이타가 들어가는 곳 , add : 숫자의 단위(ex:냥)
	void SetNumUnit(INT64 num , TCHAR * dest , SI16 destsize, TCHAR * add = NULL );

	HRGN DIBToRgn(HBITMAP hBmp, COLORREF BkColor, BOOL Direct);
};

extern CJYLibFunction g_JYLibFunction;

#endif