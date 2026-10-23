#ifndef _INTERFACEIMGOBJ_H_
#define _INTERFACEIMGOBJ_H_

#include "../ControllerBaseDefinition.h"

#include "../../Common/SMLib/LibList/Obj.h"

#include "../Lib/NGraphics/Graphics.h"

#include <time.h>

class CInterfaceImgObj : public CObj
{
public:
	CInterfaceImgObj();
	~CInterfaceImgObj();

	void Destroy();

	TSpr* LoadInterfaceImg( TCHAR *FileName, clock_t Clock );

	bool IsSameFile( TCHAR *FileName );
	TSpr *GetImgSpr( clock_t Clock )
	{
		m_ImgLoadClock = Clock;
        return m_pImgSpr;
	}

	bool Action( clock_t Clock );

private:
	TCHAR		*m_strFileName;
	TSpr		*m_pImgSpr;
	clock_t		m_ImgLoadClock;
};

#endif