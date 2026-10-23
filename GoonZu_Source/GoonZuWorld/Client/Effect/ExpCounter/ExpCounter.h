#ifndef _EXPCOUNTER_H_
#define _EXPCOUNTER_H_

#include <Directives.h>
#include <time.h>

#include "../../GlobalImgFile/GlobalImgFile.h"

#define MAX_SKILLEXP_COUNTER_NUM		3

class CExpCounter
{
public:
	CExpCounter();
	~CExpCounter();

	void	Initialize();
	void	Destroy();

	void	Set( SI32 si32ChangedExp, SI32 DrawMaxFrame );
	void	SkillSet( SI16 SkillUnique, SI32 si32ChangedExp, SI32 DrawMaxFrame );
	void	Action();
	void	Draw( SI32 si32CenterXPos = 700, SI32 si32CenterYPos = 90 );

	SI16	GetAvailableSkillExpIndex();

private:
	SI32	m_siExpDrawMaxFrame;
	SI32	m_siExpCurrentFrame;
	
	SI32	m_siChangedExp;
	
	bool	m_bExpDrawStatus;

	SI32	m_siExpWidth;
	SI32	m_siExpHeight;
	SI32	m_siExpCharNum;


	TSpr *m_pExpXSPR;
	
	
	SI32	m_siSkillUnique[ MAX_SKILLEXP_COUNTER_NUM ];
	
	SI32	m_siSkillExpDrawMaxFrame[ MAX_SKILLEXP_COUNTER_NUM ];
	SI32	m_siSkillExpCurrentFrame[ MAX_SKILLEXP_COUNTER_NUM ];
	
	bool	m_bSkillExpDrawStatus[ MAX_SKILLEXP_COUNTER_NUM ];

	SI32	m_siChangedSkillExp[ MAX_SKILLEXP_COUNTER_NUM ];

	SI32	m_siSkillExpCharNum[ MAX_SKILLEXP_COUNTER_NUM ];

	SI32	m_siSkillExpWidth;
	SI32	m_siSkillExpHeight;
	
	TSpr *m_pSkillNameXSPR;
	TSpr *m_pSkillExpXSPR;

	clock_t	m_expClock;

};

#endif