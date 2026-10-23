#include "NSkillDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Tree.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../../Common/Char/CharManager/CharManager.h"
#include "../../../Common/Skill/Skill-Manager.h"
#include "../../../Common/Skill/Skill.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../../CommonLogic/MsgType-Person.h"

#include "../../NInterface/NWeaponSkillDlg/NWeaponSkillDlg.h"

#include "Msg/MsgType-Quest.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

CNSkillDlg::CNSkillDlg() : CDialogController()
{
	m_pTree = NULL;

	SI16 i = 0;
	for ( ; i < SKILLTAB_END; ++i )
	{
		m_pTabButton[ i ] = NULL;
	}

	m_pSkillMinusButton = NULL;
	m_pSkillClearButton	=	NULL	;// 기술 전체 초기화 이용권 - 2009.11.24 손성웅
	m_pOKButton = NULL;
	m_pWeaponButton = NULL;
	
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

CNSkillDlg::~CNSkillDlg()
{
	SAFE_DELETE( m_pTree );
	SI16 i = 0;
	for ( ; i < SKILLTAB_END; ++i )
	{
		SAFE_DELETE( m_pTabButton[ i ] );
	}
	SAFE_DELETE( m_pTree );
	SAFE_DELETE( m_pSkillMinusButton );
	// 기술 전체 초기화 이용권 - 2009.11.24 손성웅
	SAFE_DELETE( m_pSkillClearButton );
	SAFE_DELETE( m_pOKButton );
	SAFE_DELETE( m_pWeaponButton );
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

void CNSkillDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;
		
		file.LoadFile(TEXT("NInterface/Data/NSkillDlg/DLG_NSkill.ddf"));
		file.CreatePopupDialog( this, NSKILL_DLG, TEXT("dialog_NSkill"), StaticCallBackDialogNSkill );

		m_pTree = new CTree( this );
		m_pTree->Create( NSKILL_DIALOG_TREE, TEXT("NInterface/Common/Common_24x24_00_109.Spr"), 0, true, 9, 6, 20, 288, 200, 288, 200 );
			

		SI16 i = 0;
		for ( ; i < SKILLTAB_END; ++i )
		{
			m_pTabButton[ i ] = new CButton( this );
			m_pTabButton[ i ]->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
			m_pTabButton[ i ]->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
			m_pTabButton[ i ]->Refresh();
		}
		m_pTabButton[ 0 ]->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pTabButton[ 0 ]->SetFontWeight(FW_BOLD);
		m_pTabButton[ 0 ]->Refresh();

		m_pSkillMinusButton = new CButton( this );
		m_pSkillMinusButton->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		// 기술 전체 초기화
		m_pSkillClearButton = new CButton(this)	;
		m_pSkillClearButton->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		
		m_pOKButton = new CButton( this );
		m_pOKButton->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		m_pWeaponButton = new CButton( this );
		m_pWeaponButton->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );

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

		
		file.CreateControl( m_pTabButton[SKILLTAB_USEWEAPON],		NSKILL_DIALOG_BUTTON_TAB_USEWEAPON,		TEXT("button_Tab_USEWEAPON") );
		file.CreateControl( m_pTabButton[SKILLTAB_MAKEWEAPON],		NSKILL_DIALOG_BUTTON_TAB_MAKEWEAPON,	TEXT("button_Tab_MAKEWEAPON") );
		file.CreateControl( m_pTabButton[SKILLTAB_ARMOR],			NSKILL_DIALOG_BUTTON_TAB_ARMOR,			TEXT("button_Tab_ARMOR") );
		file.CreateControl( m_pTabButton[SKILLTAB_MAKEHARVEST],		NSKILL_DIALOG_BUTTON_TAB_MAKEHARVEST,	TEXT("button_Tab_MAKEHARVEST") );
		file.CreateControl( m_pTabButton[SKILLTAB_HARVEST],			NSKILL_DIALOG_BUTTON_TAB_HARVEST,		TEXT("button_Tab_HARVEST") );
		file.CreateControl( m_pTabButton[SKILLTAB_MAKECLOTH],		NSKILL_DIALOG_BUTTON_TAB_MAKECLOTH,		TEXT("button_Tab_MAKECLOTH") );
		file.CreateControl( m_pTabButton[SKILLTAB_MAKEACCESSARY],	NSKILL_DIALOG_BUTTON_TAB_MAKEACCESSARY, TEXT("button_Tab_MAKEACCESSARY") );
		file.CreateControl( m_pTabButton[SKILLTAB_ETC],				NSKILL_DIALOG_BUTTON_TAB_ETC,			TEXT("button_Tab_ETC") );

		file.CreateControl( m_pSkillMinusButton, NSKILL_DIALOG_BUTTON_SKILLMINUS, TEXT("button_SkillMinus") );
		// 기술 전체 초기화 이용권 - 2009.11.24 손성웅
		file.CreateControl( m_pSkillClearButton, NSKILL_DIALOG_BUTTON_SKILLMINUS_ALL, TEXT("button_SkillMinusAll") );

		file.CreateControl( m_pOKButton, NSKILL_DIALOG_BUTTON_OK, TEXT("button_Ok") );
		file.CreateControl( m_pWeaponButton,NSKILL_DIALOG_BUTTON_WEAPONSKILLDLG,TEXT("button_WeaponSkillDlg") );

		file.CreateControl( m_pStatic[0], NSKILL_DIALOG_STATIC_0, TEXT("statictext_Static0") );
		file.CreateControl( m_pStatic[1], NSKILL_DIALOG_STATIC_1, TEXT("statictext_Static1") );
		file.CreateControl( m_pStatic[2], NSKILL_DIALOG_STATIC_2, TEXT("statictext_Static2") );

		file.CreateControl( m_pSkillLevel, NSKILL_DIALOG_STATIC_SKILLLEVEL, TEXT("statictext_SkillLevel") );
		file.CreateControl( m_pLeftSkill, NSKILL_DIALOG_STATIC_LEFTSKILL, TEXT("statictext_LeftSkill") );

		file.CreateControl( m_pSkillExplain, NSKILL_DIALOG_EDIT_SKILLEXPLAIN, TEXT("editbox_SkillExplain") );
		file.CreateControl( m_pSkillStatus, NSKILL_DIALOG_EDIT_SKILLSTATUS, TEXT("editbox_SkillStatus") );

			
		if (false == pclClient->IsCountrySwitch( Switch_Avatar_artisan))		//	[종호] 아바타 장인 스위치
		{
			//	아바타 장인 스위치가 켜져 있다면 재단술과 악세사리 제작술 버튼을비활성화 시킵니다.
			m_pTabButton[SKILLTAB_MAKECLOTH]->Show(false);
			m_pTabButton[SKILLTAB_MAKEACCESSARY]->Show(false);

			/*RECT rt;
			ZeroMemory(&rt, sizeof(RECT));
			m_pTabButton[SKILLTAB_MAKECLOTH]->GetWindowRect(&rt);*/

			m_pTabButton[SKILLTAB_ETC]->SetX(m_pTabButton[SKILLTAB_MAKECLOTH]->GetX());
			m_pTabButton[SKILLTAB_ETC]->SetY(m_pTabButton[SKILLTAB_MAKECLOTH]->GetY());
		}

		//KHY  - 0708 - 전투향상술 버튼 식제.
		m_pWeaponButton->Show(false);

		GetSkillLevel();
		SkillTabChange(SKILLTYPE_R_USEWEAPON-1);
		SetTimerOnOff( true );

		//-----------------------------------------------------------
		// 캐릭터 기술 정보창을 여는 퀘스트가 진행중이라면 
		// 게임 서버로 퀘스트가 완료되었음을 알려준다.
		//-----------------------------------------------------------
		SI16 questtype, questunique, para1, para2;
		SI16 questclearcond =  QUEST_CLEAR_COND_OPENSKILL;
		if( pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
			cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
	}
	else
	{
		DeleteDialog();
	}

	return;
}

void CALLBACK CNSkillDlg::StaticCallBackDialogNSkill( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNSkillDlg *pThis = (CNSkillDlg *)pControllerMgr;
	pThis->CallBackDialogNSkill( nEvent, nControlID, pControl );

	return;
}

void CALLBACK CNSkillDlg::CallBackDialogNSkill( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( nControlID )
	{	
	case NSKILL_DIALOG_BUTTON_OK:
	case COMMON_DIALOG_CLOSEBUTTON:
		DeleteDialog();
		break;

	case NSKILL_DIALOG_TREE:
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

							TreeViewSubItemInsert();
						}
					}
				}
				break;
			}
		}
		break;


		//	스킬탭 버튼 클릭시 Current스킬Tab를 변경시켜준다.
	case	NSKILL_DIALOG_BUTTON_TAB_USEWEAPON:
	case	NSKILL_DIALOG_BUTTON_TAB_MAKEWEAPON:
	case	NSKILL_DIALOG_BUTTON_TAB_ARMOR:
	case	NSKILL_DIALOG_BUTTON_TAB_MAKEHARVEST:
	case	NSKILL_DIALOG_BUTTON_TAB_HARVEST:
	case	NSKILL_DIALOG_BUTTON_TAB_MAKECLOTH:
	case	NSKILL_DIALOG_BUTTON_TAB_MAKEACCESSARY:
	case	NSKILL_DIALOG_BUTTON_TAB_ETC:
			{
				SkillTabChange( nControlID - NSKILL_DIALOG_BUTTON_TAB_USEWEAPON );
			}
			break;

	case NSKILL_DIALOG_BUTTON_WEAPONSKILLDLG:
		{
			pclclient->CreateInterface(NWEAPONSKILL_DLG);
		}
		break;

	case NSKILL_DIALOG_BUTTON_SKILLMINUS:
		{
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
				pclclient->SetMsgBox( &MsgBox, NULL, 0 );
			}
			else
			{
				SI32 itemunique = ITEMUNIQUE(24040) ; // 기술부분초기화이용권
				if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
				{
					TCHAR* pTitle = GetTxtFromMgr(671);
					TCHAR* pText = GetTxtFromMgr(672);

					StringCchPrintf( buffer, 256, pText );
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);
					break;
				}


				TCHAR* pTitle = GetTxtFromMgr(671);
				TCHAR* pText = GetTxtFromMgr(673);

				NTCHARString128 szExplain_Frist = GetTxtFromMgr(40433);
				szExplain_Frist.Replace( "#skillpoint#", SI32ToString( 1) );	// 획득 기술 점수 

				NTCHARString128 szExplain_Second = GetTxtFromMgr(40389);

				NTCHARString512 szText = GetTxtFromMgr( 40434 )	;
				szText.Replace( "#text1#",pText)	;
				szText.Replace( "#text2#",szExplain_Frist)	;
				szText.Replace( "#text3#",szExplain_Second)	;

				/*StringCchPrintf(buffer, 256, pText,1);*/

				stMsgBox MsgBox;
				MsgBox.Set( this, pTitle, szText, MSGBOX_TYPE_YESNO, 1 );
				pclclient->SetMsgBox( &MsgBox, NULL, 0 );
			}
		}
		break;
		// 기술 전체 초기화 이용권 - 2009.11.24 손성웅
	case NSKILL_DIALOG_BUTTON_SKILLMINUS_ALL:
		{
			SI32 id = 1;
			cltCharClient* pclCharClient  = pclclient->pclCM->GetCharClient( id )	;
			if ( pclCharClient == NULL )	return	;

			TCHAR buffer[256];
			//---------------------------------
			// 초기화 시킬 스킬을 선택 해야 한다 
			//---------------------------------
			if( m_siCurrentSkillUnique < 1 )
			{
				TCHAR* pTitle = GetTxtFromMgr(669);
				TCHAR* pText = GetTxtFromMgr(40386); // 기술등급이 0 이라 기술을 뺄수가 없다 .
				StringCchCopy(buffer, 256, pText);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);
				return	;
			}

			//---------------------------------
			// 스킬 점수가 있어야 뺄 수 있다. 
			//--------------------------------		

			SI32 skilllevel = pclCharClient->pclCI->clCharSkill.GetSkillLevel( m_siCurrentSkillUnique );

			SI32 siUseSkillPoint = 0 ;

			// 제조 활성화가 들어가면 
			if(	pclClient->IsCountrySwitch(Switch_ManufactureRevitalize) && pclClient->pclSkillManager->IsManufactureSkill(m_siCurrentSkillUnique))
			{
				siUseSkillPoint = 0;
			}
			else
			{
				siUseSkillPoint = skilllevel	;
			}
			if(skilllevel <= 0)
			{
				TCHAR* pTitle = GetTxtFromMgr(669);
				TCHAR* pText = GetTxtFromMgr(668); // 기술등급이 0 이라 기술을 뺄수가 없다 .
				StringCchCopy(buffer, 256, pText);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);
				return	;
			}
			//---------------------------------
			// 아이템이 있어야 한다 
			//--------------------------------
			SI32 itemunique = ITEMUNIQUE(24041) ; // 기술전체초기화이용권
			if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
			{
				TCHAR* pTitle = GetTxtFromMgr(671);
				TCHAR* pText = GetTxtFromMgr(40388); // 아이템이 없다 .

				StringCchPrintf( buffer, 256, pText );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);
				return	;
			}

			TCHAR szSkillname[64] ;

			TCHAR* pTitle = GetTxtFromMgr(671);

			if( pclClient->pclSkillManager->GetSkillName( m_siCurrentSkillUnique ,szSkillname ) == true )
			{
				NTCHARString128 szExplain_1st = GetTxtFromMgr(40387);
				szExplain_1st.Replace( "#skillname#", szSkillname );

				NTCHARString128 szExplain_2nd = GetTxtFromMgr(40433);
				szExplain_2nd.Replace( "#skillpoint#", SI32ToString( siUseSkillPoint ) );	// 획득 기술 점수 

				NTCHARString128 szExplain_3rd = GetTxtFromMgr(40389);

				NTCHARString512 szText = GetTxtFromMgr( 40434 )	;
				szText.Replace( "#text1#",szExplain_1st)	;
				szText.Replace( "#text2#",szExplain_2nd)	;
				szText.Replace( "#text3#",szExplain_3rd)	;

				stMsgBox MsgBox;
				MsgBox.Set( this, pTitle, szText, MSGBOX_TYPE_YESNO, 2 );
				pclclient->SetMsgBox( &MsgBox, NULL, 0 );
			}			

			
		}break;
	}
	return;
}

void CNSkillDlg::Action()
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

			if ( SKILLTYPE_FIGHT == pclClient->pclSkillManager->pclSkillInfo[ ChangedIndex ]->siSkillType )
			{
				pclClient->pclSkillManager->MakeWeaponSkillTitle( &pclClient->pclCM->CR[id]->pclCI->clCharSkill, &pclClient->pclCM->CR[id]->pclCI->clCharItem, pclClient->pclSkillManager->pclSkillInfo[ ChangedIndex ]->siUnique );
			}
			else
			{
				pclClient->pclSkillManager->MakeSkillTitle( &pclClient->pclCM->CR[id]->pclCI->clCharSkill, &pclClient->pclCM->CR[id]->pclCI->clCharItem, pclClient->pclSkillManager->pclSkillInfo[ ChangedIndex ]->siUnique );
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
		SI32 siSkillType	= pclClient->pclSkillManager->FindSkillTypeFromUnique( m_siCurrentSkillUnique );
		// 해당 스킬의 레벨을 보여준다. 
		SI32 siSkillLevel	= pclClient->pclCM->CR[id]->pclCI->clCharSkill.GetSkillLevel( m_siCurrentSkillUnique );
		SI32 siMaxLevel		= pclClient->pclSkillManager->GetMaxLevel( m_siCurrentSkillUnique );


		StringCchPrintf( Buffer, 256, TEXT("%d/%d"), siSkillLevel, siMaxLevel );
		m_pSkillLevel->SetText( Buffer, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
		m_pSkillLevel->Refresh();

		// 스킬의 설명을 보여준다. 
		StringCchPrintf(Buffer, 256, TEXT("[%s]\r\n"), pclClient->pclSkillManager->pclSkillInfo[ m_siCurrentSkillUnique ]->szName );
		TCHAR* ptext = GetTxtFromMgr( pclClient->pclSkillManager->pclSkillInfo[ m_siCurrentSkillUnique ]->siExplainUnique );
		if(ptext)
		{
			StringCchCat( Buffer, 256, ptext );
		}

		m_pSkillExplain->SetText( Buffer );

		// 스킬 경험치 관련 정보를 보여준다
		SetSkillExplainText( 0, 0, 0, 0, 0, 0 );
	}

	return;
}

void CNSkillDlg::SkillTabChange( SI16 TabIndex )
{
	if ( TabIndex < 0 || m_siCurrentTabIndex == TabIndex )
	{
		return;
	}

	m_siCurrentTabIndex = TabIndex;

	for( SI16 i = 0; i < SKILLTAB_END; ++i )
	{
		if ( i == TabIndex )
		{
			m_pTabButton[ i ]->SetFontColor( CONTROLLER_BUTTONTEXT_COLOR );
			m_pTabButton[ i ]->SetFontWeight( FW_BOLD );
			m_pTabButton[ i ]->Refresh();
		}
		else
		{
			m_pTabButton[ i ]->SetFontColor( CONTROLLER_BACK_BUTTONTEXT_COLOR );
			m_pTabButton[ i ]->SetFontWeight( FW_NORMAL );
			m_pTabButton[ i ]->Refresh();
		}
	}

	m_pTree->Clear();
	
	SI32 ref = TabIndex + 1;
	SI32 Type = 0;
	if(ref >=0)
	{
		Type = pclClient->pclSkillManager->pclSkillTypeInfo[ref]->siSkillType;
	}

	if ( Type == SKILLTYPE_R_ETC )
	{
		pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_OPENSKILLETC );
	}
	
	TreeViewInsert( Type, 0 );

	m_pTree->Refresh();

	return;
}

void CNSkillDlg::SetSkillExplainText( SI16 CurBaseSkillLevel, SI16 MaxBaseSkillLevel, SI16 ExpSkillLevel, SI16 MaxExpSkillLevel, SI32 CurExp, SI32 NextExp )
{
	if ( (0 > m_siCurrentSkillUnique) || (MAX_SKILL_NUMBER <= m_siCurrentSkillUnique) )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	TCHAR szBuffer[ 512 ] = TEXT("");

	SI32 siSkillType		= pclClient->pclSkillManager->FindSkillTypeFromUnique( m_siCurrentSkillUnique );

	SI32 siSkillLevel		= pclchar->pclCI->clCharSkill.GetSkillLevel( m_siCurrentSkillUnique );
	SI32 siMaxSkillLevel	= pclClient->pclSkillManager->GetMaxLevel( m_siCurrentSkillUnique );
	SI32 siUGSkillLevel		= pclClient->pclItemManager->GetUpgradeSkillLevel( &pclchar->pclCI->clCharItem.clItem[0], m_siCurrentSkillUnique, 1 );
	

	if ( SKILLTYPE_FIGHT == siSkillType )
	{
		TCHAR* pText = GetTxtFromMgr(5202);

		StringCchPrintf( szBuffer, 512, pText, siSkillLevel, siMaxSkillLevel, siUGSkillLevel );
	}
	else
	{
		SI32	siExpLevel		= pclchar->pclCI->clCharSkill.GetSkillExpLevel( m_siCurrentSkillUnique );
		SI32	siMaxExpLevel	= pclclient->pclSkillManager->GetMaxExpLevel( m_siCurrentSkillUnique );
		SI32	siSkillExp		= pclchar->pclCI->clCharSkill.clSkill[ m_siCurrentSkillUnique ].siSkillExp;
		SI32	siRequiredExp	= GetRequiredSkillExpForLevel( siExpLevel + 1 );
		
		TCHAR*	pText			= GetTxtFromMgr(675);

		StringCchPrintf( szBuffer, 512, pText, siSkillLevel, siMaxSkillLevel, siUGSkillLevel, siExpLevel, siMaxExpLevel, siSkillExp, siRequiredExp );
	}

	m_pSkillStatus->SetText( szBuffer );

	return;
}

void CNSkillDlg::TreeViewInsert( SI32 siType, SI32 siParentRef, SI32 siSkillUnique/* =-1 */, SI16 siDeeps/* =1 */, bool bWeaponType/* =false  */)
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	stTreeItem		sTreeItem;
	cltSkillInfo*	pclGetSkillInfo = NULL;
	SI32			siLevel			= 0;

	// 전투향상술 스위치가 꺼져있을때는 전투향상술 관련 스킬을 검사하지 않는다
	if ( false == pclClient->IsCountrySwitch(Switch_WeaponSkill) )
	{
		if ( true == bWeaponType )
		{
			return;
		}
	}

	for( SI32 siIndex = 0; siIndex < MAX_SKILL_NUMBER; ++siIndex )
	{
		pclGetSkillInfo = pclClient->pclSkillManager->pclSkillInfo[siIndex];
		if ( NULL == pclGetSkillInfo )
		{
			continue;
		}
		
		// 무기 사용술 스킬의 자식아이템들을 검사하고자 한다면
		if ( bWeaponType )
		{
			if ( pclGetSkillInfo->siSkillType != SKILLTYPE_FIGHT )
			{
				continue;
			}

			if ( pclGetSkillInfo->siWeaponType != siType )
			{
				continue;
			}

			// 히든 전투향상술은 배웠을때만 스킬트리에 나타낸다
			if ( (pclGetSkillInfo->siSkillAtb & SKILLATB_HIDDENGRADE1) || (pclGetSkillInfo->siSkillAtb & SKILLATB_HIDDENGRADE2) )
			{
				if ( 0 >= pclchar->pclCI->clCharSkill.GetSkillLevel(siIndex) )
				{
					continue;
				}
			}

			// 각 단계에 따라 넣어야하는 siDeeps가 있다
			// 실제 보이는 것은 4/5단계처럼 보이지만 사실 맨처음 무기술이 1단계라고 보면 된다. 
			// 그렇기 때문에 siDeeps가 5일때 4단계(히든1단계)를 보여주고 6일때 5단계(히든2단계를) 보여준다
			// 이것을 의미에 맞게 4/5로 하려면 siDeeps의 시작값을 0으로 하면 된다
			if ( pclGetSkillInfo->siSkillAtb & SKILLATB_HIDDENGRADE1 )
			{
				if ( 5 != siDeeps )
				{
					continue;
				}
			}
			else if ( pclGetSkillInfo->siSkillAtb & SKILLATB_HIDDENGRADE2 )
			{
				if ( 6 != siDeeps )
				{
					continue;
				}
			}
			else
			{
				if ( siParentRef != pclGetSkillInfo->siParentRef )
				{
					continue;
				}
			}

		}
		// 그냥 스킬만 검사하고자 한다면
		else
		{
			if ( pclGetSkillInfo->siSkillType != siType )
			{
				continue;
			}

			if ( siParentRef != pclGetSkillInfo->siParentRef )
			{
				continue;
			}
		}

		sTreeItem.Init();

		sTreeItem.siIndex		=	siIndex;
		sTreeItem.siFileIndex	=	1;
		sTreeItem.siImageIndex	=	pclGetSkillInfo->siBitMapIndex;
		sTreeItem.siParam		=	pclGetSkillInfo->siUnique;
		sTreeItem.siParentIndex =	siParentRef;
		
		//siLevel	= pclchar->pclCI->clCharSkill.GetTotalSkillLevel( pclGetSkillInfo->siUnique, 0, 0, &pclchar->pclCI->clCharItem.clItem[0], 1 );
		if ( bWeaponType )
		{
			pclClient->pclSkillManager->MakeWeaponSkillTitle( &pclchar->pclCI->clCharSkill, &pclchar->pclCI->clCharItem, pclGetSkillInfo->siUnique );
		}
		else
		{
			pclClient->pclSkillManager->MakeSkillTitle( &pclchar->pclCI->clCharSkill, &pclchar->pclCI->clCharItem, pclGetSkillInfo->siUnique );
		}
		
		StringCchCopy( sTreeItem.strText, MAX_TREE_TEXT_LEN, pclGetSkillInfo->szSkillTitle );

		m_pTree->InsertItem( &sTreeItem );
		
		if ( siSkillUnique == pclGetSkillInfo->siUnique )
		{
			TreeViewInsert( pclGetSkillInfo->siWeaponType, siParentRef, -1, siDeeps+1, true );
		}
		else if ( bWeaponType )
		{
			TreeViewInsert( pclGetSkillInfo->siWeaponType, siIndex, -1, siDeeps+1,  true );
		}
		else
		{
			TreeViewInsert( siType, siIndex, -1, siDeeps+1 );
		}
	}

	return;
}

void CNSkillDlg::TreeViewSubItemInsert( void )
{
	SI32 siSkillType		= pclClient->pclSkillManager->FindSkillTypeFromUnique( m_siCurrentSkillUnique );
	SI16 siSelectedIndex	= m_pTree->GetSelectedItemIndex();

	//if ( (3 == m_siCurrentTabIndex) && (SKILLTYPE_FIGHT != siSkillType) )
	if (siSkillType == SKILLTYPE_R_USEWEAPON)
	{
		SI32 siRef	= m_siCurrentTabIndex + 1;
		SI32 siType	= 0;

		if ( siRef >= 0 )
		{
			siType = pclClient->pclSkillManager->pclSkillTypeInfo[siRef]->siSkillType;
		}

		m_pTree->Clear();

		TreeViewInsert( siType, 0, m_siCurrentSkillUnique );
		m_pTree->SetSelectedItemIndex( siSelectedIndex );

		m_pTree->Refresh();
	}

}

void CNSkillDlg::GetSkillLevel()
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

SI16 CNSkillDlg::IsChanged()
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

void CNSkillDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
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
				cltGameMsgRequest_DecreaseSkillBonus clinfo( m_siCurrentSkillUnique,INIT_SKILLMODE_TICKET );
				cltMsg clMsg( GAMEMSG_REQUEST_DECREASESKILLBONUS, sizeof(clinfo), (BYTE*)&clinfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 2:
		{
			// 기술 전체 초기화 이용권 - 2009.11.24 손성웅
			if ( RetValue )
			{
				cltGameMsgRequest_DecreaseSkillBonus clinfo( m_siCurrentSkillUnique,INIT_SKILLMODE_TICKET_ALL );
				cltMsg clMsg( GAMEMSG_REQUEST_DECREASESKILLBONUS, sizeof(clinfo), (BYTE*)&clinfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	default:
		return ;
	}
}