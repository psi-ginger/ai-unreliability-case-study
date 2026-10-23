#include "NBuyWindowDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

extern cltCommonLogic* pclClient;

CNBuySecondWindowDlg::CNBuySecondWindowDlg() : CDialogController()
{
	m_uiMode					= BUY;
	m_siSelectProductKey		= 0;
	m_siSelectItemPrice			= 0;
	m_siSelectItemEventPrice	= 0;
	m_siSelectBuyItemNum		= 1;

	m_szSelectItemName[0]		= '\0';

	m_szGetAccountID[0]			= '\0'; // 선물 받을 사람 AccountID
	m_szComment[0]				= '\0';

	m_siGetServerUnique			= -1;
	m_szGetCharName[0]			= '\0';
	m_siItemImageFont			= 0;
}

CNBuySecondWindowDlg::~CNBuySecondWindowDlg()
{
}

void CNBuySecondWindowDlg::Create()
{
	if ( ! IsCreate() ) 
	{
		cltClient *pclclient = (cltClient *)pclClient;

		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NNewItemMallDlg/DLG_BuySecondWindow.ddf");
		file.CreatePopupDialog( this, NBUYSECONDWINDOW_DLG, "dialog_buysecondwindow", StaticCallBackDialogNBuySecondWindowDlg );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC , NBUYSECOND_STATIC1 , this)  , NBUYSECOND_STATIC1 , TEXT("statictext_NONAME1") );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NBUYSECOND_EDIT1 , this)  , NBUYSECOND_EDIT1 , TEXT("editbox_NONAME2") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NBUYSECOND_EDIT_INPUTSECONDKEY , this)  , NBUYSECOND_EDIT_INPUTSECONDKEY , TEXT("editbox_NONAME1") );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NBUYSECOND_BUTTON_BUY , this)  , NBUYSECOND_BUTTON_BUY , TEXT("button_NONAME1") );

		TCHAR* text = GetTxtFromMgr( 10053 );
		m_InterfaceFactory.GetEdit( NBUYSECOND_EDIT1)->SetText( text );
		m_InterfaceFactory.GetEdit( NBUYSECOND_EDIT_INPUTSECONDKEY)->SetPasswordMode( true );
		SetTopMostWindow( true );
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNBuySecondWindowDlg::StaticCallBackDialogNBuySecondWindowDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNBuySecondWindowDlg *pThis = (CNBuySecondWindowDlg *)pControllerMgr;
	pThis->CallBackDialogNBuySecondWindowDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNBuySecondWindowDlg::CallBackDialogNBuySecondWindowDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NBUYSECOND_BUTTON_BUY:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					CEdit* secondPassWord = m_InterfaceFactory.GetEdit( NBUYSECOND_EDIT_INPUTSECONDKEY);
					
					if( 0 >= strlen( secondPassWord->GetText() ) || 9 <= strlen( secondPassWord->GetText() ))
					{
					//	TCHAR* pTitle = GetTxtFromMgr(5945);
					//	TCHAR* pText  = GetTxtFromMgr(;

					//	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
					//	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
					}

					if( BUY == m_uiMode )
					{
						cltClient *pclclient = (cltClient *)pclClient;
						cltGameMsgRequest_ItemMall_Buyitem pclinfo( m_siSelectProductKey, 
																	m_siSelectBuyItemNum, 
																	m_siSelectItemPrice, 
																	m_siSelectItemEventPrice, 
																	m_szSelectItemName,
																	secondPassWord->GetText() );
						cltMsg clMsg( GAMEMSG_REQUEST_ITEMMALL_BUYITEM, sizeof( pclinfo ), (BYTE*)&pclinfo );
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
					else if( PRESENT == m_uiMode )
					{
						cltClient *pclclient = (cltClient *)pclClient;

						cltGameMsgRequest_ItemMall_Giftitem pclinfo( m_szGetAccountID, 
																	 m_siSelectProductKey,
							                                         m_siSelectBuyItemNum, 
																	 m_siSelectItemPrice ,
																	 m_siSelectItemEventPrice,
																	 m_szComment, 
																	 m_szSelectItemName,
																	 m_siGetServerUnique, 
																	 m_szGetCharName,
																	 m_siItemImageFont, 
																	 secondPassWord->GetText() );
						cltMsg clMsg( GAMEMSG_REQUEST_ITEMMALL_GIFTITEM, sizeof( pclinfo ), (BYTE*)&pclinfo );
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					}

					DeleteDialog();
				}
				break;
			}
		}
		break;
	}
}

void CNBuySecondWindowDlg::SetBuyInfo( SI32 SelectProductKey, SI32 BuyItemNum, SI32 SelectItemPrice, SI32 SelectItemEventPrice, TCHAR* SelectItemName )
{
	m_uiMode = BUY;
	m_siSelectProductKey		= SelectProductKey;
	m_siSelectItemPrice			= SelectItemPrice;
	m_siSelectItemEventPrice	= SelectItemEventPrice;
	m_siSelectBuyItemNum		= BuyItemNum;
	MStrCpy( m_szSelectItemName, SelectItemName, 50 );
}

void CNBuySecondWindowDlg::SetPresentInfo( TCHAR*getid,SI32 key,SI32 num,SI32 price,SI32 Eventprice,TCHAR*comment,TCHAR* itemname,SI16 ServerUnique,TCHAR* CharName,SI32 ItemImageFont )
{
	m_uiMode = PRESENT;

	MStrCpy( m_szGetAccountID,getid,MAX_PLAYER_NAME);
	m_siSelectProductKey = key;
	m_siSelectItemPrice = price;
	m_siSelectItemEventPrice = Eventprice;
	
	m_siSelectBuyItemNum = num;

	MStrCpy(m_szComment,comment,200);
	MStrCpy(m_szSelectItemName,itemname,50);

	m_siGetServerUnique = ServerUnique;
	MStrCpy( m_szGetCharName,CharName,MAX_PLAYER_NAME );		

	m_siItemImageFont = ItemImageFont;
}