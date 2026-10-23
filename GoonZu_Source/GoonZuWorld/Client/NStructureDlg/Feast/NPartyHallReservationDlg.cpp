#include "NFeastDlg.h"

#include "../../InterfaceMgr/Interface/ComboBox.h"
//#include "../StrBaseDlg/StrInventory.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Feast.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NPartyHallReservationDlg::NPartyHallReservationDlg()
{
	m_siMapIndex	 = 0;
	m_siEntranceType = TYPE_ENTRANCE_PUBLIC;

	// 아이템의 슬롯의 상태를 표시하기 위한 이미지.
	if(false == m_Spr.LoadTSprFromFile("GImg/partyhall_mini_map.SPR"))
	{
		MsgBox("파일열기 실패", "partyhall_mini_map.SPR");
	}

}

NPartyHallReservationDlg::~NPartyHallReservationDlg()
{
	
}

void NPartyHallReservationDlg::InitDlg()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	SetControlMap( BUTTON_RESV,				TEXT("button_resv") );

	SetControlMap( EDIT_COST,				TEXT("editbox_cost") );
	SetControlMap( EDIT_REASON,				TEXT("editbox_reason") );
	SetControlMap( EDIT_DECORATION_COST,	TEXT("editbox_decoration_cost") );
	

	SetControlMap( COMBO_YEAR,				TEXT("combobox_year") );
	SetControlMap( COMBO_MONTH,				TEXT("combobox_month") );
	SetControlMap( COMBO_DAY,				TEXT("combobox_day") );
	SetControlMap( COMBO_HOUR,				TEXT("combobox_hour") );
	SetControlMap( COMBO_MINUTE,			TEXT("combobox_minute") );
	SetControlMap( COMBO_DECORATION_LIST,	TEXT("combobox_decoration_list") );
	
	SetControlMap( BUTTON_LEFT,				TEXT("button_left") );
	SetControlMap( BUTTON_RIGHT,			TEXT("button_right") );
	SetControlMap( LIST_RESERVATION,		TEXT("listbox_reservation") );
	SetControlMap( RADIOBUTTON_PUBLIC,		TEXT("radiobutton_public") );
	SetControlMap( RADIOBUTTON_PRIVATE,		TEXT("radiobutton_private") );
	SetControlMap( RADIOBUTTON_VILLAGE,		TEXT("radiobutton_village") );
	SetControlMap( RADIOBUTTON_GUILD,		TEXT("radiobutton_guild") );

	SetControlMap( EDIT_PUBLIC,				TEXT("editbox_public") ); 
	SetControlMap( EDIT_PRIVATE,			TEXT("editbox_private") );
	SetControlMap( EDIT_VILLAGE,			TEXT("editbox_village") );
	SetControlMap( EDIT_GUILD,				TEXT("editbox_guild") );

		

	SetControlMap( COMMBO_PARTYHALL,		TEXT("combobox_partyhall_list") );


	((CButton*)m_pControlMap[BUTTON_LEFT])->Show(false);
	((CButton*)m_pControlMap[BUTTON_RIGHT])->Show(false);
	
	((CEdit*)m_pControlMap[EDIT_PUBLIC ])->SetText(GetTxtFromMgr(6290));
	((CEdit*)m_pControlMap[EDIT_PUBLIC ])->SetUseToolTip(GetTxtFromMgr(8288));	
	((CEdit*)m_pControlMap[EDIT_PRIVATE])->SetText(GetTxtFromMgr(6291));
	((CEdit*)m_pControlMap[EDIT_PRIVATE])->SetUseToolTip(GetTxtFromMgr(8291)); 
    ((CEdit*)m_pControlMap[EDIT_VILLAGE])->SetText(GetTxtFromMgr(6086));
	((CEdit*)m_pControlMap[EDIT_VILLAGE])->SetUseToolTip(GetTxtFromMgr(8289));
	((CEdit*)m_pControlMap[EDIT_GUILD  ])->SetText(GetTxtFromMgr(5560));
	((CEdit*)m_pControlMap[EDIT_GUILD  ])->SetUseToolTip(GetTxtFromMgr(8290));
	 
	// 파티장 리스트 세팅.
	CComboBox* pCombo = (CComboBox*)m_pControlMap[COMMBO_PARTYHALL];
	if( NULL == pCombo )
		return;

	pCombo->Clear();
	stComboBoxItemData itemdata;

	TCHAR* pMapName   = NULL;
	TCHAR buffer[256] = "";

	// 콤보 박스에 아이템의 종류를 설정한다. 
	for( SI32 i = 0; i < MAX_PARTYHALL_MAP; i++ )
	{	
		pMapName =  pclClient->pclMapManager->GetMapName( PARTYHALL_START_MAP_INDEX + i );	  
		if(0 == strcmp(pMapName, "empty_partyhall"))
			continue;

		itemdata.Init();
		itemdata.Set( pMapName );
		pCombo->AddItem( &itemdata );
	}
	pCombo->SetCursel(0);   
	
	m_siMapIndex = 0;
	
	CRadioButton* pRadioButton = (CRadioButton*)m_pControlMap[ RADIOBUTTON_PUBLIC ];
	pRadioButton->SetNonEventOneButtonCheck(true);


	// List 
	TCHAR* pText = NULL;	
	CList* pList = (CList*)m_pControlMap[ LIST_RESERVATION ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	pText = GetTxtFromMgr(4088);
	 
	pList->SetColumn( 0,  169,  pText ); 

	pList->Refresh();


	
	cltFeastStrInfo *pclFeastStrInfo = &pFeastManager->clClientStrInfo.clFeastStrInfo;
	
	SI32 FeastCost = 0;
	if(500000 > pclFeastStrInfo->clFeastInfo.siCost)
	{
		FeastCost = 500000;
	}
	else
	{
		FeastCost = pclFeastStrInfo->clFeastInfo.siCost;
	}

	g_JYLibFunction.SetNumUnit( FeastCost, buffer, 1024, TEXT("") );
	SetEditText( EDIT_COST, buffer );

	SetDateCombo();
	SetDecorationList();
	SetReservationList();


	SetTimeData(0); 

	SetActionTimer( 2000 );    
	SetTimerOnOff( true );
}

void NPartyHallReservationDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{ 
	SI32 X_Add = ScreenRX + 39;
	SI32 Y_Add = ScreenRY + 30;  

	GP.PutSprAlpha(&m_Spr, GetX() +  X_Add, GetY() + Y_Add, 12, m_siMapIndex, 0);  
} 

// 현재부터 예약 가능한 시간을 메모리에 담는다.
void NPartyHallReservationDlg::SetTimeData( SI32 _DayIndex )
{
	if(0 > _DayIndex || 1 < _DayIndex)
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	SYSTEMTIME LastTime;
	ZeroMemory( &LastTime, sizeof( LastTime ) );

	CComboBox* pComboDay  = (CComboBox*)m_pControlMap[ COMBO_DAY ];
	pComboDay->Clear();
	CComboBox* pComboHour = (CComboBox*)m_pControlMap[ COMBO_HOUR ];
	pComboHour->Clear();

	stComboBoxItemData	itemdata;
	bool				DayChange	= false;
	TCHAR				buffer[256] = "";

	SI32			m_Hour[2]		= { -1, };
	SI32			m_Minute[2][24] = { -1, };
	memset(m_Minute, -1, sizeof(m_Hour));
	memset(m_Minute, -1, sizeof(m_Minute));

	m_Hour[0] = pclclient->sTime.wDay;

	_itot( pclclient->sTime.wDay, buffer, 10 );
	itemdata.Init();
	itemdata.Set( buffer );
	pComboDay->AddItem( &itemdata );   

	SI32 HourArray = 0;
	for(SI32 Count = 0; Count < 24; ++Count)
	{
		TimeAddHour(&pclclient->sTime, Count+1, &LastTime);
		if( pclclient->sTime.wDay != LastTime.wDay && false == DayChange )
		{
			m_Hour[1] = LastTime.wDay;
			HourArray = 1;

			DayChange = true;

			_itot( LastTime.wDay, buffer, 10 );
			itemdata.Init();
			itemdata.Set( buffer );
			pComboDay->AddItem( &itemdata );
		}
		
		m_Minute[HourArray][Count] = LastTime.wHour;
	}
	

	
	for(SI32 Count = 0; Count < 24; ++Count)
	{
		if( -1 != m_Minute[_DayIndex][Count] )
		{
			_itot( m_Minute[_DayIndex][Count], buffer, 10 );
			itemdata.Init();
			itemdata.Set( buffer );
			pComboHour->AddItem( &itemdata );
		}
	}


	pComboDay->SetCursel(_DayIndex);
	pComboHour->SetCursel(0);

}

void NPartyHallReservationDlg::SetDateCombo()
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;

	CComboBox *pCombo;
	stComboBoxItemData itemdata;

	TCHAR buffer[256];
	SYSTEMTIME systime;
	//GetLocalTime( &systime );
	cltClient* pclclient = (cltClient*)pclClient;
	systime = pclclient->sTime;
	int i;

	pCombo = (CComboBox*)m_pControlMap[ COMBO_YEAR ];
	for( i = 0; i < 2; ++i ) {

		_itot( systime.wYear + i, buffer, 10 );
		itemdata.Init();
		itemdata.Set( buffer );
		pCombo->AddItem( &itemdata );
	}
	pCombo->SetCursel( 0 );
	pCombo->Enable(false);

	pCombo = (CComboBox*)m_pControlMap[ COMBO_MONTH ];
	for( i = 1; i <= 12; ++i ) {
		_itot( i, buffer, 10 );
		itemdata.Init();
		itemdata.Set( buffer );
		pCombo->AddItem( &itemdata );
	}
	pCombo->SetCursel( systime.wMonth - 1 );
	pCombo->Enable(false);

	
	//SetTimeData( 0 );

	pCombo = (CComboBox*)m_pControlMap[ COMBO_MINUTE ];
	for( i = 0; i < 60; i+=10 ) {
		if( i == 0 ) StringCchCopy( buffer, 256, TEXT("00") );
		else _itot( i, buffer, 10 );
		itemdata.Init();
		itemdata.Set( buffer );
		pCombo->AddItem( &itemdata );
	}
	pCombo->SetCursel( 0 );
	pCombo->Enable(false);

}


void NPartyHallReservationDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;
		
	CASE_CONTROL( BUTTON_RESV )
	{
		if(false == IsReservation())
			return;

		cltClient* pclclient = (cltClient*)pclClient;

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) 
			return;

		cltFeastManager* pFeastManager   = (cltFeastManager *)m_pStrManager;
		cltFeastStrInfo *pclFeastStrInfo = &pFeastManager->clClientStrInfo.clFeastStrInfo;
			
		CComboBox* pCombo = (CComboBox*)m_pControlMap[ COMBO_DECORATION_LIST ];
		// 1보다 작은수가 나오면 선택한 장식물이 없다.
		int index = pCombo->GetSelectedIndex();
		
		// 장식물 이용 금액.
		SI32   siDecorationMoney = 0;
		if(0 <= index)
		{
			CEdit* pEdit = pEdit =	(CEdit*)m_pControlMap[ EDIT_DECORATION_COST ];
			siDecorationMoney  = atoi( pEdit->GetText() );
		}
	
		SI32 FeastCost = 0;
		if(500000 > pclFeastStrInfo->clFeastInfo.siCost)
		{
			FeastCost = 500000;
		}
		else
		{
			FeastCost = pclFeastStrInfo->clFeastInfo.siCost;
		}

		SI32 siCostMoney = FeastCost + siDecorationMoney;
		
		NTCHARString128 kbuf(GetTxtFromMgr(8340));
		kbuf.Replace("#money#",  SI32ToString(siCostMoney));
		
		MsgBox.Set( pParent, GetTxtFromMgr(5614), kbuf, MSGBOX_TYPE_YESNO, 1 );
		pclclient->SetMsgBox( &MsgBox, NULL, 0 );
		
	}
	else CASE_CONTROL( BUTTON_LEFT )
	{
		SetMapNum( -1 );
		
		SetReservationList();
		
	}
	else CASE_CONTROL( BUTTON_RIGHT )
	{
		SetMapNum( 1 );
		
		SetReservationList();
	}
	else CASE_CONTROL( RADIOBUTTON_PUBLIC )
	{
		SetRadioButton();
	}
	else CASE_CONTROL( RADIOBUTTON_PRIVATE )
	{
		SetRadioButton();
	}
	else CASE_CONTROL( RADIOBUTTON_VILLAGE )
	{
		SetRadioButton();
	}
	else CASE_CONTROL( RADIOBUTTON_GUILD )
	{
		SetRadioButton();
	}
	else CASE_CONTROL( COMBO_DAY )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{
				CComboBox* pComboBox = (CComboBox*)m_pControlMap[ COMBO_DAY ];

				SI16 Index = pComboBox->GetSelectedIndex();
				static SI16 PrevIndex = 0;

				if( 0 > Index )
					return;
				if( PrevIndex == Index )
					return;
				
				PrevIndex = Index; 

				SetTimeData( Index );

				pComboBox->SetCursel(Index); 
			}
			break;    
		}
	}
	else CASE_CONTROL( COMMBO_PARTYHALL )
	{
		CComboBox* pComboBox = (CComboBox*)m_pControlMap[ COMMBO_PARTYHALL ];

		m_siMapIndex = pComboBox->GetSelectedIndex();
		if(0 > m_siMapIndex || MAX_PARTYHALL_MAP < m_siMapIndex)
			return;
        
		SetReservationList();
	}

}

bool NPartyHallReservationDlg::SetReservationTime(SI32& year, SI32& month, SI32& day, SI32& hour, SI32& minute)
{
	int					 index		= 0;
	CComboBox			*pCombo		= NULL;
	stComboBoxItemData	*pItemData	= NULL;

	// year
	pCombo = (CComboBox*)m_pControlMap[ COMBO_YEAR ];
	index = pCombo->GetSelectedIndex();
	if( index < 0 ) 
		return false;
	pItemData = pCombo->GetItemData( index );
	year = _tstoi( pItemData->strText );

	// month
	pCombo = (CComboBox*)m_pControlMap[ COMBO_MONTH ];
	index = pCombo->GetSelectedIndex();
	if( index < 0 ) 
		return false;
	pItemData = pCombo->GetItemData( index );
	month = _tstoi( pItemData->strText );

	// day
	pCombo = (CComboBox*)m_pControlMap[ COMBO_DAY ];
	index = pCombo->GetSelectedIndex();
	if( index < 0 ) 
		return false;
	pItemData = pCombo->GetItemData( index );
	day = _tstoi( pItemData->strText );

	// hour
	pCombo = (CComboBox*)m_pControlMap[ COMBO_HOUR ];
	index = pCombo->GetSelectedIndex();
	if( index < 0 ) 
		return false;
	pItemData = pCombo->GetItemData( index );
	hour = _tstoi( pItemData->strText );

	// minute
	pCombo = (CComboBox*)m_pControlMap[ COMBO_MINUTE ];
	index = pCombo->GetSelectedIndex();
	if( index < 0 ) 
		return false;
	pItemData = pCombo->GetItemData( index );
	minute = _tstoi( pItemData->strText );

	return true;
}

void NPartyHallReservationDlg::SetDecorationList( void )
{
	cltFeastManager *pFeastManager              = (cltFeastManager *)m_pStrManager;
	PartyHall_Decoration *pclDecorationInfoList = &pFeastManager->clDecoration[0];

	CComboBox* pCombo = (CComboBox*)m_pControlMap[ COMBO_DECORATION_LIST ];
	if( NULL == pCombo )
		return;

	pCombo->Clear();

	stComboBoxItemData itemdata;
	
	TCHAR  buffer[64] = "";
	TCHAR* npcName	  = NULL;
	// 콤보 박스에 아이템의 종류를 설정한다. 
	for( SI32 i = 0; i < MAX_PARTYHALL_DECORATION_ITEM; i++, pclDecorationInfoList++ )
	{	
		// 장식물이 있으면.
		if( 0 >= pclDecorationInfoList->m_DacorationKind ||
			0 >= pclDecorationInfoList->m_DacorationNum		)
			continue;

		npcName = (TCHAR*)pclClient->pclKindInfoSet->GetName(pclDecorationInfoList->m_DacorationKind);
		if( NULL == npcName )
			break;
		
		StringCchPrintf( buffer, 256, TEXT("%s"),   npcName	);
		
		itemdata.Init();
		itemdata.Set( buffer );
		pCombo->AddItem( &itemdata );
	}
}

void NPartyHallReservationDlg::SetReservationList( void )
{
	cltFeastManager*	 pFeastManager			 = (cltFeastManager *)m_pStrManager;
	cltPatyHallResvInfo* pclPatyHallResvInfoList = &pFeastManager->clPartyHallResvInfo[ m_siMapIndex ][ 0 ];
	
	CList* pList = (CList*)m_pControlMap[ LIST_RESERVATION ];
	if( NULL == pList )
		return;

	pList->Clear();

	stListBoxItemData itemdata;

	TCHAR buffer[64] = "";
	
	for( SI32 ReservationCount = 0; ReservationCount < MAX_PARTYHALL_RESERVATION_INFO_LIST; ReservationCount++, pclPatyHallResvInfoList++ )
	{	
		// 12개월 안의 값만 표기.
		if( 1  > pclPatyHallResvInfoList->m_SystemTime.wMonth ||
			12 < pclPatyHallResvInfoList->m_SystemTime.wMonth		)
			continue;
		
		StringCchPrintf( buffer, 256, TEXT("%d / %d / %d    %d : %d%d"),    pclPatyHallResvInfoList->m_SystemTime.wYear,  
																			pclPatyHallResvInfoList->m_SystemTime.wMonth,	
																			pclPatyHallResvInfoList->m_SystemTime.wDay,	
																			pclPatyHallResvInfoList->m_SystemTime.wHour,	
																			pclPatyHallResvInfoList->m_SystemTime.wMinute/10,
																			pclPatyHallResvInfoList->m_SystemTime.wMinute%10  );		
			
		itemdata.Init();
		itemdata.Set( 0, buffer );
		pList->AddItem( &itemdata );
	}

	pList->Refresh();
}


void NPartyHallReservationDlg::SetMapNum( SI32 _IndexChange )
{
	m_siMapIndex += _IndexChange;
	
	if( MAX_PARTYHALL_MAP <= m_siMapIndex || 0 > m_siMapIndex )
	{
		m_siMapIndex -= _IndexChange;
	}
}

void NPartyHallReservationDlg::SetRadioButton( void )
{
	CRadioButton* pRadioButton = NULL;

	pRadioButton = (CRadioButton*)m_pControlMap[ RADIOBUTTON_PUBLIC ];
	if( true == pRadioButton->IsCheck() ) 
	{
		m_siEntranceType = TYPE_ENTRANCE_PUBLIC;
	}
	
	pRadioButton = (CRadioButton*)m_pControlMap[ RADIOBUTTON_PRIVATE ];
	if( true == pRadioButton->IsCheck() ) 
	{
		m_siEntranceType = TYPE_ENTRANCE_PRIVATE;
	}
	
	pRadioButton = (CRadioButton*)m_pControlMap[ RADIOBUTTON_VILLAGE ];
	if( true == pRadioButton->IsCheck() ) 
	{
		m_siEntranceType = TYPE_ENTRANCE_VILLAGE;
	}
	
	pRadioButton = (CRadioButton*)m_pControlMap[ RADIOBUTTON_GUILD ];
	if( true == pRadioButton->IsCheck() ) 
	{
		m_siEntranceType = TYPE_ENTRANCE_GUILD;
	}
}


BOOL NPartyHallReservationDlg::IsReservation(void)
{
	TCHAR buffer[ 256 ] = "";
	MStrCpy( buffer, GetEditText( EDIT_REASON ), sizeof( buffer ) );

	CComboBox* pCombo = (CComboBox*)m_pControlMap[ COMBO_DECORATION_LIST ];

	cltFeastManager* pFeastManager              = (cltFeastManager *)m_pStrManager;
	PartyHall_Decoration* pclDecorationInfoList = pFeastManager->clClientStrInfo.clFeastStrInfo.clDecoration;

	SI32 DecorationKind = -1;
	
	// 0보다 작은수가 나오면 선택한 장식물이 없다.
	int index = pCombo->GetSelectedIndex();
	if( 0 < index)
	{
		DecorationKind = pclDecorationInfoList[index].m_DacorationKind;
	}

	// 장식물 이용 금액.
	SI32   siDecorationMoney = 0;
	CEdit* pEdit			 = NULL;
	if(0 < DecorationKind)
	{
		pEdit =	(CEdit*)m_pControlMap[ EDIT_DECORATION_COST ];
		siDecorationMoney  = atoi( pEdit->GetText() );
	}

	cltFeastStrInfo *pclFeastStrInfo = &pFeastManager->clClientStrInfo.clFeastStrInfo;

	if(0 > pclFeastStrInfo->clFeastInfo.siCost)
	{
		cltClient *pclclient = (cltClient *)pclClient;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(1367));
		return false;   
	}

	// 파티비.
	SI32 siCostMoney = 0;
	pEdit =	(CEdit*)m_pControlMap[ EDIT_COST ];

	SI32 FeastCost = 0;
	if(500000 > pclFeastStrInfo->clFeastInfo.siCost)
	{
		FeastCost = 500000;
	}
	else
	{
		FeastCost = pclFeastStrInfo->clFeastInfo.siCost;
	}

	// 장식비 + 파티비.
	siCostMoney = FeastCost + siDecorationMoney;


	// 가진 돈이 지불할 돈보다 적다면 예약 불가능.
	if( siCostMoney > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
	{
		cltClient *pclclient = (cltClient *)pclClient;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(1367));
		return false;   
	}
	
	return true;
}

void NPartyHallReservationDlg::SendReservation(void)
{
	TCHAR buffer[ 128 ] = "";
	MStrCpy( buffer, GetEditText( EDIT_REASON ), sizeof( buffer ) );

	SI32 valuetype = -1;
	switch(m_siEntranceType)
	{
	case	TYPE_ENTRANCE_PUBLIC:	break;
	case	TYPE_ENTRANCE_VILLAGE:	
		{
			valuetype = pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage;
			if(0 >= valuetype)
			{
				cltClient *pclclient = (cltClient *)pclClient;
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(8292));
				return;   
			}
		}
		break;
	case	TYPE_ENTRANCE_GUILD:	
		{
			valuetype = pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siGuildUnitIndex;
			if(0 >= valuetype)
			{
				cltClient *pclclient = (cltClient *)pclClient;
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(8292));
				return;   
			}
		}
		break;		
	case	TYPE_ENTRANCE_PRIVATE:	break;
	}

	cltFeastManager* pFeastManager              = (cltFeastManager *)m_pStrManager;
	PartyHall_Decoration* pclDecorationInfoList = pFeastManager->clClientStrInfo.clFeastStrInfo.clDecoration;

	CComboBox* pCombo = (CComboBox*)m_pControlMap[ COMBO_DECORATION_LIST ];
	// 1보다 작은수가 나오면 선택한 장식물이 없다.
	int		index		   = pCombo->GetSelectedIndex();
	SI32	DecorationKind = 0;
	TCHAR*	npcName		   = NULL;
	if( 0 <= index)  
	{
		for( SI32 i = 0; i < MAX_PARTYHALL_DECORATION_ITEM; i++ )
		{	
			npcName = (TCHAR*)pclClient->pclKindInfoSet->GetName(pclDecorationInfoList[i].m_DacorationKind);
			if(NULL == npcName)
				continue;
			
			if(0 == strcmp(npcName, pCombo->GetText(index)))
			{
				DecorationKind = pclDecorationInfoList[i].m_DacorationKind;
				index = i;
				break;
			}
		
		}
	}

	// 장식물 이용 금액.
	SI32   siDecorationMoney = 0;
	CEdit* pEdit			 = NULL;
	if(0 < DecorationKind)
	{
		pEdit =	(CEdit*)m_pControlMap[ EDIT_DECORATION_COST ];
		siDecorationMoney  = atoi( pEdit->GetText() );
	}

	// 연회비.
	SI32 siCostMoney = 0;
	pEdit =	(CEdit*)m_pControlMap[ EDIT_COST ];

	// 장식비 + 연회비.
	cltFeastStrInfo *pclFeastStrInfo = &pFeastManager->clClientStrInfo.clFeastStrInfo;

	SI32 FeastCost = 0;
	if(500000 > pclFeastStrInfo->clFeastInfo.siCost)
	{
		FeastCost = 500000;
	}
	else
	{
		FeastCost = pclFeastStrInfo->clFeastInfo.siCost;
	}

	siCostMoney = FeastCost + siDecorationMoney;

	// 가진 돈이 지불할 돈보다 적다면 예약 불가능.
	if( siCostMoney > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
	{
		cltClient *pclclient = (cltClient *)pclClient;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(1367));
		return;   
	}

	SI32 year, month, day, hour, minute;

	// 예약할 시간을 얻어온다.
	if(false == SetReservationTime( year, month, day, hour, minute ))
	{
		// 시간이 잘못됐다면.
		cltClient *pclclient = (cltClient *)pclClient;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(15421));
		return;   
	}

	cltGameMsgRequest_PartyHall_Reservation sendmsg( year, month, day, hour, minute, buffer, DecorationKind, m_siEntranceType, valuetype, m_siMapIndex, siCostMoney, index );
	cltMsg clMsg(GAMEMSG_REQUEST_PARTYHALL_RESERVATION, sizeof(sendmsg), (BYTE*)&sendmsg);
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
}

void NPartyHallReservationDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
		case 1:
			{
				if ( RetValue )
				{
					if(false == IsReservation())
						return;

					SendReservation();
				}
				else
				{

				}
			}
	}
}

void NPartyHallReservationDlg::Action(void)
{
	cltFeastManager *pFeastManager = (cltFeastManager *)m_pStrManager;
	cltFeastStrInfo *pclFeastStrInfo = &pFeastManager->clClientStrInfo.clFeastStrInfo;

	TCHAR buffer[32] = "";

	SI32 FeastCost = 0;
	if(500000 > pclFeastStrInfo->clFeastInfo.siCost)
	{
		FeastCost = 500000;
	}
	else
	{
		FeastCost = pclFeastStrInfo->clFeastInfo.siCost;
	}

	g_JYLibFunction.SetNumUnit( FeastCost, buffer, sizeof(buffer), TEXT("") );
	SetEditText( EDIT_COST, buffer );

	
	CComboBox* pCombo = (CComboBox*)m_pControlMap[ COMBO_DECORATION_LIST ];
	if(0 <= pCombo->GetSelectedIndex())
	{
		SI32 DecorationCost = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate * 500;
		//g_JYLibFunction.SetNumUnit( DecorationCost, buffer, sizeof(buffer), TEXT("") );
		itoa(DecorationCost, buffer, 10);	
		CEdit* pEdit = (CEdit*)m_pControlMap[ EDIT_DECORATION_COST ];   
		pEdit->SetText(buffer);
	}

	// 시간 설정.
	CComboBox* pComboBox = (CComboBox*)m_pControlMap[ COMBO_DAY ];

	SI16 Index = pComboBox->GetSelectedIndex();
	static SI16 PrevIndex = 0;

	if( 0 > Index )
		return;
	if( PrevIndex == Index )
		return;

	PrevIndex = Index; 

	SetTimeData( Index );

	pComboBox->SetCursel(Index); 
}
	
void NPartyHallReservationDlg::SetReservationSuccess(void)
{
	TCHAR* pText = GetTxtFromMgr(15417);
	SetMainExplain(pText);
	
	((CEdit*)m_pControlMap[EDIT_REASON])->SetText("");
}