#include "NHalloweenSpiritBox.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "Char/CharCommon/Char-Common.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/MsgType-HalloweenSpiritBox.h"
#include "../../Client/Client.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

extern cltCommonLogic* pclClient;

NHalloweenSpiritBoxDlg::NHalloweenSpiritBoxDlg()
{	
	for(SI08 siIndex = 0; siIndex < HALLOWEENSPIRITBOX_END; siIndex++)
	{
		m_siImageType[siIndex]	= -1;
		m_siImageIndex[siIndex]	= -1;
		m_siItemUnique[siIndex] = -1;
	}

	m_siSelectedIndex = 0;
}

NHalloweenSpiritBoxDlg::~NHalloweenSpiritBoxDlg()
{

}


void NHalloweenSpiritBoxDlg::Create()
{
	cltClient *pClient = (cltClient*)pclClient;

	if(pClient == NULL)
		return;

	if (IsCreate() == false)
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NHalloweenSpiritBox/DLG_NHalloweenSpiritBox.ddf"));
		file.CreatePopupDialog( this, NHALLOWEENSPIRITBOX_DLG, TEXT("dialog_halloweenspiritbox"), StaticNHalloweenSpiritBoxDlgProc );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		NHALLOWEENSPIRIT_BTN_OK,		this ), NHALLOWEENSPIRIT_BTN_OK,		"button_getitem");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC, NHALLOWEENSPIRIT_IMAGE_ITEM1,	this ), NHALLOWEENSPIRIT_IMAGE_ITEM1,	"imagestatic_item1");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC, NHALLOWEENSPIRIT_IMAGE_ITEM2,	this ), NHALLOWEENSPIRIT_IMAGE_ITEM2,	"imagestatic_item2");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NHALLOWEENSPIRIT_EDIT,			this ), NHALLOWEENSPIRIT_EDIT,			"editbox_memo");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NHALLOWEENSPIRIT_ITEMNAME1,			this ), NHALLOWEENSPIRIT_ITEMNAME1,			"editbox_itemname1");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NHALLOWEENSPIRIT_ITEMNAME2,			this ), NHALLOWEENSPIRIT_ITEMNAME2,			"editbox_itemname2");
	

		CEdit* pEditDescription = m_InterfaceFactory.GetEdit(NHALLOWEENSPIRIT_EDIT);

		if(pEditDescription != NULL)
			pEditDescription->SetText(GetTxtFromMgr(40163));

		//m_siItemUnique[HALLOWEENSPIRITBOX_1] = 13626; // 근거리
		//m_siItemUnique[HALLOWEENSPIRITBOX_2] = 13627; // 원거리

		//

		//m_InterfaceFactory.GetEdit(NHALLOWEENSPIRIT_ITEMNAME1)->SetText((TCHAR*)(pClient->pclItemManager->GetName(m_siItemUnique[HALLOWEENSPIRITBOX_1])));
		//m_InterfaceFactory.GetEdit(NHALLOWEENSPIRIT_ITEMNAME2)->SetText((TCHAR*)(pClient->pclItemManager->GetName(m_siItemUnique[HALLOWEENSPIRITBOX_2])));

		//for(SI08 siIndex = 0; siIndex < HALLOWEENSPIRITBOX_END; siIndex++)
		//{
		//	if(FALSE == pclClient->pclItemManager->GetItemGImageFileIndexAndFont(m_siItemUnique[siIndex], &m_siImageType[siIndex], &m_siImageIndex[siIndex]))
		//	{
		//		_ASSERT(0);
		//		pclClient->pclLog->LogPrint("[nhalloweenspiritbox.cpp]	아이템 이미지를 찾을 수 없습니다.");
		//	}
		//}
	}
	else
		DeleteDialog();
}

void CALLBACK NHalloweenSpiritBoxDlg::StaticNHalloweenSpiritBoxDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NHalloweenSpiritBoxDlg *pThis = (NHalloweenSpiritBoxDlg*) pControllerMgr;
	pThis->NHalloweenSpiritBoxDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NHalloweenSpiritBoxDlg::NHalloweenSpiritBoxDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pClient = (cltClient*)pclClient;

	if(pClient == NULL)
		return;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NHALLOWEENSPIRIT_IMAGE_ITEM1:
		{
			switch(nEvent)
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_siSelectedIndex = HALLOWEENSPIRITBOX_1;
				}
				break;
			}
		}
		break;

	case NHALLOWEENSPIRIT_IMAGE_ITEM2:
		{
			switch(nEvent)
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_siSelectedIndex = HALLOWEENSPIRITBOX_2;
				}
				break;
			}
		}
		break;

	case NHALLOWEENSPIRIT_BTN_OK:
		{
			cltGameRequest_HalloweenSpiritBox HalloweenSpiritBox(m_siSpritBoxUnique, ITEMUNIQUE(m_siItemUnique[m_siSelectedIndex]));
			cltMsg clMsg(GAMEMSG_REQUEST_HALLOWEENSPIRITBOX_OK, sizeof(cltGameRequest_HalloweenSpiritBox), (BYTE*)&HalloweenSpiritBox);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

			DeleteDialog();
		}
		break;
	}
}


void NHalloweenSpiritBoxDlg::DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY)
{	
	cltClient* pClient = (cltClient*)pclClient;

	if(pClient == NULL)
		return;

	TSpr *pSpr = NULL;

	pSpr =  pClient->pclGImg->GetGlobalSpr( m_siImageType[HALLOWEENSPIRITBOX_1] );
	if(pSpr)
	{
		GP.PutSpr(pSpr,	ScreenRX +  GetX() + m_InterfaceFactory.GetImageStatic(NHALLOWEENSPIRIT_IMAGE_ITEM1)->GetX() + 10,
						ScreenRY +  GetY() + m_InterfaceFactory.GetImageStatic(NHALLOWEENSPIRIT_IMAGE_ITEM1)->GetY(),
						m_siImageIndex[HALLOWEENSPIRITBOX_1] );
	}

	pSpr =  pClient->pclGImg->GetGlobalSpr( m_siImageType[HALLOWEENSPIRITBOX_2] );
	if(pSpr)
	{
		GP.PutSpr(pSpr,	ScreenRX +  GetX() + m_InterfaceFactory.GetImageStatic(NHALLOWEENSPIRIT_IMAGE_ITEM2)->GetX() + 10,
						ScreenRY +  GetY() + m_InterfaceFactory.GetImageStatic(NHALLOWEENSPIRIT_IMAGE_ITEM2)->GetY(),
						m_siImageIndex[HALLOWEENSPIRITBOX_2] );
	}

	CImageStatic*	pImageStatic	= NULL;
	CEdit*			pEditBox		= NULL;

	if(m_siSelectedIndex == HALLOWEENSPIRITBOX_1)
	{
		pImageStatic	= m_InterfaceFactory.GetImageStatic(NHALLOWEENSPIRIT_IMAGE_ITEM1);
		pEditBox		= m_InterfaceFactory.GetEdit(NHALLOWEENSPIRIT_ITEMNAME1);
	}
	else if(m_siSelectedIndex == HALLOWEENSPIRITBOX_2)
	{
		pImageStatic	= m_InterfaceFactory.GetImageStatic(NHALLOWEENSPIRIT_IMAGE_ITEM2);
		pEditBox		= m_InterfaceFactory.GetEdit(NHALLOWEENSPIRIT_ITEMNAME2);
	}

	if(pImageStatic == NULL)
		return;

	SI32 siX1 = pImageStatic->GetX() + GetX() + ScreenRX;
	SI32 siY1 = pImageStatic->GetY() + GetY() + ScreenRY;
	SI32 siX2 = siX1 + pImageStatic->GetWidth();
	SI32 siY2 = siY1 + pImageStatic->GetHeight();

	GP.Box(siX1-1, siY1-1, siX2-1, siY2-1, 41);

	siX1 = pEditBox->GetX() + GetX() + ScreenRX;
	siY1 = pEditBox->GetY() + GetY() + ScreenRY;
	siX2 = siX1 + pEditBox->GetWidth();
	siY2 = siY1 + pEditBox->GetHeight();

	GP.Box(siX1-1, siY1-1, siX2-1, siY2-1, 41);




	//pSpr =  pClient->pclGImg->GetGlobalSpr( m_siImageType[HALLOWEENSPIRITBOX_2] );
	//if ( pSpr )
	//{
	//	GP.PutSpr(pSpr,	ScreenRX +  GetX() + m_InterfaceFactory.GetImageStatic(NHALLOWEENSPIRIT_IMAGE_ITEM2)->GetX() + 10,
	//					ScreenRY +  GetY() + m_InterfaceFactory.GetImageStatic(NHALLOWEENSPIRIT_IMAGE_ITEM2)->GetY(),
	//					m_siImageIndex[HALLOWEENSPIRITBOX_2] );
	//}
}
void NHalloweenSpiritBoxDlg::SetSpritItmeUnique(SI32 siItemUnique)
{
	cltClient *pClient = (cltClient*)pclClient;

	if(pClient == NULL)	return;
	// 박스 아이템에 때라서 해당 스피릿 유니크를 생성 한다.

	switch( siItemUnique ) 
	{
	case ITEMUNIQUE(13356): // 할로윈 스피릿 상자
		{
			m_siSpritBoxUnique	=	siItemUnique	;
			m_siItemUnique[HALLOWEENSPIRITBOX_1] = 13626; // 근거리
			m_siItemUnique[HALLOWEENSPIRITBOX_2] = 13627; // 원거리
		}break;
	case ITEMUNIQUE(13357): // 눈의 여왕 사스콰치 스피릿 상자
		{
			m_siSpritBoxUnique	=	siItemUnique	;
			m_siItemUnique[HALLOWEENSPIRITBOX_1] = 13656; // 근거리
			m_siItemUnique[HALLOWEENSPIRITBOX_2] = 13657; // 원거리
		}
		break;
	case ITEMUNIQUE(13338):	// 크리스마스 스피릿 상자
		{
			m_siSpritBoxUnique	=	siItemUnique	;
			m_siItemUnique[HALLOWEENSPIRITBOX_1] = 13633; // 근거리
			m_siItemUnique[HALLOWEENSPIRITBOX_2] = 13632; // 원거리
		}
		break;
	case ITEMUNIQUE(13343):	// 투사 스피릿 상자
		{
			m_siSpritBoxUnique	=	siItemUnique	;
			m_siItemUnique[HALLOWEENSPIRITBOX_1] = 13641; // 근거리
			m_siItemUnique[HALLOWEENSPIRITBOX_2] = 13642; // 원거리
		}
		break;
	case ITEMUNIQUE(13352):	// 요정 스피릿 상자
		{
			m_siSpritBoxUnique	=	siItemUnique	;
			m_siItemUnique[HALLOWEENSPIRITBOX_1] = 13652; // 근거리
			m_siItemUnique[HALLOWEENSPIRITBOX_2] = 13653; // 원거리
		}
		break;
	case ITEMUNIQUE(13317):	// 닌자 스피릿 상자
		{
			m_siSpritBoxUnique	=	siItemUnique	;
			m_siItemUnique[HALLOWEENSPIRITBOX_1] = 13615; // 근거리
			m_siItemUnique[HALLOWEENSPIRITBOX_2] = 13616; // 원거리
		}
		break;
	case ITEMUNIQUE(13330):	// 고양이 스피릿 상자
		{
			m_siSpritBoxUnique	=	siItemUnique	;
			m_siItemUnique[HALLOWEENSPIRITBOX_1] = 13623; // 근거리
			m_siItemUnique[HALLOWEENSPIRITBOX_2] = 13624; // 원거리
		}
		break;
	default:
		{
		}
	}
	m_InterfaceFactory.GetEdit(NHALLOWEENSPIRIT_ITEMNAME1)->SetText((TCHAR*)(pClient->pclItemManager->GetName(m_siItemUnique[HALLOWEENSPIRITBOX_1])));
	m_InterfaceFactory.GetEdit(NHALLOWEENSPIRIT_ITEMNAME2)->SetText((TCHAR*)(pClient->pclItemManager->GetName(m_siItemUnique[HALLOWEENSPIRITBOX_2])));

	for(SI08 siIndex = 0; siIndex < HALLOWEENSPIRITBOX_END; siIndex++)
	{
		if(FALSE == pclClient->pclItemManager->GetItemGImageFileIndexAndFont(m_siItemUnique[siIndex], &m_siImageType[siIndex], &m_siImageIndex[siIndex]))
		{
			_ASSERT(0);
			pclClient->pclLog->LogPrint("[nhalloweenspiritbox.cpp]	아이템 이미지를 찾을 수 없습니다.");
		}
	}
}