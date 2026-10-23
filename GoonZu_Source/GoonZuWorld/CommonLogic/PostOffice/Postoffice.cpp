#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"

#include "PostOffice.h"
#include "..\..\Resource.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/msg/MsgType-PostOffice.h"

#include "../CityHall/TaxpaymentListDlg.h"
#include "../Server/Server.h"

//#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Stock.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;


//----------------------------------------------
// cltPostOffice
//----------------------------------------------
cltPostOffice::cltPostOffice(SI32 sivillageunique) : cltStructureBasic(sivillageunique)
{

}


cltPostOffice::~cltPostOffice()
{

}




cltPostOfficeManager::cltPostOfficeManager(cltCharManager* pclcm, SI32 gamemode, SI32 ranktype, TCHAR *strFileName ) : cltStructureMngBasic(MAX_STOCK_TAB_NUMBER, pclcm, ranktype,  strFileName )
{
	
	m_hwndRecvList = NULL;

	m_pclMyItem = new cltItem[ MAX_ITEM_PER_PERSON ];


}

cltPostOfficeManager::~cltPostOfficeManager()
{

	if ( m_pclMyItem ) delete [] m_pclMyItem;

}



// 최초 환영 인사말표시..
void cltPostOfficeManager::GreetingMsg(HWND hDlg)
{
	TCHAR* pText = GetTxtFromMgr(1738);
	SetMainExplain(hDlg, pText);
}

void cltPostOfficeManager::Do_WM_INITDIALOG(SI32 charunique )
{
	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);
}


// 우체국을 신설한다.
BOOL cltPostOfficeManager::CreatePostOffice(SI32 siunique)
{
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltPostOffice(siunique);
		return TRUE;
	}

	return FALSE;
}


