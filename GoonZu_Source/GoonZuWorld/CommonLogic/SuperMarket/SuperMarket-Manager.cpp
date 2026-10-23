#include <CommonLogic.h>
#include "../../Server/Server.h"
#include "../../Client/Client.h"

#include "NewMarket-Manager.h"

cltNewMarketManager::cltNewMarketManager( cltCharManager* _pclCM, SI32 _siGamemode, SI32 _siRanktype, TCHAR* _pszFileName )
: cltStructureMngBasic( MAX_MARKET_TAB_NUMBER, _pclCM, _siRanktype, _pszFileName )
{
	
}

cltNewMarketManager::~cltNewMarketManager()
{

}

void cltNewMarketManager::Init( void )
{

}

void cltNewMarketManager::Action( bool _bDaychangedswitch )
{
	cltServer* pclserver = (cltServer*)pclClient;

	// DB 초기화 완료될때까지 기다림
	if( ((cltServer*)pclClient)->bCompleteDBInitAll == FALSE ) return;

	

}

void cltNewMarketManager::GreetingMsg( HWND _hDlg )
{
	SetMainExplain( _hDlg,  "이거 뭐지?" ); //GetTxtFromMgr(1667); 진성 TEXT
}

void cltNewMarketManager::Do_WM_INITDIALOG( SI32 _siCharUnique )
{
	cltStructureMngBasic::Do_WM_INITDIALOG( _siCharUnique );
}

BOOL cltNewMarketManager::CreateNewMarket( SI32 _siVillageUnique )
{
	// 객주를 만든다. 
	if( NULL == pclStruct[_siVillageUnique] )
	{
		pclStruct[_siVillageUnique] = new cltNewMarket( _siVillageUnique );
		return TRUE;
	}



	return FALSE;
}


