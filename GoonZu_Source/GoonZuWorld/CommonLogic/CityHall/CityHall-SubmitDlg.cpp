#include "CityHall-SubmitDlg.h"


#include <CommonLogic.h>

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "CityHall.h"


#include "..\..\Server\Rank\Rank.h"

#include "..\..\Resource.h"

#include "MsgType-Structure.h"

extern cltCommonLogic* pclClient;

cltSuggestDlg g_cityhallSuggestdlg;

cltSuggestDlg::cltSuggestDlg()
{
}

cltSuggestDlg::~cltSuggestDlg()
{
}


void cltSuggestDlg::Create( HINSTANCE hInst, HWND hParent, cltStructureMngBasic *pclStructureManager )
{

	m_pclStructureManager = pclStructureManager;
}



//----------------------------------------------------------------------------------------------------

cltGM_AppointmentChiefDlg	g_appointmentChiefDlg;

cltGM_AppointmentChiefDlg::cltGM_AppointmentChiefDlg()
{
}

cltGM_AppointmentChiefDlg::~cltGM_AppointmentChiefDlg()
{

}

