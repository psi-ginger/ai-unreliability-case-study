//#include "../Client/NInterface/NHomePageDlg/HomePageDlg.h"
//
//#include "../Client/InterfaceMgr/InterfaceMgr.h"
//
//#include "../Client/InterfaceMgr/Interface/Static.h"
//#include "../Client/InterfaceMgr/Interface/Edit.h"
//#include "../Client/InterfaceMgr/Interface/ListView.h"
//#include "../Client/InterfaceMgr/Interface/Button.h"
//#include "../Client/InterfaceMgr/Interface/ComboBox.h"
//#include "../Client/InterfaceMgr/Interface/ImageStatic.h"
//
//#include "../Client/InterfaceMgr/InterfaceMgr.h"
//#include "../Client/InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
//
//#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
//
//#include "../../Client.h"
//
//#include "..\..\Resource.h"
//#include "Msg\MsgType-Quest.h"
//#include "../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"
////#include "../Client/Interface/MonsterInfoDlg//MonsterInfoDlg.h"
//
//#include "../CommonLogic/MsgType-Person.h"
//
//extern cltCommonLogic* pclClient;
//
////-------------------------------------
//// Common
////-------------------------------------
//#include "PersonInfo.h"
//#include "Char\CharManager\CharManager.h"
//
//extern cltCommonLogic* pclClient;
//
//CNHomePageDlg::CNHomePageDlg() : CDialogController()
//{
//	SI32 i;
//	for( i=0; i < 6; ++i )
//		m_pButton[i] = NULL;
//}
//
//CNHomePageDlg::~CNHomePageDlg()
//{
//	for(int i=0; i< 6; ++i)
//	{
//		if(m_pButton[i])
//		{
//			delete m_pButton[i];
//			m_pButton[i] = NULL;
//		}
//	}
//}
//
//void CNHomePageDlg::Create()
//{
//	cltClient *pclclient = (cltClient *)pclClient;
//	TCHAR* pText= GetTxtFromMgr(3287);
//
//	CDialogController::Create( NHOMEPAGE_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNHomePageDlg, pText, TEXT("NInterface/Interface/Interface_20X20_00_109.Spr"), 9, true, 9, 450, 70, 180, 220, true, false, true );
//
//	//	m_pImage = new CImageStatic( this );
//	//	m_pImage->Create( NITEM_DIALOG_IMAGESTATIC, NULL, 0, false, 0, 0, 18, 370, 495, 370, 495 );
//
//	SI32 i;
//	for ( i = 0; i < 6; ++i )
//	{
//		m_pButton[i] = new CButton( this );
//
//		switch ( i )
//		{
//		case 0:
//			{
//				TCHAR* pText = GetTxtFromMgr(3159);
//				m_pButton[i]->Create( NHOMEPAGE_DIALOG_GOONZU_BUTTON, NULL, TEXT("NInterface/Button/Button_4x24_00_103.SPR"), 0, true, 0, 50, 30, 120, 24, 120, 24 );
//				m_pButton[i]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//			}
//			break;
//		case 1:
//			{
//				TCHAR* pText = GetTxtFromMgr(3160);
//				m_pButton[i]->Create( NHOMEPAGE_DIALOG_GOMZI_BUTTON, NULL, TEXT("NInterface/Button/Button_4x24_00_103.SPR"), 0, true, 0, 50, 60, 120, 24, 120, 24 );
//				m_pButton[i]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//			}
//			break;
//		case 2:
//			{
//				TCHAR* pText = GetTxtFromMgr(3161);
//				m_pButton[i]->Create( NHOMEPAGE_DIALOG_MEDIA_BUTTON, NULL, TEXT("NInterface/Button/Button_4x24_00_103.SPR"), 0, true, 0, 50, 90, 120, 24, 120, 24 );
//				m_pButton[i]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//			}
//			break;
//		case 3:
//			{
//				TCHAR* pText = GetTxtFromMgr(3162);
//				m_pButton[i]->Create( NHOMEPAGE_DIALOG_KNOWLEDGE_BUTTON, NULL, TEXT("NInterface/Button/Button_4x24_00_103.SPR"), 0, true, 0, 50, 120, 120, 24, 120, 24 );
//				m_pButton[i]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//			}
//			break;
//		case 4:
//			{
//				TCHAR* pText = GetTxtFromMgr(3163);
//				m_pButton[i]->Create( NHOMEPAGE_DIALOG_LOVEROOM_BUTTON, NULL, TEXT("NInterface/Button/Button_4x24_00_103.SPR"), 0, true, 0, 50, 150, 120, 24, 120, 24 );
//				m_pButton[i]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//			}
//			break;
//		case 5:
//			{
//				TCHAR* pText = GetTxtFromMgr(3164);
//				m_pButton[i]->Create( NHOMEPAGE_DIALOG_POLICE_BUTTON, NULL, TEXT("NInterface/Button/Button_4x24_00_103.SPR"), 0, true, 0, 50, 180, 120, 24, 120, 24 );
//				m_pButton[i]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//			}
//			break;
//		}
//	}
//
//	Hide();
//}
//
//
//void CALLBACK CNHomePageDlg::StaticCallBackDialogNHomePageDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
//{
//	CNHomePageDlg *pThis = (CNHomePageDlg *)pControllerMgr;
//	pThis->CallBackDialogNHomePageDlg( nEvent, nControlID, pControl );
//}
//
//void CALLBACK CNHomePageDlg::CallBackDialogNHomePageDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
//{
//	cltClient *pclclient = (cltClient *)pclClient;
//
//	switch ( nControlID )
//	{
//	case COMMON_DIALOG_CLOSEBUTTON:
//		{
//			Hide();
//		}
//		break;
//	case NHOMEPAGE_DIALOG_GOONZU_BUTTON:
//		{
//			switch( nEvent )
//			{
//			case EVENT_BUTTON_CLICKED:
//				{
//					TCHAR url[ MAX_PATH ];
//					pclClient->GetGoonZuUrl( url, pclclient->pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );
//
//					ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), url, NULL, SW_SHOWNORMAL );
//				}
//				break;
//			}
//		}
//		break;
//	case NHOMEPAGE_DIALOG_GOMZI_BUTTON:
//		{
//			switch( nEvent )
//			{
//			case EVENT_BUTTON_CLICKED:
//				{
//					SI32 siCharUnique = pclClient->pclCM->CR[1]->GetCharUnique();
//					cltGameMsgRequest_Userid clUserid( siCharUnique );
//					cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
//					
//					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
//				}
//				break;
//			}
//		}
//		break;
//	case NHOMEPAGE_DIALOG_MEDIA_BUTTON:
//		{
//			switch( nEvent )
//			{
//			case EVENT_BUTTON_CLICKED:
//				{
//					TCHAR url[ MAX_PATH ];
//					pclClient->GetSubSiteUrl( url, TEXT("center/board/media_center.asp?menu=new"), pclclient->pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );
//
//					ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), url, NULL, SW_SHOWNORMAL );
//
//					//-----------------------------------------------------------
//					// 게임 홈페이지로 가는 퀘스트가 진행중이면, 
//					// 게임 서버로 퀘스트가 완료되었음을 알려준다.
//					//-----------------------------------------------------------
//					SI16 questtype, questunique, para1, para2;
//					SI16 questclearcond =  QUEST_CLEAR_COND_NEWS;
//					if(pclclient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
//					{
//						cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
//						cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
//						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
//					}
//				}
//				break;
//			}
//		}
//		break;
//	case NHOMEPAGE_DIALOG_KNOWLEDGE_BUTTON:
//		{
//			switch( nEvent )
//			{
//			case EVENT_BUTTON_CLICKED:
//				{
//					if(pclclient->m_pSearchDetailDlg)	pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_KNOWLEDGE, TEXT(""));
//				}
//				break;
//			}
//		}
//		break;
//	case NHOMEPAGE_DIALOG_LOVEROOM_BUTTON:
//		{
//			switch( nEvent )
//			{
//			case EVENT_BUTTON_CLICKED:
//				{
//					if(pclclient->m_pSearchDetailDlg)	pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_SARANGBANG, TEXT(""));
//				}
//				break;
//			}
//		}
//		break;
//	case NHOMEPAGE_DIALOG_POLICE_BUTTON:
//		{
//			switch( nEvent )
//			{
//			case EVENT_BUTTON_CLICKED:
//				{
//					if(pclclient->m_pSearchDetailDlg)	pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_HYUNGZO, TEXT(""));
//				}
//				break;
//			}
//		}
//		break;
//		break;
//	}
//}
//
//void CNHomePageDlg::Hide()
//{
//	CControllerMgr::Show( false );
//
//	return;
//}
//
//void CNHomePageDlg::Show()
//{
//	if(CDialogController::IsShow())
//	{	Hide();	return;	}
//
//	CControllerMgr::Show( true );
//
//	return;
//}
//
//void CNHomePageDlg::Action()
//{
//
//}
//
//void CNHomePageDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
//{
//
//}