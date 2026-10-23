#include "../Client/NInterface/NIzoDlg/NIzoPriceValueDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

extern cltCommonLogic* pclClient;

CNIzoPriceValueDlg::CNIzoPriceValueDlg() : CDialogController()
{
	m_siPriceValue = 0;
	m_siTinTin = 0;
	m_siPriceTime = 0;

	for(SI32 i=0;i<11;++i)
		m_pStatic[i] = NULL;
	m_pEdit[0] = NULL;
	m_pEdit[1] = NULL;

	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pOutLine = NULL;

}

CNIzoPriceValueDlg::~CNIzoPriceValueDlg()
{
	for(SI32 i=0;i<11;++i)
		SAFE_DELETE(m_pStatic[i]);

	SAFE_DELETE(m_pEdit[0]);
	SAFE_DELETE(m_pEdit[1]);

	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pOutLine);

	SAFE_DELETE(m_pEdit_TinTin);
	SAFE_DELETE(m_pButton[2]);
	SAFE_DELETE(m_pButton[3]);
}

void CNIzoPriceValueDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NIzoDlg/DLG_Izo_PriceValue.ddf"));
	file.CreateChildDialog( this, NIZOPRICEVALUE_DLG, TEXT("dialog_Izo_PriceValue"), StaticCallBackDialogNIzoPriceValueDlg, pParent);
	

 	m_pOutLine = new COutLine(this);

	for(SI32 i=0;i<11;++i)
		NEWCSTATIC(m_pStatic[i]);

	NEWCEDIT(m_pEdit[0]);
	NEWCEDIT(m_pEdit[1]);

 	NEWCBUTTON(m_pButton[0]);
	NEWCBUTTON(m_pButton[1]);
	NEWCBUTTON(m_pButton[2]);
	NEWCBUTTON(m_pButton[3]);
	
	NEWCEDIT(m_pEdit_TinTin);

	//file.CreateControl( m_pOutLine, NIZOPRICEVALUE_DIALOG_OUTLINE, TEXT("outline_NONAME1") );
	file.CreateControl( m_pStatic[0], NIZOPRICEVALUE_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NIZOPRICEVALUE_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
	file.CreateControl( m_pStatic[2], NIZOPRICEVALUE_DIALOG_STATIC3, TEXT("statictext_NONAME3") );
	file.CreateControl( m_pStatic[3], NIZOPRICEVALUE_DIALOG_STATIC4, TEXT("statictext_NONAME4") );
	file.CreateControl( m_pStatic[4], NIZOPRICEVALUE_DIALOG_STATIC5, TEXT("statictext_NONAME5") );
	file.CreateControl( m_pStatic[5], NIZOPRICEVALUE_DIALOG_STATIC6, TEXT("statictext_NONAME6") );
	file.CreateControl( m_pStatic[6], NIZOPRICEVALUE_DIALOG_STATIC7, TEXT("statictext_NONAME7") );
	file.CreateControl( m_pStatic[7], NIZOPRICEVALUE_DIALOG_STATIC8, TEXT("statictext_NONAME8") );
	file.CreateControl( m_pStatic[8], NIZOPRICEVALUE_DIALOG_STATIC9, TEXT("statictext_NONAME9") );

	if(pclClient->IsCountrySwitch(Switch_Izo_TinTinPercent))
	{
		file.CreateControl( m_pButton[2], NIZOPRICEVALUE_DIALOG_BUTTON3, TEXT("button_NONAME3") );
		file.CreateControl( m_pButton[3], NIZOPRICEVALUE_DIALOG_BUTTON4, TEXT("button_NONAME4") );

		file.CreateControl( m_pStatic[9], NIZOPRICEVALUE_DIALOG_STATIC10, TEXT("statictext_NONAME10") );
		file.CreateControl( m_pStatic[10],NIZOPRICEVALUE_DIALOG_STATIC11, TEXT("statictext_NONAME11") );

		file.CreateControl( m_pEdit_TinTin, NIZOPRICEVALUE_DIALOG_EDIT_TINTIN, TEXT("editbox_TinTin") );		
	}
	else
	{
		file.CreateControl( m_pStatic[9], NIZOPRICEVALUE_DIALOG_STATIC10, TEXT("statictext_NONAME10") );
		file.CreateControl( m_pStatic[10],NIZOPRICEVALUE_DIALOG_STATIC11, TEXT("statictext_NONAME11") );
	}

	file.CreateControl( m_pEdit[0], NIZOPRICEVALUE_DIALOG_EDIT1, TEXT("editbox_NONAME1") );
	file.CreateControl( m_pEdit[1], NIZOPRICEVALUE_DIALOG_EDIT2, TEXT("editbox_NONAME2") );

	file.CreateControl( m_pButton[0], NIZOPRICEVALUE_DIALOG_BUTTON1, TEXT("button_NONAME1") );
   	file.CreateControl( m_pButton[1], NIZOPRICEVALUE_DIALOG_BUTTON2, TEXT("button_NONAME2") );
	
	m_pStatic[6]->SetTextPrintOption( TA_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pStatic[6]->Refresh();
	m_pStatic[7]->SetTextPrintOption( TA_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pStatic[7]->Refresh();
	m_pStatic[8]->SetTextPrintOption( TA_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pStatic[8]->Refresh();
	m_pStatic[9]->SetTextPrintOption( TA_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pStatic[9]->Refresh();

	if(pclClient->IsCountrySwitch(Switch_Izo_TinTinPercent))
	{
	}

	file.CreateControl( m_pOutLine, NIZOPRICEVALUE_DIALOG_OUTLINE, TEXT("outline_NONAME1") );
	//_LEON_ENGLISH
	/*
	m_pStatic[6]->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pStatic[9]->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_WORDBREAK);
	m_pStatic[6]->Refresh();
	m_pStatic[9]->Refresh();
	*/
	Hide();
}

void CNIzoPriceValueDlg::Action()
{

}

void CNIzoPriceValueDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNIzoPriceValueDlg::Show()
{
	CControllerMgr::Show( true );

}

void CALLBACK CNIzoPriceValueDlg::StaticCallBackDialogNIzoPriceValueDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNIzoPriceValueDlg *pThis = (CNIzoPriceValueDlg *)pControllerMgr;
	pThis->CallBackDialogNIzoPriceValueDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNIzoPriceValueDlg::CallBackDialogNIzoPriceValueDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case NIZOPRICEVALUE_DIALOG_BUTTON1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED: // 올리기
				{
  					m_siPriceValue++;
					if(m_siPriceValue > 75)
						m_siPriceValue = 75;
					
					TCHAR buffer[10] = TEXT("");

					m_pEdit[1]->SetText(_itot(m_siPriceValue, buffer, 10));
				}
				break;
			}
		}
		break;

	case NIZOPRICEVALUE_DIALOG_BUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:	// 내리기
				{
					m_siPriceValue--;
					if(m_siPriceValue < 50)
						m_siPriceValue = 50;

					TCHAR buffer[10] = TEXT("");

					m_pEdit[1]->SetText(_itot(m_siPriceValue, buffer, 10));
				}
				break;
			}
		}
		break;
	case NIZOPRICEVALUE_DIALOG_BUTTON3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED: // 올리기
				{
					m_siTinTin++;
					if(m_siTinTin > 10)
						m_siTinTin = 10;

					TCHAR buffer[10] = TEXT("");

					m_pEdit_TinTin->SetText(_itot(m_siTinTin, buffer, 10));
				}
				break;
			}
		}
		break;

	case NIZOPRICEVALUE_DIALOG_BUTTON4:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:	// 내리기
				{
					m_siTinTin--;
					if(m_siTinTin < 1)
						m_siTinTin = 1;

					TCHAR buffer[10] = TEXT("");

					m_pEdit_TinTin->SetText(_itot(m_siTinTin, buffer, 10));
				}
				break;
			}
		}
		break;
	}
}
