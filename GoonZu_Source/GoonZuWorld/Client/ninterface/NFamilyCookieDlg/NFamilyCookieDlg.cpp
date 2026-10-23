#include "NFamilyCookieDlg.h"

#include "../../Client.h"

#include "../../common/Family/Marriage.h"
#include "../../common/Char/CharManager/CharManager.h"

#include "../../CommonLogic/Marriage/MarriageManager.h"

#include "../nletterdlg/NLetterDlg.h"
#include "../npartydlg/NPartyDlg.h"
#include "../nprivatechatdlg/NPrivateChatDlgMgr.h"
#include "../NHelpButton/NHelpButtonDlg.h"
#include "../NFamilyInfoDlg/NFamilyInfoDlg.h"
#include "../NDormancyAccount/NDormancyPointDlg.h"


extern cltCommonLogic* pclClient;

CNFamilyCookieDlg::CNFamilyCookieDlg() : CDialogController()
{
	m_siMinimizedSlot	= 0;

	m_pclBackGround		= NULL;
}

CNFamilyCookieDlg::~CNFamilyCookieDlg()
{
	SAFE_DELETE( m_pclBackGround );
}

void CNFamilyCookieDlg::Init()
{

}

void CNFamilyCookieDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}
	cltClient *pclclient = (cltClient *)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	// 이미지 사이즈
	SI16 siDialogWidth	= 34;
	SI16 siDialogHeight	= 34;

	// 다이얼로그 생성
	CDialogController::Create( NFAMILYCOOKIELEVEL_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogCNFamilyCookieDlg, NULL, NULL, 0, false, 0, 0, 0, siDialogWidth, siDialogHeight, false, false, false );

	// 배경 그림&툴팁
	NEWCIMAGESTATIC( m_pclBackGround );
	m_pclBackGround->Create( 0, TEXT("NInterface/image/IMAGE_34x34_00_000.SPR"), 16, false, 0, 0, 0, siDialogWidth, siDialogHeight, siDialogWidth, siDialogHeight );//그냥 ...그림이 없어서 결혼가져다 씀


	// 최소화한 다이얼로그의 위치를 다른 최소화다이얼로그의 위치를 따져 설정한다.
	SI16 LetterMinimizedSlot = 0, PrivateChatMinimizedSlot = 0, PartyMinimizedSlot = 0, HelpButtonMiniMizedSlot = 0 , MerrageLeveLevelMinizedSlot = 0 ,FamilyMini = 0, siDormancyMiniMizedSlot = 0;

	if( pclclient->m_pDialog[NLETTER_DLG])
		LetterMinimizedSlot = ((CNLetterDlg*)pclclient->m_pDialog[NLETTER_DLG])->GetMinimizedSlot();

	if( pclclient->m_pNPrivateChatDlgMgr)
		PrivateChatMinimizedSlot = pclclient->m_pNPrivateChatDlgMgr->GetMinimizedSlot();

	if( pclclient->m_pDialog[NPARTY_DLG])
		PartyMinimizedSlot = ((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->GetMinimizedSlot();

	if(pclClient->IsCountrySwitch(Switch_FreshManHelpDialog))
	{
		if( pclclient->m_pDialog[NHELPBUTTON_DLG])
			HelpButtonMiniMizedSlot = ((CNHelpButtonDlg*)pclclient->m_pDialog[NHELPBUTTON_DLG])->GetMinimizedSlot();
	}
	if(pclClient->IsCountrySwitch(Switch_WeddingSystem))	
	{
		if( pclclient->m_pDialog[NMARRIAGELOVELEVEL_DLG])
			MerrageLeveLevelMinizedSlot = ((CNPartyDlg*)pclclient->m_pDialog[NMARRIAGELOVELEVEL_DLG])->GetMinimizedSlot();
	}

	if ( pclclient->m_pDialog[NFAMILYINFO_DLG] )
		FamilyMini = ((CNFamilyInfoDlg*)pclclient->m_pDialog[NFAMILYINFO_DLG])->GetMinimizedSlot();

	if ( pclclient->m_pDialog[NDORMANCYPOINT_DLG] )
		siDormancyMiniMizedSlot = ((CNDormancyPointDlg*)pclclient->m_pDialog[NDORMANCYPOINT_DLG])->GetMinimizedSlot();
	// 빈 공간을 찾는다.
	for ( SI16 i = 1; i <= MAX_MINIMIZE_SLOT; i++ )
	{
		if( i == LetterMinimizedSlot || i == PrivateChatMinimizedSlot || i==PartyMinimizedSlot || i == HelpButtonMiniMizedSlot || i == MerrageLeveLevelMinizedSlot || i == FamilyMini || i == siDormancyMiniMizedSlot )
			continue;

		m_siMinimizedSlot = i;
		break;
	}

	SetDialogPos( pclclient->GetScreenXSize() - 40 - (m_siMinimizedSlot-1) * 45 , 215, 34, 34 );

	SetActionTimer( 100 );
	SetTimerOnOff( true );
}

void CNFamilyCookieDlg::Action()
{
	cltClient *pclclient = (cltClient*)pclClient;
	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )	return	;
	SI32 siMyPersonID	=	pclChar->pclCI->GetPersonID()	;
	if(pclChar->pclFamily	== NULL )	return	;
	// 켜지면 안되는 상황
	// 레벨이 비정상 적이다.
	SI16 siCookieLevel = pclChar->pclFamily->GetMyCookieLevel( siMyPersonID );
	if ( siCookieLevel < USE_COOKIE_LEVEL_1 || siCookieLevel > USE_COOKIE_LEVEL_5 )
	{
		DeleteDialog();
		return;
	}
	// 부모 케릭터가 한명도 없다 그러면 끈다 
	if ( pclChar->pclFamily->GetOnParentNum() <= 0 )
	{
		DeleteDialog();
		return;
	}
	//// 툴팁 설정
	//============================================================
	NTCHARString512 strToolTip = GetTxtFromMgr( 30211 );
	// 공방 상승분 체크 
	SI32 siAtt	=	pclChar->pclFamily->GetCookieEffect( siCookieLevel)	;
	SI32 siDff	=	pclChar->pclFamily->GetCookieEffect( siCookieLevel)	;
	//남은 시간

	// 남은 시간 가져오기
	SI32 siNowTime	= pclclient->clClientDate.GetHourVary();
	SI32 siEndTime	= pclChar->pclFamily->GetMyCookie_EndTime( siMyPersonID );
	SYSTEMTIME stFamilyTime =	pclChar->pclFamily->FInd_Add_Date(siMyPersonID)	;

	SI32 siResult  = siEndTime - siNowTime ;
	if ( siResult <= 0 )
	{
		siResult	=	0	;
		DeleteDialog();
	}
	siResult = siResult*2	; // 현실시간은 2배~

	strToolTip.Replace( TEXT("#attuprate#"),	SI32ToString(siAtt) );
	strToolTip.Replace( TEXT("#dfcuprate#"),	SI32ToString(siDff) );
	strToolTip.Replace( TEXT("#minute#"),		SI32ToString(siResult) );
	strToolTip.Replace( TEXT("#year#"),			SI32ToString(stFamilyTime.wYear) );
	strToolTip.Replace( TEXT("#Month#"),		SI32ToString(stFamilyTime.wMonth) );
	strToolTip.Replace( TEXT("#day#"),			SI32ToString(stFamilyTime.wDay) );


	if ( m_pclBackGround )
	{
		m_pclBackGround->SetUseToolTip( strToolTip );
	}
}

void CALLBACK CNFamilyCookieDlg::StaticCallBackDialogCNFamilyCookieDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNFamilyCookieDlg* pThis = (CNFamilyCookieDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogCNFamilyCookieDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNFamilyCookieDlg::CallBackDialogCNFamilyCookieDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
}