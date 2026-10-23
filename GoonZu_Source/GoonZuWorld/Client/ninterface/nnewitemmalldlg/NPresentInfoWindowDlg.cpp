#include "../Client/NInterface/NNewItemMallDlg/NPresentInfoWindowDlg.h"
#include "NBuyWindowDlg.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNPresentInfoWindowDlg::CNPresentInfoWindowDlg() : CDialogController()
{

	m_siSelectProductKey		= 0;
	m_siSelectItemPrice			= 0;
	m_siSelectItemEventPrice	= 0;
	m_siSelectBuyItemNum		= 0;
	
	strcpy(m_szAccountID, "");
	strcpy(m_szPresentMessage,"");
	strcpy(m_szItemName,"");

	SI32 i;
	for(i=0; i < 2; ++i)
		m_pBtn[i] = NULL;

	for(i=0; i < 12; ++i)
		m_pStatic[i] = NULL;


	m_siItemImageFont = 0;
}

CNPresentInfoWindowDlg::~CNPresentInfoWindowDlg()
{
	SI32 i;
	for(i=0; i < 2; ++i)
		SAFE_DELETE(m_pBtn[i]);

	for(i=0; i < 12; ++i)
		SAFE_DELETE(m_pStatic[i]);
}

void CNPresentInfoWindowDlg::Create()
{
	if ( ! IsCreate() ) 
	{
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NNewItemMallDlg/DLG_PresentInfoWindow.ddf");
		file.CreatePopupDialog( this, NPRESENTINFOWINDOW_DLG, "dialog_presentinfo", StaticCallBackDialogNPresentInfoWindowDlg );

		NEWCBUTTON(m_pBtn[0]);
		NEWCBUTTON(m_pBtn[1]);

		cltClient *pclclient = (cltClient *)pclClient;
		MovePos( pclclient->GetScreenXSize()/2 - 300 + 45, pclclient->GetScreenYSize()/2 - 400 + 100 );

		SI32 i;
		for(i=0; i < 12; ++i)
			NEWCSTATIC(m_pStatic[i]);

		char tempBuffer[100] = { '\0' };

		for( i=0; i < 12; ++i)
		{
			sprintf(tempBuffer, "statictext_NONAME%d", i + 1);
			file.CreateControl( m_pStatic[i], NPRESENTINFOWINDOW_DIALOG_STATIC1 + i, tempBuffer);
		}

		file.CreateControl( m_pBtn[0], NPRESENTINFOWINDOW_DIALOG_BUTTONPRESENT, "button_NONAME1"); 
		file.CreateControl( m_pBtn[1], NPRESENTINFOWINDOW_DIALOG_BUTTONCLOSE, "button_NONAME2"); 

		SetTopMostWindow(true);
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNPresentInfoWindowDlg::StaticCallBackDialogNPresentInfoWindowDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNPresentInfoWindowDlg *pThis = (CNPresentInfoWindowDlg *)pControllerMgr;
	pThis->CallBackDialogNPresentInfoWindowDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNPresentInfoWindowDlg::CallBackDialogNPresentInfoWindowDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NPRESENTINFOWINDOW_DIALOG_BUTTONPRESENT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( m_szAccountID[0] == '\0')
						return;

					if(m_siSelectProductKey < 1)
						return;

					if(m_siSelectItemPrice < 1)
						return;

					if(m_siSelectBuyItemNum < 1)
						return;

					if( ConstServiceArea_China == pclClient->siServiceArea )
					{
						cltClient *pclclient = (cltClient *)pclClient;
						pclclient->CreateInterface( NBUYSECONDWINDOW_DLG );
						((CNBuySecondWindowDlg*)pclclient->m_pDialog[ NBUYSECONDWINDOW_DLG ])->SetPresentInfo(	m_szAccountID, 
																												m_siSelectProductKey, 
																												m_siSelectBuyItemNum,
																												m_siSelectItemPrice , 
																												m_siSelectItemEventPrice ,
																												m_szPresentMessage, 
																												m_szItemName,
																												m_siServerUnique,
																												m_szCharName,
																												m_siItemImageFont );
					}
					else 
					{
						cltClient *pclclient = (cltClient *)pclClient;

						cltGameMsgRequest_ItemMall_Giftitem pclinfo(m_szAccountID, 
																	m_siSelectProductKey, 
																	m_siSelectBuyItemNum,
																	m_siSelectItemPrice, 
																	m_siSelectItemEventPrice, 
																	m_szPresentMessage, 
																	m_szItemName,
																	m_siServerUnique,
																	m_szCharName,
																	m_siItemImageFont);
						cltMsg clMsg( GAMEMSG_REQUEST_ITEMMALL_GIFTITEM, sizeof( pclinfo ), (BYTE*)&pclinfo );
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					}

					DeleteDialog();
				}
				break;
			}
		}
		break;

	case NPRESENTINFOWINDOW_DIALOG_BUTTONCLOSE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;
	}
}

void CNPresentInfoWindowDlg::SetPresentInfoWindow( TCHAR*	ServerAndName , 
												   TCHAR*	message,
												   TCHAR*	itemname, 
												   SI32		SelectProductKey, 
												   SI32		SelectNumber, 
												   SI32		SelectPrice, 
												   SI32		SelectEventPrice, 
												   SI32		SelectFontIndex,
												   TCHAR*	accountid, 
												   SI16		ServerUnique, 
												   TCHAR*	CharName)
{
	MStrCpy(m_szAccountID, accountid, 20);
	MStrCpy(m_szPresentMessage, message, 200);
	MStrCpy(m_szItemName, itemname, 50);
	MStrCpy(m_szCharName, CharName,20 );

	m_siServerUnique		 = ServerUnique;
	m_siSelectProductKey	 = SelectProductKey;
	m_siSelectItemPrice		 = SelectPrice;
	m_siSelectItemEventPrice = SelectEventPrice;
	m_siSelectBuyItemNum	 = SelectNumber;

	m_siItemImageFont		 = SelectFontIndex;
	// 아이템 이미지
	// 아이템 이미지가 넘어 가서 이제 읽어오는걸 바꿨다.
	// 받은 아이템 폰트 인덱스에 따라서 읽어 오는 파일을 바꾼다.
	// 그리고 바뀐 파일일 경우에 새롭게 폰트 인덱스를 넣어주는 작업이 필요함.
	TCHAR szImageFilename[256];
	SI32 siImageFontindex;
	pclClient->pclItemManager->GetItemImageItemmallFileName( SelectFontIndex, szImageFilename,sizeof(szImageFilename), &siImageFontindex);
	m_pStatic[0]->SetFileName(szImageFilename);
	m_pStatic[0]->SetFontIndex(siImageFontindex);

	// 아이템 이름
	m_pStatic[2]->SetText(itemname);

	char tempBuffer[50] = {'\0'};
	// 구매 수량
	sprintf(tempBuffer,"%d",m_siSelectBuyItemNum);
	m_pStatic[4]->SetText(tempBuffer);

	// 구매 금액
	sprintf(tempBuffer,"%d",m_siSelectItemPrice*m_siSelectBuyItemNum);
	m_pStatic[7]->SetText(tempBuffer);

	// 선물 받을 사람 계정
	m_pStatic[9]->SetText(ServerAndName);
}

