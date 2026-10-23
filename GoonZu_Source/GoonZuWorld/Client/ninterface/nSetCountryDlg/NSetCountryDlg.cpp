#include "NSetCountryDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client/Client.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/CountryMgr/CountryManager.h"
#include "../../CommonLogic/MsgType-Person.h"

#include "Char/CharManager/CharManager.h"

#define		APRILFOOL_UNIQUE	76
#define		APRILFOOL_UNIQUE_J	47

extern cltCommonLogic* pclClient;

CNSetCountryDlg::CNSetCountryDlg()	
{
	m_pStaticSubject		= NULL;
	m_pStaticContinent		= NULL;
	m_pComboBoxContinent	= NULL;
	m_pStaticCountry		= NULL;
	m_pComboBoxCountry		= NULL;
	m_pImageStaticFlag		= NULL;
	m_pStaticSetting		= NULL;
	m_pOutLineSetting		= NULL;
	m_pStaticShowCountry	= NULL;
	m_pRadioButtonShowOn	= NULL;
	m_pRadioButtonShowOff	= NULL;
	m_pStaticExplain1		= NULL;
	m_pStaticExplain2		= NULL;
	m_pButtonSubmit			= NULL;
}
CNSetCountryDlg::~CNSetCountryDlg()
{
	SAFE_DELETE( m_pStaticSubject );
	SAFE_DELETE( m_pStaticContinent );
	SAFE_DELETE( m_pComboBoxContinent );
	SAFE_DELETE( m_pStaticCountry );
	SAFE_DELETE( m_pComboBoxCountry );
	SAFE_DELETE( m_pImageStaticFlag );
	SAFE_DELETE( m_pStaticSetting );
	SAFE_DELETE( m_pOutLineSetting );
	SAFE_DELETE( m_pStaticShowCountry );
	SAFE_DELETE( m_pRadioButtonShowOn );
	SAFE_DELETE( m_pRadioButtonShowOff );
	SAFE_DELETE( m_pStaticExplain1 );
	SAFE_DELETE( m_pStaticExplain2 );
	SAFE_DELETE( m_pButtonSubmit );
}
void CNSetCountryDlg::Create()
{
	m_pStaticSubject		= new CStatic( this );
	m_pStaticContinent		= new CStatic( this );
	m_pComboBoxContinent	= new CComboBox( this );
	m_pStaticCountry		= new CStatic( this );
	m_pComboBoxCountry		= new CComboBox( this );
	m_pImageStaticFlag		= new CImageStatic( this );
	m_pStaticSetting		= new CStatic( this );
	m_pOutLineSetting		= new COutLine( this );
	m_pStaticShowCountry	= new CStatic( this );
	m_pRadioButtonShowOn	= new CRadioButton( this );
	m_pRadioButtonShowOff	= new CRadioButton( this );
	m_pStaticExplain1		= new CStatic( this );
	m_pStaticExplain2		= new CStatic( this );
	m_pButtonSubmit			= new CButton( this );

	CInterfaceFile file;

	file.LoadFile( TEXT( "NInterface/Data/NSetCountryDlg/DLG_NSetCountry.ddf" ) );
	file.CreatePopupDialog( this, NSETCOUNTRY_DLG, TEXT("dialog_setcountry"), StaticCallBackDialogNSetCountry,false );

	file.CreateControl( m_pStaticSubject, NSETCOUNTRY_STATIC_SUBJECT, TEXT("statictext_subject") );
	file.CreateControl( m_pStaticContinent, NSETCOUNTRY_STATIC_CONTINENT, TEXT("statictext_continent") );
	file.CreateControl( m_pComboBoxContinent, NSETCOUNTRY_COMBO_CONTINENT, TEXT("combobox_continent") );
	stComboBoxItemData	data;
	TCHAR* pText = TEXT("");
	pText = GetTxtFromMgr(6280);
	data.Set(pText);
	m_pComboBoxContinent->AddItem(&data);

	pText = GetTxtFromMgr(6281);
	data.Set(pText);
	m_pComboBoxContinent->AddItem(&data);

	pText = GetTxtFromMgr(6282);
	data.Set(pText);
	m_pComboBoxContinent->AddItem(&data);

	pText = GetTxtFromMgr(6283);
	data.Set(pText);
	m_pComboBoxContinent->AddItem(&data);

	pText = GetTxtFromMgr(6284);
	data.Set(pText);
	m_pComboBoxContinent->AddItem(&data);

	pText = GetTxtFromMgr(6285);
	data.Set(pText);
	m_pComboBoxContinent->AddItem(&data);

	file.CreateControl( m_pStaticCountry, NSETCOUNTRY_STATIC_COUNTRY, TEXT("statictext_country") );
	file.CreateControl( m_pComboBoxCountry, NSETCOUNTRY_COMBO_COUNTRY, TEXT("combobox_country") );
	
	file.CreateControl( m_pImageStaticFlag, NSETCOUNTRY_IMAGE_FLAG, TEXT("imagestatic_flag") );
	if ( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		m_pImageStaticFlag->SetFileName(TEXT("GImg/JapanCity_flag.SPR"));
		m_pImageStaticFlag->SetFontIndex( 79 );
	}
	else
	{
		m_pImageStaticFlag->SetFileName(TEXT("GImg/National_flag.SPR"));
		m_pImageStaticFlag->SetFontIndex( 79 );
	}


	file.CreateControl( m_pStaticSetting, NSETCOUNTRY_STATIC_SETTING, TEXT("statictext_setting") );
	file.CreateControl(	m_pStaticShowCountry, NSETCOUNTRY_STATIC_SHOW, TEXT("statictext_showcountry") );
	file.CreateControl(	m_pRadioButtonShowOn, NSETCOUNTRY_RADIO_SHOWON, TEXT("radiobutton_showon") );
	file.CreateControl( m_pRadioButtonShowOff, NSETCOUNTRY_RADIO_SHOWOFF, TEXT("radiobutton_showoff") );
	m_pRadioButtonShowOff->SetCheck( true , CONTROLLER_KIND_RADIOBUTTON );
	m_pRadioButtonShowOn->Enable( true );
	m_pRadioButtonShowOff->Enable( true );
	
	file.CreateControl( m_pOutLineSetting, NSETCOUNTRY_OUTLINE_SETTING, TEXT("outline_setting") );
	file.CreateControl( m_pStaticExplain1, NSETCOUNTRY_STATIC_EXPLAIN1, TEXT("statictext_explain1") );
	file.CreateControl( m_pStaticExplain2, NSETCOUNTRY_STATIC_EXPLAIN2, TEXT("statictext_explain2") );
	file.CreateControl( m_pButtonSubmit, NSETCOUNTRY_BUTTON_SUBMIT, TEXT("button_submit") );

	// 일본판인 경우 대륙 선택할수 없게
	if ( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		m_pComboBoxContinent->Enable( false );
		m_pComboBoxContinent->Show( false );
		m_pStaticContinent->Show( false );
		pCountryMgr = pclClient->pclCountryManagerJapan;
		SetComboBoxCountry();
	}
	else
	{
		pCountryMgr = pclClient->pclCountryManager;
	}

	// 국가의 정보가 있는 경우 선택되어 있는 대륙을 바꾸어준다.
	if ( pclClient->siServiceArea == ConstServiceArea_English )
	{
		if ( pclClient->pclCM->CR[1]->pclCI->clBI.siCountryID != -1 )
		{
			m_pComboBoxContinent->SetCursel(static_cast<SI16>(pCountryMgr->GetContinentFromCountry(pclClient->pclCM->CR[1]->pclCI->clBI.siCountryID)) - 1 );
			m_pComboBoxContinent->Refresh();
		}
	}
}

void CALLBACK CNSetCountryDlg::StaticCallBackDialogNSetCountry( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CNSetCountryDlg *pThis = (CNSetCountryDlg*)pControllerMgr;
	pThis->CallBackDialogNSetCountry( nEvent, nControlID, pControl );

}
void CALLBACK CNSetCountryDlg::CallBackDialogNSetCountry( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		DeleteDialog();
		break;
	case NSETCOUNTRY_COMBO_CONTINENT:
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			// 대륙을 결정하면 대륙에 속하는 나라를 콤보박스에 입력
			SetComboBoxCountry();
			break;
		}
		break;
	case NSETCOUNTRY_COMBO_COUNTRY:
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			// 나라를 결정하면 해당 나라의 국기를 보여줌
			if ( m_pComboBoxCountry->GetSelectedIndex() != -1 )
                ShowFlagImage();
			break;
		}
		break;
	case NSETCOUNTRY_BUTTON_SUBMIT:
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			if ( m_pComboBoxCountry->GetSelectedIndex() >= 0  )
                SendCountryInfo();
			break;
		}
		break;
	}
}

void CNSetCountryDlg::Action()
{
}

void CNSetCountryDlg::ShowFlagImage()
{
	SI32 selectedindex = m_pComboBoxCountry->GetSelectedIndex();
	TCHAR* country = m_pComboBoxCountry->GetText( selectedindex );
	SI32 countryunique = pCountryMgr->GetUniqueFromCountry( country );

	m_pImageStaticFlag->SetFontIndex( countryunique );
	m_pImageStaticFlag->Show( true );
}

void CNSetCountryDlg::SetComboBoxCountry()
{
	m_pComboBoxCountry->Clear();

	SI32 selectedindex = m_pComboBoxContinent->GetSelectedIndex();

	// 일본판일경우 대륙은 자동으로 선택한다.
	SI32 continentunique = 0;
	if ( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		continentunique = 1;
	}
	else
	{
		continentunique = selectedindex + 1;
	}

	SI16 i = 0;
	stComboBoxItemData	data;

	for( i=0;  i<MAX_COUNTRY ; i++)
	{
		if ( pclClient->siServiceArea == ConstServiceArea_Japan)
		{
			if( APRILFOOL_UNIQUE_J == pCountryMgr->siCountryUnique[i] ) continue;
		}
		else
		{
			if( APRILFOOL_UNIQUE == pCountryMgr->siCountryUnique[i] ) continue;
		}
		

        if( pCountryMgr->siContinentUnique[i] == continentunique )	
		{
			data.Set( pCountryMgr->szCountryName[i] );
			m_pComboBoxCountry->AddItem( &data );
		}
	}

	// 국가의 정보가 있는 경우 선택되어 있는 국가을 바꾸어준다.
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.siCountryID != -1 )
	{
		for ( SI32 i=0; i<m_pComboBoxCountry->GetItemNum() ; i++ )
		{
			if ( _tcscmp( m_pComboBoxCountry->GetText(i), pCountryMgr->GetCountryFromUnique(pclClient->pclCM->CR[1]->pclCI->clBI.siCountryID)) == 0 )
                m_pComboBoxCountry->SetCursel( i );
		}
		m_pComboBoxCountry->Refresh();

		if ( pclClient->pclCM->CR[1]->pclCI->clBI.bShowCountry )
		{
			m_pRadioButtonShowOn->SetNonEventCheck();
		}
		else
		{
            m_pRadioButtonShowOff->SetNonEventCheck();
		}
	}
}
void CNSetCountryDlg::SendCountryInfo()
{
	SI32 selectedindex = m_pComboBoxCountry->GetSelectedIndex();
	TCHAR* country = m_pComboBoxCountry->GetText( selectedindex );
	SI32 countryunique = pCountryMgr->GetUniqueFromCountry( country );

	bool showcountry = false;
	if( m_pRadioButtonShowOn->IsCheck() )
	{
        showcountry = true;
		pclClient->pclCM->CR[1]->pclCI->clBI.bShowCountry = true;
	}
	else
	{
		pclClient->pclCM->CR[1]->pclCI->clBI.bShowCountry = false;
	}

	pclClient->pclCM->CR[1]->pclCI->clBI.siCountryID = countryunique;

	cltGameMsgRequest_SetCountryInfo clinfo( countryunique, showcountry );
	cltMsg clMsg(GAMEMSG_REQUEST_SETCOUNTRYINFO, sizeof(cltGameMsgRequest_SetCountryInfo), (BYTE*)&clinfo);
	
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

	DeleteDialog();
}