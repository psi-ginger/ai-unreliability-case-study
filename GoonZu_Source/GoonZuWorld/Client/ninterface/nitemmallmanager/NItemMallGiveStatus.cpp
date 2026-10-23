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
#include "Msg/MsgType-ItemMall.h"

#define DEFAULT_GIVESTATUS_BONUS 10

extern cltCommonLogic* pclClient;

CNItemMallGiveStatus::CNItemMallGiveStatus()
{
	m_pImageStatic = NULL;
	m_pEdit_ItemExplain = NULL;
	m_pEdit_StatusExplain = NULL;
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

	m_pStatic_Bonus = NULL;
	m_pEdit_Str = NULL;
	m_pEdit_Vit = NULL;
	m_pEdit_Dex = NULL;
	m_pEdit_Hnd = NULL;
	m_pEdit_Mag = NULL;
	m_pEdit_Luk = NULL;
	m_pEdit_Bonus = NULL;

	m_siitemid = 0;
	m_siitemnum = 0;
	m_siselectitem = 0;
 
}

CNItemMallGiveStatus::~CNItemMallGiveStatus()
{		
	SAFE_DELETE(m_pImageStatic);
	SAFE_DELETE(m_pEdit_ItemExplain);
	SAFE_DELETE(m_pEdit_StatusExplain);
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

	SAFE_DELETE(m_pStatic_Bonus);
	SAFE_DELETE(m_pEdit_Str);
	SAFE_DELETE(m_pEdit_Vit);
	SAFE_DELETE(m_pEdit_Dex);
	SAFE_DELETE(m_pEdit_Hnd);
	SAFE_DELETE(m_pEdit_Mag);
	SAFE_DELETE(m_pEdit_Luk);
	SAFE_DELETE(m_pEdit_Bonus);


}



void CNItemMallGiveStatus::Create()
{

	if(!IsCreate())
	{		
		CInterfaceFile file;

		file.LoadFile( TEXT("NInterface/Data/NItemMall/DLG_NItemMallGiveStatus.ddf"));
		file.CreatePopupDialog( this, NITEMMALL_GIVESTATUS_DLG,  TEXT("dialog_itemmallgivestatus"), StatiCNItemMallGiveStatusProc);		

		m_pImageStatic = new CImageStatic(this);
		m_pEdit_ItemExplain = new CEdit(this);
		m_pEdit_StatusExplain = new CEdit(this);
		m_pStatic_Str = new CStatic(this);
		m_pStatic_Vit = new CStatic(this);
		m_pStatic_Dex = new CStatic(this);
		m_pStatic_Hnd = new CStatic(this);
		m_pStatic_Mag = new CStatic(this);
		m_pStatic_Luk = new CStatic(this);
		m_pButton_Ok = new CButton(this);
		m_pButton_Cancel = new CButton(this);

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

		m_pStatic_Bonus = new CStatic(this);
		m_pEdit_Str = new CEdit(this);
		m_pEdit_Vit = new CEdit(this);
		m_pEdit_Dex = new CEdit(this);
		m_pEdit_Hnd = new CEdit(this);
		m_pEdit_Mag = new CEdit(this);
		m_pEdit_Luk = new CEdit(this);
		m_pEdit_Bonus = new CEdit(this);
		m_pOutLine = new COutLine(this);

		file.CreateControl(m_pImageStatic, NGIVESTATUS_STATIC_ITEMIMAGE,  TEXT("statictext_itemimage"));
		file.CreateControl(m_pEdit_ItemExplain, NGIVESTATUS_STATIC_ITEMEXPLAIN,  TEXT("editbox_itemexplain"));
		file.CreateControl(m_pEdit_StatusExplain, NGIVESTATUS_EDIT_STATUSEXPLAIN,  TEXT("editbox_statusexpain"));
		file.CreateControl(m_pStatic_Str, NGIVESTATUS_STATIC_STR,  TEXT("statictext_Str"));
		file.CreateControl(m_pStatic_Vit, NGIVESTATUS_STATIC_VIT,  TEXT("statictext_Vit"));
		file.CreateControl(m_pStatic_Dex, NGIVESTATUS_STATIC_DEX,  TEXT("statictext_Dex"));
		file.CreateControl(m_pStatic_Hnd, NGIVESTATUS_STATIC_HND,  TEXT("statictext_Hnd"));
		file.CreateControl(m_pStatic_Mag, NGIVESTATUS_STATIC_MAG,  TEXT("statictext_Mag"));
		file.CreateControl(m_pStatic_Luk, NGIVESTATUS_STATIC_LUK,  TEXT("statictext_Luk"));
		file.CreateControl(m_pButton_Ok, NGIVESTATUS_BUTTON_OK,  TEXT("button_OK"));
		file.CreateControl(m_pButton_Cancel, NGIVESTATUS_BUTTON_CANCEL,  TEXT("button_Cancel"));

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

		file.CreateControl(m_pStatic_Bonus, NGIVESTATUS_STATIC_BONUS, TEXT("statictext_bonus"));
		file.CreateControl(m_pEdit_Str, NGIVESTATUS_EDIT_STR, TEXT("editbox_Str"));
		file.CreateControl(m_pEdit_Vit, NGIVESTATUS_EDIT_DEX, TEXT("editbox_Vit"));
		file.CreateControl(m_pEdit_Dex, NGIVESTATUS_EDIT_MAG, TEXT("editbox_Dex"));
		file.CreateControl(m_pEdit_Hnd, NGIVESTATUS_EDIT_VIT, TEXT("editbox_Hnd"));
		file.CreateControl(m_pEdit_Mag, NGIVESTATUS_EDIT_HND, TEXT("editbox_Mag"));
		file.CreateControl(m_pEdit_Luk, NGIVESTATUS_EDIT_LUK, TEXT("editbox_Luk"));
		file.CreateControl(m_pEdit_Bonus, NGIVESTATUS_EDIT_BONUS, TEXT("editbox_bonus"));

		file.CreateControl(m_pOutLine, NGIVESTATUS_OUTLINE,  TEXT("outline_NONAME1"));

		m_pStatic_Str->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatic_Dex->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatic_Mag->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatic_Vit->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatic_Hnd->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatic_Luk->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE)	;
		m_pStatic_Bonus->SetTextPrintOption(DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		m_pStatic_Str->Refresh();
		m_pStatic_Dex->Refresh();
		m_pStatic_Mag->Refresh();
		m_pStatic_Vit->Refresh();
		m_pStatic_Hnd->Refresh();
		m_pStatic_Luk->Refresh();
		m_pStatic_Bonus->Refresh();
		
		m_pEdit_ItemExplain->SetBKColorUse(true);
		m_pEdit_ItemExplain->SetBorder(true);

		m_pEdit_StatusExplain->SetBKColorUse(true);
		m_pEdit_StatusExplain->SetBorder(true);
		TCHAR *pText = GetTxtFromMgr(6109);
		m_pEdit_StatusExplain->SetText(pText );
		m_pEdit_StatusExplain->SetTextPrintOption( DT_CENTER | DT_VCENTER );
		m_pEdit_StatusExplain->Refresh();

		TCHAR buf[16] = TEXT("0");

		m_pEdit_Str->SetText(buf);
		m_pEdit_Vit->SetText(buf);
		m_pEdit_Dex->SetText(buf);
		m_pEdit_Hnd->SetText(buf);
		m_pEdit_Mag->SetText(buf);
		m_pEdit_Luk->SetText(buf);
		StringCchPrintf(buf, 16, "%d", DEFAULT_GIVESTATUS_BONUS);
		m_pEdit_Bonus->SetText(buf);

		Show(false);

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
	case NGIVESTATUS_BUTTON_CANCEL:
		{
			DeleteDialog();
		}
		break;
	case NGIVESTATUS_BUTTON_STR_PLUS:
	case NGIVESTATUS_BUTTON_DEX_PLUS:
	case NGIVESTATUS_BUTTON_MAG_PLUS:
	case NGIVESTATUS_BUTTON_VIT_PLUS:
	case NGIVESTATUS_BUTTON_HND_PLUS:
	case NGIVESTATUS_BUTTON_LUK_PLUS:
	case NGIVESTATUS_BUTTON_STR_MINUS:
	case NGIVESTATUS_BUTTON_DEX_MINUS:
	case NGIVESTATUS_BUTTON_MAG_MINUS:
	case NGIVESTATUS_BUTTON_VIT_MINUS:
	case NGIVESTATUS_BUTTON_HND_MINUS:
	case NGIVESTATUS_BUTTON_LUK_MINUS:
		{
			CalcBonus(nControlID);
		}
		break;

	case NGIVESTATUS_BUTTON_OK:
		{
			SI32 siBonus = -1;
			siBonus = _tstoi(m_pEdit_Bonus->GetText());

			if (siBonus > 0)
			{
				TCHAR *pTitle = GetTxtFromMgr(836);
				TCHAR *pText = GetTxtFromMgr(6110);

				stMsgBox MsgBox;
				MsgBox.Set( this,  pTitle, pText, MSGBOX_TYPE_OK, 0 );
				pclclient->SetMsgBox( &MsgBox, NULL, 0 );	

			}
			else if (siBonus == 0)
			{
				TCHAR *pTitle = GetTxtFromMgr(3889);
				TCHAR *pText = GetTxtFromMgr(6111);

				stMsgBox MsgBox;
				MsgBox.Set( this,  pTitle, pText, MSGBOX_TYPE_YESNO, 1 );
				pclclient->SetMsgBox( &MsgBox, NULL, 0 );	

			}

		}
		break;

	}
	return;
}

bool CNItemMallGiveStatus::SetSelectedItem(SI32 itemid, SI32 itemnum, SI32 selectitem)
{
	cltClient *pclclient = (cltClient*)pclClient;

	cltItem item = pclclient->pclCM->CR[1]->pclCI->clItemMall.GetItem(selectitem);

	m_siitemid = itemid;
	m_siitemnum = itemnum;
	m_siselectitem = selectitem;

	SI32 itemunique = item.siUnique;

	if(itemunique)
	{
		TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
		pclClient->pclItemManager->GetExplain( &item, ToolTipMsg);

		if (ToolTipMsg)
		{
			m_pEdit_ItemExplain->SetText(ToolTipMsg);
		}

		m_pEdit_ItemExplain->Refresh();

		Show(true);

		return true;

	}
	else
		return false;


}

void CNItemMallGiveStatus::CalcBonus(SI32 buttonindex)
{
	SI32 siBonus = -1, siValue = -1;
	TCHAR buf[16] = TEXT("0");

	CEdit *pEdit_Value = NULL;
	bool bPlus = false;

	switch (buttonindex)
	{
	case NGIVESTATUS_BUTTON_STR_PLUS:
		pEdit_Value = m_pEdit_Str;
		bPlus = true;
		break;
	case NGIVESTATUS_BUTTON_DEX_PLUS:
		pEdit_Value = m_pEdit_Dex;
		bPlus = true;
		break;
	case NGIVESTATUS_BUTTON_MAG_PLUS:
		pEdit_Value = m_pEdit_Mag;
		bPlus = true;
		break;
	case NGIVESTATUS_BUTTON_VIT_PLUS:
		pEdit_Value = m_pEdit_Vit;
		bPlus = true;
		break;
	case NGIVESTATUS_BUTTON_HND_PLUS:
		pEdit_Value = m_pEdit_Hnd;
		bPlus = true;
		break;
	case NGIVESTATUS_BUTTON_LUK_PLUS:
		pEdit_Value = m_pEdit_Luk;
		bPlus = true;
		break;
	case NGIVESTATUS_BUTTON_STR_MINUS:
		pEdit_Value = m_pEdit_Str;
		bPlus = false;
		break;
	case NGIVESTATUS_BUTTON_DEX_MINUS:
		pEdit_Value = m_pEdit_Dex;
		bPlus = false;
		break;
	case NGIVESTATUS_BUTTON_MAG_MINUS:
		pEdit_Value = m_pEdit_Mag;
		bPlus = false;
		break;
	case NGIVESTATUS_BUTTON_VIT_MINUS:
		pEdit_Value = m_pEdit_Vit;
		bPlus = false;
		break;
	case NGIVESTATUS_BUTTON_HND_MINUS:
		pEdit_Value = m_pEdit_Hnd;
		bPlus = false;
		break;
	case NGIVESTATUS_BUTTON_LUK_MINUS:
		pEdit_Value = m_pEdit_Luk;
		bPlus = false;
		break;

	}

	if (pEdit_Value)
	{
		siBonus = _tstoi(m_pEdit_Bonus->GetText());
		siValue = _tstoi(pEdit_Value->GetText());

		if (bPlus)
		{
			if (siBonus >= 5)
			{
				siBonus = siBonus - 5;
				siValue = siValue + 5;

				StringCchPrintf(buf, 16, "%d", siBonus);
				m_pEdit_Bonus->SetText(buf);

				StringCchPrintf(buf, 16, "%d", siValue);
				pEdit_Value->SetText(buf);

				m_pEdit_Bonus->Refresh();
				pEdit_Value->Refresh();

			}
		}
		else
		{
			if (siBonus < 10 && siValue >= 5)
			{
				siBonus = siBonus + 5;
				siValue = siValue - 5;

				StringCchPrintf(buf, 16, "%d", siBonus);
				m_pEdit_Bonus->SetText(buf);

				StringCchPrintf(buf, 16, "%d", siValue);
				pEdit_Value->SetText(buf);

				m_pEdit_Bonus->Refresh();
				pEdit_Value->Refresh();

			}
		}

	}


}


void CNItemMallGiveStatus::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = ( cltClient * )pclClient;


	switch( MsgIndex )
	{
	case 1:
		{
			if (RetValue)
			{
				cltItem item = pclclient->pclCM->CR[1]->pclCI->clItemMall.GetItem(m_siselectitem);
				item.siItemNum = m_siitemnum;

				UI16 uiStr = 0, uiDex = 0, uiMag = 0, uiVit = 0, uiHnd = 0, uiLuk = 0;

				uiStr = _tstoi(m_pEdit_Str->GetText());
				uiDex = _tstoi(m_pEdit_Dex->GetText());
				uiMag = _tstoi(m_pEdit_Mag->GetText());
				uiVit = _tstoi(m_pEdit_Vit->GetText());
				uiHnd = _tstoi(m_pEdit_Hnd->GetText());
				uiLuk = _tstoi(m_pEdit_Luk->GetText());

				cltGameMsgRequest_ItemMallGetItem clinfo(m_siitemid,m_siselectitem, &item , 
										uiStr, uiDex, uiMag, uiVit, uiHnd, uiLuk);
				cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLGETITEM ,sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);	

				DeleteDialog();

			}

		}
		break;
	}
	return;
}

void CNItemMallGiveStatus::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{


}

void CNItemMallGiveStatus::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient *pclclient = (cltClient*)pclClient;

	cltItem item = pclclient->pclCM->CR[1]->pclCI->clItemMall.GetItem(m_siselectitem);
	SI32 itemunique = item.siUnique;
	SI32 siImageType = 0;
	SI32 siImageIndex = 0;

	if (itemunique > 0)
	{
		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

		if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( itemunique, &siImageType, &siImageIndex ) == false)
			return;

		if(siImageIndex < 0) 
			return;
		cltClient *pclclient = ( cltClient * )pclClient;
		TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( siImageType );

		if ( pSpr )
		{
			GP.PutSpr(pSpr, ScreenRX + GetX() + 27, ScreenRY + GetY() + 52, siImageIndex );
		}

	}

}


