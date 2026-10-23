//---------------------------------
// 2004/9/15 성준엽
//---------------------------------
/* ==========================================================================
	클래스 :

	작성일 :		05/03/17
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#include "..\..\Common\JWLib\IsCorrectID\IsCorrectID.h"
#include "Char\CharManager\CharManager.h"

#include "..\Client\Client.h"
#include "./NCharNameChange.h"
#include "..\resource.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"

#include "MsgType-Person.h"
#include "Msg/MsgType-ItemMall.h"

#include "../../Client/InterfaceMgr/Interface/Button.h"
#include "../../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../../Client/InterfaceMgr/Interface/Static.h"
#include "../../Client/InterfaceMgr/Interface/Edit.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "abusefilter/AbuseFilter.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"

extern cltCommonLogic* pclClient;

cltNCharNameChange::cltNCharNameChange()
{
	m_DlgRectInitSwitch = FALSE;

	m_pEditbox_explain	 = NULL; 
	m_pStatictext_changename	 = NULL; 
	m_pEditbox_changename	 = NULL; 
	m_pButton_changename	 = NULL; 
	m_pButton_cancel	 = NULL; 

}

cltNCharNameChange::~cltNCharNameChange()
{
	m_DlgRectInitSwitch = FALSE;

	if( 	m_pEditbox_explain	 )  	delete 	m_pEditbox_explain	;
	if( 	m_pStatictext_changename	 )  	delete 	m_pStatictext_changename	;
	if( 	m_pEditbox_changename	 )  	delete 	m_pEditbox_changename	;
	if( 	m_pButton_changename	 )  	delete 	m_pButton_changename	;
	if( 	m_pButton_cancel	 )  	delete 	m_pButton_cancel	;

}

void cltNCharNameChange::Create()
{
	cltClient* pclclient = (cltClient*)pclClient;

	// 대화상자가 없는 상태라면 만든다. 
	if( ! IsCreate() )
	{
		//CDialogController::Create( CLTNCHARNAMECHANGE_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, NCharNameChangeStaticProc, TEXT("이름 변경하기"), TEXT("NInterface/Interface/Interface_20X20_00_109.Spr"), 9, true, 80, 100, 380,240, true, true, true);

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NChangeNameDlg/DLG_NCharStatus_NChangeName.ddf"));
		file.CreatePopupDialog( this, CLTNCHARNAMECHANGE_DLG, TEXT("dialog_changename"), NCharNameChangeStaticProc );

		m_pEditbox_explain	 = new 	CEdit	( this )	;
		m_pStatictext_changename	 = new 	CStatic	( this )	;
		m_pEditbox_changename	 = new 	CEdit	( this )	;
		m_pButton_changename	 = new 	CButton	( this )	;
		m_pButton_cancel	 = new 	CButton	( this )	;

		file.CreateControl( m_pEditbox_explain, CLTNCHARNAMECHANGE_DLG_EDITBOX_EXPLAIN, TEXT("editbox_explain") );
		file.CreateControl( m_pStatictext_changename, CLTNCHARNAMECHANGE_DLG_STATICTEXT_CHANGENAME, TEXT("statictext_changename") );
		file.CreateControl( m_pEditbox_changename, CLTNCHARNAMECHANGE_DLG_EDITBOX_CHANGENAME, TEXT("editbox_changename") );
		file.CreateControl( m_pButton_changename, CLTNCHARNAMECHANGE_DLG_BUTTON_CHANGENAME, TEXT("button_changename") );
		file.CreateControl( m_pButton_cancel, CLTNCHARNAMECHANGE_DLG_BUTTON_CANCEL, TEXT("button_cancel") );


		TCHAR* pText = NULL;
		TCHAR* pTitle = GetTxtFromMgr(3719);
		SetTitle( pTitle);

		pText= GetTxtFromMgr(3720);
		m_pStatictext_changename->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pText= GetTxtFromMgr(3721);
		m_pButton_changename->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pText= GetTxtFromMgr(3902);
		m_pButton_cancel->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);



		
		TCHAR buffer[1024] = TEXT("") ;
		pText = GetTxtFromMgr(563);
		StringCchPrintf(buffer, 1024, pText,pclclient->pclCM->CR[1]->GetName());

		//m_pEditbox_explain->SetBorder( true );
        m_pEditbox_explain->SetText( buffer );
	}
	else
	{
		DeleteDialog();
	}
}

void cltNCharNameChange::Close()
{
	DeleteDialog();
}

void CALLBACK cltNCharNameChange::NCharNameChangeStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	cltNCharNameChange* pThis = (cltNCharNameChange*)pControllerMgr;
	pThis->NCharNameChangeProc( nEvent, nControlID, pControl );
}

void CALLBACK cltNCharNameChange::NCharNameChangeProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			m_DlgRectInitSwitch = TRUE;
			Close();
		}
		break;
	case CLTNCHARNAMECHANGE_DLG_BUTTON_CHANGENAME:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltClient *pclclient = (cltClient *)pclClient;
					TCHAR buffer[256] = TEXT("") ;
					TCHAR szName[MAX_PLAYER_NAME] = TEXT("") ;

					SI32 itemunique = 24114 ;  // 이름변경이용권
					if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
					{
						TCHAR* pText = GetTxtFromMgr(564);
						StringCchPrintf(buffer, 256, pText);
						
						pText = GetTxtFromMgr(565);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
						return ;
					}

                    TCHAR *pTmpChar = NULL;
					SI16 tmpLen = -1;
					pTmpChar = m_pEditbox_changename->GetText();

					if(pTmpChar == NULL )
						return;

					tmpLen = _tcslen( pTmpChar );

					//cyj 캐릭이름에도 욕설필터 적용
					TCHAR buf[ MAX_PLAYER_NAME ];
					StringCchPrintf(buf, MAX_PLAYER_NAME, szName);
#ifdef _IAF_EXPORT
					if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
					{
						TCHAR ResultBuf[1024] = "";
						IAF::Filtering( buf, ResultBuf, sizeof(ResultBuf), IAF::FILTER_CHARACTER );
						memcpy( buf, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
					}
#else
					pclclient->m_pCAbuseFilter->CheckAbuse(buf);
#endif

					SI32 siMaxNameLength = 0;
					if ( pclClient->siServiceArea == ConstServiceArea_China )
					{
						siMaxNameLength = 12;
					}
					else
					{
						siMaxNameLength = 16;
					}

					if(tmpLen <= 0 || tmpLen > siMaxNameLength || !IsCorrectID(buf) )
					{
						TCHAR* pText = GetTxtFromMgr(566);
						StringCchPrintf(buffer, 256, pText,siMaxNameLength/2);
						
						pText = GetTxtFromMgr(567);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
						return ;
					}

					StringCchCopy( szName, MAX_PLAYER_NAME, pTmpChar );

					TCHAR* pText = GetTxtFromMgr(568);
					StringCchCopy(buffer, 256, pText);

					pText = GetTxtFromMgr(565);

					stMsgBox MsgBox;
					MsgBox.Set( this, pText, buffer, MSGBOX_TYPE_YESNO, 0 );
					BYTE TempBuffer[ 128 ];
					SI16 Size = sizeof( szName );
					memcpy( TempBuffer, szName, Size );
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

					//if (  MessageBox(NULL, buffer,pText,
					//	MB_ICONQUESTION | MB_YESNO) == IDNO )
					//{
					//	return ;
					//}

					//cltGameMsgRequest_ChangeCharName clinfo((TCHAR*)szName);
					//cltMsg clMsg(GAMEMSG_REQUEST_CHANGE_CHARNAME, sizeof(clinfo), (BYTE*)&clinfo);
					//pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					//DeleteDialog();
				
				}
				break;
			}
		}
		break;
	case CLTNCHARNAMECHANGE_DLG_BUTTON_CANCEL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				DeleteDialog();
				break;
			}
		}
		break; ;
	}
}

void cltNCharNameChange::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:	
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				cltGameMsgRequest_ChangeCharName clinfo((TCHAR*)pData, true);
				cltMsg clMsg(GAMEMSG_REQUEST_CHANGE_CHARNAME, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

				Close();
			}
		}
		break;
	}
	return;
}
//#endif