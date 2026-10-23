#include "NYesOrNoDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../Client/ninterface/NElixirDlg/NElixirUseDlg.h"

CNYesOrNoDlg::CNYesOrNoDlg()
{
	m_siCreateReason = NONE;
}


CNYesOrNoDlg::~CNYesOrNoDlg()
{
	Destroy();
}


void CNYesOrNoDlg::Init()
{

}


void CNYesOrNoDlg::Destroy()
{


}


void CNYesOrNoDlg::Create()
{
	if( IsCreate() )
	{
		return;
	}

	CInterfaceFile file;

	file.LoadFile("Ninterface/Data/NYesOrNoDlg/NYesOrNoDlg.ddf");

	SI32 controlindex = file.GetControlIndex( TEXT("dialog_yes_or_no") );
	file.m_control[ controlindex ].dialog.bClosable = false;

	file.CreatePopupDialog( this , NYESORNODLG, "dialog_yes_or_no" , StaticNYesOrNoDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	 NYESORNODLG_BUTTON_YES,	this),	NYESORNODLG_BUTTON_YES,		"button_yes");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	 NYESORNODLG_BUTTON_NO,		this),	NYESORNODLG_BUTTON_NO,		"button_no");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	 NYESORNODLG_EIDT_EXPLAIN,  this),	NYESORNODLG_EIDT_EXPLAIN,   "editbox_explain");
}


void CNYesOrNoDlg::Action()
{

}


void CALLBACK CNYesOrNoDlg::StaticNYesOrNoDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNYesOrNoDlg *pThis = (CNYesOrNoDlg *)pControllerMgr;
	pThis->NYesOrNoDlgProc( nEvent, nControlID, pControl );
}


void CALLBACK CNYesOrNoDlg::NYesOrNoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case NYESORNODLG_BUTTON_YES:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:	
				{
					Yes(); 
					DeleteDialog();
				}
				break;
			}		
		}
		break;
	case NYESORNODLG_BUTTON_NO:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:	
				{
					No(); 
					DeleteDialog();
				}
				break;
			}
		}
		break;
	}

}


void CNYesOrNoDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{


}

void CNYesOrNoDlg::SetVillageResidentInvite( SI32 _siCreateReason, NTCHARStringName _kCharName, SI32 _siPara1 )
{
	m_siCreateReason = _siCreateReason;
	m_siPara1		 = _siPara1;
	m_kCharName		 = _kCharName;

	NTCHARString256 kBuffer(GetTxtFromMgr(8917));
	kBuffer.Replace("#name#", _kCharName);	
	kBuffer.Replace("#villagename#", pclClient->pclVillageManager->GetName(_siPara1));

	m_InterfaceFactory.GetEdit(NYESORNODLG_EIDT_EXPLAIN)->SetText(kBuffer);
}

void CNYesOrNoDlg::SetHelperAlreadyQuestion( void )
{
	m_siCreateReason = REASON_HELPER_ALREADY_QUESTION;

	NTCHARString256 kBuffer( GetTxtFromMgr(9615) );

	m_InterfaceFactory.GetEdit(NYESORNODLG_EIDT_EXPLAIN)->SetText(kBuffer);
}

void CNYesOrNoDlg::SetElixirNew( void )
{
	m_siCreateReason = REASON_ELIXIR_SET;

	NTCHARString256 kBuffer( GetTxtFromMgr(9928) );

	m_InterfaceFactory.GetEdit(NYESORNODLG_EIDT_EXPLAIN)->SetText(kBuffer);
}

void CNYesOrNoDlg::SetHelpMe( SI32 _siCreateReason, NTCHARStringName _kCharName, SI32 _siPara1 )
{
	m_siCreateReason = _siCreateReason;
	m_siPara1		 = _siPara1;
	m_kCharName		 = _kCharName;

	NTCHARString256 kBuffer(GetTxtFromMgr(8922));
	kBuffer.Replace("#name#", _kCharName);	

	m_InterfaceFactory.GetEdit(NYESORNODLG_EIDT_EXPLAIN)->SetText(kBuffer);
}

void CNYesOrNoDlg::SetHelpYesorNo( SI32 _siCreateReason, SI32 _siPara1 )
{
	m_siCreateReason = _siCreateReason;
	m_siPara1		 = _siPara1;

	m_InterfaceFactory.GetEdit(NYESORNODLG_EIDT_EXPLAIN)->SetText("asdf");
}

void CNYesOrNoDlg::Yes( void )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch( m_siCreateReason )
	{
	case REASON_VILLAGE_RESIDENT_INVITE:
		{
			// 서버로 주민 등록 신청을 한다
			cltGameMsgRequest_ApplyResident2 clInfo( m_siPara1, m_kCharName );
			cltMsg clMsg( GAMEMSG_REQUEST_APPLYRESIDENT2, sizeof(cltGameMsgRequest_ApplyResident2), (BYTE*)&clInfo );
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		break;
	case REASON_HELPER_ALREADY_QUESTION:
		{
			// 이전 질문을 삭제하자.
			SI32 siPersonID = 0; // GM이 지우는 것때문에 자신이 올린 질문은 PersonID는 0으로 보낸다
			cltMsg clMsg( GAMEMSG_REQUEST_DEL_QUESTION, sizeof(SI32), (BYTE*)&siPersonID );
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		break;
	case REASON_ELIXIR_SET:
		{
			cltClient* pclclient = (cltClient *)pclClient;
			if( (NElixirUseDlg*)pclclient->m_pDialog[NELIXIR_USE_DLG] )
			{
				((NElixirUseDlg*)pclclient->m_pDialog[NELIXIR_USE_DLG])->ElixirStart();
			}
		}
		break;

	default:	
		{

		}
		break;
	}
}

void CNYesOrNoDlg::No( void )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch( m_siCreateReason )
	{
	case REASON_VILLAGE_RESIDENT_INVITE:
		{
			DeleteDialog();
		}
		break;
	case REASON_HELPER_ALREADY_QUESTION:
		{
			DeleteDialog();
		}
		break;
	case REASON_ELIXIR_SET:
		{
			DeleteDialog();
		}	
		break;


	default:	break;
	}
}

void CNYesOrNoDlg::SetText( TCHAR* pStr )
{
	m_InterfaceFactory.GetEdit(NYESORNODLG_EIDT_EXPLAIN)->SetText( pStr );
}