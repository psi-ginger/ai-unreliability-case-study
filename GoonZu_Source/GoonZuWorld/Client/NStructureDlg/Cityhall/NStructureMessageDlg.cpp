//#if defined(_NSTRUCTUREMESSAGEDLG)

#include <tchar.h>
#include "./NStructureMessageDlg.h" 

#include <CommonLogic.h>

#include "MsgType-Structure.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../../Client/InterfaceMgr/Interface/Edit.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

cltStructureMngBasic*	NStructureMessageDlg::m_pclStructureManager;
SI32					NStructureMessageDlg::m_siStrType;

extern cltCommonLogic* pclClient;

NStructureMessageDlg::NStructureMessageDlg()
{
	m_bShow = false;
	m_siStrType = 0;

	m_pEdit_message = NULL;
	m_pButton_set = NULL;  
	m_pButton_close = NULL;
}

NStructureMessageDlg::~NStructureMessageDlg()
{
	if(m_pEdit_message) delete m_pEdit_message;
	if(m_pButton_set) delete m_pButton_set;  
	if(m_pButton_close) delete m_pButton_close;
}

void NStructureMessageDlg::Create()
{
	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NStructureMessage/DLG_StructureMessage.ddf"));

		file.CreatePopupDialog( this, NSTRUCTUREMESSAGE_DLG, TEXT("dialog_structure_message"), NStructureMessageStaticProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_structure_message") );

		if(siTmp < 0 )
			siTmpWidth = DLG_BIG_POS_X;
		else
			siTmpWidth = file.m_control[ siTmp  ].width;

		if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
		{
			this->MovePos(DLG_BIG_POS_X - siTmpWidth, DLG_BIG_POS_Y);
		}
		else
		{
			this->MovePos(DLG_SMALL_POS_X - siTmpWidth, DLG_SMALL_POS_Y);
		}

		m_pEdit_message = new CEdit( this );    
		m_pButton_set = new CButton(this );
		m_pButton_close = new CButton( this);     

		file.CreateControl( 	m_pEdit_message	, 	NSTRUCTUREMESSAGE_EDITBOX_MESSAGE	, TEXT("editbox_message") )	;
		file.CreateControl( 	m_pButton_set	, 	NSTRUCTUREMESSAGE_BUTTON_SET	, TEXT("button_set") )	;
		file.CreateControl( 	m_pButton_close	, 	NSTRUCTUREMESSAGE_BUTTON_CLOSE	, TEXT("button_close") )	;

		m_pEdit_message->SetMaxEditTextLength( 512 );
		m_pEdit_message->SetAutoScroll( true );
		m_pclStructureManager = pclClient->pclCityHallManager;

		m_siStrType = RANKTYPE_CITYHALL;

		cltStrInfo *pStrInfo = &m_pclStructureManager->clClientStrInfo;
#ifdef _IAF_EXPORT
		if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
		{
			TCHAR ResultBuf[1024] = "";
			IAF::Filtering( pStrInfo->clCommonInfo.strNotice, ResultBuf, sizeof(ResultBuf) );
			memcpy( pStrInfo->clCommonInfo.strNotice, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
		}
#else
		pclClient->m_pCAbuseFilter->CheckAbuse(pStrInfo->clCommonInfo.strNotice);
#endif
		if( pStrInfo ) {			
			m_pEdit_message->SetText( pStrInfo->clCommonInfo.strNotice );
		}
	}
	else
	{
		DeleteDialog();
	}
}


void NStructureMessageDlg::NStructureMessageStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	NStructureMessageDlg* pThis = (NStructureMessageDlg*)pControllerMgr;
    pThis->NStructureMessageProc( nEvent, nControlID, pControl );
}

void NStructureMessageDlg::NStructureMessageProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pclStructureManager);
	cltClient *pclclient = ( cltClient * ) pclClient;
	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	//case NSTRUCTUREMESSAGE_EDITBOX_MESSAGE:
	//	{
	//		switch( nEvent )
	//		{
	//		case EVENT_EDITBOX_CHANGE:
	//			{
	//				SI16 pos = m_pEdit_message->GetYPos();
	//			}
	//		}

	//	}break;
	case NSTRUCTUREMESSAGE_BUTTON_SET:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buf[ MAX_VILLAGENOTICE_LENGTH ];
					SI32 Textsize = lstrlen( m_pEdit_message->GetText() );
					if ( Textsize > MAX_VILLAGENOTICE_LENGTH ){
						TCHAR* pTitle = GetTxtFromMgr(393); // 설정 실패 
						TCHAR* pText  = GetTxtFromMgr(8555);//
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );//실패 알림 창을 띄운다.
						return; 
					}
					
					GET_EDITBOX_TEXT( m_pEdit_message, buf, MAX_VILLAGENOTICE_LENGTH );
					SI16 pos = m_pEdit_message->GetYPos();// 커서 포지션을 가지고 온다
					SI16 siLineIndex = m_pEdit_message->GetLineIndexFromPos( pos); //  라인인덱스를 가지고 온뒤
					if(siLineIndex > 8)// 라인덱스가 8줄 이상이면 쓸수 없도록 한다.
					{
						TCHAR* pTitle = GetTxtFromMgr(393); // 설정 실패 
						TCHAR* pText  = GetTxtFromMgr(8555);//
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );//실패 알림 창을 띄운다.
						return; 
					}
					//GetDlgItemText( hDlg, IDC_EDIT_MESSAGE, buf, MAX_VILLAGENOTICE_LENGTH - 1 );
                    cltGameMsgRequest_SetStructureMessage sendMsg( m_pclStructureManager->siVillageUnique, m_siStrType, buf );
#ifdef _IAF_EXPORT
					if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
					{
						TCHAR ResultBuf[1024] = "";
						IAF::Filtering( sendMsg.szMessage, ResultBuf, sizeof(ResultBuf) );
						memcpy( sendMsg.szMessage, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
					}
#else					
					pclClient->m_pCAbuseFilter->CheckAbuse(sendMsg.szMessage);
#endif
					cltMsg clMsg( GAMEMSG_REQUEST_SETSTRUCTUREMESSAGE, sizeof( sendMsg ), (BYTE*)&sendMsg );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg );

					DeleteDialog();
				}
				break;
			}
		}
		break;
	case NSTRUCTUREMESSAGE_BUTTON_CLOSE:
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

//#endif