#include "../Client/NInterface/NHyungzoDlg/NHyungzoAnnouncementDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"

#include "../Client/Client.h"

#include "../common/Char/CharManager/CharManager.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

extern cltCommonLogic* pclClient;

CNHyungzoAnnouncementDlg::CNHyungzoAnnouncementDlg() : CDialogController()
{
	m_pStatic = NULL;
	m_pButton = NULL;
	m_pList	  = NULL;
}

CNHyungzoAnnouncementDlg::~CNHyungzoAnnouncementDlg()
{
	SAFE_DELETE(m_pStatic);
	SAFE_DELETE(m_pButton);
	SAFE_DELETE(m_pList);
}

void CNHyungzoAnnouncementDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NHyungzoDlg/DLG_Hyungzo_Announcement.ddf"));
	file.CreateChildDialog( this, NHYUNGZOANNOUNCEMENT_DLG, TEXT("dialog_Hyungzo_Announcement"), StaticCallBackDialogNHyungzoAnnouncementDlg, pParent );

	NEWCSTATIC(m_pStatic);
	NEWCBUTTON(m_pButton);
	NEWCLIST(m_pList);

	file.CreateControl( m_pStatic, NHYUNGZOANNOUNCEMENT_DIALOG_STATIC, TEXT("statictext_NONAME1") );
//KHY - 0609 - 포도청 삭제.
//	file.CreateControl( m_pButton, NHYUNGZOANNOUNCEMENT_DIALOG_BUTTON, TEXT("button_NONAME1") );
	file.CreateControl( m_pList, NHYUNGZOANNOUNCEMENT_DIALOG_LIST, TEXT("listbox_NONAME1") );

	TCHAR* pText = GetTxtFromMgr(1564);
	m_pList->SetColumn(0,85, pText);
	pText = GetTxtFromMgr(2042);
	m_pList->SetColumn(1,80, pText);
	pText = GetTxtFromMgr(2043);
	m_pList->SetColumn(2,115, pText);
	pText = GetTxtFromMgr(2044);
	//m_pList->SetColumn(3,150,pText);
	m_pList->SetBKColorUse( true );
	m_pList->SetBorder( true );
	m_pList->Refresh();

	Hide();
}

void CNHyungzoAnnouncementDlg::Action()
{

}

void CNHyungzoAnnouncementDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHyungzoAnnouncementDlg::Show()
{
	CControllerMgr::Show( true );

	pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_LOOKPRISON);
}

void CALLBACK CNHyungzoAnnouncementDlg::StaticCallBackDialogNHyungzoAnnouncementDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHyungzoAnnouncementDlg *pThis = (CNHyungzoAnnouncementDlg *)pControllerMgr;
	pThis->CallBackDialogNHyungzoAnnouncementDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHyungzoAnnouncementDlg::CallBackDialogNHyungzoAnnouncementDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case NHYUNGZOANNOUNCEMENT_DIALOG_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
 					//pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_HYUNGZO, TEXT("") );
				}
				break;
			}
		}
		break;
	}
}

void CNHyungzoAnnouncementDlg::SetHyungzoAnnouncement(HyungzoMember *pHyungzo)
{
//	ListView_DeleteAllItems(m_hLogList);
	m_pList->Clear();

	SI32 listindex = 0 ;

	for ( int i = 0 ; i < MAX_PRISON_LIST_NUM ; i ++ )
	{
		if ( pHyungzo[i].siDate <= 0 ) continue ;
		if ( pHyungzo[i].siPrisonSecond <= 0 ) continue ;
		TCHAR buffer[256] = TEXT("") ;
		cltDate cldate;
#if 0 //KHY - 0606
		cldate.MakeFullHourFromHourVary(pHyungzo[i].siDate);
#else
		cldate.MakeFullDateFromDateVary(pHyungzo[i].siDate);
#endif

#if 1 //KHY - 0607 : 시간을 제외하고 출력한다.
		if(cldate.GetRealDateText(buffer) == false) continue;
#else
		if(cldate.GetDateText(buffer) == false) continue;
#endif

		stListBoxItemData Data;
		Data.Set(0, buffer);
		Data.Set(1, pHyungzo[i].szCharName);
		SI32 minute = pHyungzo[i].siPrisonSecond / 60 ;
		TCHAR* pText = GetTxtFromMgr(1252);
		StringCchPrintf(buffer, 256, pText,minute);
		Data.Set(2, buffer);
		Data.Set(3, pHyungzo[i].szPrisonReason);
		
		m_pList->AddItem(&Data);
		listindex ++ ;
	}
	m_pList->Refresh();
}
