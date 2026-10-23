#pragma once 

#include "../../InterfaceMgr/DialogController.h"

class CComboBox;
class CEdit;
class CStatic;
class COutLine;

class CNYezoJapanWarDlg : public CDialogController
{
public:

	CNYezoJapanWarDlg();
	~CNYezoJapanWarDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNYezoJapanWarDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNYezoJapanWarDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetJapanWar(SI32 Sword, SI32 Gun, SI32 Command, SI32 Porter, SI32 VillageUnique);
	void GetJapanWar();
	void JapanWarCheck();
	void SetVillageCombo();
	void EnableEditBox();
	void SetPrintEditBox(BOOL flag);

	inline SI32 GetSword()		{ return m_siSword; }
	inline SI32 GetGun()		{ return m_siGun; }
	inline SI32 GetCommand()	{ return m_siCommand; }
	inline SI32 GetPorter()	{ return m_siPorter; }
	inline SI32 GetVillageUniqe() { return m_siVillageUnique; }

private:

	SI32 m_siSword;
	SI32 m_siGun;
	SI32 m_siCommand;
	SI32 m_siPorter;
	SI32 m_siVillageUnique;
	
	SI32 m_siSelectComboIndex;

	CStatic*	m_pStatic[12];
	CEdit*		m_pEdit[5];
	CComboBox*	m_pCombo;
	COutLine*   m_pOutLine;
};