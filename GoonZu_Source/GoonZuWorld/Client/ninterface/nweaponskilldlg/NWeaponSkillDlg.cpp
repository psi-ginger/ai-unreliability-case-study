#include "NWeaponSkillDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Tree.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../../Common/Char/CharManager/CharManager.h"
#include "../../../Common/Skill/Skill-Manager.h"
#include "../../../Common/Skill/Skill.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../../CommonLogic/MsgType-Person.h"

#include "Msg/MsgType-Quest.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

CNWeaponSkillDlg::CNWeaponSkillDlg() : CDialogController()
{
	m_pTree = NULL;

	SI16 i = 0;
	for ( ; i < WEAPONSKILL_TAB_NUM; ++i )
	{
		m_pTabButton[ i ] = NULL;
	}

	m_pSkillMinusButton = NULL;
	m_pOKButton = NULL;

	for ( i = 0; i < 3; ++i  )
	{
		m_pStatic[ i ] = NULL;
	}

	m_pSkillLevel = NULL;
	m_pLeftSkill = NULL;

	m_pSkillExplain = NULL;
	m_pSkillStatus = NULL;

	m_siCurrentSkillUnique = 0;
	m_siCurrentTabIndex = -1;
}

CNWeaponSkillDlg::~CNWeaponSkillDlg()
{
	SAFE_DELETE( m_pTree );
	SI16 i = 0;
	for ( ; i < WEAPONSKILL_TAB_NUM; ++i )
	{
		SAFE_DELETE( m_pTabButton[ i ] );
	}
	SAFE_DELETE( m_pTree );
	SAFE_DELETE( m_pSkillMinusButton );
	SAFE_DELETE( m_pOKButton );
	for ( i = 0; i < 3; ++i  )
	{
		SAFE_DELETE( m_pStatic[ i ] );
	}
	SAFE_DELETE( m_pSkillLevel );
	SAFE_DELETE( m_pLeftSkill );

	SAFE_DELETE( m_pSkillExplain );
	SAFE_DELETE( m_pSkillStatus );

	SetTimerOnOff( false );
}

void CNWeaponSkillDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NWeaponSkill/DLG_NWeaponSkill.ddf"));
		file.CreatePopupDialog( this, NWEAPONSKILL_DLG, TEXT("dialog_NWeaponSkill"), StaticCallBackDialogNSkill );

		m_pTree = new CTree( this );
		//if ( pclClient->siServiceArea == ConstServiceArea_Korea )
		//{
		//	m_pTree->Create( NWEAPONSKILL_DIALOG_TREE, TEXT("NInterface/Common/Common_24x24_00_109.Spr"), 0, true, 9, 6, 58, 288, 178, 288, 178 );
		//}		
		//else
		//{
			//cyj m_pTree->Create( NWEAPONSKILL_DIALOG_TREE, TEXT("NInterface/Common/Common_24x24_00_109.Spr"), 9, true, 9, 6, 20, 288, 178, 288, 178 );
			m_pTree->Create( NWEAPONSKILL_DIALOG_TREE, TEXT("NInterface/Common/Common_24x24_00_109.Spr"), 0, true, 9, 6, 72, 288, 154, 288, 154 );
		//}

		SI16 i = 0;
		for ( ; i < WEAPONSKILL_TAB_NUM; ++i )
		{
			m_pTabButton[ i ] = new CButton( this );
			m_pTabButton[ i ]->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
		}

		m_pSkillMinusButton = new CButton( this );
		m_pSkillMinusButton->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		m_pOKButton = new CButton( this );
		m_pOKButton->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );

		for ( i = 0; i < 3; ++i  )
		{
			m_pStatic[ i ] = new CStatic( this );
			m_pStatic[ i ]->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );

		}

		m_pSkillLevel = new CStatic( this );
		m_pSkillLevel->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		m_pLeftSkill = new CStatic( this );
		m_pLeftSkill->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );

		m_pSkillExplain = new CEdit( this );
		m_pSkillExplain->SetTextPrintOption( DT_TOP | DT_LEFT | DT_WORDBREAK );
		m_pSkillStatus = new CEdit( this );
		m_pSkillStatus->SetTextPrintOption( DT_TOP | DT_LEFT | DT_WORDBREAK );


		file.CreateControl( m_pTabButton[ 0 ], NWEAPONSKILL_DIALOG_BUTTON_TAB_SWORD, TEXT("button_Sword") );
		file.CreateControl( m_pTabButton[ 1 ], NWEAPONSKILL_DIALOG_BUTTON_TAB_SPEAR, TEXT("button_Spear") );
		file.CreateControl( m_pTabButton[ 2 ], NWEAPONSKILL_DIALOG_BUTTON_TAB_AXE,	 TEXT("button_Axe") );
		file.CreateControl( m_pTabButton[ 3 ], NWEAPONSKILL_DIALOG_BUTTON_TAB_STAFF, TEXT("button_Staff") );
		file.CreateControl( m_pTabButton[ 4 ], NWEAPONSKILL_DIALOG_BUTTON_TAB_BOW,	 TEXT("button_Bow") );
		file.CreateControl( m_pTabButton[ 5 ], NWEAPONSKILL_DIALOG_BUTTON_TAB_GUN,	 TEXT("button_Gun") );

		file.CreateControl( m_pSkillMinusButton, NWEAPONSKILL_DIALOG_BUTTON_SKILLMINUS, TEXT("button_SkillMinus") );
		file.CreateControl( m_pOKButton, NWEAPONSKILL_DIALOG_BUTTON_OK, TEXT("button_Ok") );

		file.CreateControl( m_pStatic[0], NWEAPONSKILL_DIALOG_STATIC_0, TEXT("statictext_Static0") );
		file.CreateControl( m_pStatic[1], NWEAPONSKILL_DIALOG_STATIC_1, TEXT("statictext_Static1") );
		file.CreateControl( m_pStatic[2], NWEAPONSKILL_DIALOG_STATIC_2, TEXT("statictext_Static2") );

		file.CreateControl( m_pSkillLevel, NWEAPONSKILL_DIALOG_STATIC_SKILLLEVEL, TEXT("statictext_SkillLevel") );
		file.CreateControl( m_pLeftSkill, NWEAPONSKILL_DIALOG_STATIC_LEFTSKILL, TEXT("statictext_LeftSkill") );

		file.CreateControl( m_pSkillExplain, NWEAPONSKILL_DIALOG_EDIT_SKILLEXPLAIN, TEXT("editbox_SkillExplain") );
		file.CreateControl( m_pSkillStatus, NWEAPONSKILL_DIALOG_EDIT_SKILLSTATUS, TEXT("editbox_SkillStatus") );

		GetSkillLevel();
		SkillTabChange( 0 );
		SetTimerOnOff( true );
	}
	else
	{
		DeleteDialog();
	}

	return;
}

void CALLBACK CNWeaponSkillDlg::StaticCallBackDialogNSkill( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNWeaponSkillDlg *pThis = (CNWeaponSkillDlg *)pControllerMgr;
	pThis->CallBackDialogNSkill( nEvent, nControlID, pControl );

	return;
}

void CALLBACK CNWeaponSkillDlg::CallBackDialogNSkill( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{	
	case NWEAPONSKILL_DIALOG_BUTTON_OK:
	case COMMON_DIALOG_CLOSEBUTTON:
		DeleteDialog();
		break;

	case NWEAPONSKILL_DIALOG_TREE:
		{
			switch ( nEvent )
			{
			case EVENT_TREE_SELECTION:
				{
					SI16 SelectedIndex = m_pTree->GetSelectedItemIndex();
					if ( SelectedIndex > 0 )
					{
						stTreeItem *pTreeItem = m_pTree->GetTreeItemFromIndex( SelectedIndex );
						if ( pTreeItem )
						{
							m_siCurrentSkillUnique = pTreeItem->siParam;
						}
					}
				}
				break;
			}
		}
		break;

	case NWEAPONSKILL_DIALOG_BUTTON_TAB_SWORD:
	case NWEAPONSKILL_DIALOG_BUTTON_TAB_SPEAR:
	case NWEAPONSKILL_DIALOG_BUTTON_TAB_AXE:
	case NWEAPONSKILL_DIALOG_BUTTON_TAB_STAFF:
	case NWEAPONSKILL_DIALOG_BUTTON_TAB_BOW:
	case NWEAPONSKILL_DIALOG_BUTTON_TAB_GUN:
		SkillTabChange( nControlID - NWEAPONSKILL_DIALOG_BUTTON_TAB_SWORD );
		break;
	case NWEAPONSKILL_DIALOG_BUTTON_SKILLMINUS:
		{
			cltClient *pclclient = (cltClient *)pclClient;

			SI32 id = 1;
			if( m_siCurrentSkillUnique < 1 )
				break ;

			//---------------------------------
			// 스킬 점수가 있어야 뺄 수 있다. 
			//--------------------------------
			TCHAR buffer[256] = TEXT("");
			SI32 skilllevel = pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetSkillLevel( m_siCurrentSkillUnique );
			if(skilllevel <= 0)
			{
				TCHAR* pTitle = GetTxtFromMgr(669);
				TCHAR* pText = GetTxtFromMgr(668);
				StringCchCopy(buffer, 256, pText);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);
				break ;
			}

			bool bCondition = pclclient->pclCM->CR[id]->pclCI->clIP.GetLevel() == INIT_LEVEL || pclClient->pclCM->CR[id]->pclCI->clIP.GetLevel() == INIT_LEVEL_SECOND;

			if ( bCondition )
			{
				TCHAR* pTitle = GetTxtFromMgr(671);
				TCHAR* pText = GetTxtFromMgr(670);
				StringCchCopy(buffer, 256, pText);

				stMsgBox MsgBox;
				MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 0 );
				pclclient->SetMsgBox( &MsgBox, NULL, 0 ) ;
			}
			else
			{
				SI32 skilllevel = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetSkillLevel(m_siCurrentSkillUnique);

				SI32 itemunique = ITEMUNIQUE(24040) ; // 기술부분초기화이용권  14100 -> ITEMUNIQUE(24040)
				if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) < skilllevel )
				{
					TCHAR* pTitle = GetTxtFromMgr(671);
					TCHAR* pText = GetTxtFromMgr(672);

					StringCchPrintf( buffer, 256, pText );
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);
					break;
				}

				TCHAR* pTitle = GetTxtFromMgr(671);
				TCHAR* pText = GetTxtFromMgr(673);
				StringCchPrintf(buffer, 256, pText,skilllevel);

				stMsgBox MsgBox;
				MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 1 );
				pclclient->SetMsgBox( &MsgBox, NULL, 0 ) ;
			}
		}
		break;
	}
	return;
}

void CNWeaponSkillDlg::Action()
{
	SI32 id = 1;
	SI32 ChangedIndex = IsChanged();

	if ( ChangedIndex > 0 )
	{
		stTreeItem *pTreeItem = m_pTree->GetTreeItemFromIndex( ChangedIndex );
		if( pTreeItem )
		{
			SI32 level  = pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel( pclClient->pclSkillManager->pclSkillInfo[ ChangedIndex ]->siUnique, 0, 0, &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[0], 1 );
			m_siSkillLevel[ ChangedIndex ] = level;
			if ( pclClient->pclSkillManager->pclSkillInfo[ChangedIndex]->siSkillType == SKILLTYPE_FIGHT )
			{
				pclClient->pclSkillManager->MakeSkillTitle( &pclClient->pclCM->CR[id]->pclCI->clCharSkill,
					&pclClient->pclCM->CR[id]->pclCI->clCharItem,
					pclClient->pclSkillManager->pclSkillInfo[ ChangedIndex ]->siUnique,
					true);
			}
			else
			{
				pclClient->pclSkillManager->MakeSkillTitle( &pclClient->pclCM->CR[id]->pclCI->clCharSkill,
					&pclClient->pclCM->CR[id]->pclCI->clCharItem,
					pclClient->pclSkillManager->pclSkillInfo[ ChangedIndex ]->siUnique);
			}

			StringCchCopy( pTreeItem->strText, MAX_TREE_TEXT_LEN, pclClient->pclSkillManager->pclSkillInfo[ ChangedIndex ]->szSkillTitle );

			m_pTree->UpdateItem( pTreeItem );
		}
	}



	// 스킬 보너스를 설정한다. 
	TCHAR Buffer[ 256 ] = TEXT("");
	StringCchPrintf( Buffer, 256, TEXT("%d"), pclClient->pclCM->CR[id]->pclCI->clIP.GetSkillBonus());
	m_pLeftSkill->SetText( Buffer, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	m_pLeftSkill->Refresh();

	if( m_siCurrentSkillUnique )
	{
		// 해당 스킬의 레벨을 보여준다. 
		SI32 skilllevel = pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetSkillLevel( m_siCurrentSkillUnique );
		SI32 maxlevel	= pclClient->pclSkillManager->GetMaxLevel( m_siCurrentSkillUnique );

		StringCchPrintf( Buffer, 256, TEXT("%d/%d"), skilllevel, maxlevel );
		m_pSkillLevel->SetText( Buffer, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		m_pSkillLevel->Refresh();

		SetSkillExplainText( 0, 0, 0, 0, 0, 0 );

		// 스킬의 설명을 보여준다. 

		StringCchPrintf(Buffer, 256, TEXT("[%s]\r\n"), pclClient->pclSkillManager->pclSkillInfo[ m_siCurrentSkillUnique ]->szName );
		TCHAR* ptext = GetTxtFromMgr( pclClient->pclSkillManager->pclSkillInfo[ m_siCurrentSkillUnique ]->siExplainUnique );
		if(ptext)
		{
			StringCchCat( Buffer, 256, ptext );
		}

		m_pSkillExplain->SetText( Buffer );
		m_pSkillExplain->Refresh() ;
	}

	return;
}

void CNWeaponSkillDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch(MsgIndex)
	{
	case 0:
		{
			if ( RetValue )
			{
				cltGameMsgRequest_DecreaseSkillBonus clinfo( m_siCurrentSkillUnique, INIT_SKILLMODE_LEVEL );
				cltMsg clMsg( GAMEMSG_REQUEST_DECREASESKILLBONUS, sizeof(clinfo), (BYTE*)&clinfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 1:
		{
			if ( RetValue )
			{
				cltGameMsgRequest_DecreaseSkillBonus clinfo( m_siCurrentSkillUnique,INIT_SKILLMODE_WEAPONSKILL );
				cltMsg clMsg( GAMEMSG_REQUEST_DECREASESKILLBONUS, sizeof(clinfo), (BYTE*)&clinfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	default:
		return ;
	}

	return;
}

void CNWeaponSkillDlg::SkillTabChange( SI16 TabIndex )
{
	if ( TabIndex < 0 || m_siCurrentTabIndex == TabIndex )
	{
		return;
	}

	m_siCurrentTabIndex = TabIndex;

	for( SI16 i = 0; i < WEAPONSKILL_TAB_NUM; ++i )
	{
		if ( i == TabIndex )
		{
			m_pTabButton[ i ]->SetFontColor( CONTROLLER_BUTTONTEXT_COLOR );
			m_pTabButton[ i ]->SetFontWeight(FW_BOLD);
			m_pTabButton[ i ]->Refresh();
		}
		else
		{
			m_pTabButton[ i ]->SetFontColor( CONTROLLER_BACK_BUTTONTEXT_COLOR );
			m_pTabButton[ i ]->SetFontWeight(FW_NORMAL);
			m_pTabButton[ i ]->Refresh();
		}
	}

	m_pTree->Clear();

	SI32 ref = TabIndex + 1;
	SI32 Type = 0;
	if(ref >=0)
	{
		Type = ref ;
	}

	TreeViewInsert( Type, 0 );

	m_pTree->Refresh();

	return;
}

void CNWeaponSkillDlg::SetSkillExplainText( SI16 CurBaseSkillLevel, SI16 MaxBaseSkillLevel, SI16 ExpSkillLevel, SI16 MaxExpSkillLevel, SI32 CurExp, SI32 NextExp )
{
	cltClient *pclclient = (cltClient *)pclClient;
	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

	TCHAR Buffer[ 512 ] = TEXT("");

	if ( pclClient->pclSkillManager->pclSkillInfo[m_siCurrentSkillUnique]->siSkillType == SKILLTYPE_FIGHT )
	{
		TCHAR* pText = GetTxtFromMgr(5202);

		StringCchPrintf( Buffer, 512, pText,
			pclchar->pclCI->clCharSkill.GetSkillLevel( m_siCurrentSkillUnique ),
			pclclient->pclSkillManager->GetMaxLevel( m_siCurrentSkillUnique ),
			pclClient->pclItemManager->GetUpgradeSkillLevel( &pclchar->pclCI->clCharItem.clItem[0], m_siCurrentSkillUnique, 1));

	}
	else
	{
		SI32 skillexp = 0;
		if( m_siCurrentSkillUnique >= 0 )
		{
			skillexp   = pclchar->pclCI->clCharSkill.clSkill[ m_siCurrentSkillUnique ].siSkillExp;
		}

		SI32 explevel	= pclchar->pclCI->clCharSkill.GetSkillExpLevel( m_siCurrentSkillUnique );

		TCHAR* pText = GetTxtFromMgr(675);

		StringCchPrintf( Buffer, 512, pText,
			pclchar->pclCI->clCharSkill.GetSkillLevel( m_siCurrentSkillUnique ),
			pclclient->pclSkillManager->GetMaxLevel( m_siCurrentSkillUnique ),
			pclClient->pclItemManager->GetUpgradeSkillLevel( &pclchar->pclCI->clCharItem.clItem[0], m_siCurrentSkillUnique, 1),
			explevel,
			pclclient->pclSkillManager->GetMaxExpLevel( m_siCurrentSkillUnique ),
			skillexp,
			GetRequiredSkillExpForLevel( explevel + 1 ) );
	}

	m_pSkillStatus->SetText( Buffer );

	return;
}

void CNWeaponSkillDlg::TreeViewInsert( SI32 type, SI32 parentref )
{
	SI32 index	=  1;

	SI32 id = 1;

	stTreeItem	sTreeItem;

	for( index = 0; index < MAX_SKILL_NUMBER; ++index )
	{
		if( pclClient->pclSkillManager->pclSkillInfo[index] == NULL )
			continue;

		if( pclClient->pclSkillManager->pclSkillInfo[index]->siWeaponType == type )
		{
			if( pclClient->pclSkillManager->pclSkillInfo[index]->siParentRef == parentref )
			{
				sTreeItem.siIndex		=	index;
				sTreeItem.siFileIndex	=	1;
				sTreeItem.siImageIndex	=	pclClient->pclSkillManager->pclSkillInfo[index]->siBitMapIndex;
				sTreeItem.siParam		=	pclClient->pclSkillManager->pclSkillInfo[index]->siUnique;
				sTreeItem.siParentIndex =	parentref;

//				SI32 level  = pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel( pclClient->pclSkillManager->pclSkillInfo[index]->siUnique, 0, 0, &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[0], 1 );
				if ( pclClient->pclSkillManager->pclSkillInfo[index]->siSkillType == SKILLTYPE_FIGHT )
					pclClient->pclSkillManager->MakeWeaponSkillTitle( &pclClient->pclCM->CR[id]->pclCI->clCharSkill, &pclClient->pclCM->CR[id]->pclCI->clCharItem, pclClient->pclSkillManager->pclSkillInfo[index]->siUnique );
				else
					pclClient->pclSkillManager->MakeSkillTitle( &pclClient->pclCM->CR[id]->pclCI->clCharSkill, &pclClient->pclCM->CR[id]->pclCI->clCharItem, pclClient->pclSkillManager->pclSkillInfo[index]->siUnique );

				StringCchCopy( sTreeItem.strText, MAX_TREE_TEXT_LEN, pclClient->pclSkillManager->pclSkillInfo[index]->szSkillTitle );

				m_pTree->InsertItem( &sTreeItem );

				TreeViewInsert( type, index );
			}
		}
	}

	return;
}

void CNWeaponSkillDlg::GetSkillLevel()
{
	SI32 id = 1;

	SI32 i;

	for( i = 1; i < MAX_SKILL_NUMBER; ++i )
	{
		if( pclClient->pclSkillManager->pclSkillInfo[i] != NULL)
		{
			m_siSkillLevel[ i ] = pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel( pclClient->pclSkillManager->pclSkillInfo[i]->siUnique, 0, 0, &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[0]);
		}
	}

	return;

}

SI16 CNWeaponSkillDlg::IsChanged()
{
	SI32 id = 1;

	for ( SI32 i = 1; i < MAX_SKILL_NUMBER; ++i )
	{
		if( pclClient->pclSkillManager->pclSkillInfo[i] != NULL)
		{
			SI32 TotalSkillLevel = pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel( pclClient->pclSkillManager->pclSkillInfo[i]->siUnique, 0, 0, &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[0] );
			if ( m_siSkillLevel[ i ] != TotalSkillLevel )
			{
				return i;
			}
		}
	}

	return 0;
}