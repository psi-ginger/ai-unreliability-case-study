#include "Client.h"
#include "NBattleRoyalMoveDlg.h"

#include "Char\CharManager\CharManager.h"
#include "CommonLogic.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

CNBattleRoyalMoveDlg::CNBattleRoyalMoveDlg()
{
	m_pButtonMove= NULL;
	m_siBitmapAnimationIndex = 0 ;
	SetTimerOnOff( true );

	m_bFlash = false;
	m_uiFlashCount = 0;
}


CNBattleRoyalMoveDlg::~CNBattleRoyalMoveDlg()
{
	m_siBitmapAnimationIndex = 0;
	SAFE_DELETE(m_pButtonMove);
	SetTimerOnOff( false );
	m_bFlash = false;
	m_uiFlashCount = 0;
}

void CNBattleRoyalMoveDlg::Action()
{	
	if( m_bFlash == true )
	{
		m_pButtonMove->SetFontIndex(m_siBitmapAnimationIndex ++);

		if ( m_siBitmapAnimationIndex >= 2 )
			m_siBitmapAnimationIndex = 0;
		
		++m_uiFlashCount;

		if( m_uiFlashCount >= 4)
		{
			m_uiFlashCount = 0;
			m_bFlash = false;
		}
	}

}

void CNBattleRoyalMoveDlg::Create()
{
	SetActionTimer(200);

	//	중국도 아이템몰이 들어가서 
	//    if ( pclClient->siServiceArea == ConstServiceArea_China )
	//		return;

	if(!IsCreate())
	{
		CInterfaceFile file;

		if ( g_SoundOnOffDlg.m_bFullScreenMode )
		{
			file.LoadFile(TEXT("NInterface/Data/NBattleRoyal/DLG_NBattleRoyalMove.ddf"));


			file.CreatePopupDialog( this, NBATTLEROYALMOVE_DLG, TEXT("dialog_NBattleRoyalMove"), StaticNBattleRoyalMoveProc);		

			m_pButtonMove = new CButton(this);

			file.CreateControl(m_pButtonMove, NBATTLEROYALMOVE_BUTTON, TEXT("button_MoveToStadium"));
			m_pButtonMove->SetUseToolTip( GetTxtFromMgr(9568) );
			MovePos(342,98);

			m_pButtonMove->SetFileName(TEXT("ItemMall/itemmall.SPR"));	

			SetTopMostWindow(true);
		}
	}
}



void CALLBACK CNBattleRoyalMoveDlg::StaticNBattleRoyalMoveProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNBattleRoyalMoveDlg *pThis = (CNBattleRoyalMoveDlg*) pControllerMgr;
	pThis->NBattleRoyalMoveProc( nEvent, nControlID, pControl );
}


void CALLBACK CNBattleRoyalMoveDlg::NBattleRoyalMoveProc(  UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient *pclclient = (cltClient*)pclClient;
	switch( nControlID )
	{

	case NBATTLEROYALMOVE_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{                    
					//강제로 받는 페이지 부른다
					if ( g_SoundOnOffDlg.m_bFullScreenMode )
					{							
						cltMsg clMsg(GAMEMSG_REQUEST_BATTLEROYAL_MOVETOSTADIUM, 0);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
					else
					{
						
					}
				}
				break;
			}

		}
		break;


	}

	return;

}
