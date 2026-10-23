#ifndef _WATERDROPMGR_H_
#define _WATERDROPMGR_H_

#include <time.h>

#include "WaterDropBaseInfo.h"
#include "TSpr.h"

struct stWaterDropObj
{
	SI32 siIndex;
	SI32 siImageIndex;
	SI32 siXPos;
	SI32 siYPos;
};

class CWaterDropMgr
{
public:
	CWaterDropMgr( TSpr *pXSPR, SI32 siWidth, SI32 siHeight, SI32 siMaxWaterDropNum );
	~CWaterDropMgr();

	void	Initialize();
	void	Destroy();
			
	void SetMapArea( SI32 siWidth, SI32 siHeight );
	
	bool IsInArea( SI32 xPos, SI32 yPos );
	bool IsInScreenArea( SI32 WaterDropxPos, SI32 WaterDropyPos, SI32 siScreenXPos, SI32 siScreenYPos, SI32 siScreenWidth, SI32 siScreenHeight );

	bool CreateWaterDrop( SI32 xPos, SI32 yPos );
	void DeleteWaterDrop( SI32 siIndex );

	void Action();	

	bool IsRepetition( SI32 siXPos, SI32 siYPos );

	void Draw( SI32 xPos, SI32 yPos, SI32 siScreenWidth, SI32 siScreenHeight );

	void SetActive( bool bActive );


private:

	TSpr *m_pWaterDropXSPR;
	
	SI32 m_siWidth;
	SI32 m_siHeight;

	SI32 m_siWaterDropNum;
	SI32 m_siMaxWaterDropNum;

	stWaterDropObj *m_pWaterDropObjList;

	bool m_bActive;

	clock_t	m_delayClock;

};

#endif