
// NNPCMenuDlg .cpp: implementation of the CNNPCMenuDlg  class.
//
//////////////////////////////////////////////////////////////////////

#include "../../../Client/client.h"
//#include "Resource.h"
#include "Char/CharManager/CharManager.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/Msg/MsgType-Quest.h"
#include "../../../CommonLogic/CommonLogic.h"
#include "../../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../../Client/Interface/NPC1/NPCTradeDlg.h"
#include "../../../Client/Interface/JangGilSan/JangGilSanDlg.h"
#include "../../../Client/Interface/InvestmentDlg/InvestmentDlg.h"
#include "../../../Client/Interface/TradeMerchantDlg/TradeMerchantDlg.h"
#include "NInterface/NInvestmentDlg/NInvestmentDlg.h"  //ºÀÀÌ±è¼±´Þ 
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "NInterface/NNPC1/NNPCTradeDlg.h"
#include "ninterface/nmarketconditionsdlg/NMarketConditionsDlg.h"

#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "../Client/NInterface/NTradeMerchant/NTradeMerchant.h"



extern cltCommonLogic* pclClient;

#include "NNPCMenuDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNNPCMenuDlg ::CNNPCMenuDlg ()
{
	m_pNNPCMenuStatic1 = NULL;
	m_pNNPCMenuStatic2 = NULL;
	m_pNNPCMenuButton1 = NULL;
	m_pNNPCMenuButton2 = NULL;
	m_pNNPCMenuButton3 = NULL;
	m_pNNPCMenuButton4 = NULL;
	
	SetRect(&m_rtDlg,0,0,0,0);
	m_bRect = FALSE ;
	m_siCharKind = 0 ;
}

CNNPCMenuDlg ::~CNNPCMenuDlg ()
{	
	SAFE_DELETE(m_pNNPCMenuStatic1);
	SAFE_DELETE(m_pNNPCMenuStatic2);
	SAFE_DELETE(m_pNNPCMenuButton1);
	SAFE_DELETE(m_pNNPCMenuButton2);	
	SAFE_DELETE(m_pNNPCMenuButton3);	
	SAFE_DELETE(m_pNNPCMenuButton4);
}


void CNNPCMenuDlg ::Create()
{
	
	if(!IsCreate())	{

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NNPCMenu/DLG_NNPCMenu.ddf"));
		file.CreatePopupDialog( this, NNPCMENU_DLG, TEXT("dialog_NNPCMenu"), StatiCNNPCMenuProc);


		m_pNNPCMenuStatic1 = new CStatic( this ); 
		m_pNNPCMenuStatic2 = new CStatic( this ); 	
		m_pNNPCMenuButton1 = new CButton( this );
		m_pNNPCMenuButton2 = new CButton( this );	
		m_pNNPCMenuButton3 = new CButton( this );	
		m_pNNPCMenuButton4 = new CButton( this );
		
		file.CreateControl(m_pNNPCMenuStatic1, NNPCMENU_STATIC1 , TEXT("statictext_NNPCMenu1"));
		file.CreateControl(m_pNNPCMenuStatic2, NNPCMENU_STATIC2 , TEXT("statictext_NNPCMenu2"));
		file.CreateControl(m_pNNPCMenuButton1, NNPCMENU_BUTTON1 , TEXT("button_NNPCMenu1"));	
		file.CreateControl(m_pNNPCMenuButton2, NNPCMENU_BUTTON2 , TEXT("button_NNPCMenu2"));
		file.CreateControl(m_pNNPCMenuButton3, NNPCMENU_BUTTON3 , TEXT("button_NNPCMenu3"));
		file.CreateControl(m_pNNPCMenuButton4, NNPCMENU_BUTTON4 , TEXT("button_NNPCMenu4"));
	}

}

void CNNPCMenuDlg ::CreateDlg(SI32 charkind,SI32 charunique,TCHAR * txt1 , TCHAR * txt2 ,
							bool button1,bool button2,TCHAR * txt )
{
	if(!IsCreate())	{
				
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NNPCMenu/DLG_NNPCMenu.ddf"));
		file.CreatePopupDialog( this, NNPCMENU_DLG, TEXT("dialog_NNPCMenu"), StatiCNNPCMenuProc);


		m_pNNPCMenuStatic1 = new CStatic( this ); 
		m_pNNPCMenuStatic2 = new CStatic( this ); 	

		m_pNNPCMenuButton1 = new CButton( this );
		m_pNNPCMenuButton2 = new CButton( this );	
		m_pNNPCMenuButton3 = new CButton( this );	
		m_pNNPCMenuButton4 = new CButton( this );	
		
		file.CreateControl(m_pNNPCMenuStatic1, NNPCMENU_STATIC1 , TEXT("statictext_NNPCMenu1"));
		file.CreateControl(m_pNNPCMenuStatic2, NNPCMENU_STATIC2 , TEXT("statictext_NNPCMenu2"));

		file.CreateControl(m_pNNPCMenuButton1, NNPCMENU_BUTTON1 , TEXT("button_NNPCMenu1"));	
		file.CreateControl(m_pNNPCMenuButton2, NNPCMENU_BUTTON2 , TEXT("button_NNPCMenu2"));
		file.CreateControl(m_pNNPCMenuButton3, NNPCMENU_BUTTON3 , TEXT("button_NNPCMenu3"));
		// [Èñ¿µ] ½Ã¼¼È®ÀÎ 2008-07-18
		file.CreateControl(m_pNNPCMenuButton4, NNPCMENU_BUTTON4 , TEXT("button_NNPCMenu4"));

	}
	
	//if (m_siCharKind == charkind )

	SI16 questtype, para1, para2;
	SI16 questunique;
	SI16 questclearcond =  QUEST_CLEAR_COND_GONPC;
	if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
	{
		if(para1 == charkind )
		{
			cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
			cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
	}
	

	if ( txt1 == NULL ) return ;
	if ( txt2 == NULL ) return ;
	if ( txt == NULL ) return ;
	if ( charkind <= 0 || charkind > MAX_KIND_NUMBER ) return ;
	if ( charunique <= 0 ) return ;
	cltKindInfo* clKind = pclClient->pclKindInfoSet->pclKI[charkind];
	if ( clKind == NULL ) return ;

	m_siCharKind = charkind ;
	m_siCharUnique = charunique ;
	
	TCHAR buffer[1024] = TEXT("") ;
	
	TCHAR szPortrait[256];
	SI32 siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(m_siCharKind, szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_pNNPCMenuStatic1->SetFileName(szPortrait);
	m_pNNPCMenuStatic1->SetFontIndex(siPortraitFont);

	if (m_siCharKind==pclClient->GetUniqueFromHash(TEXT("KIND_INVESTMENT")))  // ºÀÀÌ±è¼±´ÞÀÏ¶§ 
	{
        TCHAR* pText = GetTxtFromMgr(3390);		
		m_pNNPCMenuButton1->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pText = GetTxtFromMgr(3391);	
		m_pNNPCMenuButton2->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		//cyj Ãß°¡
		pText = GetTxtFromMgr(5640);
		m_pNNPCMenuButton3->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		
		pText = GetTxtFromMgr(3394);	
		SetTitle(pText);
	}
	else  //¾ß¸°½ºÅ° ÀÏ¶§ 
	{
		TCHAR* pText = GetTxtFromMgr(3392);
		m_pNNPCMenuButton1->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pText = GetTxtFromMgr(3393);
		m_pNNPCMenuButton2->SetText(pText,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pText = GetTxtFromMgr(3395);
		SetTitle(pText);
		
		m_pNNPCMenuButton3->Show(false);
	}	

	m_pNNPCMenuButton4->Show(false);

	if ( pclClient->IsCountrySwitch(Switch_MarketConditions) ) // [Èñ¿µ] ½Ã¼¼È®ÀÎ 2008-07-18
	{	
		m_pNNPCMenuButton4->Show(true);

		TCHAR* pText = GetTxtFromMgr(8595);
		m_pNNPCMenuButton4->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	StringCchCopy(buffer, 1024, txt);
	m_pNNPCMenuStatic2->SetBorder(true);
	m_pNNPCMenuStatic2->SetBKColorUse(true);
	m_pNNPCMenuStatic2->SetText(buffer,DT_WORDBREAK);

			
	
	
}

void CALLBACK CNNPCMenuDlg ::StatiCNNPCMenuProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNNPCMenuDlg *pThis = (CNNPCMenuDlg*) pControllerMgr;
	pThis->NNPCMenuProc( nEvent, nControlID, pControl );
}

void CALLBACK CNNPCMenuDlg ::NNPCMenuProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;
	
	
	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:	
			{
				DeleteDialog();
				//Hide();
			}
			break;

		case NNPCMENU_BUTTON1:
			{
				switch( nEvent)			
				{
				case EVENT_BUTTON_CLICKED:
					{
						if(m_siCharKind ==pclClient->GetUniqueFromHash(TEXT("KIND_INVESTMENT")) ) //ºÀÀÌ±è¼±´ÞÀÌ¸é ..
						{       
							if((m_siCharKind != NULL) && ((pclclient->m_pDialog[ NNPCTRADE1_DLG ]) == NULL))
							{
								pclclient->CreateInterface( NNPCTRADE1_DLG );
								((CNNPCTradeDlg*)pclclient->m_pDialog[ NNPCTRADE1_DLG  ])->Show(m_siCharKind);
								DeleteDialog();
							}
							else
							{
								(pclclient->m_pDialog[ NNPCTRADE1_DLG  ])->DeleteDialog();

							}
						}
						else    // ¾ß¸°½ºÅ°¸é ..
						{	
							if((m_siCharKind != NULL) && ((pclclient->m_pDialog[ NNPCTRADE1_DLG ]) == NULL))
							{
								pclclient->CreateInterface( NNPCTRADE1_DLG );
								((CNNPCTradeDlg*)pclclient->m_pDialog[ NNPCTRADE1_DLG  ])->Show(m_siCharKind);
								DeleteDialog();
							}
							else
							{
								( pclclient->m_pDialog[ NNPCTRADE1_DLG ])->DeleteDialog();
							}
						}

					}
					break;
				}
			}
			break;


		case NNPCMENU_BUTTON2:
			{
				switch(nEvent)
				{
					case EVENT_BUTTON_CLICKED:
						{
							
							if(m_siCharKind ==pclClient->GetUniqueFromHash(TEXT("KIND_INVESTMENT")) ) //ºÀÀÌ±è¼±´ÞÀÌ¸é ..						
							{
								if((m_siCharKind != NULL) &&( (pclclient->m_pDialog[NINVESTMENT_DLG]) == NULL))
								{
									pclclient->CreateInterface(NINVESTMENT_DLG);
									((CNInvestmentDlg*)pclclient->m_pDialog[NINVESTMENT_DLG])->Set(m_siCharUnique);
									DeleteDialog();
								}
								else
								{
									(pclclient->m_pDialog[NINVESTMENT_DLG])->DeleteDialog();
								}
								
							}
							else    // ¾ß¸°½ºÅ°¸é ..
							{
								pclclient->CreateInterface( NTRADEMERCHANT_DLG );
								if( ((CNTradeMerchantDlg*)pclclient->m_pDialog[NTRADEMERCHANT_DLG]) )
									((CNTradeMerchantDlg*)pclclient->m_pDialog[ NTRADEMERCHANT_DLG ])->SetTradeMerChant(m_siCharUnique);
								DeleteDialog();
							}

						}
						break;
				}
			}
			break;

		case NNPCMENU_BUTTON3:
			{
				switch( nEvent )
				{
				case EVENT_BUTTON_CLICKED:
					{
						if(m_siCharKind ==pclClient->GetUniqueFromHash(TEXT("KIND_INVESTMENT")) ) //ºÀÀÌ±è¼±´ÞÀÌ¸é ..
						{                  

							if((m_siCharKind != NULL) && ((pclclient->m_pDialog[ NSAVEITEMLIST_DLG ]) == NULL))
							{
								pclclient->CreateInterface( NSAVEITEMLIST_DLG );

								cltMsg clMsg(GAMEMSG_REQUEST_GET_SAVEUSERITEMLIST, 0, NULL);
								pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
							
								DeleteDialog();
							}
							else
							{
								(pclclient->m_pDialog[ NSAVEITEMLIST_DLG  ])->DeleteDialog();
							}

						}
					}
					break;
				}
			}
			break;
		// [Èñ¿µ] ½Ã¼¼È®ÀÎ 2008-07-18	
		case NNPCMENU_BUTTON4:
			{
				switch( nEvent )
				{
				case EVENT_BUTTON_CLICKED:
					{
						if(m_siCharKind ==pclClient->GetUniqueFromHash(TEXT("KIND_INVESTMENT")) ) //ºÀÀÌ±è¼±´ÞÀÌ¸é ..
						{                  

							if((m_siCharKind != NULL) && ((pclclient->m_pDialog[ NMARKETCONDITIONS_DLG ]) == NULL))
							{
								pclclient->CreateInterface( NMARKETCONDITIONS_DLG );

								((CNMarketConditionsDlg*)pclclient->m_pDialog[NMARKETCONDITIONS_DLG])->Set(m_siCharUnique);							
								DeleteDialog();
							}
							else
							{
								(pclclient->m_pDialog[ NMARKETCONDITIONS_DLG  ])->DeleteDialog();
							}

						}
					}
					break;
				}
			}
			break;

	}//end swith

	return;
}
