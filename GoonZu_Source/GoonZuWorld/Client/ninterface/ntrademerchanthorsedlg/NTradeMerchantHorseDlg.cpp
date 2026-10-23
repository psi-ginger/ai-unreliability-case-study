#include "../Client/NInterface/NTradeMerchantHorseDlg/NTradeMerchantHorseDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/Msg/MsgType-Village.h"
#include "../../CommonLogic/Msg/MsgType-TradeMerchant.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNTradeMerchantHorseDlg::CNTradeMerchantHorseDlg() : CDialogController()
{
	m_siHorsePrice = 0;
	m_siHorseIndex = 2;	// 일단 임시로 투명공간을 찍어준다 이미지가 들어가면 바뀌어야 할 숫자다

	m_pStatic[0] = NULL;	
	m_pStatic[1] = NULL;	

	m_pEdit[0] = NULL;	
	m_pEdit[1] = NULL;	

	m_pButtonSellHorse = NULL;
	m_pButtonClose = NULL;
}

CNTradeMerchantHorseDlg::~CNTradeMerchantHorseDlg()
{
	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pEdit[0]);
	SAFE_DELETE(m_pEdit[1]);
	SAFE_DELETE(m_pButtonSellHorse);
	SAFE_DELETE(m_pButtonClose);
}

void CNTradeMerchantHorseDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NTradeMerchantHorseDlg/DLG_TradeMerchantHorse.ddf"));
		file.CreatePopupDialog( this, NTRADEMERCHANTHORSE_DLG, TEXT("dialog_TradeMerchant_Horse"), StaticCallBackDialogNTradeMerchantHorseDlg );

		m_pStatic[0]			  = new CStatic( this );
		m_pStatic[1]			  = new CStatic( this );
		m_pEdit[0]				  = new CEdit( this );
		m_pEdit[1]				  = new CEdit( this );
		m_pButtonSellHorse		  = new CButton( this );
		m_pButtonClose			  = new CButton( this );

		file.CreateControl( m_pStatic[0], NTRADEMERCHANTHORSE_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
		m_pStatic[0]->SetFontIndex( pclClient->GetUniqueFromHash(TEXT("KIND_MERCHANTHORSE")) - 1);

		file.CreateControl( m_pStatic[1], NTRADEMERCHANTHORSE_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
		
		file.CreateControl( m_pEdit[0], NTRADEMERCHANTHORSE_DIALOG_EDIT1, TEXT("editbox_NONAME1") );
		file.CreateControl( m_pEdit[1], NTRADEMERCHANTHORSE_DIALOG_EDIT2, TEXT("editbox_NONAME2") );

		file.CreateControl( m_pButtonSellHorse, NTRADEMERCHANTHORSE_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButtonClose, NTRADEMERCHANTHORSE_DIALOG_BUTTON2, TEXT("button_NONAME2") );

		if ( ! pclClient->pclCM->CR[1]->HaveHorse() )
		{
			TCHAR* pText =GetTxtFromMgr(2114);
			m_pEdit[0]->SetText(pText);
			return;
		}

		cltClient *pclclient = (cltClient *)pclClient;
		SI32 datevary = pclclient->clClientDate.GetDateVary();

		TCHAR* pText =GetTxtFromMgr(2115);
		m_pEdit[0]->SetText(pText);

		cltGameMsgRequest_TradeMerchantHorseBuyInfo clGameMsgRequest_TradeMerchantHorseBuyInfo( pclClient->GetUniqueFromHash(TEXT("KIND_MERCHANTHORSE")) );
		cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTHORSEBUYINFO, sizeof(clGameMsgRequest_TradeMerchantHorseBuyInfo), (BYTE*)&clGameMsgRequest_TradeMerchantHorseBuyInfo );
		pclClient->SendMsgToServer( (sPacketHeader*) &clMsg );

		LoadHorse();	

	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNTradeMerchantHorseDlg::StaticCallBackDialogNTradeMerchantHorseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNTradeMerchantHorseDlg *pThis = (CNTradeMerchantHorseDlg *)pControllerMgr;
	pThis->CallBackDialogNTradeMerchantHorseDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNTradeMerchantHorseDlg::CallBackDialogNTradeMerchantHorseDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NTRADEMERCHANTHORSE_DIALOG_BUTTON1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_siHorsePrice < 1 )
					{
						return;
					}

					cltHorse* pclhorse = pclClient->pclCM->CR[1]->pclCI->clHorseInfo.GetCurrentShowHorse();

					if ( pclhorse == NULL || pclhorse->siHorseUnique < 1 )
					{
						return;
					}

					if ( pclClient->pclCM->CR[1]->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == true )
					{
						TCHAR* pText = GetTxtFromMgr(2088);
						m_pEdit[0]->SetText(pText);
						return ;
					}

					if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.IsHorseHaveItem() )
					{
						TCHAR* pText = GetTxtFromMgr(2107);
						m_pEdit[0]->SetText(pText);
						return ;
					}

					TCHAR buffer[256 ] = TEXT("");
					TCHAR* pText = GetTxtFromMgr(2110);
					StringCchPrintf( buffer, 256, pText, m_siHorsePrice );
					pText = GetTxtFromMgr(2113);

					cltClient *pclclient = (cltClient*)pclClient;

					stMsgBox MsgBox;
					MsgBox.Set( this, pText, buffer, MSGBOX_TYPE_YESNO, 0 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );

				//	if( MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) break;

				//	cltGameMsgRequest_TradeMerchantBuyHorseFromPerson clGameMsgRequest_TradeMerchantBuyHorseFromPerson( KIND_MERCHANTHORSE );
				//	cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYHORSEFROMPERSON, sizeof(clGameMsgRequest_TradeMerchantBuyHorseFromPerson), (BYTE*)&clGameMsgRequest_TradeMerchantBuyHorseFromPerson );
				//	pclClient->SendMsgToServer( (BYTE*) &clMsg );
				}
				break;
			}
		}
		break;

	case NTRADEMERCHANTHORSE_DIALOG_BUTTON2:
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

void CNTradeMerchantHorseDlg::LoadHorse()
{
	//==========================================
	// 초상화 
	//==========================================
	// 초상화용 핸들을 삭제한다.
	
	cltHorse* pclhorse = pclClient->pclCM->CR[1]->pclCI->clHorseInfo.GetCurrentShowHorse();

	if( pclhorse != NULL || pclhorse->siHorseUnique )
	{
		TCHAR strFileName[ 128 ] = TEXT("");
		TCHAR strPathFileName[ 128 ] = TEXT("");
		switch( pclhorse->siHorseUnique )
		{
		case 1:	m_siHorseIndex = 56; break;
		case 2:	m_siHorseIndex = 111; break;
		case 3:	m_siHorseIndex = 112; break;
		case 4:	m_siHorseIndex = 151;	break;
		case 5: m_siHorseIndex = 167; break;
		case 6: m_siHorseIndex = 179; break;
		case 7: m_siHorseIndex = 189; break;
		case 8: m_siHorseIndex = 207; break;
		default:m_siHorseIndex = 56;	break;
		}
		
		if( m_siHorseIndex == -1)
			m_pStatic[1]->SetFontIndex(m_siHorseIndex);
		else
			m_pStatic[1]->SetFontIndex(m_siHorseIndex);		
	}
	else
		m_pStatic[1]->SetFontIndex(m_siHorseIndex);

//	if ( m_hHorsePortraitBitmap != NULL )
//	{
//		SendDlgItemMessage( m_hDlg, IDC_STATIC_HORSEIMAGE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHorsePortraitBitmap );
//	}

	TCHAR Buffer[ 512 ] = TEXT("");
	if ( pclClient->pclHorseManager->GetText( pclhorse, Buffer, 512 ) )
	{
		m_pEdit[1]->SetText(Buffer);
	}

	return;
}

void CNTradeMerchantHorseDlg::SetHorsePrice( SI16 HorseUnique, GMONEY HorsePrice )
{
	TCHAR *HorseKindName = pclClient->pclHorseManager->GetHorseName( HorseUnique );
	TCHAR Buffer[256 ] = TEXT("");
	TCHAR* pText =GetTxtFromMgr(2116);
	StringCchPrintf( Buffer, 256, pText, HorseKindName, HorsePrice );

	m_siHorsePrice = HorsePrice;
	
	m_pEdit[0]->SetText(Buffer);
}

void CNTradeMerchantHorseDlg::SellToNPCSuccess()
{
	TCHAR* pText =GetTxtFromMgr(2117);

	m_pEdit[0]->SetText(pText);
	// 말이 팔렸음으로, 말 정보 초기화
	m_siHorsePrice = 0;

	m_pEdit[1]->SetText(TEXT(""));
	m_pStatic[1]->SetFontIndex(2);
	return;
}

void CNTradeMerchantHorseDlg::SetData( void *pData )
{
	stNTradeMerchantHorseData *pNTradeMerchantHorseData = (stNTradeMerchantHorseData *)pData;

	switch(pNTradeMerchantHorseData->siParam)
	{
	case 0:
		SetHorsePrice(pNTradeMerchantHorseData->siFirst, pNTradeMerchantHorseData->siSecond);
		break;
	case 1:	
		SellToNPCSuccess();
		break;
	}
}

void CNTradeMerchantHorseDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( RetValue )
			{
				cltGameMsgRequest_TradeMerchantBuyHorseFromPerson clGameMsgRequest_TradeMerchantBuyHorseFromPerson( pclClient->GetUniqueFromHash(TEXT("KIND_MERCHANTHORSE")) );
				cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYHORSEFROMPERSON, sizeof(clGameMsgRequest_TradeMerchantBuyHorseFromPerson), (BYTE*)&clGameMsgRequest_TradeMerchantBuyHorseFromPerson );
				pclClient->SendMsgToServer( (sPacketHeader*) &clMsg );
			}
			else
				return;
		}
		break;

	}
}
