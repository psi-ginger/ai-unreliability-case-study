#ifndef _PCROOMINTRO_H
#define _PCROOMINTRO_H

#include <graphic.h>
#include "TSpr.h"


const SI32 Const_SpecialPCRoomMark = 4; // PC¹æ ¸¶Å© °¹¼ö


class cltPCRoomIntro
{
public:
	cltPCRoomIntro();
	~cltPCRoomIntro();

	void	Start();
	void	Stop();
	void	Draw();

public:
	TSpr			spr;

	bool			m_bDraw;

	SI32			m_si32CurrentFrame;
	SI16			m_siFadeMode;
	SI16			m_siFadeStep;


};


#endif