// NInvestmentDlg.cpp: implementation of the CNInvestmentDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "../../Client/client.h"


#include "../../lib/WebHTML/WebHTML.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"

#include "NInvestmentDlg.h"


#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "Resource.h"


extern cltCommonLogic* pclClient;
stListBoxItemData NInvestListBoxItemData;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNInvestmentDlg::CNInvestmentDlg()
{


	m_bRectSwitch = FALSE ;
	SetRect(&m_rtDlg,0,0,0,0);

	m_pInvestmentStatic1 = NULL;
	m_pInvestmentStatic2 = NULL;
	m_pInvestmentStatic3 = NULL;
	m_pInvestmentList = NULL;	
	m_pInvestmentButton1 = NULL;
	m_pInvestmentButton2 = NULL;

	m_siNPCCharUnique = 0 ;

	for ( int i = 0 ; i < MAX_INVESTMENT_ITEM_NUMBER ; i ++ )
		m_siItemUnique[i] = 0 ;
}

CNInvestmentDlg::~CNInvestmentDlg()
{	
	SAFE_DELETE(m_pInvestmentStatic1);
	SAFE_DELETE(m_pInvestmentStatic2);
	SAFE_DELETE(m_pInvestmentStatic3);
	SAFE_DELETE(m_pInvestmentList);
	SAFE_DELETE(m_pInvestmentButton1);
	SAFE_DELETE(m_pInvestmentButton2);

}

void CNInvestmentDlg::Create()
{
	
	if(!IsCreate())
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NInvestment/DLG_NInvestment.ddf"));
		file.CreatePopupDialog( this, NINVESTMENT_DLG, TEXT("dialog_Investment"), StatiCNInvestmentDlgProc);


		m_pInvestmentStatic1 = new CStatic( this ); 
		m_pInvestmentStatic2 = new CStatic( this ); 
		m_pInvestmentStatic3 = new CStatic( this );
		m_pInvestmentList      = new CList( this ); 
		m_pInvestmentButton1 = new CButton( this );
		m_pInvestmentButton2 = new CButton( this );	
	 
		file.CreateControl(m_pInvestmentStatic1, NINVESTMENT_STATIC1 , TEXT("statictext_Investment1"));
		file.CreateControl(m_pInvestmentStatic2, NINVESTMENT_STATIC2 , TEXT("statictext_Investment2"));
		file.CreateControl(m_pInvestmentStatic3, NINVESTMENT_STATIC3 , TEXT("statictext_Investment3"));
		file.CreateControl( m_pInvestmentList,    NINVESTMENT_LIST   , TEXT("listbox_Investment"));
		file.CreateControl(m_pInvestmentButton1, NINVESTMENT_BUTTON1 , TEXT("button_Investment1"));	
		file.CreateControl(m_pInvestmentButton2, NINVESTMENT_BUTTON2 , TEXT("button_Investment2"));
		
		TCHAR* pText = GetTxtFromMgr(1641);
		TCHAR buffer[1024] = TEXT("") ;
		m_pInvestmentList->SetColumn(0,40,pText);
		pText = GetTxtFromMgr(1831);
		m_pInvestmentList->SetColumn(1,160,pText);
		pText = GetTxtFromMgr(2303);
		m_pInvestmentList->SetColumn(2,100,pText);
		pText = GetTxtFromMgr(2304);
		m_pInvestmentList->SetColumn(3,100,pText);
		pText = GetTxtFromMgr(2305);
		m_pInvestmentList->SetColumn(4,100,pText);
		m_pInvestmentList->Refresh();
		pText = GetTxtFromMgr(2306);
		StringCchCopy(buffer, 1024, pText);
		m_pInvestmentStatic2->SetBorder(true);
		m_pInvestmentStatic2->SetBKColorUse(true);
		m_pInvestmentStatic2->SetText(buffer,DT_WORDBREAK);

		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash(TEXT("KIND_INVESTMENT")), szPortrait, sizeof(szPortrait), &siPortraitFont);
		m_pInvestmentStatic1->SetFileName(szPortrait);
		m_pInvestmentStatic1->SetFontIndex(siPortraitFont);
		m_pInvestmentStatic3->SetBKColorUse(true);
		m_pInvestmentStatic3->SetBorder(true);
		m_pInvestmentList->SetBorder(true);
		m_pInvestmentList->SetBKColorUse(true);
	}

	//Hide();

	
}

void CALLBACK CNInvestmentDlg::StatiCNInvestmentDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{	
	CNInvestmentDlg *pThis = (CNInvestmentDlg*) pControllerMgr;
	pThis->NInvestmentDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNInvestmentDlg::NInvestmentDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;


	
	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON :	
			{
				DeleteDialog();
			}
			break;
		case NINVESTMENT_BUTTON2:
			{
				DeleteDialog();
			}
			break;
		
		case NINVESTMENT_BUTTON1:
			{
				switch( nEvent)			
				{
				case EVENT_BUTTON_CLICKED:
					{
								
						TCHAR buffer[256] = TEXT("") ;
						TCHAR* pText = GetTxtFromMgr(2309);
						StringCchPrintf(buffer, 256, pText, 	INVESTMENT_GETINFO_MONEY);
						TCHAR* pTitle = GetTxtFromMgr(2310);

						//if (  MessageBox(NULL, buffer,pTitle, MB_ICONQUESTION | MB_YESNO) == IDNO )
						//	break;

						stMsgBox MsgBox;
						MsgBox.Set( this, pTitle,buffer, MSGBOX_TYPE_YESNO, 0 );			

						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof(m_siNPCCharUnique );
						memcpy( TempBuffer, &m_siNPCCharUnique, Size );
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

					
					}
					break;
				}

			}
			break;


		case NINVESTMENT_LIST:
			{
				switch( nEvent)			
				{
				case EVENT_LISTBOX_SELECTION:
					{
						TCHAR buffer[256] = TEXT("") ;
						SI32 siIndex = 0 ;		
						siIndex= m_pInvestmentList->GetSelectedIndex();

						SI32 ref = pclclient->pclItemManager->FindItemRefFromUnique(m_siItemUnique[siIndex ]);
						if (ref <= 0)	break;

						cltItemInfo * pclinfo = pclclient->pclItemManager->pclItemInfo[ref] ;
						
						if ( pclinfo == NULL ) break ;

						for ( int i = 0 ; i < MAX_MATERIAL_NUMBER ; i ++ )
						{
							SI32 materialunique = pclinfo->clProductionInfo.clMaterial[i].siUnique ;
							if ( materialunique <= 0 ) continue;

							const TCHAR * name = pclclient->pclItemManager->GetName(materialunique);

							SI32 neednum = pclinfo->clProductionInfo.clMaterial[i].siNeedNum;

							TCHAR buf[256] = TEXT("") ;

							if ( i == 0 )
							{
								TCHAR* pText = GetTxtFromMgr(2307);
								StringCchPrintf( buf, 256, pText,name,neednum);
							}
							else
							{
								TCHAR* pText = GetTxtFromMgr(2308);
								StringCchPrintf( buf, 256, pText,name,neednum);
							}

							StringCchCat(buffer, 256, buf);
						}

						m_pInvestmentStatic3->SetBKColorUse(true);
						m_pInvestmentStatic3->SetBorder(true);
						m_pInvestmentStatic3->SetText(buffer,DT_WORDBREAK);
						
						

					}
					break;
				}
			}
			break;

	}


    
	return ;
}

void CNInvestmentDlg::Set(SI32 npcunique)
{
	if ( !IsCreate() )
	{
		Create();
	}			
	


	m_pInvestmentStatic3->SetText(TEXT(" "),DT_WORDBREAK);
	m_pInvestmentList->Clear();
	m_pInvestmentList->Refresh();
	m_siNPCCharUnique = npcunique ;	
		
	
	
}

/*
void CNInvestmentDlg::Hide()
{	
	CDialogController::Show( false );
}

void CNInvestmentDlg::Show()
{
	cltClient *pclclient = (cltClient*)pclClient;
	CDialogController::Show( true );	
}
*/

void CNInvestmentDlg::ShowListData(InvestmentLines* investmentlines)
{
								
	cltClient *pclclient = (cltClient*)pclClient;

	SI32 siIndex = 0 ;
	for ( int i = 0 ; i < MAX_INVESTMENT_ITEM_NUMBER ; i ++ )
	{
		if ( investmentlines[i].siInvestmentItemUnique <= 0 ) continue ;

		SI32 ref = pclclient->pclItemManager->FindItemRefFromUnique(investmentlines[i].siInvestmentItemUnique);
		if (ref <= 0) return;

		TCHAR buffer[256] = TEXT("") ;
		StringCchPrintf(buffer, 256, TEXT("%d"),siIndex+1);
		NInvestListBoxItemData.Init();
		NInvestListBoxItemData.Set(0,buffer);

	

		StringCchPrintf(buffer, 256, TEXT("%s"),pclClient->pclItemManager->GetName(investmentlines[i].siInvestmentItemUnique));
		NInvestListBoxItemData.Set(1,buffer);
	
		m_siItemUnique[siIndex] = investmentlines[i].siInvestmentItemUnique ;

		GMONEY materialprice = 0 ;
		for ( int j = 0 ; j < MAX_MATERIAL_NUMBER ; j ++ )
		{
			if ( investmentlines[i].siMaterialItemPrice[j] <= 0 ) continue;

			SI32 neednum = pclclient->pclItemManager->pclItemInfo[ref]->clProductionInfo.clMaterial[j].siNeedNum;
			GMONEY price = investmentlines[i].siMaterialItemPrice[j] * neednum ;

			materialprice += price ;
		}

		TCHAR* pText = GetTxtFromMgr(611);
		g_JYLibFunction.SetNumUnit(materialprice,buffer, 256, pText);

		NInvestListBoxItemData.Set(2,buffer);
	

		g_JYLibFunction.SetNumUnit(investmentlines[i].siInvestmentItemPrice,buffer, 256, pText);	
		NInvestListBoxItemData.Set(3,buffer);

		g_JYLibFunction.SetNumUnit(investmentlines[i].siMarginalProfit,buffer, 256, pText);		
		NInvestListBoxItemData.Set(4,buffer);
		SI32 index = m_pInvestmentList->AddItem( &NInvestListBoxItemData );
		siIndex ++ ;
	}
	m_pInvestmentList->Refresh();
	
}



void CNInvestmentDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	switch( MsgIndex )
	{
	case 0:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{

				//SI32 m_iSelectVillageInx= (SI32)pData;					
				cltGameMsgRequest_InvestmentGetInfo clinfo(m_siNPCCharUnique);
				cltMsg clMsg( GAMEMSG_REQUEST_INVESTMENTGETINFO, sizeof( clinfo ), (BYTE*)&clinfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

			
			
			}
		}
		break;
	}
	return;
}
