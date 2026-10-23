#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../../common/item/ItemCommon/ItemUnit.h"
#include "../../CommonLogic/CountryMgr/CountryManager.h"



class CNSetCountryDlg : public CDialogController
{
public:
	CStatic*		m_pStaticSubject;
	CStatic*		m_pStaticContinent;
	CComboBox*		m_pComboBoxContinent;
	CStatic*		m_pStaticCountry;
	CComboBox*		m_pComboBoxCountry;
	CImageStatic*	m_pImageStaticFlag;
	CStatic*		m_pStaticSetting;
	COutLine*		m_pOutLineSetting;
	CStatic*		m_pStaticShowCountry;
	CRadioButton*	m_pRadioButtonShowOn;
	CRadioButton*	m_pRadioButtonShowOff;
	CStatic*		m_pStaticExplain1;
	CStatic*		m_pStaticExplain2;
	CButton*		m_pButtonSubmit;

	CNSetCountryDlg();
	~CNSetCountryDlg();

	void Create();
	void Action();
	void ShowFlagImage();
	void SetComboBoxCountry();
	void SendCountryInfo();

	static void CALLBACK StaticCallBackDialogNSetCountry( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK CallBackDialogNSetCountry( UINT nEvent, int nControlID, CControllerObj* pControl );

private:
	cltCountryManager*	pCountryMgr;
};