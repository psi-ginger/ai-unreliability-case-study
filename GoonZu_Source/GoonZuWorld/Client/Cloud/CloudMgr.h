#ifndef _CLOUDMGR_H_
#define _CLOUDMGR_H_

#include <Directives.h>

#include "..\..\Common\Weather\Weather.h"
#include "TSpr.h"

#define CLOUD_IMAGE_NUM		3


enum { CLD_SOUTH_EAST, CLD_SOUTH, CLD_SOUTH_WEST, CLD_WEST, CLD_NORTH_WEST, CLD_NORTH, CLD_NORTH_EAST, CLD_EAST };

struct stCloudObj
{
	SI32 siIndex;
	SI32 siImageIndex;
	SI32 siXPos;
	SI32 siYPos;	
};

class CCloudMgr  
{
public:
	CCloudMgr( TSpr *pXSPR0, TSpr *pXSPR1, TSpr *pXSPR2, SI32 siWidth, SI32 siHeight, SI32 siMaxCloudNum );
	//CCloudMgr( cltTSpr *pXSPR0, cltTSpr *pXSPR1, cltTSpr *pXSPR2, cltTSpr *pXSPR3, SI32 siWidth, SI32 siHeight, SI32 siMaxCloudNum );
	~CCloudMgr();

	void Initialize();
	void Destroy();
	
	void SetMapArea( SI32 siWidth, SI32 siHeight );
	
	bool IsInArea( SI32 xPos, SI32 yPos, SI32 CloudObjListIndex );
	bool IsInScreenArea( SI32 CloudxPos, SI32 CloudyPos, SI32 siScreenXPos, SI32 siScreenYPos, SI32 siScreenWidth, SI32 siScreenHeight, SI32 siImageIndex );

	bool CreateCloud( SI32 xPos, SI32 yPos );
	void DeleteCloud( SI32 siIndex );

	void Action( SI08 siWindDirection = CLD_SOUTH_EAST );
	void Action( stWeatherData *pWeatherData );

	bool IsRepetition( SI32 siXPos, SI32 siYPos );

	void Draw( SI32 xPos, SI32 yPos, SI32 siScreenWidth, SI32 siScreenHeight );

	void SetActive( bool bActive, SI32 siWidth = 0, SI32 siHeight = 0, SI32 siMaxCloudNum = 0 );

private:
	TSpr *m_pCloudXSPR[ CLOUD_IMAGE_NUM ];
	
	
	SI32 m_siWidth;
	SI32 m_siHeight;

	SI32 m_siBigXSize;
	SI32 m_siBigYSize;

	SI32 m_siCloudNum;
	SI32 m_siMaxCloudNum;

	stCloudObj *m_pCloudObjList;

	bool m_bActive;
};

#endif