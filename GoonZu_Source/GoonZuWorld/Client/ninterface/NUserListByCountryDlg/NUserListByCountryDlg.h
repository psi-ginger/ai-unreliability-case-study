#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../../common/CommonHeader.h"
#include "../../CommonLogic/CountryMgr/CountryManager.h"



class CNUserListByCountryDlg : public CDialogController
{
public:
	CNUserListByCountryDlg();
	~CNUserListByCountryDlg();

	void Create();
	void Action();
	void SetComboBoxCountry();
	void ShowUserList();
	void AddFriend();
	void SendWhisper();
	void WarpToPerson();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	static void CALLBACK StaticCallBackDialogNUserListByCountry( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK CallBackDialogNUserListByCountry( UINT nEvent, int nControlID, CControllerObj* pControl );

	CStatic*	m_pStaticContinent;
	CComboBox*	m_pComboBoxContinent;
	CStatic*	m_pStaticCountry;
	CComboBox*	m_pComboBoxCountry;
	CList*		m_pListUser;
	CEdit*		m_pEditGreet;
	CButton*	m_pButtonWhisper;
	CButton*	m_pButtonAddfriend;
	CButton*	m_pButtonWarp;
	CButton*	m_pButtonChange;

	SI32		siPersonID[MAX_USERLIST_NUM];

private:
	cltCountryManager	*m_pCountryMgr;

};