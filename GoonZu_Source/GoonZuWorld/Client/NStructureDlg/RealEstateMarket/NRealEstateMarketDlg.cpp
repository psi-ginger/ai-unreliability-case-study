#include "NRealEstateMarketDlg.h"

#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"
#include "../Client/NStructureDlg/House/NHouseDlg.h"
#include "../Client/NStructureDlg/House/UserInfo/NHouseUserInfoBaseDlg.h"
#include "../Client/NStructureDlg/House/UserInfo/NHouseUserHorseDlg.h"
#include "../Client/NStructureDlg/House/UserInfo/NHouseUserManagementDlg.h"
#include "../Client/NStructureDlg/House/UserInfo/NHouseUserItemDlg.h"


extern cltCommonLogic *pclClient;


NRealEstateMarketDlg::NRealEstateMarketDlg()
{

}

NRealEstateMarketDlg::~NRealEstateMarketDlg()
{

}

void NRealEstateMarketDlg::Create()
{
	cltClient *pclclient = ( cltClient * ) pclClient;

	CreateDlg( NREALESTATEMARKET_DLG, pclClient->pclRealEstateMarketManager );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	TCHAR* pText = GetTxtFromMgr(3481);
	AddUserTabDialog( 0, pText, 80, new NRealEstateMarketUserDlg, TEXT("NInterface/Data/NStructureDlg/DLG_RealEstateMarketUserDLG.ddf"), TEXT("dialog_realestatemarket_user") );
	pText = GetTxtFromMgr(3482);
	AddUserTabDialog( 1, pText, 80, new NRealEstateMarketBuyDlg, TEXT("NInterface/Data/NStructureDlg/DLG_RealEstateMarketBuyDLG.ddf"), TEXT("dialog_realestatemarket_buy") );
	pText = GetTxtFromMgr(3483);
	AddUserTabDialog( 2, pText, 80, new NStrInfoDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );	

	// 행수 메뉴
	SetTabInfo( 1, 1 );
	pText = GetTxtFromMgr(3442);
	AddChiefTabDialog( 0, pText, 60, new NRealEstateMarketChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_RealEstateMarketChiefDlg.ddf"), TEXT("dialog_realestatemarketchief") );
	pText = GetTxtFromMgr(3443);
	AddChiefTabDialog( 1, pText, 60, new NStrPayTaxDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf"), TEXT("dialog_strpaytax") );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 2, pText, 60, new NStrRepairDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair") );
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 3, pText, 60, new NStrResignChiefDlg, TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );
 
	InitDlg();

	if(pclclient->m_pDialog[NHOUSE_DLG] != NULL)
	{
		((NHouseDlg*)pclclient->m_pDialog[NHOUSE_DLG])->DeleteDialog()	;
	}
	if( pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG] != NULL )
	{
		((CNHouseUserInfoBaseDlg*)pclclient->m_pDialog[NHOUSEUSERINFOBASE_DLG])->DeleteDialog()	;
	}


	SetActionTimer( 200 );
	SetTimerOnOff( true );
}

void NRealEstateMarketDlg::GreetingMsg()
{
	TCHAR* pText = GetTxtFromMgr(1779);
	SetMainExplain( pText );
}

void NRealEstateMarketDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 부동산 팔기
		((NRealEstateMarketUserDlg*)m_pUserChildDlg[ 0 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;
	}
}