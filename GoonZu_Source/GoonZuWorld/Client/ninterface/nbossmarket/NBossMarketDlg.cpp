/* ==========================================================================
	클래스 :		CNBossMarketDlg

	작성일 :		05/03/28
	작성자 :		정용래

	용  법 :		

	변경사항 :		없음
	
	차후작업 :		

   ========================================================================*/
//#if defined(_LEON_NBOSSMARKET_) && defined(_DEBUG)
#include <tchar.h>
#include <Directives.h>
#include "../../Client/client.h"
#include "./NBossMarketDlg.h"
#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/interface/Edit.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

CNBossMarketDlg::CNBossMarketDlg()
{
	//m_bShow = false;

	//m_pPortraitImage = NULL;
	//m_pExplainEdit = NULL;
	//for(SI16 i=0; i<MAX_DETAIL_DLG_NUM; i++)
	//{
	//	m_pDetailButton[ i ] = NULL;
	//}
	//m_pCloseButton = NULL;
}

CNBossMarketDlg::~CNBossMarketDlg()
{
	//if(m_pPortraitImage)
	//	delete m_pPortraitImage ;
	//if(m_pExplainEdit)
	//	delete m_pExplainEdit ;
	//for(SI16 i=0; i<MAX_DETAIL_DLG_NUM; i++)
	//{
	//	if(m_pDetailButton[ i ])
	//		delete m_pDetailButton[ i ] ;
	//}
	//if(m_pCloseButton)
	//	delete m_pCloseButton ;

	//// WM_TIMER 를 사용한다면
	////SetTimerOnOff( false );
}

void CNBossMarketDlg::Create()
{
//	cltClient *pclclient = (cltClient *)pclClient;
//
//	if(!IsCreate())
//	{
//		// Dialog 생성!
//		CInterfaceFile file;
//		file.LoadFile(TEXT("NInterface/Data/NBossMarketDlg/DLG_NBossMarket.ddf"));
//		file.CreatePopupDialog( this, NBOSSMARKET_DLG, TEXT("dialog_bossmarket"), StaticCNBossMarketDlgProc );
//
//		m_pPortraitImage = new CImageStatic( this );
//		m_pExplainEdit = new CEdit( this );
//		for(SI16 i=0; i<MAX_DETAIL_DLG_NUM; i++)
//		{
//			m_pDetailButton[ i ] = new CButton( this );
//		}
//		m_pCloseButton = new CButton( this );
//
//		TCHAR tmpTChar[50];
//		tmpTChar[49] = 0;
//
//		SI32 siTmp = 0, siTmpWidth = 0;
//		siTmp = file.GetInfoWithStringID( TEXT("dialog_bossmarket") );
//
//		if(siTmp < 0 )
//			siTmpWidth = DLG_BIG_POS_X;
//		else
//			siTmpWidth = file.m_control[ siTmp ].width ;
//
//		// Dialog 고정 위치 지정!
//		if(pclclient && pclclient->m_pDialog[ NSOUNDONOFF_DLG ])
//		{			
//			if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
//			{
//				this->MovePos(DLG_BIG_POS_X - siTmpWidth, DLG_BIG_POS_Y);
//			}
//			else
//			{
//				this->MovePos(DLG_SMALL_POS_X - siTmpWidth, DLG_SMALL_POS_Y);
//			}
//		}
//		
//		// Control 생성!
//		file.CreateControl(m_pPortraitImage,	NBOSSMARKET_DLG_PORTRAITIMAGE,	TEXT("imagestatic_portrait"));
//		file.CreateControl(m_pExplainEdit,		NBOSSMARKET_DLG_EXPLAINEDIT,     TEXT("editbox_explain"));
//		// NBOSSMARKET_DLG_BUTTON1 ~ NBOSSMARKET_DLG_BUTTON5까지
//		for(SI16 i=0; i<MAX_DETAIL_DLG_NUM; i++)
//		{
//			StringCchPrintf(tmpTChar, 50, TEXT("button_%d"), i+1 );
//
//			file.CreateControl(m_pDetailButton[i], NBOSSMARKET_DLG_BUTTON1 + i,	tmpTChar);
//
//			// Popup Dialog 생성!
//			m_DetailDlg[i].Create();
//	
//			if( siTmp < 0 ) continue;
//
//			if(pclclient && pclclient->m_pDialog[ NSOUNDONOFF_DLG ] && ((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
//			{
//				m_DetailDlg[i].MovePos(DLG_BIG_POS_X - siTmpWidth*2, DLG_BIG_POS_Y + 18*i);
//			}
//			else
//			{
//				m_DetailDlg[i].MovePos(DLG_SMALL_POS_X - siTmpWidth*2, DLG_SMALL_POS_Y + 18*i);
//			}
//		}
//
//		file.CreateControl(m_pCloseButton, NBOSSMARKET_DLG_CLOSE,	TEXT("button_close"));
//
////	#define _CNBossMarketDlg_SAMPLE_
//		// 컨트롤 셋팅!
////	#if defined( _DEBUG ) && defined( _CNBossMarketDlg_SAMPLE_ )
//		//SetPortrait(TEXT("Interface/Portrait/PortraitIcon.spr"),pclClient->GetUniqueFromHash( TEXT("KIND_WANGCHO"))-1);
//		SetSpeech( 3220 );
//		//SI16 tmpFontIndex[11] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
//		SI16 tmpTextIndex[11] = { 3209, 3210, 3211, 3212, 3213, 3214, 3215, 3216, 3217, 3218, 3219 };
//
//		m_DetailDlg[0].SetMaxScene( 11 );
//		m_DetailDlg[0].SetTextInfo( tmpTextIndex, 11 );
//		m_DetailDlg[0].SetPortrait(TEXT("Interface/Portrait/PortraitIcon.spr"),pclClient->GetUniqueFromHash( TEXT("KIND_WANGCHO"))-1);
//		m_DetailDlg[0].SetImageInfo(TEXT("Interface/EventNotice/Comic01.spr"), 11);
//
////	#endif
//		TCHAR* pTitle= GetTxtFromMgr(3208);
//		SetTitle(pTitle);
//
//		TCHAR* pText = GetTxtFromMgr(3706);
//        m_pDetailButton[0]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//		pText = GetTxtFromMgr(3303);
//		m_pCloseButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
//
//		m_pDetailButton[1]->Enable( false );
//		m_pDetailButton[2]->Enable( false );
//		m_pDetailButton[3]->Enable( false );
//		m_pDetailButton[4]->Enable( false );
//		// WM_TIMER 를 사용할 때
//		//SetTimerOnOff( true );
//
//		//Hide();
//	}
//	else
//	{
//		DeleteDialog();        
//	}
}

void CALLBACK CNBossMarketDlg::StaticCNBossMarketDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNBossMarketDlg *pThis = (CNBossMarketDlg*) pControllerMgr;
	pThis->CNBossMarketDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNBossMarketDlg::CNBossMarketDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NBOSSMARKET_DLG_CLOSE:
		{
			//Hide();
			DeleteDialog();
		}
		break;

	case NBOSSMARKET_DLG_BUTTON1:
	    LOOP_OF_BUTTON( 0 );
	case NBOSSMARKET_DLG_BUTTON1 + 1:
	    LOOP_OF_BUTTON( 1 );
	case NBOSSMARKET_DLG_BUTTON1 + 2:
	    LOOP_OF_BUTTON( 2 );
	case NBOSSMARKET_DLG_BUTTON1 + 3:
	    LOOP_OF_BUTTON( 3 );
	case NBOSSMARKET_DLG_BUTTON1 + 4:
	    LOOP_OF_BUTTON( 4 );
	}
}

void CNBossMarketDlg::SetPortrait(TCHAR* in_pFileFullPath, SI16 in_siFontIndex )
{
	if( in_pFileFullPath == NULL || in_siFontIndex < 0 ) return;

	m_pPortraitImage->SetFileName( in_pFileFullPath );
	m_pPortraitImage->SetFontIndex( in_siFontIndex );
}

void CNBossMarketDlg::SetSpeech( SI32 in_siSpeechNum )
{
	if( in_siSpeechNum < 0 ) return;
	
	TCHAR* pText = NULL;
	pText = GetTxtFromMgr( in_siSpeechNum );

	if(pText == NULL ) return;

	m_pExplainEdit->SetText( pText );
}

bool CNBossMarketDlg::IsShow()
{
	return m_bShow;
}

void CNBossMarketDlg::Show()
{
	m_bShow = true;
	CDialogController::Show( m_bShow );
}

void CNBossMarketDlg::Hide()
{
	m_bShow = false;
	CDialogController::Show( m_bShow );
}

//// WM_TIMER 메세지 처리 루틴
//void CNBossMarketDlg::Action()
//{
//}

//#endif
