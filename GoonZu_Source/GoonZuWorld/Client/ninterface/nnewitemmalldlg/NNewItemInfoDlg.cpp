#include "../Client/NInterface/NNewItemMalldlg/NNewItemInfoDlg.h"
#include "../Client/NInterface/NNewItemMalldlg/NNewItemMall.h"

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

CNNewItemMallBuyInfoDlg::CNNewItemMallBuyInfoDlg() : CDialogController()
{
	SI32 i;

	for(i=0; i < 11; ++i)
		m_pStatic[i] = NULL;

	for(i=0; i < 2; ++i)
		m_pBtn[i] = NULL;
}

CNNewItemMallBuyInfoDlg::~CNNewItemMallBuyInfoDlg()
{
	SI32 i;

	for(i=0; i < 11; ++i)
		SAFE_DELETE(m_pStatic[i]);

	for(i=0; i < 2; ++i)
		SAFE_DELETE(m_pBtn[i]);
}

void CNNewItemMallBuyInfoDlg::Create()
{
	if ( ! IsCreate() ) 
	{
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NNewItemMallDlg/DLG_NNewItemMallInfo.ddf");
		file.CreatePopupDialog( this, NBUYITEMINFOWINDOW_DLG, "dialog_info", StaticCallBackDialogNNewItemMallBuyInfoDlg );

		cltClient *pclclient = (cltClient *)pclClient;
		//		MovePos( pclclient->GetScreenXSize()/2 - 300 + 45, pclclient->GetScreenYSize()/2 - 400 + 100);

		MovePos(445, 185);

		SI32 i;

		for(i=0; i < 11; ++i)
			NEWCSTATIC(m_pStatic[i]);

		for(i=0; i < 2; ++i)
			NEWCBUTTON(m_pBtn[i]);

		TCHAR tempBuffer[100] = {'\0'};
		for(i=0; i < 11; ++i)
		{
			if( i != 0 )
			{
				sprintf(tempBuffer, "statictext_NONAME%d", i + 1);
				file.CreateControl( m_pStatic[i], NBUYITEMIFNOWINDOW_DIALOG_STATIC1 + i, tempBuffer);
			}
		}
		sprintf(tempBuffer, "statictext_NONAME%d", 1);
		file.CreateControl( m_pStatic[0], NBUYITEMIFNOWINDOW_DIALOG_STATIC1 + 0, tempBuffer);


		file.CreateControl( m_pBtn[0], NBUYITEMIFNOWINDOW_DIALOG_BUTTONNCOIN, "button_NONAME1"); 
		file.CreateControl( m_pBtn[1], NBUYITEMIFNOWINDOW_DIALOG_BUTTONCLOSE, "button_NONAME2"); 


		m_pStatic[7]->Show(false);
		m_pStatic[8]->Show(false);
		m_pStatic[9]->Show(false);
		m_pStatic[10]->Show(false);

		SetTopMostWindow( true );
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNNewItemMallBuyInfoDlg::StaticCallBackDialogNNewItemMallBuyInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNNewItemMallBuyInfoDlg *pThis = (CNNewItemMallBuyInfoDlg *)pControllerMgr;
	pThis->CallBackDialogNNewItemMallBuyInfoDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNNewItemMallBuyInfoDlg::CallBackDialogNNewItemMallBuyInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NBUYITEMIFNOWINDOW_DIALOG_BUTTONNCOIN:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(pclClient->siServiceArea == ConstServiceArea_Japan)
					{
						cltGameMsgRequest_BillPage_URL clinfo( pclclient->szID,pclclient->szPassword);
						cltMsg clMsg(GAMEMSG_REQUEST_BILLPAGE_URL,sizeof(clinfo),(BYTE*)&clinfo);
						pclclient->SendMsgToServer( (sPacketHeader*)&clMsg);
					}
					else if( pclClient->siServiceArea == ConstServiceArea_Korea )
					{
						ShellExecute( NULL, NULL, TEXT("http://cash.ndoors.com/"), NULL, NULL, SW_SHOWNORMAL );
					}
					else if( pclClient->IsWhereServiceArea( ConstServiceArea_China) )
					{
						ShellExecute( NULL,NULL,TEXT("http://www.youxi.net.cn/AccountManage/AddCostIdx.aspx"),NULL,NULL,SW_SHOWNORMAL );
					}
					else if( pclClient->IsWhereServiceArea( ConstServiceArea_USA) )
					{						
						ShellExecute( NULL,NULL,TEXT("https://billing.ijji.com/payment/PaymentProcess.nhn"),NULL,NULL,SW_SHOWNORMAL );
					}
					else if( pclClient->IsWhereServiceArea( ConstServiceArea_NHNChina) )
					{
						//KHY - 0304 - 중국 NHNChinaBilling -  코인충전용 웹페이지 주소 넣는부분.
						//ShellExecute( NULL,NULL,TEXT("http://www.youxi.net.cn/AccountManage/AddCostIdx.aspx"),NULL,NULL,SW_SHOWNORMAL );
						ShellExecute( NULL,NULL,TEXT("https://billing.ijji.com/payment/PaymentProcess.nhn"),NULL,NULL,SW_SHOWNORMAL );
					}
					else if ( pclClient->IsWhereServiceArea( ConstServiceArea_EUROPE ) )
					{
						ShellExecute( NULL,NULL,TEXT("http://www.newhjz.cn"),NULL,NULL,SW_SHOWNORMAL );
					}
					else if( pclClient->IsWhereServiceArea( ConstServiceArea_English) )
					{
						// PCK : 글로벌의 경우 NHN회원과 NDI회원에 따라서 보여지는 링크페이지가 다르다. (09.03.30)			
						NTCHARString32	kID(pclclient->szID);

						if( kID.Find("@nh") >= 0 )		// NHN 회원이면
						{
							ShellExecute( NULL, NULL, TEXT("http://luminary.ijji.com/center/billing/transfer_gcoin_01.asp"), NULL, NULL, SW_SHOWNORMAL );
						}
						else							// NDI 회원이면
						{
							ShellExecute( NULL, NULL, TEXT("https://billing.ndoorsgames.com/fillup/payment_option.asp?site_code=luminary"), NULL, NULL, SW_SHOWNORMAL );
						}
					}
					else
					{
						ShellExecute( NULL,NULL,TEXT("https://billing.ndoorsgames.com/fillup/payment_option.asp?site_code=luminary"),NULL,NULL,SW_SHOWNORMAL );
					}
				}
				break;
			}
		}
		break;

	case NBUYITEMIFNOWINDOW_DIALOG_BUTTONCLOSE:
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

void CNNewItemMallBuyInfoDlg::SetBuyItemInfo(cltItemMallData& itemdada, SI32 itemnum, SI32 realcash, SI32 eventcash)
{
	m_pStatic[2]->SetText(itemdada.szName);

	TCHAR tempItemNum[20] = {'\0'};
	_itoa( itemnum, tempItemNum, 10 );

	TCHAR tempBuffer[50] = {'\0'};

 	MStrCpy(tempBuffer, tempItemNum, 50);

	//TCHAR * pText = GetTxtFromMgr( 2219 );

	//strcat(tempBuffer, pText);

	m_pStatic[4]->SetText(tempBuffer);
}

void CNNewItemMallBuyInfoDlg::SetCashInfo(SI32 realcash, SI32 eventcash)
{
	TCHAR tempBuffer[50] = { '\0' };
	TCHAR* pText = GetTxtFromMgr(5944); // "원"
	g_JYLibFunction.SetNumUnit(realcash+eventcash, tempBuffer,50, pText);
	m_pStatic[7]->SetText(tempBuffer);
}
