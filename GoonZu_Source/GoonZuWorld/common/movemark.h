#ifndef _MOVEMARK_H
#define _MOVEMARK_H

#include "Map\Map.h"

class _MoveMark {
	cltMapClient* pclMap;

	SHORT X, Y;   // 이동 표시의 위치 
	int RealX, RealY;

	SHORT MoveMarkSwitch;
	SHORT Step;
	SHORT MaxStep;

	public:
	_MoveMark(cltMapClient* pclMap);
	~_MoveMark();
	void DrawMoveMark();
	void Action();
	void SetMoveMark(SHORT x, SHORT y);
	// real 좌표로 설정한다. 
    void SetMoveMarkReal(SHORT x, SHORT y);

};



#endif