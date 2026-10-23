#pragma once 

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"
#include "../Server/Minister/MinisterBase.h"
#include "../Server/Minister/Minister-Hozo.h"


class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class CNHozoAppointmentDlg : public CDialogController
{
public:

	CNHozoAppointmentDlg();
	~CNHozoAppointmentDlg();

	void Create();
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNHozoAppointmentDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHozoAppointmentDlg( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	bool SetListDlg(SI32 index);
	void ShowListDlg(HozoAngelList Hozoangellist []);

	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화

private:


	HozoAngelList	m_stHozoAngelList[MAX_HOZO_ANGELLIST];

	SI32 m_siSelectListInx;
	SI32 m_siListIndex[MAX_HOZO_ANGELLIST];

	SI32 m_siSelectList;

	CList*		m_pList;
	CButton*	m_pButton[3];


};