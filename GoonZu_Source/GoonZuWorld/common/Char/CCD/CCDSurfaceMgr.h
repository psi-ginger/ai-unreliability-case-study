#ifndef _CCDSURFACEMGR_H_
#define _CCDSURFACEMGR_H_

#define SURFACE_OBJ_SURVIVAL_FRAME		60

#include <Graphic.h>

#include "BaseDefinition.h"
#include "..\..\SMLib\LibList\LibListMgr.h"

class CCCDSurfaceMgr : public CLibListMgr
{
public:
	CCCDSurfaceMgr();
	~CCCDSurfaceMgr();

	void Initialize();
	void Destroy();

	void CreateEmptyObj( SI32 siMaxEmptyObj );

	SI32 GetSurfaceIndex();

	LPDIRECTDRAWSURFACE7 GetSurface( SI32 siIndex );

private:
	SI32 m_siCurrentIndex;


};

#endif