#include "NBattleRoyalResultDlg.h"

#include "../../CommonLogic/MsgType-System.h"

#include "Char\CharManager\CharManager.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "NInterface/NHintDlg/NHintDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../GlobalImgFile/GlobalImgFile.h"

#include "../Client/MarkManager/MarkManager.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;
//CNRareItemDlg		g_RareItemMsg;

CNBattleRoyalResultDlg::CNBattleRoyalResultDlg()
{
	
	m_pResultStatic = NULL;  
	m_pResultImageStatic = NULL;  
    m_siBitmapAnimationIndex = 0 ;
	StringCchCopy( m_strPlayerName, MAX_PLAYER_NAME, TEXT(""));	
	m_siHintDlg = 0 ;
	Popupflag =0;
	SetTimerOnOff( true );
	
}

CNBattleRoyalResultDlg::~CNBattleRoyalResultDlg()
{
	SAFE_DELETE(m_pResultStatic);
	SAFE_DELETE(m_pResultImageStatic);
	
	Popupflag =0;
	SetTimerOnOff( false );
	

	return;

}


void CNBattleRoyalResultDlg::Create()
{
	m_siImageType = -1;
	m_siImageIndex = -1;

    SetActionTimer( POPUP_TIME );		
	if ( ! IsCreate() )
	{
		//CDialogController::Create( NRAREITEM_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCNRareItemDlgProc, TEXT("¸íÇ° "), TEXT("NInterface/Interface/Interface_20X20_00_109.Spr"), 9, true, 80, 100, 300, 240, true, false, true);

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NBattleRoyal/DLG_NBattleRoyalResult.ddf"));
		file.CreatePopupDialog( this, NBATTLEROYALRESULT1_DLG, TEXT("dialog_NBattleRoyalResultDlg"),StatiCNBattleRoyalResultDlgProc);	
		if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		{
			MovePos( 720, 66 );		
		}	
		else
		{
			MovePos( 554, 66 );
		}
		m_pResultStatic= new CStatic( this );
		m_pResultImageStatic= new CImageStatic( this );

		file.CreateControl( m_pResultImageStatic, NBATTLEROYALRESILT_IMAGE , TEXT("imagestatic_Result") );
		file.CreateControl( m_pResultStatic, NBATTLEROYALRESILT_STATIC , TEXT("statictext_Result") );
	}
	return;
}



void CNBattleRoyalResultDlg::Action()
{
	if( m_bResult == true)
	{
		m_pResultImageStatic->SetFileName(TEXT("Interface/EventNotice/rareitem.SPR"));
		m_pResultImageStatic->SetFontIndex(m_siBitmapAnimationIndex++);

		if ( m_siBitmapAnimationIndex >= 2 )
			m_siBitmapAnimationIndex = 0;
	}
	else
	{
		m_pResultImageStatic->SetFileName(TEXT("gimg/warkill.SPR"));
		m_pResultImageStatic->SetFontIndex(3);
	}
	
	Popupflag++;
		
	if(Popupflag>50)
	{
		DeleteDialog();
	}

}

void	CNBattleRoyalResultDlg::SetResult(bool bResult)
{
	m_bResult = bResult;
}

void CALLBACK CNBattleRoyalResultDlg::StatiCNBattleRoyalResultDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl)
{
	CNBattleRoyalResultDlg *pThis = (CNBattleRoyalResultDlg*) pControllerMgr;
	pThis->NBattleRoyalResultDlgProc( nEvent, nControlID, pControl);
}

void CALLBACK CNBattleRoyalResultDlg::NBattleRoyalResultDlgProc(  UINT nEvent, int nControlID, CControllerObj* pControl  )
{
	
	cltClient *pclclient = (cltClient*)pclClient;

	switch(nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	}
	return;
}

void CNBattleRoyalResultDlg::SetDescription(TCHAR* szDescription)
{
	m_pResultStatic->SetBorder(true);
	m_pResultStatic->SetBKColorUse(true);
	m_pResultStatic->SetText(szDescription, DT_WORDBREAK );
}

void CNBattleRoyalResultDlg::SetImage(TCHAR* ImagePath, SI32 siIndex)
{
	if(ImagePath == NULL)
		return;

	m_pResultImageStatic->SetFileName(ImagePath);
	m_pResultImageStatic->SetFontIndex(siIndex);
}

void CNBattleRoyalResultDlg::SetResize(SI32 siWidth, SI32 siHeight)
{
	this->SetHeight(siHeight);
	this->SetWidth(siWidth);

	return;
}

void CNBattleRoyalResultDlg::SetPosition(SI32 siX, SI32 siY) 
{
	this->SetX(siX);
	this->SetY(siY);

	return;
}