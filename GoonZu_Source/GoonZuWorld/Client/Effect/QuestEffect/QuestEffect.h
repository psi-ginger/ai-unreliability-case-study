#pragma once


#include <Directives.h>
#include <time.h>

#include "TSpr.h"


class CQuestEff
{
public:
	CQuestEff( TSpr *pXSPR_QuestReceive , TSpr *pXSPR_QuestClear );
	~CQuestEff();

	void	Initialize();
	void	Destroy();


	void	SetReceive();
	void	SetClear();
	void	Action();
	void	Draw( SI32 siCenterXPos, SI32 siCenterYPos );

private:
	TSpr	*m_pQuestReceiveXSPR;
	TSpr	*m_pQuestClearXSPR;

	SI32	m_si32CurrentDrawFrame;

	SI32	m_si32SprReceiveImageNum;
	SI32	m_si32SprClearImageNum;

	
	
	BOOL	m_bReceive;
	BOOL	m_bClear;

	clock_t	m_delayClock;

};
