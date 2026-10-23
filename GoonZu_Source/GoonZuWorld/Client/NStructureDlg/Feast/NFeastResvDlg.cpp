#include "NFeastDlg.h"

#include "../../InterfaceMgr/Interface/ComboBox.h"
//#include "../StrBaseDlg/StrInventory.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Feast.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NFeastResvDlg::NFeastResvDlg()
{
}

NFeastResvDlg::~NFeastResvDlg()
{
	
}

void NFeastResvDlg::InitDlg()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	SetControlMap( BUTTON_RESV, TEXT("button_resv") );

	SetControlMap( EDIT_COST, TEXT("editbox_cost") );
	SetControlMap( EDIT_REASON, TEXT("editbox_reason") );
	SetControlMap( EDIT_EXPLAIN, TEXT("editbox_explain") );

	SetControlMap( COMBO_YEAR, TEXT("combobox_year") );
	SetControlMap( COMBO_MONTH, TEXT("combobox_month") );
	SetControlMap( COMBO_DAY, TEXT("combobox_day") );
	SetControlMap( COMBO_HOUR, TEXT("combobox_hour") );
	SetControlMap( COMBO_MINUTE, TEXT("combobox_minute") );

	TCHAR buffer[1024];

	TCHAR* pText = GetTxtFromMgr(1837);
	SetEditText( EDIT_EXPLAIN, pText );

	cltFeastStrInfo *pclFeastStrInfo = &pFeastManager->clClientStrInfo.clFeastStrInfo;
	
	g_JYLibFunction.SetNumUnit( pclFeastStrInfo->clFeastInfo.siCost, buffer, 1024, TEXT("") );
	SetEditText( EDIT_COST, buffer );

	SetDateCombo();
}

void NFeastResvDlg::Action()
{

}


void NFeastResvDlg::SetDateCombo()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	CComboBox *pCombo;
	stComboBoxItemData itemdata;

	TCHAR buffer[256];
	SYSTEMTIME systime;
	GetLocalTime( &systime );

	int i;

	pCombo = (CComboBox*)m_pControlMap[ COMBO_YEAR ];
	for( i = 0; i < 2; ++i ) {

		_itot( systime.wYear + i, buffer, 10 );
		itemdata.Init();
		itemdata.Set( buffer );
		pCombo->AddItem( &itemdata );
	}
	pCombo->SetCursel( 0 );

	pCombo = (CComboBox*)m_pControlMap[ COMBO_MONTH ];
	for( i = 1; i <= 12; ++i ) {
		_itot( i, buffer, 10 );
		itemdata.Init();
		itemdata.Set( buffer );
		pCombo->AddItem( &itemdata );
	}
	pCombo->SetCursel( systime.wMonth - 1 );

	pCombo = (CComboBox*)m_pControlMap[ COMBO_DAY ];
	for( i = 1; i <= 31; ++i ) {
		_itot( i, buffer, 10 );
		itemdata.Init();
		itemdata.Set( buffer );
		pCombo->AddItem( &itemdata );
	}
	pCombo->SetCursel( systime.wDay - 1 );

	pCombo = (CComboBox*)m_pControlMap[ COMBO_HOUR ];
	for( i = 0; i <= 23; ++i ) {
		_itot( i, buffer, 10 );
		itemdata.Init();
		itemdata.Set( buffer );
		pCombo->AddItem( &itemdata );
	}
	pCombo->SetCursel( 0 );

	pCombo = (CComboBox*)m_pControlMap[ COMBO_MINUTE ];
	for( i = 0; i < 60; i+=10 ) {
		if( i == 0 ) StringCchCopy( buffer, 256, TEXT("00") );
		else _itot( i, buffer, 10 );
		itemdata.Init();
		itemdata.Set( buffer );
		pCombo->AddItem( &itemdata );
	}
	pCombo->SetCursel( 0 );

}


void NFeastResvDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	CComboBox *pCombo;
	stComboBoxItemData *pItemData;

	TCHAR buffer[ 256 ];

	CASE_CONTROL( BUTTON_RESV )
	{

		int index;
		SI32 year, month, day, hour, minute;

		// year
		pCombo = (CComboBox*)m_pControlMap[ COMBO_YEAR ];
		index = pCombo->GetSelectedIndex();
		if( index < 0 ) return;
		pItemData = pCombo->GetItemData( index );
		
		year = _tstoi( pItemData->strText );
	
		// month
		pCombo = (CComboBox*)m_pControlMap[ COMBO_MONTH ];
		index = pCombo->GetSelectedIndex();
		if( index < 0 ) return;
		pItemData = pCombo->GetItemData( index );

		month = _tstoi( pItemData->strText );

		// day
		pCombo = (CComboBox*)m_pControlMap[ COMBO_DAY ];
		index = pCombo->GetSelectedIndex();
		if( index < 0 ) return;
		pItemData = pCombo->GetItemData( index );

		day = _tstoi( pItemData->strText );

		// hour
		pCombo = (CComboBox*)m_pControlMap[ COMBO_HOUR ];
		index = pCombo->GetSelectedIndex();
		if( index < 0 ) return;
		pItemData = pCombo->GetItemData( index );
	
		hour = _tstoi( pItemData->strText );
		
		// minute
		pCombo = (CComboBox*)m_pControlMap[ COMBO_MINUTE ];
		index = pCombo->GetSelectedIndex();
		if( index < 0 ) return;
		pItemData = pCombo->GetItemData( index );

		minute = _tstoi( pItemData->strText );

		MStrCpy( buffer, GetEditText( EDIT_REASON ), 128 );

		cltGameMsgRequest_FeastReserve sendmsg( year, month, day, hour, minute, buffer );
		cltMsg clMsg(GAMEMSG_REQUEST_FEAST_RESERVE, sizeof(sendmsg), (BYTE*)&sendmsg);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

	}
}