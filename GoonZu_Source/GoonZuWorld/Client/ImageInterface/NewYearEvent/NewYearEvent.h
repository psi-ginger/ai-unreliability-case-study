#ifndef _NEWYEAR_EVENT_NOTICE_H_
#define _NEWYEAR_EVENT_NOTICE_H_

#include <Graphic.h>
#include <Graphic_Capsule.h>

class CNewYearEventNotice
{
public:
	CNewYearEventNotice();
	~CNewYearEventNotice();

	void Initialize();	

	void Set();

	void Action();
	void Draw(SI32 XMovePos, SI32 YMovePos );

private:

	bool					m_bShow;

	SI32					m_siFrameCount;
	SI32					m_siContinueCount;	
};

#endif
