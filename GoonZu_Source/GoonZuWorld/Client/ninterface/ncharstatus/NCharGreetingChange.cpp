/* ==========================================================================
	클래스 :

	작성일 :		05/03/17
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#include "..\..\Common\JWLib\IsCorrectID\IsCorrectID.h"
#include "Char\CharManager\CharManager.h"

#include "..\Client\Client.h"
#include "./NCharGreetingChange.h"
#include "..\resource.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"

#include "../CommonLogic/MsgType-Person.h"
#include "Msg/MsgType-ItemMall.h"

#include "../../Client/InterfaceMgr/Interface/Button.h"
#include "../../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../../Client/InterfaceMgr/Interface/Static.h"
#include "../../Client/InterfaceMgr/Interface/Edit.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "../common/Event/event.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"

extern cltCommonLogic* pclClient;

cltNCharGreetingChange::cltNCharGreetingChange()
{
	MStrCpy(m_szGreetMessage, TEXT(""), 128);
	m_DlgRectInitSwitch = FALSE;

	m_pEditbox_greeting	 = NULL; 
	m_pEditbox_newtext	 = NULL;
	m_pEditbox_homepage	 = NULL;

	m_pStatictext_1		 = NULL;
	m_pStatictext_2		 = NULL;
	m_pStatictext_3		 = NULL;

	m_pButton_set		 = NULL; 
	m_pButton_cancel	 = NULL; 

	m_pButton_character_change	= NULL; 
	m_pButton_ability_init		= NULL;
	m_pButton_naming			= NULL;// 네이밍 퀘스트 버튼 추가


}

cltNCharGreetingChange::~cltNCharGreetingChange()
{
	SetTimerOnOff(false);

	m_DlgRectInitSwitch = FALSE;

	if(	m_pEditbox_greeting ) delete m_pEditbox_greeting;
	if( m_pEditbox_newtext ) delete m_pEditbox_newtext;
	if( m_pEditbox_homepage ) delete m_pEditbox_homepage;

	if(	m_pStatictext_1 ) delete m_pStatictext_1;
	if(	m_pStatictext_2 ) delete m_pStatictext_2;
	if(	m_pStatictext_3 ) delete m_pStatictext_3;

	if(	m_pButton_set ) delete m_pButton_set	;
	if(	m_pButton_cancel ) delete m_pButton_cancel	;

	if(	m_pButton_character_change	 ) delete m_pButton_character_change;
	if(	m_pButton_ability_init		 ) delete m_pButton_ability_init;
	if(	m_pButton_naming		 ) delete m_pButton_naming;// 네이밍 퀘스트 버튼 추가
	
}

void cltNCharGreetingChange::Create()
{
	// 대화상자가 없는 상태라면 만든다. 
	if( ! IsCreate() )
	{
		//CDialogController::Create( CLTNCharGreetingCHANGE_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, NCharGreetingChangeStaticProc, TEXT("이름 변경하기"), TEXT("NInterface/Interface/Interface_20X20_00_109.Spr"), 9, true, 80, 100, 380,240, true, true, true);

		cltClient* pclclient = (cltClient*)pclClient;
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NChangeGreetingDlg/DLG_NCharStatus_NChangeGreeting_New.ddf"));
		//	file.LoadFile(TEXT("NInterface/Data/NChangeGreetingDlg/DLG_NCharStatus_NChangeGreeting.ddf"));
		
		file.CreatePopupDialog( this, CLTNCHARGREETINGCHANGE_DLG, TEXT("dialog_setgreeting"), NCharGreetingChangeStaticProc );

		m_pEditbox_greeting			= new 	CEdit( this );
		m_pEditbox_newtext			= new	CEdit( this );
		m_pEditbox_homepage			= new	CEdit( this );

		m_pStatictext_1				= new 	CStatic( this )	;
		m_pStatictext_2				= new 	CStatic( this )	;
		m_pStatictext_3				= new 	CStatic( this )	;
		
		m_pButton_set				= new 	CButton( this )	;
		m_pButton_cancel				= new 	CButton( this )	;		
		m_pButton_character_change	= new 	CButton( this )	;
		m_pButton_ability_init			= new 	CButton( this )	;
		m_pButton_naming				= new 	CButton( this )	;
		
		
		file.CreateControl( m_pEditbox_greeting, CLTNCHARGREETINGCHANGE_DLG_EDITBOX_GREETING, TEXT("editbox_greeting") );
		file.CreateControl( m_pEditbox_newtext, CLTNCHARGREETINGCHANGE_DLG_EDITBOX_NEWTEXT, TEXT("editbox_newtext") );
		file.CreateControl( m_pEditbox_homepage, CLTNCHARGREETINGCHANGE_DLG_EDITBOX_HOMEPAGE, TEXT("editbox_homepage") );

		file.CreateControl( m_pStatictext_1, CLTNCHARGREETINGCHANGE_DLG_STATICTEXT_1, TEXT("statictext_1") );
		file.CreateControl( m_pStatictext_2, CLTNCHARGREETINGCHANGE_DLG_STATICTEXT_2, TEXT("statictext_2") );
		file.CreateControl( m_pStatictext_3, CLTNCHARGREETINGCHANGE_DLG_STATICTEXT_3, TEXT("statictext_3") );

		file.CreateControl( m_pButton_set, CLTNCHARGREETINGCHANGE_DLG_BUTTON_SET, TEXT("button_set") );
		file.CreateControl( m_pButton_cancel, CLTNCHARGREETINGCHANGE_DLG_BUTTON_CANCEL, TEXT("button_cancel") );
		file.CreateControl( m_pButton_character_change, CLTNCHARGREETINGCHANGE_DLG_BUTTON_CHARACTER_CHANGE, TEXT("button_charater_change") );
		file.CreateControl( m_pButton_ability_init,	CLTNCHARGREETINGCHANGE_DLG_BUTTON_ABILITY_INIT, TEXT("button_ability_init") );
		file.CreateControl( m_pButton_naming, CLTNCHARGREETINGCHANGE_DLG_BUTTON_NAMING, TEXT("button_naming") );// 네이밍 퀘스트 버튼 추가
		
				
		m_pStatictext_2->SetTextPrintOption(DT_LEFT);
		m_pStatictext_2->Refresh();

		m_pEditbox_newtext->SetMaxEditTextLength( MAX_GREETING_LENGTH + 1 );
		m_pEditbox_newtext->SetAutoScroll(true);

		m_pEditbox_homepage->SetMaxEditTextLength(128);
		m_pEditbox_homepage->SetAutoScroll(true);

		TCHAR* pText = NULL;
		TCHAR* pTitle = GetTxtFromMgr(5417);
		SetTitle( pTitle);

		pText= GetTxtFromMgr(3721);
		m_pButton_set->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pText= GetTxtFromMgr(3902);
		m_pButton_cancel->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		m_pEditbox_homepage->Show(false);
		m_pStatictext_3->Show(false);
		m_pButton_character_change->Show(false);
		m_pButton_ability_init->Show(false);
		m_pButton_naming->Show(false);
			
		if(pclClient->IsCountrySwitch(Switch_CharStatusReform))
		{
			m_pButton_character_change->Show(true);
			m_pButton_ability_init->Show(true);
		}

		if(pclClient->IsCountrySwitch(Switch_NamingQuest))
		{
			m_pButton_naming->Show(true);
		}

		if(pclClient->IsCountrySwitch(Switch_Homepage_link))
		{
			m_pEditbox_homepage->Show(true);
			m_pStatictext_3->Show(true);

			cltGameMsgRequest_GetHomepage_URL clinfo(pclClient->pclCM->CR[1]->pclCI->GetPersonID());

			cltMsg clMsg(GAMEMSG_REQUEST_GET_HOMEPAGE_URL, sizeof(clinfo), (BYTE*)&clinfo);
			pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		
		cltGameMsgRequest_GetGreeting clinfo(pclClient->pclCM->CR[1]->pclCI->GetPersonID(), 0);

		cltMsg clMsg(GAMEMSG_REQUEST_GETGREETING, sizeof(clinfo), (BYTE*)&clinfo);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

		SetTimerOnOff(true);
	}
	else
	{
		DeleteDialog();
	}
}

void cltNCharGreetingChange::Close()
{
	DeleteDialog();
}

void CALLBACK cltNCharGreetingChange::NCharGreetingChangeStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	cltNCharGreetingChange* pThis = (cltNCharGreetingChange*)pControllerMgr;
	pThis->NCharGreetingChangeProc( nEvent, nControlID, pControl );
}

void CALLBACK cltNCharGreetingChange::NCharGreetingChangeProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			m_DlgRectInitSwitch = TRUE;
			Close();
		}
		break;
	case CLTNCHARGREETINGCHANGE_DLG_BUTTON_SET:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					MStrCpy(m_szGreetMessage, m_pEditbox_newtext->GetText(), MAX_GREETING_LENGTH + 1 );

#ifdef _IAF_EXPORT
					if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
					{
						TCHAR ResultBuf[1024] = "";
						IAF::Filtering( m_szGreetMessage, ResultBuf, sizeof(ResultBuf) );
						memcpy( m_szGreetMessage, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
					}
#else
					//KHY - 0813 - 욕설 필터링.
					pclclient->m_pCAbuseFilter->CheckAbuse( m_szGreetMessage);
#endif				
					SI32 limit = _tcslen(m_szGreetMessage);
					if ( limit > 0 && limit < 128 )
					{
						
						cltGameMsgRequest_SetGreeting clinfo(m_szGreetMessage);
						
						cltMsg clMsg(GAMEMSG_REQUEST_SETGREETING, sizeof(clinfo), (BYTE*)&clinfo);
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

						m_pEditbox_greeting->SetText(m_szGreetMessage);
						m_pEditbox_newtext->SetText(TEXT(""));
					}
				}
				break;
			}
		}
		break;
	case CLTNCHARGREETINGCHANGE_DLG_BUTTON_NAMING:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( pclclient->m_pDialog[NSELECTNAMING_DLG] == NULL)
						pclclient->CreateInterface( NSELECTNAMING_DLG );
				}
			}
		}
	case CLTNCHARGREETINGCHANGE_DLG_BUTTON_CANCEL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				DeleteDialog();
				break;
			}
		}
		break; ;
	case CLTNCHARGREETINGCHANGE_DLG_BUTTON_CHARACTER_CHANGE:
		{
			switch( nEvent )
			{
				case EVENT_BUTTON_CLICKED:
					{
						if (pclclient->pclCM->CR[1]->pclCI->clBI.GetKind() <= 0 ||
							pclclient->pclCM->CR[1]->pclCI->clBI.GetKind() >= KIND_SYSTEM )
							return ;

						//if ( !pclCharKindChange->CreateDlg(pclClient->GetInstance(),pclClient->GetHwnd()) )
						//	pclCharKindChange->Close();
						pclclient->CreateInterface( CLTNCHARKINDCHANGE_DLG );
					}
					break;
			}
		}
		break;
	case CLTNCHARGREETINGCHANGE_DLG_BUTTON_ABILITY_INIT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( pclClient->IsWhereServiceArea(ConstServiceArea_China | ConstServiceArea_Taiwan) )
					{
						TCHAR buffer[256] = TEXT("") ;

						//[진성] 캐릭터 상태 초기화 일정기간동안 마음것 사용. 
						if(pclClient->pclEventTimeManager->InEventTime("char_ability_init", &pclclient->sTime) )
						{
							cltMsg clMsg(GAMEMSG_REQUEST_INITSTATUS, 0);
							pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
							return;
						}

						//cyj 상태초기화이용권 유니크변경 14090->24030
						//SI32 itemunique = 14090 ;  // 상태초기화이용권
						SI32 itemunique1 = ITEMUNIQUE(24030) ;  // 상태초기화이용권
						SI32 itemunique2 = ITEMUNIQUE(24031) ;  // 중국이벤트 상태초기화이용권
						if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique1) <= 0 &&
							pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique2) <= 0 )
						{
							TCHAR* pTitle = GetTxtFromMgr(605);
							TCHAR* pText = GetTxtFromMgr(608);

							StringCchPrintf(buffer, 256, pText);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
							return ;
						}

						if ( pclclient->pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_PRISON))
						{
							TCHAR* pTitle = GetTxtFromMgr(836);
							TCHAR* pText = GetTxtFromMgr(6033);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,pText);
							return ;
						}


						// 장비가 장착되어있는지 검사한다.
						for ( SI16 i=0 ; i<PERSONITEM_RESERVED1 ; i++ )
						{
							if ( i == PERSONITEM_BAG || i == PERSONITEM_BAG_SUB )
								continue;

							if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[i].siUnique != 0 )
							{
								TCHAR* pText = GetTxtFromMgr(6572);
								StringCchPrintf(buffer, 256, pText);

								pText = GetTxtFromMgr(605);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
								return;
							}
						}

						TCHAR* pText = GetTxtFromMgr(609);
						TCHAR* pTitle = GetTxtFromMgr(605);

						stMsgBox MsgBox;
						// 현재 dialog가 탭 창 같은 Child Dialog일때
						//CControllerMgr *pParent = NULL;
						//pParent = this->GetParentDialog();
						//if(pParent == NULL ) return;
						MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_OKCANCEL, 0 );

						pclclient->SetMsgBox( &MsgBox, NULL, 0 );
					
					}
					else
					{
						TCHAR buffer[256] = TEXT("") ;

						//[진성] 캐릭터 상태 초기화 일정기간동안 마음것 사용. 
						if(pclClient->pclEventTimeManager->InEventTime("char_ability_init", &pclclient->sTime) )
						{
							cltMsg clMsg(GAMEMSG_REQUEST_INITSTATUS, 0);
							pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
							return;
						}
						
						//cyj 상태초기화이용권 유니크변경 14090->24030
						//SI32 itemunique = 14090 ;  // 상태초기화이용권
						SI32 itemunique			= ITEMUNIQUE(24030);	//	상태초기화이용권
						SI32 siEventItemUnique	= ITEMUNIQUE(24125);	// [종호] 이벤트 아이템 유니크. 24030 과 같은 기능을 한다.
						
						// 상태초기화 아이템이 없느 경우.
						if (	pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 && 
								pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(siEventItemUnique) <= 0)
						{
							TCHAR* pTitle = GetTxtFromMgr(605);
							TCHAR* pText = GetTxtFromMgr(608);

							StringCchPrintf(buffer, 256, pText);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
							return ;
						}

						if ( pclclient->pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_PRISON))
						{
							TCHAR* pTitle = GetTxtFromMgr(836);
							TCHAR* pText = GetTxtFromMgr(6033);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,pText);
							return ;
						}


						// 장비가 장착되어있는지 검사한다.
						for ( SI16 i=0 ; i<PERSONITEM_RESERVED1 ; i++ )
						{
							if ( i == PERSONITEM_BAG || i == PERSONITEM_BAG_SUB )
								continue;

							if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[i].siUnique != 0 )
							{
								TCHAR* pText = GetTxtFromMgr(6572);
								StringCchPrintf(buffer, 256, pText);

								pText = GetTxtFromMgr(605);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
								return;
							}
						}

						TCHAR* pText = GetTxtFromMgr(609);
						TCHAR* pTitle = GetTxtFromMgr(605);

						stMsgBox MsgBox;
						// 현재 dialog가 탭 창 같은 Child Dialog일때
						//CControllerMgr *pParent = NULL;
						//pParent = this->GetParentDialog();
						//if(pParent == NULL ) return;
						MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_OKCANCEL, 0 );
						//MsgBox.Set(GetParentDialog(), pTitle, pText, MSGBOX_TYPE_OKCANCEL, 0 );

						
						pclclient->SetMsgBox( &MsgBox, NULL, 0 );
					}
				}
				break;
			}

		}
		break;

	}
}

void cltNCharGreetingChange::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
		case 0:	
			{	
				if( RetValue )
				{
					cltMsg clMsg(GAMEMSG_REQUEST_INITSTATUS, 0);
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
			}
			break;
	}
	return;
	
}
//#endif

void cltNCharGreetingChange::SetGreetingText(TCHAR* pText)
{
	m_pEditbox_greeting->SetText(pText);
}

void cltNCharGreetingChange::Action()
{
	TCHAR buffer[128];
	TCHAR msg[ 128 ];
	//[GOONZU-588 김영훈 수정:2007.11.28 - 인사말 설정시 MAX(127)자가 되도 표시는 126자료 표시되는 버그 수정]
	// 글로벌에서 인사말 버그가 발생하여, 최대입력 바이트를 100바이트로 제한한다. - by LEEKH 2009-02-24
	MStrCpy( msg, m_pEditbox_newtext->GetText(), MAX_GREETING_LENGTH + 1 );

	TCHAR* pText = GetTxtFromMgr(5419);
	int len = _tcslen( msg );
	
	StringCchPrintf( buffer, sizeof(buffer), pText, len );
	
	m_pStatictext_2->SetText(buffer);
}
