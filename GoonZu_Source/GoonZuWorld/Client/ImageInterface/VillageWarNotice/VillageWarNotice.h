#ifndef _VILLAGEWAR_NOTICE_H_
#define _VILLAGEWAR_NOTICE_H_

#include <Graphic.h>
#include <Graphic_Capsule.h>

#define VILLAGEWAR_NOTICE_NONE	0
#define VILLAGEWAR_NOTICE_ATTACK	1
#define VILLAGEWAR_NOTICE_DEFENCE	2
#define VILLAGEWAR_NOTICE_WIN		3
#define VILLAGEWAR_NOTICE_LOSE		4

class CVillageWarNotice
{
public:
	CVillageWarNotice();
	~CVillageWarNotice();

	void Initialize();	
	
	void Set(SI16 siVillageWarNoticeMode);

	void Action();
	void Draw( SI32 XMovePos, SI32 YMovePos );

private:

	TSpr					*m_pVillageWarSpr;

	bool					m_bShow;
	SI32					m_siVillageWarNoticeMode;
	SI32					m_siFrameCount;
	SI32					m_siContinueCount;	
};

#endif
