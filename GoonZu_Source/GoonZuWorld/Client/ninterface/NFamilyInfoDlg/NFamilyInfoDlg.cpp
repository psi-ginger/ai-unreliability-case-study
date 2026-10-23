
#include "NFamilyInfoDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../../common/Char/CharManager/CharManager.h"
#include "../../common/Family/Marriage.h"
#include "../../common/Family/Family.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Person.h"

#include "../../ninterface//NFamilyInfoDlg//NButtonMenuDlg.h"

#include "Client.h" //[영진]

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ChatEdit.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../ninterface/npartydlg/NPartyDlg.h"
#include "../../ninterface/nletterdlg/NLetterDlg.h"

#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/MsgType-system.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../AbuseFilter/AbuseFilter.h"
#include "../../Music/music.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../../Client.h"

#include "../NHelpButton/NHelpButtonDlg.h"
#include "../NMarriageLoveLevelDlg/NMarriageLoveLevelDlg.h"

#include "../nprivatechatdlg/NPrivateChatDlgMgr.h"
#include "../NFamilyCookieDlg/NFamilyCookieDlg.h"
#include "../NDormancyAccount/NDormancyPointDlg.h"

extern cltCommonLogic* pclClient;

CNFamilyInfoDlg::CNFamilyInfoDlg( void )
{
	Init( );
}

CNFamilyInfoDlg::~CNFamilyInfoDlg( void )
{
	Init( );
}

void CNFamilyInfoDlg::Init( )
{
		siWarpTargetFamily	=	0	;
		m_bMini	=	false	;
		m_siOldPosX	=	0	;
		m_siOldPosY	=	0	;
		m_siOldXSize	=	0	;
		m_siOldYSize	=	0	;
		m_siMinimizedSlot	=	0	;
		m_bShowparents	=	true	;
		m_bShowchildres	=	true	;
		m_bShowchat	=	true	;
}

void CNFamilyInfoDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}

	CInterfaceFile file;
	if ( pclClient->IsCountrySwitch( Switch_We_Are_Family ) == true )
	{
		file.LoadFile( TEXT( "NInterface/Data/NFamilyDlg/DLG_NFamilyInfo.ddf" ) );
	}
	else
	{
		file.LoadFile( TEXT( "NInterface/Data/NFamilyDlg/DLG_NFamilyInfo_old.ddf" ) );
	}	
	file.CreatePopupDialog( this, NFAMILYINFO_DLG, TEXT( "dialog_familyinfo" ), StaticCallBackDialogNFamilyInfoDlg );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	FAMILYINFO_DLG_BUTTON_MY,		this),	FAMILYINFO_DLG_BUTTON_MY,		TEXT("button_my") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	FAMILYINFO_DLG_BUTTON_MATE,		this),	FAMILYINFO_DLG_BUTTON_MATE,		TEXT("button_mate") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	FAMILYINFO_DLG_EDIT_SHOWCHILD,		this),	FAMILYINFO_DLG_EDIT_SHOWCHILD,		TEXT("editbox_NONAME2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	FAMILYINFO_DLG_EDITBOX_MY,		this),	FAMILYINFO_DLG_EDITBOX_MY,		TEXT("editbox_my") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	FAMILYINFO_DLG_EDITBOX_MATE,	this),	FAMILYINFO_DLG_EDITBOX_MATE,	TEXT("editbox_mate") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,	FAMILYINFO_DLG_OUTLINE_NONAME1,	this),	FAMILYINFO_DLG_OUTLINE_NONAME1,	TEXT("outline_NONAME1") );
	

	if ( pclClient->IsCountrySwitch( Switch_We_Are_Family ) == true )
	{
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_INTERFACEBUTTON_MINI,	this),	MYFAMILYINFO_DLG_INTERFACEBUTTON_MINI,	TEXT("button_mini") );// 최소화 버튼
		// 버튼 추가 - 가족 정보

		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	MYFAMILYINFO_DLG_EDIT_CHILDREN,		this),	MYFAMILYINFO_DLG_EDIT_CHILDREN,		TEXT("editbox_NONAME1") );	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st,	TEXT("button_NONAME1") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_2nd,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_2nd,	TEXT("button_NONAME6") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_3rd,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_3rd,	TEXT("button_NONAME2") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_4th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_4th,	TEXT("button_NONAME7") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_5th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_5th,	TEXT("button_NONAME3") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_6th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_6th,	TEXT("button_NONAME8") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_7th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_7th,	TEXT("button_NONAME4") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_8th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_8th,	TEXT("button_NONAME9") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_9th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_9th,	TEXT("button_NONAME5") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_CHILDREN_10th,	this),	MYFAMILYINFO_DLG_BUTTON_CHILDREN_10th,	TEXT("button_NONAME10") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,	MYFAMILYINFO_DLG_OUTLINE_NONAME2,	this),	FAMILYINFO_DLG_OUTLINE_NONAME1,	TEXT("outline_NONAME2") );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface(CHATEDIT,	MYFAMILYINFO_DLG_EDIT_CHAT,		this),	MYFAMILYINFO_DLG_EDIT_CHAT,		TEXT("chatedit_NONAME2") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	MYFAMILYINFO_DLG_EDIT_CHAT_INPUT,		this),	MYFAMILYINFO_DLG_EDIT_CHAT_INPUT,		TEXT("editbox_NONAME3") );
		
		
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_EBUTTON_MINIDLG,	this),	MYFAMILYINFO_DLG_EBUTTON_MINIDLG,	TEXT("button_minibutton") );//최소화 버튼을 누르면 이렇게 바뀐다

		// 가족 정보창 개편 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_PARENTS,	this),	MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_PARENTS,	TEXT("button_Show_Parents") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHILDREN,	this),	MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHILDREN,	TEXT("button_children") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHAT,		this),	MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHAT,		TEXT("button_chat") );

	}
	
	// 텍스트 설정
	CEdit* pMyEdit = m_InterfaceFactory.GetEdit( FAMILYINFO_DLG_EDITBOX_MY );
	if ( pMyEdit )
	{
		pMyEdit->SetTextPrintOption( DT_CENTER );
		pMyEdit->SetText( GetTxtFromMgr(30101) );
		pMyEdit->Refresh();
	}

	CEdit* pMateEdit = m_InterfaceFactory.GetEdit( FAMILYINFO_DLG_EDITBOX_MATE );
	if ( pMateEdit )
	{
		pMateEdit->SetTextPrintOption( DT_CENTER );
		pMateEdit->SetText( GetTxtFromMgr(30102) );
		pMateEdit->Refresh();
	}
	CEdit* pShowChildEdit = m_InterfaceFactory.GetEdit( FAMILYINFO_DLG_EDIT_SHOWCHILD );
	if ( pShowChildEdit )
	{
		pShowChildEdit->SetTextPrintOption( DT_CENTER );
		pShowChildEdit->SetText( GetTxtFromMgr(30121) );	// 부모
		pShowChildEdit->Refresh();
	}
	if ( pclClient->IsCountrySwitch( Switch_We_Are_Family ) == true )
	{
		CEdit* pChat_inputEdit = m_InterfaceFactory.GetEdit( MYFAMILYINFO_DLG_EDIT_CHAT_INPUT );
		if (pChat_inputEdit )
		{
			pChat_inputEdit->SetAutoScroll(true)	;
			pChat_inputEdit->SetMaxEditTextLength(MAX_CHAT_DATA_LENGTH - 2 )	;

		}
		cltCharCommon* pChar = pclClient->pclCM->GetChar(1)	;
		if(pChar != NULL )
		{
			CEdit* ptEdit = m_InterfaceFactory.GetEdit( MYFAMILYINFO_DLG_EDIT_CHILDREN );
			if(pChar->pclFamily != NULL )
			{
				if (pChar->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_PARANTS )
				{
					pMyEdit->Show( true)	;
					pMateEdit->Show( true)	;
					pShowChildEdit->Show( false)	;
					// 자식 
					ptEdit->SetTextPrintOption( DT_CENTER );
					TCHAR* pText = GetTxtFromMgr( 30118)	;
					ptEdit->SetText(pText)	;
					ptEdit->Refresh()	;
				}
				else
				{
					pMyEdit->Show( false)	;
					pMateEdit->Show( false)	;
					pShowChildEdit->Show( true)	;

					ptEdit->SetTextPrintOption( DT_CENTER );
					// 형제
					TCHAR* pText = GetTxtFromMgr( 30122)	;
					ptEdit->SetText(pText)	;
					ptEdit->Refresh()	;
				}
			}

		}
		// 최소화 버튼을 보이지 않도록 한다,
		CButton* pButton_Mini = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_EBUTTON_MINIDLG );
		pButton_Mini->Show(false);
	}
	DisplayFamilyInfo();

	SetActionTimer( 1000 );
	SetTimerOnOff( true );
	
}

void CALLBACK CNFamilyInfoDlg::StaticCallBackDialogNFamilyInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNFamilyInfoDlg* pThis = (CNFamilyInfoDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNFamilyInfoDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNFamilyInfoDlg::CallBackDialogNFamilyInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*)pclClient;

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar ){		return;		}

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
			if ( pclclient->m_pDialog[ NBUTTONMENU_DLG ] )
			{
				((CNButtonMenuDlg *)pclclient->m_pDialog[ NBUTTONMENU_DLG ])->DeleteDialog()	;
			}
			
		}break	;
	case FAMILYINFO_DLG_BUTTON_MATE:	// 배우자
		{
			if( pclChar->IsMarried() )	// 결혼을 해야 부모가 되므로 결혼해야 이창을 연다
			{
				CButton* pButton = m_InterfaceFactory.GetButton( FAMILYINFO_DLG_BUTTON_MATE )	;
				if(pButton != NULL )
				{
					SI32 siPersonId = pclChar->pclFamily->GetChildPersonIDfromName( pButton->GetText() )	;
					OpenPopUp( FAMILYINFO_DLG_BUTTON_MATE, true,pclChar->pclMarriage->GetMatePersonID() ,pButton->GetText())	;
				}
			}
			else
			{
				CButton* pButton = m_InterfaceFactory.GetButton( FAMILYINFO_DLG_BUTTON_MATE )	;
				if(pButton != NULL )
				{
					SI32 siPersonId = pclChar->pclFamily->GetChildPersonIDfromName( pButton->GetText() )	;
					OpenPopUp( FAMILYINFO_DLG_BUTTON_MATE, false,pclChar->pclMarriage->GetMatePersonID() ,pButton->GetText())	;
				}

			}

		}break;
	case MYFAMILYINFO_DLG_INTERFACEBUTTON_MINI :	// 창을 최소화 할떄 쓰는 버툰
		{
			switch ( nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					Minimi(TRUE);
				}
			}
		}break;	
	case MYFAMILYINFO_DLG_EBUTTON_MINIDLG:	// 최소화로 창을 만들었다
		{	
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					Minimi(FALSE);
				}
			}
		}
		break;
	case MYFAMILYINFO_DLG_EDIT_CHAT_INPUT :	// 채팅 입력 
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{	
					SendChat();
				}
				break;
			}

		}break	;
	case FAMILYINFO_DLG_BUTTON_MY	:	// 내 창
		{
			switch ( nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				if( pclChar->IsMarried() )	// 결혼을 해야 부모가 되므로 결혼해야 이창을 연다
				{
					CButton* pButton = m_InterfaceFactory.GetButton( FAMILYINFO_DLG_BUTTON_MY )	;
					if ( pButton != NULL )
					{
						SI32 siPersonId = pclChar->pclFamily->GetChildPersonIDfromName( pButton->GetText() )	;
						OpenPopUp( FAMILYINFO_DLG_BUTTON_MY, true, pclChar->pclCI->GetPersonID()  ,pButton->GetText() )	;	
					}
				}
				else
				{
					CButton* pButton = m_InterfaceFactory.GetButton( FAMILYINFO_DLG_BUTTON_MY )	;
					if ( pButton != NULL )
					{
						SI32 siPersonId = pclChar->pclFamily->GetChildPersonIDfromName( pButton->GetText() )	;
						OpenPopUp( FAMILYINFO_DLG_BUTTON_MATE, false,pclChar->pclMarriage->GetMatePersonID() ,pButton->GetText())	;
					}
				}
			}

		}break	;
	case MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st	:	//자식버튼들을 눌렀다.
	case MYFAMILYINFO_DLG_BUTTON_CHILDREN_2nd	:
	case MYFAMILYINFO_DLG_BUTTON_CHILDREN_3rd	:
	case MYFAMILYINFO_DLG_BUTTON_CHILDREN_4th	:
	case MYFAMILYINFO_DLG_BUTTON_CHILDREN_5th	:
	case MYFAMILYINFO_DLG_BUTTON_CHILDREN_6th	:
	case MYFAMILYINFO_DLG_BUTTON_CHILDREN_7th	:
	case MYFAMILYINFO_DLG_BUTTON_CHILDREN_8th	:
	case MYFAMILYINFO_DLG_BUTTON_CHILDREN_9th	:
	case MYFAMILYINFO_DLG_BUTTON_CHILDREN_10th	:
		{
			switch ( nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{	
					OpenPopUpSet(nControlID)	;
				}break;
			}

		}break	;
	case MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_PARENTS:	// 부모 버튼
		{
			switch ( nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_bShowparents == true	)	// 부모님창이 보이고 있는중 
					{
						SI32 siHight = m_InterfaceFactory.GetOutLine(FAMILYINFO_DLG_OUTLINE_NONAME1)->GetHeight()	;
						SI16 siDilogHight = GetHeight( )	;

						ShowParent(false)	;						
						MoveChindren(-siHight)	;
						MoveChat(-siHight)	;						

						SetHeight( siDilogHight - siHight)	;

						m_bShowparents	=	false	;
						m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_PARENTS)->SetFontIndex(4);
						m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_PARENTS)->Refresh();
					}
					else if(m_bShowparents == false )// 부모님창이 사라짐
					{
						SI32 siHight = m_InterfaceFactory.GetOutLine(FAMILYINFO_DLG_OUTLINE_NONAME1)->GetHeight()	;
						SI16 siDilogHight = GetHeight( )	;

						ShowParent(true)	;						
						MoveChindren(siHight)	;
						MoveChat(siHight)	;

						SetHeight( siDilogHight + siHight)	;

						m_bShowparents	=	true	;
						m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_PARENTS)->SetFontIndex(0);
						m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_PARENTS)->Refresh();
					}

				}
			}			
		}break;
	case MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHILDREN:	// 자식버튼 
		{
			switch ( nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_bShowchildres == true	)	// 부모님창이 보이고 있는중 
					{
						SI32 siHight = m_InterfaceFactory.GetOutLine(MYFAMILYINFO_DLG_OUTLINE_NONAME2)->GetHeight()	;
						SI32 siHight2 = m_InterfaceFactory.GetEdit(MYFAMILYINFO_DLG_EDIT_CHILDREN)->GetHeight()	;
						SI16 siDilogHight = GetHeight( )	;
						SI32 siUseHight = siHight+ siHight2	;

						ShowChindren(false)	;
						MoveChindren(-siUseHight)	;
						MoveChat(-siUseHight)	;

						SetHeight( siDilogHight - siUseHight)	;
						
						m_bShowchildres	=	false	;
						m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHILDREN)->SetFontIndex(4);
						m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHILDREN)->Refresh();
					}
					else if(m_bShowchildres == false )// 부모님창이 사라짐
					{
						SI32 siHight = m_InterfaceFactory.GetOutLine(MYFAMILYINFO_DLG_OUTLINE_NONAME2)->GetHeight()	;
						SI32 siHight2 = m_InterfaceFactory.GetEdit(MYFAMILYINFO_DLG_EDIT_CHILDREN)->GetHeight()	;
						SI16 siDilogHight = GetHeight( )	;

						SI32 siUseHight = siHight+ siHight2	;
						
						ShowChindren(true)	;
						MoveChindren(siUseHight)	;
						MoveChat(siUseHight)	;

						SetHeight( siDilogHight + siUseHight)	;
						
						m_bShowchildres	=	true	;
						m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHILDREN)->SetFontIndex(0);
						m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHILDREN)->Refresh();
					}

				}
			}			
		}break;
	case MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHAT:
		{
			switch ( nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_bShowchat == true	)
					{
						SI32 siHight = m_InterfaceFactory.GetEdit(MYFAMILYINFO_DLG_EDIT_CHAT)->GetHeight()	;
						SI32 siHight2 = m_InterfaceFactory.GetEdit(MYFAMILYINFO_DLG_EDIT_CHAT_INPUT)->GetHeight()	;
						SI32 siUseHight = siHight+ siHight2	;
						SI16 siDilogHight = GetHeight( )	;

						ShowChat(false)	;
						SetHeight( siDilogHight - siUseHight)	;

						m_bShowchat	=	false	;
						m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHAT)->SetFontIndex(4);
						m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHAT)->Refresh();
					}
					else if(m_bShowchat == false )
					{
						SI32 siHight = m_InterfaceFactory.GetEdit(MYFAMILYINFO_DLG_EDIT_CHAT)->GetHeight()	;
						SI32 siHight2 = m_InterfaceFactory.GetEdit(MYFAMILYINFO_DLG_EDIT_CHAT_INPUT)->GetHeight()	;
						SI32 siUseHight = siHight+ siHight2	;
						SI16 siDilogHight = GetHeight( )	;

						ShowChat(true)	;						
						SetHeight( siDilogHight + siUseHight)	;
						
						m_bShowchat	=	true	;
						m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHAT)->SetFontIndex(0);
						m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_SHOWHIDE_CHAT)->Refresh();
					}

				}
			}			
		}break;
	}
}

void CNFamilyInfoDlg::Action()
{
	cltClient* pclclient = (cltClient*)pclClient;

	CheckMateConnect();
	if ( pclclient->m_pDialog[ NFAMILYINFO_DLG ] != NULL )	// 다이얼로그 열려 있으면 체크 해서 보여준다
	{
		DisplayFamilyInfo();
	}
	//만약 팝업이 열려 있으면 계속 위치를 조정 한다.
	if ( pclclient->m_pDialog[ NBUTTONMENU_DLG ] != NULL )
	{
		if(pclclient->m_pDialog[ NBUTTONMENU_DLG ]->IsCreate()==true)
		{
			((CNButtonMenuDlg *)pclclient->m_pDialog[ NBUTTONMENU_DLG ])->SetPosDlg( pclclient->m_pDialog[ NFAMILYINFO_DLG ]->GetX(),pclclient->m_pDialog[ NFAMILYINFO_DLG ]->GetY(),
				pclclient->m_pDialog[ NFAMILYINFO_DLG ]->GetWidth(),
				pclclient->m_pDialog[ NFAMILYINFO_DLG ]->GetHeight())	;

		}

	}
}
void CNFamilyInfoDlg::DisplayFamilyInfo( void )
{
	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );if ( NULL == pclChar )	return;
	// 부모
	CButton* pclMyButton = m_InterfaceFactory.GetButton( FAMILYINFO_DLG_BUTTON_MY );if ( NULL == pclMyButton ){	return;}
	CButton* pclMateButton = m_InterfaceFactory.GetButton( FAMILYINFO_DLG_BUTTON_MATE );if ( NULL == pclMateButton ){	return;}

	if ( TRUE == pclChar->IsMarried() )	// 내가 부모다 .
	{
		// 내이름 설정
		NTCHARString32 strMyName = (TCHAR*)pclChar->pclCI->clBI.GetName();
		pclMyButton->SetText( strMyName );
		pclMyButton->Refresh();
		// 배우자 이름 설정
		NTCHARString32 strMateName = pclChar->pclMarriage->GetMateName();
		pclMateButton->SetText( strMateName );
		pclMateButton->Refresh();
		if ( pclClient->IsCountrySwitch( Switch_We_Are_Family ) == true )
		{
			// 자식들이 있으면 자식들 설정을 해준다
			for ( SI32 siSlot = 0 ; siSlot < MAX_CHILDREN_NUM ; siSlot++ )
			{
				CButton* pclChildrenButton = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st + siSlot );
				if( NULL == pclChildrenButton )	continue;
				TCHAR* strChildName = pclChar->pclFamily->GetChildrenName( siSlot )	;
				pclChildrenButton->SetText( strChildName );
				pclChildrenButton->Refresh();
			}

		}
	}
	// 결혼은 안했다 하지만 자신의 신분이 차일드 일떄
	else if ( MYCLASS_IN_FAMILY_CHILDREN == pclChar->GetMyFamilyClass() )	// 내가 자식일때 
	{
		if ( pclClient->IsCountrySwitch( Switch_We_Are_Family ) == true )
		{
			if(pclChar->pclFamily != NULL)
			{
				// 부모는 2명뿐이니 그냥 2 받는다.
				TCHAR *pParentName_Slot1	=	pclChar->pclFamily->GetParent_Name( 0 )	;
				TCHAR *pParentName_Slot2	=	pclChar->pclFamily->GetParent_Name( 1 )	;

				// 부모
				CButton* pclParentButton = m_InterfaceFactory.GetButton( FAMILYINFO_DLG_BUTTON_MY );if ( NULL == pclParentButton ){	return;}
				CButton* pclMateButton = m_InterfaceFactory.GetButton( FAMILYINFO_DLG_BUTTON_MATE );if ( NULL == pclMateButton ){	return;}
				pclParentButton->SetText(pParentName_Slot1)	;
				pclParentButton->Refresh();
				pclMateButton->SetText(pParentName_Slot2)	;
				pclMateButton->Refresh();
				bool Parent1_OnGame = pclChar->pclFamily->GetOnGame_Parent( 0 )	;
				bool Parent2_OnGame = pclChar->pclFamily->GetOnGame_Parent( 1 )	;
				if( Parent1_OnGame	== false )	// 부모 접속 여부 
				{
					pclParentButton->Enable( false)	;
				}
				else
				{
					pclParentButton->Enable( true)	;
				}
				if( Parent2_OnGame	== false )
				{
					pclMateButton->Enable( false)	;
				}
				else
				{
					pclMateButton->Enable( true)	;
				}
				for ( SI32 siSlot = 0 ; siSlot < MAX_CHILDREN_NUM ; siSlot++ )
				{
					CButton* pclChildrenButton = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st + siSlot );
					if( NULL == pclChildrenButton )	return ;
					TCHAR* strChildName = pclChar->pclFamily->GetChildrenName( siSlot )	;
					pclChildrenButton->SetText( strChildName );
					pclChildrenButton->Refresh();
					if( pclChar->pclFamily->GetOnGame_Children(siSlot)	== false )	pclChildrenButton->Enable( false)	;
					else if( pclChar->pclFamily->GetOnGame_Children(siSlot)	== true )	pclChildrenButton->Enable( true)	;
				}
			}
		}
		
		
	}
	else	// 이도 저도 아닐때
	{
		pclMyButton->Enable(false) ;
		pclMateButton->Enable(false) ;
		if ( pclClient->IsCountrySwitch( Switch_We_Are_Family ) == true )
		{
			// 결혼도 안했고 난 자식도 없으면 자식버튼 비활성화 
			for ( SI32 siSlot = 0 ; siSlot < MAX_CHILDREN_NUM ; siSlot++ )
			{
				CButton* pclChildrenButton = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st + siSlot );
				if( NULL == pclChildrenButton )	continue;
				pclChildrenButton->Enable(false) ;
			}

		}
	}

}

void CNFamilyInfoDlg::CheckMateConnect( void )
{
	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( FALSE == pclChar->IsMarried() )
	{
		return;
	}

	CButton* pclMateButton = m_InterfaceFactory.GetButton( FAMILYINFO_DLG_BUTTON_MATE );
	CButton* pclMYButton = m_InterfaceFactory.GetButton( FAMILYINFO_DLG_BUTTON_MY );
	if ( NULL == pclMYButton )	// 나 이미 접속 해있으니까 
	{
		return	;
	}
	if ( NULL == pclMateButton )
	{
		return;
	}
	if ( MARRIAGE_MATE_CONNECT_MODE_CONNECT <= pclChar->pclMarriage->GetConnectMode() )
	{
		pclMYButton->Enable(true)	;
		pclMateButton->Enable( true );

		for ( SI32 i = 0 ; i < MAX_CHILDREN_NUM; i++ )
		{
			CButton* pclChildrenButton = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st+i );
			if ( pclChildrenButton != NULL )
			{
				pclChildrenButton->Enable(true);
			}
		}
	}
	else
	{
		pclMYButton->Enable(true)	;
		pclMateButton->Enable( false );

		for ( SI32 i = 0 ; i < MAX_CHILDREN_NUM; i++ )
		{
			CButton* pclChildrenButton = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st+i );
			if(pclChildrenButton !=NULL)
			{
				pclChildrenButton->Enable(true);
			}
		}
	}
}

void CNFamilyInfoDlg::OnButtonClickMate()
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( FALSE == pclChar->IsMarried() )
	{
		return;
	}

	if ( MARRIAGE_MATE_CONNECT_MODE_NONE == pclChar->pclMarriage->GetConnectMode() )
	{
		return;
	}

	if ( false == pclChar->CanWarpFrom() )
	{
		return;
	}

	TCHAR*	pTitle		= GetTxtFromMgr( 3326 );

	TCHAR	szText[256]	= TEXT("");
	TCHAR*	pText		= GetTxtFromMgr(5185);

	StringCchPrintf( szText, 256, pText, pclChar->pclMarriage->GetMateName() );

	stMsgBox MsgBox;
	MsgBox.Set( this, pTitle, szText, MSGBOX_TYPE_YESNO, 1 );
	pclclient->SetMsgBox( &MsgBox, NULL, 0 );

}
void CNFamilyInfoDlg::WarpFamily(SI32 siTargetGamilyPersonID, TCHAR* TarGetCharName )
{
	cltClient* pclclient = (cltClient*)pclClient;						if ( NULL == pclclient )	return;

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );		if ( NULL == pclChar )		return;

	if ( false == pclChar->CanWarpFrom() )		return;

	siWarpTargetFamily	=	0	;
	siWarpTargetFamily	=	siTargetGamilyPersonID	;

	TCHAR*	pTitle		= GetTxtFromMgr( 3326 );

	TCHAR	szText[256]	= TEXT("");
	TCHAR*	pText		= GetTxtFromMgr(5185);

	StringCchPrintf( szText, 256, pText, TarGetCharName );

	stMsgBox MsgBox;
	MsgBox.Set( this, pTitle, szText, MSGBOX_TYPE_YESNO, 2 );
	pclclient->SetMsgBox( &MsgBox, NULL, 0 );

}

void CNFamilyInfoDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 1:	
		{
			if ( true == RetValue )
			{
				cltMsg clMsg( GAMEMSG_REQUEST_MARRIAGEMATE_WARP, 0 );
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
			}
		}
	case 2:	
		{
			if ( true == RetValue )
			{
				cltGameRequest_WarpFamily clinfo ( siWarpTargetFamily);
				cltMsg clMsg(GAMEMSG_REQUEST_FAMILY_WARP, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
	}
}
void CNFamilyInfoDlg::OpenPopUp( SI32 ButtonIndex, bool bParent,SI32 siPersonID , TCHAR* CharName )
{
	cltClient* pclclient = (cltClient*)pclClient;	if ( NULL == pclclient ){		return;	}
	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );	if ( NULL == pclChar ){	return;}

	CButton* pclButton = m_InterfaceFactory.GetButton( ButtonIndex );		
	if( NULL == pclButton )	return	;

	SI32 siMode = CLICK_MODE_PARENT_NONE	;

	if(bParent == true )	// 부모면
	{
		switch ( ButtonIndex )
		{
		case FAMILYINFO_DLG_BUTTON_MY:
		case FAMILYINFO_DLG_BUTTON_MATE:
			{
				// 나인지 아닌지 구분 한다. 나라면 탈퇴 내가 아니면 상대방 클릭
				if(pclChar->pclCI->GetPersonID() == siPersonID)
				{
					siMode	=	CLICK_MODE_PARENT_ME;
				}
				else
				{	
					siMode	= CLICK_MODE_PARENT_MATE;
				}

			}
		}
	}
	else	// 자식이면 
	{
		TCHAR* pChildName = pclButton->GetText()	;
		// 이럴때는 이름이 없는 빈공간 -빈공간은은 자신이 부모일때 새로운 자식을 등록하려고 할떄쓴다
		if ( _tcslen( pChildName ) <= 0 )	// 텍스트 길이가 0 이면 빈것이다.
		{
			// 부모가 빈공간을 찍었다.
			if(pclChar->IsMarried() == TRUE ){	siMode = CLICK_MODE_PARENT_NONE	;}
		}
		else{
			if(pclChar->IsMarried() ==  FALSE)
			{
				if (pclChar->GetMyFamilyClass() == MYCLASS_IN_FAMILY_CHILDREN )	// 빈공간이 아니고, 해당 버튼에 누군가 등록되어 있을때.
				{	
					// 나자신을 찍었다
					if(  pclChar->pclCI->GetPersonID() == pclclient->pclCM->GetPersonIDFromName(pclButton->GetText() ) )
					{	
						siMode =  CLICK_MODE_CHILDREN_ME	;	
					}
					else if( pclclient->pclCM->GetPersonIDFromName(pclButton->GetText() ) == pclChar->pclFamily->GetParentPersonIDfromName(pChildName  ))// 자식이 부모를 찍었따.
					{
						siMode = CLICK_MODE_CHILDREN_PARENT	;
					}
					// 다른 자식을 찍었다.
					else
					{	
						siMode =  CLICK_MODE_CHILDREN_OTHER	;	
					}
				}
			}
			else
			{
				//  부모가 자식을 클릭 했다.
				siMode =  CLICK_MODE_PARENT_CHILDREN	;

			}
		}
	}
	if ( siMode == CLICK_MODE_PARENT_NONE )
	{
		if( pclChar->IsMarried()	==	TRUE )	
		{
			if ( pclclient->m_pDialog[ NFAMILY_DLG_ADD_CHILDREN ] == NULL )
			{
				pclclient->CreateInterface( NFAMILY_DLG_ADD_CHILDREN );
			}

			if ( pclclient->m_pDialog[ NFAMILY_DLG_ADD_CHILDREN ])
			{
				pclclient->m_pDialog[ NFAMILY_DLG_ADD_CHILDREN ]->Show( true ) ;
			}

		}
	}
	else
	{
		if ( pclclient->m_pDialog[ NBUTTONMENU_DLG ] == NULL )
		{
			pclclient->CreateInterface( NBUTTONMENU_DLG );		

		}
		if ( pclclient->m_pDialog[ NBUTTONMENU_DLG ] )
		{
			((CNButtonMenuDlg *)pclclient->m_pDialog[ NBUTTONMENU_DLG ])->Set( pclclient->m_pDialog[ NFAMILYINFO_DLG ]->GetX(),pclclient->m_pDialog[ NFAMILYINFO_DLG ]->GetY(),
				siMode ,
				pclclient->m_pDialog[ NFAMILYINFO_DLG ]->GetWidth(),
				pclclient->m_pDialog[ NFAMILYINFO_DLG ]->GetHeight(),
				siPersonID
				,CharName
				)	;// 내가 부모이고 나를 찍었을때
		}
	}
}
void CNFamilyInfoDlg::SendChat( )
{
	cltClient *pclclient = (cltClient *)pclClient;
	CEdit* m_pInputEdit = m_InterfaceFactory.GetEdit( MYFAMILYINFO_DLG_EDIT_CHAT_INPUT )	;	

	TCHAR *ChatText = m_pInputEdit->GetText();


	if ( ChatText == NULL || _tcslen( ChatText ) < 1 )
	{
		return;
	}
#ifdef _IAF_EXPORT
	if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
	{
		TCHAR ResultBuf[1024] = "";
		if( 1 == IAF::Filtering( ChatText, ResultBuf, sizeof(ResultBuf) ) )
		{
			TCHAR* pTitle = GetTxtFromMgr(1853);
			TCHAR* pText = GetTxtFromMgr(1854);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			m_pInputEdit->SetText(TEXT(""));
			return;
		}
	}
#else
	if ( pclclient->m_pCAbuseFilter->CheckAbuse( ChatText ) == false )
	{
		TCHAR* pTitle = GetTxtFromMgr(1853);
		TCHAR* pText = GetTxtFromMgr(1854);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		m_pInputEdit->SetText(TEXT(""));
		return;
	}
#endif

	//가족 채팅 메세지를 날린다.
	cltGameRequest_ChatFamily clinfo ((TCHAR*)pclClient->pclCM->CR[1]->GetName() ,ChatText);
	cltMsg clMsg(GAMEMSG_REQUEST_FAMILY_CHAT, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	m_pInputEdit->Clear()	;
}
void CNFamilyInfoDlg::RecvChat( stChatEditData* szChatData )
{
	CChatEdit* m_pInputEdit = m_InterfaceFactory.GetChatEdit( MYFAMILYINFO_DLG_EDIT_CHAT)	;
    m_pInputEdit->Insert( szChatData )	;
}
void CNFamilyInfoDlg::Minimi(BOOL bMini)
{
	if(m_bMini == bMini)	return;
	cltClient *pclclient = (cltClient*)pclClient;
	m_bMini = bMini;
	if(m_bMini == TRUE)
	{	
		m_siOldPosX = GetX();
		m_siOldPosY	= GetY();

		m_siOldXSize = GetWidth();
		m_siOldYSize = GetHeight();

		SetMoveable(FALSE);
		SetFileName(NULL);
		SetTitle(NULL);
		SI16 PartyMinimizedSlot = 0, LetterMinimizedSlot = 0, HelpButtonMiniMizedSlot = 0, LoveLevelMiniMizedSlot = 0 , FamilyCookieEffect = 0 , PrivateChatMinimizedSlot = 0 , siDormancyMiniMizedSlot = 0;
		if( pclclient->m_pDialog[NPARTY_DLG])
			PartyMinimizedSlot = ((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->GetMinimizedSlot();

		if( pclclient->m_pDialog[NLETTER_DLG])
			LetterMinimizedSlot = ((CNLetterDlg*)pclclient->m_pDialog[NLETTER_DLG])->GetMinimizedSlot();

		if(pclClient->IsCountrySwitch(Switch_FreshManHelpDialog))
		{
			if( pclclient->m_pDialog[NHELPBUTTON_DLG])
				HelpButtonMiniMizedSlot = ((CNHelpButtonDlg*)pclclient->m_pDialog[NHELPBUTTON_DLG])->GetMinimizedSlot();
		}

		if ( pclclient->m_pDialog[NMARRIAGELOVELEVEL_DLG] )
			LoveLevelMiniMizedSlot = ((CNMarriageLoveLevelDlg*)pclclient->m_pDialog[NMARRIAGELOVELEVEL_DLG])->GetMinimizedSlot();

		if ( pclclient->m_pDialog[NFAMILYCOOKIELEVEL_DLG] )
			FamilyCookieEffect = ((CNFamilyCookieDlg*)pclclient->m_pDialog[NFAMILYCOOKIELEVEL_DLG])->GetMinimizedSlot();

		if( pclclient->m_pNPrivateChatDlgMgr)
			PrivateChatMinimizedSlot = pclclient->m_pNPrivateChatDlgMgr->GetMinimizedSlot();

		if ( pclclient->m_pDialog[NDORMANCYPOINT_DLG] )
			siDormancyMiniMizedSlot = ((CNDormancyPointDlg*)pclclient->m_pDialog[NDORMANCYPOINT_DLG])->GetMinimizedSlot();


		// 빈 공간을 찾는다.
		for(SI16 i = 1; i <= MAX_MINIMIZE_SLOT; i++)
		{
			if( i == PartyMinimizedSlot || i == LetterMinimizedSlot || i == HelpButtonMiniMizedSlot || i == LoveLevelMiniMizedSlot || i == FamilyCookieEffect || i == PrivateChatMinimizedSlot || i == siDormancyMiniMizedSlot)
				continue;
			m_siMinimizedSlot = i;
			break;
		}

		// 슬롯에 따른 x 좌표로 움직여 준다.
		SetDialogPos( pclclient->GetScreenXSize() - 40 - (m_siMinimizedSlot -1) * 45 , 215, 34, 34 );
		ShowParent(false);
		ShowChindren(false);
		ShowChat(false);

		CButton* pButton_Mini = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_EBUTTON_MINIDLG );
		pButton_Mini->Show(true);

	}
	else
	{
		SetMoveable(TRUE);
		SetDialogPos( m_siOldPosX, m_siOldPosY, m_siOldXSize, m_siOldYSize );
		SetFileName("ninterface/interface/interface_20x20_00_109.spr");
		ShowParent(true);
		ShowChindren(true);
		ShowChat(true);
		//-----------------------------------------------------------------------------------------------
		if ( m_bShowparents == true	)	// 부모님창이 보이고 있는중 
		{
			ShowParent(true)	;						
		}
		else if(m_bShowparents == false )// 부모님창이 사라짐
		{
			ShowParent(false)	;									
		}
		if ( m_bShowchildres == true	)	// 부모님창이 보이고 있는중 
		{
			ShowChindren(true)	;
		}
		else if(m_bShowchildres == false )// 부모님창이 사라짐
		{
			ShowChindren(false)	;			
		}
		if ( m_bShowchat == true	)
		{
			ShowChat(true)	;			
		}
		else if(m_bShowchat == false )
		{
			ShowChat(false)	;						
		}
		//-----------------------------------------------------------------------------------------------
		CButton* pButton_Mini = m_InterfaceFactory.GetButton( MYFAMILYINFO_DLG_EBUTTON_MINIDLG );
		pButton_Mini->Show(false);
	}
}

void CNFamilyInfoDlg::OpenPopUpSet(SI32 siButtonID )
{
	cltClient * pclclient = (cltClient*)pclClient;
	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar ){		return;		}

	CButton* pButton = m_InterfaceFactory.GetButton( siButtonID )	;
	if ( pButton != NULL )
	{
		SI32 siPersonId = pclChar->pclFamily->GetChildPersonIDfromName( pButton->GetText() )	;
		OpenPopUp( siButtonID, false ,siPersonId,pButton->GetText())	;	
	}
}
void CNFamilyInfoDlg::ShowParent( bool bShow )
{
	cltClient * pclclient = (cltClient*)pclClient;

	// 작았다가 커질때 내가 부모일때 아닐때 이건 셋팅 해줘야 한다
	cltCharClient* pCharClient = pclclient->pclCM->GetCharClient(1)	;

	if(pCharClient != NULL )
	{
		if(bShow == false )
		{
			m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MY)->Show(false);
			m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MATE)->Show(false);
			m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDIT_SHOWCHILD)->Show(false);
		}
		else
		{
			if( pCharClient->IsMarried() == TRUE )	// 내가 부모다 
			{
				m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MY)->Show(true);
				m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MATE)->Show(true);
				m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDIT_SHOWCHILD)->Show(false);
			}
			else	// 부모가 아니거나 자식이거나 가족이 없다.
			{
				if( pCharClient->pclFamily != NULL )
				{
					if ( pCharClient->pclFamily->GetMyClass_InFamily() == MYCLASS_IN_FAMILY_CHILDREN )// 내 등급이 자식이면 
					{
						m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MY)->Show(false);
						m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MATE)->Show(false);
						m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDIT_SHOWCHILD)->Show(true);
					}
					else	// 혹시나 기본 셋팅 
					{
						m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MY)->Show(true);
						m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MATE)->Show(true);
						m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDIT_SHOWCHILD)->Show(false);
					}
				}
				else
				{
					m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MY)->Show(false);
					m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MATE)->Show(false);
					m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDIT_SHOWCHILD)->Show(false);

				}
			}


		}
	}
	m_InterfaceFactory.GetButton(FAMILYINFO_DLG_BUTTON_MY)->Show( bShow)	;		
	m_InterfaceFactory.GetButton(FAMILYINFO_DLG_BUTTON_MATE)->Show( bShow)	;		
	m_InterfaceFactory.GetOutLine(FAMILYINFO_DLG_OUTLINE_NONAME1)->Show( bShow)	;		
	m_InterfaceFactory.GetButton(FAMILYINFO_DLG_BUTTON_MY)->Show( bShow)	;		
	m_InterfaceFactory.GetButton(FAMILYINFO_DLG_BUTTON_MY)->Show( bShow)	;
}
void CNFamilyInfoDlg::ShowChindren( bool bShow)
{
	m_InterfaceFactory.GetEdit(MYFAMILYINFO_DLG_EDIT_CHILDREN)->Show( bShow)	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st)->Show( bShow)	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_2nd)->Show( bShow)	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_3rd)->Show( bShow)	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_4th)->Show( bShow)	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_5th)->Show( bShow)	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_6th)->Show( bShow)	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_7th)->Show( bShow)	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_8th)->Show( bShow)	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_9th)->Show( bShow)	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_10th)->Show( bShow)	;
	m_InterfaceFactory.GetOutLine(MYFAMILYINFO_DLG_OUTLINE_NONAME2)->Show( bShow)	;	
}
void CNFamilyInfoDlg::ShowChat( bool bShow)
{
	m_InterfaceFactory.GetChatEdit(MYFAMILYINFO_DLG_EDIT_CHAT)->Show( bShow)	;

	if ( bShow == true ) 
	{
		if(m_InterfaceFactory.GetChatEdit(MYFAMILYINFO_DLG_EDIT_CHAT)->IsShow() == true )
		{
			m_InterfaceFactory.GetChatEdit(MYFAMILYINFO_DLG_EDIT_CHAT)->Refresh()	;
			m_InterfaceFactory.GetChatEdit(MYFAMILYINFO_DLG_EDIT_CHAT)->RefreshScroll()	;
		}
	}
	m_InterfaceFactory.GetEdit(MYFAMILYINFO_DLG_EDIT_CHAT_INPUT)->Show( bShow)	;
}
void CNFamilyInfoDlg::MoveCParent( SI32 sihight)
{
	m_InterfaceFactory.GetButton(FAMILYINFO_DLG_BUTTON_MY)->Move(0,sihight )	;
	m_InterfaceFactory.GetButton(FAMILYINFO_DLG_BUTTON_MATE)->Move(0,sihight )	;
	m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDIT_SHOWCHILD)->Move(0,sihight )	;
	m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MY)->Move(0,sihight )	;
	m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MATE)->Move(0,sihight )	;
	m_InterfaceFactory.GetOutLine(FAMILYINFO_DLG_OUTLINE_NONAME1)->Move(0,sihight )	;
	m_InterfaceFactory.GetButton(FAMILYINFO_DLG_BUTTON_MY)->Move(0,sihight )	;
	m_InterfaceFactory.GetButton(FAMILYINFO_DLG_BUTTON_MY)->Move(0,sihight )	;

	m_InterfaceFactory.GetButton(FAMILYINFO_DLG_BUTTON_MY)->Refresh()	;
	m_InterfaceFactory.GetButton(FAMILYINFO_DLG_BUTTON_MATE)->Refresh()	;
	m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDIT_SHOWCHILD)->Refresh()	;
	m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MY)->Refresh()	;
	m_InterfaceFactory.GetEdit(FAMILYINFO_DLG_EDITBOX_MATE)->Refresh()	;
	m_InterfaceFactory.GetOutLine(FAMILYINFO_DLG_OUTLINE_NONAME1)->Refresh()	;
	m_InterfaceFactory.GetButton(FAMILYINFO_DLG_BUTTON_MY)->Refresh()	;
	m_InterfaceFactory.GetButton(FAMILYINFO_DLG_BUTTON_MY)->Refresh()	;
}
void CNFamilyInfoDlg::MoveChindren( SI32 sihight)
{
	m_InterfaceFactory.GetEdit(MYFAMILYINFO_DLG_EDIT_CHILDREN)->Move(0,sihight )	;
	m_InterfaceFactory.GetEdit(MYFAMILYINFO_DLG_EDIT_CHILDREN)->Refresh()	;

	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st)->Move(0,sihight )	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_2nd)->Move(0,sihight )	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_3rd)->Move(0,sihight )	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_4th)->Move(0,sihight )	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_5th)->Move(0,sihight )	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_6th)->Move(0,sihight )	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_7th)->Move(0,sihight )	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_8th)->Move(0,sihight )	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_9th)->Move(0,sihight )	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_10th)->Move(0,sihight )	;

	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_1st)->Refresh()	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_2nd)->Refresh()	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_3rd)->Refresh()	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_4th)->Refresh()	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_5th)->Refresh()	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_6th)->Refresh()	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_7th)->Refresh()	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_8th)->Refresh()	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_9th)->Refresh()	;
	m_InterfaceFactory.GetButton(MYFAMILYINFO_DLG_BUTTON_CHILDREN_10th)->Refresh()	;

	m_InterfaceFactory.GetOutLine(MYFAMILYINFO_DLG_OUTLINE_NONAME2)->Move(0,sihight )	;
}
void CNFamilyInfoDlg::MoveChat( SI32 sihight)
{
	m_InterfaceFactory.GetChatEdit(MYFAMILYINFO_DLG_EDIT_CHAT)->Move(0,sihight )	;
	if(m_InterfaceFactory.GetChatEdit(MYFAMILYINFO_DLG_EDIT_CHAT)->IsShow() == true )
	{
		m_InterfaceFactory.GetChatEdit(MYFAMILYINFO_DLG_EDIT_CHAT)->Refresh();
		m_InterfaceFactory.GetChatEdit(MYFAMILYINFO_DLG_EDIT_CHAT)->RefreshScroll();
	}
	m_InterfaceFactory.GetEdit(MYFAMILYINFO_DLG_EDIT_CHAT_INPUT)->Move(0,sihight )	;
}