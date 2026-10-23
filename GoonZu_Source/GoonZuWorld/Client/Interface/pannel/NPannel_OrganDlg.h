// [영훈] 내부 UI : Pannel (소환수정보)
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"


class CNPannel_OrganDlg : public CDialogController
{

public:
	CNPannel_OrganDlg();
	~CNPannel_OrganDlg();

	void Create();
	void Action();

	static void CALLBACK StaticPannel_OrganDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NPannel_OrganDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Show();
	void Hide();

private:
	void WriteOrgan( SI32 siNowOrganController, SI32 siMaxOrganController, SI32 siOrganType );
	void Magnet();

private:
	InterfaceFactory	m_InterfaceFactory;

};
