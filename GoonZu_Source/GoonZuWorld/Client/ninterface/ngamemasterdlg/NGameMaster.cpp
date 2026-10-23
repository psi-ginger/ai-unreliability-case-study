/* ==========================================================================
클래스 :		CNGameMaster

작성일 :		05/10/31
작성자 :		최연준

변경사항 :		없음

차후작업 :		

========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)



#include "CommonLogic.h"
#include "Client.h"
#include "Resource.h"
#include <mbstring.h>
#include "MsgType-System.h"

#include "./NGameMaster.h"
#include "../../Common/GameMaster/GameMaster.h"
#include "../../../Common/Skill/Skill-Manager.h"
#include "../../../Common/Skill/Skill.h"
#include "../../../Common/Util/Util.h"

#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/interface/Button.h"
#include "../../InterfaceMgr/interface/List.h"
#include "../../InterfaceMgr/interface/Edit.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

extern cltCommonLogic* pclClient;

CNGameMaster::CNGameMaster()
{
	int i;
	m_listbox_GM_order = NULL;
	m_editbox_GM_info = NULL;
	m_editbox_GM_txt = NULL;

	for (i = 0; i < 9; i++)
		m_pStatic[i] = NULL;

	for (i = 0; i < 6; i++)
		m_pComboBox[i] = NULL;

	for (i = 0; i < 2; i++)
		m_pEdit[i] = NULL;

	m_button_ok = NULL;
	m_button_cancel = NULL;

	for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{	
		m_arrItemTypeIndex[i] = 0;
	}

}

CNGameMaster::~CNGameMaster()
{
	int i;

	NDelete(m_listbox_GM_order);
	NDelete(m_editbox_GM_info);
	NDelete(m_editbox_GM_txt);

	for (i = 0; i < 9; i++)
		NDelete(m_pStatic[i]);

	for (i = 0; i < 6; i++)
		NDelete(m_pComboBox[i]);

	for (i = 0; i < 2; i++)
		NDelete(m_pEdit[i]);

	NDelete(m_button_ok);
	NDelete(m_button_cancel);
}

void CNGameMaster::Create()
{
	if ( ! IsCreate() )
	{

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/DLG_NGameMasterOrder.ddf"));
		file.CreatePopupDialog( this, NGAMEMASTERORDER_DLG, TEXT("dialog_NGameMaster"), NGameMasterStaticProc);		
		
		m_listbox_GM_order = new CGMList(this);
		m_editbox_GM_info = new CEdit(this);
		m_editbox_GM_txt = new CEdit(this);
		m_button_ok = new CButton(this);
		m_button_cancel = new CButton(this);

		int i;
		for (i = 0; i < 9; i++)
			m_pStatic[i] = new CStatic(this);
		
		for (i = 0; i < 6; i++)
			m_pComboBox[i] = new CComboBox(this);

		for (i = 0; i < 2; i++)
			m_pEdit[i] = new CEdit(this);

		file.CreateControl( m_listbox_GM_order, NGAMEMASTERORDER_DIALOG_ORDERLIST, TEXT("listbox_GM_order") );
		file.CreateControl( m_editbox_GM_info, NGAMEMASTERORDER_DIALOG_INFOEDIT, TEXT("editbox_GM_info") );
		file.CreateControl( m_editbox_GM_txt, NGAMEMASTERORDER_DIALOG_TXTEDIT, TEXT("editbox_GM_txt") );
		file.CreateControl( m_button_ok, NGAMEMASTERORDER_DIALOG_OKBUTTON, TEXT("button_ok") );
		file.CreateControl( m_button_cancel, NGAMEMASTERORDER_DIALOG_CANCELBUTTON, TEXT("button_cancel") );
		
		file.CreateControl( m_pStatic[0], NGAMEMASTERORDER_DIALOG_STATIC0, TEXT("statictext_NONAME0") );
		file.CreateControl( m_pStatic[1], NGAMEMASTERORDER_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
		file.CreateControl( m_pStatic[2], NGAMEMASTERORDER_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
		file.CreateControl( m_pStatic[3], NGAMEMASTERORDER_DIALOG_STATIC3, TEXT("statictext_NONAME3") );
		file.CreateControl( m_pStatic[4], NGAMEMASTERORDER_DIALOG_STATIC4, TEXT("statictext_NONAME4") );
		file.CreateControl( m_pStatic[5], NGAMEMASTERORDER_DIALOG_STATIC5, TEXT("statictext_NONAME5") );
		file.CreateControl( m_pStatic[6], NGAMEMASTERORDER_DIALOG_STATIC6, TEXT("statictext_NONAME6") );
		file.CreateControl( m_pStatic[7], NGAMEMASTERORDER_DIALOG_STATIC7, TEXT("statictext_NONAME7") );
		file.CreateControl( m_pStatic[8], NGAMEMASTERORDER_DIALOG_STATIC8, TEXT("statictext_NONAME8") );

		file.CreateControl( m_pComboBox[0], NGAMEMASTERORDER_DIALOG_COMBO0, TEXT("combobox_NONAME0") );
		file.CreateControl( m_pComboBox[1], NGAMEMASTERORDER_DIALOG_COMBO1, TEXT("combobox_NONAME1") );
		file.CreateControl( m_pComboBox[2], NGAMEMASTERORDER_DIALOG_COMBO2, TEXT("combobox_NONAME2") );
		file.CreateControl( m_pComboBox[3], NGAMEMASTERORDER_DIALOG_COMBO3, TEXT("combobox_NONAME3") );
		file.CreateControl( m_pComboBox[4], NGAMEMASTERORDER_DIALOG_COMBO4, TEXT("combobox_NONAME4") );
		file.CreateControl( m_pComboBox[5], NGAMEMASTERORDER_DIALOG_COMBO5, TEXT("combobox_NONAME5") );

		file.CreateControl( m_pEdit[0], NGAMEMASTERORDER_DIALOG_EDIT0, TEXT("editbox_NONAME0") );
		file.CreateControl( m_pEdit[1], NGAMEMASTERORDER_DIALOG_EDIT1, TEXT("editbox_NONAME1") );

		if(m_listbox_GM_order)
		{
			m_listbox_GM_order->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
			m_listbox_GM_order->SetColumn( 0, 162, NULL );
			m_listbox_GM_order->SetBorder( true );
			m_listbox_GM_order->SetBKColorUse( true );
			m_listbox_GM_order->Refresh();
		}

		AddGMList();
		AddMonsters();
		SetItemTypeInComboBox();
		AddSkills();
		AddVillageList();
		AddHuntmaps();

		for (i = 0; i < 9; i++)
			m_pStatic[i]->SetText(m_pStatic[i]->GetText(), DT_LEFT | DT_VCENTER | DT_SINGLELINE );

		if(m_pComboBox[0]->GetItemNum() > 0)
			m_pComboBox[0]->SetCursel( 0 );

		m_pComboBox[1]->SetCursel(0);
		m_pComboBox[3]->SetCursel(0);
		m_pComboBox[4]->SetCursel(0);
		m_pComboBox[5]->SetCursel(0);

		for (i = 0; i < 6; i++)
			m_pComboBox[i]->Enable(FALSE);

		for (i = 0; i < 2; i++)
		{
			m_pEdit[i]->Enable(FALSE);
			m_pEdit[i]->Clear();
		}
		
		Show();
		
		SetFocus( m_listbox_GM_order );
		
	}
}

void CNGameMaster::Action()
{
//	ShowGMList();
}


void CNGameMaster::Hide()
{
	SetTimerOnOff(false);
	CControllerMgr::Show( false );

	return;
}

void CNGameMaster::Show()
{
	SetTimerOnOff(true);
	if(CDialogController::IsShow())
	{	Hide();	return;	}

	if(CDialogController::IsShow() ==false)	
	{
	}

	CControllerMgr::Show( true );

	return;
}


void CALLBACK CNGameMaster::NGameMasterStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNGameMaster* pThis = (CNGameMaster*)pControllerMgr;

	pThis->NGameMasterStatusProc( nEvent, nControlID, pControl);
}

// 소환수 
void CALLBACK CNGameMaster::NGameMasterStatusProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;
	SI16 siTmp = 0;

	switch( nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				Hide();
			}
			break;

		case NGAMEMASTERORDER_DIALOG_EDIT0 :
    	case NGAMEMASTERORDER_DIALOG_EDIT1 :
			{
				switch(nEvent)
				{
				case EVENT_EDITBOX_STRING :
					{
						SendGMOrder();
						Hide();
					}
					break;
				}
			}
			break;

		case NGAMEMASTERORDER_DIALOG_ORDERLIST:
			{
				switch(nEvent)
				{
					case EVENT_LISTBOX_SELECTION:
						{
							TCHAR buf[2048] = TEXT("");
							SI32 i;
							SI32 selectinx = m_listbox_GM_order->GetSelectedIndex();
							if(selectinx < 0 ) break;

							selectinx = selectinx + 1;

							TCHAR* pText = GetTxtFromMgr(1922);
							StringCchPrintf(buf, 2048, pText,pclClient->pclGMManager->pclGMInfo[selectinx]->siNeedGMLevel);
							
							m_editbox_GM_info->SetText(buf);

							if ( pclClient->pclGMManager->GetText(pclClient->pclGMManager->pclGMInfo[selectinx]->szOrder,buf, 2048))
							{
								m_editbox_GM_txt->SetText(buf);
							}

							for (i = 0; i < 6; i++)
								m_pComboBox[i]->Enable(FALSE);

							for (i = 0; i < 2; i++)
							{
								m_pEdit[i]->Enable(FALSE);
								m_pEdit[i]->Clear();
							}

							if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_CHARUNIQUE)
								m_pEdit[0]->Enable(TRUE);

							if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_MONSTER)
								m_pComboBox[0]->Enable(TRUE);

							if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_ITEM)
							{
								m_pComboBox[1]->Enable(TRUE);
								m_pComboBox[2]->Enable(TRUE);
							}

							if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_SKILL)
								m_pComboBox[3]->Enable(TRUE);

							if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_VILLAGE)
								m_pComboBox[4]->Enable(TRUE);

							if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_HUNTMAP)
								m_pComboBox[5]->Enable(TRUE);

							if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_ETC)
								m_pEdit[1]->Enable(TRUE);

						}
						break;
				}
			}
			break;
		case NGAMEMASTERORDER_DIALOG_OKBUTTON : 
			{
				SendGMOrder();
				Hide();
			}
			break;
		case NGAMEMASTERORDER_DIALOG_CANCELBUTTON : 
			{
				Hide();
			}
			break;

		case NGAMEMASTERORDER_DIALOG_COMBO1:
			{
				switch( nEvent )
				{
				case EVENT_COMBOBOX_SELECTION_CHANGED:
					{
						int iTypeIndex = 0;
						if (m_pComboBox[1]->GetSelectedIndex() >= 0)
						{
							iTypeIndex = m_arrItemTypeIndex[m_pComboBox[1]->GetSelectedIndex()];

							m_pComboBox[2]->Clear();

							if ( iTypeIndex != -1 )
							{
								ShowItemListInComboBox(m_pComboBox[2], iTypeIndex);
							}
							m_pComboBox[2]->SetCursel(0);
						}
					}
					break;
				}
			}
			break;

	}
}

void CNGameMaster::AddGMList()
{
	SI32 i;

	TCHAR buffer[1024] = TEXT("");
	stListBoxItemData itemdata;

	m_listbox_GM_order->Clear();

	for(i = 0;i < MAX_GMINFO_NUMBER ;i++)
	{

		if(pclClient->pclGMManager->pclGMInfo[i] == NULL)continue;

		StringCchPrintf(buffer, 1024, TEXT("%s"),pclClient->pclGMManager->pclGMInfo[i]->szOrder);

		if ( !_tcscmp(buffer,TEXT("")) ) continue;

		itemdata.Init();
		itemdata.Set( 0, buffer );

		m_listbox_GM_order->AddItem(&itemdata);
	}
	m_listbox_GM_order->Refresh();
}

void CNGameMaster::AddMonsters()
{
	cltKindInfo* pclki = NULL;
	stComboBoxItemData tmpComboBoxItemData;

	// 콤보 박스를 지운다.
	m_pComboBox[0]->Clear();


	for ( SI32 i = 0; i < MAX_KIND_NUMBER ; ++i )
	{
		pclki = pclClient->pclKindInfoSet->pclKI[ i ];

		if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false ) 
			continue;

		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( (TCHAR *)pclki->GetName() );

		m_pComboBox[0]->AddItem(&tmpComboBoxItemData);
	}
}

void CNGameMaster::SetItemTypeInComboBox()
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 i;
	SI32 cnt = 0;
	for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{	
		cltItemTypeInfo* pinfo = pclclient->pclItemManager->pclItemTypeInfo[i];
		if(pinfo == NULL)continue;

		stComboBoxItemData ComboBoxItemData;
		ComboBoxItemData.Init();
		ComboBoxItemData.Set( (TCHAR*)pinfo->GetName() );
		ComboBoxItemData.siParam = i;

		m_pComboBox[1]->AddItem( &ComboBoxItemData );   // 타입값에 넣는다
		m_arrItemTypeIndex[cnt] = i;
		cnt++;
	}
}

BOOL CNGameMaster::ShowItemListInComboBox(CComboBox* pComboBox, SI32 typeindex)
{
	cltClient *pclclient = (cltClient *)pclClient;
	stComboBoxItemData ComboBoxItemData;

	//cltItemTypeInfo* pinfo = pclclient->pclItemManager->pclItemTypeInfo[typeindex]; 

	SI32 itemtypeunique = typeindex;
	//pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(pinfo->GetName());
	if(itemtypeunique >=0)
	{
		int i;
		// 콤보 박스에 아이템의 종류를 설정한다. 
		for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
		{	
			cltItemInfo* pinfo = pclClient->pclItemManager->pclItemInfo[i];
			if(pinfo == NULL)continue;
			if(pinfo->siType != itemtypeunique)continue;

			// 주어진 타입과 같은 아이템만 열거한다. 
			// 사용자에게 보여지는 아이템인 경우에만. 
			//if(pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST))
			{
				ComboBoxItemData.Init();
				TCHAR ptext[256] ={0,};
				StringCchPrintf( ptext, 256, TEXT("[%d]%s"), pinfo->clItem.siUnique, (TCHAR*)pinfo->GetName() );
				ComboBoxItemData.Set( ptext );		
				ComboBoxItemData.siParam = pinfo->clItem.siUnique;
				pComboBox->AddItem( &ComboBoxItemData );
			}
		}
	}

	return TRUE;
}

void CNGameMaster::AddSkills()
{
	SI32 i;
	stComboBoxItemData ComboBoxItemData;
	TCHAR buf[MAX_SKILL_NAME_LENGTH] = TEXT("");

	for(i = 1; i < MAX_SKILL_NUMBER;i++)
	{
		if (pclClient->pclSkillManager->pclSkillInfo[i] == NULL)
			continue;
	
		if (pclClient->pclSkillManager->pclSkillInfo[i]->szName == NULL)
			continue;

		StringCchPrintf(buf, MAX_SKILL_NAME_LENGTH, pclClient->pclSkillManager->pclSkillInfo[i]->szName);

		ComboBoxItemData.Init();
		ComboBoxItemData.Set( 0, buf );
		m_pComboBox[3]->AddItem( &ComboBoxItemData );
	}
}

void CNGameMaster::AddVillageList()
{
	stComboBoxItemData tmpComboBoxItemData;

	// 기존의 리스트를 삭제한다. 
	m_pComboBox[4]->Clear();

	SI32 i;

	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		if (pclClient->pclVillageManager->GetName(i) == NULL)
			continue;

		if (!_tcscmp( pclClient->pclVillageManager->GetName(i), TEXT("")))
			continue;

		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( 0, pclClient->pclVillageManager->GetName(i) );
		m_pComboBox[4]->AddItem( &tmpComboBoxItemData );

	}
}

void CNGameMaster::AddHuntmaps()
{
	SI32 i;
	stComboBoxItemData ComboBoxItemData;

	for(i = 0;i < MAX_MAP_NUMBER;i++)
	{
		if (pclClient->pclMapManager->GetMapName(i) == NULL)
			continue;

		if (!_tcscmp( pclClient->pclMapManager->GetMapName(i), TEXT("")))
			continue;

		if (!_tcscmp( pclClient->pclMapManager->GetMapName(i), TEXT("NoName")))
			continue;

		if (pclClient->pclMapManager->IsInstanceMap(i))
			continue;

		if (pclClient->pclMapManager->IsHuntMap(i))
			continue;

		if (pclClient->pclVillageManager->GetMapIndexFromName(pclClient->pclMapManager->GetMapName(i)) > 0)
			continue;

		ComboBoxItemData.Init();
		ComboBoxItemData.Set( 0, pclClient->pclMapManager->GetMapName(i) );
		m_pComboBox[5]->AddItem( &ComboBoxItemData );
	}
}

void CNGameMaster::SendGMOrder()
{
	TCHAR *pBuf;
	TCHAR order[2048]=TEXT("");
	SI32 selectinx = m_listbox_GM_order->GetSelectedIndex();

	if ( selectinx < 0 )
		return;

	StringCchPrintf(order, 2048, TEXT("/%s"), m_listbox_GM_order->GetText(selectinx, 0));

	selectinx = selectinx + 1;

	if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_CHARUNIQUE)
	{
		pBuf = m_pEdit[0]->GetText();
		StringCchPrintf(order, 2048, TEXT("%s %s"), order, pBuf);
	}

	if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_MONSTER)
	{
		pBuf = m_pComboBox[0]->GetText(m_pComboBox[0]->GetSelectedIndex());
		ConvertSpaceToUnderLine( pBuf ); 
		StringCchPrintf(order, 2048, TEXT("%s %s"), order, pBuf);
	}

	if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_ITEM)
	{
		//pBuf = m_pComboBox[2]->GetText(m_pComboBox[2]->GetSelectedIndex());
		//ConvertSpaceToUnderLine( pBuf ); 
		SI32 itemunique = m_pComboBox[2]->GetParam( m_pComboBox[2]->GetSelectedIndex() );
		StringCchPrintf( order, 2048, TEXT("%s %d"), order, itemunique);
	}

	if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_SKILL)
	{
		pBuf = m_pComboBox[3]->GetText(m_pComboBox[3]->GetSelectedIndex());
		ConvertSpaceToUnderLine( pBuf ); 
		StringCchPrintf(order, 2048, TEXT("%s %s"), order, pBuf);
	}

	if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_VILLAGE)
	{
		pBuf = m_pComboBox[4]->GetText(m_pComboBox[4]->GetSelectedIndex());
		StringCchPrintf(order, 2048, TEXT("%s %s"), order, pBuf);
	}

	if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_HUNTMAP)
	{
		pBuf = m_pComboBox[5]->GetText(m_pComboBox[5]->GetSelectedIndex());
		ConvertSpaceToUnderLine( pBuf ); 
		StringCchPrintf(order, 2048, TEXT("%s %s"), order, pBuf);
	}

	if (pclClient->pclGMManager->pclGMInfo[selectinx]->mGMatb & GM_ATB_ETC)
	{
		pBuf = m_pEdit[1]->GetText();
		StringCchPrintf(order, 2048, TEXT("%s %s"), order, pBuf);
	}


	((cltClient*)pclClient)->SendChatMessage(CHATMODE_NORMAL, order);

}

bool CGMList::HandleKeyboard( UINT uMsg, WPARAM &wParam, LPARAM &lParam )
{
	//CList에도 이부분이 있지만 이것은 CList보다 먼저 실행되기 가장 기본적인 오류 체크이기 때문에 이곳에 존재한다.
	if ( ( ! IsAvailable() ) || ( ! IsFocus() ) )
	{
		return false;
	}

	switch( uMsg )
	{
	case WM_CHAR:
		{
			// 영문만 되도록
			if ( ((wParam >= 'a') && (wParam <= 'z')) || ((wParam >= 'A') && (wParam <= 'Z')) )
			{
				SI16	siListCount	= GetListNum();
				SI16	siLength	= 0;
				TCHAR	tsChar[10]	= {'\0', };

				static TCHAR	tsSearch[64]	= {0,};
				static DWORD	dwSearchTime	= ::GetTickCount();


				// 우선 문자를 소문자로 변환한다
				StringCchPrintf(tsChar, 10, TEXT("%c"), wParam);
				CharLower( tsChar ); 

				// 현재 문자가 들어온 시간이 1초가 넘어가면 기존 문자열은 지운다
				if ( (dwSearchTime + 1000) < GetTickCount() )
				{
					StringCchCopy( tsSearch, 64, TEXT("") );
				}
				dwSearchTime = ::GetTickCount();
				
				// 찾으려는 문자를 기존문자열에 더하고
				StringCchCat( tsSearch, 64, tsChar );

				// 현재 찾으려는 단어의 길이를 구해 놓는다
				siLength = _tcslen(tsSearch);
				

				//많이 무식하지만 리스트의 처음부터 끝까지 찾다가 맞으면 그곳에서 멈춘다.
				for ( SI32 siIndex=0; siIndex<siListCount; siIndex++)
				{
					TCHAR*	ptsListText	= GetText( siIndex, 0 );
					if ( NULL == ptsListText )
					{
						continue;
					}

					// 비교하려는 대상문자의 길이가 더 짧을 수 있기때문에 더 짧은쪽 길이로 해준다
					SI16 siCompareLength = _tcslen(ptsListText);
					if ( siCompareLength > siLength )
					{
						siCompareLength = siLength;
					}
					
					if ( _tcsncmp( ptsListText, tsSearch, siLength ) == 0 )
					{
						SetSelectIndex( siIndex );
						GetParent()->SendEvent( EVENT_LISTBOX_SELECTION, true, this );
						return true;
					}
				}
			}
		}
		break;
	}

	return CList::HandleKeyboard( uMsg, wParam, lParam );

}
//#endif