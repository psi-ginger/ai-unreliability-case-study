#ifndef _IceFlashMGR_H_
#define _IceFlashMGR_H_

#include <Directives.h>
#include <time.h>

#include "IceFlashBaseInfo.h"
#include "TSpr.h"

struct stIceFlashObj
{
	SI32 siIndex;
	SI32 siImageIndex;
	SI32 siXPos;
	SI32 siYPos;
	SI32 siDelayFlame;
};

class CIceFlashMgr
{
public:
	CIceFlashMgr( TSpr *pXSPR, SI32 siWidth, SI32 siHeight, SI32 siMaxIceFlashNum );
	~CIceFlashMgr();

	void	Initialize();
	void	Destroy();
			
	void SetMapArea( SI32 siWidth, SI32 siHeight );
	
	bool IsInArea( SI32 xPos, SI32 yPos );
	bool IsInScreenArea( SI32 IceFlashxPos, SI32 IceFlashyPos, SI32 siScreenXPos, SI32 siScreenYPos, SI32 siScreenWidth, SI32 siScreenHeight );

	bool CreateIceFlash( SI32 xPos, SI32 yPos );
	void DeleteIceFlash( SI32 siIndex );

	void Action();	

	bool IsRepetition( SI32 siXPos, SI32 siYPos );

	void Draw( SI32 xPos, SI32 yPos, SI32 siScreenWidth, SI32 siScreenHeight );

	void SetActive( bool bActive );
	void SetActive( bool bActive, int XLeft, int YTop, int XRight, int YBottom, int siMaxIceFlashNum );


private:
	TSpr *m_pIceFlashXSPR;
	
	SI32 m_siWidth;
	SI32 m_siHeight;

	SI32 m_siXLeft;
	SI32 m_siXRight;

	SI32 m_siYTop;
	SI32 m_siYBottom;

	SI32 m_siIceFlashNum;
	SI32 m_siMaxIceFlashNum;

	stIceFlashObj *m_pIceFlashObjList;

	bool m_bActive;

	bool m_bAreaCheck;

	clock_t	m_delayClock;

};

#endif