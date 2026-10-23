#include "NItemMallGiveStatus.h"

#include "Client.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"
#include "Resource.h"
#include "CommonLogic.h"

#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/interface/Button.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "MsgType.h"
#include "MsgType-System.h"

extern cltCommonLogic* pclClient;

CNItemMallGiveStatus::CNItemMallGiveStatus()
{
	m_pStatic_ItemImage = NULL;
	m_pStatic_ItemExplain = NULL;
	m_pStatic_StatusExplain = NULL;
	m_pStatic_Str = NULL;
	m_pStatic_Vit = NULL;
	m_pStatic_Dex = NULL;
	m_pStatic_Hnd = NULL;
	m_pStatic_Mag = NULL;
	m_pStatic_Luk = NULL;
	m_pButton_Ok = NULL; 
	m_pButton_Cancel = NULL;

	m_pOutLine = NULL;

	m_pButton_Str_Plus = NULL;
	m_pButton_Vit_Plus = NULL;
	m_pButton_Dex_Plus = NULL;
	m_pButton_Hnd_Plus = NULL;
	m_pButton_Mag_Plus = NULL;
	m_pButton_Luk_Plus = NULL;
	m_pButton_Str_Minus = NULL;
	m_pButton_Vit_Minus = NULL;
	m_pButton_Dex_Minus = NULL;
	m_pButton_Hnd_Minus = NULL;
	m_pButton_Mag_Minus = NULL;
	m_pButton_Luk_Minus = NULL;

}

CNItemMallGiveStatus::~CNItemMallGiveStatus()
{		
	SAFE_DELETE(m_pStatic_ItemImage);
	SAFE_DELETE(m_pStatic_ItemExplain);
	SAFE_DELETE(m_pStatic_StatusExplain);
	SAFE_DELETE(m_pStatic_Str);
	SAFE_DELETE(m_pStatic_Vit);
	SAFE_DELETE(m_pStatic_Dex);
	SAFE_DELETE(m_pStatic_Hnd);
	SAFE_DELETE(m_pStatic_Mag);
	SAFE_DELETE(m_pStatic_Luk);
	SAFE_DELETE(m_pButton_Ok);
	SAFE_DELETE(m_pButton_Cancel);

	SAFE_DELETE(m_pOutLine);

	SAFE_DELETE(m_pButton_Str_Plus);
	SAFE_DELETE(m_pButton_Vit_Plus);
	SAFE_DELETE(m_pButton_Dex_Plus);
	SAFE_DELETE(m_pButton_Hnd_Plus);
	SAFE_DELETE(m_pButton_Mag_Plus);
	SAFE_DELETE(m_pButton_Luk_Plus);
	SAFE_DELETE(m_pButton_Str_Minus);
	SAFE_DELETE(m_pButton_Vit_Minus);
	SAFE_DELETE(m_pButton_Dex_Minus);
	SAFE_DELETE(m_pButton_Hnd_Minus);
	SAFE_DELETE(m_pButton_Mag_Minus);
	SAFE_DELETE(m_pButton_Luk_Minus);

}



void CNItemMallGiveStatus::Create()
{

	if(!IsCreate())
	{		
		CInterfaceFile file;

		file.LoadFile( TEXT("NInterface/Data/NItemMall/DLG_NItemMallGiveStatus.ddf"));
		file.CreatePopupDialog( this, NITEMMALL_GIVESTATUS_DLG,  TEXT("dialog_itemmallgivestatus"), StatiCNItemMallGiveStatusProc);		

		m_pStatic_ItemImage = new CStatic(this);
		m_pStatic_ItemExplain = new CStatic(this);
		m_pStatic_StatusExplain = new CStatic(this);
		m_pStatic_Str = new CStatic(this);
		m_pStatic_Vit = new CStatic(this);
		m_pStatic_Dex = new CStatic(this);
		m_pStatic_Hnd = new CStatic(this);
		m_pStatic_Mag = new CStatic(this);
		m_pStatic_Luk = new CStatic(this);
		m_pButton_Ok = new CButton(this);
		m_pButton_Cancel = new CButton(this);

		m_pOutLine = new COutLine(this);

		m_pButton_Str_Plus = new CButton(this);
		m_pButton_Vit_Plus = new CButton(this);
		m_pButton_Dex_Plus = new CButton(this);
		m_pButton_Hnd_Plus = new CButton(this);
		m_pButton_Mag_Plus = new CButton(this);
		m_pButton_Luk_Plus = new CButton(this);
		m_pButton_Str_Minus = new CButton(this);
		m_pButton_Vit_Minus = new CButton(this);
		m_pButton_Dex_Minus = new CButton(this);
		m_pButton_Hnd_Minus = new CButton(this);
		m_pButton_Mag_Minus = new CButton(this);
		m_pButton_Luk_Minus = new CButton(this);

		file.CreateControl(m_pStatic_ItemImage, NGIVESTATUS_STATIC_ITEMIMAGE,  TEXT("statictext_itemimage"));
		file.CreateControl(m_pStatic_ItemExplain, NGIVESTATUS_STATIC_ITEMEXPLAIN,  TEXT("statictext_itemexplain"));
		file.CreateControl(m_pStatic_StatusExplain, NGIVESTATUS_STATIC_STATUSEXPLAIN,  TEXT("statictext_statusexpain"));
		file.CreateControl(m_pStatic_Str, NGIVESTATUS_STATIC_STR,  TEXT("statictext_Str"));
		file.CreateControl(m_pStatic_Vit, NGIVESTATUS_STATIC_VIT,  TEXT("statictext_Vit"));
		file.CreateControl(m_pStatic_Dex, NGIVESTATUS_STATIC_DEX,  TEXT("statictext_Dex"));
		file.CreateControl(m_pStatic_Hnd, NGIVESTATUS_STATIC_HND,  TEXT("statictext_Hnd"));
		file.CreateControl(m_pStatic_Mag, NGIVESTATUS_STATIC_MAG,  TEXT("statictext_Mag"));
		file.CreateControl(m_pStatic_Luk, NGIVESTATUS_STATIC_LUK,  TEXT("statictext_Luk"));
		file.CreateControl(m_pButton_Ok, NGIVESTATUS_BUTTON_OK,  TEXT("button_OK"));
		file.CreateControl(m_pButton_Cancel, NGIVESTATUS_BUTTON_CANCEL,  TEXT("button_Cancel"));

		file.CreateControl(m_pOutLine, NGIVESTATUS_OUTLINE,  TEXT("outline_NONAME1"));

		file.CreateControl(m_pButton_Str_Plus, NGIVESTATUS_BUTTON_STR_PLUS,  TEXT("button_Str_Plus"));
		file.CreateControl(m_pButton_Vit_Plus, NGIVESTATUS_BUTTON_VIT_PLUS,  TEXT("button_Vit_Plus"));
		file.CreateControl(m_pButton_Dex_Plus, NGIVESTATUS_BUTTON_DEX_PLUS,  TEXT("button_Dex_Plus"));
		file.CreateControl(m_pButton_Hnd_Plus, NGIVESTATUS_BUTTON_HND_PLUS,  TEXT("button_Hnd_Plus"));
		file.CreateControl(m_pButton_Mag_Plus, NGIVESTATUS_BUTTON_MAG_PLUS,  TEXT("button_Mag_Plus"));
		file.CreateControl(m_pButton_Luk_Plus, NGIVESTATUS_BUTTON_LUK_PLUS,  TEXT("button_Luk_Plus"));
		file.CreateControl(m_pButton_Str_Minus, NGIVESTATUS_BUTTON_STR_MINUS,  TEXT("button_Str_Minus"));
		file.CreateControl(m_pButton_Vit_Minus, NGIVESTATUS_BUTTON_VIT_MINUS,  TEXT("button_Vit_Minus"));
		file.CreateControl(m_pButton_Dex_Minus, NGIVESTATUS_BUTTON_DEX_MINUS,  TEXT("button_Dex_Minus"));
		file.CreateControl(m_pButton_Hnd_Minus, NGIVESTATUS_BUTTON_HND_MINUS,  TEXT("button_Hnd_Minus"));
		file.CreateControl(m_pButton_Mag_Minus, NGIVESTATUS_BUTTON_MAG_MINUS,  TEXT("button_Mag_Minus"));
		file.CreateControl(m_pButton_Luk_Minus, NGIVESTATUS_BUTTON_LUK_MINUS,  TEXT("button_Luk_Minus"));

		m_pStatic_ItemImage->SetBKColorUse(true);
		m_pStatic_ItemImage->SetBorder(true);

		m_pStatic_ItemExplain->SetBKColorUse(true);
		m_pStatic_ItemExplain->SetBorder(true);

		m_pStatic_StatusExplain->SetBKColorUse(true);
		m_pStatic_StatusExplain->SetBorder(true);
		m_pStatic_StatusExplain->SetTextPrintOption(DT_LEFT);

		m_pStatic_ItemImage->SetFileName(TEXT("GIMg\\ITEMIMG0.SPR"));
		m_pStatic_ItemImage->SetFontIndex(0);

	}

}


void CALLBACK CNItemMallGiveStatus::StatiCNItemMallGiveStatusProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNItemMallGiveStatus *pThis = (CNItemMallGiveStatus*) pControllerMgr;
	pThis->NItemMallGiveStatusProc( nEvent, nControlID, pControl );
}

void CALLBACK CNItemMallGiveStatus::NItemMallGiveStatusProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{	
	cltClient *pclclient = (cltClient*)pclClient;
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	}
	return;
}


void CNItemMallGiveStatus::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = ( cltClient * )pclClient;


	switch( MsgIndex )
	{
	case 0:
		{
		}
		break;

	case 1:
		{		

		}
		break;
	}
	return;
}

