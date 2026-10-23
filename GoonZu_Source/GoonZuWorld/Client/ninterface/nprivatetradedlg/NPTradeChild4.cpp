#include "NPTradeChild4.h"
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



#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/interface/Button.h"
#include "../../InterfaceMgr/interface/Static.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"




#include "Client.h"

extern cltCommonLogic* pclClient;

//CNPrivateTradeDlg g_NPrivateTradeDlg;

stListBoxItemData HouseListBoxItemData;

CNTradeChild4::CNTradeChild4()
{	
	m_pNPTrade4Static1 = NULL;
	m_pNPTrade4Static2 = NULL;
	m_pNPTrade4List = NULL;
	m_pNPTrade4Button = NULL;
	m_pParent= NULL;
	


}

CNTradeChild4::~CNTradeChild4()
{	
	SAFE_DELETE(m_pNPTrade4Static1);
	SAFE_DELETE(m_pNPTrade4Static2);
	SAFE_DELETE(m_pNPTrade4List);
	SAFE_DELETE(m_pNPTrade4Button);
	//SAFE_DELETE( m_pParent);
}

void CNTradeChild4::Create(CControllerMgr *pParent)
{
	if(!IsCreate())
	{
		m_pParent =(CNPrivateTradeDlg*)pParent;
		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NPrivateTrade/DLG_PTradeChild4.ddf"));
		file.CreateChildDialog( this, NPTRADECHILD4_DLG, TEXT("dialog_PTradeChild4"),StaticNTradeChild4Proc, pParent);
		

		
		m_pNPTrade4Static1 = new CStatic(this);
		m_pNPTrade4Static2 = new CStatic(this);
		m_pNPTrade4List =   new CList(this);
		m_pNPTrade4Button = new CButton(this);
		

		file.CreateControl(m_pNPTrade4Static1, NPTRADECHILD4_STATIC1 , TEXT("statictext_PrivateTrade4_1"));	
		file.CreateControl(m_pNPTrade4Static2, NPTRADECHILD4_STATIC2 , TEXT("statictext_PrivateTrade4_2"));	
		file.CreateControl(m_pNPTrade4List, NPTRADECHILD4_LISTBOX , TEXT("listbox_PrivateTrade4"));	
		file.CreateControl(m_pNPTrade4Button ,NPTRADECHILD4_BUTTON , TEXT("button_PrivateTrade4"));	
		TCHAR* pText = GetTxtFromMgr(3405);
		m_pNPTrade4Static1->SetText(TEXT(" "),DT_WORDBREAK);
		m_pNPTrade4List->SetColumn(0,213,pText);
		
		m_pNPTrade4List->SetBKColorUse(true);
		m_pNPTrade4List->SetBorder(true);
		LoadHouseUnitList();

		m_pNPTrade4List->Refresh();
		//m_pNPTradeListView4->SetBKColorUse(true);
		Hide();
	}
	
}

	
void  CNTradeChild4::Show()
{
	CDialogController::Show( true );
}

void  CNTradeChild4::Hide()
{
	CDialogController::Show( false );

}

void CALLBACK CNTradeChild4::StaticNTradeChild4Proc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNTradeChild4 *pThis = (CNTradeChild4*) pControllerMgr;
	pThis->NTradeChild4Proc( nEvent, nControlID, pControl );

}
	
void CALLBACK CNTradeChild4::NTradeChild4Proc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
		
	switch (nControlID  )
	{	
	

	case NPTRADECHILD4_LISTBOX:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{									
					SI16 index = m_pNPTrade4List->GetSelectedIndex();
					if ( index < 0 ) break ;
					



				}
				break;
			}
		}break;

	case NPTRADECHILD4_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_pParent->PushHouseUnit();
					
				}
				break;
			}
		}
		break;



	}

	return;

}



void  CNTradeChild4::LoadHouseUnitList()
{
	memset( m_pParent->m_siHouseVillageUniqueList, 0, sizeof( SI32 ) * MAX_REAL_ESTATE_NUMBER_PER_PERSON );
	memset( m_pParent->m_siHouseUnitSlotList, 0, sizeof( SI32 ) * MAX_REAL_ESTATE_NUMBER_PER_PERSON );

	//SendMessage( GetDlgItem( m_hLandDlg, IDC_LIST_HOUSEUNIT ), LB_RESETCONTENT, 0, 0 );
	m_pNPTrade4List->Clear();

	TCHAR buffer[256] = TEXT("");
	
	for ( SI16 i = 0; i < MAX_REAL_ESTATE_NUMBER_PER_PERSON; ++i )
	{
		m_pParent->m_siHouseVillageUniqueList[ i ] = pclClient->pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siVillageUnique;
		m_pParent->m_siHouseUnitSlotList[ i ] = pclClient->pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siIndex;
		
		if ( m_pParent->m_siHouseVillageUniqueList[ i ] > 0 )
		{
			TCHAR *VillageName = pclClient->pclVillageManager->GetName( m_pParent->m_siHouseVillageUniqueList[ i ] );
			if ( VillageName != NULL )
			{
				TCHAR* pText = GetTxtFromMgr(2338);
				StringCchPrintf( buffer, 256, pText, VillageName, m_pParent->m_siHouseUnitSlotList[ i ] + 1 );
				if (!_tcscmp(buffer,TEXT("")) ) continue;
				HouseListBoxItemData.Init();	
				HouseListBoxItemData.Set(0,buffer);	
				HouseListBoxItemData.siParam[0] =m_pParent->m_siHouseVillageUniqueList[ i ];
				SI32 index = m_pNPTrade4List->AddItem( &HouseListBoxItemData );	
			
				//SendMessage( GetDlgItem( m_hLandDlg, IDC_LIST_HOUSEUNIT ), LB_ADDSTRING, 0, (LPARAM)buffer );
			}
		}
	}
	//m_pNPTrade4List->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	m_pNPTrade4List->Refresh();
	
	return;
}



bool  CNTradeChild4::IsHouseUnitListChanged()
{
	for ( SI16 i = 0; i < MAX_REAL_ESTATE_NUMBER_PER_PERSON; ++i )
	{
		if ( m_pParent->m_siHouseVillageUniqueList[i] != pclClient->pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siVillageUnique || 
			m_pParent->m_siHouseUnitSlotList[i] != pclClient->pclCM->CR[1]->pclCI->clRealEstate.clRealEstate[i].siIndex )
		{
			return true;
		}
	}

	return false;
}




