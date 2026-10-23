//---------------------------------
// 2005/03/07 성준엽
//---------------------------------
#include "NQuestDlg.h"

#include "Client.h"

#include "Char/CharCommon/Char-Common.h"
#include "Char/CharManager/CharManager.h"

#include "../../../resource.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-Quest.h"

#include "Music/Music.h"
#include "Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../../Common/SystemNPC/SystemNPC.h"

//------------------------------------
// Lib
//------------------------------------
#include "HashTable/HashTable_StringInt.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

extern cltCommonLogic* pclClient;

//--------------------------------------------
// cltNQuestDlg
//--------------------------------------------
cltNQuestDlg::cltNQuestDlg()
{
	m_pPortraitStatic=NULL;
	m_pConditionEdit=NULL;
	m_pQuestRewardEdit=NULL;
	m_pStatusEdit=NULL;
	m_pExplainEdit=NULL;
	m_pSprStatic=NULL;
	m_pAcceptButton=NULL;
	m_pCancelButton=NULL;
	m_pOKButton=NULL;
	m_pCondition=NULL;
	m_pQuestReward=NULL;
	m_pStatus=NULL;
}

void cltNQuestDlg::Create()
{
	CInterfaceFile file;

	m_pPortraitStatic = new CImageStatic( this );

	m_pConditionEdit = new CEdit( this );
	m_pQuestRewardEdit = new CEdit( this );
	m_pStatusEdit = new CEdit( this );
	m_pExplainEdit = new CEdit( this );

	m_pSprStatic = new CImageStatic( this ) ;

	m_pAcceptButton = new CButton( this );
	m_pCancelButton = new CButton( this );
	m_pOKButton = new CButton( this );

	m_pCondition = new CStatic( this ) ;
	m_pQuestReward = new CStatic( this ) ;
	m_pStatus = new CStatic( this ) ;

	file.LoadFile(TEXT("NInterface/Data/DLG_NQuest.ddf"));
	file.CreatePopupDialog(this, NQUEST_DLG, TEXT("dialog_QUEST"), StaticDlgProc);

	file.CreateControl(m_pPortraitStatic,NQUEST_DIALOG_PORTRAIT_STATIC,TEXT("imagestatic_PORTRAIT"));
	m_pPortraitStatic->SetBorder(true);

	file.CreateControl( m_pConditionEdit, NQUEST_DIALOG_CONDITION_EDIT, TEXT("editbox_CONDITION"));
	m_pConditionEdit->SetBorder(true);
	file.CreateControl( m_pQuestRewardEdit, NQUEST_DIALOG_QUESTREWARD_EDIT, TEXT("editbox_QUESTREWARD"));
	m_pQuestRewardEdit->SetBorder(true);
	file.CreateControl( m_pStatusEdit, NQUEST_DIALOG_STATUS_EDIT, TEXT("editbox_STATUS"));
	m_pStatusEdit->SetBorder(true);
	file.CreateControl( m_pExplainEdit, NQUEST_DIALOG_EXPLAIN_EDIT, TEXT("editbox_EXPLAIN"));
	m_pExplainEdit->SetBorder(true);
	m_pExplainEdit->SetMaxEditTextLength(1024) ;

	file.CreateControl( m_pSprStatic, NQUEST_DIALOG_SPR_STATIC, TEXT("imagestatic_EXPLAINSPR"));
	m_pSprStatic->SetBorder(true);

	file.CreateControl( m_pAcceptButton, NQUEST_DIALOG_ACCEPT_BUTTON, TEXT("button_ACCEPT"));
	file.CreateControl( m_pCancelButton, NQUEST_DIALOG_CANCEL_BUTTON, TEXT("button_CANCELQUEST"));
	file.CreateControl( m_pOKButton, NQUEST_DIALOG_OK_BUTTON, TEXT("button_OK"));

	file.CreateControl( m_pCondition, NQUEST_DIALOG_CONDITION, TEXT("statictext_CONDITION"));
	file.CreateControl( m_pQuestReward, NQUEST_DIALOG_QUESTREWARD, TEXT("statictext_QUESTREWARD"));
	file.CreateControl( m_pStatus, NQUEST_DIALOG_STATUS, TEXT("statictext_STATUS"));

	Hide();
}

cltNQuestDlg::~cltNQuestDlg()
{
	if ( m_pStatus )
	{
		delete m_pStatus ;
		m_pStatus = NULL ;
	}
	if ( m_pQuestReward )
	{
		delete m_pQuestReward ;
		m_pQuestReward = NULL ;
	}
	if ( m_pCondition )
	{
		delete m_pCondition ;
		m_pCondition = NULL ;
	}
	if ( m_pOKButton )
	{
		delete m_pOKButton;
		m_pOKButton=NULL;
	}
	if ( m_pCancelButton )
	{
		delete m_pCancelButton;
		m_pCancelButton=NULL;
	}
	if ( m_pAcceptButton )
	{
		delete m_pAcceptButton;
		m_pAcceptButton=NULL;
	}
	if ( m_pSprStatic )
	{
		delete m_pSprStatic;
		m_pSprStatic=NULL;
	}
	if ( m_pExplainEdit )
	{
		delete m_pExplainEdit;
		m_pExplainEdit=NULL;
	}
	if ( m_pStatusEdit )
	{
		delete m_pStatusEdit;
		m_pStatusEdit=NULL;
	}
	if ( m_pQuestRewardEdit )
	{
		delete m_pQuestRewardEdit;
		m_pQuestRewardEdit=NULL;
	}
	if ( m_pConditionEdit )
	{
		delete m_pConditionEdit;
		m_pConditionEdit=NULL;
	}
	if ( m_pPortraitStatic )
	{
		delete m_pPortraitStatic;
		m_pPortraitStatic=NULL;
	}
}

void cltNQuestDlg::Show(SI32 kind)
{
	pclClient->pclQuestManager->InitDialog(kind);

	if( IsShow() )
	{
		Hide();
		return;
	}

	SetTimerOnOff( true );

	CDialogController::Show( true );

	return;
}

void cltNQuestDlg::Hide()
{
	SetTimerOnOff( false );

	CDialogController::Show( false );

	return;
}

void cltNQuestDlg::Action()
{
	pclClient->pclQuestManager->Action();
}

void CALLBACK cltNQuestDlg::StaticDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	cltNQuestDlg *pThis = (cltNQuestDlg*) pControllerMgr;
	pThis->DlgProc( nEvent, nControlID, pControl );
}

void CALLBACK cltNQuestDlg::DlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;
	TCHAR Nbuffer[512]=TEXT(" ");


	switch(nControlID)
	{
	case NQUEST_DIALOG_CANCEL_BUTTON:
		{
			if(pclClient->pclQuestManager->siCurrentQuestUnique)
			{
				cltGameMsgRequest_CancelQuest clInfo(pclClient->pclQuestManager->siQuestType,
					pclClient->pclQuestManager->siCurrentQuestUnique);
				cltMsg clMsg(GAMEMSG_REQUEST_CANCELQUEST, sizeof(clInfo), (BYTE*)&clInfo);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;

	case NQUEST_DIALOG_ACCEPT_BUTTON:
		{
			// 게임 서버로 퀘스트의 수락 요청을 보낸다. 
			if(pclClient->pclQuestManager->siCurrentQuestUnique)
			{
				//-----------------------------
				// 자격이 있는지 확인한다. 
				//-----------------------------
				cltQuestInfo* pclquestinfo = pclClient->pclQuestManager->GetQuestInfo(pclClient->pclQuestManager->siCurrentQuestUnique);

				// 레벨 확인. 
				if(pclquestinfo->clInitConditionInfo.siLevel)
				{
					if(pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < pclquestinfo->clInitConditionInfo.siLevel)
					{
						TCHAR* pTitle = GetTxtFromMgr(3087);
						TCHAR* pText = GetTxtFromMgr(3088);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, pclquestinfo->clInitConditionInfo.siLevel);
						break;
					}
				}
				// 기술 확인. 
				SI32 skillunique = pclquestinfo->clInitConditionInfo.siSkillUnique;
				if(skillunique)
				{
					if(pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0]) < pclquestinfo->clInitConditionInfo.siSkillLevel)
					{
						TCHAR skillname[128];
						if(pclClient->pclSkillManager->GetSkillName(skillunique, skillname) == true)
						{
							TCHAR* pTitle = GetTxtFromMgr(3087);
							TCHAR* pText = GetTxtFromMgr(3089);
							TCHAR level[128]= TEXT(" ") ;

							StringCchPrintf(level, 128, TEXT("%d"),pclquestinfo->clInitConditionInfo.siSkillLevel);
							Nsprintf(Nbuffer,pText,TEXT("what"), skillname   ,TEXT("level"),  level  ,NULL);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,Nbuffer);
							//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, skillname, pclquestinfo->clInitConditionInfo.siSkillLevel);
						}
						break;
					}
				}

				cltGameMsgRequest_QuestGoing clInfo(pclClient->pclQuestManager->siQuestType,
					pclClient->pclQuestManager->siCurrentQuestUnique, QUEST_STEP_MIDDLE);
				cltMsg clMsg(GAMEMSG_REQUEST_SETQUESTGOING, sizeof(clInfo), (BYTE*)&clInfo);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;

	case NQUEST_DIALOG_OK_BUTTON:
		// 현재 퀘스트가 완료된 것이라면 다음 퀘스트를 요청한다. 
		if(pclClient->pclQuestManager->siCurrentQuestUnique)
		{
			if(pclClient->pclQuestManager->siCurrentQuestStep == QUEST_STEP_CLEAR)
			{
				cltGameMsgRequest_QuestGoing clInfo(pclClient->pclQuestManager->siQuestType,
					pclClient->pclQuestManager->siCurrentQuestUnique, QUEST_STEP_NEXTQUEST);
				cltMsg clMsg(GAMEMSG_REQUEST_SETQUESTGOING, sizeof(clInfo), (BYTE*)&clInfo);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
		}
		break;
	}
}
