//---------------------------------
// 2004/9/01 ¼ºÁØ¿±
//---------------------------------

#include "CommonLogic.h"
#include "../../Client.h"

#include "NAutoHunting.h"

#include "Resource.h"

#include "../../lib/WebHTML/WebHTML.h"

//----------------------------
// CommonLogic 
//---------------------------
#include "CommonLogic.h"
#include "MsgType.h"
#include "MsgType-System.h"
#include "MsgType-Person.h"
#include "MsgRval-Define.h"



//----------------------------
// Server
//----------------------------
#include "../../../Server/Server.h"
#include "../../../Server/Minister/MinisterBase.h"
#include "../../../Server/Minister/MinisterMgr.h"
#include "../../../Server/Minister/Minister-Hyungzo.h"

//---------------------------
// Client
//---------------------------
#include "Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "FontManager/FontManager.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

//----------------------------
// DBManager
//----------------------------
#include "../../../../DBManager/GameDBManager_World/DBMsg-System.h"

#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Edit.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"



#include "../../../Resource.h"
#include "../../InterfaceMgr/DialogController.h"


extern cltCommonLogic* pclClient;

CNcltAutoHuntingManager::CNcltAutoHuntingManager()
{
	
	m_pAutoHuntStatic1 = NULL ;
	m_pAutoHuntStatic2 = NULL ;
	m_pAutoHuntStatic3 = NULL ;
	m_pAutoHuntStatic4 = NULL ;
	m_pAutoHuntEdit    = NULL ;
	m_pAutoHuntButton = NULL ;
	SetTimerOnOff( true );

	
}

CNcltAutoHuntingManager::~CNcltAutoHuntingManager()
{		
	SAFE_DELETE(m_pAutoHuntStatic1);
	SAFE_DELETE(m_pAutoHuntStatic2);
	SAFE_DELETE(m_pAutoHuntStatic3);
	SAFE_DELETE(m_pAutoHuntStatic4);
	SAFE_DELETE(m_pAutoHuntEdit);
	SAFE_DELETE(m_pAutoHuntButton);
	SetTimerOnOff( false );

}

void CNcltAutoHuntingManager::Action()
{

	/*
	cltServer* pclServer = (cltServer*) pclClient ;
	
	if ( pclServer == NULL ) return ;
	
	CMinisterHyungzo *pclminister = (CMinisterHyungzo *)pclServer->pclMinisterMgr->GetMinister(MINISTER_HYUNGZO);

	if ( pclminister == NULL ) return ;
	SI32 TimeLimit	= min(300, max(20, pclminister->m_siSuspectTime)) ;
	SI16 PrisonTime = min(60, max(5, pclminister->m_siPrisonTime));
	*/
	

	
}

void CNcltAutoHuntingManager::Create()
{

	if(!IsCreate())
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NAutoHunting/DLG_NAutoHunting.ddf"));
		file.CreatePopupDialog( this, NAUTOHUNT_DLG, TEXT("dialog_AutoHunting"), StatiCNAutoHuntingDlgMsgProc);

		m_pAutoHuntStatic1 = new CStatic( this ); 
		m_pAutoHuntStatic2 = new CEdit( this ); 
		m_pAutoHuntStatic3 = new CStatic( this ); 
		m_pAutoHuntStatic4 = new CStatic( this );
		m_pAutoHuntEdit    = new CEdit  ( this );	
		m_pAutoHuntButton  = new CButton( this );	     



		file.CreateControl(m_pAutoHuntStatic1, NAUTOHUNT_STATIC1 , TEXT("statictext_AutoHunting1"));
		file.CreateControl(m_pAutoHuntStatic2, NAUTOHUNT_STATIC2 , TEXT("editbox_AutoHunting2"));
		file.CreateControl(m_pAutoHuntStatic3, NAUTOHUNT_STATIC3 , TEXT("statictext_AutoHunting3"));
		file.CreateControl(m_pAutoHuntStatic4, NAUTOHUNT_STATIC4 , TEXT("statictext_AutoHunting4"));	
		file.CreateControl(m_pAutoHuntEdit   , NAUTOHUNT_EDIT    , TEXT("editbox_AutoHunting"));
		file.CreateControl(m_pAutoHuntButton , NAUTOHUNT_BUTTON  , TEXT("button_AutoHunting"));
		TCHAR* pTitle = GetTxtFromMgr(3685);
		SetTitle(pTitle);
		TCHAR* pText = GetTxtFromMgr(3686);
		m_pAutoHuntStatic4->SetText(pText,DT_WORDBREAK);
		 pText = GetTxtFromMgr(3687);
		m_pAutoHuntButton->SetText( pText,DT_WORDBREAK|DT_CENTER|DT_VCENTER );

		
	}

}


void CNcltAutoHuntingManager::Set(TCHAR * Txt,SI32 slot,SI32 minuts,SI16 prisontime,SI16 prisonerpenalty)
{
	cltClient *pclclient = (cltClient*)pclClient;

	if(!IsCreate())
	{
		Create();
	}
		
	TCHAR buffer[2048] = TEXT("") ;
	SetActionTimer( 200);

	TCHAR* pText = GetTxtFromMgr(3383);
	StringCchPrintf(buffer, 2048, pText,Txt);

	
	m_pAutoHuntStatic1->SetBKColor(true);
	m_pAutoHuntStatic1->SetFontSize(20);

	m_pAutoHuntStatic1->SetText(buffer,DT_WORDBREAK);	

	const TCHAR* name = pclClient->pclCM->CR[1]->GetName();
	pText = GetTxtFromMgr(3384);
	StringCchPrintf(buffer, 2048, pText,
		name,minuts/60, prisontime,
		pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() * prisonerpenalty );

	m_siLeftTime = minuts ;
	m_siSlot = slot ;	
	m_pAutoHuntStatic2->SetBKColor(true);
	m_pAutoHuntStatic2->SetText(buffer);
//	m_pAutoHuntEdit->SetFocus(true);

	pclclient->m_pInterfaceMgr->SetFocus( this, m_pAutoHuntEdit );


	
	
}


void CNcltAutoHuntingManager::SetStaticTime(SI32 totaltime)
{
	TCHAR buffer[256] = TEXT("") ;
	TCHAR* pText = GetTxtFromMgr(3385);
	StringCchPrintf(buffer, 256, pText,
					GetLeftTime(),totaltime);

	m_pAutoHuntStatic3->SetText(buffer,DT_WORDBREAK);

}



void CALLBACK CNcltAutoHuntingManager::StatiCNAutoHuntingDlgMsgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNcltAutoHuntingManager *pThis = (CNcltAutoHuntingManager*) pControllerMgr;
	pThis->NAutoHuntingDlgMsgProc( nEvent, nControlID, pControl );
	
}

void CALLBACK CNcltAutoHuntingManager::NAutoHuntingDlgMsgProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch(nControlID)
	{	
	case COMMON_DIALOG_CLOSEBUTTON:	
	{
		
		//DeleteDialog();
	}
	break;

	case NAUTOHUNT_EDIT:
		{
			switch(nEvent)
			{			
			case EVENT_EDITBOX_STRING:
				{
					TCHAR buffer[256] = TEXT("")		;
					StringCchCopy(buffer, 256, m_pAutoHuntEdit->GetText());					
					cltGameMsgRequest_AutoHunting_Ask clinfo(m_siSlot,buffer);
					cltMsg clMsg(GAMEMSG_REQUEST_AUTOHUNTING_ASK, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	case NAUTOHUNT_BUTTON:
		{
			switch(nEvent)
			{			
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buffer[256] = TEXT("")		;
					StringCchCopy(buffer, 256, m_pAutoHuntEdit->GetText());					
					cltGameMsgRequest_AutoHunting_Ask clinfo(m_siSlot,buffer);
					cltMsg clMsg(GAMEMSG_REQUEST_AUTOHUNTING_ASK, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
		
	}

	return  ;
}






