#pragma once 

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;


class CNHyungzoAnnouncementDlg : public CDialogController
{
public:

	CNHyungzoAnnouncementDlg();
	~CNHyungzoAnnouncementDlg();

	void Create( CControllerMgr *pParent );
	void Action();

	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogNHyungzoAnnouncementDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNHyungzoAnnouncementDlg( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void SetHyungzoAnnouncement(HyungzoMember *pHyungzo);

private:

	CStatic*	m_pStatic;
	CButton*	m_pButton;
	CList*		m_pList;

};