//---------------------------------
// 2005/3/17 ¼ºÁØ¿±
//---------------------------------
#ifndef _GAMEEVENT_H
#define _GAMEEVENT_H

#include <Directives.h>

#define MAX_WINPRIZE_MEMBER		30

#define MAX_PARA_1				10
#define MAX_PARA_2				20

class cltGameEvent{
public:
	cltGameEvent();
	~cltGameEvent();

//	SI32 m_siWinPrizeMemebers; // ´çÃ·ÀÚ ¼ö
//	SI32 m_siTotalCheckMemebers;

	SI32 m_siPara1 ;
	SI32 m_siPara2 ;

	void Init();

	bool CheckPrizeEvent(SI32 id,SI32 ticket);

	void GetItemUniqueAndNumRamdomSelect(SI32 * unique , SI32 * num);
};

#endif