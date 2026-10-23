#ifndef _FIREFLYMGR_H_
#define _FIREFLYMGR_H_

#include <Directives.h>
#include <time.h>

#include "FireFlyBaseInfo.h"
#include "TSpr.h"

struct stFireFlyObj
{
	SI32 siIndex;
	SI32 siImageIndex;
	SI32 siXPos;
	SI32 siYPos;
	SI32 siDelayFlame;
};

class CFireFlyMgr
{
public:
	CFireFlyMgr( TSpr *pXSPR, SI32 siWidth, SI32 siHeight, SI32 siMaxFireFlyNum );
	~CFireFlyMgr();

	void	Initialize();
	void	Destroy();
			
	void SetMapArea( SI32 siWidth, SI32 siHeight );
	
	bool IsInArea( SI32 xPos, SI32 yPos );
	bool IsInScreenArea( SI32 FireFlyxPos, SI32 FireFlyyPos, SI32 siScreenXPos, SI32 siScreenYPos, SI32 siScreenWidth, SI32 siScreenHeight );

	bool CreateFireFly( SI32 xPos, SI32 yPos );
	void DeleteFireFly( SI32 siIndex );

	void Action();	

	bool IsRepetition( SI32 siXPos, SI32 siYPos );

	void Draw( SI32 xPos, SI32 yPos, SI32 siScreenWidth, SI32 siScreenHeight );

	void SetActive( bool bActive );
	void SetActive( bool bActive, int XLeft, int YTop, int XRight, int YBottom, int siMaxFireFlyNum );


private:
	TSpr *m_pFireFlyXSPR;
	
	SI32 m_siWidth;
	SI32 m_siHeight;

	SI32 m_siXLeft;
	SI32 m_siXRight;

	SI32 m_siYTop;
	SI32 m_siYBottom;

	SI32 m_siFireFlyNum;
	SI32 m_siMaxFireFlyNum;

	stFireFlyObj *m_pFireFlyObjList;

	bool m_bActive;

	bool m_bAreaCheck;

	clock_t	m_delayClock;
};

#endif