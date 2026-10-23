#ifndef _ANI_SPR_
#define _ANI_SPR_

#include "../Lib/Graphic/graphic.h"
#include "../lib/ngraphics/graphics.h"

#include "ndataTypes.h"
#include "ntimer.h"
#include "TSpr.h"

class CAniSpr
{
public:
	TSpr		m_pSpr;

	SI32		m_MaxFrame;		// 프래임 개수
	SI32		m_NowFrame;		// 현재 프레임
	
	UI32		m_DelayTime;	// 지연 시간
	UI32		m_LastTime;		

public:
	CAniSpr();
	~CAniSpr();

	void	SetAniData(SI32 delay);
	void	ResetFrame();
	void	Release();								// 해제 
	int		AniProc();								// 프레임 연산
	int		AniProcEnd();
	
	BOOL	LoadSpr(TCHAR* pName, SI32 delay);
	void	DrawAniSpr(SI32 x, SI32 y, SI32 imagenum =0);
	void	DrawAniSprT(SI32 x, SI32 y, SI32 imagenum =0);
};

#endif