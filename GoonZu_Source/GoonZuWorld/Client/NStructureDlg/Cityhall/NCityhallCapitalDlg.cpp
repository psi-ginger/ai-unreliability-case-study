#include "NCityhallDlg.h"

//#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "MsgType-Stock.h"

#include "..\..\Client\Music\Music.h"

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NCityhallCapitalDlg::NCityhallCapitalDlg()
{
}

NCityhallCapitalDlg::~NCityhallCapitalDlg()
{
}

void NCityhallCapitalDlg::InitDlg()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	SetControlMap( BUTTON_REQUESTSTOCK, TEXT("button_requeststock") );
	SetControlMap( BUTTON_TOCAPITALFUND, TEXT("button_tocapitalfund") );

	SetControlMap( EDIT_STOCKMONEY, TEXT("editbox_stockmoney") );
	SetControlMap( EDIT_MONEY, TEXT("editbox_money") );

	SetControlMap( STATIC_ADDCAPITAL_TITLE, TEXT("statictext_NONAME1") );
	SetControlMap( STATIC_DESC1, TEXT("statictext_NONAME2") );

	SetControlMap( STATIC_ADDCAPITAL, TEXT("statictext_NONAME5") );

	SetControlMap( OUTLINE_APPCAPITAL, TEXT("outline_NONAME2") );

	if(	pclClient->IsCountrySwitch( Switch_AddCapitalRemove ) )
	{
		m_pControlMap[ EDIT_STOCKMONEY ]->Show( false );
		m_pControlMap[ BUTTON_REQUESTSTOCK ]->Show( false );

		m_pControlMap[ STATIC_ADDCAPITAL_TITLE ]->Show( false );
		m_pControlMap[ STATIC_DESC1 ]->Show( false );
		m_pControlMap[ STATIC_ADDCAPITAL ]->Show( false );
		m_pControlMap[ OUTLINE_APPCAPITAL ]->Show( false );
	}
}

void NCityhallCapitalDlg::Action()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
}

void NCityhallCapitalDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	TCHAR *pText;

	CASE_CONTROL( BUTTON_REQUESTSTOCK )
	{
		// 증자 요청
		cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
		if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
		{
			char* pText;
			if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
			else pText = GetTxtFromMgr(1440);
			SetMainExplain(pText);
			return;
		}

		SI32 villageunique = pclCM->CR[1]->GetCurrentVillageUnique();

		if( villageunique < 0 )
		{
			pText = GetTxtFromMgr(6006);
			SetMainExplain( pText );
			return;
		}

		CEdit* pEdit = (CEdit *)m_pControlMap[ EDIT_STOCKMONEY ];
		SI64 tempStockMoney = _atoi64( pEdit->GetText() );

		// 주식 가격제한
		if( tempStockMoney < LIMIT_STOCKMONEY )
		{
			pText = GetTxtFromMgr(6007);
			SetMainExplain( pText );
			return;
		}

		cltGameMsgRequest_BidStock clInfo( villageunique, tempStockMoney );

		cltMsg clMsg(GAMEMSG_REQUEST_BIDSTOCK, sizeof(clInfo), (BYTE*)&clInfo);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);	

		pEdit->SetText(TEXT(""));
	}
	else CASE_CONTROL( BUTTON_TOCAPITALFUND )
	{

		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				char* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		GMONEY money = _atoi64( GetEditText( EDIT_MONEY ) );

		if( money == 0 )
		{
			char* pText = GetTxtFromMgr(1512);
			SetMainExplain( pText );
			return;
		} 

		if( money > 5000000000 )
		{
			char* pText = GetTxtFromMgr(1513);
			SetMainExplain( pText );
			return;
		}

		cltGameMsgRequest_ProfitToCapitalFund sendMsg( money );

		cltMsg clMsg( GAMEMSG_REQUEST_PROFITTOCAPITALFUND, sizeof( cltGameMsgRequest_ProfitToCapitalFund ), (BYTE *)&sendMsg );
		pclClient->pclCM->CR[ 1 ]->SendNetMsg( (sPacketHeader*)&clMsg );

	}
}


//NCityhallCapitalDlg::NCityhallCapitalDlg()
//{
//}
//
//NCityhallCapitalDlg::~NCityhallCapitalDlg()
//{
//}
//
//void NCityhallCapitalDlg::InitDlg()
//{
//	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
//
//	SetControlMap( BUTTON_DIVIDEND, TEXT("button_dividend") );
//	SetControlMap( BUTTON_RESIDENTSHARE, TEXT("button_residentshare") );
//	SetControlMap( BUTTON_TOCAPITALFUND, TEXT("button_tocapitalfund") );
//
//	SetControlMap( EDIT_DIVIDEND, TEXT("editbox_dividend") );
//	SetControlMap( EDIT_RESIDENTSHARE, TEXT("editbox_residentshare") );
//	SetControlMap( EDIT_MONEY, TEXT("editbox_money") );
//
//	SetControlMap( COMBO_RESIDENTSHARE, TEXT("combobox_residentshare") );
//
//	CComboBox *pCombo = (CComboBox *)m_pControlMap[ COMBO_RESIDENTSHARE ];
//	stComboBoxItemData itemdata;
//
//	// 콤보 박스 설정
//	TCHAR* pText = GetTxtFromMgr(1509);
//
//	itemdata.Init();
//	itemdata.Set( pText );
//
//	pCombo->AddItem( &itemdata );
//	pCombo->SetCursel( 0 );
//
//}
//
//void NCityhallCapitalDlg::Action()
//{
//	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
//}
//
//void NCityhallCapitalDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
//{
//	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
//
//	CASE_CONTROL( BUTTON_DIVIDEND )
//	{
//		// 배당하기
//
//		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
//		{
//			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
//			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
//			{
//				TCHAR* pText;
//				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
//				else pText = GetTxtFromMgr(1440);
//				SetMainExplain(pText);
//				return;
//			}
//		}
//
//
//		SI32 id = 1;
//
//		//TCHAR buf[ 256 ];
//		cltMoney clMoney;
//
//		int totdividend = _tstoi( GetEditText( EDIT_DIVIDEND ) );
//
//		if( totdividend <= 0 ) {
//			
//			TCHAR* pText = GetTxtFromMgr(1510);
//			SetMainExplain( pText);
//			pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );
//
//		} else if( totdividend < 100000 ) {
//			TCHAR* pText = GetTxtFromMgr(1511);
//			SetMainExplain( pText);
//			pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );
//
//		} else {
//
//			clMoney.Set( totdividend );
//
//			cltGameMsgRequest_PayDividend clPayDividend( pCityhallManager->siVillageUnique, &clMoney );
//
//			cltMsg clMsg( GAMEMSG_REQUEST_PAYDIVIDEND, sizeof(cltGameMsgRequest_PayDividend), (BYTE*)&clPayDividend );
//			
//			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
//
//
//		}				
//	}
//
//	else CASE_CONTROL( BUTTON_RESIDENTSHARE )
//	{
//
//		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
//		{
//			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
//			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
//			{
//				TCHAR* pText;
//				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
//				else pText = GetTxtFromMgr(1440);
//				SetMainExplain(pText);
//				return;
//			}
//		}
//
//		cltMoney clTotalShare;
//		
//		clTotalShare.Set( _tstoi64( GetEditText( EDIT_RESIDENTSHARE ) ) );
//		
//		SI32 shareType = 1;
//
//		cltGameMsgRequest_ResidentShare share( &clTotalShare, shareType );
//
//		cltMsg clMsg( GAMEMSG_REQUEST_RESIDENTSHARE, sizeof( cltGameMsgRequest_ResidentShare ), (BYTE *)&share );
//		pclClient->pclCM->CR[ 1 ]->SendNetMsg((sPacketHeader*)&clMsg );
//		
//
//	}
//
//	else CASE_CONTROL( BUTTON_TOCAPITALFUND )
//	{
//	
//		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
//		{
//			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
//			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
//			{
//				TCHAR* pText;
//				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
//				else pText = GetTxtFromMgr(1440);
//				SetMainExplain(pText);
//				return;
//			}
//		}
//
//		int money = _tstoi( GetEditText( EDIT_MONEY ) );
//
//		if( money == 0 ) {
//			TCHAR* pText = GetTxtFromMgr(1512);
//			SetMainExplain( pText );
//			return;
//		} 
//
//		if( money > 100000000 ) {
//			TCHAR* pText = GetTxtFromMgr(1513);
//			SetMainExplain( pText );
//			return;
//		}
//
//		cltGameMsgRequest_ProfitToCapitalFund sendMsg( money );
//
//		cltMsg clMsg( GAMEMSG_REQUEST_PROFITTOCAPITALFUND, sizeof( cltGameMsgRequest_ProfitToCapitalFund ), (BYTE*)&sendMsg );
//		pclClient->pclCM->CR[ 1 ]->SendNetMsg((sPacketHeader*)&clMsg );
//
//	}
//
//}