

#include "../../../Client.h"
#include "../../../InterfaceMgr/InterfaceFile.h"
#include "../../../InterfaceMgr/InterfaceMgr.h"
#include "MyGuildMasterMark.h"

#include "MsgType-Guild.h"
#include "Guild/Guild.h"

#include "../../Client/MarkManager/MarkManager.h"

extern cltCommonLogic* pclClient;

CNMyGuildMasterMark::CNMyGuildMasterMark()
{
	Initialize();
}	

CNMyGuildMasterMark::~CNMyGuildMasterMark()
{
	Destroy();
}

void CNMyGuildMasterMark::Initialize()
{

}

void CNMyGuildMasterMark::Destroy()
{

}

void CNMyGuildMasterMark::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{


/*	CImageStatic *pImageMyVill = (CImageStatic *)m_pControlMap[ IMAGE_MYVILLMARK ];
	CImageStatic *pImageVillage = (CImageStatic *)m_pControlMap[ IMAGE_VILLAGEMARK ];

	TSpr *pspr = pclClient->GetGlobalSpr( GIMG_VILLAGEMARK01 );

	if( pCityhallManager->m_siMyVillMarkIndex > 0 ) {

		GP.PutSpr(pspr, GetX() + pImageMyVill->GetX() + ScreenRX,
			GetY() + pImageMyVill->GetY() + ScreenRY,
			pCityhallManager->m_siMyVillMarkIndex - 1 );
	}

	if( pCityhallManager->m_siVillageMarkIndex > 0 ) {

		//pspr->PutSprT( GetX() + pImageVillage->GetX() + ScreenRX,
		GP.PutSpr(pspr, GetX() + pImageVillage->GetX() + ScreenRX,
			GetY() + pImageVillage->GetY() + ScreenRY,
			pCityhallManager->m_siVillageMarkIndex - 1 );
	}*/
}

void CNMyGuildMasterMark::Create( CControllerMgr *pParent )
{

	cltClient *pclclient = (cltClient *)pclClient;

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/Guild/DLG_MyGuildMasterMark.ddf"));

		file.CreateChildDialog( this, NMYGUILDMASTERMARK_DLG, TEXT("dialog_MyGuildMasterMark"),StaticMyGuildMasterMarkDlgProc , pParent);

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERMARK_STATIC_1 , this)  , NMYGUILDMASTERMARK_STATIC_1 , TEXT("statictext_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERMARK_STATIC_2 , this)  , NMYGUILDMASTERMARK_STATIC_2 , TEXT("statictext_NONAME2"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERMARK_STATIC_3 , this)  , NMYGUILDMASTERMARK_STATIC_3 , TEXT("statictext_NONAME3"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERMARK_STATIC_4 , this)  , NMYGUILDMASTERMARK_STATIC_4 , TEXT("statictext_NONAME4"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERMARK_STATIC_5 , this)  , NMYGUILDMASTERMARK_STATIC_5 , TEXT("statictext_NONAME5"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERMARK_STATIC_6 , this)  , NMYGUILDMASTERMARK_STATIC_6 , TEXT("statictext_NONAME6"));



		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NMYGUILDMASTERMARK_EDIT_OLDNAME, this )   , NMYGUILDMASTERMARK_EDIT_OLDNAME , TEXT("editbox_OldName"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NMYGUILDMASTERMARK_EDIT_NEWNAME, this )   , NMYGUILDMASTERMARK_EDIT_NEWNAME , TEXT("editbox_NewName"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NMYGUILDMASTERMARK_EDIT_MARKNAME, this )   , NMYGUILDMASTERMARK_EDIT_MARKNAME , TEXT("editbox_MarkName"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERMARK_BUTTON_CHANGENAME, this )   , NMYGUILDMASTERMARK_BUTTON_CHANGENAME , TEXT("button_ChangeName"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERMARK_BUTTON_MARKPRE, this )   , NMYGUILDMASTERMARK_BUTTON_MARKPRE , TEXT("button_Pre"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERMARK_BUTTON_MARKNEXT, this )   , NMYGUILDMASTERMARK_BUTTON_MARKNEXT , TEXT("button_Next"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERMARK_BUTTON_CHANGEMARK, this )   , NMYGUILDMASTERMARK_BUTTON_CHANGEMARK , TEXT("button_ChangeMark"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NMYGUILDMASTERMARK_IMAGESTATIC_OLDMARK, this) , NMYGUILDMASTERMARK_IMAGESTATIC_OLDMARK, TEXT("imagestatic_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NMYGUILDMASTERMARK_IMAGESTATIC_NEWMARK, this) , NMYGUILDMASTERMARK_IMAGESTATIC_NEWMARK, TEXT("imagestatic_NONAME2"));
		
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE , NMYGUILDMASTERMARK_OUTLINE_1, this )   , NMYGUILDMASTERMARK_OUTLINE_1 , TEXT("outline_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE , NMYGUILDMASTERMARK_OUTLINE_2, this )   , NMYGUILDMASTERMARK_OUTLINE_2 , TEXT("outline_NONAME2"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE , NMYGUILDMASTERMARK_OUTLINE_3, this )   , NMYGUILDMASTERMARK_OUTLINE_3 , TEXT("outline_NONAME3"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE , NMYGUILDMASTERMARK_OUTLINE_4, this )   , NMYGUILDMASTERMARK_OUTLINE_4 , TEXT("outline_NONAME4"));


		m_InterfaceFactory.GetEdit(NMYGUILDMASTERMARK_EDIT_NEWNAME)->SetMaxEditTextLength(13);
	}


	cltGuildManager *pGuildManager = ((cltGuildManager *)pclClient->pclGuildManager);
	if ( pGuildManager == NULL )
		return ;

	cltGuildInfo* pGuild = &pGuildManager->clCommonUnit;
	if ( pGuild == NULL )
		return ;

	m_siNewMarkIndex = pGuild->clGPoint.siMarkIndex;

	TCHAR* fileName = NULL;
	TSpr* pSpr = NULL;
	SI32 fontIndex = 0;

	if ( fileName = pclclient->m_pMarkManager->GetMarkSprAndFontIndex( m_siNewMarkIndex, &pSpr, &fontIndex ) )
	{
		CImageStatic* pImageNew = m_InterfaceFactory.GetImageStatic(NMYGUILDMASTERMARK_IMAGESTATIC_OLDMARK);
		pImageNew->SetFileName( fileName );
		pImageNew->SetFontIndex( fontIndex );


		CImageStatic* pImageOld = m_InterfaceFactory.GetImageStatic(NMYGUILDMASTERMARK_IMAGESTATIC_NEWMARK);
		pImageOld->SetFileName( fileName );
		pImageOld->SetFontIndex( fontIndex );
	}
}

void CALLBACK CNMyGuildMasterMark::StaticMyGuildMasterMarkDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMyGuildMasterMark *pThis = (CNMyGuildMasterMark*) pControllerMgr;
	pThis->NMyGuildMasterMarkDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNMyGuildMasterMark::NMyGuildMasterMarkDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;
	TCHAR buffer[256] = "" ;
	TCHAR temp[128] = "" ;

	cltGuildManager *pGuildManager = ((cltGuildManager *)pclClient->pclGuildManager);
	if ( pGuildManager == NULL )
		return ;

	cltGuildInfo* pGuild = &pGuildManager->clCommonUnit;
	if ( pGuild == NULL )
		return ;

	switch(nControlID)
	{
	case NMYGUILDMASTERMARK_BUTTON_CHANGENAME:
		{
			CEdit* pEditNewName = m_InterfaceFactory.GetEdit(NMYGUILDMASTERMARK_EDIT_NEWNAME);
			MStrCpy(temp,pEditNewName->GetText(),128);

#ifdef _IAF_EXPORT
			if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
			{
				TCHAR ResultBuf[1024] = "";
				IAF::Filtering( temp, ResultBuf, sizeof(ResultBuf), IAF::FILTER_CHARACTER );
				memcpy( temp, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
			}
#else	
			pclclient->m_pCAbuseFilter->CheckAbuse(temp);
#endif
			SI32 len = strlen(temp);
			if ( len <= 0 )
				break;

			TCHAR * pTitle = GetTxtFromMgr(6589);
			TCHAR * pText = GetTxtFromMgr(6591);
			sprintf(buffer,pText,pGuild->szName,temp);

			stMsgBox MsgBox;
			// 현재 dialog가 탭 창 같은 Child Dialog일때
			CControllerMgr *pParent = NULL;
			pParent = this->GetParentDialog();
			if(pParent == NULL ) return;
			MsgBox.Set( pParent, pTitle,buffer, MSGBOX_TYPE_YESNO, 4 );

			BYTE TempBuffer[ 128 ];
			SI16 Size = sizeof( TCHAR ) * len ;
			memcpy( TempBuffer, &temp, sizeof( TCHAR ) * len );
			pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
		}
		break;
	case NMYGUILDMASTERMARK_BUTTON_MARKPRE:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			m_siNewMarkIndex--;
			if ( m_siNewMarkIndex < 1 )
				m_siNewMarkIndex = pclclient->m_pMarkManager->GetTotalCount();

			TCHAR* fileName = NULL;
			TSpr* pSpr = NULL;
			SI32 fontIndex = 0;

			if ( fileName = pclclient->m_pMarkManager->GetMarkSprAndFontIndex( m_siNewMarkIndex, &pSpr, &fontIndex ) )
			{
				CImageStatic* pImageNew = m_InterfaceFactory.GetImageStatic(NMYGUILDMASTERMARK_IMAGESTATIC_NEWMARK);
				pImageNew->SetFileName( fileName );
				pImageNew->SetFontIndex( fontIndex );
			}

		/*	m_InterfaceFactory.GetImageStatic(NMYGUILDMASTERMARK_IMAGESTATIC_NEWMARK)->SetFontIndex(m_siNewMarkIndex - 1);*/
		}
		break;
	case NMYGUILDMASTERMARK_BUTTON_MARKNEXT:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			m_siNewMarkIndex++;
			if ( m_siNewMarkIndex > pclclient->m_pMarkManager->GetTotalCount() )
				m_siNewMarkIndex = 1;

			TCHAR* fileName = NULL;
			TSpr* pSpr = NULL;
			SI32 fontIndex = 0;

			if ( fileName = pclclient->m_pMarkManager->GetMarkSprAndFontIndex( m_siNewMarkIndex, &pSpr, &fontIndex ) )
			{
				CImageStatic* pImageNew = m_InterfaceFactory.GetImageStatic(NMYGUILDMASTERMARK_IMAGESTATIC_NEWMARK);
				pImageNew->SetFileName( fileName );
				pImageNew->SetFontIndex( fontIndex );
			}

			//m_InterfaceFactory.GetImageStatic(NMYGUILDMASTERMARK_IMAGESTATIC_NEWMARK)->SetFontIndex(m_siNewMarkIndex - 1);
		}
		break;
	case NMYGUILDMASTERMARK_BUTTON_CHANGEMARK:
		if ( EVENT_BUTTON_CLICKED == nEvent )
		{
			TCHAR* pTitle = GetTxtFromMgr(6592);
			TCHAR* pText = GetTxtFromMgr(6593);

			stMsgBox MsgBox;
			// 현재 dialog가 탭 창 같은 Child Dialog일때
			CControllerMgr *pParent = NULL;
			pParent = this->GetParentDialog();
			if(pParent == NULL ) return;
			MsgBox.Set( pParent, pTitle,pText, MSGBOX_TYPE_YESNO, 5 );

			pclclient->SetMsgBox( &MsgBox, (BYTE*)&m_siNewMarkIndex, sizeof(SI32) );
		}
	}
}


void CNMyGuildMasterMark::Hide()
{
	CControllerMgr::Show(false);

}

void CNMyGuildMasterMark::Show()
{
	CControllerMgr::Show(true);

}

void CNMyGuildMasterMark::RefreshWindow()
{
	cltGuildManager *pGuildManager = ((cltGuildManager *)pclClient->pclGuildManager);
	if ( pGuildManager == NULL )
		return ;

	cltGuildInfo* pGuild = &pGuildManager->clCommonUnit;
	if ( pGuild == NULL )
		return ;

	CEdit* pEditOrgName = m_InterfaceFactory.GetEdit(NMYGUILDMASTERMARK_EDIT_OLDNAME);
	pEditOrgName->SetText(pGuild->szName);
}
