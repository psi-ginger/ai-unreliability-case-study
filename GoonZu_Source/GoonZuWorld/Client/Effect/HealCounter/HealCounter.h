#ifndef _HEALCOUNTER_H_
#define _HEALCOUNTER_H_

#include <Directives.h>
#include <time.h>

#include "../../GlobalImgFile/GlobalImgFile.h"

class CHealCounter
{
public:
	CHealCounter();
	~CHealCounter();

	void	Initialize();
	void	Destroy();

	void	Set( SI32 si32ChangedExp, SI32 DrawMaxFrame );
	void	Action();
	void	Draw( SI32 si32CenterXPos = 700, SI32 si32CenterYPos = 90 );

private:
	SI32	m_si32DrawMaxFrame;
	SI32	m_si32CurrentFrame;
	SI32	m_si32ChangedExp;
	
	bool	m_bDrawStatus;

	SI32	m_si32Width;
	SI32	m_si32Height;
	SI32	m_si32ExpCharNum;

	TSpr *m_pXSPR;

	clock_t	m_delayClock;


};

#endif