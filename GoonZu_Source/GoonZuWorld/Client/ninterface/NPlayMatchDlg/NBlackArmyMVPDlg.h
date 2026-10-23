
#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CCheckBox;
class CComboBox;
class CEdit;
class CImageStatic;
class CList;
class CRadioButton;
class CStatic;

class NBlackArmyMVPDlg : public CDialogController
{
public:


	CComboBox     		*m_pCombo_MapName;
	CList				*m_pList_MVP;
	CStatic				*m_pStatic_MapName;
	CStatic				*m_pStatic_WarLose;
	
	CButton 			*m_pButton_close;

	NBlackArmyMVPDlg();
	~NBlackArmyMVPDlg();

	void Create();
	//void RetMsgBox( bool RetValue, SI16 MsgIndex, char *pData, SI16 DataLen );

	static void CALLBACK NBlackArmyMVPStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NBlackArmyMVPProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Close();

	void LoadMapName();
	void SetRanks(SI32 warmapindex, SI32 *scores, TCHAR **players);

};

//#endif
