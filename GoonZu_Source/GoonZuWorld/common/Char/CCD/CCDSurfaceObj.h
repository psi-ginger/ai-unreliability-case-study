#ifndef _CCDSURFACEOBJ_H_
#define _CCDSURFACEOBJ_H_

#include <Graphic.h>
#include <Graphic_Capsule.h>
#include "BaseDefinition.h"

#include "..\..\SMLib\LibList\Obj.h"

class CCCDSurfaceObj : public CObj
{
public:
	CCCDSurfaceObj();
	~CCCDSurfaceObj();

	void Initialize();
	void Destroy();
	
	bool IsExist( SI32 siIndex, UI32 *puiFrame );
	SI32 GetIndex();

	void Set( SI32 siIndex );

	LPDIRECTDRAWSURFACE7 GetSurface( UI32 uiFrame );

	

private:
	LPDIRECTDRAWSURFACE7	m_lpSurface;
	SI32					m_siIndex;
	UI32					m_uiFrame;

};

#endif