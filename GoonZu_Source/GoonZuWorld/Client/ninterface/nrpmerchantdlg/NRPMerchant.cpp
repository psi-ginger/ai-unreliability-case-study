

#include "../../../Client/Client.h"
#include "../../../CommonLogic/Msg/MsgType-NPC.h"
#include "../../../Common/SystemNPC/SystemNPC.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "NRPMerchant.h"
#include "NRPBase.h"


extern cltCommonLogic* pclClient;

CNRPMerchant::CNRPMerchant()
{
	m_SelectedListIndex = -1;
	m_siImageType = -1;
	m_siImageIndex = -1;
}

CNRPMerchant::~CNRPMerchant()
{

}

void CNRPMerchant::Initialize()
{

}

void CNRPMerchant::Destroy()
{

}

void CNRPMerchant::Create( CControllerMgr *pParent )
{
 	cltClient *pclclient = (cltClient *)pclClient;
	
	if ( ! IsCreate() )
	{
		CInterfaceFile file;
 
		file.LoadFile(TEXT("NInterface/Data/NRPMerchant/DLG_RPMerchant.ddf"));
		file.CreateChildDialog( this, NRPMERCHANT_DLG, TEXT("dialog_RPMerchant"), StaticNRPMerchantDlgProc, pParent);

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NRPMERCHANT_STATIC_ITEMNAME,		this), NRPMERCHANT_STATIC_ITEMNAME,  TEXT("statictext_noname2"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NRPMERCHANT_STATIC_ITEMTYPE,		this), NRPMERCHANT_STATIC_ITEMTYPE,  TEXT("statictext_noname3"));		
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC ,NRPMERCHANT_IMAGESTATIC_ITEM , this), NRPMERCHANT_IMAGESTATIC_ITEM, TEXT("imagestatic_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NRPMERCHANT_BUTTON_BUY,				this), NRPMERCHANT_BUTTON_BUY,		 TEXT("button_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST ,NRPMERCHANT_LIST,						this), NRPMERCHANT_LIST,			 TEXT("listbox_NONAME1"));

 		TCHAR* pText = GetTxtFromMgr(6243);
		m_InterfaceFactory.GetList(NRPMERCHANT_LIST)->SetColumn(0, 65, pText);

		pText = GetTxtFromMgr(1928);
		m_InterfaceFactory.GetList(NRPMERCHANT_LIST)->SetColumn(1, 146,  pText);

		m_InterfaceFactory.GetList(NRPMERCHANT_LIST)->SetBorder(TRUE);
		m_InterfaceFactory.GetList(NRPMERCHANT_LIST)->SetBKColorUse(TRUE);
		m_InterfaceFactory.GetList(NRPMERCHANT_LIST)->Refresh();
			
		m_SelectedListIndex = -1;

		SetRecommandProductList();
	}
}


void CALLBACK CNRPMerchant::StaticNRPMerchantDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNRPMerchant *pThis = (CNRPMerchant*) pControllerMgr;
	pThis->NRPMerchantDlgProc( nEvent, nControlID, pControl );
}


void CALLBACK CNRPMerchant::NRPMerchantDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
 	switch(nControlID)
	{
	case NRPMERCHANT_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					cltClient* pclclient = (cltClient*)pclClient;

					SI32 TempSelectIndex = m_InterfaceFactory.GetList(NRPMERCHANT_LIST)->GetSelectedIndex();

					if(TempSelectIndex < 0)
						break;

					m_SelectedListIndex = TempSelectIndex;
					
					if( m_SelectedListIndex < 0  || m_SelectedListIndex >= MAX_RECOMMAND_PRODUCT ) break;

					SetItemImageData(pclClient->pclRPMerchantNPCMgr->m_RecommandProduct[m_SelectedListIndex].ItemUnique);
					
					if(pclClient->pclRPMerchantNPCMgr->m_RecommandProduct[m_SelectedListIndex].NeedPoint 
						< ((CNRPBase*)pclclient->m_pDialog[NRPBASE_DLG])->GetRecommandPoint() 
						&& pclClient->pclRPMerchantNPCMgr->m_RecommandProduct[m_SelectedListIndex].NeedPoint  != 0 )
					{
						m_InterfaceFactory.GetButton(NRPMERCHANT_BUTTON_BUY)->Enable(true);

					}
					else
					{
						m_InterfaceFactory.GetButton(NRPMERCHANT_BUTTON_BUY)->Enable(false);
					}

				}
				break;
			}
		}
		break;
	case NRPMERCHANT_BUTTON_BUY:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltClient* pclclient = (cltClient*)pclClient;

					if( m_SelectedListIndex < 0  || m_SelectedListIndex >= MAX_RECOMMAND_PRODUCT ) break;

					stRecommandProduct* sRecommandProduct = &pclClient->pclRPMerchantNPCMgr->m_RecommandProduct[m_SelectedListIndex];

					SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( sRecommandProduct->ItemUnique );
					if(ref == 0)break;

					cltItem clItem	= pclClient->pclItemManager->pclItemInfo[ref]->clItem;
					
					clItem.SetItemNum( sRecommandProduct->ItemNum );
					clItem.uiDateUseDay = sRecommandProduct->ItemUseDate;

					TCHAR buffer[512] = {'\0',};
					TCHAR * pItemName = (TCHAR*)pclClient->pclItemManager->GetName(sRecommandProduct->ItemUnique);
					TCHAR * pTitle = GetTxtFromMgr(5951);
					TCHAR * pstrSrc	= GetTxtFromMgr(6332);
					
					StringCchPrintf(buffer,512, pstrSrc ,sRecommandProduct->NeedPoint , pItemName);

					stMsgBox MsgBox;
					MsgBox.Set( this->GetParentDialog(),  pTitle, buffer, MSGBOX_TYPE_YESNO, 0 );	
					BYTE TempBuffer[256] = {'\0',};
					SI16 Size = sizeof(m_SelectedListIndex ) + sizeof(clItem );
					memcpy( TempBuffer, &m_SelectedListIndex, sizeof(m_SelectedListIndex ) );
					memcpy( &TempBuffer[ sizeof( m_SelectedListIndex ) ], &clItem, sizeof( clItem ) );					
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );	


					/*
					if( m_SelectedListIndex < 0  || m_SelectedListIndex >= MAX_RECOMMAND_PRODUCT ) break;

					stRecommandProduct* sRecommandProduct = &pclClient->pclRPMerchantNPCMgr->m_RecommandProduct[m_SelectedListIndex];

					SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( sRecommandProduct->ItemUnique );

					cltItem clItem	= pclClient->pclItemManager->pclItemInfo[ref]->clItem;

					clItem.SetItemNum( sRecommandProduct->ItemNum );
					clItem.uiDateUseDay = sRecommandProduct->ItemUseDate;

					cltGameMsgRequest_BuyRecommandProduct	clRPMerchantBuyItem( m_SelectedListIndex , &clItem  );
					cltMsg clMsg( GAMEMSG_REQUEST_BUYRECOMMANDPRODUCT,sizeof(cltGameMsgRequest_BuyRecommandProduct), (BYTE*)&clRPMerchantBuyItem  );
					pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );*/

				}
				break;
			}
		}
		break;
	}
}


void CNRPMerchant::SetRecommandProductList()
{	

	m_InterfaceFactory.GetList(NRPMERCHANT_LIST)->Clear();

	SI32 Count = 0;
	TCHAR strPoint[32] = {'\0'};
	TCHAR strProduct[256] = {'\0'};
	TCHAR strUseDate[32] = {'\0'};
	stListBoxItemData Data;

	TCHAR * pRPText =	GetTxtFromMgr(6335);
	TCHAR * pDayText =	GetTxtFromMgr(6336);

	for ( int i = 0; i < MAX_RECOMMAND_PRODUCT; ++i )
	{
		if ( pclClient->pclRPMerchantNPCMgr->m_RecommandProduct[i].ItemUnique > 0 )
		{
			StringCchPrintf( strPoint, 32, pRPText,  pclClient->pclRPMerchantNPCMgr->m_RecommandProduct[i].NeedPoint );
			Data.Set(0, strPoint);
			
			StringCchPrintf( strProduct, 256, TEXT("%s (%d)"), (TCHAR*)pclClient->pclItemManager->GetName( pclClient->pclRPMerchantNPCMgr->m_RecommandProduct[i].ItemUnique ), pclClient->pclRPMerchantNPCMgr->m_RecommandProduct[i].ItemNum );
			if(pclClient->pclRPMerchantNPCMgr->m_RecommandProduct[i].ItemUseDate > 0 )
			{
				StringCchPrintf( strUseDate, 32, pDayText , pclClient->pclRPMerchantNPCMgr->m_RecommandProduct[i].ItemUseDate );
				StringCchCat( strProduct, 256, strUseDate  );
			}			
			Data.Set(1, strProduct);
			m_InterfaceFactory.GetList(NRPMERCHANT_LIST)->AddItem(&Data);

			Count++;
		}
	}
	m_InterfaceFactory.GetList(NRPMERCHANT_LIST)->Refresh();
}


void CNRPMerchant::SetItemImageData(SI32 ItemUnique)
{
	if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( ItemUnique, &m_siImageType, &m_siImageIndex ) )
	{}
	else
	{
		m_siImageType = -1;
		m_siImageIndex = -1;
	}
}

void CNRPMerchant::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY )
{
	if ( ! IsShow() )
		return;

	if(m_siImageIndex < 0)   
		return;

	cltClient *pclclient = ( cltClient * )pclClient;
	TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( m_siImageType );

	if ( pSpr )
	{
  		GP.PutSpr( pSpr ,ScreenRX + GetParentDialog()->GetX() +  GetX() + 263,ScreenRY + GetParentDialog()->GetY() + GetY() + 170, m_siImageIndex);
	}
}

void CNRPMerchant::Hide()
{	
	CControllerMgr::Show(false);
}

void CNRPMerchant::Show()
{	
	CControllerMgr::Show(true);
}

void CNRPMerchant::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch(MsgIndex)
	{
		case 0:
		{
			if(RetValue)
			{
				SI32* SelectedListIndex = (SI32*)pData;
				
				cltItem* item = (cltItem*)&pData[sizeof(SI32)];

				if( *SelectedListIndex < 0  || *SelectedListIndex >= MAX_RECOMMAND_PRODUCT ) return;

				cltGameMsgRequest_BuyRecommandProduct	clRPMerchantBuyItem( *SelectedListIndex , item  );
				cltMsg clMsg( GAMEMSG_REQUEST_BUYRECOMMANDPRODUCT,sizeof(cltGameMsgRequest_BuyRecommandProduct), (BYTE*)&clRPMerchantBuyItem  );
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
			}
			break;
		}
		default:
			break;
	}

}
