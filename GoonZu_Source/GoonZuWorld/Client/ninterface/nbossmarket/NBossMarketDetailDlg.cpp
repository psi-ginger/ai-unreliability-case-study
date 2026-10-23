/* ==========================================================================
	클래스 :		NBossMarketDetailDlg

	작성일 :		05/03/28
	작성자 :		정용래

	용  법 :		

	변경사항 :		없음
	
	차후작업 :		cpp 참고

   ========================================================================*/

//#if defined(_LEON_NBOSSMARKET_) && defined(_DEBUG)

#include "NBossMarketDetailDlg.h"

#include "Char/CharManager/CharManager.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/interface/Edit.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "Msg/MsgType-Quest.h"

extern cltCommonLogic* pclClient;


CNBossMarketDetailDlg::CNBossMarketDetailDlg()
{
	//m_bShow = true;
	//m_bFileNameMode = false;	// 디폴트가 그림 파일 하나, Index만 바뀜
	//m_bMustDelete = false;		// m_psiImageIndex를 new로 할당 했는지 여부
	//m_siCurSceneIndex = 0;		// 현재 장면 번호
	//m_siMaxScene = -1;			
	//m_psiTextIndex = NULL;		
	//m_psiImageIndex = NULL;		
	//m_ppImageFileName = NULL;

	//m_pPortraitImage = NULL;
	//m_pDetailImage = NULL;
	//m_pExplainEdit = NULL;
	//m_pPrevButton = NULL;
	//m_pNextButton = NULL;
	//m_pCloseButton = NULL;

}

CNBossMarketDetailDlg::~CNBossMarketDetailDlg()
{
	//if(m_pPortraitImage)
	//	delete m_pPortraitImage ;
	//if(m_pDetailImage)
	//	delete m_pDetailImage ;
	//if(m_pExplainEdit)
	//	delete m_pExplainEdit ;
	//if(m_pPrevButton)
	//	delete m_pPrevButton ;
	//if(m_pNextButton)
	//	delete m_pNextButton ;
	//if(m_pCloseButton)
	//	delete m_pCloseButton ;

	//if(m_psiTextIndex)
	//	delete m_psiTextIndex;
	//if(m_bMustDelete && m_psiImageIndex)
	//{
	//	delete m_psiImageIndex;
	//	m_psiImageIndex = NULL;
	//}		
	//if(m_ppImageFileName)
	//	delete [] m_ppImageFileName;
 //   // WM_TIMER 를 사용할때
	////SetTimerOnOff( false );

}

void CNBossMarketDetailDlg::Create()
{
	////if(!IsCreate())
	//{
	//	cltClient *pclclient = (cltClient *)pclClient;

	//	// Dialog 생성!
	//	CInterfaceFile file;
	//	file.LoadFile(TEXT("NInterface/Data/NBossMarketDlg/DLG_NBossMarketDetail.ddf"));
	//	file.CreatePopupDialog( this, NBOSSMARKETDETAIL_DLG, TEXT("dialog_bossmarketdetail"), StaticNBossMarketDetailDlgProc );

	//	m_pPortraitImage = new CImageStatic( this );
	//	m_pDetailImage = new CImageStatic( this );
	//	m_pExplainEdit = new CEdit( this );
	//	m_pPrevButton = new CButton( this );
	//	m_pNextButton = new CButton( this );
	//	m_pCloseButton = new CButton( this );

	//	// Dialog 고정 위치 지정!
	//	if(pclclient && pclclient->m_pDialog[ NSOUNDONOFF_DLG ])
	//	{
	//		if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
	//		{
	//			this->MovePos(DLG_BIG_POS_X, DLG_BIG_POS_Y);
	//		}
	//		else
	//		{
	//			this->MovePos(DLG_SMALL_POS_X, DLG_SMALL_POS_Y);
	//		}
	//	}
	//	
	//	// Control 생성!
	//	file.CreateControl(m_pPortraitImage,	NBOSSMARKETDETAIL_DLG_PORTRAITIMAGE,	TEXT("imagestatic_portrait"));
	//	file.CreateControl(m_pDetailImage,		NBOSSMARKETDETAIL_DLG_DETAILIMAGE,		TEXT("imagestatic_detail"));
	//	file.CreateControl(m_pExplainEdit,		NBOSSMARKETDETAIL_DLG_EXPLAINEDIT,     TEXT("editbox_explain"));
	//	// NBOSSMARKET_DLG_BUTTON1 ~ NBOSSMARKET_DLG_BUTTON5까지

	//	file.CreateControl(m_pPrevButton, NBOSSMARKETDETAIL_DLG_PREVBUTTON,	TEXT("button_prev"));
	//	file.CreateControl(m_pNextButton, NBOSSMARKETDETAIL_DLG_NEXTBUTTON,	TEXT("button_next"));
	//	file.CreateControl(m_pCloseButton, NBOSSMARKETDETAIL_DLG_CLOSE,	TEXT("button_close"));
	//	TCHAR* pTitle = GetTxtFromMgr(3707);
	//	SetTitle(pTitle);

	//	TCHAR* pText = GetTxtFromMgr(3708);
	//	m_pPrevButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//	pText = GetTxtFromMgr(3709);
	//	m_pNextButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//	pText = GetTxtFromMgr(3303);
	//	m_pCloseButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	//	CheckValidation();
	//	// WM_TIMER 를 사용할 때
	//	//SetTimerOnOff( true );
	//	// WM_TIMER를 사용할 때
	//	//SetActionTimer( ... );
	//	//SetTimerOnOff( true );

	//	Hide();
	//}
	////else
	////{
	////	DeleteDialog();
	////}

	//return;
}

void CALLBACK CNBossMarketDetailDlg::StaticNBossMarketDetailDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNBossMarketDetailDlg *pThis = (CNBossMarketDetailDlg*) pControllerMgr;
	pThis->NBossMarketDetailDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNBossMarketDetailDlg::NBossMarketDetailDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	//switch( nControlID )
	//{
	//case COMMON_DIALOG_CLOSEBUTTON:
	//	{
	//		Hide();
	//	}
	//	break;

	//case NBOSSMARKETDETAIL_DLG_CLOSE:
	//	{
	//		Hide();
	//	}
	//	break;
	//case NBOSSMARKETDETAIL_DLG_PREVBUTTON:
	//	{
	//		switch( nEvent )
	//		{
	//		case EVENT_BUTTON_CLICKED:
	//			{
	//				m_siCurSceneIndex--;

	//				if( CheckValidation() == TRUE )
	//					UpdateControls( m_siCurSceneIndex );
	//				else
	//					m_siCurSceneIndex++;
	//			}
	//			break;
	//		}
	//	}
	//	break;
	//case NBOSSMARKETDETAIL_DLG_NEXTBUTTON:
	//	{
	//		switch( nEvent )
	//		{
	//		case EVENT_BUTTON_CLICKED:
	//			{
 //                   m_siCurSceneIndex++;

	//				if( CheckValidation() == TRUE )
	//					UpdateControls( m_siCurSceneIndex );
	//				else
	//					m_siCurSceneIndex--;
	//			}
	//			break;
	//		}
	//	}
	//	break;
	//}
}

bool CNBossMarketDetailDlg::CheckValidation()
{
	//if( m_bFileNameMode == TRUE )
	//{
	//	if( m_ppImageFileName == NULL ) return FALSE;

	//	for(SI16 i= 0; i<m_siMaxScene; i++)
	//	{
	//		if(m_ppImageFileName[i] == NULL)
	//			return FALSE;
	//	}
	//}
	//else
	//{
	//	if( m_siCurSceneIndex <= 0 ) 
 //           m_pPrevButton->Enable( false );
	//	else
	//		m_pPrevButton->Enable( true );

	//	if( m_psiTextIndex == NULL || m_siMaxScene <= 0 ) return FALSE;

	//	if(m_siCurSceneIndex >= m_siMaxScene - 1)
	//	{
	//		m_pNextButton->Enable( false );

	//		SI16 questtype, questunique, para1, para2;
	//		SI16 questclearcond =  QUEST_CLEAR_COND_GONPC;
	//		if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
	//		{
	//			//if(para1 == pclClient->GetUniqueFromHash( TEXT("KIND_WANGCHO")) && para2 == 1 )
	//			//{
	//			//	cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
	//			//	cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
	//			//	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	//			//}
	//		}
	//	}
	//	else
	//		m_pNextButton->Enable( true );
	//
	//	if(m_siCurSceneIndex < 0 || m_siCurSceneIndex >= m_siMaxScene )
	//		return FALSE;

	//	if( m_psiTextIndex[ m_siCurSceneIndex ] < 0 ) return FALSE;

	//	for(SI16 i= 0; i<m_siMaxScene; i++)
	//	{
	//		if(m_psiImageIndex[ m_siCurSceneIndex ] < 0)
	//			return FALSE;
	//	}

	//	// FontIndex가 셋팅이 안되었다면.. 디폴트로 0~MaxScene까지 설정함
	//	if(m_psiImageIndex == NULL)
	//	{
	//		m_bMustDelete = true;
	//		m_psiImageIndex = new SI16[m_siMaxScene];
	//		for(SI16 i= 0; i<m_siMaxScene; i++)
	//		{
 //               m_psiImageIndex[ i ] = i;           
	//		}
	//	}
	//}

	return TRUE;
}

void CNBossMarketDetailDlg::UpdateControls( SI16 in_psiIndex )
{
/*	if( CheckValidation() == FALSE ) return;

	if(m_bFileNameMode)
	{
        m_pDetailImage->SetFileName( m_ppImageFileName[ in_psiIndex ] );
	}
	else
	{
		m_pDetailImage->SetFontIndex( m_psiImageIndex[ m_siCurSceneIndex ]);
	}

	TCHAR* pText = NULL;
	pText = GetTxtFromMgr( m_psiTextIndex[ m_siCurSceneIndex ] );

	if(pText == NULL ) return;

	m_pExplainEdit->SetText( pText );   */                    
}

void CNBossMarketDetailDlg::SetPortrait( TCHAR* in_pImageFullPath, SI16 in_siIndex )
{
	//if( in_pImageFullPath == NULL || in_siIndex < 0 ) return;

 //   m_pPortraitImage->SetFileName( in_pImageFullPath );
	//m_pPortraitImage->SetFontIndex( in_siIndex );	
}

void CNBossMarketDetailDlg::SetTextInfo( SI16 *in_pTextNum, SI16 in_siMaxIndex )
{
/*	if(in_pTextNum == NULL && in_siMaxIndex <= 0 ) return;

	m_psiTextIndex = new SI16[in_siMaxIndex];

	for(SI16 i=0; i<in_siMaxIndex; i++)
        m_psiTextIndex[i] = in_pTextNum[i];

	TCHAR* pText = NULL;
	pText = GetTxtFromMgr( m_psiTextIndex[ m_siCurSceneIndex ] );

	if(pText == NULL ) return;

	m_pExplainEdit->SetText( pText );   */                    
}

void CNBossMarketDetailDlg::SetMaxScene( SI16 in_siMaxScene )
{
	//if( in_siMaxScene < 0 ) return;

 //   m_siMaxScene = in_siMaxScene;
}

void CNBossMarketDetailDlg::SetCurControl(  SI16 in_siCurIndex )
{
	//if( in_siCurIndex < 0 || in_siCurIndex >= m_siMaxScene ) return;

 //   m_siCurSceneIndex = in_siCurIndex;
}

// 그림 파일 하나, fontindex만 변하는 방식 일때 사용
void CNBossMarketDetailDlg::SetImageInfo( TCHAR* in_pImageFullPath, SI16 in_siMaxIndex, SI16 *in_pImageFont )
{
	//if( in_pImageFullPath == NULL && in_siMaxIndex <= 0 ) return;

	//m_bFileNameMode = false;

	//m_pDetailImage->SetFileName( in_pImageFullPath );

	//m_psiImageIndex = new SI16[in_siMaxIndex];

	//for(SI16 i=0; i<in_siMaxIndex; i++)
	//{
	//	if(in_pImageFont != NULL )
	//		m_psiImageIndex[ i ] = in_pImageFont[ i ];
	//	else
	//		m_psiImageIndex[ i ] = i;
	//}

	//m_pDetailImage->SetFontIndex( m_siCurSceneIndex );
}

// 그림 파일 여러개, 파일이 직접 변하는 방식 일때 사용
void CNBossMarketDetailDlg::SetImageInfoWithFiles( TCHAR** in_ppImageFullPath, SI16 in_siMaxIndex )
{
 //   if( in_ppImageFullPath == NULL ) return;

	//for(SI16 i= 0; i<m_siMaxScene; i++)
	//{
	//	if(in_ppImageFullPath[i] == NULL)
	//		return;
	//}

	//m_bFileNameMode = true;

	//m_ppImageFileName = new TCHAR* [ in_siMaxIndex ];

	//for(SI16 i=0; i<in_siMaxIndex; i++)
	//{
	//	m_ppImageFileName[ i ] = new TCHAR[ MAX_FILE_FULLPATH ];

	//	StringCchCopy(m_ppImageFileName[i], MAX_FILE_FULLPATH, in_ppImageFullPath[i]);
	//}
}

bool CNBossMarketDetailDlg::IsShow()
{
	return m_bShow;
}

void CNBossMarketDetailDlg::Show()
{
	//if( m_bShow == true )
	//	m_bShow = false;
	//else
	//	m_bShow = true;

	//CDialogController::Show( m_bShow );
}

void CNBossMarketDetailDlg::Hide()
{
	//if( m_bShow == false )
	//	m_bShow = true;
	//else
	//	m_bShow = false;

	//CDialogController::Show( m_bShow );
}

//// WM_TIMER 메세지 처리 루틴
//void NBossMarketDlg::Action()
//{
//}


//#endif	//#if defined(_LEON_NBOSSMARKET_) && defined(_DEBUG)
