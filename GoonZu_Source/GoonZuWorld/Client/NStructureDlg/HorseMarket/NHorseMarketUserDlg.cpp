#include "NHorseMarketDlg.h"

#include "..\..\Server\Server.h"
#include "Char\CharManager\CharManager.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-HorseMarket.h"

#include <tchar.h>
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Static.h"

#include "../../../CommonLogic/MsgType-Structure.h"
#include "../../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

extern cltCommonLogic* pclClient;

NHorseMarketUserDlg::NHorseMarketUserDlg()
{
}

NHorseMarketUserDlg::~NHorseMarketUserDlg()
{
}

void NHorseMarketUserDlg::InitDlg()
{
	// 사복시 매니져
	cltHorseMarketManager *pHorseMarketManager = ((cltHorseMarketManager *)m_pStrManager);
	if( pHorseMarketManager == NULL ) return;

	SetControlMap(	BUTTON_SELL	,TEXT("button_sell"));
	SetControlMap(	BUTTON_CANCELORDER	,TEXT("button_cancelorder"));
	SetControlMap(	EDITBOX_REALFEE	,TEXT("editbox_realfee"));
	SetControlMap(	EDITBOX_SELLPRICE	,TEXT("editbox_sellprice"));
	SetControlMap(	EDITBOX_SELLPRICECHAR	,TEXT("editbox_sellpricechar"));
	
	SetControlMap(	EDITBOX_MYHORSE	,TEXT("editbox_myhorse"));
	SetControlMap(	EDITBOX_MYORDER	,TEXT("editbox_myorder"));
	SetControlMap(	EDITBOX_EXPLAIN	,TEXT("editbox_explain"));

	SetControlMap(  STATICTEXT_FEERATE, TEXT("statictext_feerate" ));

	CEdit *pEdit_sellprice = (CEdit*)m_pControlMap[ EDITBOX_SELLPRICE ];
	pEdit_sellprice->SetNumberMode( true );

	pHorseMarketManager->clMyAccount.Init();

	ShowMyHorseInfo();
	ShowMyOrderInfo();

	SI32 id = 1;

	// 내 주문 정보 요청 
	cltMsg clMsg( GAMEMSG_REQUEST_HORSEMARKET_MYACCOUNT, 0 );
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void NHorseMarketUserDlg::Action()
{

	// 사복시 매니져
	cltHorseMarketManager *pHorseMarketManager = ((cltHorseMarketManager *)m_pStrManager);
	if( pHorseMarketManager == NULL ) return;

	// 수수료 정보를 표시한다. 
	ShowFee();

	if( m_pBaseDlg->m_dwActionCount % 5 == 0 ) {
		ShowMyHorseInfo();
		ShowMyOrderInfo();
	}

	TCHAR buffer[256];
	TCHAR *pchTmp = NULL;
	pchTmp = GetEditText( EDITBOX_SELLPRICE );
	if( pchTmp == NULL ) return;
	if( _tcslen(pchTmp) >= 50 ) return;
	_tcscpy( buffer, pchTmp );
	SI64 price = _tstoi64(buffer);

	TCHAR* pText = GetTxtFromMgr(611);
	g_JYLibFunction.SetNumUnit(price, buffer, 256, pText);
	SetEditText( EDITBOX_SELLPRICECHAR, buffer );

	ShowRealFee( price );


}

void NHorseMarketUserDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	// 사복시 매니져
	cltHorseMarketManager *pHorseMarketManager = ((cltHorseMarketManager *)m_pStrManager);
	if( pHorseMarketManager == NULL ) return;

	SI32 id = 1;

	CASE_CONTROL( BUTTON_SELL )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				cltHorse* pclHorse = pclCM->CR[ 1 ]->pclCI->clHorseInfo.GetCurrentShowHorse();
				if( pclHorse == NULL)		return;

				// 팔수 있는 말인지 체크한다.
				// KHY - 0622 - 군주S 초보자용  망아지 추가.   //KHY - 0911 - 호랑이 추가.
				// PCK : 낙타추가 (08.03.06)
				// PCK : 이벤트용 호랑이 추가 (08.04.01)
				// PCK : 코끼리 추가 (08.06.12)
				// PCK : 백호 추가 (08.08.14)
				// PCK : 오토바이 추가 (08.09.02)
				// PCK : 불해치 추가 (08.12.10)
				// 체험용 백마 : 손성웅-2009.03.17
				// PCK : 바포메트 추가 (09.07.30)
				switch ( pclHorse->siHorseUnique )
				{
					case HORSEUNIQUE_WOLF:
					case HORSEUNIQUE_FOAL:
					case HORSEUNIQUE_TIGER:
					case HORSEUNIQUE_CAMEL:
					case HORSEUNIQUE_TIGER2:
					case HORSEUNIQUE_ELEPHANT:
					case HORSEUNIQUE_WHITETIGER:
					case HORSEUNIQUE_MOTORCYCLE:
					case HORSEUNIQUE_FIREHATCH:
					case HORSEUNIQUE_WHITE_TEST:
					case HORSEUNIQUE_BAPHOMET:
					case HORSEUNIQUE_GRAYWOLF:
					case HORSEUNIQUE_SASURI:
						{
							// 팔수없다는 메시지
							TCHAR * pTitle = GetTxtFromMgr(512);
							TCHAR * pText = GetTxtFromMgr(7043);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

							return;
						}
						break;
				}	// switch ( pclHorse->siHorseUnique ) 종료

				//KHY - 1001 - 기승동물 5마리 보유 수정.
				// 말을 타고 있는 상태에서, 말을 넣어 버리면, 캐릭터 조합쪽에 에러 날수 있으므로...
				if ( pclClient->pclCM->CR[1]->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == true )
				{
					TCHAR* pTitle = GetTxtFromMgr(372);
					TCHAR* pText = GetTxtFromMgr(7209);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					return;
				}

				TCHAR buffer[ 256 ];
				GMONEY price;

				price = _tstoi64( GetEditText( EDITBOX_SELLPRICE ) );

				TCHAR* pText = NULL;
				if( price <= 0 ) {
					pText = GetTxtFromMgr(1758);
					SetMainExplain( pText );
 					break;
				}

				if( price < 5000 ) {
					pText = GetTxtFromMgr(1759);
					SetMainExplain( pText );
					break;
				}
				
				if( price > 5000000000 ) {
					SI64 tempPrice = 5000000000	;
					NTCHARString128	strText( GetTxtFromMgr(1760) );
					strText.Replace( TEXT("#money#"), SI64ToString(tempPrice) );
					SetMainExplain( strText );
					break;
				}

				if( pclClient->pclCM->CR[id]->pclCI->clCharItem.IsHorseHaveItem() == TRUE ) {
					pText = GetTxtFromMgr(1761);
					SetMainExplain( pText );
					break;
				}

				// 전장에 계좌가 있는지 체크 한다.
				SI32 siBankCheck = pclClient->pclVillageManager->IsThereStructure(pclCM->CR[id]->GetCurrentVillageUnique(),RANKTYPE_BANK);
				if(pclCM->CR[id]->pclCI->clBank.clAccountNum.siVillageUnique == 0 && siBankCheck==1)
				{
					pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
					TCHAR* pText = GetTxtFromMgr(1669);
					SetMainExplain(pText);

					break;
				}


				GMONEY tax = (GMONEY)((double)price * 0.01);

				pText = GetTxtFromMgr(1762);

				StringCchPrintf(buffer, 256, pText, tax);

				pText = GetTxtFromMgr(1763);

				stMsgBox MsgBox;
				BYTE TempBuffer[ 128 ];
				// 현재 dialog가 탭 창 같은 Child Dialog일때
				CControllerMgr *pParent = NULL;
				pParent = this->GetParentDialog();
				if(pParent == NULL ) return;
				SI16 Size = sizeof( id ) + sizeof( price );
				memcpy( TempBuffer, &id, sizeof( id ) );
				memcpy( &TempBuffer[ sizeof( id ) ], &price, sizeof( price ) );
				MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 0 );
				pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
               
				//if (  MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) == IDNO ) 
				//{
				//	break;
				//}

				//cltGameMsgRequest_HorseMarketSellHorse sendMsg( 0, price );
				//cltMsg clMsg( GAMEMSG_REQUEST_HORSEMARKET_SELLHORSE, sizeof( sendMsg ), (BYTE*)&sendMsg );
				//pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );

			}
			break;
		}
	}

	else CASE_CONTROL( BUTTON_CANCELORDER )
	{
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			{
				// 회수 
				cltMsg clMsg( GAMEMSG_REQUEST_HORSEMARKET_OUTHORSE, 0 );
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );

			}
			break;
		}
	}

}

void NHorseMarketUserDlg::ShowMyHorseInfo( )
{
	TCHAR buf[ 1024 ];

	cltHorse *pclHorse = pclCM->CR[ 1 ]->pclCI->clHorseInfo.GetCurrentShowHorse();

	if( pclHorse != NULL && pclHorse->siHorseUnique )
	{
		pclClient->pclHorseManager->GetText( pclHorse, buf, 1024 );
	} 
	else
	{
		TCHAR* pText = GetTxtFromMgr(1764);
		StringCchCopy( buf, 1024, pText );
	}

	SetEditText( EDITBOX_MYHORSE, buf );
}

void NHorseMarketUserDlg::ShowMyOrderInfo( )
{
	// 사복시 매니져
	cltHorseMarketManager *pHorseMarketManager = ((cltHorseMarketManager *)m_pStrManager);
	if( pHorseMarketManager == NULL ) return;

	TCHAR buf[ 1024 ];

	cltHorse *pclHorse = &pHorseMarketManager->clMyAccount.clHorse;

	if( pclHorse != NULL && pclHorse->siHorseUnique ) 
	{
		if(pclClient->pclHorseManager->GetText( pclHorse, buf, 1024 ) == false)return ;
	}
	else 
	{
		TCHAR* pText = GetTxtFromMgr(1765);
		StringCchCopy( buf, 1024, pText );
	}

	SetEditText( EDITBOX_MYORDER, buf );
}

// 수수료 정보를 보여준다.
BOOL NHorseMarketUserDlg::ShowFee()
{
	// 사복시 매니져
	cltHorseMarketManager *pHorseMarketManager = ((cltHorseMarketManager *)m_pStrManager);
	if( pHorseMarketManager == NULL ) return false;

	CStatic *pStatic_feerate = (CStatic*)m_pControlMap[ STATICTEXT_FEERATE ];
	TCHAR buffer[256];
	float feerate = (float)(pHorseMarketManager->clClientStrInfo.clHorseMarketStrInfo.clFee.GetFeeRate() / 10);

	StringCchPrintf(buffer, 256, TEXT("%3.1f%%"), feerate);
    pStatic_feerate->SetText( buffer, DT_LEFT | DT_VCENTER | DT_SINGLELINE );

	return TRUE;
}

// 실제로 지불해야 할 금액을 보여준다. 
BOOL NHorseMarketUserDlg::ShowRealFee(GMONEY price)
{
	// 사복시 매니져
	cltHorseMarketManager *pHorseMarketManager = ((cltHorseMarketManager *)m_pStrManager);
	if( pHorseMarketManager == NULL ) return false;

	TCHAR buffer[256];

	StringCchPrintf(buffer, 256, TEXT("%I64d"), pHorseMarketManager->clClientStrInfo.clHorseMarketStrInfo.clFee.CalcFee( price ));
	SetEditText( EDITBOX_REALFEE, buffer );

	return TRUE;
}

void NHorseMarketUserDlg::ShowMyHorseList()
{

}

void NHorseMarketUserDlg::ShowMyOrderList()
{

}

void NHorseMarketUserDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	// 사복시 매니져
	cltHorseMarketManager *pHorseMarketManager = ((cltHorseMarketManager *)m_pStrManager);
	if( pHorseMarketManager == NULL ) return ;

	switch( MsgIndex )
	{
	case 0:	// 말 팔기
		{
			if( pData == NULL || DataLen < 1 )
				return;

			if( RetValue )
			{
				SI32 id = *((SI32*)pData);
				GMONEY price = *((GMONEY*)(pData + 4 ));

				//KHY - 1001 - 기승동물 5마리 보유 수정.
				cltGameMsgRequest_HorseMarketSellHorse sendMsg( pclCM->CR[ 1 ]->pclCI->clHorseInfo.GetCurrentShowHorseIndex(), price );
				cltMsg clMsg( GAMEMSG_REQUEST_HORSEMARKET_SELLHORSE, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
			}
		}
		break;
	}
}
