#ifndef _REMEMBERWINDOWPOSOBJ_H_
#define _REMEMBERWINDOWPOSOBJ_H_

#include "..\..\..\Common\SMLib\LibList\Obj.h"

class CRememberWindowPosObj : public CObj
{
public:
	CRememberWindowPosObj();
	~CRememberWindowPosObj();

	void	Initialize();
	void	Destroy();

	bool	IsAvailablePos( SI32 siLeft, SI32 siTop, SI32 siRight, SI32 siBottom );
	void	ChangePos( UI32 uiWindowKind, SI32 siLeft, SI32 siTop, SI32 siRight, SI32 siBottom );

	UI32	GetRememberWindowKind();
	void	GetRememberWindowPosData( UI32 *pWindowKind, SI32 *pLeftPos, SI32 *pTopPos, SI32 *pRightPos, SI32 *pBottomPos );
		

private:

	UI32	m_uiWindowKind;				// 윈도우가 어떤 거냐?

	SI32	m_siLeftPos;				// 윈도우 왼쪽 좌표
	SI32	m_siTopPos;					// 윈도우 상단 좌표

	SI32	m_siRightPos;				// 윈도우 오른쪽 좌표
	SI32	m_siBottomPos;				// 윈도우 하단 좌표
};

#endif