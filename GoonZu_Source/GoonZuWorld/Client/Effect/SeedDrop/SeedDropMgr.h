#ifndef _SEEDDROPMGR_H_
#define _SEEDDROPMGR_H_


#include <Directives.h>
#include <time.h>

#include "TSpr.h"

#define	MOVE_X_POS					3
#define	MOVE_Y_POS					5
#define	CREATE_Y_INDENT_POS			5

struct stSeedDropObj
{
	SI32 siIndex;
	SI32 siImageIndex;
	SI32 siXPos;
	SI32 siYPos;
	SI32 siSeedTypeIndex;
};

class CSeedDropMgr
{
public:
	CSeedDropMgr( SI32 GimgIndex, SI32 SeedTypeNum, SI32 siWidth, SI32 siHeight, SI32 siMaxSeedDropNum );
	~CSeedDropMgr();

	void	Initialize();
	void	Destroy();

	void SetMapArea( SI32 siWidth, SI32 siHeight );

	bool IsInArea( SI32 type, SI32 xPos, SI32 yPos );
	bool IsInScreenArea( SI32 type, SI32 SeedDropxPos, SI32 SeedDropyPos, SI32 siScreenXPos, SI32 siScreenYPos, SI32 siScreenWidth, SI32 siScreenHeight );

	bool CreateSeedDrop( SI32 xPos, SI32 yPos );
	void DeleteSeedDrop( SI32 siIndex );

	void Action();	

	bool IsRepetition( SI32 siXPos, SI32 siYPos );

	void Draw( SI32 xPos, SI32 yPos, SI32 siScreenWidth, SI32 siScreenHeight );

	void SetActive( bool bActive );


private:

	TSpr *m_pSeedDropXSPR[4];
	SI32 m_siSeedTypeNum;

	SI32 m_siWidth;
	SI32 m_siHeight;

	SI32 m_siSeedDropNum;
	SI32 m_siMaxSeedDropNum;

	stSeedDropObj *m_pSeedDropObjList;

	bool m_bActive;
	clock_t	m_delayClock;
};

#endif