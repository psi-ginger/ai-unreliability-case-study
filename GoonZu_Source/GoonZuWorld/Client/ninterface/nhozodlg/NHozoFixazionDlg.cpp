#include "../Client/NInterface/NHozoDlg/NHozoFixazionDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"


extern cltCommonLogic* pclClient;

CNHozoFixazionDlg::CNHozoFixazionDlg() : CDialogController()
{
	m_pOutLine = NULL;

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;
	m_pStatic[3] = NULL;

	m_pStatic_ankarandividend	= NULL;
	m_pStatic_NONAME12			= NULL;
	m_pStatic_NONAME13			= NULL;
	m_pEdit_ankarandividend		= NULL;

	m_pEdit = NULL;

}

CNHozoFixazionDlg::~CNHozoFixazionDlg()
{
	SAFE_DELETE(m_pOutLine);

	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pStatic[3]);

	SAFE_DELETE(m_pStatic_ankarandividend);
	SAFE_DELETE(m_pStatic_NONAME12);
	SAFE_DELETE(m_pStatic_NONAME13);
	SAFE_DELETE(m_pEdit_ankarandividend);

	SAFE_DELETE(m_pEdit);


}

void CNHozoFixazionDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NHozoDlg/DLG_Hozo_Prize.ddf"));
	file.CreateChildDialog( this, NHOZOFIXATION_DLG, TEXT("dialog_Hozo_Prize"), StaticCallBackDialogNHozoFixazionDlg, pParent );

	NEWCOUTLINE(m_pOutLine);

	NEWCSTATIC(m_pStatic[0]);
	NEWCSTATIC(m_pStatic[1]);
	NEWCSTATIC(m_pStatic[2]);
	NEWCSTATIC(m_pStatic[3]);

	NEWCEDIT(m_pEdit);

	file.CreateControl( m_pStatic[0], NHOZOFIXATION_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NHOZOFIXATION_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
	file.CreateControl( m_pStatic[2], NHOZOFIXATION_DIALOG_STATIC3, TEXT("statictext_NONAME3") );
	file.CreateControl( m_pStatic[3], NHOZOFIXATION_DIALOG_STATIC4, TEXT("statictext_NONAME4") );

	if(pclClient->IsCountrySwitch(Switch_TutorialVillage))
	{
		NEWCSTATIC(m_pStatic_ankarandividend);
		NEWCSTATIC(m_pStatic_NONAME12);
		NEWCSTATIC(m_pStatic_NONAME13);

		NEWCEDIT(m_pEdit_ankarandividend);

		file.CreateControl( m_pStatic_ankarandividend, NHOZOFIXATION_DIALOG_STATIC_ANKARANDIVIDEND, TEXT("statictext_ankarandividend") );
		file.CreateControl( m_pStatic_NONAME12, NHOZOFIXATION_DIALOG_STATIC12, TEXT("statictext_NONAME12"));
		file.CreateControl( m_pStatic_NONAME13, NHOZOFIXATION_DIALOG_STATIC13, TEXT("statictext_NONAME13"));
		file.CreateControl(	m_pEdit_ankarandividend, NHOZOFIXATION_DIALOG_EDIT_ANKARANDIVIDEND, TEXT("editbox_ankarandividend") );


		NTCHARString64	kVillageName;
		kVillageName = pclClient->pclVillageManager->GetName(Const_Beginner_Village);
		NTCHARString128	kText(GetTxtFromMgr(8243));

		kText.Replace(TEXT("#villagename#"), kVillageName);
		m_pStatic_ankarandividend->SetText(kText);
	}

	file.CreateControl( m_pEdit, NHOZOFIXATION_DIALOG_EDIT, TEXT("editbox_NONAME1") );


	TCHAR* pText = GetTxtFromMgr(3237);
	m_pStatic[0]->SetText(pText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	file.CreateControl( m_pOutLine, NHOZOFIXATION_DIALOG_OUTLINE, TEXT("outline_NONAME1") );

	if (pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina)) 
	{
		// 중국은 20레벨시 받는 정착금관련 설정버튼을 보지이 않게 한다.
		m_pStatic[0]->Show(false)	;
		m_pStatic[1]->Show(false)	;
		m_pStatic[2]->Show(false)	;
		m_pStatic[3]->Show(false)	;
		m_pEdit->Show(false)	;
	}
	Hide();
}

void CNHozoFixazionDlg::Action()
{

}

void CNHozoFixazionDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHozoFixazionDlg::Show()
{
	CControllerMgr::Show( true );
}

void CALLBACK CNHozoFixazionDlg::StaticCallBackDialogNHozoFixazionDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHozoFixazionDlg *pThis = (CNHozoFixazionDlg *)pControllerMgr;
	pThis->CallBackDialogNHozoFixazionDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHozoFixazionDlg::CallBackDialogNHozoFixazionDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
/*	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}*/
}
