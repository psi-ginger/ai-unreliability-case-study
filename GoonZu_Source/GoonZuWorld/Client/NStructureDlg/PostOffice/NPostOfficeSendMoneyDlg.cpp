#include "NPostOfficeDlg.h"

//#include "../../InterfaceMgr/Interface/ListView.h"
//#include "../StrBaseDlg/StrInventory.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-PostOffice.h"
#include "../../AbuseFilter/AbuseFilter.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NPostOfficeSendMoneyDlg::NPostOfficeSendMoneyDlg()
{
}

NPostOfficeSendMoneyDlg::~NPostOfficeSendMoneyDlg()
{
	
}

void NPostOfficeSendMoneyDlg::InitDlg()
{
	SetControlMap( EDIT_PLAYERNAME, TEXT("editbox_playername") );
	
	SetControlMap( EDIT_CURRENTMONEY, TEXT("editbox_currentmoney") );
	SetControlMap( EDIT_MONEY, TEXT("editbox_money") );
	SetControlMap( EDIT_MONEY_UNIT, TEXT("editbox_money_unit") );
	SetControlMap( EDIT_FEE, TEXT("editbox_fee") );
	SetControlMap( EDIT_FEERATE, TEXT("editbox_feerate") );
	SetControlMap( EDIT_MSG, TEXT("editbox_msg") );
	SetControlMap( EDIT_RECEIVERNAME, TEXT("editbox_receivername") );

	SetControlMap( BUTTON_SENDMONEY, TEXT("button_sendmoney") );
	SetControlMap( BUTTON_SENDMSG, TEXT("button_sendmsg") );

	//KHY - 0629 - ID  20바이트로 제한.
	((CEdit*)m_pControlMap[ EDIT_PLAYERNAME ])->SetMaxEditTextLength(20);

	//KHY - 0813 - 메세지  127바이트로 제한.
	((CEdit*)m_pControlMap[ EDIT_MSG ])->SetMaxEditTextLength(127);

	//[김영훈 추가:2007.11.26 - 송금란에 숫자만 입력되도록 변환]
	//((CEdit*)m_pControlMap[ EDIT_MONEY ])->SetNumberMode( true );


}

void NPostOfficeSendMoneyDlg::Action()
{
	cltPostOfficeManager *pPostOfficeManager = (cltPostOfficeManager *)m_pStrManager;

	TCHAR buffer[ 256 ];
	TCHAR msg[ 256 ];
	TCHAR playername[ 128 ];
	buffer[ 0 ] = NULL;

	TCHAR* pText = GetTxtFromMgr(611);
	g_JYLibFunction.SetNumUnit( pclCM->CR[1]->pclCI->clIP.GetMoney(), buffer, 256, pText );

	SetEditText( EDIT_CURRENTMONEY, buffer );

	//----------------------
	TCHAR szMoney[ 128 ];
	GMONEY money;

	MStrCpy( szMoney, GetEditText( EDIT_MONEY ), 126 );

	money = _tstoi64( szMoney );

	pText = GetTxtFromMgr(611);
	g_JYLibFunction.SetNumUnit( money, szMoney, 128, pText );
	SetEditText( EDIT_MONEY_UNIT, szMoney );

	GMONEY fee = m_pStrManager->clClientStrInfo.clBankStrInfo.clFee.CalcFee( money );

	g_JYLibFunction.SetNumUnit( fee, szMoney, 128, pText );
	SetEditText( EDIT_FEE, szMoney );

	REAL64 feerate = (REAL64)(pPostOfficeManager->clClientStrInfo.clPostOfficeStrInfo.clMoneyFee.GetFeeRate() / 10.0);
	
	StringCchPrintf( buffer, 256, TEXT("%.1f%%"), feerate);
	SetEditText( EDIT_FEERATE, buffer );

	//----------------------
	MStrCpy( playername, GetEditText( EDIT_PLAYERNAME ), 32 );
	MStrCpy( msg, GetEditText( EDIT_MSG ), 256 );
	

	int len = _tcslen( msg );
	//[김영훈 수정 : 최대 입력 표시 글자를 127에서 126으로 수정]
	StringCchPrintf( buffer, 256, TEXT("To [%s] (%d/126)bytes"), playername, len );
	SetEditText( EDIT_RECEIVERNAME, buffer );


/*
	if( pPostOfficeManager->m_bMainExplain == FALSE ) {
		pPostOfficeManager->m_bMainExplain = TRUE;
		pPostOfficeManager->ShowPostOfficeInfo();
	}

	if( GetTickCount() - pPostOfficeManager->dwLastExplainTick > 6000 ) {
		pPostOfficeManager->ShowPostOfficeInfo();
	}
*/
}

void NPostOfficeSendMoneyDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	cltPostOfficeManager *pPostOfficeManager = (cltPostOfficeManager *)m_pStrManager;
	
	CASE_CONTROL( EDIT_MONEY ){
		switch( nEvent ) {
			case EVENT_EDITBOX_CHANGE:
			{
				NTCHARString512	kRawPrice(GetEditText( EDIT_MONEY ));
				TCHAR		szPrice[64] = { '\0', };
				GMONEY		siPrice = _tstoi64(kRawPrice);
				if( siPrice > pclClient->GetGlobalValue("GV_PostOfficeMoneyLimit"))
					siPrice = pclClient->GetGlobalValue("GV_PostOfficeMoneyLimit");
				SetEditText( EDIT_MONEY, SI64ToString( siPrice ) );
			}break;
		}
	}


	CASE_CONTROL( BUTTON_SENDMONEY )
	{
		TCHAR buf[ 256 ];
		TCHAR personName[ MAX_PLAYER_NAME ];

		MStrCpy( personName, GetEditText( EDIT_PLAYERNAME ), MAX_PLAYER_NAME );
		
		if( personName[ 0 ] == NULL )
		{
			TCHAR* pText = GetTxtFromMgr(1725);
			SetMainExplain(pText);
			return;
		}

		GMONEY money = _tstoi64( GetEditText( EDIT_MONEY ) );

		if( money < 1000 ) 
		{
			TCHAR* pText = GetTxtFromMgr(1733);
			SetMainExplain( pText );
			return;
		}
		// 우체국 송금 제한.
		if ( money > pclClient->GetGlobalValue("GV_PostOfficeMoneyLimit") )
		{
			TCHAR* pText = GetTxtFromMgr(1734);
			SetMainExplain( pText );
			return;
		}				

		float feerate = (float)(pPostOfficeManager->clClientStrInfo.clPostOfficeStrInfo.clMoneyFee.GetFeeRate() / 10.0);

		GMONEY fee = pPostOfficeManager->clClientStrInfo.clPostOfficeStrInfo.clMoneyFee.CalcFee( money );
		TCHAR* pText = GetTxtFromMgr(1735);

		TCHAR	money1[128]	=	{ '\0', };
		TCHAR	fee1[128]	=	{ '\0', };
		TCHAR	total[128]	=	{ '\0', };
		if( money > 0 )
			g_JYLibFunction.SetNumUnit(money, money1, sizeof(money1));
		if( fee > 0 )
			g_JYLibFunction.SetNumUnit(fee, fee1, sizeof(fee1));
		if( fee > 0 )
			g_JYLibFunction.SetNumUnit(money + fee, total, sizeof(total));

		Nsprintf(buf,pText,TEXT("user"), personName   ,TEXT("money"), money1   ,TEXT("fee"),  fee1  ,TEXT("total"),  total  ,NULL);
		//sprintf( buf, pText, personName, money, fee, money + fee );

		pText = GetTxtFromMgr(1736);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buf, MSGBOX_TYPE_YESNO, 2 );
		BYTE TempBuffer[ 128 ];
		SI16 Size = sizeof( personName ) + sizeof( money );
		memcpy( TempBuffer, personName, sizeof( personName ) );
		memcpy( &TempBuffer[ sizeof(personName) ], &money, sizeof( money ) );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

		//if( MessageBox(NULL, buf, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) return;

		//cltGameMsgRequest_PostOfficeSendMoney sendMsg( personName, money );

		//cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SENDMONEY, 
		//		sizeof(cltGameMsgRequest_PostOfficeSendMoney), (BYTE*)&sendMsg);
		//
		//pclClient->SendMsgToServer((sPacketHeader*)&clMsg );

	}

	else CASE_CONTROL( BUTTON_SENDMSG )
	{
		TCHAR buf[ 256 ];
		TCHAR personName[ 32 ];
		TCHAR szmsg[ 128 ];

		MStrCpy( personName, GetEditText( EDIT_PLAYERNAME ), MAX_PLAYER_NAME );
		MStrCpy( szmsg, GetEditText( EDIT_MSG ), 128 );
		
		if( personName[ 0 ] == NULL )
		{
			TCHAR* pText = GetTxtFromMgr(1725);
			SetMainExplain( pText );
			return;
		}

#ifdef _IAF_EXPORT
		if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
		{
			TCHAR ResultBuf[1024] = "";
			if( 1 == IAF::Filtering( szmsg, ResultBuf, sizeof(ResultBuf) ) )
			{
				TCHAR* pTitle = GetTxtFromMgr(1853);
				TCHAR* pText = GetTxtFromMgr(1854);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return;
			}

		}
#else
		//cyj 역창메세지에도 욕설필터 적용
		if (pclclient->m_pCAbuseFilter->CheckAbuse( szmsg ) == false)
		{
			TCHAR* pTitle = GetTxtFromMgr(1853);
			TCHAR* pText = GetTxtFromMgr(1854);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return;
		}
#endif

		TCHAR* pText = GetTxtFromMgr(1730);
		StringCchPrintf( buf, 256, 
			pText,
			personName, 
			pPostOfficeManager->clClientStrInfo.clPostOfficeStrInfo.siMsgFee );

		pText = GetTxtFromMgr(1731);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buf, MSGBOX_TYPE_YESNO, 3 );
		BYTE TempBuffer[ 256 ];
		SI16 Size = sizeof( personName ) + sizeof( szmsg );
		memcpy( TempBuffer, personName, sizeof( personName ) );
		memcpy( &TempBuffer[ sizeof(personName) ], szmsg, sizeof( szmsg ) );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

		//if( MessageBox(NULL, buf, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) return;;

		//cltGameMsgRequest_PostOfficeSendMsg sendMsg( personName, szmsg );
		//
		//cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SENDMSG, 
		//		sizeof(cltGameMsgRequest_PostOfficeSendMsg), (BYTE*)&sendMsg);
		//
		//pclClient->SendMsgToServer((sPacketHeader*)&clMsg );
	}
}

void NPostOfficeSendMoneyDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 2:	// money 보내기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				TCHAR personName[20];
				memcpy( personName, pData, sizeof(personName));
				SI64 money = *((SI64*)(pData + sizeof(personName) ));

				cltGameMsgRequest_PostOfficeSendMoney sendMsg( personName, money );

				cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SENDMONEY, 
						sizeof(cltGameMsgRequest_PostOfficeSendMoney), (BYTE*)&sendMsg);
				
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg );
			}
		}
		break;
	case 3:	// 메세지 보내기
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				TCHAR personName[32];
				TCHAR szmsg[128];
				memcpy( personName, pData, sizeof(personName));
				memcpy( szmsg, pData + sizeof(personName), sizeof(szmsg) );

				cltGameMsgRequest_PostOfficeSendMsg sendMsg( personName, szmsg );
				
				cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SENDMSG, 
						sizeof(cltGameMsgRequest_PostOfficeSendMsg), (BYTE*)&sendMsg);
				
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg );
			}
		}
		break;
	}
}
