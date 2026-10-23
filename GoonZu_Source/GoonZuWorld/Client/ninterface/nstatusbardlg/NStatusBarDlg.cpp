#include "NStatusBarDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/Interface/ChatEdit.h"

#include "../../StatusBar/ChatStatusBar/ChatStatusBarBuffer.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "../../Interface/ChatDlg/ChatDlg.h"
//#include "../../Interface/ResidentsChatDlg/ResidentsChatDlg.h"
#include "../../Interface/RichEditChatMgrDlg/RichEditChatMgrDlg.h"

#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../Prompt/PromptMgr.h"

#include "../NMenuDlg/NMenuDlg.h"

#include "../NChatDlg/NChatDlg.h"
#include "../Client/NInterface/NResidentsDlg/NResidentsDlg.h"
#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/CommonLogic.h"
#include "../../../CommonLogic/MsgType-System.h"
#include "../../../CommonLogic/MsgType-Item.h"
#include "../../../CommonLogic/Msg/MsgType-ItemMall.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../../Common/Char/CharManager/CharManager.h"
#include "../../../Common/Skill/Skill-Manager.h"
#include "../../../Common/Skill/Skill.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../../CommonLogic/MsgType-Person.h"

#include "../../StatusBar/ChatStatusBar/ChatStatusBarBuffer.h"

#include "../../Interface/ChatDlg/ChatDlg.h"
//#include "../../Interface/ResidentsChatDlg/ResidentsChatDlg.h"
#include "../../Interface/RichEditChatMgrDlg/RichEditChatMgrDlg.h"

#include "../../../lib/ScreenShot/CaptureJPG.h"

#include "../../Interface/ResidentsViewDlg/ResidentsViewDlg.h"
#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../NInterface/NQuestDlg/NQuestDlg.h"
#include "../../NInterface/NNewQuestDlg/NNewQuestDlg.h"
#include "../../NInterface/NMagicInfoDlg/MagicInfoDlg.h"
#include "../../NInterface/NMagicInfoDlg/NewMagicInfoDlg.h"
#include "../../NInterface/NCommonMsgDlg/NCommonMsgDlg.h"
#include "../../NInterface/ncharstatus/NCharQuest.h"
#include "../../NInterface/nminimapdlg/NMiniMapDlg.h"
#include "../../NInterface/nsoundonoffdlg/NSoundOnOffDlg.h"
#include "../../NInterface/nfrienddlg/NNewFriendDlg.h"
#include "../../NInterface/NNewMakeItem/NItemRepair.h"
#include "../../NInterface/NNewMakeItem/NNewMakeItemDlg.h"

#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"

#include "Msg/MsgType-PersonalShop.h"
#include "Msg/MsgType-Quest.h"

extern cltCommonLogic*	pclClient;
extern SI32				gsiResolution;
extern CLoginNoticeDlg	g_LoginNoticeDlg;

//================================================================================================
// CNStatusBarPopupParentDlg
//================================================================================================
CNStatusBarPopupParentDlg::CNStatusBarPopupParentDlg()
: m_pclButton_Skill(NULL),
  m_pclButton_Magic(NULL),
  m_pclButton_Quest(NULL),
  m_pclButton_Community(NULL),
  m_pclButton_System(NULL),
  m_pclButton_HomePage(NULL)
{

}

CNStatusBarPopupParentDlg::~CNStatusBarPopupParentDlg()
{
	SAFE_DELETE(m_pclButton_Skill);
	SAFE_DELETE(m_pclButton_Magic);
	SAFE_DELETE(m_pclButton_Quest);
	SAFE_DELETE(m_pclButton_Community);
	SAFE_DELETE(m_pclButton_System);
	SAFE_DELETE(m_pclButton_HomePage);
}

void CNStatusBarPopupParentDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if (g_SoundOnOffDlg.m_bMaxWindowMode)		//전체화면
	{
		CDialogController::Create( STATUSBAR_POPUPPARENT_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCallBackDialogNStatusBarPopupParentProc, NULL, TEXT("NInterface/Interface/Interface_114X153_00_000.SPR"), 0, false, 0, MAP_TILE_XSIZE + pclclient->GetScreenRX(), pclclient->GetScreenYSize() - 25 - 153, 114, 153, false, false, false );
	}
	else
	{	
		CDialogController::Create( STATUSBAR_POPUPPARENT_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCallBackDialogNStatusBarPopupParentProc, NULL, TEXT("NInterface/Interface/Interface_114X153_00_000.SPR"), 0, false, 0, MAP_TILE_XSIZE + pclclient->GetScreenRX(), pclclient->GetScreenYSize() - 25 - 153, 114, 153, false, false, false );
	}

	m_pclButton_Skill		= new CButton(this);
	m_pclButton_Magic		= new CButton(this);
	m_pclButton_Quest		= new CButton(this);
	m_pclButton_Community	= new CButton(this);
	m_pclButton_System		= new CButton(this);
	m_pclButton_HomePage	= new CButton(this);

	NTCHARString128	kText;
	kText = GetTxtFromMgr(8313);
	m_pclButton_Skill->Create(STATUSBAR_POPUPPARENT_DLG_BUTTON_SKILL, kText, TEXT("NInterface/Button/Button_106X20_00_000.SPR"), 0, false, 0, 4, 4, 106, 20, 106, 20);
	kText = GetTxtFromMgr(8314);
	m_pclButton_Magic->Create(STATUSBAR_POPUPPARENT_DLG_BUTTON_MAGIC, kText, TEXT("NInterface/Button/Button_106X20_00_000.SPR"), 4, false, 0, 4, 29, 106, 20, 106, 20);
	kText = GetTxtFromMgr(8315);
	m_pclButton_Quest->Create(STATUSBAR_POPUPPARENT_DLG_BUTTON_QUEST, kText, TEXT("NInterface/Button/Button_106X20_00_000.SPR"), 8, false, 0, 4, 54, 106, 20, 106, 20);
	kText = GetTxtFromMgr(8316);
	m_pclButton_Community->Create(STATUSBAR_POPUPPARENT_DLG_BUTTON_COMMUNITY, kText, TEXT("NInterface/Button/Button_106X20_00_000.SPR"), 12, false, 0, 4, 79, 106, 20, 106, 20);
	kText = GetTxtFromMgr(8317);
	m_pclButton_System->Create(STATUSBAR_POPUPPARENT_DLG_BUTTON_HOMEPAGE, kText, TEXT("NInterface/Button/Button_106X20_00_000.SPR"), 20, false, 0, 4, 104, 106, 20, 106, 20);
	kText = GetTxtFromMgr(8318);
	m_pclButton_HomePage->Create(STATUSBAR_POPUPPARENT_DLG_BUTTON_SYSTEM, kText, TEXT("NInterface/Button/Button_106X20_00_000.SPR"), 16, false, 0, 4, 129, 106, 20, 106, 20);

	if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
	{
		m_pclButton_Skill->SetFontKind(TEXT("TimeNTales"));
		m_pclButton_Magic->SetFontKind(TEXT("TimeNTales"));
		m_pclButton_Quest->SetFontKind(TEXT("TimeNTales"));
		m_pclButton_Community->SetFontKind(TEXT("TimeNTales"));
		m_pclButton_System->SetFontKind(TEXT("TimeNTales"));
		m_pclButton_HomePage->SetFontKind(TEXT("TimeNTales"));

		m_pclButton_Skill->SetFontHeight(15);
		m_pclButton_Magic->SetFontHeight(15);
		m_pclButton_Quest->SetFontHeight(15);
		m_pclButton_Community->SetFontHeight(15);
		m_pclButton_System->SetFontHeight(15);
		m_pclButton_HomePage->SetFontHeight(15);
	}
	else
	{
		m_pclButton_Skill->SetFontHeight(11);
		m_pclButton_Magic->SetFontHeight(11);
		m_pclButton_Quest->SetFontHeight(11);
		m_pclButton_Community->SetFontHeight(11);
		m_pclButton_System->SetFontHeight(11);
		m_pclButton_HomePage->SetFontHeight(11);
	}

	m_pclButton_Skill->Refresh();
	m_pclButton_Magic->Refresh();
	m_pclButton_Quest->Refresh();
	m_pclButton_Community->Refresh();
	m_pclButton_System->Refresh();
	m_pclButton_HomePage->Refresh();

	SetTopMostWindow( true );
	SetTimerOnOff( true );
}

void CNStatusBarPopupParentDlg::ShowStatusBarPopupParentDialog(bool bShow)
{
	cltClient *pclclient = (cltClient *)pclClient;

	if(bShow)
	{
		Show(bShow);
	}
	else
	{
		Show(false);
		if(pclclient->m_pDialog[STATUSBAR_POPUPCHILDSYSTEM_DLG])		pclclient->m_pDialog[STATUSBAR_POPUPCHILDSYSTEM_DLG]->Show(false);
		if(pclclient->m_pDialog[STATUSBAR_POPUPCHILDCOMMUNITY_DLG])		pclclient->m_pDialog[STATUSBAR_POPUPCHILDCOMMUNITY_DLG]->Show(false);
	}
}

void CALLBACK CNStatusBarPopupParentDlg::StatiCallBackDialogNStatusBarPopupParentProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNStatusBarPopupParentDlg *pThis = (CNStatusBarPopupParentDlg *)pControllerMgr;
	pThis->CallBackDialogNStatusBarPopupParentProc( nEvent, nControlID, pControl );
	return;
}


void CNStatusBarPopupParentDlg::CallBackDialogNStatusBarPopupParentProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient *pclclient = (cltClient *)pclClient;

	CNStatusBarPopupParentDlg* pPopupParent = this;

	switch(nEvent)
	{
	case EVENT_BUTTON_CLICKED:
		{
			switch(nControlID)
			{
			case STATUSBAR_POPUPPARENT_DLG_BUTTON_SKILL:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					if ( pclclient->GetGameStatus() == GS_GAME )
					{
						if( pclclient->pclSkillManager)
						{
							if ( g_SoundOnOffDlg.m_bFullScreenMode )
							{
								pclclient->CreateInterface( NSKILL_DLG );
							}
							else
							{
								pclclient->CreateSkillDlg(0);
							}
						}
					}
				}
				break;
			case STATUSBAR_POPUPPARENT_DLG_BUTTON_MAGIC:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					if( true == pclClient->IsCountrySwitch(Switch_SkillBook) )
					{
						if ( ((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->IsShow() == false )
						{
							((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->Show();
						}
						else
						{
							((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->Hide();
						}	
					}
					else
					{
						if ( ((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->IsShow() == false )
						{
							((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->Show();
						}
						else
						{
							((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->Hide();
						}	
					}
				}
				break;
			case STATUSBAR_POPUPPARENT_DLG_BUTTON_QUEST:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);
					
					if ( true == pclClient->IsCountrySwitch(Switch_QuestInfoDlg_Renewal) )
					{
						pclclient->CreateInterface( NQUESTINFODLG_PARENT );
					}
					else
					{
						if ( ((cltNQuestStatus*)pclclient->m_pDialog[ NQUESTINFO_DLG ])->IsShow() == false )
						{
							((cltNQuestStatus*)pclclient->m_pDialog[ NQUESTINFO_DLG ])->Show();
						}
						else
						{
							((cltNQuestStatus*)pclclient->m_pDialog[ NQUESTINFO_DLG ])->Hide();
						}
					}
				}
				break;
			case STATUSBAR_POPUPPARENT_DLG_BUTTON_COMMUNITY:
				{
					pclclient->m_pDialog[STATUSBAR_POPUPCHILDSYSTEM_DLG]->Show(false);
					if(pclclient->m_pDialog[STATUSBAR_POPUPCHILDCOMMUNITY_DLG])
					{
						bool bShow = pclclient->m_pDialog[STATUSBAR_POPUPCHILDCOMMUNITY_DLG]->IsShow();
						pclclient->m_pDialog[STATUSBAR_POPUPCHILDCOMMUNITY_DLG]->Show(!bShow);
					}
				}
				break;
			case STATUSBAR_POPUPPARENT_DLG_BUTTON_SYSTEM:
				{
					pclclient->m_pDialog[STATUSBAR_POPUPCHILDCOMMUNITY_DLG]->Show(false);
					if(pclclient->m_pDialog[STATUSBAR_POPUPCHILDSYSTEM_DLG])
					{
						bool bShow = pclclient->m_pDialog[STATUSBAR_POPUPCHILDSYSTEM_DLG]->IsShow();
						pclclient->m_pDialog[STATUSBAR_POPUPCHILDSYSTEM_DLG]->Show(!bShow);
					}
				}
				break;
			case STATUSBAR_POPUPPARENT_DLG_BUTTON_HOMEPAGE:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					if( pclclient->siServiceArea == ConstServiceArea_English )
					{
						// PCK : 글로벌의 경우 NHN회원과 NDI회원에 따라서 보여지는 링크페이지가 다르다. (09.03.30)			
						NTCHARString32	kID(pclclient->szID);

						if( kID.Find("@nh") >= 0 )		// NHN 회원이면
						{
							ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.ijji.com/", NULL, SW_SHOWNORMAL );
						}
						else							// NDI 회원이면
						{
							ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.ndoorsgames.com/center/default.asp", NULL, SW_SHOWNORMAL );
						}								
					}
					//KHY - 0617 - 일본 채널링 추가.
					else if( pclclient->siServiceArea == ConstServiceArea_Japan )
					{
						TCHAR * AccountID = pclclient->pclCM->CR[1]->pclCI->clBI.szAccountID;

						// 퍼블리셔 확인을 위한 파싱 부분.
						SI16 idSize = strlen(AccountID);

						TCHAR temPublisher[20] = {'\0',};
						bool	    tempFlage =false;
						SI16    tempNum = 0;

						for(SI16 i =0;i < idSize; i++)
						{
							// "@" 문자 이전의 문자만을 리턴한다.
							if((tempFlage == false) &&( AccountID[i] == '@'))
							{	
								tempFlage = true;
								continue;
							}

							if(tempFlage == true)
							{
								temPublisher[tempNum] = AccountID[i];
								tempNum++;
							}
						}
				
						if(strcmp(temPublisher, "HG") == 0) // 한게임.
						{
							ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://kunshu.hangame.co.jp", NULL, SW_SHOWNORMAL );
						}
						else if(strcmp(temPublisher, "GP") == 0) // 게임팟.
						{
							ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://kunshu.postar.jp", NULL, SW_SHOWNORMAL );
						}
						else if(strcmp(temPublisher, "GA") == 0) // 감마니아.
						{
							ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://kunshu.ndoors.jp/gamania", NULL, SW_SHOWNORMAL );							
						}
						else if(strcmp(temPublisher, "ND") == 0) // NDE
						{
							ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://kunshu.ndoors.jp", NULL, SW_SHOWNORMAL );
						}
						else if(strcmp(temPublisher, "NX") == 0) // 넥슨  - 임시 NDE
						{
							ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://kunshu.nexon.co.jp", NULL, SW_SHOWNORMAL );
						}
						else
						{
							return;	
						}
						
					}
					else if( pclclient->siServiceArea == ConstServiceArea_NHNChina )// 임시 NHN중국 
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://www.www.newhljz.cn", NULL, SW_SHOWNORMAL );								
					}
					else if( pclclient->siServiceArea == ConstServiceArea_China )
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://www.hljz.net/Default_Z1.aspx", NULL, SW_SHOWNORMAL );				
					}
					//KHY -0610 - S 추가.
					else if( pclclient->siServiceArea == ConstServiceArea_Korea )
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://goonzus.ndoors.com/", NULL, SW_SHOWNORMAL );
					}
					else if( pclclient->siServiceArea == ConstServiceArea_Taiwan )
					{
						//ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://global.goonzu.com", NULL, SW_SHOWNORMAL );
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://www.omg.com.tw/", NULL, SW_SHOWNORMAL );
					}
					else if( pclclient->siServiceArea == ConstServiceArea_USA )
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.ijji.com/", NULL, SW_SHOWNORMAL );								
					}
					else if( pclclient->siServiceArea == ConstServiceArea_EUROPE )
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.aeriagames.com", NULL, SW_SHOWNORMAL );								
					}
				}
				break;
			}
		}
		break;
	}
}


//================================================================================================
// CNStatusBarPopupChildCommunityDlg
//================================================================================================
CNStatusBarPopupChildCommunityDlg::CNStatusBarPopupChildCommunityDlg()
{
	m_pclButtons.ZeroMem();
	m_pclButton_Buttons.ZeroMem();
}

CNStatusBarPopupChildCommunityDlg::~CNStatusBarPopupChildCommunityDlg()
{
	for(SI32 i=0; i<NSTATUSBAR_POPUPCHILD_MAXBUTTON; i++)
	{
		SAFE_DELETE(m_pclButton_Buttons[i]);
	}

	for(SI32 i=0; i<NSTATUSBAR_POPUPCHILD_MAXBUTTON; i++)
	{
		SAFE_DELETE(m_pclButtons[i]);
	}
}

void CNStatusBarPopupChildCommunityDlg::Create()
{
	SI32		siWidth = 0;
	SI32		siHeight = 0;

	cltClient *pclclient = (cltClient *)pclClient;

	SI32		siButtonCount = 0;

	NTCHARString128	kText;

	kText = GetTxtFromMgr(8319);
	m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDCOMMUNITY_DLG_VILLAGE, kText);	siButtonCount++;
	kText = GetTxtFromMgr(8320);
	m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDCOMMUNITY_DLG_PARENT, kText);	siButtonCount++;
	kText = GetTxtFromMgr(8321);
	m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDCOMMUNITY_DLG_GUILD, kText);	siButtonCount++;
	kText = GetTxtFromMgr(8322);
	m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDCOMMUNITY_DLG_FRIEND, kText);	siButtonCount++;
	kText = GetTxtFromMgr(8323);
	m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDCOMMUNITY_DLG_PRIVATESHOP, kText);	siButtonCount++;
	kText = GetTxtFromMgr(8324);
	m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDCOMMUNITY_DLG_MANUFACTURE, kText);	siButtonCount++;
	kText = GetTxtFromMgr(8325);
	m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDCOMMUNITY_DLG_SKILLMASTER, kText);	siButtonCount++;
	if ( pclClient->IsCountrySwitch(Switch_PVP_League) )
	{
		kText = GetTxtFromMgr(9581);
		m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDCOMMUNITY_DLG_PVPLEAGUE, kText);	siButtonCount++;
	}
	if ( pclClient->IsCountrySwitch(Switch_WeddingSystem) )
	{
		kText = GetTxtFromMgr(30100);
		m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDCOMMUNITY_DLG_FAMILYINFO, kText);	siButtonCount++;
		//m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDCOMMUNITY_DLG_FAMILYINFO, kText);	siButtonCount++;

	}

	if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
	{
		kText = GetTxtFromMgr(40126);
		m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDCOMMUNITY_DLG_REPAIRREQUEST, kText);	siButtonCount++;
	}

	siWidth = 100;
	siHeight = siButtonCount * 20 + 16;

	if (g_SoundOnOffDlg.m_bMaxWindowMode)		//전체화면
	{
		CDialogController::Create( STATUSBAR_POPUPCHILDCOMMUNITY_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCallBackDialogNStatusBarPopupChildCommunityProc, NULL, TEXT("NInterface/Interface/INTERFACE_4x4_00_000.SPR"), 0, true, 9, MAP_TILE_XSIZE + pclclient->GetScreenRX() + 114, pclclient->GetScreenYSize() - 25 - 51 - siHeight, siWidth, siHeight, false, false, false );
	}
	else
	{	
		CDialogController::Create( STATUSBAR_POPUPCHILDCOMMUNITY_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCallBackDialogNStatusBarPopupChildCommunityProc, NULL, TEXT("NInterface/Interface/INTERFACE_4x4_00_000.SPR"), 0, true, 9, MAP_TILE_XSIZE + pclclient->GetScreenRX() + 114, pclclient->GetScreenYSize() - 25 - 51 - siHeight, siWidth, siHeight, false, false, false );
	}

	for(SI32 i=0; i<NSTATUSBAR_POPUPCHILD_MAXBUTTON; i++)
	{
		if(m_pclButtons[i] == NULL)		continue;

		m_pclButton_Buttons[i]	 = new CButton(this);
		m_pclButton_Buttons[i]->Create(m_pclButtons[i]->m_siControlID, m_pclButtons[i]->m_kText, TEXT("NInterface/Button/BUTTON_4X24_00_000.SPR"), 0, true, 3, 6, 8 + i*20, siWidth - 12, 20, siWidth - 12, 20);
		m_pclButton_Buttons[i]->SetFontColor(RGB(255, 255, 255));
		m_pclButton_Buttons[i]->SetTextPrintOption(DT_VCENTER | DT_LEFT | DT_SINGLELINE);

		if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
		{
			m_pclButton_Buttons[i]->SetFontKind(TEXT("TimeNTales"));
			m_pclButton_Buttons[i]->SetFontHeight(15);
		}
		m_pclButton_Buttons[i]->Refresh();
	}

	SetAlphaMode( 13 );
	SetTopMostWindow( true );
}

void CALLBACK CNStatusBarPopupChildCommunityDlg::StatiCallBackDialogNStatusBarPopupChildCommunityProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNStatusBarPopupChildCommunityDlg *pThis = (CNStatusBarPopupChildCommunityDlg *)pControllerMgr;
	pThis->CallBackDialogNStatusBarPopupChildCommunityProc( nEvent, nControlID, pControl );
	return;
}

void CNStatusBarPopupChildCommunityDlg::CallBackDialogNStatusBarPopupChildCommunityProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient *pclclient = (cltClient *)pclClient;
	CNStatusBarPopupParentDlg* pPopupParent = (CNStatusBarPopupParentDlg*)pclclient->m_pDialog[STATUSBAR_POPUPPARENT_DLG];

	switch(nEvent)
	{
	case EVENT_BUTTON_CLICKED:
		{
			switch(nControlID)
			{
			case STATUSBAR_POPUPCHILDCOMMUNITY_DLG_VILLAGE:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					if ( pclclient->m_pDialog[ NRESIDENTS_DLG ] )
					{
						( ( CNResidentsDlg * )pclclient->m_pDialog[ NRESIDENTS_DLG ] )->Show();
					}			
				}
				break;
			case STATUSBAR_POPUPCHILDCOMMUNITY_DLG_HOMEPAGE:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);
				}
				break;
			case STATUSBAR_POPUPCHILDCOMMUNITY_DLG_PARENT:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					// 스승이 필요한 레벨이면 후견인 창을 보여준다. 
					//KHY - 0610 - 스승제자 S 추가수정. 50레벨 이하, 초보자 리스트 삭제.
					if ( ConstServiceArea_Japan != pclClient->siServiceArea )	
					{
						if(pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < pclClient->siMinimumTeacherLevel)
						{
							pclclient->CreateInterface(NCHILD_DLG);
							//pclclient->CreateInterface(NLOWLEVEL_DLG);
						}
						else
						{	
							pclclient->CreateInterface(NFATHER_DLG);
							pclclient->CreateInterface(NLOWLEVEL_DLG);
						}
					}
					else
					{
						if(pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < FATHER_LEVEL)
						{
							pclclient->CreateInterface(NCHILD_DLG);
							pclclient->CreateInterface(NLOWLEVEL_DLG);
						}
						else
						{	
							pclclient->CreateInterface(NFATHER_DLG);
							pclclient->CreateInterface(NLOWLEVEL_DLG);
						}
					}
				}
				break;
			case STATUSBAR_POPUPCHILDCOMMUNITY_DLG_GUILD:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);
				
					pclclient->CreateInterface(NGUILDMEMBER_DLG);
				}
				break;
			case STATUSBAR_POPUPCHILDCOMMUNITY_DLG_FRIEND:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					if(pclclient->m_pDialog[ NNEWFRIEND_DLG ])
					{	
						((CNNewFriendDlg*)pclclient->m_pDialog[ NNEWFRIEND_DLG ])->Show();
					}
				}
				break;
			case STATUSBAR_POPUPCHILDCOMMUNITY_DLG_PRIVATESHOP:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					cltGameMsgRequest_PersonalShopData clinfo(pclClient->pclCM->CR[1]->GetCharUnique(),FALSE,FALSE);	//  만드는게 아니니까 당연히 FALSE 
					cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);		

					pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_CLICKPERSONALSHOP );
				}
				break;
			case STATUSBAR_POPUPCHILDCOMMUNITY_DLG_MANUFACTURE:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);
					//상점중이라면 열리면 안된다 
					cltCharClient* pChar = pclClient->pclCM->GetCharClient(1)	;
					if(pChar != NULL )
					{
						if( pclClient->IsCountrySwitch( Switch_Change_manufacture_request_interface ) )
						{
							if(pChar->siShopMode == PSHOP_MODE_BUY || pChar->siShopMode == PSHOP_MODE_SELL)
							{
								// 열지 말고 팝업창을 보여준다 .
								TCHAR* pTitle = GetTxtFromMgr( 512 );
								TCHAR* pText = GetTxtFromMgr( 40863 );
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
								
							}
							else
							{
								cltGameMsgRequest_PersonalShopData clinfo(pclClient->pclCM->CR[1]->GetCharUnique(),TRUE, FALSE );	// 기본적으로는 제작의뢰 하는 것이므로 일단 제작자는 아닌 FALSE 
								cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
								pclClient->SendMsgToServer((sPacketHeader*)&clMsg);	
							}
						}
						else
						{
							cltGameMsgRequest_PersonalShopData clinfo(pclClient->pclCM->CR[1]->GetCharUnique(),TRUE, FALSE );	// 기본적으로는 제작의뢰 하는 것이므로 일단 제작자는 아닌 FALSE 
							cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);	
						}
					}						
				}
				break;
			case STATUSBAR_POPUPCHILDCOMMUNITY_DLG_SKILLMASTER:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					pclclient->CreateInterface( NMASTERINFO_DLG );
				}
				break;
			case STATUSBAR_POPUPCHILDCOMMUNITY_DLG_PVPLEAGUE:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					pclclient->CreateInterface( PVPLEAGUE_DLG );
				}
				break;
			case STATUSBAR_POPUPCHILDCOMMUNITY_DLG_FAMILYINFO:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					pclclient->CreateInterface( NFAMILYINFO_DLG );					
				}
				break;
			case STATUSBAR_POPUPCHILDCOMMUNITY_DLG_MYFAMILYINFO:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					pclclient->CreateInterface( NMYFAMILYINFO_DLG_WITH_CHILDREN );
				}
				break;
			case STATUSBAR_POPUPCHILDCOMMUNITY_DLG_REPAIRREQUEST:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

//					pclclient->CreateInterface( NITEMREPAIR_DLG );
					pclclient->CreateInterface( NNEWITEMMAKE_DLG );
					CNewMakeItemDlg* pclNewMakeItemDlg = (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];
					NItemRepairDlg* pclItemRepairDlg = NULL;
					if( pclNewMakeItemDlg )		pclItemRepairDlg = pclNewMakeItemDlg->NewItemRepair;
					if( pclItemRepairDlg )
					{
						cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
						if( pclchar )
						{
							if( pclchar->bRepairShopSwitch == true )
							{
								cltGameMsgRequest_NewItemRepairRequestInfo clinfo(pclchar->GetCharUnique(), NEWITEMREPAIR_INFO_ITEM);
								cltMsg clMsg(GAMEMSG_REQUEST_NEWITEMREPAIRREQUESTINFO, sizeof(clinfo), (BYTE*)&clinfo);
								pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
							}
							else
							{
								pclItemRepairDlg->SetMode( 2 );
								pclNewMakeItemDlg->ShowRepairTab();
							}
						}
					}
				}
				break;
			}
		}
		break;
	}
}

//================================================================================================
// CNStatusBarPopupChildSystemDlg
//================================================================================================
CNStatusBarPopupChildSystemDlg::CNStatusBarPopupChildSystemDlg()
{
	ZeroMemory(m_pclButtons, sizeof(m_pclButtons));
	ZeroMemory(m_pclButton_Buttons, sizeof(m_pclButton_Buttons));
}

CNStatusBarPopupChildSystemDlg::~CNStatusBarPopupChildSystemDlg()
{
	for(SI32 i=0; i<NSTATUSBAR_POPUPCHILD_MAXBUTTON; i++)
	{
		SAFE_DELETE(m_pclButton_Buttons[i]);
	}

	for(SI32 i=0; i<NSTATUSBAR_POPUPCHILD_MAXBUTTON; i++)
	{
		SAFE_DELETE(m_pclButtons[i]);
	}
}

void CNStatusBarPopupChildSystemDlg::Create()
{
	SI32		siWidth = 0;
	SI32		siHeight = 0;

	cltClient *pclclient = (cltClient *)pclClient;

	SI32		siButtonCount = 0;

	NTCHARString128	kText;

	kText = GetTxtFromMgr(8326);
	m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDSYSTEM_DLG_MINIMAP, kText);	siButtonCount++;
	kText = GetTxtFromMgr(8327);
	m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDSYSTEM_DLG_OPTION, kText);	siButtonCount++;
	kText = GetTxtFromMgr(8328);
	m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDSYSTEM_DLG_SCREENSHOT, kText);	siButtonCount++;
	kText = GetTxtFromMgr(8329);
	m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDSYSTEM_DLG_HELP, kText);	siButtonCount++;
	kText = GetTxtFromMgr(8330);
	m_pclButtons[siButtonCount] = new cltNPopupButtonInfo(STATUSBAR_POPUPCHILDSYSTEM_DLG_EXIT, kText);	siButtonCount++;

	siWidth = 100;
	siHeight = siButtonCount * 20 + 16;

	if (g_SoundOnOffDlg.m_bMaxWindowMode)		//전체화면
	{
		CDialogController::Create( STATUSBAR_POPUPCHILDSYSTEM_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCallBackDialogNStatusBarPopupChildSystemProc, NULL, TEXT("NInterface/Interface/INTERFACE_4x4_00_000.SPR"), 0, true, 9, MAP_TILE_XSIZE + pclclient->GetScreenRX() + 114, pclclient->GetScreenYSize() - 25 - 25 - siHeight, siWidth, siHeight, false, false, false );
	}
	else
	{	
		CDialogController::Create( STATUSBAR_POPUPCHILDSYSTEM_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCallBackDialogNStatusBarPopupChildSystemProc, NULL, TEXT("NInterface/Interface/INTERFACE_4x4_00_000.SPR"), 0, true, 9, MAP_TILE_XSIZE + pclclient->GetScreenRX() + 114, pclclient->GetScreenYSize() - 25 - 25 - siHeight, siWidth, siHeight, false, false, false );
	}

	for(SI32 i=0; i<NSTATUSBAR_POPUPCHILD_MAXBUTTON; i++)
	{
		if(m_pclButtons[i] == NULL)		continue;

		m_pclButton_Buttons[i]	 = new CButton(this);
		m_pclButton_Buttons[i]->Create(m_pclButtons[i]->m_siControlID, m_pclButtons[i]->m_kText, TEXT("NInterface/Button/BUTTON_4X24_00_000.SPR"), 0, true, 3, 6, 8 + i*20, siWidth - 12, 20, siWidth - 12, 20);
		m_pclButton_Buttons[i]->SetFontColor(RGB(255, 255, 255));		
		m_pclButton_Buttons[i]->SetTextPrintOption(DT_VCENTER | DT_LEFT | DT_SINGLELINE);
		if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
		{
			m_pclButton_Buttons[i]->SetFontKind(TEXT("TimeNTales"));
			m_pclButton_Buttons[i]->SetFontHeight(15);
		}
		m_pclButton_Buttons[i]->Refresh();
	}

	SetAlphaMode( 13 );
	SetTopMostWindow( true );
}

void CALLBACK CNStatusBarPopupChildSystemDlg::StatiCallBackDialogNStatusBarPopupChildSystemProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNStatusBarPopupChildSystemDlg *pThis = (CNStatusBarPopupChildSystemDlg *)pControllerMgr;
	pThis->CallBackDialogNStatusBarPopupChildSystemProc( nEvent, nControlID, pControl );
	return;
}

void CNStatusBarPopupChildSystemDlg::CallBackDialogNStatusBarPopupChildSystemProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient *pclclient = (cltClient *)pclClient;
	CNStatusBarPopupParentDlg* pPopupParent = (CNStatusBarPopupParentDlg*)pclclient->m_pDialog[STATUSBAR_POPUPPARENT_DLG];

	switch(nEvent)
	{
	case EVENT_BUTTON_CLICKED:
		{
			switch(nControlID)
			{
			case STATUSBAR_POPUPCHILDSYSTEM_DLG_MINIMAP:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					if(pclclient->m_pDialog[ NMINIMAP_DLG])
					{
						if( pclclient->m_pDialog[NMINIMAP_DLG]->IsShow() == false )
							pclclient->m_pDialog[NMINIMAP_DLG]->Show(TRUE);

						((CNMiniMapDlg*)pclclient->m_pDialog[ NMINIMAP_DLG])->
							Minimi( !((CNMiniMapDlg*)pclclient->m_pDialog[ NMINIMAP_DLG])->GetMinimi()   );
					}
					else
						pclclient->CreateInterface( NMINIMAP_DLG );
				}
				break;
			case STATUSBAR_POPUPCHILDSYSTEM_DLG_OPTION:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					if(pclclient->m_pDialog[ NSOUNDONOFF_DLG ] )
					{
						g_SoundOnOffDlg.Hide();
						((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->Show();
					}
					else
					{
						if( pclclient->m_pDialog[ NSOUNDONOFF_DLG ] )
							((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->Hide();
						g_SoundOnOffDlg.Show();
					}
				}
				break;
			case STATUSBAR_POPUPCHILDSYSTEM_DLG_SCREENSHOT:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					pclclient->pclCapture->CaptureToJPG(); 

					TCHAR TempWorkDir[MAX_PATH];
					_tgetcwd( TempWorkDir, MAX_PATH );

					TCHAR buffer[256];
					TCHAR* pText = GetTxtFromMgr(960);


					StringCchPrintf(buffer, 256, pText, 
						TempWorkDir, pclclient->pclCapture->GetFileName());

					TCHAR* pTitle = GetTxtFromMgr(961);

					pclclient->pclMessage->SetMsg(buffer);
				}
				break;
			case STATUSBAR_POPUPCHILDSYSTEM_DLG_HELP:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					pclclient->ShowHelpWindow(TEXT("NULL"));
				}
				break;
			case STATUSBAR_POPUPCHILDSYSTEM_DLG_EXIT:
				{
					if(pPopupParent)		pPopupParent->ShowStatusBarPopupParentDialog(false);

					//cyj 죽어있을때는 부활 다이얼로그 띄운다.
					if (pclclient->pclCM->CR[1]->GetLife() <= 0 && pclclient->bReviveRequestSwitch)
					{
						if (pclclient->m_pDialog[NCOMMONMSG_DLG] == NULL)
						{
							pclclient->CreateInterface( NCOMMONMSG_DLG );
						}
						((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->Set(COMMONDLG_REVIVE);
					}
					else
						pclclient->CreateInterface( NEXIT_DLG );
				}
				break;
			}
		}
		break;
	}
}


//================================================================================================
// CNStatusBarDlg
//================================================================================================
CNStatusBarDlg::CNStatusBarDlg() : CDialogController()
{
	m_szVersionAndServerName[0] = NULL;

	SI16 i = 0;

	m_pChatStatusBarBuffer = new CChatStatusBarBuffer();
	m_strChatData = new TCHAR[ MAX_CHAT_DATA_LENGTH ];

	m_bOpened				= false;
	m_pButton_Menu			= NULL;
	m_pStatic_ServerName	= NULL;
}

CNStatusBarDlg::~CNStatusBarDlg()
{
	SI16 i = 0;

	SAFE_DELETE( m_pButton_Menu );
	SAFE_DELETE( m_pStatic_ServerName );
	SAFE_DELETE( m_pChatStatusBarBuffer );
	SAFE_DELETE_A( m_strChatData );

	SAFE_DELETE( m_pUrlLink );
}

void CNStatusBarDlg::Create()
{	
	cltClient *pclclient = (cltClient *)pclClient;
	
	if (g_SoundOnOffDlg.m_bMaxWindowMode)		//전체화면
	{
		CDialogController::Create( STATUSBAR_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCallBackDialogNStatusBarProc, NULL, TEXT("NInterface/Interface/Interface_1024x50_Statusbar.SPR"), 1, true, 3, MAP_TILE_XSIZE + pclclient->GetScreenRX(), pclclient->GetScreenYSize() - 25, 1024, 25, false, false, false );
	}
	else
	{	
		//if(gsiResolution ==4) // 1024x600
		//	CDialogController::Create( STATUSBAR_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCallBackDialogNStatusBarProc, NULL, TEXT("NInterface/Interface/Interface_960x50_Statusbar.SPR"), 1, true, 3, MAP_TILE_XSIZE + pclclient->GetScreenRX(), pclclient->GetScreenYSize() - 25, 960, 15, false, false, false );
		//else
			CDialogController::Create( STATUSBAR_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCallBackDialogNStatusBarProc, NULL, TEXT("NInterface/Interface/Interface_960x50_Statusbar.SPR"), 1, true, 3, MAP_TILE_XSIZE + pclclient->GetScreenRX(), pclclient->GetScreenYSize() - 25, 960, 25, false, false, false );
	}

	m_pButton_Menu	= new CButton(this);
	m_pStatic_ServerName = new CStatic(this);

	//if(gsiResolution ==4) // 1024x600
	//	m_pButton_Menu->Create(STATUSBAR_DLG_BUTTON_MENU, TEXT(""), TEXT("NInterface\\Button\\BUTTON_114x23_00_000.SPR"), 0, false, 0, 1, -9, 114, 23, 114, 23);
	//else
		m_pButton_Menu->Create(STATUSBAR_DLG_BUTTON_MENU, TEXT(""), TEXT("NInterface\\Button\\BUTTON_114x23_00_000.SPR"), 0, false, 0, 1, 1, 114, 23, 114, 23);

	if (g_SoundOnOffDlg.m_bMaxWindowMode)
	{
		m_pStatic_ServerName->Create(STATUSBAR_DLG_STATIC_SERVERNAME, TEXT(""), TEXT(""), 0, false, 0, 905, 7, 115, 18, 115, 18);
	}
	else
	{
		//if(gsiResolution ==4) // 1024x600
		//	m_pStatic_ServerName->Create(STATUSBAR_DLG_STATIC_SERVERNAME, TEXT(""), TEXT(""), 0, false, 0, 845, -3, 115, 18, 115, 18);
		//else
			m_pStatic_ServerName->Create(STATUSBAR_DLG_STATIC_SERVERNAME, TEXT(""), TEXT(""), 0, false, 0, 845, 7, 115, 18, 115, 18);
	}
	if ( pclClient->IsWhereServiceArea(ConstServiceArea_English | ConstServiceArea_NHNChina | ConstServiceArea_EUROPE) )
	{
		m_pStatic_ServerName->SetFontKind(TEXT("Tahoma"));
		m_pStatic_ServerName->SetFontHeight( 12 );
	}
	else
	{
		m_pStatic_ServerName->SetFontHeight( 11 );
	}
	m_pStatic_ServerName->SetTextPrintOption(DT_TOP | DT_CENTER | DT_SINGLELINE);
	m_pStatic_ServerName->SetFontColor(RGB(255, 255, 255));

	NEWCBUTTON( m_pUrlLink );
	//if(gsiResolution ==4) // 1024x600
	//	m_pUrlLink->Create( STATUSBAR_DIALOG_BUTTON_URLLINK, NULL, TEXT(""), 0, 0, 0, 115, -10, 1024 - 115, 15, 1024 - 115, 25  );
	//else
		m_pUrlLink->Create( STATUSBAR_DIALOG_BUTTON_URLLINK, NULL, TEXT(""), 0, 0, 0, 115, 0, 1024 - 115, 25, 1024 - 115, 25  );

	SetTopMostWindow( true );
	SetTimerOnOff( true );

	return;
}

void CALLBACK CNStatusBarDlg::StatiCallBackDialogNStatusBarProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNStatusBarDlg *pThis = (CNStatusBarDlg *)pControllerMgr;
	pThis->CallBackDialogNStatusBarProc( nEvent, nControlID, pControl );
	return;
}

void CALLBACK CNStatusBarDlg::CallBackDialogNStatusBarProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient* pclclient = (cltClient*)pclClient;

	if ( nEvent == EVENT_BUTTON_CLICKED )
	{

		//  딜레이 줌
		static SI32 siOldClock =  0;
		if( pclClient->CurrentClock - siOldClock < 1000 )
		{
			return;
		}       

		siOldClock = pclClient->CurrentClock;
		

		if ( g_PromptMgr.HaveUrl() )
		{
			TCHAR*  pszURL = g_PromptMgr.GetURL();
			/*
			cltClient *pclclient = (cltClient*)pclClient;
			
			cltGameMsgRequest_LoginPage_URL clinfo( pclclient->szID, pclclient->szPassword, pText );
			cltMsg clMsg(GAMEMSG_REQUEST_LOGINPAGE_URL,sizeof(clinfo),(BYTE*)&clinfo);
			pclclient->SendMsgToServer( (sPacketHeader*)&clMsg);
			*/
			g_LoginNoticeDlg.Set(pszURL, 692, 390);
		}
	}

	switch(nEvent)
	{
	case EVENT_BUTTON_CLICKED:
		{
			switch(nControlID)
			{
			case STATUSBAR_DLG_BUTTON_MENU:
				{
					if(pclclient->m_pDialog[STATUSBAR_POPUPPARENT_DLG])
					{
						CNStatusBarPopupParentDlg* pPopupParent = (CNStatusBarPopupParentDlg*)pclclient->m_pDialog[STATUSBAR_POPUPPARENT_DLG];
						if(pclclient->m_pDialog[STATUSBAR_POPUPPARENT_DLG]->IsShow())
						{
							pPopupParent->ShowStatusBarPopupParentDialog(false);
						}
						else
						{
							pPopupParent->ShowStatusBarPopupParentDialog(true);
						}
					}
				}
				break;
			}
		}
		break;
	}

	return;
}

void CNStatusBarDlg::SetData( void *pBuffer )
{
	
}

void CNStatusBarDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient *pclclient = ( cltClient * )pclClient;

	if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
	{	
		if ( g_PromptMgr.GetPromptNum() > 0 )
		{	
			g_PromptMgr.Draw( 155 + 10 + ScreenRX, pclclient->GetScreenYSize() - 17 + ScreenRY );
		}
	}
	else  //전체화면 
	{	
		if ( g_PromptMgr.GetPromptNum() > 0 )   // 1024(+타일하나크기+RX) * 768(+타일하나크기+RY) 
		{
			g_PromptMgr.Draw( 155 + 40 + ScreenRX, 622 + ScreenRY );
		}
	}

	TSpr *pVersionSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( TEXT("Ninterface/interface/Interface_106x24_Statusbar_version.SPR") );
	if( NULL!= pVersionSpr )
	{
		//GP.PutSpr( pVersionSpr, GetX()+ GetWidth() - pVersionSpr->GetXSize() + pclclient->GetScreenRX()  , GetY() + GetHeight() - pVersionSpr->GetYSize() + pclclient->GetScreenRY() ,0 );
	}

 //  	if(pclclient->m_pInterfaceMgr->GetToggleState() == IMEUI_STATE_ON )    
	//{
	//	TSpr *pImeSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( TEXT("Ninterface/common/common_18x18_00_000.SPR") );
	//	if( NULL != pImeSpr )
	//	{
	//		GP.PutSpr( pImeSpr, GetX()+ GetWidth() - pVersionSpr->GetXSize() + pclclient->GetScreenRX() +2 , GetY() + GetHeight() - pVersionSpr->GetYSize() + pclclient->GetScreenRY() + 5 ,0 );
	//	}
	//}
	//else
	//{
	//	TSpr *pImeSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( TEXT("Ninterface/common/common_18x18_00_000.SPR") );
	//	if( NULL != pImeSpr )
	//	{
	//		GP.PutSpr( pImeSpr, GetX()+ GetWidth() - pVersionSpr->GetXSize() + pclclient->GetScreenRX() +2 , GetY() + GetHeight() - pVersionSpr->GetYSize() + pclclient->GetScreenRY() + 5 ,1 );
	//	}
	//}
	
/*	HDC hdc;	
	if(pclclient->lpBackScreen->GetDC(&hdc) == DD_OK)
	{
		SetBkMode(hdc, TRANSPARENT);


		HFONT hFont;
		if ( pclClient->siServiceArea == ConstServiceArea_English )
		{
			hFont = g_FontManager.SetFont( 12, TEXT("Tahoma") );
		}
		else
		{
			hFont = g_FontManager.SetFont( 11 );
		}

		HFONT hOldFont = NULL;
		if ( hFont != NULL )
		{
			hOldFont = (HFONT)SelectObject( hdc, hFont );
		}		

		SetTextAlign( hdc, TA_CENTER ); 
 		SetTextColor(hdc,RGB(255,255,255));
   		TextOut(hdc,GetX()+ GetWidth() - pVersionSpr->GetXSize() + pclclient->GetScreenRX() + 63 ,GetY() + GetHeight() - pVersionSpr->GetYSize() + pclclient->GetScreenRY() + 8,m_szVersionAndServerName,lstrlen(m_szVersionAndServerName));
		

		if ( hOldFont )
		{
			SelectObject( hdc, hOldFont );			
		}			
		pclclient->lpBackScreen->ReleaseDC(hdc);
	}
	///////// 텍스트 출력 끝*/

	return;
}

void CNStatusBarDlg::CharacterLogin()
{
	
	return;
}

void CNStatusBarDlg::ChatDataSend()
{
	
}

void CNStatusBarDlg::Push( SI16 ChatDataKind, TCHAR *ChatText )
{
	
}

bool CNStatusBarDlg::Pop( stChatStatusBarData * pChatStatusBarData )
{
	memset( pChatStatusBarData, 0, sizeof( stChatStatusBarData ) );

	if ( m_pChatStatusBarBuffer->Pop( pChatStatusBarData ) )
	{
		return true;
	}

	return false;
}

void CNStatusBarDlg::SetInputEditBoxFocus()
{
	
	return;
}
void CNStatusBarDlg::SmallShow()
{
	return;
}

void CNStatusBarDlg::BigShow()
{
	return;
}

bool CNStatusBarDlg::CheckData( TCHAR *PlayerName, TCHAR *ChatText )
{
	
	return TRUE;
}

void CNStatusBarDlg::ChatRecv( TCHAR *PlayerName, TCHAR *ChatText, SI32 Kind )
{	
	
	return;
}

void CNStatusBarDlg::MasterNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
{
	
	return;
}

void CNStatusBarDlg::VillageNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
{
	
	return;
}

void CNStatusBarDlg::ResidentsNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
{
	
	return;
}

void CNStatusBarDlg::KingNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
{
	
	return;
}

void CNStatusBarDlg::GoodsProductionNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText )
{
	
	return;
}

void CNStatusBarDlg::SetOpenedChatDlg( bool Open )
{
	
	return;
} 




void CNStatusBarDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	
	return;
}

void CNStatusBarDlg::ChatMessage(SI16 chatmode, SI32 systemtxt, TCHAR* pmsg)
{
	
}

SI32 CNStatusBarDlg::CompareKey(TCHAR *data, SI32 type)
{
	switch(type)
	{
	case 0:
		{
			if(_tcsncmp(data,TEXT("@"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_RESIDENTSCHAT;
			}
			else if(_tcsncmp(data,TEXT("#"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_GUILDCHAT;
			}
			else if(_tcsncmp(data,TEXT("$"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_PARTYCHAT;
			}
			else
				return TAB_MODE_DEAULT;
		}
		break;

	case 1:
		{
			if(_tcsncmp(data,TEXT("!"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_DEAULT;
			}
			else if(_tcsncmp(data,TEXT("#"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_GUILDCHAT;
			}
			else if(_tcsncmp(data,TEXT("$"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_PARTYCHAT;
			}
			else
				return TAB_MODE_RESIDENTSCHAT;
		}
		break;

	case 2:
		{
			if(_tcsncmp(data,TEXT("!"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_DEAULT;
			}
			else if(_tcsncmp(data,TEXT("@"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_RESIDENTSCHAT;
			}
			else if(_tcsncmp(data,TEXT("$"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_PARTYCHAT;
			}
			else
				return TAB_MODE_GUILDCHAT;
		}
		break;
	case 3:
		{
			if(_tcsncmp(data,TEXT("!"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_DEAULT;
			}
			else if(_tcsncmp(data,TEXT("@"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_RESIDENTSCHAT;
			}
			else if(_tcsncmp(data,TEXT("#"),1)==0)
			{
				MStrCpy(m_strChatData, &data[1], MAX_CHAT_DATA_LENGTH);
				return TAB_MODE_GUILDCHAT;
			}
			else
				return TAB_MODE_PARTYCHAT;
		}
		break;
	}
	return TAB_MODE_DEAULT;
}

void CNStatusBarDlg::SetVersionAndServerName(TCHAR * VersionAndServerName)
{
	if( VersionAndServerName  == NULL) return;

	MStrCpy(m_szVersionAndServerName,VersionAndServerName,256);	

	m_pStatic_ServerName->SetText(VersionAndServerName);
}
