#include "../Client/NInterface/NMagicInfoDlg/NewMagicInfoDlg.h"


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

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "..\..\..\CommonLogic\MagicMgr\MagicMgr.h"
#include "..\..\..\CommonLogic\MagicMgr\MagicAttribute\MagicAttribute.h"

#include "..\..\..\Common\Skill\Skill-Manager.h"
#include "Char\CharManager\CharManager.h"

#include "../../Lib/Sound/SoundMgr.h"
#include "../../Client/Music/music.h"

#include "../../Client/client.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgType-Item.h"

extern cltCommonLogic* pclClient;

CNNewMagicInfoDlg::CNNewMagicInfoDlg() : CDialogController()
{
	m_siCurrentButtonIndex	= 0;
	m_siSelectListIndex		= -1;
	m_siActiveMagicKind		= -1;
	m_siMagicKind			= -1;
	m_siParentMagicKind		= 0;		//[진성] 스킬북

	m_dwLastChangedMagic	= 0;

	m_CoolTimeStart			= 0;
	m_CoolTime				= 0;

	SI32 i;
	for( i = 0; i < 2; ++i )
		m_pList[i] = NULL;
	for( i = 0; i < 6; ++i )
		m_pMagicNameButton[i] = NULL;
	for( i = 0; i < 2; ++i )
		m_pButton[i] = NULL;

	m_pEdit = NULL;

	m_pKey				= NULL;
	m_pKeyExplain		= NULL;
	m_pMagicName		= NULL;
	m_pSubMagicName		= NULL;

	GP.LoadSprFromFile(TEXT("./Interface/MagicMgrDlg/MagicIcon.SPR"),		&m_SprMagicIcon );
	GP.LoadSprFromFile(TEXT("./Interface/MagicMgrDlg/MagicIcon_Out.SPR"),	&m_SprMagicIconOut );
	GP.LoadSprFromFile(TEXT("./Interface/MagicMgrDlg/MagicCoolTime.SPR"),	&m_SprMagicCoolTime );
	GP.LoadSprFromFile(TEXT("./gimg/Skillbook.SPR"),						&m_SprSkillBook );
}

CNNewMagicInfoDlg::~CNNewMagicInfoDlg()
{

	GP.FreeSpr( m_SprMagicIcon );
	GP.FreeSpr( m_SprMagicIconOut );
	GP.FreeSpr( m_SprMagicCoolTime );
	GP.FreeSpr( m_SprSkillBook );
	
	SI32 i;
	for( i = 0; i < 2; ++i )
	{
		if(m_pList[i])	delete m_pList[i];
	}

	for( i = 0; i < 6; ++i )
	{
		if(m_pMagicNameButton[i])	delete m_pMagicNameButton[i];
	}

	for( i = 0; i < 2; ++i )
	{
		if(m_pButton[i])	delete m_pButton[i];
	}

	for( i = 0; i < MAGIC_NUM; ++i )
	{
		if( m_pMagicImageStatic[i] )	delete m_pMagicImageStatic[i];
	}

	if( m_pSelcetMagicImageStatic )		delete m_pSelcetMagicImageStatic;
	if( m_pUpgradeMagicImageStatic )	delete m_pUpgradeMagicImageStatic;
	if( m_pEdit )						delete m_pEdit;	
	if( m_pEdit2 )						delete m_pEdit2;
	if( m_pKey )						delete m_pKey;	
	if( m_pKeyExplain )					delete m_pKeyExplain;
	if( m_pMagicName )					delete m_pMagicName;
	if( m_pSubMagicName )				delete m_pSubMagicName;
	
	if( m_pMagicCancelButton )			delete m_pMagicCancelButton;
}

void CNNewMagicInfoDlg::Clear( void )
{
	m_siCurrentButtonIndex  = MAGIC_TYPE_SWORD;
	m_siSelectListIndex		= MAGIC_TYPE_SWORD;
	m_siActiveMagicKind		= -1;
	m_siMagicKind			= -1;
	m_siParentMagicKind		=  0;		//[진성] 스킬북

	SetMagicInfo(1);
}

void CNNewMagicInfoDlg::Create()	
{
	if ( IsCreate() )	return;

	//if( false == pclClient->IsCountrySwitch(Switch_SkillBook) )	return;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NMagicInfoDlg/DLG_NNewMagicInfo.ddf"));
	file.CreatePopupDialog( this, NNEWMAGICINFO_DLG, TEXT("dialog_NMagicInfo"), StaticCallBackDialogNNewMagicInfoDlg);

	SI32 i;
	for( i = 0; i < 2; ++i )	m_pList[i]				= new CList( this );
	for( i = 0; i < 6; ++i )	m_pMagicNameButton[i]	= new CButton( this );
	for( i = 0; i < 2; ++i )	m_pButton[i]			= new CButton( this );
	
	for( i = 0; i < MAGIC_NUM; ++i )
		m_pMagicImageStatic[i]  = new CImageStatic( this );
	
	m_pSelcetMagicImageStatic	= new CImageStatic( this );	
	m_pUpgradeMagicImageStatic	= new CImageStatic( this );
	

	m_pEdit		= new CEdit( this );
	m_pEdit2	= new CEdit( this );

	m_pKey				= new CStatic( this );
	m_pKeyExplain		= new CStatic( this );
	m_pMagicName		= new CStatic( this );
	m_pSubMagicName		= new CStatic( this );
	m_pMagicCancelButton = new CButton( this );

	file.LoadFile(TEXT("NInterface/Data/NMagicInfoDlg/DLG_NMagicInfo.ddf"));

	file.CreateControl( m_pList[0],					NMAGICINFO_DIALOG_LIST1,   						TEXT("listbox_NONAME1")); 
	file.CreateControl( m_pList[1],					NMAGICINFO_DIALOG_LIST2,   						TEXT("listbox_NONAME2")); 
	file.CreateControl( m_pMagicNameButton[0],		NMAGICINFO_DIALOG_MAGICNAME_BUTTON1,   			TEXT("button_NONAME1")); 
	file.CreateControl( m_pMagicNameButton[1],		NMAGICINFO_DIALOG_MAGICNAME_BUTTON2,   			TEXT("button_NONAME2")); 
	file.CreateControl( m_pMagicNameButton[2],		NMAGICINFO_DIALOG_MAGICNAME_BUTTON3,   			TEXT("button_NONAME3")); 
	file.CreateControl( m_pMagicNameButton[3],		NMAGICINFO_DIALOG_MAGICNAME_BUTTON4,   			TEXT("button_NONAME4")); 
	file.CreateControl( m_pMagicNameButton[4],		NMAGICINFO_DIALOG_MAGICNAME_BUTTON5,   			TEXT("button_NONAME5")); 
	file.CreateControl( m_pMagicNameButton[5],		NMAGICINFO_DIALOG_MAGICNAME_BUTTON6,   			TEXT("button_NONAME6")); 
	file.CreateControl( m_pButton[0],				NMAGICINFO_DIALOG_BUTTON1,						TEXT("button_NONAME7")); 
	file.CreateControl( m_pButton[1],				NMAGICINFO_DIALOG_BUTTON2,						TEXT("button_NONAME8")); 
	file.CreateControl( m_pEdit,					NMAGICINFO_DIALOG_EDIT,							TEXT("editbox_NONAME1")); 
	
	file.CreateControl( m_pMagicImageStatic[0],		NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC0,			TEXT("imagestatic_NONAME0")); 
	file.CreateControl( m_pMagicImageStatic[1],		NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC1,			TEXT("imagestatic_NONAME1")); 
	file.CreateControl( m_pMagicImageStatic[2],		NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC2,			TEXT("imagestatic_NONAME2")); 
	file.CreateControl( m_pMagicImageStatic[3],		NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC3,			TEXT("imagestatic_NONAME3")); 
	file.CreateControl( m_pMagicImageStatic[4],		NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC4,			TEXT("imagestatic_NONAME4")); 
	file.CreateControl( m_pMagicImageStatic[5],		NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC5,			TEXT("imagestatic_NONAME5")); 
	file.CreateControl( m_pMagicImageStatic[6],		NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC6,			TEXT("imagestatic_NONAME6")); 
	file.CreateControl( m_pMagicImageStatic[7],		NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC7,			TEXT("imagestatic_NONAME7")); 
	file.CreateControl( m_pMagicImageStatic[8],		NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC8,			TEXT("imagestatic_NONAME8")); 
	file.CreateControl( m_pMagicImageStatic[9],		NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC9,			TEXT("imagestatic_NONAME9")); 
	file.CreateControl( m_pSelcetMagicImageStatic,	NMAGICINFO_DIALOG_SELECT_MAGIC_IMAGESTATIC,		TEXT("imagestatic_select_magic")); 
	file.CreateControl( m_pUpgradeMagicImageStatic,	NMAGICINFO_DIALOG_UPGRADE_MAGIC_IMAGESTATIC,	TEXT("imagestatic_upgrade_magic")); 
	file.CreateControl( m_pEdit2,					NMAGICINFO_DIALOG_EDIT2,						TEXT("editbox_NONAME2")); 
	file.CreateControl( m_pKey,						NMAGICINFO_DIALOG_KEY_STATIC,					TEXT("statictext_NONAME1")); 
	file.CreateControl( m_pKeyExplain,				NMAGICINFO_DIALOG_KEY_EXPLAIN_STATIC,			TEXT("statictext_NONAME2")); 
	file.CreateControl( m_pMagicName,				NMAGICINFO_DIALOG_MAGIC_NAME,					TEXT("statictext_NONAME3")); 
	file.CreateControl( m_pSubMagicName,			NMAGICINFO_DIALOG_SUB_MAGIC_NAME,				TEXT("statictext_NONAME4")); 
	file.CreateControl( m_pMagicCancelButton,		NMAGICINFO_DIALOG_MASIC_CALCEL_BUTTON,			TEXT("button_NONAME9")); 

	m_pKey->SetTextPrintOption( DT_VCENTER | DT_CENTER );
	m_pKey->Refresh();

	m_pKeyExplain->SetTextPrintOption( DT_VCENTER | DT_CENTER );
	m_pKeyExplain->Refresh();

	m_pMagicName->SetTextPrintOption( DT_VCENTER | DT_CENTER );
	m_pMagicName->Refresh();

	m_pSubMagicName->SetTextPrintOption( DT_VCENTER | DT_CENTER );
	m_pSubMagicName->Refresh();
	
	m_siCurrentButtonIndex = MAGIC_TYPE_SWORD;
	// 버튼 글자 처리 선택안된건 회색톤으로
	SetButtonTextColor(0);

	EnableButton(m_pButton[0], false);

	SI16 siTop0Width = 150;
	SI16 siTop1Width = m_pList[0]->GetWidth() - siTop0Width;
	TCHAR* pText= GetTxtFromMgr(3336);
	m_pList[0]->SetColumn(0, siTop0Width, pText);
	pText= GetTxtFromMgr(3337);
	m_pList[0]->SetColumn(1,  siTop1Width, pText);
	m_pList[0]->SetBorder(TRUE);
	m_pList[0]->SetBKColorUse(TRUE);
	m_pList[0]->Refresh();

	SI16 siBottom0Width = 180;
	SI16 siBottom1Width = m_pList[1]->GetWidth() - siBottom0Width;
	pText= GetTxtFromMgr(2413);
	m_pList[1]->SetColumn(0, siBottom0Width, pText);
	pText= GetTxtFromMgr(3338);
	m_pList[1]->SetColumn(1, siBottom1Width, pText);
	m_pList[1]->SetBorder(TRUE);
	m_pList[1]->SetBKColorUse(TRUE);
	m_pList[1]->Refresh();

	ShowMagicList( m_siCurrentButtonIndex );

	m_pList[0]->SetSelectIndex(0);
	SetMagic();
	SetExplain();
	
	Hide();
	
	// 보이지 않지만 이 리스트의 정보를 사용한다.
	m_pList[0]->Show(false);
	m_pList[0]->GetScroll()->Show(false);
}


void CALLBACK CNNewMagicInfoDlg::StaticCallBackDialogNNewMagicInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNNewMagicInfoDlg *pThis = (CNNewMagicInfoDlg *)pControllerMgr;
	pThis->CallBackDialogNNewMagicInfoDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNNewMagicInfoDlg::CallBackDialogNNewMagicInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
		}
		break;
	case NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC0:	
		{	
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					SetMagicInfo(1);
				}
				break;
			}
		}	
		break;
	case NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC1:	
		{	
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					SetMagicInfo(2);
				}
				break;
			}
		}	
		break;
	case NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC2:	
		{	
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					SetMagicInfo(3);
				}
				break;
			}
		}	
		break;
	case NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC3:	
		{	
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					SetMagicInfo(4);
				}
				break;
			}
		}	
		break;
	case NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC4:	
		{	
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					SetMagicInfo(5);
				}
				break;
			}
		}	
		break;
	case NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC5:	
		{	
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					SetMagicInfo(6);

				}
				break;
			}
		}	
		break;
	case NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC6:	
		{	
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					SetMagicInfo(7);

				}
				break;
			}
		}	
		break;
	case NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC7:	
		{	
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					SetMagicInfo(8);

				}
				break;
			}
		}	
		break;
	case NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC8:	
		{	
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					SetMagicInfo(9);

				}
				break;
			}
		}	
		break;
	case NMAGICINFO_DIALOG_MAGIC_IMAGESTATIC9:	
		{	
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					SetMagicInfo(0);
				}
				break;
			}
		}	
		break;
	
	case NMAGICINFO_DIALOG_MAGICNAME_BUTTON1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siCurrentButtonIndex = MAGIC_TYPE_SWORD;
					// 버튼 글자 처리 선택안된건 회색톤으로
					SetButtonTextColor(0);

					EnableButton(m_pButton[0], false);

					ShowMagicList( m_siCurrentButtonIndex );

					SetMagicInfo(1);
				}
				break;
			}
		}
		break;

	case NMAGICINFO_DIALOG_MAGICNAME_BUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siCurrentButtonIndex = MAGIC_TYPE_SPEAR;
					// 버튼 글자 처리 선택안된건 회색톤으로
					SetButtonTextColor(1);
					EnableButton(m_pButton[0], false);

					ShowMagicList( m_siCurrentButtonIndex );
					SetMagicInfo(1);
				}
				break;
			}
		}
		break;

	case NMAGICINFO_DIALOG_MAGICNAME_BUTTON3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siCurrentButtonIndex = MAGIC_TYPE_AXE;
					// 버튼 글자 처리 선택안된건 회색톤으로
					SetButtonTextColor(2);

					EnableButton(m_pButton[0], false);

					ShowMagicList( m_siCurrentButtonIndex );
					SetMagicInfo(1);
				}
				break;
			}
		}
		break;

	case NMAGICINFO_DIALOG_MAGICNAME_BUTTON4:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siCurrentButtonIndex = MAGIC_TYPE_STAFF;
					// 버튼 글자 처리 선택안된건 회색톤으로
					SetButtonTextColor(3);

					EnableButton(m_pButton[0], false);

					ShowMagicList( m_siCurrentButtonIndex );
					SetMagicInfo(1);
				}
				break;
			}
		}
		break;

	case NMAGICINFO_DIALOG_MAGICNAME_BUTTON5:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siCurrentButtonIndex = MAGIC_TYPE_BOW;
					// 버튼 글자 처리 선택안된건 회색톤으로
					SetButtonTextColor(4);

					EnableButton(m_pButton[0], false);

					ShowMagicList( m_siCurrentButtonIndex );
					SetMagicInfo(1);
				}
				break;
			}
		}
		break;

	case NMAGICINFO_DIALOG_MAGICNAME_BUTTON6:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siCurrentButtonIndex = MAGIC_TYPE_GUN;
					// 버튼 글자 처리 선택안된건 회색톤으로
					SetButtonTextColor(5);

					EnableButton(m_pButton[0], false);

					ShowMagicList( m_siCurrentButtonIndex );
					SetMagicInfo(1);
				}
				break;
			}
		}
		break;

	case NMAGICINFO_DIALOG_BUTTON1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetNewMagic(-1);
				}
				break;
			}
		}
		break;

	case NMAGICINFO_DIALOG_BUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					Hide();
				}
				break;
			}
		}
		break;
	case NMAGICINFO_DIALOG_MASIC_CALCEL_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltCharClient*			pclChar = pclClient->pclCM->GetCharClient( 1 );
					if( NULL == pclChar ) return;
					cltSkillBookSubMagic*	pclSkillBookSubMagic = pclChar->pclCharSkillBookInfo->FindSkillBookSubMagicFromParentMagic( m_siParentMagicKind );

					if( pclSkillBookSubMagic )
					{
						cltGameMsgRequest_SkillBook_SubMagic_Del clSkillSubMagicDel( pclSkillBookSubMagic );
						cltMsg clMsg( GAMEMSG_REQUEST_SKILLBOOK_SUBMAGIC_DEL, PACKET(clSkillSubMagicDel) );
						pclClient->SendMsgToServer( (sPacketHeader*)&clMsg ); 					
					}
				}
				break;
			}
		}
		break;
	}
}

void CNNewMagicInfoDlg::Hide()
{
	SetTimerOnOff(false);

	CControllerMgr::Show( false );

	return;
}

void CNNewMagicInfoDlg::Show()
{
	
	SetTimerOnOff(true);

	CControllerMgr::Show( true );

	SetMagic();

	ShowMagicList( m_siCurrentButtonIndex );

	pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_MAGICINFODLG);

	return;
}

void CNNewMagicInfoDlg::Action()
{

}

void CNNewMagicInfoDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if(m_siMagicKind < 0)  
		return;
}

void CNNewMagicInfoDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	DrawMagic( ScreenRX, ScreenRY );
}

// 마법 기술레벨이 변경 되었는지 안 되었는지만 체크하면 될듯.
bool CNNewMagicInfoDlg::IsChanged()
{
	return false;
}

BOOL CNNewMagicInfoDlg::SetMagic()
{
	SI16 MagicKind= GetMagicKind();

	if( MagicKind < MAGIC_INDEX_START ) return FALSE;

	SI32 siSkillUnique = 0;
	SI32 siAttackType  = 0;
	switch( m_siCurrentButtonIndex )
	{
		case MAGIC_TYPE_SWORD:		siSkillUnique = SKILL_USESWORD1	;		siAttackType = ATTACKTYPE_SWORD; break;// 검술break;// 검술
		case MAGIC_TYPE_SPEAR:		siSkillUnique = SKILL_USESPEAR1	;		siAttackType = ATTACKTYPE_SPEAR; break;// 창술break;// 창술
		case MAGIC_TYPE_BOW:		siSkillUnique = SKILL_USEBOW1	;		siAttackType = ATTACKTYPE_BOW;   break;// 궁술break;// 궁술
		case MAGIC_TYPE_AXE:		siSkillUnique = SKILL_USEAXE1	;		siAttackType = ATTACKTYPE_AXE;	 break;// 부술(도끼술)break;// 부술(도끼술)
		case MAGIC_TYPE_GUN:		siSkillUnique = SKILL_USEGUN1	;		siAttackType = ATTACKTYPE_GUN;	 break;// 총술break;// 총술
		case MAGIC_TYPE_STAFF:		siSkillUnique = SKILL_USESTAFF1	;		siAttackType = ATTACKTYPE_STAFF; break;// 지팡이술break;// 지팡이술
		case MAGIC_TYPE_MON:		siSkillUnique = 0;					break;
		case MAGIC_TYPE_SOULGUARD:	siSkillUnique = 0;					break;
		case MAGIC_TYPE_SKILL_BOOK: 
			{
				cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
				if( NULL == pclChar ) return -1;

				cltSkillBookMagic*	pclSkillBookMagic = pclChar->pclCharSkillBookInfo->GetSkillBookMagic( pclChar->GetAttackType() );	
				if( NULL == pclSkillBookMagic ) return false;

				siSkillUnique = pclSkillBookMagic->GetSkillUnique();
			}
			break;
		default :					return FALSE;
	}

	BOOL bWeaponTypeOK = FALSE;
	// 현재 장착된 무기의 Type에 따른 설정가능성을 타진한다.
	switch( pclClient->pclCM->CR[1]->GetAttackType() )
	{
		case ATTACKTYPE_SWORD:		if(m_siCurrentButtonIndex == MAGIC_TYPE_SWORD) 	bWeaponTypeOK = TRUE;	break;
		case ATTACKTYPE_SPEAR:		if(m_siCurrentButtonIndex == MAGIC_TYPE_SPEAR) 	bWeaponTypeOK = TRUE;	break;
		case ATTACKTYPE_AXE:		if(m_siCurrentButtonIndex == MAGIC_TYPE_AXE)	bWeaponTypeOK = TRUE;	break;
		case ATTACKTYPE_STAFF:		if(m_siCurrentButtonIndex == MAGIC_TYPE_STAFF) 	bWeaponTypeOK = TRUE;	break;
		case ATTACKTYPE_STAFF1:		if(m_siCurrentButtonIndex == MAGIC_TYPE_STAFF) 	bWeaponTypeOK = TRUE;	break;
		case ATTACKTYPE_STAFF2:		if(m_siCurrentButtonIndex == MAGIC_TYPE_STAFF) 	bWeaponTypeOK = TRUE;	break;
		case ATTACKTYPE_NEW_STAFF2:	if(m_siCurrentButtonIndex == MAGIC_TYPE_STAFF) 	bWeaponTypeOK = TRUE;	break;
		case ATTACKTYPE_BOW:		if(m_siCurrentButtonIndex == MAGIC_TYPE_BOW)	bWeaponTypeOK = TRUE;	break;
		case ATTACKTYPE_BOW1:		if(m_siCurrentButtonIndex == MAGIC_TYPE_BOW)	bWeaponTypeOK = TRUE;	break;
		case ATTACKTYPE_NEW_BOW1:	if(m_siCurrentButtonIndex == MAGIC_TYPE_BOW)	bWeaponTypeOK = TRUE;	break;
		case ATTACKTYPE_GUN:		if(m_siCurrentButtonIndex == MAGIC_TYPE_GUN)	bWeaponTypeOK = TRUE;	break;
		case ATTACKTYPE_GUN1:		if(m_siCurrentButtonIndex == MAGIC_TYPE_GUN)	bWeaponTypeOK = TRUE;	break;
		case ATTACKTYPE_NEW_GUN1:	if(m_siCurrentButtonIndex == MAGIC_TYPE_GUN)	bWeaponTypeOK = TRUE;	break;
		default:					bWeaponTypeOK = FALSE;	break;
	}

	// 현재 장착된 무기의 Type에 따른 설정가능성을 타진한다.
	if( bWeaponTypeOK )
	{
		SI32 SkillLevel = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0] );
		if( SkillLevel > 0 )
		{
			stMagicAttributeData* pMagicAttributeData = pclClient->pclMagicManager->GetMagicAttribute( MagicKind );
			if( NULL == pMagicAttributeData ) return FALSE;

			if ( SkillLevel >= pMagicAttributeData->si16MagicSkillLevel )
			{
				EnableButton(m_pButton[0], true);
			}
			else
			{
				EnableButton(m_pButton[0], false);
			}
		}
		else
		{
			EnableButton(m_pButton[0], false);
		}
	}
	else
	{
		EnableButton(m_pButton[0], false);
	}

	return TRUE;
}

SI16 CNNewMagicInfoDlg::GetMagicKind()
{
	if ( m_siSelectListIndex < 0 )	return -1;

	// 스킬북 마법일 때.
	if( SKILLBOOK_MAGIC == m_siSelectListIndex ) 
	{
		cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
		if( NULL == pclChar ) return -1;
		
		if( pclChar->pclCharSkillBookInfo->IsSkillBookMagic() )
		{
			cltSkillBookMagic*	pclSkillBookMagic = pclChar->pclCharSkillBookInfo->GetSkillBookMagic( pclChar->GetAttackType() );	
			if( NULL == pclSkillBookMagic ) return false;

			return pclSkillBookMagic->GetMagicKind();
		}
	}
	else
	{
		SI32 MagicKind = pclClient->pclMagicManager->GetMagicAttributePtr()->FindRefFromName( m_pList[0]->GetText( m_siSelectListIndex, 0 ), m_siCurrentButtonIndex );
		if( MagicKind > 0 ) return MagicKind;
	}
 		
	return -1;
}

SI16 CNNewMagicInfoDlg::GetMagicKind( SI32 key )
{
	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();

	SI32 index = -1;
	// 현재 장착된 무기의 Type에 따른 설정가능성을 타진한다.
	switch( pclClient->pclCM->CR[1]->GetAttackType() )
	{
		case ATTACKTYPE_SWORD:		index = MAGIC_TYPE_SWORD;	break;
		case ATTACKTYPE_SPEAR:		index = MAGIC_TYPE_SPEAR;	break;
		case ATTACKTYPE_AXE:		index = MAGIC_TYPE_AXE;		break;
		case ATTACKTYPE_STAFF:		index = MAGIC_TYPE_STAFF;	break;
		case ATTACKTYPE_STAFF1:		index = MAGIC_TYPE_STAFF;	break;
		case ATTACKTYPE_STAFF2:		index = MAGIC_TYPE_STAFF;	break;
		case ATTACKTYPE_NEW_STAFF2:	index = MAGIC_TYPE_STAFF;	break;
		case ATTACKTYPE_BOW:		index = MAGIC_TYPE_BOW;		break;
		case ATTACKTYPE_BOW1:		index = MAGIC_TYPE_BOW;		break;
		case ATTACKTYPE_NEW_BOW1:	index = MAGIC_TYPE_BOW;		break;
		case ATTACKTYPE_GUN:		index = MAGIC_TYPE_GUN;		break;
		case ATTACKTYPE_GUN1:		index = MAGIC_TYPE_GUN;		break;
		case ATTACKTYPE_NEW_GUN1:	index = MAGIC_TYPE_GUN;		break;
		default:					return -1;					break;
	}

	SI32 Start, End;
	BOOL bSuccess = pMagicAttributePtr->GetMagicStartEnd( index, Start, End );
	if( bSuccess == FALSE ) return -1;

	SI32					siCount				= 1;
	stMagicAttributeData*	pMagicAttributeData = NULL;
	for (SI32 i = Start; i<= End; ++i )
	{
		pMagicAttributeData = pMagicAttributePtr->GetMagicAttribute(i);
		// 유저가 사용 가능한 마법주에서 선택한다.
		if ( pMagicAttributeData->si16MagicUserUsing == 1)
		{
			if(key == siCount)	return i;
			siCount++;
		}		
	}	

	return -1;
}

void CNNewMagicInfoDlg::ShowMagicList( SI32 SelectButtonIdex ) // 마법이름과 단축키
{			
	// ListView를 초기화한다.
	m_pList[0]->Clear();

	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();

	SI32 Start, End;
	BOOL bSuccess = pMagicAttributePtr->GetMagicStartEnd( SelectButtonIdex, Start, End );
	if(bSuccess == FALSE) return;

	AddMagicList( "", 0 );

	SI32					siCount				= 1;
	stMagicAttributeData*	pMagicAttributeData = NULL;
	for( SI32 i = Start; i<= End; ++i )
	{
		pMagicAttributeData = pMagicAttributePtr->GetMagicAttribute(i);

		// 유저가 사용 가능한 마법을 보여줌
		if ( pMagicAttributeData->si16MagicUserUsing == 1)
		{
			AddMagicList( pMagicAttributeData->strMagicName, siCount );
			siCount++;
		}		
	}	
}

void CNNewMagicInfoDlg::AddMagicList( TCHAR *MagicName, SI32 index )
{
	stListBoxItemData Data;

	if ( MagicName == NULL || _tcslen( MagicName ) < 1 )
	{
		Data.Set(0, TEXT(""));
		m_pList[0]->AddItem( &Data );
		m_pList[0]->Refresh();
		return;
	}
	else
	{
		Data.Set(0, MagicName);

		TCHAR buffer[256]=TEXT("");
		StringCchPrintf( buffer, 256, TEXT("CTRL+%d"), index );
		//		ListViewInsertItem(m_hMagicList, -1 , index, 1, buffer, -1);
		Data.Set(1, buffer);

		m_pList[0]->AddItem( &Data );
		m_pList[0]->Refresh();
	}
	return;
}

void CNNewMagicInfoDlg::SetExplain()
{
	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	//=======================================================
	// MagicDialog의 설정된 마법이 Valid한지 검사한다.
	//=======================================================
	SI16 SelectedIndex = m_siSelectListIndex;
	if ( SelectedIndex < 0 ){	return;	}

	TCHAR Buffer[256 ] = TEXT("");
	CMagicAttribute*	pMagicAttributePtr	= pclClient->pclMagicManager->GetMagicAttributePtr();
	SI16				MagicKind			= 0;
	SI16				Group				= 0;

	if( SKILLBOOK_MAGIC == SelectedIndex )
	{
		if( NULL == pclChar ) return;

		if( pclChar->pclCharSkillBookInfo->IsSkillBookMagic() )
		{
			SI32 siAttackType  = 0;
			switch( m_siCurrentButtonIndex )
			{
				case MAGIC_TYPE_SWORD:		siAttackType = ATTACKTYPE_SWORD; break;// 검술
				case MAGIC_TYPE_SPEAR:		siAttackType = ATTACKTYPE_SPEAR; break;// 창술
				case MAGIC_TYPE_BOW:		siAttackType = ATTACKTYPE_BOW;   break;// 궁술
				case MAGIC_TYPE_AXE:		siAttackType = ATTACKTYPE_AXE;	 break;// 부술(도끼술)
				case MAGIC_TYPE_GUN:		siAttackType = ATTACKTYPE_GUN;	 break;// 총술
				case MAGIC_TYPE_STAFF:		siAttackType = ATTACKTYPE_STAFF; break;// 지팡이술
			}			
			
			cltSkillBookMagic*	pclSkillBookMagic = pclChar->pclCharSkillBookInfo->GetSkillBookMagic( siAttackType );	
			if( NULL == pclSkillBookMagic ) return;

			MagicKind = pclSkillBookMagic->GetMagicKind();

			if( 0 >= MagicKind) return;

			Group = pMagicAttributePtr->GetMagicGroup(MagicKind);
			
			m_pKeyExplain->SetText( TEXT("CTRL+0") );
		}
	}
	else
	{
		MagicKind = pMagicAttributePtr->FindRefFromName(m_pList[0]->GetText(SelectedIndex, 0), m_siCurrentButtonIndex);
		if(MagicKind < 0) return;

		Group = pMagicAttributePtr->GetMagicGroup(MagicKind);

		m_pKeyExplain->SetText( m_pList[0]->GetText(SelectedIndex, 1) );
	}

	m_siMagicKind = Group;

	m_siParentMagicKind = MagicKind;	//[진성] 스킬북

	stMagicAttributeData* pMagicAttributeData = pMagicAttributePtr->GetMagicAttribute( m_siParentMagicKind );
	if( NULL == pMagicAttributeData ) return;

	m_pMagicName->SetText( pMagicAttributeData->strMagicName );

	SI32 siItemUnique = 0;
	SI32 siImageNum	  = 0;
	SI32 siTextNum    = 0;
	stMagicAttributeData*	pSubMagicAttributeData = NULL;
	cltSkillBookManager*	pclSkillBookManager    = pclClient->pclItemManager->pclSkillBookManager;
	if( pclSkillBookManager->IsSubMagicFromParentMagicKind( m_siParentMagicKind ) )
	{
		cltSkillBookSubMagic* pclSkillBookSubMagic = pclChar->pclCharSkillBookInfo->FindSkillBookSubMagicFromParentMagic( m_siParentMagicKind );
		if( pclSkillBookSubMagic )
		{
			pclSkillBookManager->GetSkillBookUniqueFromMagicKind( m_siParentMagicKind, pclSkillBookSubMagic->GetMagicKind(), siItemUnique, siImageNum, siTextNum );

			m_pSubMagicName->SetText( pMagicAttributePtr->GetMagicAttribute( pclSkillBookSubMagic->GetMagicKind() )->strMagicName );

			pSubMagicAttributeData = pMagicAttributePtr->GetMagicAttribute( pclSkillBookSubMagic->GetMagicKind() ); 
			
			NTCHARString256 SkillBookText( GetTxtFromMgr( siTextNum ) );
			SkillBookText.Replace( "#magicname#", pMagicAttributePtr->GetMagicAttribute(m_siParentMagicKind)->strMagicName ); 
			m_pEdit2->SetText( SkillBookText );
		}
		else
		{
			m_pSubMagicName->SetText( GetTxtFromMgr(30520) );
			m_pEdit2->SetText( GetTxtFromMgr( 30522 ) );
			
		}
	}
	else 
	{
		m_pEdit2->SetText( "" );
	}

	SI32 siSkillUnique = 0;
	switch(m_siCurrentButtonIndex)
	{
	case MAGIC_TYPE_SWORD:		siSkillUnique = SKILL_USESWORD1	;	break;// 검술
	case MAGIC_TYPE_SPEAR:		siSkillUnique = SKILL_USESPEAR1	;	break;// 창술
	case MAGIC_TYPE_BOW:		siSkillUnique = SKILL_USEBOW1	;	break;// 궁술
	case MAGIC_TYPE_AXE:		siSkillUnique = SKILL_USEAXE1	;	break;// 부술(도끼술)
	case MAGIC_TYPE_GUN:		siSkillUnique = SKILL_USEGUN1	;	break;// 총술
	case MAGIC_TYPE_STAFF:		siSkillUnique = SKILL_USESTAFF1	;	break;// 지팡이술
	case MAGIC_TYPE_MON:		siSkillUnique = 0;					break;
	case MAGIC_TYPE_SOULGUARD:	siSkillUnique = 0;					break;
	case MAGIC_TYPE_SKILL_BOOK: siSkillUnique = 0;					break;

	default :				return;
	}


	// ListView를 초기화한다.
	m_pList[1]->Clear();

	SI32 SkillLevel		= pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0]);
	SI32 AddDamage		= pclClient->pclCM->CR[1]->clPB.GetAddMagicDamage();
	SI32 siOrganUnique	= pclClient->pclCM->CR[1]->pclCI->clHealthInfo.GetOrg(pMagicAttributeData->si16MagicOrganKind);
	SI32 CharLevel		= pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel();

	TCHAR explain[1024]  = TEXT("");
	TCHAR skillname[128] = TEXT("");;

	if(pclClient->pclSkillManager->GetSkillName(siSkillUnique, skillname) == true)
	{
		stListBoxItemData Data;
		if ( SkillLevel >= pMagicAttributeData->si16MagicSkillLevel )
		{
			// Skill Level에 의한 영향력을 주고 싶으면 영향력을 넣어 마나소비량을 계산한다.
			SI32 mana		= 0; 
			SI32 Min		= 0;
			SI32 Max		= 0;
			SI32 SkillMin   = 0; 
			SI32 SkillMax   = 0;
			SI32 HealthMin  = 0; 
			SI32 HealthMax  = 0;
			mana = pclClient->pclMagicManager->GetSpendMana( MagicKind, SkillLevel );
			stMagicInfo clMagicInfo( MagicKind, 0, 0, 0 );
			pclClient->pclMagicManager->GetMinMaxDamage( &clMagicInfo, AddDamage, SkillLevel, Min, Max, siOrganUnique );
			pclClient->pclMagicManager->GetMinMaxSkillDamage( MagicKind, SkillLevel, SkillMin, SkillMax );

			SI32 totalMagMinDmg = pclClient->pclCM->CR[1]->clPB.GetTotalMagicalMinAttackDmg();
			SI32 totalMagMaxDmg = pclClient->pclCM->CR[1]->clPB.GetTotalMagicalMaxAttackDmg();

			if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
			{
				HealthMin= Min-SkillMin-totalMagMinDmg-pMagicAttributeData->si32MagicMinDamage;
				HealthMax= Max-SkillMax-totalMagMaxDmg-pMagicAttributeData->si32MagicMaxDamage;
			}
			else
			{
				HealthMin= Min-SkillMin-AddDamage-pMagicAttributeData->si32MagicMinDamage;
				HealthMax= Max-SkillMax-AddDamage-pMagicAttributeData->si32MagicMaxDamage;
			}

			if ( (MAGIC_CASTTYPE_ACTIVE == pMagicAttributeData->si08MagicCastType) ||
				 (MAGIC_CASTTYPE_BUF    == pMagicAttributeData->si08MagicCastType)		)
			{
				SI32	siIndex = 0;
				TCHAR*	pText	= GetTxtFromMgr(2417);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2418);
				StringCchPrintf(explain, 1024, pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2419);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2420);
				if( pSubMagicAttributeData && pSubMagicAttributeData->si32MagicDelay )
				{
					SI32  siTemp = pMagicAttributeData->si32MagicDelay / 20.0f;

					StringCchPrintf(explain, 1024, pText, (float)(siTemp + (siTemp * pSubMagicAttributeData->si32MagicDelay) / 100) ); 
					Data.Set( 1, explain, COLORREF(0x0000aa00)); 
				}
				else
				{
					StringCchPrintf(explain, 1024, pText, pMagicAttributeData->si32MagicDelay/20.0f); 
					Data.Set( 1, explain);
				}
				
				
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2421);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2422);
				if( pSubMagicAttributeData && pSubMagicAttributeData->si16MagicOrganDamage )
				{
					SI32  siTemp  = pMagicAttributeData->si16MagicOrganDamage;

					StringCchPrintf(explain, 1024, pText, pMagicAttributeData->strMagicOrganName, siTemp + (siTemp * pSubMagicAttributeData->si16MagicOrganDamage) / 100 ); 
					Data.Set( 1, explain, COLORREF(0x0000aa00)); 
				}
				else
				{
					StringCchPrintf(explain, 1024, pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					Data.Set( 1, explain);
				}
								
				siIndex++;

				m_pList[1]->AddItem( &Data );
                Data.Init();

				pText = GetTxtFromMgr(2423);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2424);
				if( pSubMagicAttributeData && pSubMagicAttributeData->si32MagicSpendMana )
				{
					SI32  siTemp = pMagicAttributeData->si32MagicSpendMana;
					
					StringCchPrintf(explain, 1024, pText, mana, siTemp + (siTemp * pSubMagicAttributeData->si32MagicSpendMana) / 100 ); 
					Data.Set( 1, explain, COLORREF(0x0000aa00)); 
				}
				else
				{
					StringCchPrintf(explain, 1024, pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					Data.Set( 1, explain);
				}
				
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2425);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				if( pSubMagicAttributeData && pSubMagicAttributeData->si32MagicMinDamage && pSubMagicAttributeData->si32MagicMaxDamage )
				{
					StringCchPrintf(explain, 1024, TEXT("%d - %d"), Min, Max ); 
					Data.Set( 1, explain, COLORREF(0x0000aa00));   
				}
				else
				{
					StringCchPrintf(explain, 1024, TEXT("%d - %d"), Min, Max); 
					Data.Set( 1, explain);
				}
				
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				Data.Set( 0, TEXT(""));
				Data.Set( 1, TEXT(""));
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2426);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				Data.Set( 1, TEXT(""));
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2427);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				if( pSubMagicAttributeData && pSubMagicAttributeData->si32MagicMinDamage && pSubMagicAttributeData->si32MagicMaxDamage )  
				{
					SI32  siTemp1	  = pMagicAttributeData->si32MagicMinDamage;
					SI32  siTemp2	  = pMagicAttributeData->si32MagicMaxDamage;
					
					StringCchPrintf(explain, 1024, TEXT("%d - %d"),	siTemp1 + ((siTemp1 * pSubMagicAttributeData->si32MagicMinDamage) / 100), 
																	siTemp2 + ((siTemp2 * pSubMagicAttributeData->si32MagicMaxDamage) / 100) ); 
					Data.Set( 1, explain, COLORREF(0x0000aa00));  
				}
				else
				{
					StringCchPrintf(explain, 1024, TEXT("%d - %d"), pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					Data.Set( 1, explain);
				}
				
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2428);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);

				if (MAGIC_CASTTYPE_BUF == pMagicAttributeData->si08MagicCastType)
				{
					StringCchPrintf( explain, 1024, TEXT("0") ); 
				}
				else if (pclClient->IsCountrySwitch(Switch_HealthAddChange))
				{
					StringCchPrintf(explain, 1024, TEXT("%d - %d"), totalMagMinDmg , totalMagMaxDmg); 
				}
				else
				{
					StringCchPrintf(explain, 1024, TEXT("%d"), AddDamage); 
				}

				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2429);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				StringCchPrintf(explain, 1024,  TEXT("%d - %d"), SkillMin, SkillMax); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2430);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				StringCchPrintf(explain, 1024,  TEXT("%d - %d"), HealthMin, HealthMax); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();
				m_pList[1]->Refresh();

			}
			else if ( MAGIC_CASTTYPE_PASSIVE == pMagicAttributeData->si08MagicCastType )
			{
				if ( pMagicAttributePtr->GetMagicGroup(MagicKind) == MAGIC_GROUP_DRAIN )
				{

					SI32 siIndex = 0;
					TCHAR* pText = GetTxtFromMgr(2417);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2418);
					StringCchPrintf(explain, 1024,  pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2419);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2431);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2421);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2422);
					StringCchPrintf(explain, 1024,  pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();


					pText = GetTxtFromMgr(2423);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2424);
					//StringCchPrintf(explain, 1024,  pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					if( pSubMagicAttributeData && pSubMagicAttributeData->si32MagicSpendMana )
					{
						SI32  siTemp = pMagicAttributeData->si32MagicSpendMana;

						StringCchPrintf(explain, 1024, pText, mana, siTemp + (siTemp * pSubMagicAttributeData->si32MagicSpendMana) / 100 ); 
						Data.Set( 1, explain, COLORREF(0x0000aa00)); 
					}
					else
					{
						StringCchPrintf(explain, 1024, pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
						Data.Set( 1, explain);
					}
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();


					pText = GetTxtFromMgr(2432);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d%%-%d%%"), min(30, Min), min(30, Max)); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();


					Data.Set( 0, TEXT(""));
					Data.Set( 1, TEXT(""));
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2433);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					Data.Set( 1, TEXT(""));
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2434);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();


					pText = GetTxtFromMgr(2435);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d"), AddDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2436);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), SkillMin, SkillMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2437);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), HealthMin, HealthMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();
					m_pList[1]->Refresh();
				}
				else if ( pMagicAttributePtr->GetMagicGroup(MagicKind) == MAGIC_GROUP_CURSECRITICAL)
				{

					SI32 siIndex = 0;
					TCHAR* pText = GetTxtFromMgr(2417);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2418);
					StringCchPrintf(explain, 1024,  pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2419);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2431);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2421);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2422);
					StringCchPrintf(explain, 1024,  pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2423);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2424);
					//StringCchPrintf(explain, 1024,  pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					if( pSubMagicAttributeData && pSubMagicAttributeData->si32MagicSpendMana )
					{
						SI32  siTemp = pMagicAttributeData->si32MagicSpendMana;

						StringCchPrintf(explain, 1024, pText, mana, siTemp + (siTemp * pSubMagicAttributeData->si32MagicSpendMana) / 100 ); 
						Data.Set( 1, explain, COLORREF(0x0000aa00)); 
					}
					else
					{
						StringCchPrintf(explain, 1024, pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
						Data.Set( 1, explain);
					}
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2438);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d%%-%d%%"), min(80, Min), min(80, Max)); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					Data.Set( 0, TEXT(""));
					Data.Set( 1, TEXT(""));
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2439);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					Data.Set( 1, TEXT(""));
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();


					pText = GetTxtFromMgr(2440);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2441);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d"), AddDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2442);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), SkillMin, SkillMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2443);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), HealthMin, HealthMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();
					m_pList[1]->Refresh();
				}

			}
		}
		else
		{
			SI32 mana = 0, Min = 0, Max = 0;
			SI32 SkillMin= 0, SkillMax = 0;
			SI32 HealthMin = 0,HealthMax = 0;

			if ( (MAGIC_CASTTYPE_ACTIVE == pMagicAttributeData->si08MagicCastType) ||
				(MAGIC_CASTTYPE_BUF == pMagicAttributeData->si08MagicCastType) 
				)
			{
				SI32 siIndex = 0;
				TCHAR* pText = GetTxtFromMgr(2417);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2418);
				StringCchPrintf(explain, 1024,  pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2419);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2420);
				StringCchPrintf(explain, 1024,  pText, pMagicAttributeData->si32MagicDelay/20.0f); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2421);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2422);
				StringCchPrintf(explain, 1024,  pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2423);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2424);
				StringCchPrintf(explain, 1024,  pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2425);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				if ( MAGIC_CASTTYPE_BUF == pMagicAttributeData->si08MagicCastType ) 
				{
					StringCchPrintf( explain, 1024,  TEXT("0") ); 
				}
				else
				{
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), Min, Max); 
				}
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				Data.Set( 0, TEXT(""));
				Data.Set( 1, TEXT(""));
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2426);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2444);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2427);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				StringCchPrintf(explain, 1024,  TEXT("%d - %d"), pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2428);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);

				if (MAGIC_CASTTYPE_BUF == pMagicAttributeData->si08MagicCastType)
				{
					StringCchPrintf( explain, 1024, TEXT("0") ); 
				}
				else
				{
					StringCchPrintf(explain, 1024,  TEXT("%d"), AddDamage); 
				}

				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2429);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				StringCchPrintf(explain, 1024,  TEXT("%d - %d"), SkillMin, SkillMax); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );
				Data.Init();

				pText = GetTxtFromMgr(2430);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				StringCchPrintf(explain, 1024,  TEXT("%d - %d"), HealthMin, HealthMax); 
				Data.Set( 1, explain);
				siIndex++;		

				m_pList[1]->AddItem( &Data );
				m_pList[1]->Refresh();

			}
			else if ( MAGIC_CASTTYPE_PASSIVE == pMagicAttributeData->si08MagicCastType )
			{
				if ( pMagicAttributePtr->GetMagicGroup(MagicKind) == MAGIC_GROUP_DRAIN )
				{
					SI32 siIndex = 0;
					TCHAR* pText = GetTxtFromMgr(2417);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2418);
					StringCchPrintf(explain, 1024,  pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2419);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2431);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2421);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2422);
					StringCchPrintf(explain, 1024,  pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();


					pText = GetTxtFromMgr(2423);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2424);
					StringCchPrintf(explain, 1024,  pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2432);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d%%-%d%%"), min(30, Min), min(30, Max)); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					Data.Set( 0, TEXT(""));
					Data.Set( 1, TEXT(""));
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2433);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2444);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2434);
					StringCchPrintf(explain, 1024, pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2435);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d"), AddDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2436);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), SkillMin, SkillMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2437);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), HealthMin, HealthMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();
					m_pList[1]->Refresh();
				}
				else if ( pMagicAttributePtr->GetMagicGroup(MagicKind) == MAGIC_GROUP_CURSECRITICAL)
				{

					SI32 siIndex = 0;
					TCHAR* pText = GetTxtFromMgr(2417);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2418);
					StringCchPrintf(explain, 1024,  pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2419);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2431);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2421);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2422);
					StringCchPrintf(explain, 1024,  pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2423);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2424);
					StringCchPrintf(explain, 1024,  pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2438);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d%%-%d%%"), min(80, Min), min(80, Max)); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					Data.Set( 0,TEXT(""));
					Data.Set( 1, TEXT(""));
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2439);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2444);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2440);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2441);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d"), AddDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2442);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), SkillMin, SkillMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();

					pText = GetTxtFromMgr(2443);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), HealthMin, HealthMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
					Data.Init();
					m_pList[1]->Refresh();
				}

			}
		}

		// 아이템 설명
		if ( 0 < pMagicAttributeData->si32MagicExplainNum )
		{
			TCHAR* pText = GetTxtFromMgr( pMagicAttributeData->si32MagicExplainNum );
			if ( pText )
			{
				m_pEdit->SetText( pText );
			}
		}

	}

	//SetWindowText( m_hMagicExplainEdit, TEXT("") );

	return;
}

BOOL CNNewMagicInfoDlg::SetNewMagic(SI32 index)
{
	cltClient* pclclient = (cltClient*)pclClient;

	// 적어도 0.25 초 이하로 바꾸지 않도록 한다.
	if( TABS(pclclient->CurrentClock - m_dwLastChangedMagic) < 250 ) return FALSE;
	m_dwLastChangedMagic = pclclient->CurrentClock;

	SI16 MagicKind    = 0;

	if( SKILLBOOK_MAGIC == index )
	{
		cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
		if( NULL == pclChar ) return FALSE;

		
		cltSkillBookMagic*	pclSkillBookMagic = pclChar->pclCharSkillBookInfo->GetSkillBookMagic( pclChar->GetAttackType() );	
		if( NULL == pclSkillBookMagic ) return FALSE;
		
		MagicKind = pclSkillBookMagic->GetMagicKind();
		
		if( 0 >= MagicKind )	pclChar->SetCurrentMagic( 0, 0 ); 
	}
	else
	{
		if(index<0)		{	MagicKind = GetMagicKind();			}
		else			{	MagicKind = GetMagicKind(index);	}
	}

	SI32 CurrentMagic = pclClient->pclCM->CR[1]->GetCurrentMagic();

	if ( MagicKind == CurrentMagic )	return FALSE ;
	if ( MagicKind < MAGIC_INDEX_START )
	{
		m_pButton[0]->SetFontColor(RGB(225, 225, 225));
		m_pButton[0]->SetText( GetTxtFromMgr(3339), DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		
		return FALSE;
	}

	TCHAR					Explain[ 128 ]		= TEXT("");
	CMagicAttribute*		pMagicAttributePtr	= pclClient->pclMagicManager->GetMagicAttributePtr();
	stMagicAttributeData*	pMagicAttributeData	= pclClient->pclMagicManager->GetMagicAttribute( MagicKind );
	
	SI32 siSkillType = pMagicAttributePtr->GetMagicSkillType( MagicKind );
	if( SKILL_SIKLLBOOK ==  siSkillType )
	{
		cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
		if( NULL == pclChar ) return FALSE;

		cltSkillBookMagic*	pclSkillBookMagic = pclChar->pclCharSkillBookInfo->GetSkillBookMagic( pclChar->GetAttackType() );	
		if( NULL == pclSkillBookMagic ) return FALSE;

		siSkillType = pclSkillBookMagic->GetSkillUnique();
	}

	SI32 SkillLevel	= pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( siSkillType, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], 1);
	if( SkillLevel > 0 )
	{
		if( SkillLevel >= pMagicAttributeData->si16MagicSkillLevel )
		{
			if ( MAGIC_CASTTYPE_ACTIVE == pMagicAttributeData->si08MagicCastType )
			{
				TCHAR* pText = GetTxtFromMgr(2414);
				StringCchPrintf( Explain, 128, pText, pMagicAttributeData->strMagicName );
				m_pEdit->SetText(Explain);
			}
			else if ( MAGIC_CASTTYPE_PASSIVE == pMagicAttributeData->si08MagicCastType )
			{
				TCHAR* pText = GetTxtFromMgr(2415);
				StringCchPrintf( Explain, 128, pText, pMagicAttributeData->strMagicName );
				m_pEdit->SetText(Explain);
			}
			else if ( MAGIC_CASTTYPE_BUF == pMagicAttributeData->si08MagicCastType )
			{
				TCHAR* pText = GetTxtFromMgr(2414);
				StringCchPrintf( Explain, 128, pText, pMagicAttributeData->strMagicName );
				m_pEdit->SetText(Explain);
			}

			cltMsg clMsg( GAMEMSG_REQUEST_CHANGEMAGIC, MagicKind );
			pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );

		}
	}
	else
	{
		StringCchPrintf( Explain, 128, GetTxtFromMgr(2416), pMagicAttributeData->strMagicName );
		m_pEdit->SetText( Explain );
	}

	return TRUE;
}

void CNNewMagicInfoDlg::DrawMagicIcon(LPDIRECTDRAWSURFACE7 lpsurface,SI32 x , SI32 y )
{

	cltClient *pclclient = (cltClient *)pclClient;

	SI32 siScreenDrawX =  x  + 90;
	SI32 siScreenDrawY =	pclclient->siScreenMode800X600YSize + pclclient->siScreenModeYOffSet
							- m_SprMagicIcon.GetYSize()  + y  - 80 ;

	//KHY - 0608 - 마법위치 변경. - -0619-수정: 모든국가 적용
	//KKM - 0625 중국은 제외
	if ( pclClient->siServiceArea != ConstServiceArea_China )
	{

		if ( g_SoundOnOffDlg.m_bMaxWindowMode == false )
		{
			siScreenDrawX = x + 960; //960
			siScreenDrawY =	pclclient->siScreenMode800X600YSize + pclclient->siScreenModeYOffSet
								- m_SprMagicIcon.GetYSize()  + y  - 75 ;		
		}
		else
		{
			siScreenDrawX = x + 1010;
			siScreenDrawY =	pclclient->siScreenMode800X600YSize + pclclient->siScreenModeYOffSet
								- m_SprMagicIcon.GetYSize()  + y  - 75 ;
		}
	}

	//===============================================================
	// 현재 선정된 마법이 적절한 그림을 표시될수 있는지를 검사한다.
	//===============================================================
	SI32 CurrentMagic = pclClient->pclCM->CR[1]->GetCurrentMagic(); 
	if ( CurrentMagic < MAGIC_INDEX_START ) return ;
	//CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	//SI32 Group = pMagicAttributePtr->GetMagicGroup(CurrentMagic);
	//if ( Group <= 0 ) return ;
	//if ( Group >= m_MagicIcon.clHeader.siFontNum )
	

	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();

	SI16 ImageFont =	pMagicAttributePtr->GetMagicGroup(CurrentMagic);	;
	

	if ( ImageFont >=m_SprMagicIcon.GetImageNum() ) //	m_MagicIcon.clHeader.siFontNum)
	{
		return;
	}
	
	//===============================================================
	// 무기에 따른 적절한 마법이 세팅되어 있는것인지 체크한다
	//===============================================================
	
	//===============================================================
	// 현재 마법을 사용할수 있는 상태인지를 검사한다. 
	// 지금은  CLIENT에서는 알수가 없다.
	//===============================================================
	SI32 returnval = 0;
	if(pclClient->pclCM->CR[1]->CanMagicManaDelay(pclClient->pclCM->CR[1]->GetCurrentMagic(), &returnval) == true)
	{
		if( GP.LockSurface(lpsurface) == TRUE )
		{
			 
			GP.PutSprScreen( &m_SprMagicIcon, siScreenDrawX, siScreenDrawY, ImageFont );

			if(m_CoolTime > 0 && m_SprMagicCoolTime.pImage != NULL)
			{
				SI32 siDiff = TABS(pclClient->CurrentClock - m_CoolTimeStart);
				if(siDiff <= m_CoolTime)
				{
					SI32 font = m_SprMagicCoolTime.GetImageNum() * siDiff / m_CoolTime;
					GP.PutSprScreen( &m_SprMagicCoolTime, siScreenDrawX, siScreenDrawY, font );
				}
				else
					m_CoolTime = 0;
			}
			GP.UnlockSurface(lpsurface);
		}
	}
	else
	{
		//===============================================================
		// 사용할수 없는 이유에 따라 화면에 표시를 다르게 한다. (현재는 같음)
		//===============================================================
		switch(returnval)
		{
		case CANUSEMAGIC_ERROR_NEEDMANA:
		case CANUSEMAGIC_ERROR_NEEDTIMEDELAY:
		case CANUSEMAGIC_ERROR_NOTENOUGHSKILL:
			{
				if( GP.LockSurface(lpsurface) == TRUE )
				{
					GP.PutSprMultiply( &m_SprMagicIcon, siScreenDrawX, siScreenDrawY, ImageFont );	//m_MagicIcon.PutSprMultiplyT( siScreenDrawX, siScreenDrawY, 0 );
					GP.UnlockSurface(lpsurface);
				}
			}
			break;
		}
	}
}


void CNNewMagicInfoDlg::EnableButton(CButton* pButton, BOOL Flag)
{
	if(Flag)
	{
		pButton->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		pButton->Refresh();
		pButton->Enable(true);
	}
	else
	{
		pButton->SetFontColor(CONTROLLER_DISABLE_BUTTONTEXT_COLOR);
		pButton->Refresh();
		pButton->Enable(false);
	}
}

void CNNewMagicInfoDlg::SetButtonTextColor(SI32 Index)
{
	for( SI16 i = 0; i < 6; ++i )
	{
		if ( i == Index )
		{
			m_pMagicNameButton[ i ]->SetFontColor( CONTROLLER_BUTTONTEXT_COLOR );
			m_pMagicNameButton[ i ]->SetFontWeight( FW_BOLD );
			m_pMagicNameButton[ i ]->Refresh();

		}
		else
		{
			m_pMagicNameButton[ i ]->SetFontColor(  CONTROLLER_BACK_BUTTONTEXT_COLOR  );
			m_pMagicNameButton[ i ]->SetFontWeight( FW_NORMAL );
			m_pMagicNameButton[ i ]->Refresh();
		}
	}
}

void CNNewMagicInfoDlg::StartMagicCoolTime(SI32 CoolTime)
{
	m_CoolTime		= CoolTime;
	m_CoolTimeStart = pclClient->CurrentClock;
}

void CNNewMagicInfoDlg::DrawMagic( SI32 ScreenRX, SI32 ScreenRY )
{
	static CMagicAttribute*		pMagicAttributePtr  = pclClient->pclMagicManager->GetMagicAttributePtr();
	static cltSkillBookManager*	pclSkillBookManager = pclClient->pclItemManager->pclSkillBookManager;

	static SI16					MagicKind			= 0;
	static SI32					siMagicKind			= 0;
	static SI16					Group				= 0;
	for( SI32 siCount = 0; siCount < m_pList[0]->GetListNum(); ++siCount ) 
	{
		MagicKind	= pMagicAttributePtr->FindRefFromName( m_pList[0]->GetText( siCount+1, 0 ), m_siCurrentButtonIndex );
		if(MagicKind < 0) continue;
		
		Group		= pMagicAttributePtr->GetMagicGroup( MagicKind );
		siMagicKind	= Group;
		
		GP.PutSpr( &m_SprMagicIcon, ScreenRX + GetX() + m_pMagicImageStatic[siCount]->GetX(), ScreenRY + GetY() + m_pMagicImageStatic[siCount]->GetY(), siMagicKind );

		if( siCount == m_siSelectListIndex - 1 )
			GP.PutSpr( &m_SprMagicIconOut,	ScreenRX + GetX() + m_pMagicImageStatic[siCount]->GetX(), ScreenRY + GetY() + m_pMagicImageStatic[siCount]->GetY(), 0 );
	}

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if( NULL == pclChar ) return;

	SI32				siSkillUnique = 0;
	SI32				siAttackType  = 0;
	cltSkillBookMagic*	pclSkillBookMagic = NULL;
	if( pclChar->pclCharSkillBookInfo->IsSkillBookMagic() )
	{
		switch( m_siCurrentButtonIndex )
		{
		case MAGIC_TYPE_SWORD:		siSkillUnique = SKILL_USESWORD1	;	siAttackType = ATTACKTYPE_SWORD; break;// 검술
		case MAGIC_TYPE_SPEAR:		siSkillUnique = SKILL_USESPEAR1	;	siAttackType = ATTACKTYPE_SPEAR; break;// 창술
		case MAGIC_TYPE_BOW:		siSkillUnique = SKILL_USEBOW1	;	siAttackType = ATTACKTYPE_BOW;   break;// 궁술
		case MAGIC_TYPE_AXE:		siSkillUnique = SKILL_USEAXE1	;	siAttackType = ATTACKTYPE_AXE;	 break;// 부술(도끼술)
		case MAGIC_TYPE_GUN:		siSkillUnique = SKILL_USEGUN1	;	siAttackType = ATTACKTYPE_GUN;	 break;// 총술
		case MAGIC_TYPE_STAFF:		siSkillUnique = SKILL_USESTAFF1	;	siAttackType = ATTACKTYPE_STAFF; break;// 지팡이술
		}

		pclSkillBookMagic = pclChar->pclCharSkillBookInfo->GetSkillBookMagic( siAttackType );	
		if( NULL == pclSkillBookMagic ) return; 

		Group = pMagicAttributePtr->GetMagicGroup( pclSkillBookMagic->GetMagicKind() );
		if( 0 <= Group ) 
		{
			siMagicKind	= Group;

			GP.PutSpr( &m_SprMagicIcon,	ScreenRX + GetX() + m_pMagicImageStatic[9]->GetX(), ScreenRY + GetY() + m_pMagicImageStatic[9]->GetY(), siMagicKind );

			if( SKILLBOOK_MAGIC == m_siSelectListIndex )
			{
				GP.PutSpr( &m_SprMagicIconOut,	ScreenRX + GetX() + m_pMagicImageStatic[9]->GetX(), ScreenRY + GetY() + m_pMagicImageStatic[9]->GetY(), 0 );
			}
		}
	}

	if( 0 > m_siMagicKind )	return;
 
	GP.PutSpr( &m_SprMagicIcon, ScreenRX + GetX() + m_pSelcetMagicImageStatic->GetX(), ScreenRY + GetY() + m_pSelcetMagicImageStatic->GetY(), m_siMagicKind ); 

	// 선택된 마법이 스킬북으로 습득된 마법이면 남은 시간을 그려준다. 
	if( pclChar->pclCharSkillBookInfo->IsSkillBookMagic() )
	{
		if( SKILLBOOK_MAGIC == m_siSelectListIndex )
		{
			cltClient*	pclclient	 = (cltClient*)pclClient;
			UI32		uiRemainTime = cltSkillBookMagic::LIMIT_DAY - DiffDate( &pclclient->sTime, &pclSkillBookMagic->m_stTime );

			TSpr* pCountSpr = pclClient->GetGlobalSpr( GIMG_DMGCOUNTER );
			uiRemainTime %= 100;
			GP.PutSpr( pCountSpr, ScreenRX + GetX() + m_pSelcetMagicImageStatic->GetX() + 40 - 14, ScreenRY + GetY() + m_pSelcetMagicImageStatic->GetY() + 36, (uiRemainTime / 10) + 20);
			GP.PutSpr( pCountSpr, ScreenRX + GetX() + m_pSelcetMagicImageStatic->GetX() + 40 ,	    ScreenRY + GetY() + m_pSelcetMagicImageStatic->GetY() + 36, (uiRemainTime % 10) + 20 );  
		}
	}

	static SI32 siItemUnique = 0;
	static SI32 siImageNum	 = 0;
	static SI32 siTextNum    = 0;
	cltSkillBookSubMagic* pclSkillBookSubMagic = pclChar->pclCharSkillBookInfo->FindSkillBookSubMagicFromParentMagic( m_siParentMagicKind );
	if( pclSkillBookSubMagic )
	{
		if( pclSkillBookManager->GetSkillBookUniqueFromMagicKind( m_siParentMagicKind, pclSkillBookSubMagic->GetMagicKind(), siItemUnique, siImageNum, siTextNum ) )
		{
			GP.PutSpr( &m_SprSkillBook, ScreenRX + GetX() + m_pUpgradeMagicImageStatic->GetX(), ScreenRY + GetY() + m_pUpgradeMagicImageStatic->GetY(), siImageNum );		

			cltClient*	pclclient	 = (cltClient*)pclClient;
			UI32		uiRemainTime = cltSkillBookSubMagic::LIMIT_DAY - DiffDate( &pclclient->sTime, &pclSkillBookSubMagic->m_stTime );

			TSpr* pCountSpr = pclClient->GetGlobalSpr( GIMG_DMGCOUNTER );
			uiRemainTime %= 100;
			GP.PutSpr( pCountSpr, ScreenRX + GetX() + m_pUpgradeMagicImageStatic->GetX() + 40 - 14, ScreenRY + GetY() + m_pUpgradeMagicImageStatic->GetY() + 36, (uiRemainTime / 10) + 20);		
			GP.PutSpr( pCountSpr, ScreenRX + GetX() + m_pUpgradeMagicImageStatic->GetX() + 40 ,	    ScreenRY + GetY() + m_pUpgradeMagicImageStatic->GetY() + 36, (uiRemainTime % 10) + 20 );  
		}
	}
}

void CNNewMagicInfoDlg::SetMagicInfo( SI32 index )
{
	if( SKILLBOOK_MAGIC == index )
	{
		cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
		if( false == pclChar->pclCharSkillBookInfo->IsSkillBookMagic() )	return;
	}
	else
	{
		if( m_pList[0]->GetListNum() <= index ) return;
	}
		
	m_siSelectListIndex = index;
	
	SetMagic();
	SetExplain();	
}

void CNNewMagicInfoDlg::Refresh( void )
{
	if( -1 >= m_siSelectListIndex ) return;

	SetMagic();
	SetExplain();	

	CMagicAttribute*	pMagicAttributePtr	= pclClient->pclMagicManager->GetMagicAttributePtr();
	SI32				siSkillType			= pMagicAttributePtr->GetMagicSkillType( pclClient->pclCM->CR[1]->GetCurrentMagic() );
	if( SKILL_SIKLLBOOK == siSkillType )
	{
		SetNewMagic( SKILLBOOK_MAGIC );
	}
}

void CNNewMagicInfoDlg::SetActiveMagicKind( SI32 _siActiveMagicKind )
{
	m_siActiveMagicKind = _siActiveMagicKind;
}

