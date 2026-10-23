#include "NCityhallDlg.h"

//#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "Msg\MsgType-Village.h"

#include "..\..\Client\Music\Music.h"

#include "../Lib/JYLibFunction.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NCityhallGodDlg::NCityhallGodDlg()
{
}

NCityhallGodDlg::~NCityhallGodDlg()
{
}

void NCityhallGodDlg::InitDlg()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	SetControlMap( BUTTON_GOD_KIND, TEXT("button_god_kind") );
	SetControlMap( BUTTON_GOD_LIFEUP, TEXT("button_god_lifeup") );
	SetControlMap( BUTTON_GOD_LEVELUP, TEXT("button_god_levelup") );

	SetControlMap( BUTTON_CASTLE_LIFEUP, TEXT("button_castle_lifeup") );
	SetControlMap( BUTTON_CASTLE_LEVELUP, TEXT("button_castle_levelup") );

	SetControlMap( EDIT_GOD_LASTUPDATE, TEXT("editbox_god_lastupdate") );
	SetControlMap( EDIT_GOD_KIND, TEXT("editbox_god_kind") );
	SetControlMap( EDIT_GOD_KINDEXP, TEXT("editbox_god_kindexp") );
	SetControlMap( EDIT_GOD_LIFE, TEXT("editbox_god_life") );
	SetControlMap( EDIT_GOD_LEVEL, TEXT("editbox_god_level") );

	SetControlMap( EDIT_CASTLE_LASTUPDATE, TEXT("editbox_castle_lastupdate") );
	SetControlMap( EDIT_CASTLE_KIND, TEXT("editbox_castle_kind") );
	SetControlMap( EDIT_CASTLE_KINDEXP, TEXT("editbox_castle_kindexp") );
	SetControlMap( EDIT_CASTLE_LIFE, TEXT("editbox_castle_life") );
	SetControlMap( EDIT_CASTLE_LEVEL, TEXT("editbox_castle_level") );

	SetControlMap( COMBO_GOD_KIND, TEXT("combobox_god_kind") );

	CComboBox *pCombo = (CComboBox *)m_pControlMap[ COMBO_GOD_KIND ];
	stComboBoxItemData itemdata;

	// 콤보설정 
	cltKindInfo *pclKindInfo;
	for( int i = 0; i < MAX_KINDINFO_NUMBER; ++i ) {
		
		pclKindInfo = pclClient->pclKindInfoSet->pclKI[ i ];
		if( pclKindInfo == NULL ) continue;
		if(pclKindInfo->IsAtb(ATB_DEFAULTGOD|ATB_GOD) == false)continue;

		itemdata.Init();
		itemdata.Set( (TCHAR *)pclKindInfo->GetName() );

		pCombo->AddItem( &itemdata );
	}	


}

void NCityhallGodDlg::Action()
{
	TCHAR buffer[ 256 ];

	if( m_pBaseDlg->m_dwActionCount % 3 ) return;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	cltDate *pDate;

	// 성황신 
	cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[ pCityhallManager->clClientStrInfo.clCityhallStrInfo.siSymbolKind ];

	if( pclKindInfo ) {
		SetEditText( EDIT_GOD_KIND, (TCHAR *)pclKindInfo->GetName() );
	}


	SI32 life = pCityhallManager->clClientStrInfo.clCityhallStrInfo.siSymbolLife;
	SI32 maxlife = pCityhallManager->clClientStrInfo.clCityhallStrInfo.siSymbolMaxLife;

	if( maxlife == 0 ) life = 0;

	TCHAR buf[64];
	TCHAR buf2[64];
	g_JYLibFunction.SetNumUnit( life, buf, 64 );
	g_JYLibFunction.SetNumUnit( maxlife, buf2, 64 );

	StringCchPrintf( buffer, 256, TEXT("%s/%s"), buf, buf2 );
	SetEditText( EDIT_GOD_LIFE, buffer );

	_itot( pCityhallManager->clClientStrInfo.clCityhallStrInfo.siSymbolCapa - 99, buffer, 10 );
	SetEditText( EDIT_GOD_LEVEL, buffer );
	
	pDate = &pCityhallManager->clClientStrInfo.clCityhallStrInfo.clLastSymbolUpdate;
	TCHAR* pText = GetTxtFromMgr(1514);

	StringCchPrintf( buffer, 256, pText, 
		pDate->uiYear, pDate->uiMonth, pDate->uiDay );

	SetEditText( EDIT_GOD_LASTUPDATE, buffer );
	

	// 성벽 
	life = pCityhallManager->clClientStrInfo.clCityhallStrInfo.siCastleLife;
	maxlife = pCityhallManager->clClientStrInfo.clCityhallStrInfo.siCastleMaxLife;

	if( maxlife == 0 ) life = 0;

	g_JYLibFunction.SetNumUnit( life, buf, 64 );
	g_JYLibFunction.SetNumUnit( maxlife, buf2, 64 );

	StringCchPrintf( buffer, 256, TEXT("%s/%s"), buf, buf2 );
	SetEditText( EDIT_CASTLE_LIFE, buffer );

	_itot( pCityhallManager->clClientStrInfo.clCityhallStrInfo.siCastleCapa - 99, buffer, 10 );
	SetEditText( EDIT_CASTLE_LEVEL, buffer );

	pDate = &pCityhallManager->clClientStrInfo.clCityhallStrInfo.clLastCastleUpdate;
	pText = GetTxtFromMgr(1514);

	StringCchPrintf( buffer, 256, pText, 
		pDate->uiYear, pDate->uiMonth, pDate->uiDay );

	SetEditText( EDIT_CASTLE_LASTUPDATE, buffer );


}

void NCityhallGodDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	TCHAR buffer[ 256 ];
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	CComboBox *pCombo = (CComboBox *)m_pControlMap[ COMBO_GOD_KIND ];
	stComboBoxItemData *pitemdata;
	
	CASE_CONTROL( COMBO_GOD_KIND )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{
				// 사용자 유효성 검사. 
				int index = pCombo->GetSelectedIndex();

				if( index < 0 ) break;

				TCHAR *szSelText = pCombo->GetText( index );

				SI32 kind = pclClient->pclKindInfoSet->FindKindFromName( szSelText );

				cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[ kind ];

				if( pclKindInfo == NULL ) break;

				TCHAR* pText = GetTxtFromMgr(1523);
				StringCchPrintf( buffer, 256, pText, 
					pclKindInfo->GetMoneyForVillageGod(),
					pclKindInfo->GetResidentNumForVillageGod() );

				SetEditText( EDIT_GOD_KINDEXP, buffer );

			}
			break;
		}

	}

	else CASE_CONTROL( BUTTON_GOD_KIND )
	{

		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		//TCHAR buf[ 256 ];
		SI32 index = pCombo->GetSelectedIndex();

		if( index < 0 ) return;

		pitemdata = pCombo->GetItemData( index );

		SI32 kind = pclClient->pclKindInfoSet->FindKindFromName( pitemdata->strText );
		
		if( kind == pCityhallManager->clClientStrInfo.clCityhallStrInfo.siSymbolKind ) {

			TCHAR* pText = GetTxtFromMgr(1515);
			SetMainExplain( pText );
			pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );
			return;
		}

		cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[ kind ];

		if( pclKindInfo == NULL ) return;

		TCHAR* pText = GetTxtFromMgr(1516);
		TCHAR money[128]= TEXT(" ") ;
		StringCchPrintf(money, 128, TEXT("%d"),pclKindInfo->GetMoneyForVillageGod() );

		Nsprintf(buffer,pText,TEXT("god"),pclKindInfo->GetName()    ,TEXT("money"),  money  ,NULL);
		//sprintf( buffer, pText,
		//	pclKindInfo->GetName(), pclKindInfo->GetMoneyForVillageGod() );

		pText = GetTxtFromMgr(1517);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 21 );
		BYTE TempBuffer[128];
		memset( TempBuffer, 0, sizeof(TempBuffer));
		memcpy( TempBuffer, &kind, sizeof(kind));
		SI16 Size = sizeof(kind);
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size);

		//if( MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) return;
		
		//cltGameMsgRequest_SetSymbolStatus sendMsg( SYMBOLSTAT_KIND, kind );

		//cltMsg clMsg( GAMEMSG_REQUEST_SETSYMBOLSTATUS, sizeof( cltGameMsgRequest_SetSymbolStatus ), (BYTE*)&clpacket );

		//pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

	}

	else CASE_CONTROL( BUTTON_GOD_LIFEUP )
	{

		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		SI32 siMaxLife = pCityhallManager->clClientStrInfo.clCityhallStrInfo.siSymbolMaxLife;
		if( siMaxLife == 0 ) return;

		SI32 siNowLife = pCityhallManager->clClientStrInfo.clCityhallStrInfo.siSymbolLife;
		

		SI32 siIncLife = SI32(siMaxLife * 0.2);
		if( siNowLife + siIncLife > siMaxLife ) siIncLife = siMaxLife - siNowLife;

		if( siIncLife <= 0 ) {
			TCHAR* pText = GetTxtFromMgr(1518);
			SetMainExplain( pText );
			pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );
			return;
		}

		SI32 value = 0 ;
		cltKindInfo * pclkind = pclClient->pclKindInfoSet->pclKI[ pCityhallManager->clClientStrInfo.clCityhallStrInfo.siSymbolKind] ;
		if ( pclkind )
		{
			if ( pclkind->IsAtb(ATB_GOD) )
				value = 300 ;
			else
				value = 600 ;
		}
		else return;

		TCHAR* pText = GetTxtFromMgr(1519);

		StringCchPrintf( buffer, 256, pText, siIncLife, siIncLife * value );

			pText = GetTxtFromMgr(1520);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 22 );
		pclclient->SetMsgBox( &MsgBox, NULL, 0);

		//if( MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) return;
		
		//cltGameMsgRequest_SetSymbolStatus sendMsg( SYMBOLSTAT_LIFE, 0 );

		//cltMsg clMsg( GAMEMSG_REQUEST_SETSYMBOLSTATUS, sizeof( cltGameMsgRequest_SetSymbolStatus ), (BYTE*)&clpacket );

		//pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

	}

	else CASE_CONTROL( BUTTON_GOD_LEVELUP )
	{

		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		GMONEY siCost = 100000;

		cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[ pCityhallManager->clClientStrInfo.clCityhallStrInfo.siSymbolKind ];

		if( pclKindInfo != NULL ) {

			siCost = pclKindInfo->GetUpMoneyForVillageGod();
		}
		
		TCHAR* pText = GetTxtFromMgr(1521);
		StringCchPrintf( buffer, 256, pText, siCost );

		pText = GetTxtFromMgr(1522);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 23 );
		pclclient->SetMsgBox( &MsgBox, NULL, 0);

		//if( MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) return;

		//cltGameMsgRequest_SetSymbolStatus sendMsg( SYMBOLSTAT_CAPA, 0 );

		//cltMsg clMsg( GAMEMSG_REQUEST_SETSYMBOLSTATUS, sizeof( cltGameMsgRequest_SetSymbolStatus ), (BYTE*)&clpacket );

		//pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);


	}

	else CASE_CONTROL( BUTTON_CASTLE_LIFEUP )
	{


		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}

		SI32 siMaxLife = pCityhallManager->clClientStrInfo.clCityhallStrInfo.siCastleMaxLife;
		if( siMaxLife == 0 ) return;

		SI32 siNowLife = pCityhallManager->clClientStrInfo.clCityhallStrInfo.siCastleLife;
		
		// 수리내구도를 조정함. (pck - 07.12.07)
		SI32 siIncLife = SI32(siMaxLife * 0.2);
		if( siNowLife + siIncLife > siMaxLife ) siIncLife = siMaxLife - siNowLife;

		if( siIncLife <= 0 ) {

			TCHAR* pText = GetTxtFromMgr(1524);
			SetMainExplain( pText );
			pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );
			return;
		}

		TCHAR* pText = GetTxtFromMgr(1525);
		StringCchPrintf( buffer, 256, pText, siIncLife, siIncLife * 200 );

		pText = GetTxtFromMgr(1526);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 24 );
		pclclient->SetMsgBox( &MsgBox, NULL, 0);

		//if( MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) return;
		
		//// 효과음 연주. 
		//pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIRWALL")), 0, 0);

		//cltGameMsgRequest_SetCastleStatus sendMsg( SYMBOLSTAT_LIFE, 0 );

		//cltMsg clMsg( GAMEMSG_REQUEST_SETCASTLESTATUS, sizeof( cltGameMsgRequest_SetCastleStatus ), (BYTE*)&clpacket );

		//pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

	}

	else CASE_CONTROL( BUTTON_CASTLE_LEVELUP )
	{

		// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
		{
			cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
			if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
			{
				TCHAR* pText;
				if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
				else pText = GetTxtFromMgr(1440);
				SetMainExplain(pText);
				return;
			}
		}


		GMONEY siCost = 100000;

		/*
		cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[ clClientStrInfo.clCityhallStrInfo.siSymbolKind ];

		if( pclKindInfo != NULL ) {

			siCost = pclKindInfo->GetUpMoneyForVillageGod();
		}
		*/

		TCHAR* pText = GetTxtFromMgr(1527);
		StringCchPrintf( buffer, 256, pText ,siCost );

		pText = GetTxtFromMgr(1528);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 25 );
		pclclient->SetMsgBox( &MsgBox, NULL, 0);

		//if( MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) return;

		//// 효과음 연주. 
		//pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIRWALL")), 0, 0);

		//cltGameMsgRequest_SetCastleStatus sendMsg( SYMBOLSTAT_CAPA, 0 );

		//cltMsg clMsg( GAMEMSG_REQUEST_SETCASTLESTATUS, sizeof( cltGameMsgRequest_SetCastleStatus ), (BYTE*)&clpacket );

		//pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void NCityhallGodDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	switch( MsgIndex )
	{
	case 21:	//
		{
			if( pData == NULL || DataLen < 1 ) return;

			if ( RetValue )
			{
				SI32 kind =-1;
				kind = *((SI32*)pData);

				cltGameMsgRequest_SetSymbolStatus sendMsg( SYMBOLSTAT_KIND, kind );

				cltMsg clMsg( GAMEMSG_REQUEST_SETSYMBOLSTATUS, sizeof( cltGameMsgRequest_SetSymbolStatus ), (BYTE *)&sendMsg );

				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 22:
		{
			if( RetValue )
			{
				cltGameMsgRequest_SetSymbolStatus sendMsg( SYMBOLSTAT_LIFE, 0 );

				cltMsg clMsg( GAMEMSG_REQUEST_SETSYMBOLSTATUS, sizeof( cltGameMsgRequest_SetSymbolStatus ), (BYTE*)&sendMsg );

				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 23:
		{
			if( RetValue )
			{
				cltGameMsgRequest_SetSymbolStatus sendMsg( SYMBOLSTAT_CAPA, 0 );

				cltMsg clMsg( GAMEMSG_REQUEST_SETSYMBOLSTATUS, sizeof( cltGameMsgRequest_SetSymbolStatus ), (BYTE*)&sendMsg );

				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 24:
		{
			if( RetValue )
			{
				// 효과음 연주. 
				pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIRWALL")), 0, 0);

				cltGameMsgRequest_SetCastleStatus sendMsg( SYMBOLSTAT_LIFE, 0 );

				cltMsg clMsg( GAMEMSG_REQUEST_SETCASTLESTATUS, sizeof( cltGameMsgRequest_SetCastleStatus ), (BYTE*)&sendMsg );

				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 25:
		{
			if( RetValue )
			{
				// 효과음 연주. 
				pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_REPAIRWALL")), 0, 0);

				cltGameMsgRequest_SetCastleStatus sendMsg( SYMBOLSTAT_CAPA, 0 );

				cltMsg clMsg( GAMEMSG_REQUEST_SETCASTLESTATUS, sizeof( cltGameMsgRequest_SetCastleStatus ), (BYTE*)&sendMsg );

				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}