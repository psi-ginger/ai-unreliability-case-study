#include "NChangeCharName.h"

#include "Char/CharCommon/Char-Common.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/Msg/MsgType-ItemMall.h"
#include "../../Client/Client.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "..\..\Common\JWLib\IsCorrectID\IsCorrectID.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"



extern cltCommonLogic* pclClient;

CNChangeCharName::CNChangeCharName()
{	
	ZeroMemory(m_pStrName, MAX_PLAYER_NAME);
	m_siSlotIndex = 0;
}

CNChangeCharName::~CNChangeCharName()
{
	
}


void CNChangeCharName::Create()
{
	if (IsCreate() == false)
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/DLG_NTempChangeName.ddf"));
		file.CreatePopupDialog( this, NCHANGECHARNAME_DLG , TEXT("dialog_NTempChangeName"), StaticNChangeCharNameProc );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	STATIC_TEMPCHANGENAME_INFO, this ),	STATIC_TEMPCHANGENAME_INFO,	"statictext_NTempChangeName_Info"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	EDIT_TEMPCHANGENAME_INPUT,	this ),	EDIT_TEMPCHANGENAME_INPUT,	"editbox_NTempChangeName_Name"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	BUTTON_TEMPCHANGENAME_OK,	this ),	BUTTON_TEMPCHANGENAME_OK,	"button_NTempChangeName_OK"); 

		m_InterfaceFactory.GetEdit(STATIC_TEMPCHANGENAME_INFO)->SetText(GetTxtFromMgr(40188));
		m_InterfaceFactory.GetEdit(EDIT_TEMPCHANGENAME_INPUT)->SetMaxEditTextLength(8*2+1); // 8글자 이상 입력못하게
	}
	else
        DeleteDialog();
}

void CALLBACK CNChangeCharName::StaticNChangeCharNameProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNChangeCharName *pThis = (CNChangeCharName*) pControllerMgr;
	pThis->NChangeCharNameProc( nEvent, nControlID, pControl );
}


// 동일한 캐릭터명이 있는경우
void CNChangeCharName::SameName()
{
	m_InterfaceFactory.GetEdit(STATIC_TEMPCHANGENAME_INFO)->SetText(GetTxtFromMgr(40189));
}

void CALLBACK CNChangeCharName::NChangeCharNameProc( UINT nEvent, int nControlID, CControllerObj* pControl )
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

	case BUTTON_TEMPCHANGENAME_OK:
		{
			if(m_pStrName == NULL)
				return;

			TCHAR* pText = m_InterfaceFactory.GetEdit(EDIT_TEMPCHANGENAME_INPUT)->GetText();

			if(pText == NULL)
				return;

			// 욕설 체크
			pClient->m_pCAbuseFilter->CheckAbuse(pText);
						
			if(IsCorrectID(pText) == TRUE)
			{
				 //욕설이 없는 경우
				cltGameMsgRequest_ChangeCharName clinfo((TCHAR*)pText, false);
				clinfo.m_siSlotIndex = m_siSlotIndex;

				cltMsg clMsg(GAMEMSG_REQUEST_CHANGE_CHARNAME, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);		
				
			}
			else
			{
				// 욕설이 있는 경우
				CEdit* pEdit = m_InterfaceFactory.GetEdit(STATIC_TEMPCHANGENAME_INFO);
				
				if(pEdit == NULL)
					return;

				pEdit->SetText(GetTxtFromMgr(1115));
			}
		}
		break;
	}
}
