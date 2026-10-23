//---------------------------------
// 2004/9/15 성준엽
//---------------------------------
/* ==========================================================================
	클래스 :

	작성일 :		05/03/18
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#include "..\Client\Client.h"
#include "./NCharHorseTypeChange.h"
#include "..\resource.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"
#include "Msg\MsgType-Horse.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "../../../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../../Client/InterfaceMgr/Interface/Static.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

extern cltCommonLogic* pclClient;

TCHAR** NHorseTypeItemUnique = NULL;
//char* NHorseTypeItemUnique[] =
//{
//		TEXT(""),		TEXT("5503"),		TEXT("300"),
//		TEXT(""),		TEXT("5506"),		TEXT("400"),
//		TEXT(""),	    TEXT("5510"),		TEXT("500"),
//		TEXT(""),	    TEXT("5520"),		TEXT("500"),
//		TEXT(""),	    TEXT("5530"),		TEXT("500"),
//		TEXT(""),	    TEXT("5540"),		TEXT("500"),
//};

cltNHorseTypeChange::cltNHorseTypeChange()
{
	m_DlgRectInitSwitch = FALSE;
	
	m_siSelectedUnique = 0 ;
	m_siOriUnique = 0 ;
	m_siMaxUnique = 0 ;

	m_pButton_left	 = NULL; 
	m_pImagestatic_portrait	 = NULL; 
	m_pButton_right	 = NULL; 
	m_pStatictext_name	 = NULL; 
	m_pButton_change	 = NULL; 
	m_pButton_cancel	 = NULL; 

	SI32 maxtype = ( MAX_HORSE_TYPE_INFO - 1 ) * 3 ;

    NHorseTypeItemUnique = new TCHAR*[maxtype];
	for(SI16 i=0; i<maxtype; i++)
	{
		NHorseTypeItemUnique[i] = new TCHAR[30];
	}

	StringCchCopy(NHorseTypeItemUnique[0] , 30, TEXT(""));
	StringCchCopy(NHorseTypeItemUnique[1] , 30, TEXT("5503"));
	StringCchCopy(NHorseTypeItemUnique[2] , 30, TEXT("300"));
	StringCchCopy(NHorseTypeItemUnique[3] , 30, TEXT(""));
	StringCchCopy(NHorseTypeItemUnique[4] , 30, TEXT("5506"));
	StringCchCopy(NHorseTypeItemUnique[5] , 30, TEXT("400"));
	StringCchCopy(NHorseTypeItemUnique[6] , 30, TEXT(""));
	StringCchCopy(NHorseTypeItemUnique[7] , 30, TEXT("5510"));
	StringCchCopy(NHorseTypeItemUnique[8] , 30, TEXT("500"));
	StringCchCopy(NHorseTypeItemUnique[9] , 30, TEXT(""));
	StringCchCopy(NHorseTypeItemUnique[10] , 30, TEXT("5520"));
	StringCchCopy(NHorseTypeItemUnique[11] , 30, TEXT("500"));
	StringCchCopy(NHorseTypeItemUnique[12] , 30, TEXT(""));
	StringCchCopy(NHorseTypeItemUnique[13] , 30, TEXT("5530"));
	StringCchCopy(NHorseTypeItemUnique[14] , 30, TEXT("500"));
	StringCchCopy(NHorseTypeItemUnique[15] , 30, TEXT(""));
	StringCchCopy(NHorseTypeItemUnique[16] , 30, TEXT("5540"));
	StringCchCopy(NHorseTypeItemUnique[17] , 30, TEXT("500"));
	StringCchCopy(NHorseTypeItemUnique[18] , 30, TEXT(""));
	StringCchCopy(NHorseTypeItemUnique[19] , 30, TEXT("5550"));
	StringCchCopy(NHorseTypeItemUnique[20] , 30, TEXT("500"));
	StringCchCopy(NHorseTypeItemUnique[21] , 30, TEXT(""));
	StringCchCopy(NHorseTypeItemUnique[22] , 30, TEXT("5560"));
	StringCchCopy(NHorseTypeItemUnique[23] , 30, TEXT("500"));
}

cltNHorseTypeChange::~cltNHorseTypeChange()
{
	m_DlgRectInitSwitch = FALSE;

	if( 	m_pButton_left	 )  	delete 	m_pButton_left	;
	if( 	m_pImagestatic_portrait	 )  	delete 	m_pImagestatic_portrait	;
	if( 	m_pButton_right	 )  	delete 	m_pButton_right	;
	if( 	m_pStatictext_name	 )  	delete 	m_pStatictext_name	;
	if( 	m_pButton_change	 )  	delete 	m_pButton_change	;
	if( 	m_pButton_cancel	 )  	delete 	m_pButton_cancel	;

	for(SI16 i=0; i<18; i++)
	{
		if(NHorseTypeItemUnique[i]) delete NHorseTypeItemUnique[i];
		NHorseTypeItemUnique[i] = NULL;
	}

	if(NHorseTypeItemUnique) delete [] NHorseTypeItemUnique;
}

void cltNHorseTypeChange::Create()
{
	TCHAR* pText = GetTxtFromMgr(2806);
	StringCchCopy(NHorseTypeItemUnique[0], 30, pText);
	 
	pText = GetTxtFromMgr(2805);
	StringCchCopy(NHorseTypeItemUnique[3], 30, pText);

	pText = GetTxtFromMgr(2753);
	StringCchCopy(NHorseTypeItemUnique[6], 30, pText);

	pText = GetTxtFromMgr(2769);
	StringCchCopy(NHorseTypeItemUnique[9], 30, pText);

	pText = GetTxtFromMgr(2781);
	StringCchCopy(NHorseTypeItemUnique[12], 30, pText);

	pText = GetTxtFromMgr(2791);
	StringCchCopy(NHorseTypeItemUnique[15], 30, pText);

	pText = GetTxtFromMgr(5174);
	StringCchCopy(NHorseTypeItemUnique[18], 30, pText);

	pText = GetTxtFromMgr(5394);
	StringCchCopy(NHorseTypeItemUnique[21], 30, pText);

	cltClient* pclclient = (cltClient*)pclClient;
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsCreate())
	{
		//CDialogController::Create( CLTNCHARKINDCHANGE_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, NHorseTypeChangeStaticProc, TEXT("품종 변경 "), TEXT("NInterface/Interface/Interface_20X20_00_109.Spr"), 9, true, 80, 100, 300, 220, true, true, true);

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NChangeHorseDlg/DLG_NCharStatus_NChangeHorse.ddf"));
		file.CreatePopupDialog( this, CLTNCHARKINDCHANGE_DLG, TEXT("dialog_horsekind"), NHorseTypeChangeStaticProc );

		m_pButton_left	 = new 	CButton	( this )	;
		m_pImagestatic_portrait	 = new 	CImageStatic	( this )	;
		m_pButton_right	 = new 	CButton	( this )	;
		m_pStatictext_name	 = new 	CStatic	( this )	;
		m_pButton_change	 = new 	CButton	( this )	;
		m_pButton_cancel	 = new 	CButton	( this )	;

		file.CreateControl( m_pButton_left, CLTNHORSETYPECHANGE_DLG_BUTTON_LEFT, TEXT("button_left") );
		file.CreateControl( m_pImagestatic_portrait, CLTNHORSETYPECHANGE_DLG_IMAGESTATIC_PORTRAIT, TEXT("imagestatic_portrait") );
		file.CreateControl( m_pButton_right, CLTNHORSETYPECHANGE_DLG_BUTTON_RIGHT, TEXT("button_right") );
		file.CreateControl( m_pStatictext_name, CLTNHORSETYPECHANGE_DLG_STATICTEXT_NAME, TEXT("statictext_name") );
		file.CreateControl( m_pButton_change, CLTNHORSETYPECHANGE_DLG_BUTTON_CHANGE, TEXT("button_change") );
		file.CreateControl( m_pButton_cancel, CLTNHORSETYPECHANGE_DLG_BUTTON_CANCEL, TEXT("button_cancel") );

		TCHAR *pTitle = GetTxtFromMgr(3717);
		SetTitle(pTitle);

		
		TCHAR *pText = GetTxtFromMgr(3718);
		m_pButton_change->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		pText = GetTxtFromMgr(3902);
		m_pButton_cancel->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		m_siMaxUnique = 0 ;

		//bool bInit = false ;
		//for( SI32 i = 0; i < MAX_HORSE_TYPE_INFO; i++ )
		//{
		//	if ( i == m_siOriUnique ) continue ;
		//	cltHorseTypeInfo *pclHorseTypeInfo = pclClient->pclHorseManager->pclHorseTypeInfo[i];
		//	if ( pclHorseTypeInfo == NULL ) continue ;
		//	TCHAR* pText = GetTxtFromMgr(667);
		//	if ( !_tcscmp(pText,pclHorseTypeInfo->szSpecies) ) continue ;

		//	if ( !bInit )
		//	{
		//		m_siSelectedUnique = i ;
		//		bInit = true ;
		//	}

		//	m_siMaxUnique = i ;
		//}

		//SetHorsePortrait();

	}
	else
	{
		DeleteDialog();
	}
}

void cltNHorseTypeChange::Close()
{
	DeleteDialog();
}

void CALLBACK cltNHorseTypeChange::NHorseTypeChangeStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	cltNHorseTypeChange *pThis = (cltNHorseTypeChange*)pControllerMgr;
	pThis->NHorseTypeChangeProc( nEvent, nControlID, pControl );
}

void CALLBACK cltNHorseTypeChange::NHorseTypeChangeProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;
	cltHorse* pclhorse	= pclclient->pclCM->CR[1]->pclCI->clHorseInfo.GetCurrentShowHorse();

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			m_DlgRectInitSwitch = TRUE;
            DeleteDialog();
		}
		break;
	case CLTNHORSETYPECHANGE_DLG_BUTTON_LEFT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					for ( SI32 i = m_siSelectedUnique - 1 ; i > 0 ; i -- )
					{
						if ( i == m_siOriUnique ) continue ;
						cltHorseTypeInfo *pclHorseTypeInfo = pclClient->pclHorseManager->pclHorseTypeInfo[i];
						if ( pclHorseTypeInfo == NULL ) continue ;
						TCHAR* pText = GetTxtFromMgr(667);
						if ( !_tcscmp(pText,pclHorseTypeInfo->szSpecies) ) continue ;

						m_siSelectedUnique = i ;
						SetHorsePortrait();
						break;
					}
				}
				break;
			}
		}
		break;
	case CLTNHORSETYPECHANGE_DLG_BUTTON_RIGHT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					for ( SI32 i = m_siSelectedUnique + 1 ; i < MAX_HORSE_TYPE_INFO ; i ++ )
					{
						if ( i == m_siOriUnique ) continue ;
						cltHorseTypeInfo *pclHorseTypeInfo = pclClient->pclHorseManager->pclHorseTypeInfo[i];
						if ( pclHorseTypeInfo == NULL ) continue ;
						TCHAR* pText = GetTxtFromMgr(667);
						if ( !_tcscmp(pText,pclHorseTypeInfo->szSpecies) ) continue ;

						m_siSelectedUnique = i ;
						SetHorsePortrait();
						break;
					}
				}
				break;
			}
		}
		break;
	case CLTNHORSETYPECHANGE_DLG_BUTTON_CHANGE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					char horseunique = m_siSelectedUnique ;
					if ( horseunique == m_siOriUnique ) return  ;
					if ( horseunique <= 1 && horseunique >= MAX_HORSE_TYPE_INFO ) return  ;
					cltHorseTypeInfo *pclHorseTypeInfo = pclClient->pclHorseManager->pclHorseTypeInfo[horseunique];
					if ( pclHorseTypeInfo == NULL ) return  ;

					TCHAR buffer[256] = TEXT("") ;

					SI32 itemunique = 0 ;
					SI32 horselife = 0 ;
					SI32 itemnum = 2 ;
					for ( SI32 i = 0 ; i < m_siMaxUnique * 3 ; i += 3 )
					{
						if ( !_tcscmp(NHorseTypeItemUnique[i],pclHorseTypeInfo->szSpecies) )
						{
							itemunique = _tstoi(NHorseTypeItemUnique[i+1]);
							horselife = _tstoi(NHorseTypeItemUnique[i+2]);
							break ;
						}
					}

					if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) < itemnum )
					{
						TCHAR* pText = GetTxtFromMgr(2834);
						TCHAR num[128]= TEXT(" ") ;
						StringCchPrintf(num, 128, TEXT("%d"),itemnum);
						Nsprintf(buffer,pText,TEXT("item1"),  pclClient->pclItemManager->GetName(itemunique)  ,TEXT("num"),  num  ,TEXT("item2"), pclClient->pclItemManager->GetName(itemunique)   ,NULL);

						//sprintf(buffer, pText,
						//	pclClient->pclItemManager->GetName(itemunique),itemnum,
						//	pclClient->pclItemManager->GetName(itemunique));
						pText = GetTxtFromMgr(663);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
						return ;
					}

/*					if ( pclhorse->siLife - horselife < -4000 )
					{
						TCHAR* pText = GetTxtFromMgr(664);
						sprintf(buffer, pText,
							MAX_HORSE_LIFE+4000+4000);
						pText = GetTxtFromMgr(665);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
						return ;
					}*/

					TCHAR* pText = GetTxtFromMgr(666);
					StringCchPrintf(buffer, 256, pText,
						pclHorseTypeInfo->szSpecies,horselife);
					pText = GetTxtFromMgr(663);

					stMsgBox MsgBox;
					MsgBox.Set( this, pText, buffer, MSGBOX_TYPE_YESNO, 0 );
					BYTE TempBuffer[ 128 ];
					SI16 Size = sizeof( horseunique );
					memcpy( TempBuffer, &horseunique, Size );
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

					//if (  MessageBox(NULL, buffer, pText,
					//	MB_ICONQUESTION | MB_YESNO) == IDNO )
					//{
					//	return ;
					//}

					//cltMsg clMsg(GAMEMSG_REQUEST_CHANGE_HORSETYPE, 1, &horseunique);
					//pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

					//DeleteDialog();
				}
				break;
			}
		}
		break;
	case CLTNHORSETYPECHANGE_DLG_BUTTON_CANCEL:
		{
			DeleteDialog();
		}
		break;
	}
}

void cltNHorseTypeChange::SetHorsePortrait()
{
	cltHorseTypeInfo *pclHorseTypeInfo = pclClient->pclHorseManager->pclHorseTypeInfo[m_siSelectedUnique];

	if ( pclHorseTypeInfo == NULL ) return ;

	TCHAR strFileName[ 128 ] = TEXT("");
	TCHAR strPathFileName[ 128 ] = TEXT("");
	TCHAR* pText= GetTxtFromMgr(3365);
	switch(m_siSelectedUnique)
	{
		case 1:	
			{
				StringCchCopy(strFileName, 128, pText);	
			}
				break;
		case 2:
			{
				pText= GetTxtFromMgr(3366);
				StringCchCopy(strFileName, 128, pText); 
			}
			break;
		case 3:
			{
				pText= GetTxtFromMgr(3367);
				StringCchCopy(strFileName, 128, pText); 
			}break;
		case 4:	
			{
				pText= GetTxtFromMgr(2753);
				StringCchCopy(strFileName, 128, pText);	
			}break;
		case 5: 
			{
				pText= GetTxtFromMgr(3369);
				StringCchCopy(strFileName,	128, pText);	
			}break;
		case 6:
			{
				pText= GetTxtFromMgr(3370);
				StringCchCopy(strFileName,	128, pText);	
			}break;
		case 7: 
			{
				pText= GetTxtFromMgr(3371);
				StringCchCopy(strFileName,	128, pText); 
			}break;
		case 8:
			{
				pText= GetTxtFromMgr(5174);
				StringCchCopy(strFileName,	128, pText); 
			}break;
		case 9:
			{
				pText= GetTxtFromMgr(5394);
				StringCchCopy(strFileName,	128, pText); 
			}break;
		default:
			{
				pText= GetTxtFromMgr(667);
				StringCchCopy(strFileName, 128, pText);	
			}break;
		

	//case 1:	_tcscpy(strFileName, TEXT("BROWN_HORSE"));	break;
	//case 2:	_tcscpy(strFileName, TEXT("BLACK_HORSE")); break;
	//case 3:	_tcscpy(strFileName, TEXT("WHITE_HORSE")); break;
	//case 4:	_tcscpy(strFileName, TEXT("RED_HORSE"));	break;
	//case 5: _tcscpy(strFileName,	TEXT("ZEBRA_HORSE"));	break;
	//case 6: _tcscpy(strFileName,	TEXT("RUDOLF"));	break;
	//case 7: _tcscpy(strFileName,	TEXT("THOUSAND_HORSE")); break;
	//default:_tcscpy(strFileName, TEXT("BROWN_HORSE"));	break;
	}
	//sprintf( strPathFileName, TEXT("Interface/Portrait/%s.bmp"),	strFileName );
	//m_hHorseBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), strPathFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	//if ( m_hHorseBitmap != NULL )
	//{
	//	SendDlgItemMessage( hDlg, IDC_STATIC_PORTRAIT_HORSETYPE, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hHorseBitmap );
	//}

	//SetWindowText(GetDlgItem(hDlg,IDC_STATIC_HORSETYPE_NAME),pclHorseTypeInfo->szSpecies);

	// leon - todo strFileName 셋팅 하는 문자열 테스트 필요!
	SI16 tmpSelectedIndex = pclClient->pclKindInfoSet->FindKindFromName( strFileName );

	TCHAR szPortrait[256];
	SI32 siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(tmpSelectedIndex, szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_pImagestatic_portrait->SetFileName( szPortrait );
	m_pImagestatic_portrait->SetFontIndex( siPortraitFont );

	m_pStatictext_name->SetText(pclHorseTypeInfo->szSpecies, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
}

void cltNHorseTypeChange::SetOriUnique( SI32 in_siOriUnique )
{
	m_siOriUnique = in_siOriUnique;

	bool bInit = false ;
	for( SI32 i = 0; i < MAX_HORSE_TYPE_INFO; i++ )
	{
		if ( i == m_siOriUnique ) continue ;
		cltHorseTypeInfo *pclHorseTypeInfo = pclClient->pclHorseManager->pclHorseTypeInfo[i];
		if ( pclHorseTypeInfo == NULL ) continue ;
		TCHAR* pText = GetTxtFromMgr(667);
		if ( !_tcscmp(pText,pclHorseTypeInfo->szSpecies) ) continue ;

		if ( !bInit )
		{
			m_siSelectedUnique = i ;
			bInit = true ;
		}

		m_siMaxUnique = i ;
	}

	SetHorsePortrait();

}

void cltNHorseTypeChange::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
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
				cltMsg clMsg(GAMEMSG_REQUEST_CHANGE_HORSETYPE, 1, (BYTE*)pData);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				
				Close();
			}
		}
		break;
	}
	return;
}
//#endif