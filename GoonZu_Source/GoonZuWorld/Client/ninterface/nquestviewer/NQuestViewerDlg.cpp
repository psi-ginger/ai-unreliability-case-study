#include "NQuestViewerDlg.h"
#include "../Client/NInterface/NNewQuestDlg/NNewQuestDlg.h"

#include "Client.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"
#include "../../../common/Skill/Skill-Manager.h"
#include "../Client/NInterface/NQuestDlg/NQuestDlg.h"
#include "Msg/MsgType-Quest.h"

#include "../../InterfaceMgr/Interface/OutLine.h"
#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/interface/Button.h"
#include "../../InterfaceMgr/interface/RadioButton.h"
#include "../../InterfaceMgr/interface/Edit.h"
#include "../../InterfaceMgr/interface/ComboBox.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"


extern cltCommonLogic* pclClient;


CNQuestViewerDlg::CNQuestViewerDlg()
{
	m_siNpcKind = 0;
	m_sirealNpcKind=0;
	m_siButtonType=0;
	m_siQuestUnique =0;
	m_bOpened= FALSE;
	m_pstatictext_SmallQuestStatic1 = NULL;
	m_pstatictext_SmallQuestStatic2 = NULL;
	m_pradiobutton_QuestViewer1 = NULL;
	m_pradiobutton_QuestViewer2 = NULL;
	m_pradiobutton_QuestViewer3 = NULL;
	m_pradiobutton_QuestViewer4 = NULL;
	m_pradiobutton_QuestViewer5 = NULL;
	m_pbutton_QuestViewer1 = NULL;
	m_pbutton_QuestViewer2 = NULL;
	m_pbutton_QuestViewer3 = NULL;
	m_pbutton_QuestViewer4 = NULL;
	m_peditbox_QuestViewer = NULL;
	m_poutline_QuestViewer = NULL;
	m_pCompobox_QuestViewer= NULL;
}

CNQuestViewerDlg::~CNQuestViewerDlg()
{

	SAFE_DELETE(m_pstatictext_SmallQuestStatic1 );
	SAFE_DELETE(m_pstatictext_SmallQuestStatic2);
	SAFE_DELETE(m_pradiobutton_QuestViewer1);
	SAFE_DELETE(m_pradiobutton_QuestViewer2);
	SAFE_DELETE(m_pradiobutton_QuestViewer3);
	SAFE_DELETE(m_pradiobutton_QuestViewer4);
	SAFE_DELETE(m_pradiobutton_QuestViewer5);
	SAFE_DELETE(m_pbutton_QuestViewer1);
	SAFE_DELETE(m_pbutton_QuestViewer2);	
	SAFE_DELETE(m_peditbox_QuestViewer);
	SAFE_DELETE(m_poutline_QuestViewer);
	SAFE_DELETE(m_pCompobox_QuestViewer);
	SAFE_DELETE(m_pbutton_QuestViewer3);
	SAFE_DELETE(m_pbutton_QuestViewer4);
}

void CNQuestViewerDlg::Create()
{
	cltClient* pclclient = (cltClient*)pclClient;
	
	if(!IsCreate())
	{
		
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NQuestViewer/DLG_NQuestViewer.ddf"));
		file.CreatePopupDialog( this, QUESTVIEWER_DLG, TEXT("dialog_QuestViewer"), StatiCNQuestViewerDlgProc);		

		m_pstatictext_SmallQuestStatic1 = new CStatic(this);
		m_pstatictext_SmallQuestStatic2 = new CStatic(this);
		m_pradiobutton_QuestViewer1 = new CRadioButton(this);
		m_pradiobutton_QuestViewer2 = new CRadioButton(this);
		m_pradiobutton_QuestViewer3 = new CRadioButton(this);
		m_pradiobutton_QuestViewer4 = new CRadioButton(this);
		m_pradiobutton_QuestViewer5 = new CRadioButton(this);
		m_pbutton_QuestViewer1 = new CButton(this);
		m_pbutton_QuestViewer2 = new CButton(this);		
		m_pbutton_QuestViewer3 = new CButton(this);		
		m_pbutton_QuestViewer4 = new CButton(this);	
		m_peditbox_QuestViewer = new CEdit(this);
		m_poutline_QuestViewer = new COutLine(this);  
		m_pCompobox_QuestViewer = new CComboBox(this);


		file.CreateControl( m_pstatictext_SmallQuestStatic1     ,  STATICTEXT_SMALLQUESTSTATIC1,   TEXT("statictext_SmallQuestStatic1") );
		file.CreateControl( m_pstatictext_SmallQuestStatic2      ,  STATICTEXT_SMALLQUESTSTATIC2,  TEXT("statictext_SmallQuestStatic2")   );
		file.CreateControl( m_pradiobutton_QuestViewer1      ,  RADIOBUTTON_QUESTVIEWER1  ,   TEXT("radiobutton_QuestViewer1")     );
		file.CreateControl( m_pradiobutton_QuestViewer2     ,  RADIOBUTTON_QUESTVIEWER2  ,   TEXT("radiobutton_QuestViewer2")      );
		file.CreateControl( m_pradiobutton_QuestViewer3    ,  RADIOBUTTON_QUESTVIEWER3 ,   TEXT("radiobutton_QuestViewer3")        );
		file.CreateControl( m_pradiobutton_QuestViewer4    ,  RADIOBUTTON_QUESTVIEWER4 ,   TEXT("radiobutton_QuestViewer4")        );
		file.CreateControl( m_pradiobutton_QuestViewer5    ,  RADIOBUTTON_QUESTVIEWER5 ,   TEXT("radiobutton_QuestViewer5")        );
		file.CreateControl( m_pbutton_QuestViewer1    ,  BUTTON_QUESTVIEWER1 ,   TEXT("button_QuestViewer1")         );
		file.CreateControl( m_pbutton_QuestViewer2      ,  BUTTON_QUESTVIEWER2 ,   TEXT("button_QuestViewer2")       );	 
		file.CreateControl( m_pbutton_QuestViewer3      ,  BUTTON_QUESTVIEWER3 ,   TEXT("button_QuestViewer3")       );
		file.CreateControl( m_pbutton_QuestViewer4      ,  BUTTON_QUESTVIEWER4 ,   TEXT("button_QuestViewer4")       );
		file.CreateControl( m_peditbox_QuestViewer      ,  EDITBOX_QUESTVIEWER,   TEXT("editbox_QuestViewer")        );
		file.CreateControl( m_poutline_QuestViewer     ,  OUTLINE_QUESTVIEWER,   TEXT("outline_QuestViewer")       );
		file.CreateControl( m_pCompobox_QuestViewer    ,  COMBOBOX_QUESTVIEWER,   TEXT("combobox_QuestViewer") );
		InitCombo();

		if(pclclient->m_pDialog[ NNEWQUEST_DLG ])
			(pclclient->m_pDialog[ NNEWQUEST_DLG ])->DeleteDialog();	

		
		
	}


	//Hide();
}

void CALLBACK CNQuestViewerDlg::StatiCNQuestViewerDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNQuestViewerDlg *pThis = (CNQuestViewerDlg*) pControllerMgr;
	pThis->NQuestViewerDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNQuestViewerDlg::NQuestViewerDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
		
	cltClient* pclclient = (cltClient*)pclClient;
	
	switch( nControlID )
	{
	case BUTTON_QUESTVIEWER1:
		{		

            
			SI32 QuestUnique = _tstoi(m_peditbox_QuestViewer->GetText());
			SI32 QuestStep= QUEST_STEP_GREETING;
				
			if(pclclient->m_pDialog[ NNEWQUEST_DLG ])
			{
				pclclient->m_pDialog[ NNEWQUEST_DLG ]->Show(true);
                m_bOpened=true;			
				(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[0])->SetChangeTextDrawMode(false);
				(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[1])->SetChangeTextDrawMode(false);				
				(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditConversation)->SetChangeTextDrawMode(true);

			}			
			else
			{
                pclclient->CreateInterface(NNEWQUEST_DLG);
                m_bOpened=true;						
				(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[0])->SetChangeTextDrawMode(false);
				(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[1])->SetChangeTextDrawMode(false);
				(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditConversation)->SetChangeTextDrawMode(true);

			}
			           
				
			((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->SetTimerOnOff(false);
			pclClient->pclQuestManager->siViewState=1;
			if( m_pradiobutton_QuestViewer4->IsCheck())
			{
				pclClient->pclQuestManager->siViewState=0;
				ShowQuestView(QuestUnique,QUEST_STEP_CLEAR );				
				QuestStep = QUEST_STEP_CLEAR;
			}	
			else if( m_pradiobutton_QuestViewer5->IsCheck())
			{
					pclClient->pclQuestManager->siViewState=0;
					ShowQuestView(QuestUnique,QUEST_STEP_GREETING );				
					QuestStep = QUEST_STEP_GREETING;
			}		
			else if( m_pradiobutton_QuestViewer1->IsCheck())
			{				
				ShowQuestView(QuestUnique,QUEST_STEP_GREETING );
				QuestStep = QUEST_STEP_GREETING;
			}
			else if(m_pradiobutton_QuestViewer2->IsCheck())
			{				
				ShowQuestView(QuestUnique,QUEST_STEP_MIDDLE );
				QuestStep = QUEST_STEP_MIDDLE;
			}
			else if(m_pradiobutton_QuestViewer3->IsCheck())
			{				
				ShowQuestView(QuestUnique,QUEST_STEP_CLEAR );
				QuestStep = QUEST_STEP_CLEAR;
			}
			CNNewQuestDlg* NewQuestDlg = (CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ];

			if(pclClient->pclQuestManager->siViewState == 0 || pclClient->pclQuestManager->siViewState == 2)
			{					
				NewQuestDlg->SetFontIndex( 1 );
				NewQuestDlg->SetBKTileImgNum( 0 );

				NewQuestDlg->m_pStatic[0]->Show(false);
				NewQuestDlg->m_pStatic[1]->Show(false);
				NewQuestDlg->m_pStatic[2]->Show(false);
				NewQuestDlg->m_pStatic[4]->Show(false);
				NewQuestDlg->m_pStatic[5]->Show(false);
				NewQuestDlg->m_pStatic[6]->Show(false);
				NewQuestDlg->m_pStatic[3]->Show(true);

				NewQuestDlg->m_pEditCondition->Show(false);
				NewQuestDlg->m_pEditStatus->Show(false);
				NewQuestDlg->m_pEditQuestReward->Show(false);
				NewQuestDlg->m_pEditExplain[0]->Show(false);
				NewQuestDlg->m_pEditExplain[1]->Show(false);
			}
			else
			{
				NewQuestDlg->SetFontIndex( 0 );
				NewQuestDlg->SetBKTileImgNum( 0 );

				NewQuestDlg->m_pStatic[0]->Show(true);
				NewQuestDlg->m_pStatic[1]->Show(true);
				NewQuestDlg->m_pStatic[2]->Show(true);
				NewQuestDlg->m_pStatic[4]->Show(true);
				NewQuestDlg->m_pStatic[5]->Show(true);
				NewQuestDlg->m_pStatic[6]->Show(true);
				NewQuestDlg->m_pStatic[3]->Show(false);
				NewQuestDlg->m_pEditConversation->Show(false);
			}

			ShowOtherThings( QuestUnique, QuestStep);

			
		}
		break;
	case BUTTON_QUESTVIEWER2:
		{
			if(pclclient->m_pDialog[ NNEWQUEST_DLG ]==NULL)
				return;
			CNNewQuestDlg* NewQuestDlg = ((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ]);				
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditConversation)->SetText(TEXT(" "));
			(NewQuestDlg->m_pEditQuestReward)->SetText(TEXT(""));
			(NewQuestDlg->m_pEditCondition)->SetText(TEXT(""));
			(NewQuestDlg->m_pEditStatus)->SetText(TEXT(""));			
			(NewQuestDlg->m_pEditExplain[1])->SetText(TEXT(""));
			(NewQuestDlg->m_pEditExplain[0])->SetText(TEXT(""));
			(NewQuestDlg->m_pStatic[3])->SetText(TEXT(""));
			(NewQuestDlg->m_pStatic[4])->SetFileName(TEXT(""));
			(NewQuestDlg->m_pStatic[5])->Show(false);			    
			pclClient->pclQuestManager->siStartConversationCounter=0;
			pclClient->pclQuestManager->siEndConversationCounter=0;

			

		}
		break;
	case BUTTON_QUESTVIEWER3:
		{	
			if(m_pradiobutton_QuestViewer5->IsCheck())
			{			
				
				pclClient->pclQuestManager->siStartConversationCounter++;	
				SI32 QuestUnique = _tstoi(m_peditbox_QuestViewer->GetText());
				SI32 QuestStep= QUEST_STEP_GREETING;
				pclclient->pclQuestManager->LoadQuestText();          
				ShowQuestView(QuestUnique,QUEST_STEP_GREETING );		
				ShowOtherThings( QuestUnique, QuestStep);
			}
			else
			{			
				pclClient->pclQuestManager->siEndConversationCounter++;
				SI32 QuestUnique = _tstoi(m_peditbox_QuestViewer->GetText());
				SI32 QuestStep= QUEST_STEP_CLEAR;
				pclclient->pclQuestManager->LoadQuestText();          
				ShowQuestView(QuestUnique,QUEST_STEP_CLEAR );		
				ShowOtherThings( QuestUnique, QuestStep);
			}
		
		}
		break;

	case BUTTON_QUESTVIEWER4:
		{
			if(m_pradiobutton_QuestViewer5->IsCheck())
			{						
				if(pclClient->pclQuestManager->siStartConversationCounter<1 )
					return;

				pclClient->pclQuestManager->siStartConversationCounter--;	
				SI32 QuestUnique = _tstoi(m_peditbox_QuestViewer->GetText());
				SI32 QuestStep= QUEST_STEP_GREETING;
				pclclient->pclQuestManager->LoadQuestText();			
				ShowQuestView(QuestUnique,QUEST_STEP_GREETING );		
				ShowOtherThings( QuestUnique, QuestStep);
			}
			else
			{
				if(pclClient->pclQuestManager->siEndConversationCounter<1)
						return;
					pclClient->pclQuestManager->siEndConversationCounter--;
					SI32 QuestUnique = _tstoi(m_peditbox_QuestViewer->GetText());
					SI32 QuestStep= QUEST_STEP_CLEAR;
					pclclient->pclQuestManager->LoadQuestText();			
					ShowQuestView(QuestUnique,QUEST_STEP_CLEAR );		
					ShowOtherThings( QuestUnique, QuestStep);
			}
		}
		break;

	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}

	return;
}

void CNQuestViewerDlg::InitCombo()
{
	stComboBoxItemData QuestViewerComboData;
	QuestViewerComboData.Init();
    
	QuestViewerComboData.Set(TEXT("장금"));
	m_pCompobox_QuestViewer->AddItem(&QuestViewerComboData);

	QuestViewerComboData.Set(TEXT("내금위"));
	m_pCompobox_QuestViewer->AddItem(&QuestViewerComboData);

	QuestViewerComboData.Set(TEXT("내시"));
	m_pCompobox_QuestViewer->AddItem(&QuestViewerComboData);	

	QuestViewerComboData.Set(TEXT("김정호"));
	m_pCompobox_QuestViewer->AddItem(&QuestViewerComboData);	

	QuestViewerComboData.Set(TEXT("장길산"));
	m_pCompobox_QuestViewer->AddItem(&QuestViewerComboData);

	QuestViewerComboData.Set(TEXT("역관"));
	m_pCompobox_QuestViewer->AddItem(&QuestViewerComboData);


}
void CNQuestViewerDlg::ShowOtherThings(SI32 QuestUnique, SI32 QuestStep)
{
	cltClient* pclclient = (cltClient*)pclClient;
	//--------------------
	// 타이틀을 보여준다. 
	//--------------------
	SI32 NPCkind = m_pCompobox_QuestViewer->GetSelectedIndex();
	pclClient->pclQuestManager->siQuestType =QUEST_TYPE_BEGINNER2;
	if(NPCkind==0)	
	{	
		m_siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_JANGGUMI")); //장금이
		m_sirealNpcKind =0;
	}
	else if(NPCkind==1)
	{
		m_siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_ROYALGUARD") ); //내금위
		m_sirealNpcKind=2;
	}
	else if(NPCkind==2)
	{
		m_siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_WAITINGMAN") ); //내시
		m_sirealNpcKind =4;
	}
	else if(NPCkind==3)
	{
		m_siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_WAITINGMAN2") ); //김정호
		m_sirealNpcKind=6;
	}
	else if(NPCkind==4)
	{
		m_siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_JANGGILSAN")); //장길산
		m_sirealNpcKind=10;
	}
	//else if(NPCkind==5)
	//{
	//	m_siNpcKind = pclClient->GetUniqueFromHash( TEXT("KIND_WAITINGMAN3") ); //역관
	//	pclClient->pclQuestManager->siQuestType =QUEST_TYPE_BEGINNER5;
	//	m_sirealNpcKind=8;
	//}

	else
		m_siNpcKind = 0;


	//---------------------------
	// 보상 조건을 보여준다. 
	//---------------------------	
	TCHAR buffer[1024]=TEXT("");
	cltQuestInfo* pclquestinfo =pclClient->pclQuestManager->GetQuestInfo(QuestUnique);
	if(pclquestinfo)
	{
		pclquestinfo->clQuestRewardInfo.GetExplainText(buffer, 1024);
	}
	else
	{
		return;
	}
	(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditQuestReward)->SetText(buffer);

	StringCchCopy( buffer, 1024, TEXT(" "));
	//---------------------------
	// 임무 부여 조건을 보여준다. 
	//---------------------------	
	if(pclquestinfo->clInitConditionInfo.siLevel)
	{
		TCHAR temp[256];

		TCHAR* pText = GetTxtFromMgr(3008);
		StringCchPrintf(temp, 256, pText, pclquestinfo->clInitConditionInfo.siLevel);
		StringCchCat(buffer, 1024, temp);
	}
	if(pclquestinfo->clInitConditionInfo.siSkillUnique)
	{
		TCHAR skillname[128];
		if(pclclient->pclSkillManager->GetSkillName(pclquestinfo->clInitConditionInfo.siSkillUnique, skillname) == true)
		{
			TCHAR temptext[128];
			TCHAR* pText = GetTxtFromMgr(3009);
			StringCchPrintf(temptext, 128, pText, skillname, pclquestinfo->clInitConditionInfo.siSkillLevel);

			StringCchCat(buffer, 1024, temptext);
		}
	}
	(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditCondition)->SetText(buffer);
	
		
	if(pclquestinfo)
	{
		const TCHAR* pkindname = pclClient->pclKindInfoSet->GetName(m_siNpcKind);
		if(pkindname)
		{
			TCHAR buffer[256];
			StringCchPrintf(buffer, 256, TEXT("%s - %s"), pkindname, pclquestinfo->szTitleText);

			((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->SetQuestName(buffer);
		}
	}

	//-----------------------
	// 현재 상태를 보여준다. 
	//----------------------
	TCHAR szcurstatustext[1024] =TEXT("");
	SI32 siID = 1;
	pclClient->pclQuestManager->GetCurStatusText(siID, QuestStep, QuestUnique, szcurstatustext, 1024);
	((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->SetEditStatus(szcurstatustext);
	
}

void CNQuestViewerDlg::ShowQuestView(SI32 QuestUnique, SI32 QuestStep)
{
	cltClient* pclclient = (cltClient*)pclClient;
	pclclient->pclQuestManager->LoadQuestText();	

	TCHAR	FullPathName[MAX_PATH] = TEXT("") ;
	SI32	Font  = 0 ;
	TCHAR buffer[1024]= TEXT(" ");
	
	StringCchCopy(buffer, 1024, pclClient->pclQuestManager->GetText(QuestUnique,QuestStep));
	


	if ( pclClient->pclQuestManager->GetSprPath(QuestUnique,QuestStep,&Font,FullPathName) == true && pclClient->pclQuestManager->siViewState==1)
	{
		if(pclclient->m_pDialog[ NNEWQUEST_DLG ])
		{	
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[0])->SetChangeTextDrawMode(true);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[1])->SetChangeTextDrawMode(true);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditConversation)->Show(false);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[1])->Show(false);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pStatic[4])->Show(true);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pStatic[5])->Show(true);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[0])->Show(true);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[0])->SetText(buffer);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pStatic[4])->SetFileName(FullPathName);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pStatic[4])->SetFontIndex(Font);			
		}
	}
	else if(pclClient->pclQuestManager->siViewState==0)
	{
		if(pclclient->m_pDialog[ NNEWQUEST_DLG ])
		{			
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditConversation)->Show(true);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[0])->Show(false);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pStatic[4])->Show(false);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pStatic[5])->Show(false);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[1])->Show(false);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[1])->SetText(buffer);
			((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->SetEditConversation(buffer);
		}	
	}
	else
	{
		if(pclclient->m_pDialog[ NNEWQUEST_DLG ])
		{			
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[0])->SetChangeTextDrawMode(true);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[1])->SetChangeTextDrawMode(true);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditConversation)->Show(false);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[0])->Show(false);
            (((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pStatic[4])->Show(false);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pStatic[5])->Show(false);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[1])->Show(true);
			(((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_pEditExplain[1])->SetText(buffer);
		}
	}  
}



void CNQuestViewerDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient *pclclient = (cltClient *)pclClient;
	SI32 QuestUnique = _tstoi(m_peditbox_QuestViewer->GetText());
	cltQuestInfo* pclquestinfo =pclClient->pclQuestManager->GetQuestInfo(QuestUnique);
	SetNpcFontIndex(0);
	

	if( m_bOpened )
	{
		SI32 bit = pclclient->pclQuestManager->GetBitFromUnique(pclquestinfo->siType,QuestUnique);
		//if( pclClient->pclQuestManager->siQuestType == QUEST_TYPE_BEGINNER5 && pclClient->siServiceArea == ConstServiceArea_Korea && pclClient->pclQuestManager->bNewQuestDoing )
		//{
		//	if( g_SoundOnOffDlg.m_bFullScreenMode )
		//	{
		//		if(m_siNpcFontType == 0)	// 임무 받을때 모습
		//		{
		//			GP.PutSpr(&m_SprNpcFace,pclclient->GetScreenXSize() - 810 + ScreenRX, pclclient->GetScreenYSize() - 279 + ScreenRY , m_sirealNpcKind);
		//		}
		//		else if(m_siNpcFontType == 1)	// 임무 완료했을때 모습
		//		{
		//			GP.PutSpr(&m_SprNpcFace,pclclient->GetScreenXSize() - 810 + ScreenRX, pclclient->GetScreenYSize() - 279 + ScreenRY , m_sirealNpcKind + 1);
		//		}
		//		else if(m_siNpcFontType == 2)	// 임무 받을때 모습
		//		{
		//			GP.PutSpr(&m_SprNpcFace,pclclient->GetScreenXSize() - 810 + ScreenRX, pclclient->GetScreenYSize() - 279 + ScreenRY , 24);
		//		}
		//		else if(m_siNpcFontType == 3)	// 임무 완료했을때 모습
		//		{
		//			GP.PutSpr(&m_SprNpcFace,pclclient->GetScreenXSize() - 810 + ScreenRX, pclclient->GetScreenYSize() - 279 + ScreenRY , 25);
		//		}
		//	}
		//	else
		//	{
		//		if(m_siNpcFontType == 0)	// 임무 받을때 모습
		//		{
		//			GP.PutSpr(&m_SprNpcFace,pclclient->GetScreenXSize() - 810 + ScreenRX, pclclient->GetScreenYSize() - 259 + ScreenRY , m_sirealNpcKind);
		//		}
		//		else if(m_siNpcFontType == 1)	// 임무 완료했을때 모습
		//		{
		//			GP.PutSpr(&m_SprNpcFace,pclclient->GetScreenXSize() - 810 + ScreenRX, pclclient->GetScreenYSize() - 259 + ScreenRY , m_sirealNpcKind + 1);
		//		}
		//		else if(m_siNpcFontType == 2)	// 임무 받을때 모습
		//		{
		//			GP.PutSpr(&m_SprNpcFace,pclclient->GetScreenXSize() - 810 + ScreenRX, pclclient->GetScreenYSize() - 259 + ScreenRY , 0);
		//		}
		//		else if(m_siNpcFontType == 3)	// 임무 완료했을때 모습
		//		{
		//			GP.PutSpr(&m_SprNpcFace,pclclient->GetScreenXSize() - 810 + ScreenRX, pclclient->GetScreenYSize() - 259 + ScreenRY , 1);
		//		}
		//	}
		//}
		//else
		{
			if( g_SoundOnOffDlg.m_bFullScreenMode )
			{
				if(m_siButtonType == 0 || m_siButtonType == 1 || m_siButtonType == 3 || m_siButtonType == 4)	// 임무 받을때 모습
				{
					GP.PutSpr(&m_SprNpcFace,pclclient->GetScreenXSize() - 810 + ScreenRX, pclclient->GetScreenYSize() - 279 + ScreenRY , m_sirealNpcKind);
				}
				else if(m_siButtonType == 2)	// 임무 완료했을때 모습
				{
					GP.PutSpr(&m_SprNpcFace,pclclient->GetScreenXSize() - 810 + ScreenRX, pclclient->GetScreenYSize() - 279 + ScreenRY , m_sirealNpcKind + 1);
				}
			}
			else
			{
				if(m_siButtonType == 0 || m_siButtonType == 1 || m_siButtonType == 3 || m_siButtonType == 4)	// 임무 받을때 모습
				{
					GP.PutSpr(&m_SprNpcFace,pclclient->GetScreenXSize() - 810 + ScreenRX, pclclient->GetScreenYSize() - 259 + ScreenRY , m_sirealNpcKind);
				}
				else if(m_siButtonType == 2)	// 임무 완료했을때 모습
				{
					GP.PutSpr(&m_SprNpcFace,pclclient->GetScreenXSize() - 810 + ScreenRX, pclclient->GetScreenYSize() - 259 + ScreenRY , m_sirealNpcKind + 1);
				}
			}
		}
	}

}
