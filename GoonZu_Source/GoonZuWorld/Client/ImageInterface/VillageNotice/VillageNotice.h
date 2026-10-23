#ifndef _VILLAGENOTICE_H_
#define _VILLAGENOTICE_H_

#include <Graphic_Capsule.h>
#include "TSpr.h"
#include "NStructureDlg/Cityhall/NStructureMessageDlg.h"


class CVillageNotice
{
public:
	CVillageNotice();
	~CVillageNotice();

	bool Set( SI32 VillageUnique, SI32 XPos, SI32 YPos );
	void Set( TCHAR *szVillageNotice, bool SetVillageNotice );
	void Draw( LPDIRECTDRAWSURFACE7 lpsurface, SI32 ScreenXPos, SI32 ScreenYPos );

private:
	SI32 m_siVillageUnique;
	bool m_bSetVillageNotice;
	TCHAR m_strVillageNotice[ MAX_VILLAGENOTICE_LENGTH ];
	TSpr	m_VillageNoticeSpr;

	SI32 m_siXPos;
	SI32 m_siYPos;
};

#endif