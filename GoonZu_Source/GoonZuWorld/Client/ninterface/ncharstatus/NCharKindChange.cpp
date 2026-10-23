//---------------------------------
// 2004/10/20 성준엽
//---------------------------------
/* ==========================================================================
	클래스 :		

	작성일 :		05/03/14
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#include "..\Client\Client.h"
#include "./NCharKindChange.h"
#include "..\resource.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "Msg\MsgType-Horse.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../../../Client/InterfaceMgr/Interface/Static.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

cltNCharKindChange::cltNCharKindChange()
{
	m_DlgRectInitSwitch = FALSE;
	m_pButton_left	 = NULL; 
	m_pImagestatic_portrait	 = NULL; 
	m_pButton_right	 = NULL; 
	m_pStatictext_name	 = NULL; 
	m_pButton_change	 = NULL; 
	m_pButton_cancel	 = NULL; 
}

cltNCharKindChange::~cltNCharKindChange()
{
	m_DlgRectInitSwitch = FALSE;

	if( 	m_pButton_left	 )  	delete 	m_pButton_left	;
	if( 	m_pImagestatic_portrait	 )  	delete 	m_pImagestatic_portrait	;
	if( 	m_pButton_right	 )  	delete 	m_pButton_right	;
	if( 	m_pStatictext_name	 )  	delete 	m_pStatictext_name	;
	if( 	m_pButton_change	 )  	delete 	m_pButton_change	;
	if( 	m_pButton_cancel	 )  	delete 	m_pButton_cancel	;

}

void cltNCharKindChange::Create()
{
	cltClient* pclclient = (cltClient*)pclClient;
	// 대화상자가 없는 상태라면 만든다. 
	if( ! IsCreate() )
	{
		//CDialogController::Create( CLTNCHARKINDCHANGE_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, NCharKindChangeStaticProc, TEXT("캐릭터 변경 "), TEXT("NInterface/Interface/Interface_20X20_00_109.Spr"), 9, true, 80, 100, 300, 220, true, true, true);

		CInterfaceFile file;

		file.LoadFile( TEXT("NInterface/Data/NChangeCharDlg/DLG_NCharStatus_NChangeChar.ddf"));
		file.CreatePopupDialog( this, CLTNCHARKINDCHANGE_DLG, TEXT("dialog_charkind"), NCharKindChangeStaticProc );

		m_pButton_left = new CButton( this );
		m_pImagestatic_portrait = new CImageStatic( this );
		m_pButton_right = new CButton( this );
		m_pStatictext_name = new CStatic( this );
		m_pButton_change = new CButton( this );
		m_pButton_cancel = new CButton( this );

		file.CreateControl( m_pButton_left, CLTNCHARKINDCHANGE_DLG_BUTTON_LEFT, TEXT("button_left") );
		file.CreateControl( m_pImagestatic_portrait, CLTNCHARKINDCHANGE_DLG_IMAGESTATIC_PORTRAIT, TEXT("imagestatic_portrait") );
		file.CreateControl( m_pButton_right, CLTNCHARKINDCHANGE_DLG_BUTTON_RIGHT, TEXT("button_right") );
		file.CreateControl( m_pStatictext_name, CLTNCHARKINDCHANGE_DLG_STATICTEXT_NAME, TEXT("statictext_name") );
		file.CreateControl( m_pButton_change, CLTNCHARKINDCHANGE_DLG_BUTTON_CHANGE, TEXT("button_change") );
		file.CreateControl( m_pButton_cancel, CLTNCHARKINDCHANGE_DLG_BUTTON_CANCEL, TEXT("button_cancel") );

		TCHAR* pTitle = GetTxtFromMgr(3716);
		SetTitle(pTitle);

		TCHAR* pText = GetTxtFromMgr(3715);
		m_pButton_change->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pText = GetTxtFromMgr(3902);
		m_pButton_cancel->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		m_siSelectedUnique = pclclient->pclCM->CR[1]->pclCI->clBI.GetKind();

		//for ( int i = 0 ; i < KIND_SYSTEM ; i ++ )
		//{
		//	if ( i == pclclient->pclCM->CR[1]->pclCI->clBI.GetKind() ) continue;
		//	cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[i];
		//	if ( pclKindInfo == NULL ) continue ;

		//	m_siSelectedUnique = i ;
		//	break ;
		//}

		SetPortrait();

	}
	else
	{
		DeleteDialog();
	}
}

void cltNCharKindChange::Close()
{
	//if( m_pButton_left )  delete m_pButton_left;
	//if( m_pImagestatic_portrait )  delete m_pImagestatic_portrait;
	//if( m_pButton_right )  delete m_pButton_right;
	//if( m_pStatictext_name )  delete m_pStatictext_name;
	//if( m_pButton_change )  delete m_pButton_change;
	//if( m_pButton_cancel )  delete m_pButton_cancel;
	
	DeleteDialog();
}

void CALLBACK cltNCharKindChange::NCharKindChangeStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltNCharKindChange *pThis = (cltNCharKindChange*)pControllerMgr;
	pThis->NCharKindChangeProc( nEvent, nControlID, pControl);

}

void CALLBACK cltNCharKindChange::NCharKindChangeProc( UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Close();
		}
		break;

	case CLTNCHARKINDCHANGE_DLG_BUTTON_LEFT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siSelectedUnique--;

/*#define KIND_HERO1						1		// 견이
#define KIND_HEROINE1					2		// 연이
#define KIND_HERO2						3		// 신이
#define KIND_HEROINE2					4		// 신이짝
#define KIND_HERO3						5		// 일본남
#define KIND_HEROINE3					6		// 일본여
#define KIND_HERO4						7		// 서양남
#define KIND_HEROINE4					8		// 서양녀*/
					if ( m_siSelectedUnique < KIND_HERO3 )
					{
						m_siSelectedUnique = KIND_HEROINE4;
					}
					SetPortrait();
					//for ( SI32 i = m_siSelectedUnique - 1 ; i > 0 ; i -- )
					//{
					//	if ( i == pclclient->pclCM->CR[1]->pclCI->clBI.GetKind() ) continue ;
					//	cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[i];
					//	if ( pclKindInfo == NULL ) continue ;
					//	if ( pclKindInfo->IsAtb(ATB_HERO) == false ) continue ;

					//	m_siSelectedUnique = i ;
					//	SetPortrait();
					//	break;
					//}
				}
				break;
			}
		}
		break;
	case CLTNCHARKINDCHANGE_DLG_BUTTON_RIGHT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siSelectedUnique++;

					/*#define KIND_HERO1						1		// 견이
					#define KIND_HEROINE1					2		// 연이
					#define KIND_HERO2						3		// 신이
					#define KIND_HEROINE2					4		// 신이짝
					#define KIND_HERO3						5		// 일본남
					#define KIND_HEROINE3					6		// 일본여
					#define KIND_HERO4						7		// 서양남
					#define KIND_HEROINE4					8		// 서양녀*/

					if ( m_siSelectedUnique > KIND_HEROINE4 )
					{
						m_siSelectedUnique = KIND_HERO3;
					}
					SetPortrait();

					//for ( SI32 i = m_siSelectedUnique + 1 ; i < KIND_SYSTEM ; i ++ )
					//{
					//	if ( i == pclclient->pclCM->CR[1]->pclCI->clBI.GetKind() ) continue ;
					//	cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[i];
					//	if ( pclKindInfo == NULL ) continue ;
					//	if ( pclKindInfo->IsAtb(ATB_HERO) == false ) continue ;

					//	m_siSelectedUnique = i ;
					//	SetPortrait();
					//	break;
					//}
				}
				break;
			}
		}
		break;
	case CLTNCHARKINDCHANGE_DLG_BUTTON_CHANGE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buffer[256] = TEXT("") ;

					SI32 mykind = pclclient->pclCM->CR[1]->pclCI->clBI.GetKind() ;

					if ( mykind <= 0 || mykind >= KIND_SYSTEM )
						return ;
					if ( m_siSelectedUnique == mykind )
						return ;

					SI32 itemunique = ITEMUNIQUE(24070) ; // 캐릭터변경이용권
					if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
					{
						TCHAR* pText = GetTxtFromMgr(555);
						StringCchPrintf(buffer, 256, pText);
						
						pText = GetTxtFromMgr(556);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
						return;
					}
					//감옥이면 사용불가능
					if ( pclclient->pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_PRISON))
					{
						TCHAR* pTitle = GetTxtFromMgr(836);
						TCHAR* pText = GetTxtFromMgr(6033);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,pText);
						return ;
					}

					

					// 장비가 장착되어있는지 검사한다.
					for ( SI16 i=0 ; i<PERSONITEM_RESERVED1 ; i++ )
					{
						if ( i == PERSONITEM_BAG || i == PERSONITEM_BAG_SUB )
							continue;

						if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[i].siUnique != 0 )
						{
							TCHAR* pText = GetTxtFromMgr(6456);
							StringCchPrintf(buffer, 256, pText);

							pText = GetTxtFromMgr(556);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
							return;
						}
					}
					//if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique != 0 )
					//{
					//	TCHAR* pText = GetTxtFromMgr(557);
					//	StringCchPrintf(buffer, 256, pText);
					//	
					//	pText = GetTxtFromMgr(558);
					//	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
					//	return;
					//}

					//if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_HAT].siUnique != 0 )
					//{
					//	TCHAR* pText = GetTxtFromMgr(559);
					//	StringCchPrintf(buffer, 256, pText);
					//	
					//	pText = GetTxtFromMgr(560);
					//	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
					//	return;
					//}

					TCHAR* pText = GetTxtFromMgr(561);

					StringCchCopy(buffer, 256, pText);
					pText = GetTxtFromMgr(562);

					stMsgBox MsgBox;
					MsgBox.Set( this, pText, buffer, MSGBOX_TYPE_YESNO, 0 );
					BYTE TempBuffer[ 128 ];
					SI16 Size = sizeof( m_siSelectedUnique );
					memcpy( TempBuffer, &m_siSelectedUnique, Size );
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

					//if (  MessageBox(NULL, buffer, pText,
					//	MB_ICONQUESTION | MB_YESNO) == IDNO )
					//{
					//	return;
					//}

					//BYTE senddata = m_siSelectedUnique ;
					//cltMsg clMsg(GAMEMSG_REQUEST_CHANGE_CHARKIND,1,&senddata);
					//pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					//
					//Close();
				}
				break;
			}
		}
		break;
	case CLTNCHARKINDCHANGE_DLG_BUTTON_CANCEL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					Close();
				}
				break;
			}
		}
		break;
	}
}

void cltNCharKindChange::SetPortrait()
{
	cltKindInfo *pclKindInfo = pclClient->pclKindInfoSet->pclKI[m_siSelectedUnique];

	if ( pclKindInfo == NULL ) return ;
	if ( pclKindInfo->IsAtb(ATB_HERO) == false ) return ;

	TCHAR szPortrait[256];
	SI32 siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(m_siSelectedUnique, szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_pImagestatic_portrait->SetFileName( szPortrait );

	TCHAR strFileName[ 128 ] = TEXT("");
	TCHAR strPathFileName[ 128 ] = TEXT("");

	if ( m_siSelectedUnique >= KIND_HERO3 && m_siSelectedUnique <=KIND_HEROINE4)
	{
		m_pImagestatic_portrait->SetFontIndex(siPortraitFont);
	}

	
	else
	{
		return;
	}
//	switch(m_siSelectedUnique)
//	{
//	case 1:	m_pImagestatic_portrait->SetFontIndex( 0 ); break;//_tcscpy(strFileName, TEXT("KIND_HERO1"));	break;
//	case 2:	m_pImagestatic_portrait->SetFontIndex( 1 ); break;//strcpy(strFileName, TEXT("KIND_HEROINE1")); break;
//
////#if _DEBUG
//	case 3:	m_pImagestatic_portrait->SetFontIndex( 2 ); break;
////#endif
//	default: return ;
//	}

    m_pStatictext_name->SetText((TCHAR*)pclKindInfo->GetName(), DT_CENTER|DT_VCENTER|DT_SINGLELINE );
}

void cltNCharKindChange::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
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
				BYTE senddata = *((SI32*)pData);
				cltMsg clMsg(GAMEMSG_REQUEST_CHANGE_CHARKIND,1,(BYTE*)&senddata);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				
				Close();
			}
		}
		break;
	}
	return;
}

//#endif
