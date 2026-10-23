#include "./NCharSoulGuardHouse.h"
#include "./NCharSoulGuard.h"
#include "./NCharStatus.h"

#include "../Client/Client.h"

#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "Msg/MsgType-Horse.h"
#include "Msg/MsgType-Summon.h"
#include "Msg/MsgType-SoulGuard.h"

#include "Char/CharManager/CharManager.h"

#include "../../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../../Client/InterfaceMgr/Interface/Static.h"
#include "../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../Client/InterfaceMgr/Interface/Button.h"
#include "../../Client/InterfaceMgr/Interface/ComboBox.h"
#include "../../Client/InterfaceMgr/Interface/CheckBox.h"
#include "../../Client/InterfaceMgr/Interface/RadioButton.h"
#include "../../Client/InterfaceMgr/Interface/OutLine.h"

#include "../../Client/InterfaceMgr/InterfaceMgr.h"
#include "../../Client/InterfaceMgr/InterfaceFile.h"

extern cltCommonLogic* pclClient;

cltNSoulGuardHouseStatus::cltNSoulGuardHouseStatus()
{
	m_siCurrentSelectIndex	= -1;
	m_clAniInfo.Init1();

	m_pclImage_Back			= NULL;

	m_pclButton_Summon		= NULL;

}

cltNSoulGuardHouseStatus::~cltNSoulGuardHouseStatus()
{
	SAFE_DELETE( m_pclImage_Back );

	SAFE_DELETE( m_pclButton_Summon );
}

void cltNSoulGuardHouseStatus::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NCharStatusDlg/DLG_NCharStatus_Child_NStatusSoulGuard_House.ddf" ) );
	file.CreatePopupDialog( this, CLTNSOULGUARDHOUSESTATUS_DLG, TEXT( "dialog_soulguardhouse" ), NSoulGuardHouseStatusStaticProc );

	NEWCBUTTON( m_pclButton_Summon );	file.CreateControl( m_pclButton_Summon,	CLTNSOULGUARDHOUSESTATUS_DLG_BUTTON_SUMMMON,	TEXT("button_summon") );

	NEWCIMAGESTATIC( m_pclImage_Back );	file.CreateControl( m_pclImage_Back,	CLTNSOULGUARDHOUSESTATUS_DLG_IMAGESTATIC_BACK,	TEXT("imagestatic_back") );

	SetAlphaMode( 12 );
	m_pclImage_Back->SetDrawOption( SPR_PUTSPRALPHAT );
	m_pclImage_Back->SetAlphaValue( 12 );

	SetTopMostWindow( true );
		
}

void CALLBACK cltNSoulGuardHouseStatus::NSoulGuardHouseStatusStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	cltNSoulGuardHouseStatus* pThis = (cltNSoulGuardHouseStatus*)pControllerMgr;
	if ( pThis )
	{
		pThis->NSoulGuardHouseStatusProc( nEvent, nControlID, pControl);
	}
}

void CALLBACK cltNSoulGuardHouseStatus::NSoulGuardHouseStatusProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case CLTNSOULGUARDHOUSESTATUS_DLG_BUTTON_SUMMMON:
		{
			OnButtonClickSummon();
		}
		break;
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
	}

}

void cltNSoulGuardHouseStatus::Action()
{
}

void cltNSoulGuardHouseStatus::DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY )
{
	if ( (0 > m_siCurrentSelectIndex) || (MAX_SOULGUARD_HOUSE_NUM <= m_siCurrentSelectIndex) )
	{
		return;
	}

	cltClient*		pclclient	= (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient*	pclChar		= pclclient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	SI32 siDrawKind = pclChar->pclSoulGuard->m_clHouse.GetKindUniqueFromIndex( m_siCurrentSelectIndex );
	if ( false == pclClient->pclKindInfoSet->IsValidKind(siDrawKind) )
	{
		return;
	}

	cltKindInfo* pclGetKI = pclClient->pclKindInfoSet->pclKI[siDrawKind];
	if ( NULL == pclGetKI )
	{
		return;
	}

	SI32	siAniRef		= pclGetKI->clCharAniManager.FindRefFromAnitype(ANITYPE_WAIT0);
	SI32	siFramenumber	= pclGetKI->clCharAniManager.GetFrameNum(siAniRef);
	SI32	siDelay			= 0;

	if ( pclclient->GetFrameDelay() == 25 )
	{
		siDelay = m_clAniInfo.siAniDelay * 2 + 2;
	}
	else
	{
		siDelay = m_clAniInfo.siAniDelay + 2;
	} 

	m_clAniInfo.IncreaseAniDelayor(1);

	if ( m_clAniInfo.GetAniDelayor() >= siDelay )
	{
		m_clAniInfo.SetAniDelayor(0);

		m_clAniInfo.IncreaseAniStep(1);

		if(siFramenumber == 0)
		{
			m_clAniInfo.SetAniStep(0);
		}
		else
		{
			m_clAniInfo.SetAniStep(m_clAniInfo.GetAniStep() % siFramenumber );
		}
	}

	TSpr*	psprDraw		= NULL;
	SI32	siFontIndex		= 0;
	bool	bReverse		= false;
	SI32	siEffectFont	= 0;

	SI32	siRef			= pclGetKI->clCharAniManager.FindFirstAni();

	SI32	siImageXsize	= pclGetKI->clCharAniManager.GetImageXsize(siRef, pclClient->CurrentClock);
	SI32	siImageYsize	= pclGetKI->clCharAniManager.GetImageYsize(siRef, pclClient->CurrentClock);

	SI32	siRealXSize		= pclGetKI->clKIDrawInfo.siXsizePercent*siImageXsize/100;
	SI32	siRealYSize		= pclGetKI->clKIDrawInfo.siYsizePercent*siImageYsize/100;

	SI32	siXStart		= (siImageXsize - siRealXSize) / 2;
	SI32	siYStart		= siImageYsize - pclGetKI->clKIDrawInfo.siBottomIndent - siRealYSize;

	SI32	siX				= m_pclImage_Back->GetX() + (m_pclImage_Back->GetWidth() / 2 - siRealXSize / 2);
	SI32	siY				= m_pclImage_Back->GetY() + (m_pclImage_Back->GetHeight() / 2 - siRealYSize / 2);

	SI32	siDrawX			= GetX() + siX - siXStart + ScreenRX;
	SI32	siDrawY			= GetY() + siY - siYStart + ScreenRY;

	pclGetKI->clCharAniManager.DecideDrawImg( ANITYPE_WAIT0, (SOUTH | EAST), m_clAniInfo, TABS(pclClient->CurrentClock), &psprDraw, &siFontIndex, &bReverse, &siEffectFont );

	if ( psprDraw )
	{
		GP.PutSpr( psprDraw, siDrawX, siDrawY, siFontIndex, bReverse );
	}

}

void cltNSoulGuardHouseStatus::OnButtonClickSummon()
{
	if ( (0 > m_siCurrentSelectIndex) || (MAX_SOULGUARD_HOUSE_NUM <= m_siCurrentSelectIndex) )
	{
		return;
	}

	cltClient*		pclclient	= (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient*	pclChar		= pclclient->pclCM->GetCharClient( 1 );
	if ( NULL == pclChar )
	{
		return;
	}

	if ( NULL == pclChar->pclSoulGuard )
	{
		return;
	}

	SI16 siGetKind = pclChar->pclSoulGuard->m_clHouse.GetKindUniqueFromIndex( m_siCurrentSelectIndex );
	if ( false == pclclient->pclKindInfoSet->IsValidKind(siGetKind) )
	{
		return;
	}

	SI32 siGetSoulGuardID = pclChar->pclSoulGuard->m_clHouse.GetSoulGuardIDFromIndex( m_siCurrentSelectIndex );
	if ( 0 >= siGetSoulGuardID )
	{
		return;
	}

	cltGameMsgRequest_SoulGuardHouse_Change clInfo( m_siCurrentSelectIndex );
	cltMsg clMsg( GAMEMSG_REQUEST_SOULGUARD_HOUSE_CHANGE, sizeof(clInfo), (BYTE*)&clInfo );
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

}

