// VillageWarInfoDlg.cpp: implementation of the CVillageWarInfoDlg class.
//
//////////////////////////////////////////////////////////////////////


#include "../../Client/client.h"
#include "../../common/Char/CharManager/CharManager.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"

//Ãß°¡ 5ÁÙ
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"






#include "..\..\FontManager\FontManager.h"
#include "../../Music/Music.h"
#include "../../../lib/WebHTML/WebHTML.h"
#include "NVillageWarInfoDlg.h"
extern cltCommonLogic* pclClient;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CNVillageWarInfoDlg::CNVillageWarInfoDlg()
{

	m_pnstatic_villagewarinfo1=NULL;
	m_pnstatic_villagewarinfo2=NULL;
	m_bRectSwitch = FALSE ;
	m_siBitmapAnimationIndex = 0;
	SetTimerOnOff( true );
	SetActionTimer(200 );
}

CNVillageWarInfoDlg::~CNVillageWarInfoDlg()
{
	SAFE_DELETE(m_pnstatic_villagewarinfo1);
	SAFE_DELETE(m_pnstatic_villagewarinfo2);
	SetTimerOnOff( false );	
	m_siBitmapAnimationIndex = 0;
}



void CNVillageWarInfoDlg::Action()
{
		//m_pnstatic_villagewarinfo1->SetFileName(TEXT("Interface/VillageWarInfoDlg/VillageWarInfo.SPR"));
		m_pnstatic_villagewarinfo1->SetFontIndex(m_siBitmapAnimationIndex++);
		

		if ( m_siBitmapAnimationIndex >= 2 )
			m_siBitmapAnimationIndex = 0;	
}


void CNVillageWarInfoDlg::Create()
{

	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/Nvillagewarinfor/DLG_Nvillagewarinfor.ddf"));
	file.CreatePopupDialog( this, NVILLAGEWARINFO_DLG, TEXT("dialog_Nvillagewarinfo"),StatiCNVillageWarInfoDlgProc);
	m_pnstatic_villagewarinfo1 = new CStatic( this );             
	m_pnstatic_villagewarinfo2 = new CStatic( this );    
	file.CreateControl(m_pnstatic_villagewarinfo1, NVILLAGEWARINFO_DIALOG_STATIC1, TEXT("statictext_Nvillagewarinfo1"));
	file.CreateControl(m_pnstatic_villagewarinfo2, NVILLAGEWARINFO_DIALOG_STATIC2, TEXT("statictext_Nvillagewarinfo2"));	
	


}

void CALLBACK CNVillageWarInfoDlg::StatiCNVillageWarInfoDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNVillageWarInfoDlg *pThis= (CNVillageWarInfoDlg *) pControllerMgr;
	pThis->CNVillageWarInfoDlgProc(nEvent, nControlID, pControl );
}

void CALLBACK CNVillageWarInfoDlg::CNVillageWarInfoDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();	
		}
		break;	
	}		
	return;

}

void CNVillageWarInfoDlg::Set(SI32 winvillage,SI32 losevillage)
{
	if ( winvillage < 1 || winvillage > CUR_VILLAGE_LIMIT) return ;
	if ( losevillage < 1 || losevillage > CUR_VILLAGE_LIMIT ) return ;

	cltClient *pclclient = (cltClient*)pclClient;


	TCHAR buf[256] = TEXT("") ;
	TCHAR path[512] = TEXT("") ;

	TCHAR* pText= GetTxtFromMgr(3358);
	Nsprintf(buf,pText,TEXT("who"),  pclclient->pclVillageManager->GetName(winvillage)  ,TEXT("whom"), pclclient->pclVillageManager->GetName(losevillage)   ,NULL);
	//sprintf(buf,pText,pclclient->pclVillageManager->GetName(winvillage),pclclient->pclVillageManager->GetName(losevillage));
	m_pnstatic_villagewarinfo2->SetBorder(true);
	m_pnstatic_villagewarinfo2->SetBKColorUse(true);
	m_pnstatic_villagewarinfo2->SetText( buf,DT_WORDBREAK );

	m_pnstatic_villagewarinfo1->SetFileName(TEXT("Interface/VillageWarInfoDlg/VillageWarInfo_Lose.SPR"));

	if (pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage == winvillage)
		m_pnstatic_villagewarinfo1->SetFileName(TEXT("Interface/VillageWarInfoDlg/VillageWarInfo_Win.SPR"));
	else if (pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage == losevillage)
		m_pnstatic_villagewarinfo1->SetFileName(TEXT("Interface/VillageWarInfoDlg/VillageWarInfo_Lose.SPR"));
}

