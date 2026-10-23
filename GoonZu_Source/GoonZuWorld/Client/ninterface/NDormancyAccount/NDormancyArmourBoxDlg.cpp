#include "client.h"


#include "NDormancyArmourBoxDlg.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "Char\CharManager\CharManager.h"

#include "../../../CommonLogic/MsgType-Item.h"
#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/CommonLogic.h"

extern cltCommonLogic* pclClient;

CNDormancyArmourBoxDlg::CNDormancyArmourBoxDlg()
{
	m_siSelctedOrder = 0;	// 선택된 아이템

	m_pOkButton = NULL;
	m_pTextStatic = NULL;
	m_pTextEdit = NULL;

	for(SI32 i = 0 ; i < MAX_ITEM_COUNT ; i++)
	{
		m_pItemListView[i] = NULL;
	}
}
CNDormancyArmourBoxDlg::~CNDormancyArmourBoxDlg()
{
	SAFE_DELETE(m_pOkButton);
	SAFE_DELETE(m_pTextStatic);
	SAFE_DELETE(m_pTextEdit);
	for(SI32 i = 0 ; i < MAX_ITEM_COUNT ; i++)
	{
		SAFE_DELETE(m_pItemListView[i]);
	}
}

void CNDormancyArmourBoxDlg::Destroy()
{
	DeleteDialog();
}

void CNDormancyArmourBoxDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( !IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NDormancyAccount/DLG_NDormancyArmourBoxDlg.ddf");

		// 선택 확인 버튼
		m_pOkButton = new CButton( this );
		if( NULL == m_pOkButton)
			return;
		// 이미지 버튼
		for(SI32 i = 0 ; i < MAX_ITEM_COUNT ; i++)
		{
			m_pItemListView[i] = new CListView( this, sizeof(SI32) );
			if( NULL == m_pItemListView[i])
				return;
		}
		// 설명
		m_pTextStatic = new CStatic( this );
		if( NULL == m_pTextStatic)
			return;
		m_pTextEdit = new CEdit(this);
		if( NULL == m_pTextEdit)
			return;
		file.CreatePopupDialog( this, NDORMANCYARMOURBOX_DLG, "dialog_DormancyArmourBox", StatiCNDormancyArmourBoxProc);

		file.CreateControl( m_pItemListView[0], NDORMANCYARMOURBOX_LISTVIEW_HELMET,   "listview_Helmet"); 
		file.CreateControl( m_pItemListView[1], NDORMANCYARMOURBOX_LISTVIEW_ARMOUR,   "listview_Armour"); 
		file.CreateControl( m_pItemListView[2], NDORMANCYARMOURBOX_LISTVIEW_BELT,   "listview_Belt"); 
		file.CreateControl( m_pItemListView[3], NDORMANCYARMOURBOX_LISTVIEW_SHOES,   "listview_Shoes"); 
		file.CreateControl( m_pOkButton, NDORMANCYARMOURBOX_BUTTON_OK,   "button_OK"); 
		file.CreateControl( m_pTextStatic, NDORMANCYARMOURBOX_STATIC_TEXT,   "statictext_Text"); 
		file.CreateControl( m_pTextEdit, NDORMANCYARMOURBOX_EDIT_TEXT,   "editbox_Text"); 

		m_pOkButton->Enable(FALSE);

		// 설명
		m_pTextStatic->SetText(GetTxtFromMgr(40060));
		// 버튼 문자열
		m_pOkButton->SetText(GetTxtFromMgr(40173));

		//	아이템 이미지 값 얻기 
		SetItemImage();
	}
	return;
}

void CALLBACK CNDormancyArmourBoxDlg::StatiCNDormancyArmourBoxProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNDormancyArmourBoxDlg *pThis = (CNDormancyArmourBoxDlg*)pControllerMgr;
	if(NULL != pThis)
	{
		pThis->NDormancyArmourBoxProc( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNDormancyArmourBoxDlg::NDormancyArmourBoxProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient *pclclient = (cltClient *)pclClient;
	CNDormancyArmourBoxDlg* pclDlg = (CNDormancyArmourBoxDlg*)pclclient->m_pDialog[NDORMANCYARMOURBOX_DLG];
	if( NULL == pclDlg)
		return;

	switch( nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;	
	case NDORMANCYARMOURBOX_LISTVIEW_HELMET:
	case NDORMANCYARMOURBOX_LISTVIEW_ARMOUR:	
	case NDORMANCYARMOURBOX_LISTVIEW_BELT:	
	case NDORMANCYARMOURBOX_LISTVIEW_SHOES:	
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
			case EVENT_LISTVIEW_RBUTTONDOWN:
				{
					pclDlg->SelectItem(nControlID,pControl);
					break;
				}
			}
		}
		break;
	case NDORMANCYARMOURBOX_BUTTON_OK:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					pclDlg->CompleteToSelectItem();

				}
				break;
			}
		}
		break;	
	}
	return;
}

//	아이템 이미지 값 얻기
VOID CNDormancyArmourBoxDlg::SetItemImage()
{
	cltClient *pclclient = (cltClient *)pclClient;

	CDormancySystem *pclDormancySystem = pclclient->pclDormancySystem;
	if(NULL == pclDormancySystem)
	{
		return;
	}

	CDormancyEquipItemRewardClient *pclDormancyClient = (CDormancyEquipItemRewardClient *)pclDormancySystem->GetDormancyEquipItemReward();
	if(NULL == pclDormancyClient)
	{
		return;
	}

	SI32 siItemUnique = -1;
	for(SI32 i = 0 ; i < MAX_ITEM_COUNT ; i++)
	{
		siItemUnique = pclDormancyClient->GetItemUniqueInBox(UNIQUE_BOX,i);
		if( siItemUnique > -1)
		{
			if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siItemUnique, &m_siImageType[i], &m_siImageIndex[i] ))
			{
				TCHAR *szItemName = (char*)pclclient->pclItemManager->GetName( siItemUnique );

				if ( NULL != szItemName )
				{
					TCHAR szToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";

					((cltItemManagerClient*)pclclient->pclItemManager)->GetExplain( siItemUnique, szToolTipMsg );
					
					stListViewItemData ListViewItemData;
					ListViewItemData.Set( m_siImageType[i], m_siImageIndex[i], 0 , 1, szItemName, szToolTipMsg );
					m_pItemListView[i]->InsertItem(&ListViewItemData);
					m_pItemListView[i]->SetData((BYTE*)&i , sizeof(SI32));
				}
			}
		}
	}
	return;
}

// 선택 완료
VOID CNDormancyArmourBoxDlg::CompleteToSelectItem()
{
	cltClient *pclclient = (cltClient *)pclClient;

	CDormancySystem *pclDormancySystem = pclclient->pclDormancySystem;
	if(NULL == pclDormancySystem)
	{
		return;
	}

	CDormancyEquipItemRewardClient *pclDormancyClient = (CDormancyEquipItemRewardClient *)pclDormancySystem->GetDormancyEquipItemReward();
	if(NULL == pclDormancyClient)
	{
		return;
	}

	pclDormancyClient->SelectItemInBox(m_siSelctedOrder, m_siBoxUnique, m_siBoxPos);
}

VOID CNDormancyArmourBoxDlg::SelectItem(SI32 siControllID,CControllerObj* pControl)
{
	UI32	uiSize = sizeof(SI32);
	
	if( NULL == pControl)
		return;

	pControl->GetData((BYTE*)&m_siSelctedOrder, &uiSize);

	m_pOkButton->Enable(TRUE);
}

// 보급 박스 유니크 저장
VOID CNDormancyArmourBoxDlg::SetBoxInfo(SI32	siBoxUnique, SI32 siBoxPos)
{
	m_siBoxUnique = siBoxUnique;
	m_siBoxPos = siBoxPos;
}
