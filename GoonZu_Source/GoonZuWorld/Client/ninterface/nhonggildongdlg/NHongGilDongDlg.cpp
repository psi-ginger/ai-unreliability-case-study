
#include "NHongGilDongDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Tree.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../../Common/Char/CharManager/CharManager.h"
#include "../../../Common/Skill/Skill-Manager.h"
#include "../../../Common/Skill/Skill.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../../CommonLogic/MsgType-Person.h"

#include "../../Music/music.h"

#include "Msg/MsgType-Quest.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

CNHongGilDongDlg::CNHongGilDongDlg() : CDialogController()
{
	m_siCurrentSkillUnique		= 0;
	m_siCurrentSkillTypeUnique	= 0;

	m_siCurrentTabControllerID	= 0;

	ZeroMemory( m_siSkillLevel, sizeof(m_siSkillLevel) );

	m_siCostMoney				= 0;

	m_siTreeClickTime			= 0;

	m_siSelectedTreeIndex		= 0;

	m_siRequestCostTime			= 0;
}

CNHongGilDongDlg::~CNHongGilDongDlg()
{
	SetTimerOnOff( false );
}

void CNHongGilDongDlg::Create()
{
	// 이미 다이얼로그를 만들었다면 지운다
	if ( IsCreate() )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/NHongGilDong/DLG_NHongGilDong.ddf"));
	file.CreatePopupDialog( this, NHONGGILDONG_DLG, TEXT("dialog_NHongGilDong"), StaticCallBackDialogNSkill );

	// 트리 생성은 따로
	m_InterfaceFactory.CreateUserInterface( TREE, NHONGGILDONG_DIALOG_TREE, this );
	m_InterfaceFactory.GetTree(NHONGGILDONG_DIALOG_TREE)->Create( NHONGGILDONG_DIALOG_TREE, TEXT("NInterface/Common/Common_24x24_00_109.Spr"), 9, true, 9, 8, 168, 352, 120, 352, 120 );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC, NHONGGILDONG_DIALOG_PORTRAIT,			this),	NHONGGILDONG_DIALOG_PORTRAIT,			TEXT("imagestatic_HongGilDong") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NHONGGILDONG_DIALOG_BUTTON_TAB_SWORD,	this),	NHONGGILDONG_DIALOG_BUTTON_TAB_SWORD,	TEXT("button_Sword") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NHONGGILDONG_DIALOG_BUTTON_TAB_SPEAR,	this),	NHONGGILDONG_DIALOG_BUTTON_TAB_SPEAR,	TEXT("button_Spear") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NHONGGILDONG_DIALOG_BUTTON_TAB_AXE,		this),	NHONGGILDONG_DIALOG_BUTTON_TAB_AXE,		TEXT("button_Axe") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NHONGGILDONG_DIALOG_BUTTON_TAB_STAFF,	this),	NHONGGILDONG_DIALOG_BUTTON_TAB_STAFF,	TEXT("button_Staff") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NHONGGILDONG_DIALOG_BUTTON_TAB_BOW,		this),	NHONGGILDONG_DIALOG_BUTTON_TAB_BOW,		TEXT("button_Bow") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NHONGGILDONG_DIALOG_BUTTON_TAB_GUN,		this),	NHONGGILDONG_DIALOG_BUTTON_TAB_GUN,		TEXT("button_Gun") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NHONGGILDONG_DIALOG_BUTTON_SKILLPLUS,	this),	NHONGGILDONG_DIALOG_BUTTON_SKILLPLUS,	TEXT("button_SkillPlus") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NHONGGILDONG_DIALOG_BUTTON_OK,			this),	NHONGGILDONG_DIALOG_BUTTON_OK,			TEXT("button_Ok") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,		NHONGGILDONG_DIALOG_STATIC_1,			this),	NHONGGILDONG_DIALOG_STATIC_1,			TEXT("statictext_Static1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,		NHONGGILDONG_DIALOG_STATIC_2,			this),	NHONGGILDONG_DIALOG_STATIC_2,			TEXT("statictext_Static2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,		NHONGGILDONG_DIALOG_STATIC_SKILLLEVEL,	this),	NHONGGILDONG_DIALOG_STATIC_SKILLLEVEL,	TEXT("statictext_SkillLevel") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,		NHONGGILDONG_DIALOG_STATIC_LEFTSKILL,	this),	NHONGGILDONG_DIALOG_STATIC_LEFTSKILL,	TEXT("statictext_LeftSkill") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NHONGGILDONG_DIALOG_EDIT_SKILLSTATUS,	this),	NHONGGILDONG_DIALOG_EDIT_SKILLSTATUS,	TEXT("editbox_SkillStatus") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NHONGGILDONG_DIALOG_EDIT_EXPLAIN,		this),	NHONGGILDONG_DIALOG_EDIT_EXPLAIN,		TEXT("editbox_HongGilDong") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(COMBOBOX,	NHONGGILDONG_DIALOG_COMBO_HIDDEN1,		this),	NHONGGILDONG_DIALOG_COMBO_HIDDEN1,		TEXT("combobox_hidden1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(COMBOBOX,	NHONGGILDONG_DIALOG_COMBO_HIDDEN2,		this),	NHONGGILDONG_DIALOG_COMBO_HIDDEN2,		TEXT("combobox_hidden2") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,		NHONGGILDONG_DIALOG_OUTLILE_1,			this),	NHONGGILDONG_DIALOG_OUTLILE_1,			TEXT("outline_NONAME1") );
		
	GetSkillLevel();
	SkillTabChange( NHONGGILDONG_DIALOG_BUTTON_TAB_SWORD );
	SetTimerOnOff( true );
	SetBasicInfo();

}

void CNHongGilDongDlg::SetBasicInfo( void )
{
	TCHAR	szFilePath[256]	= { '\0' };
	SI32	siFont			= 0;
	SI32	siKind			= pclClient->GetUniqueFromHash( TEXT("KIND_ALEX") );

	if ( false == pclClient->pclKindInfoSet->GetNPCImageFileAndFont(siKind, szFilePath, sizeof(szFilePath), &siFont) )
	{
		return;
	}

	CImageStatic* pctrlImage = m_InterfaceFactory.GetImageStatic( NHONGGILDONG_DIALOG_PORTRAIT );
	if ( pctrlImage )
	{
		pctrlImage->SetFileName( szFilePath );
		pctrlImage->SetFontIndex( siFont );
		pctrlImage->Refresh();
	}

	CEdit* pctrlExplain = m_InterfaceFactory.GetEdit( NHONGGILDONG_DIALOG_EDIT_EXPLAIN );
	if ( pctrlExplain )
	{
		TCHAR* pText = GetTxtFromMgr(5224);

		pctrlExplain->SetText( pText );
	}

	SI32 siTextPrintOption = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	CStatic* pctrlLeftSkill = m_InterfaceFactory.GetStatic( NHONGGILDONG_DIALOG_STATIC_LEFTSKILL );
	if ( pctrlLeftSkill )
	{
		pctrlLeftSkill->SetTextPrintOption( siTextPrintOption );
	}

	CStatic* pctrlSkillLevel = m_InterfaceFactory.GetStatic( NHONGGILDONG_DIALOG_STATIC_SKILLLEVEL );
	if ( pctrlSkillLevel )
	{
		pctrlSkillLevel->SetTextPrintOption( siTextPrintOption );
	}
	
}

void CALLBACK CNHongGilDongDlg::StaticCallBackDialogNSkill( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHongGilDongDlg *pThis = (CNHongGilDongDlg *)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNSkill( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNHongGilDongDlg::CallBackDialogNSkill( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( nControlID )
	{	
	case NHONGGILDONG_DIALOG_BUTTON_OK:
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NHONGGILDONG_DIALOG_TREE:
		{
			switch ( nEvent )
			{
			case EVENT_TREE_SELECTION:
				{
					OnTreeSelection();
				}
				break;
			}
		}
		break;

	case NHONGGILDONG_DIALOG_BUTTON_TAB_SWORD:
	case NHONGGILDONG_DIALOG_BUTTON_TAB_SPEAR:
	case NHONGGILDONG_DIALOG_BUTTON_TAB_AXE:
	case NHONGGILDONG_DIALOG_BUTTON_TAB_STAFF:
	case NHONGGILDONG_DIALOG_BUTTON_TAB_BOW:
	case NHONGGILDONG_DIALOG_BUTTON_TAB_GUN:
		{
			// [영훈] 검술 버튼 클릭시 퀘스트 검사
			if ( NHONGGILDONG_DIALOG_BUTTON_TAB_SWORD == nControlID )
			{
				pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_SWORDSKILLCLICK );
			}

			SkillTabChange( nControlID );
		}
		break;

	case NHONGGILDONG_DIALOG_BUTTON_SKILLPLUS:
		{
			OnButtonSkillPlus();
		}
		break;
	
	case NHONGGILDONG_DIALOG_COMBO_HIDDEN1:
	case NHONGGILDONG_DIALOG_COMBO_HIDDEN2:
		{
			switch ( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					// 히든 무기사용술을 선택한것으로 변경
					OnComboBoxSelection( nControlID );
				}
				break;
			}
		}
		break;

	}
	return;
}

void CNHongGilDongDlg::Action()
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	SI32 siChangedIndex = IsChanged();
	if ( (0 < siChangedIndex) && (MAX_SKILL_NUMBER > siChangedIndex) )
	{
		// 변경된 스킬트리 아이템 정보
		UpdataTreeViewItem( siChangedIndex );

		// 히든스킬
		ShowHiddenSkill();

		//
		GetSkillLevel();

		//		SendPacketGetWeaponSkillCost();
	}
	
	// 선택한 스킬 설명
	if ( (0 < m_siCurrentSkillUnique) && (MAX_SKILL_NUMBER > m_siCurrentSkillUnique) )
	{
		//	체크 시간 간격
		const SI32 ConstIntervalTime = 1000;
		SI32	siTmpTime = GetTickCount();
		// 선택한 스킬 비용이 0골드이고 자동 요청한지 1초이상 지난경우 자동으로 비용을 받아온다.
		if(siTmpTime < m_siRequestCostTime)
			m_siRequestCostTime = 0;
		if((m_siCostMoney == 0) &&
		   ((siTmpTime - m_siRequestCostTime) >= ConstIntervalTime) )
		{
			SendPacketGetWeaponSkillCost();
		}

		// 스킬 설명
		SetSkillExplainText( );
	}

	// 스킬 보너스 남은양
	CStatic* pctrlLeftSkill = m_InterfaceFactory.GetStatic( NHONGGILDONG_DIALOG_STATIC_LEFTSKILL );
	if ( NULL == pctrlLeftSkill )
	{
		return;
	}

	pctrlLeftSkill->SetText( SI32ToString(pclchar->pclCI->clIP.GetSkillBonus()) );

	return;
}

void CNHongGilDongDlg::OnButtonSkillPlus( void )
{
	if ( (0 >= m_siCurrentSkillUnique) || (MAX_SKILL_NUMBER <= m_siCurrentSkillUnique) )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	CEdit* pctrlExplain = m_InterfaceFactory.GetEdit( NHONGGILDONG_DIALOG_EDIT_EXPLAIN );
	if ( NULL == pctrlExplain )
	{
		return;
	}

	// 현재 스킬 체크
	SI32 siNowSkillLevel = pclchar->pclCI->clCharSkill.GetSkillLevel( m_siCurrentSkillUnique );
	if ( MAX_WEAPONSKILL_LEVEL <= siNowSkillLevel )
	{
		TCHAR* pText = GetTxtFromMgr(9788);

		pctrlExplain->SetText( pText );
		return;
	}

	// 현재 스킬레벨에서 1단계 업그레이드 시키므로 +1을 더해준다
	SI32 siUGSkillLevel = siNowSkillLevel + 1;

	// 남은 스킬 보너스 체크
	SI32 siNeedSkillPoint = 0;
	if ( false == pclClient->pclSkillManager->GetWeaponSkillNeedSkillPoint(m_siCurrentSkillUnique, siUGSkillLevel, &siNeedSkillPoint) )
	{
		// 음 이건 어떻게 할까?..
		return;
	}

	// 이값은 1이상이어야 한다
	if ( 0 >= siNeedSkillPoint )
	{
		return;
	}

	SI32 siSkillBonus = pclchar->pclCI->clIP.GetSkillBonus();
	if ( siSkillBonus < siNeedSkillPoint )
	{
		TCHAR* pText = GetTxtFromMgr(9786);	// 메시지가 동일하지만 다르게 분리될 수 있으므로 처리루틴도 분리 했다

		pctrlExplain->SetText( pText );
		return;
	}

	//-----------------------------------------------------------------------
	// 히든 스킬 일때
	//-----------------------------------------------------------------------
	if ( true == m_bSelectHiddenWeaponSkill )
	{
		//-----------------------------------------------------------------------
		// 우선 실제로 배울 수 있는지 조건 검사
		if ( false == pclClient->pclSkillManager->IsUpgradeHiddenWeaponSkill(m_siCurrentSkillUnique, siUGSkillLevel, 1) )
		{
			TCHAR* pText = GetTxtFromMgr(9789);

			pctrlExplain->SetText( pText );
			return;
		}

		//-----------------------------------------------------------------------
		// 히든스킬은 특정 아이템이 소모된다
		cltItem clGetItem;
		clGetItem.Init();

		if ( false == pclClient->pclSkillManager->GetHiddenWeaponSkillItem(m_siCurrentSkillUnique, siUGSkillLevel, &clGetItem) )
		{
			// 음 어떻게 처리할까
			return;
		}

		bool	bItemCheck		= false;

		SI32	siNeedItemPos	= pclchar->pclCI->clCharItem.FindItemInv(&clGetItem, false);
		if ( 0 < siNeedItemPos )
		{
			if ( clGetItem.GetItemNum() <= pclchar->pclCI->clCharItem.clItem[ siNeedItemPos ].GetItemNum() )
			{
				bItemCheck = true;
			}
		}

		if ( false == bItemCheck )
		{
			NTCHARString256 strText( GetTxtFromMgr(9790) );

			strText.Replace( TEXT("#itemname#"),	clGetItem.GetShortName(pclClient->pclItemManager) );
			strText.Replace( TEXT("#itemnum#"),		SI32ToString(clGetItem.GetItemNum()) );

			pctrlExplain->SetText( strText );
			return;
		}
	}
	//-----------------------------------------------------------------------
	// 그냥 전투 향상술
	//-----------------------------------------------------------------------
	else
	{
		//-----------------------------------------------------------------------
		// 우선 실제로 배울 수 있는지 조건 검사
		if ( false == pclClient->pclSkillManager->IsUpgradeWeaponSkill(m_siCurrentSkillUnique, siUGSkillLevel, 1) )
		{
			TCHAR* pText = GetTxtFromMgr(9789);

			pctrlExplain->SetText( pText );
			return;
		}

		//-----------------------------------------------------------------------
		// 스킬을 배우기 위한 돈이 부족할때

		// 아직 서버로 부터 가격 정보를 받지 않았다
		if ( 0 >= m_siCostMoney )
		{
			SendPacketGetWeaponSkillCost();
			return;
		}

		GMONEY gmMyPocketMoney = pclchar->pclCI->clIP.GetMoney();
		if ( (0 < m_siCostMoney) && (m_siCostMoney > gmMyPocketMoney) )
		{
			TCHAR* pText = GetTxtFromMgr(9787);

			pctrlExplain->SetText( pText );
			return;
		}
	}

	// 메시지 출력
	ShowRegMsgBox( siUGSkillLevel );
}

void CNHongGilDongDlg::ShowRegMsgBox( SI32 siUpgradeSkillLevel )
{
	if ( (0 >= siUpgradeSkillLevel) || (MAX_WEAPONSKILL_LEVEL < siUpgradeSkillLevel) )
	{
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	SI32 siNeedSkillPoint = 0;
	if ( false == pclClient->pclSkillManager->GetWeaponSkillNeedSkillPoint(m_siCurrentSkillUnique, siUpgradeSkillLevel, &siNeedSkillPoint) )
	{
		// 음 이건 어떻게 할까?..
		return;
	}

	// 소모 스킬포인트는 0이 나올수는 없다 
	if ( 0 >= siNeedSkillPoint )
	{
		return;
	}


	NTCHARString32	strTitle( GetTxtFromMgr(5138) );
	NTCHARString512	strText;

	if ( m_bSelectHiddenWeaponSkill )
	{
		cltItem clNeedItem;
		pclClient->pclSkillManager->GetHiddenWeaponSkillItem( m_siCurrentSkillUnique, siUpgradeSkillLevel, &clNeedItem );

		strText = GetTxtFromMgr(9794);

		strText.Replace( TEXT("#skillnum#"),	SI32ToString(siNeedSkillPoint) );
		strText.Replace( TEXT("#itemnum#"),		SI32ToString(clNeedItem.GetItemNum()) );
		strText.Replace( TEXT("#itemname#"),	clNeedItem.GetShortName(pclClient->pclItemManager) );
	}
	else
	{
		strText	= GetTxtFromMgr(9793);

		strText.Replace( TEXT("#skillnum#"),	SI32ToString(siNeedSkillPoint) );
		strText.Replace( TEXT("#Money#"),		SI64ToString(m_siCostMoney) );
	}

	stMsgBox msgBox;

	msgBox.Set( this, strTitle, strText, MSGBOX_TYPE_YESNO, 0 );

	stHongGilDongMsgData msgData;

	msgData.siSkillUnique	= m_siCurrentSkillUnique;
	msgData.siSkillLevel	= siUpgradeSkillLevel;

	pclclient->SetMsgBox( &msgBox, (BYTE*)&msgData, sizeof(msgData) );

}

void CNHongGilDongDlg::OnTreeSelection( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	CTree* pctrlTree = m_InterfaceFactory.GetTree( NHONGGILDONG_DIALOG_TREE );
	if ( NULL == pctrlTree )
	{
		return;
	}

	SI16 siSelectedIndex = pctrlTree->GetSelectedItemIndex();
	if ( 0 > siSelectedIndex )
	{
		return;
	}

	stTreeItem *pTreeItem = pctrlTree->GetTreeItemFromIndex( siSelectedIndex );
	if ( NULL == pTreeItem )
	{
		return;
	}

	if ( (0 >= pTreeItem->siParam) || (MAX_SKILL_NUMBER <= pTreeItem->siParam) )
	{
		return;
	}

	// 트리 아이템을 셋팅할때 스킬 유니크를 Param에 넣어놨으므로 가져온다
	m_siCurrentSkillUnique = pTreeItem->siParam;

	// 기본적인 무기사용술을 선택한것으로 변경
	m_bSelectHiddenWeaponSkill = false;

	// 선택한 스킬이 히든스킬이라면
	if ( pclclient->pclSkillManager->pclSkillInfo[m_siCurrentSkillUnique] )
	{
		SI16 siSkillAtb = pclclient->pclSkillManager->pclSkillInfo[m_siCurrentSkillUnique]->siSkillAtb;
		if ( (SKILLATB_HIDDENGRADE1 & siSkillAtb) || (SKILLATB_HIDDENGRADE2 & siSkillAtb) )
		{
			m_bSelectHiddenWeaponSkill = true;
		}
	}

	// 히든 스킬일 경우 가격정보를 얻어올 필요가 없다
	if ( true == m_bSelectHiddenWeaponSkill )
	{
		return;
	}

	// 우선 기본적으로 스킬레벨만 검사해서 스킬을 배울 수 있다면 서버에 가격 요청을 한다
	SI32 siSkillLevel	= pclchar->pclCI->clCharSkill.GetSkillLevel( m_siCurrentSkillUnique );
	SI32 siUGSkillLevel	= siSkillLevel + 1; // 현재 스킬보다 한단계 위인 스킬레벨을 배워야 함으로 +1을 해준다
	if ( (0 >= siUGSkillLevel) || (MAX_WEAPONSKILL_LEVEL < siUGSkillLevel) )
	{
		return;
	}


	if(IsRapidlyClicked(siSelectedIndex) == FALSE)
		return;
	
	// 트리 노드 선택
	m_siSelectedTreeIndex = siSelectedIndex;

	// 가격 정보는 초기화 시켜 준다
	m_siCostMoney = 0;
	
	// 비용값 얻기
	SendPacketGetWeaponSkillCost();
}

void CNHongGilDongDlg::OnComboBoxSelection( SI16 siComboBoxID )
{
	if ( (NHONGGILDONG_DIALOG_COMBO_HIDDEN1 != siComboBoxID) && (NHONGGILDONG_DIALOG_COMBO_HIDDEN2 != siComboBoxID) )
	{
		return;
	}

	CComboBox* pctrlHidden = m_InterfaceFactory.GetComboBox( siComboBoxID );
	if ( NULL == pctrlHidden )
	{
		return;
	}

	stComboBoxItemData* pComboBoxItem = pctrlHidden->GetItemData( pctrlHidden->GetSelectedIndex() );
	if ( NULL == pComboBoxItem )
	{
		return;
	}

	if ( (0 >= pComboBoxItem->siParam) || (MAX_SKILL_NUMBER <= pComboBoxItem->siParam) )
	{
		return;
	}
	
	m_siCurrentSkillUnique = pComboBoxItem->siParam;

	// 히든 무기사용술을 선택한것으로 변경
	m_bSelectHiddenWeaponSkill = true;
}

void CNHongGilDongDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	switch(MsgIndex)
	{
	case 0:
		{
			if ( RetValue )
			{
				stHongGilDongMsgData* pclInfo = (stHongGilDongMsgData*)pData;
				if ( NULL == pclInfo )
				{
					return;
				}

				cltGameMsgRequest_Set_WeaponSkill clinfo( pclInfo->siSkillUnique, pclInfo->siSkillLevel );
				cltMsg clMsg( GAMEMSG_REQUEST_SET_WEAPONSKILL, sizeof(clinfo), (BYTE*)&clinfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	default:
		return ;
	}

	return;
}

void CNHongGilDongDlg::SkillTabChange( SI16 siControllerID )
{
	/*
	if ( (0 > siControllerID) || (m_siCurrentTabControllerID == siControllerID) )
	{
		return;
	}
	*/
	// 스킬타입을 못 가져온다면 다른 컨트롤러ID가 들어온 것이다
	SI16 siWeaponSkillType = GetWeaponSkillTypeFromControllerID( siControllerID );
	if ( 0 >= siWeaponSkillType )
	{
		return;
	}

	CTree* pctrlTree = m_InterfaceFactory.GetTree( NHONGGILDONG_DIALOG_TREE );
	if ( NULL == pctrlTree )
	{
		return;
	}

	CEdit* pctrlExplain = m_InterfaceFactory.GetEdit( NHONGGILDONG_DIALOG_EDIT_EXPLAIN );
	if ( NULL == pctrlExplain )
	{
		return;
	}

	CEdit* pctrlSkillStatus = m_InterfaceFactory.GetEdit( NHONGGILDONG_DIALOG_EDIT_SKILLSTATUS );
	if ( NULL == pctrlSkillStatus )
	{
		return;
	}

	// 저장해 놓는다
	m_siCurrentTabControllerID = siControllerID;
	
	// 버튼 폰트 상태 변경
	SetAllTabButtonFontStatus( siControllerID );

	// 트리 정보 변경
	pctrlTree->Clear();

	TreeViewInsert( siWeaponSkillType, 0 );

	pctrlTree->Refresh();

	// 히든 스킬 정보
	ShowHiddenSkill();

	// 관련 변수들은 초기화
	m_siCurrentSkillUnique		= 0;
	m_siCurrentSkillTypeUnique	= 0;
	m_siCostMoney				= 0;
	m_bSelectHiddenWeaponSkill	= false;

	// 설명 텍스트를 초기문구로 변경한다
	TCHAR* pText = GetTxtFromMgr(5224);
	pctrlExplain->SetText( pText );

	// 기존 스킬관련 텍스트를 지운다
	pctrlSkillStatus->SetText( TEXT("") );

	return;
}

void CNHongGilDongDlg::SetAllTabButtonFontStatus( SI16 siControllerID )
{
	// Tab버튼 컨트롤러 ID
	SI16 siTabButtonID[] = {
		NHONGGILDONG_DIALOG_BUTTON_TAB_SWORD,
		NHONGGILDONG_DIALOG_BUTTON_TAB_SPEAR,
		NHONGGILDONG_DIALOG_BUTTON_TAB_AXE,	
		NHONGGILDONG_DIALOG_BUTTON_TAB_STAFF,
		NHONGGILDONG_DIALOG_BUTTON_TAB_BOW,	
		NHONGGILDONG_DIALOG_BUTTON_TAB_GUN
	};

	SI32 siTabButtonIDCount = sizeof(siTabButtonID) / sizeof(siTabButtonID[0]);
	if ( 0 >= siTabButtonIDCount )
	{
		return;
	}

	CButton* pctrlTabButton = NULL;

	for ( SI32 siCount=0; siCount<siTabButtonIDCount; ++siCount )
	{
		pctrlTabButton = m_InterfaceFactory.GetButton( siTabButtonID[siCount] );
		if ( NULL == pctrlTabButton )
		{
			continue;
		}

		if ( siControllerID == siTabButtonID[siCount] )
		{
			pctrlTabButton->SetFontColor( CONTROLLER_BUTTONTEXT_COLOR );
			pctrlTabButton->SetFontWeight( FW_BOLD );
		}
		else
		{
			pctrlTabButton->SetFontColor( CONTROLLER_BACK_BUTTONTEXT_COLOR );
			pctrlTabButton->SetFontWeight( FW_NORMAL );
		}
		
		pctrlTabButton->Refresh();
	}

}

void CNHongGilDongDlg::SetSkillExplainText( void )
{
	if ( (0 > m_siCurrentSkillUnique) || (MAX_SKILL_NUMBER <= m_siCurrentSkillUnique) )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	CEdit* pctrlSkillStatus = m_InterfaceFactory.GetEdit( NHONGGILDONG_DIALOG_EDIT_SKILLSTATUS );
	if ( NULL == pctrlSkillStatus )
	{
		return;
	}

	CStatic* pctrlSkillLevel = m_InterfaceFactory.GetStatic( NHONGGILDONG_DIALOG_STATIC_SKILLLEVEL );
	if ( NULL == pctrlSkillLevel )
	{
		return;
	}

	//-----------------------------------------------------------------------------------------------------------------
	// 스킬 설명관련 텍스트를 뿌려준다 
	//-----------------------------------------------------------------------------------------------------------------
	const	SI32	ConstBufferSize = 512;
	TCHAR	szBuffer[ConstBufferSize]	= TEXT("");
	SI32	siSkillLevel	= pclchar->pclCI->clCharSkill.GetSkillLevel( m_siCurrentSkillUnique );

	SI32	siMaxSkillLevel	= pclClient->pclSkillManager->GetMaxLevel( m_siCurrentSkillUnique );
	SI32	siUGSkillLevel	= pclClient->pclItemManager->GetUpgradeSkillLevel( &pclchar->pclCI->clCharItem.clItem[0], m_siCurrentSkillUnique, 1 );

	// 기본 기술레벨 텍스트
	TCHAR*	pText			= GetTxtFromMgr(5202);
	StringCchPrintf( szBuffer, ConstBufferSize, pText, siSkillLevel, siMaxSkillLevel, siUGSkillLevel );

	// 무기술 설명 텍스트
	SI32	siNextLevel		= siSkillLevel + 1;	// 업그레이드 되야하는 레벨의 정보를 가져와야 하므로 +1을 한다
	TCHAR	szExplain[ConstBufferSize]	= TEXT("");
	pclClient->pclSkillManager->GetWeaponSkillExplain( m_siCurrentSkillUnique, siNextLevel, szExplain );

	// 모두 합쳐서 뿌려준다
	StringCchCat( szBuffer, ConstBufferSize, szExplain );

	//-----------------------------------------------------------------------------------------------------------------
	// 비용 설명관련 텍스트를 뿌려준다 
	//-----------------------------------------------------------------------------------------------------------------
	if ( (0 < siNextLevel) && (MAX_WEAPONSKILL_LEVEL >= siNextLevel) )
	{
		TCHAR*	pszTextNeed	= GetTxtFromMgr(30265);
		TCHAR*	pszTextGold = GetTxtFromMgr(611);
		TCHAR   szBufferMoney[ConstBufferSize] ;
		ZeroMemory(szBufferMoney, ConstBufferSize * sizeof(TCHAR));
		g_JYLibFunction.SetNumUnit( m_siCostMoney, szBufferMoney, ConstBufferSize, pszTextGold );
		TCHAR	szCostBuffer[ConstBufferSize];
		ZeroMemory(szCostBuffer, ConstBufferSize * sizeof(TCHAR));
		StringCchPrintf( szCostBuffer, ConstBufferSize, TEXT("\r\n%s : %s "), pszTextNeed, szBufferMoney );

		// 모두 합쳐서 뿌려준다
		StringCchCat( szBuffer, ConstBufferSize, szCostBuffer );
	}

	pctrlSkillStatus->SetText( szBuffer );

	//-----------------------------------------------------------------------------------------------------------------
	// 해당 스킬의 레벨을 뿌려준다
	//-----------------------------------------------------------------------------------------------------------------
	NTCHARString64 strSkillLevel( TEXT("#nowlevel#/#maxlevel#") );

	strSkillLevel.Replace( TEXT("#nowlevel#"), SI32ToString(siSkillLevel) );
	strSkillLevel.Replace( TEXT("#maxlevel#"), SI32ToString(siMaxSkillLevel) );

	pctrlSkillLevel->SetText( strSkillLevel );

}

//-----------------------------------------------------------------------------------------------------------------------------
// 설명 : 제귀호출 함수, 해당 무기 타입의 무기 사용술의 아이템을 Tree에 넣는다 제귀호출로 자식의 아이템들을 넣어준다
// 인수 : SI32 siWeaponType = 추가하고자하는 무기술 타입, 
//		  SI32 siParentRef = 부모 레퍼런스
// 반환 : void
//-----------------------------------------------------------------------------------------------------------------------------
void CNHongGilDongDlg::TreeViewInsert( SI32 siWeaponType, SI32 siParentRef, SI16 siDeeps )
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	CTree* pctrlTree = m_InterfaceFactory.GetTree( NHONGGILDONG_DIALOG_TREE );
	if ( NULL == pctrlTree )
	{
		return;
	}

	stTreeItem		sTreeItem;
	cltSkillInfo*	pclGetSkillInfo = NULL;

	for ( SI32 siIndex = 0; siIndex < MAX_SKILL_NUMBER; ++siIndex )
	{
		pclGetSkillInfo = pclClient->pclSkillManager->pclSkillInfo[siIndex];
		if ( NULL == pclGetSkillInfo )
		{
			continue;
		}

		// 무기사용술이 아니면 패스
		if ( SKILLTYPE_FIGHT != pclGetSkillInfo->siSkillType )
		{
			continue;
		}

		// 현재 무기 타입과 달라도 패스
		if ( siWeaponType != pclGetSkillInfo->siWeaponType )
		{
			continue;
		}

		// 부모 레퍼런스가 달라도 패스
		if ( siParentRef != pclGetSkillInfo->siParentRef )
		{
			//continue;
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
		if ( pclGetSkillInfo->siSkillAtb & SKILLATB_HIDDENGRADE1 )
		{
			if ( 4 != siDeeps )
			{
				continue;
			}
		}
		else if ( pclGetSkillInfo->siSkillAtb & SKILLATB_HIDDENGRADE2 )
		{
			if ( 5 != siDeeps )
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

		// 실질적으로 Tree에 들어갈 아이템
		sTreeItem.Init();

		sTreeItem.siIndex		=	siIndex;
		sTreeItem.siFileIndex	=	1;
		sTreeItem.siImageIndex	=	pclGetSkillInfo->siBitMapIndex;
		sTreeItem.siParam		=	pclGetSkillInfo->siUnique;
		sTreeItem.siParentIndex =	siParentRef;

		// 무기술 사용 설명을 갖고 온다
		pclClient->pclSkillManager->MakeWeaponSkillTitle( &pclchar->pclCI->clCharSkill, &pclchar->pclCI->clCharItem, pclGetSkillInfo->siUnique );
		StringCchCopy( sTreeItem.strText, MAX_TREE_TEXT_LEN, pclGetSkillInfo->szSkillTitle );

		// 정보를 Tree에 넣어주고
		pctrlTree->InsertItem( &sTreeItem );

		// 제귀호출을 통해 자식 아이템들을 넣어준다
		TreeViewInsert( siWeaponType, siIndex, siDeeps+1 );
		
	}

	return;
}

//-----------------------------------------------------------------------------------------------------------------------------
// 설명 : 이미 설정된 Tree Item중 하나가 변경되어 그 아이템의 내용을 변경한다
// 인수 : SI32 siChangeItemIndex = 변경된 스킬의 유니크
// 반환 : void
//-----------------------------------------------------------------------------------------------------------------------------
void CNHongGilDongDlg::UpdataTreeViewItem( SI32 siChangeItemIndex )
{
	if ( (0 >= siChangeItemIndex) || (MAX_SKILL_NUMBER <= siChangeItemIndex) )
	{
		return;
	}

	CTree* pctrlTree = m_InterfaceFactory.GetTree( NHONGGILDONG_DIALOG_TREE );
	if ( NULL == pctrlTree )
	{
		return;
	}

	stTreeItem* pTreeItem = pctrlTree->GetTreeItemFromIndex( siChangeItemIndex );
	if ( NULL == pTreeItem )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	cltSkillInfo* pclSkillInfo = pclClient->pclSkillManager->pclSkillInfo[siChangeItemIndex];
	if ( NULL == pclSkillInfo )
	{
		return;
	}

	SI32 siGetTotalSkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel( pclSkillInfo->siUnique, 0, 0, &pclchar->pclCI->clCharItem.clItem[0], 1 );

	// 스킬 변경내용을 Tree Item에 다시 넣어준다
	pclClient->pclSkillManager->MakeWeaponSkillTitle( &pclchar->pclCI->clCharSkill, &pclchar->pclCI->clCharItem, pclSkillInfo->siUnique );

	StringCchCopy( pTreeItem->strText, MAX_TREE_TEXT_LEN, pclSkillInfo->szSkillTitle );

	pctrlTree->UpdateItem( pTreeItem );

}

//-----------------------------------------------------------------------------------------------------------------------------
// 설명 : 캐릭터의 모든 스킬의 레벨을 저장해 두었다가 Action()함수에서 스킬의 변경된 레벨을 검사하기 위해 모든 스킬의 레벨을 저장해 둔다
// 인수 : void
// 반환 : void
//-----------------------------------------------------------------------------------------------------------------------------
void CNHongGilDongDlg::GetSkillLevel()
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	cltSkillInfo* pclSkillInfo = NULL;

	for( SI32 i = 1; i < MAX_SKILL_NUMBER; ++i )
	{
		pclSkillInfo = pclClient->pclSkillManager->pclSkillInfo[i];
		if ( NULL == pclSkillInfo )
		{
			continue;
		}

		m_siSkillLevel[ i ] = pclchar->pclCI->clCharSkill.GetTotalSkillLevel( pclSkillInfo->siUnique, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]);
	}

	return;
}

//-----------------------------------------------------------------------------------------------------------------------------
// 설명 : GetSkillLevel()에서 저장해 두었던 레벨을 기준으로 스킬 레벨이 변경된 것이 있는지 검사한다
// 인수 : void
// 반환 : 레벨이 변경된 스킬의 Unique를 반환
//-----------------------------------------------------------------------------------------------------------------------------
SI16 CNHongGilDongDlg::IsChanged()
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return 0;
	}

	cltSkillInfo*	pclSkillInfo	= NULL;
	SI32			siGetSkillLevel	= 0;

	for ( SI32 i = 1; i < MAX_SKILL_NUMBER; ++i )
	{
		pclSkillInfo = pclClient->pclSkillManager->pclSkillInfo[i];
		if ( NULL == pclSkillInfo )
		{
			continue;
		}

		siGetSkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel( pclSkillInfo->siUnique, 0, 0, &pclchar->pclCI->clCharItem.clItem[0] );
		if ( m_siSkillLevel[ i ] != siGetSkillLevel )
		{
			return i;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------
// 설명 : 서버에 요청했던 내용을 다루는 함수 배우는금액/스킬배우기성공/스킬배우기실패의 정보를 다룬다
// 인수 : void 포인터형(stHongGilDongData 의 포인터)
// 반환 : void
//-----------------------------------------------------------------------------------------------------------------------------
void CNHongGilDongDlg::SetData(void *pBuffer)
{
	stHongGilDongData* pclinfo = (stHongGilDongData*)pBuffer;
	if ( NULL == pclinfo )
	{
		return;
	}

	CEdit* pctrlExplain = m_InterfaceFactory.GetEdit( NHONGGILDONG_DIALOG_EDIT_EXPLAIN );
	if ( NULL == pctrlExplain )
	{
		return;
	}

	SI32 siMsgType = pclinfo->Type;

	switch ( siMsgType )
	{
	case HONGGILDONG_SETDATATYPE_GETCOSTINFO:
		{
			/*
			TCHAR buffer[512] = TEXT("") ;
			TCHAR * pText = GetTxtFromMgr(5225);

			StringCchPrintf(buffer, 512, pText,pclinfo->SkillLevel,pclinfo->CostMoney);

			pctrlExplain->SetText(buffer);
			pctrlExplain->Refresh();
			*/
			m_siCostMoney = pclinfo->CostMoney;
		}
		break;

	case HONGGILDONG_SETDATATYPE_SUCCESSSKILLUP:
		{
			if ( pclinfo->Result )
			{
				TCHAR* pText = GetTxtFromMgr(5226);

				pctrlExplain->SetText(pText);
				pctrlExplain->Refresh();

				pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_WEAPONSKILL_UP")),0,0);

				SkillTabChange( m_siCurrentTabControllerID );
			}
		}
		break;

	case HONGGILDONG_SETDATATYPE_NOTENOUGHTMONEY:
		{
			TCHAR *pText = GetTxtFromMgr(5521);
			TCHAR buffer[256] ;
			StringCchPrintf(buffer, 256, pText,pclinfo->CostMoney*10000) ;
			pctrlExplain->SetText(buffer);
			pctrlExplain->Refresh() ;
		}
		break;
	}

}

//-----------------------------------------------------------------------------------------------------------------------------
// 설명 : 컨트롤 아이디에 해당하는 무기술 타입을 가져온다(텝 형식의 버튼때문)
// 인수 : 텝버튼 컨트롤러 ID
// 반환 : 무기술 타입
//-----------------------------------------------------------------------------------------------------------------------------
SI16 CNHongGilDongDlg::GetWeaponSkillTypeFromControllerID( SI16 siControllerID )
{
	SI16 siWeaponSkillType = 0;

	switch ( siControllerID )
	{
		case NHONGGILDONG_DIALOG_BUTTON_TAB_SWORD:	{	siWeaponSkillType = WEAPONSKILLTYPE_SWORD;	}	break;
		case NHONGGILDONG_DIALOG_BUTTON_TAB_SPEAR:	{	siWeaponSkillType =	WEAPONSKILLTYPE_SPEAR;	}	break;
		case NHONGGILDONG_DIALOG_BUTTON_TAB_AXE:	{	siWeaponSkillType =	WEAPONSKILLTYPE_AXE;	}	break;
		case NHONGGILDONG_DIALOG_BUTTON_TAB_STAFF:	{	siWeaponSkillType =	WEAPONSKILLTYPE_STAFF;	}	break;
		case NHONGGILDONG_DIALOG_BUTTON_TAB_BOW:	{	siWeaponSkillType =	WEAPONSKILLTYPE_BOW;	}	break;
		case NHONGGILDONG_DIALOG_BUTTON_TAB_GUN:	{	siWeaponSkillType =	WEAPONSKILLTYPE_GUN;	}	break;
	}

	return siWeaponSkillType;
}

void CNHongGilDongDlg::ShowHiddenSkill( void )
{
	SI16 siNowWeaponSkillType = GetWeaponSkillTypeFromControllerID( m_siCurrentTabControllerID );
	if ( 0 >= siNowWeaponSkillType )
	{
		return;
	}

	SI16 siHiddenGrade1[MAX_WEAPONSKILLTYPE_NUM] = { 0, };
	SI16 siHiddenGrade2[MAX_WEAPONSKILLTYPE_NUM] = { 0, };

	// 우선 히든 스킬 목록을 가져온다
	GetHiddenWeaponSkillFromWeaponSkillType( siNowWeaponSkillType, siHiddenGrade1, siHiddenGrade2 );

	// !!목록에 나타나는 것은 먼저 배운것이 있다면 그것만 나오도록 한다 아래 작업은 그것을 위한 것들!! //

	// 각 단계 히든 스킬 목록중 배운 스킬 인덱스를 가져온다
	SI16 siLearndeHiddenSkill1 = IsLearnedHiddenWeaponSkill( siHiddenGrade1 );
	SI16 siLearndeHiddenSkill2 = IsLearnedHiddenWeaponSkill( siHiddenGrade2 );

	// 히든 1단계(무기향상술 4단계) 스킬중 배운것이 있다면 모든 히든 스킬을 지워 버리고 배운 스킬로만 채워 넣는다
	if ( 0 < siLearndeHiddenSkill1 )
	{
		ZeroMemory( siHiddenGrade1, sizeof(siHiddenGrade1) );

		siHiddenGrade1[0] = siLearndeHiddenSkill1;
	}

	// 히든 2단계(무기향상술 5단계) 스킬중 배운것이 있다면 모든 히든 스킬을 지워 버리고 배운 스킬로만 채워 넣는다
	if ( 0 < siLearndeHiddenSkill2 )
	{
		ZeroMemory( siHiddenGrade2, sizeof(siHiddenGrade2) );

		siHiddenGrade2[0] = siLearndeHiddenSkill2;
	}

	// 목록의 스킬들을 콤보 박스에 넣는다(단 목록에 있는 스킬들을 목록에 넣을 수 있는지는 아래 함수 안에서 한다)
	AddHiddenSkillToComboBoxItem( NHONGGILDONG_DIALOG_COMBO_HIDDEN1, siNowWeaponSkillType, siHiddenGrade1 );
	AddHiddenSkillToComboBoxItem( NHONGGILDONG_DIALOG_COMBO_HIDDEN2, siNowWeaponSkillType, siHiddenGrade2 );

}

void CNHongGilDongDlg::GetHiddenWeaponSkillFromWeaponSkillType( IN SI16 siWeaponSkillType, OUT SI16* psiHiddenGrade1, OUT SI16* psiHiddenGrade2 )
{
	if ( (0 >= siWeaponSkillType) || (MAX_WEAPONSKILLTYPE_NUM < siWeaponSkillType) )
	{
		return;
	}

	if ( (NULL == psiHiddenGrade1) || (NULL == psiHiddenGrade2) )
	{
		return;
	}
	
	SI32			siHidden1Count	= 0;
	SI32			siHidden2Count	= 0;
	cltSkillInfo*	pclSkill		= NULL;

	for ( SI32 siSkillIndex=1; siSkillIndex<MAX_SKILL_NUMBER; ++siSkillIndex )
	{
		pclSkill = pclClient->pclSkillManager->pclSkillInfo[siSkillIndex];
		if ( NULL == pclSkill )
		{
			continue;
		}
		
		// 전투 향상 술이 아니면
		if ( SKILLTYPE_FIGHT != pclSkill->siSkillType )
		{
			continue;
		}
		
		// 해당 무기 타입이 아니면
		if ( siWeaponSkillType != pclSkill->siWeaponType )
		{
			continue;
		}
		
		// 스킬 히든 등급에 따라 목록에 추가한다
		if ( (SKILLATB_HIDDENGRADE1 & pclSkill->siSkillAtb) && (MAX_WEAPONSKILLTYPE_NUM > siHidden1Count) )
		{
			psiHiddenGrade1[siHidden1Count++] = siSkillIndex;
		}
		else if ( (SKILLATB_HIDDENGRADE2 & pclSkill->siSkillAtb) && (MAX_WEAPONSKILLTYPE_NUM > siHidden2Count) )
		{
			psiHiddenGrade2[siHidden2Count++] = siSkillIndex;
		}
	}
}

SI16 CNHongGilDongDlg::IsLearnedHiddenWeaponSkill( IN SI16 *psiHiddenSkillList )
{
	if ( NULL == psiHiddenSkillList )
	{
		return 0;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return 0;
	}

	SI16 siSkillUnique = 0;

	for ( SI32 siIndex=0; siIndex<MAX_WEAPONSKILLTYPE_NUM; ++siIndex )
	{
		siSkillUnique = psiHiddenSkillList[siIndex];
		if ( 0 >= siSkillUnique )
		{
			continue;
		}
		
		// 해당 히든스킬레벨이 1 이상이라면 배운것이다
		if ( 0 < pclchar->pclCI->clCharSkill.GetSkillLevel(siSkillUnique) )
		{
			return siSkillUnique;
		}
	}

	return 0;
}

void CNHongGilDongDlg::AddHiddenSkillToComboBoxItem( IN SI16 siComboBoxControllerID, IN SI16 siWeaponSkillType, IN SI16 *psiHiddenSkillList )
{
	if ( (siComboBoxControllerID != NHONGGILDONG_DIALOG_COMBO_HIDDEN1) && (siComboBoxControllerID != NHONGGILDONG_DIALOG_COMBO_HIDDEN2) )
	{
		return;
	}

	if ( NULL == psiHiddenSkillList )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}
	
	CComboBox* pctrlHiddenSkill = m_InterfaceFactory.GetComboBox( siComboBoxControllerID );
	if ( NULL == pctrlHiddenSkill )
	{
		return;
	}

	SI32 siFindSkillUnique = pclClient->pclSkillManager->FindSkillUniqueFromWeaponSkillType( siWeaponSkillType );
	if ( 0 >= siFindSkillUnique )
	{
		return;
	}

	SI16 siCharLevel	= pclchar->pclCI->clIP.GetLevel();
	SI16 siSkillLevel	= pclchar->pclCI->clCharSkill.GetSkillExpLevel( siFindSkillUnique );
	SI16 siSkillUnique	= 0;
	SI16 siParentUnique	= 0;

	cltSkillInfo*		pclSkill		= NULL;
	cltSkillInfo*		pclParentSkill	= NULL;
	cltWeaponSkillInfo*	pclWeaponSkill	= NULL;

	stComboBoxItemData	comboBoxData;

	// 우선 콤보박스를 모두 비운다
	pctrlHiddenSkill->Clear();

	for ( SI32 siIndex=0; siIndex<MAX_WEAPONSKILLTYPE_NUM; ++siIndex )
	{
		siSkillUnique = psiHiddenSkillList[siIndex];
		if ( 0 >= siSkillUnique )
		{
			continue;
		}

		pclSkill = pclClient->pclSkillManager->pclSkillInfo[siSkillUnique];
		if ( NULL == pclSkill )
		{
			continue;
		}

		siParentUnique = pclSkill->siParentRef;
		if ( (0 >= siParentUnique) || (MAX_SKILL_NUMBER <= siParentUnique) )
		{
			continue;
		}

		pclParentSkill = pclClient->pclSkillManager->pclSkillInfo[siParentUnique];
		if ( NULL == pclParentSkill )
		{
			continue;
		}

		if ( NULL == pclParentSkill->pclWeaponSkillInfo )
		{
			continue;
		}

		// 최소 레벨(1레벨, 인덱스는 0) 조건만 맞으면 추가한다
		pclWeaponSkill = pclParentSkill->pclWeaponSkillInfo[0];
		if ( NULL == pclWeaponSkill )
		{
			continue;
		}
		
		// 조건을 만족한다면 콤보박스에 추가한다
		if ( (siCharLevel >= pclWeaponSkill->siHiddenLimitLevel) && (siSkillLevel >= pclWeaponSkill->siHiddenLimitSkillLevel) )
		{
			comboBoxData.Init();

			pclClient->pclSkillManager->MakeWeaponSkillTitle( &pclchar->pclCI->clCharSkill, &pclchar->pclCI->clCharItem, siSkillUnique );
			
			// 스킬유니크와 텍스트를 같이 넣는다
			comboBoxData.Set( siSkillUnique, pclSkill->szSkillTitle );

			pctrlHiddenSkill->AddItem( &comboBoxData );
		}
	}

	// 히든스킬 목록이 있을때만 컨트롤을 보여준다
	if ( 0 < pctrlHiddenSkill->GetItemNum() )
	{
		pctrlHiddenSkill->Show( true );
	}
	else
	{
		pctrlHiddenSkill->Show( false );
	}

}

void CNHongGilDongDlg::SendPacketGetWeaponSkillCost( void )
{
	if ( (0 >= m_siCurrentSkillUnique) || (MAX_SKILL_NUMBER <= m_siCurrentSkillUnique) )
	{
		return;
	}

	if ( true == m_bSelectHiddenWeaponSkill )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	// 비용 요청 시간 
	m_siRequestCostTime = GetTickCount();

	// 우선 기본적으로 스킬레벨만 검사해서 스킬을 배울 수 있다면 서버에 가격 요청을 한다
	SI32 siSkillLevel	= pclchar->pclCI->clCharSkill.GetSkillLevel( m_siCurrentSkillUnique );
	SI32 siUGSkillLevel	= siSkillLevel + 1; // 현재 스킬보다 한단계 위인 스킬레벨을 배워야 함으로 +1을 해준다
	if ( (0 >= siUGSkillLevel) || (MAX_WEAPONSKILL_LEVEL < siUGSkillLevel) )
	{
		return;
	}

	cltGameMsgRequest_Get_WeaponSkillCost clinfo( m_siCurrentSkillUnique, siUGSkillLevel );
	cltMsg clMsg( GAMEMSG_REQUEST_GET_WEAPONSKILLCOST, sizeof(clinfo), (BYTE*)&clinfo );
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	
}

// 같은 트리 노드의 빠른 클릭 체크
BOOL CNHongGilDongDlg::IsRapidlyClicked(SI32 siSelectedIndex)
{
	if(siSelectedIndex < 0)
		return FALSE;

	// 빠른 클릭 방지용
	SI32 siTmpTime = GetTickCount();
	// 1초
	const SI32 ConstOneSecond = 1000;

	if( m_siTreeClickTime > siTmpTime)
	{
		m_siTreeClickTime = siTmpTime;
	}
	else
	{
		if(((siTmpTime - m_siTreeClickTime) < ConstOneSecond) && (m_siSelectedTreeIndex == siSelectedIndex))
		{
			return FALSE;
		}
		else
		{
			m_siTreeClickTime = siTmpTime;
		}
	}
	
	return TRUE;
}