#include "NPTradeChild2.h"
#include "NPrivateTradeDlg.h"

#include "..\..\..\Common\SMLib\LibList\LibList.h"
#include "..\..\..\Common\PrivateTradeOrderMgr\PrivateTradeOrderObj.h"

//#include "..\..\Interface\PrivateChatDlg\PrivateChatDlgMgr.h"

#include "..\..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Interface\NotifyMsgDlg\NotifyMsgDlg.h"


#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/House/House.h"

#include "../../../lib/WebHTML/WebHTML.h"



//#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/interface/ComboBox.h"
#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/interface/Edit.h"
#include "../../InterfaceMgr/interface/Button.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"




#include "Client.h"

extern cltCommonLogic* pclClient;

//CNPrivateTradeDlg g_NPrivateTradeDlg;

CNTradeChild2::CNTradeChild2()
{	
	//m_pNPTradeListView2  = NULL;
	m_pNPTrade2Button1 = NULL;
	m_pNPTrade2Button2 = NULL;
	m_pNPTrade2Combo = NULL;
	m_pNPTrade2Edit = NULL;
	m_pNPTrade2Static1 = NULL;
	m_pNPTrade2Static2 = NULL;
	m_pParent = NULL;

	m_siSelectedVillageIndex = 0;

}

CNTradeChild2::~CNTradeChild2()
{	
	SAFE_DELETE(m_pNPTrade2Button1);
	SAFE_DELETE(m_pNPTrade2Button2);
	SAFE_DELETE(m_pNPTrade2Combo);
	SAFE_DELETE(m_pNPTrade2Edit);
	SAFE_DELETE(m_pNPTrade2Static1);
	SAFE_DELETE(m_pNPTrade2Static2);
	//SAFE_DELETE( m_pParent);
}

void CNTradeChild2::Create(CControllerMgr *pParent)
{
	if(!IsCreate())
	{
	
		m_pParent =(CNPrivateTradeDlg*)pParent;
		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NPrivateTrade/DLG_PTradeChild2.ddf"));
		file.CreateChildDialog( this, NPTRADECHILD2_DLG, TEXT("dialog_PTradeChild2"),StaticNTradeChild2Proc, pParent);
		
		m_pNPTrade2Button1 = new CButton(this);
		m_pNPTrade2Button2 = new CButton(this);
		m_pNPTrade2Combo =  new CComboBox(this);
		m_pNPTrade2Edit = new CEdit(this);
		m_pNPTrade2Static1 = new CStatic(this);
		m_pNPTrade2Static2 = new CStatic(this);		

		file.CreateControl(m_pNPTrade2Button1, NPTRADECHILD2_BUTTON1, TEXT("button_PTradeChild2_1"));
		file.CreateControl(m_pNPTrade2Button2, NPTRADECHILD2_BUTTON2 , TEXT("button_PTradeChild2_2"));		
		file.CreateControl(m_pNPTrade2Edit, NPTRADECHILD2_EDIT , TEXT("editbox_PTradeChild2"));
		file.CreateControl(m_pNPTrade2Static1, NPTRADECHILD2_STATIC1 , TEXT("statictext_PTradeChild2_1"));
		file.CreateControl(m_pNPTrade2Static2, NPTRADECHILD2_STATIC2, TEXT("statictext_PTradeChild2_2"));
		file.CreateControl(m_pNPTrade2Combo, NPTRADECHILD2_COMBO , TEXT("combobox_PTradeChild2"));
		m_pNPTrade2Edit->SetText(TEXT("0"));
	
		Hide();
	}
}

	
void  CNTradeChild2::Show()
{
	CDialogController::Show( true );
}

void  CNTradeChild2::Hide()
{
	CDialogController::Show( false );

}

void CALLBACK CNTradeChild2::StaticNTradeChild2Proc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNTradeChild2 *pThis = (CNTradeChild2*) pControllerMgr;
	pThis->NTradeChild2Proc( nEvent, nControlID, pControl );

}
	
void CALLBACK CNTradeChild2::NTradeChild2Proc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	//cltClient *pclclient = (cltClient*)pclClient;  
	//m_uiLastAcceptFrame =(((CNPrivateTradeDlg*)pclclient->m_pDialog[ NPRIVATETRADE_DLG]))->m_uiLastAcceptFrame;
	 
		
	switch (nControlID  )
	{	
	case NPTRADECHILD2_COMBO:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
                    m_siSelectedVillageIndex = m_pNPTrade2Combo->GetSelectedIndex();
					//m_pParent->m_siStockVillageUnique = pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[m_siSelectedIndex].siVillageUnique;

				}
				break;
			}
		}break;

	case NPTRADECHILD2_BUTTON1:
		{				
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED: //주식 help 버튼 
				{						
					TCHAR* pText = GetTxtFromMgr(2316);
					(m_pParent->m_pNPTradeEdit)->SetText(pText);	
				
				}
				break;
			}

		}
		break;
		
	case NPTRADECHILD2_BUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:   //주식 입력 버튼 
				{					
					/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
					m_SILastAcceptTime = pclClient->CurrentClock	;
					/*m_pParent->m_uiLastAcceptFrame =  m_uiLastAcceptFrame;*/
					m_pParent->m_siLastAcceptTime = m_SILastAcceptTime	;

					m_pParent->m_siStockNum = _tstoi(m_pNPTrade2Edit->GetText());

					//	[종호]	m_siStockNum이 0 일 경우 Release Version에서 클라이언트가 죽는다.
					//			m_siStockNum이 0 이면 PushStock() 하지않는다.
					if(m_pParent->m_siStockNum <= 0)
						break;


					//m_pNPTrade2Edit->Get

					(m_pParent->m_pNPTradeButton9) ->Enable(true);
					m_pParent->PushStock();						
						

				}
				break;
			}

		}
		break;

	}
	

	return;

}



void CNTradeChild2::LoadStockList()
{
	memset( m_pParent->m_siStockKindList, 0, sizeof(SI32) * MAX_STOCK_KIND_IN_ACOUNT );
	memset(m_pParent->m_siStockNumList, 0, sizeof(SI32) * MAX_STOCK_KIND_IN_ACOUNT );

	//SendMessage( GetDlgItem( m_hStockDlg, IDC_COMBO_MY_STOCK_SELECT ), CB_RESETCONTENT, 0, 0 );

	stComboBoxItemData tmpComboBoxItemData;	
	TCHAR buffer[256] = TEXT("");
	for ( SI16 i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
	{
		// [영훈] 초보자 마을 : 초보자 마을의 주식은 나타내지 않는다
		/*if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
		{
			if ( Const_Beginner_Village == pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique )
			{
				continue;
			}
		}*/
		// [영훈] 초보자 마을 : 초보자 마을의 주식은 나타내지 않는다
		// [ 성웅 ] 개인거래시 앙카란 주식 보유시 리스트가 깜빡거리고 선택이 되지 않는 버그 발생으로 체크 방식 변경
		if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) ){
			m_pParent->m_siStockKindList[i] = pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique;
			m_pParent->m_siStockNumList[i] = pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siAmount;

			if ( m_pParent->m_siStockKindList[i] > 0 && m_pParent->m_siStockNumList[i] > 0 )
			{
				TCHAR *VillageName = pclClient->pclVillageManager->GetName( m_pParent->m_siStockKindList[i] );
				if ( Const_Beginner_Village != pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique 
					&& VillageName != NULL )
				{
					m_pNPTrade2Combo->GetItemData(NPTRADECHILD2_COMBO);
					tmpComboBoxItemData.Init();
					tmpComboBoxItemData.Set( VillageName );
					m_pNPTrade2Combo->AddItem( &tmpComboBoxItemData );

					//SendMessage( GetDlgItem( m_hStockDlg, IDC_COMBO_MY_STOCK_SELECT  ), CB_ADDSTRING, 0, (LPARAM)VillageName );
				}
			}

		}
		else{
			m_pParent->m_siStockKindList[i] = pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique;
			m_pParent->m_siStockNumList[i] = pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siAmount;

			if ( m_pParent->m_siStockKindList[i] > 0 && m_pParent->m_siStockNumList[i] > 0 )
			{
				TCHAR *VillageName = pclClient->pclVillageManager->GetName( m_pParent->m_siStockKindList[i] );
				if ( VillageName != NULL )
				{
					m_pNPTrade2Combo->GetItemData(NPTRADECHILD2_COMBO);
					tmpComboBoxItemData.Init();
					tmpComboBoxItemData.Set( VillageName );
					m_pNPTrade2Combo->AddItem( &tmpComboBoxItemData );

					//SendMessage( GetDlgItem( m_hStockDlg, IDC_COMBO_MY_STOCK_SELECT  ), CB_ADDSTRING, 0, (LPARAM)VillageName );
				}
			}

		}
	}
	
	return;
}


bool CNTradeChild2::IsStockListChanged()
{
	for ( SI16 i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
	{
		if ( m_pParent->m_siStockKindList[i] != pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique || 
			m_pParent->m_siStockNumList[i] != pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siAmount )
		{
			m_pNPTrade2Combo->Clear();
			return true;
		}
		

	}

	return false;
}



