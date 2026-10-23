#pragma once


#include "NewMarket.h"

class cltNewMarketManager : public cltStructureMngBasic
{
public:
	cltNewMarketManager( cltCharManager* _pclCM, SI32 _siGamemode, SI32 _siRanktype, TCHAR* _pszFileName );
	~cltNewMarketManager();

public: 
	void 		Init( void );
	void 		Action( bool _bDaychangedswitch );

	// 최초 환영 인사말표시 . 
	void		GreetingMsg( HWND _hDlg );
	void		Do_WM_INITDIALOG( SI32 _siCharUnique );

	BOOL 		CreateNewMarket( SI32 _siVillageUnique );

	
private:
	

};