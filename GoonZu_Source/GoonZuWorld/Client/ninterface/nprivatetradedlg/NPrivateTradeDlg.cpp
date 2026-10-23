#include "NPrivateTradeDlg.h"

#include "NPTradeChild1.h"
#include "NPTradeChild2.h"
#include "NPTradeChild3.h"
#include "NPTradeChild4.h"

#include "..\..\..\Common\SMLib\LibList\LibList.h"
#include "..\..\..\Common\PrivateTradeOrderMgr\PrivateTradeOrderObj.h"

//#include "..\..\Interface\PrivateChatDlg\PrivateChatDlgMgr.h"

#include "..\..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Interface\NotifyMsgDlg\NotifyMsgDlg.h"


#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/House/House.h"
#include "../../CommonLogic/Msg/MsgType-Web.h"

#include "../../../lib/WebHTML/WebHTML.h"



#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/interface/Button.h"
#include "../../InterfaceMgr/interface/ComboBox.h"
#include "../../InterfaceMgr/interface/OutLine.h"
#include "../../InterfaceMgr/interface/List.h"
#include "../../InterfaceMgr/interface/Edit.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "PrivateTradeBaseInfo.h"


#include "Client.h"

extern cltCommonLogic* pclClient;

stListBoxItemData ptradeListBoxItemData;
stListBoxItemData ptrade_otherListBoxItemData;

CNPrivateTradeDlg::CNPrivateTradeDlg()
{	
	m_pNPTradeOutline1 = NULL;
	m_pNPTradeOutline2 = NULL;
	m_pNPTradeOutline3 = NULL;
	m_pNPTradeStatic1  = NULL;
	m_pNPTradeEdit  = NULL;
	m_pNPTradeStatic3  = NULL;
	m_pNPTradeList4  = NULL;
	m_pNPTradeEdit5  = NULL;
	m_pNPTradeList6  = NULL;
	m_pNPTradeEdit7  = NULL;
	m_pNPTradeStatic8  = NULL;

	m_pNPTradeButton1  = NULL; 
	m_pNPTradeButton2  = NULL; 
	m_pNPTradeButton3  = NULL; 
	m_pNPTradeButton4  = NULL; 
	m_pNPTradeButton5  = NULL; 
	m_pNPTradeButton6  = NULL; 
	m_pNPTradeButton7  = NULL; 
	m_pNPTradeButton8  = NULL; 
	m_pNPTradeButton9  = NULL; 
	m_pNPTradeButton10 = NULL; 
	m_pNPTradeButton11 = NULL; 

	NTradetabDlg1 = NULL;
	NTradetabDlg2 = NULL;
	NTradetabDlg3 = NULL;
	NTradetabDlg4 = NULL;
	SetActionTimer( 200);
	SetTimerOnOff( true );
	m_siBitmapAnimationIndex = 3;
	m_pPrivateTradeBufferMgr = NULL;
	m_pEachPersonTradeOrderInfo = new CEachPersonTradeOrderInfo();

	m_siStockNum = 0;
	m_siStockVillageUnique = 0;

}

CNPrivateTradeDlg::~CNPrivateTradeDlg()
{	
	
	SetTimerOnOff( false );
	SAFE_DELETE(m_pNPTradeOutline1);
	SAFE_DELETE(m_pNPTradeOutline2);
	SAFE_DELETE(m_pNPTradeOutline3);
 
	SAFE_DELETE(m_pNPTradeStatic1);
	SAFE_DELETE(m_pNPTradeEdit);
	SAFE_DELETE(m_pNPTradeStatic3);
	SAFE_DELETE(m_pNPTradeList4);
	SAFE_DELETE(m_pNPTradeEdit5);
	SAFE_DELETE(m_pNPTradeList6);
	SAFE_DELETE(m_pNPTradeEdit7);
	SAFE_DELETE(m_pNPTradeStatic8);
	SAFE_DELETE(m_pNPTradeStatic9);

	SAFE_DELETE(m_pNPTradeButton1);
	SAFE_DELETE(m_pNPTradeButton2);
	SAFE_DELETE(m_pNPTradeButton3);
	SAFE_DELETE(m_pNPTradeButton4);
	SAFE_DELETE(m_pNPTradeButton5);
	SAFE_DELETE(m_pNPTradeButton6);
	SAFE_DELETE(m_pNPTradeButton7);
	SAFE_DELETE(m_pNPTradeButton8);
	SAFE_DELETE(m_pNPTradeButton9);
	SAFE_DELETE(m_pNPTradeButton10);
	SAFE_DELETE(m_pNPTradeButton11);


	SAFE_DELETE(m_pPrivateTradeBufferMgr);
	SAFE_DELETE(m_pEachPersonTradeOrderInfo);

	SAFE_DELETE(NTradetabDlg1);
	SAFE_DELETE(NTradetabDlg2);
	SAFE_DELETE(NTradetabDlg3);
	SAFE_DELETE(NTradetabDlg4);


}


void CNPrivateTradeDlg::Create()
{
	if(!IsCreate()) 
	{

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NPrivateTrade/DLG_NPrivateTrade.ddf"));
		file.CreatePopupDialog( this, NPRIVATETRADE_DLG, TEXT("dialog_PrivateTrade"),StaticNPrivateTradeDlgProc);
		

		m_pNPTradeOutline1 = new COutLine(this);
		m_pNPTradeOutline2 = new COutLine(this);
		m_pNPTradeOutline3 = new COutLine(this);

		m_pNPTradeStatic1  = new CStatic( this );
		m_pNPTradeEdit  = new CEdit( this );
		m_pNPTradeStatic3  = new CStatic( this );
		m_pNPTradeList4  = new CList( this );
		m_pNPTradeEdit5  = new CEdit( this );
		m_pNPTradeList6  = new CList( this );
		m_pNPTradeEdit7  = new CEdit( this );
		m_pNPTradeStatic8  = new CStatic( this );
		m_pNPTradeStatic9  = new CStatic( this );

		m_pNPTradeButton1  = new CButton( this ); 
		m_pNPTradeButton2  = new CButton( this ); 
		m_pNPTradeButton3  = new CButton( this ); 
		m_pNPTradeButton4  = new CButton( this ); 
		m_pNPTradeButton5  = new CButton( this ); 
		m_pNPTradeButton6  = new CButton( this ); 
		m_pNPTradeButton7  = new CButton( this ); 
		m_pNPTradeButton8  = new CButton( this ); 
		m_pNPTradeButton9  = new CButton( this ); 
		m_pNPTradeButton10 = new CButton( this ); 
		m_pNPTradeButton11 = new CButton( this ); 


		file.CreateControl(m_pNPTradeOutline1, NPRIVATETRADE_OUTLINE1,  TEXT("outline_PrivateTrade1"));
		file.CreateControl(m_pNPTradeOutline2, NPRIVATETRADE_OUTLINE2,  TEXT("outline_PrivateTrade2"));
		file.CreateControl(m_pNPTradeOutline3, NPRIVATETRADE_OUTLINE3,  TEXT("outline_PrivateTrade3"));

		file.CreateControl(m_pNPTradeStatic1, NPRIVATETRADE_STATIC1 , TEXT("statictext_PrivateTrade1"));	
		file.CreateControl(m_pNPTradeEdit, NPRIVATETRADE_STATIC2 , TEXT("editbox_PrivateTrade2"));
		file.CreateControl(m_pNPTradeStatic3, NPRIVATETRADE_STATIC3 , TEXT("statictext_PrivateTrade3"));
		file.CreateControl(m_pNPTradeList4, NPRIVATETRADE_LIST4 , TEXT("listbox_PrivateTrade4"));
		file.CreateControl(m_pNPTradeEdit5, NPRIVATETRADE_EDIT5 , TEXT("editbox_PrivateTrade5"));
		file.CreateControl(m_pNPTradeList6, NPRIVATETRADE_LIST6 , TEXT("listbox_PrivateTrade6"));
		file.CreateControl(m_pNPTradeEdit7, NPRIVATETRADE_EDIT7 , TEXT("editbox_PrivateTrade7"));
		file.CreateControl(m_pNPTradeStatic8, NPRIVATETRADE_STATIC8 , TEXT("statictext_PrivateTrade8"));
		file.CreateControl(m_pNPTradeStatic9, NPRIVATETRADE_STATIC9 , TEXT("statictext_PrivateTrade9"));		
		file.CreateControl(m_pNPTradeButton1 , NPRIVATETRADE_BUTTON1 , TEXT("button_PrivateTrade1" )); 
		file.CreateControl(m_pNPTradeButton2 , NPRIVATETRADE_BUTTON2 , TEXT("button_PrivateTrade2" ));
		file.CreateControl(m_pNPTradeButton3 , NPRIVATETRADE_BUTTON3 , TEXT("button_PrivateTrade3" ));
		file.CreateControl(m_pNPTradeButton4 , NPRIVATETRADE_BUTTON4 , TEXT("button_PrivateTrade4" ));
		file.CreateControl(m_pNPTradeButton5 , NPRIVATETRADE_BUTTON5 , TEXT("button_PrivateTrade5" ));
		file.CreateControl(m_pNPTradeButton6 , NPRIVATETRADE_BUTTON6 , TEXT("button_PrivateTrade6" ));
		file.CreateControl(m_pNPTradeButton7 , NPRIVATETRADE_BUTTON7 , TEXT("button_PrivateTrade7" ));
		file.CreateControl(m_pNPTradeButton8 , NPRIVATETRADE_BUTTON8 , TEXT("button_PrivateTrade8" ));
		file.CreateControl(m_pNPTradeButton9 , NPRIVATETRADE_BUTTON9 , TEXT("button_PrivateTrade9" ));
		file.CreateControl(m_pNPTradeButton10, NPRIVATETRADE_BUTTON10, TEXT("button_PrivateTrade10"));
		file.CreateControl(m_pNPTradeButton11, NPRIVATETRADE_BUTTON11, TEXT("button_PrivateTrade11"));
		
		m_pNPTradeStatic3->SetText(TEXT(" "),DT_WORDBREAK);
		m_pNPTradeStatic8->SetText(TEXT(" "),DT_WORDBREAK);
		m_pNPTradeEdit5->SetBKColorUse(true);
		m_pNPTradeEdit5->SetText(TEXT(" "));
		m_pNPTradeEdit7->SetBKColorUse(true);
		m_pNPTradeEdit7->SetText(TEXT(" "));
		m_pNPTradeList4->SetBorder(true);
		m_pNPTradeList4->SetBKColorUse(true);
		m_pNPTradeList6->SetBorder(true);
		m_pNPTradeList6->SetBKColorUse(true);

		//cyj 시전거래는 삭제
		m_pNPTradeButton7->Show(false);


		if(!NTradetabDlg1)
		{
            NTradetabDlg1= new CNTradeChild1();
			NTradetabDlg1->Create(this);
			NTradetabDlg1->Show();
		}
		
		if(!NTradetabDlg2)
		{
            NTradetabDlg2= new CNTradeChild2();
			NTradetabDlg2->Create(this);
			NTradetabDlg2->Hide();
		}

		
		if(!NTradetabDlg3)
		{
            NTradetabDlg3= new CNTradeChild3();
			NTradetabDlg3->Create(this);
			NTradetabDlg3->Hide();
		}

		if(!NTradetabDlg4)
		{
            NTradetabDlg4= new CNTradeChild4();
			NTradetabDlg4->Create(this);
			NTradetabDlg4->Hide();
		}


		
		m_pPersonItemInfo = &pclClient->pclCM->CR[1]->pclCI->clCharItem;

		if ( m_pPrivateTradeBufferMgr == NULL )
		{
			m_pPrivateTradeBufferMgr = new CLibListMgr();
			m_pPrivateTradeBufferMgr->CreateMaxEmptyObj( MAX_PRIVATETRADE_OBJ_NUM, MAX_PRIVATETRADE_OBJ_BUFFER_SIZE );
		}

		TCHAR* pText = GetTxtFromMgr(3403);
		m_pNPTradeList6->SetColumn(0,155,pText);
		m_pNPTradeList6->Refresh();
		pText = GetTxtFromMgr(3404);
		m_pNPTradeList4->SetColumn(0,155,pText);

		m_pNPTradeList4->Refresh();

		pText = GetTxtFromMgr(3412);
		m_pNPTradeButton9->SetText(pText, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
		if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
		{
			m_pNPTradeButton3->Show(false);
		}

	}

}

void CNPrivateTradeDlg::Action()
{
	
	if(m_siSelectedModeIndex ==PRIVATETRADE_TRADE_MODE)
	{
		m_pNPTradeStatic1->SetFileName(TEXT("Interface/PrivateTrade/repair.SPR"));
		m_pNPTradeStatic1->SetFontIndex(m_siBitmapAnimationIndex++);
		if ( m_siBitmapAnimationIndex >= 6 )
			m_siBitmapAnimationIndex = 3;
	}

	// 수락버튼 비활성화 -> (활성화 60->80 으로 증가.2009-03-06 거래 실패확률이 높아 버튼 비활성화 시간을 늘려달라는 요청에 따라 수정)

	/*if ( m_bMyAccept == false && TABS( pclClient->GetFrame() - m_uiLastAcceptFrame ) > 60 )			*/
	// 3초 지나서 활성화 시킨다
	if ( m_bMyAccept == false && TABS( pclClient->CurrentClock - m_siLastAcceptTime ) > 3000 )
	{
		m_pNPTradeButton9->Enable(true);				
	}
		
	if ( NTradetabDlg1->IsInventoryChanged() )
	{	
		NTradetabDlg1->LoadInventoryData();
	}
		
	if ( NTradetabDlg2->IsStockListChanged() )
	{	
		NTradetabDlg2->LoadStockList();
	}

		
	if (  NTradetabDlg4->IsHouseUnitListChanged() )	
	{				
		 NTradetabDlg4->LoadHouseUnitList();
	}

}

void CALLBACK CNPrivateTradeDlg::StaticNPrivateTradeDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNPrivateTradeDlg *pThis = (CNPrivateTradeDlg*) pControllerMgr;
	pThis->NPrivateTradeDlgProc( nEvent, nControlID, pControl );

}
	
void CALLBACK CNPrivateTradeDlg::NPrivateTradeDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
	switch (nControlID  )
	{	
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			PushCancel();		
		}
		break;
	case NPRIVATETRADE_LIST4:
		{			
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{					
					SI32 SeletedNum = m_pNPTradeList4->GetSelectedIndex();
					PrivateTradeOtherListInfo(  SeletedNum );			
				}
				break;
			}
		}
		break;

	case NPRIVATETRADE_LIST6:
		{			
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{				
					SI32 SeletedNum = m_pNPTradeList6->GetSelectedIndex();
					PrivateTradeMyListInfo( SeletedNum );				
				}
				break;
			}
		}
		break;
	

	case NPRIVATETRADE_BUTTON1:   // ?
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
									
					if ( m_siSelectedModeIndex == PRIVATETRADE_TRADE_MODE )
					{
						TCHAR* pText = GetTxtFromMgr(2322);
						m_pNPTradeEdit->SetText(pText);						
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr(2323);
						m_pNPTradeEdit->SetText(pText);						
					}

				}
				break;
			}
		}break;

	case NPRIVATETRADE_BUTTON2: //귓속말 보내기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{	
					if ( m_siOtherCharUnique > 0 )
					{
						cltClient *pclclient = ( cltClient * )pclClient;

						if ( m_strOtherCharName != NULL && _tcslen( m_strOtherCharName ) > 0 )
						{							
							pclclient->RequestSecretChatByCharUnique(m_siOtherCharUnique);
						}
						else
						{							
							pclclient->RequestSecretChatByCharUnique(m_siOtherCharUnique);
						}
					}
				
				}
				break;
			}
		}break;

	case NPRIVATETRADE_BUTTON3: //상대방 꼼지가기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
						
					if ( m_siOtherCharUnique > 0 )
					{
						cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( m_siOtherCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

						/*
						cltGameMsgRequest_Userid clUserid( m_siOtherCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
						cltClient *pclclient = ( cltClient * )pclClient;
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
						*/
					}


				}
				break;
			}
		}break;

	case NPRIVATETRADE_BUTTON4:   //돈 물품 탭
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{	
					
					if(NTradetabDlg1->IsShow())
						break;

					if(NTradetabDlg2->IsShow())
						NTradetabDlg2->Hide();
					if(NTradetabDlg3->IsShow())
						NTradetabDlg3->Hide();
					if(NTradetabDlg4->IsShow())
						NTradetabDlg4->Hide();

					NTradetabDlg1->Show();

				}
				break;
			}
		}break;

	case NPRIVATETRADE_BUTTON5: //주식 탭
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(NTradetabDlg2->IsShow())
						break;
					if(NTradetabDlg1->IsShow())
						NTradetabDlg1->Hide();
					if(NTradetabDlg3->IsShow())
						NTradetabDlg3->Hide();
					if(NTradetabDlg4->IsShow())
						NTradetabDlg4->Hide();		

					NTradetabDlg2->Show();
				}
				break;
			}
		}break;

	case NPRIVATETRADE_BUTTON6:  //말 탭
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{										
					if(NTradetabDlg3->IsShow())
						break;
					if(NTradetabDlg1->IsShow())
						NTradetabDlg1->Hide();
					if(NTradetabDlg2->IsShow())
						NTradetabDlg2->Hide();
					if(NTradetabDlg4->IsShow())
						NTradetabDlg4->Hide();
				

					NTradetabDlg3->Show();
				}
				break;
			}
		}break;


	case NPRIVATETRADE_BUTTON7: //부동산 탭
		{
			/*cyj 시전거래는 삭제
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(NTradetabDlg4->IsShow())
						break;
					if(NTradetabDlg1->IsShow())
						NTradetabDlg1->Hide();
					if(NTradetabDlg2->IsShow())
						NTradetabDlg2->Hide();
					if(NTradetabDlg3->IsShow())
						NTradetabDlg3->Hide();
				

					NTradetabDlg4->Show();
				}
				break;
			}
			*/
		}break;
	
	case NPRIVATETRADE_BUTTON8: //선택 지우기
		{
			switch( nEvent )
			{
				//listbox 가 하나 이상 추가 될때 선택해서 물품 정보 가져오는  알고리즘 필요함 
			case EVENT_BUTTON_CLICKED:
				{	
					/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
					m_siLastAcceptTime = pclClient->CurrentClock;
					//EnableWindow( m_hMyAcceptButton, false );
					m_pNPTradeButton9->Enable(false);

					PushDelList();

			
				}
				break;
			}
		}break;

	case NPRIVATETRADE_BUTTON9: //수락하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					
									
					if ( m_bMyAccept )
					{
						PushMyCancel();
					}
					else
					{
						/*if ( TABS( pclClient->GetFrame() - m_uiLastAcceptFrame ) < 60 )*/
						if ( TABS( pclClient->CurrentClock - m_siLastAcceptTime ) < 1000 )
						{
							break;
							//return FALSE;
						}

						PushMyAccept();
					}
				}
				break;
			}	
		}break;

	case NPRIVATETRADE_BUTTON10: //수락하기 옆의?
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
								
					if ( m_siSelectedModeIndex == PRIVATETRADE_TRADE_MODE )
					{
						TCHAR* pText = GetTxtFromMgr(2326);						
						m_pNPTradeEdit->SetText(pText);	
					}
					/*
					else if ( m_siSelectedModeIndex == PRIVATETRADE_REPAIR_MODE )
					{
						TCHAR* pText = GetTxtFromMgr(2327);						
						m_pNPTradeEdit->SetText(pText);					
					}
					*/
				}
				break;
			}
		}break;

	
	case NPRIVATETRADE_BUTTON11: //보낼물품 옆의 ?
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
								
					if ( m_siSelectedModeIndex == PRIVATETRADE_TRADE_MODE )
					{
						TCHAR* pText = GetTxtFromMgr(2324);
						m_pNPTradeEdit->SetText(pText);						
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr(2325);
						m_pNPTradeEdit->SetText(pText);					
					}
				}
				break;
			}
		}break;


	}
	

	return;

}

void CNPrivateTradeDlg::Show( SI32 siOtherCharUnique, SI16 TradeMode, SI16 Level, TCHAR *Name, TCHAR *AccountID, SI16 siPrivateTradeOrderMgrIndex, SI16 siPrivateTradeOrderCountIndex, SI32 OtherCharLevel )
{
	// PrivateTrdeOrderMgr로 부터 할당받지 못한 경우
	// siOtherCharUnique 가 잘못된 값인 경우
	cltClient *pclclient = (cltClient *)pclClient;


	if ( siPrivateTradeOrderMgrIndex < 1 || siOtherCharUnique < 1 )		
	{
		return;
	}


	
	if( pclclient->bWatchModeSwitch )
	{
		return;
	}

	m_siOtherCharUnique = siOtherCharUnique;

	m_siOtherCharLevel = Level;

	m_siSelectedModeIndex = TradeMode;

	TCHAR Buffer[256 ] = TEXT("");

	m_siBitmapAnimationIndex = 3;

	if ( TradeMode == PRIVATETRADE_TRADE_MODE )
	{
		m_siBitmapAnimationIndex = 3;
		if ( Name != NULL && _tcslen( Name ) > 0 )
		{		
			TCHAR* pText = GetTxtFromMgr(2328);
			StringCchPrintf( Buffer, 256, pText, Name, OtherCharLevel );			
			SetTitle(Buffer);
			MStrCpy( m_strOtherCharName, Name, MAX_PLAYER_NAME );
			
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(2329);
			//SetWindowText( m_hDlg, pText ); //거래하기
		}
		
		TCHAR* pText = GetTxtFromMgr(2330);
		m_pNPTradeStatic3->SetText(pText,DT_WORDBREAK);		
		pText = GetTxtFromMgr(2331);
		m_pNPTradeStatic8->SetText(pText,DT_WORDBREAK);

	}
	

	// 내 계정 아이디
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.szAccountID == NULL || _tcslen ( pclClient->pclCM->CR[1]->pclCI->clBI.szAccountID ) < 1 )
	{
		return;
	}



	// 다른 사람 계정 아이디
	if ( AccountID == NULL || _tcslen( AccountID ) < 1 )
	{
		return;
	}

	m_siAccpetCounter = 0;

	m_siPrivateTrdeOrderMgrIndex = siPrivateTradeOrderMgrIndex;
	m_siPrivateTrdeOrderCountIndex = siPrivateTradeOrderCountIndex;

	
	(NTradetabDlg1->m_pNPTrade1Edit2)->SetText(TEXT("0"));

	m_bMyAccept = false;

	(NTradetabDlg1->m_pNPTrade1Button2)->Enable(true);
	(NTradetabDlg1->m_pNPTrade1Edit2)->Enable(true);

	// 돈 입력 버튼 활성화
	m_pNPTradeButton4->Enable(true);	
	// 돈 입력 에디트박스 활성화
	m_pNPTradeEdit5->Enable(true);
	
	
	

	m_pNPTradeButton9->Enable(true);
	//TCHAR* pText = GetTxtFromMgr(2336);	
	//m_pNPTradeStatic9->SetText(pText,DT_WORDBREAK);
	m_pNPTradeEdit5->SetText(TEXT("0"));
	m_pNPTradeEdit7->SetText(TEXT("0"));

	NTCHARString512 kTradeBaseInfo(GetTxtFromMgr(2337));
	TCHAR szPrivateTradeMoneyLimit[32];
	g_JYLibFunction.SetNumUnit(pclClient->GetGlobalValue("GV_PrivateTradeMoneyLimit"), szPrivateTradeMoneyLimit, sizeof(szPrivateTradeMoneyLimit));
	
	NTCHARString512	kRawPrice( szPrivateTradeMoneyLimit );
	kRawPrice.ReplaceAll(",", "");	// , 삭제
	TCHAR		szPrice[64] = { '\0', };
	GMONEY		siPrice = _tstoi64(kRawPrice);
	if( kRawPrice.Length() > 0 )
		g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));
	kTradeBaseInfo.ReplaceAll("#limitmoney#", szPrice);
	kTradeBaseInfo.ReplaceAll("#limitstock#", SI64ToString(pclClient->GetGlobalValue("GV_PrivateTradeStockLimit")));
	m_pNPTradeEdit->SetText(kTradeBaseInfo);

	//SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_EXPLAIN ), TradeBaseInfo );

	m_pNPTradeEdit5->SetText(TEXT(" "));
	m_pNPTradeEdit7->SetText(TEXT(" "));
		
	m_siStockVillageUnique = 0;
	m_siStockNum = 0;
	
	
	NTradetabDlg2->LoadStockList();
	(NTradetabDlg2->m_pNPTrade2Edit)->SetText(TEXT("0"));
	
	m_siMyPrivateTradeListSelectedIndex = -1;

	SetActionTimer(200);	
	
	m_bShow = true;


	return ;

}


void CNPrivateTradeDlg::PushAddItem()
{
	SI32 m_siSelectedIndex = NTradetabDlg1->m_siInvSelectedIndex;
	if( m_siSelectedIndex < PERSONITEM_INV0  || m_siSelectedIndex >= MAX_ITEM_PER_PERSON)
	{
		return;
	}

	m_clItem[m_siSelectedIndex] = NTradetabDlg1->m_clUserItem[m_siSelectedIndex];
	
    // 숫자 몇개가 올라갔는지 .
	SI32 AddItemNum = _tstoi((NTradetabDlg1->m_pNPTrade1Edit1)->GetText());

	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stPrivateTrade_Add_Item sAddItem( m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		m_siSelectedIndex,
		&m_clItem[ m_siSelectedIndex ],
		AddItemNum
		);

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sAddItem, sAddItem.sHeader.uiSize );
	}	

	return;
}



void CNPrivateTradeDlg::PushMyAccept()
{
	cltClient *pclclient = (cltClient *)pclClient;

	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
	
	//////////////////////////////////////////////////////////////////////////	
	// 말 거래시에 말 인벤토리에 아이템이 들어 있는지 체크
	if ( pTradeData->m_siHorseIndex != -1 )
	{
		if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.IsHorseHaveItem() == true )
		{
			TCHAR* pTitle = GetTxtFromMgr(2132);
			TCHAR* pText = GetTxtFromMgr(2133);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////	


	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();
	
	stPrivateTrade_MyAccept sMyAccept( m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		m_siAccpetCounter
		);

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sMyAccept, sMyAccept.sHeader.uiSize );
	}

	return;
}

void CNPrivateTradeDlg::PushMyCancel()
{
	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stPrivateTrade_MyCancel sMyCancel
		(
		m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex
		);

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sMyCancel, sMyCancel.sHeader.uiSize );
	}


	return;
}



void CNPrivateTradeDlg::PushStock()
{
	SI32 siSelectedVillageIndex = NTradetabDlg2->m_siSelectedVillageIndex;	
	
	if ( siSelectedVillageIndex < 0 )
	{
		return;
	}

	//	[종호]	NTradetabDlg2->m_pNPTrade2Combo->GetText 를 할때 가져올 Data가 없으면 클라이언트가 죽는다.
	if(NTradetabDlg2->m_pNPTrade2Combo->GetItemNum() <= 0)
		return;

	TCHAR VillageName[128] = TEXT("");
	StringCchCopy(VillageName, 128, (NTradetabDlg2->m_pNPTrade2Combo)->GetText( siSelectedVillageIndex) );			
	
	if ( VillageName == NULL || _tcslen( VillageName ) < 1 )
	{
		return;
	}
	
	for ( SI16 i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
	{
		if ( m_siStockKindList[i] > 0 && m_siStockNumList[i] > 0 )
		{
			TCHAR *VillageTempName = pclClient->pclVillageManager->GetName( m_siStockKindList[i] );
			if ( VillageTempName != NULL && _tcscmp( VillageName, VillageTempName ) == 0 )
			{			
				break;
			}
		}
	}

	// 같은 주식 못 찾았음.
	if ( i == MAX_STOCK_KIND_IN_ACOUNT )
	{
		return;
	}

	m_siStockVillageUnique = m_siStockKindList[i];
	
	
	if ( m_siStockNum < 1 )
	{
		return;
	}

	if ( m_siStockNum > pclClient->GetGlobalValue("GV_PrivateTradeStockLimit") )
	{
		TCHAR* pTitle = GetTxtFromMgr(2352);
		TCHAR* pText = GetTxtFromMgr(2353);
		
		//[수정 : 황진성 2008. 1. 24 => 유저 거래시 주식이 1000주 이상등록되어 거래되면 윈도우 창이 뜨는거 수정.]
		cltClient *pclclient = (cltClient *)pclClient;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		//MessageBox( NULL, pTitle, pText, MB_OK|MB_TOPMOST );
		return;
	}

	stPrivateTrade_Stock sStock
		(
		m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		m_siStockVillageUnique,
		m_siStockNum
		);

	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sStock, sStock.sHeader.uiSize );
	}

	return;
}


bool CNPrivateTradeDlg::IsShow()
{
	return m_bShow;
}



void CNPrivateTradeDlg::ClearPrivateTradeList()
{
	if ( m_pEachPersonTradeOrderInfo != NULL )
	{
		m_pEachPersonTradeOrderInfo->Initialize();

		m_pNPTradeList4->Clear();
	    m_pNPTradeList6->Clear(); 
		//ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_OTHER ) );
		//ListView_DeleteAllItems( GetDlgItem( m_hDlg, IDC_LIST_MY ) );
	}

	return;
}



void CNPrivateTradeDlg::PrivateTradeOtherListReload()
{
	
	TCHAR Buffer[256 ] = TEXT("");
	SI16 siCount = 0;
	
	 m_pNPTradeList4->Clear();
	
	//m_pNPTradeList4->SetDrawLine( true );
	m_pNPTradeList4->Refresh();

//내가 보낼물품
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();
	
	if ( pTradeData->m_siPrice > 0 )
	{
		
		TCHAR* pText = GetTxtFromMgr(611);
		g_JYLibFunction.SetNumUnit(pTradeData->m_siPrice,Buffer, 256, pText);
		ptradeListBoxItemData.Init();									
		ptradeListBoxItemData.Set( 0, Buffer, ((cltClient*)pclClient)->GetColorByPrice( pTradeData->m_siPrice ) );	
		ptradeListBoxItemData.siParam[0] =0;
		ptradeListBoxItemData.siSmallIconFileIndex[0] = 2;
		ptradeListBoxItemData.siSmallIconFontIndex[0] = 4;
		SI32 index = m_pNPTradeList4->AddItem( &ptradeListBoxItemData );		
		
	}
	TCHAR ItemName[ 128 ] = TEXT("");
	for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		m_clItem[i] = NTradetabDlg1->m_clUserItem[i];
		if ( pTradeData->m_clItem[ i ].siUnique > 0 )
		{
			if ( pTradeData->m_clItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) )
			{
			
				if ( m_siSelectedModeIndex == PRIVATETRADE_TRADE_MODE )
				{
					TCHAR* pText = GetTxtFromMgr(2339);
					StringCchPrintf( Buffer, 256, pText, ItemName, pTradeData->m_clItem[ i ].GetItemNum() );
				}
				else
				{
					TCHAR* pText = GetTxtFromMgr(2340);
					StringCchPrintf( Buffer, 256, pText, ItemName );
				}
				

				if ( !_tcscmp(Buffer,TEXT("")) ) continue;
				ptradeListBoxItemData.Init();									
				ptradeListBoxItemData.Set(0,Buffer);	
				ptradeListBoxItemData.siParam[0] =i;
				ptradeListBoxItemData.siSmallIconFileIndex[0] = 2;
				ptradeListBoxItemData.siSmallIconFontIndex[0] = 5;
				SI32 index = m_pNPTradeList4->AddItem( &ptradeListBoxItemData );					
			
			}
		}
	}

	if ( pTradeData->m_siStockVillageUnique > 0 && pTradeData->m_siStockNum )
	{
		TCHAR *VillageName = pclClient->pclVillageManager->GetName( pTradeData->m_siStockVillageUnique );

		if ( VillageName != NULL )
		{
			//Li.iImage = 2;
			//Li.iItem = siCount++;

			TCHAR* pText = GetTxtFromMgr(2341);
			StringCchPrintf( Buffer, 256, pText, VillageName, pTradeData->m_siStockNum );
			//if ( !_tcscmp(Buffer, TEXT("")) ) continue;
			ptradeListBoxItemData.Init();									
			ptradeListBoxItemData.Set(0,Buffer);	
			ptradeListBoxItemData.siParam[0] =i;
			ptradeListBoxItemData.siSmallIconFileIndex[0] = 2;
			ptradeListBoxItemData.siSmallIconFontIndex[0] = 6;
			SI32 index = m_pNPTradeList4->AddItem( &ptradeListBoxItemData );	
			
			//Li.pszText = Buffer;
			//SendMessage( hOtherList, LVM_INSERTITEM, 0, (LPARAM)&Li );
		}
	}

	if ( pTradeData->m_siHouseVillageUnique > 0 )
	{
		TCHAR *VillageName = pclClient->pclVillageManager->GetName( pTradeData->m_siHouseVillageUnique );

		if ( VillageName != NULL )
		{
			//Li.iImage = 4;
			//Li.iItem = siCount++;

			TCHAR* pText = GetTxtFromMgr(2342);
			StringCchPrintf( Buffer, 256, pText, VillageName, pTradeData->m_siHouseUnitSlot + 1 );
					
			//if ( !_tcscmp(Buffer, TEXT("")) ) continue;
			ptradeListBoxItemData.Init();									
			ptradeListBoxItemData.Set(0,Buffer);	
			ptradeListBoxItemData.siParam[0] =i;
			ptradeListBoxItemData.siSmallIconFileIndex[0] = 2;
			ptradeListBoxItemData.siSmallIconFontIndex[0] = 8;
			SI32 index = m_pNPTradeList4->AddItem( &ptradeListBoxItemData );	
			//Li.pszText = Buffer;
			//SendMessage( hOtherList, LVM_INSERTITEM, 0, (LPARAM)&Li );
		}
	}

	if ( pTradeData->m_siHorseIndex != -1 )
	{
		//Li.iImage = 3;
		//Li.iItem = siCount++;

		if ( pTradeData->m_clTradeHorse.szName != NULL )
		{
			StringCchPrintf( Buffer, 256, TEXT("%s"), pTradeData->m_clTradeHorse.szName );
			
			//if ( !_tcscmp(Buffer, TEXT("")) ) continue;
			ptradeListBoxItemData.Init();									
			ptradeListBoxItemData.Set(0,Buffer);	
			ptradeListBoxItemData.siParam[0] =i;
			ptradeListBoxItemData.siSmallIconFileIndex[0] = 2;
			ptradeListBoxItemData.siSmallIconFontIndex[0] = 7;
			SI32 index = m_pNPTradeList4->AddItem( &ptradeListBoxItemData );	
		
		}
	}
	//m_pNPTradeEdit5->SetText(TEXT(" "));
	m_pNPTradeList4->Refresh();
	


}

void CNPrivateTradeDlg::PrivateTradeMyListReload()
{
	
	m_pNPTradeList6->Clear();

	TCHAR Buffer[256 ] =TEXT("");

		
	SI16 siCount = 0;
	
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();

	if ( pTradeData->m_siPrice > 0 )
	{
		
		TCHAR* pText = GetTxtFromMgr(611);
		g_JYLibFunction.SetNumUnit(pTradeData->m_siPrice,Buffer, 256, pText);
		ptradeListBoxItemData.Init();									
		ptradeListBoxItemData.Set(0, Buffer, ((cltClient*)pclClient)->GetColorByPrice( pTradeData->m_siPrice ) );	
		ptradeListBoxItemData.siParam[0] =0;
		ptradeListBoxItemData.siSmallIconFileIndex[0] = 2;
		ptradeListBoxItemData.siSmallIconFontIndex[0] = 4;
	
		SI32 index = m_pNPTradeList6->AddItem( &ptradeListBoxItemData  );
		
	}

	TCHAR ItemName[ 128 ] = TEXT("");
	for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		if ( pTradeData->m_clItem[ i ].siUnique > 0 )
		{
			if ( pTradeData->m_clItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) )
			{
				//Li.iImage = 1;
				//Li.iItem = siCount++;

				if ( m_siSelectedModeIndex == PRIVATETRADE_TRADE_MODE )
				{
					TCHAR* pText = GetTxtFromMgr(2339);
					StringCchPrintf( Buffer, 256, pText, ItemName, pTradeData->m_clItem[ i ].GetItemNum() );
				}
				else
				{
					TCHAR* pText = GetTxtFromMgr(2340);
					StringCchPrintf( Buffer, 256, pText, ItemName );
				}

				if (!_tcscmp(Buffer,TEXT("")) ) continue;
				ptradeListBoxItemData.Init();									
				ptradeListBoxItemData.Set(0,Buffer);	
				ptradeListBoxItemData.siParam[0] =i;
				ptradeListBoxItemData.siSmallIconFileIndex[0] = 2;
				ptradeListBoxItemData.siSmallIconFontIndex[0] = 5;
			
				SI32 index = m_pNPTradeList6->AddItem( &ptradeListBoxItemData  );
		
			}
		}
	}//end for 

	

	if ( pTradeData->m_siStockVillageUnique > 0 && pTradeData->m_siStockNum )
	{
		TCHAR *VillageName = pclClient->pclVillageManager->GetName( pTradeData->m_siStockVillageUnique );

		if ( VillageName != NULL )
		{
			//Li.iImage = 2;
			//Li.iItem = siCount++;

			TCHAR* pText = GetTxtFromMgr(2341);
			StringCchPrintf( Buffer, 256, pText, VillageName, pTradeData->m_siStockNum );
			
			ptradeListBoxItemData.Init();
			ptradeListBoxItemData.Set(0,Buffer);	
			ptradeListBoxItemData.siParam[0] = i;
			ptradeListBoxItemData.siSmallIconFileIndex[0] = 2;
			ptradeListBoxItemData.siSmallIconFontIndex[0] = 6;
			
			SI32 index = m_pNPTradeList6->AddItem( &ptradeListBoxItemData  );
		}
	}

	if ( pTradeData->m_siHouseVillageUnique > 0 )
	{
		TCHAR *VillageName = pclClient->pclVillageManager->GetName( pTradeData->m_siHouseVillageUnique );

		if ( VillageName != NULL )
		{
			//Li.iImage = 4;
			//Li.iItem = siCount++;
		
			TCHAR* pText = GetTxtFromMgr(2342);
			StringCchPrintf( Buffer, 256, pText, VillageName, pTradeData->m_siHouseUnitSlot + 1 );
			ptradeListBoxItemData.Init();									
			ptradeListBoxItemData.Set(0,Buffer);	
			ptradeListBoxItemData.siParam[0] = i;
			ptradeListBoxItemData.siSmallIconFileIndex[0] = 2;
			ptradeListBoxItemData.siSmallIconFontIndex[0] = 8;
			
			SI32 index = m_pNPTradeList6->AddItem( &ptradeListBoxItemData  );
		
		}
	}

	if ( pTradeData->m_siHorseIndex != -1 )
	{
		//Li.iImage = 3;
		//Li.iItem = siCount++;

		if ( pTradeData->m_clTradeHorse.szName != NULL )
		{
			StringCchPrintf( Buffer, 256, TEXT("%s"), pTradeData->m_clTradeHorse.szName );
			ptradeListBoxItemData.Init();									
			ptradeListBoxItemData.Set(0,Buffer);	
			ptradeListBoxItemData.siParam[0] = i;
			ptradeListBoxItemData.siSmallIconFileIndex[0] = 2;
			ptradeListBoxItemData.siSmallIconFontIndex[0] = 7;
			
			SI32 index = m_pNPTradeList6->AddItem( &ptradeListBoxItemData  );
			
		}
	}

	m_pNPTradeList6->Refresh();
	m_pNPTradeEdit7->SetText(TEXT(" "));
	//SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_MY_EXPLAIN ), TEXT("") );
	
	return;
}

void CNPrivateTradeDlg::PrivateTradeOtherListInfo( SI32 SelectedListIndex )
{
	
	//HWND hOtherListInfoEdit = GetDlgItem( m_hDlg, IDC_EDIT_OTHER_EXPLAIN );

	SI16 siCount = 0;

	TCHAR Buffer[ 1024 ] = TEXT("");

	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();

	if ( pTradeData->m_siPrice > 0 )
	{
		if ( siCount == SelectedListIndex )
		{
			TCHAR moneybuf[512] = TEXT("");
			g_JYLibFunction.SetNumUnit(pTradeData->m_siPrice,moneybuf, 512);
			TCHAR* pText = GetTxtFromMgr(2343);
			StringCchPrintf( Buffer, 1024, pText, moneybuf );
			m_pNPTradeEdit5->SetText(Buffer, ((cltClient*)pclClient)->GetColorByPrice( pTradeData->m_siPrice ));
			//SetWindowText( hOtherListInfoEdit, Buffer );
			
			//SendDlgItemMessage( m_hDlg, IDC_STATIC_PRIVATETRADE_OTHER_IMGAE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hMoneyBitmap );

			return;
		}

		++siCount;
	}

	TCHAR ItemName[ 128 ] = TEXT("");
	TCHAR ItemExplain[ 512 ] = TEXT("");
	for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		if ( pTradeData->m_clItem[ i ].siUnique > 0 )
		{
			if ( pTradeData->m_clItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) )
			{				
				if ( siCount == SelectedListIndex )
				{
					if ( pclClient->pclItemManager->GetExplain( &pTradeData->m_clItem[ i ], ItemExplain ) == TRUE )
					{
						StringCchPrintf( Buffer, 1024, TEXT("%s"), ItemExplain );
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr(2344);
						StringCchPrintf( Buffer, 1024, pText, ItemName, pTradeData->m_clItem[ i ].siItemNum );
					}				

					 //ptrade_otherListBoxItemData
					m_pNPTradeEdit5->SetText(Buffer);
					//SetWindowText( hOtherListInfoEdit, Buffer );
					
					
					return;
				}

				++siCount;
			}
		}
	}
	

	if ( pTradeData->m_siStockVillageUnique > 0 && pTradeData->m_siStockNum )
	{
		TCHAR *VillageName = pclClient->pclVillageManager->GetName( pTradeData->m_siStockVillageUnique );

		if ( VillageName != NULL )
		{
			if ( siCount == SelectedListIndex )
			{
				TCHAR* pText = GetTxtFromMgr(2345);
				StringCchPrintf( Buffer, 1024, pText, VillageName, pTradeData->m_siStockNum );
				m_pNPTradeEdit5->SetText(Buffer);
				//SetWindowText( hOtherListInfoEdit, Buffer );
				

				return;
			}
			
			siCount++;
		}
	}

	if ( pTradeData->m_siHouseVillageUnique > 0 )
	{
		if ( siCount == SelectedListIndex )
		{
			pTradeData->m_clRentContract.GetRentInfoText( Buffer, 1024 );
			m_pNPTradeEdit5->SetText(Buffer);
			//SetWindowText( hOtherListInfoEdit, Buffer );
			return;
		}

		++siCount;
	}


	if ( pTradeData->m_siHorseIndex != -1 )
	{
		if ( pclClient->pclHorseManager->GetText( &pTradeData->m_clTradeHorse, Buffer, 1024 ) )
		{
			if ( siCount == SelectedListIndex )
			{
				m_pNPTradeEdit5->SetText(Buffer);			
				return;
			}
			
			++siCount;
		}
	}

	m_pNPTradeList4->Refresh();

	return;
}

void CNPrivateTradeDlg::PrivateTradeMyListInfo( SI32 SelectedListIndex )
{
	
	TCHAR Buffer[ 1024 ] = TEXT("");

	SI16 siCount = 0;
	
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();

	if ( pTradeData->m_siPrice > 0 )
	{
		TCHAR moneybuf[512] = TEXT("") ;		
		g_JYLibFunction.SetNumUnit(pTradeData->m_siPrice,moneybuf, 512);
		TCHAR* pText = GetTxtFromMgr(2343);
		StringCchPrintf( Buffer, 1024, pText, moneybuf );
		m_pNPTradeEdit7->SetText(Buffer, ((cltClient*)pclClient)->GetColorByPrice( pTradeData->m_siPrice ));	
		++siCount;
	}

	TCHAR ItemName[ 128 ] = TEXT("");
	TCHAR ItemExplain[ 512 ] = TEXT("");
	for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		if ( pTradeData->m_clItem[ i ].siUnique > 0 )
		{
			if ( pTradeData->m_clItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) )
			{	
				if ( siCount == SelectedListIndex )
				{
					if ( pclClient->pclItemManager->GetExplain( &pTradeData->m_clItem[ i ], ItemExplain ) == TRUE )
					{
						StringCchPrintf( Buffer, 1024, TEXT("%s"), ItemExplain );
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr(2344);
						StringCchPrintf( Buffer, 1024, pText, ItemName, pTradeData->m_clItem[ i ].siItemNum );
					}
					m_pNPTradeEdit7->SetText(Buffer);			
					
					return;
				}

				++siCount;
			}
		}
	}

	if ( pTradeData->m_siStockVillageUnique > 0 && pTradeData->m_siStockNum )
	{
		TCHAR *VillageName = pclClient->pclVillageManager->GetName( pTradeData->m_siStockVillageUnique );

		if ( VillageName != NULL )
		{
			if ( siCount == SelectedListIndex )
			{
				TCHAR* pText = GetTxtFromMgr(2345);
				StringCchPrintf( Buffer, 1024, pText, VillageName, pTradeData->m_siStockNum );
				m_pNPTradeEdit7->SetText(Buffer);		

				return;
			}
			
			++siCount;
		}
	}

	if ( pTradeData->m_siHouseVillageUnique > 0 )
	{
		if ( siCount == SelectedListIndex )
		{
			pTradeData->m_clRentContract.GetRentInfoText( Buffer , 1024 );
			m_pNPTradeEdit7->SetText(Buffer);		
			return;
		}

		++siCount;
	}


	if ( pTradeData->m_siHorseIndex != -1 )
	{
		if ( pclClient->pclHorseManager->GetText( &pTradeData->m_clTradeHorse, Buffer, 1024 ) )
		{
			if ( siCount == SelectedListIndex )
			{
				m_pNPTradeEdit7->SetText(Buffer);		
				return;
			}
			
			++siCount;
		}
	}	

	return;
}

void CNPrivateTradeDlg::PrivateTradeAddItem( SI32 siCharUnique, SI32 siAddedIndex, cltItem *pclItem )
{
	
	if ( m_siOtherCharUnique == siCharUnique )		// 상대방이 목록에 올렸음
	{	
		//m_uiLastAcceptFrame = pclClient->GetFrame();
		m_siLastAcceptTime = pclClient->CurrentClock;// 프레임대신 커런트 클럭을 저장해서
		m_pNPTradeButton9->Enable(false);
		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();
		pTradeData->m_clItem[ siAddedIndex ].Set( pclItem );
		
		PrivateTradeOtherListReload();

		TCHAR ItemExplain[ 512 ] = TEXT("");
		if ( pclClient->pclItemManager->GetExplain( pclItem, ItemExplain ) == TRUE )
		{			
			m_pNPTradeEdit5->SetText(ItemExplain);			
		}

		return;
	}
	else											// 내가 내 목록에 올렸음.
	{		
		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeButton9->Enable(false);		

		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
		pTradeData->m_clItem[ siAddedIndex ].Set( pclItem );		

		PrivateTradeMyListReload();

		TCHAR ItemExplain[ 512 ] = TEXT("");
		if ( pclClient->pclItemManager->GetExplain( pclItem, ItemExplain ) == TRUE )
		{			
			m_pNPTradeEdit7->SetText(ItemExplain);			
		}

		return;
	}

	return;
}

void CNPrivateTradeDlg::PrivateTradeDelList( SI32 CharUnique, SI16 DelType, SI16 Param )
{
	
	if ( m_siOtherCharUnique == CharUnique )		// 상대방이 목록에 내렸음
	{
		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeButton9->Enable(false);	

		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();

		switch( DelType )
		{
		case PRIVATETRADE_TYPE_MONEY:
			pTradeData->m_siPrice = 0;
			break;
		case PRIVATETRADE_TYPE_ITEM:
			if ( pTradeData->m_clItem[ Param ].siUnique != 0 )
			{
				memset( &pTradeData->m_clItem[ Param ], 0, sizeof( cltItem ) );			
			}
			break;
		case PRIVATETRADE_TYPE_STOCK:
			pTradeData->m_siStockVillageUnique = 0;
			pTradeData->m_siStockNum = 0;
			break;
		case PRIVATETRADE_TYPE_HOUSEUNIT:
			pTradeData->m_siHouseVillageUnique = 0;
			pTradeData->m_siHouseUnitSlot = 0;
			break;
		case PRIVATETRADE_TYPE_HORSE:
			pTradeData->m_siHorseIndex = -1;
			pTradeData->m_clTradeHorse.Init();
			break;
		}

		PrivateTradeOtherListReload();
	}
	else
	{
		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeButton9->Enable(false);
	

		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
	
		switch( DelType )
		{
		case PRIVATETRADE_TYPE_MONEY:
			pTradeData->m_siPrice = 0;
			break;
		case PRIVATETRADE_TYPE_ITEM:
			if ( pTradeData->m_clItem[ Param ].siUnique != 0 )
			{
				memset( &pTradeData->m_clItem[ Param ], 0, sizeof( cltItem ) );			
			}
			break;
		case PRIVATETRADE_TYPE_STOCK:
			pTradeData->m_siStockVillageUnique = 0;
			pTradeData->m_siStockNum = 0;
			break;
		case PRIVATETRADE_TYPE_HOUSEUNIT:
			pTradeData->m_siHouseVillageUnique = 0;
			pTradeData->m_siHouseUnitSlot = 0;
			break;
		case PRIVATETRADE_TYPE_HORSE:
			pTradeData->m_siHorseIndex = -1;
			pTradeData->m_clTradeHorse.Init();
			break;
		}
		
		PrivateTradeMyListReload();
	}
	
	return;
}

void CNPrivateTradeDlg::PrivateTradeDelItem( SI32 siCharUnique, SI16 siDelIndex )
{
	if ( m_siOtherCharUnique == siCharUnique )		// 상대방이 목록에 내렸음
	{
		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeButton9->Enable(false);
	
		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();
		
		if ( pTradeData->m_clItem[ siDelIndex ].siUnique != 0 )
		{
			memset( &pTradeData->m_clItem[ siDelIndex ], 0, sizeof( cltItem ) );			
		}

		//SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_PRIVATETRADE_THEOTHER_ITEM, NULL, pTradeData->m_clItem, MAX_PRIVATETRADEORDER_ITEM_NUM, true );
		PrivateTradeOtherListReload();
		return;
	}
	else											// 내가 내 목록에서 내렸음.
	{
		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeButton9->Enable(false);
	
		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
		
		if ( pTradeData->m_clItem[ siDelIndex ].siUnique != 0 )
		{
			memset( &pTradeData->m_clItem[ siDelIndex ], 0, sizeof( cltItem ) );
		}

		//SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_PRIVATETRADE_MY_ITEM, NULL, pTradeData->m_clItem, MAX_PRIVATETRADEORDER_ITEM_NUM, true );
		PrivateTradeMyListReload();
		return;
	}

	return;
}

void CNPrivateTradeDlg::PrivateTradeAddPrice( SI32 siCharUnique, GMONEY siPrice )
{
	/*
	if ( m_siSelectedModeIndex == PRIVATETRADE_ENCHANT_MODE )
	{
		TCHAR* pText = GetTxtFromMgr(2346);
		m_pNPTradeEdit->SetText(pText);	
	}
	*/

	if ( m_siOtherCharUnique == siCharUnique )		// 상대방이 돈 설정했음.
	{
		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeButton9->Enable(false);
	
		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();
		pTradeData->m_siPrice = siPrice;

		//SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_PRIVATETRADE_OTHER_PRICE ), buffer );
		PrivateTradeOtherListReload();
	}
	else											// 내가 돈 설정 했음.
	{
		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeEdit->Enable(false);
		
		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
		pTradeData->m_siPrice = siPrice;

		//SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_PRIVATETRADE_MY_PRICE ), buffer );
		PrivateTradeMyListReload();
	}
}

void CNPrivateTradeDlg::PrivateTradeAddStock( SI32 siCharUnique, SI32 siStockVillageUnique, SI32 siStockNum )
{
	
	if ( m_siSelectedModeIndex == PRIVATETRADE_ENCHANT_MODE )
	{
		return;
	}

	if ( m_siOtherCharUnique == siCharUnique )		// 상대방이 주식 설정했음.
	{
		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeButton9->Enable(false);		

		TCHAR *VillageName = pclClient->pclVillageManager->GetName( siStockVillageUnique );

		if ( VillageName != NULL )
		{
			stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();
			pTradeData->m_siStockVillageUnique = siStockVillageUnique;
			pTradeData->m_siStockNum = siStockNum;
		
			PrivateTradeOtherListReload();
		}
	}
	else											// 내가 주식 설정 했음.
	{
		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeButton9->Enable(false);		

		TCHAR *VillageName = pclClient->pclVillageManager->GetName( siStockVillageUnique );

		if ( VillageName != NULL )
		{
			stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
			pTradeData->m_siStockVillageUnique = siStockVillageUnique;
			pTradeData->m_siStockNum = siStockNum;
			
			PrivateTradeMyListReload();
		}
	}

	return;
}

void CNPrivateTradeDlg::PrivateTradeAddHouseUnit( SI32 siCharUnique, SI32 HouseVillageUnique, SI32 HouseUnitSlot, cltRentContract *pRentContract )
{
	
	if ( m_siSelectedModeIndex == PRIVATETRADE_ENCHANT_MODE )
	{
		return;
	}

	TCHAR Buffer[ 1024 ] = TEXT("");

	if ( m_siOtherCharUnique == siCharUnique )		// 상대방이 말 설정했음.
	{
		//HWND hOtherListInfoEdit = GetDlgItem( m_hDlg, IDC_EDIT_OTHER_EXPLAIN );

		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeButton9->Enable(false);
		

		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();
		pTradeData->m_siHouseVillageUnique = HouseVillageUnique;
		pTradeData->m_siHouseUnitSlot = HouseUnitSlot;
		pTradeData->m_clRentContract.Set( pRentContract );

		pTradeData->m_clRentContract.GetRentInfoText( Buffer , 1024);
		//SetWindowText( hOtherListInfoEdit, Buffer );

		PrivateTradeOtherListReload();
	}
	else
	{
		//HWND hMyListInfoEdit = GetDlgItem( m_hDlg, IDC_EDIT_MY_EXPLAIN );

		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeButton9->Enable(false);
		

		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
		pTradeData->m_siHouseVillageUnique = HouseVillageUnique;
		pTradeData->m_siHouseUnitSlot = HouseUnitSlot;
		pTradeData->m_clRentContract.Set( pRentContract );

		pTradeData->m_clRentContract.GetRentInfoText( Buffer , 1024);
		//SetWindowText( hMyListInfoEdit, Buffer );

		PrivateTradeMyListReload();
	}
	
}


void CNPrivateTradeDlg::PrivateTradeAddHorse( SI32 siCharUnique, SI16 HorseIndex, cltHorse *pclHorse )
{
	if ( m_siSelectedModeIndex == PRIVATETRADE_ENCHANT_MODE )
	{
		return;
	}

	TCHAR Buffer[ 512 ] = TEXT("");

	if ( m_siOtherCharUnique == siCharUnique )		// 상대방이 말 설정했음.
	{
		//HWND hOtherListInfoEdit = GetDlgItem( m_hDlg, IDC_EDIT_OTHER_EXPLAIN );

		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeButton9->Enable(false);
		

		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();
		pTradeData->m_clTradeHorse.Set( pclHorse );
		pTradeData->m_siHorseIndex = HorseIndex;

		PrivateTradeOtherListReload();

		if ( pclClient->pclHorseManager->GetText( pclHorse, Buffer, 512 ) )
		{		
			m_pNPTradeEdit5->SetText(Buffer);
		}
	}
	else
	{
		//HWND hMyListInfoEdit = GetDlgItem( m_hDlg, IDC_EDIT_MY_EXPLAIN );

		/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
		m_siLastAcceptTime = pclClient->CurrentClock;
		m_pNPTradeButton9->Enable(false);
	

		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
		pTradeData->m_clTradeHorse.Set( pclHorse );
		pTradeData->m_siHorseIndex = HorseIndex;

		PrivateTradeMyListReload();
		
		if ( pclClient->pclHorseManager->GetText( pclHorse, Buffer, 512 ) )
		{			
			m_pNPTradeEdit7->SetText(Buffer);
		}
	}

	return;
}

void CNPrivateTradeDlg::PrivateTradeMyAccept()
{
	++m_siAccpetCounter;
	m_bMyAccept = true;	
	
	//m_uiLastAcceptFrame = 0;
	m_siLastAcceptTime	=	0	;
	m_pNPTradeButton9->Enable(false);
	TCHAR* pText = GetTxtFromMgr(3902);
	m_pNPTradeButton9->SetText(pText, DT_CENTER|DT_VCENTER|DT_SINGLELINE);	

}

void CNPrivateTradeDlg::PrivateTradeMyCancel()
{
	++m_siAccpetCounter;
	m_bMyAccept = false;

	
	/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
	m_siLastAcceptTime = pclClient->CurrentClock;
	m_pNPTradeButton9->Enable(false);


	TCHAR* pText = GetTxtFromMgr(3897);
	m_pNPTradeButton9->SetText(pText, DT_CENTER|DT_VCENTER|DT_SINGLELINE );	
	if ( m_siSelectedModeIndex == PRIVATETRADE_TRADE_MODE )
	{
		//TCHAR* pText = GetTxtFromMgr(2336);
		m_pNPTradeStatic9->SetText(pText,DT_WORDBREAK);		
	}
	/*
	else if ( m_siSelectedModeIndex == PRIVATETRADE_REPAIR_MODE )
	{
		TCHAR* pText = GetTxtFromMgr(2336);
		//SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_OTHER_ACCEPT ), pText );
	}
	*/
	
}

void CNPrivateTradeDlg::PrivateTradeOtherAccept( SI16 siResult )
{
	/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
	m_siLastAcceptTime = pclClient->CurrentClock;
	m_pNPTradeButton9->Enable(false);

	++m_siAccpetCounter;

	if ( m_siSelectedModeIndex == PRIVATETRADE_TRADE_MODE )
	{
		TCHAR* pText = GetTxtFromMgr(2347);
		m_pNPTradeStatic9->SetText(pText,DT_WORDBREAK);
		//SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_OTHER_ACCEPT ), pText );
	}
	/*
	else if ( m_siSelectedModeIndex == PRIVATETRADE_REPAIR_MODE )
	{
		TCHAR* pText = GetTxtFromMgr(2347);
		//SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_OTHER_ACCEPT ), pText );
	}
	*/
	
	
	

	return;
}

void CNPrivateTradeDlg::PrivateTradeOtherCancel()
{
	/*m_uiLastAcceptFrame = pclClient->GetFrame();*/
	m_siLastAcceptTime = pclClient->CurrentClock;
	m_pNPTradeButton9->Enable(false);

	++m_siAccpetCounter;

	if ( m_siSelectedModeIndex == PRIVATETRADE_TRADE_MODE )
	{
		//TCHAR* pText = GetTxtFromMgr(2336);
		//m_pNPTradeStatic9->SetText(pText,DT_WORDBREAK);
		//SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_OTHER_ACCEPT ), pText );
	}
	/*
	else if ( m_siSelectedModeIndex == PRIVATETRADE_REPAIR_MODE )
	{
		TCHAR* pText = GetTxtFromMgr(2336);
		//SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_OTHER_ACCEPT ), pText );
	}
	*/

	

	m_bMyAccept = false;
	m_pNPTradeButton9->Enable(true);
	TCHAR* pText = GetTxtFromMgr(3412);
	m_pNPTradeButton9->SetText(pText, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CNPrivateTradeDlg::PrivateTradeEnd( SI32 siResult, 
									   SI32 RecvStockVillageUnique, SI32 RecvStockNum, 
									   SI32 SendStockVillageUnique, SI32 SendStockNum, 
									   SI32 RecvHouseVillageUnique, SI32 RecvHouseUnitSlot, 
									   cltRentContract *pclRecvRentContract,
									   SI32 SendHouseVillageUnique, SI32 SendHouseUnitSlot, 
									   SI16 HorseIndex, cltHorse *pclHorse )
{
	if ( siResult == 1 )
	{
		if ( RecvStockVillageUnique > 0 && RecvStockNum > 0 )
		{
			SI16 EmptyIndex = -1;
			for ( SI16 i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
			{
				if ( pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique > 0 && pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique == RecvStockVillageUnique )
				{
					pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siAmount = RecvStockNum;
					break;
				}
				else if ( pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique == 0 && EmptyIndex == -1 )
				{
					EmptyIndex = i;
				}
			}
			
			// 이 경우는 주식 추가지...
			if ( i == MAX_STOCK_KIND_IN_ACOUNT && EmptyIndex > -1 && EmptyIndex < MAX_STOCK_KIND_IN_ACOUNT )
			{
				pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[EmptyIndex].siVillageUnique = RecvStockVillageUnique;
				pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[EmptyIndex].siAmount = RecvStockNum;
			}
		}
		
		if ( SendStockVillageUnique > 0 )
		{
			for ( SI16 i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
			{
				if ( pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique > 0 && pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique == SendStockVillageUnique )
				{
					if ( SendStockNum > 0 )
					{
						pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siAmount = SendStockNum;
					}
					else
					{
						pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siVillageUnique = 0;
						pclClient->pclCM->CR[1]->pclCI->clStock.clInfo.clUnit[i].siAmount = 0;
					}
					break;
				}
			}
		}
		
		if ( SendHouseVillageUnique > 0 )
		{
			pclClient->pclCM->CR[1]->pclCI->clRealEstate.Delete( REALESTATE_TYPE_HOUSE, SendHouseVillageUnique, SendHouseUnitSlot );
		}

		if ( RecvHouseVillageUnique > 0 )
		{
			pclClient->pclCM->CR[1]->pclCI->clRealEstate.Add( REALESTATE_TYPE_HOUSE, RecvHouseVillageUnique, RecvHouseUnitSlot, pclRecvRentContract );
		}

		// by LEEKH - 말이 거래되었다면, 말 변경을 위해 따라다니는 말을 없앤다.
		if ( HorseIndex >= 0 )
		{
			pclClient->pclCM->CR[1]->DeleteSlave();

			pclClient->pclCM->CR[1]->pclCI->clHorseInfo.SetCurrentShowHorseIndex( HorseIndex );
			cltHorse* pclhorse = pclClient->pclCM->CR[1]->pclCI->clHorseInfo.GetCurrentShowHorse();
			
			pclhorse->Set( pclHorse );
		}

		//cltHorse* pclhorse = pclClient->pclCM->CR[1]->pclCI->clHorseInfo.GetCurrentShowHorse();
//KHY - 1025 - 일반 거래시 말에서 내리는 문제 해결.
/*		if ( HorseIndex != -1 )
		{
			// 말에서 내린 다음에, 초기화 해줌 ( 캐릭터 조합에서 에러가 날 수 있으므로... )
			//pclClient->pclCM->CR[1]->UnrideHorse();
			//pclhorse->Init();
			pclClient->pclCM->CR[1]->DeleteSlave();
			
			// 말 재 셑팅
			pclhorse->Set( pclHorse );
		}
		else
*/
		// by LEEKH - 말은 업데이트 정보가 보내지므로 처리를 하지 않는다.
/*		{
			// 말 재 셑팅
			pclhorse->Set( pclHorse );
		}*/
	}	
	//Hide();
	DeleteDialog();
	return;
}

void CNPrivateTradeDlg::PushCancel()
{
	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stPrivateTrade_Cancel sCancel( m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex );

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sCancel, sCancel.sHeader.uiSize );
	}

	return;
}


bool CNPrivateTradeDlg::GetDelType( SI32 SelectedListIndex, SI16 *pType, SI16 *pParam )
{	
	cltClient *pclclient = (cltClient *)pclClient;

	SI16 siCount = 0;

	TCHAR Buffer[ 1024 ] = TEXT("");

	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();

	if ( pTradeData->m_siPrice > 0 )
	{
		if ( siCount == SelectedListIndex )
		{			
			*pType = PRIVATETRADE_TYPE_MONEY;
			
			TCHAR* pTitle = GetTxtFromMgr(2350);
			TCHAR* pText = GetTxtFromMgr(2351);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );

			return false;
			
			//return true;		//<--- 과거에는 돈을 지울 수 있었지만, 이제 안됨
		}

		++siCount;
	}

	TCHAR ItemName[ 128 ] = TEXT("");
	TCHAR ItemExplain[ 512 ] = TEXT("");
	for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		if ( pTradeData->m_clItem[ i ].siUnique > 0 )
		{
			if ( pTradeData->m_clItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) )
			{				
				if ( siCount == SelectedListIndex )
				{
					*pType = PRIVATETRADE_TYPE_ITEM;
					*pParam = i;
					return true;
				}
			}
			
			++siCount;
		}
	}

	if ( pTradeData->m_siStockVillageUnique > 0 && pTradeData->m_siStockNum )
	{
		TCHAR *VillageName = pclClient->pclVillageManager->GetName( pTradeData->m_siStockVillageUnique );

		if ( VillageName != NULL )
		{
			if ( siCount == SelectedListIndex )
			{
				*pType = PRIVATETRADE_TYPE_STOCK;
				return true;
			}
		}

		++siCount;
	}
	
	if ( pTradeData->m_siHouseVillageUnique > 0 )
	{
		TCHAR *VillageName = pclClient->pclVillageManager->GetName( pTradeData->m_siHouseVillageUnique );
		if ( VillageName != NULL )
		{
			if ( siCount == SelectedListIndex )
			{
				*pType = PRIVATETRADE_TYPE_HOUSEUNIT;
				return true;
			}			
		}
		
		++siCount;
	}


	if ( pTradeData->m_siHorseIndex != -1 )
	{
		if ( pTradeData->m_clTradeHorse.szName != NULL )
		{
			if ( siCount == SelectedListIndex )
			{
				*pType = PRIVATETRADE_TYPE_HORSE;
				return true;
			}
		}

		++siCount;
	}
	
	return false;
}

void CNPrivateTradeDlg::PushDelList()
{
	m_siMyPrivateTradeListSelectedIndex= m_pNPTradeList6->GetSelectedIndex();
	if ( m_siMyPrivateTradeListSelectedIndex < 0 )
	{
		return;
	}

	SI16 Type = 0;
	SI16 Param = 0;

	if ( ! GetDelType( m_siMyPrivateTradeListSelectedIndex, &Type, &Param ) )
	{
		return;
	}

	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();

	stPrivateTrade_Del_List sDelList
		(
		m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		Type,
		Param
		);

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sDelList, sDelList.sHeader.uiSize );
	}

	m_pNPTradeList6->Clear();
	m_pNPTradeList6->Refresh();
	m_pNPTradeEdit7->SetText(TEXT(" "));

	return;
}

void CNPrivateTradeDlg::PushDelItem()
{
	if ( m_siMyPrivateTradeListSelectedIndex < 0 )
	{
		return;
	}
	
	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();

	stPrivateTrade_Del_Item sDelItem
		(
		m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		m_siMyPrivateTradeListSelectedIndex,
		&pTradeData->m_clItem[ m_siMyPrivateTradeListSelectedIndex ]
		);

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sDelItem, sDelItem.sHeader.uiSize );
	}
	
	return;
}

void CNPrivateTradeDlg::PushHouseUnit()
{
	
	cltClient *pclclient = (cltClient *)pclClient;

	// 시전 사용 기본 레벨 조건을 충족시키지 못하면 실패. 
	if( pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < LEVELLIMIT_USE_HOUSE)
	{
		TCHAR* pTitle = GetTxtFromMgr(2354);
		TCHAR* pText = GetTxtFromMgr(2355);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, LEVELLIMIT_USE_HOUSE );
		return;
	}
	
	//군주
	

	SI32 siSelectedIndex = (NTradetabDlg4->m_pNPTrade4List)->GetSelectedIndex();
	if ( siSelectedIndex < 0 )
	{
		return;
	}

	SI32 Count = 0;

	SI16 i = 0;
	for ( ; i < MAX_REAL_ESTATE_NUMBER_PER_PERSON; ++i )
	{		
		if ( m_siHouseVillageUniqueList[ i ] > 0 )
		{
			TCHAR *VillageName = pclClient->pclVillageManager->GetName( m_siHouseVillageUniqueList[ i ] );
			if ( VillageName != NULL )
			{
				if ( Count == siSelectedIndex )
				{
					break;
				}
				++Count;
			}
		}
	}

	if ( i>= MAX_REAL_ESTATE_NUMBER_PER_PERSON )
	{
		return;
	}
	

	stPrivateTrade_HouseUnit sHouseUnit
		(
		m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		m_siHouseVillageUniqueList[ i ],
		m_siHouseUnitSlotList[ i ]
		);

	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sHouseUnit, sHouseUnit.sHeader.uiSize );
	}
	
}

void CNPrivateTradeDlg::PushHorse()
{
	cltClient *pclclient = (cltClient *)pclClient;

	cltHorse* pclhorse = pclClient->pclCM->CR[1]->pclCI->clHorseInfo.GetCurrentShowHorse();

	if ( pclhorse == NULL || pclhorse->siHorseUnique < 1 )
	{
		return;
	}
	
	if ( pclClient->pclCM->CR[1]->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == true )
	{
		TCHAR* pTitle = GetTxtFromMgr(2317);
		TCHAR* pText = GetTxtFromMgr(2356);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		return;
	}

	// 말 인벤토리에 아이템이 들어 있는지 체크
	if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.IsHorseHaveItem() == true )
	{
		TCHAR* pTitle = GetTxtFromMgr(2132);
		TCHAR* pText = GetTxtFromMgr(2133);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		return;
	}
	

	stPrivateTrade_Horse sHorse
		(
		m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		pclClient->pclCM->CR[1]->pclCI->clHorseInfo.siCurrentShowHorse
		);

	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sHorse, sHorse.sHeader.uiSize );
	}	
}

void CNPrivateTradeDlg::PushPrice()
{
	cltClient *pclclient = (cltClient *)pclClient;
	NTCHARString512	kRawPrice( (NTradetabDlg1->m_pNPTrade1Edit2)->GetText() );
	kRawPrice.ReplaceAll(",", "");	// , 삭제

	GMONEY m_siPrice = _tstoi64(kRawPrice);
	

	if ( m_siPrice < 1 )
	{
		return;
	}

	if ( m_siPrice > pclClient->GetGlobalValue("GV_PrivateTradeMoneyLimit") )
	{
		TCHAR Buffer[256 ] = TEXT("");
		TCHAR* pText = GetTxtFromMgr(2357);
		StringCchPrintf( Buffer, 256, pText, pclClient->GetGlobalValue("GV_PrivateTradeMoneyLimit") );
		TCHAR* pTitle = GetTxtFromMgr(2358);
		if(pclclient->m_pDialog[ NOTIFYMSG_DLG ]==NULL)
			pclclient->CreateInterface(NOTIFYMSG_DLG);
		(pclclient->m_pDialog[ NOTIFYMSG_DLG ])->SetText(  pTitle,Buffer);
		//MessageBox( NULL, pTitle, Buffer, MB_OK|MB_TOPMOST );
		return;
	}

	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stPrivateTrade_Price sPrice
		(
		m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		m_siPrice
		);

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sPrice, sPrice.sHeader.uiSize );
		
		TCHAR* pTitle = GetTxtFromMgr(2359);
		TCHAR* pText = GetTxtFromMgr(2360);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		//EnableWindow( GetDlgItem( m_hMoneyItemDlg, IDC_BUTTON_PRIVATETRADE_PRICE ), FALSE );
		//EnableWindow( GetDlgItem( m_hMoneyItemDlg, IDC_EDIT_PRIVATETRADE_MONEY ), FALSE );
	}

	return;
}


bool CNPrivateTradeDlg::Pop( BYTE *pData )
{
	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToExistListAndPushBackToEmptyList();

	if ( pTemp )
	{
		UI32 uiDataSize;
		pTemp->GetData( (BYTE*)pData, &uiDataSize );
		return true;
	}

	return false;
}



