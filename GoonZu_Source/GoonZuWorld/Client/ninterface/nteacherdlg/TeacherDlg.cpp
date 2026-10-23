#include "../Client/NInterface/NTeacherDlg/TeacherDlg.h"
#include "../Client/NInterface/NTeacherDlg/FatherDlg.h"
#include "../Client/NInterface/NTeacherDlg/ChildDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

extern cltCommonLogic* pclClient;

CNTeacherDlg::CNTeacherDlg() : CDialogController()
{
	m_bTeacherMode = false;

	m_pNFatherDlg = NULL;
	m_pNChildDlg = NULL;
}

CNTeacherDlg::~CNTeacherDlg()
{
	if(m_pNFatherDlg)
	{
		delete m_pNFatherDlg;
		m_pNFatherDlg = NULL;
	}

	if(m_pNChildDlg)
	{
		delete m_pNChildDlg;
		m_pNChildDlg = NULL;
	}
}

void CNTeacherDlg::Create(bool bTeacherMode)
{
/*	if(bTeacherMode)
	{
		m_bTeacherMode = true;
		if(m_pNChildDlg == NULL)
		{
			m_pNChildDlg = new CNChildDlg();
			m_pNChildDlg->Create();
		}
	}
	else
	{
		m_bTeacherMode = false;
		if(m_pNFatherDlg == NULL)
		{
			m_pNFatherDlg = new CNFatherDlg();
			m_pNFatherDlg->Create();
		}
	}

	Show();*/
}


void CALLBACK CNTeacherDlg::StaticCallBackDialogNTeacherDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
}

void CALLBACK CNTeacherDlg::CallBackDialogNTeacherDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
}

void CNTeacherDlg::Hide()
{
/*	SetTimerOnOff( false );

	CControllerMgr::Show( false );

	if(m_bTeacherMode)
	{
		m_pNChildDlg->Hide();
	}
	else
	{
		m_pNFatherDlg->Hide();
	}

	return;*/
}

void CNTeacherDlg::Show()
{
/*	SetTimerOnOff( true );

	CControllerMgr::Show( true );

	if(m_bTeacherMode)
	{
		m_pNChildDlg->Show();
	}
	else
	{
		m_pNFatherDlg->Show();
	}


	return;*/
}

void CNTeacherDlg::Action()
{
	if(m_bTeacherMode)
	{
		m_pNChildDlg->Action();
	}
	else
	{
		m_pNFatherDlg->Action();
	}
}

void CNTeacherDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{

}