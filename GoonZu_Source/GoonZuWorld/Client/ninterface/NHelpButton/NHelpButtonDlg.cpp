#include "NHelpButtonDlg.h"
#include "../nletterdlg/NLetterDlg.h"
#include "../npartydlg/NPartyDlg.h"
#include "../nprivatechatdlg/NPrivateChatDlgMgr.h"
#include "Char\CharManager\CharManager.h"


#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "../NMarriageLoveLevelDlg/NMarriageLoveLevelDlg.h"
#include "../NFamilyCookieDlg/NFamilyCookieDlg.h"
#include "../NFamilyInfoDlg/NFamilyInfoDlg.h"
#include "../NDormancyAccount/NDormancyPointDlg.h"

extern cltCommonLogic* pclClient;

CNHelpButtonDlg::CNHelpButtonDlg()
{	
	Initialize();
}

CNHelpButtonDlg::~CNHelpButtonDlg()
{
	Destroy();
}


void CNHelpButtonDlg::Init()
{
	m_siMinimizedSlot = 0;
}

void CNHelpButtonDlg::Destroy()
{

}

void CNHelpButtonDlg::Create()
{
	if ( IsCreate() )
		return;

	if ( pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() >= 50 )
		return;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NHelpButtonDlg/DLG_NHelpButtonDlg.ddf"));
	file.CreatePopupDialog( this, NHELPBUTTON_DLG , TEXT("dialog_helpbutton"), StaticHelpButtonDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NHELPBUTTON_IMAGESTATIC_HELP , this ), NHELPBUTTON_IMAGESTATIC_HELP,   "imagestatic_help");

	// 최소화한 다이얼로그의 위치를 다른 최소화다이얼로그의 위치를 따져 설정한다.

	SI16 LetterMinimizedSlot = 0, PrivateChatMinimizedSlot = 0, PartyMinimizedSlot = 0, LoveLevelMiniMizedSlot = 0 , FamilyCookieEffect = 0 ,FamilyMini = 0, siDormancyMiniMizedSlot = 0;

	cltClient* pclclient = (cltClient*)pclClient;

	if( pclclient->m_pDialog[NLETTER_DLG])
		LetterMinimizedSlot = ((CNLetterDlg*)pclclient->m_pDialog[NLETTER_DLG])->GetMinimizedSlot();

	if( pclclient->m_pNPrivateChatDlgMgr)
		PrivateChatMinimizedSlot = pclclient->m_pNPrivateChatDlgMgr->GetMinimizedSlot();

	if( pclclient->m_pDialog[NPARTY_DLG])
		PartyMinimizedSlot = ((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->GetMinimizedSlot();

	if ( pclclient->m_pDialog[NMARRIAGELOVELEVEL_DLG] )
		LoveLevelMiniMizedSlot = ((CNMarriageLoveLevelDlg*)pclclient->m_pDialog[NMARRIAGELOVELEVEL_DLG])->GetMinimizedSlot();

	if ( pclclient->m_pDialog[NFAMILYCOOKIELEVEL_DLG] )
		FamilyCookieEffect = ((CNFamilyCookieDlg*)pclclient->m_pDialog[NFAMILYCOOKIELEVEL_DLG])->GetMinimizedSlot();

	if ( pclclient->m_pDialog[NFAMILYINFO_DLG] )
		FamilyMini = ((CNFamilyInfoDlg*)pclclient->m_pDialog[NFAMILYINFO_DLG])->GetMinimizedSlot();
		
	if ( pclclient->m_pDialog[NDORMANCYPOINT_DLG] )
		siDormancyMiniMizedSlot = ((CNDormancyPointDlg*)pclclient->m_pDialog[NDORMANCYPOINT_DLG])->GetMinimizedSlot();


	// 빈 공간을 찾는다.
	for(SI16 i = 1; i <= MAX_MINIMIZE_SLOT; i++)
	{
		if( i == LetterMinimizedSlot || i == PrivateChatMinimizedSlot || i==PartyMinimizedSlot || i == LoveLevelMiniMizedSlot  || i == FamilyCookieEffect || i == FamilyMini || i == siDormancyMiniMizedSlot)
			continue;

		m_siMinimizedSlot = i;
		break;
	}

	// 슬롯에 따른 x 좌표로 움직여 준다.
	SetDialogPos( pclclient->GetScreenXSize() - 32 - (m_siMinimizedSlot-1) * 35 , 210, 32, 34 );

	SetTimerOnOff( true );
	SetActionTimer( 200 );
}

void CALLBACK CNHelpButtonDlg::StaticHelpButtonDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNHelpButtonDlg *pThis = (CNHelpButtonDlg*) pControllerMgr;
	pThis->NHelpButtonDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNHelpButtonDlg::NHelpButtonDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NHELPBUTTON_IMAGESTATIC_HELP:
		{
			if ( EVENT_IMAGESTATIC_LBUTTONUP == nEvent )
			{
				pclclient->ShowHelpWindow( TEXT("") );
			}
		}
		break;
	}
}

void CNHelpButtonDlg::Action()
{
	cltClient* pclclient = (cltClient*)pclClient;
	bool bLoginDlgShow = pclclient->m_pDialog[NLOGIN_DLG] && pclclient->m_pDialog[NLOGIN_DLG]->IsShow();
	bool bSelectCharDlgShow = pclclient->m_pDialog[NSELECTCHAR_DLG] && pclclient->m_pDialog[NSELECTCHAR_DLG]->IsShow();
	bool bSelectWorldDlgShow = pclclient->m_pDialog[NSELECTWORLDLIST_DLG] && pclclient->m_pDialog[NSELECTWORLDLIST_DLG]->IsShow();
	// 캐릭터 선택창, 로그인창, 서버선택창이 떠있으면 출력 안한다.
	if ( (bLoginDlgShow || bSelectCharDlgShow || bSelectWorldDlgShow) == true )
	{
		Show( false );
	}
	else
	{
		if ( IsShow() == false )
		{
			Show( true );
		}
	}

}