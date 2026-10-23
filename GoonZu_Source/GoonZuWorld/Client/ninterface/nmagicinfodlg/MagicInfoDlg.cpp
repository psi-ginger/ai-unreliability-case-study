#include "../Client/NInterface/NMagicInfoDlg/MagicInfoDlg.h"


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

extern cltCommonLogic* pclClient;

CNMagicInfoDlg::CNMagicInfoDlg() : CDialogController()
{
	m_siCurrentButtonIndex = 0;
	m_siSelectListIndex    = 0;
	m_siMagicKind		   = 0;

	m_dwLastChangedMagic = 0;

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

	GP.LoadSprFromFile(TEXT("./Interface/MagicMgrDlg/MagicIcon.SPR"), &m_SprMagicIcon );

	GP.LoadSprFromFile(TEXT("./Interface/MagicMgrDlg/MagicCoolTime.SPR"), &m_SprMagicCoolTime );

}

CNMagicInfoDlg::~CNMagicInfoDlg()
{

	GP.FreeSpr( m_SprMagicIcon );
	GP.FreeSpr( m_SprMagicCoolTime );

	SI32 i;
	for( i = 0; i < 2; ++i )
	{
		if(m_pList[i])
		{
			delete m_pList[i];
			m_pList[i] = NULL;
		}
	}

	for( i = 0; i < 6; ++i )
	{
		if(m_pMagicNameButton[i])
		{
			delete m_pMagicNameButton[i];
			m_pMagicNameButton[i] = NULL;
		}
	}

	for( i = 0; i < 2; ++i )
	{
		if(m_pButton[i])
		{
			delete m_pButton[i];
			m_pButton[i] = NULL;
		}
	}

	if( m_pEdit )
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}

	//GP.FreeSpr( m_SprMagicIcon );
	//GP.FreeSpr( m_SprMagicCoolTime );
}

void CNMagicInfoDlg::Create()
{
	if( pclClient->IsCountrySwitch(Switch_SkillBook) )	return;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NMagicInfoDlg/DLG_NMagicInfo.ddf"));
	file.CreatePopupDialog( this, NMAGICINFO_DLG, TEXT("dialog_NMagicInfo"), StaticCallBackDialogNMagicInfoDlg);

	SI32 i;
	for( i = 0; i < 2; ++i )
		m_pList[i] = new CList( this );
	for( i = 0; i < 6; ++i )
		m_pMagicNameButton[i] = new CButton( this );
	for( i = 0; i < 2; ++i )
		m_pButton[i] = new CButton( this );

	m_pEdit = new CEdit( this );

	file.LoadFile(TEXT("NInterface/Data/NMagicInfoDlg/DLG_NMagicInfo.ddf"));

	file.CreateControl( m_pList[0], NMAGICINFO_DIALOG_LIST1,   TEXT("listbox_NONAME1")); 
	file.CreateControl( m_pList[1], NMAGICINFO_DIALOG_LIST2,   TEXT("listbox_NONAME2")); 

	file.CreateControl( m_pMagicNameButton[0], NMAGICINFO_DIALOG_MAGICNAME_BUTTON1,   TEXT("button_NONAME1")); 
	file.CreateControl( m_pMagicNameButton[1], NMAGICINFO_DIALOG_MAGICNAME_BUTTON2,   TEXT("button_NONAME2")); 
	file.CreateControl( m_pMagicNameButton[2], NMAGICINFO_DIALOG_MAGICNAME_BUTTON3,   TEXT("button_NONAME3")); 
	file.CreateControl( m_pMagicNameButton[3], NMAGICINFO_DIALOG_MAGICNAME_BUTTON4,   TEXT("button_NONAME4")); 
	file.CreateControl( m_pMagicNameButton[4], NMAGICINFO_DIALOG_MAGICNAME_BUTTON5,   TEXT("button_NONAME5")); 
	file.CreateControl( m_pMagicNameButton[5], NMAGICINFO_DIALOG_MAGICNAME_BUTTON6,   TEXT("button_NONAME6")); 

	file.CreateControl( m_pButton[0], NMAGICINFO_DIALOG_BUTTON1,   TEXT("button_NONAME7")); 
	file.CreateControl( m_pButton[1], NMAGICINFO_DIALOG_BUTTON2,   TEXT("button_NONAME8")); 


	file.CreateControl( m_pEdit, NMAGICINFO_DIALOG_EDIT,   TEXT("editbox_NONAME1")); 

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

	ShowMagicList(m_siCurrentButtonIndex);

	m_pList[0]->SetSelectIndex(0);
	m_siSelectListIndex = m_pList[0]->GetSelectedIndex();
	SetMagic();
	SetExplain();
	
	

	Hide();


}


void CALLBACK CNMagicInfoDlg::StaticCallBackDialogNMagicInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNMagicInfoDlg *pThis = (CNMagicInfoDlg *)pControllerMgr;
	pThis->CallBackDialogNMagicInfoDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNMagicInfoDlg::CallBackDialogNMagicInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
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

					ShowMagicList(m_siCurrentButtonIndex);
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

					ShowMagicList(m_siCurrentButtonIndex);
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

					ShowMagicList(m_siCurrentButtonIndex);
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

					ShowMagicList(m_siCurrentButtonIndex);
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

					ShowMagicList(m_siCurrentButtonIndex);
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

					ShowMagicList(m_siCurrentButtonIndex);
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

	case NMAGICINFO_DIALOG_LIST1:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					m_siSelectListIndex = m_pList[0]->GetSelectedIndex();
					SetMagic();
					SetExplain();
				}
				break;

			}
		}
		break;
	}
}

void CNMagicInfoDlg::Hide()
{

	SetTimerOnOff(false);

	CControllerMgr::Show( false );

	return;
}

void CNMagicInfoDlg::Show()
{
	
	SetTimerOnOff(true);

	CControllerMgr::Show( true );

	SetMagic();

	pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_MAGICINFODLG);

	return;
}

void CNMagicInfoDlg::Action()
{

}

void CNMagicInfoDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if(m_siMagicKind < 0) 
		return;

	GP.PutSpr(&m_SprMagicIcon, ScreenRX + GetX() + 265, ScreenRY + GetY() + 70, m_siMagicKind );
}

// 마법 기술레벨이 변경 되었는지 안 되었는지만 체크하면 될듯.
bool CNMagicInfoDlg::IsChanged()
{
	return false;
}

BOOL CNMagicInfoDlg::SetMagic()
{
	SI16 MagicKind= GetMagicKind();

	if ( MagicKind < MAGIC_INDEX_START )
	{
		if(m_pButton)
		{
			TCHAR* pText= GetTxtFromMgr(3287);
			m_pButton[0]->SetFontColor(RGB(225, 225, 225));
			m_pButton[0]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	
		return FALSE;
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
		
	default :				return FALSE;
	}

	BOOL bWeaponTypeOK = FALSE;
	// 현재 장착된 무기의 Type에 따른 설정가능성을 타진한다.
	switch(pclClient->pclCM->CR[1]->GetAttackType())
	{
	case ATTACKTYPE_SWORD:		if(m_siCurrentButtonIndex == MAGIC_TYPE_SWORD)	bWeaponTypeOK = TRUE;	break;
	case ATTACKTYPE_SPEAR:		if(m_siCurrentButtonIndex == MAGIC_TYPE_SPEAR)	bWeaponTypeOK = TRUE;	break;
	case ATTACKTYPE_AXE:		if(m_siCurrentButtonIndex == MAGIC_TYPE_AXE)	bWeaponTypeOK = TRUE;	break;
	case ATTACKTYPE_STAFF:		if(m_siCurrentButtonIndex == MAGIC_TYPE_STAFF)	bWeaponTypeOK = TRUE;	break;
	case ATTACKTYPE_STAFF1:		if(m_siCurrentButtonIndex == MAGIC_TYPE_STAFF)	bWeaponTypeOK = TRUE;	break;
	case ATTACKTYPE_STAFF2:		if(m_siCurrentButtonIndex == MAGIC_TYPE_STAFF)	bWeaponTypeOK = TRUE;	break;
	case ATTACKTYPE_NEW_STAFF2:	if(m_siCurrentButtonIndex == MAGIC_TYPE_STAFF)	bWeaponTypeOK = TRUE;	break;
	case ATTACKTYPE_BOW:		if(m_siCurrentButtonIndex == MAGIC_TYPE_BOW)	bWeaponTypeOK = TRUE;	break;
	case ATTACKTYPE_BOW1:		if(m_siCurrentButtonIndex == MAGIC_TYPE_BOW)	bWeaponTypeOK = TRUE;	break;
	case ATTACKTYPE_NEW_BOW1:	if(m_siCurrentButtonIndex == MAGIC_TYPE_BOW)	bWeaponTypeOK = TRUE;	break;
	case ATTACKTYPE_GUN:		if(m_siCurrentButtonIndex == MAGIC_TYPE_GUN)	bWeaponTypeOK = TRUE;	break;
	case ATTACKTYPE_GUN1:		if(m_siCurrentButtonIndex == MAGIC_TYPE_GUN)	bWeaponTypeOK = TRUE;	break;
	case ATTACKTYPE_NEW_GUN1:	if(m_siCurrentButtonIndex == MAGIC_TYPE_GUN)	bWeaponTypeOK = TRUE;	break;
	default:				bWeaponTypeOK = FALSE;	break;
	}

	// 현재 장착된 무기의 Type에 따른 설정가능성을 타진한다.
	if( bWeaponTypeOK )
	{
		SI32 SkillLevel = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0]);

		if ( SkillLevel > 0 )
		{
			stMagicAttributeData *pMagicAttributeData = pclClient->pclMagicManager->GetMagicAttribute( MagicKind );

			if ( SkillLevel >= pMagicAttributeData->si16MagicSkillLevel )
			{
				EnableButton(m_pButton[0], true);
				//EnableWindow(m_hButton,TRUE);
			}
			else
			{
				EnableButton(m_pButton[0], false);
				//EnableWindow(m_hButton,FALSE);
			}
		}
		else
		{
			EnableButton(m_pButton[0], false);
			//EnableWindow(m_hButton,FALSE);
		}
	}
	else
	{
		EnableButton(m_pButton[0], false);
		//EnableWindow(m_hButton,FALSE);
	}
/*	
	if ( IsChanged() )
	{
	}
	*/
	return TRUE;
}

SI16 CNMagicInfoDlg::GetMagicKind()
{
	SI16 SelectedIndex = m_siSelectListIndex;
	if ( SelectedIndex < 0 )
	{
		return -1;
	}

	TCHAR Buffer[256 ] = TEXT("");
	//	ListView_GetItemText(m_hMagicList, SelectedIndex, 0, Buffer, 64);

	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();

	SI32 MagicKind = pMagicAttributePtr->FindRefFromName(m_pList[0]->GetText(SelectedIndex, 0), m_siCurrentButtonIndex);
	if(MagicKind > 0)
	{
		return MagicKind;
	}
	return -1;
}

SI16 CNMagicInfoDlg::GetMagicKind(SI32 key)
{
	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();

	SI32 index = -1;
	// 현재 장착된 무기의 Type에 따른 설정가능성을 타진한다.
	switch(pclClient->pclCM->CR[1]->GetAttackType())
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
	BOOL bSuccess = pMagicAttributePtr->GetMagicStartEnd(index, Start, End);
	if(bSuccess == FALSE) return -1;

	SI32 siCount = 1;
	for (SI32 i = Start; i<= End; ++i )
	{
		stMagicAttributeData *pMagicAttributeData = pMagicAttributePtr->GetMagicAttribute(i);
		// 유저가 사용 가능한 마법주에서 선택한다.
		if ( pMagicAttributeData->si16MagicUserUsing == 1)
		{
			if(key == siCount)	return i;
			siCount++;
		}		
	}	

	return -1;
}

void CNMagicInfoDlg::ShowMagicList(SI32 SelectButtonIdex) // 마법이름과 단축키
{			
	// ListView를 초기화한다.
	//	ListView_DeleteAllItems( m_hMagicList );
	m_pList[0]->Clear();

	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();

	SI32 Start, End;
	BOOL bSuccess = pMagicAttributePtr->GetMagicStartEnd(SelectButtonIdex, Start, End);
	if(bSuccess == FALSE) return;

	SI32 siCount = 0;
	for (SI32 i = Start; i<= End; ++i )
	{
		stMagicAttributeData *pMagicAttributeData = pMagicAttributePtr->GetMagicAttribute(i);

		// 유저가 사용 가능한 마법을 보여줌
		if ( pMagicAttributeData->si16MagicUserUsing == 1)
		{
			AddMagicList( pMagicAttributeData->strMagicName, siCount );
			siCount++;
		}		
	}	
}

void CNMagicInfoDlg::AddMagicList( TCHAR *MagicName, SI32 index )
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
		StringCchPrintf(buffer, 256, TEXT("CTRL+%d"), index+1);
		//		ListViewInsertItem(m_hMagicList, -1 , index, 1, buffer, -1);
		Data.Set(1, buffer);

		m_pList[0]->AddItem( &Data );
		m_pList[0]->Refresh();
	}
	return;
}

void CNMagicInfoDlg::SetExplain()
{
	//=======================================================
	// MagicDialog의 설정된 마법이 Valid한지 검사한다.
	//=======================================================
	SI16 SelectedIndex = m_siSelectListIndex;
	if ( SelectedIndex < 0 ){	return;	}

	TCHAR Buffer[256 ] = TEXT("");

	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	SI16 MagicKind = pMagicAttributePtr->FindRefFromName(m_pList[0]->GetText(SelectedIndex, 0), m_siCurrentButtonIndex);
	SI16 Group = pMagicAttributePtr->GetMagicGroup(MagicKind);

	m_siMagicKind = Group;
	if(MagicKind < 0) return;
	stMagicAttributeData *pMagicAttributeData = pMagicAttributePtr->GetMagicAttribute(MagicKind);

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

	SI32 SkillLevel = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( siSkillUnique, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0]);
	SI32 AddDamage  = pclClient->pclCM->CR[1]->clPB.GetAddMagicDamage();
	SI32 siOrganUnique = pclClient->pclCM->CR[1]->pclCI->clHealthInfo.GetOrg(pMagicAttributeData->si16MagicOrganKind);
	SI32 CharLevel  = pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel();

	TCHAR explain[1024] =TEXT("");
	TCHAR skillname[128];


	if(pclClient->pclSkillManager->GetSkillName(siSkillUnique, skillname) == true)
	{
		stListBoxItemData Data;

		if ( SkillLevel >= pMagicAttributeData->si16MagicSkillLevel )
		{
			// Skill Level에 의한 영향력을 주고 싶으면 영향력을 넣어 마나소비량을 계산한다.
			SI32 mana = 0, Min = 0, Max = 0;
			SI32 SkillMin= 0, SkillMax = 0;
			SI32 HealthMin= 0, HealthMax= 0;
			mana = pclClient->pclMagicManager->GetSpendMana(MagicKind, SkillLevel);
			stMagicInfo clMagicInfo(MagicKind, 0, 0, 0);
			pclClient->pclMagicManager->GetMinMaxDamage(&clMagicInfo, AddDamage, SkillLevel, Min, Max, siOrganUnique);
			pclClient->pclMagicManager->GetMinMaxSkillDamage(MagicKind, SkillLevel, SkillMin, SkillMax);
			
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
				(MAGIC_CASTTYPE_BUF == pMagicAttributeData->si08MagicCastType) 
			)
			{

				SI32 siIndex = 0;
				TCHAR* pText = GetTxtFromMgr(2417);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2418);
				StringCchPrintf(explain, 1024, pText, skillname, pMagicAttributeData->si16MagicSkillLevel);
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

				pText = GetTxtFromMgr(2419);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2420);
				StringCchPrintf(explain, 1024, pText, pMagicAttributeData->si32MagicDelay/20.0f); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

				pText = GetTxtFromMgr(2421);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2422);
				StringCchPrintf(explain, 1024, pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

				pText = GetTxtFromMgr(2423);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2424);
				StringCchPrintf(explain, 1024, pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

				pText = GetTxtFromMgr(2425);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				StringCchPrintf(explain, 1024, TEXT("%d - %d"), Min, Max); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

				Data.Set( 0, TEXT(""));
				Data.Set( 1, TEXT(""));
				siIndex++;

				m_pList[1]->AddItem( &Data );

				pText = GetTxtFromMgr(2426);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				Data.Set( 1, TEXT(""));
				siIndex++;

				m_pList[1]->AddItem( &Data );

				pText = GetTxtFromMgr(2427);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				StringCchPrintf(explain, 1024, TEXT("%d - %d"), pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

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

				pText = GetTxtFromMgr(2429);
				StringCchPrintf(explain, 1024, pText); 
				Data.Set( 0, explain);
				StringCchPrintf(explain, 1024,  TEXT("%d - %d"), SkillMin, SkillMax); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

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

					pText = GetTxtFromMgr(2419);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2431);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2421);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2422);
					StringCchPrintf(explain, 1024,  pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );


					pText = GetTxtFromMgr(2423);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2424);
					StringCchPrintf(explain, 1024,  pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );


					pText = GetTxtFromMgr(2432);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d%%-%d%%"), min(30, Min), min(30, Max)); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );


					Data.Set( 0, TEXT(""));
					Data.Set( 1, TEXT(""));
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2433);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					Data.Set( 1, TEXT(""));
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2434);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );


					pText = GetTxtFromMgr(2435);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d"), AddDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2436);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), SkillMin, SkillMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2437);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), HealthMin, HealthMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
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

					pText = GetTxtFromMgr(2419);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2431);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2421);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2422);
					StringCchPrintf(explain, 1024,  pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2423);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2424);
					StringCchPrintf(explain, 1024,  pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2438);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d%%-%d%%"), min(80, Min), min(80, Max)); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					Data.Set( 0, TEXT(""));
					Data.Set( 1, TEXT(""));
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2439);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					Data.Set( 1, TEXT(""));
					siIndex++;

					m_pList[1]->AddItem( &Data );


					pText = GetTxtFromMgr(2440);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2441);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d"), AddDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2442);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), SkillMin, SkillMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2443);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), HealthMin, HealthMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
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

				pText = GetTxtFromMgr(2419);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2420);
				StringCchPrintf(explain, 1024,  pText, pMagicAttributeData->si32MagicDelay/20.0f); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

				pText = GetTxtFromMgr(2421);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2422);
				StringCchPrintf(explain, 1024,  pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

				pText = GetTxtFromMgr(2423);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2424);
				StringCchPrintf(explain, 1024,  pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

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

				Data.Set( 0, TEXT(""));
				Data.Set( 1, TEXT(""));
					siIndex++;

				m_pList[1]->AddItem( &Data );

				pText = GetTxtFromMgr(2426);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				pText = GetTxtFromMgr(2444);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

				pText = GetTxtFromMgr(2427);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				StringCchPrintf(explain, 1024,  TEXT("%d - %d"), pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

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

				pText = GetTxtFromMgr(2429);
				StringCchPrintf(explain, 1024,  pText); 
				Data.Set( 0, explain);
				StringCchPrintf(explain, 1024,  TEXT("%d - %d"), SkillMin, SkillMax); 
				Data.Set( 1, explain);
				siIndex++;

				m_pList[1]->AddItem( &Data );

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

					pText = GetTxtFromMgr(2419);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2431);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2421);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2422);
					StringCchPrintf(explain, 1024,  pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );


					pText = GetTxtFromMgr(2423);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2424);
					StringCchPrintf(explain, 1024,  pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2432);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d%%-%d%%"), min(30, Min), min(30, Max)); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					Data.Set( 0, TEXT(""));
					Data.Set( 1, TEXT(""));
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2433);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2444);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2434);
					StringCchPrintf(explain, 1024, pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2435);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d"), AddDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2436);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), SkillMin, SkillMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2437);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), HealthMin, HealthMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
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

					pText = GetTxtFromMgr(2419);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2431);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2421);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2422);
					StringCchPrintf(explain, 1024,  pText, pMagicAttributeData->strMagicOrganName, pMagicAttributeData->si16MagicOrganDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2423);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2424);
					StringCchPrintf(explain, 1024,  pText, mana ,pMagicAttributeData->si32MagicSpendMana); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2438);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d%%-%d%%"), min(80, Min), min(80, Max)); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					Data.Set( 0,TEXT(""));
					Data.Set( 1, TEXT(""));
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2439);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					pText = GetTxtFromMgr(2444);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2440);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), pMagicAttributeData->si32MagicMinDamage, pMagicAttributeData->si32MagicMaxDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2441);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d"), AddDamage); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2442);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), SkillMin, SkillMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );

					pText = GetTxtFromMgr(2443);
					StringCchPrintf(explain, 1024,  pText); 
					Data.Set( 0, explain);
					StringCchPrintf(explain, 1024,  TEXT("%d - %d"), HealthMin, HealthMax); 
					Data.Set( 1, explain);
					siIndex++;

					m_pList[1]->AddItem( &Data );
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

BOOL CNMagicInfoDlg::SetNewMagic(SI32 index)
{
	cltClient *pclclient = (cltClient *)pclClient;

	// 적어도 0.25 초 이하로 바꾸지 않도록 한다.
	if(TABS(pclclient->CurrentClock - m_dwLastChangedMagic) < 250) return FALSE;
	m_dwLastChangedMagic = pclclient->CurrentClock;

	SI16 MagicKind = 0;

	if(index<0)		{	MagicKind = GetMagicKind();		}
	else			{	MagicKind = GetMagicKind(index);}

	SI32 CurrentMagic = pclClient->pclCM->CR[1]->GetCurrentMagic();

	if ( MagicKind == CurrentMagic )	return FALSE ;
	if ( MagicKind < MAGIC_INDEX_START )
	{
		TCHAR* pText= GetTxtFromMgr(3339);
		m_pButton[0]->SetFontColor(RGB(225, 225, 225));
		m_pButton[0]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//	if (m_hButton &&  IsWindowEnabled(m_hButton) )	EnableWindow(m_hButton,FALSE);
		return FALSE;
	}

	TCHAR Explain[ 128 ] =TEXT("");
	CMagicAttribute *pMagicAttributePtr			= pclClient->pclMagicManager->GetMagicAttributePtr();
	stMagicAttributeData *pMagicAttributeData	= pclClient->pclMagicManager->GetMagicAttribute( MagicKind );
	
	// [진성] 스킬북 획득 마법일 경우 스킬 정보를 얻어준다.
	SI32 SkillType = pMagicAttributePtr->GetMagicSkillType(MagicKind);
	if( SKILL_SIKLLBOOK == SkillType )
	{
		cltSkillBookMagic*	pclSkillBookMagic = pclClient->pclCM->CR[1]->pclCharSkillBookInfo->GetSkillBookMagic( pclClient->pclCM->CR[1]->GetAttackType() );	
		if( NULL == pclSkillBookMagic ) return false;

		SkillType = pclSkillBookMagic->GetSkillUnique();
	}

	SI32 SkillLevel = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SkillType, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0]);
	if ( SkillLevel > 0 )
	{
		if ( SkillLevel >= pMagicAttributeData->si16MagicSkillLevel )
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
//			if(m_hMagicExplainEdit)	SetWindowText( m_hMagicExplainEdit, Explain );
			//========================================
			// JAYCEE - 서버에서 받아야한다.
			//pclClient->pclCM->CR[1]->SetCurrentMagic( MagicKind, pMagicAttributeData->b16MagicActive );
			//========================================
			// 서버로 현재의 MagicKind를 날려줘야함 
			cltMsg clMsg(GAMEMSG_REQUEST_CHANGEMAGIC, MagicKind);
			pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

		}
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(2416);
		StringCchPrintf( Explain, 128, pText, pMagicAttributeData->strMagicName );
//		if(m_hMagicExplainEdit)	SetWindowText( m_hMagicExplainEdit, Explain );
		m_pEdit->SetText(Explain);
	}

	return TRUE;
}

void CNMagicInfoDlg::DrawMagicIcon(LPDIRECTDRAWSURFACE7 lpsurface,SI32 x , SI32 y )
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


void CNMagicInfoDlg::EnableButton(CButton* pButton, BOOL Flag)
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

void CNMagicInfoDlg::SetButtonTextColor(SI32 Index)
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

void CNMagicInfoDlg::StartMagicCoolTime(SI32 CoolTime)
{
	m_CoolTime		= CoolTime;
	m_CoolTimeStart = pclClient->CurrentClock;
}
