#ifndef _CITYHALLSUGGESTDLG_H
#define _CITYHALLSUGGESTDLG_H

#include "CommonHeader.h"
#include "StructureClass/StructureClass.h"

class cltStructureMngBasic;

class cltSuggestDlg
{
public:
	cltSuggestDlg();
	~cltSuggestDlg(); 

	void Create( HINSTANCE hInst, HWND hParent, cltStructureMngBasic *pclStructureManager );

public:

	cltStructureMngBasic*		m_pclStructureManager;

};

class cltGM_AppointmentChiefDlg
{
public:
	cltGM_AppointmentChiefDlg();
	~cltGM_AppointmentChiefDlg();

public:


};

#endif
